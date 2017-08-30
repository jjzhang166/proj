#include "StdAfx.h"
#include "JFZRActionSystem.h"
#include "BTTraverse.h"

typedef struct
{
	DWORD	str_dwCmdType;
	char	str_szSkillName[64];
}CMD_USE_SKILL,*PCMD_USE_SKILL;		// 使用技能 

typedef struct  
{
	DWORD	str_dwCount;			// 发送伤害封包的数量，代表攻击次数
	char	str_szSkillName[64];	// 技能名称------------
	wchar_t	str_szTypeName[64];		// 攻击对象的类型，如 怪物，箱子
	BYTE	str_bFirstHit;
	BYTE	str_bIsCounterAttack;
	DWORD	str_dwSerial;
}CMD_HIT,*PCMD_HIT;

typedef struct
{
	DWORD	str_dwCmdType;
	DWORD	str_dwSerial;
}CMD_PICKUPITEM,*PCMD_PICKUPITEM;		

typedef struct
{
	DWORD	str_dwCmdType;
	wchar_t	str_wszWarp[64];
}GOROOM,*PGOROOM;

typedef struct
{
	DWORD	str_dwLevel;
	wchar_t str_wszFbName[64];
	wchar_t str_wszWarpGateId[64];
	BYTE	str_bIsRestart;
}CMD_FB,*PCMD_FB;

typedef struct
{
	float	str_fx;
	float	str_fy;
	float	str_fz;
	wchar_t	str_wszCmdWalk[64];
}CMD_WALK,*PCMD_WALK;

typedef struct
{
	int		str_nEquipSlot;			// 装备槽
	int		str_nInvenSlot;			// 背包槽
}EQUIPITEM,*PEQUIPITEM;

typedef struct
{
	int		str_nUsingSlotIndex;
	int		str_nTargetSlotIndex;
}USEFORITEM,*PUSEFORITEM;

typedef struct
{
	wchar_t		str_wszNpcName[64];
	int			str_nTabCategory;
	int			str_nSlotIndex;
	int			str_nCount;
	DWORD		str_dwItemId;
}SELLITEM,*PSELLITEM;	// 买卖物品

typedef struct
{
	wchar_t		str_wszSkillName[64];
	DWORD		str_dwLevel;
}LEARNSKILL,*PLEARNSKILL;

JFZRActionSystem::JFZRActionSystem(void)
{
}

JFZRActionSystem::~JFZRActionSystem(void)
{
}

int senduseskill(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_USE_SKILL ptheUseSkill = (PCMD_USE_SKILL)lpParameter;

	JFZRSkill theSkill;
	assert(theSkill.TraverseSkill());

	PJFZR_SKILL_OBJ	ptheSkillObj = theSkill.GetSkillObj(ptheUseSkill->str_szSkillName);
	assert(ptheSkillObj);

	switch(ptheUseSkill->str_dwCmdType)
	{
	case 1:
		{
			return JFZR_UserSkillPacket1(pszsendpackebuffer,ptheSkillObj->str_dwSkillId);
		}
		break;
	case 2:
		{
			JFZRRole theRole;
			theRole.UpData();
			return JFZR_UserSkillPacket2(	pszsendpackebuffer,
											theRole.m_szJobNameEx,
											ptheSkillObj->str_szSkillNameEx,
											theRole.m_fx,
											theRole.m_fy,
											theRole.m_fz,
											0x0BB89,			// 方向x，后期需要时，可能会变
											0x355D,				// 方向y
											(FLOAT)JFZR_GetTickCount(),
											ptheSkillObj->str_dwSkillId,
											ptheSkillObj->str_dwSkillLevel,
											JFZR_GetTickCount()-1000);
		}
		break;
	case 3:
		{
			JFZRRole theRole;
			theRole.UpData();
			return JFZR_UserSkillPacket3(	pszsendpackebuffer,
											theRole.m_szJobNameEx,
											theRole.m_fx+5.0f,			// 使用技能结束后的坐标变化的 x ，这些值随时可能要变动
											theRole.m_fy+5.0f,			// y
											theRole.m_fz+1.0f,			// z
											0x0BB89,					// 方向x
											0x355D,						// 方向y
											(FLOAT)JFZR_GetTickCount(),
											JFZR_GetTickCount()-2000);
		}
		break;
	default:
		{
			assert(false);
		}
	}
	return 0;
}

DWORD g_dwTimeStamp = 0;

_declspec(naked) void HookGetTimeStamp()
{
	__asm
	{
		mov g_dwTimeStamp, ecx
	}
	__asm
	{
// 		PUSH EBP
// 		MOV EBP,ESP
// 		PUSH -0x1

		mov eax, 0x01CABECC


		//add eax, 5
		jmp eax
	}
}

// 进入副本时，取一次此值（g_dwTimeStamp）即可，因为这个值在每次重新进入副本的时候才会获取一个新的值
DWORD GetTimeStampAddr()
{
	g_dwTimeStamp = 0;
	Hook(JFZR_REVERSAL_TIME_STAMP_HOOK,HookGetTimeStamp);

	while(!g_dwTimeStamp)
	{
		Sleep(1);
	}
	return g_dwTimeStamp;
}

DWORD GetTimeStamp()
{
	DWORD dwTimeStamp = 0;
	assert(g_dwTimeStamp);
	if( g_dwTimeStamp != 0 )
	{
		// 取完值之后再加1
		dwTimeStamp = *(DWORD*)(g_dwTimeStamp+JFZR_REVERSAL_TIME_STAMP_OFFSET);

		// 每攻击一个怪物，都需要加1
		*(DWORD*)(g_dwTimeStamp+JFZR_REVERSAL_TIME_STAMP_OFFSET) += 1;
	}
	return dwTimeStamp;
}

int hitinfo(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_HIT ptheHit = (PCMD_HIT)lpParameter;
	assert(ptheHit);
	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
	memset(&theObjStruct,0,sizeof(theObjStruct));
	memcpy(theObjStruct.str_szType,ptheHit->str_szTypeName,wcslen(ptheHit->str_szTypeName)*2+1);

	// 遍历对象
	theMonsterObj.TraverseObj(false);
	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,ptheHit->str_dwCount);

// 	JFZRActionSystem theActionSystem;
// 	theActionSystem.Walk(L"Stand",theObjStruct.str_theMonsterObj[0].str_fx,theObjStruct.str_theMonsterObj[0].str_fy,theObjStruct.str_theMonsterObj[0].str_fz);


	//obj addr = 0x26D74864 
	//job name = Rg02Boss_HeavyDroidEX  
	//obj name = HeavyDroidBoss  
	//type name = NPC  
	//serial = 008990BF x = 106.179703 y = 69.661743 z = -0.235719 hp = 4973 mp = 0

