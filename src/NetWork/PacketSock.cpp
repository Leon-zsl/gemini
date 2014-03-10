#include "PacketSock.h"
#include "PacketHandler.h"
#include "SockMgr.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

namespace base
{
PacketSock::PacketSock(SockMgr* mgr)
:_mgr(mgr),
_handler(NULL),
_sock(0),
_ip(""),
_port(0),
//_send_lock(NULL),
//_recv_lock(NULL),
_read_buf_len(0),
_write_buf_len(0),
_read_buf_pos(0),
_write_buf_pos(0),
_read_buf(NULL),
_write_buf(NULL),
_running(false),
_error(0)
{
    _read_buf = new char[512];
    _read_buf_len = 512;
    _read_buf_pos = 0;
    
    int ret = pthread_mutex_init(&_recv_lock, NULL);
    if(ret != 0) {
        printf("init read lock failed %d", ret);
        delete[] _read_buf;
        return;
    }
    
    _write_buf = new char[512];
    _write_buf_len = 512;
    _write_buf_pos = 0;
    
    ret = pthread_mutex_init(&_send_lock, NULL);
    if(ret != 0) {
        printf("init send lock failed: %d", ret);
        delete [] _read_buf;
        delete [] _write_buf;
        return;
    }
}

PacketSock::~PacketSock()
{
    Disconnect();
    
    if(_read_buf != NULL) {
        delete[] _read_buf;
        _read_buf = NULL;
        _read_buf_len = 0;
        _read_buf_pos = 0;
    }
    
    if(_write_buf != NULL) {
        delete[] _write_buf;
        _write_buf = NULL;
        _write_buf_len = 0;
        _write_buf_pos = 0;
    }

    pthread_mutex_destroy(&_recv_lock);
    pthread_mutex_destroy(&_send_lock);
    //the owner delete it
    //delete _handler;
}

void PacketSock::Connect(const std::string &ip, short port)
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd == -1) {
        printf("create socket failed\n");
        return;
    }
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    
    int ret = ::connect(fd, (struct sockaddr*) &addr, sizeof(addr));
    if(ret == -1) {
        if(errno != EINPROGRESS) {
            printf("connect failed, errcode: %d\n", errno);
            _error = errno;
            if(_handler != NULL)
                _handler->HandleError(this, errno);
            Disconnect();
            return;
        }
    }
    
    int nodelay = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 2;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
    
    int flag = fcntl(fd, F_GETFL);
    if(flag < 0) {
        printf("get flag failed");
        return;
    }
    
    ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    if(ret == -1) {
        printf("set non block failed");
        return;
    }
    
    _ip = ip;
    _port = port;
    _sock = fd;
    _running = true;
    _error = 0;
    
    _mgr->_AddSock(this);
}

void PacketSock::Disconnect()
{
    pthread_mutex_lock(&_recv_lock);
    pthread_mutex_lock(&_send_lock);

    _running = false;
    _error = 0;
    
    _mgr->_DelSock(this);
    
    while(!_recv_queue.empty()) _recv_queue.pop();
    while(!_send_queue.empty()) _send_queue.pop();
    
    if(_sock != 0) {
        ::close(_sock);
        _sock = 0;
    }
    
    _ip = "";
    _port = 0;
    
    pthread_mutex_unlock(&_recv_lock);
    pthread_mutex_unlock(&_send_lock);
}

void PacketSock::SendPacket(const Packet &pck)
{
    pthread_mutex_lock(&_send_lock);
    _send_queue.push(pck);
    pthread_mutex_unlock(&_send_lock);
}

void PacketSock::DispatchPacket()
{
    if(!_running)
        return;
    
    if(_error != 0) {
        if(_handler != NULL)
            _handler->HandleError(this, _error);
        Disconnect();
        return;
    }

    pthread_mutex_lock(&_recv_lock);
    while(!_recv_queue.empty()) {
        Packet pck = _recv_queue.front();
        _recv_queue.pop();
        if(_handler == NULL)
            continue;
        _handler->HandlePacket(this, pck);
    }
    pthread_mutex_unlock(&_recv_lock);
}

