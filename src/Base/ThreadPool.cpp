#include "ThreadPool.h"
#include <stdio.h>
#include <unistd.h>

namespace base
{
struct ThreadParam
{
    Thread* _thread;
    ThreadPool* _pool;
};

void* ThreadPool::thread_main(void* param)
{
    ThreadPool* pool = ((ThreadParam*)param)->_pool;
    Thread* th = ((ThreadParam*)param)->_thread;
    //th->_GetLock().lock();
    //wait main thread to prepare ok
    //Runner* runner = th->_GetRunner();
    //th->_GetLock().unlock();

//    if(th == NULL || pool == NULL) {
//        printf("thread main quit unnormally!");
//        delete (ThreadParam*)param;
//        pthread_exit(0);
//    }

    while(true) {
        Runner* r = th->_GetRunner();
        if(r != NULL) {
            r->Run();
            th->_SetRunner(NULL);
            delete(r);
        }

        bool q = th->_NeedQuit();
        pool->_ThreadFinish(th, q);

        if(q) break;

        th->Suspend();
    }

    //quit safely now
    delete (ThreadParam*)param;
    pthread_exit(0);
}

ThreadPool::ThreadPool()
{
    _lock = new Lock(true);
}

ThreadPool::~ThreadPool()
{
    Shutdown();
    delete _lock;
}

void ThreadPool::Startup()
{
    _lock->lock();
    for(int i = 0; i < 8; ++i) {
        _StartThread(NULL);
    }
    _lock->unlock();
}

void ThreadPool::Shutdown()
{
    _lock->lock();
    _KillFreeThreads();
    _CloseActiveThreads();
    _lock->unlock();
    _WaitEnding();
}

void ThreadPool::_KillFreeThreads()
{
    for(std::set<Thread*>::iterator itr = _free_threads.begin(); itr != _free_threads.end(); ++itr) {
        Thread* th = *itr;
        //th->_SetRunner(NULL);
        th->_Quit();
        th->Resume();
    }
    //do not clear
    //_free_threads.clear();
}

void ThreadPool::_CloseActiveThreads()
{
    for(std::set<Thread*>::iterator itr = _active_threads.begin(); itr != _active_threads.end(); ++itr) {
        Thread* th = *itr;
        Runner* r = th->_GetRunner();
        if(r != NULL) r->OnShutdown();
        th->_Quit();
        th->Resume();
    }
    //do not clear
    //_active_threads.clear();
}

void ThreadPool::_WaitEnding()
{
    usleep(1000 * 30);

    while(true) {
        _lock->lock();
        if(!_active_threads.empty() || !_free_threads.empty()) {
            for(std::set<Thread*>::iterator itr = _free_threads.begin(); itr != _free_threads.end(); ++itr) {
                (*itr)->Resume();
            }

            _lock->unlock();
            usleep(1000 * 30);
            continue;
        }
        
        _lock->unlock();
        break;
    }
}

void ThreadPool::Schedule(Runner* runner)
{
    if(runner == NULL) return;

    Thread* th = NULL;
    _lock->lock();
    if(_free_threads.empty()) {
        th = _StartThread(runner);
    } else {
        std::set<Thread*>::iterator itr = _free_threads.begin();
        _free_threads.erase(itr);

        th = *itr;
        th->_SetRunner(runner);
        th->Resume();
    }
    _active_threads.insert(th);
    _lock->unlock();
}

Thread* ThreadPool::_StartThread(Runner* runner)
{
    if(runner == NULL) return NULL;

    Thread* th = new Thread();
    th->_SetRunner(runner);

    ThreadParam* p = new ThreadParam();
    p->_thread = th;
    p->_pool = this;

    pthread_t h;

    //th->_GetLock().lock();
    pthread_create(&h, NULL, thread_main, p);
    pthread_detach(h);
    //th->_SetHandle(h);
    //detach it or not?
    //th->_GetLock().unlock();

    return th;
}

void ThreadPool::_ThreadFinish(Thread* th, bool quit)
{
    if(th == NULL) return;

    _lock->lock();
    _active_threads.erase(th);
    if(quit) {
        _free_threads.erase(th);
        delete th;
    } else {
        _free_threads.insert(th);
    }
    _lock->unlock();
}
}