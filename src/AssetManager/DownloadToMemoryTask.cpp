#include "DownloadToMemoryTask.h"

#include <string.h>

namespace base
{
struct UserData
{
    DownloadToMemoryTask* task;
    char*                 data;
    int                   len;
    int                   finish_size;
};

DownloadToMemoryTask::DownloadToMemoryTask(const std::string& url, D2MFinish cb, void* uptr)
    :_url(url),
    _func(cb),
    _user_ptr(uptr),
    _quit(false),
    _retry_count(0)
{
    //TODO: curl can not cross thread, so init and clean it in run function
}

DownloadToMemoryTask::~DownloadToMemoryTask()
{
    //TODO: curl can not cross thread, so init and clean it in run function
}

void DownloadToMemoryTask::Run()
{
    D2MStatus status;
    status.status = D2M_OK;
    status.detail_error_code = 0;
    status.data = NULL;
    status.len = 0;
    status.user_ptr = _user_ptr;
    
    _curl = curl_easy_init();
    
//    int len = 0;
//    char* buffer = NULL;
//    
//    CURLcode ret = GetLength(_curl, &len);
    

    UserData dt;
    dt.task = this;
    dt.len = 1024;
    dt.data = new char[1024];
    dt.finish_size = 0;
        
    int offset = 0;
    CURLcode ret = CURLE_OK;
    
    while(_retry_count++ < 3) {
        ret = DoDownload(_curl, offset, &dt);
        if(ret != CURLE_OK && !_quit) {
            //try again
            offset = dt.finish_size;
        } else {
            break;
        }
    }
    
    status.len = dt.finish_size;
    status.data = dt.data;
    
    if(ret != CURLE_OK) {
        status.status = D2M_DOWNLOAD_ERROR;
        status.detail_error_code = ret;
        status.len = 0;
        status.data = NULL;

        delete[] dt.data;
    }
    
    if(_func != NULL) {
        _func(_url, status);
    } else {
        delete[] dt.data;
        printf("download res to memory status: %s, %d", _url.c_str(), status.status);
    }

    curl_easy_cleanup(_curl);
}

void DownloadToMemoryTask::OnShutdown()
{
    _quit = true;
}

CURLcode DownloadToMemoryTask::DoDownload(CURL* curl, size_t offset, void* userdata)
{
    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 256);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    //curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnRecv);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, offset);
    
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
    
    return curl_easy_perform(curl);
}

//CURLcode DownloadToMemoryTask::GetLength(CURL* curl, int *len)
//{
//    curl_easy_reset(curl);
//    curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
////    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
////    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
////    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 256);
////    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30);
////    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
////    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
////    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
//    //curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
//    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
//    
//    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
//    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
//    
//    int ln = 0;
//    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &ln);
//    
//    CURLcode ret = curl_easy_perform(curl);
//    if(ret == CURLE_OK) {
//        size_t l = 0;
//        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &l);
//        *len = l;
//    } else {
//        *len = -1;
//    }
//    return ret;
//}
    
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
    
size_t DownloadToMemoryTask::OnRecv(void *buffer, size_t size, size_t count, void *userdata)
{
    UserData* ud = (UserData*)userdata;
    DownloadToMemoryTask* task = ud->task;
    if(task->_quit) {
        return 0;//stop the download
    }
    
    int len = size * count;
    if(len < ud->len - ud->finish_size) {
        memcpy(ud->data + ud->finish_size, buffer, len);
    } else {
        int alc = max(1024, len);
        int totlen = ud->finish_size + alc;
        char* data = new char[totlen];
        memcpy(data, ud->data, ud->finish_size);
        memcpy(data + ud->finish_size, buffer, len);
        delete [] ud->data;
        ud->data = data;
        ud->len = totlen;
    }

    ud->finish_size += len;
    return len;
}
}