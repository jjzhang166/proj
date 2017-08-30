#include "stdafx.h"
#include "UsefulFns.h"
#include "../gwlrbase.h"
#include "GameStructs.h"
#include <CommonX/CmnX_HookerMore.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//*********************全局变量*******************************************
//是否开启无敌
volatile bool g_enable_wu_di_ = true;
//////////////////////////////////////////////////////////////////////////
// 获取的对象地址和遍历到的对象地址是一样的
// 自己代码实现判断，代码不好看，顾调用游戏函数
// 参数为本地角色id
DWORD GetActorObjAddr(DWORD dwActorId)
{
	DWORD dwActorObjAddr = 0;
	try
	{
		DWORD dwMapAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if(IsUnValidPtr(dwMapAddr))
			return 0;
		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET1);
		if(IsUnValidPtr(dwMapAddr))
			return 0;
		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET2);
		if(IsUnValidPtr(dwMapAddr))
			return 0;
		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET3);
		if(IsUnValidPtr(dwMapAddr))
			return 0;
		dwMapAddr += MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET4___;

		DWORD dwGetActorObjAddrCall = MMOCLIENT_REVERSAL_GET_ACTOR_OBJADDR_CALL+GetCryGameBaseAddr();
		if( dwMapAddr != 0 )
		{
			__asm
			{
				lea eax, dwActorId
				push eax
				lea eax, dwActorObjAddr
				push eax
				mov ecx, dwMapAddr
				mov eax, dwGetActorObjAddrCall
				call eax
			}

			if( dwActorObjAddr == dwMapAddr )
			{
				return 0;
			}
			if (IsUnValidPtr(dwActorObjAddr))
				return 0;
			PMMO_ACTOR ptheActor = PMMO_ACTOR(dwActorObjAddr);

			dwActorObjAddr = ptheActor->dwObjAddr;
		}
	}
	catch (...)
	{
		assert(false);
	}
	return dwActorObjAddr;
}

bool UseSkill( int skill_id, const void* skill_obj_addr )
{
	DWORD dwValue[20];

	if( skill_id == MMOCLIENT_REVERSAL_SKILL_ID_KEYBOARD_F )
	{
		dwValue[0] = 4;
		dwValue[1] = 1;
		dwValue[2] = skill_id;
		dwValue[3] = 0xFFFFFFFF;
		dwValue[4] = 0xFFFFFFFF;
		dwValue[5] = 0xFFFFFFFF;
		dwValue[6] = 0xBF800000;
		dwValue[7] = 0;
		dwValue[8] = 0;
		dwValue[9] = 0;
		dwValue[10] = 2;
		dwValue[11] = 0x24;
		dwValue[12] = 0x17;
	}
	else
	{
		dwValue[0] = 4;
		dwValue[1] = 1;
		dwValue[2] = skill_id;
		dwValue[3] = 0xFFFFFFFF;
		dwValue[4] = 0xFFFFFFFF;
		dwValue[5] = 0xFFFFFFFF;
		dwValue[6] = 0xBF800000;
		dwValue[7] = 0;
		dwValue[8] = 0;
		dwValue[9] = 0;
		dwValue[10] = 0;
		dwValue[11] = 0;
		dwValue[12] = 0;
	}

	if( skill_obj_addr != 0 )
	{
		DWORD dwSkillCall = MMOCLIENT_REVERSAL_SKILL_CALL+GetCryGameBaseAddr();
		try
		{
			__asm
			{
				mov ecx, skill_obj_addr
				lea eax, dwValue
				push eax
				mov eax, dwSkillCall
				call eax
			}

			return true;
		}
		catch (...)
		{

		}
	}
	return false;
}


bool UseItem(int nItemTabIndex, int nItemSlotIndex)
{
	try
	{
		DWORD dwUseItemAddr = *(DWORD*)(MMOCLIENT_REVERSAL_USE_ITEM_OBJ_ADDR+GetCryGameBaseAddr());

		if( dwUseItemAddr )
		{
			*(DWORD*)(dwUseItemAddr+MMOCLIENT_REVERSAL_USE_ITEM_TAB_INDEX_OFFSET___) = nItemTabIndex;		// 背包栏=0,任务栏=5
			*(DWORD*)(dwUseItemAddr+MMOCLIENT_REVERSAL_USE_ITEM_SLOT_INDEX_OFFSET___) = nItemSlotIndex;		// 物品格子
			*(DWORD*)(dwUseItemAddr+MMOCLIENT_REVERSAL_USE_ITEM_TRIGGER_FLAG_OFFSET___) = 0x03;				// 触发使用物品的标志  0x03

			return true;
		}
	}
	catch (...)
	{
		assert(false);
	}
	return false;
}

DWORD GetTaskAddr(DWORD dwTaskId)
{
	DWORD dwTaskObjAddr = 0;
	try
	{
		DWORD dwGetTaskObjAddrCall = MMOCLIENT_REVERSAL_GET_TASK_OBJ_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwTaskId
			mov eax, dwGetTaskObjAddrCall
			call eax
			add esp, 4
			mov dwTaskObjAddr, eax
		}
	}
	catch (...)
	{
		return 0;
	}

	return dwTaskObjAddr;
}

bool IsHaveTaskId_npc( DWORD dwPlayerAddr, DWORD dwTaskId )
{
	bool bRet = false;
	try
	{

// 		__asm		// 老版本的数据
// 		{
// 			push MMOCLIENT_REVERSAL_GAME_OBJ_TASK_ADDR		
// 			mov eax, dwTaskId
// 			push eax
// 			mov ebx, dwPlayerAddr
// 			MOV ECX,DWORD PTR DS:[EBX+MMOCLIENT_REVERSAL_GAME_OBJ_OFFSET1]	// 0x00001FA0
// 			MOV EDX,DWORD PTR DS:[ECX]
// 			MOV EAX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_GAME_OBJ_IS_RECEVIED_TASK_VFUNADDR_OFFSET]	// 0x0000000C
// 			CALL EAX
// 			mov bRet, al
// 		}

		DWORD dwGetCanAccpetTaskCall = MMOCLINET_REVERSAL_GAME_GET_CAN_ACCPET_TASK_CALL+GetCryGameBaseAddr();

		__asm
		{
// 			mov ebx, DWORD PTR DS:[0x02582C74]	// 02582C74  305ABA40
// 			push ebx

			mov eax, dwPlayerAddr
			MOV ECX,DWORD PTR DS:[eax+MMOCLIENT_REVERSAL_GAME_OBJ_OFFSET1]

			mov eax, dwTaskId
			push eax

			mov eax, dwGetCanAccpetTaskCall
			call eax

			mov bRet, al

		}
		return bRet;
	}
	catch (...)
	{
		assert(false);
	}
	return bRet;
}

// 函数功能： 通过参数id获取对象，此id为封包id
// 访问角色的地址为 [[[[0x01926D00+0C370000]+0x000000CC]+0x1C]+0x214]
DWORD GetObjAddr(DWORD dwObjPacketId)
{
	DWORD dwObjAddr = 0;
	try
	{

		DWORD dwGameObjAddr = GetCryActionBaseAddr()+MMOCLIENT_REVERSAL_GAME_OBJ_ADDR;
		DWORD dwGameObjCall = GetCryActionBaseAddr()+MMOCLIENT_REVERSAL_GAME_OBJ_CALL;
		__asm
		{
			mov ecx, dwGameObjAddr
			mov ecx, dword ptr [ecx]
			mov eax, dwObjPacketId
			push eax
			mov eax, dwGameObjCall
			call eax
			mov dwObjAddr, eax
		}
	}
	catch (...)
	{
		//assert(false);
	}

	return dwObjAddr;
}



// 函数功能： 通过采集物品的id，获取到物品的另一个对象地址，用于取组包数据
// DWORD GetCollectObjAddr(DWORD dwPacketId)
// {
// 	DWORD dwObjAddr = 0;
// 	try
// 	{
// 		__asm
// 		{
// 			mov eax, MMOCLIENT_REVERSAL_GAME_OBJ_ADDR
// 			mov eax, dword ptr [eax]
// 			MOV ECX,DWORD PTR DS:[EAX+MMOCLIENT_REVERSAL_COLLECT_VFUN_ADDR_OFFSET1]
// 			MOV EDX,DWORD PTR DS:[ECX]
// 			MOV EAX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_COLLECT_VFUN_ADDR_OFFSET2]
// 			mov edi, dwPacketId
// 			push edi
// 			CALL EAX
// 			mov dwObjAddr, eax
// 		}
// 	}
// 	catch (...)
// 	{
// 		assert(false);
// 	}
// 	return dwObjAddr;
// }

DWORD GetInFbInfoAddr(DWORD dwFbId)
{
	DWORD dwInfbInfoAddr = 0;

	try
	{
		DWORD dwGetFbInfoAddrCall = MMOCLIENT_REVERSAL_GET_FB_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			mov eax, dwFbId
			push eax
			mov eax, dwGetFbInfoAddrCall
			call eax
			mov dwInfbInfoAddr, eax
			add esp, 4
		}
	}
	catch (...)
	{
		
	}


// 	char* pszFbLevel = "CMHLevelInfo";
// 	try
// 	{
// 		__asm
// 		{
// 			push 0
// 			push pszFbLevel
// 			push 0
// 			mov eax, MMOCLIENT_REVERSAL_GET_FB_BASE_ADDR_CALL
// 			call eax
// 			add esp, 0x0c
// 			mov ecx, eax
// 			mov eax, dwFbId
// 			push eax
// 			mov eax, MMOCLIENT_REVERSAL_GET_FB_INFO_ADDR_CALL
// 			call eax
// 			mov dwInfbInfoAddr, eax
// 		}
// 	}
// 	catch (...)
// 	{
// 
// 	}
	return dwInfbInfoAddr;
}


