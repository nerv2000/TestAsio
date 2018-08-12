#pragma once
#include <deque>
#include "boost/asio.hpp"

#include "ErrorFunction.h"
#include "NetPacket.h"
#include "NetThread.h"

class NetClient
	: public IDefaultCallBackFunction
	, public IPacketCallFunction
	, public PacketManager
{
public:
	NetClient();
	virtual ~NetClient();

	void Connect(const std::string& domain_or_ip, const unsigned short port);
	void CloseSocket();

	void FrameMove();

private:
	void DoReadHeader();
	void DoReadBody(int bodySize);
	void DoSend(PacketBufferPtr buffer);

	void OnRecv(PacketBufferPtr buffer);
	void OnSend(PacketBufferPtr buffer);

	std::string DomainCheck(const std::string strDomain);

public:
	virtual void CallRecv(SessionID sessionId, PacketBufferPtr buffer) final;
	virtual void CallSend(SessionID sessionId, PacketBufferPtr buffer) final;

private:
	static boost::asio::io_service		m_ioService;
	static std::unique_ptr<NetThread>	m_thread;

	boost::asio::ip::tcp::socket	m_socket;
	boost::asio::ip::tcp::resolver	m_resolve;
	boost::asio::io_service::strand	m_strand;

	std::mutex	m_lock;

	PacketBufferPtr m_recvBuffer;
	std::deque<PacketBufferPtr> m_recvBufferQueue;
	std::deque<PacketBufferPtr> m_sendBufferQueue;
};

