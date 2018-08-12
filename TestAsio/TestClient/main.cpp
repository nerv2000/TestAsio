// TestClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "TestClient.h"

int main(int argc, char* argv[])
{
	TestClient client;
	client.Connect(std::string("127.0.0.1"), 27001);

	Sleep(1000);

	while (true)
	{
		client.FrameMove();
		Sleep(1);
	}

	return 0;
}