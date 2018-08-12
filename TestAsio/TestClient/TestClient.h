#pragma once
#include "NetClient.h"

#include "Test_PacketCommon.h"
#include "Test_PacketRecv.h"
#include "Test_PacketSend.h"

class TestClient : public NetClient, public Test_PacketRecv
{
public:
	TestClient();
	~TestClient();

public:
	Test_PacketSend	 m_testSender;

public:
	virtual void OnError(ErrorInfo& errorInfo) final;
	virtual void OnSessionConnect(SessionID sessionId) final;
	virtual void OnSessionClose(SessionID sessionId) final;

public:
	DECLARATION_AckTestPacket;
};

