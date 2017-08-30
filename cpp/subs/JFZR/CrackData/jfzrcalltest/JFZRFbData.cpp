#include "stdafx.h"


CString	g_csPrintFbVillage;
int	g_nPrintFbVillageCount = 0;

void FbLevelPrint(DWORD lpSubTreeRoot)
{
	FB_LEVEL theFbObj = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theFbObj,sizeof(theFbObj));

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"[TASK] i = %d : %p ==> id = %s level = %p\r\n",++g_nPrintFbVillageCount,lpSubTreeRoot,GetString_(&theFbObj.str_theId),theFbObj.str_dwLevel);
	OutputDebugStringW(wszPrint);
	g_csPrintFbVillage += wszPrint;

}

void ToFbVillageTraverse()
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_FB_OFFSET1+JFZR_REVERSAL_FB_OFFSET2+JFZR_REVERSAL_FB_OFFSET3___,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csPrintFbVillage = "";
		g_nPrintFbVillageCount = 0;

		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,FbLevelPrint);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
CString	g_csPrintMapData;
DWORD g_dwLocalMapObjAddr = 0;
_declspec(naked) void HookGetLocalObjMapObj()
{
	__asm
	{
		mov g_dwLocalMapObjAddr, ecx
	}
	__asm
	{
		PUSH EBX
		MOV EBX,ECX
		MOV EAX,DWORD PTR DS:[EBX+0x37C]

		mov eax, JFZR_REVERSAL_LOCAL_MAP_OBJ_HOOK
		add eax, 9
		jmp eax
	}
}


void MapInfoPrint(DWORD lpSubTreeRoot)
{
	MAP_INFO	theMapInfo = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theMapInfo,sizeof(theMapInfo));

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"addr = 0x%p map Info %s ==> %s \r\n",lpSubTreeRoot,GetString(&theMapInfo.str_theSectorId),GetVString(&theMapInfo.str_theMapId));

	OutputDebugStringW(wszPrint);
	g_csPrintMapData += wszPrint;
}


void BoosInfoPrint(DWORD lpSubTreeRoot)
{
	BOOS_INFO	theBossInfo = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theBossInfo,sizeof(theBossInfo));

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"addr = 0x%p boos Info %s\r\n",lpSubTreeRoot,GetString_(&theBossInfo.str_theId));

	OutputDebugStringW(wszPrint);
	g_csPrintMapData += wszPrint;
}


void ClearSectorPrint(DWORD lpSubTreeRoot)
{
	PCLEAR_SECTOR ptheClearSectot = (PCLEAR_SECTOR)lpSubTreeRoot;

	wchar_t	wszPrint[256] = {0};
	swprintf_s(wszPrint,L"clear sector name = %s\r\n",GetString(&ptheClearSectot->str_theId));
	g_csPrintMapData += wszPrint;

}

bool UpDataMap()
{
	MAP_DATA theMapData = {0};

	g_dwLocalMapObjAddr = 0;
	Hook(JFZR_REVERSAL_LOCAL_MAP_OBJ_HOOK,HookGetLocalObjMapObj);

	while( !g_dwLocalMapObjAddr )
	{
		Sleep(1);
	}

	unsigned char szCode[] = {0x53,0x8B,0xD9,0x8B,0x83,0x7C,0x03,0x00,0x00};
	_WriteProcessMemory(JFZR_REVERSAL_LOCAL_MAP_OBJ_HOOK,szCode,9);

	g_csPrintMapData = "";

	DWORD	dwMapObjAddr = 0;
	_ReadProcessMemory(g_dwLocalMapObjAddr+JFZR_REVERSAL_LOCAL_MAP_OBJ_OFFSET,&dwMapObjAddr,sizeof(dwMapObjAddr));

	if( dwMapObjAddr != NULL )
	{
		_ReadProcessMemory(dwMapObjAddr,&theMapData,sizeof(theMapData));

		wchar_t	wszPrint[256] = {0};
		swprintf_s(wszPrint,L"addr = 0x%p map = %s level = %d\r\n",dwMapObjAddr,GetString(&theMapData.str_theId),theMapData.str_dwLevel);

		g_csPrintMapData += wszPrint;

		// 新增当某个房间被清理完毕，打开门后，则此表+1
		if( theMapData.str_theAlreadyClearSector.str_dwCount > 0 )
		{
			assert(theMapData.str_theAlreadyClearSector.str_dwBinaryTreeNullMapAddr);

			NODE theNode = {0};
			_ReadProcessMemory(theMapData.str_theAlreadyClearSector.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

			Traverse(theNode.str_dwNextAddrParent,theMapData.str_theAlreadyClearSector.str_dwBinaryTreeNullMapAddr,ClearSectorPrint);

		}

		// SDB_STAGE_INFO
		assert(theMapData.str_dwObjAddr);
		if( theMapData.str_dwObjAddr != NULL )
		{
			SDB_STAGE_INFO	theSdbStageInfo = {0};
			_ReadProcessMemory(theMapData.str_dwObjAddr,&theSdbStageInfo,sizeof(theSdbStageInfo));

			assert(theSdbStageInfo.str_theSectors.str_dwBinaryTreeNullMapAddr);

			// 房间序号
			if( theSdbStageInfo.str_theSectors.str_dwCount > 0 )
			{
				NODE theNode = {0};
				_ReadProcessMemory(theSdbStageInfo.str_theSectors.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

				Traverse(theNode.str_dwNextAddrParent,theSdbStageInfo.str_theSectors.str_dwBinaryTreeNullMapAddr,MapInfoPrint);
			}

			// 遍历当前副本BOOS名称
			if( theSdbStageInfo.str_theStageObjectNpcs.str_dwCount > 0 )
			{
				NODE theNode = {0};
				_ReadProcessMemory(theSdbStageInfo.str_theStageObjectNpcs.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

				Traverse(theNode.str_dwNextAddrParent,theSdbStageInfo.str_theStageObjectNpcs.str_dwBinaryTreeNullMapAddr,BoosInfoPrint);
			}
		}
	}

	return false;
}