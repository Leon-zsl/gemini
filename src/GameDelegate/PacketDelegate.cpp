#include <string>
#include "SockMgr.h"
#include "Packet.h"
#include "PacketSock.h"
#include "PacketDelegate.h"
#include "FxShippingPlayer.h"

PacketDelegate::PacketDelegate()
{
//    _buffer_size = 512;
//    _buffer = new char[512];
}

PacketDelegate::~PacketDelegate()
{
    for(SockMap::iterator itr = _socks.begin(); itr != _socks.end(); ++itr) {
        base::SockMgr::Instance()->DestroySock(itr->second);
    }
    _socks.clear();
    
//    delete[] _buffer;
}

void PacketDelegate::ConnectServer(const std::string& name, const std::string &ip, short port)
{
    SockMap::iterator itr = _socks.find(name);
    if(itr != _socks.end()) {
        base::SockMgr::Instance()->DestroySock(itr->second);
        _socks.erase(name);
    }
    
    base::PacketSock* sock = base::SockMgr::Instance()->CreateSock();
    sock->SetHandler(this);
    _socks[name] = sock;

    sock->Connect(ip, port);
}

void PacketDelegate::DisconnectServer(const std::string &name)
{
    SockMap::iterator itr = _socks.find(name);
    if(itr != _socks.end()) {
        base::SockMgr::Instance()->DestroySock(itr->second);
        _socks.erase(name);
    }
}

void PacketDelegate::SendPacket(const std::string& name, const base::Packet& pck)
{
    SockMap::iterator itr = _socks.find(name);
    if(itr != _socks.end()) {
        itr->second->SendPacket(pck);
    } else {
        printf("can not find sock: %s", name.c_str());
    }
}

void PacketDelegate::Accept(FxDelegateHandler::CallbackProcessor* cbreg)
{
    cbreg->Process("Packet_TransferMsgFromSF", PacketDelegate::_TransferMsgFromSF);
    cbreg->Process("Packet_ConnectServer", PacketDelegate::_ConnectServer);
    cbreg->Process("Packet_DisconnectServer", PacketDelegate::_DisconnectServer);
}

void PacketDelegate::HandlePacket(base::PacketSock* sock, const base::Packet& pck)
{   
    std::string name = "";
    for(SockMap::iterator itr = _socks.begin(); itr != _socks.end(); ++itr) {
        if(itr->second == sock)
            name = itr->first;
    }
    if(name == "")
        return;
    
//    if (pck.DataLen() + 1 > buf_size) {
//        delete[] _buffer;
//        _buffer_size += 512;
//        _buffer = new char[_buffer_size];
//    }
//    memcpy(_buffer, pck.Data(), pck.DataLen());
//    _buffer[pck.DataLen()] = '\0';
    
    char* buf = NULL;
    char stack_buffer[512];
    char* heap_buffer = NULL;
    if(pck.DataLen() + 1 > 512) {
        heap_buffer = new char[pck.DataLen() + 1];
        buf = heap_buffer;
    } else {
        buf = stack_buffer;
    }
    memcpy(buf, pck.Data(), pck.DataLen());
    buf[pck.DataLen()] = '\0';

    Scaleform::GFx::Value args[3];
    args[0].SetString(name.c_str());
    args[1].SetInt(pck.Opcode());
    args[2].SetString(buf);
    FxShippingPlayer::pApp->pMovie->Invoke("_level0.c2f.packetDispathcer._recvPacket", NULL, args, 3);
    
    if(heap_buffer != NULL)
        delete[] heap_buffer;
}

void PacketDelegate::HandleError(base::PacketSock* sock, int error)
{   
    //printf("sock error: [ip]%s, [port]%d, [error]%d\n", sock->Ip().c_str(), sock->Port(), error);
    
    std::string name = "";
    for(SockMap::iterator itr = _socks.begin(); itr != _socks.end(); ++itr) {
        if(itr->second == sock)
            name = itr->first;
    }
    if(name == "")
        return;
    
    Scaleform::GFx::Value args[2];
    args[0].SetString(name.c_str());
    args[1].SetInt(error);
    FxShippingPlayer::pApp->pMovie->Invoke("_level0.c2f.packetDispathcer._connError", NULL, args, 2);
}

void PacketDelegate::_TransferMsgFromSF(const FxDelegateArgs& params)
{
    const char* name = params[0].GetString();
    int op = params[1].GetInt();
    const char* data = params[2].GetString();
    PacketDelegate* delegate = (PacketDelegate*)params.GetHandler();
    delegate->SendPacket(name, base::Packet(op, data, strlen(data)));
}

void PacketDelegate::_ConnectServer(const FxDelegateArgs& params)
{
    const char* name = params[0].GetString();
    const char* ip = params[1].GetString();
    int port = params[2].GetInt();
    
    PacketDelegate* delegate = (PacketDelegate*)params.GetHandler();
    delegate->ConnectServer(name, ip, (short)port);
}

void PacketDelegate::_DisconnectServer(const FxDelegateArgs& params)
{
    const char* name = params[0].GetString();
    PacketDelegate* delegate = (PacketDelegate*)params.GetHandler();
    delegate->DisconnectServer(name);
}