#include "stdafx.h"
#include "TestClient.h"

#include "PacketStreamAdd.h"
#include "Test_PacketCommon.cpp"
#include "Test_PacketRecv.cpp"
#include "Test_PacketSend.cpp"

TestClient::TestClient()
{
	Attch(this);
	Attch(&m_testSender);
}

TestClient::~TestClient()
{
	Detach(this);
	Detach(&m_testSender);
}

void TestClient::OnError(ErrorInfo & errorInfo)
{
	printf("OnError %d - %s\n", errorInfo.socketError, errorInfo.message.c_str());
}

void TestClient::OnSessionConnect(SessionID sessionId)
{
	printf("connect 연결 성공\n");

	int value = 100;
	m_testSender.ReqTestPacket(1, value);

	printf("ReqTestPacket 패킷 보냄 - 값 %d\n", value);
}

void TestClient::OnSessionClose(SessionID sessionId)
{
	printf("connect 연결 끊어짐\n");
}

DEFINITION_AckTestPacket(TestClient)
{
	printf("AckTestPacket 패킷 받음 - val1:%d val2:%s\n"
		, data.val1
		, data.val2.c_str()
	);
}