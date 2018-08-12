#include "stdafx.h"
#include "TestServer.h"

int main(int argc, char* argv[])
{
	TestServer server;

	server.Start(27001, 20);

	printf("server strart\n");

	while (1)
	{
		Sleep(1);
	}

	return 0;
}