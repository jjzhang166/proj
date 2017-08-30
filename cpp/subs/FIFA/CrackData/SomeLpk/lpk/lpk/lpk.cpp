
// bp DialogBoxParamA

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include <stdio.h>
#include <assert.h>
#include <Windows.h>
#include <ntsecapi.h>
#include "Path.h"
#include "detours.h"
#include "header.h"
#include <string.h>
#include "../command/reversalsock.h"
#include <CD_CommonFiles/CD_Updates.h>
#include "../protect/Protect.h"
#include "../rootkit/rootkit.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;


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
HANDLE GetProcessKernelObject(DWORD ProcessId);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl

#define EVENT_TYPE	DWORD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hook(DWORD dwHookAddr,LPVOID pfun)
{
	try
	{
		unsigned char oldcode;
		ReadProcessMemory((void*)-1,(void*)dwHookAddr,&oldcode,1,NULL);
		unsigned char code = 0xe9;

		if( oldcode == code )
		{
			return ;
		}
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



//////////////////////////////////////////////////////////////////////////////////////////////////
// CreateToolhelp32Snapshot
typedef	HANDLE (WINAPI *LPK_CreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID );
LPK_CreateToolhelp32Snapshot  Old_CreateToolhelp32Snapshot;

HANDLE WINAPI My_CreateToolhelp32Snapshot( 
	DWORD dwFlags, 
	DWORD th32ProcessID )
{
	//OutputDebugString("[LOG] My_CreateToolhelp32Snapshot");

	return INVALID_HANDLE_VALUE;
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
	//OutputDebugString("[LOG] My_EnumProcesses");
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
	//OutputDebugString("[LOG] My_EnumProcessModules");
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// NtCreateFile

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

DWORD g_dwReal_Jmp_Old_NtCreateFile;
_declspec(naked) void Real_Jmp_Old_NtCreateFile()
{
	__asm
	{
		MOV EAX,0x52
		jmp dword ptr[g_dwReal_Jmp_Old_NtCreateFile]
	}
}

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
	if( ObjectAttributes->ObjectName != NULL )
	{
		if( ObjectAttributes->ObjectName->Length > 0 )
		{
// 			wchar_t wszPrint[256] = {0};
// 			swprintf_s(wszPrint,L"[LOG] file name = %s\r\n",ObjectAttributes->ObjectName->Buffer);
// 			OutputDebugStringW(wszPrint);
		}
	}
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
				//OutputDebugString("[LOG] My_NtCreateFile");
				return Jmp_Old_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,FILE_SHARE_READ | FILE_SHARE_WRITE,CreateDisposition,CreateOptions,EaBuffer,EaLength);
			}
		}
	}

	//OutputDebugString("[LOG] Old_NtCreateFile");
	return Jmp_Old_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,FILE_SHARE_READ | FILE_SHARE_WRITE,CreateDisposition,CreateOptions,EaBuffer,EaLength);
}

//4. ZwQuerySystemInformation
typedef LONG (WINAPI *LPK_ZwQuerySystemInformation)(
	__in          SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__in_out     PVOID SystemInformation,
	__in          ULONG SystemInformationLength,
	__out_opt    PULONG ReturnLength

	);
LPK_ZwQuerySystemInformation	Old_ZwQuerySystemInformation;
LPK_ZwQuerySystemInformation	Jmp_Old_ZwQuerySystemInformation;

DWORD g_dwReal_Jmp_Old_ZwQuerySystemInformation;
_declspec(naked) void Real_Jmp_Old_ZwQuerySystemInformation()
{
	__asm
	{
		MOV EAX,0x33
		jmp dword ptr[g_dwReal_Jmp_Old_ZwQuerySystemInformation]
	}
}

bool KillTenSLX();

