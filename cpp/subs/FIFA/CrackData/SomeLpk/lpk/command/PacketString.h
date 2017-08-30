#pragma once
#include "../lpk/lpkstring.h"
#include <assert.h>
#include <string>
#include <CD_CommonFiles/CD_CmnDefines.h>


class PacketString
{
public:
	BYTE*	m_lpPacketBegin;
	BYTE*	m_lpPacketCurrent;
	BYTE*	m_lpPacketEnd;

	wchar_t	m_wszPrint[REVERSAL_SOCK_BUFFER_SIZE];
	char	m_szPrint[REVERSAL_SOCK_BUFFER_SIZE];
public:
	PacketString(const BYTE* lpPacket);
	~PacketString(void);

	void operator = (const BYTE* lpPacket)
	{
		m_lpPacketBegin = (BYTE*)lpPacket;
		m_lpPacketCurrent = (BYTE*)lpPacket;
		m_lpPacketEnd = (BYTE*)lpPacket+(DWORD)(*(WORD*)lpPacket);
	}

	// LONGLONG
	void operator >> (LONGLONG& dwTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		dwTemp = *(LONGLONG*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(LONGLONG);

// 		swprintf_s(m_wszPrint,L"[LOG] %016I64X",dwTemp);
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator << (LONGLONG dwTemp)
	{
		*(LONGLONG*)m_lpPacketCurrent = dwTemp;
		m_lpPacketCurrent += sizeof(LONGLONG);
	}

	// DWORD
	void operator >> (DWORD& dwTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		dwTemp = *(DWORD*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(DWORD);

// 		swprintf_s(m_wszPrint,L"[LOG] %08X",dwTemp);
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator << (DWORD dwTemp)
	{
		*(DWORD*)m_lpPacketCurrent = dwTemp;
		m_lpPacketCurrent += sizeof(DWORD);
	}

	// WORD
	void operator >> (WORD& wTemp)
	{	
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		wTemp = *(WORD*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(WORD);

// 		swprintf_s(m_wszPrint,L"[LOG] %04X",wTemp);
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator << (WORD wTemp)
	{
		*(WORD*)m_lpPacketCurrent = wTemp;
		m_lpPacketCurrent += sizeof(WORD);
	}

	// BYTE
	void operator >> (BYTE& bTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		bTemp = *(BYTE*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(BYTE);

// 		swprintf_s(m_wszPrint,L"[LOG] %02X",bTemp);
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator << (BYTE bTemp)
	{
		*(BYTE*)m_lpPacketCurrent = bTemp;
		m_lpPacketCurrent += sizeof(BYTE);
	}

	// FLOAT
	void operator >> (FLOAT& fTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		fTemp = *(FLOAT*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(FLOAT);

// 		swprintf_s(m_wszPrint,L"[LOG] %f",fTemp);
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator << (FLOAT fTemp)
	{
		*(FLOAT*)m_lpPacketCurrent = fTemp;
		m_lpPacketCurrent += sizeof(FLOAT);
	}

	// string
	void operator >> (std::wstring& str)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		str = (wchar_t*)m_lpPacketCurrent;
		m_lpPacketCurrent += (str.size()+1)*2;

// 		swprintf_s(m_wszPrint,L"[LOG] %s",str.c_str());
// 		OutputDebugStringW(m_wszPrint);
	}

	void operator >> (lpkstring& str)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			assert(false);
		}
		str = (char*)m_lpPacketCurrent;
		m_lpPacketCurrent += (str.size()+1);

// 		wsprintf(m_szPrint,"[LOG] %s",str.c_str());
// 		OutputDebugStringA(m_szPrint);
	}

	void operator << (std::wstring str)
	{
		WORD wStrLen = wcslen(str.c_str())+1;
		*(WORD*)m_lpPacketCurrent = wStrLen;
		m_lpPacketCurrent += sizeof(WORD);
		memcpy(m_lpPacketCurrent,str.c_str(),wStrLen*2);
		m_lpPacketCurrent += wStrLen*2;
	}

	void operator << (lpkstring str)
	{
		WORD wStrLen = strlen(str.c_str())+1;
		*(WORD*)m_lpPacketCurrent = wStrLen;
		m_lpPacketCurrent += sizeof(WORD);
		memcpy(m_lpPacketCurrent,str.c_str(),wStrLen);
		m_lpPacketCurrent += wStrLen;
	}
};
