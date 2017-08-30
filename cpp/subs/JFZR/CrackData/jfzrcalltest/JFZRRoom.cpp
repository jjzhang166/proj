#include "StdAfx.h"
#include "JFZRRoom.h"
#include "Shlwapi.h"

JFZRRoom::JFZRRoom(void)
{
}

JFZRRoom::~JFZRRoom(void)
{
}


DWORD g_dwLocalRoomObjAddr = 0;
_declspec(naked) void HookGetLocalObjRoomObj()
{
	__asm
	{
		mov g_dwLocalRoomObjAddr, edi
	}
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		PUSH -0x1

		mov eax, JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK
		add eax, 5
		jmp eax
	}
}

CString g_csWarpGate;

void RoomWarpPrint(DWORD lpSubTreeRoot)
{
	PSDB_WARP_GATE ptheWarpGate = (PSDB_WARP_GATE)lpSubTreeRoot;

	wchar_t wszPrint[256] = {0};
	swprintf_s(wszPrint,L"warp1 obj addr = 0x%p warp name = %s x = %f y = %f\r\n",
							lpSubTreeRoot,GetString(&ptheWarpGate->theWarpGate.str_theId),ptheWarpGate->theWarpGate.str_fPositionX,ptheWarpGate->theWarpGate.str_fPositionY);
	g_csWarpGate += wszPrint;
}