// 	if( strstr(theObjStruct.str_theMonsterObj[0].str_szJobName,"Boss") != NULL )
// 	{
// 		if( theObjStruct.str_theMonsterObj[0].str_dwCurrentHp <= 400 )
// 		{
// 			Sleep(1000);
// 		}
// 	}


	DWORD dwOffset = 0;
	if( theObjStruct.str_nCount != 0 )
	{
		dwOffset = JFZR_SkillHitPacketHeader(pszsendpackebuffer,theObjStruct.str_nCount);
	}

	// 更新角色数据
	JFZRRole theRole;
	theRole.UpData();

	JFZRSkill theSkill;

	PJFZR_SKILL_OBJ ptheSkillObj = theSkill.GetSkillObj(ptheHit->str_szSkillName);
	if( ptheSkillObj == NULL )
	{
		assert(ptheSkillObj);
		return 0;
	}


	WORD  wPhysicalAttribute = 0;
	wchar_t wszElementalAttribute[256] = {0};


	DWORD dwProjectileHash = 0;
	
	if( strcmp(ptheSkillObj->str_szSkillName,"HalfMoonKick") == 0 )	// 刃武者-掠空腿 
	{
		wPhysicalAttribute = 0x0005;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"HalfMoonKick1");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"ShadowAttack") == 0 )	// 刃武者-瞬杀
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"UpperClaw") == 0 )	// 猎刃星-术法·升天
	{
		wPhysicalAttribute = 0x0001;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"LightningSlash") == 0 )	// 天狼星-天狼狂飙
	{
		wPhysicalAttribute = 0x0002;
		wcscpy_s(wszElementalAttribute,L"ELEC");
		dwProjectileHash = GetSigHash(L"Wolf_LightningSlash");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"5Slash") == 0 )		// 凤舞星-五喵杀
	{
		wPhysicalAttribute = 0x0009;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"5SlashEnd");		// 最后一击
	}

	//////////////////////////////////////////////////////////////////////////
	// 剑斗士系列
	else if( strcmp(ptheSkillObj->str_szSkillName,"HandShot") == 0 )	// 剑斗士-上勾拳
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"Horizon") == 0 )			// 剑魔-一字斩·凝光
	{
		wPhysicalAttribute = 0x0004;
		wcscpy_s(wszElementalAttribute,L"DARK");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"TekkenPunch") == 0 )		// 拳刹-重炮拳
	{
		wPhysicalAttribute = 0x0002;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"ChargeHack") == 0 )		// 斩狂-旋风斩
	{
		wPhysicalAttribute = 0x0001;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"ChargeHack_A");	
	}

	//////////////////////////////////////////////////////////////////////////
	// 控魔师系列
	else if( strcmp(ptheSkillObj->str_szSkillName,"IcePick") == 0 )		// 控魔师-冰刺
	{
		wPhysicalAttribute = 0x0005;
		wcscpy_s(wszElementalAttribute,L"ICE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"SmallMeteor") == 0 )		// 时空术士-S·星际陨石
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(L"SmallMeteor_Explosion");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"CrystalShower") == 0 )	// 霜语者-寒冰刃雨
	{
		wPhysicalAttribute = 0x0009;
		wcscpy_s(wszElementalAttribute,L"ICE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
		//dwProjectileHash = GetSigHash(L"CrystalShower_Crystal");   // 两个都可以用
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"Stinger") == 0 )		// 魔影师-瞬步影袭
	{
		wPhysicalAttribute = 0x0008;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"Stinger_Bullet_Long");
	}
	else
	{
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
		assert(false);
	}

	
	FLOAT fAttackPowerRate;

	try
	{
		fAttackPowerRate = GetSdbSkillInfo(ptheSkillObj->str_szSkillNameEx,ptheSkillObj->str_dwSkillLevel);
		assert(fAttackPowerRate);
	}
	catch (...)
	{
		assert(false);
	}

	
	DWORD dwProjectileSerial;
	try
	{
		dwProjectileSerial = GetProjectileSerial();			// 同一个技能的攻击是累加的，比如使用闪电球，攻击6次，只需要第一次使用的时候调用此函数，此后每次攻击只要累加即可，但LockAdd也是需要累加的
	}
	catch (...)
	{
		assert(false);
	}


	//assert(theObjStruct.str_nCount);
	for (int i=0; i<theObjStruct.str_nCount; i++)
	{
// 		if( wcscmp(ptheHit->str_szTypeName,L"NPC") == 0 )
// 		{
// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[JFZR] serial = %p x = %f y = %f z = %f",theObjStruct.str_theMonsterObj[i].str_dwSerial,theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz);
// 			OutputDebugString(szPrint);
// 
//  			JFZRActionSystem theActionSystem;
// 
// 			// 本地修改坐标，实现一次攻击一个NPC
// 			theActionSystem.LocalActionSyncPacket(theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz);
// 			Sleep(50);
// 
// // 			theActionSystem.ActionSyncPacket(L"Stand",theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz);
// //  		theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"RandomRun",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"Standby_Hold_Realize",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"StandBy_Run_Realize",&theObjStruct.str_theMonsterObj[i]);
// // 			theActionSystem.NpcActionSyncPacket(L"StandBy_Cognition_Realize",&theObjStruct.str_theMonsterObj[i]);
// 		}

		extern DWORD g_dwItemStatusValue;
		DWORD dwItemStatusValue = 0;
		if( theObjStruct.str_theMonsterObj[i].str_dwCurrentHp > g_dwItemStatusValue)
		{
			dwItemStatusValue = g_dwItemStatusValue;
		}
		else
		{
			dwItemStatusValue = theObjStruct.str_theMonsterObj[i].str_dwCurrentHp;
		}

		if( dwItemStatusValue <= 0 )
		{
			//assert(false);
			return 0;
		}

		dwOffset += JFZR_SkillHitPacketBody(
			pszsendpackebuffer+dwOffset,
			i,
			dwProjectileSerial,
			theObjStruct.str_theMonsterObj[i].str_dwSerial,
			theRole.m_dwSerial,
			fAttackPowerRate,				// 攻击力
			dwProjectileHash,		// GetSigHash(L"DoubleSlash_B") 具体技能具体添加 技能名为 DoubleSlash
			GetHash(theRole.m_szJobNameEx,ptheSkillObj->str_szSkillNameEx),
			ptheSkillObj->str_dwSkillId,
			ptheSkillObj->str_dwSkillLevel,
			GetTimeStamp(),
			ptheHit->str_bFirstHit,
			ptheHit->str_bIsCounterAttack,
			wPhysicalAttribute,
			wszElementalAttribute,
			dwItemStatusValue
			);
	}
	return dwOffset;
}

int actionhitinfo(LPVOID lpParameter, OUT char* pszsendpackebuffer)
{
	PCMD_HIT ptheHit = (PCMD_HIT)lpParameter;
	assert(ptheHit);

	if( ptheHit->str_dwSerial == 0 )
	{
		assert(false);
		return 0;
	}

	DWORD dwOffset = 0;
	dwOffset = JFZR_SkillHitPacketHeader(pszsendpackebuffer,1);


	// 更新角色数据
	JFZRRole theRole;
	theRole.UpData();

	JFZRSkill theSkill;

	PJFZR_SKILL_OBJ ptheSkillObj = theSkill.GetSkillObj(ptheHit->str_szSkillName);
	assert(ptheSkillObj);


	WORD  wPhysicalAttribute = 0;
	wchar_t wszElementalAttribute[256] = {0};


	DWORD dwProjectileHash = 0;

	if( strcmp(ptheSkillObj->str_szSkillName,"HalfMoonKick") == 0 )	// 刃武者-掠空腿 
	{
		wPhysicalAttribute = 0x0005;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"HalfMoonKick1");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"ShadowAttack") == 0 )	// 刃武者-瞬杀
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"UpperClaw") == 0 )	// 猎刃星-术法·升天
	{
		wPhysicalAttribute = 0x0001;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"LightningSlash") == 0 )	// 天狼星-天狼狂飙
	{
		wPhysicalAttribute = 0x0002;
		wcscpy_s(wszElementalAttribute,L"ELEC");
		dwProjectileHash = GetSigHash(L"Wolf_LightningSlash");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"5Slash") == 0 )		// 凤舞星-五喵杀
	{
		wPhysicalAttribute = 0x0009;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"5SlashEnd");		// 最后一击
	}

	//////////////////////////////////////////////////////////////////////////
	// 剑斗士系列
	else if( strcmp(ptheSkillObj->str_szSkillName,"HandShot") == 0 )	// 剑斗士-上勾拳
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"Horizon") == 0 )			// 剑魔-一字斩·凝光
	{
		wPhysicalAttribute = 0x0004;
		wcscpy_s(wszElementalAttribute,L"DARK");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"TekkenPunch") == 0 )		// 拳刹-重炮拳
	{
		wPhysicalAttribute = 0x0002;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"ChargeHack") == 0 )		// 斩狂-旋风斩
	{
		wPhysicalAttribute = 0x0001;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"ChargeHack_A");	
	}

	//////////////////////////////////////////////////////////////////////////
	// 控魔师系列
	else if( strcmp(ptheSkillObj->str_szSkillName,"IcePick") == 0 )		// 控魔师-冰刺
	{
		wPhysicalAttribute = 0x0005;
		wcscpy_s(wszElementalAttribute,L"ICE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"SmallMeteor") == 0 )		// 时空术士-S·星际陨石
	{
		wPhysicalAttribute = 0x0003;
		wcscpy_s(wszElementalAttribute,L"FIRE");
		dwProjectileHash = GetSigHash(L"SmallMeteor_Explosion");
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"CrystalShower") == 0 )	// 霜语者-寒冰刃雨
	{
		wPhysicalAttribute = 0x0009;
		wcscpy_s(wszElementalAttribute,L"ICE");
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
		//dwProjectileHash = GetSigHash(L"CrystalShower_Crystal");   // 两个都可以用
	}
	else if( strcmp(ptheSkillObj->str_szSkillName,"Stinger") == 0 )		// 魔影师-瞬步影袭
	{
		wPhysicalAttribute = 0x0008;
		wcscpy_s(wszElementalAttribute,L"NONE");
		dwProjectileHash = GetSigHash(L"Stinger_Bullet_Long");
	}
	else
	{
		dwProjectileHash = GetSigHash(ptheSkillObj->str_szSkillNameEx);
		assert(false);
	}


	FLOAT fAttackPowerRate;

	try
	{
		fAttackPowerRate = GetSdbSkillInfo(ptheSkillObj->str_szSkillNameEx,ptheSkillObj->str_dwSkillLevel);
		assert(fAttackPowerRate);
	}
	catch (...)
	{
		assert(false);
	}


	DWORD dwProjectileSerial;
	try
	{
		dwProjectileSerial = GetProjectileSerial();			// 同一个技能的攻击是累加的，比如使用闪电球，攻击6次，只需要第一次使用的时候调用此函数，此后每次攻击只要累加即可，但LockAdd也是需要累加的
	}
	catch (...)
	{
		assert(false);
	}

	// 遍历对象
	JFZRMonsterObj theMonsterObj;
	theMonsterObj.TraverseObj(false);

	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
	memset(&theObjStruct,0,sizeof(theObjStruct));
	memcpy(theObjStruct.str_szType,L"NPC",wcslen(L"NPC")*2);

	if( theMonsterObj.GetObjCount(L"NPC") == 0 )
	{
		return 0;
	}

	DWORD dwCurrentHp = 0;

	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,100);

	for (int i=0; i<theObjStruct.str_nCount; i++)
	{
		if( theObjStruct.str_theMonsterObj[i].str_dwSerial == ptheHit->str_dwSerial )
		{
			dwCurrentHp = theObjStruct.str_theMonsterObj[i].str_dwCurrentHp;
		}
	}

	if( dwCurrentHp == 0 )
	{
		return 0;
	}

	dwOffset += JFZR_SkillHitPacketBody(
		pszsendpackebuffer+dwOffset,
		0,
		dwProjectileSerial,
		ptheHit->str_dwSerial,
		theRole.m_dwSerial,
		fAttackPowerRate,				// 攻击力
		dwProjectileHash,		// GetSigHash(L"DoubleSlash_B") 具体技能具体添加 技能名为 DoubleSlash
		GetHash(theRole.m_szJobNameEx,ptheSkillObj->str_szSkillNameEx),
		ptheSkillObj->str_dwSkillId,
		ptheSkillObj->str_dwSkillLevel,
		GetTimeStamp(),
		ptheHit->str_bFirstHit,
		ptheHit->str_bIsCounterAttack,
		wPhysicalAttribute,
		wszElementalAttribute,
		dwCurrentHp
		);

	return dwOffset;
}


