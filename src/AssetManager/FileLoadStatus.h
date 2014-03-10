#ifndef Example_FileLoadStatus_h
#define Example_FileLoadStatus_h

namespace base
{
#define LF_OK                           0
#define LF_OPENFILE_FAILED              -1
#define LF_MEMORY_ERROR                 -2
#define LF_LOAD_FAILED                  -3

struct LFStatus
{
    int                         status;
    char*                       data;
    int                         len;
    void*                       user_ptr;
};

typedef void(*LFFinish)(const std::string& url, LFStatus status);
}
#endif