DWORD GetTemplateItemAddr(DWORD dwItemId)
{
	// $+2      004658B2      68 A0D00602      PUSH MHOClien.0206D0A0                   ; ASCII "CItemBaseInfo"
	DWORD dwItemInfoAddr = 0;
	try
	{
		DWORD dwGetItemInfoAddrCall = MMOCLIENT_REVERSAL_GET_ITEM_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			mov eax, dwItemId
			push eax
			mov eax, dwGetItemInfoAddrCall
			call eax
			add esp, 4
			mov dwItemInfoAddr, eax
		}
	}
	catch (...)
	{

	}

	return dwItemInfoAddr;
}

//////////////////////////////////////////////////////////////////////////
// 优化


void Hook2( const char* hooker_name, DWORD dwHookAddr,LPVOID pfun)
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

DWORD DoCutscene(DWORD dwCutSceneInfoAddr)
{
	try
	{
		DWORD dwCutSecenInfoId = *(DWORD*)(dwCutSceneInfoAddr+0x14);

		if( dwCutSecenInfoId == 0x907 )
		{
			return dwCutSceneInfoAddr;
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	return 0;
}

_declspec(naked) void HookCutscene()
{
	__asm
	{
		MOV EAX,DWORD PTR DS:[EAX]
		MOV EAX,DWORD PTR DS:[EAX+EDX*4]
		
		push EAX
		call DoCutscene
		add esp, 4
		retn 
	}
// 	__asm
// 	{
// 		retn 4
// 	}
}

void Optimize_Cutscene()
{
	//Hook2(MMOCLIENT_REVERSAL_OPTIMIZE_CUTSCENE_HOOK,HookCutscene);
	//Hook2(MMOCLIENT_REVERSAL_OPTIMIZE_CUTSCENE_HOOK2___,HookCutscene);
}


//////////////////////////////////////////////////////////////////////////
// 鼠标操作
void mouse_down(int x, int y, int nFlags)
{
	try
	{
		DWORD dwGwlrWindowsBaseAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GWLR_WINDOWS_BASE_ADDR+GetCryGameBaseAddr());
		__asm
		{
			MOV EAX,DWORD PTR DS:[dwGwlrWindowsBaseAddr]

			MOV EAX,DWORD PTR DS:[EAX+MMOCLIENT_REVERSAL_MOUSE_LBUTTONDOWN_OFFSET1]
			MOV EDX,DWORD PTR DS:[EAX]
			MOV EDX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_MOUSE_LBUTTONDOWN_OFFSET2___]

			push 0
			mov esi, nFlags
			push esi
			mov esi, y
			push esi
			mov ecx, x
			push ecx
			MOV ECX,EAX
			CALL EDX
		}
	}
	catch (...)
	{
		assert(false);
	}
}

DWORD GetCraftInfoAddr(DWORD dwCraftId)
{
	DWORD dwCraftInfoAddr = 0;
	try
	{
		DWORD dwGetCraftInfoAddrCall = MMOCLIENT_REVERSAL_GET_CRAFT_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			mov eax, dwCraftId
			push eax
			mov eax, dwGetCraftInfoAddrCall
			call eax
			mov dwCraftInfoAddr, eax
			add esp, 4
		}
	}
	catch (...)
	{

	}
	return dwCraftInfoAddr;
}

DWORD GetEquipInfoAddr(int nIndex)
{
	DWORD dwEquipInfoAddr = 0;

	try
	{
		DWORD dwGetEquipInfoBaseAddrCall = MMOCLIENT_REVERSAL_GET_EQUIP_INF0_BASE_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			mov edx, nIndex
			push edx
			mov eax, dwGetEquipInfoBaseAddrCall
			call eax
			mov dwEquipInfoAddr, eax
			add esp, 4
		}
	}
	catch (...)
	{

	}
	return dwEquipInfoAddr;
}

//获得升级物品所需要的材料，参数1是物品等级，参数2是物品模板里+10C的值 MMOCLIENT_REVERSAL_ITEM_GETUPITEMARG_OFFSET____这个宏的值
DWORD  dwGetUpEquipInfoAddr(int nItemLeve,int nArg2)
{
	DWORD dwRet=0;
	__try
	{
		DWORD dwCallAddr=MMOCLIENT_REVERSAL_ITEM_UP_INFO_CALL+GetCryGameBaseAddr();

		__asm
		{
			push nItemLeve
			push nArg2
			mov eax,dwCallAddr
			call eax
			add esp,0x8
			mov dwRet,eax
		}

	}__except(1)
	{

	}
	return dwRet;
}


// dd [[[0x0208C0B4]+0x48]+0]
void DxPressKey(DWORD dwVk,DWORD dwFlags)
{
	try
	{
		DWORD dwDxAddr = *(DWORD*)(MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_BASE_ADDR);
		if( dwDxAddr == 0 )
			return;
		dwDxAddr = *(DWORD*)(dwDxAddr+MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_OFFSET1___);
		if( dwDxAddr == 0 )
			return;
		dwDxAddr = *(DWORD*)(dwDxAddr+MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_OFFSET2___);
		if( dwDxAddr == 0 )
			return;

		__asm
		{
			push dwFlags
			push dwVk
			mov ecx, dwDxAddr
			mov eax, MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_CALL
			call eax
		}
	}
	catch (...)
	{
		assert(false);
	}
}


//////////////////////////////////////////////////////////////////////////
// 多倍攻击

int g_nHitCount = 3;		// 默认攻击倍数

/*
void Hit(DWORD dwHitInfoAddr)
{
	DWORD dwHitCall = MMOCLIENT_REVERSAL_HIT_CALL+GetCryGameBaseAddr();
	DWORD dwHitBaseAddr = MMOCLIENT_REVERSAL_HIT_BASE_ADDR+GetCryGameBaseAddr();
	__asm
	{
		MOV ECX,DWORD PTR DS:[dwHitBaseAddr]
		mov esi, dwHitInfoAddr
		push esi
		mov eax, dwHitCall
		call eax
	}
}*/

DWORD g_dwGongJiBuWei = 0;

char g_szHitCode[0x15C*4];

/*
void SendHit(DWORD dwHitInfoAddr)
{
	try
	{
		if( dwHitInfoAddr == 0 )
			return ;

		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( dwGameAddr2 == 0 )
			return;

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( dwGameAddr2 == 0 )
			return;

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( dwGameAddr2 == 0 )
			return;

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( dwGameAddr2 == 0 )
			return;

		DWORD dwRoleId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);

		DWORD dwCurObjId = *(DWORD*)(dwHitInfoAddr);

		if( dwRoleId == dwCurObjId )
		{
			if( g_dwGongJiBuWei != 0 )
				*(DWORD*)(dwHitInfoAddr+0x48) = g_dwGongJiBuWei;
	
			char szPrint[256];
			sprintf_s(szPrint,"[LOG] 攻击部位 = 0x%p",*(DWORD*)(dwHitInfoAddr+0x48));
			OutputDebugStringA(szPrint);
			for (int i=0; i<g_nHitCount; i++)
			{
				Hit(dwHitInfoAddr);
			}
		}
	}
	catch (...)
	{

	}
}*/

DWORD GetHitCallHook()
{
	return MMOCLIENT_REVERSAL_HIT_CALL_HOOK+GetCryGameBaseAddr();
}
/*
_declspec(naked) void HookHit()
{
	__asm
	{
		call SendHit
		add esp, 4
		mov eax, GetHitCallHook
		call eax
		add eax, 5
		jmp eax
	}
}*/

byte g_bHitCallHookCode[20];
bool g_bHitCallHookFlag = false;

/*
void MMO_HookHit()
{
	ReadProcessMemory((HANDLE)-1,(LPCVOID)(MMOCLIENT_REVERSAL_HIT_CALL_HOOK+GetCryGameBaseAddr()),g_bHitCallHookCode,5,NULL);

	Hook2(MMOCLIENT_REVERSAL_HIT_CALL_HOOK+GetCryGameBaseAddr(),HookHit);

	g_bHitCallHookFlag = true;
}*/

void MMO_UnHookHit()
{
	if( g_bHitCallHookFlag )
	{
		WriteProcessMemory((HANDLE)-1,(LPVOID)(MMOCLIENT_REVERSAL_HIT_CALL_HOOK+GetCryGameBaseAddr()),g_bHitCallHookCode,5,NULL);
		g_bHitCallHookFlag = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 攻击部位测试


DWORD g_dwCode[54];

PMMO_BU_WEI_OBJ_STRUCT	g_ptheHitBuWeiObj = NULL;		// 指向当前被攻击对象的部位结构体数据
MMO_ActorVector*		g_ptheActorVector = NULL;		// 指向当前被攻击对象的数据，此数据和对象的addr2一样，但是内存不是同一个
DWORD					g_dwHitBuWeiId = 0x12;			// 攻击部位id

typedef struct
{
	MMO_Vector3		theVector1;
	MMO_Vector3		theVector2;
	MMO_Direction2	theDirection1;
	MMO_Direction2	theDirection2;
	MMO_Direction2	theDirection3;
	MMO_Direction2	theDirection4;
	MMO_Direction2	theDirection5;
	MMO_Direction2	theDirection6;
	DWORD			dwMonsterId;
	DWORD			dwGongJiBuWeiId;
	int				nUnKnow1;
	DWORD			dwUnKnow1;
	DWORD			dwUnKnow2;
	DWORD			dwUnKnow3;
	int				nFlag1;
	MMO_Vector3		theVector3;
	MMO_Vector3		theVectorZeor1;
	MMO_Direction2	theDirection7;
	DWORD			dwUnKnow4;
	DWORD			dwUnKnow5;
	DWORD			dwUnKnow6;
	float			fUnKnow1;		// +0x90
	DWORD			dwUnKnow94;
	DWORD			dwUnKnow98;
	DWORD			dwUnKnow9C;
	DWORD			dwUnKnowA0;
	DWORD			dwUnKnowA4;
	DWORD			dwUnKnowA8;
	DWORD			dwUnKnowAC;
	DWORD			dwUnKnowB0;
	DWORD			dwUnKnowB4;
	DWORD			dwUnKnowB8;
	DWORD			dwUnKnowBC;
	DWORD			dwUnKnowC0;
}MMO_HIT_PARAMETER,*PMMO_HIT_PARAMETER;

void SendHitParameter(DWORD dwHitInfoAddr)
{
	try
	{
		PMMO_HIT_PARAMETER ptheHitParameter = (PMMO_HIT_PARAMETER)dwHitInfoAddr;

		char szPrint[1024];
		sprintf_s(szPrint,"[LOG] 攻击对象id = 0x%p 攻击部位id = 0x%p %f %f %f %f %f",
			ptheHitParameter->dwMonsterId,
																	*(DWORD*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_ID_PARAMETER_OFFSET___),
																	*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_X_PARAMETER_OFFSET___),
																	*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_Y_PARAMETER_OFFSET___),
																	*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET___),
																	*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET___),
																	*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET___));
		OutputDebugStringA(szPrint);