LONG WINAPI My_ZwQuerySystemInformation(
	__in          SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__in_out     PVOID SystemInformation,
	__in          ULONG SystemInformationLength,
	__out_opt    PULONG ReturnLength

	)
{
// 	static int i = 0;
// 	char szPrint[256] = {0};
// 	wsprintf(szPrint,"[LOG] ZwQuerySystemInformation = %d Index = %d",SystemInformationClass,i++);
// 	OutputDebugString(szPrint);

	KillTenSLX();

	LONG status = Jmp_Old_ZwQuerySystemInformation(SystemInformationClass,SystemInformation,SystemInformationLength,ReturnLength);
	if( SystemModuleInformation == SystemInformationClass )
	{
		//OutputDebugString("[LOG] 遍历模块");

		PSYSTEM_MODULE_INFORMATION pModle = (PSYSTEM_MODULE_INFORMATION)SystemInformation;

		if( pModle != NULL )
		{
			pModle->ModulesCount = 1;
			SystemInformation = NULL;
		}
		else
		{
			//OutputDebugString("[LOG] 遍历模块 SystemInformation = NULL");
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
				//OutputDebugString("[LOG] 成功的干掉了一次检测进程");
			}
		}
	}
	// 		else if( SystemBasicInformation == SystemInformationClass )
	// 		{
	// 			OutputDebugString("[LOG] 获取系统信息");
	// 			PSYSTEM_BASIC_INFORMATION pBasic = (PSYSTEM_BASIC_INFORMATION)SystemInformation;
	// 			pBasic->NumberProcessors = 4;
	// 		}
	return status;
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

DWORD g_dwReal_Jmp_Old_ZwCreateMutant;
_declspec(naked) void Real_Jmp_Old_ZwCreateMutant()
{
	__asm
	{
		MOV EAX,0x9A
		jmp dword ptr[g_dwReal_Jmp_Old_ZwCreateMutant]
	}
}

ULONG __stdcall My_ZwCreateMutant(
	OUT PHANDLE MutantHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN BOOLEAN InitialOwner )
{
	Jmp_Old_ZwCreateMutant(MutantHandle,DesiredAccess,ObjectAttributes,InitialOwner);
	return 0;
}

bool	g_bDeleteFile = true;			// 关闭Check检测

#include   "Tlhelp32.h"

typedef   enum   _THREADINFOCLASS   { 
	ThreadBasicInformation, 
	ThreadTimes, 
	ThreadPriority, 
	ThreadBasePriority, 
	ThreadAffinityMask, 
	ThreadImpersonationToken, 
	ThreadDescriptorTableEntry, 
	ThreadEnableAlignmentFaultFixup, 
	ThreadEventPair_Reusable, 
	ThreadQuerySetWin32StartAddress, 
	ThreadZeroTlsCell, 
	ThreadPerformanceCount, 
	ThreadAmILastThread, 
	ThreadIdealProcessor, 
	ThreadPriorityBoost, 
	ThreadSetTlsArrayAddress, 
	ThreadIsIoPending, 
	ThreadHideFromDebugger, 
	ThreadBreakOnTermination, 
	MaxThreadInfoClass 
}   THREADINFOCLASS;
typedef HANDLE (__stdcall *OPENTHREAD) (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);
typedef LONG (__stdcall *_pfnZwQueryInformationThread) (
	IN   HANDLE   ThreadHandle, 
	IN   THREADINFOCLASS   ThreadInformationClass, 
	OUT   PVOID   ThreadInformation, 
	IN   ULONG   ThreadInformationLength, 
	OUT   PULONG   ReturnLength   OPTIONAL 
	);
OPENTHREAD lpOpenThread;

_pfnZwQueryInformationThread ZwQueryInformationThread;

