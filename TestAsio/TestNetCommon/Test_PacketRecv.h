#pragma once

class Test_PacketRecv : public IRecvPacket, public Test_PacketCommon
{
public:
Test_PacketRecv();
~Test_PacketRecv();

private:
    void CallPacketFunction(SessionID sessionId, PacketBufferPtr buffer) final;

public:
    virtual void ReqTestPacket(SessionID sessionId, int& value) {}
    virtual void AckTestPacket(SessionID sessionId, TestData& data) {}

};
#define DECLARATION_ReqTestPacket virtual void ReqTestPacket(SessionID sessionId, int& value) final
#define DEFINITION_ReqTestPacket(className) void className##::ReqTestPacket(SessionID sessionId, int& value)

#define DECLARATION_AckTestPacket virtual void AckTestPacket(SessionID sessionId, TestData& data) final
#define DEFINITION_AckTestPacket(className) void className##::AckTestPacket(SessionID sessionId, TestData& data)