/*++
		PMMO_HIT_PARAMETER ptheHitParameter = (PMMO_HIT_PARAMETER)dwHitInfoAddr;
// 		sprintf_s(szPrint,"[LOG] 攻击部位浮点 %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f 0x%p 0x%p 0x%p %f %f %f %f %f 0x%p 0x%p 0x%p %f",
// 										ptheHitParameter->theVector1.fx,
// 										ptheHitParameter->theVector1.fy,
// 										ptheHitParameter->theVector1.fz,
// 										ptheHitParameter->theVector2.fx,
// 										ptheHitParameter->theVector2.fy,
// 										ptheHitParameter->theVector2.fz,
// 										ptheHitParameter->theDirection1.dir_x,
// 										ptheHitParameter->theDirection1.dir_y,
// 										ptheHitParameter->theDirection2.dir_x,
// 										ptheHitParameter->theDirection2.dir_y,
// 										ptheHitParameter->theDirection3.dir_x,
// 										ptheHitParameter->theDirection3.dir_y,
// 										ptheHitParameter->theDirection4.dir_x,
// 										ptheHitParameter->theDirection4.dir_y,
// 										ptheHitParameter->theDirection5.dir_x,
// 										ptheHitParameter->theDirection5.dir_y,
// 										ptheHitParameter->theDirection6.dir_x,
// 										ptheHitParameter->theDirection6.dir_y,
// 										ptheHitParameter->dwUnKnow1,
// 										ptheHitParameter->dwUnKnow2,
// 										ptheHitParameter->dwUnKnow3,
// 										ptheHitParameter->theVector3.fx,
// 										ptheHitParameter->theVector3.fy,
// 										ptheHitParameter->theVector3.fz,
// 										ptheHitParameter->theDirection7.dir_x,
// 										ptheHitParameter->theDirection7.dir_y,
// 										ptheHitParameter->dwUnKnow4,
// 										ptheHitParameter->dwUnKnow5,
// 										ptheHitParameter->dwUnKnow6,
// 										ptheHitParameter->fUnKnow1
// 			);

		sprintf_s(szPrint,"[LOG] 攻击部位浮点 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p",
						ptheHitParameter->dwUnKnow94,
						ptheHitParameter->dwUnKnow98,
						ptheHitParameter->dwUnKnow9C,
						ptheHitParameter->dwUnKnowA0,
						ptheHitParameter->dwUnKnowA4,
						ptheHitParameter->dwUnKnowA8,
						ptheHitParameter->dwUnKnowAC,
						ptheHitParameter->dwUnKnowB8
						);
		OutputDebugStringA(szPrint);


		if( g_ptheHitBuWeiObj != NULL && g_ptheActorVector != NULL )
		{
			*(DWORD*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_ID_PARAMETER_OFFSET___) = g_ptheHitBuWeiObj->dwBuWeiId;

			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_X_PARAMETER_OFFSET___) = 0.62f;		// 攻击部位时，角色武器的面向对象的dir2数据
			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_Y_PARAMETER_OFFSET___) = 0.77f;

			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET___) = g_ptheHitBuWeiObj->theRangeLeft.fx-0.2f;
			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET___) = g_ptheHitBuWeiObj->theRangeLeft.fy-1.0f;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET___) = g_ptheHitBuWeiObj->theRangeRight.fz+0.2f;

			// 把其他的坐标都修改成攻击点的坐标，这样攻击后，显示的血值是攻击部位弹出
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET4___) = g_ptheHitBuWeiObj->theRangeLeft.fx;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET4___) = g_ptheHitBuWeiObj->theRangeLeft.fy;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET4___) = g_ptheHitBuWeiObj->theRangeLeft.fz;

			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET2___) = g_ptheHitBuWeiObj->theRangeLeft.fx;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET2___) = g_ptheHitBuWeiObj->theRangeLeft.fy;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET2___) = g_ptheHitBuWeiObj->theRangeLeft.fz;

			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET3___) = g_ptheHitBuWeiObj->theRangeLeft.fx;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET3___) = g_ptheHitBuWeiObj->theRangeLeft.fy;
			//*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET3___) = g_ptheHitBuWeiObj->theRangeLeft.fz;

			char szPrint[1024];
			sprintf_s(szPrint,"[LOG] 修改后攻击部位id = 0x%p %f %f %f %f %f",
																			*(DWORD*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_ID_PARAMETER_OFFSET___),
																			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_X_PARAMETER_OFFSET___),
																			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_Y_PARAMETER_OFFSET___),
																			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET___),
																			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET___),
																			*(FLOAT*)(dwHitInfoAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET___));
			OutputDebugStringA(szPrint);

			g_ptheHitBuWeiObj = NULL;		// 清0
			g_ptheActorVector = NULL;

// 			ptheHitParameter->theVector1.fx += 5.0f;
// 			ptheHitParameter->theVector1.fy += 5.0f;
// 			ptheHitParameter->theVector1.fz += 0.0f;
// 			ptheHitParameter->theVector2.fx += 5.0f;
// 			ptheHitParameter->theVector2.fy += 5.0f;
// 			ptheHitParameter->theVector2.fz += 0.0f;

// 			ptheHitParameter->theDirection1.dir_x = 0.0f;
// 			ptheHitParameter->theDirection1.dir_y = 0.0f;
// 			ptheHitParameter->theDirection2.dir_x = 0.0f;
// 			ptheHitParameter->theDirection2.dir_y = 0.0f;
// 			ptheHitParameter->theDirection3.dir_x = 0.0f;
// 			ptheHitParameter->theDirection3.dir_y = 0.0f;
// 			ptheHitParameter->theDirection4.dir_x = 0.0f;
// 			ptheHitParameter->theDirection4.dir_y = 0.0f;
// 			ptheHitParameter->theDirection5.dir_x = 0.0f;
// 			ptheHitParameter->theDirection5.dir_y = 0.0f;
// 			ptheHitParameter->theDirection6.dir_x = 0.0f;
// 			ptheHitParameter->theDirection6.dir_y = 0.0f;

// 			ptheHitParameter->dwUnKnow1 = 0;
// 			ptheHitParameter->dwUnKnow2 = 0;
// 			ptheHitParameter->dwUnKnow3 = 0;


// 			sprintf_s(szPrint,"[LOG] 修改后攻击部位浮点 %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f 0x%p 0x%p 0x%p %f %f %f %f %f 0x%p 0x%p 0x%p %f",
// 				ptheHitParameter->theVector1.fx,
// 				ptheHitParameter->theVector1.fy,
// 				ptheHitParameter->theVector1.fz,
// 				ptheHitParameter->theVector2.fx,
// 				ptheHitParameter->theVector2.fy,
// 				ptheHitParameter->theVector2.fz,
// 				ptheHitParameter->theDirection1.dir_x,
// 				ptheHitParameter->theDirection1.dir_y,
// 				ptheHitParameter->theDirection2.dir_x,
// 				ptheHitParameter->theDirection2.dir_y,
// 				ptheHitParameter->theDirection3.dir_x,
// 				ptheHitParameter->theDirection3.dir_y,
// 				ptheHitParameter->theDirection4.dir_x,
// 				ptheHitParameter->theDirection4.dir_y,
// 				ptheHitParameter->theDirection5.dir_x,
// 				ptheHitParameter->theDirection5.dir_y,
// 				ptheHitParameter->theDirection6.dir_x,
// 				ptheHitParameter->theDirection6.dir_y,
// 				ptheHitParameter->dwUnKnow1,
// 				ptheHitParameter->dwUnKnow2,
// 				ptheHitParameter->dwUnKnow3,
// 				ptheHitParameter->theVector3.fx,
// 				ptheHitParameter->theVector3.fy,
// 				ptheHitParameter->theVector3.fz,
// 				ptheHitParameter->theDirection7.dir_x,
// 				ptheHitParameter->theDirection7.dir_y,
// 				ptheHitParameter->dwUnKnow4,
// 				ptheHitParameter->dwUnKnow5,
// 				ptheHitParameter->dwUnKnow6,
// 				ptheHitParameter->fUnKnow1
// 				);
			sprintf_s(szPrint,"[LOG] 修改后攻击部位浮点 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p 0x%p",
				ptheHitParameter->dwUnKnow94,
				ptheHitParameter->dwUnKnow98,
				ptheHitParameter->dwUnKnow9C,
				ptheHitParameter->dwUnKnowA0,
				ptheHitParameter->dwUnKnowA4,
				ptheHitParameter->dwUnKnowA8,
				ptheHitParameter->dwUnKnowAC,
				ptheHitParameter->dwUnKnowB8
				);
			OutputDebugStringA(szPrint);

			

		}
		--*/
	}
	catch (...)
	{
		assert(false);
	}
}

