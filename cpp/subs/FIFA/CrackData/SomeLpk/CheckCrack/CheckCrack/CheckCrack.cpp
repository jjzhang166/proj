// CheckCrack.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CheckCrack.h"
#include "../command/CheckThrad.h"
#include <process.h>
#include "../protect/CheckLpk.h"
#include <CD_CommonFiles/CD_AntiCrack.h>
#include "../command/CRC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCheckCrackApp

BEGIN_MESSAGE_MAP(CCheckCrackApp, CWinApp)
END_MESSAGE_MAP()


// CCheckCrackApp 构造

CCheckCrackApp::CCheckCrackApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_hCheckCrackThread = NULL;
}

// 唯一的一个 CCheckCrackApp 对象

CCheckCrackApp theApp;
bool g_bConnectFlag = true;

// 函数使用：在hook某一地址后，传入hook的地址，存储到某变量中，在检测的时候，只需要再次调用，并判断返回值
DWORD	GetHookGoalAddr(DWORD dwHookAddr)
{
	DWORD	dwHookGoalAddr = 0;
	dwHookAddr++;
	__asm
	{
		mov eax, dwHookAddr
		mov ebx, dword ptr [eax]
		mov dwHookGoalAddr, ebx
	}

	// 	char szPrint[256] = {0};
	// 	sprintf_s(szPrint,"[LOG] %p : %p ==> %p call ==> %p",dwHookAddr,dwHookAddr+4,dwHookGoalAddr,dwHookAddr+4+dwHookGoalAddr);
	// 	OutputDebugString(szPrint);
	return dwHookAddr+4+dwHookGoalAddr;
}

HANDLE	g_hBackUpThread = NULL;
DWORD	g_dwBackUpThreadId = 0;
HANDLE	g_hProcess = NULL;
bool    g_bCheckThreadRunFlag = false;

typedef struct
{
	DWORD	dwFunAddr;		// 函数地址
	DWORD	dwHookAddr;		// 函数hook后的跳转地址
}CHECK_HOOK_ADDR,*PCHECK_HOOK_ADDR;

CHECK_HOOK_ADDR	theConnect;

bool g_bConnectCodeCRCflag = false;		// 连接服务器时对控制台代码 CRC检测
DWORD g_dwConnectCodeCRC = 0;

typedef int (WINAPI *PCONNECT)(
								SOCKET s,                          
								const struct sockaddr FAR *name,  
								int namelen                        
								);

PCONNECT	Old_connect;
PCONNECT	Jmp_Old_connect;

// struct sockaddr {
// 	u_short sa_family;              /* address family */
// 	char    sa_data[14];            /* up to 14 bytes of direct address */
// };

int WINAPI CHECKconnect(
			SOCKET s,                          
			const struct sockaddr FAR *name,  
			int namelen                        
			)
{
	// 代码CRC
	PIMAGE_DOS_HEADER	    pDosHeader=NULL;
	PIMAGE_NT_HEADERS       pNtHeader=NULL;
	PIMAGE_SECTION_HEADER   pSecHeader=NULL;
	DWORD					ImageBase;
	DWORD					dwCodeCrc = 0;
	unsigned char*			pCheckCodeBaseAddr = NULL;
	IMAGE_OPTIONAL_HEADER32* pImageOptionalHeader32 = NULL;
	IMAGE_SECTION_HEADER*	pImageSectionHeader = NULL;

	try
	{
		ImageBase=(DWORD)GetModuleHandle(NULL);//取基址，其实本例也可直接用0x4000000这个值

		pDosHeader=(PIMAGE_DOS_HEADER)ImageBase;	 	 
		pNtHeader=(PIMAGE_NT_HEADERS32)((DWORD)pDosHeader+pDosHeader->e_lfanew);
		pSecHeader=IMAGE_FIRST_SECTION(pNtHeader);   //得到第一个区块的起始地址  
		pImageOptionalHeader32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pDosHeader+pDosHeader->e_lfanew+sizeof(IMAGE_FILE_HEADER)+sizeof(DWORD));
		pImageSectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)pDosHeader+pDosHeader->e_lfanew+sizeof(IMAGE_FILE_HEADER)+sizeof(DWORD)+sizeof(IMAGE_OPTIONAL_HEADER32));
		pCheckCodeBaseAddr = (unsigned char*)(ImageBase+pSecHeader->VirtualAddress);

		CRC32_INLINE(pCheckCodeBaseAddr,pImageSectionHeader->Misc.PhysicalAddress,dwCodeCrc);
	}
	catch (...)
	{
		assert(false);
	}

	VMP_BEGIN_EX;

	try
	{
		// 这里开始 干扰 分析
		DWORD dwCodeCrcTemp = 0;
		for (int i=0;i<1000; i++)
		{
			dwCodeCrcTemp += dwCodeCrc;
		}

		if( dwCodeCrcTemp == 0 )
		{
			g_dwConnectCodeCRC = dwCodeCrcTemp;
		}
		// 结束 干扰分析
	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		sockaddr_in* pname = (sockaddr_in*)name;

		if( IP == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP1 == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP2 == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP3 == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP4 == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP5 == XOR_DWORD(*(DWORD*)&pname->sin_addr) ||
			IP6 == XOR_DWORD(*(DWORD*)&pname->sin_addr) )
		{
			g_bConnectFlag = true;			// 如果存在白名单的ip，则置标志为true
			DEBUG_PRINT("hook connect server");

#ifdef	_OPEN_CRC_

			// 读取lpk写入的crc
			static DWORD dwLpkCRC = *(DWORD*)(pCheckCodeBaseAddr+pImageSectionHeader->Misc.PhysicalAddress);

#ifdef _DEBUG_LOG_
			char szPrint[256];
			sprintf_s(szPrint,"[LOG] CHECKconnect code addr = 0x%p size = 0x%p lpk crc = 0x%p crc = 0x%p ip = %d.%d.%d.%d",
				ImageBase+pSecHeader->VirtualAddress,pImageSectionHeader->Misc.PhysicalAddress,dwLpkCRC,dwCodeCrc,
				pname->sin_addr.S_un.S_un_b.s_b1,
				pname->sin_addr.S_un.S_un_b.s_b2,
				pname->sin_addr.S_un.S_un_b.s_b3,
				pname->sin_addr.S_un.S_un_b.s_b4);
			OutputDebugStringA(szPrint);
#endif

			if( dwLpkCRC != dwCodeCrc )
			{
#ifdef _DEBUG_LOG_
				OutputDebugStringA("[LOG] connect crc error!");
#endif
				g_bConnectFlag = false;	// 连接失败
				return WSAEADDRNOTAVAIL;
			}
#endif

		}
	}
	catch (...)
	{
		assert(false);
	}

	VMP_END;

	return Jmp_Old_connect(s,name,namelen);
}

