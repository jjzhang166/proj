#pragma once
#include <assert.h>
#include <string>

class PacketString
{
public:
	BYTE*	m_lpPacketBegin;
	BYTE*	m_lpPacketCurrent;
	BYTE*	m_lpPacketEnd;

	wchar_t	m_wszPrint[0x1024];
	char	m_szPrint[0x1024];
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
			//assert(false);
		}
		dwTemp = *(LONGLONG*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(LONGLONG);

		swprintf_s(m_wszPrint,L"[JFZR] >> %016I64X",dwTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (LONGLONG dwTemp)
	{
		*(LONGLONG*)m_lpPacketCurrent = dwTemp;
		m_lpPacketCurrent += sizeof(LONGLONG);

		swprintf_s(m_wszPrint,L"[JFZR] << %016I64X",dwTemp);
		OutputDebugStringW(m_wszPrint);
	}

	// DWORD
	void operator >> (DWORD& dwTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		dwTemp = *(DWORD*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(DWORD);

		swprintf_s(m_wszPrint,L"[JFZR] >> %08X",dwTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator >> (int& nTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		nTemp = *(int*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(int);

		swprintf_s(m_wszPrint,L"[JFZR] >> %p",nTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (DWORD dwTemp)
	{
		*(DWORD*)m_lpPacketCurrent = dwTemp;
		m_lpPacketCurrent += sizeof(DWORD);

		swprintf_s(m_wszPrint,L"[JFZR] << %08X",dwTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (int nTemp)
	{
		*(int*)m_lpPacketCurrent = nTemp;
		m_lpPacketCurrent += sizeof(int);

		swprintf_s(m_wszPrint,L"[JFZR] << %p",nTemp);
		OutputDebugStringW(m_wszPrint);
	}

	// WORD
	void operator >> (WORD& wTemp)
	{	
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		wTemp = *(WORD*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(WORD);

		swprintf_s(m_wszPrint,L"[JFZR] >> %04X",wTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (WORD wTemp)
	{
		*(WORD*)m_lpPacketCurrent = wTemp;
		m_lpPacketCurrent += sizeof(WORD);

		swprintf_s(m_wszPrint,L"[JFZR] << %04X",wTemp);
		OutputDebugStringW(m_wszPrint);
	}

	// BYTE
	void operator >> (BYTE& bTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		bTemp = *(BYTE*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(BYTE);

		swprintf_s(m_wszPrint,L"[JFZR] >> %02X",bTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (BYTE bTemp)
	{
		*(BYTE*)m_lpPacketCurrent = bTemp;
		m_lpPacketCurrent += sizeof(BYTE);
		swprintf_s(m_wszPrint,L"[JFZR] << %02X",bTemp);
		OutputDebugStringW(m_wszPrint);
	}

	// FLOAT
	void operator >> (FLOAT& fTemp)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		fTemp = *(FLOAT*)m_lpPacketCurrent;
		m_lpPacketCurrent += sizeof(FLOAT);

		swprintf_s(m_wszPrint,L"[JFZR] >> %f",fTemp);
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (FLOAT fTemp)
	{
		*(FLOAT*)m_lpPacketCurrent = fTemp;
		m_lpPacketCurrent += sizeof(FLOAT);
		swprintf_s(m_wszPrint,L"[JFZR] << %f",fTemp);
		OutputDebugStringW(m_wszPrint);
	}

	// string
	void operator >> (std::wstring& str)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		str = (wchar_t*)m_lpPacketCurrent;
		m_lpPacketCurrent += (str.size()+1)*2;

		swprintf_s(m_wszPrint,L"[JFZR] >> %s",str.c_str());
		OutputDebugStringW(m_wszPrint);
	}

	void operator >> (std::string& str)
	{
		if( m_lpPacketCurrent >= m_lpPacketEnd )
		{
			//assert(false);
		}
		str = (char*)m_lpPacketCurrent;
		m_lpPacketCurrent += (str.size()+1);

		sprintf_s(m_szPrint,"[JFZR] >> %s",str.c_str());
		OutputDebugStringA(m_szPrint);
	}

	void operator << (std::wstring str)
	{
		WORD wStrLen = wcslen(str.c_str())+1;
		*(WORD*)m_lpPacketCurrent = wStrLen;
		m_lpPacketCurrent += sizeof(WORD);
		memcpy(m_lpPacketCurrent,str.c_str(),wStrLen*2);
		m_lpPacketCurrent += wStrLen*2;

		swprintf_s(m_wszPrint,L"[JFZR] << %s",str.c_str());
		OutputDebugStringW(m_wszPrint);
	}

	void operator << (std::string str)
	{
		WORD wStrLen = strlen(str.c_str())+1;
		*(WORD*)m_lpPacketCurrent = wStrLen;
		m_lpPacketCurrent += sizeof(WORD);
		memcpy(m_lpPacketCurrent,str.c_str(),wStrLen);
		m_lpPacketCurrent += wStrLen;

		sprintf_s(m_szPrint,"[JFZR] << %s",str.c_str());
		OutputDebugStringA(m_szPrint);
	}
};
