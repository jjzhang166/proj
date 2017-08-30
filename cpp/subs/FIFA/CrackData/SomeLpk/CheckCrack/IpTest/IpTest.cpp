// IpTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IpTest.h"
#include "../command/CRC.h"
#include "../command/CheckIp.h"
#include "CheckHook.h"
#include <time.h>
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

// int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
// {
// 	int nRetCode = 0;
// 
// 	// 初始化 MFC 并在失败时显示错误
// 	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
// 	{
// 		// TODO: 更改错误代码以符合您的需要
// 		_tprintf(_T("错误: MFC 初始化失败\n"));
// 		nRetCode = 1;
// 	}
// 	else
// 	{
// 		// TODO: 在此处为应用程序的行为编写代码。
// 	}
// 
// 	return nRetCode;
// }

#include <stdio.h>     
#include <windows.h>     
#include <Iphlpapi.h>     
#include <tlhelp32.h>     

#pragma comment(lib, "Iphlpapi.lib")     
#pragma comment(lib, "WS2_32.lib")     




typedef struct  
{     
	DWORD   dwState;          // 连接状态     
	DWORD   dwLocalAddr;      // 本地地址     
	DWORD   dwLocalPort;      // 本地端口     
	DWORD   dwRemoteAddr;     // 远程地址     
	DWORD   dwRemotePort;     // 远程端口     
	DWORD   dwProcessId;      // 进程ID号     
} MIB_TCPEXROW, *PMIB_TCPEXROW;     

typedef struct  
{     
	DWORD           dwNumEntries;     
	MIB_TCPEXROW    table[ANY_SIZE];     
} MIB_TCPEXTABLE, *PMIB_TCPEXTABLE;     

typedef struct  
{     
	DWORD   dwLocalAddr;      // 本地地址     
	DWORD   dwLocalPort;      // 本地端口     
	DWORD   dwProcessId;      // 进程ID号     
} MIB_UDPEXROW, *PMIB_UDPEXROW;     

typedef struct  
{     
	DWORD           dwNumEntries;     
	MIB_UDPEXROW    table[ANY_SIZE];     
} MIB_UDPEXTABLE, *PMIB_UDPEXTABLE;     


// 扩展函数原型     
typedef DWORD (WINAPI *PFNAllocateAndGetTcpExTableFromStack)(     
	PMIB_TCPEXTABLE *pTcpTable,      
	BOOL bOrder,                  
	HANDLE heap,     
	DWORD zero,     
	DWORD flags     
	);     

typedef DWORD (WINAPI *PFNAllocateAndGetUdpExTableFromStack)(     
	PMIB_UDPEXTABLE *pUdpTable,       
	BOOL bOrder,                   
	HANDLE heap,     
	DWORD zero,     
	DWORD flags     
	);     


TCHAR* ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, TCHAR* ProcessName);



BOOL GetInternetTime(CTime* pTime, CString strServer)
{
	CSocket sockClient;
	sockClient.Create();			//创建socket

	sockClient.Connect((LPCTSTR)strServer, 37); // strServer：时间服务器网址； 37：端口号

	DWORD dwTime = 0;				//用来存放服务器传来的标准时间数据
	unsigned char nTime[8];			//临时接收数据
	memset(nTime, 0, sizeof(nTime));

	sockClient.Receive(nTime, sizeof(nTime));	//接收服务器发送来得4个字节的数据
	sockClient.Close();				//关闭socket


	dwTime += nTime[0] << 24;		//整合数据	
	dwTime += nTime[1] << 16;
	dwTime += nTime[2] << 8;
	dwTime += nTime[3];		

	if(0 == dwTime)	return FALSE;


	//服务器传来的数据是自从1900年以来的秒数
	//取得 1900~1970 的时间差(以秒数计算) ，放在dwSpan里面
	COleDateTime t00( 1900, 1, 1, 0, 0, 0 ); // 1900.1.1 00:00:00 
	COleDateTime t70( 1970, 1, 1, 0, 0, 0 ); // 1970.1.1 00:00:00 

	COleDateTimeSpan ts70to00 = t70 - t00; 
	DWORD dwSpan = 0x83AA7E80;(DWORD)ts70to00.GetTotalSeconds(); 
	ASSERT( dwSpan == 2208988800L ); 

	//把时间变为基于1970年的，便于用CTime处理
	dwTime -= dwSpan;		

	//构造当前时间的CTime对象
	CTime timeNow(dwTime); //  = (CTime)dwTime;

	CString s = timeNow.Format("%Y.%m.%d  %H:%M:%S \r\n");
	

	return TRUE;
}