bool JFZRRoom::UpDataRoom(PROOM ptheCurRoom)
{
	g_csWarpGate = "";
	g_dwLocalRoomObjAddr = 0;
	// 只有完全进入副本，hook的地址才会执行
	Hook(JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK,HookGetLocalObjRoomObj);

	while(!g_dwLocalRoomObjAddr)
	{
		Sleep(10);
	}
 
	DWORD dwLocalRoomObjAddr = g_dwLocalRoomObjAddr;

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD	dwMapLen = 0;
	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_LEN_OFFSET___,&dwMapLen,sizeof(dwMapLen));

	DWORD dwRoomMapDataAddr = 0;
	if( dwMapLen > 7 )
	{
		_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_OFFSET___,&dwRoomMapDataAddr,sizeof(dwRoomMapDataAddr));
	}
	else
	{
		dwRoomMapDataAddr = dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_OFFSET___;
	}

	char szReadMapData[256] = {0};
	_ReadProcessMemory(dwRoomMapDataAddr,szReadMapData,dwMapLen*2);

	DWORD dwMapLen_ = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadMapData,-1,NULL,0,NULL,FALSE);

	char* pszRoomObjMapData = NULL;
	pszRoomObjMapData = new char[dwMapLen_+2];

	if( pszRoomObjMapData != NULL )
	{
		memset(pszRoomObjMapData,0,dwMapLen_+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadMapData,-1,pszRoomObjMapData,dwMapLen_,NULL,FALSE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwRoomIdLen = 0;

	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_LEN_OFFSET___,&dwRoomIdLen,sizeof(dwRoomIdLen));

	DWORD dwRoomIdNameAddr = 0;
	if( dwRoomIdLen > 7 )
	{
		_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_OFFSET___,&dwRoomIdNameAddr,sizeof(dwRoomIdNameAddr));
	}
	else
	{
		dwRoomIdNameAddr = dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_OFFSET___;
	}

	char szReadRoomIdName[256] = {0};
	_ReadProcessMemory(dwRoomIdNameAddr,szReadRoomIdName,dwRoomIdLen*2);

	DWORD dwRoomIdNameLen = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadRoomIdName,-1,NULL,0,NULL,FALSE);

	char* pszRoomIdName = NULL;
	pszRoomIdName = new char[dwRoomIdNameLen+2];

	if( pszRoomIdName != NULL )
	{
		memset(pszRoomIdName,0,dwRoomIdNameLen+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadRoomIdName,-1,pszRoomIdName,dwRoomIdNameLen,NULL,FALSE);
	}

	char szPrint[256] = {0};
	sprintf_s(szPrint,"room obj addr = 0x%p map name = %s  room name = %s\r\n",dwLocalRoomObjAddr,pszRoomObjMapData,pszRoomIdName);

	g_csWarpGate += szPrint;

	BINARY_TREE_MAP	theRoomDoorObjAddr;
	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_DOOR_OBJ_OFFSET___,&theRoomDoorObjAddr,sizeof(theRoomDoorObjAddr));
	assert(theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr);

	if( theRoomDoorObjAddr.str_dwCount > 0 )
	{
		NODE	theNode;
		_ReadProcessMemory(theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));


		Traverse(theNode.str_dwNextAddrParent,theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr,RoomWarpPrint);


		DWORD dwDoorObjAddr1 = 0;
		_ReadProcessMemory(theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr+0,&dwDoorObjAddr1,sizeof(dwDoorObjAddr1));
		assert(dwDoorObjAddr1); 

		DWORD dwWarpLen1 = 0;

		_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen1,sizeof(dwWarpLen1));

		DWORD dwDoorObjNameAddr1 = 0;
		if( dwWarpLen1 > 7 )
		{
			_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr1,sizeof(dwDoorObjNameAddr1));
		}
		else
		{
			dwDoorObjNameAddr1 = dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
		}

		char szReadWarpName1[256] = {0};
		_ReadProcessMemory(dwDoorObjNameAddr1,szReadWarpName1,dwWarpLen1*2);

		DWORD dwWarpNameLen1 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName1,-1,NULL,0,NULL,FALSE);

		char* pszWarpName1 = NULL;
		pszWarpName1 = new char[dwWarpNameLen1+2];

		if( pszWarpName1 != NULL )
		{
			memset(pszWarpName1,0,dwWarpNameLen1+2);
			WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName1,-1,pszWarpName1,dwWarpNameLen1,NULL,FALSE);
		}

		float	fx1;
		float	fy1;
		float	fz1;

		_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx1,sizeof(fx1));
		_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy1,sizeof(fy1));
		_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz1,sizeof(fz1));

		ptheCurRoom->str_Coord[0].str_fx = fx1;
		ptheCurRoom->str_Coord[0].str_fy = fy1;
		ptheCurRoom->str_Coord[0].str_fz = fz1;

		////////////////////////////////////////////////////////////////////////////////////////////
		DWORD dwDoorObjAddr2 = 0;
		_ReadProcessMemory(theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr+4,&dwDoorObjAddr2,sizeof(dwDoorObjAddr2));
		assert(dwDoorObjAddr2);

		DWORD dwWarpLen2 = 0;

		_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen2,sizeof(dwWarpLen2));

		DWORD dwDoorObjNameAddr2 = 0;
		if( dwWarpLen2 > 7 )
		{
			_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr2,sizeof(dwDoorObjNameAddr2));
		}
		else
		{
			dwDoorObjNameAddr2 = dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
		}

		char szReadWarpName2[256] = {0};
		_ReadProcessMemory(dwDoorObjNameAddr2,szReadWarpName2,dwWarpLen2*2);

		DWORD dwWarpNameLen2 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName2,-1,NULL,0,NULL,FALSE);

		char* pszWarpName2 = NULL;
		pszWarpName2 = new char[dwWarpNameLen2+2];

		if( pszWarpName2 != NULL )
		{
			memset(pszWarpName2,0,dwWarpNameLen2+2);
			WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName2,-1,pszWarpName2,dwWarpNameLen2,NULL,FALSE);
		}

		float	fx2;
		float	fy2;
		float	fz2;

		_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx2,sizeof(fx2));
		_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy2,sizeof(fy2));
		_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz2,sizeof(fz2));

		ptheCurRoom->str_Coord[1].str_fx = fx2;
		ptheCurRoom->str_Coord[1].str_fy = fy2;
		ptheCurRoom->str_Coord[1].str_fz = fz2;


		////////////////////////////////////////////////////////////////////////////////////////////
		DWORD dwDoorObjAddr3 = 0;
		_ReadProcessMemory(theRoomDoorObjAddr.str_dwBinaryTreeNullMapAddr+8,&dwDoorObjAddr3,sizeof(dwDoorObjAddr3));
		assert(dwDoorObjAddr3);

		DWORD dwWarpLen3 = 0;

		_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen3,sizeof(dwWarpLen3));

		DWORD dwDoorObjNameAddr3 = 0;
		if( dwWarpLen3 > 7 )
		{
			_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr3,sizeof(dwDoorObjNameAddr3));
		}
		else
		{
			dwDoorObjNameAddr3 = dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
		}

		char szReadWarpName3[256] = {0};
		_ReadProcessMemory(dwDoorObjNameAddr3,szReadWarpName3,dwWarpLen3*2);

		DWORD dwWarpNameLen3 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName3,-1,NULL,0,NULL,FALSE);

		char* pszWarpName3 = NULL;
		pszWarpName3 = new char[dwWarpNameLen3+2];

		if( pszWarpName3 != NULL )
		{
			memset(pszWarpName3,0,dwWarpNameLen3+2);
			WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName3,-1,pszWarpName3,dwWarpNameLen3,NULL,FALSE);
		}

		float	fx3;
		float	fy3;
		float	fz3;

		_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx3,sizeof(fx3));
		_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy3,sizeof(fy3));
		_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz3,sizeof(fz3));

		ptheCurRoom->str_Coord[2].str_fx = fx3;
		ptheCurRoom->str_Coord[2].str_fy = fy3;
		ptheCurRoom->str_Coord[2].str_fz = fz3;

		////////////////////////////////////////////////////////////////////////////////////////////


		char szWarpPrint[256] = {0};
		sprintf_s(szWarpPrint,"warp1 obj addr = 0x%p warp name = %s x = %f y = %f z = %f\r\n",
			dwDoorObjAddr1,pszWarpName1,fx1,fy1,fz1);
		csTxt += szWarpPrint;
		sprintf_s(szWarpPrint,"warp2 obj addr = 0x%p warp name = %s  x = %f y = %f z = %f\r\n",
			dwDoorObjAddr2,pszWarpName2,fx2,fy2,fz2);
		csTxt += szWarpPrint;
		sprintf_s(szWarpPrint,"warp3 obj addr = 0x%p warp name = %s  x = %f y = %f z = %f\r\n",
			dwDoorObjAddr3,pszWarpName3,fx3,fy3,fz3);

		csTxt += szWarpPrint;

		try
		{
			memcpy(ptheCurRoom->str_wszFbNameEx,szReadMapData,dwMapLen*2+1);
			memcpy(ptheCurRoom->str_wszRoomSerialEx,szReadRoomIdName,dwRoomIdLen*2+1);
			memcpy(ptheCurRoom->str_wszRoomWarpEx[0],szReadWarpName1,dwWarpLen1*2+2);
			memcpy(ptheCurRoom->str_wszRoomWarpEx[1],szReadWarpName2,dwWarpLen2*2+2);
			memcpy(ptheCurRoom->str_wszRoomWarpEx[2],szReadWarpName3,dwWarpLen3*2+2);
			memcpy(ptheCurRoom->str_szRoomSerial,pszRoomIdName,dwRoomIdNameLen+1);
		}
		catch (...)
		{
			assert(false);
		}

		////////////////////////////////////////////////////////////////////////////////////////////
		if( pszRoomObjMapData != NULL )
		{
			delete[] pszRoomObjMapData;
			pszRoomObjMapData = NULL;
		}

		if( pszRoomIdName != NULL )
		{
			delete[] pszRoomIdName;
			pszRoomIdName = NULL;
		}

		if( pszWarpName3 != NULL )
		{
			delete[] pszWarpName3;
			pszWarpName3 = NULL;
		}

		if( pszWarpName2 != NULL )
		{
			delete[] pszWarpName2;
			pszWarpName2 = NULL;
		}

		if( pszWarpName1 != NULL )
		{
			delete[] pszWarpName1;
			pszWarpName1 = NULL;
		}

	}

	

	

