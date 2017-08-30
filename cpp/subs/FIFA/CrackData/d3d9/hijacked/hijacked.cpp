// hijacked.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "hijacked.h"
#include "updates.h"

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

// ChijackedApp

BEGIN_MESSAGE_MAP(ChijackedApp, CWinApp)
END_MESSAGE_MAP()

namespace AheadLib{
	BOOL WINAPI Load();
	VOID WINAPI Free();
}

// ChijackedApp 构造

ChijackedApp::ChijackedApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

ChijackedApp::~ChijackedApp()
{
	using namespace AheadLib;
	Free();
}


// 唯一的一个 ChijackedApp 对象

ChijackedApp theApp;
HANDLE g_hGameThread;

BOOL GetProcessHostFullName(char* pszFullFileName)
{
	DWORD dwResult = 0;
	::ZeroMemory((PBYTE)pszFullFileName, MAX_PATH);
	if (TRUE != ::IsBadReadPtr((PBYTE)pszFullFileName, MAX_PATH))
		dwResult = ::GetModuleFileNameA(
		NULL,                   // handle to module
		pszFullFileName,        // file name of module
		MAX_PATH                // size of buffer
		);

	return (dwResult != 0);
}

BOOL GetProcessHostName(char* pszFullFileName)
{
	BOOL  bResult;
	char  *pdest;

	int   ch = '\\';

	bResult = GetProcessHostFullName(pszFullFileName);
	if (bResult)
	{
		// Search backward 
		pdest = strrchr(pszFullFileName, ch);
		if( pdest != NULL )
			strcpy_s(pszFullFileName, MAX_PATH, &pdest[1]);
	} // if

	return bResult;
}


