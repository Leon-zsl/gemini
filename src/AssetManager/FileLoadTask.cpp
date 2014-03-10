#include "FileLoadTask.h"

#include <stdio.h>

namespace base
{
FileLoadTask::FileLoadTask(const std::string& file, LFFinish cb, void* uptr)
:_file(file),
_func(cb),
_user_ptr(uptr)
{}

void FileLoadTask::Run()
{
    LFStatus status;
    status.status = LF_OK;
    status.data = NULL;
    status.len = 0;
    status.user_ptr = _user_ptr;
    
    FILE* file = fopen(_file.c_str(), "rb");
    if(file == NULL) {
        status.status = LF_OPENFILE_FAILED;
        if(_func != NULL) {
            _func(_file, status);
        } else {
            printf("open file failed: %s", _file.c_str());
        }
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* data = new char[len];
    if(data == NULL) {
        status.status = LF_MEMORY_ERROR;
        if(_func != NULL) {
            _func(_file, status);
        } else {
            printf("load file failed: %s", _file.c_str());
        }
        
        fclose(file);
        return;
    }
    
    long size = fread(data, 1, len, file);
    if(size != len) {
        status.status = LF_LOAD_FAILED;
        if(_func != NULL) {
            _func(_file, status);
        } else {
            printf("load file failed: %s", _file.c_str());
        }
        
        delete[] data;
        fclose(file);
        return;
    }
    
    fclose(file);
    
    status.data = data;
    status.len = len;
    if(_func != NULL) {
        _func(_file, status);
    } else {
        delete[] data;
        printf("load file success: %s", _file.c_str());
    }
}
}