// 	unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
// 	_WriteProcessMemory(JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK,szCode,5);
	return false;
}


bool JFZRRoom::UpDataCity(PROOM ptheCurRoom)
{
	csTxt = "";
	g_dwLocalRoomObjAddr = 0;
	// 只有在城镇中，hook的地址才会执行
	Hook(JFZR_REVERSAL_LOCAL_CITY_OBJ_HOOK,HookGetLocalObjRoomObj);

	while(!g_dwLocalRoomObjAddr)
	{
		Sleep(10);
	}

	DWORD dwLocalRoomObjAddr = g_dwLocalRoomObjAddr;

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD	dwMapLen = 0;
	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_LEN_OFFSET___,&dwMapLen,sizeof(dwMapLen));

	DWORD dwRoomMapDataAddr = 0;
	if( dwMapLen > 7 )
	{
		_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_OFFSET___,&dwRoomMapDataAddr,sizeof(dwRoomMapDataAddr));
	}
	else
	{
		dwRoomMapDataAddr = dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_MAP_OFFSET___;
	}

	char szReadMapData[256] = {0};
	_ReadProcessMemory(dwRoomMapDataAddr,szReadMapData,dwMapLen*2);

	DWORD dwMapLen_ = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadMapData,-1,NULL,0,NULL,FALSE);

	char* pszRoomObjMapData = NULL;
	pszRoomObjMapData = new char[dwMapLen_+2];

	if( pszRoomObjMapData != NULL )
	{
		memset(pszRoomObjMapData,0,dwMapLen_+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadMapData,-1,pszRoomObjMapData,dwMapLen_,NULL,FALSE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwRoomIdLen = 0;

	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_LEN_OFFSET___,&dwRoomIdLen,sizeof(dwRoomIdLen));

	DWORD dwRoomIdNameAddr = 0;
	if( dwRoomIdLen > 7 )
	{
		_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_OFFSET___,&dwRoomIdNameAddr,sizeof(dwRoomIdNameAddr));
	}
	else
	{
		dwRoomIdNameAddr = dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_ID_OFFSET___;
	}

	char szReadRoomIdName[256] = {0};
	_ReadProcessMemory(dwRoomIdNameAddr,szReadRoomIdName,dwRoomIdLen*2);

	DWORD dwRoomIdNameLen = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadRoomIdName,-1,NULL,0,NULL,FALSE);

	char* pszRoomIdName = NULL;
	pszRoomIdName = new char[dwRoomIdNameLen+2];

	if( pszRoomIdName != NULL )
	{
		memset(pszRoomIdName,0,dwRoomIdNameLen+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadRoomIdName,-1,pszRoomIdName,dwRoomIdNameLen,NULL,FALSE);
	}

	DWORD dwRoomDoorObjAddr = 0;
	_ReadProcessMemory(dwLocalRoomObjAddr+JFZR_REVERSAL_LOCAL_ROOM_DOOR_OBJ_OFFSET___+4,&dwRoomDoorObjAddr,sizeof(dwRoomDoorObjAddr));
	assert(dwRoomDoorObjAddr);

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwDoorObjAddr1 = 0;
	_ReadProcessMemory(dwRoomDoorObjAddr+0,&dwDoorObjAddr1,sizeof(dwDoorObjAddr1));
	assert(dwDoorObjAddr1);

	DWORD dwWarpLen1 = 0;

	_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen1,sizeof(dwWarpLen1));

	DWORD dwDoorObjNameAddr1 = 0;
	if( dwWarpLen1 > 7 )
	{
		_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr1,sizeof(dwDoorObjNameAddr1));
	}
	else
	{
		dwDoorObjNameAddr1 = dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
	}

	char szReadWarpName1[256] = {0};
	_ReadProcessMemory(dwDoorObjNameAddr1,szReadWarpName1,dwWarpLen1*2);

	DWORD dwWarpNameLen1 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName1,-1,NULL,0,NULL,FALSE);

	char* pszWarpName1 = NULL;
	pszWarpName1 = new char[dwWarpNameLen1+2];

	if( pszWarpName1 != NULL )
	{
		memset(pszWarpName1,0,dwWarpNameLen1+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName1,-1,pszWarpName1,dwWarpNameLen1,NULL,FALSE);
	}

	float	fx1;
	float	fy1;
	float	fz1;

	_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx1,sizeof(fx1));
	_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy1,sizeof(fy1));
	_ReadProcessMemory(dwDoorObjAddr1+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz1,sizeof(fz1));

	ptheCurRoom->str_Coord[0].str_fx = fx1;
	ptheCurRoom->str_Coord[0].str_fy = fy1;
	ptheCurRoom->str_Coord[0].str_fz = fz1;

	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwDoorObjAddr2 = 0;
	_ReadProcessMemory(dwRoomDoorObjAddr+4,&dwDoorObjAddr2,sizeof(dwDoorObjAddr2));
	assert(dwDoorObjAddr2);

	DWORD dwWarpLen2 = 0;

	_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen2,sizeof(dwWarpLen2));

	DWORD dwDoorObjNameAddr2 = 0;
	if( dwWarpLen2 > 7 )
	{
		_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr2,sizeof(dwDoorObjNameAddr2));
	}
	else
	{
		dwDoorObjNameAddr2 = dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
	}

	char szReadWarpName2[256] = {0};
	_ReadProcessMemory(dwDoorObjNameAddr2,szReadWarpName2,dwWarpLen2*2);

	DWORD dwWarpNameLen2 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName2,-1,NULL,0,NULL,FALSE);

	char* pszWarpName2 = NULL;
	pszWarpName2 = new char[dwWarpNameLen2+2];

	if( pszWarpName2 != NULL )
	{
		memset(pszWarpName2,0,dwWarpNameLen2+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName2,-1,pszWarpName2,dwWarpNameLen2,NULL,FALSE);
	}

	float	fx2;
	float	fy2;
	float	fz2;

	_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx2,sizeof(fx2));
	_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy2,sizeof(fy2));
	_ReadProcessMemory(dwDoorObjAddr2+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz2,sizeof(fz2));

	ptheCurRoom->str_Coord[1].str_fx = fx2;
	ptheCurRoom->str_Coord[1].str_fy = fy2;
	ptheCurRoom->str_Coord[1].str_fz = fz2;


	////////////////////////////////////////////////////////////////////////////////////////////
	DWORD dwDoorObjAddr3 = 0;
	_ReadProcessMemory(dwRoomDoorObjAddr+8,&dwDoorObjAddr3,sizeof(dwDoorObjAddr3));
	assert(dwDoorObjAddr3);

	DWORD dwWarpLen3 = 0;

	_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___,&dwWarpLen3,sizeof(dwWarpLen3));

	DWORD dwDoorObjNameAddr3 = 0;
	if( dwWarpLen3 > 7 )
	{
		_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___,&dwDoorObjNameAddr3,sizeof(dwDoorObjNameAddr3));
	}
	else
	{
		dwDoorObjNameAddr3 = dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
	}

	char szReadWarpName3[256] = {0};
	_ReadProcessMemory(dwDoorObjNameAddr3,szReadWarpName3,dwWarpLen3*2);

	DWORD dwWarpNameLen3 = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName3,-1,NULL,0,NULL,FALSE);

	char* pszWarpName3 = NULL;
	pszWarpName3 = new char[dwWarpNameLen3+2];

	if( pszWarpName3 != NULL )
	{
		memset(pszWarpName3,0,dwWarpNameLen3+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadWarpName3,-1,pszWarpName3,dwWarpNameLen3,NULL,FALSE);
	}

	float	fx3;
	float	fy3;
	float	fz3;

	_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___,&fx3,sizeof(fx3));
	_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_Y_OFFSET___,&fy3,sizeof(fy3));
	_ReadProcessMemory(dwDoorObjAddr3+JFZR_REVERSAL_LOCAL_ROOM_Z_OFFSET___,&fz3,sizeof(fz3));

	ptheCurRoom->str_Coord[2].str_fx = fx3;
	ptheCurRoom->str_Coord[2].str_fy = fy3;
	ptheCurRoom->str_Coord[2].str_fz = fz3;

	////////////////////////////////////////////////////////////////////////////////////////////
	char szPrint[256] = {0};
	sprintf_s(szPrint,"room obj addr = 0x%p map name = %s  room name = %s\r\n",dwLocalRoomObjAddr,pszRoomObjMapData,pszRoomIdName);

	csTxt += szPrint;

	char szWarpPrint[256] = {0};
	sprintf_s(szWarpPrint,"warp1 obj addr = 0x%p warp name = %s x = %f y = %f z = %f\r\n",
		dwDoorObjAddr1,pszWarpName1,fx1,fy1,fz1);
	csTxt += szWarpPrint;
	sprintf_s(szWarpPrint,"warp2 obj addr = 0x%p warp name = %s  x = %f y = %f z = %f\r\n",
		dwDoorObjAddr2,pszWarpName2,fx2,fy2,fz2);
	csTxt += szWarpPrint;
	sprintf_s(szWarpPrint,"warp3 obj addr = 0x%p warp name = %s  x = %f y = %f z = %f\r\n",
		dwDoorObjAddr3,pszWarpName3,fx3,fy3,fz3);

	csTxt += szWarpPrint;

	memcpy(ptheCurRoom->str_wszFbNameEx,szReadMapData,dwMapLen*2+1);
	memcpy(ptheCurRoom->str_wszRoomSerialEx,szReadRoomIdName,dwRoomIdLen*2+1);
	memcpy(ptheCurRoom->str_wszRoomWarpEx[0],szReadWarpName1,dwWarpLen1*2+2);
	memcpy(ptheCurRoom->str_wszRoomWarpEx[1],szReadWarpName2,dwWarpLen2*2+2);
	memcpy(ptheCurRoom->str_wszRoomWarpEx[2],szReadWarpName3,dwWarpLen3*2+2);
	memcpy(ptheCurRoom->str_szRoomSerial,pszRoomIdName,dwRoomIdNameLen+1);


	////////////////////////////////////////////////////////////////////////////////////////////
	if( pszRoomObjMapData != NULL )
	{
		delete[] pszRoomObjMapData;
		pszRoomObjMapData = NULL;
	}

	if( pszRoomIdName != NULL )
	{
		delete[] pszRoomIdName;
		pszRoomIdName = NULL;
	}

	if( pszWarpName3 != NULL )
	{
		delete[] pszWarpName3;
		pszWarpName3 = NULL;
	}

	if( pszWarpName2 != NULL )
	{
		delete[] pszWarpName2;
		pszWarpName2 = NULL;
	}

	if( pszWarpName1 != NULL )
	{
		delete[] pszWarpName1;
		pszWarpName1 = NULL;
	}

	// 	unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
	// 	_WriteProcessMemory(JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK,szCode,5);
	return false;
}

