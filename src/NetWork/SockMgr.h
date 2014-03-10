#ifndef __Gemini__SockMgr__
#define __Gemini__SockMgr__

#include <map>
#include <pthread.h>
//#include <sys/socket.h>
#include "Singleton.h"

namespace base
{
class PacketSock;
class SockMgr : public Singleton<SockMgr>
{
public:
    SockMgr();
    ~SockMgr();
    
    void Start();
    void Shutdown();
    void Update();
    
    PacketSock* CreateSock();
    void DestroySock(PacketSock* sock);
    
    void _AddSock(PacketSock* sock);
    void _DelSock(PacketSock* sock);
    
private:
    static void* select_loop(void* sock);
    
private:
    typedef std::map<int, PacketSock*> SockMap;
    
    volatile bool _running;
    
    volatile int _maxfd;
    
    pthread_mutex_t _lock;
    SockMap _sock_map;
    
    pthread_t _select_thread;
};
}
#endif