PVOID ShowThreadInfo(DWORD dwThreadPid) 
{
	PVOID lpStartAddr = NULL; 
	HANDLE	hThread = NULL;     
	hThread = lpOpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadPid);
	if (hThread == NULL) 
		return FALSE;

	ZwQueryInformationThread(hThread,
		ThreadQuerySetWin32StartAddress,   
		&lpStartAddr,   
		sizeof(lpStartAddr),   
		NULL);

	DWORD dwThreadAddr = (DWORD)lpStartAddr;
	dwThreadAddr &= 0xFFFF;

	//0x6359 非法线程
	if ( dwThreadAddr == 0x6359 || dwThreadAddr == 0xBE70 )
	{
#ifdef _DEBUG_LOG_
		OutputDebugStringA("[LOG] 我要杀死你1!!!");
#endif
		
		TerminateThread(hThread,0);
	}

	if ( dwThreadAddr == 0x6100 )
	{
#ifdef _DEBUG_LOG_
		OutputDebugStringA("[LOG] 我要杀死你2!!!");
#endif

		TerminateThread(hThread,0);
	}

	CloseHandle(hThread);
	return lpStartAddr;
}

BOOL KillCheckThread()
{
	ZwQueryInformationThread=(_pfnZwQueryInformationThread)GetProcAddress(LoadLibraryA("ntdll.dll"),"ZwQueryInformationThread");
	lpOpenThread=(OPENTHREAD)GetProcAddress(LoadLibraryA("kernel32.dll"),"OpenThread");

	PVOID lpThreadAddr = NULL;
	DWORD dwThreadPid = 0;
	HANDLE hThreadShot=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
	THREADENTRY32 *tagThreadInfo = new THREADENTRY32;
	tagThreadInfo->dwSize = sizeof(THREADENTRY32);

	while(Thread32Next(hThreadShot,tagThreadInfo)!=FALSE)
	{
		if(GetCurrentProcessId()==tagThreadInfo->th32OwnerProcessID)
		{
			lpThreadAddr = ShowThreadInfo(tagThreadInfo->th32ThreadID);
			// 			dwThreadPid = tagThreadInfo->th32ThreadID;
			// 			int dwThreadAddr = (int)lpThreadAddr;
		}     
	}
	CloseHandle(hThreadShot);
	return TRUE;
}

DWORD g_dwGameMainThreadSleepHook = 0;
DWORD g_dwGameSetStreamSourceAddr = 0;


void MainThreadKillTenSlX()
{
	try
	{
		KillCheckThread();
	}
	catch (...)
	{
#ifdef _DEBUG_LOG_
		OutputDebugStringA("[LOG] seh error");
#endif
	}
}

// 降低主线程的CPU
__declspec(naked) void DoHookTask1()
{
	__asm
	{
		pushfd
		pushad
	}
	Sleep(2);

	__asm 
	{
		popad
		popfd
		retn 
	}
}

#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___		0x00830AA0
#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET1___	0x44
#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET2___	0x50
#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET3___	0x5C
#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET4___	0x68


_declspec(naked) void HookRevampTime()
{
	__asm
	{
		MOV ECX,EDI
		FSTP DWORD PTR DS:[ESI+0x20]

		cmp DWORD PTR [ECX+0x44], 20
		ja UN_REVAMP
		// 从上半场的30分钟左右开始
		add dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET1___], 0x00004CAC-0x1800
		mov dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET2___], 0x00003FA2-0x1800
		add dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET3___], 0x00003AAE-0x1800
		add dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET4___], 0x00004A5E-0x1800

		// 下半场，直接到90分钟结束
// 		mov dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET1___], 0x0000A7A0
// 		mov dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET2___], 0x00003E94
// 		mov dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET3___], 0x00003A46
// 		mov dword ptr [ECX+UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET4___], 0x000098E2

UN_REVAMP:
		mov eax, UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___
		add eax, 5
		jmp eax
	}
}


