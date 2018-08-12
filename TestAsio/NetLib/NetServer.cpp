#include "stdafx.h"
#include "NetServer.h"
#include "NetServerSession.h"

NetServer::NetServer()
	: PacketManager(this)
	, m_currentSessionId(0)
{
}

NetServer::~NetServer()
{
	m_netServerSessionMap.clear();
}

void NetServer::Start(const unsigned short port, const int threadCount)
{
	m_netAccept = std::make_unique<NetAccept>(m_ioService, port, this);
	m_netThread = std::make_unique<NetThread>(m_ioService, threadCount);

	m_netAccept->Start();
}

NetServerSessionPtr NetServer::InsertSession(boost::asio::ip::tcp::socket & socket)
{
	std::lock_guard<std::mutex> guard(m_lock);

	NetServerSessionPtr session = std::make_shared<NetServerSession>(GetNewSessionId(), std::move(socket), this);
	m_netServerSessionMap.insert(NetServerSessionMap_ValueType(session->GetSessionID(), session));

	return session;
}

void NetServer::CloseSession(const SessionID sessionId)
{
	std::lock_guard<std::mutex> guard(m_lock);

	auto it = m_netServerSessionMap.find(sessionId);
	if (it == m_netServerSessionMap.end())
		return;

	OnSessionClose(sessionId);

	it->second->Close();

	m_netServerSessionMap.erase(it);
}

void NetServer::CallRecv(SessionID sessionId, PacketBufferPtr buffer)
{
	RecvNotify(sessionId, buffer);
}

void NetServer::CallSend(SessionID sessionId, PacketBufferPtr buffer)
{
	std::lock_guard<std::mutex> guard(m_lock);

	auto it = m_netServerSessionMap.find(sessionId);
	if (it == m_netServerSessionMap.end())
		return;

	it->second->OnSend(buffer);
}
