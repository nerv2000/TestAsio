#pragma once
#include "boost/asio.hpp"

class NetServer;
class NetAccept
{
public:
	NetAccept(boost::asio::io_service& io_service, const uint16_t port, NetServer* pServer);
	~NetAccept();

	void Start();

private:
	void DoAccept();

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket   m_acceptSocket;

	NetServer*	m_pServer;
};