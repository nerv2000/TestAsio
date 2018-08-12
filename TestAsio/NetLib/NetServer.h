#pragma once
#include <mutex>
#include "boost/asio.hpp"

#include "ErrorFunction.h"
#include "NetPacket.h"
#include "NetServerAccept.h"
#include "NetThread.h"

class NetServerSession;
typedef std::shared_ptr<NetServerSession> NetServerSessionPtr;
class NetServer : public IDefaultCallBackFunction, public IPacketCallFunction, public PacketManager
{
private:
	typedef std::map<SessionID, NetServerSessionPtr> NetServerSessionMap;
	typedef NetServerSessionMap::value_type			 NetServerSessionMap_ValueType;

public:
	NetServer();
	virtual ~NetServer();

	void Start(const unsigned short port, const int threadCount);

	NetServerSessionPtr InsertSession(boost::asio::ip::tcp::socket& socket);
	void CloseSession(const SessionID sessionId);

	NetThread* GetThread() { return m_netThread.get(); }

private:
	SessionID GetNewSessionId() { return InterlockedIncrement(&m_currentSessionId); }

// IPacketCallFunction »ó¼Ó
public:
	virtual void CallRecv(SessionID sessionId, PacketBufferPtr buffer) final;
	virtual void CallSend(SessionID sessionId, PacketBufferPtr buffer) final;

private:
	std::mutex				   m_lock;
	boost::asio::io_service    m_ioService;

	std::unique_ptr<NetAccept> m_netAccept;
	std::unique_ptr<NetThread> m_netThread;

	volatile SessionID         m_currentSessionId;
	NetServerSessionMap        m_netServerSessionMap;
};

