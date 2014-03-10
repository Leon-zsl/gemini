#ifndef Gemini_PacketHandler_h
#define Gemini_PacketHandler_h

namespace base
{
class Packet;
class PacketSock;
class PacketHandler
{
public:
    virtual ~PacketHandler() {}

    virtual void HandlePacket(PacketSock* sock, const Packet& pck) = 0;
    virtual void HandleError(PacketSock* sock, int errcode) = 0;
};
}
#endif
