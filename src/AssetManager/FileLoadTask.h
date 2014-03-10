#ifndef __Example__FileLoadTask__
#define __Example__FileLoadTask__

#include <string>

#include "Runner.h"
#include "FileLoadStatus.h"

namespace base
{
class FileLoadTask : public Runner
{
public:
    FileLoadTask(const std::string& file, LFFinish cb, void* uptr);
    
    void Run();
    //shutdown will be ignored
    //void OnShutdown();
    
private:
    std::string         _file;
    LFFinish            _func;
    void*               _user_ptr;
};
}
#endif