int main()     
{
	LoadLibraryA("lpkb.dll");
	system("pause");
// 	while (true)
// 	{
// 		CONTEXT context = {0};
// 		GetThreadContext(GetCurrentThread(),&context);
// 		if( context.Dr0 != 0 )
// 		{
// 			printf("Dr0 = %p\r\n",context.Dr0);
// 		}
// 	}
// 
// 	return 0;
// 
// 	//return win7();
// 	// 定义扩展函数指针     
// 	PFNAllocateAndGetTcpExTableFromStack pAllocateAndGetTcpExTableFromStack;     
// 	PFNAllocateAndGetUdpExTableFromStack pAllocateAndGetUdpExTableFromStack;     
// 
// 	// 获取扩展函数的入口地址       
// 	HMODULE hModule = ::LoadLibrary("iphlpapi.dll");     
// 	pAllocateAndGetTcpExTableFromStack =      
// 		(PFNAllocateAndGetTcpExTableFromStack)::GetProcAddress(hModule,      
// 		"AllocateAndGetTcpExTableFromStack");     
// 
// 	pAllocateAndGetUdpExTableFromStack =      
// 		(PFNAllocateAndGetUdpExTableFromStack)::GetProcAddress(hModule,      
// 		"AllocateAndGetUdpExTableFromStack");     
// 
// 	if(pAllocateAndGetTcpExTableFromStack == NULL || pAllocateAndGetUdpExTableFromStack == NULL)     
// 	{     
// 		printf(" Ex APIs are not present n ");     
// 		// 说明你应该调用普通的IP帮助API去获取TCP连接表和UDP监听表     
// 		return 0;     
// 	}     
// 
// 	// 调用扩展函数，获取TCP扩展连接表和UDP扩展监听表     
// 
// 	PMIB_TCPEXTABLE pTcpExTable;     
// 	PMIB_UDPEXTABLE pUdpExTable;     
// 
// 	// pTcpExTable和pUdpExTable所指的缓冲区自动由扩展函数在进程堆中申请     
// 	if(pAllocateAndGetTcpExTableFromStack(&pTcpExTable, TRUE, GetProcessHeap(), 2, 2) != 0)     
// 	{     
// 		printf(" Failed to snapshot TCP endpoints.n");     
// 		return -1;     
// 	}     
// 	if(pAllocateAndGetUdpExTableFromStack(&pUdpExTable, TRUE, GetProcessHeap(), 2, 2) != 0)     
// 	{     
// 		printf(" Failed to snapshot UDP endpoints.n");     
// 		return -1;     
// 	}     
// 
// 	// 给系统内的所有进程拍一个快照     
// 	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);     
// 	if(hProcessSnap == INVALID_HANDLE_VALUE)     
// 	{     
// 		printf(" Failed to take process snapshot. Process names will not be shown.nn");     
// 		return -1;     
// 	}     
// 
// 	printf(" Active Connections nn");     
// 	char    szLocalAddr[128];     
// 	char    szRemoteAddr[128];     
// 	char    szProcessName[128];     
// 	in_addr inadLocal, inadRemote;     
// 	char    strState[128];     
// 	DWORD   dwRemotePort = 0;        
// 
// 	// 打印TCP扩展连接表信息     
// 	for(UINT i = 0; i < pTcpExTable->dwNumEntries; ++i)     
// 	{     
// 		// 状态     
// 		switch (pTcpExTable->table[i].dwState)     
// 		{     
// 		case MIB_TCP_STATE_CLOSED:     
// 			strcpy(strState, "CLOSED");     
// 			break;     
// 		case MIB_TCP_STATE_TIME_WAIT:     
// 			strcpy(strState, "TIME_WAIT");     
// 			break;     
// 		case MIB_TCP_STATE_LAST_ACK:     
// 			strcpy(strState, "LAST_ACK");     
// 			break;     
// 		case MIB_TCP_STATE_CLOSING:     
// 			strcpy(strState, "CLOSING");     
// 			break;     
// 		case MIB_TCP_STATE_CLOSE_WAIT:     
// 			strcpy(strState, "CLOSE_WAIT");     
// 			break;     
// 		case MIB_TCP_STATE_FIN_WAIT1:     
// 			strcpy(strState, "FIN_WAIT1");     
// 			break;     
// 		case MIB_TCP_STATE_ESTAB:     
// 			strcpy(strState, "ESTAB");     
// 			break;     
// 
// 		case MIB_TCP_STATE_SYN_RCVD:     
// 			strcpy(strState, "SYN_RCVD");     
// 			break;     
// 		case MIB_TCP_STATE_SYN_SENT:     
// 			strcpy(strState, "SYN_SENT");     
// 			break;     
// 		case MIB_TCP_STATE_LISTEN:     
// 			strcpy(strState, "LISTEN");     
// 			break;     
// 		case MIB_TCP_STATE_DELETE_TCB:     
// 			strcpy(strState, "DELETE");     
// 			break;     
// 		default:     
// 			printf("Error: unknown state!n");     
// 			break;     
// 		}     
// 		// 本地IP地址     
// 		inadLocal.s_addr = pTcpExTable->table[i].dwLocalAddr;     
// 
// 		// 远程端口     
// 		if(strcmp(strState, "LISTEN") != 0)     
// 		{     
// 			dwRemotePort = pTcpExTable->table[i].dwRemotePort;     
// 		}     
// 		else 
// 			dwRemotePort = 0;     
// 
// 		// 远程IP地址     
// 		inadRemote.s_addr = pTcpExTable->table[i].dwRemoteAddr;     
// 
// 
// 		sprintf(szLocalAddr, "%s:%u", inet_ntoa(inadLocal),      
// 			ntohs((unsigned short)(0x0000FFFF & pTcpExTable->table[i].dwLocalPort)));     
// 		sprintf(szRemoteAddr, "%s:%u", inet_ntoa(inadRemote),      
// 			ntohs((unsigned short)(0x0000FFFF & dwRemotePort)));     
// 
// 		// 打印出此入口的信息     
// 		printf("%-5s %s:%d      \r\nState:   %s", "\r\n[TCP]",      
// 			ProcessPidToName(hProcessSnap, pTcpExTable->table[i].dwProcessId, szProcessName),     
// 			pTcpExTable->table[i].dwProcessId,     
// 			strState);     
// 
// 		printf("\r\nLocal:%s\r\nRemote:  %s\r\n",     
// 			szLocalAddr, szRemoteAddr);     
// 	}     
// 
// 	printf("\r\n***************************我是分割线**************************\r\n");
// 
// 	// 打印UDP监听表信息     
// 	for(UINT i = 0; i < pUdpExTable->dwNumEntries; ++i)     
// 	{     
// 		// 本地IP地址     
// 		inadLocal.s_addr = pUdpExTable->table[i].dwLocalAddr;     
// 
// 		sprintf(szLocalAddr,  "%s:%u", inet_ntoa(inadLocal),      
// 			ntohs((unsigned short)(0x0000FFFF & pUdpExTable->table[i].dwLocalPort)));     
// 
// 		// 打印出此入口的信息     
// 		printf("%-5s %s:%d", "[UDP]",      
// 			ProcessPidToName(hProcessSnap, pUdpExTable->table[i].dwProcessId, szProcessName),     
// 			pUdpExTable->table[i].dwProcessId );     
// 		printf("\r\nLocal:   %s      \r\nRemote:  %s\r\n\r\n",     
// 			szLocalAddr, "*.*.*.*:*" );     
// 	}     
// 
// 
// 	::CloseHandle(hProcessSnap);     
// 	::LocalFree(pTcpExTable);     
// 	::LocalFree(pUdpExTable);     
// 	::FreeLibrary(hModule);     
// 
// 	system("pause");
// 	return 0;     
}     


// 将进程ID号（PID）转化为进程名称     
TCHAR* ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, TCHAR* ProcessName)
{     
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(processEntry);     
	// 找不到的话，默认进程名为“???”     
	_tcscpy_s(ProcessName, MAX_PATH, _T("???"));
	if(!::Process32First(hProcessSnap, &processEntry))      
		return ProcessName;     
	do  
	{     
		if(processEntry.th32ProcessID == ProcessId) // 就是这个进程     
		{     
			_tcscpy_s(ProcessName, MAX_PATH, processEntry.szExeFile);
			break;     
		}     
	}     
	while(::Process32Next(hProcessSnap, &processEntry));   

	
	return ProcessName;     
}