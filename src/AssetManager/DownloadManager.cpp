#include "DownloadManager.h"
#include "DownloadToFileTask.h"
#include "DownloadToMemoryTask.h"
#include "Guard.h"
#include "PathUtil.h"

namespace base
{
DownloadManager::DownloadManager()
:_local_addr(""),
_remote_addr(""),
_max_task(8)
{}

//DownloadManager::~DownloadManager()
//{
//    //do not override the base destractor
//}

void DownloadManager::Shutdown()
{
    UpdateFinishMap();
 
    _map_lock.lock();
    _wait_map.clear();
    _download_map.clear();
    _finish_map.clear();
    _map_lock.unlock();
    
    ThreadPool::Shutdown();
}

void DownloadManager::Config(DownloadConfig conf)
{
    _local_addr = conf.localDir;
    _remote_addr = conf.serverAddr;
    _max_task = conf.maxDownloadTask;
}

void DownloadManager::Update()
{
    UpdateFinishMap();
    UpdateDownloadMap();
    UpdateWaitMap();
}

void DownloadManager::DownloadToFile(const std::string &file, D2FFinish cb, void* uptr)
{
    Guard guard(&_map_lock);
    //_map_lock.lock();
    
    std::string key = GetKey(file, 0);
    
    CallbackMap::iterator itr =  _finish_map.find(key);
    if(itr != _finish_map.end()) {
        itr->second.d2f.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }
    itr = _download_map.find(key);
    if(itr != _download_map.end()) {
        itr->second.d2f.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }
    itr = _wait_map.find(key);
    if(itr != _wait_map.end()) {
        itr->second.d2f.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }

    DownloadInfo info;
    //info.type = 0;
    info.d2f.push_back(cb);
    info.user_ptrs.push_back(uptr);
    _wait_map[key] = info;

    //_map_lock.unlock();
}

void DownloadManager::DownloadToMemory(const std::string &file, D2MFinish cb, void* uptr)
{
    Guard guard(&_map_lock);
    //_map_lock.lock();
    
    std::string key = GetKey(file, 1);
    
    CallbackMap::iterator itr =  _finish_map.find(key);
    if(itr != _finish_map.end()) {
        itr->second.d2m.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }
    itr = _download_map.find(key);
    if(itr != _download_map.end()) {
        itr->second.d2m.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }
    itr = _wait_map.find(key);
    if(itr != _wait_map.end()) {
        itr->second.d2m.push_back(cb);
        itr->second.user_ptrs.push_back(uptr);
    }

    DownloadInfo info;
    //info.type = 1;
    info.d2m.push_back(cb);
    info.user_ptrs.push_back(uptr);
    _wait_map[key] = info;

    //_map_lock.unlock();
}

void DownloadManager::UpdateWaitMap()
{
    Guard guard(&_map_lock);
    //_map_lock.lock();
    
//    if(_wait_map.empty()) return;
//    if(_download_map.size() >= _max_task) return;
    
    int new_task = _max_task - _download_map.size();
    for(int i = 0; i < new_task; ++i) {
        if(_wait_map.empty()) break;
        if(_download_map.size() >= _max_task) break;
        
        CallbackMap::iterator itrw = _wait_map.begin();
        DownloadInfo info = itrw->second;
        CreateNewTask(GetFile(itrw->first), GetType(itrw->first), info);
        _download_map[itrw->first] = info;
        _wait_map.erase(itrw);
    }
    //_map_lock.unlock();
}

void DownloadManager::UpdateDownloadMap()
{
    Guard guard(&_map_lock);
    //_map_lock.lock();
    //do nothing now
    
    //_map_lock.unlock();
}

void DownloadManager::UpdateFinishMap()
{
    Guard guard(&_map_lock);
    //_map_lock.lock();
    
    if(_finish_map.empty()) return;
    
    for(CallbackMap::iterator itr = _finish_map.begin(); itr != _finish_map.end(); ++itr) {
        std::string file = GetFile(itr->first);
        int type = GetType(itr->first);
        if(type == 0) {
            for(int i = 0; i < itr->second.d2f.size(); ++i) {
                D2FFinish func = itr->second.d2f[i];
                D2FStatus st = itr->second.d2f_st;
                st.user_ptr = itr->second.user_ptrs[i];
                func(file, st);
            }
        } else if(type == 1) {
            for(int i = 0; i < itr->second.d2m.size(); ++i) {
                D2MFinish func = itr->second.d2m[i];
                D2MStatus st = itr->second.d2m_st;
                st.user_ptr = itr->second.user_ptrs[i];
                func(file, st);
            }
        }
    }
    
    _finish_map.clear();
    //_map_lock.unlock();
}

void DownloadManager::CreateNewTask(const std::string& file, int type, DownloadManager::DownloadInfo &info)
{
    std::string url = PathUtil::Join(_remote_addr, file);
    std::string local = PathUtil::Join(_local_addr, file);

    if(type == 0) {
        DownloadToFileTask* task = new DownloadToFileTask(url, local, OnLoadToFile, this);
        ThreadPool::Schedule(task);
    } else if (type == 1) {
        DownloadToMemoryTask* task = new DownloadToMemoryTask(url, OnLoadToMemory, this);
        ThreadPool::Schedule(task);
    }
}

void DownloadManager::OnLoadToFile(const std::string &url, D2FStatus status)
{
    DownloadManager* mgr = (DownloadManager*)status.user_ptr;
    std::string file = PathUtil::GetFile(url);
    Guard guard(&mgr->_map_lock);

    std::string key = GetKey(file, 0);
    CallbackMap::iterator itr = mgr->_download_map.find(key);
    if(itr != mgr->_download_map.end()) {
        itr->second.d2f_st = status;
        mgr->_finish_map[itr->first] = itr->second;
        mgr->_download_map.erase(itr);
    }
}

void DownloadManager::OnLoadToMemory(const std::string &url, D2MStatus status)
{
    DownloadManager* mgr = (DownloadManager*)status.user_ptr;
    std::string file = PathUtil::GetFile(url);
    Guard guard(&mgr->_map_lock);
    
    std::string key = GetKey(file, 1);
    CallbackMap::iterator itr = mgr->_download_map.find(key);
    if(itr != mgr->_download_map.end()) {
        itr->second.d2m_st = status;
        mgr->_finish_map[itr->first] = itr->second;
        mgr->_download_map.erase(itr);
    } else {
        //no user care of the data, release
        delete[] status.data;
    }
}

std::string DownloadManager::GetKey(const std::string &file, int type)
{
    if(type == 0) return file + "_d2f";
    if(type == 1) return file + "_d2m";
    return "";
}

std::string DownloadManager::GetFile(const std::string &key)
{
    return key.substr(0, key.size() - 4);
}

int DownloadManager::GetType(const std::string &key)
{
    std::string ty = key.substr(key.size() - 4, 4);
    if(ty == "_d2f") return 0;
    if(ty == "_d2m") return 1;
    return -1;
}
}