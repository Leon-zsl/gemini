#include "SockMgr.h"
#include "PacketSock.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

namespace base
{
SockMgr::SockMgr()
:_running(false)
{}

SockMgr::~SockMgr()
{
    Shutdown();
}

void SockMgr::Start()
{
    _running = true;
    _maxfd = 0;
    
    pthread_mutex_init(&_lock, NULL);
    
    int ret = pthread_create(&_select_thread, NULL, SockMgr::select_loop, this);
    if(ret == -1) {
        printf("create select thread failed");
        return;
    }
}

void SockMgr::Shutdown()
{
    _running = false;
    _maxfd = 0;
    
    int ret = pthread_join(_select_thread, NULL);
    if(ret == -1) {
        printf("join thead failed");
    }
    
    pthread_mutex_lock(&_lock);
    for(SockMap::iterator itr = _sock_map.begin(); itr != _sock_map.end(); ++itr) {
        delete itr->second;
    }
    _sock_map.clear();
    pthread_mutex_unlock(&_lock);
    
    pthread_mutex_destroy(&_lock);
}

void SockMgr::Update()
{
    pthread_mutex_lock(&_lock);
    for(SockMap::iterator itr = _sock_map.begin(); itr != _sock_map.end(); ++itr) {
        itr->second->DispatchPacket();
    }
    pthread_mutex_unlock(&_lock);
        
}

PacketSock* SockMgr::CreateSock()
{
    PacketSock* sock = new PacketSock(this);
    return sock;
}

void SockMgr::DestroySock(PacketSock *sock)
{
    if(sock == NULL) return;    
    delete sock;
}

void SockMgr::_AddSock(PacketSock *sock)
{
    if(sock == NULL) return;
    
    pthread_mutex_lock(&_lock);
    _sock_map[sock->_Fd()] = sock;
    if(_maxfd < sock->_Fd())
        _maxfd = sock->_Fd();
    pthread_mutex_unlock(&_lock);
}

void SockMgr::_DelSock(PacketSock *sock)
{
    if(sock == NULL) return;
    
    pthread_mutex_lock(&_lock);
    if(_sock_map.find(sock->_Fd()) != _sock_map.end()) {
        _sock_map.erase(sock->_Fd());
        if(_maxfd == sock->_Fd()) {
            _maxfd = 0;
            for(SockMap::iterator itr = _sock_map.begin(); itr != _sock_map.end(); ++itr) {
                if(itr->first > _maxfd)
                    _maxfd = itr->first;
            }
        }
    }
    pthread_mutex_unlock(&_lock);
}

void* SockMgr::select_loop(void* mgr)
{    
    SockMgr* owner = (SockMgr*)mgr;
    while(owner->_running) {
        fd_set read_set;
        fd_set write_set;
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        
        int fds[32];
        int fdcnt = 0;
        if(!owner->_running)
            break;
        
        pthread_mutex_lock(&owner->_lock);
        for(SockMap::iterator itr = owner->_sock_map.begin(); itr != owner->_sock_map.end(); ++itr) {
            FD_SET(itr->first, &read_set);
            FD_SET(itr->first, &write_set);
            fds[fdcnt] = itr->first;
            fdcnt++;
        }
        pthread_mutex_unlock(&owner->_lock);
        
        if(fdcnt == 0) {
            usleep(10 * 1000);
            continue;
        }
        
        if(!owner->_running)
            break;
        
        struct timeval val;
        val.tv_sec = 0;
        val.tv_usec = 1000 * 10;//60 ms
        int ret = ::select(owner->_maxfd + 1, &read_set, &write_set, NULL, &val);
        
        if(!owner->_running)
            break;
        
        switch (ret) {
            case -1:
                printf("select error");
                break;
            case 0:
                //no sock event, time out
                break;
            default:
                for(int i = 0; i < fdcnt; ++i) {
                    pthread_mutex_lock(&owner->_lock);
                    if(owner->_sock_map.find(fds[i]) == owner->_sock_map.end()) {
                        pthread_mutex_unlock(&owner->_lock);
                        continue;
                    }
                    
                    int fd = fds[i];
                    PacketSock* sock = owner->_sock_map[fd];
                    pthread_mutex_unlock(&owner->_lock);
                    
                    int error = 0;
                    int len = sizeof(error);
                    if(getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)&error, (socklen_t*)&len) < 0) {
                       sock->_RecvError(error);
                        continue;
                    }

                    if(FD_ISSET(fd, &read_set))
                        sock->_RecvData();
                    if(FD_ISSET(fd, &write_set))
                        sock->_SendData();
                }
                break;
        }
    }
    
    return 0;
}
}