bool KillTenSLX()
{
	static int nCount = 0;

	static bool bKillTenSLXFlag = false;

	if( bKillTenSLXFlag == true )
	{
		static bool bInitHookFlag = false;
		if( bInitHookFlag == true )
		{
			return false;
		}
		if( g_dwGameMainThreadSleepHook != 0 )
		{
			Hook(g_dwGameMainThreadSleepHook,DoHookTask1);				// 主线程轻微优化

			if( g_dwGameSetStreamSourceAddr != 0 )
			{
				unsigned char code = 0xC3;

				WriteProcessMemory((void*)-1,(void*)g_dwGameSetStreamSourceAddr,&code,1,NULL);
// 				__asm 
// 				{
// 					mov eax, g_dwGameSetStreamSourceAddr				// 黑屏优化
// 					mov byte ptr [eax], 0xC3						// SetStreamSource
// 				}
			}

			Hook(UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___,HookRevampTime);

			bInitHookFlag = true;
		}
		return false;
	}

	if( nCount++ < 300 )
	{
		return true;
	}

	if( nCount > 330 )
	{
		bKillTenSLXFlag = true;
	}
	try
	{
		MainThreadKillTenSlX();
	}
	catch (...)
	{
#ifdef _DEBUG_LOG_
		OutputDebugStringA("[LOG] seh error");
#endif
	}
	return true;
}


typedef struct
{
	char*	pScrCur;
	int		nRemainBufLen;
	char*	pScrBuf;
}JFZRBASE_SPRINTF,*PJFZRBASE_SPRINTF;

typedef DWORD (WINAPI *LPK_GetCurrentProcessId)(void);
extern LPK_GetCurrentProcessId pGetCurrentProcessId;

void jfzrbase_sprintf(PJFZRBASE_SPRINTF ptheScrBuf)
{
	try
	{
		static bool bInitJfzrBaseSpirntfFlag = false;

		if( bInitJfzrBaseSpirntfFlag )
		{
			return ;
		}

#ifdef _DEBUG_LOG_
		char szPrint[1024];
		sprintf_s(szPrint,sizeof(szPrint) - 1,"[LOG] %s",ptheScrBuf->pScrBuf);	
		//OutputDebugString(szPrint);
#endif
		if( strstr(ptheScrBuf->pScrBuf,"Global-Event-") != 0 )
		{
			bInitJfzrBaseSpirntfFlag = true;
			char szProcessId[256] = {0};
			sprintf_s(szProcessId,"%p",pGetCurrentProcessId());

			int nLen = strlen(szProcessId);
			memcpy(ptheScrBuf->pScrCur,szProcessId,nLen);
			ptheScrBuf->nRemainBufLen -= nLen;
			ptheScrBuf->pScrCur += nLen;

		}

		// Global-Event-
	}
	catch (...)
	{

	}
}

DWORD g_dwJmpjfzrbase_sprintf = 0;

__declspec(naked) void hook_jfzrbase_sprintf()
{
	__asm
	{
		pushfd
		pushad

		push edi
		lea eax, jfzrbase_sprintf
		call eax
		add esp, 4

		popad
		popfd

		MOV EDI,EDI
		PUSH EBP
		MOV EBP,ESP

		jmp dword ptr [g_dwJmpjfzrbase_sprintf]
	}
}









