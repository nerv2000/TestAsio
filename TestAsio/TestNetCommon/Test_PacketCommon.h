#pragma once
#include "PacketStream.h"
#include "NetPacket.h"

class Test_PacketCommon
{
public:
    Test_PacketCommon();
    ~Test_PacketCommon();

protected:
    enum PacketNo : uint16_t
    {
        PN_ReqTestPacket = 1000
        , PN_AckTestPacket
    };

};

