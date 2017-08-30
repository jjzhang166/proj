#include "StdAfx.h"
#ifdef _ENABLE_GAME_PACKET_TEST_ZHP_
#include "../JFZRPacket.h"
#include "../SendPacket.h"
#include "../jfzrbase.h"
#include "../EnumPidWnd.h"
#include "../jfzrpacketstruct.h"
#include "../jfzrstruct.h"
#else
#include "JFZRPacket.h"
#include "PacketString.h"
#include "jfzrbase.h"
#endif


DWORD g_dwValidationData_X = 0;		// 过图验证数据x
DWORD g_dwValidationData_Y = 0;		// 过图验证数据y
BYTE  g_bIsDummyExist = 0;

class JFZRGroupPacket : public PacketString
{
public:
	JFZRGroupPacket();
	JFZRGroupPacket(const BYTE* lpPacket);
	~JFZRGroupPacket(void);
};

JFZRGroupPacket::JFZRGroupPacket() : PacketString((const BYTE *)"")
{

}

JFZRGroupPacket::JFZRGroupPacket(const BYTE* lpPacket) : PacketString(lpPacket)
{

}

JFZRGroupPacket::~JFZRGroupPacket(void)
{
}

typedef	struct
{
	DWORD	str_dwNextAddrLeft;
	DWORD	str_dwNextAddrParent;
	DWORD	str_dwNextAddrRight;
	union
	{
		wchar_t*	wstr_ptr_english_id;
		wchar_t		wstr_english_id[8];			// wchar_t*8  大小为16
	};
	DWORD	str_dwenglishStrLen;
	DWORD	str_dwenglishStrLen_;
	DWORD	str_dwUnknow1;

	union
	{
		wchar_t*	wstr_ptr_chinese_id;
		wchar_t		wstr_chinese_id[8];			// wchar_t*8  大小为16
	};
	DWORD	str_dwChineseStrLen;
	DWORD	str_dwChineseStrLen_;
	DWORD	str_dwUnknow2;
}CHINESE_MAP,*PCHINESE_MAP;


// 游戏内部将字符串赋值给申请出来的内存
void JFZR_GameMemcpy(DWORD lpAddr, WCHAR* pszStr)
{
	int nLen = wcslen(pszStr);
	__asm
	{
		push nLen
			push pszStr
			mov ecx, lpAddr
			mov eax, JFZR_REVERSAL_GAME_MEMCPY_CALL
			call eax
	}
}


// 传入角色数据的 Direction x,y，获得封包发送数据 DirectionX,DirectionY
WORD GetDirection(FLOAT fDirection)
{
	try
	{
		WORD wDirection = 0;
		__asm
		{
			mov eax, dword ptr [fDirection]
			push eax
			mov eax, JFZR_REVERSAL_GET_DIRECTION_CALL
			call eax
			add esp, 4
			mov WORD ptr [wDirection], ax
		}
		return wDirection;
	}
	catch (...)
	{
		assert(false);
	}
	return 0;
}


FLOAT GetSpeed(DWORD dwSpeedAddr)
{
	try
	{

		FLOAT fSpeed = 0.0f;
		__asm
		{
			mov eax, dwSpeedAddr
			MOV ECX,DWORD PTR [eax]
			PUSH ECX
			mov eax, UPDATES_JFZR_REVERSAL_ROLE_BASE_ADDR
			mov eax, dword ptr[eax]
			LEA ECX,DWORD PTR DS:[EAX+JFZR_REVERSAL_GET_SPEED_OFFSET]
			mov eax, JFZR_REVERSAL_GET_SPEED_CALL
			call eax
			fstp dword ptr [fSpeed]
		}
		return fSpeed;
	}
	catch (...)
	{
		assert(false);
	}
	return 0.0f;
}

void UserSkillLockAdd()
{
	__asm
	{
		MOV ECX,JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR
		MOV EAX,0x1
		LOCK XADD DWORD PTR DS:[ECX],EAX
		INC EAX
	}
}

DWORD LockAdd(DWORD dwLockAddr)
{
	DWORD dwRet = 0;
	__asm
	{
		MOV ECX,dwLockAddr
		MOV EAX,0x1
		LOCK XADD DWORD PTR DS:[ECX],EAX
		INC EAX
		mov dwRet, eax
	}
	return dwRet;
}

DWORD GetSkillObjAddr(DWORD ptheSkillObjAddr,DWORD* dwPoint)
{
	DWORD dwResultObjAddr = 0;
	__asm
	{
		mov ecx, ptheSkillObjAddr
		push dwPoint
		mov eax, JFZR_REVERSAL_GET_SKILL_OBJ_ADDR_CALL
		call eax
		mov dwResultObjAddr, eax
	}

	return dwResultObjAddr;
}

DWORD GetProjectileSerial()
{
	DWORD	dwProjcetileSerial = 0;
	LockAdd(JFZR_REVERSAL_PROJECTILE_SERIAL_ADDR);
	__asm
	{
		push 0
		mov eax, JFZR_REVERSAL_PROJECTILE_SERIAL_ADDR
		mov eax, dword ptr [eax]
		push eax
		push 1
		mov eax, JFZR_REVERSAL_ROLE_BASE_ADDR
		mov eax, dword ptr [eax]
		push eax
		mov eax, JFZR_REVERSAL_PROJECTILE_SERIAL_CALL
		call eax
		mov dwProjcetileSerial, eax
	}

	return dwProjcetileSerial;
}

const wchar_t* GetChinese(const wchar_t* ptheEnglish)
{
	if( ptheEnglish == NULL )
	{
		return L"";
	}
	int nLen = wcslen(ptheEnglish)*2+0x20;		// +20足够大就好
	wchar_t* pwszNameid = (wchar_t*)JFZR_AllmMemory_Alloc(nLen);
	memset(pwszNameid,0,nLen);
	memcpy(pwszNameid,ptheEnglish,nLen);

	wchar_t* pwszNameIdTemp = (wchar_t*)&pwszNameid;

	PCHINESE_MAP ptheChineseMapObj = NULL;
	__asm
	{
		mov ebx, dword ptr [pwszNameIdTemp]
		push ebx
		mov eax, JFZR_REVERSAL_CHINESE_ENGLISH_MAP_ADDR
		mov edi, dword ptr [eax]
		lea ecx, [edi+JFZR_REVERSAL_CHINESE_ENGLISH_MAP_OFFSET___]
		mov esi, ecx
		mov eax, JFZR_REVERSAL_CHINESE_ENGLISH_MAP_OFFSET___
		call eax
		mov ptheChineseMapObj, eax
	}

	if( ptheChineseMapObj->str_dwChineseStrLen == 0 )
	{
		return L"";
	}

	if( ptheChineseMapObj->str_dwChineseStrLen_ >= 8 )
	{
		return ptheChineseMapObj->wstr_ptr_chinese_id;
	}

	return ptheChineseMapObj->wstr_chinese_id;
}	

_declspec(naked) void GetStringHash()
{
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		PUSH ESI
		MOV ESI,DWORD PTR SS:[EBP+0x8]
		MOVZX EAX,WORD PTR DS:[ESI]
		XOR ECX,ECX
		TEST AX,AX
		JE SHORT local_end
		PUSH EDI
local_do:
		MOVZX EAX,AX
		CDQ
		MOV EDI,0x1F
		IDIV EDI
		ADD ESI,0x2
		MOV EAX,EDX
		MOV EDX,ECX
		SHL EDX,0x5
		SUB EDX,ECX
		ADD EAX,EDX
		XOR EDX,EDX
		MOV ECX,0x3D09000
		DIV ECX
		MOVZX EAX,WORD PTR DS:[ESI]
		MOV ECX,EDX
		TEST AX,AX
		JNZ SHORT local_do
		POP EDI
local_end:
		MOV EAX,ECX
		POP ESI
		POP EBP
		RETN
	}
}

// 获取2字节的字串hash值
WORD GetHash(const wchar_t* wszStr)
{
	DWORD	dwHash = 0;
	__asm
	{
		push wszStr
		lea eax, GetStringHash
		call eax
		add esp, 4
		mov dwHash, eax
	}

	return (WORD)dwHash;
}

