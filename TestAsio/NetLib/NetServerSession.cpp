#include "stdafx.h"
#include "NetServerSession.h"
#include "NetServer.h"
#include "PacketStream.h"

NetServerSession::NetServerSession(const SessionID sessionId, boost::asio::ip::tcp::socket socket, NetServer* pServer)
	: m_sessionId(sessionId)
	, m_socket(std::move(socket))
	, m_pServer(pServer)
	, m_strand(socket.get_io_service())
{
	m_recvBuffer.reset();
}

NetServerSession::~NetServerSession()
{
	m_recvBuffer.reset();
	m_recvBufferQueue.clear();
	m_sendBufferQueue.clear();
}

void NetServerSession::Start()
{
	DoReadHeader();
}

void NetServerSession::Close()
{
	if (false == m_socket.is_open())
		return;

	m_socket.close();
}

void NetServerSession::DoReadHeader()
{
	if (NULL == m_recvBuffer)
		m_recvBuffer = PacketBufferPtr(new PacketBuffer());

	auto self(shared_from_this());
	boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuffer->GetPos(), sizeof(PacketHeader)),
		m_strand.wrap([this, self](boost::system::error_code ec, std::size_t length)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				m_pServer->OnError(errorInfo);
				m_pServer->CloseSession(m_sessionId);
				return;
			}

			// recv 받은 만큼 버퍼 이동
			m_recvBuffer->IncreaseOffset(length);

			PacketHeader* pHead = reinterpret_cast<PacketHeader*>(m_recvBuffer->GetBegin());
			DoReadBody(pHead->size);
		}
	));
}

void NetServerSession::DoReadBody(int bodySize)
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_recvBuffer->GetPos(), bodySize),
		m_strand.wrap([this, self](boost::system::error_code ec, std::size_t length)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				m_pServer->OnError(errorInfo);
				m_pServer->CloseSession(m_sessionId);
				return;
			}

			// recv 받은 만큼 버퍼 이동
			m_recvBuffer->IncreaseOffset(length);
			m_recvBufferQueue.push_back(m_recvBuffer);
			m_pServer->GetThread()->Post(m_strand.wrap(boost::bind(&NetServerSession::OnRecv, self, m_recvBuffer)));

			m_recvBuffer.reset();

			DoReadHeader();
		}
	));
}

void NetServerSession::DoSend(PacketBufferPtr buffer)
{
	auto self(shared_from_this());
	boost::asio::async_write(m_socket,
		boost::asio::buffer(buffer->GetBegin(), buffer->GetCurrentSize()),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				m_pServer->OnError(errorInfo);
				return;
			}

			m_sendBufferQueue.pop_front();
		}
	);
}

void NetServerSession::OnRecv(PacketBufferPtr buffer)
{
	m_pServer->CallRecv(m_sessionId, buffer);
	m_recvBufferQueue.pop_front();
}

void NetServerSession::OnSend(PacketBufferPtr buffer)
{
	m_sendBufferQueue.push_back(buffer);

	auto self(shared_from_this());
	m_pServer->GetThread()->Post(m_strand.wrap(boost::bind(&NetServerSession::DoSend, self, buffer)));
}
