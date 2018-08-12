#include "stdafx.h"
#include "NetClient.h"
#include "PacketStream.h"

boost::asio::io_service  NetClient::m_ioService;
std::unique_ptr<NetThread> NetClient::m_thread = NULL;

NetClient::NetClient()
	: PacketManager(this)
	, m_socket(m_ioService)
	, m_resolve(m_ioService)
	, m_strand(m_socket.get_io_service())
{
	if (NULL == m_thread)
	{
		m_thread = std::make_unique<NetThread>(m_ioService, 1);
	}
}

NetClient::~NetClient()
{
}

void NetClient::Connect(const std::string & domain_or_ip, const unsigned short port)
{
	std::string ip = DomainCheck(domain_or_ip);

	auto endPoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
	m_socket.async_connect(endPoint
		, m_strand.wrap([this](boost::system::error_code ec)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				OnError(errorInfo);

				CloseSocket();
				return;
			}

			OnSessionConnect(1);

			DoReadHeader();
		}
	));
}

void NetClient::CloseSocket()
{
	if (m_socket.is_open())
	{
		OnSessionClose(1);

		m_socket.close();
	}
}

void NetClient::FrameMove()
{
	while (!m_recvBufferQueue.empty())
	{
		std::lock_guard<std::mutex> lock(m_lock);

		OnRecv(m_recvBufferQueue.front());
		m_recvBufferQueue.pop_front();
	}
}

void NetClient::DoReadHeader()
{
	if (NULL == m_recvBuffer)
		m_recvBuffer = PacketBufferPtr(new PacketBuffer);

	boost::asio::async_read(m_socket, boost::asio::buffer(m_recvBuffer->GetPos(), sizeof(PacketHeader))
		, m_strand.wrap([this](boost::system::error_code ec, std::size_t length)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				errorInfo.message = ec.message();
				OnError(errorInfo);

				CloseSocket();
				return;
			}
			m_recvBuffer->IncreaseOffset(length);

			PacketHeader* pHead = reinterpret_cast<PacketHeader*>(m_recvBuffer->GetBegin());
			DoReadBody(pHead->size);
		}
	));
}

void NetClient::DoReadBody(int bodySize)
{
	m_socket.async_read_some(boost::asio::buffer(m_recvBuffer->GetPos(), bodySize),
		m_strand.wrap([this](boost::system::error_code ec, std::size_t length)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();
				OnError(errorInfo);

				CloseSocket();
				return;
			}

			// recv 받은 만큼 버퍼 이동
			m_recvBuffer->IncreaseOffset(length);

			{
				std::lock_guard<std::mutex> lock(m_lock);
				m_recvBufferQueue.push_back(m_recvBuffer);
			}

			m_recvBuffer.reset();

			DoReadHeader();
		}
	));
}

void NetClient::DoSend(PacketBufferPtr buffer)
{
	boost::asio::async_write(m_socket,
		boost::asio::buffer(buffer->GetBegin(), buffer->GetCurrentSize())
		, m_strand.wrap([this](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (0 != ec.value())
			{
				ErrorInfo errorInfo;
				errorInfo.socketError = ec.value();
				errorInfo.message = ec.message();

				OnError(errorInfo);

				CloseSocket();
				return;
			}

			if(!m_sendBufferQueue.empty())
				m_sendBufferQueue.pop_front();
		}
	));
}

void NetClient::OnRecv(PacketBufferPtr buffer)
{
	CallRecv(1, buffer);
}

void NetClient::OnSend(PacketBufferPtr buffer)
{
	m_sendBufferQueue.push_back(buffer);
	DoSend(buffer);
}

std::string NetClient::DomainCheck(const std::string strDomain)
{
	boost::asio::ip::tcp::resolver::query query(strDomain, "");
	boost::asio::ip::tcp::resolver::iterator it = m_resolve.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end;

	std::string temp;
	while (it != end)
	{
		boost::asio::ip::tcp::endpoint cur = *it++;
		temp = cur.address().to_v4().to_string();
	}

	return temp;
}

void NetClient::CallRecv(SessionID sessionId, PacketBufferPtr buffer)
{
	RecvNotify(sessionId, buffer);
}

void NetClient::CallSend(SessionID sessionId, PacketBufferPtr buffer)
{
	OnSend(buffer);
}
