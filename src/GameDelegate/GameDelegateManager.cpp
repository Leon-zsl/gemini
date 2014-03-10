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

void GameDelegateManager::OnInit(FxShippingPlayer* player, Loader &loader,
                                 const std::string& serverIp, short serverPort)
{
    pGameDelegate   = *new FxDelegate();
    pPacketDelegate = *new PacketDelegate();
    pAssetDelegate  = *new AssetDelegate();
    pLogDelegate    = *new LogDelegate();
    //pPacketDelegate->ConnectServer(serverIp, serverPort);
    
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