extern bool	g_bStopDoFb;
bool JFZRRoom::DoFb(wchar_t* wszFbName,DWORD dwLevel,wchar_t* wszBoosSerialEx)
{
	JFZRActionSystem theActionSystem;
	theActionSystem.StartStage(wszFbName,L"StageStartWarpGate01",dwLevel,0);

	CList<ROOM*,ROOM*> theRoomList;
	int		nIdex = 0;
	bool	bPopRoom = false;
	while(true)
	{
		JFZRRoom theRoom;
		// new
		PROOM ptheCurRoom = new ROOM;
		memset(ptheCurRoom,0,sizeof(ROOM));
		// 等待进入房间后，刷新当前房间数据
		theRoom.UpDataRoom(ptheCurRoom);
		nIdex++;

		// 开始打怪
		while( theActionSystem.useskill(/*"DoubleSlash"*/"ThunderSlash",0,0) )
		{
			Sleep(1);
		}

		// 房间已经被清空
		ptheCurRoom->str_bIsClear = true;

		if( wcscmp(ptheCurRoom->str_wszRoomSerialEx,wszBoosSerialEx) == 0 )
		{
			// 释放内存
			MessageBox(NULL,"BOOS已经打完","恭喜",MB_OK);
			JFZRActionSystem theActionSystem;
			theActionSystem.PickUpItem();
			// 主动停止副本
			if( g_bStopDoFb == true )
			{
				return true;
			}

			theActionSystem.RepairItem();

			theActionSystem.StartStage(wszFbName,L"",dwLevel,1);
			Sleep(3000);
			continue;
		}

		Sleep(2000);
		JFZRActionSystem theActionSystem;
		theActionSystem.PickUpItem();
		Sleep(1000);

		if( push(ptheCurRoom,theRoomList) )
		{
			JFZRActionSystem theActionSystem;
			theActionSystem.GotoNextRoom(ptheCurRoom->str_wszRoomWarpEx[ptheCurRoom->str_i]);
		}
		else
		{
			ptheCurRoom = pop(theRoomList);
			JFZRActionSystem theActionSystem;
			theActionSystem.GotoNextRoom(ptheCurRoom->str_wszRoomWarpEx[ptheCurRoom->str_i]);
			delete[] ptheCurRoom;
			ptheCurRoom = NULL;
		}

		// 主动停止副本
		if( g_bStopDoFb == true )
		{
			return true;
		}

		Sleep(3000);
	}

	return false;
}