int gotonexetroom(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PGOROOM ptheGoRoom = (PGOROOM)lpParameter;
	JFZRRole theRole;
	theRole.UpData();
	if( ptheGoRoom->str_dwCmdType == 1  )
		return JFZR_GotoNextRoom1(pszsendpackebuffer,theRole.m_szRoleNameEx,ptheGoRoom->str_wszWarp);
	else if( ptheGoRoom->str_dwCmdType == 2 )
	{
		JFZRRoom theRoom;
		ROOM	rroom = {0};
		theRoom.UpDataRoom(&rroom);

		// 选择门时，需要对 rroom.str_i 赋值

		return JFZR_GotoNextRoom2(	pszsendpackebuffer,
									theRole.m_dwSerial,
									rroom.str_Coord[rroom.str_i].str_fx,
									rroom.str_Coord[rroom.str_i].str_fy,
									theRole.m_fz,
									0x3BFA,0x2CB5
									);
	}
	else 
	if( ptheGoRoom->str_dwCmdType == 3 )
	{
		extern DWORD g_dwValidationData_X;		// 过图验证数据x
		extern DWORD g_dwValidationData_Y;		// 过图验证数据y
		extern BYTE  g_bIsDummyExist;
		int nLen = JFZR_ChangeSector(pszsendpackebuffer,ptheGoRoom->str_wszWarp,g_dwValidationData_Y,g_dwValidationData_X,g_bIsDummyExist,
			GetSpeed(JFZR_REVERSAL_GET_ATTACK_SPEED_ADDR),GetSpeed(JFZR_REVERSAL_GET_MOVE_SPEED_ADDR),GetSpeed(JFZR_REVERSAL_GET_EVADE_RATING_ADDR));

		g_dwValidationData_Y = 0;	// 清0，防止重复使用
		g_dwValidationData_X = 0;
		g_bIsDummyExist = 0;

		return nLen;
	}
	return 0;
}

int retcity(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_RetCity(pszsendpackebuffer);
}

int pickupitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_PICKUPITEM pthePickUpItem = (PCMD_PICKUPITEM)lpParameter;
	if( pthePickUpItem->str_dwCmdType == 1 )
		return JFZR_PickUpItem1(pszsendpackebuffer,pthePickUpItem->str_dwSerial);
	else if( pthePickUpItem->str_dwCmdType == 2 )
	{
		return JFZR_PickUpItem2(pszsendpackebuffer,1);
	}
	else if( pthePickUpItem->str_dwCmdType == 3 )
	{
		return JFZR_PickUpItem2(pszsendpackebuffer,0);
	}
	return 0;
}

int repairitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_RepairItem(pszsendpackebuffer);
}

int equipitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PEQUIPITEM ptheEquipItem = (PEQUIPITEM)lpParameter;
	return JFZR_EquipItem(pszsendpackebuffer,ptheEquipItem->str_nEquipSlot,ptheEquipItem->str_nInvenSlot,L"BASIC",L"EQUIPMENT_TAB");
}

int unequipitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PEQUIPITEM ptheEquipItem = (PEQUIPITEM)lpParameter;
	return JFZR_UnEquipItem(pszsendpackebuffer,ptheEquipItem->str_nEquipSlot,ptheEquipItem->str_nInvenSlot,L"BASIC",L"EQUIPMENT_TAB");
}

int useforitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PUSEFORITEM	ptheUseForItem = (PUSEFORITEM)lpParameter;
	return JFZR_UseForItem(pszsendpackebuffer,ptheUseForItem->str_nUsingSlotIndex,ptheUseForItem->str_nTargetSlotIndex);
}

int changevillage(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_ChangeVillage(pszsendpackebuffer,(wchar_t *)lpParameter);
}

int sellitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSELLITEM ptheSellItem = (PSELLITEM)lpParameter;
	return JFZR_SellItem(pszsendpackebuffer,ptheSellItem->str_wszNpcName,ptheSellItem->str_nTabCategory,ptheSellItem->str_nSlotIndex,ptheSellItem->str_nCount);
}

int buyitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSELLITEM ptheSellItem = (PSELLITEM)lpParameter;
	return JFZR_BuyItem(pszsendpackebuffer,ptheSellItem->str_wszNpcName,ptheSellItem->str_dwItemId,ptheSellItem->str_nCount);
}

int learnskill(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PLEARNSKILL ptheLearnSkill = (PLEARNSKILL)lpParameter;
	return JFZR_LearnSkill(pszsendpackebuffer,ptheLearnSkill->str_wszSkillName,ptheLearnSkill->str_dwLevel);
}

typedef struct  
{
	wchar_t		str_szRoleName[MAX_PATH];
	DWORD		str_dwPlayerGender;
	DWORD		str_dwClassType;
}CREATE_PLAYER,*PCREATE_PLAYER;
int createplayer(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCREATE_PLAYER pthePlayer = (PCREATE_PLAYER)lpParameter;
	return JFZR_CreatePlayer(pszsendpackebuffer,pthePlayer->str_szRoleName,pthePlayer->str_dwPlayerGender,pthePlayer->str_dwClassType);
}

int selectplayer(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_SelectPlayer(pszsendpackebuffer,(wchar_t*)lpParameter);
}


typedef struct  
{
	wchar_t		str_wszReceiverId[256];
	wchar_t		str_wszTitle[256];
	wchar_t		str_wszMessage[256];
	UINT64		str_unGold;
	struct  
	{
		wchar_t		str_wszInventoryTab[256];
		int			str_nItemSlotIndex;
		int			str_nCount;
	}str_Item[100];
	int				str_nItemCount;
}MAIL_SEND_ITEM,*PMAIL_SEND_ITEM;

DWORD g_dwOidReceiverAddr = 0;

int mailsend(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PMAIL_SEND_ITEM ptheMailSend = (PMAIL_SEND_ITEM)lpParameter;
	int nOffset = 0;
	if( g_dwOidReceiverAddr == 0 )
	{
		g_dwOidReceiverAddr = JFZR_AllmMemory_Alloc(0x24);		// 申请一段内存，用于存放邮寄目标名称
	}
	// 将邮寄目标名称拷贝到申请的内存中
	JFZR_GameMemcpy(g_dwOidReceiverAddr,(WCHAR*)ptheMailSend->str_wszReceiverId);

	nOffset = JFZR_MailSendHeader(	pszsendpackebuffer,
									ptheMailSend->str_wszTitle,
									ptheMailSend->str_wszReceiverId,
									ptheMailSend->str_wszMessage,
									ptheMailSend->str_unGold,
									ptheMailSend->str_nItemCount,
									g_dwOidReceiverAddr);

	for( int i=0; i<ptheMailSend->str_nItemCount; i++ )
	{
		nOffset += JFZR_MailSendBody(	pszsendpackebuffer+nOffset,
										i,
										ptheMailSend->str_Item[i].str_wszInventoryTab,
										ptheMailSend->str_Item[i].str_nItemSlotIndex,
										ptheMailSend->str_Item[i].str_nCount);
	}

	nOffset += JFZR_MailSendTail(pszsendpackebuffer+nOffset);
	return nOffset;
}


int joinvillage(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_JoinVillage(pszsendpackebuffer);
}

int accept(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_Accept(pszsendpackebuffer,(const wchar_t *)lpParameter,1,-1,-1);
}

