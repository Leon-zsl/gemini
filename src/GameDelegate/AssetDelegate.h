#ifndef __Gemini__AssetDelegate__
#define __Gemini__AssetDelegate__

#include "FxGameDelegate.h"
#include "AssetManager.h"

class AssetDelegate : public FxDelegateHandler
{
public:
    AssetDelegate() {}
    
    void OnInit();
    virtual void Accept(FxDelegateHandler::CallbackProcessor* cbreg);
    
    static void _LoadAsset(const FxDelegateArgs& params);
    static void _GetAssetFile(const FxDelegateArgs& params);
    static void _Prepare(const FxDelegateArgs& params);
    static void _Prepared(const FxDelegateArgs& params);
    
private:
    AssetDelegate(AssetDelegate& val);
    void operator=(AssetDelegate& val);
};

#endif
