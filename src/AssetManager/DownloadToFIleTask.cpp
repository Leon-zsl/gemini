#include "DownloadToFileTask.h"

#include "DownloadStatus.h"

namespace base
{
struct UserData
{
    DownloadToFileTask* task;
    FILE*               file;
};

DownloadToFileTask::DownloadToFileTask(const std::string& url,
                                       const std::string& file,
                                       D2FFinish cb,
                                       void* uptr)
    :_url(url),
    _file(file),
    _func(cb),
    _user_ptr(uptr),
    _curl(NULL),
    _quit(false),
    _retry_count(0)
{
    //TODO: curl can not cross thread, so init and clean it in run function
}

DownloadToFileTask::~DownloadToFileTask()
{
    //TODO: curl can not cross thread, so init and clean it in run function
}

void DownloadToFileTask::Run()
{
    D2FStatus status;
    status.status = D2F_OK;
    status.detail_error_code = 0;
    status.full_path = _file;
    status.user_ptr = _user_ptr;
    
    FILE* file = fopen((_file).c_str(), "wb");
    if(file == NULL) {
        if(_func != NULL) {
            status.status = D2F_OPENFILE_FAILED;
            status.full_path = "";
            _func(_url, status);
        } else {
            printf("open file failed: %s", _file.c_str());
        }
        return;
    }
 
    UserData* dt = new UserData();
    dt->task = this;
    dt->file = file;
    
    _curl = curl_easy_init();
    
    CURLcode ret = CURLE_OK;
    size_t offset = 0;
    while(_retry_count++ < 3) {
        ret = DoDownload(_curl, offset, dt);
        if(ret != CURLE_OK && !_quit) {
            //try again
            offset = ftell(file);
            fflush(file);
        } else {
            break;
        }
    }

    curl_easy_cleanup(_curl);
    fflush(file);
    fclose(file);
    delete dt;
    
    if(ret != CURLE_OK) {
        status.status = D2F_DOWNLOAD_ERROR;
        status.detail_error_code = ret;
        status.full_path = "";
    }

    if(_func != NULL) {
        _func(_url, status);
    } else {
        printf("download res to file status: %s, %d", _url.c_str(), status.status);
    }
}

void DownloadToFileTask::OnShutdown()
{
    _quit = true;
}

CURLcode DownloadToFileTask::DoDownload(CURL* curl, size_t offset, void* userdata)
{
    //curl_easy_reset(_curl);
    curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 256);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    //curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
    //curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1);
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnRecv);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, offset);
    
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    
    return curl_easy_perform(_curl);
}

size_t DownloadToFileTask::OnRecv(void *buffer, size_t size, size_t count, void *userdata)
{
    DownloadToFileTask* task = ((UserData*)userdata)->task;
    FILE* file = ((UserData*)userdata)->file;
    if(task->_quit) {
        return 0;//stop the download
    }
    
    return fwrite(buffer, size, count, file);
}
}