//////////////////////////////////////////////////////////////////////////////////////////
bool JFZRActionSystem::useskill(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack)
{
	try
	{
		theMonsterObj.TraverseObj(false);

		if( theMonsterObj.GetObjCount(L"NPC") == 0 )
		{
			return false;
		}

	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		CMD_USE_SKILL theUseSkill;
		if( bFirstHit == 1 )
		{
			// 技能使用1
			theUseSkill.str_dwCmdType = 1;
			memcpy(theUseSkill.str_szSkillName,pszskillname,strlen(pszskillname)+1);
			send(&theUseSkill,senduseskill);
			// 技能使用2
			theUseSkill.str_dwCmdType = 2;
			send(&theUseSkill,senduseskill);
		}


		CMD_HIT theHit;

		// 	memset(&theHit,0,sizeof(theHit));
		// 	theHit.str_dwCount = 10;			// 每次攻击怪物对象最大个数
		//  	memcpy(theHit.str_szSkillName,pszskillname,strlen(pszskillname)+1);
		// 
		//  // 打静态物品对象
		// 	char szTypeCmdMisc[32] = "MISC";
		// 	memcpy(theHit.str_szTypeName,szTypeCmdMisc,strlen(szTypeCmdMisc));
		// 	send(&theHit,hitinfo);


		// 打怪物对象
		memset(&theHit,0,sizeof(theHit));

		theHit.str_bFirstHit = bFirstHit;
		theHit.str_bIsCounterAttack	= bIsCounterAttack;
		theHit.str_dwCount = 15;			// 每次攻击怪物对象最大个数		// 此方法只能一次攻击一个
		memcpy(theHit.str_szSkillName,pszskillname,strlen(pszskillname)+1);

		wchar_t szTypeCmdNpc[32] = L"NPC";
		memcpy(theHit.str_szTypeName,szTypeCmdNpc,wcslen(szTypeCmdNpc)*2+1);
		// 发送伤害封包
		send(&theHit,hitinfo);


		if( bFirstHit == 1 )
		{
			// 技能使用3
			theUseSkill.str_dwCmdType = 3;
			send(&theUseSkill,senduseskill);
		}
	}
	catch (...)
	{
		assert(false);
	}

	//Sleep(400);		// 先移动到怪物身边，并发送攻击包，然后等待，需要让NPC有反应，不然依然被检测
	
	return true;
}

bool JFZRActionSystem::useskill2(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack)
{
	try
	{
		theMonsterObj.TraverseObj(false);

		if( theMonsterObj.GetObjCount(L"MISC") == 0 )
		{
			//assert(false);
			return false;
		}

	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		CMD_USE_SKILL theUseSkill;
		if( bFirstHit == 1 )
		{
			// 技能使用1
			theUseSkill.str_dwCmdType = 1;
			memcpy(theUseSkill.str_szSkillName,pszskillname,strlen(pszskillname)+1);
			send(&theUseSkill,senduseskill);
			// 技能使用2
			theUseSkill.str_dwCmdType = 15;
			send(&theUseSkill,senduseskill);
		}


		CMD_HIT theHit;

		// 	memset(&theHit,0,sizeof(theHit));
		// 	theHit.str_dwCount = 10;			// 每次攻击怪物对象最大个数
		//  	memcpy(theHit.str_szSkillName,pszskillname,strlen(pszskillname)+1);
		// 
		//  // 打静态物品对象
		// 	char szTypeCmdMisc[32] = "MISC";
		// 	memcpy(theHit.str_szTypeName,szTypeCmdMisc,strlen(szTypeCmdMisc));
		// 	send(&theHit,hitinfo);


		// 打怪物对象
		memset(&theHit,0,sizeof(theHit));

		theHit.str_bFirstHit = bFirstHit;
		theHit.str_bIsCounterAttack	= bIsCounterAttack;
		theHit.str_dwCount = 15;			// 每次攻击怪物对象最大个数
		memcpy(theHit.str_szSkillName,pszskillname,strlen(pszskillname)+1);

		wchar_t wszTypeCmdNpc[32] = L"MISC";
		memcpy(theHit.str_szTypeName,wszTypeCmdNpc,wcslen(wszTypeCmdNpc)*2+1);
		// 发送伤害封包
		send(&theHit,hitinfo);


		if( bFirstHit == 1 )
		{
			// 技能使用3
			theUseSkill.str_dwCmdType = 3;
			send(&theUseSkill,senduseskill);
		}
	}
	catch (...)
	{
		assert(false);
	}



	return true;
}

bool JFZRActionSystem::actionuseskill(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack,DWORD dwSerial)
{
	try
	{
		if( dwSerial == 0 )
		{
			assert(false);
			return false;
		}

		theMonsterObj.TraverseObj(false);

		if( theMonsterObj.GetObjCount(L"NPC") == 0 )
		{
			return false;
		}

	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		CMD_HIT theHit;

		// 打怪物对象
		memset(&theHit,0,sizeof(theHit));

		theHit.str_bFirstHit = bFirstHit;
		theHit.str_bIsCounterAttack	= bIsCounterAttack;
		theHit.str_dwCount = 1;			// 每次攻击怪物对象最大个数
		theHit.str_dwSerial = dwSerial;
		memcpy(theHit.str_szSkillName,pszskillname,strlen(pszskillname)+1);

		wchar_t szTypeCmdNpc[32] = L"NPC";
		memcpy(theHit.str_szTypeName,szTypeCmdNpc,wcslen(szTypeCmdNpc)*2+1);
		// 发送伤害封包
		send(&theHit,actionhitinfo);


	}
	catch (...)
	{
		assert(false);
	}



	return true;
}

bool JFZRActionSystem::send(LPVOID lpParameter,PSENDPACKET pFun)
{
	try
	{
		DWORD	dwArgc1 = 0;
		// 读取参数一的值
		_ReadProcessMemory(JFZR_REVERSAL_SEND_PACKET_SOCKET_ADDR,&dwArgc1,sizeof(dwArgc1));
		_ReadProcessMemory(dwArgc1+JFZR_REVERSAL_SEND_PACKET_SOCKET_OFFSET___,&dwArgc1,sizeof(dwArgc1));
		assert(dwArgc1);

		// 组建对象,即调用游戏内部函数申请缓冲区
		GAME_BUIILD_OBJ theBuildObj;
		__asm
		{
			push 0
			lea ecx, theBuildObj
			mov eax, JFZR_REVERSAL_SEND_ALLOC_OBJ_CALL
			call eax
		}

		int nDataLen = pFun(lpParameter,(char*)theBuildObj.dwBufferBeginAddr1);

		if( nDataLen > 0 )
		{
			theBuildObj.dwBufferEndAddr = nDataLen + (DWORD)theBuildObj.dwBufferBeginAddr1;
			DWORD	dwSendBufferLen = (DWORD)(WORD)(theBuildObj.dwBufferEndAddr -theBuildObj.dwBufferBeginAddr1);

			if( dwSendBufferLen != 0 )
			{
				*(WORD*)theBuildObj.dwBufferBeginAddr1 = (WORD)dwSendBufferLen;

				SEND_CALL_ARGC theArgc;

				// 基址未获取，暂且先用这个
				DWORD	dwEcx = g_dwSendObjAddr+JFZR_REVERSAL_SEND_PACKET_OFFSET;

				_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_JIAMI_OBJ_OFFSET,&theArgc.dwArgc3,sizeof(theArgc.dwArgc3));
				_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_TASK_OBJ_OFFSET,&theArgc.dwArgc4,sizeof(theArgc.dwArgc4));

				// 发包前任务对象+1
				__asm
				{
					mov EBX, theArgc.dwArgc4
					add EBX, 4
					mov ECX, dword ptr [EBX]
					mov eax, ecx
					add ECX, 1
					LOCK CMPXCHG DWORD PTR DS:[EBX],ECX
				}


				__asm
				{
					sub esp, 8
					mov ecx, esp
					lea edx, theArgc		// 加密对象,发包任务对象
					push edx
					mov dword ptr [ecx], 0
					mov dword ptr [ecx+4], 0
					mov eax, JFZR_REVERSAL_SEND_PUSH_ARGC3_ARGC4_CALL
					call eax

					lea eax, theBuildObj	// 明文数据
					push eax				
					push dwArgc1			// socket 对象
					mov eax, JFZR_REVERSAL_SEND_ADDR_CALL
					call eax
					add esp, 0x10
				}

				// 发包结束后任务-1
				__asm
				{
					mov EDX, theArgc.dwArgc4
					add EDX, 4
					OR EAX,0xFFFFFFFF
					LOCK XADD DWORD PTR DS:[EDX],EAX
				}
			}
		}
		// 释放组建对象时申请的内存
		__asm
		{
			lea ecx, theBuildObj
			mov eax, JFZR_REVERSAL_SEND_FREE_OBJ_CALL
			call eax
		}

		return true;
	}
	catch (...)
	{
		assert(false);
	}


	return false;
}

int startstage(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_FB	theFb = (PCMD_FB)lpParameter;
	return JFZR_StartStage(pszsendpackebuffer,theFb->str_wszFbName,theFb->str_wszWarpGateId,theFb->str_dwLevel,theFb->str_bIsRestart);
}

