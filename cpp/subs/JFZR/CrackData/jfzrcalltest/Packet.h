#pragma once
#include "packetstring.h"

class Packet : public PacketString
{
public:
	Packet(void);
	Packet(const BYTE* lpPacket);
	~Packet(void);
public:
	DWORD	GetPacketLen()
	{
		WORD wLen = m_lpPacketCurrent - m_lpPacketBegin;
		*(WORD*)m_lpPacketBegin = wLen;
		return wLen;
	}
};
