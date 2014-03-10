//#include "Packet.h"
#include "GameDelegateManager.h"

void GameDelegateManager::RegisterDelegateHandler()
{
    pGameDelegate->RegisterHandler(pPacketDelegate);
    pGameDelegate->RegisterHandler(pAssetDelegate);
    pGameDelegate->RegisterHandler(pLogDelegate);
}

void GameDelegateManager::UnRegisterDelegateHandler()
{
    
}

void GameDelegateManager::OnInit(FxShippingPlayer* player, Loader &loader)
{
    pGameDelegate   = *new FxDelegate();
    pLogDelegate    = *new LogDelegate();
    pPacketDelegate = *new PacketDelegate();
    pPacketDelegate->OnInit();
    pAssetDelegate  = *new AssetDelegate();
    pAssetDelegate->OnInit();
    
    loader.SetExternalInterface(pGameDelegate);
    
    RegisterDelegateHandler();
}

void GameDelegateManager::OnShutDown()
{
    UnRegisterDelegateHandler();
}

void GameDelegateManager::OnUpdate()
{
}