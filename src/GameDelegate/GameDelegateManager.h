#ifndef __Gemini__GameDelegateManager__
#define __Gemini__GameDelegateManager__

#include <iostream>
#include <string>

#include "Singleton.h"
#include "FxGameDelegate.h"

#include "AssetDelegate.h"
#include "LogDelegate.h"
#include "PacketDelegate.h"

class FxShippingPlayer;

class GameDelegateManager: public Singleton<GameDelegateManager>
{
private:
    GameDelegateManager(void)
    {
    }
    virtual ~GameDelegateManager(void)
	{
	}
    
    Ptr<FxDelegate>         pGameDelegate;
    Ptr<PacketDelegate>     pPacketDelegate;
    Ptr<AssetDelegate>      pAssetDelegate;
    Ptr<LogDelegate>        pLogDelegate;

public:
    void RegisterDelegateHandler();
    void UnRegisterDelegateHandler();
    void OnInit(FxShippingPlayer* player, Loader &loader,
                const std::string& serverIp, short serverPort);
    void OnShutDown();
    void OnUpdate();
    
    DECLARE_SINGLETON_CLASS(GameDelegateManager);
};


#endif
