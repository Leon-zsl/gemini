#ifndef __Gemini__DownloadToMemoryTask__
#define __Gemini__DownloadToMemoryTask__

#include <string>

#include "curl.h"

#include "DownloadTask.h"
#include "DownloadStatus.h"

namespace base
{
class DownloadToMemoryTask : public DownloadTask
{
public:
    DownloadToMemoryTask(const std::string& url, D2MFinish cb, void* uptr);
    ~DownloadToMemoryTask();
    
    void Run();
    void OnShutdown();
    
private:
    static size_t OnRecv(void* buffer, size_t size, size_t count, void* userdata);
    
    CURLcode GetLength(CURL* curl, int* len);
    CURLcode DoDownload(CURL* curl, size_t offset, void* userdata);
    
private:
    std::string         _url;
    D2MFinish           _func;
    CURL*               _curl;
    bool volatile       _quit;
    int                 _retry_count;
    void*               _user_ptr;
};
}
#endif
