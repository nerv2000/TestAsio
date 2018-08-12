#pragma once
#include <deque>
#include "boost/asio.hpp"

class PacketBuffer;
typedef std::shared_ptr<PacketBuffer> PacketBufferPtr;
class NetServer;
class NetServerSession : public std::enable_shared_from_this<NetServerSession>
{
private:
	typedef std::deque<PacketBufferPtr> PacketBufferQueue;

public:
	NetServerSession(const SessionID sessionId, boost::asio::ip::tcp::socket socket, NetServer* pServer);
	~NetServerSession();

	boost::asio::ip::tcp::socket& GetSocket() { return m_socket; }
	const SessionID GetSessionID() const { return m_sessionId; }

	void Start();
	void Close();

	void DoReadHeader();
	void DoReadBody(int bodySize);
	void DoSend(PacketBufferPtr buffer);

	void OnRecv(PacketBufferPtr buffer);
	void OnSend(PacketBufferPtr buffer);

private:
	const SessionID	m_sessionId;
	NetServer*		m_pServer;

	boost::asio::ip::tcp::socket	m_socket;
	boost::asio::io_service::strand	m_strand;

	PacketBufferPtr		m_recvBuffer;
	PacketBufferQueue	m_recvBufferQueue;
	PacketBufferQueue	m_sendBufferQueue;
};
typedef std::shared_ptr<NetServerSession> NetServerSessionPtr;

