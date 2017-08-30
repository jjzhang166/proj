// stdafx.cpp : 只包括标准包含文件的源文件
// jfzrcalltest.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <CommonX/CmnX_HookerMore.h>

void Hook( DWORD dwHookAddr,LPVOID pfun, const char* hooker_name )
{
	try
	{
		unsigned char oldcode = 0;
		ReadProcessMemory((void*)-1,(void*)dwHookAddr,&oldcode,1,NULL);
		unsigned char code = 0xe9;
		if( oldcode == code )
		{
			assert(false);
			return ;
		}

		DWORD dwDoHookAddr = (DWORD)(void*)pfun;

		DWORD dwcAddr = dwDoHookAddr - dwHookAddr - 1 - 4;
		unsigned char code_buff[5];
		code_buff[0] = code;
		*(DWORD*)&(code_buff[1]) = dwcAddr;
		if ( !CmnX_AntiHookCheck::GetMe().AddHookCodeInfo(hooker_name, (const void*)dwHookAddr, _countof(code_buff)) )
		{
			assert(false);
			return;
		}
		WriteProcessMemory((void*)-1,(void*)dwHookAddr,code_buff,_countof(code_buff),NULL);
	}
	catch (...)
	{
		MessageBoxA(NULL,"哒哒哒","啊啊啊",MB_OK);
	}
}

BOOL _ReadProcessMemory(DWORD dwReadAddr,void* pReadData,DWORD dwReadlen)
{
	return ::ReadProcessMemory((HANDLE)-1,(LPCVOID)dwReadAddr,pReadData,dwReadlen,0);
}

BOOL _WriteProcessMemory( DWORD dwWriteAddr, const void* pWriteData,DWORD dwWritelen, const char* hooker_name )
{
	if (!CmnX_AntiHookCheck::GetRealMe().AddHookCodeInfo(hooker_name, (const void*)dwWriteAddr, dwWritelen))
	{
		assert(false);
		return FALSE;
	}
	return ::WriteProcessMemory((HANDLE)-1,(LPVOID)dwWriteAddr,pWriteData,dwWritelen,0);
}

void FuncRefTest( const char* the_prompt_str )
{
	auto the_str = the_prompt_str;
}
