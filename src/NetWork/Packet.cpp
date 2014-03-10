#include <string>

#include "Packet.h"

namespace base
{
Packet::Packet()
:_opcode(0),
_data_len(0),
_data(NULL)
{}

Packet::Packet(int op, const char* data, int datalen)
:_opcode(op),
_data_len(datalen)
{
    if(data == NULL || datalen == 0)
        return;
    _data = new char[_data_len];
    memcpy(_data, data, _data_len);
}

Packet::Packet(const Packet& pck)
{
    _opcode = pck._opcode;
    _data_len = pck._data_len;
    _data = new char[_data_len];
    memcpy(_data, pck._data, _data_len);
}

Packet::~Packet()
{
    if(_data != NULL) delete[] _data;
}

void Packet::operator=(const Packet& pck)
{
    _opcode = pck._opcode;
    _data_len = pck._data_len;
    
    if(_data != NULL) delete[] _data;
    
    _data = new char[_data_len];
    memcpy(_data, pck._data, _data_len);
}
}