#include "AssetDelegate.h"
#include "FxShippingPlayer.h"

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
        
        FxShippingPlayer::pApp->pMovie->Invoke("_level0.assetLoader._onLoadAsset", NULL, args, 4);
        
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
        FxShippingPlayer::pApp->pMovie->Invoke("_level0.assetLoader._onGetAssetFile", NULL, args, 4);
        
        //trick code!!!
        delete this;
    }
};

void AssetDelegate::Accept(FxDelegateHandler::CallbackProcessor* cbreg)
{
    cbreg->Process("_LoadAsset", AssetDelegate::_LoadAsset);
    cbreg->Process("_GetAssetFile", AssetDelegate::_GetAssetFile);
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