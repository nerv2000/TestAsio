#pragma once
#include "PacketBuffer.h"
#include "PacketStream.h"
#include <assert.h>

template<typename T>
inline void PacketStream::Write(const std::vector<T>& value)
{
	assert(PacketStreamMode::WRITE == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned short) + (sizeof(T) * value.size())))
		throw std::invalid_argument("buffer overflow");

	*reinterpret_cast<unsigned short*>(m_buffer->GetPos()) = static_cast<unsigned short>(value.size());
	m_buffer->IncreaseOffset(sizeof(unsigned short));

	for (auto it = value.begin(); it != value.end(); ++it)
	{
		*this >> (*it);
	}
}

template<typename T>
inline void PacketStream::Read(std::vector<T>& value)
{
	assert(PacketStreamMode::READ == m_streamMode);

	if (!m_buffer->IsOverflow(sizeof(unsigned short)))
		throw std::invalid_argument("buffer overflow");

	uint16_t size = *reinterpret_cast<unsigned short*>(m_buffer->GetPos());
	m_buffer->IncreaseOffset(sizeof(unsigned short));

	value.reserve(size);

	for (uint16_t i = 0; i < size; ++i)
	{
		T data;
		*this << data;
		value.push_back(data);
	}

	return true;
}

template<typename T>
PacketStream& operator<<(PacketStream& a, const std::vector<T>& value)
{
	a.Write(value);
	return a;
}

template<typename T>
PacketStream& operator>>(PacketStream& a, std::vector<T>& value)
{
	a.Read(value);
	return a;
}