reversalsock	theSocket;
// CCheckCrackApp 初始化
unsigned int __stdcall CheckCrackThreadProc(
									LPVOID lpParameter)
{
	CheckIp theCheckIp;
	try
	{
		CheckLpk theCheckLpk;

#if _DEBUG
		pthePacket = new FIFAPacket;
		if( pthePacket == NULL )
		{
			assert(false);
			return 0;
		}
		ptheDBConnectionManager	= new FIFADBConnectionManager;
		if( ptheDBConnectionManager == NULL )
		{
			assert(false);
			return 0;
		}
		if( false == theSocket.bind("lpkb_fifa",&theSocket.m_hsocket.m_theScrSocket) )
		{
			assert(false);
			return 0;
		}

		while( g_bCheckThreadRunFlag )
		{
			if( theCheckIp.CheckTcp() == false )
			{
				DEBUG_PRINT("CheckCrackThreadProc local host error1!");
				return false;
			}
		}

		if( pthePacket != NULL )
		{
			delete pthePacket;
			pthePacket = NULL;
		}
#else
		const BYTE* pszConsoleVersion = (BYTE*)theCheckLpk.LpkOpenConsoleFileMapping(VMProtectDecryptString(kCheckFileMapName));

		if( pszConsoleVersion != NULL )
		{
			BYTE bCheckConsoleVersionKey = (BYTE)(pszConsoleVersion[kKeyValueOffset]^GetCurrentProcessId());

			if( bCheckConsoleVersionKey == kXorConstValue )
			{
				pthePacket = new FIFAPacket;
				if( pthePacket == NULL )
				{
					assert(false);
					return 0;
				}
				ptheDBConnectionManager	= new FIFADBConnectionManager;
				if( ptheDBConnectionManager == NULL )
				{
					assert(false);
					return 0;
				}
				if( false == theSocket.bind("lpkb_fifa",&theSocket.m_hsocket.m_theScrSocket) )
				{
					assert(false);
					return 0;
				}

				while( g_bCheckThreadRunFlag )
				{
					if( theCheckIp.CheckTcp() == false )
					{
						DEBUG_PRINT("CheckCrackThreadProc local host error1!");
						return false;
					}
				}

				if( pthePacket != NULL )
				{
					delete pthePacket;
					pthePacket = NULL;
				}
			}
		}
#endif

		
	}
	catch (...)
	{
		VERIFY(false);
	}

	return 0;
}

BOOL CCheckCrackApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	VMP_BEGIN_EX;

	g_hProcess = (HANDLE)XOR_DWORD(GetCurrentProcessId());


	unsigned int dwThreadId = 0;
	m_hCheckCrackThread = (HANDLE)_beginthreadex(NULL,0,CheckCrackThreadProc,NULL,0,&dwThreadId);

	g_hBackUpThread = (HANDLE)XOR_DWORD(m_hCheckCrackThread);
	g_dwBackUpThreadId = XOR_DWORD(dwThreadId);

	if( m_hCheckCrackThread == NULL )
	{
		DEBUG_PRINT("m_hCheckCrackThread error");
		assert(false);
		return FALSE;
	}

	g_bCheckThreadRunFlag = true;
	//by zhp, 2016/12/19 16:59去掉CRC检测
	/*Old_connect = (PCONNECT)GetProcAddress(GetModuleHandleA("Ws2_32.dll"), VMProtectDecryptString("connect"));
	if( Old_connect != NULL )
	{
		HOOKEX(Old_connect,CHECKconnect,Jmp_Old_connect);
		theConnect.dwFunAddr = (DWORD)Old_connect;
		theConnect.dwHookAddr = GetHookGoalAddr(theConnect.dwFunAddr);
		assert(theConnect.dwHookAddr);
		DEBUG_PRINT("hook connect success");

		g_bCheckThreadRunFlag = true;
	}*/

	DEBUG_PRINT("Init success!");
	VMP_END;
	return TRUE;
}

int CCheckCrackApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	g_bCheckThreadRunFlag = false;

	if( m_hCheckCrackThread != NULL )
	{
		CloseHandle(m_hCheckCrackThread);
		m_hCheckCrackThread = NULL;
		
	}
	return CWinApp::ExitInstance();
}
