#ifndef __Example__Packet__
#define __Example__Packet__

#include <iostream>

namespace base
{
class Packet
{
public:
    Packet();
    Packet(const Packet& pck);
    Packet(int op, const char* data, int datalen);
    ~Packet();
    
    void operator=(const Packet& pck);
    
    inline int Opcode() const { return _opcode; }
    inline int DataLen() const { return _data_len; }
    inline const char* Data() const { return _data; }
    
private:
    int _opcode;
    int _data_len;
    char* _data;
};
}
#endif /* defined(__Example__Packet__) */
