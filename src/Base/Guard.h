#ifndef __Gemini_Guard__
#define __Gemini_Guard__

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