bool DoLpk(char* pszProcessName,PFIFA_PACKET_CONNECT ptheFifaPacket)
{
	VMP_BEGIN;

	DWORD dwHookJfzrBaseSprintfAddr = (DWORD)LoadLibraryA("fifazfBase.dll")+ptheFifaPacket->dwFIFAZFBaseSprintfAddr;
	if( g_dwJmpjfzrbase_sprintf == 0 )
	{
		g_dwJmpjfzrbase_sprintf = dwHookJfzrBaseSprintfAddr+5;
	}
	Hook(dwHookJfzrBaseSprintfAddr,hook_jfzrbase_sprintf);

	Old_CreateToolhelp32Snapshot = (LPK_CreateToolhelp32Snapshot)GetProcAddress(LoadLibraryA("kernel32.dll"),ptheFifaPacket->strCreateToolhelp32Snapshot.c_str());
	if( Old_CreateToolhelp32Snapshot != 0 )
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
// 		DetourAttach(&(PVOID&)Old_CreateToolhelp32Snapshot,My_CreateToolhelp32Snapshot);
// 		DetourTransactionCommit();
//		Hook((DWORD)Old_CreateToolhelp32Snapshot,My_CreateToolhelp32Snapshot);
	}

	Old_EnumProcesses = (LPK_EnumProcesses)GetProcAddress(LoadLibraryA("PSAPI.dll"),ptheFifaPacket->strEnumProcesses.c_str()); 
	if( Old_EnumProcesses != 0 )
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
// 		DetourAttach(&(PVOID&)Old_EnumProcesses,My_EnumProcesses);
// 		DetourTransactionCommit();
		Hook((DWORD)Old_EnumProcesses,My_EnumProcesses);
	}

	Old_EnumProcessModules = (LPK_EnumProcessModules)GetProcAddress(LoadLibraryA("PSAPI.dll"),ptheFifaPacket->strEnumProcessModules.c_str()); 
	if( Old_EnumProcessModules != 0 )
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
// 		DetourAttach(&(PVOID&)Old_EnumProcessModules,My_EnumProcessModules);
// 		DetourTransactionCommit();
		Hook((DWORD)Old_EnumProcessModules,My_EnumProcessModules);
	}

	Old_NtCreateFile = (LPK_NtCreateFile)GetProcAddress(LoadLibraryA("ntdll.dll"),ptheFifaPacket->strNtCreateFile.c_str());
	if( Old_NtCreateFile != 0 )
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
// 		DetourAttachEx(&(PVOID&)Old_NtCreateFile,My_NtCreateFile,&(PDETOUR_TRAMPOLINE&)Jmp_Old_NtCreateFile,0, 0);
// 		DetourTransactionCommit();
		Hook((DWORD)Old_NtCreateFile,My_NtCreateFile);
		Jmp_Old_NtCreateFile = (LPK_NtCreateFile)Real_Jmp_Old_NtCreateFile;
		g_dwReal_Jmp_Old_NtCreateFile = (DWORD)Old_NtCreateFile+5;
	}

	Old_ZwQuerySystemInformation = (LPK_ZwQuerySystemInformation)GetProcAddress(LoadLibraryA("ntdll.dll"),ptheFifaPacket->strZwQuerySystemInformation.c_str()); 
	if( Old_ZwQuerySystemInformation != 0 )
	{
// 		DetourTransactionBegin();
// 		DetourUpdateThread(GetCurrentThread());
// 		DetourAttachEx(&(PVOID&)Old_ZwQuerySystemInformation,My_ZwQuerySystemInformation,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwQuerySystemInformation,0, 0);
// 		DetourTransactionCommit();
		Hook((DWORD)Old_ZwQuerySystemInformation,My_ZwQuerySystemInformation);
		Jmp_Old_ZwQuerySystemInformation = (LPK_ZwQuerySystemInformation)Real_Jmp_Old_ZwQuerySystemInformation;
		g_dwReal_Jmp_Old_ZwQuerySystemInformation = (DWORD)Old_ZwQuerySystemInformation+5;
	}

	if( _stricmp(pszProcessName,"fifazf.exe") == 0 )
	{
		Old_ZwCreateMutant = (LPK_ZwCreateMutant)GetProcAddress(LoadLibraryA("ntdll.dll"),ptheFifaPacket->strZwCreateMutant.c_str());
		if( Old_ZwCreateMutant != 0 )
		{
// 			DetourTransactionBegin();
// 			DetourUpdateThread(GetCurrentThread());
// 			DetourAttachEx(&(PVOID&)Old_ZwCreateMutant,My_ZwCreateMutant,&(PDETOUR_TRAMPOLINE&)Jmp_Old_ZwCreateMutant,0, 0);
// 			DetourAttach(&(PVOID&)Old_ZwCreateMutant,My_ZwCreateMutant);
// 			DetourTransactionCommit();
			Hook((DWORD)Old_ZwCreateMutant,My_ZwCreateMutant);
			Jmp_Old_ZwCreateMutant = (LPK_ZwCreateMutant)Real_Jmp_Old_ZwCreateMutant;
			g_dwReal_Jmp_Old_ZwCreateMutant = (DWORD)Old_ZwCreateMutant+5;
		}			
		
		g_dwGameSetStreamSourceAddr = ptheFifaPacket->dwGameSetStreamSourceAddr;
		g_dwGameMainThreadSleepHook = ptheFifaPacket->dwGameMainThreadSleepHook;
	}

	

	VMP_END;
	return true;
}

