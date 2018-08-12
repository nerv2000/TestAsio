#pragma once

class Test_PacketSend : public ISendPacket, public Test_PacketCommon
{
public:
    Test_PacketSend();
    ~Test_PacketSend();

    void ReqTestPacket(const SessionID sessionId, const int& value);
    void AckTestPacket(const SessionID sessionId, const TestData& data);

};
