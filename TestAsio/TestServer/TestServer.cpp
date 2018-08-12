#include "stdafx.h"
#include "TestServer.h"

#include "PacketStreamAdd.h"
#include "Test_PacketCommon.cpp"
#include "Test_PacketRecv.cpp"
#include "Test_PacketSend.cpp"

TestServer::TestServer()
{
	Attch(this);
	Attch(&m_testSender);
}

TestServer::~TestServer()
{
	Detach(this);
	Detach(&m_testSender);
}

void TestServer::OnError(ErrorInfo & errorInfo)
{
	printf("OnError %d - %s\n", errorInfo.socketError, errorInfo.message.c_str());
}

void TestServer::OnSessionConnect(SessionID sessionId)
{
	printf("Session Connect %d\n", sessionId);
}

void TestServer::OnSessionClose(SessionID sessionId)
{
	printf("Session Close %d\n", sessionId);
}

DEFINITION_ReqTestPacket(TestServer)
{
	printf("ReqTestPacket ��Ŷ ���� - value : %d\n", value);

	TestData data;
	data.val1 = value;
	data.val2 = "�� �޾Ѵ�.";
	m_testSender.AckTestPacket(sessionId, data);

	printf("AckTestPacket ��Ŷ ���� - val1:%d val2:%s\n"
		, data.val1
		, data.val2.c_str()
	);
}