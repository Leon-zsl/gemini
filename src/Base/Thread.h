#ifndef __Gemini_Thread__
#define __Gemini_Thread__

#include <pthread.h>
//#include <semaphore.h>

#include "Runner.h"
#include "Lock.h"

namespace base
{
class Thread
{
public:
    Thread();
    virtual ~Thread();

    void Resume();
    void Suspend();
//    void Join();

private:
    friend class ThreadPool;

    void _SetRunner(Runner* r) { _runner = r; }
    Runner* _GetRunner(void) const { return _runner; }

//    void _SetHandle(pthread_t handle) { _handle = handle; }
//    pthread_t _GetHandle(void) { return _handle; }

    //Lock& _GetLock(void) { return _lock; }

    bool _NeedQuit(void) const { return _quit; }
    void _Quit() { _quit = true; }

private:
    //Lock _lock;
    
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    //pthread_t _handle;
    
    volatile bool _quit;
    Runner* _runner;
};
}
#endif
