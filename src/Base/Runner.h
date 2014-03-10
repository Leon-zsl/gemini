#ifndef __Gemini_Runner__
#define __Gemini_Runner__

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
