// stdafx.cpp : 只包括标准包含文件的源文件
// jfzrcalltest.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


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