void Hook(DWORD dwHookAddr,LPVOID pfun)
{
	try
	{
		unsigned char code = 0xe9;
		WriteProcessMemory((void*)-1,(void*)dwHookAddr,&code,1,NULL);

		DWORD dwDoHookAddr = (DWORD)(void*)pfun;

		DWORD dwcAddr = dwDoHookAddr - dwHookAddr - 1 - 4;

		WriteProcessMemory((void*)-1,(void*)(dwHookAddr+1),&dwcAddr,4,NULL);
	}
	catch (...)
	{
		MessageBoxA(NULL,"哒哒哒","啊啊啊",MB_OK);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// HOOK   6D185163		SetCooperativeLevel
DWORD	g_dwSetCooperativeLevelAddr = 0;
#define DISCL_EXCLUSIVE     0x00000001
#define DISCL_NONEXCLUSIVE  0x00000002
#define DISCL_FOREGROUND    0x00000004
#define DISCL_BACKGROUND    0x00000008		//后台模式，一般我们让他与DISCL_NONEXCLUSIVE（非独占模式）配合使用
#define DISCL_NOWINKEY      0x00000010

void DoSetCooperativeLevel(HWND hWnd,DWORD dwFlags)
{
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] hWnd = 0x%p  dwFlags = 0x%p",hWnd,dwFlags);
	OutputDebugStringA(szPrint);
}

__declspec(naked) void DoHookSetCooperativeLevel()
{
	MessageBoxA(NULL,"","",MB_OK);
	__asm
	{
		pushfd
		pushad
	}

// 	__asm
// 	{
// 		mov eax, DISCL_BACKGROUND
// 		or eax, DISCL_NONEXCLUSIVE
// 		mov dword ptr [esp+0x30], eax
// 
// 		mov eax, dword ptr [esp+0x30]
// 		mov ebx, dword ptr [esp+0x2c]
// 		push eax
// 		push ebx
// 		lea eax, DoSetCooperativeLevel
// 		call eax
// 		add esp, 8
// 	}

	__asm
	{
		popad
		popfd
		mov     edi, edi
		push    ebp
		mov     ebp, esp
		mov eax, g_dwSetCooperativeLevelAddr
		add eax, 5
		Jmp eax
	}
}



///////////////////////////////////////////////////////////////////////////////////////////
// HOOK GetDeviceState
DWORD	g_pFunGetDeviceStateAddr;

BOOL	g_bKeyBoardFlag = false;


bool g_IsSleepTime = false;		// 测试优化用的标志，用来开启和关闭优化
void PrintKeyBoard(int i)
{
	char szPrint[256] = {0};
	switch(i)
	{
	case DIK_ESCAPE:
		{
			sprintf_s(szPrint,"[LOG] 按键：ESC");
		}
		break;
	case DIK_A:
		{
			sprintf_s(szPrint,"[LOG] 按键：A");
		}
		break;
	case DIK_S:
		{
			sprintf_s(szPrint,"[LOG] 按键：S");
		}
		break;
	case DIK_D:
		{
			sprintf_s(szPrint,"[LOG] 按键：D");
		}
		break;
	case DIK_W:
		{
			sprintf_s(szPrint,"[LOG] 按键：W");
		}
		break;
	case DIK_Q:
		{
			sprintf_s(szPrint,"[LOG] 按键：Q");
		}
		break;
	case DIK_F3:
		{
			sprintf_s(szPrint,"[LOG] 按键：F3 开启优化");
			g_IsSleepTime = true;
		}
		break;
	case DIK_F4:
		{
			sprintf_s(szPrint,"[LOG] 按键：F4 关闭优化");
			g_IsSleepTime = false;
		}
		break;
	default:
		{

		}
	}

	OutputDebugStringA(szPrint);
}

void DoGetDeviceState(byte* pKeyBuffer,int nlen)
{
	if( nlen == 0x100 )
	{
		for(int i = 0; i < 256; i++ ) 
		{
			if( pKeyBuffer[i] & 0x80 ) 
			{
				PrintKeyBoard(i);
			}
			else
			{
				continue;
				if( DIK_ESCAPE == i )
				{
					if( g_bKeyBoardFlag == FALSE )
					{
						pKeyBuffer[DIK_ESCAPE] = 0x80;
						PrintKeyBoard(i);
						g_bKeyBoardFlag = TRUE;
					}
					else if( g_bKeyBoardFlag == TRUE )
					{
						g_bKeyBoardFlag = FALSE;
					}
				}
			}
			
		}
	}
}

__declspec(naked) void DoHookGetDeviceState()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		mov eax, dword ptr [esp+0x30]
		mov ebx, dword ptr [esp+0x2c]
		push ebx
		push eax
		lea eax, DoGetDeviceState
		call eax
		add esp, 8
	}

	__asm
	{
		popad
		popfd
		retn 0x0c
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// HOOK 游戏领空调用 GetDeviceState
DWORD	g_pFunGameGetDeviceStateAddr = 0x00B829D6;
#define	GAME_GETDEVICESTATE_JNZ		0x00B82D23
// $+2      01167F02          85C0            test    eax, eax
// $+4      01167F04          0F85 17030000   jnz     fifazf.01168221			01168221 - 0x01167F02 = 0x31f
// $+A      01167F0A          3943 08         cmp     dword ptr ds:[ebx+8], eax	01167F0A - 0x01167F02 = 8

__declspec(naked) void DoHookGameGetDeviceState()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		push 0x100
		lea eax, [EBP-0x104]
		push eax
		lea eax, DoGetDeviceState
		call eax
		add esp, 8
	}

	__asm
	{
		popad
		popfd
	}

	__asm
	{
		test    eax, eax
		jnz     GAME_GETDEVICESTATE_JNZ
		mov		ecx, g_pFunGameGetDeviceStateAddr
		add		ecx, 0x8
		jmp		ecx
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//	修改游戏领空,将dinput设为后台模式or共享模式  01167354 6A 06 PUSH 0x6			
#define GAME_SETCOOPERATIVELEVEL	0x01167354

void DoGameSetCooperativeLevel()
{
 	unsigned char code = 0x0a;
 	WriteProcessMemory((HANDLE)-1,(void*)(GAME_SETCOOPERATIVELEVEL+1),&code,1,NULL);
}



int SleepTime(int nTime)
{
	if( g_IsSleepTime )
	{
		static int nCount = 0;

		if( nCount++ > 20 )
		{
			OutputDebugStringA("[LOG] 刷新一次!");
			nCount = 0;
			return 0;
		}

		Sleep(nTime);
		return 1;	// 返回1 开启优化模式
	}

	return 0;		// 返回0 不做任何事情
}

_declspec(naked) void SleepMainThread()
{
	__asm
	{
		pushfd
		pushad

		push 50
		lea eax, SleepTime
		call eax
		add esp, 4

		test eax, eax
		jnz FIFA_RENDER_RETURN

		popad
		popfd

		PUSH EBX
		PUSH ESI
		PUSH EDI
		MOV EDI,ECX

		mov ecx, UPDATES_FIFA_REVERSAL_STOP_RENDER_AND_SLEEP_HOOK  
		add ecx, 5
		jmp ecx
FIFA_RENDER_RETURN:

		popad
		popfd
		retn
	}
}

// ChijackedApp 初始化

BOOL ChijackedApp::InitInstance()
{
	CWinApp::InitInstance();

	char szProcessName[MAX_PATH] = {0};
	GetProcessHostName(szProcessName);

#ifdef _DEBUG
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[HIJACKED] 进程:%s !!!",szProcessName);
	OutputDebugStringA(szPrint);
#endif

	if( strcmp(szProcessName,"fifazf.exe") == 0 || strcmp(szProcessName,"keyboard.exe") == 0 )
	{
		
		// 加载我需要的东西
		HMODULE hdll = LoadLibraryA("dinput8.dll");
		if( hdll )
		{
// 			DoGameSetCooperativeLevel();
 			Hook(g_pFunGameGetDeviceStateAddr,DoHookGameGetDeviceState);		// hook dx

			Hook(UPDATES_FIFA_REVERSAL_STOP_RENDER_AND_SLEEP_HOOK,SleepMainThread);	// hook render 优化

			g_pFunGetDeviceStateAddr = (DWORD)hdll + 0x80A7;
			//Hook(g_pFunGetDeviceStateAddr+0x0BE,DoHookGetDeviceState);

			g_dwSetCooperativeLevelAddr = (DWORD)hdll + 0x477F;//0x5163;
			//Hook(g_dwSetCooperativeLevelAddr,DoHookSetCooperativeLevel);

// 			g_dwSetCooperativeLevelAddr = 0x0119C0C6;
// 			byte bCode[] = {0x6a,0x0a,0x90,0x90};
// 			WriteProcessMemory( (HANDLE)-1,(void*)g_dwSetCooperativeLevelAddr,bCode,4,NULL);
		}
		
		using namespace AheadLib;
		return Load();
	}

	if( strcmp(szProcessName,"fifalauncher.exe") == 0 || strcmp(szProcessName,"Client.exe") == 0 )
	{
		using namespace AheadLib;
		return Load();
	}

	return FALSE;

}
