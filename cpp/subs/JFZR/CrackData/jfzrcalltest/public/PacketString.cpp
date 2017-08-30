#include "StdAfx.h"

#ifdef _ENABLE_GAME_PACKET_TEST_ZHP_
/*
#include "../JFZRPacket.h"
#include "../SendPacket.h"
#include "../jfzrbase.h"
#include "../EnumPidWnd.h"
#include "../jfzrpacketstruct.h"
#include "../jfzrstruct.h"*/

#include "../PacketString.h"
#else
#include "PacketString.h"
#endif

PacketString::PacketString(const BYTE* lpPacket)
{
	m_lpPacketBegin = (BYTE*)lpPacket;
	m_lpPacketCurrent = (BYTE*)lpPacket;
	m_lpPacketEnd = (BYTE*)lpPacket+(DWORD)(*(WORD*)lpPacket);
}

PacketString::~PacketString(void)
{
}
