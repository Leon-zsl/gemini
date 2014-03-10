#ifndef __Example_Runner__
#define __Example_Runner__

namespace base
{
class Runner
{
public:
    virtual ~Runner() {}

    virtual void Run() = 0;
    virtual void OnShutdown() {}
};
}
#endif
