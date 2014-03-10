#ifndef Example_DownloadStatus_h
#define Example_DownloadStatus_h

namespace base
{
#define D2F_OK                          0
#define D2F_OPENFILE_FAILED            -1
#define D2F_DOWNLOAD_ERROR             -2


struct D2FStatus
{
    int                         status;
    int                         detail_error_code;
    std::string                 full_path;
    void*                       user_ptr;
};

typedef void(*D2FFinish)(const std::string& url, D2FStatus status);

#define D2M_OK                          0
#define D2M_MEMORY_FAILED              -1
#define D2M_DOWNLOAD_ERROR             -2

struct D2MStatus
{
    int                         status;
    int                         detail_error_code;
    //user response to free the memory
    char*                       data;
    int                         len;
    void*                       user_ptr;
};

typedef void (* D2MFinish)(const std::string& url, D2MStatus status);
}
#endif
