#include "stdafx.h"
#include "NetPacket.h"

PacketManager::PacketManager(IPacketCallFunction* packetCallFunction)
	: m_packetCallFunction(packetCallFunction)
{
}

PacketManager::~PacketManager()
{
	m_recvList.clear();
	m_sendList.clear();
}

void PacketManager::Attch(IRecvPacket * observerFunction)
{
	m_recvList.push_back(observerFunction);
}

void PacketManager::Attch(ISendPacket * observerFunction)
{
	observerFunction->SetPacketManager(this);
	m_sendList.push_back(observerFunction);
}

void PacketManager::Detach(IRecvPacket * observerFunction)
{
	auto it = std::find(m_recvList.begin(), m_recvList.end(), observerFunction);
	if (it == m_recvList.end())
		return;

	m_recvList.erase(it);
}

void PacketManager::Detach(ISendPacket * observerFunction)
{
	auto it = std::find(m_sendList.begin(), m_sendList.end(), observerFunction);
	if (it == m_sendList.end())
		return;

	m_sendList.erase(it);
	observerFunction->SetPacketManager(NULL);
}

void PacketManager::SendNotify(SessionID sessionId, PacketBufferPtr buffer)
{
	m_packetCallFunction->CallSend(sessionId, buffer);
}

void PacketManager::RecvNotify(SessionID sessionId, PacketBufferPtr buffer)
{
	for (IRecvPacket* pObserverFunction : m_recvList)
	{
		pObserverFunction->CallPacketFunction(sessionId, buffer);
	}
}