DWORD g_dwHookHit3Jmp = 0;
_declspec(naked) void HookHitParameter()
{
	__asm
	{
		pushfd
		pushad

		mov eax, dword ptr[esp+0x24+4]
		push eax
		call SendHitParameter
		add esp, 4


		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH EBX
		PUSH ESI

		jmp dword ptr[g_dwHookHit3Jmp]
	}
}


void MMO_HookHitParameter()
{
	g_dwHookHit3Jmp = MMOCLIENT_REVERSAL_HIT_PARAMETER_CALL_HOOK+GetCryGameBaseAddr()+5;
	Hook2("MMO_HookHitParameter", MMOCLIENT_REVERSAL_HIT_PARAMETER_CALL_HOOK+GetCryGameBaseAddr(),HookHitParameter);
}



// 获取对象部位，及对象的数据
void PrintHitBuWei(DWORD dwBuWeiObjAddr)
{
	try
	{
		// 取对象addr2,此处的addr2和遍历出来的内存值相同，但是地址不同
		DWORD dwActorAddr2 = *(DWORD*)(dwBuWeiObjAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_ACOTR_ADDR2_OFFSET___);
		if( dwActorAddr2 == NULL )
		{
			return ;
		}

		DWORD dwPacketId = *(DWORD*)(dwActorAddr2+MMOCLIENT_REVERSAL_HIT_BU_WEI_MONSTER_PACKET_ID_OFFSET___);

		try
		{
			int nNpcType = -1;
			char* pszActorName = NULL;

			DWORD dwActorAddr = dwActorAddr2;
			if( dwActorAddr != NULL )
			{
				// 首先过滤掉玩家被攻击的流程
				WORD wClass = *(WORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___);
				if( wClass != MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___ )
				{
					//OutputDebugStringA("[LOG] 其他对象被攻击，不做处理!");
					return ;
				}
			}
			
			if( dwActorAddr != NULL )
				dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME1___);
			/*if( dwActorAddr != NULL )
				dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME2___);
			if( dwActorAddr != NULL )
				dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME3___);*/
			if( dwActorAddr != NULL )
				dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME4);
			if( dwActorAddr != NULL )
				nNpcType = *(int*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NPC_TYPE___);					// 对象类型
			if( dwActorAddr != NULL )
				pszActorName = (char*)*(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME5___);		// 对象名字


			// 如果被攻击对象不是BOOS，则不处理		这里在测试教练练习副本内的草食龙，而他的类型是小怪类型，在进入副本内，需要改为BOSS
			if( nNpcType != MMOCLIENT_REVERSAL_NPC_TYPE_BOSS  /*MMOCLIENT_REVERSAL_NPC_TYPE_XIAO_GUAI*/ )
			{
				//OutputDebugStringA("[LOG] 对象不为BOSS，不处理攻击部位!");
				return ;
			}

			char szPrint[256] = {0};
			sprintf_s(szPrint,"[LOG] remote 被攻击对象名称 = %s 类型 = 0x%p ecx = 0x%p addr = 0x%p packet id = 0x%p",pszActorName,nNpcType,dwBuWeiObjAddr,*(DWORD*)(dwBuWeiObjAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_STRUCT_ADDR_OFFSET___),dwPacketId);
			OutputDebugStringA(szPrint);
		}
		catch (...)
		{
			assert(false);
			return ;		// 如果发生异常，则不处理
		}

		PMMO_BU_WEI_OBJ_STRUCT ptheBuWeiObjStruct = (PMMO_BU_WEI_OBJ_STRUCT)*(DWORD*)(dwBuWeiObjAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_STRUCT_ADDR_OFFSET___);
		int	  nBuWeiCount = *(int*)(dwBuWeiObjAddr+MMOCLIENT_REVERSAL_HIT_BU_WEI_STRUCT_COUNT_OFFSET___);

		if( nBuWeiCount == 3 )
		{
			OutputDebugStringA("[LOG] remote 怪物整体的部位，过滤掉不用管!");
			return ;
		}

		char szPrint[256] = {0};
		for (int i=0; i<nBuWeiCount; i++)
		{
			sprintf_s(szPrint,"[LOG] remote 部位 id = 0x%p 右边: %f %f %f ==> 左边: %f %f %f",ptheBuWeiObjStruct->dwBuWeiId,
									ptheBuWeiObjStruct->theRangeRight.fx,
									ptheBuWeiObjStruct->theRangeRight.fy,
									ptheBuWeiObjStruct->theRangeRight.fz,
									ptheBuWeiObjStruct->theRangeLeft.fx,
									ptheBuWeiObjStruct->theRangeLeft.fy,
									ptheBuWeiObjStruct->theRangeLeft.fz);
			OutputDebugStringA(szPrint);

			// 选择攻击部位
			if( ptheBuWeiObjStruct->dwBuWeiId == g_dwHitBuWeiId )
			{
				// 取攻击部位数据
				g_ptheHitBuWeiObj = ptheBuWeiObjStruct;

				// 取对象的坐标
				g_ptheActorVector = (MMO_ActorVector*)(dwActorAddr2+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);
			}
			ptheBuWeiObjStruct++;
		}
	}
	catch (...)
	{
		assert(false);
	}
}

_declspec(naked) void HookHitBuWei()
{
	__asm
	{
		pushfd
		pushad

		push ecx
		call PrintHitBuWei
		add esp, 4

		popad
		popfd
		
		MOV EDX,DWORD PTR DS:[ECX+0xC4]
		mov ecx, MMOCLIENT_REVERSAL_HIT_BU_WEI_HOOK
		add ecx, 6
		jmp ecx

	}
}





void MMO_HookHitBuWei()
{
	Hook2("MMO_HookHitBuWei", MMOCLIENT_REVERSAL_HIT_BU_WEI_HOOK,HookHitBuWei);
}


//////////////////////////////////////////////////////////////////////////
// 无敌

std::vector<DWORD> GetTechnologyTestLocalId()
{
	std::vector<DWORD> RetTmp;
	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET1);
		PGAME_STRUCT_ARRAY_POINT ptheDoorObjAddr = (PGAME_STRUCT_ARRAY_POINT)(dwGameAddr+MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET2);

		for (DWORD dwGameDoorAddr=ptheDoorObjAddr->str_dwBeginAddr; dwGameDoorAddr<ptheDoorObjAddr->str_dwEndAddr1; dwGameDoorAddr+=4)
		{
			DWORD dwTempGameDoorAddr = *(DWORD*)dwGameDoorAddr;

			if( dwTempGameDoorAddr != 0 )
			{
				WORD wClass = *(WORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___);

				if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___ )
				{
					char* pszObjName = (char*)*(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFFSET_NAME___);

					if( pszObjName != NULL )
					{
						if(strstr(pszObjName,"TechnologyTest") != nullptr ||strstr(pszObjName,"木马怪") != nullptr)
						{
							DWORD dwLocalId = *(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_LOCAL_ID_OFSET___);

							RetTmp.push_back(dwLocalId);

// 							char szPrint[256];
// 							sprintf_s(szPrint,"[LOG] 0x%p",dwLocalId);
// 							OutputDebugStringA(szPrint);

							
						}
					}
				}
			}
		}
		return RetTmp;
	}
	catch (...)
	{

	}

	return RetTmp;
}

DWORD g_dwAttacCount=0;
// 返回角色是否被怪物攻击
int MonsterHit2(DWORD dwActorEcxAddr)
{
	//不开启无敌
	if ( !g_enable_wu_di_ )
		return 0;
	std::vector<DWORD> dwTechnologyTestLocalId;
	try
	{
		dwTechnologyTestLocalId = GetTechnologyTestLocalId();
		if( dwTechnologyTestLocalId.size() == 0 )
		{
			// 如果没有遍历到木马怪的id，则程序异常，统统放行
			//assert(false);
			return 0;
		}

		DWORD dwGongJiActorId = *(DWORD*)(dwActorEcxAddr);


		for (int i=0;i< (int)dwTechnologyTestLocalId.size();i++)
		{
			if( dwGongJiActorId == dwTechnologyTestLocalId[i] )
			{
				//OutputDebugStringA("[LOG] 检测攻击流程对象 ==》 木马怪执行攻击任务!!!");
				return 0;
			}
		}



		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( dwGameAddr2 == 0 )
			return 0;
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( dwGameAddr2 == 0 )
			return 0;
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( dwGameAddr2 == 0 )
			return 0;
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( dwGameAddr2 == 0 )
			return 0;
		DWORD dwActorId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);

		DWORD dwBeiGongJiActorId = *(DWORD*)(dwActorEcxAddr+0x04);

		if( dwActorId == dwBeiGongJiActorId )
		{
// 			if (*(DWORD*)(dwActorEcxAddr+0x1bc)!=1)
// 			{
// 				char szprint[30];
// 				sprintf_s(szprint,"[LOG] 过滤的ID=%p",*(DWORD*)(dwActorEcxAddr+0x1bc));
// 				OutputDebugStringA(szprint);
// 				return 0;
// 			}
// 			g_dwAttacCount++;
// 			if (g_dwAttacCount>=20)
// 			{
// 				g_dwAttacCount=0;
// 				return 0;
// 			}


			//OutputDebugStringA("[LOG] 玩家正在被攻击!!!!");
				// 检测到玩家被怪物攻击
			return 1;
		}
		else
		{
			//OutputDebugStringA("[LOG] 攻击怪物!!!!");
		}

	}
	catch (...)
	{
		
	}

	return 0;
}

