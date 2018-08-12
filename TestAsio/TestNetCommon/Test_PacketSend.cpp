#include "Test_PacketSend.h"

Test_PacketSend::Test_PacketSend()
{
}

Test_PacketSend::~Test_PacketSend()
{
}

void Test_PacketSend::ReqTestPacket(const SessionID sessionId, const int& value)
{
    PacketStream stream(std::make_shared<PacketBuffer>(), PacketStream::WRITE);
    PacketHeader* pHead = stream.GetPacketHead();

    stream << value;

    pHead->packetNo = PN_ReqTestPacket;
    pHead->size = stream.GetWriteLength();

    m_pPacketManager->SendNotify(sessionId, stream.GetBuffer());
}

void Test_PacketSend::AckTestPacket(const SessionID sessionId, const TestData& data)
{
    PacketStream stream(std::make_shared<PacketBuffer>(), PacketStream::WRITE);
    PacketHeader* pHead = stream.GetPacketHead();

    stream << data;

    pHead->packetNo = PN_AckTestPacket;
    pHead->size = stream.GetWriteLength();

    m_pPacketManager->SendNotify(sessionId, stream.GetBuffer());
}

