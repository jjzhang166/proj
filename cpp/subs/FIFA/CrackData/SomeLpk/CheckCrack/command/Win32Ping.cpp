// Win32Ping.cpp: implementation of the KWin32Ping  class.
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "win32ping.h"
 
 
#define GOTOFINISHED(nRet)\
    {\
        if (NULL != m_pVoid && NULL != m_pCallFun && -1 != m_nIndex)\
        {\
            m_pCallFun(m_pVoid, m_nIndex, nRet);\
        }\
        return nRet;\
    }\
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
KWin32Ping::KWin32Ping ()
        :m_pVoid(NULL),
        m_pCallFun(NULL),
        m_nIndex(-1),
        m_nNetStartUp(-1),
        m_hIcmp(NULL),
        m_pIcmpCreateFile(NULL),
        m_pIcmpCloseHandle(NULL),                   
        m_pIcmpSendEcho(NULL)
{
    memset(m_pszAddress, 0, sizeof(m_pszAddress));
 
    //初始化连接
    WSADATA wsaData;
    m_nNetStartUp = ::WSAStartup(0x0202, &wsaData);
 
    m_hIcmp = ::LoadLibraryA("ICMP.DLL");
 
    if (NULL != m_hIcmp)
    {
        m_pIcmpCreateFile = (pfnHV)::GetProcAddress(m_hIcmp,  "IcmpCreateFile");
        m_pIcmpCloseHandle = (pfnBH)::GetProcAddress(m_hIcmp, "IcmpCloseHandle");
        m_pIcmpSendEcho = (pfnDHDPWPipPDD)::GetProcAddress(m_hIcmp, "IcmpSendEcho");
    }
}

 
KWin32Ping ::~KWin32Ping()
{
     
    if (NULL != m_hIcmp)
    {
        ::FreeLibrary(m_hIcmp);
        m_hIcmp = NULL;
    }
     
    ::WSACleanup();
}
#define IP_REQUEST_TIMED_OUT 11010
//返回ping的时间单位是ms,如果过程中出现异常那么返回PINGTIMEOUT
//直接使用windows自带的dll,其实也可以自己写程序来实现ping
int KWin32Ping ::PingAddress(const char *pszAddr)
{
	memset(m_pszAddress, 0, sizeof(m_pszAddress));
	if (strlen(pszAddr) > sizeof(m_pszAddress) - 1)
		return false;

	memcpy(m_pszAddress, pszAddr, strlen(pszAddr));
    //地址不为空
    if (NULL == m_pszAddress || m_pszAddress[0] =='\0')
        return -1;
     
    if (0 != m_nNetStartUp)
        return -1;
         
    if (NULL == m_hIcmp)
        return -1;
 
    //将地址转为ip
    struct hostent* phe;
    if ((phe = gethostbyname(m_pszAddress)) == 0)
        return -1;
   
 
    if ((NULL == m_pIcmpCreateFile) ||
        (NULL == m_pIcmpCloseHandle) || 
        (NULL == m_pIcmpSendEcho)
        )
    {
        return -1;
    }
     
    // 打开ping服务
    HANDLE hIP = m_pIcmpCreateFile();
    if (hIP == INVALID_HANDLE_VALUE) {
        return -1;
    }
     
    // 构造ping数据包
    char acPingBuffer[64];
    memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
    PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc( GMEM_FIXED | GMEM_ZEROINIT,
        sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer)); 
    if (pIpe == 0) 
    {
        return -1;
    }
    pIpe->Data = acPingBuffer;
    pIpe->DataSize = sizeof(acPingBuffer);      
     
    // 发送ping数据包
    DWORD dwStatus = m_pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), 
        acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, 
        sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 1000);
    if (dwStatus != 0)
    {
        /*
        cout << "Addr: " <<
            int(LOBYTE(LOWORD(pIpe->Address))) << "." <<
            int(HIBYTE(LOWORD(pIpe->Address))) << "." <<
            int(LOBYTE(HIWORD(pIpe->Address))) << "." <<
            int(HIBYTE(HIWORD(pIpe->Address))) << ", " <<
            "RTT: " << int(pIpe->RoundTripTime) << "ms, " <<
            "TTL: " << int(pIpe->Options.Ttl) << endl;
 
        */
		return pIpe->RoundTripTime;
    }
    else
    {
		if( IP_REQUEST_TIMED_OUT != GetLastError() )
		{
			return -1;
		}
    }
     
    // 关闭,回收资源
    GlobalFree(pIpe);

    return PINGTIMEOUT;
}