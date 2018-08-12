#pragma once
#include <string>
#include "netCommon.h"

struct ErrorInfo
{
	int			socketError;
	std::string message;

	ErrorInfo()
	{
		socketError = -1;
	}
};

__interface IDefaultCallBackFunction
{
	virtual void OnError(ErrorInfo& errorInfo) = 0;
	virtual void OnSessionConnect(SessionID sessionId) = 0;
	virtual void OnSessionClose(SessionID sessionId) = 0;
};