DWORD g_dwMonsterHitAddrHook = 0;
DWORD g_dwMonsterHitAddrCall = 0;
_declspec(naked) void HookMonsterHit2()
{
	__asm
	{
		pushfd
		pushad

		push eax
		call MonsterHit2
		add esp, 4

		cmp eax, 1
		je BOOS_EXIT		// 返回值 = 1 则跳走，不执行该函数

		popad
		popfd

		mov eax, g_dwMonsterHitAddrHook
		add eax, 5
		push eax
		mov eax, g_dwMonsterHitAddrCall
		jmp eax

BOOS_EXIT:
		popad
		popfd
		
		add esp, 8
		mov eax, g_dwMonsterHitAddrHook
		add eax, 5
		jmp eax
	}
}

byte g_bMonsterHitCode[20];
bool g_bMonsterHitFlag = false;

void MMO_HookMonsterHit()
{
	g_dwMonsterHitAddrHook = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_HOOK;
	ReadProcessMemory((HANDLE)-1,(LPCVOID)g_dwMonsterHitAddrHook,g_bMonsterHitCode,6,NULL);

	g_dwMonsterHitAddrCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_CALL;
	Hook2("MMO_HookMonsterHit", g_dwMonsterHitAddrHook,HookMonsterHit2);
	g_bMonsterHitFlag = true;
}

void MMO_UnHookMonsterHit()
{
	if( g_bMonsterHitFlag == true )
	{
		WriteProcessMemory((HANDLE)-1,(LPVOID)(GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_HOOK),g_bMonsterHitCode,6,NULL);
		g_bMonsterHitFlag = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 远程无敌

int MonsterRemoteHit(DWORD dwActorEcxAddr)
{
	//不开启无敌
	if ( !g_enable_wu_di_ )
		return 0;
	std::vector<DWORD> TechnologyTestLocalIdVectorTemp;

	try
	{

			TechnologyTestLocalIdVectorTemp = GetTechnologyTestLocalId();
		if( TechnologyTestLocalIdVectorTemp.size() == 0 )
		{
			// 如果没有遍历到木马怪的id，则程序异常，统统放行
			//assert(false);
			return 0;
		}
		DWORD dwGongJiActorId = *(DWORD*)(dwActorEcxAddr);
		for (int i=0;i< (int)TechnologyTestLocalIdVectorTemp.size();i++)
		{
			if( dwGongJiActorId == TechnologyTestLocalIdVectorTemp[i] )
			{
				//OutputDebugStringA("[LOG] 检测攻击流程对象 ==》 木马怪执行攻击任务!!!");
				return 0;
			}
		}

		



		DWORD dwHitObjAddr = *(DWORD*)(dwActorEcxAddr+MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_OBJ_OFFSET___);
		if( dwHitObjAddr != 0 )
		{
			// 取被攻击者对象的id
			DWORD dwLocalId = *(DWORD*)(dwHitObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_LOCAL_ID_OFSET___);

			// 取角色id
			DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
			if( dwGameAddr2 == 0 )
				return 0;
			dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
			if( dwGameAddr2 == 0 )
				return 0;
			dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
			if( dwGameAddr2 == 0 )
				return 0;
			dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
			if( dwGameAddr2 == 0 )
				return 0;
			DWORD dwActorId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);

// 			char szPrint[256];
// 			sprintf_s(szPrint,"[LOG] remote 被远程攻击对象id = 0x%p 玩家id = 0x%p",dwLocalId,dwActorId);
// 			OutputDebugStringA(szPrint);

			// 判断对比
			if( dwActorId == dwLocalId )
			{
				__asm{
					    mov byte ptr ds:[ecx+0x54],1
				}
				//OutputDebugStringA("[LOG] 玩家正在被远程攻击攻击!!!!");
				// 检测到玩家被怪物攻击
					return 1;
				// 如果是玩家则返回1，不放行，产生无敌
			}

		}
	}
	catch (...)
	{
		
	}
	
	return 0;
}

DWORD GetMonsterRemoteHitAddrHook()
{
	return MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK+GetCryGameBaseAddr();
}

DWORD g_dwMonsterRemoteHitAddrHookJmp = 0;
_declspec(naked) void HookMonsterRemoteHit()
{
	__asm
	{
		pushfd
		pushad
		
		//push edi
		mov eax, dword ptr[esp+0x24+4]
		push eax
		call MonsterRemoteHit
		add esp, 4

// 		cmp eax, 1
// 		je PLAYER_HIT		// 返回值 = 1 则跳走，不执行该函数

		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		FLDZ

		jmp dword ptr[g_dwMonsterRemoteHitAddrHookJmp]

// PLAYER_HIT:
// 		popad
// 		popfd
// 
// 		retn 0x04
	}
}

void MMO_HookMonsterRemoteHit()
{
	g_dwMonsterRemoteHitAddrHookJmp = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK+5;
	Hook2("MMO_HookMonsterRemoteHit", MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK+GetCryGameBaseAddr(),HookMonsterRemoteHit);
}

void MMO_UnHookMonsterRemoteHit()
{
	byte bCode[] = {0x55 ,0x8B ,0xEC ,0xD9 ,0xEE};
	WriteProcessMemory((HANDLE)-1,(LPVOID)(MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK+GetCryGameBaseAddr()),bCode,5,NULL);
}

// 空中使用技能
void MMO_KongZhongUseSkillJmp()
{
	byte bCode[] = {0xEB};
	auto the_addr = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_KONG_ZHONG_USE_SKILL_JMP_ADDR;
	if ( !CmnX_AntiHookCheck::GetRealMe().AddHookCodeInfo("MMO_KongZhongUseSkillJmp", (const void*)the_addr, 1) )
	{
		assert(false);
		return;
	}

	WriteProcessMemory((HANDLE)-1,(LPVOID)(the_addr),bCode,1,NULL);
}

// 解密CALL
int GetDecryValue( DWORD dwAddr, int def_ret )
{
	int nRetValue = def_ret;
	try
	{
		DWORD dwDecryCall = MMOCLIENT_REVERSAL_DECRY_CALL+GetCryGameBaseAddr();
		__asm
		{
			mov ecx, dwAddr
			mov eax, dwDecryCall
			call eax
			mov nRetValue, eax
		}
	}
	catch (...)
	{
		return nRetValue;
	}

	return nRetValue;
}

// 加密int值
void EncryptionValue_int(DWORD dwValueAddr,int nValue)
{
	try
	{
		DWORD dwEncryPotionCall = MMOCLIENT_REVERSAL_ENCRYPTION_CALL+GetCryGameBaseAddr();
		__asm
		{
			lea eax, nValue
			push eax
			mov ecx, dwValueAddr
			mov eax,dwEncryPotionCall
			call eax
		}
	}
	catch (...)
	{
		assert(false);	
	}
}



// 获取物品出售价钱
int GetSellItemPrice(DWORD dwItemMuBanAddr)
{
	int nRet = 0;
	try
	{
		DWORD dwGetSellItemPriceCall = MMOCLIENT_REVERSAL_GET_SELL_ITEM_PRICE_CALL+GetCryGameBaseAddr();
		__asm
		{
			push 1
			push dwItemMuBanAddr
			mov eax, dwGetSellItemPriceCall
			call eax
			mov nRet, eax
			add esp, 8
		}
	}
	catch (...)
	{
		assert(false);
	}

	return nRet;
}


/*
DWORD GetReqItemAddr(DWORD dwRecvReqItemByAddr)
{

	DWORD dwItemAddr = 0;
	try
	{
		int nIndex = 0;
		__asm
		{
			mov ebx, dwRecvReqItemByAddr
			MOVZX EDX,WORD PTR DS:[EBX+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET1]
			LEA ECX,DWORD PTR DS:[EBX+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET2]
			PUSH EDX
			lea edi, dword ptr[nIndex]
			mov dword ptr[nIndex], 0
			mov eax, MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_CALL
			call eax
			add esp, 4
			mov dwItemAddr, eax
		}
	}
	catch (...)
	{

	}

	return dwItemAddr;
}
*/
DWORD GetReqItemAddr(DWORD dwRecvReqItemByAddr)
{

	DWORD dwItemAddr = 0;
	try
	{

		DWORD dwReqItemByGetItemIdCall = MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_CALL+GetCryGameBaseAddr();

		int nIndex = 0;
		__asm
		{
			lea eax, dword ptr[nIndex]
			mov dword ptr[nIndex], 0
			push eax

			mov edi, dwRecvReqItemByAddr
			MOVZX EDX,WORD PTR DS:[edi+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET1]
			LEA ECX,DWORD PTR DS:[edi+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET2]
			push ecx
			PUSH EDX

			mov eax, dwReqItemByGetItemIdCall
			call eax
			add esp, 0x0C
			mov dwItemAddr, eax
		}
	}
	catch (...)
	{

	}

	return dwItemAddr;
}



_declspec(naked)void GetHitSendDir_(DWORD dwDirAddr,float* fdirx,float* fdiry)
{
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		AND ESP,0xFFFFFFF0
		SUB ESP,0x74
		PUSH EBX
		MOV EBX,DWORD PTR SS:[dwDirAddr]
		PUSH ESI
		PUSH EDI

		MOV ESI,DWORD PTR SS:[EBP+0x8]
		MOVSS XMM7,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		MOVSS XMM6,DWORD PTR DS:[ESI+0x28]
		MOVSS XMM5,DWORD PTR DS:[ESI+0x14]
		MOVSS XMM4,DWORD PTR DS:[ESI]
		MOVSS XMM3,DWORD PTR DS:[ESI+0x8]
		MOVSS XMM0,DWORD PTR DS:[ESI+0x10]
		MOVSS XMM2,DWORD PTR DS:[ESI+0x20]
		MOVSS XMM1,DWORD PTR DS:[ESI+0x24]
		MOVSS DWORD PTR SS:[ESP+0x6C],XMM6
		MOVSS DWORD PTR SS:[ESP+0x1C],XMM7
		MOVSS DWORD PTR SS:[ESP+0x20],XMM7
		MOVSS DWORD PTR SS:[ESP+0x24],XMM7
		XORPS XMM7,XMM7
		ADDSS XMM6,XMM5
		ADDSS XMM6,XMM4
		COMISS XMM6,XMM7
		//JBE SHORT 00951627
		JBE GO_TO_00951627

		ADDSS XMM6,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		XORPS XMM4,XMM4
		MOVSS XMM4,XMM6
		SQRTSS XMM4,XMM4
		MOVAPS XMM5,XMM4
		MOVSS XMM4,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]
		DIVSS XMM4,XMM5
		MOVSS DWORD PTR SS:[ESP+0x18],XMM5
		MOVSS XMM5,DWORD PTR DS:[ESI+0x18]
		SUBSS XMM1,XMM5
		MULSS XMM1,XMM4
		MOVAPS XMM6,XMM1
		MOVSS XMM1,DWORD PTR DS:[ESI+0x4]
		SUBSS XMM0,XMM1
		MULSS XMM0,XMM4
		MOVAPS XMM5,XMM0
		MOVSS XMM0,DWORD PTR SS:[ESP+0x18]
		MULSS XMM0,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]

		SUBSS XMM3,XMM2
		MULSS XMM3,XMM4
		MOVAPS XMM7,XMM3
		//JMP 00951777
		JMP GO_TO_00951777
