#ifndef __Example__FileLoader__
#define __Example__FileLoader__

#include <map>
#include <vector>

#include "ThreadPool.h"
#include "FileLoadConfig.h"
#include "FileLoadStatus.h"

namespace base
{
class FileLoader : public ThreadPool
{
public:
    void Shutdown();
    
    void Config(FileLoadConfig conf);
    const std::string& GetPath() const { return _local_path; }
    
    void LoadFile(const std::string& file, LFFinish cb, void* uptr);
    
    void Update();
    
private:
    void UpdateFinishMap();

    static void OnLoadFile(const std::string& path, LFStatus status);
    
private:
    struct LoadInfo {
        std::vector<LFFinish>           funcs;
        std::vector<void*>              user_ptrs;
        LFStatus                        status;
    };
    
    std::map<std::string, LoadInfo>   _load_map;
    std::map<std::string, LoadInfo>   _finish_map;
    
    Lock                              _map_lock;
    std::string                       _local_path;
};
}
#endif
