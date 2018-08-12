#pragma once
#include <memory>
#include <string>
#include <vector>

class PacketBuffer;
typedef std::shared_ptr<PacketBuffer> PacketBufferPtr;

struct PacketHeader
{
	// 패킷 크기
	unsigned short size;
	// 패킷 번호
	unsigned short packetNo;

	PacketHeader()
		: size(0), packetNo(0)
	{
	}
};

class PacketStream
{
public:
	// 패킷 스트림 모드 - 한가지만 세팅해서 씁시다...
	enum PacketStreamMode
	{
		NONE,	// 없음
		READ,	// 읽기모드
		WRITE,	// 쓰기 모드
	};

public:
	PacketStream(PacketBufferPtr buffer, const PacketStreamMode streamMode);
	~PacketStream();

	// 패킷 헤더 읽어오기
	PacketHeader* GetPacketHead();

	unsigned short	GetWriteLength();
	PacketBufferPtr	GetBuffer();

public:
	void Write(const bool value);
	void Write(const signed char value);
	void Write(const unsigned char value);
	void Write(const short value);
	void Write(const unsigned short value);
	void Write(const int value);
	void Write(const unsigned int  value);
	void Write(const long value);
	void Write(const unsigned long value);
	void Write(const long long value);
	void Write(const unsigned long long value);
	void Write(const float value);
	void Write(const double value);
	void Write(const char* value, const size_t size);
	void Write(const std::string& value);
	template<typename T>
	void Write(const std::vector<T>& value);

	void Read(bool& value);
	void Read(signed char& value);
	void Read(unsigned char& value);
	void Read(short& value);
	void Read(unsigned short& value);
	void Read(int& value);
	void Read(unsigned int& value);
	void Read(long& value);
	void Read(unsigned long& value);
	void Read(long long& value);
	void Read(unsigned long long& value);
	void Read(float& value);
	void Read(double& value);
	void Read(char* value, const size_t size);
	void Read(std::string& value);
	template<typename T>
	void Read(std::vector<T>& value);

public:
	PacketStream& operator<<(const bool& value)               { Write(value); return *this; }
	PacketStream& operator<<(const signed char& value)        { Write(value); return *this; }
	PacketStream& operator<<(const unsigned char& value)      { Write(value); return *this; }
	PacketStream& operator<<(const short& value)              { Write(value); return *this; }
	PacketStream& operator<<(const unsigned short& value)     { Write(value); return *this; }
	PacketStream& operator<<(const int& value)                { Write(value); return *this; }
	PacketStream& operator<<(const unsigned int& value)       { Write(value); return *this; }
	PacketStream& operator<<(const long& value)               { Write(value); return *this; }
	PacketStream& operator<<(const unsigned long& value)      { Write(value); return *this; }
	PacketStream& operator<<(const long long& value)          { Write(value); return *this; }
	PacketStream& operator<<(const unsigned long long& value) { Write(value); return *this; }
	PacketStream& operator<<(const float& value)              { Write(value); return *this; }
	PacketStream& operator<<(const double& value)             { Write(value); return *this; }
	PacketStream& operator<<(const std::string& value)        { Write(value); return *this; }

	PacketStream& operator>>(bool& value)                     { Read(value); return *this; }
	PacketStream& operator>>(signed char& value)              { Read(value); return *this; }
	PacketStream& operator>>(unsigned char& value)            { Read(value); return *this; }
	PacketStream& operator>>(short& value)                    { Read(value); return *this; }
	PacketStream& operator>>(unsigned short& value)           { Read(value); return *this; }
	PacketStream& operator>>(int& value)                      { Read(value); return *this; }
	PacketStream& operator>>(unsigned int& value)             { Read(value); return *this; }
	PacketStream& operator>>(long& value)                     { Read(value); return *this; }
	PacketStream& operator>>(unsigned long& value)            { Read(value); return *this; }
	PacketStream& operator>>(long long& value)                { Read(value); return *this; }
	PacketStream& operator>>(unsigned long long& value)       { Read(value); return *this; }
	PacketStream& operator>>(float& value)                    { Read(value); return *this; }
	PacketStream& operator>>(double& value)                   { Read(value); return *this; }
	PacketStream& operator>>(std::string& value)              { Read(value); return *this; }

private:
	// 버퍼 클래스
	PacketBufferPtr m_buffer;
	// 상태 세팅
	PacketStreamMode m_streamMode;
};

#include "PacketStream.inl"

