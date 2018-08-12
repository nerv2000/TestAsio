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
	printf("connect ���� ����\n");

	int value = 100;
	m_testSender.ReqTestPacket(1, value);

	printf("ReqTestPacket ��Ŷ ���� - �� %d\n", value);
}

void TestClient::OnSessionClose(SessionID sessionId)
{
	printf("connect ���� ������\n");
}

DEFINITION_AckTestPacket(TestClient)
{
	printf("AckTestPacket ��Ŷ ���� - val1:%d val2:%s\n"
		, data.val1
		, data.val2.c_str()
	);
}