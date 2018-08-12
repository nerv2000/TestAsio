#pragma once
#include "boost/thread.hpp"
#include "boost/asio.hpp"

class NetThread
{
public:
	NetThread(boost::asio::io_service& io_service, const int threadCount)
		: m_ioService(io_service)
	{
		m_work.reset(new boost::asio::io_service::work(m_ioService));

		for (int i = 0; i < threadCount; ++i)
		{
			m_threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &m_ioService));
		}
	}

	~NetThread()
	{
		m_work.reset();
		m_threadGroup.join_all();
	}

	template <class F>
	void Post(F f) { m_ioService.post(f); }

	boost::asio::io_service& GetIoService() { return m_ioService; }

private:
	boost::asio::io_service&                       m_ioService;
	boost::thread_group                            m_threadGroup;
	std::shared_ptr<boost::asio::io_service::work> m_work;
};