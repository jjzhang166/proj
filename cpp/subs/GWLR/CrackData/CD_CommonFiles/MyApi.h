#pragma once
 //更改内存区域的页面属性,和VirtualProtectEx功能一样
 DWORD MyZwProtectVirtualMemory(	IN HANDLE               ProcessHandle,
												IN OUT PVOID            *BaseAddress,
												IN OUT PULONG           NumberOfBytesToProtect,
												IN ULONG                NewAccessProtection,
												OUT PULONG              OldAccessProtection); 



 BOOL MyWriteProcessMemory(
	 const char* hooker_name,
	 IN HANDLE               ProcessHandle,
	 IN PVOID                BaseAddress,
	 IN PVOID                Buffer,
	 IN ULONG                NumberOfBytesToWrite,
	 OUT PULONG              NumberOfBytesWritten OPTIONAL );



 //读值，和ReadProcessMemory用法一样
 DWORD MyReadProcessMemory(
	 IN HANDLE               ProcessHandle,
	 IN PVOID                BaseAddress,
	 OUT PVOID               Buffer,
	 IN ULONG                NumberOfBytesToRead,
	 OUT PULONG              NumberOfBytesReaded OPTIONAL );


 //结束进程

 DWORD MyTerminateThread(
	 _In_opt_  HANDLE ProcessHandle,
	 _In_      DWORD ExitStatus
	 );//介绍线程

 //获得系统时间
 DWORD  MyGetTickCount();

 void MyTrace(const char * format,...);

 uintptr_t MyBeginThread(
	 _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	 _In_ SIZE_T dwStackSize,
	 PVOID lpStartAddress,
	 _In_opt_ __drv_aliasesMem LPVOID lpParameter,
	 _In_ DWORD dwCreationFlags,
	 _Out_opt_ LPDWORD lpThreadId,
	 DWORD dwTheadEntryType
	 );