bool DoCheck(FIFA_PACKET_CONNECT& theFifaPacket)
{
	VMP_BEGIN_EX;
	bool bRet = false;
	reversalsock	theSocket;

	if( theSocket.connect("lpkb_fifa") )
	{
		int i = 0;
		while (!bRet)
		{
			if( i++ > 1000 )
			{
				break;
			}
			Sleep(30);
			if( theSocket.doconnect() )
			{
				theFifaPacket.dwGameSetStreamSourceAddr		= theFIFADBConnectionManager.m_theFifaLpkPacket.dwGameSetStreamSourceAddr;
				theFifaPacket.dwGameMainThreadSleepHook		= theFIFADBConnectionManager.m_theFifaLpkPacket.dwGameMainThreadSleepHook;
				theFifaPacket.dwFIFAZFBaseSprintfAddr		= theFIFADBConnectionManager.m_theFifaLpkPacket.dwFIFAZFBaseSprintfAddr;
				theFifaPacket.strCreateToolhelp32Snapshot	= "CreateToolhelp32";
				theFifaPacket.strCreateToolhelp32Snapshot	+= theFIFADBConnectionManager.m_theFifaLpkPacket.strCreateToolhelp32Snapshot;
				theFifaPacket.strEnumProcesses				= "Enum";
				theFifaPacket.strEnumProcesses				+= theFIFADBConnectionManager.m_theFifaLpkPacket.strEnumProcesses;
				theFifaPacket.strEnumProcessModules			= "EnumProcess";
				theFifaPacket.strEnumProcessModules			+= theFIFADBConnectionManager.m_theFifaLpkPacket.strEnumProcessModules;
				theFifaPacket.strNtCreateFile				= "NtCreate";
				theFifaPacket.strNtCreateFile				+= theFIFADBConnectionManager.m_theFifaLpkPacket.strNtCreateFile;
				theFifaPacket.strZwQuerySystemInformation	= "ZwQuerySystem";
				theFifaPacket.strZwQuerySystemInformation	+= theFIFADBConnectionManager.m_theFifaLpkPacket.strZwQuerySystemInformation;
				theFifaPacket.strZwCreateMutant				= "ZwCreate";
				theFifaPacket.strZwCreateMutant				+= theFIFADBConnectionManager.m_theFifaLpkPacket.strZwCreateMutant;
				theFifaPacket.strVersion					= "";
				theFifaPacket.strVersion					+= theFIFADBConnectionManager.m_theFifaLpkPacket.strVersion;
				bRet = true;
			}
		}
	}

	theSocket.close();

	VMP_END;
	return bRet;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	DWORD m_dwReturn[11] = {0};	// 原始函数返回地址

	BOOL LoadLpkSrc(){
		static bool exec_once = false;
		static BOOL load_res = FALSE;
		if (exec_once)
			return load_res;
		exec_once = true;

		char tzPath[MAX_PATH] = { 0 };
		char tzTemp[MAX_PATH * 2] = { 0 };

		GetSystemDirectoryA(tzPath, MAX_PATH);
		strcat_s(tzPath, "\\lpk");
		m_hModule = LoadLibraryA(tzPath);
		if (m_hModule == NULL)
		{
#ifdef _DEBUG_LOG_
			sprintf_s(tzTemp, "[LOG] 无法加载 %s，程序无法正常运行。", tzPath);
			OutputDebugStringA(tzTemp);
#endif
			return FALSE;
		}
		load_res = TRUE;
		return TRUE;
	}
	// 加载原始模块
	inline BOOL WINAPI Load()
	{
		if ( !LoadLpkSrc() )
			return FALSE;

		char szProcessName[1024] = {0};
		GetProcessHostName(szProcessName);

#ifdef _DEBUG_LOG_
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 进程:%s !!!",szProcessName);
		OutputDebugStringA(szPrint);
#endif

		//Sleep(1000*10); 
		if( _stricmp(szProcessName,"fifalauncher.exe") == 0 || _stricmp(szProcessName,"fifazf.exe") == 0)
		{
#ifdef _DEBUG_LOG_
			OutputDebugStringA("[LOG] begin DoCheck");
#endif
			FIFA_PACKET_CONNECT theFifaPacket;
			if ( !DoCheck(theFifaPacket) )
			{
#ifdef _DEBUG_LOG_
				OutputDebugStringA("[LOG] DoCheck error");
#endif
				return FALSE;
			}
#ifdef _DEBUG_LOG_
			OutputDebugStringA("[LOG] begin check version");
#endif
			if( strcmp(theFifaPacket.strVersion.c_str(),REVERSAL_VERSION) != 0 )		// 验证版本
			{
#ifdef _DEBUG_LOG_
				OutputDebugStringA("[LOG] version error");
#endif
				return FALSE;
			}

#ifdef _DEBUG_LOG_
			OutputDebugStringA("[LOG] begin DoLpk");
#endif
			if ( !DoLpk(szProcessName,&theFifaPacket) )
			{
#ifdef _DEBUG_LOG_
				OutputDebugStringA("[LOG] DoLpk error");
#endif
				return FALSE;
			}
 		}

// 		if( !DoRootKit() )
// 		{
// 			return NULL;
// 		}

		return TRUE;	
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
		FARPROC fpAddress;
		CHAR szProcName[16];
		char tzTemp[MAX_PATH];

// 		char szPrint[256] = {0};
// 		wsprintf(szPrint,"[LOG] 函数:%s !!!",pszProcName);
// 		OutputDebugString(szPrint);

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				sprintf_s(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			sprintf_s(tzTemp, "无法找到函数 %hs，程序无法正常运行。", pszProcName);
			MessageBoxA(NULL, tzTemp, "AheadLib", MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace AheadLib;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL InitLoad()
{
	static bool bInitLoadFlag = false;
	static BOOL load_res = FALSE;
	if (bInitLoadFlag)
		return load_res;
	if( bInitLoadFlag == false )
	{
		bInitLoadFlag = true;
		load_res = Load();
		if ( !load_res )
			return load_res;
		/*//LpkEditControl这个数组有14个成员，必须将其复制过来    
		memcpy((LPVOID)(LpkEditControl+1), (LPVOID)((int*)GetAddress("LpkEditControl") + 1),52);  */
	}
	return load_res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//DisableThreadLibraryCalls(hModule);

		BOOL res = InitLoad();
#ifdef _DEBUG_LOG_
		if ( !res )
		{
			OutputDebugStringA("[LOG] DllMain InitLoad return false");
		}
#endif
		return res;
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
	LoadLpkSrc();
	GetAddress("LpkInitialize");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkTabbedTextOut(void)
{
	LoadLpkSrc();
	GetAddress("LpkTabbedTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkDllInitialize(void)
{
	LoadLpkSrc();
	GetAddress("LpkDllInitialize");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkDrawTextEx(void)
{
	LoadLpkSrc();
	GetAddress("LpkDrawTextEx");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkEditControl(void)
{
	LoadLpkSrc();
	GetAddress("LpkEditControl");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkExtTextOut(void)
{
	LoadLpkSrc();
	GetAddress("LpkExtTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkGetCharacterPlacement(void)
{
	LoadLpkSrc();
	GetAddress("LpkGetCharacterPlacement");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkGetTextExtentExPoint(void)
{
	LoadLpkSrc();
	GetAddress("LpkGetTextExtentExPoint");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkPSMTextOut(void)
{
	LoadLpkSrc();
	GetAddress("LpkPSMTextOut");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkUseGDIWidthCache(void)
{
	LoadLpkSrc();
	GetAddress("LpkUseGDIWidthCache");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_ftsWordBreak(void)
{
	LoadLpkSrc();
	GetAddress("ftsWordBreak");
	__asm JMP EAX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
