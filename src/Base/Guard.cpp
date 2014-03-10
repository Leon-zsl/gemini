#include "Lock.h"
#include "Guard.h"

namespace base
{
Guard::Guard(Lock* lock)
{
    _lock = lock;
    if(_lock != NULL)
        _lock->lock();
}

Guard::~Guard()
{
    if(_lock != NULL)
        _lock->unlock();
    _lock = NULL;
}
}
