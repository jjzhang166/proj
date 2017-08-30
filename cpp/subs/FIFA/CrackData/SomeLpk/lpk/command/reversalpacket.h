#pragma once

#include "PacketString.h"
#include "packetstruct.h"

class reversalpacket : public PacketString
{
public:
	reversalpacket(void);
	reversalpacket(const BYTE* lpPacket);
	~reversalpacket(void);

};

class Packet  
{
public:
	Packet();
	~Packet();
public:
	virtual int connect(char* pszPacketBuffer,char* pszKey,WORD wUrlType) = 0;
	virtual int key(char* pszPacketBuffer,char* pszKey,WORD wUrlType) = 0;
	virtual int unconnect(char* pszPacketBuffer) = 0;
};

//extern Packet*			pthePacket;

class JFZRPacket : public Packet
{
public:
	JFZRPacket();
	~JFZRPacket();
public:
	virtual int connect(char* pszPacketBuffer,char* pszKey,WORD wUrlType);
	virtual int key(char* pszPacketBuffer,char* pszKey,WORD wUrlType);
	virtual int unconnect(char* pszPacketBuffer);
};

class FIFAPacket : public Packet
{
public:
	FIFAPacket();
	~FIFAPacket();
public:
	virtual int connect(char* pszPacketBuffer,char* pszKey,WORD wUrlType);
	virtual int key(char* pszPacketBuffer,char* pszKey,WORD wUrlType);
	virtual int unconnect(char* pszPacketBuffer);
};

extern FIFAPacket		theFIFAPacket;