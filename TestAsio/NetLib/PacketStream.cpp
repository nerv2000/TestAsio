#include "stdafx.h"
#include "PacketStream.h"
#include "PacketBuffer.h"

#include <assert.h>
#include <stdexcept>

PacketStream::PacketStream(PacketBufferPtr buffer, const PacketStreamMode streamMode)
	: m_buffer(buffer), m_streamMode(streamMode)
{
	// 버퍼 받으면 헤더 만큼 offset 이동
	buffer->ResetOffset();
	m_buffer->IncreaseOffset(sizeof(PacketHeader));
}

PacketStream::~PacketStream()
{
	m_buffer.reset();
	m_streamMode = PacketStreamMode::NONE;
}

PacketHeader* PacketStream::GetPacketHead()
{
	return reinterpret_cast<PacketHeader*>(m_buffer->GetBegin());
}

unsigned short PacketStream::GetWriteLength()
{
	return static_cast<unsigned short>(m_buffer->GetCurrentSize());
}

PacketBufferPtr PacketStream::GetBuffer()
{
	return m_buffer;
}

void PacketStream::Write(const bool value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(bool)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<bool*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(bool));
}

void PacketStream::Write(const signed char value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(signed char)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<signed char*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(signed char));
}

void PacketStream::Write(const unsigned char value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned char)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<unsigned char*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(unsigned char));
}

void PacketStream::Write(const short value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(short)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<short*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(short));
}

void PacketStream::Write(const unsigned short value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned short)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<unsigned short*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(unsigned short));
}

void PacketStream::Write(const int value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(int)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<int*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(int));
}

void PacketStream::Write(const unsigned int value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned int)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<unsigned int*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(unsigned int));
}

void PacketStream::Write(const long value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(long)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<long*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(long));
}

void PacketStream::Write(const unsigned long value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned long)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<unsigned long*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(unsigned long));
}

void PacketStream::Write(const long long value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(long long)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<long long*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(long long));
}

void PacketStream::Write(const unsigned long long value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(uint64_t)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<uint64_t*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(uint64_t));
}

void PacketStream::Write(const float value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(float)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<float*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(float));
}

void PacketStream::Write(const double value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(double)))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<double*>(m_buffer->GetPos()) = value;
	m_buffer->IncreaseOffset(sizeof(double));
}

void PacketStream::Write(const char * value, const size_t size)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(size))
		throw std::invalid_argument("buffer overflow");

	memcpy(m_buffer->GetPos(), value, size);
	m_buffer->IncreaseOffset(size);
}

void PacketStream::Write(const std::string & value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(uint16_t) + value.size()))
		throw std::invalid_argument("buffer overflow");

	// 문자열 길이 쓰기
	*reinterpret_cast<uint16_t*>(m_buffer->GetPos()) = static_cast<uint16_t>(value.size());
	m_buffer->IncreaseOffset(sizeof(uint16_t));

	// 문자열 쓰기
	return Write(value.c_str(), value.size());
}

void PacketStream::Read(bool & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(bool)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<bool*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(bool));
}

void PacketStream::Read(signed char & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(signed char)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<signed char*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(signed char));
}

void PacketStream::Read(unsigned char & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned char)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<unsigned char*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned char));
}

void PacketStream::Read(short & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(short)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<short*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(short));
}

void PacketStream::Read(unsigned short & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned short)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<unsigned short*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned short));
}

void PacketStream::Read(int & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(int)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<int*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(int));
}

void PacketStream::Read(unsigned int & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned int)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<unsigned int*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned int));
}

void PacketStream::Read(long & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(long)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<long*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(long));
}

void PacketStream::Read(unsigned long & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned long)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<unsigned long*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned long));
}

void PacketStream::Read(long long & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(long long)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<long long*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(long long));
}

void PacketStream::Read(unsigned long long & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned long long)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<unsigned long long*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned long long));
}

void PacketStream::Read(float & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(float)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<float*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(float));
}

void PacketStream::Read(double & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(double)))
		throw std::invalid_argument("buffer overflow");

	value = *reinterpret_cast<double*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(double));
}

void PacketStream::Read(char * value, const size_t size)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(size))
		throw std::invalid_argument("buffer overflow");

	memcpy(value, m_buffer->GetPos(), size);
	m_buffer->IncreaseOffset(size);
}

void PacketStream::Read(std::string & value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(uint16_t)))
		throw std::invalid_argument("buffer overflow");

	uint16_t size = *reinterpret_cast<uint16_t*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(uint16_t));

	if (!m_buffer->IsOverflow(size))
		throw std::invalid_argument("buffer overflow");

	value.assign(m_buffer->GetPos(), size);
	m_buffer->IncreaseOffset(size);
}
