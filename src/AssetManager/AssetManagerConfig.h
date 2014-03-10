#ifndef Gemini_AssetManagerConfig_h
#define Gemini_AssetManagerConfig_h

#include <string>

#include "DownloadConfig.h"
#include "FileLoadConfig.h"

namespace base
{
struct AssetManagerConfig
{
    std::string     remote_addr;
    std::string     local_addr;
    std::string     settings_file;
    int             task_parallel;
    
    AssetManagerConfig()
    :task_parallel(8)
    {}
};
}
#endif
