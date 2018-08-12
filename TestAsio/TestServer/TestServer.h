#pragma once
#include "NetServer.h"

#include "Test_PacketCommon.h"
#include "Test_PacketRecv.h"
#include "Test_PacketSend.h"

class TestServer : public NetServer, public Test_PacketRecv
{
public:
	TestServer();
	~TestServer();

private:
	Test_PacketSend m_testSender;

public:
	virtual void OnError(ErrorInfo& errorInfo) final;
	virtual void OnSessionConnect(SessionID sessionId) final;
	virtual void OnSessionClose(SessionID sessionId) final;

public:
	DECLARATION_ReqTestPacket;

};

