#pragma once
#include <memory>

class PacketBuffer
{
public:
	PacketBuffer(const size_t bufSize = 8192)
	{
		m_buffer = new char[bufSize]();
		m_offset = m_buffer;
		m_buffSize = bufSize;
	}
	~PacketBuffer()
	{
		if (nullptr == m_buffer)
			return;

		delete[] m_buffer;
		m_buffer = nullptr;
		m_offset = nullptr;
		m_buffSize = 0;
	}

	// 버퍼의 시작
	char* GetBegin() { return m_buffer; }
	// 버퍼의 끝
	char* GetEnd() { return m_buffer + m_buffSize; }
	// offset 위치
	char* GetPos() { return m_offset; }
	// 사용중인 버퍼 크기
	size_t GetCurrentSize() { return static_cast<int>(m_offset - m_buffer); }
	// 버퍼 최대 크기
	size_t GetSizeMax() { return m_buffSize; }
	// 버퍼 위치 증가
	void IncreaseOffset(const size_t increaseSize) { m_offset += increaseSize; }
	// 버퍼 위치 초기화
	void ResetOffset() { m_offset = m_buffer; }
	// 버퍼 오버 체크
	bool IsOverflow(const size_t checkSize) { return (GetSizeMax() < (GetCurrentSize() + checkSize)) ? false : true; }

private:
	// 버퍼
	char*  m_buffer;
	// 버퍼 offset
	char*  m_offset;
	// 버퍼 크기
	size_t m_buffSize;
};
typedef std::shared_ptr<PacketBuffer> PacketBufferPtr;

