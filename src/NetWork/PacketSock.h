#ifndef __Example__PacketSock__
#define __Example__PacketSock__

#include <queue>
#include <string>
#include <pthread.h>

#include "Packet.h"

namespace base
{
    
class PacketHandler;
class SockMgr;

class PacketSock
{
    friend class SockMgr;
public:
    void SetHandler(PacketHandler* h) { _handler = h; }
    PacketHandler* GetHandler() { return _handler; }
    
    std::string Ip() const { return _ip; }
    short Port() const { return _port; }
    
    void Connect(const std::string& ip, short port);
    void Disconnect();
 
    void SendPacket(const Packet& pck);
    void DispatchPacket();
    
private:
    PacketSock(SockMgr* mgr);
    ~PacketSock();
    
    PacketSock(const PacketSock& sock);
    void operator=(const PacketSock& sock);
    
    void _RecvData();
    void _SendData();
    void _RecvError(int error) { _error = error; }
    int _Fd() const { return _sock; }

private:
    typedef std::queue<Packet> PckQueue;

    SockMgr* _mgr;
    PacketHandler* _handler;
    
    std::string _ip;
    short _port;
    int _sock;
    
    volatile bool _running;
    volatile int _error;
    
    PckQueue _send_queue;
    pthread_mutex_t _send_lock;

    PckQueue _recv_queue;
    pthread_mutex_t _recv_lock;

    int _read_buf_len;
    int _read_buf_pos;
    char* _read_buf;
    
    int _write_buf_len;
    int _write_buf_pos;
    char* _write_buf;
};
}
#endif