bool JFZRActionSystem::StartStage(wchar_t* pszFbName,wchar_t* pszWarpGateId,DWORD dwLevel,BYTE	bIsRestart)
{
	CMD_FB theFb;
	memset(&theFb,0,sizeof(CMD_FB));
	memcpy(theFb.str_wszFbName,pszFbName,wcslen(pszFbName)*2+2);
	memcpy(theFb.str_wszWarpGateId,pszWarpGateId,wcslen(pszWarpGateId)*2+2);
	theFb.str_dwLevel = dwLevel;
	theFb.str_bIsRestart	= bIsRestart;
	return send(&theFb,startstage);
}

bool JFZRActionSystem::GotoNextRoom(wchar_t* pszWarp)
{
	GOROOM theGoRoom;
	memset(&theGoRoom,0,sizeof(theGoRoom));
 	theGoRoom.str_dwCmdType = 1;
 	memcpy(theGoRoom.str_wszWarp,pszWarp,wcslen(pszWarp)*2+1);
	send((LPVOID)&theGoRoom,gotonexetroom);
// 	theGoRoom.str_dwCmdType = 2;
// 	send((LPVOID)&theGoRoom,gotonexetroom);
	theGoRoom.str_dwCmdType = 3;
	send((LPVOID)&theGoRoom,gotonexetroom);
	return true;
}

bool JFZRActionSystem::RetCity(void)
{
	return send(NULL,retcity);
}

bool JFZRActionSystem::PickUpItem(void)
{
	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
	memset(&theObjStruct,0,sizeof(theObjStruct));
	memcpy(theObjStruct.str_szType,L"UNKNOWN",wcslen(L"UNKNOWN")*2+1);

	theMonsterObj.TraverseObj(false);
	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,20);

	for (int i = 0; i<theObjStruct.str_nCount; i++)
	{
		CMD_PICKUPITEM thePickUpItem;
		thePickUpItem.str_dwCmdType = 1;
		thePickUpItem.str_dwSerial = theObjStruct.str_theMonsterObj[i].str_dwSerial;


		JFZRActionSystem theActionSystem;
		assert(theObjStruct.str_theMonsterObj[i].str_fx);
		assert(theObjStruct.str_theMonsterObj[i].str_fy);

		if( (int)theObjStruct.str_theMonsterObj[i].str_fx != 0 )
		{
 			//theActionSystem.ActionSyncPacket(L"FastRun",theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz);
			theActionSystem.ActionSyncPacket(L"Stand",theObjStruct.str_theMonsterObj[i].str_fx+1,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz);

			Sleep(10);

			send(&thePickUpItem,pickupitem);
		}
	}
	return true;
}

JFZRRole theRole;

DWORD JFZRActionSystem::PassivePickUpItem(FLOAT& fx,FLOAT& fy,FLOAT& fz)
{

	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
	memset(&theObjStruct,0,sizeof(theObjStruct));
	memcpy(theObjStruct.str_szType,"UNKNOWN",strlen("UNKNOWN"));

	// 遍历物品，判断是否有需要捡取的物品
	theMonsterObj.TraverseObj(false);
	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,20);


	if( theObjStruct.str_nCount > 0 )
	{
		fx = theObjStruct.str_theMonsterObj[0].str_fx;
		fy = theObjStruct.str_theMonsterObj[0].str_fy;
		fz = theObjStruct.str_theMonsterObj[0].str_fz;

		static DWORD dwSerial = 0;
		if( dwSerial == 0 )
			theRole.UpData();

		// 如果有物品，要返回角色的序号，因为还有怪物也在走路
		return theRole.m_dwSerial;
	}


	return 0;
}

bool JFZRActionSystem::RepairItem(void)
{
	return send(NULL,repairitem);
}



int actionsyncpacket(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_WALK ptheWalk = (PCMD_WALK)lpParameter;

	JFZRRole theRole;
	theRole.UpData();

	extern FLOAT TraversePrevActionPlayTime();
	extern DWORD GetRandomSeed();

	DWORD dwActionSerial = LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	if( wcscmp(ptheWalk->str_wszCmdWalk,L"Stand") == 0 )
	{
		return JFZR_ActionSyncPacket(	pszsendpackebuffer,
			theRole.m_dwSerial,
			GetHash(theRole.m_szJobNameEx,ptheWalk->str_wszCmdWalk),
			ptheWalk->str_fx,
			ptheWalk->str_fy,
			ptheWalk->str_fz,
			0,
			0,
			0,
			GetDirection(theRole.m_fDirectionX),
			GetDirection(theRole.m_fDirectionY),
			1,
			TraversePrevActionPlayTime(),
			dwActionSerial,
			dwActionSerial-1,
			GetRandomSeed(),
			(FLOAT)GetTickCount(),
			1
			);
	}
	else if( wcscmp(ptheWalk->str_wszCmdWalk,L"FastRun") == 0 )
	{
		return JFZR_ActionSyncPacket(	pszsendpackebuffer,
			theRole.m_dwSerial,
			GetHash(theRole.m_szJobNameEx,ptheWalk->str_wszCmdWalk),
			ptheWalk->str_fx,
			ptheWalk->str_fy,
			ptheWalk->str_fz,
			theRole.m_fDirectionX,
			theRole.m_fDirectionY,
			theRole.m_fDirectionZ,
			GetDirection(theRole.m_fDirectionX),
			GetDirection(theRole.m_fDirectionY),
			0,
			TraversePrevActionPlayTime(),
			dwActionSerial,
			dwActionSerial-1,
			GetRandomSeed(),
			(FLOAT)GetTickCount(),
			1
			);
	}

	LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	return false;
}


int npchijackedactionsyncpacket(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_ACTION_SYNC_PACKET ptheActionSyncPacket = (PSEND_ACTION_SYNC_PACKET)lpParameter;

	extern DWORD GetNpcActionHash(DWORD dwSerial,wchar_t* pActionId);

	DWORD dwActionHash = GetNpcActionHash(ptheActionSyncPacket->dwSerial,L"Realize");

	if( dwActionHash == 0 )
	{
		return 0;
	}

	JFZRRole theRole;
	theRole.UpData();

	return JFZR_ActionSyncPacket(	pszsendpackebuffer,
		ptheActionSyncPacket->dwSerial,
		dwActionHash,	//ptheActionSyncPacket->dwActionHash,
		ptheActionSyncPacket->theActionStartPosition.fx,//theRole.m_fx,//
		ptheActionSyncPacket->theActionStartPosition.fy,//theRole.m_fy,//
		ptheActionSyncPacket->theActionStartPosition.fz,//theRole.m_fz,//
		0.0f,
		0.0f,
		0.0f,
		ptheActionSyncPacket->wDirectionX,
		ptheActionSyncPacket->wDirectionY,
		ptheActionSyncPacket->bSkipValidation,
		ptheActionSyncPacket->fPrevActionPlayTime,
		ptheActionSyncPacket->dwActionSerial,
		ptheActionSyncPacket->dwPrevActionSerial,
		ptheActionSyncPacket->dwRandomSeed,
		ptheActionSyncPacket->fClientAbsoluteTime,
		ptheActionSyncPacket->bExistParams
		);
}
bool JFZRActionSystem::NpcHijacketActionSyncPacket(LPVOID ptheActionSyncPacket)
{
	return send(ptheActionSyncPacket,npchijackedactionsyncpacket);
}

typedef struct  
{
	wchar_t	str_wszCmdWalk[64];
	PJFZR_MONSTER_OBJ	ptheMonster;
}NPC_ACTION_SYNC_PACKET,*PNPC_ACTION_SYNC_PACKET;

