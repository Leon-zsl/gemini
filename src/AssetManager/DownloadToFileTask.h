#ifndef __Example_DownloadToFileTask__
#define __Example_DownloadToFileTask__

#include <string>
#include <stdio.h>

#include "curl.h"

#include "DownloadStatus.h"
#include "DownloadTask.h"

namespace base
{
class DownloadToFileTask : public DownloadTask
{
public:
    DownloadToFileTask(const std::string& url,
                       const std::string& file,
                       D2FFinish cb,
                       void* uptr);

    ~DownloadToFileTask();

    void Run();
    void OnShutdown();

private:
    static size_t OnRecv(void* buffer, size_t size, size_t count, void* userdata);
    
    CURLcode DoDownload(CURL* curl, size_t offset, void* userdata);
    
private:
    std::string         _url;
    std::string         _file;
    D2FFinish           _func;
    CURL*               _curl;
    bool volatile       _quit;
    int                 _retry_count;
    void*               _user_ptr;
};
}
#endif
