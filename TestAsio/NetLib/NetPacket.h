#pragma once
#include "NetCommon.h"
#include "PacketBuffer.h"
#include <list>

__interface IPacketCallFunction
{
	virtual void CallRecv(SessionID sessionId, PacketBufferPtr buffer) = 0;
	virtual void CallSend(SessionID sessionId, PacketBufferPtr buffer) = 0;
};

class IRecvPacket
{
public:
	virtual void CallPacketFunction(SessionID sessionId, PacketBufferPtr buffer) = 0;
};

class PacketManager;
class ISendPacket
{
public:
	ISendPacket() : m_pPacketManager(NULL) {}
public:
	void SetPacketManager(PacketManager* pPacketManager) { m_pPacketManager = pPacketManager; }

protected:
	PacketManager * m_pPacketManager;
};

class PacketManager
{
public:
	PacketManager(IPacketCallFunction* packetServerCallFunction);
	virtual ~PacketManager();

	void Attch(IRecvPacket* observerFunction);
	void Attch(ISendPacket* observerFunction);

	void Detach(IRecvPacket* observerFunction);
	void Detach(ISendPacket* observerFunction);

	void SendNotify(SessionID sessionId, PacketBufferPtr buffer);
	void RecvNotify(SessionID sessionId, PacketBufferPtr buffer);

private:
	IPacketCallFunction*    m_packetCallFunction;

	std::list<IRecvPacket*> m_recvList;
	std::list<ISendPacket*> m_sendList;
};