GO_TO_00951627:
		

		COMISS XMM4,XMM5
		MOVSS XMM6,DWORD PTR SS:[ESP+0x6C]
		//JB SHORT MHOClien.00951698
		JB GO_TO_00951698
		COMISS XMM4,XMM6
		//JB SHORT MHOClien.00951698
		JB GO_TO_00951698
		SUBSS XMM4,XMM5
		SUBSS XMM4,XMM6
		ADDSS XMM4,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		MOVSS XMM6,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]
		XORPS XMM5,XMM5
		MOVSS XMM5,XMM4
		SQRTSS XMM5,XMM5
		MOVAPS XMM4,XMM6
		DIVSS XMM4,XMM5
		MULSS XMM5,XMM6
		MOVAPS XMM6,XMM5
		MOVSS XMM5,DWORD PTR DS:[ESI+0x4]
		ADDSS XMM0,XMM5
		MULSS XMM0,XMM4
		MOVAPS XMM7,XMM0
		MOVSS XMM0,DWORD PTR DS:[ESI+0x18]
		SUBSS XMM1,XMM0
		ADDSS XMM2,XMM3
		MULSS XMM2,XMM4
		MULSS XMM1,XMM4
		MOVAPS XMM5,XMM2
		MOVAPS XMM0,XMM1
		//JMP MHOClien.00951777
		JMP GO_TO_00951777
GO_TO_00951698:
		COMISS XMM5,XMM4
		//JB SHORT MHOClien.00951703
		JB GO_TO_00951703
		COMISS XMM5,XMM6
		//JB SHORT MHOClien.00951703
		JB GO_TO_00951703
		MOVSS XMM7,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]
		SUBSS XMM5,XMM6
		MOVSS XMM6,DWORD PTR DS:[ESI+0x4]
		SUBSS XMM5,XMM4
		ADDSS XMM5,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		XORPS XMM4,XMM4
		MOVSS XMM4,XMM5
		SQRTSS XMM4,XMM4
		MOVAPS XMM5,XMM4
		ADDSS XMM0,XMM6
		MOVAPS XMM4,XMM7
		DIVSS XMM4,XMM5
		MULSS XMM0,XMM4
		MOVAPS XMM6,XMM0
		MOVSS XMM0,DWORD PTR DS:[ESI+0x18]
		MULSS XMM5,XMM7
		ADDSS XMM1,XMM0
		SUBSS XMM3,XMM2
		MOVAPS XMM7,XMM5
		MULSS XMM1,XMM4
		MULSS XMM3,XMM4
		MOVAPS XMM5,XMM1
		MOVAPS XMM0,XMM3
		//JMP SHORT MHOClien.00951777
		JMP GO_TO_00951777
GO_TO_00951703:
		COMISS XMM6,XMM4
		//JB SHORT MHOClien.00951769
		JB GO_TO_00951769
		COMISS XMM6,XMM5
		//JB SHORT MHOClien.00951769
		JB GO_TO_00951769
		SUBSS XMM6,XMM4
		SUBSS XMM6,XMM5
		ADDSS XMM6,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		XORPS XMM4,XMM4
		MOVSS XMM4,XMM6
		SQRTSS XMM4,XMM4
		MOVAPS XMM5,XMM4
		MOVSS XMM4,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]
		DIVSS XMM4,XMM5
		MULSS XMM5,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2]
		ADDSS XMM2,XMM3
		MULSS XMM2,XMM4
		MOVAPS XMM6,XMM2
		MOVSS XMM2,DWORD PTR DS:[ESI+0x18]
		ADDSS XMM1,XMM2
		MULSS XMM1,XMM4
		MOVAPS XMM7,XMM1
		MOVSS XMM1,DWORD PTR DS:[ESI+0x4]
		SUBSS XMM0,XMM1
		MULSS XMM0,XMM4
		//JMP SHORT MHOClien.00951777
		JMP GO_TO_00951777
GO_TO_00951769:
		MOVSS XMM0,DWORD PTR DS:[MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1]
		MOVAPS XMM6,XMM7
		MOVAPS XMM5,XMM7
GO_TO_00951777:
		//取 XMMO=y和XMM5=x
		mov eax,dword ptr [fdirx] 
		MOVSS DWORD PTR SS:[eax],XMM5

		mov eax,dword ptr [fdiry] 
		MOVSS DWORD PTR SS:[eax],XMM0

		POP EDI
		POP ESI
		POP EBX
		MOV ESP,EBP
		POP EBP
		retn 
	}
}

DWORD GetManufactureInfoAddr(DWORD dwManufactureId)
{
	DWORD dwManufactureInfoAddr = 0;
	try
	{
		char* pszManufavtureInfoString = "CManufactureInfo";
		DWORD dwGetManuFactureInfoAddrCall = MMOCLIENT_REVERSAL_GET_MANU_FACTURE_INFO_ADDR_CALL+GetCryGameBaseAddr();
		DWORD dwGetMuBanInfoAddrCall = MMOCLIENT_REVERSAL_GET_MU_BAN_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			push 0
			push pszManufavtureInfoString
			push 0
			mov eax, dwGetManuFactureInfoAddrCall
			call eax
			add esp, 0x0C
			mov ecx, eax
			push dwManufactureId
			mov eax, dwGetMuBanInfoAddrCall
			call eax
			mov dwManufactureInfoAddr, eax
		}
	}
	catch (...)
	{	
		assert(false);
	}
	return dwManufactureInfoAddr;
}

DWORD GetFarmFacilityInfoAddr(DWORD dwFarmFacilityId)
{
	DWORD dwFarmFacilityaddr = 0;

	try
	{
		DWORD dwFarmFacilityInfoCall = MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwFarmFacilityId
			mov eax, dwFarmFacilityInfoCall
			call eax
			add esp, 4
			mov dwFarmFacilityaddr, eax
		}
	}
	catch (...)
	{

	}

	return dwFarmFacilityaddr;
}

DWORD GetFarmFacilityId(int nCollectIndex,int nCollectLevel)
{
	DWORD dwFarmFacilityId = 0;
	try
	{
		DWORD dwGetFarmFacilityIdCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_GET_FARM_FACILITY_ID_CALL;
		__asm
		{
			push nCollectLevel
			push nCollectIndex
			push 0	// 庄园的id
			mov eax, dwGetFarmFacilityIdCall
			call eax
			add esp, 0x0C
			mov dwFarmFacilityId, eax
		}
	}
	catch (...)
	{

	}

	return dwFarmFacilityId;
}
DWORD GetFarmCpInfoAddr(DWORD dwFarmFacilityId)
{
	DWORD dwFarmCpInfoAddr = 0;
	try
	{
		DWORD dwFarmCpInfoCall = MMOCLIENT_REVERSAL_FARM_CP_INFO_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwFarmFacilityId
			mov eax, dwFarmCpInfoCall
			call eax
			add esp, 4
			mov dwFarmCpInfoAddr, eax
		}
	}
	catch (...)
	{

	}
	return dwFarmCpInfoAddr;
}
// 获取植物剩余时间
int GetFarmCpTime(DWORD dwFarmFacilityId,int nPlantCpTime)
{
	int nCpTime = 0; 
	try
	{
		if( nPlantCpTime <= 0 )
		{
			return 0;
		}

		// 获取田地模板数据
		DWORD dwFarmCpInfoAddr = GetFarmCpInfoAddr(dwFarmFacilityId);

		if( dwFarmCpInfoAddr != 0 )
		{
			/*++
			$ ==>    0C938ED6      E8 A19DDF00     CALL <JMP.&MSVCR90._time64>
			$+5      0C938EDB      8B5424 10       MOV EDX,DWORD PTR SS:[ESP+0x10]
			$+9      0C938EDF      8B4A 1C         MOV ECX,DWORD PTR DS:[EDX+0x1C]
			$+C      0C938EE2      8BD1            MOV EDX,ECX
			$+E      0C938EE4      C1E2 04         SHL EDX,0x4
			$+11     0C938EE7      2BD1            SUB EDX,ECX
			$+13     0C938EE9      8B4C24 14       MOV ECX,DWORD PTR SS:[ESP+0x14]
			$+17     0C938EED      8B49 04         MOV ECX,DWORD PTR DS:[ECX+0x4]
			$+1A     0C938EF0      8D0C91          LEA ECX,DWORD PTR DS:[ECX+EDX*4]
			$+1D     0C938EF3      2BC8            SUB ECX,EAX
			--*/


			// 获取采集点植物成熟周期时间
			int nCpTime1 = *(int*)(dwFarmCpInfoAddr+MMOCLIENT_REVERSAL_CUR_FARM_CP_TIME_OFFSET1___);

			nCpTime = nCpTime1<<4;
			nCpTime -= nCpTime1;
			nCpTime *= 4;
			nCpTime += nPlantCpTime;
			nCpTime -= (DWORD)_time64(0);
		}
	}
	catch (...)
	{

	}

	return nCpTime;
}

