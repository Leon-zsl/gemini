#include "Thread.h"

#include <assert.h>

namespace base
{
Thread::Thread()
    ://_handle(0),
     _runner(NULL),
    _quit(false)
{
//sem_init(&_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

Thread::~Thread()
{
    //sem_destroy(&_sem);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void Thread::Resume()
{
//    assert(!pthread_equal(pthread_self(), _handle));
//    sem_post(&_sem);
    pthread_cond_wait(&cond, &mutex);
}

void Thread::Suspend()
{
//    assert(pthread_equal(pthread_self(), _handle));
//    sem_wait(&_sem);
    pthread_cond_signal(&cond);
}

// Thread::Join()
// {
//     pthread_join(_handle, NULL);
// }
}