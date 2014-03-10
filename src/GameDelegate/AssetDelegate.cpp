#include "AssetDelegate.h"
#include "FxShippingPlayer.h"
#include "PathUtil.h"

#include <assert.h>
#include <string.h>

class MovieLoadHandler : public base::AssetHandler
{
public:
    void Handle(const std::string& asset, base::AssetLoadStatus status)
    {
        assert(!asset.empty());
        
        Scaleform::GFx::Value args[4];
        args[0].SetString(asset.c_str());
        args[1].SetInt(status.errcode);
        args[2].SetInt(status.internal_errcode);
        //args[3].SetString("");
        if(status.errcode == ASSET_OK) {
            args[3].WriteToByteArray((const UByte*)status.data, status.len);
        }
        
        FxShippingPlayer::pApp->pMovie->Invoke("_level0.c2f.assetLoader._onLoadAsset", NULL, args, 4);
        
        //trick code!!!
        delete this;
    }
};

class MovieFileHandler : public base::AssetFileHandler
{
public:
    void Handle(const std::string& asset, base::AssetFileStatus status)
    {
        assert(!asset.empty());
        
        Scaleform::GFx::Value args[4];
        args[0].SetString(asset.c_str());
        args[1].SetInt(status.status);
        args[2].SetInt(status.internal_errorcode);
        args[3].SetString(status.full_path.c_str());
        
        printf("invoke flash get asset file\n");
        FxShippingPlayer::pApp->pMovie->Invoke("_level0.c2f.assetLoader._onGetAssetFile", NULL, args, 4);
        
        //trick code!!!
        delete this;
    }
};

void AssetDelegate::OnInit()
{
    base::AssetManagerConfig conf;
    conf.local_addr = FxShippingPlayer::GetContentDirectory().ToCStr();
    conf.remote_addr = "http://127.0.0.1";
    conf.settings_file = "index.info";
    conf.task_parallel = 8;
    base::AssetManager::Instance()->Prepare(conf);
}

void AssetDelegate::Accept(FxDelegateHandler::CallbackProcessor* cbreg)
{
    cbreg->Process("Asset_LoadAsset", AssetDelegate::_LoadAsset);
    cbreg->Process("Asset_GetAssetFile", AssetDelegate::_GetAssetFile);
    cbreg->Process("Asset_Prepare", AssetDelegate::_Prepare);
    cbreg->Process("Asset_Prepared", AssetDelegate::_Prepared);
}

void AssetDelegate::_LoadAsset(const FxDelegateArgs &params)
{
    base::AssetManager* mgr = base::AssetManager::Instance();
    const char* asset = params[0].GetString();
    mgr->LoadAsset(asset, new MovieLoadHandler());
}

void AssetDelegate::_GetAssetFile(const FxDelegateArgs &params)
{
    base::AssetManager* mgr = base::AssetManager::Instance();
    const char* asset = params[0].GetString();
    mgr->GetAssetFile(asset, new MovieFileHandler());
}

void AssetDelegate::_Prepare(const FxDelegateArgs& params)
{
//    base::AssetManagerConfig conf;
//    conf.local_addr = base::PathUtil::Join(FxShippingPlayer::GetContentDirectory().ToCStr(), params[0].GetString());
//    conf.remote_addr = params[1].GetString();
//    conf.settings_file = params[2].GetString();
//    conf.task_parallel = params[3].GetInt();
//    base::AssetManager::Instance()->Prepare(conf);
}

void AssetDelegate::_Prepared(const FxDelegateArgs& params)
{
    Scaleform::GFx::Value args[1];
    args[1].SetBoolean(base::AssetManager::Instance()->Prepared());
    FxShippingPlayer::pApp->pMovie->Invoke("_level0.c2f.assetLoader._onPrepared", NULL, args, 1);

}