void PacketSock::_RecvData()
{
    pthread_mutex_lock(&_recv_lock);
    if(!_running) {
        pthread_mutex_unlock(&_recv_lock);
        return;
    }
    
    while(true) {
        int len = recv(_sock, _read_buf + _read_buf_pos, _read_buf_len - _read_buf_pos, NULL);
        if(len < 0) {
            //no data to read
            if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                break;
            else {
                printf("read error: %d", errno);
                _error = errno;
                break;
            }
        } else if(len == 0) {
            //sock closed
            _error = -1;
            break;
        } else if(len == _read_buf_len - _read_buf_pos) {
            //pthread_mutex_lock(&_recv_lock);
            if(_running) {
                //more data to read
                char* buf = new char[_read_buf_len + 512];
                memcpy(buf, _read_buf, _read_buf_len);
                delete[] _read_buf;
                _read_buf = buf;
                _read_buf_len += 512;
                _read_buf_pos += len;
            }
            //pthread_mutex_unlock(&_recv_lock);
        } else {
            //read all data
            _read_buf_pos += len;
            break;
        }
    }
    
    //pthread_mutex_lock(&_recv_lock);
    while(true) {
        //recv data
        if(_read_buf_pos < 4)
            break;
        
        int len = ntohl(*(int*)_read_buf);
        if(_read_buf_pos - 4 < len)
            break;
        
        int op = ntohl(*(int*)(_read_buf + 4));
        _recv_queue.push(Packet(op, _read_buf + 4 + 4, len - 4));
        
        memcpy(_read_buf, _read_buf + 4 + len, _read_buf_len - (4 + len));
        _read_buf_pos = _read_buf_pos - (4 + len);
    }
    pthread_mutex_unlock(&_recv_lock);
}

void PacketSock::_SendData()
{
    pthread_mutex_lock(&_send_lock);
    if(!_running) {
        pthread_mutex_unlock(&_send_lock);
        return;
    }
    
    while(!_send_queue.empty()) {
        Packet pck = _send_queue.front();
        _send_queue.pop();
        
        int len = 4 + pck.DataLen();
        
        if(_write_buf_pos + len + 4 > _write_buf_len) {
            int inc = len + 4 > 512 ? len + 4 : 512;
            char* buf = new char[_write_buf_len + inc];
            memcpy(buf, _write_buf, _write_buf_len);
            delete[] _write_buf;
            _write_buf = buf;
            _write_buf_len = _write_buf_len + inc;
        }
        
        int op = pck.Opcode();
        const char* data = pck.Data();
        
        int lenval = htonl(len);
        int opval = htonl(op);
        memcpy(_write_buf + _write_buf_pos, &lenval, 4);
        memcpy(_write_buf + _write_buf_pos + 4, &opval, 4);
        memcpy(_write_buf + _write_buf_pos + 4 + 4, data, pck.DataLen());
        _write_buf_pos = _write_buf_pos + 4 + 4 + pck.DataLen();
    }
    //pthread_mutex_unlock(&_send_lock);
    
    while(true) {
        int len = ::send(_sock, _write_buf, _write_buf_pos, 0);
        if(len < 0) {
            //intr singal
            if(errno == EINTR) {
                printf("send error %d", errno);
                _error = errno;
                break;;
            }
        
            //send bufer full, wait and retry
            if(errno == EAGAIN) {
                usleep(10 * 1000);//10 ms
                continue;
            }
            
            printf("send error %d", errno);
            _error = errno;
            break;
        } else if(len == _write_buf_pos) {
            //send ok
            _write_buf_pos = 0;
            break;
        } else {
            //pthread_mutex_lock(&_send_lock);
            if(_running) {
                //send part, resend
                memcpy(_write_buf, _write_buf + len, _write_buf_pos - len);
                _write_buf_pos = _write_buf_pos - len;
            }
            //pthread_mutex_unlock(&_send_lock);
        }
    }
    pthread_mutex_unlock(&_send_lock);
}
}