#include "StdAfx.h"
#include "Sdb.h"



Sdb::Sdb(void)
{
}

Sdb::~Sdb(void)
{
}



typedef struct  
{
	BINARY_TREE_MAP				str_theSdbInfo;
	GAME_STRUCT_ARRAY_POINT		str_theNullAddr;
//	BINARY_TREE_MAP				str_theSdbInfo_;
	GAME_STRING_ID				str_theSdbString;
	GAME_STRING_ID				str_theSdbString_;
}SDB_INFO,*PSDB_INFO;

typedef struct
{	
	DWORD		str_dwSdbInfoAddr;
	SDB_INFO	str_theSdbInfo;
}SDB_INFO_,*PSDB_INFO_;

CList<SDB_INFO_,SDB_INFO_>	g_theSdbInfoList;

bool IsHaveSdbInfo(PSDB_INFO_ ptheSdbInfo)
{
	POSITION pos = g_theSdbInfoList.GetHeadPosition();
	for (int i=0;i < g_theSdbInfoList.GetCount();i++)
	{
		PSDB_INFO_ ptheSdbInfoTemp = &g_theSdbInfoList.GetNext(pos);

		if( ptheSdbInfo->str_dwSdbInfoAddr == ptheSdbInfoTemp->str_dwSdbInfoAddr )
		{
			return false;
		}
	}
	return true;
}

void PrintSdbAddr(DWORD	dwSdbAddr)
{
	SDB_INFO_ theSdbInfo = {0};
	_ReadProcessMemory(dwSdbAddr,&theSdbInfo.str_theSdbInfo,sizeof(SDB_INFO));

	theSdbInfo.str_dwSdbInfoAddr = dwSdbAddr;

	try
	{
		if( IsHaveSdbInfo(&theSdbInfo) )
		{
			g_theSdbInfoList.AddHead(theSdbInfo);

			wchar_t wszPrint[256] = {0};
			swprintf_s(wszPrint,L"[JFZR] %p %s\r\n",theSdbInfo.str_dwSdbInfoAddr,GetString(&theSdbInfo.str_theSdbInfo.str_theSdbString_));
			OutputDebugStringW(wszPrint);
		}

	}
	catch (...)
	{

	}


}

_declspec(naked) void HookPrintSdb()
{
	__asm
	{
		pushfd
		pushad

		push ecx
		lea eax, PrintSdbAddr
		call eax
		add esp, 4

		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH ECX
		MOV EDX,DWORD PTR DS:[ECX+0x4]

		mov eax, JFZR_REVERSAL_SDB_BASE_ADDR_UP_DATA_HOOK
		add eax, 7
		jmp eax
	}
}

void SdbBase()
{
	Hook(JFZR_REVERSAL_SDB_BASE_ADDR_UP_DATA_HOOK,HookPrintSdb);
}