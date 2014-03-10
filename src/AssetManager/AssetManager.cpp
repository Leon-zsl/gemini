#include "AssetManager.h"
#include "PathUtil.h"

#include <assert.h>
#include <stdio.h>

namespace base
{
AssetManager::AssetManager()
{
    _file_loader = new FileLoader();
    _download_mgr = new DownloadManager();
    _prepared = false;
}

AssetManager::~AssetManager()
{
    Shutdown();

    delete _file_loader;
    delete _download_mgr;
}

void AssetManager::Startup(void)
{
    _file_loader->Startup();
    _download_mgr->Startup();
}

void AssetManager::Shutdown()
{
    _file_loader->Shutdown();
    _download_mgr->Shutdown();
    
    _load_map.clear();
    _d2f_map.clear();
    _d2m_map.clear();
    
    AssetSettingUtil::SaveSettings(PathUtil::Join(_file_loader->GetPath(), _settings_file), _info);
}

void AssetManager::Update()
{
    _download_mgr->Update();
    _file_loader->Update();
}
    
void AssetManager::Prepare(base::AssetManagerConfig conf)
{
    if(_prepared) return;
    
    FileLoadConfig fc;
    fc.file_path = conf.local_addr;
    _file_loader->Config(fc);
    
    DownloadConfig df;
    df.localDir = conf.local_addr;
    df.serverAddr = conf.remote_addr;
    df.maxDownloadTask = conf.task_parallel;
    _download_mgr->Config(df);

    _settings_file = conf.settings_file;
    
    _file_loader->LoadFile(_settings_file, OnLoadSettings, this);
}
    
void AssetManager::GetAssetFile(const std::string& asset, AssetFileHandler* h)
{
    if(!_prepared) return;
    if(h == NULL || asset.empty()) return;
    
    std::map<std::string, FileInfo>::iterator itr = _info.find(asset);
    if(itr == _info.end()) {
        AssetFileStatus st;
        st.status = AF_UNKNOWNASSET;
        h->Handle(asset, st);
    }
    
    if(itr->second.expired) {
        std::map<std::string, std::vector<AssetFileHandler*> >::iterator itr = _d2af_map.find(asset);
        if(itr != _d2af_map.end()) {
            itr->second.push_back(h);
        }
        
        std::vector<AssetFileHandler*> hs;
        hs.push_back(h);
        _d2af_map[asset] = hs;
        
        _download_mgr->DownloadToFile(asset, OnDownloadToFile, this);
    } else {
        AssetFileStatus st;
        st.status = AF_USE_LOCAL;
        st.full_path = PathUtil::Join(_file_loader->GetPath(), asset);
        h->Handle(asset, st);
    }
}

void AssetManager::LoadAsset(const std::string &asset, AssetHandler* h)
{
    if(!_prepared) return;
    if(h == NULL || asset.empty()) return;
    
    std::map<std::string, FileInfo>::iterator itr = _info.find(asset);
    if(itr == _info.end()) {
        AssetLoadStatus st;
        st.errcode = ASSET_ERR_UNKNOWNASSET;
        h->Handle(asset, st);
    }
    
    if(itr->second.expired) {
        std::map<std::string, std::vector<AssetHandler*> >::iterator itr = _d2f_map.find(asset);
        if(itr != _d2f_map.end()) {
            itr->second.push_back(h);
        }
        
        std::vector<AssetHandler*> hs;
        hs.push_back(h);
        _d2f_map[asset] = hs;

        _download_mgr->DownloadToFile(asset, OnDownloadToFile, this);
    } else {
        LoadLocalAsset(asset, h);
    }
}

void AssetManager::LoadLocalAsset(const std::string &asset, AssetHandler* h)
{
    if(!_prepared) return;
    if(h == NULL || asset.empty()) return;
    
    std::map<std::string, std::vector<AssetHandler*> >::iterator itr = _load_map.find(asset);
    if(itr != _load_map.end()) {
        itr->second.push_back(h);
    }
    
    std::vector<AssetHandler*> hs;
    hs.push_back(h);
    _load_map[asset] = hs;
    
    _file_loader->LoadFile(asset, OnLoadFile, this);
}

//void AssetManager::LoadRemoteAssetToMemory(const std::string &asset, AssetHandler* h)
//{
//    if(_prepared) return;
//    if(h == NULL || asset.empty()) return;
//    
//    std::map<std::string, std::vector<AssetHandler*> >::iterator itr = _d2m_map.find(asset);
//    if(itr != _d2m_map.end()) {
//        itr->second.push_back(h);
//    }
//    
//    std::vector<AssetHandler*> hs;
//    hs.push_back(h);
//    _d2m_map[asset] = hs;
//    
//    _download_mgr->DownloadToMemory(asset, OnDownloadToMemory, this);
//}

void AssetManager::OnDownloadToMemory(const std::string& asset, D2MStatus st)
{
    AssetManager* mgr = (AssetManager*)st.user_ptr;
    std::map<std::string, std::vector<AssetHandler*> >::iterator itr = mgr->_d2m_map.find(asset);
    if(itr != mgr->_d2m_map.end()) {
        AssetLoadStatus ast;
        if(st.status != D2M_OK) {
            ast.errcode = ASSET_ERR_DOWNLOADFAILED;
            ast.internal_errcode = st.status;
        } else {
            ast.data = st.data;
            ast.len = st.len;
        }
        for(std::vector<AssetHandler*>::iterator itr0 = itr->second.begin(); itr0 != itr->second.end(); ++itr0) {
            (*itr0)->Handle(asset, ast);
        }
        
        mgr->_d2m_map.erase(itr);
    } else {
        delete[] st.data;
    }
}

void AssetManager::OnDownloadToFile(const std::string& asset, D2FStatus st)
{
    AssetManager* mgr = (AssetManager*)st.user_ptr;
    std::map<std::string, std::vector<AssetHandler*> >::iterator itr = mgr->_d2f_map.find(asset);
    if(itr != mgr->_d2f_map.end()) {
        AssetLoadStatus ast;
        if(st.status != D2F_OK) {
            ast.errcode = ASSET_ERR_DOWNLOADFAILED;
            ast.internal_errcode = st.status;
            
            for(std::vector<AssetHandler*>::iterator itr0 = itr->second.begin(); itr0 != itr->second.end(); ++itr0) {
                (*itr0)->Handle(asset, ast);
            }
        } else {
            std::map<std::string, FileInfo>::iterator itrf = mgr->_info.find(asset);
            if(itrf != mgr->_info.end()) {
                itrf->second.expired = false;
            }
            for(std::vector<AssetHandler*>::iterator itr0 = itr->second.begin(); itr0 != itr->second.end(); ++itr0) {
                mgr->LoadLocalAsset(asset, *itr0);
            }
        }
        mgr->_d2f_map.erase(itr);
    }
    
    std::map<std::string, std::vector<AssetFileHandler*> >::iterator itra = mgr->_d2af_map.find(asset);
    if(itra != mgr->_d2af_map.end()) {
        AssetFileStatus ast;
        if(st.status != D2F_OK) {
            ast.status = AF_DOWNLOADFAILED;
            ast.internal_errorcode = st.status;
            
            for(std::vector<AssetFileHandler*>::iterator itr0 = itra->second.begin(); itr0 != itra->second.end(); ++itr0) {
                (*itr0)->Handle(asset,ast);
            }
        } else {
            std::map<std::string, FileInfo>::iterator itrf = mgr->_info.find(asset);
            if(itrf != mgr->_info.end()) {
                itrf->second.expired = false;
            }
            for(std::vector<AssetFileHandler*>::iterator itr0 = itra->second.begin(); itr0 != itra->second.end(); ++itr0) {
                ast.status = AF_USE_REMOTE;
                ast.full_path = PathUtil::Join(mgr->_file_loader->GetPath(), asset);
                (*itr0)->Handle(asset, ast);
            }
        }
    }
}

void AssetManager::OnLoadFile(const std::string& asset, LFStatus st)
{
    AssetManager* mgr = (AssetManager*)st.user_ptr;
    std::map<std::string, std::vector<AssetHandler*> >::iterator itr = mgr->_load_map.find(asset);
    if(itr != mgr->_load_map.end()) {
        AssetLoadStatus ast;
        if(st.status != LF_OK) {
            ast.errcode = ASSET_ERR_LOADFAILED;
            ast.internal_errcode = st.status;
        } else {
            ast.data = st.data;
            ast.len = st.len;
        }
        for(std::vector<AssetHandler*>::iterator itr0 = itr->second.begin(); itr0 != itr->second.end(); ++itr0) {
            (*itr0)->Handle(asset, ast);
        }
        
        mgr->_load_map.erase(itr);
    } else {
        delete[] st.data;
    }
}

void AssetManager::OnLoadSettings(const std::string& asset, LFStatus st)
{
    AssetManager* mgr = (AssetManager*)st.user_ptr;
    assert(asset == mgr->_settings_file);
    
    if(st.status == LF_OK) {
        AssetSettingUtil::ParseLocalSettings(st.data, st.len, mgr->_info);
    }
    
    delete[] st.data;
    printf("load local settings finish: %s, %d\n", mgr->_settings_file.c_str(), st.status);

    mgr->_download_mgr->DownloadToMemory(mgr->_settings_file, OnDownloadSettings, mgr);
}

void AssetManager::OnDownloadSettings(const std::string& asset, D2MStatus st)
{
    AssetManager* mgr = (AssetManager*)st.user_ptr;
    assert(asset == mgr->_settings_file);
    
    if(st.status == D2M_OK) {
        AssetSettingUtil::ParseRemoteSettings(st.data, st.len, mgr->_info);
    }

    delete[] st.data;
    printf("download remote settings finish: %s, %d\n", mgr->_settings_file.c_str(), st.status);
    
    if(st.status == D2M_OK)
        mgr->_prepared = true;
}
}