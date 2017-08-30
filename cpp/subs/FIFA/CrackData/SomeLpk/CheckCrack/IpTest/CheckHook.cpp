#include "StdAfx.h"
#include "CheckHook.h"
#include "../command/scan.h"


CheckHook::CheckHook(void)
{
}

CheckHook::~CheckHook(void)
{
}


bool CheckHook::CheckInlineHook(const char* pszModuleName,const char* pszFunName)
{
	DWORD dwCheckAddr		= (DWORD)GetModuleHandle(pszModuleName);
	DWORD dwBeginCheckAddr	= dwCheckAddr+0x1000;
	DWORD dwEndCheckAddr	= dwCheckAddr + 0x18000;

	while (true)
	{
		DWORD	dwE9Addr = ScanAddr((HANDLE)-1,"E9",0,4,1,dwBeginCheckAddr,dwEndCheckAddr);

		if( dwE9Addr != 0 )
		{
			DWORD dwJmpReadAddr = 0;
			ReadProcessMemory((HANDLE)-1,(LPCVOID)(dwE9Addr+1),&dwJmpReadAddr,sizeof(dwJmpReadAddr),NULL);

			DWORD dwJmpAddr = dwE9Addr+1+dwJmpReadAddr+1+4;

			if( !(dwCheckAddr < dwJmpAddr && dwJmpAddr < dwEndCheckAddr) )
			{
				if( dwJmpAddr < 0x70000000 )
				{
					printf("%p - %p\r\n",dwE9Addr,dwJmpAddr);
					//WriteProcessMemory((HANDLE)-1,(LPVOID)(dwE9Addr+1),&dwJmpAddr,sizeof(dwJmpAddr),NULL);
				}
			}
			
			dwBeginCheckAddr = dwE9Addr+1;
		}
		else
		{
			break;
		}

	}

	return false;
}
