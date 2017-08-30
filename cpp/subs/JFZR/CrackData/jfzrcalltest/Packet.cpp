#include "StdAfx.h"
#include "Packet.h"


Packet::Packet() : PacketString((const BYTE *)"")
{

}

Packet::Packet(const BYTE* lpPacket) : PacketString(lpPacket)
{

}

Packet::~Packet(void)
{
}
