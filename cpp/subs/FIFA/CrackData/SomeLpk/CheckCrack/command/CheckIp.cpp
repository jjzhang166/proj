#include "StdAfx.h"
#include "CheckIp.h"
#include "../protect/CheckLpk.h"

#define XOR_DECRYPT(data,nlen)	for (size_t i = 0; i<nlen; i++)	\
								{								\
								if( i%10 == 0 )				\
									{							\
									continue;				\
									}							\
									if( i%3 == 0 )				\
									data[i] -= 1;			\
									data[i] ^= 0x7F;			\
								}

typedef DWORD (WINAPI *PFNGetExtendedTcpTable)(
	__out         PVOID pTcpTable,				//返回查询结构体指针
	_Inout_       PDWORD pdwSize,				//第一次调用该参数会返回所需要的缓冲区大小
	__in          BOOL bOrder,					//是否排序
	__in          ULONG ulAf,					//是 AF_INET还是AF_INET6
	__in          TCP_TABLE_CLASS TableClass,	// 表示结构体的种类，此处设为 TCP_TABLE_OWNER_PID_ALL
	__in          ULONG Reserved				//保留不用，设为 0
	);

bool IsDebug(DWORD dwFun,int nSize)
{
	BYTE bCode = 0;
	for( int i = 0; i<nSize; i++,dwFun++)
	{
		__asm
		{
			mov eax, dword ptr [dwFun]
			mov al, byte ptr [eax]
			mov bCode, al
		}

		// 如果OD下了函数断点，则不执行 
		if( bCode == 0xCC )
		{
			return true;
		}
	}

	return false;
}

CheckIp::CheckIp(void)
{
	m_pfnGetTcpTable = NULL;
}

CheckIp::~CheckIp(void)
{
}

extern HANDLE	g_hProcess;
extern DWORD	GetHookGoalAddr(DWORD dwHookAddr);


bool	g_bConnectCheckServerIpFlag = false;		// 连接服务器的标志
extern  bool g_bConnectFlag;						// connect有白名单ip来到
extern  reversalsock	theSocket;