// 获取种子信息

DWORD GetFarmSeedAddr(DWORD dwFarmSeedId)
{
	DWORD dwFarmSeedddr = 0;

	try
	{
		DWORD dwFramSeedCall = MMOCLIENT_REVERSAL_FARM_SEED_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwFarmSeedId
			mov eax, dwFramSeedCall
			call eax
			add esp, 4
			mov dwFarmSeedddr, eax
		}
	}
	catch (...)
	{

	}

	return dwFarmSeedddr;
}

// 计算草药田的剩余时间
int GetCollectTime(DWORD dwFarmAddr, int nFamIndex)
{
	int nCollectTime = 0;
	try
	{
		DWORD dwFarmFacilityStateTimeCall = MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_TIME_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwFarmAddr
			push nFamIndex
			mov eax, dwFarmFacilityStateTimeCall
			call eax
			add esp, 8
			mov nCollectTime, eax
		}
	}
	catch (...)
	{

	}

	return nCollectTime;
}

MMO_Direction2 GetHitSendDir(MMO_ActorVector& dwDirAddr)
{
	float fdirx = 0.0f;
	float fdiry = 0.0f;

	MMO_Direction2 theDirection = {0};
	try
	{
		__asm{

			lea eax, fdiry
			push eax
			lea eax, fdirx
			push eax
			push dwDirAddr
			call GetHitSendDir_
			add esp, 0x0C
		}
		theDirection.dir_x = fdirx;
		theDirection.dir_y = fdiry;

	}
	catch (...)
	{
		assert(false);
	}
	return theDirection;
}

DWORD GetCryActionBaseAddr()
{
	static DWORD dwCryActionBaseAddr = (DWORD)GetModuleHandleA("CryAction.dll");
	assert(dwCryActionBaseAddr);
	return dwCryActionBaseAddr;
}

DWORD GetCryGameBaseAddr()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("CryGame.dll");
	assert(dwCryGameBaseAddr);
	return dwCryGameBaseAddr;
}


// cpu 优化
DWORD g_dwSleepTime = 10;

void SleepRender(DWORD dwSleepTime)
{
	Sleep(g_dwSleepTime);
}

_declspec(naked) void HookRender()
{
	__asm
	{
		push g_dwSleepTime
		lea eax, SleepRender
		call eax
		add esp, 4

		POP ESI
		MOV ESP,EBP
		POP EBP
		RETN
	}
}

void MMO_Render(DWORD dwSleepTime)
{
	g_dwSleepTime = dwSleepTime;
	Hook2("MMO_Render", MMOCLIENT_REVERSAL_RENDER_ADDR_HOOK,HookRender);
}


// 添加buff

typedef void (*PFUN_PRINT_)(DWORD lpSubTreeRoot);

void Traverse_( DWORD lpSubTreeRoot, DWORD dwNullNode, PFUN_PRINT_ pFunPrint)
{
	if ( lpSubTreeRoot != NULL )
	{
		NODE	theNode = {0};
		ReadProcessMemory((HANDLE)-1, (LPCVOID)lpSubTreeRoot,&theNode,sizeof(theNode),NULL);

		if( lpSubTreeRoot != dwNullNode )
		{
			pFunPrint(lpSubTreeRoot);
		}

		if( lpSubTreeRoot != dwNullNode )
		{
			Traverse_( theNode.str_dwNextAddrLeft,dwNullNode,pFunPrint );
			Traverse_( theNode.str_dwNextAddrRight,dwNullNode,pFunPrint );
		}
	}
}

DWORD g_dwCheckBuffId = 0;
bool  g_bCheckBuffIdFlag = false;

void PrintBuff_(DWORD lpSubTreeRoot)
{
	try
	{
		DWORD dwBuffAddr = *(DWORD*)(lpSubTreeRoot+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET3___);
		if( IsUnValidPtr(dwBuffAddr) )
		{
			assert(false);
			return ;
		}
		DWORD dwActorPacketId = *(DWORD*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_ACTOR_PACKET_ID_OFFSET___);
		int	  nBuffFlag = *(int*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_FLAG_OFFSET);
		//if( nBuffFlag != -1 )
		{
			DWORD dwBuffInfoAddr = *(DWORD*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_INFO_ADDR_OFFSET);
			if( IsUnValidPtr(dwBuffInfoAddr) )
			{
				assert(false);
				return ;
			}

			if( dwBuffInfoAddr != 0 )
			{
				DWORD dwBuffInfoId = *(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_ID_OFFSET___);

				if( dwBuffInfoId == g_dwCheckBuffId )
				{
					g_bCheckBuffIdFlag = true;
				}
			}

		}
	}
	catch (...)
	{

	}
}

bool GetRoleBuffState(DWORD dwBuffId)
{
	// 初始化标志为0
	g_bCheckBuffIdFlag = false;
	g_dwCheckBuffId = dwBuffId;
	try
	{
		// dd [[[[[[0x01928EA0+0C370000]+0x000000D0]+0x0000001C]+0x214]+0x10]+0x18]
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_ADDR_OFFSET___);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return false;
		}

		DWORD dwBuffAddr1 = dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET2___;


		if( dwBuffAddr1 != 0 )
		{
			NODE	theNode = {0};
			ReadProcessMemory((HANDLE)-1,(LPCVOID)dwBuffAddr1,&theNode,sizeof(theNode),NULL);
			Traverse_(theNode.str_dwNextAddrParent,dwBuffAddr1,PrintBuff_);
		}


		DWORD dwBuffAddr2 = dwGameAddr2+MMOCLIENT_REVERSAL_TIAN_QI_BUFF_STATE_OFFSET___;


		if( dwBuffAddr2 != 0 )
		{
			NODE	theNode = {0};
			ReadProcessMemory((HANDLE)-1,(LPCVOID)dwBuffAddr2,&theNode,sizeof(theNode),NULL);
			Traverse_(theNode.str_dwNextAddrParent,dwBuffAddr2,PrintBuff_);
		}

	}
	catch (...)
	{

	}

	return g_bCheckBuffIdFlag;
}


typedef struct
{
	DWORD	dwUnKnow1;
	DWORD	dwBuffId;
	DWORD	dwRolePacketId;
	DWORD	dwUnKnow2;
	DWORD	dwUnKnow3;
	DWORD	dwUnKnow4;
}CALL_BUFF,*PCALL_BUFF;

bool MMO_AddBuffer(DWORD dwBufferId)
{
	if( GetRoleBuffState(dwBufferId) == true )
	{
		// 已存在奥义觉醒
		return true;
	}

	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}

		DWORD dwActorId = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);
		if( dwActorId == 0 )
		{
			assert(dwActorId);
			return false;
		}

		DWORD dwActorObjAddr = GetActorObjAddr(dwActorId);
		if( IsUnValidPtr(dwActorObjAddr) )
		{
			assert(dwActorObjAddr);
			return false;
		}

		DWORD dwActorAddr = *(DWORD*)(dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
		if( IsUnValidPtr(dwActorAddr) )
		{
			assert(dwActorAddr);
			return false;
		}

		DWORD dwActorPacketId = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___);
		if( dwActorPacketId == 0 )
		{
			assert(false);
			return false;
		}


		CALL_BUFF theBuffer;

		theBuffer.dwUnKnow1 = 0x000047F5;
		theBuffer.dwBuffId = dwBufferId;
		theBuffer.dwRolePacketId = dwActorPacketId;
		theBuffer.dwUnKnow2 = 1;
		theBuffer.dwUnKnow3 = 0x00000BB8;
		theBuffer.dwUnKnow4 = 0x41940000;

		DWORD dwAddBuffCall = GetCryGameBaseAddr()+0x120D1F0;


		DWORD dwGameAddr2 = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_ADDR_OFFSET___);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(dwGameAddr2);
			return false;
		}

		try
		{
			__asm
			{
				lea eax, theBuffer
				push eax
				mov ecx, dwGameAddr2
				mov eax, dwAddBuffCall
				call eax
			}
		}
		catch (...)
		{
			assert(false);
		}
	
	

		return true;
	}
	catch (...)
	{
		assert(false);
	}

	return false;

}

DWORD g_dwHookAddBufferAddr = 0;
DWORD g_dwJmpHookAddrBufferAddr = 0;
DWORD g_dwAddNewBuffId = 0;
void AddBuffer(DWORD dwBufferId)
{
	g_dwAddNewBuffId = dwBufferId;

	if( dwBufferId == 0xC352 )
	{	
		if( GetRoleBuffState(MMOCLIENT_REVERSAL_SKILL_BUFF_ID_F) == true )
		{
			// 已存在奥义觉醒
			return ;
		}

		g_dwAddNewBuffId = MMOCLIENT_REVERSAL_SKILL_BUFF_ID_F;
	}
}


_declspec(naked) void HookAddBuffer()
{
	__asm
	{
		MOV EAX,DWORD PTR DS:[ESI+0x4]
		PUSH EBX
		
		pushad
		pushfd

		PUSH EAX
		mov eax,AddBuffer
		call eax
		add esp, 4

		popfd
		popad

		mov eax, g_dwAddNewBuffId
		push eax

		jmp dword ptr[g_dwJmpHookAddrBufferAddr]
	}
}


