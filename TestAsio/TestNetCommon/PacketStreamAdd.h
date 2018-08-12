#pragma once
#include "CommonStruct.h"
#include "PacketStream.h"

PacketStream& operator<<(PacketStream& a, const TestData& value)
{
	a << value.val1;
	a << value.val2;
	return a;
}

PacketStream& operator>>(PacketStream& a, TestData& value)
{
	a >> value.val1;
	a >> value.val2;
	return a;
}



