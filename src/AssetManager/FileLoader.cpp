#include "FileLoader.h"
#include "FileLoadTask.h"
#include "PathUtil.h"
#include "Guard.h"

namespace base
{
void FileLoader::Shutdown()
{
    _map_lock.lock();
    UpdateFinishMap();
    _load_map.clear();
    _finish_map.clear();
    _map_lock.unlock();
    
    ThreadPool::Shutdown();
}

void FileLoader::Config(FileLoadConfig conf)
{
    _local_path = conf.file_path;
}

void FileLoader::Update()
{
    UpdateFinishMap();
}

void FileLoader::LoadFile(const std::string &file, LFFinish cb, void* uptr)
{
    _map_lock.lock();
    
    std::map<std::string, LoadInfo>::iterator itr = _finish_map.find(file);
    if(itr != _finish_map.end()) {
        itr->second.funcs.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
        _map_lock.unlock();
        return;
    }
    
    itr = _load_map.find(file);
    if(itr != _load_map.end()) {
        itr->second.funcs.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    } else {
        LoadInfo info;
        info.funcs.push_back(cb);
        info.user_ptrs.push_back(uptr);
        _load_map[file] = info;
    }
    _map_lock.unlock();

    std::string path = PathUtil::Join(_local_path, file);
    FileLoadTask* task = new FileLoadTask(path, OnLoadFile, this);
    Schedule(task);
}

void FileLoader::UpdateFinishMap()
{
    Guard guard(&_map_lock);
    
    if(_finish_map.size() <= 0) return;
    
    for(std::map<std::string, LoadInfo>::iterator itr = _finish_map.begin(); itr != _finish_map.end(); ++itr) {
        for(int i = 0; i < itr->second.funcs.size(); ++i) {
            LFFinish func = itr->second.funcs[i];
            LFStatus st = itr->second.status;
            st.user_ptr = itr->second.user_ptrs[i];
            func(itr->first, st);
        }
    }
    _finish_map.clear();
}

void FileLoader::OnLoadFile(const std::string &path, LFStatus status)
{
    FileLoader* loader = (FileLoader*)status.user_ptr;
    Guard guard(&loader->_map_lock);
    
    std::string file = PathUtil::GetFile(path);
    std::map<std::string, LoadInfo>::iterator itr = loader->_load_map.find(file);
    if(itr != loader->_load_map.end()) {
        itr->second.status = status;
        loader->_finish_map[itr->first] = itr->second;
        loader->_load_map.erase(itr);
    } else {
        //no user care of the data, release
        delete[] status.data;
    }
}
}