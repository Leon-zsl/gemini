#ifndef __Example__AssetManager__
#define __Example__AssetManager__

#include <string>
#include <map>

#include "FileLoader.h"
#include "DownloadManager.h"
#include "AssetHandler.h"
#include "DownloadStatus.h"
#include "AssetManagerConfig.h"
#include "AssetSettingUtil.h"
#include "Singleton.h"

namespace base
{
class AssetManager : public Singleton<AssetManager>
{
public:
    ~AssetManager();
    
    //the mgr can not work properly until this method return true
    bool Prepared() const { return _prepared; }
    
    void Startup(AssetManagerConfig conf);
    void Shutdown();
    void Update();

    void GetAssetFile(const std::string& asset, AssetFileHandler* h);
    void LoadAsset(const std::string& asset, AssetHandler* h);
    void LoadLocalAsset(const std::string& asset, AssetHandler* h);

protected:
//    void LoadRemoteAssetToMemory(const std::string& asset, AssetHandler* h);
//    void LoadRemoteAssetToFile(const std::string& asset, AssetFileHandler* h);

private:
    static void OnDownloadToMemory(const std::string& asset, D2MStatus st);
    static void OnDownloadToFile(const std::string& asset, D2FStatus st);
    static void OnLoadFile(const std::string& asset, LFStatus st);
    static void OnLoadSettings(const std::string& asset, LFStatus st);
    static void OnDownloadSettings(const std::string& asset, D2MStatus st);
    
private:
    std::map<std::string, std::vector<AssetHandler*> >  _d2f_map;
    std::map<std::string, std::vector<AssetHandler*> >  _d2m_map;
    std::map<std::string, std::vector<AssetHandler*> >  _load_map;
    std::map<std::string, std::vector<AssetFileHandler*> >  _d2af_map;
    
    DownloadManager*                                    _download_mgr;
    FileLoader*                                         _file_loader;
    std::string                                         _settings_file;
    bool                                                _prepared;
    std::map<std::string, FileInfo>                     _info;
};
}
#endif
