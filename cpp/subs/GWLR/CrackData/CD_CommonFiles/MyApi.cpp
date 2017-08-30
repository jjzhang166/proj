#include "stdafx.h"
#include "MyApi.h"
#include <CommonX/CmnX_HookerMore.h>


_declspec(naked) DWORD MyZwProtectVirtualMemory(	IN HANDLE               ProcessHandle,
												IN OUT PVOID            *BaseAddress,
												IN OUT PULONG           NumberOfBytesToProtect,
												IN ULONG                NewAccessProtection,
												OUT PULONG              OldAccessProtection)
{
	__asm
	{
		mov eax,0x4d
			xor ecx,ecx
			lea edx,[esp+4]
		call fs:[0xc0]
		add esp,4
			ret
	}
}




_declspec(naked) DWORD  MyNtReadVirtualMemory(

	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	OUT PVOID               Buffer,
	IN ULONG                NumberOfBytesToRead,
	OUT PULONG              NumberOfBytesReaded OPTIONAL )
{

	__asm
	{
		mov eax,0x3c
			xor ecx,ecx
			lea edx,[esp+4]
		call fs:[0xc0]
		add esp,4
			ret
	}
}

_declspec(naked) DWORD NtWriteVirtualMemory(



	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL )
{
	__asm
	{

		mov eax,0x37
			xor ecx,ecx
			lea edx,[esp+4]
		call fs:[0xc0]
		add esp,4
			ret
	}

}



BOOL MyWriteProcessMemory(
	const char* hooker_name,
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL )
{
	if ( !CmnX_AntiHookCheck::GetMe().AddHookCodeInfo(hooker_name, (const void*)BaseAddress, NumberOfBytesToWrite) )
	{
		assert(false);
		return FALSE;
	}
	DWORD dwOldProject;
	DWORD dwOldNum=NumberOfBytesToWrite;
	PVOID dwOldAddr=BaseAddress;
	MyZwProtectVirtualMemory(ProcessHandle,&BaseAddress,&NumberOfBytesToWrite,0x40, &dwOldProject);
	PVOID dwOldAddr2=BaseAddress;
	DWORD dwRet=NtWriteVirtualMemory(ProcessHandle, dwOldAddr, Buffer, dwOldNum, NumberOfBytesWritten);
	MyZwProtectVirtualMemory(ProcessHandle,&dwOldAddr2,&NumberOfBytesToWrite,dwOldProject,&dwOldProject);
	if ( 0 == dwRet )
		return TRUE;
	assert(false);
	return false;
}

_declspec(naked) DWORD MyReadProcessMemory(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	OUT PVOID               Buffer,
	IN ULONG                NumberOfBytesToRead,
	OUT PULONG              NumberOfBytesReaded OPTIONAL )
{
	__asm
	{

		mov eax,0x3C
			xor ecx,ecx
			lea edx,[esp+4]
		call fs:[0xc0]
		add esp,4
			ret
	}

}
void MyHook(DWORD dwHookAddr,LPVOID pfun)
{
	try
	{
		unsigned char oldcode;
		MyReadProcessMemory((void*)-1,(void*)dwHookAddr,&oldcode,1,NULL);

		unsigned char code = 0xe9;
		if( oldcode == code )
			return ;

		MyWriteProcessMemory("TreadEntryHook",(void*)-1,(void*)dwHookAddr,&code,1,NULL);

		DWORD dwDoHookAddr = (DWORD)(void*)pfun;

		DWORD dwcAddr = dwDoHookAddr - dwHookAddr - 1 - 4;
		__asm
		{
			MOV EBX,EBX
				MOV EBX,EBX
		}
		MyWriteProcessMemory("TreadEntryHook",(void*)-1,(void*)(dwHookAddr+1),&dwcAddr,4,NULL);
	}
	catch (...)
	{
		MessageBoxA(NULL,"哒哒哒","啊啊啊",MB_OK);
	}
}


uintptr_t MyBeginThread(
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	PVOID lpStartAddress,
	_In_opt_ __drv_aliasesMem LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId,
	DWORD dwTheadEntryType
	)
{

	//取得第一步线程入口
	unsigned char byCode[5]={0};
	PTHREAD_START_ROUTINE dwTheadEntry=NULL;
	if (dwTheadEntryType==1)
	{
		dwTheadEntry=(PTHREAD_START_ROUTINE)((DWORD)GetProcAddress(LoadLibraryA("msvcr90.dll"),"NtProtectVirtualMemory")+0x6F);//这个入口就是腾讯线程入口地址
		unsigned char byTemp[5]={0x8B,0xFF,0x55,0x8b,0xEC};
		memcpy(byCode,byTemp,5);
	}
	else if (dwTheadEntryType==2)
	{
		dwTheadEntry=(PTHREAD_START_ROUTINE)((DWORD)GetProcAddress(GetModuleHandleA("C:\\Windows\\SysWow64\\ntdll.dll"),"TpCallbackIndependent")+0x238);//这个入口就是腾讯线程入口地址
		unsigned char byTemp[5]={0xAA,0x90,0x90,0x90,0x90};
		memcpy(byCode,byTemp,5);
	}else if(dwTheadEntryType==3)
	{
		dwTheadEntry=(PTHREAD_START_ROUTINE)((DWORD)GetProcAddress(LoadLibraryA("ntdll.dll"),"memcpy")-5);//233B
		unsigned char byTemp[5]={0x90,0x90,0x90,0x90,0x90};
		memcpy(byCode,byTemp,5);
	}


	MyHook((DWORD)dwTheadEntry,(LPVOID)lpStartAddress);
	while (true)
	{
		unsigned char oldcode;
		MyReadProcessMemory((void*)-1,(void*)dwTheadEntry,&oldcode,1,NULL);
		if (oldcode==0xE9)
			break;
	}
	uintptr_t Ret= (uintptr_t)CreateThread(lpThreadAttributes,dwStackSize,dwTheadEntry,lpParameter,dwCreationFlags,lpThreadId);

	Sleep(100);//给他执行好
	//MyWriteProcessMemory((void*)-1,(void*)dwTheadEntry,&byCode,5,NULL);
	return Ret;

}


_declspec(naked) DWORD  MyGetTickCount()
{
	__asm
	{

BEGIN:	pause
			mov ecx,dword ptr ds:[0x7FFE0324]
		mov edx,dword ptr ds:[0x7FFE0320]
		mov eax,dword ptr ds:[0x7FFE0328]
		cmp ecx,eax
			jnz BEGIN
			mov eax,dword ptr ds:[0x7FFE0004]
		mul edx
			shl ecx,0x8
			imul ecx,dword ptr ds:[0x7FFE0004]
		shrd eax,edx,0x18
			shr edx,0x18
			add eax,ecx
			retn


	}
}


void MyTrace(const char * format, ...)
{
#if _DEBUG
	static const int BufferLen = 1212;
	va_list PNextArg;
	char szMessageBuffer[BufferLen] = { 0 };
	va_start(PNextArg, format);
	sprintf_s(szMessageBuffer, BufferLen - 1, format, PNextArg);
	OutputDebugStringA(szMessageBuffer);
#endif
}

_declspec(naked) DWORD MyTerminateThread(
	_In_opt_  HANDLE ProcessHandle,
	_In_      DWORD ExitStatus
	)
{
	__asm
	{
		mov eax,0x50
			xor ecx,ecx
			lea edx,[esp+4]
		call fs:[0xc0]
		add esp,4
			ret
	}
}