int npcactionsyncpacket(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PNPC_ACTION_SYNC_PACKET ptheWalk = (PNPC_ACTION_SYNC_PACKET)lpParameter;

	extern FLOAT TraversePrevActionPlayTime();
	extern DWORD GetRandomSeed();

	JFZRRole theRole;
	theRole.UpData();

// 	char szPrint[256] = {0};
// 	sprintf_s(szPrint,"[JFZR] x = %f key = %d y = %f key = %f",
// 		ptheWalk->ptheMonster->str_fDirectionX,GetDirection(ptheWalk->ptheMonster->str_fDirectionX),ptheWalk->ptheMonster->str_fDirectionY,GetDirection(ptheWalk->ptheMonster->str_fDirectionY));
// 	OutputDebugString(szPrint);

	DWORD dwActionSerial = LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);


	PREV_ACTION_DATA thePrevActionData = {0};
	thePrevActionData.dwSerial = ptheWalk->ptheMonster->str_dwSerial;	// 传入对象序号，得到对象的上次行动序号和上次行动间隔时间
	thePrevActionData.dwActionSerial = dwActionSerial;		// 本身是用来发送后写入当前序号的，但是如果为0值则不发送，所以暂时不用

	try
	{
		// 取NPC 上次行动序号和上次行动间隔时间
		if( true == TraversePrevActionPlayTime_(&thePrevActionData) )
		{
			DWORD dwDirectionX = 0;
			DWORD dwDirectionY = 0;

			// 暂时不用改，预计后期会用到
			if( (int)ptheWalk->ptheMonster->str_fDirectionX == 0 || (int)ptheWalk->ptheMonster->str_fDirectionY == 0 )
			{
				dwDirectionX = 0xBBDC;
				dwDirectionY = 0xB1F5;
			}
			else
			{
				dwDirectionX = GetDirection(ptheWalk->ptheMonster->str_fDirectionX);
				dwDirectionY = GetDirection(ptheWalk->ptheMonster->str_fDirectionY);
			}

			if( dwDirectionX == 0 || dwDirectionY == 0 )
			{
				assert(false);
				return 0;
			}

// 			if( thePrevActionData.dwPrevActionSerial != 0 )
// 			{
// 				if( wcscmp(ptheWalk->str_wszCmdWalk,L"Realize") == 0 )
// 				{
// 					return 0;
// 				}
// 				return 0;
// 			}

			return JFZR_ActionSyncPacket(	pszsendpackebuffer,
				ptheWalk->ptheMonster->str_dwSerial,		// NPC 序号
				GetHash(ptheWalk->ptheMonster->str_szJobName,ptheWalk->str_wszCmdWalk),		// NPC 职业字符串，状态字符串
				ptheWalk->ptheMonster->str_fx,	// NPC 坐标
				ptheWalk->ptheMonster->str_fy,
				ptheWalk->ptheMonster->str_fz,
				0,
				0,
				0,
				dwDirectionX,		// NPC的方向
				dwDirectionY,
				1,
				thePrevActionData.fPrevActionPlayTime,	// TraversePrevActionPlayTime
				dwActionSerial,		// dwActionSerial
				thePrevActionData.dwPrevActionSerial,		// dwActionSerial-n
				GetRandomSeed(),
				(FLOAT)GetTickCount(),
				0
				);
		}
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

bool JFZRActionSystem::NpcActionSyncPacket(wchar_t* pwszCmdWalk,PJFZR_MONSTER_OBJ ptheMonster)
{
	NPC_ACTION_SYNC_PACKET theNpcActionSyncPacket;
	memcpy(theNpcActionSyncPacket.str_wszCmdWalk,pwszCmdWalk,wcslen(pwszCmdWalk)*2+2);
	theNpcActionSyncPacket.ptheMonster = ptheMonster;
	return send(&theNpcActionSyncPacket,npcactionsyncpacket);
}

bool JFZRActionSystem::ActionSyncPacket(wchar_t* pwszCmdWalk,float fx, float fy, float fz)
{
	CMD_WALK theWalk;
	memcpy(theWalk.str_wszCmdWalk,pwszCmdWalk,wcslen(pwszCmdWalk)*2+2);
	theWalk.str_fx = fx;
	theWalk.str_fy = fy;
	theWalk.str_fz = fz;
	return send(&theWalk,actionsyncpacket);
}

bool JFZRActionSystem::LocalActionSyncPacket(float fx, float fy, float fz)
{
	JFZRRole theRole;
	theRole.LocalActionSyncxyz(fx,fy,fz);
	Sleep(50);
	return true;
}



int directionsyncpacket(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_WALK ptheWalk = (PCMD_WALK)lpParameter;

	JFZRRole theRole;
	theRole.UpData();

	return JFZR_GotoNextRoom2(	pszsendpackebuffer,
								theRole.m_dwSerial,
								ptheWalk->str_fx,
								ptheWalk->str_fy,
								theRole.m_fz,
								GetDirection(theRole.m_fDirectionX),
								GetDirection(theRole.m_fDirectionY)
								);

}


bool JFZRActionSystem::DirectionSyncPacket(float fx, float fy, float fz)
{
	CMD_WALK theWalk;
	theWalk.str_fx = fx;
	theWalk.str_fy = fy;
	theWalk.str_fz = fz;

	return send(&theWalk,directionsyncpacket);
}


int villageactionsyncpacket(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCMD_WALK ptheWalk = (PCMD_WALK)lpParameter;

	JFZRRole theRole;
	theRole.UpData();

	if( wcscmp(ptheWalk->str_wszCmdWalk,L"Stand") == 0 )
	{
		return JFZR_VillageActionSyncPacket(	pszsendpackebuffer,
			theRole.m_dwSerial,
			theRole.m_szJobNameEx,
			ptheWalk->str_wszCmdWalk,
			ptheWalk->str_fx,
			ptheWalk->str_fy,
			ptheWalk->str_fz,
			0,
			0,
			0,
			0x0BB89,
			0x355D
			);
	}
	else if( wcscmp(ptheWalk->str_wszCmdWalk,L"FastRun") == 0 )
	{
		return JFZR_VillageActionSyncPacket(	pszsendpackebuffer,
			theRole.m_dwSerial,
			theRole.m_szJobNameEx,
			ptheWalk->str_wszCmdWalk,
			ptheWalk->str_fx,
			ptheWalk->str_fy,
			ptheWalk->str_fz,
			theRole.m_fDirectionX,
			theRole.m_fDirectionY,
			theRole.m_fDirectionZ,
			0x0BB89,
			0x355D
			);
	}
	return false;
}

bool JFZRActionSystem::VillageActionSyncPacket(wchar_t* pwszCmdWalk,float fx, float fy, float fz)
{
	CMD_WALK theWalk;
	memcpy(theWalk.str_wszCmdWalk,pwszCmdWalk,wcslen(pwszCmdWalk)*2+2);
	theWalk.str_fx = fx;
	theWalk.str_fy = fy;
	theWalk.str_fz = fz;
	return send(&theWalk,villageactionsyncpacket);
}



bool JFZRActionSystem::EquipItem(int nEquipSlot, int nInvenSlot)
{
	EQUIPITEM theEquipItem;
	theEquipItem.str_nEquipSlot = nEquipSlot;
	theEquipItem.str_nInvenSlot = nInvenSlot;
	return send(&theEquipItem,equipitem);
}

bool JFZRActionSystem::UnEquipItem(int nEquipSlot, int nInvenSlot)
{
	EQUIPITEM theEquipItem;
	theEquipItem.str_nEquipSlot = nEquipSlot;
	theEquipItem.str_nInvenSlot = nInvenSlot;
	return send(&theEquipItem,unequipitem);
}

// 函数功能：鉴定或解除封印  参数1：鉴定书或解除封印石的位置； 参数2：物品位置
bool JFZRActionSystem::UseForItem(int nUsingSlotIndex, int nTargetSlotIndex)
{
	USEFORITEM	theUseForItem;
	theUseForItem.str_nUsingSlotIndex = nUsingSlotIndex;
	theUseForItem.str_nTargetSlotIndex = nTargetSlotIndex;
	return send(&theUseForItem,useforitem);
}

bool JFZRActionSystem::ChangeVillage(wchar_t* pwszVillageId)
{
	return send(pwszVillageId,changevillage);
}

bool JFZRActionSystem::SellItem(wchar_t* pwszNpcName, int nTabCategory, int nSlotIndex, int nCount)
{
	SELLITEM theSellItem;
	memcpy(theSellItem.str_wszNpcName,pwszNpcName,wcslen(pwszNpcName)*2+2);
	theSellItem.str_nTabCategory = nTabCategory;
	theSellItem.str_nSlotIndex	= nSlotIndex;
	theSellItem.str_nCount = nCount;
	return send(&theSellItem,sellitem);
}

bool JFZRActionSystem::BuyItem(wchar_t* pwszNpcName,DWORD dwSellId,int nCount)
{
	SELLITEM theSellItem;
	memcpy(theSellItem.str_wszNpcName,pwszNpcName,wcslen(pwszNpcName)*2+2);
	theSellItem.str_dwItemId = dwSellId;
	theSellItem.str_nCount = nCount;
	return send(&theSellItem,buyitem);
}

bool JFZRActionSystem::LearnSkill(wchar_t* pwszSkillName, DWORD dwSkillLevel)
{
	LEARNSKILL theLearnSkill;
	memcpy(theLearnSkill.str_wszSkillName,pwszSkillName,wcslen(pwszSkillName)*2+2);
	theLearnSkill.str_dwLevel = dwSkillLevel;
	return send(&theLearnSkill,learnskill);
}

bool JFZRActionSystem::CreatePlayer(wchar_t* pszRoleName,DWORD dwClassType,DWORD dwPlayerGender)
{
	CREATE_PLAYER thePlayer;
	memcpy(thePlayer.str_szRoleName,pszRoleName,(wcslen(pszRoleName)+1)*2);
	thePlayer.str_dwClassType = dwClassType;
	thePlayer.str_dwPlayerGender = dwPlayerGender;
	return send(&thePlayer,createplayer);
}

bool JFZRActionSystem::SelectPlayer(wchar_t* pszRoleName)
{
	return send((LPVOID)pszRoleName,selectplayer);
}


int deselectplayer(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_DeselectPlayer(pszsendpackebuffer);
}

bool JFZRActionSystem::DeselectPlayer()
{
	return send(NULL,deselectplayer);
}

int listplayers(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_ListPlayers(pszsendpackebuffer);
}

bool JFZRActionSystem::ListPlayers()
{
	return send(NULL,listplayers);
}

int requestswitchchannel(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_RequestSwitchChannel(pszsendpackebuffer,(wchar_t*)lpParameter);
}

bool JFZRActionSystem::RequestSwitchChannel(wchar_t* pszChannelName)
{
	return send((LPVOID)pszChannelName,requestswitchchannel);
}

typedef struct  
{
	wchar_t	str_szChannelName[256];				// 频道名称
	wchar_t	str_szAccountId[256];				// QQ账号
	wchar_t	str_szSessionKey[256];				// KEY
}SWITCH_CHANNEL,*PSWITCH_CHANNEL;

int switchchannel(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSWITCH_CHANNEL ptheSwitchChannel = (PSWITCH_CHANNEL)lpParameter;
	return JFZR_SwitchChannel(pszsendpackebuffer,ptheSwitchChannel->str_szChannelName,ptheSwitchChannel->str_szAccountId,ptheSwitchChannel->str_szSessionKey);
}

bool JFZRActionSystem::SwitchChannel(const wchar_t* pszChannelName,const wchar_t* pszAccountId,const wchar_t* pszSessionKey)
{
	try
	{
		SWITCH_CHANNEL theSwitchChannel;
		memcpy(theSwitchChannel.str_szChannelName,pszChannelName,wcslen(pszChannelName)*2+2);
		memcpy(theSwitchChannel.str_szAccountId,pszAccountId,wcslen(pszAccountId)*2+2);
		memcpy(theSwitchChannel.str_szSessionKey,pszSessionKey,wcslen(pszSessionKey)*2+2);

		return send(&theSwitchChannel,switchchannel);
	}
	catch (...)
	{
		assert(false);
	}
	
	return false;
}

bool JFZRActionSystem::MailSend(int nItemSlotIndex)
{
	MAIL_SEND_ITEM theMailSend = {0};
	memcpy(theMailSend.str_wszReceiverId,L"斯和co",wcslen(L"斯和co")*2+2);
	memcpy(theMailSend.str_wszTitle,L"987654",wcslen(L"987654")*2+2);
	memcpy(theMailSend.str_wszMessage,L"123456",wcslen(L"123456")*2+2);
	theMailSend.str_unGold = 1000;					// 金币

	theMailSend.str_nItemCount = 1;
	memcpy(theMailSend.str_Item[0].str_wszInventoryTab,L"EQUIPMENT_TAB",wcslen(L"EQUIPMENT_TAB")*2+2);		// 普通栏
	theMailSend.str_Item[0].str_nItemSlotIndex = nItemSlotIndex;			// 背包格子，从0开始表示
	theMailSend.str_Item[0].str_nCount = 1;					// 物品数量
	
	return send(&theMailSend,mailsend);
}

bool JFZRActionSystem::JoinVillage(void)
{
	return send(NULL,joinvillage);
}

bool JFZRActionSystem::Accept(wchar_t* pwszQuestId)
{
	return send(pwszQuestId,accept);
}

typedef struct  
{
	wchar_t		str_wszQuestId[256];
	wchar_t		str_wszRewardItemId[256];
}COMPLETE,*PCOMPLETE;

int complete(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCOMPLETE ptheComplete = (PCOMPLETE)lpParameter;
	return JFZR_Complete(pszsendpackebuffer,ptheComplete->str_wszQuestId,ptheComplete->str_wszRewardItemId);
}

bool JFZRActionSystem::Complete(wchar_t* pwszQuestId, wchar_t*	pwszRewardItemId)
{
	COMPLETE theComplete;

	memcpy(theComplete.str_wszQuestId,pwszQuestId,wcslen(pwszQuestId)*2+2);
	memcpy(theComplete.str_wszRewardItemId,pwszRewardItemId,wcslen(pwszRewardItemId)*2+2);

	return send(&theComplete,complete);
}

int giveup(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PCOMPLETE ptheGiveUp = (PCOMPLETE)lpParameter;
	return JFZR_GiveUp(pszsendpackebuffer,ptheGiveUp->str_wszQuestId);
}

bool JFZRActionSystem::GiveUp(wchar_t* pwszQuestId)
{
	COMPLETE theGiveUp;

	memcpy(theGiveUp.str_wszQuestId,pwszQuestId,wcslen(pwszQuestId)*2+2);

	return send(&theGiveUp,giveup);
}

typedef struct  
{
	wchar_t		str_wszQuestId[256];
	DWORD		str_dwState;
}TELEPORTTOVILLAGE,*PTELEPORTTOVILLAGE;

int teleporttovillage(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PTELEPORTTOVILLAGE ptheTeleporttovillage = (PTELEPORTTOVILLAGE)lpParameter;
	return JFZR_TeleportToVillage(pszsendpackebuffer,ptheTeleporttovillage->str_wszQuestId,ptheTeleporttovillage->str_dwState);
}

bool JFZRActionSystem::TeleportToVillage(wchar_t* pwszQuestId, DWORD dwState)
{
	TELEPORTTOVILLAGE theTeleporttovillage;
	memcpy(theTeleporttovillage.str_wszQuestId,pwszQuestId,wcslen(pwszQuestId)*2+2);
	theTeleporttovillage.str_dwState = dwState;
	return send(&theTeleporttovillage,teleporttovillage);
}

typedef struct
{
	DWORD		str_dwTabCategory;
	DWORD		str_dwSlotIndex;
	WORD		str_wIsAvatarItem;
}DISASSEMBLY,*PDISASSEMBLY;

int disassembly(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PDISASSEMBLY ptheDisassembly = (PDISASSEMBLY)lpParameter;
	return JFZR_Disassembly(pszsendpackebuffer,ptheDisassembly->str_dwTabCategory,ptheDisassembly->str_dwSlotIndex,ptheDisassembly->str_wIsAvatarItem);
}

bool JFZRActionSystem::Disassembly(DWORD dwTabCategory, DWORD dwSlotIndex,WORD wIsAvatarItem)
{
	DISASSEMBLY	theDisassembly;
	theDisassembly.str_dwSlotIndex = dwSlotIndex;
	theDisassembly.str_dwTabCategory = dwTabCategory;
	theDisassembly.str_wIsAvatarItem = wIsAvatarItem;
	return send(&theDisassembly,disassembly);
}

int stagereward(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_StageReward(pszsendpackebuffer,(DWORD)lpParameter);
}

bool JFZRActionSystem::StageReward(DWORD dwRewardIndex)
{
	return send((LPVOID)dwRewardIndex,stagereward);
}

typedef struct  
{
	wchar_t	str_wszCategory[256];
	wchar_t	str_wszKeyWord[256];
	DWORD	str_dwPageNum;
}REQUEST_PRODUCT_LIST,*PREQUEST_PRODUCT_LIST;

int requestproductlist(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PREQUEST_PRODUCT_LIST ptheRequestProductList = (PREQUEST_PRODUCT_LIST)lpParameter;
	return JFZR_RequestProductList(pszsendpackebuffer,ptheRequestProductList->str_wszCategory,ptheRequestProductList->str_wszKeyWord,ptheRequestProductList->str_dwPageNum);
}

bool JFZRActionSystem::RequestProductList(wchar_t* pwszCategory,wchar_t* pwszKeyWord,DWORD dwPageNum)
{
	REQUEST_PRODUCT_LIST	theRequestProductList = {0};
	memcpy(theRequestProductList.str_wszCategory,pwszCategory,wcslen(pwszCategory)*2+2);		// 物品模板+D0的偏移为此数据
	memcpy(theRequestProductList.str_wszKeyWord,pwszKeyWord,wcslen(pwszKeyWord)*2+2);
	theRequestProductList.str_dwPageNum = dwPageNum;
	
	return send(&theRequestProductList,requestproductlist);
}

typedef struct
{
	LONGLONG	str_llOidMarket;
	DWORD		str_dwCount;
}BUY_PRODUCT,*PBUY_PRODUCT;

int buyproduct(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PBUY_PRODUCT ptheBuyProduct = (PBUY_PRODUCT)lpParameter;
	return JFZR_BuyProduct(pszsendpackebuffer,ptheBuyProduct->str_llOidMarket,ptheBuyProduct->str_dwCount);
}

bool JFZRActionSystem::BuyProduct(LONGLONG llOidMarket, DWORD dwCount)
{
	BUY_PRODUCT	theBuyProduct = {0};
	theBuyProduct.str_llOidMarket = llOidMarket;
	theBuyProduct.str_dwCount = dwCount;
	return send(&theBuyProduct,buyproduct);
}

typedef struct  
{
	wchar_t		str_wszName[256];
	DWORD		str_dwItemTabCategory;
	DWORD		str_dwItemSlotIndex;
	LONGLONG	str_llPrice;
	DWORD		str_dwPeriod;
	DWORD		str_dwdwCount;
}REGISTER_PRODUCT,*PREGISTER_PRODUCT;

int registerproduct(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PREGISTER_PRODUCT ptheRegisterProduct = (PREGISTER_PRODUCT)lpParameter;
	return JFZR_RegisterProduct(pszsendpackebuffer,ptheRegisterProduct->str_wszName,ptheRegisterProduct->str_dwItemTabCategory,ptheRegisterProduct->str_dwItemSlotIndex,
		ptheRegisterProduct->str_llPrice,ptheRegisterProduct->str_dwPeriod,ptheRegisterProduct->str_dwdwCount);
}

bool JFZRActionSystem::RegisterProduct(wchar_t* pwszName, DWORD dwItemTabCategory, DWORD dwItemSlotIndex, LONGLONG llPrice, DWORD dwPeriod, DWORD dwCount)
{
	REGISTER_PRODUCT	theRegisterProduct = {0};
	memcpy(theRegisterProduct.str_wszName,pwszName,wcslen(pwszName)*2+2);
	theRegisterProduct.str_dwItemTabCategory = dwItemTabCategory;
	theRegisterProduct.str_dwItemSlotIndex = dwItemSlotIndex;
	theRegisterProduct.str_llPrice = llPrice;
	theRegisterProduct.str_dwPeriod = dwPeriod;
	theRegisterProduct.str_dwdwCount = dwCount;

	return send(&theRegisterProduct,registerproduct);
}

int requestmyproductlist(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_RequestMyProductList(pszsendpackebuffer);
}

bool JFZRActionSystem::RequestMyProductList()
{
	return send(NULL,requestmyproductlist);
}

typedef struct
{
	LONGLONG	str_llOidMarket;
}UN_REGISTER_PRODUCT,*PUN_REGISTER_PRODUCT;

int unregisterproduct(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PUN_REGISTER_PRODUCT ptheUnRegisterProduct = (PUN_REGISTER_PRODUCT)lpParameter;
	return JFZR_UnregisterProduct(pszsendpackebuffer,ptheUnRegisterProduct->str_llOidMarket);
}

bool JFZRActionSystem::UnregisterProduct(LONGLONG llOidMarket)
{
	UN_REGISTER_PRODUCT theUnRegisterProduct = {0};
	theUnRegisterProduct.str_llOidMarket = llOidMarket;
	return send(&theUnRegisterProduct,unregisterproduct);
}

typedef struct  
{
	LONGLONG	str_llMailSerial;
	BYTE		str_bIsFirst;
}GET_ATTACHMENTS,*PGET_ATTACHMENTS;

int getattachments(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PGET_ATTACHMENTS ptheGetAttachments = (PGET_ATTACHMENTS)lpParameter;
	return JFZR_GetAttachments(pszsendpackebuffer,ptheGetAttachments->str_llMailSerial,ptheGetAttachments->str_bIsFirst);
}

bool JFZRActionSystem::GetAttachments(LONGLONG llMailSerial, BYTE bIsFirst)
{
	GET_ATTACHMENTS theGetAttachments = {0};
	theGetAttachments.str_llMailSerial = llMailSerial;
	theGetAttachments.str_bIsFirst = bIsFirst;
	return send(&theGetAttachments,getattachments);
}


int mailremove(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PGET_ATTACHMENTS ptheGetAttachments = (PGET_ATTACHMENTS)lpParameter;
	return JFZR_MailRemove(pszsendpackebuffer,ptheGetAttachments->str_llMailSerial,ptheGetAttachments->str_bIsFirst);
}

bool JFZRActionSystem::MailRemove(LONGLONG llMailSerial, BYTE bIsFirst)
{
	GET_ATTACHMENTS theGetAttachments = {0};
	theGetAttachments.str_llMailSerial = llMailSerial;
	theGetAttachments.str_bIsFirst = bIsFirst;
	return send(&theGetAttachments,mailremove);
}


int activity(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_ACTOR_SYSTEM_ACTIVITY	ptheActivity = (PSEND_ACTOR_SYSTEM_ACTIVITY)lpParameter;
	return JFZR_Activity(pszsendpackebuffer,ptheActivity->dwType,ptheActivity->dwCount,ptheActivity->dwDefenderSerial,ptheActivity->dwAttackerSerial);
}

bool JFZRActionSystem::Activity(DWORD dwType,DWORD dwCount,DWORD dwDefenderSerial,DWORD dwdwAttackerSerial)
{
	SEND_ACTOR_SYSTEM_ACTIVITY	theActivity;
	theActivity.dwType	= dwType;
	theActivity.dwCount	= dwCount;
	theActivity.dwDefenderSerial	= dwDefenderSerial;
	theActivity.dwAttackerSerial	= dwdwAttackerSerial;
	return send(&theActivity,activity);
}

int requestproductinfo(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_REQUEST_PRODUCT_INFO ptheRequestProductInfo = (PSEND_REQUEST_PRODUCT_INFO)lpParameter;
	return JFZR_RequestProductInfo(pszsendpackebuffer,ptheRequestProductInfo->dwItemTabCategory,ptheRequestProductInfo->dwItemSlotIndex);
}

bool JFZRActionSystem::RequestProductInfo(DWORD dwItemTabCategory,DWORD dwItemSlotIndex)
{
	SEND_REQUEST_PRODUCT_INFO	theRequestProductInfo;
	theRequestProductInfo.dwItemTabCategory	= dwItemTabCategory;
	theRequestProductInfo.dwItemSlotIndex	= dwItemSlotIndex;

	return send(&theRequestProductInfo,requestproductinfo);
}


int openitempackage(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_OPEN_ITEM_PACKAGE ptheOpenItemPackage = (PSEND_OPEN_ITEM_PACKAGE)lpParameter;
	return JFZR_OpenItemPackage(pszsendpackebuffer,ptheOpenItemPackage->nItemPackageTabIndex,ptheOpenItemPackage->wItemPackageSlotIndex,ptheOpenItemPackage->nRequireItemTabIndex,ptheOpenItemPackage->wRequireItemSlotIndex);
}

bool JFZRActionSystem::OpenItemPackage(int			nItemPackageTabIndex,	// 物品栏
									   WORD			wItemPackageSlotIndex,	// 物品在第几格
									   int			nRequireItemTabIndex,	// 配合打开物品需要的物品栏  -1表示没有
									   WORD			wRequireItemSlotIndex	// 物品在第几格，如果不存在物品，则置0
									   )
{
	SEND_OPEN_ITEM_PACKAGE	theOpenItemPackage;
	theOpenItemPackage.nItemPackageTabIndex		= nItemPackageTabIndex;
	theOpenItemPackage.wItemPackageSlotIndex	= wItemPackageSlotIndex;
	theOpenItemPackage.nRequireItemTabIndex		= nRequireItemTabIndex;
	theOpenItemPackage.wRequireItemSlotIndex	= wRequireItemSlotIndex;
	return send(&theOpenItemPackage,openitempackage);
}

int useitem(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_USE_ITEM ptheUseItem = (PSEND_USE_ITEM)lpParameter;
	return JFZR_UseItem(pszsendpackebuffer,ptheUseItem->nTabIndex,ptheUseItem->wSlotIndex);
}

bool JFZRActionSystem::UseItem(	int			nTabIndex,	// 物品栏
								WORD		wSlotIndex	// 物品在第几格
									   )
{
	SEND_USE_ITEM	theUseItem;
	theUseItem.nTabIndex	= nTabIndex;
	theUseItem.wSlotIndex	= wSlotIndex;

	return send(&theUseItem,useitem);
}



int createnpcbyclient(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	PSEND_CREATE_NPC_BY_CLIENT ptheCreateNpcByClient = (PSEND_CREATE_NPC_BY_CLIENT)lpParameter;
	return JFZR_CreateNpcByClient(pszsendpackebuffer,ptheCreateNpcByClient->dwMasterSerial,ptheCreateNpcByClient->dwRequestSerial);
}

bool JFZRActionSystem::CreateNpcByClient(	DWORD			dwMasterSerial,
											DWORD			dwRequestSerial
							   )
{
	SEND_CREATE_NPC_BY_CLIENT	theCreateNpcByClient;
	theCreateNpcByClient.dwMasterSerial = dwMasterSerial;
	theCreateNpcByClient.dwRequestSerial = dwRequestSerial;

	return send(&theCreateNpcByClient,createnpcbyclient);
}


int destroyactors(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{

	return JFZR_DestroyActors(pszsendpackebuffer,(DWORD)lpParameter);
}

bool JFZRActionSystem::DestroyActors(DWORD dwSerial)
{
	if( dwSerial == 0 )
	{
		assert(false);
		return false;
	}
	return send((LPVOID)dwSerial,destroyactors);
}

int test(LPVOID lpParameter,OUT char* pszsendpackebuffer)
{
	return JFZR_Test(pszsendpackebuffer,(DWORD)lpParameter);
}

 bool JFZRActionSystem::Test(DWORD dwIndex)
{
	return send((LPVOID)dwIndex,test);
}
