void MMO_HookAddBuffer()
{
	//120D237
	static bool bInitFlag = false;

	if( bInitFlag == false )
	{
		g_dwHookAddBufferAddr = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_ADD_BUFFER_HOOK;
		g_dwJmpHookAddrBufferAddr = g_dwHookAddBufferAddr+5;
		Hook2("MMO_HookAddBuffer", g_dwHookAddBufferAddr,HookAddBuffer);

		bInitFlag = true;
	}
}


//////////////////////////////////////////////////////////////////////////
// hook 记录当前角色所在坐标的地图z坐标

float	g_fMapCoorDinates_Z = 0.0f;		// 角色所在地图点的z坐标
DWORD	g_dwJmpMapCoorDinates = 0;
DWORD	g_dwHookMapCoorIinates = 0;

float GetMapCoorDinates_Z()
{
	return g_fMapCoorDinates_Z;
}

void PrintMapCoorDinates()
{
	char szPrint[256];
	sprintf_s(szPrint,"[LOG] %f",g_fMapCoorDinates_Z);
	OutputDebugStringA(szPrint);
}

_declspec(naked) void HookMapCoorDinates()
{
	__asm
	{
		FLD DWORD PTR SS:[EBP-0x4]
		FLD DWORD PTR SS:[EBP+0x8]

		pushfd
		pushad

		FLD DWORD PTR SS:[EBP-0x54]
		FSTP DWORD PTR SS:[g_fMapCoorDinates_Z]

// 		mov eax, PrintMapCoorDinates
// 		call eax

		popad
		popfd

		jmp DWORD PTR [g_dwJmpMapCoorDinates]
	}
}

void MMO_HookMapCoorDinates()
{
	g_dwHookMapCoorIinates = MMOCLIENT_REVERSAL_MAP_COORDINATES_HOOK___+GetCryGameBaseAddr();
	g_dwJmpMapCoorDinates = g_dwHookMapCoorIinates+6;
	Hook2("MMO_HookMapCoorDinates", g_dwHookMapCoorIinates,HookMapCoorDinates);
}

DWORD GetMianBanTaskFbInfoBaseAddr(DWORD dwTaskIndex)
{
	DWORD dwTaskInfoAddr = 0;
	try
	{
		DWORD dwTaskInfoCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_ZHU_XIAN_TASK_FB_INFO_ADDR_CALL;

		__asm
		{
			push dwTaskIndex
				mov eax, dwTaskInfoCall
				call eax
				add esp, 4

				mov dwTaskInfoAddr, eax
		}
	}
	catch (...)
	{
		assert(false);
	}

	return dwTaskInfoAddr;
}


/*++
	函数功能： 判断主线任务是否完成，判断正常副本是否完成黄金以上评价，判断喵喵副本是否完成

	返回=1 表示主线完成
	返回=1 表示此副本黄金评价以上
	返回=1 表示喵喵副本已经完成，不可再进

--*/
bool IsInTaskFb(DWORD dwFbId)
{
	bool bRet = false;
	try
	{

		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}

		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_TASK_FB_OFFSET);
		if( IsUnValidPtr(dwGameAddr) )
		{
			assert(false);
			return false;
		}

		DWORD dwIsTaskInFbCall = MMOCLIENT_REVERSAL_IS_IN_TASK_FB_CALL+GetCryGameBaseAddr();

		__asm
		{
			MOV ECX, dwGameAddr
				push dwFbId
				mov eax, dwIsTaskInFbCall
				call eax
				mov bRet, al
		}
	}
	catch (...)
	{
		assert(false);
	}
	return bRet;
}

void GetShangJinTask(GAME_STRUCT_ARRAY_POINT& theShangJinTask)
{
	try
	{
		DWORD dwShangJinTaskAddr = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_BASE_ADDR;

		GAME_STRUCT_ARRAY_POINT theTempShangJinTask = {0};
		__asm
		{
			mov ecx, dword ptr[dwShangJinTaskAddr]
			MOV ECX,DWORD PTR DS:[ECX]
			MOV EDX,DWORD PTR DS:[ECX]
			MOV EDX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_CALL_OFFSET]

			lea eax, theTempShangJinTask
				PUSH EAX
				CALL EDX
		}

		theShangJinTask = theTempShangJinTask;
	}
	catch (...)
	{
		assert(false);
	}
}

int GetShangJinCompleteCount()
{
	int nCompleCount = 0;
	try
	{
		DWORD dwShangJinTaskAddr = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_BASE_ADDR;

		DWORD dwShangJinCountAddr = 0;

		__asm
		{
			mov ecx, dword ptr[dwShangJinTaskAddr]
			MOV ECX,DWORD PTR DS:[ECX]
			MOV EDX,DWORD PTR DS:[ECX]
			MOV EAX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_COUNT_CALL_OFFSET]
			CALL EAX
				mov dwShangJinCountAddr, eax
		}


		if( dwShangJinCountAddr != 0 )
		{
			nCompleCount = *(int*)(dwShangJinCountAddr+MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_COMPLE_OFFSET);
		}

	}
	catch (...)
	{
		assert(false);
	}

	return nCompleCount;
}


int GetShangJinTotalCount()
{
	int nTotalCount = 0;
	try
	{
		DWORD dwShangJinTaskAddr = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_BASE_ADDR;

		DWORD dwShangJinCountAddr = 0;

		__asm
		{
			mov ecx, dword ptr[dwShangJinTaskAddr]
			MOV ECX,DWORD PTR DS:[ECX]
			MOV EDX,DWORD PTR DS:[ECX]
			MOV EAX,DWORD PTR DS:[EDX+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_COUNT_CALL_OFFSET]
			CALL EAX
				mov dwShangJinCountAddr, eax
		}


		if( dwShangJinCountAddr != 0 )
		{
			DWORD dwTotalCountCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_TOTAL_CALL;
			__asm
			{
				mov ecx, dwShangJinCountAddr
					mov eax, dwTotalCountCall
					call eax
					mov nTotalCount, eax
			}
		}

	}
	catch (...)
	{
		assert(false);
	}

	return nTotalCount;
}

// 判断赏金任务的状态  状态=1 表示已接  状态=10 表示已经完成
int GetShangJinTaskStatus(DWORD dwShangJinTaskId)
{
	int nRetValue = 0;

	DWORD dwShangJinTaskStatusCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_STATUS_CALL;

	try
	{
		__asm
		{
			mov esi, dwShangJinTaskId
				mov eax, dwShangJinTaskStatusCall
				call eax

				mov nRetValue, eax
		}

	}
	catch (...)
	{
		nRetValue = 0;
	}

	return nRetValue;
}



// 函数功能：获取角色在城镇中的房间序号，副本内无效
int GetMsgRoleCityRoomIndex()
{
	int nRoleCityRoomIndex = -1;

	try
	{
		DWORD	dwBeginListAddr = MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_ADDR+GetCryGameBaseAddr();

		DWORD	dwNextListAddr = *(DWORD*)(dwBeginListAddr);

		if( dwBeginListAddr != dwNextListAddr )
		{
			do
			{
				DWORD dwObjMsgAddr = *(DWORD*)(dwNextListAddr+MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_OFFSET);

				if( dwObjMsgAddr != 0 )
				{
					char* pszObjMsgName = (char*)*(DWORD*)(dwObjMsgAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_MSG_NAME___);

					if( pszObjMsgName != NULL )
					{
						//sprintf_s(szPrint,"[LOG] Index = %d addr = 0x%p msg = %s\r\n",++i,dwObjMsgAddr,pszObjMsgName);

						if( strcmp(pszObjMsgName,"Minimap") == 0 )
						{
							nRoleCityRoomIndex = *(int*)(dwObjMsgAddr+MMOCLIENT_REVERSAL_MINIMAP_ROLE_ROOM_INDEX_OFFSET___);
							break;
						}
					}
				}

				dwNextListAddr = *(DWORD*)dwNextListAddr;

			}while( dwBeginListAddr != dwNextListAddr );
		}

	}
	catch (...)
	{
		assert(false);
	}

	return nRoleCityRoomIndex;
}

// 整理物品   普通仓库=2 素材仓库=0x0A 背包=0
void TidyInventoryItem(int nTabSoltIndex)
{
	DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
	if( dwGameAddr2 == 0 )
		return;

	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
	if( dwGameAddr2 == 0 )
		return;

	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
	if( dwGameAddr2 == 0 )
		return;

	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
	if( dwGameAddr2 == 0 )
		return;

	try
	{
		DWORD dwTidyInventoryItemSizeCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_SIZE_CALL;
		DWORD dwTidyInventoryItemCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_CALL;
		__asm
		{
			mov esi, dwGameAddr2
			MOV ECX,DWORD PTR DS:[ESI+MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_OFFSET]

			push nTabSoltIndex
			call DWORD PTR[dwTidyInventoryItemSizeCall]

			MOV ECX,DWORD PTR DS:[ESI+MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_OFFSET]

			DEC EAX
			push eax
			push 0
			push nTabSoltIndex
			call dword ptr[dwTidyInventoryItemCall]

		}
	}
	catch (...)
	{
		assert(false);
	}
}

DWORD GetKingQuestInfoAddr(int nKingQuestIndex )
{
	DWORD dwKingQuestInfoAddr = 0;
	try
	{
		DWORD dwKingQuestInfoAddrCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_GET_KING_QUEST_INFO_ADDR_CALL;

		__asm
		{
			push nKingQuestIndex
				call dword ptr[dwKingQuestInfoAddrCall]
			add esp, 4

				mov dwKingQuestInfoAddr, eax
		}
	}
	catch (...)
	{
		assert(false);
		dwKingQuestInfoAddr = 0;
	}

	return dwKingQuestInfoAddr;
}

void EnableWuDi( bool enable_wu_di )
{
	g_enable_wu_di_ = enable_wu_di;
}
