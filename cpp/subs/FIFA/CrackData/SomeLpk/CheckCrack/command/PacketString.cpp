#include "StdAfx.h"
#include "PacketString.h"

PacketString::PacketString(const BYTE* lpPacket)
{
	m_lpPacketBegin = (BYTE*)lpPacket;
	m_lpPacketCurrent = (BYTE*)lpPacket;
	m_lpPacketEnd = (BYTE*)lpPacket+(DWORD)(*(WORD*)lpPacket);
}

PacketString::~PacketString(void)
{
}
