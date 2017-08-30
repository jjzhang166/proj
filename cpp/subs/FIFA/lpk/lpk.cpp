

//   http://processhacker.sourceforge.net/doc/ntexapi_8h_source.html   各种关于进程结构体
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include "stdafx.h"
#include "detours.h"
#include <Tlhelp32.h>
#include "CheckLpk.h"
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include "MyResCtrl.h"
#include "resource.h"
#include "Protect.h"
#include "CheckServer.h"


// #include <afxtempl.h>
// #include <afxcoll.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "VMProtectSDK.h"

#define __in_out


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:LpkInitialize=_AheadLib_LpkInitialize,@1")
#pragma comment(linker, "/EXPORT:LpkTabbedTextOut=_AheadLib_LpkTabbedTextOut,@2")
#pragma comment(linker, "/EXPORT:LpkDllInitialize=_AheadLib_LpkDllInitialize,@3")
#pragma comment(linker, "/EXPORT:LpkDrawTextEx=_AheadLib_LpkDrawTextEx,@4")
#pragma comment(linker, "/EXPORT:LpkEditControl=_AheadLib_LpkEditControl,@5")
#pragma comment(linker, "/EXPORT:LpkExtTextOut=_AheadLib_LpkExtTextOut,@6")
#pragma comment(linker, "/EXPORT:LpkGetCharacterPlacement=_AheadLib_LpkGetCharacterPlacement,@7")
#pragma comment(linker, "/EXPORT:LpkGetTextExtentExPoint=_AheadLib_LpkGetTextExtentExPoint,@8")
#pragma comment(linker, "/EXPORT:LpkPSMTextOut=_AheadLib_LpkPSMTextOut,@9")
#pragma comment(linker, "/EXPORT:LpkUseGDIWidthCache=_AheadLib_LpkUseGDIWidthCache,@10")
#pragma comment(linker, "/EXPORT:ftsWordBreak=_AheadLib_ftsWordBreak,@11")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
//#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



BOOL GetProcessHostFullName(char* pszFullFileName)
{
	DWORD dwResult = 0;
	::ZeroMemory((PBYTE)pszFullFileName, MAX_PATH);
	if (TRUE != ::IsBadReadPtr((PBYTE)pszFullFileName, MAX_PATH))
		dwResult = ::GetModuleFileName(
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
			strcpy(pszFullFileName, &pdest[1]);
	} // if

	return bResult;
}

