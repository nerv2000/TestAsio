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

	// ������ ����
	char* GetBegin() { return m_buffer; }
	// ������ ��
	char* GetEnd() { return m_buffer + m_buffSize; }
	// offset ��ġ
	char* GetPos() { return m_offset; }
	// ������� ���� ũ��
	size_t GetCurrentSize() { return static_cast<int>(m_offset - m_buffer); }
	// ���� �ִ� ũ��
	size_t GetSizeMax() { return m_buffSize; }
	// ���� ��ġ ����
	void IncreaseOffset(const size_t increaseSize) { m_offset += increaseSize; }
	// ���� ��ġ �ʱ�ȭ
	void ResetOffset() { m_offset = m_buffer; }
	// ���� ���� üũ
	bool IsOverflow(const size_t checkSize) { return (GetSizeMax() < (GetCurrentSize() + checkSize)) ? false : true; }

private:
	// ����
	char*  m_buffer;
	// ���� offset
	char*  m_offset;
	// ���� ũ��
	size_t m_buffSize;
};
typedef std::shared_ptr<PacketBuffer> PacketBufferPtr;

