#ifndef __Gemini__PacketDelegate__
#define __Gemini__PacketDelegate__

#include <iostream>
#include <map>
#include "FxGameDelegate.h"
#include "PacketHandler.h"

#include "PacketSock.h"

class PacketDelegate : public FxDelegateHandler, base::PacketHandler
{
public:
    PacketDelegate();
    virtual ~PacketDelegate();
    
    base::PacketSock* GetSock(const std::string& name) { return _socks[name]; }

    void ConnectServer(const std::string& name, const std::string& ip, short port);
    void DisconnectServer(const std::string& name);
    void SendPacket(const std::string& name, const base::Packet& pck);
    
    virtual void Accept(FxDelegateHandler::CallbackProcessor* cbreg);
    
    virtual void HandlePacket(base::PacketSock* sock, const base::Packet& pck);
    virtual void HandleError(base::PacketSock* sock, int error);
    
    static void _TransferMsgFromSF(const FxDelegateArgs& params);
    static void _ConnectServer(const FxDelegateArgs& params);
    static void _DisconnectServer(const FxDelegateArgs& params);
    
private:
    PacketDelegate(PacketDelegate& val);
    void operator=(PacketDelegate& val);
    
private:
    typedef std::map<std::string, base::PacketSock*> SockMap;
    SockMap _socks;
    
//    static int _buffer_size;
//    static char* _buffer;
};

#endif /* defined(__Gemini__PacketDelegate__) */
