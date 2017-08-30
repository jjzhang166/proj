// jfzrcalltest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "jfzrcalltest.h"
#include "jfzrbase.h"
#include "InjectedWnd.h"
#include <process.h>

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

// CjfzrcalltestApp

BEGIN_MESSAGE_MAP(CjfzrcalltestApp, CWinApp)
END_MESSAGE_MAP()


// CjfzrcalltestApp 构造

CjfzrcalltestApp::CjfzrcalltestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CjfzrcalltestApp 对象

CjfzrcalltestApp theApp;


DWORD	g_dwSendObjAddr = 0;
typedef DWORD (*PGetAllmMemorySendObjAddr)();

void InitBase()
{
	g_dwSendObjAddr = (DWORD)((PGetAllmMemorySendObjAddr)GetProcAddress(LoadLibraryA("d3d9.dll"),"GetAllmMemorySendObjAddr")());
}

BOOL _ReadProcessMemory(DWORD dwReadAddr,void* pReadData,DWORD dwReadlen)
{
	return ::ReadProcessMemory((HANDLE)-1,(LPCVOID)dwReadAddr,pReadData,dwReadlen,0);
}

BOOL _WriteProcessMemory(DWORD dwWriteAddr,void* pWriteData,DWORD dwWritelen)
{
	return ::WriteProcessMemory((HANDLE)-1,(LPVOID)dwWriteAddr,pWriteData,dwWritelen,0);
}


CInjectedWnd m_theDlgUnModel;


#define MFC_DLL_MSG_API

MFC_DLL_MSG_API int __stdcall YAANIPCLIB_Init(IN int nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

MFC_DLL_MSG_API int __stdcall YAANIPCLIB_UnInit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

MFC_DLL_MSG_API long __stdcall YAANIPCLIB_CreateInstance(IN const int nFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInjectedWnd   *  pDlg = new CInjectedWnd;
	if (pDlg == NULL)
	{
		return -1;
	}
	if (FALSE == pDlg->Create(IDD_DIALOG1))
	{
		return -2;
	}

	pDlg->ShowWindow(SW_SHOW);
	return (long)pDlg;
}

MFC_DLL_MSG_API int __stdcall YAANIPCLIB_DestroyInstance(IN long lInstace)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CInjectedWnd   *  pDlg = (CInjectedWnd*)lInstace;
	if (pDlg)
	{
		pDlg->DestroyWindow();
		delete pDlg;
		pDlg = NULL;
	}
	return 0;
}

HANDLE	g_hDlgThread;
long	g_lInstance = 0;
bool	g_bWndRunFlag = false;

uintptr_t __stdcall ThreadFunc( void* param )
{
	g_bWndRunFlag = true;
	g_lInstance = YAANIPCLIB_CreateInstance(0);//创建窗口

	while (true)
	{
		MSG msg;	
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 转化键盘消息
			::TranslateMessage(&msg);
			// 将消息发送到相应的窗口函数
			::DispatchMessage(&msg);
		}

		if( g_bWndRunFlag == false )
		{
			break;
		}
	}

	return 0;
}

// CjfzrcalltestApp 初始化
BOOL CjfzrcalltestApp::InitInstance()
{
	CWinApp::InitInstance();

// 	g_dwSendPacketCallAddr = JFZR_REVERSAL_SEND_ADDR_CALL;
// 	Hook(g_dwSendPacketCallAddr,HookSendPacket);

// 	MessageBox(NULL,"","",MB_OK);
// 	InitBase();
// 	//JFZR_Send("1234567",SayPacket);
// 	JFZR_Send("1234567",UserSkill1);
// 	JFZR_Send("1234567",UserSkill2);
// 	Sleep(300);
// 	JFZR_Send("1234567",UserSkill3);

	//direction2();
	//JFZR_CMD(L"PressMove");
	//JFZR_CMD(L"ReleaseMove");

// 	JFZR_CMD(L"Command");
// 	JFZR_CMD(L"CommandRelease");
//	JFZR_CMD2(L"Jump");
	



	int nRet = 0;
	nRet = YAANIPCLIB_Init(0);
	g_hDlgThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, 0);
	
	return TRUE;
}

int CjfzrcalltestApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_bWndRunFlag = false;
	if( g_hDlgThread )
	{
		::CloseHandle(g_hDlgThread);
		//TerminateThread(g_hDlgThread,0);
		g_hDlgThread = NULL;
	}
	
	YAANIPCLIB_UnInit();
	return CWinApp::ExitInstance();
}
