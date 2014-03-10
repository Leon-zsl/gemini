#ifndef __Example__DownloadManager__
#define __Example__DownloadManager__

#include <string>
#include <map>
#include <vector>

#include "ThreadPool.h"
#include "DownloadStatus.h"
#include "DownloadConfig.h"

namespace base
{
class DownloadManager : public ThreadPool
{
public:
    DownloadManager();
    //~DownloadManager();
    
    void Shutdown();
    
    void Config(DownloadConfig conf);
    const std::string& GetServerAddr() const { return _remote_addr; }
    
    void DownloadToFile(const std::string& file, D2FFinish cb, void* uptr);
    void DownloadToMemory(const std::string& file, D2MFinish cb, void* uptr);
    
    void Update();
    
private:
    struct DownloadInfo
    {
        //int                               type;//0 means file while 1 means memory
        //std::string                       url;
        //std::string                       file;
        std::vector<D2FFinish>      d2f;
        D2FStatus                   d2f_st;
        std::vector<D2MFinish>      d2m;
        D2MStatus                   d2m_st;
        std::vector<void*>          user_ptrs;
    };
    
    typedef std::map<std::string, DownloadInfo> CallbackMap;
    
private:
    void UpdateWaitMap();
    void UpdateDownloadMap();
    void UpdateFinishMap();
    
    void CreateNewTask(const std::string& file, int type, DownloadInfo& info);
    
    static void OnLoadToFile(const std::string& url, D2FStatus status);
    static void OnLoadToMemory(const std::string& url, D2MStatus status);
    
    static std::string GetKey(const std::string& file, int type);
    static std::string GetFile(const std::string& key);
    static int         GetType(const std::string& key);
    
private:

    CallbackMap _wait_map;
    CallbackMap _download_map;
    CallbackMap _finish_map;
    Lock        _map_lock;
    
    std::string _remote_addr;
    std::string _local_addr;
    int         _max_task;
};
}
#endif
