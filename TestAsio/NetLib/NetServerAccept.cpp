#include "stdafx.h"
#include "NetServerAccept.h"
#include "NetServer.h"
#include "NetServerSession.h"

NetAccept::NetAccept(boost::asio::io_service & io_service, const uint16_t port, NetServer * pServer)
	: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, m_acceptSocket(io_service)
	, m_pServer(pServer)
{
}

NetAccept::~NetAccept()
{
}

void NetAccept::Start()
{
	DoAccept();
}

void NetAccept::DoAccept()
{
	m_acceptor.async_accept(m_acceptSocket,
		[this](boost::system::error_code ec)
	{
		if (0 != ec.value())
		{
			ErrorInfo errorInfo;
			errorInfo.socketError = ec.value();
			errorInfo.message = ec.message();
			m_pServer->OnError(errorInfo);
		}
		else
		{
			NetServerSessionPtr session = m_pServer->InsertSession(m_acceptSocket);
			if (NULL != session)
			{
				session->Start();
				m_pServer->OnSessionConnect(session->GetSessionID());
			}
		}

		DoAccept();
	});
}
