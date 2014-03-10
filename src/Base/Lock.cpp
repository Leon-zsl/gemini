#include "Lock.h"

namespace base
{
Lock::Lock()
{
    Lock(false);
}

Lock::Lock(bool recursive)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_mutex, &attr);
}

Lock::~Lock()
{
    pthread_mutex_destroy(&_mutex);
}

void Lock::lock()
{
    pthread_mutex_lock(&_mutex);
}

void Lock::unlock()
{
    pthread_mutex_unlock(&_mutex);
}
}
