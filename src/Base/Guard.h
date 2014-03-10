#ifndef __Example_Guard__
#define __Example_Guard__

namespace base
{
class Lock;
class Guard
{
public:
    Guard(Lock* lock);
    ~Guard();

private:
    Lock* _lock;
};
}
#endif