bool JFZRRoom::push(PROOM ptheRoom,CList<ROOM*,ROOM*>& theRoomList)
{

	// 第一个房间，直接push
	if( wcscmp(ptheRoom->str_wszRoomSerialEx,L"Sector01") == 0 )
	{
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	// 如果只有warp01,需要pop,说明已经到达一个分支的尽头
	if( wcscmp(ptheRoom->str_wszRoomWarpEx[0],L"Warp01") == 0 &&
		wcscmp(ptheRoom->str_wszRoomWarpEx[1],L"Warp01") == 0 &&
		wcscmp(ptheRoom->str_wszRoomWarpEx[2],L"Warp01") == 0)
	{
		ptheRoom->str_i = 0;
		theRoomList.AddHead(ptheRoom);
		return false;
	}

	// 判断当前房间是否是上一个房间
	PROOM ptheListRoom = NULL;
	if( theRoomList.GetCount() )
	{
		ptheListRoom = theRoomList.GetHead();
		assert(ptheListRoom);

		if( strcmp(ptheRoom->str_szRoomSerial,ptheListRoom->str_szRoomSerial) == 0 )
		{
			return false;
		}
	}

	// 如果存在warp02
	if( wcscmp(ptheRoom->str_wszRoomWarpEx[0],L"Warp02") == 0 )
	{
		ptheRoom->str_bRoom2Flag = true;
		ptheRoom->str_i = 0;
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	if( wcscmp(ptheRoom->str_wszRoomWarpEx[1],L"Warp02") == 0 )
	{
		ptheRoom->str_bRoom2Flag = true;
		ptheRoom->str_i = 1;
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	if( wcscmp(ptheRoom->str_wszRoomWarpEx[2],L"Warp02") == 0 )
	{
		ptheRoom->str_bRoom2Flag = true;
		ptheRoom->str_i = 2;
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	// 如果存在warp03
	if( wcscmp(ptheRoom->str_wszRoomWarpEx[0],L"Warp03") == 0 )
	{
		ptheRoom->str_bRoom3Flag = true;
		ptheRoom->str_i = 0;
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	if( wcscmp(ptheRoom->str_wszRoomWarpEx[1],L"Warp03") == 0 )
	{
		ptheRoom->str_bRoom3Flag = true;
		ptheRoom->str_i = 1;
		theRoomList.AddHead(ptheRoom);
		return true;
	}

	if( wcscmp(ptheRoom->str_wszRoomWarpEx[2],L"Warp03") == 0 )
	{
		ptheRoom->str_bRoom3Flag = true;
		ptheRoom->str_i = 2;
		theRoomList.AddHead(ptheRoom);
		return true;
	}


	return true;
}

PROOM JFZRRoom::pop(CList<ROOM*,ROOM*>& theRoomList)
{
	PROOM ptheRoom = theRoomList.RemoveHead();
	assert(ptheRoom);

	ptheRoom->str_i = 0;

	// 如果2门未打过
	if( ptheRoom->str_bRoom2Flag == false )
	{
		ptheRoom->str_i = 1;
	}

	if( ptheRoom->str_bRoom3Flag == false )
	{
		ptheRoom->str_i = 2;
	}
	return ptheRoom;
}
