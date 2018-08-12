#include "Test_PacketRecv.h"

Test_PacketRecv::Test_PacketRecv()
{
}

Test_PacketRecv::~Test_PacketRecv()
{
}

void Test_PacketRecv::CallPacketFunction(SessionID sessionId, PacketBufferPtr buffer)
{
    PacketStream stream(buffer, PacketStream::READ);
    PacketHeader* pHead = stream.GetPacketHead();

    switch (pHead->packetNo)
    {
        case PN_ReqTestPacket :
        {
            int value;

            stream >> value;

            ReqTestPacket(sessionId, value);
        }
        break;
        case PN_AckTestPacket :
        {
            TestData data;

            stream >> data;

            AckTestPacket(sessionId, data);
        }
        break;
        default:
        {
        }
        break;
    }
}
