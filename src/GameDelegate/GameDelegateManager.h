//
//  DelegateManager.h
//  MagicTT
//
//  Created by tao.wu on 13-4-8.
//
//

#ifndef __MagicTT__GameDelegateManager__
#define __MagicTT__GameDelegateManager__

#include <iostream>
#include <string>
#include "MagicDelegate.h"
#include "Singleton.h"

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


#endif /* defined(__MagicTT__GameDelegateManager__) */
