// Win32Ping.h: interface for the KWin32Ping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN32PING_H__C183B112_6516_4FA5_B4CB_860ED5D98728__INCLUDED_)
#define AFX_WIN32PING_H__C183B112_6516_4FA5_B4CB_860ED5D98728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>
#pragma comment(lib, "ws2_32")

#ifndef BUFSIZ
#define BUFSIZ 256
#endif

typedef int    (*fnCallBackOutput)(LPVOID pVoid, unsigned int nParam, unsigned int nResult);

// typedef struct {
// 	unsigned char Ttl;                         // Time To Live
// 	unsigned char Tos;                         // Type Of Service
// 	unsigned char Flags;                       // IP header flags
// 	unsigned char OptionsSize;                 // Size in bytes of options data
// 	unsigned char *OptionsData;                // Pointer to options data
// } IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;

typedef struct {
	DWORD Address;                             // Replying address
	unsigned long  Status;                     // Reply status
	unsigned long  RoundTripTime;              // RTT in milliseconds
	unsigned short DataSize;                   // Echo data size
	unsigned short Reserved;                   // Reserved for system use
	void *Data;                                // Pointer to the echo data
	IP_OPTION_INFORMATION Options;             // Reply options
} IP_ECHO_REPLY, * PIP_ECHO_REPLY;

#define PINGTIMEOUT        1000
#define PINGERROR        -1

class KWin32Ping  
{
public:
	KWin32Ping();
	virtual ~KWin32Ping();

	int PingAddress(const char *pszAddr);   
private:
	// 定义函数三个指针类型
	//从ICMP.DLL中得到函数入口地址
	typedef HANDLE (WINAPI* pfnHV)(VOID);
	typedef BOOL (WINAPI* pfnBH)(HANDLE);
	typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
		PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); // evil, no?

	char m_pszAddress[BUFSIZ];
	LPVOID    m_pVoid;
	fnCallBackOutput m_pCallFun;
	int     m_nIndex;

	//网络相关
	int m_nNetStartUp;
	HINSTANCE m_hIcmp;

	pfnHV m_pIcmpCreateFile;
	pfnBH m_pIcmpCloseHandle;
	pfnDHDPWPipPDD m_pIcmpSendEcho;
};

#endif // !defined(AFX_WIN32PING_H__C183B112_6516_4FA5_B4CB_860ED5D98728__INCLUDED_)