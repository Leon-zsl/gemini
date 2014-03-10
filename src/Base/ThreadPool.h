#ifndef __Gemini_ThreadPool__
#define __Gemini_ThreadPool__

#include <set>

#include "MemoryManager.h"
#include "GlobalAllocator.h"
#include "SimpleAllocator.h"
#include "UniformAllocator.h"

#include "Thread.h"
#include "Runner.h"

namespace base
{
class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();
    
    virtual void Startup();
    virtual void Shutdown();

    void Schedule(Runner* runner);

private:
    friend class Thread;

    Thread* _StartThread(Runner* runner);
    void _ThreadFinish(Thread* th, bool quit);

    void _KillFreeThreads();
    void _CloseActiveThreads();
    void _WaitEnding();

    Thread* CreateThread();
    
    static void* thread_main(void* param);

private:
    //int _pool_size;
    Lock* _lock;
    std::set<Thread*> _active_threads;
    std::set<Thread*> _free_threads;
};
}
#endif