BOOL GetProcessPatch(char* pszPatch)
{
	BOOL  bResult;
	char  *pdest;

	int   ch = '\\';

	bResult = GetProcessHostFullName(pszPatch);
	if (bResult)
	{
		// Search backward 
		pdest = strrchr(pszPatch, ch);
		if( pdest != NULL )
		{
			pdest[1] = 0;
		}
	} // if

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	DWORD m_dwReturn[11] = {0};	// 原始函数返回地址

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// CreateToolhelp32Snapshot
	typedef	HANDLE (WINAPI *LPK_CreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID );
	LPK_CreateToolhelp32Snapshot  Old_CreateToolhelp32Snapshot;

	HANDLE WINAPI My_CreateToolhelp32Snapshot( 
		DWORD dwFlags, 
		DWORD th32ProcessID )
	{
		//OutputDebugString("[HOOK] My_CreateToolhelp32Snapshot");
		
		return INVALID_HANDLE_VALUE;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// TerminateProcess
	typedef BOOL (WINAPI *LPK_TerminateProcess)(HANDLE hProcess, DWORD uExitCode);
	LPK_TerminateProcess	Old_TerminateProcess;
	LPK_TerminateProcess	Jmp_Old_TerminateProcess;

	BOOL WINAPI My_TerminateProcess(
		HANDLE hProcess, 
		DWORD uExitCode)
	{
		if( hProcess == (HANDLE)0x88888888 )
		{
			//OutputDebugString("[HOOK] 放行自己关闭进程");
			Jmp_Old_TerminateProcess(GetCurrentProcess(),uExitCode);
		}
		//OutputDebugString("[HOOK] My_TerminateProcess");
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// EnumProcesses
	typedef BOOL (WINAPI *LPK_EnumProcesses)(DWORD *lpidProcess, DWORD cb,DWORD *cbNeeded );
	LPK_EnumProcesses		Old_EnumProcesses;

	BOOL WINAPI My_EnumProcesses(
									DWORD *lpidProcess,  // array of process identifiers
									DWORD cb,            // size of array
									DWORD *cbNeeded      // number of bytes returned
		)
	{
		//OutputDebugString("[HOOK] My_EnumProcesses");
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// EnumProcessModules
	typedef BOOL (WINAPI *LPK_EnumProcessModules)(
												HANDLE hProcess,      // handle to process
												HMODULE *lphModule,   // array of module handles
												DWORD cb,             // size of array
												LPDWORD lpcbNeeded    // number of bytes required
												);
	LPK_EnumProcessModules Old_EnumProcessModules;

	BOOL WINAPI My_EnumProcessModules(
										HANDLE hProcess,      // handle to process
										HMODULE *lphModule,   // array of module handles
										DWORD cb,             // size of array
										LPDWORD lpcbNeeded    // number of bytes required
										)
	{
		//OutputDebugString("[HOOK] My_EnumProcessModules");
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// OpenProcess
	typedef HANDLE (WINAPI *LPK_OpenProcess)(
												DWORD dwDesiredAccess,  // access flag
												BOOL bInheritHandle,    // handle inheritance option
												DWORD dwProcessId       // process identifier
												);
	LPK_OpenProcess		Old_OpenProcess;

	HANDLE WINAPI My_OpenProcess(
		DWORD dwDesiredAccess,  // access flag
		BOOL bInheritHandle,    // handle inheritance option
		DWORD dwProcessId       // process identifier
		)
	{
// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"[HOOK] My_OpenProcess dwProcessId = %d", dwProcessId);
// 		OutputDebugString(szPrint);

		return (HANDLE)-1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// FindWindow
	typedef HWND (WINAPI *LPK_FindWindowA)( 
											LPCTSTR lpClassName, 
											LPCTSTR lpWindowName ); 
	LPK_FindWindowA Old_FindWindowA;

	HWND My_FindWindowA( 
					LPCTSTR lpClassName, 
					LPCTSTR lpWindowName )
	{
// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"lpWindowNameA = %s",lpWindowName);
// 		OutputDebugString(szPrint);
		return NULL;
	}


	typedef HWND (WINAPI *LPK_FindWindowW)( 
											LPCTSTR lpClassName, 
											LPCTSTR lpWindowName ); 
	LPK_FindWindowW Old_FindWindowW;

	HWND My_FindWindowW( 
					LPCTSTR lpClassName, 
					LPCTSTR lpWindowName )
	{
// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"lpWindowNameW = %s",lpWindowName);
// 		OutputDebugString(szPrint);
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// NtCreateFile
	typedef struct _LSA_UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR Buffer;} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG           Length;
		HANDLE          RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG           Attributes;
		PVOID           SecurityDescriptor;
		PVOID           SecurityQualityOfService;
	}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	typedef struct _IO_STATUS_BLOCK
	{
		LONG Status;
		ULONG_PTR Information;}
	IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

	typedef LONG (NTAPI *LPK_NtCreateFile)(
		OUT PHANDLE             FileHandle,
		IN ACCESS_MASK          DesiredAccess,
		IN POBJECT_ATTRIBUTES   ObjectAttributes,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		IN PLARGE_INTEGER       AllocationSize OPTIONAL,
		IN ULONG                FileAttributes,
		IN ULONG                ShareAccess,
		IN ULONG                CreateDisposition,
		IN ULONG                CreateOptions,
		IN PVOID                EaBuffer OPTIONAL,
		IN ULONG                EaLength );

	LPK_NtCreateFile	Old_NtCreateFile;
	LPK_NtCreateFile	Jmp_Old_NtCreateFile;

	LONG NTAPI My_NtCreateFile(
		OUT PHANDLE             FileHandle,
		IN ACCESS_MASK          DesiredAccess,
		IN POBJECT_ATTRIBUTES   ObjectAttributes,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		IN PLARGE_INTEGER       AllocationSize OPTIONAL,
		IN ULONG                FileAttributes,
		IN ULONG                ShareAccess,
		IN ULONG                CreateDisposition,
		IN ULONG                CreateOptions,
		IN PVOID                EaBuffer OPTIONAL,
		IN ULONG                EaLength )
	{

// 		09F01580    8B4424 1C       mov     eax, dword ptr ss:[esp+1C]                       ; ShareAccess
// 		09F01584    81EC 00020000   sub     esp, 200
// 		09F0158A    85C0            test    eax, eax
// 		09F01594    0F85 B5000000   jnz     LPK.09F0164F
		if( ShareAccess == 0 )
		{
			// 		09F0158C    56              push    esi
			// 		09F0158D    8BB424 10020000 mov     esi, dword ptr ss:[esp+210]                      ; IoStatusBlock
			// 		09F0159A    85F6            test    esi, esi
			// 		09F0159C    0F84 AD000000   je      LPK.09F0164F
			if( IoStatusBlock != 0 )
			{
				// 		09F015A2    8B56 08         mov     edx, dword ptr ds:[esi+8]                        ; IoStatusBlock.Information
				// 		09F015A5    85D2            test    edx, edx
				// 		09F015A7    0F84 A2000000   je      LPK.09F0164F
				// 		09F015AD    8B4A 04         mov     ecx, dword ptr ds:[edx+4]                        ; 64位取低32字节
				// 		09F015B0    85C9            test    ecx, ecx
				// 		09F015B2    0F84 97000000   je      LPK.09F0164F
				if( IoStatusBlock->Information != 0 )
				{
					//OutputDebugString("[HOOK] My_NtCreateFile");
					return Jmp_Old_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,FILE_SHARE_READ | FILE_SHARE_WRITE,CreateDisposition,CreateOptions,EaBuffer,EaLength);
				}
			}
		}

		//OutputDebugString("[HOOK] Old_NtCreateFile");
		return Jmp_Old_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,FILE_SHARE_READ | FILE_SHARE_WRITE,CreateDisposition,CreateOptions,EaBuffer,EaLength);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// HOOK FindFirstFile
	typedef HANDLE (*LPK_FindFirstFile)(
		LPCTSTR lpFileName, 
		LPWIN32_FIND_DATA lpFindFileData ); 
	LPK_FindFirstFile Old_FindFirstFile;

	HANDLE My_FindFirstFile( LPCTSTR lpFileName,  LPWIN32_FIND_DATA lpFindFileData )
	{
		OutputDebugString("[HOOK] My_FindFirstFile");
		return (HANDLE)-1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// HOOK DeviceIoControl
	typedef BOOL (WINAPI*LPK_DeviceIoControl)(
		HANDLE hDevice, 
		DWORD dwIoControlCode, 
		LPVOID lpInBuffer, 
		DWORD nInBufferSize, 
		LPVOID lpOutBuffer, 
		DWORD nOutBufferSize, 
		LPDWORD lpBytesReturned, 
		LPOVERLAPPED lpOverlapped);

	LPK_DeviceIoControl Old_DeviceIoControl;
	LPK_DeviceIoControl Jmp_Old_DeviceIoControl;

	BOOL WINAPI My_DeviceIoControl(
							HANDLE hDevice, 
							DWORD dwIoControlCode, 
							LPVOID lpInBuffer, 
							DWORD nInBufferSize, 
							LPVOID lpOutBuffer, 
							DWORD nOutBufferSize, 
							LPDWORD lpBytesReturned, 
							LPOVERLAPPED lpOverlapped)
	{

// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"[HOOK] dwIoControlCode = %p",dwIoControlCode);
// 		OutputDebugString(szPrint);

		return Jmp_Old_DeviceIoControl(hDevice,dwIoControlCode,lpInBuffer,nInBufferSize,lpOutBuffer,nOutBufferSize,lpBytesReturned,lpOverlapped);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// CPU 优化

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
			MessageBox(NULL,"哒哒哒","啊啊啊",MB_OK);
		}
	}

	//1. SLEEP 做事线程,让出时间片
	DWORD	g_dwCount = 0;
	DWORD	g_dwHookDoDownCpu = 0x0100FFB0;
	__declspec(naked) void DoHookTask()
	{
		g_dwCount++;

		if( g_dwCount > 80 )
		{
			Sleep(2);
			g_dwCount = 0;
		}

		__asm
		{
			retn 
		}
	}
	//2. 优化渲染
	DWORD	g_dwHookPresent = 0x0111506B;
	__declspec(naked) void DoHookPresent()
	{
		Sleep(100);
		__asm
		{
			retn 0x04
		}
	}

	DWORD	g_dwDoNopCpu = 0;
	__declspec(naked) void  DoNopCpu()
	{
		__asm
		{
			pushad
			pushfd
		}
		if( g_dwDoNopCpu++ < 5 )
		{
			g_dwDoNopCpu = 0;
			__asm
			{
				popfd
				popad
				MOVSS XMM1,DWORD PTR DS:[ESI+0x78]
				mov edx, 0x0047537D
				jmp edx
			}
		}
		else
		{
			__asm
			{
				popfd
				popad
				POP ESI
				POP EDI
				MOV ESP,EBP
				POP EBP
				RETN
			}
		}
	}

	DWORD g_dwPresent1Count = 0;
	bool  g_dwPresent1Flag = false;
	// 降低渲染线程的CPU
	__declspec(naked) void DoHookPresent1()
	{
		__asm
		{
			pushfd
			pushad
		}
		if( g_dwPresent1Flag == false )
		{
			g_dwPresent1Count++;
			if( g_dwPresent1Count > 1500 )
			{
				g_dwPresent1Flag = true;
				OutputDebugString("[HOOK] 开始优化");
			}
		}

		if( g_dwPresent1Flag )
		{
			Sleep(0x100);
			__asm
			{
				popad
				popfd
				retn
			}
		}
		else
		{
			__asm
			{
				popad
				popfd
				push ebx
				mov ebx, esp
				sub esp,0x8
				mov eax, 0x0047EB06
				jmp eax
			}
		}

	}

	DWORD g_dwPresent2Count = 0;
	__declspec(naked) void DoHookPresent2()
	{
		__asm push ecx

		g_dwPresent2Count++;
		if( g_dwPresent2Count > 2 )
		{
			g_dwPresent2Count = 0;
			__asm
			{
				pop ecx
				PUSH EBX
				PUSH ESI
				MOV ESI,ECX
				MOV EAX,DWORD PTR DS:[ESI]
				mov edx, 0x00570546
				jmp edx
			}
		}
		__asm
		{
			pop ecx
			retn
		}
	}

	bool	g_bDeleteFile = false;

	// 降低主线程的CPU
	__declspec(naked) void DoHookTask1()
	{
		if( g_bDeleteFile == false )
		{
			if( false == g_theCheckServer.Check() )
			{
				assert(false);
				__asm
				{
					retn 4
				}
			}
			if( DeleteFile(g_theCheckServer.m_szReleaseResExeFile) )
			{
				g_bDeleteFile = true;
			}

			if( ERROR_FILE_NOT_FOUND == GetLastError() )
			{
				g_bDeleteFile = true;
			}

// 			if( g_bDeleteFile == true )
// 			{
// 				if( false == g_theCheckLpk.UnTenioDL() )
// 				{
// 					assert(false);
// 					__asm
// 					{
// 						retn 4
// 					}
// 				}
// 			}
		}

		
		Sleep(2);
		__asm 
		{
			retn 
		}
	}

	__declspec(naked) void DoHookShow()
	{
		Sleep(1);
		__asm retn 
	}


	DWORD	g_dwHookBeginSenceAddr = 0x00474576;
	DWORD	g_dwJmpEndSceneAddr = 0x0047457F;
	// 屏蔽掉所有计算绘制函数
	__declspec(naked) void DoPresent()
	{
		Sleep(0x100);
		__asm
		{
			MOV ECX,DWORD PTR DS:[0x14903A4]
			ADD ESP,0x4
			jmp dword ptr[g_dwJmpEndSceneAddr]
		}

	}


	//3. 黑屏优化
	DWORD	g_dwSetStreamSource = 0x01120E5E;
	DWORD	g_dwSetStreamSourceCount = 0;
	__declspec(naked) void DoHookSetStreamSource()
	{
		__asm
		{
			mov eax, g_dwSetStreamSource
			add eax, 0x0e
			jmp eax
		}

		__asm
		{
			push 0
			MOV ECX,DWORD PTR DS:[EDX+0x190]
			PUSH 0x0
			PUSH EAX
			PUSH EBX
			PUSH EDI
			mov eax, g_dwSetStreamSource
			add eax, 0x0c
			jmp eax
		}
	}

	//4. PeekMessage   反而高了
	DWORD	g_dwHookPeekMessage = 0x00791F95;
	BOOL g_bPeekMessageRet;
	MSG* g_pMsg;
	__declspec(naked) void DoHookPeekMessage()
	{
		__asm
		{
			LEA EAX,DWORD PTR SS:[EBP-0x20]
			mov g_pMsg, eax
			PUSH EAX
			CALL ESI
			mov g_bPeekMessageRet, eax
		}

		if( g_pMsg->message != WM_TIMER )
		{
			Sleep(5);
		}

		__asm
		{
			mov eax, g_bPeekMessageRet
			test eax, eax
			mov eax, g_dwHookPeekMessage
			add eax, 8
			jmp eax
		}
        
	}

	// 时间优化
	typedef DWORD (WINAPI *LPK_GetTickCount)( void );
	LPK_GetTickCount Old_GetTickCount;
	LPK_GetTickCount Jmp_Old_GetTickCount;

	DWORD WINAPI My_GetTickCount( void )
	{
		__asm push eax
		Sleep(1);
		__asm pop eax
		return Jmp_Old_GetTickCount();
	}

	// ZwQueryPerformanceCounter
	typedef ULONG (WINAPI *LPK_ZwQueryPerformanceCounter) (
										OUT PLARGE_INTEGER PerformanceCount,
										OUT PLARGE_INTEGER PerformanceFrequency OPTIONAL
										);
	LPK_ZwQueryPerformanceCounter Old_ZwQueryPerformanceCounter;
	LPK_ZwQueryPerformanceCounter Jmp_Old_ZwQueryPerformanceCounter;
	
	ULONG WINAPI My_ZwQueryPerformanceCounter(
					OUT PLARGE_INTEGER PerformanceCount,
					OUT PLARGE_INTEGER PerformanceFrequency OPTIONAL
					)
	{
		Sleep(1);
		return Jmp_Old_ZwQueryPerformanceCounter(PerformanceCount,PerformanceFrequency);
	}


	//3. GetSystemInfo	无效果，有比原先高的节奏
	typedef VOID (WINAPI *LPK_GetSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPK_GetSystemInfo	Old_GetSystemInfo;
	LPK_GetSystemInfo	Jmp_Old_GetSystemInfo;


	DWORD	g_dwHookGetSystemInfo = 0x7C812E37;

	SYSTEM_INFO* lpSystemInfo;
	__declspec(naked) void DoHookGetSystemInfo()
	{
		__asm
		{
			mov eax,dword ptr [esp+0x04]
			mov lpSystemInfo, eax
		}

		lpSystemInfo->dwNumberOfProcessors = 2;

		__asm
		{
			retn 4
		}
	}


	VOID My_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
	{
		MessageBox(NULL,"","",MB_OK);
		OutputDebugString("[HOOK] GetSystemInfo");
		Jmp_Old_GetSystemInfo(lpSystemInfo);
		
		//lpSystemInfo->dwNumberOfProcessors = 2;
	}

	//4. ZwQuerySystemInformation

	typedef struct
	{
		ULONG Unknown; //Always contains zero
		ULONG MaximumIncrement; //一个时钟的计量单位
		ULONG PhysicalPageSize; //一个内存页的大小
		ULONG NumberOfPhysicalPages; //系统管理着多少个页
		ULONG LowestPhysicalPage; //低端内存页
		ULONG HighestPhysicalPage; //高端内存页
		ULONG AllocationGranularity;
		ULONG LowestUserAddress; //地端用户地址
		ULONG HighestUserAddress; //高端用户地址
		ULONG ActiveProcessors; //激活的处理器
		UCHAR NumberProcessors; //有多少个处理器
	}SYSTEM_BASIC_INFORMATION,*PSYSTEM_BASIC_INFORMATION;

	//
	// System Information Classes.
	//
	typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation,
		SystemProcessorInformation,             // obsolete...delete
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
		SystemNonPagedPoolInformation,
		SystemHandleInformation,
		SystemObjectInformation,
		SystemPageFileInformation,
		SystemVdmInstemulInformation,
		SystemVdmBopInformation,
		SystemFileCacheInformation,
		SystemPoolTagInformation,
		SystemInterruptInformation,
		SystemDpcBehaviorInformation,
		SystemFullMemoryInformation,
		SystemLoadGdiDriverInformation,
		SystemUnloadGdiDriverInformation,
		SystemTimeAdjustmentInformation,
		SystemSummaryMemoryInformation,
		SystemMirrorMemoryInformation,
		SystemPerformanceTraceInformation,
		SystemObsolete0,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemExtendServiceTableInformation,
		SystemPrioritySeperation,
		SystemVerifierAddDriverInformation,
		SystemVerifierRemoveDriverInformation,
		SystemProcessorIdleInformation,
		SystemLegacyDriverInformation,
		SystemCurrentTimeZoneInformation,
		SystemLookasideInformation,
		SystemTimeSlipNotification,
		SystemSessionCreate,
		SystemSessionDetach,
		SystemSessionInformation,
		SystemRangeStartInformation,
		SystemVerifierInformation,
		SystemVerifierThunkExtend,
		SystemSessionProcessInformation,
		SystemLoadGdiDriverInSystemSpace,
		SystemNumaProcessorMap,
		SystemPrefetcherInformation,
		SystemExtendedProcessInformation,
		SystemRecommendedSharedDataAlignment,
		SystemComPlusPackage,
		SystemNumaAvailableMemory,
		SystemProcessorPowerInformation,
		SystemEmulationBasicInformation,
		SystemEmulationProcessorInformation,
		SystemExtendedHandleInformation,
		SystemLostDelayedWriteInformation,
		SystemBigPoolInformation,
		SystemSessionPoolTagInformation,
		SystemSessionMappedViewInformation,
		SystemHotpatchInformation,
		SystemObjectSecurityMode,
		SystemWatchdogTimerHandler,
		SystemWatchdogTimerInformation,
		SystemLogicalProcessorInformation,
		SystemWow64SharedInformation,
		SystemRegisterFirmwareTableInformationHandler,
		SystemFirmwareTableInformation,
		SystemModuleInformationEx,
		SystemVerifierTriageInformation,
		SystemSuperfetchInformation,
		SystemMemoryListInformation,
		SystemFileCacheInformationEx,
		MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
	} SYSTEM_INFORMATION_CLASS; 

	#define MAXIMUM_FILENAME_LENGTH 256

	typedef struct _SYSTEM_MODULE {
		ULONG                Reserved1;
		ULONG                Reserved2;
		PVOID                ImageBaseAddress;
		ULONG                ImageSize;
		ULONG                Flags;
		WORD                 Id;
		WORD                 Rank;
		WORD                 w018;
		WORD                 NameOffset;
		BYTE                 Name[MAXIMUM_FILENAME_LENGTH];
	} SYSTEM_MODULE, *PSYSTEM_MODULE;

	typedef struct _SYSTEM_MODULE_INFORMATION {


		ULONG                ModulesCount;
		SYSTEM_MODULE        Modules[0];
	} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

	typedef struct _CLIENT_ID
	{
		PVOID UniqueProcess;
		PVOID UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;
	typedef LONG       KPRIORITY;

	typedef struct _VM_COUNTERS
	{
		ULONG PeakVirtualSize; //虚拟存储峰值大小；
		ULONG VirtualSize; //虚拟存储大小；
		ULONG PageFaultCount; //页故障数目；
		ULONG PeakWorkingSetSize; //工作集峰值大小；
		ULONG WorkingSetSize; //工作集大小；
		ULONG QuotaPeakPagedPoolUsage; //分页池使用配额峰值；
		ULONG QuotaPagedPoolUsage; //分页池使用配额；
		ULONG QuotaPeakNonPagedPoolUsage;//非分页池使用配额峰值；
		ULONG QuotaNonPagedPoolUsage;//非分页池使用配额；
		ULONG PagefileUsage;//页文件使用情况；
		ULONG PeakPagefileUsage;//页文件使用峰值；
	}VM_COUNTERS,*PVM_COUNTERS;

	typedef struct _SYSTEM_THREADS{
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER CreateTime;
		ULONG WaitTime;
		PVOID StartAddress;
		CLIENT_ID ClientId;
		KPRIORITY Priority;
		LONG BasePriority;
		ULONG ContextSwitches;
		ULONG ThreadState;
		ULONG WaitReason;
		ULONG Reversed;//add by achillis
	} SYSTEM_THREAD_INFORMATION,*PSYSTEM_THREADS;

	typedef struct _SYSTEM_PROCESSES{
		ULONG NextEntryDelta;
		ULONG ThreadCount;
		LARGE_INTEGER Reserved1[3];
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER KernelTime;
		UNICODE_STRING ProcessName;
		KPRIORITY BasePriority;
		ULONG ProcessId;
		ULONG InheritedFromProcessId;
		ULONG HandleCount;
		ULONG SessionId;
		ULONG_PTR PageDirectoryBase;
		VM_COUNTERS VmCounters;
		ULONG PrivatePageCount;// add by achillis
		IO_COUNTERS IoCounters;
		_SYSTEM_THREADS Threads[1];
	} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;


//  typedef struct _SYSTEM_PROCESS_INFORMATION
//  {
//      ULONG NextEntryOffset;
//      ULONG NumberOfThreads;
//      LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
//      ULONG HardFaultCount; // since WIN7
//      ULONG NumberOfThreadsHighWatermark; // since WIN7
//      ULONGLONG CycleTime; // since WIN7
//      LARGE_INTEGER CreateTime;
//      LARGE_INTEGER UserTime;
//      LARGE_INTEGER KernelTime;
//      UNICODE_STRING ImageName;
//      KPRIORITY BasePriority;
//      HANDLE UniqueProcessId;
//      HANDLE InheritedFromUniqueProcessId;
//      ULONG HandleCount;
//      ULONG SessionId;
//      ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
//      SIZE_T PeakVirtualSize;
//      SIZE_T VirtualSize;
//      ULONG PageFaultCount;
//      SIZE_T PeakWorkingSetSize;
//      SIZE_T WorkingSetSize;
//      SIZE_T QuotaPeakPagedPoolUsage;
//      SIZE_T QuotaPagedPoolUsage;
//      SIZE_T QuotaPeakNonPagedPoolUsage;
//      SIZE_T QuotaNonPagedPoolUsage;
//      SIZE_T PagefileUsage;
//      SIZE_T PeakPagefileUsage;
//      SIZE_T PrivatePageCount;
//      LARGE_INTEGER ReadOperationCount;
//      LARGE_INTEGER WriteOperationCount;
//      LARGE_INTEGER OtherOperationCount;
//      LARGE_INTEGER ReadTransferCount;
//      LARGE_INTEGER WriteTransferCount;
//      LARGE_INTEGER OtherTransferCount;
//      SYSTEM_THREAD_INFORMATION Threads[1];
// } SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;


	typedef LONG (WINAPI *LPK_ZwQuerySystemInformation)(
		__in          SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__in_out     PVOID SystemInformation,
		__in          ULONG SystemInformationLength,
		__out_opt    PULONG ReturnLength

		);
	LPK_ZwQuerySystemInformation	Old_ZwQuerySystemInformation;
	LPK_ZwQuerySystemInformation	Jmp_Old_ZwQuerySystemInformation;

	LONG WINAPI My_ZwQuerySystemInformation(
		__in          SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__in_out     PVOID SystemInformation,
		__in          ULONG SystemInformationLength,
		__out_opt    PULONG ReturnLength

		)
	{
// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"[HOOK] ZwQuerySystemInformation = %d ",SystemInformationClass);
// 		OutputDebugString(szPrint);
		LONG status = Jmp_Old_ZwQuerySystemInformation(SystemInformationClass,SystemInformation,SystemInformationLength,ReturnLength);
		if( SystemModuleInformation == SystemInformationClass )
		{
			//OutputDebugString("[HOOK] 遍历模块");

			PSYSTEM_MODULE_INFORMATION pModle = (PSYSTEM_MODULE_INFORMATION)SystemInformation;

			if( pModle != NULL )
			{
				pModle->ModulesCount = 1;
				SystemInformation = NULL;
			}
			else
			{
				//OutputDebugString("[HOOK] 遍历模块 SystemInformation = NULL");
			}
		}
		else if( SystemProcessInformation == SystemInformationClass && status != 0x0C0000004 )
		{
			//MessageBox(NULL,"","",MB_OK);
 			PSYSTEM_PROCESSES pInfo = (PSYSTEM_PROCESSES)SystemInformation;

			while (true)
			{
				if (pInfo->NextEntryDelta == 0 )
				{
					break;
				}

				pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);

				wchar_t* pszProcessName = ((wchar_t*)(&pInfo->ProcessName)+2);

 				if( wcsstr((wchar_t*)*(DWORD*)pszProcessName,L"Console.exe") != 0 )
				{
					//pInfo->NextEntryDelta = 0;
					pInfo->ProcessId = 0;
					__asm
					{
						lea eax, pszProcessName
						mov eax, dword ptr [eax]
						mov dword ptr [eax], 0
					}
					//OutputDebugString("[HOOK] 成功的干掉了一次检测进程");
				}
			}
		}
// 		else if( SystemBasicInformation == SystemInformationClass )
// 		{
// 			OutputDebugString("[HOOK] 获取系统信息");
// 			PSYSTEM_BASIC_INFORMATION pBasic = (PSYSTEM_BASIC_INFORMATION)SystemInformation;
// 			pBasic->NumberProcessors = 4;
// 		}
		return status;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ZwQueryInformationProcess 
	typedef LONG PROCESSINFOCLASS;
	typedef LONG (WINAPI *LPK_ZwQueryInformationProcess)(
		_In_       HANDLE ProcessHandle,
		_In_       PROCESSINFOCLASS ProcessInformationClass,
		_Out_      PVOID ProcessInformation,
		_In_       ULONG ProcessInformationLength,
		_Out_opt_  PULONG ReturnLength
		);
	LPK_ZwQueryInformationProcess Old_ZwQueryInformationProcess;
	LPK_ZwQueryInformationProcess Jmp_Old_ZwQueryInformationProcess;

	LONG WINAPI My_ZwQueryInformationProcess(
		_In_       HANDLE ProcessHandle,
		_In_       PROCESSINFOCLASS ProcessInformationClass,
		_Out_      PVOID ProcessInformation,
		_In_       ULONG ProcessInformationLength,
		_Out_opt_  PULONG ReturnLength
		)
	{
		OutputDebugString("[HOOK] ZwQueryInformationProcess");
		return 0;
		//MessageBox(NULL,"","",MB_OK);
		return Jmp_Old_ZwQueryInformationProcess(ProcessHandle,ProcessInformationClass,ProcessInformation,ProcessInformationLength,ReturnLength);
	}

	//////////////////////////////////////////////////////////////////////////
	// EnumWindows
	typedef BOOL (WINAPI *LPK_EnumWindows)(
							_In_  WNDENUMPROC lpEnumFunc,
							_In_  LPARAM lParam
							);
	LPK_EnumWindows Old_EnumWindows;
	BOOL WINAPI My_EnumWindows(
		_In_  WNDENUMPROC lpEnumFunc,
		_In_  LPARAM lParam
		)
	{
		OutputDebugString("[HOOK] EnumWindows");
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// ZwQueryInformationFile 
	typedef struct _FILE_NAME_INFORMATION
	{
		ULONG FileNameLength;
		WCHAR FileName[MAX_PATH];
	} FILE_NAME_INFORMATION;
	typedef LONG (WINAPI *LPK_ZwQueryInformationFile)(
							_In_   HANDLE FileHandle,
							_Out_  PIO_STATUS_BLOCK IoStatusBlock,
							_Out_  PVOID FileInformation,
							_In_   ULONG Length,
							_In_   FILE_INFORMATION_CLASS FileInformationClass
							);
	LPK_ZwQueryInformationFile Old_ZwQueryInformationFile;
	LPK_ZwQueryInformationFile Jmp_Old_ZwQueryInformationFile;

	LONG WINAPI My_ZwQueryInformationFile(
		_In_   HANDLE FileHandle,
		_Out_  PIO_STATUS_BLOCK IoStatusBlock,
		_Out_  PVOID FileInformation,
		_In_   ULONG Length,
		_In_   FILE_INFORMATION_CLASS FileInformationClass
		)
	{
		//OutputDebugString("[HOOK] 遍历文件");
		LONG status = Jmp_Old_ZwQueryInformationFile(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
// 		FILE_NAME_INFORMATION* pfni = (FILE_NAME_INFORMATION*)FileInformation;
// 		if( !status )
// 		{
// 			pfni->FileName[pfni->FileNameLength/2] = 0;
// 			char szFilePath[MAX_PATH+1] = {0};
// 			char szFullPath[MAX_PATH*2] = {0};
// 			WideCharToMultiByte( CP_ACP, 0, pfni->FileName, -1, szFilePath, sizeof(szFilePath) - 1, NULL, NULL);
// 			wsprintf(szFullPath,"[HOOK] %s", szFilePath);
// 			OutputDebugString(szFullPath);
// 		}
		return status;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CreateMutex

	DWORD	g_dwCreateMutexCount = 0;
	typedef HANDLE (WINAPI *LPK_CreateMutex)( 
						LPSECURITY_ATTRIBUTES lpMutexAttributes, 
						BOOL bInitialOwner, 
						LPCTSTR lpName );

	LPK_CreateMutex Old_CreateMutex;
	LPK_CreateMutex Jmp_Old_CreateMutex;

	HANDLE WINAPI My_CreateMutex( 
		LPSECURITY_ATTRIBUTES lpMutexAttributes, 
		BOOL bInitialOwner, 
		LPCTSTR lpName )
	{
		char szPrint[1024] = {0};
		wsprintf(szPrint,"[HOOK] lpName = %s",lpName);
		OutputDebugString(szPrint);
		if( lpName == NULL )
		{
			char szbuf[64] = {0};
			if( g_dwCreateMutexCount == 0 )
			{
				srand((int)time(0));
				g_dwCreateMutexCount = rand();
			}

			itoa(g_dwCreateMutexCount,szbuf,10);

			wsprintf(szPrint,"[HOOK] 创建对象 = %s",szbuf);
			OutputDebugString(szPrint);
			return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,szbuf);
		}

		if( strcmp(lpName,"__DDrawCheckExclMode__") == 0 )
		{
			char szbuf[64] = {0};
			srand((int)time(0));
			itoa(rand(),szbuf,10);
			strcat(szbuf,"__DDrawCheckExclMode__");

			wsprintf(szPrint,"[HOOK] 创建对象2 = %s",szbuf);
			OutputDebugString(szPrint);
			return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,szbuf);
		}

		if( strcmp(lpName,"__DDrawExclMode__") == 0 )
		{
			char szbuf[64] = {0};
			srand((int)time(0));
			itoa(rand(),szbuf,10);
			strcat(szbuf,"__DDrawExclMode__");

			wsprintf(szPrint,"[HOOK] 创建对象3 = %s",szbuf);
			OutputDebugString(szPrint);
			return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,szbuf);
		}

		if( strcmp(lpName,"DDrawWindowListMutex") == 0 )
		{
			char szbuf[64] = {0};
			srand((int)time(0));
			itoa(rand(),szbuf,10);
			strcat(szbuf,"DDrawWindowListMutex");

			wsprintf(szPrint,"[HOOK] 创建对象4 = %s",szbuf);
			OutputDebugString(szPrint);
			return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,szbuf);
		}

		if( strcmp(lpName,"RasPbFile") == 0 )
		{
			char szbuf[64] = {0};
			srand((int)time(0));
			itoa(rand(),szbuf,10);
			strcat(szbuf,"RasPbFile");

			wsprintf(szPrint,"[HOOK] 创建对象5 = %s",szbuf);
			OutputDebugString(szPrint);
			return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,szbuf);
		}

		return Jmp_Old_CreateMutex(lpMutexAttributes,bInitialOwner,lpName);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OpenFileMappingA
	typedef HANDLE (WINAPI *LPK_OpenFileMappingA)(
									DWORD dwDesiredAccess,  // access mode
									BOOL bInheritHandle,    // inherit flag
									LPCTSTR lpName          // object name
									);
	LPK_OpenFileMappingA	Old_OpenFileMappingA;
	LPK_OpenFileMappingA	Jmp_Old_OpenFileMappingA;

	HANDLE WINAPI My_OpenFileMappingA(
								DWORD dwDesiredAccess,  // access mode
								BOOL bInheritHandle,    // inherit flag
								LPCTSTR lpName          // object name
								)
	{
		OutputDebugString("[HOOK] My_OpenFileMappingA");
		return Jmp_Old_OpenFileMappingA(dwDesiredAccess,bInheritHandle,"123456");
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OpenFileMappingW
	typedef HANDLE (WINAPI *LPK_OpenFileMappingW)(
									DWORD dwDesiredAccess,  // access mode
									BOOL bInheritHandle,    // inherit flag
									LPCTSTR lpName          // object name
									);
	LPK_OpenFileMappingW	Old_OpenFileMappingW;
	LPK_OpenFileMappingW	Jmp_Old_OpenFileMappingW;

	HANDLE WINAPI My_OpenFileMappingW(
		DWORD dwDesiredAccess,  // access mode
		BOOL bInheritHandle,    // inherit flag
		LPCTSTR lpName          // object name
		)
	{
		OutputDebugString("[HOOK] My_OpenFileMappingW");
		return Jmp_Old_OpenFileMappingW(dwDesiredAccess,bInheritHandle,"123456");
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ZwCreateMutant 
	typedef ULONG (__stdcall *LPK_ZwCreateMutant)(
									OUT PHANDLE MutantHandle,
									IN ACCESS_MASK DesiredAccess,
									IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
									IN BOOLEAN InitialOwner );
	LPK_ZwCreateMutant Old_ZwCreateMutant;
	LPK_ZwCreateMutant Jmp_Old_ZwCreateMutant;

	ULONG __stdcall My_ZwCreateMutant(
									OUT PHANDLE MutantHandle,
									IN ACCESS_MASK DesiredAccess,
									IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
									IN BOOLEAN InitialOwner )
	{
		//OutputDebugString("[HOOK] My_ZwCreateMutant");
		Jmp_Old_ZwCreateMutant(MutantHandle,DesiredAccess,ObjectAttributes,InitialOwner);
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////
	// ZwOpenFile
	typedef LONG (WINAPI *LPK_ZwOpenFile)(
										OUT PHANDLE  FileHandle,
										IN ACCESS_MASK  DesiredAccess,
										IN POBJECT_ATTRIBUTES  ObjectAttributes,
										OUT PIO_STATUS_BLOCK  IoStatusBlock,
										IN ULONG  ShareAccess,
										IN ULONG  OpenOptions
										);
	LPK_ZwOpenFile Old_ZwOpenFile;
	LPK_ZwOpenFile Jmp_Old_ZwOpenFile;

	LONG WINAPI	My_ZwOpenFile(
		OUT PHANDLE  FileHandle,
		IN ACCESS_MASK  DesiredAccess,
		IN POBJECT_ATTRIBUTES  ObjectAttributes,
		OUT PIO_STATUS_BLOCK  IoStatusBlock,
		IN ULONG  ShareAccess,
		IN ULONG  OpenOptions
		)
	{
		wchar_t szPrint[300] = {0};
		swprintf_s(szPrint,L"[HOOK] ZwOpenFile : %s",ObjectAttributes->ObjectName->Buffer);
// 		if( wcsstr(szPrint,L"d3d9.dll") != 0 )
// 		{
// 			//OutputDebugString("[HOOK] 游戏正在检测d3d9.dll ==> 1");
// 			ObjectAttributes->ObjectName->Buffer = L"\\??\\C:\\WINDOWS\\system32\\d3d9.dll";
// 			ObjectAttributes->ObjectName->Length = wcslen(ObjectAttributes->ObjectName->Buffer)*2;
// 		}
// 		if( wcsstr(szPrint,L"Game.dll") != 0 )
// 		{
// 			//OutputDebugString("[HOOK] 游戏正在检测Game.dll ==> 2");
// 			ObjectAttributes->ObjectName->Buffer = L"\\??\\C:\\WINDOWS\\system32\\d3d9.dll";
// 			ObjectAttributes->ObjectName->Length = wcslen(ObjectAttributes->ObjectName->Buffer)*2;
// 		}
		//OutputDebugStringW(szPrint);
		return Jmp_Old_ZwOpenFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,ShareAccess,OpenOptions);
	}


	typedef LONG  (WINAPI *LPK_ZwCreateFile)(
									OUT PHANDLE  FileHandle,
									IN ACCESS_MASK  DesiredAccess,
									IN POBJECT_ATTRIBUTES  ObjectAttributes,
									OUT PIO_STATUS_BLOCK  IoStatusBlock,
									IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
									IN ULONG  FileAttributes,
									IN ULONG  ShareAccess,
									IN ULONG  CreateDisposition,
									IN ULONG  CreateOptions,
									IN PVOID  EaBuffer  OPTIONAL,
									IN ULONG  EaLength
									);
 	LPK_ZwCreateFile Old_ZwCreateFile;
 	LPK_ZwCreateFile Jmp_Old_ZwCreateFile;

	LONG WINAPI My_ZwCreateFile(
									OUT PHANDLE  FileHandle,
									IN ACCESS_MASK  DesiredAccess,
									IN POBJECT_ATTRIBUTES  ObjectAttributes,
									OUT PIO_STATUS_BLOCK  IoStatusBlock,
									IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
									IN ULONG  FileAttributes,
									IN ULONG  ShareAccess,
									IN ULONG  CreateDisposition,
									IN ULONG  CreateOptions,
									IN PVOID  EaBuffer  OPTIONAL,
									IN ULONG  EaLength
									)
	{
// 		wchar_t szPrint[300] = {0};
// 		swprintf(szPrint,L"[HOOK] ZwCreateFile : %s",ObjectAttributes->ObjectName->Buffer);
// 		OutputDebugStringW(szPrint);
		wchar_t szPrint[300] = {0};
		swprintf_s(szPrint,L"[HOOK] ZwOpenFile : %s",ObjectAttributes->ObjectName->Buffer);
// 		if( wcsstr(szPrint,L"d3d9.dll") != 0 )
// 		{
// 			//OutputDebugString("[HOOK] 游戏正在检测d3d9.dll ==> 3");
// 			ObjectAttributes->ObjectName->Buffer = L"\\??\\C:\\WINDOWS\\system32\\d3d9.dll";
// 			ObjectAttributes->ObjectName->Length = wcslen(ObjectAttributes->ObjectName->Buffer)*2;
// 		}
// 		if( wcsstr(szPrint,L"Game.dll") != 0 )
// 		{
// 			//OutputDebugString("[HOOK] 游戏正在检测Game.dll ==> 4");
// 			ObjectAttributes->ObjectName->Buffer = L"\\??\\C:\\WINDOWS\\system32\\d3d9.dll";
// 			ObjectAttributes->ObjectName->Length = wcslen(ObjectAttributes->ObjectName->Buffer)*2;
// 		}
		return Jmp_Old_ZwCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,ShareAccess,CreateDisposition,CreateOptions,EaBuffer,EaLength);
	}

	//LdrGetDllHandle
	typedef LONG (WINAPI *LPK_LdrGetDllHandle)(
				IN PWORD                pwPath OPTIONAL,
				IN PVOID                Unused OPTIONAL,
				IN PUNICODE_STRING      ModuleFileName,
				OUT PHANDLE             pHModule );
	LPK_LdrGetDllHandle Old_LdrGetDllHandle;
	LPK_LdrGetDllHandle Jmp_Old_LdrGetDllHandle;

	LONG WINAPI	My_LdrGetDllHandle(
				IN PWORD                pwPath OPTIONAL,
				IN PVOID                Unused OPTIONAL,
				IN PUNICODE_STRING      ModuleFileName,
				OUT PHANDLE             pHModule )
	{
		HANDLE hdll = NULL;
	
		BOOL ret = Jmp_Old_LdrGetDllHandle(pwPath,Unused,ModuleFileName,pHModule);

		if( wcsstr(ModuleFileName->Buffer,L"d3d9.dll") != 0 )
		{
			hdll = ::LoadLibrary("C:\\WINDOWS\\system32\\d3d9.dll");
			(HANDLE)*pHModule = hdll;
		}

		if( wcsstr(ModuleFileName->Buffer,L"lpk.dll") != 0 )
		{
			hdll = ::LoadLibrary("C:\\WINDOWS\\system32\\lpk.dll");
			(HANDLE)*pHModule = hdll;
		}

		return ret;
	}

	// 加载原始模块
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH] /*= "D:\\FIFA Online 3\\lpk1.dll"*/;
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\lpk"));
		m_hModule = LoadLibrary(tzPath);

		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		}

		char szProcessName[1024] = {0};
		GetProcessHostName(szProcessName);

//#ifdef _DEBUG
		char szPrint[256] = {0};
		wsprintf(szPrint,"[HOOK] 进程:%s !!!",szProcessName);
		OutputDebugString(szPrint);
//#endif

		

 		if( strcmp(szProcessName,"fifalauncher.exe") == 0 || strcmp(szProcessName,"fifazf.exe") == 0 /*|| strcmp(szProcessName,"Client.exe") == 0*/)
 		{
			VMP_BEGIN;

			g_theCheckServer.Close();

			CheckLpk	theCheckLpk;
			if( theCheckLpk.LpkOpenEvent(VMProtectDecryptStringA("E52CAB0D-A39D-4858-8344-8A6364897A83")) )
			{
				//OutputDebugString("[HOOK] 打开事件对象成功");

				char szOutData[0x1024] = {0};
				if( theCheckLpk.LpkOpenFileMapping(VMProtectDecryptStringA("65940093-38FE-428a-85E9-4F6664C398A4"),szOutData) )
				{
					if( strcmp(szOutData,VMProtectDecryptStringA("587C0C8C-8D7C-41c7-B7B3-CBB2CD2F2055")) == 0 )
					{
						//OutputDebugString("[HOOK] 共享内存验证通过");

						Old_CreateToolhelp32Snapshot = (LPK_CreateToolhelp32Snapshot)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateToolhelp32Snapshot");

						if( Old_CreateToolhelp32Snapshot == 0 )
						{
							OutputDebugString("[HOOK] 获取 CreateToolhelp32Snapshot 失败");
						}

						Old_TerminateProcess = (LPK_TerminateProcess)GetProcAddress(LoadLibrary("kernel32.dll"),"TerminateProcess"); 

						if( Old_TerminateProcess == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_TerminateProcess 失败");
						}

						Old_EnumProcessModules = (LPK_EnumProcessModules)GetProcAddress(LoadLibrary("PSAPI.dll"),"EnumProcessModules"); 

						if( Old_EnumProcessModules == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_EnumProcessModules 失败");
						}

						Old_EnumProcesses = (LPK_EnumProcesses)GetProcAddress(LoadLibrary("PSAPI.dll"),"EnumProcesses"); 

						if( Old_EnumProcesses == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_EnumProcesses 失败");
						}

						Old_NtCreateFile = (LPK_NtCreateFile)GetProcAddress(LoadLibrary("ntdll.dll"),"NtCreateFile");

						if( Old_NtCreateFile == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_NtCreateFile 失败");
							return false;
						}
// 
// 						Old_OpenProcess = (LPK_OpenProcess)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenProcess");
// 
// 						if( Old_OpenProcess == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_OpenProcess 失败");
// 							return false;
// 						}

						Old_GetTickCount = (LPK_GetTickCount)GetProcAddress(LoadLibrary("kernel32.dll"),"GetTickCount");

						if( Old_GetTickCount == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_GetTickCount 失败");
						}

// 						Old_FindWindowA = (LPK_FindWindowA)GetProcAddress(LoadLibrary("User32.dll"),"FindWindowA");
// 
// 						if( Old_FindWindowA == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_FindWindowA 失败");
// 							//return false;
// 						}

// 						Old_FindWindowW = (LPK_FindWindowW)GetProcAddress(LoadLibrary("User32.dll"),"FindWindowW");
// 
// 						if( Old_FindWindowW == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_FindWindowW 失败");
// 							//return false;
// 						}

						Old_ZwCreateMutant = (LPK_ZwCreateMutant)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwCreateMutant");

						if( Old_ZwCreateMutant == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwCreateMutant 失败");
						}

						Old_ZwQueryPerformanceCounter = (LPK_ZwQueryPerformanceCounter)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwQueryPerformanceCounter");

						if( Old_ZwQueryPerformanceCounter == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwQueryPerformanceCounter 失败");
						}

						Old_ZwQuerySystemInformation = (LPK_ZwQuerySystemInformation)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwQuerySystemInformation"); 

						if( Old_ZwQuerySystemInformation == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwQuerySystemInformation 失败");
						}


// 						Old_OpenFileMappingW = (LPK_OpenFileMappingW)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenFileMappingW");
// 
// 						if( Old_OpenFileMappingW == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_OpenFileMappingW 失败");
// 						}


// 						Old_CreateMutex = (LPK_CreateMutex)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateMutexA");
// 
// 						if( Old_CreateMutex == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_CreateMutex 失败");
// 						}

// 						Old_OpenFileMappingA = (LPK_OpenFileMappingA)GetProcAddress(LoadLibrary("kernel32.dll"),"OpenFileMappingA");
// 
// 						if( Old_OpenFileMappingA == 0 )
// 						{
// 							OutputDebugString("[HOOK] 获取 Old_OpenFileMappingA 失败");
// 						}

						Old_EnumWindows = (LPK_EnumWindows)GetProcAddress(LoadLibrary("User32.dll"),"EnumWindows");

						if( Old_EnumWindows == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_EnumWindows 失败");
							//return false;
						}

						Old_ZwQueryInformationFile  = (LPK_ZwQueryInformationFile)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwQueryInformationFile");

						if( Old_ZwQueryInformationFile  == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwQueryInformationFile  失败");
							//return false;
						}

						Old_DeviceIoControl = (LPK_DeviceIoControl)GetProcAddress(LoadLibrary("kernel32.dll"),"DeviceIoControl");

						if( Old_DeviceIoControl == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_DeviceIoControl 失败");
						}


						Old_ZwOpenFile = (LPK_ZwOpenFile)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwOpenFile");

						if( Old_ZwOpenFile == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwOpenFile 失败");
						}

						Old_ZwCreateFile = (LPK_ZwCreateFile)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwCreateFile");

						if( Old_ZwCreateFile == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwCreateFile 失败");
						}

						Old_ZwQueryInformationProcess = (LPK_ZwQueryInformationProcess)GetProcAddress(LoadLibrary("ntdll.dll"),"ZwQueryInformationProcess");

						if( Old_ZwQueryInformationProcess == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_ZwQueryInformationProcess 失败");
						}


						Old_LdrGetDllHandle = (LPK_LdrGetDllHandle)GetProcAddress(LoadLibrary("ntdll.dll"),"LdrGetDllHandle");

						if( Old_LdrGetDllHandle == 0 )
						{
							OutputDebugString("[HOOK] 获取 Old_LdrGetDllHandle 失败");
						}

						
// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_LdrGetDllHandle,My_LdrGetDllHandle,&(PDETOUR_TRAMPOLINE&)Jmp_Old_LdrGetDllHandle,0, 0);
// 						DetourTransactionCommit();

// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_ZwQueryInformationProcess,My_ZwQueryInformationProcess,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwQueryInformationProcess,0, 0);
// 						DetourTransactionCommit();

// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_ZwCreateFile,My_ZwCreateFile,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwCreateFile,0, 0);
// 						DetourTransactionCommit();
// 
// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_ZwOpenFile,My_ZwOpenFile,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwOpenFile,0, 0);
// 						DetourTransactionCommit();


// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_DeviceIoControl,My_DeviceIoControl,&(PDETOUR_TRAMPOLINE&)Jmp_Old_DeviceIoControl,0, 0);
// 						DetourTransactionCommit();
						

// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttachEx(&(PVOID&)Old_NtCreateFile,My_ZwQueryInformationFile,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwQueryInformationFile,0, 0);
// 						DetourTransactionCommit();

// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttach(&(PVOID&)Old_EnumWindows,My_EnumWindows);
// 						DetourTransactionCommit();


						DetourTransactionBegin();
						DetourUpdateThread(GetCurrentThread());
						DetourAttach(&(PVOID&)Old_CreateToolhelp32Snapshot,My_CreateToolhelp32Snapshot);
						DetourTransactionCommit();

// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						//DetourAttach(&(PVOID&)Old_TerminateProcess,My_TerminateProcess);
// 						DetourAttachEx(&(PVOID&)Old_TerminateProcess,My_TerminateProcess,&(PDETOUR_TRAMPOLINE&)Jmp_Old_TerminateProcess,0, 0);
// 						DetourTransactionCommit();

						DetourTransactionBegin();
						DetourUpdateThread(GetCurrentThread());
						DetourAttach(&(PVOID&)Old_EnumProcesses,My_EnumProcesses);
						DetourTransactionCommit();

						DetourTransactionBegin();
						DetourUpdateThread(GetCurrentThread());
						DetourAttach(&(PVOID&)Old_EnumProcessModules,My_EnumProcessModules);
						DetourTransactionCommit();

						DetourTransactionBegin();
						DetourUpdateThread(GetCurrentThread());
						DetourAttachEx(&(PVOID&)Old_NtCreateFile,My_NtCreateFile,&(PDETOUR_TRAMPOLINE&)Jmp_Old_NtCreateFile,0, 0);
						DetourTransactionCommit();
// 
// 						DetourTransactionBegin();
// 						DetourUpdateThread(GetCurrentThread());
// 						DetourAttach(&(PVOID&)Old_OpenProcess,My_OpenProcess);
// 						DetourTransactionCommit();

	

						DetourTransactionBegin();
						DetourUpdateThread(GetCurrentThread());
						DetourAttachEx(&(PVOID&)Old_ZwQuerySystemInformation,My_ZwQuerySystemInformation,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwQuerySystemInformation,0, 0);
						DetourTransactionCommit();
	
						if( strcmp(szProcessName,"fifazf.exe") == 0 )
						{
// 							if( Old_FindWindowA )
// 							{
// 								DetourTransactionBegin();
// 								DetourUpdateThread(GetCurrentThread());
// 								DetourAttach(&(PVOID&)Old_FindWindowA,My_FindWindowA);
// 								DetourTransactionCommit();
// 							}
// 
// 							if( Old_FindWindowW )
// 							{
// 								DetourTransactionBegin();
// 								DetourUpdateThread(GetCurrentThread());
// 								DetourAttach(&(PVOID&)Old_FindWindowW,My_FindWindowW);
// 								DetourTransactionCommit();
// 							}

// 							if( Old_CreateMutex )
// 							{
// 								DetourTransactionBegin();
// 								DetourUpdateThread(GetCurrentThread());
// 								DetourAttachEx(&(PVOID&)Old_CreateMutex,My_CreateMutex,&(PDETOUR_TRAMPOLINE&)Jmp_Old_CreateMutex,0, 0);
// 								DetourTransactionCommit();
// 							}

							if( Old_ZwCreateMutant )
							{
								DetourTransactionBegin();
								DetourUpdateThread(GetCurrentThread());
								DetourAttachEx(&(PVOID&)Old_ZwCreateMutant,My_ZwCreateMutant,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwCreateMutant,0, 0);
								DetourAttach(&(PVOID&)Old_ZwCreateMutant,My_ZwCreateMutant);
								DetourTransactionCommit();
							}				
						}



// 						if( Old_OpenFileMappingA )
// 						{
// 							DetourTransactionBegin();
// 							DetourUpdateThread(GetCurrentThread());
// 							DetourAttachEx(&(PVOID&)Old_OpenFileMappingA,My_OpenFileMappingA,&(PDETOUR_TRAMPOLINE&)Jmp_Old_OpenFileMappingA,0, 0);
// 							DetourTransactionCommit();
// 						}
// 
// 						if( Old_OpenFileMappingW )
// 						{
// 							DetourTransactionBegin();
// 							DetourUpdateThread(GetCurrentThread());
// 							DetourAttachEx(&(PVOID&)Old_OpenFileMappingW,My_OpenFileMappingW,&(PDETOUR_TRAMPOLINE&)Jmp_Old_OpenFileMappingW,0, 0);
// 							DetourTransactionCommit();
// 						}



						if( strcmp(szProcessName,"fifazf.exe") == 0  )
						{
							////////////////////////////////////////////////////////////////////////////
							// CPU 优化
// 							Hook(g_dwHookDoDownCpu,DoHookTask);
// 							Hook(g_dwHookPresent,DoHookPresent);
// 							Hook(g_dwSetStreamSource,DoHookSetStreamSource);

// 							__asm
// 							{
// 								mov eax, 0x00C8340A
// 								mov byte ptr [eax], 0x90
// 								add eax, 1
// 								mov byte ptr [eax], 0x90
// 								add eax, 7
// 								mov byte ptr [eax], 0x90
// 								add eax, 1
// 								mov byte ptr [eax], 0x90
// 							}

							__asm 
							{
								mov eax, 0x0111C110
								mov byte ptr [eax], 0xC3		// SetStreamSource
							}


// 							__asm
// 							{
// 								mov eax, 0x0111C230
// 								mov byte ptr [eax], 0xC3		// 隐藏人物绘制
// 							}

							__asm
 							{
// 								mov eax, 0x00C22AD0				// 隐藏人物绘制
// 								mov byte ptr [eax], 0xC2
// 								inc eax
// 								mov byte ptr [eax], 0x04
// 								inc eax
// 								mov byte ptr [eax], 0x00

// 								mov eax, 0x00C8BD20				// 相机固定不动
// 								mov byte ptr [eax], 0xC2
// 								inc eax
// 								mov byte ptr [eax], 0x04
// 								inc eax
// 								mov byte ptr [eax], 0x00

 							}

							// 0047EB00      6A 50           PUSH 0x50		game.dll 实现此优化
							//Hook(0x0047EB00/*0x005705C5*/,DoHookPresent1);		

							Hook(0x0078BC57,DoHookTask1);				// 主线程轻微优化
							//Hook(g_dwHookBeginSenceAddr,DoPresent);				// 011103D0                 8B41 0C                    MOV EAX,DWORD PTR DS:[ECX+0xC]





							// 00570540      53              PUSH EBX
							//Hook(0x00570540,DoHookPresent2);

// 							if( Old_GetTickCount )
// 							{
// 								Old_GetTickCount = (LPK_GetTickCount)((DWORD)Old_GetTickCount+0x05);
// 								DetourTransactionBegin();
// 								DetourUpdateThread(GetCurrentThread());
// 								DetourAttachEx(&(PVOID&)Old_GetTickCount,My_GetTickCount,&(PDETOUR_TRAMPOLINE&)Jmp_Old_GetTickCount,0, 0);
// 								DetourTransactionCommit();
// 							}
						
// 							if( Old_ZwQueryPerformanceCounter )
// 							{
// 								DetourTransactionBegin();
// 								DetourUpdateThread(GetCurrentThread());
// 								DetourAttachEx(&(PVOID&)Old_ZwQueryPerformanceCounter,My_ZwQueryPerformanceCounter,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwQueryPerformanceCounter,0, 0);
// 								DetourTransactionCommit();
// 							}
							



// 
// 							Old_GetSystemInfo = (LPK_GetSystemInfo)GetProcAddress(LoadLibrary("kernel32.dll"),"GetSystemInfo"); 
// 
// 							if( Old_GetSystemInfo == 0 )
// 							{
// 								MessageBox(NULL,"获取 Old_GetSystemInfo 失败","失败提示",MB_OK);
// 								return false;
// 							}
// 
// 
// 							DetourTransactionBegin();
// 							DetourUpdateThread(GetCurrentThread());
// 							DetourAttachEx(&(PVOID&)Old_GetSystemInfo,My_GetSystemInfo,&(PDETOUR_TRAMPOLINE&)Jmp_Old_GetSystemInfo,0, 0);
// 							DetourTransactionCommit();
						}
					}
				}
			}

			VMP_END;
 		}

		return (m_hModule != NULL);	
	}

	// 释放原始模块
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// 获取原始函数地址
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
// 		char szPrint[256] = {0};
// 		sprintf_s(szPrint,"[HOOK] %s",pszProcName);
// 		OutputDebugString(szPrint);
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				wsprintf(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace AheadLib;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		return Load();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkInitialize(void)
{
	GetAddress("LpkInitialize");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkTabbedTextOut(void)
{
	GetAddress("LpkTabbedTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkDllInitialize(void)
{
	GetAddress("LpkDllInitialize");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkDrawTextEx(void)
{
	GetAddress("LpkDrawTextEx");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkEditControl(void)
{
	GetAddress("LpkEditControl");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkExtTextOut(void)
{
	GetAddress("LpkExtTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkGetCharacterPlacement(void)
{
	GetAddress("LpkGetCharacterPlacement");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkGetTextExtentExPoint(void)
{
	GetAddress("LpkGetTextExtentExPoint");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkPSMTextOut(void)
{
	GetAddress("LpkPSMTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkUseGDIWidthCache(void)
{
	GetAddress("LpkUseGDIWidthCache");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_ftsWordBreak(void)
{
	GetAddress("ftsWordBreak");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
