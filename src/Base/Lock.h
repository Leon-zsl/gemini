#ifndef __Gemini_Lock__
#define __Gemini_Lock__

#include <pthread.h>

namespace base
{
class Lock
{
public:
    Lock();
    Lock(bool recursive);
    ~Lock();

    void lock();
    void unlock();

private:
    void operator = (const Lock& lock);
    Lock(const Lock& lock);

private:
    pthread_mutex_t _mutex;
};
}
#endif