bool CheckIp::CheckTcp()
{
	VMP_BEGIN_EX;
	DWORD	dwLocalIpCount = 0;
	static  bool bLoginSucessFlag = false;
	static  bool bFindOledlgFlag = false;
	static  bool bpfnGetTcpTableSehFlag = false;

	try
	{	
#if _DEBUG
		if( true )
#else
		if(/* g_bConnectCheckServerIpFlag == true &&	*/	// API pfnGetTcpTable 检测到白名单ip时为true
			g_bConnectFlag == true &&			// API connect 检测到白名单ip时为true
			bLoginSucessFlag == true &&			// 连接服务器成功后，打开事件和内存映射后为true
			bFindOledlgFlag == false &&			// 没有发现劫持dll oledlg.dll
			bpfnGetTcpTableSehFlag == false		// 检测ip时没有发现异常
			)
#endif

		{
			printf("do...\r\n");
			theSocket.doconnect();
			Sleep(1000);
			return true;
		}

		CheckLpk	theCheckLpk;
		if (theCheckLpk.LpkOpenEvent(VMProtectDecryptString(LPK_CHECK_EVENT)))
		{
			char szOutData[REVERSAL_SOCK_BUFFER_SIZE] = {0};
			if (theCheckLpk.LpkOpenFileMapping(VMProtectDecryptString(LPK_CHEKC_FILEMAPPING), szOutData))
			{
				if( bLoginSucessFlag == false )
				{
					char* pszdata = VMProtectDecryptString(LPK_CHECK_VIEWOFFILE);

					char szdata[1024] = {0};
					memcpy(szdata,szOutData,strlen(szOutData));
					XOR_DECRYPT(szdata,strlen(szdata));

					for (int i=0; i<(int)strlen(pszdata); i++)
					{
						if( szdata[i] != pszdata[i] )
						{
							assert(false);
							return true;
						}
					}
				}
				bLoginSucessFlag = true;
				return true;
			}
		}
		
		bFindOledlgFlag = Findoledlg();
		
		HMODULE hModule = ::LoadLibraryA(VMProtectDecryptString("iphlpapi.dll"));
		PMIB_TCPTABLE_OWNER_PID pTcpTable = NULL;
		if( hModule != NULL )
		{
			PFNGetExtendedTcpTable pfnGetTcpTable = NULL;
			if( m_pfnGetTcpTable != NULL )
			{
				pfnGetTcpTable = m_pfnGetTcpTable;
			}
			else
			{
				pfnGetTcpTable = (PFNGetExtendedTcpTable)::GetProcAddress(hModule, VMProtectDecryptString("GetExtendedTcpTable"));

				if( pfnGetTcpTable == NULL )
				{
					bpfnGetTcpTableSehFlag = true;
					assert(false);
					return false;
				}

				m_pfnGetTcpTable = pfnGetTcpTable;
			}

			Sleep(100);
			if( IsDebug((DWORD)pfnGetTcpTable,0x30) )
			{
				return true;
			}

			pTcpTable = new MIB_TCPTABLE_OWNER_PID;
			if( pTcpTable != NULL )
			{
				DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);

				if (pfnGetTcpTable(pTcpTable, &dwSize,
					TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0) == ERROR_INSUFFICIENT_BUFFER)
				{	//第一次调用时不知道要传入的缓冲区大小，所以要试探一下，参数dwSize会返		回真正需要的大小
					delete pTcpTable;
					pTcpTable = (MIB_TCPTABLE_OWNER_PID *)new char[dwSize];//重新分配缓		冲区
					if( pTcpTable != NULL )
					{
						if(pfnGetTcpTable(pTcpTable,&dwSize,TRUE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0) != NO_ERROR)
						{
							delete pTcpTable;
							bpfnGetTcpTableSehFlag = true;
							assert(false);
							return false;
						}
						int nNum = (int) pTcpTable->dwNumEntries; //TCP连接的数目
						for(int i=0;i<nNum;i++)
						{
// 							char szLocalPrint[1024] = {0};
// 							sprintf_s(szLocalPrint,"本地地址：%s:%d  远程地址：%s:%d  状态：%d  进程ID:%d\r\n", 
// 								inet_ntoa(*(in_addr*)& pTcpTable->table[i].dwLocalAddr), //本地IP 地址
// 								htons((WORD)pTcpTable->table[i].dwLocalPort), //本地端口
// 								inet_ntoa(*(in_addr*)& pTcpTable->table[i].dwRemoteAddr), //远程IP地址
// 								htons((WORD)pTcpTable->table[i].dwRemotePort), //远程端口
// 								pTcpTable->table[i].dwState, //状态
// 								pTcpTable->table[i].dwOwningPid); //所属进程PID
// 							DEBUG_PRINT(szLocalPrint);




							// 检测猫头鹰服务器ip地址
							if( MAOTOUYAN_SERVER_IP == XOR_DWORD(pTcpTable->table[i].dwRemoteAddr) )
							{
								DEBUG_PRINT("error ip");
								bpfnGetTcpTableSehFlag = true;
								assert(false);
								return false;
							}

//							if( XOR_DWORD(g_hProcess) == pTcpTable->table[i].dwOwningPid )
//							{
// 								char szLocalPrint[1024] = {0};
// 								sprintf_s(szLocalPrint,"本地地址：%s:%d  远程地址：%s:%d  状态：%d  进程ID:%d\r\n", 
// 									inet_ntoa(*(in_addr*)& pTcpTable->table[i].dwLocalAddr), //本地IP 地址
// 									htons((WORD)pTcpTable->table[i].dwLocalPort), //本地端口
// 									inet_ntoa(*(in_addr*)& pTcpTable->table[i].dwRemoteAddr), //远程IP地址
// 									htons((WORD)pTcpTable->table[i].dwRemotePort), //远程端口
// 									pTcpTable->table[i].dwState, //状态
// 									pTcpTable->table[i].dwOwningPid); //所属进程PID
// 								DEBUG_PRINT(szLocalPrint);

// 								if( pTcpTable->table[i].dwState == 2 || ZEOR_IP == XOR_DWORD(pTcpTable->table[i].dwLocalAddr) )
// 								{
// 									switch(htons((WORD)pTcpTable->table[i].dwLocalPort))
// 									{
// 									case 45398:
// 									case 45399:
// 									case 45400:
// 									case 45390:
// 									case 45219:
// 										break;
// 									default:
// 										{
// 											DEBUG_PRINT("error port");
// 											bpfnGetTcpTableSehFlag = true;
// 											assert(false);
// 											return false;
// 										}
// 									}
// 								}
//							}
						}
						delete pTcpTable;

						return true;
					}
				}
				else
				{
					DEBUG_PRINT("pfnGetTcpTable fun error");
					bpfnGetTcpTableSehFlag = true;
					assert(false);
				}
			}
			else
			{
				DEBUG_PRINT("pTcpTable == NULL");
				bpfnGetTcpTableSehFlag = true;
				assert(false);
			}
		}
		else
		{
			DEBUG_PRINT("hModule == NULL");
			bpfnGetTcpTableSehFlag = true;
			assert(false);
		}

		if( pTcpTable != NULL )
		{
			delete pTcpTable;
		}
	}
	catch (...)
	{
		//MessageBox(NULL,"CheckTcp","",MB_OK);
		VERIFY(false);
	}

	VMP_END;
	return false;
}

bool CheckIp::Findoledlg(void)
{
	// 检测 oledlg.dll
	char chpath[MAX_PATH];  
	GetModuleFileNameA(NULL,(LPSTR)chpath,sizeof(chpath));
//	DEBUG_PRINT(chpath);

	char* pchpath = strrchr(chpath,'\\');
	*pchpath = 0;
//	DEBUG_PRINT(chpath);

	strcat_s(chpath,"\\oledlg.dll");
//	DEBUG_PRINT(chpath);
	if( LoadLibraryA(chpath) )
	{
		DEBUG_PRINT("find oledlg.dll");
		return true;
	}
	else
	{
		if( GetLastError() != 126 )
		{
			DEBUG_PRINT("find oledlg.dll");
			return true;
		}
	}
	return false;
}