// 获取4字节的字串hash值
DWORD GetHash(const wchar_t* pszStr1, const wchar_t* pszStr2)
{
	DWORD	dwHash = 0;
	int i = 0;
	while( *(WORD*)(pszStr1+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(pszStr1+i);
		i++;
	}

	i = 0;
	while( *(WORD*)(pszStr2+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(pszStr2+i);
		i++;
	}
	return dwHash;
}

// 获取4字节的字串hash值
DWORD GetSigHash(const wchar_t* pszStr)
{
	DWORD	dwHash = 0;
	int i = 0;
	while( *(WORD*)(pszStr+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(pszStr+i);
		i++;
	}
	return dwHash;
}

// 申请内存
DWORD WINAPI JFZR_AllmMemory_Alloc(int nsize)
{
	try
	{
		DWORD	dwAllocAddr = 0;
		__asm
		{
			push nsize
			mov eax, JFZR_REVERSAL_ALLMMEMORY_ALLOC_ADDR
			call eax
			add esp, 4
			mov dwAllocAddr, eax
		}
		return dwAllocAddr;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

// 释放内存
void WINAPI JFZR_AllmMemory_Free(DWORD dwAllocAddr)
{
	try
	{
		__asm
		{
			push dwAllocAddr
			mov eax, JFZR_REVERSAL_ALLMMEMORY_FREE_ADDR
			call eax
			add esp, 4
		}
	}
	catch (...)
	{
		assert(false);
	}
}

// 拷贝的GetTickCount，在win7上会存在返回0的时候，不知道原因，不建议使用，可直接使用GetTickCount
DWORD WINAPI JFZR_GetTickCount()
{
// 	DWORD dwTime = 0;
// 	__asm
// 	{
// 		MOV EDX,0x7FFE0000
// 		MOV EAX,DWORD PTR DS:[EDX]
// 		MUL DWORD PTR DS:[EDX+0x4]
// 		SHRD EAX,EDX,0x18
// 		mov dwTime, eax
// 	}
// 	return dwTime;
	return GetTickCount();
}


DWORD GetRandomSeed()
{
	DWORD dwTemp = 0;
	__asm
	{
		mov eax, JFZR_REVERSAL_RANDOMSEED_CALL
		call eax
		mov dwTemp, eax
	}
	return dwTemp;
}

// 函数功能：使用技能发包步骤1，告诉服务器，我准备要使用技能
int WINAPI JFZR_UserSkillPacket1(
								  OUT char* pOutszSendBuffer,
								  DWORD dwSkillId		// 技能ID
								  )
{
	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	SEND_ACTION_SYSTEM_USESKILL theActionSystemUseSkill = {0};
	theActionSystemUseSkill.theHeader.wPacketLen = sizeof(theActionSystemUseSkill);
	theActionSystemUseSkill.theHeader.wMethodType = JFZR_REVERSAL_PACKET_SEND_HEADER;
	theActionSystemUseSkill.theHeader.wUrlType = GetHash(L"16e232a49cbd80393d7f4c0009244f20"/*"Apple::Protocol::ToServer::ActionSystem::UseSkill"*/);				// 使用技能
	theActionSystemUseSkill.dwSkillHash = dwSkillId;
	theActionSystemUseSkill.wParam = GetHash(L"Param");				// Param
	theActionSystemUseSkill.wassociative_array = GetHash(L"associative_array");	// associative_array
	theActionSystemUseSkill.dwsize = 0;
	theActionSystemUseSkill.wSelectedSkillTabIndex = 0;
	theActionSystemUseSkill.fMaxDt = 0.362020f;

	thePacket << theActionSystemUseSkill.theHeader.wPacketLen;
	thePacket << theActionSystemUseSkill.theHeader.wMethodType;
	thePacket << theActionSystemUseSkill.theHeader.wUrlType;
	thePacket << theActionSystemUseSkill.dwSkillHash;
	thePacket << theActionSystemUseSkill.wParam;
	thePacket << theActionSystemUseSkill.wassociative_array;
	thePacket << theActionSystemUseSkill.dwsize;
	thePacket << theActionSystemUseSkill.wSelectedSkillTabIndex;
	thePacket << theActionSystemUseSkill.fMaxDt;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}




// 函数功能：技能发包步骤2，开始使用技能
int WINAPI JFZR_UserSkillPacket2( 
					  OUT char* pOutszSendBuffer, 
					  const wchar_t* pszJobName ,		// 角色职业名称
					  const wchar_t* pszSkillName,		// 技能名称
					  float x,							// 开始使用技能时人物的坐标
					  float y, 
					  float z, 
					  DWORD dwDirectionX,				// 方向x
					  DWORD dwDirectionY,				// 方向y
					  FLOAT	fClientAbsoluteTime,		// 客户端绝对时间
					  DWORD	dwSkillId,					// 技能id
					  DWORD dwSkillLevel,				// 技能等级
					  DWORD dwPrevActionPlayTime		// 上次行动时间
					  )
{
	SKILL_ACTION_SYNC_PACKET theSkillActionSyncPacket;

	theSkillActionSyncPacket.theHeader.wPacketLen = sizeof(theSkillActionSyncPacket);
	theSkillActionSyncPacket.theHeader.wMethodType = JFZR_REVERSAL_PACKET_SEND_HEADER;
	theSkillActionSyncPacket.theHeader.wUrlType = GetHash(L"74c86e7c7a5628a1af55467d081c8b97"/*"Apple::Protocol::Both::ActionSystem::SkillActionSyncPacket"*/);			// 技能行动
	DWORD dwSerial;
	__asm
	{
		mov eax, JFZR_REVERSAL_ROLE_BASE_ADDR
		mov eax, dword ptr[eax]
		mov eax, dword ptr[eax+JFZR_REVERSAL_ROLE_SERIAL_OFFSET___]
		mov dwSerial, eax
	}
	theSkillActionSyncPacket.dwSerial = dwSerial;

	DWORD dwActionSerial = 0;
	UserSkillLockAdd();
	__asm
	{
		mov eax, JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR
		mov eax, dword ptr[eax]
		mov dwActionSerial, eax
	}
	theSkillActionSyncPacket.dwActionSerial	= dwActionSerial;
	UserSkillLockAdd();
	theSkillActionSyncPacket.dwActionHash = GetHash(pszJobName,pszSkillName);		// 行动hash   ActionHash
	theSkillActionSyncPacket.bExistParams = 0;
	theSkillActionSyncPacket.fActionStartPosition_X = x;	
	theSkillActionSyncPacket.fActionStartPosition_Z = z;
	theSkillActionSyncPacket.fActionStartPosition_Y = y;
	theSkillActionSyncPacket.wDirectionX = (WORD)dwDirectionX;		// DirectionX
	theSkillActionSyncPacket.wDirectionY = (WORD)dwDirectionY;		// DirectionY
	theSkillActionSyncPacket.wMovingDirectionX = 0;
	theSkillActionSyncPacket.wMovingDirectionY = 0;
	theSkillActionSyncPacket.fClientAbsoluteTime = fClientAbsoluteTime;
	DWORD dwRandomSeed = 0;
	__asm
	{
		mov eax, JFZR_REVERSAL_RANDOMSEED_CALL
		call eax
		mov dwRandomSeed, eax
	}
	theSkillActionSyncPacket.dwRandomSeed = dwRandomSeed;
	theSkillActionSyncPacket.dwPrevActionSerial = dwActionSerial-2;
	theSkillActionSyncPacket.dwPrevActionPlayTime = dwPrevActionPlayTime;
	theSkillActionSyncPacket.bSkipValidation1 = 1;
	theSkillActionSyncPacket.dwSkillId = dwSkillId;
	theSkillActionSyncPacket.wSkillLevel = (WORD)dwSkillLevel;
	theSkillActionSyncPacket.bSkipValidation2 = 1;


	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	thePacket << theSkillActionSyncPacket.theHeader.wPacketLen;
	thePacket << theSkillActionSyncPacket.theHeader.wMethodType;
	thePacket << theSkillActionSyncPacket.theHeader.wUrlType;
	thePacket << theSkillActionSyncPacket.dwSerial;
	thePacket << theSkillActionSyncPacket.dwActionSerial;
	thePacket << theSkillActionSyncPacket.dwActionHash;
	thePacket << theSkillActionSyncPacket.bExistParams;
	thePacket << theSkillActionSyncPacket.fActionStartPosition_X;
	thePacket << theSkillActionSyncPacket.fActionStartPosition_Z;
	thePacket << theSkillActionSyncPacket.fActionStartPosition_Y;
	thePacket << theSkillActionSyncPacket.wDirectionX;
	thePacket << theSkillActionSyncPacket.wDirectionY;
	thePacket << theSkillActionSyncPacket.wMovingDirectionX;
	thePacket << theSkillActionSyncPacket.wMovingDirectionY;
	thePacket << theSkillActionSyncPacket.fClientAbsoluteTime;
	thePacket << theSkillActionSyncPacket.dwRandomSeed;
	thePacket << theSkillActionSyncPacket.dwPrevActionSerial;
	thePacket << theSkillActionSyncPacket.dwPrevActionPlayTime;
	thePacket << theSkillActionSyncPacket.bSkipValidation1;
	thePacket << theSkillActionSyncPacket.dwSkillId;
	thePacket << theSkillActionSyncPacket.wSkillLevel;
	thePacket << theSkillActionSyncPacket.bSkipValidation2;
	
	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;;
}


int WINAPI JFZR_UserSkillPacket3(
					  OUT char* pOutszSendBuffer,
					  const wchar_t* pszJobName,		// 职业名称
					  float x,							// 使用完技能后，人物所在的坐标
					  float y,
					  float z,
					  DWORD dwDirectionX,				// 方向x
					  DWORD dwDirectionY,				// 方向y
					  FLOAT	fClientAbsoluteTime,		// 客户端绝对时间
					  DWORD dwPrevActionPlayTime		// 上次行动时间
					  )
{
	typedef struct  
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwSerial;
		DWORD	dwActionSerial;
		BYTE	bExistParams;
		WORD	wParams;
		WORD	wassociative_array;
		DWORD	dwsize;
		WORD	wDirectionLen;
		std::wstring	strDirection;
		WORD	wDirectionDataLen;
		std::wstring	strDirectionData;
		FLOAT	fActionStartPositionX;
		FLOAT	fActionStartPositionZ;
		FLOAT	fActionStartPositionY;  
		WORD	wDirectionX;
		WORD	wDirectionY;
		WORD	wMovingDirectionX;
		WORD	wMovingDirectionY;
		FLOAT	fClientAbsoluteTime;
		DWORD	dwRandomSeed;
		DWORD	dwPrevActionSerial;
		DWORD	dwPrevActionPlayTime;
		BYTE	bSkipValidation;
	}USESKILL,*PUSESKILL;
	
	DWORD dwTemp = 0;
	int nLen = 0x38;
	//*(WORD*)pOutszSendBuffer = nLen;
	*(WORD*)(pOutszSendBuffer+2) = JFZR_REVERSAL_PACKET_SEND_HEADER;
	*(WORD*)(pOutszSendBuffer+4) = GetHash(L"038f2071d608b0d6942d097326ccbb27"/*"Apple::Protocol::Both::ActionSystem::ActionSyncPacket"*/);			// 技能行动
	__asm
	{
		mov eax, JFZR_REVERSAL_ROLE_BASE_ADDR
		mov eax, dword ptr[eax]
		mov eax, dword ptr[eax+JFZR_REVERSAL_ROLE_SERIAL_OFFSET___]
		mov dwTemp, eax
	}
	*(DWORD*)(pOutszSendBuffer+6) = dwTemp;			// 序号

	DWORD dwSerial = 0;
	__asm
	{
		mov eax, JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR
		mov eax, dword ptr[eax]
		mov dwSerial, eax
	}
	*(DWORD*)(pOutszSendBuffer+10) = dwSerial;		// 行动序号

	*(DWORD*)(pOutszSendBuffer+14) = GetHash(pszJobName,L"Stand");	// 行动hash
	*(BYTE*)(pOutszSendBuffer+18) = 0;								// ExistParams

	*(FLOAT*)(pOutszSendBuffer+19) = x;		// X
	*(FLOAT*)(pOutszSendBuffer+23) = z;		// Z
	*(FLOAT*)(pOutszSendBuffer+27) = y;		// Y

	*(WORD*)(pOutszSendBuffer+31) = (WORD)dwDirectionX;		// DirectionX
	*(WORD*)(pOutszSendBuffer+33) = (WORD)dwDirectionY;		// DirectionY
	*(WORD*)(pOutszSendBuffer+35) = 0x0;					// MovingDirectionX
	*(WORD*)(pOutszSendBuffer+37) = 0x0;					// MovingDirectionY

	*(FLOAT*)(pOutszSendBuffer+39) = fClientAbsoluteTime;		// ClientAbsoluteTime

	__asm
	{
		mov eax, JFZR_REVERSAL_RANDOMSEED_CALL
		call eax
		mov dwTemp, eax
	}
	*(DWORD*)(pOutszSendBuffer+43) = dwTemp;		// RandomSeed

	dwSerial -= 2;
	*(DWORD*)(pOutszSendBuffer+47) = dwSerial;		// PrevActionSerial

	*(DWORD*)(pOutszSendBuffer+51) = dwPrevActionPlayTime;	// PrevActionPlayTime
	*(BYTE*)(pOutszSendBuffer+55) = 1;						// SkipValidation

	return nLen;
}

// 函数功能：攻击对象的信息
int WINAPI JFZR_SkillHitPacketBody(
							OUT char* pOutszSendBuffer,
							int nitem,					// 多个被攻击对象的序号,从0开始，相当于结构体数组的下标
							DWORD dwProjectileSerial,	// 抛射序号，测试给0有效
							DWORD dwDefenderSerial,		// 防御者序号
							DWORD dwAttackerSerial,		// 攻击者序号
							FLOAT fAttackPowerRate,		// 攻击百分比
							DWORD dwProjectileHash,		// 技能名的hash，但有些技能不一样，具体技能数据看提供资料
							DWORD dwActionHash,			// 行动hash
							DWORD dwSkillHash,			// 技能hash
							DWORD dwSkillLevel,			// 技能等级
							DWORD dwTimeStamp,			// 时间戳，测试给0有效
							BYTE  bFirstHit,			// 是否第一次
							BYTE  bIsCounterAttack,
							WORD  wPhysicalAttribute,			// 攻击类型
							wchar_t* pwszElementalAttribute,		// 技能元素属性
							DWORD	dwDeltaHp						// 怪物受攻击血值，先填写怪物的血值 
							)
{
	OutputDebugStringA("[JFZR] *********************JFZR_SkillHitPacketBody*********************");
	SEND_REQUEST_PROJECTILE_HIT_BODY	theHitBody = {0};
	

	wchar_t szItem[64] = {0};
	wsprintfW(szItem,L"item_%d",nitem);
	
	wchar_t		wszHitTarget[] = L"HIT_TARGET";				// 攻击目标，攻击封包都是这个类型
// 	wchar_t		wszPhysicalAttribute[] = L"PIERCE";			// 攻击类型，未找到此标志在何处 中英文映射中倒是有地方可以判断
// 	wchar_t		wszElementalAttribute[] = L"ELEC";//L"ELEC";			// 技能元素属性

	theHitBody.wItem_i				= GetHash(szItem);					// item_0
	theHitBody.wHitInfo				= GetHash(L"Apple::HitInfo");		// Apple::HitInfo
	theHitBody.dwProjectileSerial	= dwProjectileSerial;
	theHitBody.dwDefenderSerial		= dwDefenderSerial;
	theHitBody.dwAttackerSerial		= dwAttackerSerial;
	theHitBody.wHitResultLen		= wcslen(wszHitTarget)+1;
	theHitBody.strHitResult			= wszHitTarget;
	theHitBody.dwHitType			= 1;

	dwDeltaHp = 0-dwDeltaHp+1;
	theHitBody.dwDeltaHp			= dwDeltaHp;		// 怪物当前减少血值求补
	theHitBody.dwRandomSeed			= 0;				// 游戏发包本身为0
	theHitBody.wHitProjectileInfo	= GetHash(L"HitProjectileInfo");
	theHitBody.wAppleHitProjectileInfo		= GetHash(L"Apple::HitProjectileInfo");
	theHitBody.fAttackPowerRate				= fAttackPowerRate;
	theHitBody.fDamagePlusRateToSuperArmor	= 0.0f;		// 打怪增加攻击力
	theHitBody.fDamagePlusRateForCounter	= 0.0f;
	theHitBody.wPhysicalAttribute			= wPhysicalAttribute;		// 10.30改变字段
//	theHitBody.wPhysicalAttributeLen		= wcslen(wszPhysicalAttribute)+1;
//	theHitBody.strPhysicalAttribute			= wszPhysicalAttribute;
	theHitBody.wElementalAttributeLen		= wcslen(pwszElementalAttribute)+1;
	theHitBody.strElementalAttribute		= pwszElementalAttribute;
	theHitBody.dwEventOptionSerial			= 0;
	theHitBody.dwEventOptionDamageReduceRate	= 0;
	theHitBody.bIsCounterAttack					= bIsCounterAttack;
	theHitBody.bIsHitSuperArmor					= 0;		// 打怪增加攻击力
	theHitBody.dwProjectileHash					= dwProjectileHash;		// ProjectileHash  抛射hash，其实是技能ID，也是技能名称的hash值
	theHitBody.bFirstHit						= bFirstHit;
	theHitBody.dwActionHash						= dwActionHash;
	theHitBody.dwSkillHash						= dwSkillHash;
	theHitBody.dwSkillLevel						= dwSkillLevel;
	theHitBody.fProjectileScalex				= 1.0f;
	theHitBody.fProjectileScaley				= 1.0f;
	theHitBody.fProjectileScalez				= 1.0f;
	theHitBody.dwTimeStamp						= dwTimeStamp;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	thePacket << theHitBody.wItem_i;
	thePacket << theHitBody.wHitInfo;
	thePacket << theHitBody.dwProjectileSerial;
	thePacket << theHitBody.dwDefenderSerial;
	thePacket << theHitBody.dwAttackerSerial;
	thePacket << theHitBody.strHitResult;
	thePacket << theHitBody.dwHitType;
	thePacket << theHitBody.dwDeltaHp;
	thePacket << theHitBody.dwRandomSeed;
	thePacket << theHitBody.wHitProjectileInfo;
	thePacket << theHitBody.wAppleHitProjectileInfo;
	thePacket << theHitBody.fAttackPowerRate;
	thePacket << theHitBody.fDamagePlusRateToSuperArmor;
	thePacket << theHitBody.fDamagePlusRateForCounter;
	thePacket << theHitBody.wPhysicalAttribute;			// 10.30改变字段
//	thePacket << theHitBody.strPhysicalAttribute;
	thePacket << theHitBody.strElementalAttribute;
	thePacket << theHitBody.dwEventOptionSerial;
	thePacket << theHitBody.dwEventOptionDamageReduceRate;
	thePacket << theHitBody.bIsCounterAttack;
	thePacket << theHitBody.bIsHitSuperArmor;
	thePacket << theHitBody.dwProjectileHash;
	thePacket << theHitBody.bFirstHit;
	thePacket << theHitBody.dwActionHash;
	thePacket << theHitBody.dwSkillHash;
	thePacket << theHitBody.dwSkillLevel;
	thePacket << theHitBody.fProjectileScalex;
	thePacket << theHitBody.fProjectileScaley;
	thePacket << theHitBody.fProjectileScalez;
	thePacket << theHitBody.dwTimeStamp;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：攻击对象的包头
int WINAPI JFZR_SkillHitPacketHeader(
								OUT char* pOutszSendBuffer,
								int nsize			// 被攻击对象的个数
								)
{
	OutputDebugStringA("[JFZR] *********************JFZR_SkillHitPacketHeader*********************");
	SEND_SKILL_HIT_HEADER	theSkillHitHeader;

	theSkillHitHeader.theHeader.wPacketLen	= 0;
	theSkillHitHeader.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theSkillHitHeader.theHeader.wUrlType	= GetHash(L"c492f0d9b91babcb562db514f3a9a86b"/*"Apple::Protocol::ToServer::ActionSystem::RequestProjectileHit"*/);			// 技能攻击
	theSkillHitHeader.wRequestHitInfos	= GetHash(L"RequestHitInfos");			// RequestHitInfos
	theSkillHitHeader.warray			= GetHash(L"array");			// array
	theSkillHitHeader.dwsize			= nsize;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	thePacket << theSkillHitHeader.theHeader.wPacketLen;
	thePacket << theSkillHitHeader.theHeader.wMethodType;
	thePacket << theSkillHitHeader.theHeader.wUrlType;
	thePacket << theSkillHitHeader.wRequestHitInfos;
	thePacket << theSkillHitHeader.warray;
	thePacket << theSkillHitHeader.dwsize;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：进入副本，再次挑战副本
int WINAPI JFZR_StartStage(
								   OUT char* pOutszSendBuffer,
								   const wchar_t* pszStartStageName,		// 副本名称，英文版，非显示界面的副本名称
								   const wchar_t* pszWarpGateId,
								   int nStageLevel,							// 副本难度
								   BYTE	bIsRestart							// 副本是否重启
								   )
{
	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	SEND_STAGE_SYSTEM_START_STAGE	theStartStage;
	theStartStage.theHeader.wPacketLen	= 0;
	theStartStage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theStartStage.theHeader.wUrlType	= GetHash(L"dd246d8973092d9a60c0bddb6f8f39b0"/*"Apple::Protocol::ToServer::StageSystem::StartStage"*/);		// 进入副本
	theStartStage.dwStageHash	= GetSigHash(pszStartStageName);
	theStartStage.dwDifficulty	= nStageLevel;
	theStartStage.bIsFirstPlay	= 0;
	theStartStage.wWarpGateIdLen	= wcslen(pszWarpGateId)+1;
	theStartStage.strWarpGateId		= pszWarpGateId;
	theStartStage.bIsRestart		= bIsRestart;
	theStartStage.bUseIntrusionItem	= 0;
	theStartStage.bIsMaxLevelExpendTutorialPlay = 0;
	
	thePacket << theStartStage.theHeader.wPacketLen;
	thePacket << theStartStage.theHeader.wMethodType;
	thePacket << theStartStage.theHeader.wUrlType;
	thePacket << theStartStage.dwStageHash;
	thePacket << theStartStage.dwDifficulty;
	thePacket << theStartStage.bIsFirstPlay;
	thePacket << theStartStage.strWarpGateId;
	thePacket << theStartStage.bIsRestart;
	thePacket << theStartStage.bUseIntrusionItem;
	thePacket << theStartStage.bIsMaxLevelExpendTutorialPlay;

	// 这里在进入副本的封包对其清0，以保证进入副本后，得到到的数据的正确的（如果副本内第一个房间没有数据验证，则从城镇中获取到的NPC验证数据的错误的(这个时候的验证数据应该为0)，
	// 如果有验证数据，则正确(在副本内会再次接受到NPC信息)，所以城镇数据不可取，必须清0）
	g_dwValidationData_X = 0;
	g_dwValidationData_Y = 0;
	g_bIsDummyExist = 0;

	return thePacket.m_lpPacketCurrent-thePacket.m_lpPacketBegin;
}


// 函数功能：告诉服务器步入传送门
int WINAPI JFZR_GotoNextRoom1(
							 OUT char* pOutszSendBuffer,
							 const wchar_t* pszRoleName,			// 玩家名称
							 const wchar_t* pszWarp					// 第几个门，如一门：Warp01； 二门：Warp02等
							 )
{
	SEND_STEP_IN_WARP_GATE	theStepInWarpGate;
	theStepInWarpGate.theHeader.wPacketLen	= 0;
	theStepInWarpGate.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theStepInWarpGate.theHeader.wUrlType	= GetHash(L"6490af8101d3c41132b3366b2a46a72d"/*"Apple::Protocol::ToServer::ActorSystem::StepInWarpGate"*/);
	theStepInWarpGate.wPlayerIdLen	= wcslen(pszRoleName)+1;
	theStepInWarpGate.strPlayerId	= pszRoleName;
	theStepInWarpGate.bStepIn		= 1;
	theStepInWarpGate.wWarpGateIdLen	= wcslen(pszWarp)+1;
	theStepInWarpGate.strWarpGateId		= pszWarp;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	thePacket << theStepInWarpGate.theHeader.wPacketLen;
	thePacket << theStepInWarpGate.theHeader.wMethodType;
	thePacket << theStepInWarpGate.theHeader.wUrlType;
	thePacket << theStepInWarpGate.strPlayerId;
	thePacket << theStepInWarpGate.bStepIn;
	thePacket << theStepInWarpGate.strWarpGateId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：告诉服务器，当前角色坐标等信息
int WINAPI JFZR_GotoNextRoom2(
							  OUT char* pOutszSendBuffer,
							  DWORD dwRoleSerial,			// 人物对象序号
							  float x,
							  float y,
							  float z,
							  DWORD dwDirectionX,			// 方向x
							  DWORD dwDirectionY			// 方向y
							  )
{
	SEND_DIRECTION_SYNC_PACKET	theDirectionSyncPacket;
	theDirectionSyncPacket.theHeader.wPacketLen		= 0;
	theDirectionSyncPacket.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theDirectionSyncPacket.theHeader.wUrlType		= GetHash(L"e1ddc887f465dd79053399f012a3cac9"/*"Apple::Protocol::Both::ActionSystem::DirectionSyncPacket"*/);
	theDirectionSyncPacket.dwSerial					= dwRoleSerial;
	theDirectionSyncPacket.thePosition.fx			= x;
	theDirectionSyncPacket.thePosition.fz			= z;
	theDirectionSyncPacket.thePosition.fy			= y;
	theDirectionSyncPacket.wDirectionX				= (WORD)dwDirectionX;
	theDirectionSyncPacket.wDirectionY				= (WORD)dwDirectionY;
	theDirectionSyncPacket.wMovingDirectionX		= 0;
	theDirectionSyncPacket.wMovingDirectionY		= 0;

	JFZRGroupPacket	thePacket;
	thePacket	= (const BYTE*)pOutszSendBuffer;

	thePacket << theDirectionSyncPacket.theHeader.wPacketLen;
	thePacket << theDirectionSyncPacket.theHeader.wMethodType;
	thePacket << theDirectionSyncPacket.theHeader.wUrlType;
	thePacket << theDirectionSyncPacket.dwSerial;
	thePacket << theDirectionSyncPacket.thePosition.fx;
	thePacket << theDirectionSyncPacket.thePosition.fz;
	thePacket << theDirectionSyncPacket.thePosition.fy;
	thePacket << theDirectionSyncPacket.wDirectionX;
	thePacket << theDirectionSyncPacket.wDirectionY;
	thePacket << theDirectionSyncPacket.wMovingDirectionX;
	thePacket << theDirectionSyncPacket.wMovingDirectionY;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}



// 函数功能：告诉服务器选择改变房间
int WINAPI JFZR_ChangeSector(
							  OUT char*			pOutszSendBuffer,
							  const wchar_t*	pszWarp,		// 第几个门
							  DWORD				dwValidationData_Y,
							  DWORD				dwValidationData_X,
							  BYTE				bIsDummyExist,
							  FLOAT				fAttackSpeed,
							  FLOAT				fMoveSpeed,
							  FLOAT				fEvadeRating
							  )
{
	SEND_CHANGE_SECTOR	theChangeSector;
	theChangeSector.theHeader.wPacketLen	= 0;
	theChangeSector.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theChangeSector.theHeader.wUrlType		= GetHash(L"51d107379e1955235e4e5dc41dd2e391"/*"Apple::Protocol::ToServer::StageSystem::ChangeSector"*/);
	theChangeSector.wWarpGateIdLen			= wcslen(pszWarp);
	theChangeSector.strWarpGateId			= pszWarp;
	theChangeSector.fAttackSpeed			= fAttackSpeed;
	theChangeSector.fMoveSpeed				= fMoveSpeed;
	theChangeSector.fEvadeRating			= fEvadeRating;
	theChangeSector.bIsDummyExist			= bIsDummyExist;		// 增加一个标志，如果有验证数据，则填1，如果没有验证数据，则填0
	theChangeSector.dwValidationData_Y		= dwValidationData_Y;
	theChangeSector.dwValidationData_X		= dwValidationData_X;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theChangeSector.theHeader.wPacketLen;
	thePacket << theChangeSector.theHeader.wMethodType;
	thePacket << theChangeSector.theHeader.wUrlType;
	thePacket << theChangeSector.strWarpGateId;
	thePacket << theChangeSector.fAttackSpeed;
	thePacket << theChangeSector.fMoveSpeed;
	thePacket << theChangeSector.fEvadeRating;
	thePacket << theChangeSector.bIsDummyExist;
	thePacket << theChangeSector.dwValidationData_Y;
	thePacket << theChangeSector.dwValidationData_X;

	wchar_t wszPrint[256];
	swprintf_s(wszPrint,L"[JFZR] WarpGateId = %s AttackSpeed = %f MoveSpeed = %f EvadeRating = %f ValidationData_Y = %p ValidationData_X = %p",
		theChangeSector.strWarpGateId.c_str(),
		fAttackSpeed,
		fMoveSpeed,
		fEvadeRating,
		dwValidationData_Y,
		dwValidationData_X);

	OutputDebugStringW(wszPrint);

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：返回城镇
int WINAPI JFZR_RetCity(
						 OUT char* pOutszSendBuffer
						 )
{
	SEND_RETURN_VILLAGE	theReturnVillage;
	theReturnVillage.theHeader.wPacketLen	= 0;
	theReturnVillage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theReturnVillage.theHeader.wUrlType		= GetHash(L"85ac893b6ce6de3a6dee371c56807d8e"/*"Apple::Protocol::ToServer::StageSystem::ReturnVillage"*/);
	theReturnVillage.bAlone					= 1;
	theReturnVillage.dwSumOfDeltaHp			= 0;		// 10.30新增数据段

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theReturnVillage.theHeader.wPacketLen;
	thePacket << theReturnVillage.theHeader.wMethodType;
	thePacket << theReturnVillage.theHeader.wUrlType;
	thePacket << theReturnVillage.bAlone;
	thePacket << theReturnVillage.dwSumOfDeltaHp;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：拾取物品，人物需要在物品旁边
int WINAPI JFZR_PickUpItem1(
					   OUT char* pOutszSendBuffer,
					   DWORD dwItemSerial		// 物品序号
					   )
{
	SEND_PICK_UP_ITEM	thePickUpItem;
	thePickUpItem.theHeader.wPacketLen	= 0;
	thePickUpItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	thePickUpItem.theHeader.wUrlType	= GetHash(L"76883bc7e7b32d16bdf424a2071c1ee8"/*"Apple::Protocol::ToServer::InventorySystem::PickUpItem"*/);
	thePickUpItem.dwSerial				= dwItemSerial;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << thePickUpItem.theHeader.wPacketLen;
	thePacket << thePickUpItem.theHeader.wMethodType;
	thePacket << thePickUpItem.theHeader.wUrlType;
	thePacket << thePickUpItem.dwSerial;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数无用
int WINAPI JFZR_PickUpItem2(
							OUT char* pOutszSendBuffer,
							DWORD dwFlag
							)
{
	int nLen = 0x7;
	*(WORD*)(pOutszSendBuffer+2) = JFZR_REVERSAL_PACKET_SEND_HEADER;
	*(WORD*)(pOutszSendBuffer+4) = GetHash(L"689050d4a78ae513941ff69e7c0a691d"/*"Apple::Protocol::ToServer::ShopSystem::GetRepairPrice"*/);
	*(DWORD*)(pOutszSendBuffer+6) = (BYTE)dwFlag;
	return nLen;
}

// 函数功能：修理装备
int WINAPI JFZR_RepairItem(  OUT char* pOutszSendBuffer )
{
	SEND_REPAIR_ITEM	theRepairItem;
	theRepairItem.theHeader.wPacketLen	= 0;
	theRepairItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theRepairItem.theHeader.wUrlType	= GetHash(L"a1cf156de551bb58d0271c523b5ad578"/*"Apple::Protocol::ToServer::ShopSystem::RepairItem"*/);	 // 行为
	theRepairItem.bIsEquipmentRepair	= 1;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRepairItem.theHeader.wPacketLen;
	thePacket << theRepairItem.theHeader.wMethodType;
	thePacket << theRepairItem.theHeader.wUrlType;
	thePacket << theRepairItem.bIsEquipmentRepair;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：走路封包  开始移动封包暂时不用调，调用多了客户端会崩溃，直接填写停止封包即可
// 停止移动封包和开始移动封包不同的两点分别是：
// 1//方向全部为0
// 2//GetHash(L"WARRIOR",L"Stand")
int WINAPI JFZR_ActionSyncPacket(
					 OUT char*	pOutszSendBuffer,
					 DWORD		dwRoleSerial,			// 人物序号
					 DWORD		dwActionHash,
					 float		fx,						// 坐标
					 float		fy,
					 float		fz,
					 float		fDirectionX,			// 方向1
					 float		fDirectionY,
					 float		fDirectionZ,
					 DWORD		dwDirectionX,			// 方向2
					 DWORD		dwDirectionY,
					 BYTE		bSkipValidation,
					 float		fPrevActionPlayTime,
					 DWORD		dwActionSerial,
					 DWORD		dwPrevActionSerial,
					 DWORD		dwRandomSeed,
					 FLOAT		fClientAbsoluteTime,
					 BYTE		bbExistParams			// 很重要，如果值为1表示玩家移动，如果值为0表示NPC移动
					 )
{
	OutputDebugStringA("[JFZR] *********************JFZR_ActionSyncPacket*********************");
	wchar_t	wszitem_first_i[] = L"Direction";
	wchar_t wszitem_second_i[256] = {0};
	swprintf_s(wszitem_second_i,L"(%f, %f, %f)",fDirectionX,fDirectionZ,fDirectionY);		// Direction 方向的字符串

	SEND_ACTION_SYNC_PACKET	theActionSyncPacket;
	theActionSyncPacket.theHeader.wPacketLen	= 0;
	theActionSyncPacket.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theActionSyncPacket.theHeader.wUrlType		= GetHash(L"038f2071d608b0d6942d097326ccbb27"/*"Apple::Protocol::Both::ActionSystem::ActionSyncPacket"*/);	 // 行为
	theActionSyncPacket.dwSerial				= dwRoleSerial;		// 角色Serial
	theActionSyncPacket.dwActionSerial			= dwActionSerial;	//LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);		// 行动序号   ActionSerial
	//LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	theActionSyncPacket.dwActionHash			= dwActionHash;
	theActionSyncPacket.bExistParams			= bbExistParams;

	if( theActionSyncPacket.bExistParams == 1 )
	{
		theActionSyncPacket.wParams					= GetHash(L"Params");			// Params
		theActionSyncPacket.wassociative_array		= GetHash(L"associative_array");		// associative_array
		theActionSyncPacket.dwsize					= 1;
		theActionSyncPacket.Item[0].witem_first_iLen	= wcslen(wszitem_first_i)+1;
		theActionSyncPacket.Item[0].stritem_first_i		= wszitem_first_i;
		theActionSyncPacket.Item[0].witem_second_iLen	= wcslen(wszitem_second_i)+1;
		theActionSyncPacket.Item[0].stritem_second_i	= wszitem_second_i;
	}

	theActionSyncPacket.theActionStartPosition.fx	= fx;
	theActionSyncPacket.theActionStartPosition.fz	= fz;
	theActionSyncPacket.theActionStartPosition.fy	= fy;
	theActionSyncPacket.wDirectionX					= (WORD)dwDirectionX;
	theActionSyncPacket.wDirectionY					= (WORD)dwDirectionY;
	theActionSyncPacket.wMovingDirectionX			= 0;
	theActionSyncPacket.wMovingDirectionY			= 0;
	theActionSyncPacket.fClientAbsoluteTime			= fClientAbsoluteTime;	// (FLOAT)GetTickCount();
	theActionSyncPacket.dwRandomSeed				= dwRandomSeed; // GetRandomSeed();
	theActionSyncPacket.dwPrevActionSerial			= dwPrevActionSerial;			// 不管开始还是结束，现在都有值
	theActionSyncPacket.fPrevActionPlayTime			= fPrevActionPlayTime;			// 此数据需要从对象中获取
	theActionSyncPacket.bSkipValidation				= bSkipValidation;



	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theActionSyncPacket.theHeader.wPacketLen;
	thePacket << theActionSyncPacket.theHeader.wMethodType;
	thePacket << theActionSyncPacket.theHeader.wUrlType;
	thePacket << theActionSyncPacket.dwSerial;
	thePacket << theActionSyncPacket.dwActionSerial;
	thePacket << theActionSyncPacket.dwActionHash;
	thePacket << theActionSyncPacket.bExistParams;

	if( theActionSyncPacket.bExistParams == 1 )
	{
		thePacket << theActionSyncPacket.wParams;
		thePacket << theActionSyncPacket.wassociative_array;
		thePacket << theActionSyncPacket.dwsize;
		thePacket << theActionSyncPacket.Item[0].stritem_first_i;
		thePacket << theActionSyncPacket.Item[0].stritem_second_i;
	}
	thePacket << theActionSyncPacket.theActionStartPosition.fx;
	thePacket << theActionSyncPacket.theActionStartPosition.fz;
	thePacket << theActionSyncPacket.theActionStartPosition.fy;
	thePacket << theActionSyncPacket.wDirectionX;
	thePacket << theActionSyncPacket.wDirectionY;
	thePacket << theActionSyncPacket.wMovingDirectionX;
	thePacket << theActionSyncPacket.wMovingDirectionY;
	thePacket << theActionSyncPacket.fClientAbsoluteTime;
	thePacket << theActionSyncPacket.dwRandomSeed;
	thePacket << theActionSyncPacket.dwPrevActionSerial;
	thePacket << theActionSyncPacket.fPrevActionPlayTime;
	thePacket << theActionSyncPacket.bSkipValidation;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

int WINAPI JFZR_VillageActionSyncPacket(
					 OUT char*	pOutszSendBuffer,
					 DWORD		dwRoleSerial,			// 人物序号
					 const wchar_t*	pszJobName,			// 角色职业
					 const wchar_t*	pszCmdName,			// "FastRun" 或 "Stand"
					 float		fx,						// 坐标
					 float		fy,
					 float		fz,
					 float		fDirectionX,			// 方向1
					 float		fDirectionY,
					 float		fDirectionZ,
					 DWORD		dwDirectionX,			// 方向2
					 DWORD		dwDirectionY
					 )
{
	wchar_t	wszitem_first_i[] = L"Direction";
	wchar_t wszitem_second_i[256] = {0};
	swprintf_s(wszitem_second_i,L"(%f, %f, %f)",fDirectionX,fDirectionZ,fDirectionY);		// Direction 方向的字符串

	SEND_VILLAGE_ACTION_SYNC_PACKET	theVillageActionSyncPacket;
	theVillageActionSyncPacket.theHeader.wPacketLen	= 0;
	theVillageActionSyncPacket.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theVillageActionSyncPacket.theHeader.wUrlType		= GetHash(L"982d94fcd2d7650a650a2644ff41d9ac"/*"Apple::Protocol::Both::ActionSystem::VillageActionSyncPacket"*/);	 // 行为
	theVillageActionSyncPacket.dwSerial				= dwRoleSerial;		// 角色Serial
	theVillageActionSyncPacket.dwActionSerial			= LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);		// 行动序号   ActionSerial
	LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	theVillageActionSyncPacket.dwActionHash			= GetHash(pszJobName,pszCmdName);
	theVillageActionSyncPacket.bExistParams			= 1;
	theVillageActionSyncPacket.wParams					= GetHash(L"Params");			// Params
	theVillageActionSyncPacket.wassociative_array		= GetHash(L"associative_array");		// associative_array
	theVillageActionSyncPacket.dwsize					= 1;
	theVillageActionSyncPacket.Item[0].witem_first_iLen	= wcslen(wszitem_first_i)+1;
	theVillageActionSyncPacket.Item[0].stritem_first_i		= wszitem_first_i;
	theVillageActionSyncPacket.Item[0].witem_second_iLen	= wcslen(wszitem_second_i)+1;
	theVillageActionSyncPacket.Item[0].stritem_second_i	= wszitem_second_i;
	theVillageActionSyncPacket.theActionStartPosition.fx	= fx;
	theVillageActionSyncPacket.theActionStartPosition.fz	= fz;
	theVillageActionSyncPacket.theActionStartPosition.fy	= fy;
	theVillageActionSyncPacket.wDirectionX					= (WORD)dwDirectionX;
	theVillageActionSyncPacket.wDirectionY					= (WORD)dwDirectionY;
	theVillageActionSyncPacket.wMovingDirectionX			= 0;
	theVillageActionSyncPacket.wMovingDirectionY			= 0;
	theVillageActionSyncPacket.fClientAbsoluteTime			= (FLOAT)GetTickCount();

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theVillageActionSyncPacket.theHeader.wPacketLen;
	thePacket << theVillageActionSyncPacket.theHeader.wMethodType;
	thePacket << theVillageActionSyncPacket.theHeader.wUrlType;
	thePacket << theVillageActionSyncPacket.dwSerial;
	thePacket << theVillageActionSyncPacket.dwActionSerial;
	thePacket << theVillageActionSyncPacket.dwActionHash;
	thePacket << theVillageActionSyncPacket.bExistParams;
	thePacket << theVillageActionSyncPacket.wParams;
	thePacket << theVillageActionSyncPacket.wassociative_array;
	thePacket << theVillageActionSyncPacket.dwsize;
	thePacket << theVillageActionSyncPacket.Item[0].stritem_first_i;
	thePacket << theVillageActionSyncPacket.Item[0].stritem_second_i;
	thePacket << theVillageActionSyncPacket.theActionStartPosition.fx;
	thePacket << theVillageActionSyncPacket.theActionStartPosition.fz;
	thePacket << theVillageActionSyncPacket.theActionStartPosition.fy;
	thePacket << theVillageActionSyncPacket.wDirectionX;
	thePacket << theVillageActionSyncPacket.wDirectionY;
	thePacket << theVillageActionSyncPacket.wMovingDirectionX;
	thePacket << theVillageActionSyncPacket.wMovingDirectionY;
	thePacket << theVillageActionSyncPacket.fClientAbsoluteTime;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 穿戴装备
int WINAPI JFZR_EquipItem(
						  OUT char*	pOutszSendBuffer,
						  int nEquipSlot,					// 装备槽
						  int nInvenSlot,					// 背包槽
						  const wchar_t* pwszEquipTab,		// L"BASIC"
						  const wchar_t* pwszInvenTab		// L"EQUIPMENT_TAB"
						  )
{
	SEND_EQUIP_ITEM	theEquipItem;
	theEquipItem.theHeader.wPacketLen	= 0;
	theEquipItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theEquipItem.theHeader.wUrlType		= GetHash(L"cebca38adfe1a51de79f579873e41133"/*"Apple::Protocol::ToServer::InventorySystem::EquipItem"*/);	 // 行为
	theEquipItem.wEquipTabLen			= wcslen(pwszEquipTab)+1;
	theEquipItem.strEquipTab			= pwszEquipTab;
	theEquipItem.wEquipSlot				= nEquipSlot;
	theEquipItem.wInventoryTabLen		= wcslen(pwszInvenTab)+1;
	theEquipItem.strInventoryTab		= pwszInvenTab;
	theEquipItem.wInvenSlot				= nInvenSlot;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theEquipItem.theHeader.wPacketLen;
	thePacket << theEquipItem.theHeader.wMethodType;
	thePacket << theEquipItem.theHeader.wUrlType;
	thePacket << theEquipItem.strEquipTab;
	thePacket << theEquipItem.wEquipSlot;
	thePacket << theEquipItem.strInventoryTab;
	thePacket << theEquipItem.wInvenSlot;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 脱掉装备
int WINAPI JFZR_UnEquipItem(
						  OUT char*	pOutszSendBuffer,
						  int nEquipSlot,					// 装备槽
						  int nInvenSlot,					// 背包槽
						  const wchar_t* pwszEquipTab,		// L"BASIC"
						  const wchar_t* pwszInvenTab		// L"EQUIPMENT_TAB"
						  )
{
	SEND_UN_EQUIP_ITEM	theUnequipItem;
	theUnequipItem.theHeader.wPacketLen		= 0;
	theUnequipItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theUnequipItem.theHeader.wUrlType		= GetHash(L"d5b24ed93d2a59535d3291aca7173c8e"/*"Apple::Protocol::ToServer::InventorySystem::UnequipItem"*/);	 // 行为
	theUnequipItem.wEquipTabLen				= wcslen(pwszEquipTab)+1;
	theUnequipItem.strEquipTab				= pwszEquipTab;
	theUnequipItem.wEquipSlot				= nEquipSlot;
	theUnequipItem.wInventoryTabLen			= wcslen(pwszInvenTab)+1;
	theUnequipItem.strInventoryTab			= pwszInvenTab;
	theUnequipItem.wInvenSlot				= nInvenSlot;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theUnequipItem.theHeader.wPacketLen;
	thePacket << theUnequipItem.theHeader.wMethodType;
	thePacket << theUnequipItem.theHeader.wUrlType;
	thePacket << theUnequipItem.strEquipTab;
	thePacket << theUnequipItem.wEquipSlot;
	thePacket << theUnequipItem.strInventoryTab;
	thePacket << theUnequipItem.wInvenSlot;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 鉴定和解除封印装备
int WINAPI JFZR_UseForItem(
							OUT char*	pOutszSendBuffer,
							int nUsingSlotIndex,					// 鉴定书或解封石位置
							int nTargetSlotIndex					// 装备位置
							)
{
	SEND_USE_FOR_ITEM	theUseForItem;
	theUseForItem.theHeader.wPacketLen		= 0;
	theUseForItem.theHeader.wMethodType		= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theUseForItem.theHeader.wUrlType		= GetHash(L"fcf22fe368eaea7f7a2433ab1fea257a"/*"Apple::Protocol::ToServer::InventorySystem::UseForItem"*/);	 // 行为
	theUseForItem.dwUsingTabIndex			= 1;
	theUseForItem.wUsingSlotIndex			= (WORD)nUsingSlotIndex;		// UsingSlotIndex
	theUseForItem.dwTargetTabIndex			= 0;
	theUseForItem.wTargetSlotIndex			= (WORD)nTargetSlotIndex;	// TargetSlotIndex

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theUseForItem.theHeader.wPacketLen;
	thePacket << theUseForItem.theHeader.wMethodType;
	thePacket << theUseForItem.theHeader.wUrlType;
	thePacket << theUseForItem.dwUsingTabIndex;
	thePacket << theUseForItem.wUsingSlotIndex;
	thePacket << theUseForItem.dwTargetTabIndex;
	thePacket << theUseForItem.wTargetSlotIndex;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：购买物品
int WINAPI JFZR_BuyItem(
						   OUT char*	pOutszSendBuffer,
						   const wchar_t*		pwszNpcName,// NPC名称
						   DWORD		dwSellId,			// 物品ID
						   int			nCount				// 物品数量
						   )
{
	SEND_BUY_ITEM	theBuyItem;
	theBuyItem.theHeader.wPacketLen		= 0;
	theBuyItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theBuyItem.theHeader.wUrlType		= GetHash(L"35bf1b03b248e6642008740c7cca0f01"/*"Apple::Protocol::ToServer::ShopSystem::BuyItem"*/);	 // 行为
	theBuyItem.dwFixedNpcHash			= GetSigHash(pwszNpcName);		// FixedNpcHash
	theBuyItem.wSellId					= (WORD)dwSellId;		// SellId
	theBuyItem.wCount					= (WORD)nCount;			// Count

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theBuyItem.theHeader.wPacketLen;
	thePacket << theBuyItem.theHeader.wMethodType;
	thePacket << theBuyItem.theHeader.wUrlType;
	thePacket << theBuyItem.dwFixedNpcHash;
	thePacket << theBuyItem.wSellId;
	thePacket << theBuyItem.wCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


int WINAPI JFZR_SellItem(
						OUT char*	pOutszSendBuffer,
						const wchar_t*	pwszNpcName,	// NPC名称
						int			nTabCategory,		// 0,表示普通栏; 1,表示消耗品栏;2,表示任务栏;3,表示材料栏
						int			nSlotIndex,			// 单个对象背包栏第几格
						int			nCount				// 物品数量
						)
{
	SEND_SELL_ITEM	theSellItem;
	theSellItem.theHeader.wPacketLen		= 0;
	theSellItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theSellItem.theHeader.wUrlType		= GetHash(L"2f6401c12fc6a415ea86fada78c3617c"/*"Apple::Protocol::ToServer::ShopSystem::SellItem"*/);	 // 行为
	theSellItem.dwFixedNpcHash			= GetSigHash(pwszNpcName);		// FixedNpcHash
	theSellItem.dwTabCategory			= nTabCategory;			// TabCategory
	theSellItem.dwSlotIndex				= nSlotIndex;			// SlotIndex
	theSellItem.dwCount					= nCount;				// Count

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theSellItem.theHeader.wPacketLen;
	thePacket << theSellItem.theHeader.wMethodType;
	thePacket << theSellItem.theHeader.wUrlType;
	thePacket << theSellItem.dwFixedNpcHash;
	thePacket << theSellItem.dwTabCategory;
	thePacket << theSellItem.dwSlotIndex;
	thePacket << theSellItem.dwCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

int WINAPI JFZR_ChangeVillage(
								OUT char*	pOutszSendBuffer,
								const wchar_t*	pwszVillageId			// 村庄门id
								)
{
	SEND_CHANGE_VILLGAE	theChangeVillage;
	theChangeVillage.theHeader.wPacketLen	= 0;
	theChangeVillage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theChangeVillage.theHeader.wUrlType		= GetHash(L"8700c48e9de9aa244ec92d34c498fe78"/*"Apple::Protocol::ToServer::StageSystem::ChangeVillage"*/);	 // 行为
	theChangeVillage.wWarpGateIdLen			= wcslen(pwszVillageId)+1;		// WarpGateId
	theChangeVillage.strWarpGateId			= pwszVillageId;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theChangeVillage.theHeader.wPacketLen;
	thePacket << theChangeVillage.theHeader.wMethodType;
	thePacket << theChangeVillage.theHeader.wUrlType;
	thePacket << theChangeVillage.strWarpGateId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
};


// 升级技能
int WINAPI JFZR_LearnSkill(
							  OUT char*	pOutszSendBuffer,
							  const wchar_t*	pwszSkillName,	// 技能名称
							  DWORD		dwSkillLevel			// 升级后的技能等级
							  )
{
	SEND_LEARN_SKILL_MULTIPLE	theLearnSkillMultiple;
	theLearnSkillMultiple.theHeader.wPacketLen	= 0;
	theLearnSkillMultiple.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theLearnSkillMultiple.theHeader.wUrlType	= GetHash(L"a31bfe5025bd74f8cfd945c56fb74251"/*"Apple::Protocol::ToServer::ShopSystem::LearnSkillMultiple"*/);	 // 行为
	theLearnSkillMultiple.dwSkillSet				= 0;		// 11.30新增
	theLearnSkillMultiple.wRequestLearnSkillList	= GetHash(L"RequestLearnSkillList");			// RequestLearnSkillList
	theLearnSkillMultiple.warray					= GetHash(L"array");			// array
	theLearnSkillMultiple.dwsize					= 1;
	theLearnSkillMultiple.Item[0].witem_i			= GetHash(L"item_0");		// item_0
	theLearnSkillMultiple.Item[0].wpair				= GetHash(L"pair");			// pair
	theLearnSkillMultiple.Item[0].dwkey				= GetSigHash(pwszSkillName);		// key
	theLearnSkillMultiple.Item[0].dwvalue			= dwSkillLevel;						// value

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theLearnSkillMultiple.theHeader.wPacketLen;
	thePacket << theLearnSkillMultiple.theHeader.wMethodType;
	thePacket << theLearnSkillMultiple.theHeader.wUrlType;
	thePacket << theLearnSkillMultiple.dwSkillSet;			// 11.30新增
	thePacket << theLearnSkillMultiple.wRequestLearnSkillList;
	thePacket << theLearnSkillMultiple.warray;
	thePacket << theLearnSkillMultiple.dwsize;
	thePacket << theLearnSkillMultiple.Item[0].witem_i;
	thePacket << theLearnSkillMultiple.Item[0].wpair;
	thePacket << theLearnSkillMultiple.Item[0].dwkey;
	thePacket << theLearnSkillMultiple.Item[0].dwvalue;
	
	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 创建角色
int WINAPI JFZR_CreatePlayer(
							 OUT char*	pOutszSendBuffer,
							 const wchar_t*	pszRoleName,		// 创建角色的名称
							 DWORD dwPlayerGender,				// 角色性别，默认第一个都是0，此参数一直默认为0
							 DWORD dwClassType					// 职业类型 // 1.剑斗士  2.刃武者  9.控魔师
							 )
{
	int nOffset = 2;
	*(WORD*)(pOutszSendBuffer+nOffset) = JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"cc4c3e9a5e0b08ccae365cba61f5e5c3"/*"Apple::Protocol::ToServer::PlayerManagementSystem::CreatePlayer"*/);	 // 行为
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(pszRoleName)+1;			// PlayerId
	nOffset+=2;
	memcpy(pOutszSendBuffer+nOffset,pszRoleName,(wcslen(pszRoleName)+1)*2);
	nOffset+=(wcslen(pszRoleName)+1)*2;
	*(DWORD*)(pOutszSendBuffer+nOffset) = dwPlayerGender;			// PlayerGender
	nOffset+=4;
	*(DWORD*)(pOutszSendBuffer+nOffset) = dwClassType;				// ClassType
	nOffset+=4;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"CustomizingData");
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData");
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"CustomizingPartDatas");
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"array");
	nOffset+=2;
	*(DWORD*)(pOutszSendBuffer+nOffset) = 7;		// size
	nOffset+=4;

	{
		wchar_t szBodyTemp[] = L"HEAD";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_0");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0xFF;		// SelectedPart2ColorIndex
		nOffset+=1;
	}
	
	{
		wchar_t szBodyTemp[] = L"UPPER";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_1");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart2ColorIndex
		nOffset+=1;
	}
	

	{
		wchar_t szBodyTemp[] = L"LOWER";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_2");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x01;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedPart2ColorIndex
		nOffset+=1;
	}
	

	{
		wchar_t szBodyTemp[] = L"HAND";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_3");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart2ColorIndex
		nOffset+=1;

	}
	
	{
		wchar_t szBodyTemp[] = L"FOOT";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_4");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x09;		// SelectedPart2ColorIndex
		nOffset+=1;
	}
	
	{
		wchar_t szBodyTemp[] = L"FACE";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_5");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0xFF;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0xFF;		// SelectedPart2ColorIndex	
		nOffset+=1;
	}
	
	{
		wchar_t szBodyTemp[] = L"EYE";
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"item_6");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"Apple::CustomizingData::CustomizingPartData");
		nOffset+=2;
		*(WORD*)(pOutszSendBuffer+nOffset) = wcslen(szBodyTemp)+1;
		nOffset+=2;
		memcpy(pOutszSendBuffer+nOffset,szBodyTemp,(wcslen(szBodyTemp)+1)*2);
		nOffset+=(wcslen(szBodyTemp)+1)*2;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedMeshInfoIndex
		nOffset+=1;
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0x00;		// SelectedPart1ColorIndex
		nOffset+=1;	
		*(BYTE*)(pOutszSendBuffer+nOffset) = 0xFF;		// SelectedPart2ColorIndex	
		nOffset+=1;
	}
	

	*(DWORD*)(pOutszSendBuffer+nOffset) = 2;		// SelectedSkinColorIndex
	nOffset+=4;

	return nOffset;
}


// 选择角色
int WINAPI JFZR_SelectPlayer(
						   OUT char*		pOutszSendBuffer,
						   const wchar_t*	pszRoleName				// 角色名称
						   )
{
	SEND_SELECT_PLAYER theSelectPlayer;
	theSelectPlayer.theHeader.wPacketLen	= 0;
	theSelectPlayer.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theSelectPlayer.theHeader.wUrlType		= GetHash(L"5cefcbe0615c112023bbcca2d1cbd8bc"/*"Apple::Protocol::ToServer::SessionSystem::SelectPlayer"*/);	 // 行为
	theSelectPlayer.wPlayerIdLen			= wcslen(pszRoleName)+1;
	theSelectPlayer.strPlayerId				= pszRoleName;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theSelectPlayer.theHeader.wPacketLen;
	thePacket << theSelectPlayer.theHeader.wMethodType;
	thePacket << theSelectPlayer.theHeader.wUrlType;
	thePacket << theSelectPlayer.strPlayerId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 小退，状态问题，无法实现小退
int WINAPI JFZR_DeselectPlayer(
							 OUT char*		pOutszSendBuffer
							 )
{
	int nOffset = 2;
	*(WORD*)(pOutszSendBuffer+nOffset) = JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"18893465a7097731f08e888b0fb54aae"/*"Apple::Protocol::ToServer::SessionSystem::DeselectPlayer"*/);	 // 行为
	nOffset+=2;
	return nOffset;
}

// 配合小退用到的获取角色链表
int WINAPI JFZR_ListPlayers(
							   OUT char*		pOutszSendBuffer
							   )
{
	int nOffset = 2;
	*(WORD*)(pOutszSendBuffer+nOffset) = JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	nOffset+=2;
	*(WORD*)(pOutszSendBuffer+nOffset) = GetHash(L"924d70933cc6bf77baa3b95bf2e487f6"/*"Apple::Protocol::ToServer::SessionSystem::ListPlayers"*/);	 // 行为
	nOffset+=2;
	return nOffset;
}

// 选择频道
int WINAPI JFZR_RequestSwitchChannel(
							 OUT char*		pOutszSendBuffer,
							 const wchar_t*	pszChannelName				// 频道名称
							 )
{
	SEND_REQUEST_SWITCH_CHANNEL	theRequestSwitchChannel;
	theRequestSwitchChannel.theHeader.wPacketLen	= 0;
	theRequestSwitchChannel.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theRequestSwitchChannel.theHeader.wUrlType		= GetHash(L"8bedfbed6e9106a8a208e391c60cfeb3"/*"Apple::Protocol::ToServer::SessionSystem::RequestSwitchChannel"*/);	 // 行为
	theRequestSwitchChannel.wChannelIdLen			= wcslen(pszChannelName)+1;
	theRequestSwitchChannel.strChannelId			= pszChannelName;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRequestSwitchChannel.theHeader.wPacketLen;
	thePacket << theRequestSwitchChannel.theHeader.wMethodType;
	thePacket << theRequestSwitchChannel.theHeader.wUrlType;
	thePacket << theRequestSwitchChannel.strChannelId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

int WINAPI JFZR_SwitchChannel(
									 OUT char*		pOutszSendBuffer,
									 const wchar_t*	pszChannelName,				// 频道名称
									 const wchar_t*	pszAccountId,				// QQ账号
									 const wchar_t*	pszSessionKey				// KEY
									 )
{
	SEND_SWITCH_CHANNEL	theSwitchChannel;
	theSwitchChannel.theHeader.wPacketLen	= 0;
	theSwitchChannel.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theSwitchChannel.theHeader.wUrlType		= GetHash(L"827d94dc425544c8bf32122e7d3f0f84"/*"Apple::Protocol::ToServer::SessionSystem::SwitchChannel"*/);	 // 行为
	theSwitchChannel.wChannelIdLen			= wcslen(pszChannelName)+1;
	theSwitchChannel.strChannelId			= pszChannelName;
	theSwitchChannel.wAccountIdLen			= wcslen(pszAccountId)+1;
	theSwitchChannel.strAccountId			= pszAccountId;
	theSwitchChannel.wSessionKeyLen			= wcslen(pszSessionKey)+1;
	theSwitchChannel.strSessionKey			= pszSessionKey;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theSwitchChannel.theHeader.wPacketLen;
	thePacket << theSwitchChannel.theHeader.wMethodType;
	thePacket << theSwitchChannel.theHeader.wUrlType;
	thePacket << theSwitchChannel.strChannelId;
	thePacket << theSwitchChannel.strAccountId;
	thePacket << theSwitchChannel.strSessionKey;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：发送邮件包头
int WINAPI JFZR_MailSendHeader(
									 OUT char*			pOutszSendBuffer,
									 const wchar_t*		pszTitle,				// 邮件标题
									 const wchar_t*		pszReceiverId,			// 玩家名称
									 const wchar_t*		pszMessage,				// 邮件内容
									 UINT64				nGold,					// 邮件金币
									 int				nsize,					// 邮件物品个数
									 LONGLONG			llOidReceiver			// 本地存储邮寄目标名称的内存地址
									 )
{
	llOidReceiver <<= 32;		// 此内存地址为64位，在封包存储于高位，所以需要左移32位

	SEND_MAIL_SEND_HEADER	theMailSend;
	theMailSend.theHeader.wPacketLen	= 0;
	theMailSend.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theMailSend.theHeader.wUrlType		= GetHash(L"5fc62cc2d7a859c3e039eb81b3235d3c"/*"Apple::Protocol::ToServer::MailSystem::MailSend"*/);	 // 行为
	theMailSend.dwType					= 0;	 // Type
	theMailSend.dwSystemMailType		= 0;	 // SystemMailType
	theMailSend.wTitleLen				= wcslen(pszTitle)+1;
	theMailSend.strTitle				= pszTitle;
	theMailSend.llOidReceiver			= llOidReceiver;		// OidReceiver
	theMailSend.wReceiverIdLen			= wcslen(pszReceiverId)+1;
	theMailSend.strReceiverId			= pszReceiverId;
	theMailSend.wMessageLen				= wcslen(pszMessage)+1;
	theMailSend.strMessage				= pszMessage;
	theMailSend.llGold					= nGold;
	theMailSend.dwOidMailItemList		= 1;		// OidMailItemList
	theMailSend.wItems					= GetHash(L"Items");		// Items
	theMailSend.warray					= GetHash(L"array");		// array
	theMailSend.dwsize					= nsize;		// size

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theMailSend.theHeader.wPacketLen;
	thePacket << theMailSend.theHeader.wMethodType;
	thePacket << theMailSend.theHeader.wUrlType;
	thePacket << theMailSend.dwType;
	thePacket << theMailSend.dwSystemMailType;
	thePacket << theMailSend.strTitle;
	thePacket << theMailSend.llOidReceiver;
	thePacket << theMailSend.strReceiverId;
	thePacket << theMailSend.strMessage;
	thePacket << theMailSend.llGold;
	thePacket << theMailSend.dwOidMailItemList;
	thePacket << theMailSend.wItems;
	thePacket << theMailSend.warray;
	thePacket << theMailSend.dwsize;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：发送邮件物品，没有物品，就不需要调用
int WINAPI JFZR_MailSendBody(
							   OUT char*			pOutszSendBuffer,
							   int					nitem,				// 第n个物品
							   wchar_t*				pwszInventoryTab,	// 装备:"EQUIPMENT_TAB" 材料:"MATERIAL_TAB" 消耗品:"CONSUME_TAB"
							   int					nItemSlotIndex,		// 物品所在背包槽
							   int					nCount				// 物品数量
							)
{
	wchar_t szItem[64] = {0};
	wsprintfW(szItem,L"item_%d",nitem);

	SEND_MAIL_SEND_BODY	theMailSendBody;
	theMailSendBody.witem_i					= GetHash(szItem);				// item_0
	theMailSendBody.wApple_SendingMailItem	= GetHash(L"Apple::SendingMailItem");				// Apple::SendingMailItem
	theMailSendBody.wInventoryTabLen		= wcslen(pwszInventoryTab)+1;
	theMailSendBody.strInventoryTab			= pwszInventoryTab;
	theMailSendBody.dwItemSlotIndex			= nItemSlotIndex;		// ItemSlotIndex
	theMailSendBody.dwCount					= nCount;				// Count

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theMailSendBody.witem_i;
	thePacket << theMailSendBody.wApple_SendingMailItem;
	thePacket << theMailSendBody.strInventoryTab;
	thePacket << theMailSendBody.dwItemSlotIndex;
	thePacket << theMailSendBody.dwCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：发送邮件尾部
int WINAPI JFZR_MailSendTail(
							 OUT char*			pOutszSendBuffer
							 )
{
	SEND_MAIL_SEND_TAIL	theMailSendTail;
	theMailSendTail.wExpireDate		= GetHash(L"ExpireDate");				// ExpireDate
	theMailSendTail.theAppleDateTime.wApple_DateTime	= GetHash(L"Apple::DateTime");				// Apple::DateTime
	theMailSendTail.theAppleDateTime.wYear	= 0x578;
	theMailSendTail.theAppleDateTime.wMonth	= 1;
	theMailSendTail.theAppleDateTime.wDay	= 1;
	theMailSendTail.theAppleDateTime.dwHour	= 0;
	theMailSendTail.theAppleDateTime.dwMinute	= 0;
	theMailSendTail.theAppleDateTime.dwSecond	= 0;
	theMailSendTail.theAppleDateTime.dwMilliSec	= 0;
	theMailSendTail.theAppleDateTime.IsInfinity	= 0;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theMailSendTail.wExpireDate;
	thePacket << theMailSendTail.theAppleDateTime.wApple_DateTime;
	thePacket << theMailSendTail.theAppleDateTime.wYear;
	thePacket << theMailSendTail.theAppleDateTime.wMonth;
	thePacket << theMailSendTail.theAppleDateTime.wDay;
	thePacket << theMailSendTail.theAppleDateTime.dwHour;
	thePacket << theMailSendTail.theAppleDateTime.dwMinute;
	thePacket << theMailSendTail.theAppleDateTime.dwSecond;
	thePacket << theMailSendTail.theAppleDateTime.dwMilliSec;
	thePacket << theMailSendTail.theAppleDateTime.IsInfinity;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：领取附件
int WINAPI JFZR_GetAttachments(
							 OUT char*			pOutszSendBuffer,
								 LONGLONG		llMailSerial,
								 BYTE			bIsFirst
							 )
{
	SEND_GETATTACHMEMTS	theGetAttachments;
	theGetAttachments.theHeader.wPacketLen	= 0;
	theGetAttachments.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theGetAttachments.theHeader.wUrlType	= GetHash(L"f8260de7249e4d60ab7ee464e5f499e1"/*"Apple::Protocol::ToServer::MailSystem::GetAttachments"*/);	 // 行为
	theGetAttachments.llMailSerial			= llMailSerial;
	theGetAttachments.bIsFirst				= bIsFirst;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theGetAttachments.theHeader.wPacketLen;
	thePacket << theGetAttachments.theHeader.wMethodType;
	thePacket << theGetAttachments.theHeader.wUrlType;
	thePacket << theGetAttachments.llMailSerial;
	thePacket << theGetAttachments.bIsFirst;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

int WINAPI JFZR_MailRemove(
							   OUT char*			pOutszSendBuffer,
							   LONGLONG		llMailSerial,
							   BYTE			bIsFirst
							   )
{
	SEND_MAIL_REMOVE	theMailRemove;
	theMailRemove.theHeader.wPacketLen	= 0;
	theMailRemove.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theMailRemove.theHeader.wUrlType	= GetHash(L"246578b8220355a2c542e01995fe3eff"/*"Apple::Protocol::ToServer::MailSystem::MailRemove"*/);	 // 行为
	theMailRemove.llMailSerial			= llMailSerial;
	theMailRemove.bIsFirst				= bIsFirst;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theMailRemove.theHeader.wPacketLen;
	thePacket << theMailRemove.theHeader.wMethodType;
	thePacket << theMailRemove.theHeader.wUrlType;
	thePacket << theMailRemove.llMailSerial;
	thePacket << theMailRemove.bIsFirst;
	
	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：跳过首次进入游戏动画
int WINAPI JFZR_JoinVillage(
							 OUT char*			pOutszSendBuffer
							 )
{
	SEND_JOIN_VILLAGE	theJoinVillage;
	theJoinVillage.theHeader.wPacketLen	= 0;
	theJoinVillage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theJoinVillage.theHeader.wUrlType	= GetHash(L"e56a7bfbcf0df1471b9c59d6b0444703"/*"Apple::Protocol::ToServer::StageSystem::JoinVillage"*/);	 // 行为
	theJoinVillage.bIgnoreFistPlay		= 0;						// IgnoreFistPlay
	theJoinVillage.bSwitchChannelInVillage	= 0;					// SwitchChannelInVillage

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theJoinVillage.theHeader.wPacketLen;
	thePacket << theJoinVillage.theHeader.wMethodType;
	thePacket << theJoinVillage.theHeader.wUrlType;
	thePacket << theJoinVillage.bIgnoreFistPlay;
	thePacket << theJoinVillage.bSwitchChannelInVillage;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：接收任务
int WINAPI JFZR_Accept(
							OUT		char*		pOutszSendBuffer,
							const	wchar_t*	pszQuestId,					// 任务id
							DWORD				dwRewardItemId,				// 1
							DWORD				dwInventoryTabIndex,		// -1
							DWORD				dwInventorySlotIndex		// -1
							)
{
	SEND_ACCEPT	theAccept;
	theAccept.theHeader.wPacketLen	= 0;
	theAccept.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theAccept.theHeader.wUrlType	= GetHash(L"6c32a7184cafa0cfd343ecc1a6ea3a8d"/*"Apple::Protocol::ToServer::QuestSystem::Accept"*/);	 // 行为
	theAccept.wQuestIdLen			= wcslen(pszQuestId)+1;
	theAccept.strQuestId			= pszQuestId;
	theAccept.dwRewardItemId		= dwRewardItemId;
	theAccept.dwInventoryTabIndex	= dwInventoryTabIndex;
	theAccept.dwInventorySlotIndex	= dwInventorySlotIndex;
	
	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theAccept.theHeader.wPacketLen;
	thePacket << theAccept.theHeader.wMethodType;
	thePacket << theAccept.theHeader.wUrlType;
	thePacket << theAccept.strQuestId;
	thePacket << theAccept.dwRewardItemId;
	thePacket << theAccept.dwInventoryTabIndex;
	thePacket << theAccept.dwInventorySlotIndex;
	
	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：完成任务
int WINAPI JFZR_Complete(
					   OUT		char*		pOutszSendBuffer,
					   const	wchar_t*	pszQuestId,					// 任务id
					   const	wchar_t*	pszRewardItemId
					   )
{
	SEND_COMPLETE	theComplete;
	theComplete.theHeader.wPacketLen	= 0;
	theComplete.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theComplete.theHeader.wUrlType		= GetHash(L"7824a9c8b4fc5eff77a1a20c43fe6135"/*"Apple::Protocol::ToServer::QuestSystem::Complete"*/);	 // 行为
	theComplete.wQuestIdLen				= wcslen(pszQuestId)+1;
	theComplete.strQuestId				= pszQuestId;
	theComplete.wRewardItemIdLen		= wcslen(pszRewardItemId)+1;
	theComplete.strRewardItemId			= pszRewardItemId;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theComplete.theHeader.wPacketLen;
	thePacket << theComplete.theHeader.wMethodType;
	thePacket << theComplete.theHeader.wUrlType;
	thePacket << theComplete.strQuestId;
	thePacket << theComplete.strRewardItemId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


// 函数功能：放弃任务
int WINAPI JFZR_GiveUp(
						 OUT		char*		pOutszSendBuffer,
						 const		wchar_t*	pszQuestId					// 任务id
						 )
{
	SEND_GIVEUP	theGiveUp;
	theGiveUp.theHeader.wPacketLen		= 0;
	theGiveUp.theHeader.wMethodType		= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theGiveUp.theHeader.wUrlType		= GetHash(L"bf2dd7eca4dc783cbc28d1f3004f468f"/*"Apple::Protocol::ToServer::QuestSystem::GiveUp"*/);	 // 行为
	theGiveUp.wQuestIdLen				= wcslen(pszQuestId)+1;
	theGiveUp.strQuestId				= pszQuestId;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theGiveUp.theHeader.wPacketLen;
	thePacket << theGiveUp.theHeader.wMethodType;
	thePacket << theGiveUp.theHeader.wUrlType;
	thePacket << theGiveUp.strQuestId;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：任务快速移动
int WINAPI JFZR_TeleportToVillage(
						 OUT		char*		pOutszSendBuffer,
						 const		wchar_t*	pwszQuestId,
						 DWORD		dwState
						 )
{
	SEND_TELEPORT_TO_VILLAGE	theTeleportToVillage;
	theTeleportToVillage.theHeader.wPacketLen	= 0;
	theTeleportToVillage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theTeleportToVillage.theHeader.wUrlType		= GetHash(L"92ad2a60a8f9da49132538defa4acd13"/*"Apple::Protocol::ToServer::QuestSystem::TeleportToVillage"*/);	 // 行为
	theTeleportToVillage.wQuestIdLen			= wcslen(pwszQuestId)+1;
	theTeleportToVillage.strQuestId				= pwszQuestId;
	theTeleportToVillage.dwState				= dwState;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theTeleportToVillage.theHeader.wPacketLen;
	thePacket << theTeleportToVillage.theHeader.wMethodType;
	thePacket << theTeleportToVillage.theHeader.wUrlType;
	thePacket << theTeleportToVillage.strQuestId;
	thePacket << theTeleportToVillage.dwState;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：分解物品
int WINAPI JFZR_Disassembly(
							  OUT		char*		pOutszSendBuffer,
							  DWORD		dwTabCategory,		// 普通背包 0
							  DWORD		dwSlotIndex,		// 背包内第几格
							  WORD		wIsAvatarItem		// 0，未知
							  )
{
	SEND_DISASSEMBLY	theDisassembly;
	theDisassembly.theHeader.wPacketLen		= 0;
	theDisassembly.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theDisassembly.theHeader.wUrlType		= GetHash(L"920f9bdd034c400bd59dc8c770e73d5a"/*"Apple::Protocol::ToServer::ItemTransformSystem::Disassembly"*/);	 // 行为
	theDisassembly.dwTabCategory			= dwTabCategory;
	theDisassembly.dwSlotIndex				= dwSlotIndex;
	theDisassembly.wIsAvatarItem			= wIsAvatarItem;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theDisassembly.theHeader.wPacketLen;
	thePacket << theDisassembly.theHeader.wMethodType;
	thePacket << theDisassembly.theHeader.wUrlType;
	thePacket << theDisassembly.dwTabCategory;
	thePacket << theDisassembly.dwSlotIndex;
	thePacket << theDisassembly.wIsAvatarItem;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：副本BOOS房间奖励物品
int WINAPI JFZR_StageReward(
							OUT		char*		pOutszSendBuffer,
							DWORD	dwRewardIndex
							)
{
	SEND_STAGE_REWARD	theStageReward;
	theStageReward.theHeader.wPacketLen		= 0;
	theStageReward.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theStageReward.theHeader.wUrlType		= GetHash(L"a8d7b003267a116610630b3c583ee7b3"/*"Apple::Protocol::ToServer::StageSystem::StageReward"*/);	 // 行为
	theStageReward.dwRewardIndex			= dwRewardIndex;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theStageReward.theHeader.wPacketLen;
	thePacket << theStageReward.theHeader.wMethodType;
	thePacket << theStageReward.theHeader.wUrlType;
	thePacket << theStageReward.dwRewardIndex;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

int WINAPI JFZR_RequestProductList(
							OUT		char*		pOutszSendBuffer,
							wchar_t*	pwszCategory,
							wchar_t*	pwszKeyWord,
							DWORD		dwPageNum
							)
{
	SEND_REQUEST_PRODUCT_LIST	theRequestProductList;
	theRequestProductList.theHeader.wPacketLen	= 0;
	theRequestProductList.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theRequestProductList.theHeader.wUrlType	= GetHash(L"f93c6af2752f096fd94bde35d0422520"/*"Apple::Protocol::ToServer::MarketSystem::RequestProductList"*/);	 // 行为
	theRequestProductList.dwOnePageProductCount	= 10;		// OnePageProductCount
	theRequestProductList.dwPageNum				= dwPageNum;		// PageNum
	theRequestProductList.wCategoryLen			= wcslen(pwszCategory)+1;
	theRequestProductList.strCategory			= pwszCategory;
	theRequestProductList.dwGrade				= 0x1F;		// Grade		
	theRequestProductList.bIsUseable			= 0;		// IsUseable
	theRequestProductList.dwMinLv				= 0;		// MinLv
	theRequestProductList.dwMaxLv				= 0x3C;		// MaxLv	10.30更新
	theRequestProductList.dwMinEnhantLevel		= 0;		// MinEnhantLevel
	theRequestProductList.dwMaxEnhantLevel		= 0x27;		// MaxEnhantLevel	10.30更新
	theRequestProductList.wKeyWordLen			= wcslen(pwszKeyWord)+1;
	theRequestProductList.strKeyWord			= pwszKeyWord;
	theRequestProductList.dwOrderType			= 0;		// OrderType
	theRequestProductList.wOptionList			= GetHash(L"OptionList");		// OptionList
	theRequestProductList.wassociative_array	= GetHash(L"associative_array");		// associative_array
	theRequestProductList.dwsize				= 0;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRequestProductList.theHeader.wPacketLen;
	thePacket << theRequestProductList.theHeader.wMethodType;
	thePacket << theRequestProductList.theHeader.wUrlType;
	thePacket << theRequestProductList.dwOnePageProductCount;
	thePacket << theRequestProductList.dwPageNum;
	thePacket << theRequestProductList.strCategory;
	thePacket << theRequestProductList.dwGrade;
	thePacket << theRequestProductList.bIsUseable;
	thePacket << theRequestProductList.dwMinLv;
	thePacket << theRequestProductList.dwMaxLv;
	thePacket << theRequestProductList.dwMinEnhantLevel;
	thePacket << theRequestProductList.dwMaxEnhantLevel;
	thePacket << theRequestProductList.strKeyWord;
	thePacket << theRequestProductList.dwOrderType;
	thePacket << theRequestProductList.wOptionList;
	thePacket << theRequestProductList.wassociative_array;
	thePacket << theRequestProductList.dwsize;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：拍卖购物物品
int WINAPI JFZR_BuyProduct(
								   OUT		char*		pOutszSendBuffer,
								   LONGLONG		llOidMarket,
								   DWORD		dwCount
								   )
{
	SEND_BUY_PRODUCT	theBuyProduct;
	theBuyProduct.theHeader.wPacketLen	= 0;
	theBuyProduct.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theBuyProduct.theHeader.wUrlType	= GetHash(L"9a6e248c23b03341309edf0af3d02c93"/*"Apple::Protocol::ToServer::MarketSystem::BuyProduct"*/);	 // 行为
	theBuyProduct.llOidMarket			= llOidMarket;
	theBuyProduct.wCount				= (WORD)dwCount;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theBuyProduct.theHeader.wPacketLen;
	thePacket << theBuyProduct.theHeader.wMethodType;
	thePacket << theBuyProduct.theHeader.wUrlType;
	thePacket << theBuyProduct.llOidMarket;
	thePacket << theBuyProduct.wCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：挂物品到拍卖
int WINAPI JFZR_RegisterProduct(
						   OUT		char*		pOutszSendBuffer,
									wchar_t*	pwszName,
									DWORD		dwItemTabCategory,
									DWORD		dwItemSlotIndex,
									LONGLONG	llPrice,
									DWORD		dwPeriod,
									DWORD		dwCount
						   )
{
	SEND_REGISTER_PRODUCT	theRegisterProduct;
	theRegisterProduct.theHeader.wPacketLen		= 0;
	theRegisterProduct.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theRegisterProduct.theHeader.wUrlType		= GetHash(L"c9c08a120d4964bd79fe1ee858d6a665"/*"Apple::Protocol::ToServer::MarketSystem::RegisterProduct"*/);	 // 行为
	theRegisterProduct.wCategoryLen				= wcslen(pwszName)+1;
	theRegisterProduct.strCategory				= pwszName;
	theRegisterProduct.dwItemTabCategory		= dwItemTabCategory;
	theRegisterProduct.dwItemSlotIndex			= dwItemSlotIndex;
	theRegisterProduct.llPrice					= llPrice;
	theRegisterProduct.dwPeriod					= dwPeriod;
	theRegisterProduct.dwCount					= dwCount;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRegisterProduct.theHeader.wPacketLen;
	thePacket << theRegisterProduct.theHeader.wMethodType;
	thePacket << theRegisterProduct.theHeader.wUrlType;
	thePacket << theRegisterProduct.strCategory;
	thePacket << theRegisterProduct.dwItemTabCategory;
	thePacket << theRegisterProduct.dwItemSlotIndex;
	thePacket << theRegisterProduct.llPrice;
	thePacket << theRegisterProduct.dwPeriod;
	thePacket << theRegisterProduct.dwCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：获取我的拍卖物品数据
int WINAPI JFZR_RequestMyProductList(
						   OUT		char*		pOutszSendBuffer
						   )
{
	SEND_REQUEST_MY_PRODUCT_LIST	theRequestMyProductList;
	theRequestMyProductList.theHeader.wPacketLen	= 0;
	theRequestMyProductList.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theRequestMyProductList.theHeader.wUrlType		= GetHash(L"7fe1f4a833cc612bbef1a9e14aecdc2c"/*"Apple::Protocol::ToServer::MarketSystem::RequestMyProductList"*/);	 // 行为

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRequestMyProductList.theHeader.wPacketLen;
	thePacket << theRequestMyProductList.theHeader.wMethodType;
	thePacket << theRequestMyProductList.theHeader.wUrlType;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：取消我的拍卖物品
int WINAPI JFZR_UnregisterProduct(
									 OUT		char*		pOutszSendBuffer,
									 LONGLONG	llOidMarket
									 )
{
	SEND_UNREGISTER_PRODUCT	theUnregisterProduct;
	theUnregisterProduct.theHeader.wPacketLen	= 0;
	theUnregisterProduct.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;	 // 固定不变
	theUnregisterProduct.theHeader.wUrlType		= GetHash(L"56392c225f0a796f2184b88856b055fb"/*"Apple::Protocol::ToServer::MarketSystem::UnregisterProduct"*/);	 // 行为
	theUnregisterProduct.llOidMarket			= llOidMarket;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theUnregisterProduct.theHeader.wPacketLen;
	thePacket << theUnregisterProduct.theHeader.wMethodType;
	thePacket << theUnregisterProduct.theHeader.wUrlType;
	thePacket << theUnregisterProduct.llOidMarket;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：连击封包
int WINAPI JFZR_Activity(
								  OUT		char*		pOutszSendBuffer,
											DWORD		dwType,					// 连击的类型 = 0
											DWORD		dwCount,				// 连击的次数
											DWORD		dwDefenderSerial,		// 0
											DWORD		dwdwAttackerSerial		// 0
									 )
{
	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;

	SEND_ACTOR_SYSTEM_ACTIVITY	theActivity = {0};
	theActivity.theHeader.wPacketLen	= 0;
	theActivity.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theActivity.theHeader.wUrlType		= GetHash(L"1127a4bae67312f0d7a2fd8ffdf915dd"/*"Apple::Protocol::ToServer::ActorSystem::Activity"*/);	 // 行为
	theActivity.dwType			= dwType;
	theActivity.dwCount			= dwCount;
	theActivity.dwDefenderSerial	= dwDefenderSerial;
	theActivity.dwAttackerSerial	= dwdwAttackerSerial;
	
	thePacket << theActivity.theHeader.wPacketLen;
	thePacket << theActivity.theHeader.wMethodType;
	thePacket << theActivity.theHeader.wUrlType;
	thePacket << theActivity.dwType;
	thePacket << theActivity.dwCount;
	thePacket << theActivity.dwDefenderSerial;
	thePacket << theActivity.dwAttackerSerial;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：请求物品市场价格
int WINAPI JFZR_RequestProductInfo(
						 OUT		char*		pOutszSendBuffer,
						 DWORD		dwItemTabCategory,
						 DWORD		dwItemSlotIndex
						 )
{
	SEND_REQUEST_PRODUCT_INFO	theRequestProductInfo;

	theRequestProductInfo.theHeader.wPacketLen	= 0;
	theRequestProductInfo.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theRequestProductInfo.theHeader.wUrlType	= GetHash(L"71cc57ea3c00703152f84cc428ae97ef"/*"Apple::Protocol::ToServer::MarketSystem::RequestProductInfo"*/);	 // 行为
	theRequestProductInfo.dwItemTabCategory		= dwItemTabCategory;
	theRequestProductInfo.dwItemSlotIndex		= dwItemSlotIndex;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theRequestProductInfo.theHeader.wPacketLen;
	thePacket << theRequestProductInfo.theHeader.wMethodType;
	thePacket << theRequestProductInfo.theHeader.wUrlType;
	thePacket << theRequestProductInfo.dwItemTabCategory;
	thePacket << theRequestProductInfo.dwItemSlotIndex;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：打开箱子
int WINAPI JFZR_OpenItemPackage(
								   OUT		char*		pOutszSendBuffer,
											int			nItemPackageTabIndex,	// 物品栏
											WORD		wItemPackageSlotIndex,	// 物品在第几格
											int			nRequireItemTabIndex,	// 配合打开物品需要的物品  -1表示没有
											WORD		wRequireItemSlotIndex	// 物品在第几格，如果不存在物品，则置0
								   )
{
	SEND_OPEN_ITEM_PACKAGE	theOpenItemPackage;

	theOpenItemPackage.theHeader.wPacketLen		= 0;
	theOpenItemPackage.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theOpenItemPackage.theHeader.wUrlType		= GetHash(L"415c2a704e50d150c6a26919d9c32405"/*"Apple::Protocol::ToServer::MarketSystem::RequestProductInfo"*/);	 // 行为
	theOpenItemPackage.nItemPackageTabIndex		= nItemPackageTabIndex;
	theOpenItemPackage.wItemPackageSlotIndex	= wItemPackageSlotIndex;
	theOpenItemPackage.nRequireItemTabIndex		= nRequireItemTabIndex;
	theOpenItemPackage.wRequireItemSlotIndex	= wRequireItemSlotIndex;
	theOpenItemPackage.nCount					= 0;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theOpenItemPackage.theHeader.wPacketLen;
	thePacket << theOpenItemPackage.theHeader.wMethodType;
	thePacket << theOpenItemPackage.theHeader.wUrlType;
	thePacket << theOpenItemPackage.nItemPackageTabIndex;
	thePacket << theOpenItemPackage.wItemPackageSlotIndex;
	thePacket << theOpenItemPackage.nRequireItemTabIndex;
	thePacket << theOpenItemPackage.wRequireItemSlotIndex;
	thePacket << theOpenItemPackage.nCount;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：使用物品
int WINAPI JFZR_UseItem(
								OUT		char*		pOutszSendBuffer,
								int			nTabIndex,	// 物品栏
								WORD		wSlotIndex	// 物品在第几格
								)
{
	
	SEND_USE_ITEM	theUseItem;

	theUseItem.theHeader.wPacketLen		= 0;
	theUseItem.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theUseItem.theHeader.wUrlType		= GetHash(L"3ed486eb9fcc46121a77b379e97f3b61"/*"Apple::Protocol::ToServer::InventorySystem::UseItem"*/);	 // 行为
	theUseItem.nTabIndex				= nTabIndex;
	theUseItem.wSlotIndex				= wSlotIndex;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theUseItem.theHeader.wPacketLen;
	thePacket << theUseItem.theHeader.wMethodType;
	thePacket << theUseItem.theHeader.wUrlType;
	thePacket << theUseItem.nTabIndex;
	thePacket << theUseItem.wSlotIndex;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}



int WINAPI JFZR_CreateNpcByClient(
									OUT		char*			pOutszSendBuffer,
											DWORD			dwMasterSerial,
											DWORD			dwRequestSerial
						)
{

	SEND_CREATE_NPC_BY_CLIENT	theCreateNpcByClient;

	theCreateNpcByClient.theHeader.wPacketLen		= 0;
	theCreateNpcByClient.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theCreateNpcByClient.theHeader.wUrlType		= GetHash(L"e14a6a7ed25052d68ba65f06039b538a"/*"Apple::Protocol::ToServer::ActorSystem::CreateNpcByClient"*/);	 // 行为
	theCreateNpcByClient.dwMasterSerial			= dwMasterSerial;
	theCreateNpcByClient.dwRequestSerial		= dwRequestSerial;
	theCreateNpcByClient.wNpcIdLen = 9;
	theCreateNpcByClient.strNpcId = L"Troll_Ba";
	theCreateNpcByClient.thePosition.fx = 131.309998f;
	theCreateNpcByClient.thePosition.fz = 0.730000f;
	theCreateNpcByClient.thePosition.fy = 44.040001f;
	theCreateNpcByClient.theDirection.fx = -0.011025f;
	theCreateNpcByClient.theDirection.fz = 0.000000f;
	theCreateNpcByClient.theDirection.fy = 0.999939f;

	theCreateNpcByClient.wTeamLen = 4;
	theCreateNpcByClient.strTeam = L"NPC";
	theCreateNpcByClient.bIsControllable = 0;
	theCreateNpcByClient.bDropItemWhenDie = 0;
	theCreateNpcByClient.bGiveExpWhenDie = 0;
	theCreateNpcByClient.bIsStageObject = 0;
	theCreateNpcByClient.bUninfluencedByQuest = 0;
	theCreateNpcByClient.wDefaultActionIdLen = 0x0D;
	theCreateNpcByClient.strDefaultActionId = L"Troll_Atk03a";
	theCreateNpcByClient.dwNpcType = 0;

 	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theCreateNpcByClient.theHeader.wPacketLen;
	thePacket << theCreateNpcByClient.theHeader.wMethodType;
	thePacket << theCreateNpcByClient.theHeader.wUrlType;
	thePacket << theCreateNpcByClient.dwMasterSerial;
	thePacket << theCreateNpcByClient.dwRequestSerial;
	thePacket << theCreateNpcByClient.strNpcId;
	thePacket << theCreateNpcByClient.thePosition.fx;
	thePacket << theCreateNpcByClient.thePosition.fz;
	thePacket << theCreateNpcByClient.thePosition.fy;
	thePacket << theCreateNpcByClient.theDirection.fx;
	thePacket << theCreateNpcByClient.theDirection.fz;
	thePacket << theCreateNpcByClient.theDirection.fy;
	thePacket << theCreateNpcByClient.strTeam;
	thePacket << theCreateNpcByClient.bIsControllable;
	thePacket << theCreateNpcByClient.bDropItemWhenDie;
	thePacket << theCreateNpcByClient.bGiveExpWhenDie;
	thePacket << theCreateNpcByClient.bIsStageObject;
	thePacket << theCreateNpcByClient.bUninfluencedByQuest;
	thePacket << theCreateNpcByClient.strDefaultActionId;
	thePacket << theCreateNpcByClient.dwNpcType;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}

// 函数功能：销毁对象
int WINAPI JFZR_DestroyActors(
						OUT		char*		pOutszSendBuffer,
								DWORD		dwSerial
						)
{

	SEND_DESTROY_ACTORS	theDestroyActors;

	theDestroyActors.theHeader.wPacketLen		= 0;
	theDestroyActors.theHeader.wMethodType	= JFZR_REVERSAL_PACKET_SEND_HEADER;
	theDestroyActors.theHeader.wUrlType		= GetHash(L"99a17b89ddaa7cddb22110d75f526fa0"/*"Apple::Protocol::ToServer::StageSystem::DestroyActors"*/);	 // 行为
	theDestroyActors.wSerials				= GetHash(L"Serials");
	theDestroyActors.warray					= GetHash(L"array");
	theDestroyActors.dwsize					= 1;
	theDestroyActors.theSerial[0].dwSerial	= dwSerial;
	theDestroyActors.fMaxDt					= 0.300258f;
	theDestroyActors.fClientAbsoluteTime	= (FLOAT)GetTickCount();
	theDestroyActors.wSumOfDeltaHps			= GetHash(L"SumOfDeltaHps");
	theDestroyActors.wassociative_array		= GetHash(L"associative_array");
	theDestroyActors.dwassociativesize		= 1;
	theDestroyActors.theSumOfDeltaHps[0].dwitem_first	= dwSerial;
	theDestroyActors.theSumOfDeltaHps[0].dwitem_second	= 0;

	JFZRGroupPacket	thePacket;
	thePacket = (const BYTE*)pOutszSendBuffer;
	thePacket << theDestroyActors.theHeader.wPacketLen;
	thePacket << theDestroyActors.theHeader.wMethodType;
	thePacket << theDestroyActors.theHeader.wUrlType;
	thePacket << theDestroyActors.wSerials;
	thePacket << theDestroyActors.warray;
	thePacket << theDestroyActors.dwsize;
	thePacket << theDestroyActors.theSerial[0].dwSerial;
	thePacket << theDestroyActors.fMaxDt;
	thePacket << theDestroyActors.fClientAbsoluteTime;
	thePacket << theDestroyActors.wSumOfDeltaHps;
	thePacket << theDestroyActors.wassociative_array;
	thePacket << theDestroyActors.dwassociativesize;
	thePacket << theDestroyActors.theSumOfDeltaHps[0].dwitem_first;
	thePacket << theDestroyActors.theSumOfDeltaHps[0].dwitem_second;

	return thePacket.m_lpPacketCurrent - thePacket.m_lpPacketBegin;
}


//15 00 E0 55 1C 80 0B CC 3C 39 07 00 00 00 07 00 03 00 00 00 02 
int WINAPI JFZR_Test(
							OUT		char*		pOutszSendBuffer,
							DWORD	dwIndex
							)
{
	if( dwIndex == 1 )
	{
		unsigned char szData[] = {0x15 ,0x00 ,0xE0 ,0x55 ,0x1C ,0x80 ,0x0B ,0xCC ,0x3C ,0x39 ,0x07 ,0x00 ,0x00 ,0x00 ,0x07 ,0x00 ,0x03 ,0x00 ,0x00 ,0x00 ,0x02 ,0x00 };
		memcpy(pOutszSendBuffer,szData,0x15);
		return 0x15;
	}
	
	if( dwIndex == 2 )
	{
		unsigned char szData[] = {0x0A ,0x00 ,0xE0 ,0x55 ,0x13 ,0xA6 ,0x57 ,0x2C ,0x42 ,0x06, 0x00};
		memcpy(pOutszSendBuffer,szData,0x0A);
		return 0x0A;
	}

	if( dwIndex == 3 )
	{
		unsigned char szData[] = {0x0A ,0x00 ,0xE0 ,0x55 ,0x13 ,0xA6 ,0x58 ,0x2C ,0x42 ,0x06, 0x00};
		memcpy(pOutszSendBuffer,szData,0x0A);
		return 0x0A;
	}

	return 0;
}

