#include "StdAfx.h"
#include "MMOClientTraverse.h"
#include "public/GameStructs.h"
#include "public/UsefulFns.h"


typedef void (*PFUN_PRINT)(DWORD lpSubTreeRoot);

void Traverse( DWORD lpSubTreeRoot, DWORD dwNullNode, PFUN_PRINT pFunPrint)
{
	if ( lpSubTreeRoot != NULL )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theNode,sizeof(theNode));

		if( lpSubTreeRoot != dwNullNode )
		{
			pFunPrint(lpSubTreeRoot);
		}

		if( lpSubTreeRoot != dwNullNode )
		{
			Traverse( theNode.str_dwNextAddrLeft,dwNullNode,pFunPrint );
			Traverse( theNode.str_dwNextAddrRight,dwNullNode,pFunPrint );
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// 角色信息
MMOActor::MMOActor()
{
	m_dwActorObjAddr = 0;
	m_dwActorPacketId = 0;
	m_ptheActorVector = NULL;
}

MMOActor::~MMOActor()
{

}


// dd [[[[0x0DAC8EF0]+0xc8]+0x1C]+0x214]
bool MMOActor::ReadActor()
{
	try
	{
		char szPrint[256] = {0};

		// [[[[0x01979780  +0C370000]+0x000000D0]+0x1C]+0x214]
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

		int nLevel = GetDecryValue(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_LEVEL_OFFSET);

		m_dwActorId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);
		m_nActorXianLu = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_XIAN_LU);

		m_dwActorObjAddr = GetActorObjAddr(m_dwActorId);

		sprintf_s(szPrint,"[LOG] addr = 0x%p",m_dwActorObjAddr);
		OutputDebugStringA(szPrint);
		if( m_dwActorObjAddr == 0 )
		{
			assert(false);
		}

		DWORD dwActorAddr = *(DWORD*)(m_dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
		assert(dwActorAddr);
		m_ptheActorVector = (MMO_ActorVector*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);

		m_dwActorPacketId = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___);


		DWORD dwExpAddr = dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_EXP_OFFSET1;
		int nSingleExp = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_SINGLE_EXP_OFFSET___);
		int nDoubleExp = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_DOUBLE_EXP_OFFSET___);
		int nCurExp = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_CUR_EXP_OFFSET___);
		m_nLieShouXuKeZhengCount = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_LIE_SHOU_XU_KE_ZHENG_COUNT_OFFSET1);		// 猎守许可证
		int nMemoryBangDing = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_MEMORY_BANG_DING_OFFSET);		// 绑定金币
		int nMemoryLiuTong = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_MEMORY_LIU_TONG_OFFSET);			// 流通金币	
		//int nMemoryLieBi = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_MEMORY_LIE_BI_OFFSET);				// 列币
		int nMemoryJingSuanDian = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_MEMORY_JING_SUAN_DIAN_OFFSET);		// 精算点
		int nEncryPtionLevel = GetDecryValue(dwExpAddr+MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_LEVEL_OFFSET___);			// 加密的角色等级
		int nCurHp=GetDecryValue(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_CURHP_OFFSET___);
		int nCurNeishang=GetDecryValue(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_CURNEISHANG_OFFSET___); //当前内伤

		int nWeekCreditCount = 0;


		// 武器斩味值 小于 700000，为粉红色，钝口刀型  可设定为需要磨刀
		int nZhanWeiZhi = GetDecryValue(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_ZHAN_WEI_ZHI_OFFSET);

		//EncryptionValue_int(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_ZHAN_WEI_ZHI_OFFSET,1000000);

		// 当炼气槽值=100时，则炼气槽满，右键释放，然后连续右键
		int nLianQiCao = GetDecryValue(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_LIAN_QI_CAO_OFFSET);

		// 写入炼气槽，无限使用
		EncryptionValue_int(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_LIAN_QI_CAO_OFFSET,100);

		int nZhongDuState = *(int*)(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_ZHONG_DU_STATE_OFFSET___);


		sprintf_s(szPrint,"[LOG]  血量=%d  内伤=%d 自由狩猎许可证 = %d 周可领取狩猎券 = %d 今日剩余单倍经验 = %d 今日剩余双倍经验 = %d 当前经验 = %d 玩家等级 = %d 邦定金币 = %d 流通金币 = %d 斩味值 = %d 炼气槽 = %d 中毒状态 = %d 精算点 = %d\r\n",
			nCurHp,nCurNeishang,m_nLieShouXuKeZhengCount,nWeekCreditCount,nSingleExp,nDoubleExp,nCurExp,nLevel,nMemoryBangDing,nMemoryLiuTong,nZhanWeiZhi,nLianQiCao,nZhongDuState,nMemoryJingSuanDian);
		OutputDebugStringA(szPrint);
		return true;
	}
	catch (...)
	{
		assert(false);
	}

	return false;
}


// dd [[[0x01979780+0C370000]+0xD0]+0x08]
CString MMOActor::GetRoleInMap()
{
	CString csRet;
	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_IN_MAP_OFFFSET2);
		char* pszRoleInMap = (char*)*(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_IN_MAP_OFFFSET3___);

		DWORD dwInMapFbId = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ATCOR_IN_MAP_FB_ID_OFFSET___);
		DWORD dwInMapMuBanId = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ID_OFFSET___);
		DWORD dwMapMuBanAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ADDR_OFFSET___);

		char* pszMuBanName = "";
		if( dwMapMuBanAddr != 0 )
		{
			pszMuBanName = (char*)*(DWORD*)(dwMapMuBanAddr+MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ADDR_NAME_OFFSET___);
		}

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] %s fb id = 0x%p 模板地图 id = 0x%p addr = 0x%p name = %s room Index = %d\r\n",pszRoleInMap,dwInMapFbId,dwInMapMuBanId,dwMapMuBanAddr,pszMuBanName,GetMsgRoleCityRoomIndex());
		OutputDebugStringA(szPrint);

		csRet += szPrint;
	}
	catch (...)
	{

	}
	return csRet;
}

void NpcWirtePos(DWORD dwObjAddr1,float fx, float fy, float fz)
{
	try
	{
		MMO_Vector3* ptheActorVector1 = (MMO_Vector3*)(dwObjAddr1+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR2_OFFSET___);

		ptheActorVector1->fx = fx;
		ptheActorVector1->fy = fy;
		ptheActorVector1->fz = fz;

		MMO_ActorVector* ptheActorVector2 = (MMO_ActorVector*)(dwObjAddr1+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);

		ptheActorVector2->fx = fx;
		ptheActorVector2->fy = fy;
		ptheActorVector2->fz = fz;

	}
	catch (...)
	{

	}


}

void MMOActor::WirtePos(float fx, float fy, float fz, float fdir)
{
	if( m_dwActorObjAddr == 0 )
	{
		return ;
	}
	DWORD dwActorAddr = *(DWORD*)(m_dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
	assert(dwActorAddr);

	MMO_Vector3* ptheActorVector = (MMO_Vector3*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR2_OFFSET___);

	for (int i=0; i<1; i++)
	{
		ptheActorVector->fx = fx;
		ptheActorVector->fy = fy;
		ptheActorVector->fz = fz;


		m_ptheActorVector->fx = fx;
		m_ptheActorVector->fy = fy;
		m_ptheActorVector->fz = fz;


		// 		m_ptheActorVector->dir_1.fx = 0.0f;
		// 		m_ptheActorVector->dir_1.fy = 1.0f;
		// 		m_ptheActorVector->dir_2.fx = -1.0f;
		// 		m_ptheActorVector->dir_2.fy = 0.0f;
	}

	return ;

	{
		MessageBoxA(NULL,"","",MB_OK);

		*(float*)(dwActorAddr+0x60) = -1.0f;
		*(float*)(dwActorAddr+0x64) = 0.0f;
		*(float*)(dwActorAddr+0x70) = 0.0f;
		*(float*)(dwActorAddr+0x74) = -1.0f;

		MessageBoxA(NULL,"","",MB_OK);

		*(float*)(dwActorAddr+0x60) = 0.0f;
		*(float*)(dwActorAddr+0x64) = -1.0f;
		*(float*)(dwActorAddr+0x70) = 1.0f;
		*(float*)(dwActorAddr+0x74) = 0.0f;

		MessageBoxA(NULL,"","",MB_OK);

		m_ptheActorVector->dir_1.fx = 0.0f;
		m_ptheActorVector->dir_1.fy = 1.0f;
		m_ptheActorVector->dir_2.fx = -1.0f;
		m_ptheActorVector->dir_2.fy = 0.0f;

		MessageBoxA(NULL,"","",MB_OK);

		*(float*)(dwActorAddr+0x60) = 0.0f;
		*(float*)(dwActorAddr+0x64) = 1.0f;
		*(float*)(dwActorAddr+0x70) = -1.0f;
		*(float*)(dwActorAddr+0x74) = 0.0f;
	}
}



void MMOActor::WirtePos2(float fx, float fy, float fz, float fdir)
{
	if( m_dwActorObjAddr == 0 )
	{
		return ;
	}
	DWORD dwActorAddr = *(DWORD*)(m_dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
	assert(dwActorAddr);

	MMO_Vector3* ptheActorVector = (MMO_Vector3*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR2_OFFSET___);

	for (int i=0; i<1; i++)
	{
		// 		ptheActorVector->fx = fx;
		// 		ptheActorVector->fy = fy;
		//		ptheActorVector->fz = fz;
		ptheActorVector->fz += 2.0f;

		// 		m_ptheActorVector->fx = fx;
		// 		m_ptheActorVector->fy = fy;
		//		m_ptheActorVector->fz = fz;
		m_ptheActorVector->fz += 2.0f;


		// 		m_ptheActorVector->dir_1.fx = 0.0f;
		// 		m_ptheActorVector->dir_1.fy = 1.0f;
		// 		m_ptheActorVector->dir_2.fx = -1.0f;
		// 		m_ptheActorVector->dir_2.fy = 0.0f;
	}

}
void MMOActor::WirtePos3(float fx, float fy, float fz, float fdir)
{
	if( m_dwActorObjAddr == 0 )
	{
		return ;
	}
	DWORD dwActorAddr = *(DWORD*)(m_dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
	assert(dwActorAddr);

	MMO_Vector3* ptheActorVector = (MMO_Vector3*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR2_OFFSET___);

	for (int i=0; i<1; i++)
	{
		// 		ptheActorVector->fx = fx;
		// 		ptheActorVector->fy = fy;
		//		ptheActorVector->fz = fz;
		ptheActorVector->fz += 8.0f;

		// 		m_ptheActorVector->fx = fx;
		// 		m_ptheActorVector->fy = fy;
		//		m_ptheActorVector->fz = fz;
		m_ptheActorVector->fz += 8.0f;


		// 		m_ptheActorVector->dir_1.fx = 0.0f;
		// 		m_ptheActorVector->dir_1.fy = 1.0f;
		// 		m_ptheActorVector->dir_2.fx = -1.0f;
		// 		m_ptheActorVector->dir_2.fy = 0.0f;
	}

}

#include <math.h>



void WritePos(float fx, float fy, float fz)
{
	// [[[[0C370000+0x01759EF0]+0xC8]+0x1C]+0x214]
	DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

	DWORD dwActorId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);
	DWORD dwActorObjAddr = GetActorObjAddr(dwActorId);
	DWORD dwActorAddr = *(DWORD*)(dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);

	MMO_Direction3* ptheCurActorPos = (MMO_Direction3*)(dwActorAddr+0x38);

	// 	if( fabs(fx) != 0 )
	// 		ptheCurActorPos->fx = fx;
	// 	if( fabs(fy) != 0 )
	// 		ptheCurActorPos->fy = fy;
	// 	if( fabs(fz) != 0 )
	// 		ptheCurActorPos->fz = fz;

	ptheCurActorPos->fz = 100.0f;


	//	assert(false);
	//WritePos_(dwActorAddr);
}


//////////////////////////////////////////////////////////////////////////

CString csRet;

DWORD g_dwActorObjAddr = 0;

//////////////////////////////////////////////////////////////////////////
// 玩家，NPC遍历


void PrintObj(DWORD lpSubTreeRoot)
{
	MMO_ACTOR theActor;
	_ReadProcessMemory(lpSubTreeRoot, &theActor, sizeof(theActor));

	DWORD dwObjStatus = *(DWORD*)(theActor.dwObjAddr + MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_STATUS_OFFSET___);

	DWORD dwActorAddr = *(DWORD*)(theActor.dwObjAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
	assert(dwActorAddr);
	auto old_addr = dwActorAddr;

	MMO_Direction3*	 ptheActorDirVector = (MMO_Direction3*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_DIR_OFFSET___);
	MMO_ActorVector* ptheActorVector = (MMO_ActorVector*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);

	char* pszObjName = (char*)*(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFFSET_NAME___);

	WORD wType = *(WORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___);

	DWORD dwObjPacketId = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___);

	int nNpcType = -1;
	char* pszObjTile = "";
	if (MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___ == wType)
	{
		// dd [[[[[0x6A58EF78+0x0C]+0x104]+0x40]+0x18]+0x0CFC+0x30]+0x18+4  // NPC名字
		// dd [[[[[0x6A58EF78 +0x0C]+0x104]+0x40]+0x18]+0x0CFC+0x30]+0x1c+4  // NPC类型
		// dd [[[[[0x6A58EF78+0x0C]+0x100]+0x40]+0x18]+0x0CFC]+0x18  // NPC名字
		// dd [[[[[0x6A58EF78+0x0C]+0x100]+0x40]+0x18]+0x0CFC]+0x1c  // NPC类型
		if (dwActorAddr != NULL)
			dwActorAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME1___);
		/*if (dwActorAddr != NULL)
			dwActorAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME2___);
		if (dwActorAddr != NULL)
			dwActorAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME3___);*/
		if (dwActorAddr != NULL)
			dwActorAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME4);
		if (dwActorAddr != NULL)
			nNpcType = *(int*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NPC_TYPE___);
		if (dwActorAddr != NULL)
		{
			DWORD dwActorNameAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME5___);

			if (dwActorNameAddr != 0)
			{
				pszObjName = (char*)dwActorNameAddr;
			}

			// 读取称号，并不是每一个对象都有称号的
			DWORD dwActorTitleNameAddr = *(DWORD*)(dwActorAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE5___);
			if (dwActorTitleNameAddr != 0)
			{
				dwActorTitleNameAddr = *(DWORD*)(dwActorTitleNameAddr + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE6___);
				if (dwActorTitleNameAddr != 0)
				{
					pszObjTile = (char*)dwActorTitleNameAddr;
				}
			}
		}
	}

	DWORD dwObjAddr2 = GetObjAddr(dwObjPacketId);		// 获取对象的另一个地址
	int nRoomIndex = 0;
	if (dwObjAddr2 != 0)
	{
		nRoomIndex = *(int*)(dwObjAddr2 + MMOCLIENT_REVERSAL_TRAVERSE_OBJ_ROOM_IXDEX_OFFSET___);
	}

	int nZouLuStatus = *(int*)(theActor.dwObjAddr + 0x1750);

	char szPrint[1024] = { 0 };
	sprintf_s(szPrint, sizeof(szPrint), "[LOG] 0x%p addr = 0x%p addr1 = 0x%p addr2 = 0x%p addr3 = 0x%p id = %p packet id = 0x%p room = %d status = %d %d name = %s title = %s type = %p npc type = %d dir = %f %f dir1 = %f %f dir2 = %f %f  x = %f y = %f z = %f\r\n",
		lpSubTreeRoot,
		theActor.dwObjAddr,			// 可以访问到对象的坐标等信息的地址
		old_addr, dwActorAddr,
		dwObjAddr2,					// 对象的另一个地址
		theActor.dwObjId,					// 本地表中的查询id
		dwObjPacketId,				// 与服务器通信的id
		nRoomIndex,					// 对象所在房间的序号，副本内发包过图需要用到
		dwObjStatus,				// 对象战斗状态 = 3 收刀状态 = 0 使用技能状态 = 5
		nZouLuStatus,
		pszObjName,					// 对象的名字
		pszObjTile,					// 对象称号
		wType,						// 对象的class，NPC = 00000497  详细数据见宏信息 ：MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___
		nNpcType,					// NPC对的细分类型，普通怪物 = 2，boos = 1，箱子 = 3，玩家和玩家的宠物不存在
		ptheActorDirVector->fx,
		ptheActorDirVector->fy,
		ptheActorVector->dir_1.fx,	// 对象的面向数据
		ptheActorVector->dir_1.fy,
		ptheActorVector->dir_2.fx,
		ptheActorVector->dir_2.fy,
		ptheActorVector->fx,		// 对象的坐标
		ptheActorVector->fy,
		ptheActorVector->fz
		);

	OutputDebugStringA(szPrint);

	csRet += szPrint;
}

void PrintObj2(DWORD lpSubTreeRoot)
{
	try
	{
		DWORD dwActorRoot = lpSubTreeRoot;

		DWORD dwObjStatus = *(DWORD*)(dwActorRoot+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_STATUS_OFFSET___);

		DWORD dwActorAddr = *(DWORD*)(dwActorRoot+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
		assert(dwActorAddr);

		MMO_Direction3*	 ptheActorDirVector = (MMO_Direction3*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_DIR_OFFSET___);
		MMO_ActorVector* ptheActorVector = (MMO_ActorVector*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);

		char* pszObjName = (char*)*(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFFSET_NAME___);

		WORD wType = *(WORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___);

		DWORD dwObjPacketId = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___);

		int nNpcType = -1;
		char* pszObjTile = "";
		if( MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___ == wType )
		{
			// dd [[[[[0x54D054D8+0x0C]+0x100]+0x40]+0x18]+0x0CFC]+0x18  // NPC名字
			// dd [[[[[0x2B6824A0+0x0C]+0x100]+0x40]+0x18]+0x0CFC]+0x1c  // NPC类型
			try
			{
				if( dwActorAddr != NULL )
					dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME1___);
				/*if( dwActorAddr != NULL )
					dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME2___);
				if( dwActorAddr != NULL )
					dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME3___);*/
				if( dwActorAddr != NULL )
					dwActorAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME4);
				if( dwActorAddr != NULL )
					nNpcType = *(int*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NPC_TYPE___);
				if( dwActorAddr != NULL )
				{
					DWORD dwActorNameAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME5___);

					if( dwActorNameAddr != 0 )
					{
						pszObjName = (char*)dwActorNameAddr;
					}

					// 读取称号，并不是每一个对象都有称号的
					DWORD dwActorTitleNameAddr = *(DWORD*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE5___);
					if( dwActorTitleNameAddr != 0 )
					{
						dwActorTitleNameAddr = *(DWORD*)(dwActorTitleNameAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE6___);
						if( dwActorTitleNameAddr != 0 )
						{
							pszObjTile = (char*)dwActorTitleNameAddr;
						}
					}
				}


			}
			catch (...)
			{
				assert(false);
			}
		}

		DWORD dwObjAddr2 = GetObjAddr(dwObjPacketId);		// 获取对象的另一个地址
		int nRoomIndex = 0;
		if( dwObjAddr2 != 0 )
		{
			nRoomIndex = *(int*)(dwObjAddr2+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_ROOM_IXDEX_OFFSET___);
		}

		int nZouLuStatus = *(int*)(dwActorRoot+0x1750);

		char szPrint[1024] = {0};
		sprintf_s(szPrint,sizeof(szPrint),"[LOG] 0x%p addr = 0x%p addr2 = 0x%p id = %p packet id = 0x%p room = %d status = %d %d name = %s title = %s type = %p npc type = %d dir = %f %f dir1 = %f %f dir2 = %f %f  x = %f y = %f z = %f\r\n",
			lpSubTreeRoot,
			dwActorRoot,			// 可以访问到对象的坐标等信息的地址
			dwObjAddr2,					// 对象的另一个地址
			0,					// 本地表中的查询id
			dwObjPacketId,				// 与服务器通信的id
			nRoomIndex,					// 对象所在房间的序号，副本内发包过图需要用到
			dwObjStatus,				// 对象战斗状态 = 3 收刀状态 = 0 使用技能状态 = 5
			nZouLuStatus,
			pszObjName,					// 对象的名字
			pszObjTile,					// 对象称号
			wType,						// 对象的class，NPC = 00000497  详细数据见宏信息 ：MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___
			nNpcType,					// NPC对的细分类型，普通怪物 = 2，boos = 1，箱子 = 3，玩家和玩家的宠物不存在
			ptheActorDirVector->fx,
			ptheActorDirVector->fy,
			ptheActorVector->dir_1.fx,	// 对象的面向数据
			ptheActorVector->dir_1.fy,
			ptheActorVector->dir_2.fx,
			ptheActorVector->dir_2.fy,
			ptheActorVector->fx,		// 对象的坐标
			ptheActorVector->fy,
			ptheActorVector->fz
			);

		OutputDebugStringA(szPrint);

		csRet += szPrint;


	}
	catch (...)
	{
		assert(false);
	}
}

CString TraverseObj()
{
	csRet = "";

	try
	{
		// 192CB6C
		// dd [[[0x0192CB6C+0C370000]+0x0C]+0x4F4]
		DWORD dwMapAddr = *(DWORD*)(UPDATES_MMOCLIENT_REVERSAL_GAME_RAND_BASE_ADDR+GetCryGameBaseAddr());
		//dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET1);
		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET2);
		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET3);
		dwMapAddr += MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET4___;


		NODE	theNode = {0};
		_ReadProcessMemory(dwMapAddr,&theNode,sizeof(theNode));
		Traverse(theNode.str_dwNextAddrParent,dwMapAddr,PrintObj);


		//		先遣版本分析出来的数组遍历  0x1758EF0
		// 		DWORD dwMapAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		// 		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET1);
		// 		dwMapAddr = *(DWORD*)(dwMapAddr+MMOCLIENT_REVERSAL_GAME_OFFSET2);
		// 		dwMapAddr = *(DWORD*)(dwMapAddr+0x51C);
		// 
		// 		PGAME_STRUCT_ARRAY_POINT ptheDoorObjAddr = (PGAME_STRUCT_ARRAY_POINT)(dwMapAddr+0xC4);
		// 
		// 		for (DWORD dwGameDoorAddr=ptheDoorObjAddr->str_dwBeginAddr; dwGameDoorAddr<ptheDoorObjAddr->str_dwEndAddr1; dwGameDoorAddr+=4)
		// 		{
		// 			DWORD dwTempGameDoorAddr = *(DWORD*)dwGameDoorAddr;
		// 			if( dwTempGameDoorAddr != 0 )
		// 				PrintObj2(dwTempGameDoorAddr);
		// 		}

	}
	catch (...)
	{
		assert(false);
	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// 遍历角色血值，放弃不用

#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET1___	0x174
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET2___	0x4
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET3___	0x18

#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET1___	0x0C
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2___	0x08
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2_1___	0x2c
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2_2___	0x0C
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET3___	0x10
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET4___	0x0
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET5___	0x38
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET6___	0x0c
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_ID___		0x08
#define MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_HP___		0x4D4


// 副本内遍历角色和队友的血值，通过id判断是哪个对象的，有部分对象是不同的对象，没有分析出来是什么对象，需要异常接受忽略掉
// 此函数游戏在访问时加锁了，如有需要在提供
CString TraversePlayer()
{
	// [[[[[0x0258590C]+0x4]+0x174]+0x4]+0x18]
	DWORD dwPlayerAddrBuf = 0;
	try
	{
		dwPlayerAddrBuf = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwPlayerAddrBuf = *(DWORD*)(dwPlayerAddrBuf+MMOCLIENT_REVERSAL_GAME_OFFSET1);
		dwPlayerAddrBuf = *(DWORD*)(dwPlayerAddrBuf+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET1___);
		dwPlayerAddrBuf = *(DWORD*)(dwPlayerAddrBuf+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET2___);
		dwPlayerAddrBuf = *(DWORD*)(dwPlayerAddrBuf+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_BUF_OFFSET3___);

		if( dwPlayerAddrBuf == 0 )
		{
			return _T("");
		}
	}
	catch (...)
	{
		return _T("");
	}

	CString csRet;
	try
	{
		__asm
		{
			mov eax, eax
				mov ebx ,ebx
				mov ecx, ecx
				mov edx, edx
		}
		for (int i=0; i<256; i++)
		{
			// [[[[[[esi]+0x8]+0x10]+0]+0x38]+0x0c]
			DWORD dwObjAddr = dwPlayerAddrBuf+i*8+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET1___;

			DWORD dwObjAddrValue = *(DWORD*)dwObjAddr;

			if( dwObjAddr != dwObjAddrValue )
			{
				TCHAR szPrint[256] = {0};
				_stprintf_s(szPrint, _T("[LOG] addr = 0x%p\r\n"), dwObjAddrValue);
				OutputDebugString(szPrint);

				dwObjAddrValue = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2___);
				if( dwObjAddrValue != 0 )
				{	
					int nFlag = *(int*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2_1___);

					if( nFlag != -1 )
					{
						nFlag = *(int*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET2_2___);

						if( nFlag != 0 )
						{
							dwObjAddrValue = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET3___);

							if( dwObjAddrValue != 0 )
							{
								try
								{
									dwObjAddrValue = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET4___);
									dwObjAddrValue = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET5___);
									dwObjAddrValue = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_OFFSET6___);

									DWORD dwPlayerId = *(DWORD*)(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_ID___);
									int nPlayerHp = GetDecryValue(dwObjAddrValue+MMOCLIENT_REVERSAL_TRAVERSE_PLAYER_HP___);

									_stprintf_s(szPrint, _T("[LOG] addr = 0x%p id = 0x%p hp = %d\r\n"), dwObjAddrValue, dwPlayerId, nPlayerHp);
									OutputDebugString(szPrint);

									csRet += szPrint;
								}
								catch (...)
								{

								}
							}
						}
					}
				}
			}
		}
	}
	catch (...)
	{

	}

	return csRet;
}





//////////////////////////////////////////////////////////////////////////
// 遍历背包，装备，仓库

// dd [[[[[[[0x0258590C]+0xc8]+0x28]+0x214]+0x42f4]+0x0C]+0x18]+0x08

CString GetItemInfo(DWORD dwItemBaseAddr)
{

	CString csRet;

	try
	{
		char szPrint[256] = {0};
		int nCount = *(int*)(dwItemBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_COUNT_OFFSET___);

		sprintf_s(szPrint,"[LOG] 有效格子 = %d\r\n",nCount);
		csRet += szPrint;


		if( dwItemBaseAddr != 0 )
		{
			int nItemCount = *(int*)(dwItemBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_COUNT_OFFSET__);

			DWORD dwItemAddr = *(DWORD*)(dwItemBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OFFSET1___);
			dwItemAddr += MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OFFSET2___;

			PMM0_ITEM ptheItemAddr = (PMM0_ITEM)dwItemAddr;

			for (int i=0; i<nItemCount; i++)
			{
				if( ptheItemAddr->dwItemAddr != 0 )
				{
					int nCount = *(int*)(ptheItemAddr->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_COUNT_OFFSET___);

					DWORD dwItemObjAddr = *(DWORD*)(ptheItemAddr->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_ADDR_OFFSET___);

					if( dwItemObjAddr != 0 )
					{
						if( *(DWORD*)(dwItemObjAddr) != 0 )
						{
							//DWORD dwItemPacketId = *(DWORD*)(dwItemObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_PACKET_ID_OFFSET___);
							char* pszItemName = (char*)*(DWORD*)(dwItemObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___);
							DWORD dwKey1 = *(DWORD*)(ptheItemAddr->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_PACKET_KEY1_OFFSET___);
							DWORD dwKey2 = *(DWORD*)(ptheItemAddr->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_PACKET_KEY2_OFFSET___);

							// 物品绑定  未绑定=0 装备后角色绑定=1 已绑定=2
							int	nBangDingFlag = *(int*)(ptheItemAddr->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_BANG_DING_OFFSET___);

							int nItemLevel = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_LEVEL_OFFSET___);						// 物品等级

							int nItemMax = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_YONG_YOU_SHANG_XIAN_OFFSET___);			// 拥有上限

							// 白色=1 绿色=2 淡玫红=3 蓝色=4 深玫红=5 紫色=6 橘黄色=7 金黄色=8 其他=大红色
							int nItemYanSe = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_YAN_SE_OFFSET___);					// 物品颜色，表示等级
							int nItemXieDaiMax = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_XIE_DAI_SHANG_XIAN_OFFSET___);	// 携带上限
							int nItemDuiDieMax = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_DUI_DIE_SHANG_XIAN_OFFSET___);	// 堆叠上限

							/*++
							物品描述		  		物品类型		部位类型1		部位类型2
							近程武器 大剑				2				1				1
							近程武器 锤子				2				1				2
							近程武器 长枪				2				1				3
							近程武器 片手				2				1				4	
							远程武器 弩 				2				1				5	
							近程武器 双刀				2				1				6	
							近程武器 太刀				2				1				7
							近程武器 笛子				2				1				8
							远程武器 弓					2				1				10

							近程防具 头部				2				2				1
							近程防具 腕部				2				2				2
							近程防具 胸部				2				2				3
							近程防具 腰部				2				2				4
							近程防具 腿部				2				2				5
							通用首饰 戒指				2				3				1
							通用首饰 项链				2				3				2
							通用首饰 护石				2				3				4

							素材						1				8				1
							素材2						1				8				2
							--*/

							int nItemWuPinLeiXing = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_OFFSET___);	// 物品类型，用去区分装备和消耗品之类
							int nItemBuWeiLeiXing = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_BU_WEI_LEI_XING_OFFSET___);	// 部位类型，用去区分武器和防具
							int nItemWuWeiLeiXing2 = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_SUO_OFFSET___);	// 部位类型2,用于区分是戴在哪个部位


							int nItemUseLevel = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITME_USE_LEVEL_OFFSET);				// 角色使用物品需要的等级
							assert(0<=nItemUseLevel);
							assert(nItemUseLevel<99);
							DWORD dwPacketId = *(DWORD*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___);
							int nItemFenJieFlag = *(int*)(dwItemObjAddr+MMOCLIENT_REVERSAL_ITEM_WU_PIN_FEN_JIE_OFFSET___);		// 分解物品


							sprintf_s(szPrint,"[LOG] i = %d Item addr = 0x%p id = 0x%p name = %s count = %d key = 0x%p 0x%p price = %d 分解 = %d 绑定 = %d",
								i,
								ptheItemAddr->dwItemAddr,
								dwPacketId,
								pszItemName,
								nCount,
								dwKey1,
								dwKey2,
								GetSellItemPrice(dwItemObjAddr),		// 获取物品的价钱  如果物品价值=0，则为不可出售，其余为出售价格
								nItemFenJieFlag,
								nBangDingFlag
								);
							OutputDebugStringA(szPrint);

							csRet += szPrint;

							sprintf_s(szPrint," 等级 = %d 拥有上限 = %d 物品颜色 = %d 携带上限 = %d 堆叠上限 = %d 物品类型 = %d 部位类型1 = %d 物品类型2 = %d 角色等级  = %d\r\n",
								nItemLevel,nItemMax,nItemYanSe,nItemXieDaiMax,nItemDuiDieMax,nItemWuPinLeiXing,nItemBuWeiLeiXing,nItemWuWeiLeiXing2,nItemUseLevel);
							csRet += szPrint;
						}
					}
				}

				ptheItemAddr++;
			}
		}
	}
	catch (...)
	{

	}

	return csRet;

}


CString TraverseItem()
{
	CString csRet;

	DWORD dwGeziBaseAddr = 0;
	try
	{
		// dd [[[[0xDAC8EF0]+0xc8]+0x1C]+0x214]+0x4288
		dwGeziBaseAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGeziBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGeziBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGeziBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		dwGeziBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_OFFSET1);

		assert(dwGeziBaseAddr);

		char szPrint[256];
		sprintf_s(szPrint,"addr = 0x%p\r\n",dwGeziBaseAddr);
		csRet += szPrint;
		if( dwGeziBaseAddr != 0 )
		{
			csRet += "******************************我是背包物品******************************\r\n";
			DWORD dwItemBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_ITEM_OFFSET1___);

			csRet += GetItemInfo(dwItemBaseAddr);
		}

		if( dwGeziBaseAddr != 0 )
		{
			csRet += "******************************我是任务物品******************************\r\n";
			DWORD dwItemBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_TASK_OFFSET1___);

			csRet += GetItemInfo(dwItemBaseAddr);
		}

		if( dwGeziBaseAddr != 0 )
		{
			csRet += "******************************我是装备物品******************************\r\n";
			DWORD dwEquipBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_EQUIP_OFFSET1___);
			csRet += GetItemInfo(dwEquipBaseAddr);
		}


		if( dwGeziBaseAddr != 0 )
		{
			csRet += "******************************我是普通仓库物品******************************\r\n";
			DWORD dwItemBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_CANG_KU_OFFSET1___);

			csRet += GetItemInfo(dwItemBaseAddr);
		}

		if( dwGeziBaseAddr != 0 )
		{
			csRet += "******************************我是素材仓库物品******************************\r\n";
			DWORD dwItemBaseAddr = *(DWORD*)(dwGeziBaseAddr+MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_CANG_KU_SU_CAI_OFFSET1___);

			csRet += GetItemInfo(dwItemBaseAddr);
		}
	}
	catch (...)
	{
		return csRet;
	}

	return csRet;
}








//////////////////////////////////////////////////////////////////////////
// 获取物品模板数据

CString GetTemplateItemInfo()
{
	CString csRet;

	try
	{
		char szPrint[1024];

		for (DWORD i=0; i<0x20000; i++)
		{
			DWORD dwItemInfoAddr = GetTemplateItemAddr(i);

			CString csEquip;
			if( dwItemInfoAddr != 0 )
			{
				char szUpEquipPrint[256] ={0};

				int nItemWuPinLeiXing = *(int*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_OFFSET___);	// 物品类型，用去区分装备和消耗品之类
				int nItemBuWeiLeiXing = *(int*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_BU_WEI_LEI_XING_OFFSET___);	// 部位类型，用去区分武器和防具
				int nItemWuWeiLeiXing2 = *(int*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_SUO_OFFSET___);	// 部位类型2,用于区分是戴在哪个部位


				int nItemUseLevel = *(int*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITME_USE_LEVEL_OFFSET);				// 角色使用物品需要的等级
				int nItemGetUpItemArg=*(int *)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_GETUPITEMARG_OFFSET____);	
				assert(0<=nItemUseLevel);
				assert(nItemUseLevel<1000);

				if( nItemWuPinLeiXing == 2 )		// 判断物品是否为装备
				{
					//PMMO_MATERIAL_ITEM
					DWORD	dwUpEquipInfoAddr = *(DWORD*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_UP_EQUIP_INFO_ADDR_OFFSET___);
					if( dwUpEquipInfoAddr != 0 )
					{
						PGAME_STRUCT_ARRAY_POINT ptheEquipInfo = (PGAME_STRUCT_ARRAY_POINT)(dwUpEquipInfoAddr+MMOCLIENT_REVERSAL_ITEM_UP_EQUIP_MATERIAL_INFO_OFFFSET___);

						for (PMMO_UP_EQUIP_ITEM_MATERIAL_INFO ptheCurUpEquipMaterialInfo = (PMMO_UP_EQUIP_ITEM_MATERIAL_INFO)ptheEquipInfo->str_dwBeginAddr;
							(DWORD)ptheCurUpEquipMaterialInfo < ptheEquipInfo->str_dwEndAddr1; ptheCurUpEquipMaterialInfo++)
						{

							sprintf_s(szUpEquipPrint," ==>升级装备后id = 0x%p ",ptheCurUpEquipMaterialInfo->dwUpEquipItemId);
							csEquip += szUpEquipPrint;

							for (PMMO_MATERIAL_ITEM ptheMaterial = (PMMO_MATERIAL_ITEM)ptheCurUpEquipMaterialInfo->theUpEquipMaterialInfo.str_dwBeginAddr;
								(DWORD)ptheMaterial < ptheCurUpEquipMaterialInfo->theUpEquipMaterialInfo.str_dwEndAddr1; ptheMaterial++ )
							{
								sprintf_s(szUpEquipPrint,"材料 id = 0x%p count = %d",ptheMaterial->dwMaterialItemId,ptheMaterial->nCount);
								csEquip += szUpEquipPrint;
							}
						}
					}
				}

				char* pszItemName = (char*)*(DWORD*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___);

				DWORD dwPacketId = *(DWORD*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___);

				sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p ItemUseLevel = %d name = %s %s\r\n",
					dwItemInfoAddr,dwPacketId,nItemUseLevel,pszItemName,csEquip);

				OutputDebugStringA(szPrint);



				csRet += szPrint;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// 获取任务模板信息

CString GetTaskInfo()
{
	CString csRet;

	char szError[] = "task error";

	char szPrint[0x1024] = {0};
	for ( int i=0; i<0x2000; i++)
	{
		DWORD dwTaskObjAddr = GetTaskAddr(i);

		if( dwTaskObjAddr != 0 )
		{
			char* pszTaskName = NULL;
			char* pszTaskNpcName = NULL;
			char* pszCompleteTaskNpcName = NULL;
			char* pszTaskDesc = NULL;

			DWORD dwTaskObjAddr_ = (*(DWORD*)(dwTaskObjAddr+MMOCLIENT_REVERSAL_TASK_OBJ_OFFSET1));

			if( dwTaskObjAddr_ != 0 )
			{
				try
				{
					pszTaskName = (char*)*(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_NAME_OFFSET___);
					pszTaskDesc = (char*)*(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_DESC_OFFSET___);
					pszTaskNpcName = (char*)*(DWORD*)((*(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1))+MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET2);
					pszCompleteTaskNpcName = (char*)*(DWORD*)((*(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1+4))+MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET2);

					if( pszTaskName == NULL )
					{
						pszTaskName = szError;
					}

					if( pszTaskDesc == NULL )
					{
						pszTaskDesc = szError;
					}

					if( pszTaskNpcName == NULL )
					{
						pszTaskNpcName = szError;
					}

					if( pszCompleteTaskNpcName == NULL )
					{
						pszCompleteTaskNpcName = szError;
					}
				}
				catch (...)
				{
					pszTaskName = szError;
					pszTaskDesc = szError;
					pszTaskNpcName = szError;
				}

				DWORD dwType = *(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_TYPE_OFFSET___);

				CString csFbDst;
				try
				{
					// 赏金任务的副本信息
					if( MMOCLIENT_REVERSAL_TASK_SHANG_JIN == dwType )
					{
						char szFbDstPrint[256];

						PGAME_STRUCT_ARRAY_POINT ptheDoTaskAddr = (PGAME_STRUCT_ARRAY_POINT)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_DO_TASK_DST_OFFSET1___);

						for(DWORD i = ptheDoTaskAddr->str_dwBeginAddr; i < ptheDoTaskAddr->str_dwEndAddr1; i+=4)
						{
							DWORD dwDoTaskAddr_i = *(DWORD*)i;

							PGAME_STRUCT_ARRAY_POINT ptheDoTaskAddrIndex = (PGAME_STRUCT_ARRAY_POINT)(dwDoTaskAddr_i+MMOCLIENT_REVERSAL_DO_TASK_DST_OFFSET2___);

							if( ptheDoTaskAddrIndex->str_dwBeginAddr != 0 )
							{
								for (DWORD j=ptheDoTaskAddrIndex->str_dwBeginAddr; j < ptheDoTaskAddrIndex->str_dwEndAddr1; j+=4)
								{
									DWORD dwDoTaskAddr_j = *(DWORD*)j;

									DWORD dwDoTaskFbId = *(DWORD*)(dwDoTaskAddr_j+MMOCLIENT_REVERSAL_DO_TASK_DST_FB_ID_OFFSET2___);

									if( dwDoTaskFbId != 0 )
									{
										sprintf_s(szFbDstPrint," ==> fb id = 0x%p",dwDoTaskFbId);
										csFbDst += szFbDstPrint;
									}
								}
							}
							else
							{

								DWORD dwDoTaskFbId = *(DWORD*)(dwDoTaskAddr_i+MMOCLIENT_REVERSAL_DO_TASK_DST_FB_ID_OFFSET1___);

								if( dwDoTaskFbId != 0 )
								{
									sprintf_s(szFbDstPrint," ==> fb id = 0x%p ",dwDoTaskFbId);
									csFbDst += szFbDstPrint;
								}
							}
						}
					}
				}
				catch (...)
				{

				}

				DWORD dwFbPlayerLevel = 0;
				DWORD dwMaxPlayerLevel = 0;
				try
				{
					// 取任务等级
					PGAME_STRUCT_ARRAY_POINT ptheTaskLevelAddr = (PGAME_STRUCT_ARRAY_POINT)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET1___);
					DWORD dwTaskLevelAddr = (*(DWORD*)ptheTaskLevelAddr->str_dwBeginAddr);
					dwTaskLevelAddr = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET2___);
					dwFbPlayerLevel = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_MIN_LEVEL_OFFSET___);
					dwMaxPlayerLevel = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_MAX_LEVEL_OFFSET___);
				}
				catch (...)
				{
					//assert(false);
				}

				CString csRewardPrint;
				try
				{
					char szRewardPrint[256] = {0};
					csRewardPrint = szRewardPrint;
					//if( 0x455 == i )
					{
						// 遍历任务奖励
						PGAME_STRUCT_ARRAY_POINT ptheTaskRewardAddr = (PGAME_STRUCT_ARRAY_POINT)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_OFFSET___);

						for (DWORD dwCurRewarAddr = ptheTaskRewardAddr->str_dwBeginAddr; dwCurRewarAddr < ptheTaskRewardAddr->str_dwEndAddr1; dwCurRewarAddr+=4)
						{
							if( dwCurRewarAddr != 0 )
							{
								PMMO_TASK_REWARD_ITEM ptheTaskRewardItem = (PMMO_TASK_REWARD_ITEM)*(DWORD*)dwCurRewarAddr;

								// 只处理奖励物品 对比物品的虚函数表地址来识别奖励物品   取消此段代码，特征码没有办法做  #define MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_VFUN_ADDR___		0x01D603A8
								//if( MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_VFUN_ADDR___ == ptheTaskRewardItem->dwVFunAddr )
								{
									// 用这个方法处理，没有异常出现，访问全部正确，最好加上异常忽略处理  
									if( ptheTaskRewardItem->theRewardItemAddr.str_dwBeginAddr > MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_CMP_VALUE___ 
										&& ptheTaskRewardItem->theRewardItemAddr.str_dwEndAddr1 > MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_CMP_VALUE___ 
										&& ptheTaskRewardItem->theRewardItemAddr.str_dwEndAddr2 > MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_CMP_VALUE___ )
									{
										for (DWORD dwRewardItemAddr = ptheTaskRewardItem->theRewardItemAddr.str_dwBeginAddr; dwRewardItemAddr < ptheTaskRewardItem->theRewardItemAddr.str_dwEndAddr1; dwRewardItemAddr+=4)
										{
											PMMO_TASK_REWARD_ITEM_INFO ptheRewardItemInfo = (PMMO_TASK_REWARD_ITEM_INFO)*(DWORD*)dwRewardItemAddr;
											DWORD dwItemId = *(DWORD*)(ptheRewardItemInfo->dwItemAddr+MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___);
											char* pszItemName = (char*)*(DWORD*)(ptheRewardItemInfo->dwItemAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___);

											sprintf_s(szRewardPrint," 奖励物品 id = 0x%p name = %s count = %d ",dwItemId,pszItemName,ptheRewardItemInfo->nCount);
											csRewardPrint += szRewardPrint;
											//OutputDebugStringA(szRewardPrint);
										}
									}
								}
							}
						}
					}
				}
				catch (...)
				{
					//assert(false);
					sprintf_s(szPrint,"[LOG] 异常id = 0x%p",i);
					OutputDebugStringA(szPrint);
				}




				try
				{
					sprintf_s(szPrint,sizeof(szPrint),"[LOG] task addr = 0x%p id = 0x%p type = 0x%p level = %d:%d npc = %s comnpc = %s name = %s%s %s desc = %s\r\n",
						dwTaskObjAddr,i,dwType,dwFbPlayerLevel,dwMaxPlayerLevel,pszTaskNpcName,pszCompleteTaskNpcName,pszTaskName,csFbDst,csRewardPrint,pszTaskDesc);

					csRet += szPrint;
				}
				catch (...)
				{
					assert(false);
				}
			}
		}
	}

	return csRet;
}


//////////////////////////////////////////////////////////////////////////
// 遍历可接任务


// 函数功能： 传入一个任务id，判断此任务是否可接

// 此函数暴力遍历，并显示所有可接任务，用于测试使用
CString GetReceivedNpcTask()
{
	CString csRet;
	char szPrint[256] = {0};
	// 	MMOActor theActor;
	// 	theActor.ReadActor();


	DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

	DWORD dwPlayerAddr = dwGameAddr2;


	if( dwPlayerAddr == 0 )
	{
		assert(false);
		return csRet;
	}
	for (int i=0; i<0x2000; i++)
	{
		// 已知要接的任务，传入id判断是否可接
		if( true == IsHaveTaskId_npc(dwPlayerAddr, i) )
		{
			DWORD dwTaskObjAddr = GetTaskAddr(i);

			if( dwTaskObjAddr != 0 )
			{
				char* pszTaskName = NULL;

				DWORD dwTaskObjAddr_ = (*(DWORD*)(dwTaskObjAddr+MMOCLIENT_REVERSAL_TASK_OBJ_OFFSET1));

				if( dwTaskObjAddr_ != 0 )
				{
					try
					{
						pszTaskName = (char*)*(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_NAME_OFFSET___);

						DWORD dwType = *(DWORD*)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_TYPE_OFFSET___);

						DWORD dwFbPlayerLevel = 0;
						DWORD dwMaxPlayerLevel = 0;

						try
						{
							// 取任务等级
							PGAME_STRUCT_ARRAY_POINT ptheTaskLevelAddr = (PGAME_STRUCT_ARRAY_POINT)(dwTaskObjAddr_+MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET1___);
							DWORD dwTaskLevelAddr = (*(DWORD*)ptheTaskLevelAddr->str_dwBeginAddr);
							dwTaskLevelAddr = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET2___);
							dwFbPlayerLevel = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_MIN_LEVEL_OFFSET___);
							dwMaxPlayerLevel = *(DWORD*)(dwTaskLevelAddr+MMOCLIENT_REVERSAL_TASK_MAX_LEVEL_OFFSET___);
						}
						catch (...)
						{
							//assert(false);
						}

						sprintf_s(szPrint,"[LOG] task addr = 0x%p id = 0x%p type = 0x%p level = %d:%d name = %s\r\n",dwTaskObjAddr,i,dwType,dwFbPlayerLevel,dwMaxPlayerLevel,pszTaskName);

						csRet += szPrint;
					}
					catch (...)
					{

					}


				}
			}
		}
	}

	return csRet;
}




//////////////////////////////////////////////////////////////////////////

CString g_csShangJinTaskInfo;


CString GetShangJinTaskInfo()
{
	g_csShangJinTaskInfo = "";

	try
	{
		char szPrint[256];
		sprintf_s(szPrint,sizeof(szPrint),"[LOG] total count = %d comple count = %d\r\n",GetShangJinTotalCount(),GetShangJinCompleteCount());
		g_csShangJinTaskInfo += szPrint;


		GAME_STRUCT_ARRAY_POINT theShangJinTask;
		GetShangJinTask(theShangJinTask);


		for (DWORD i=theShangJinTask.str_dwBeginAddr; i<theShangJinTask.str_dwEndAddr1; i+=sizeof(DWORD))
		{
			DWORD dwShangJinAddr = *(DWORD*)i;

			if( dwShangJinAddr != 0 )
			{
				DWORD dwShangJinTaskId = *(DWORD*)(dwShangJinAddr+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_ID_OFFSET___);
				DWORD dwShangJinTaskInfoAddr = *(DWORD*)(dwShangJinAddr+MMOCLIENT_REVERSAL_SHANG_JIN_TASK_INFO_ADDR_OFFSET___);


				char* pszTaskName = (char*)*(DWORD*)(dwShangJinTaskInfoAddr+MMOCLIENT_REVERSAL_TASK_NAME_OFFSET___);


				sprintf_s(szPrint,sizeof(szPrint),"[LOG] addr1 = 0x%p addr2 = 0x%p task id = 0x%p status = %d name = %s\r\n",
					theShangJinTask.str_dwBeginAddr,theShangJinTask.str_dwEndAddr1,dwShangJinTaskId,GetShangJinTaskStatus(dwShangJinTaskId),pszTaskName);
				OutputDebugStringA(szPrint);

				g_csShangJinTaskInfo += szPrint;

			}
		}
	}
	catch (...)
	{
		assert(false);

		g_csShangJinTaskInfo = "esh error!";
	}

	return g_csShangJinTaskInfo;
}

//////////////////////////////////////////////////////////////////////////
// UI界面采集物品的数量，测试用，暂不用理睬
#define MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET1___		0x514
#define MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET2___		0x30
#define MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET3___		0x54*4
#define MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET4___		0x64



typedef struct
{
	DWORD	dwVfunAddr;
	DWORD	dwDoTaskInfoAddr;
	DWORD	dwUnKnowAddr1;
	DWORD	dwUnKnowAddr2;
	char*	pszTaskId;
	DWORD	dwUnKnowZero1;
	DWORD	dwTaskUnKnowValue1;
	DWORD	dwUnKnowZero2;
	DWORD	dwTaskUnKnowValue2;
}MMO_RECEIVED_TASK_INFO,*PMMO_RECEIVED_TASK_INFO;



CString GetReceivedTaskInfo()
{
	// dd [[[[[[[[0x258590C]+0x04]+0x0c]+0x514]+0x30]+0x150*1]+0x64]+0x28]   // 访问任务对象

	CString csRet;

	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_GAME_OFFSET1);
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_GAME_OFFSET2);
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET1___);
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET2___);
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET3___);
		PGAME_STRUCT_ARRAY_POINT ptheArrayPoint = (PGAME_STRUCT_ARRAY_POINT)(dwGameAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_OFFSET4___);

		PMMO_RECEIVED_TASK_INFO ptheReceivedTaskInfo = (PMMO_RECEIVED_TASK_INFO)ptheArrayPoint->str_dwBeginAddr;

		char szPrint[256];
		for (; (DWORD)ptheReceivedTaskInfo<ptheArrayPoint->str_dwEndAddr1;ptheReceivedTaskInfo++ )
		{
			sprintf_s(szPrint,"[LOG] task addr = 0x%p %s\r\n",ptheReceivedTaskInfo,ptheReceivedTaskInfo->pszTaskId);
			OutputDebugStringA(szPrint);

			csRet += szPrint;
		}
	}
	catch (...)
	{

	}
	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// UI界面显示的中文查询函数

DWORD GetUiInfoAddr(DWORD dwUiId)
{
	DWORD dwUiInfoAddr = 0;
	char* pszUiString = "CMessageInfo";

	try
	{
		DWORD dwGetUiInfoBaseAddrCall = MMOCLIENT_REVERSAL_GET_UI_INFO_BASE_ADDR_CALL+GetCryGameBaseAddr();
		DWORD dwGetMuBanInfoAddrCall = MMOCLIENT_REVERSAL_GET_MU_BAN_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			push 0
				push pszUiString
				push 0
				MOV EAX, dwGetUiInfoBaseAddrCall
				call eax
				add esp, 0x0C

				MOV ECX,EAX
				push dwUiId
				mov eax, dwGetMuBanInfoAddrCall
				call eax
				mov dwUiInfoAddr, eax
		}
	}
	catch (...)
	{

	}

	return dwUiInfoAddr;
}

typedef struct
{
	DWORD	dwVfunAddr;
	DWORD	dwUnKnowZeor1;
	DWORD	dwUnKnowZeor2;
	DWORD	dwUnKnowZeor3;
	DWORD	dwUiId;
	char*	pszUiName;
}MMO_UI_INFO,*PMMO_UI_INFO;

CString GetUiInfo()
{
	CString csRet;

	try
	{
		DWORD dwUiId = 0;
		char szPrint[1024] = {0};

		for (DWORD i=0; i<0x30000; i++)
		{
			DWORD dwUiInfoAddr = GetUiInfoAddr(i);

			if( dwUiInfoAddr != 0 )
			{
				try
				{
					PMMO_UI_INFO ptheUiInfo = (PMMO_UI_INFO)dwUiInfoAddr;
					sprintf_s(szPrint,"[LOG] ui addr = 0x%p id = 0x%p %s\r\n",dwUiInfoAddr,i,ptheUiInfo->pszUiName);
					OutputDebugStringA(szPrint);

					csRet += szPrint;
				}
				catch (...)
				{

				}

			}
		}

	}
	catch (...)
	{

	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// 获取弹框内容信息



DWORD GetDialogInfoAddr(DWORD dwDialogId)
{
	DWORD dwDialogInfoAddr = 0;
	char* pszDialogString = "CDialogInfo";

	try
	{
		// 		_asm
		// 		{
		// 			push dwDialogId
		// 			mov eax, MMOCLIENT_REVERSAL_GET_DIALOG_INFO_BASE_ADDR_CALL
		// 			call eax
		// 			add esp, 4
		// 			mov dwDialogInfoAddr, eax
		// 		}

		DWORD dwGetDialogInfoBaseAddrCall = MMOCLIENT_REVERSAL_GET_DIALOG_INFO_BASE_ADDR_CALL+GetCryGameBaseAddr();
		DWORD dwGetMuBanInfoAddrCall = MMOCLIENT_REVERSAL_GET_MU_BAN_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			push 0
				push pszDialogString
				push 0
				MOV EAX, dwGetDialogInfoBaseAddrCall
				call eax
				add esp, 0x0C

				MOV ECX,EAX
				push dwDialogId
				mov eax, dwGetMuBanInfoAddrCall
				call eax
				mov dwDialogInfoAddr, eax
		}
	}
	catch (...)
	{

	}

	return dwDialogInfoAddr;
}


CString GetCDialogInfo()
{
	CString csTxt;

	for (DWORD i=0; i<0x10000; i++)
	{
		DWORD dwDialogInfoAddr = GetDialogInfoAddr(i);

		if( dwDialogInfoAddr != 0 )
		{
			try
			{
				char* pszDesc = (char*)*(DWORD*)(dwDialogInfoAddr+MMOCLIENT_REVERSAL_GET_DIALOG_INFO_DESC_OFFSET___);
				char szPrint[1024];
				sprintf_s(szPrint,"[LOG] id = 0x%p %s\r\n",i,pszDesc);
				csTxt += szPrint;
			}
			catch (...)
			{

			}

		}
	}

	return csTxt;
}


//////////////////////////////////////////////////////////////////////////
// hook 弹框

void PirntDialogInfo(DWORD dwDialogInfoAddr,DWORD dwDialogParamter)
{
	try
	{
		/*++
		[LOG] id = 0x00001F9A 对不起，您的帐号在其他地方登录
		[LOG] id = 0x00001F9B 对不起，该帐号已经登录.请重新启动客户端尝试登录
		[LOG] id = 0x00001F79 对不起，您的帐号被停封！
		[LOG] id = 0x00001F78 对不起，您的帐号未激活！
		[LOG] id = 0x00001FD9 您的帐号被检测出安全方面的异常，您将被强制退出游戏！
		[LOG] id = 0x00001F66 您与服务器的连接已中断。请尝试重新登陆！
		[LOG] id = 0x00001F64 连接副本服务器失败!
		[LOG] id = 0x00001F63 服务器连接失败，错误类型%s!
		[LOG] id = 0x00001F5F 与服务器断开连接，错误类型%s!

		弹框得到如上信息，则需要做出相应的处理
		--*/
		char* pszDesc = (char*)*(DWORD*)(dwDialogInfoAddr+MMOCLIENT_REVERSAL_GET_DIALOG_INFO_DESC_OFFSET___);
		char szPirnt[1024];

		// 增加获取封号时间
		if( strstr(pszDesc,"帐号已被封停") != 0 )
		{
			char* pszDescTime = "";

			try
			{
				pszDescTime = (char*)*(DWORD*)(*(DWORD*)(*(DWORD*)(dwDialogParamter+MMOCLIENT_REVERSAL_GET_DIALOG_INFO_DESC_PARAMTER_OFFSET___)));
			}
			catch (...)
			{
				assert(false);
			}

			sprintf_s(szPirnt,"[LOG] %s %s",pszDesc,pszDescTime);
		}
		else
		{
			sprintf_s(szPirnt,"[LOG] %s",pszDesc);
		}

		OutputDebugStringA(szPirnt);
	}
	catch (...)
	{

	}

}

_declspec(naked) void HookDialogInfo()
{
	__asm
	{
		pushfd
			pushad

			push ebp
			push esi
			call PirntDialogInfo
			add esp, 8

			popad
			popfd

			MOV ECX,DWORD PTR DS:[ESI+0x54]
		LEA EAX,DWORD PTR DS:[ESI+0x54]

		mov edx, MMOCLIENT_REVERSAL_DIALOG_HOOK
			add edx, 6
			jmp edx
	}
}

void HookDialogInfoCall()
{
	Hook(MMOCLIENT_REVERSAL_DIALOG_HOOK,HookDialogInfo);
}

//////////////////////////////////////////////////////////////////////////
// 怪物血值hook

// 进入副本后，怪物对象初始化时，会调用此函数初始化血值
// 在怪物的血值发生变化时，会调用此函数改变血值
void PrintMonsterHp(DWORD dwCallBack, DWORD dwMonsterObjAddr, int nRetValue)
{
	try
	{
		// 判断调用函数，只有此调用函数的值才可以确保访问正确
		//if( dwCallBack == (MMOCLIENT_REVERSAL_MONSTER_HP_CALL_CMP_ADDR2+GetCryGameBaseAddr()) )
		{
			char szPrint[256] = {0};
			sprintf_s(szPrint,sizeof(szPrint),"[LOG] call = 0x%p addr = 0x%p id = 0x%p hp = %d",
				dwCallBack,dwMonsterObjAddr,*(DWORD*)((*(DWORD*)(dwMonsterObjAddr+MMOCLIENT_REVERSAL_MONSTER_HP_ADDR_OFFSET1___))+MMOCLIENT_REVERSAL_MONSTER_HP_PACKET_ID_OFFSET___),nRetValue);
			OutputDebugStringA(szPrint);
		}
	}
	catch (...)
	{
		assert(false);
	}
}

DWORD GetMonsterHpCallHook()
{
	return MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK+GetCryGameBaseAddr();
}

DWORD g_dwJmpMonsterHpCallHook = 0;

_declspec(naked) void HookMonsterHp()
{
	__asm
	{
		pushfd
			pushad

			mov eax, [esp+0x24+4]
		mov eax, [eax]
		mov edx, [esp+0x24]
		push eax
			push ecx
			push edx
			call PrintMonsterHp
			add esp, 0x0C

			popad
			popfd

			PUSH EBP
			MOV EBP,ESP
			SUB ESP,0x20

			// 		PUSH EBP
			// 		MOV EBP,ESP
			// 		CMP DWORD PTR SS:[EBP+0xC],0x0

			jmp dword ptr[g_dwJmpMonsterHpCallHook]
	}
}

void HookMonsterHpCall()
{
	g_dwJmpMonsterHpCallHook = GetMonsterHpCallHook()+6;
	Hook(MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK+GetCryGameBaseAddr(),HookMonsterHp);
}

//////////////////////////////////////////////////////////////////////////
// 玩家血值hook

void PrintActorHp(DWORD dwObjAddr, int nhp)
{
	try
	{
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] obj id = 0x%p hp = %d",*(DWORD*)(*(DWORD*)(dwObjAddr+MMOCLIENT_REVERSAL_MONSTER_HP_ADDR_OFFSET1___)+MMOCLIENT_REVERSAL_MONSTER_HP_PACKET_ID_OFFSET___),nhp);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
}

DWORD GetActorFeerHpCallHook()
{
	return MMOCLIENT_REVERSAL_ACTOR_FEEB_HP_CALL_HOOK+GetCryGameBaseAddr();
}

DWORD g_dwActorFeerHpCallHook = 0;

_declspec(naked) void HookActorFeebHp()
{
	__asm
	{
		pushfd
			pushad

			mov eax, [esp+0x24+4]
		mov eax, [eax]
		push eax
			push ecx
			call PrintActorHp
			add esp, 8

			popad
			popfd

			// 		PUSH EBP
			// 		MOV EBP,ESP
			// 		SUB ESP,0x24

			SUB ESP,0x24
			PUSH ESI
			MOV ESI,ECX

			jmp dword ptr[g_dwActorFeerHpCallHook]
	}
}

DWORD GetActorHitHpCallHook()
{
	return MMOCLIENT_REVERSAL_ACTOR_HIT_HP_CALL_HOOK+GetCryGameBaseAddr();
}

DWORD g_dwActorHitHpCallHook = 0;
_declspec(naked) void HookActorHitHp()
{
	__asm
	{
		pushfd
			pushad

			mov eax, [esp+0x24+4]
		mov eax, [eax]
		push eax
			push ecx
			call PrintActorHp
			add esp, 8

			popad
			popfd

			// 		PUSH EBP
			// 		MOV EBP,ESP
			// 		SUB ESP,0x14

			SUB ESP,0x14
			PUSH ESI
			MOV ESI,ECX

			jmp dword ptr[g_dwActorHitHpCallHook]
	}
}


void HookActorHpCall()
{
	g_dwActorHitHpCallHook = GetActorHitHpCallHook()+6;
	g_dwActorFeerHpCallHook = GetActorFeerHpCallHook()+6;
	Hook(MMOCLIENT_REVERSAL_ACTOR_FEEB_HP_CALL_HOOK+GetCryGameBaseAddr(),HookActorFeebHp);
	Hook(MMOCLIENT_REVERSAL_ACTOR_HIT_HP_CALL_HOOK+GetCryGameBaseAddr(),HookActorHitHp);
}

//////////////////////////////////////////////////////////////////////////
// 遍历已接任务

void PrintReceivedTask(DWORD lpSubTreeRoot)
{
	PMMO_RECEIVED_TASK_POINT ptheReceivedTaskPoint = (PMMO_RECEIVED_TASK_POINT)lpSubTreeRoot;

	BYTE	bIsPlayerTask = *(BYTE*)(ptheReceivedTaskPoint->dwTaskAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_TRUE_OFFSET___);

	//if( bIsPlayerTask == 1 )
	{
		DWORD dwTaskObjAddr = *(DWORD*)(ptheReceivedTaskPoint->dwTaskAddr+MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_ADDR___);	// GetTaskAddr(ptheReceivedTaskPoint->dwTaskId);

		char* pszTaskName = "error";

		if( dwTaskObjAddr != 0 )
		{
			try
			{
				pszTaskName = (char*)*(DWORD*)(dwTaskObjAddr+MMOCLIENT_REVERSAL_TASK_NAME_OFFSET___);
				char* pszTaskDesc = (char*)*(DWORD*)(dwTaskObjAddr+MMOCLIENT_REVERSAL_TASK_DESC_OFFSET___);

				// CompleteValue = 4 表示任务完成
				DWORD dwCompleteValue = *(DWORD*)(ptheReceivedTaskPoint->dwTaskAddr+MMOCLIENT_REVERSAL_TASK_COMPLETE_OFFSET___);

				char szPrint[0x1024] = {0};
				sprintf_s(szPrint,sizeof(szPrint),"[LOG] task addr = 0x%p id = 0x%p complete = 0x%p name = %s desc = %s\r\n",lpSubTreeRoot,ptheReceivedTaskPoint->dwTaskId,dwCompleteValue,pszTaskName,pszTaskDesc);
				OutputDebugStringA(szPrint);

				csRet += szPrint;
			}
			catch (...)
			{
				assert(false);
			}
		}
	}

}

CString TraverseReceivedTask()
{
	csRet = "";

	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_TASK_BASE_ADDR+GetCryGameBaseAddr());
		dwGameAddr = dwGameAddr+MMOCLIENT_REVERSAL_TASK_BASE_OFFSET1___;

		NODE	theNode = {0};
		_ReadProcessMemory(dwGameAddr,&theNode,sizeof(theNode));

		Traverse(theNode.str_dwNextAddrParent,dwGameAddr,PrintReceivedTask);
	}
	catch (...)
	{
		assert(false);
	}

	return csRet;
}



CString GetSuoYouShiXianObjInfo(char* szScrName,DWORD dwTempGameDoorAddr,WORD wClass)
{
	CString csRet;

	try
	{
		DWORD dwLocalObjId = *(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_LOCAL_ID_OFSET___);
		DWORD dwPacketId = *(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___);
		char* pszDoorStr = (char*)*(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFFSET_NAME___);
		MMO_ActorVector* ptheActorVector = (MMO_ActorVector*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___);

		DWORD dwCollectAddr2 = GetObjAddr(dwPacketId);		// GetObjAddr	GetCollectObjAddr

		int nRoomIndex = -1;


		if( MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_COLLECT___ == wClass )
		{
			if( dwCollectAddr2 != 0 )
			{
				DWORD dwCollectRoomIdAddr = *(DWORD*)(dwCollectAddr2+MMOCLIENT_REVERSAL_TRAVERSE_COLLECT_ROOM_INDEX_OFFSET1___);
				if( dwCollectRoomIdAddr != 0 )
				{
					nRoomIndex = *(DWORD*)(dwCollectRoomIdAddr+MMOCLIENT_REVERSAL_TRAVERSE_COLLECT_ROOM_INDEX_OFFSET2___);
				}
				else
				{
					// 非采集物品本身,忽略不计
					nRoomIndex = -1;
				}
			}
		}
		else
		{
			nRoomIndex = *(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_DOOR_IN_ROOM_INDEX_OFFSET___);
		}


		// 		if( nRoomIndex != 3 )
		// 		{
		// 			return "";
		// 		}
		char* pszObjName = "未知";
		DWORD dwMonsterPacketId = 0;
		try
		{
			// 此ID为死亡后怪物尸体对象所对应的怪物的id
			dwMonsterPacketId = *(DWORD*)(dwCollectAddr2+MMOCLIENT_REVERSAL_COLLECT_OBJ_MONSTER_PACKET_ID_OFFSET);

			// 读取名字
			DWORD dwNameAddr = *(DWORD*)(dwCollectAddr2+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_NAME1_OFFSET___);

			if( dwNameAddr != 0 )
			{
				pszObjName = (char*)dwNameAddr;
			}
			else
			{
				// 如果读取不到的，则可能是采集物品的名字，用此方法遍历
				dwNameAddr = *(DWORD*)(dwCollectAddr2+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_NAME2_OFFSET___);
				if( dwNameAddr != 0 )
				{
					dwNameAddr = *(DWORD*)dwNameAddr;

					if( dwNameAddr != 0 )
					{
						pszObjName = (char*)dwNameAddr;
					}
				}
			}

		}
		catch (...)
		{

		}

		char szPrint[0x1024] = {0};

		sprintf_s(szPrint,sizeof(szPrint),"[LOG] %s addr = 0x%p addr2 = 0x%p id = 0x%p local id = 0x%p room = %d class = 0x%p packet id = 0x%p name = %s name = %s x = %f y = %f z = %f\r\n",
			szScrName,
			dwTempGameDoorAddr,
			dwCollectAddr2,
			dwPacketId,
			dwLocalObjId,
			nRoomIndex,
			wClass,
			dwMonsterPacketId,		// 剥皮对象所属怪物的id
			pszObjName,
			pszDoorStr,
			ptheActorVector->fx,
			ptheActorVector->fy,
			ptheActorVector->fz
			);
		OutputDebugStringA(szPrint);
		csRet = szPrint;

		if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_COLLECT___ )
		{
			// 			WritePos(ptheActorVector->fx,ptheActorVector->fy,ptheActorVector->fz);
			// 			MessageBox(NULL,"","",MB_OK);

			// 			if( strcmp("btobj",pszObjName) == 0 )
			// 				NpcWirtePos(dwTempGameDoorAddr,ptheActorVector->fx+1.0f,ptheActorVector->fy,ptheActorVector->fz);
		}


		// 		if( nRoomIndex == 1 )
		// 		{
		// 			/*++
		// 			[LOG] 其他对象 addr = 0x33544AB8 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_4 x = 857.004883 y = 762.906433 z = 18.984663
		// 			[LOG] 其他对象 addr = 0x33E8E130 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_2 x = 827.407166 y = 726.304993 z = 18.984663
		// 			[LOG] 其他对象 addr = 0x33E5BEA8 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_6 x = 851.729858 y = 786.043030 z = 18.984663
		// 			[LOG] 其他对象 addr = 0x33E5C4C0 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_3 x = 874.577820 y = 766.830688 z = 18.984663
		// 			[LOG] 其他对象 addr = 0x33E77DF0 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_1 x = 815.267151 y = 745.045898 z = 18.984663
		// 			[LOG] 其他对象 addr = 0x33E7C710 addr2 = 0x00000000 id = 0xFFFFFFFF room = 1 class = 0x00000510 name = 未知 name = 100465_Bug_SFX_5 x = 817.057922 y = 779.050232 z = 18.984663
		// 			--*/
		// 			if( strstr(pszDoorStr,"Bug_SFX_") != 0  )		// 处理发光的东西
		// 			{
		// 				
		// 				MMOActor theActor;
		// 				theActor.ReadActor();
		// 				theActor.WirtePos(ptheActorVector->fx,ptheActorVector->fy,ptheActorVector->fz,0.0f);		// 只要不停的遍历，不停的移动到这些对象的位置，遍历背包任务栏，判断任务是否完成  任务物品名字 = "发光的东西"
		// 				
		// 				Sleep(500);
		// 				//MessageBox(NULL,"","",MB_OK);
		// 			}
		// 		}

	}
	catch (...)
	{

	}
	return csRet;
}


//////////////////////////////////////////////////////////////////////////
// 遍历所有实现对象，可以通过对象名称得到对象的坐标
CString GetSuoYouShiXianDuiXiang(DWORD Dwtype)
{
	CString csRet;

	try
	{
		DWORD dwGameAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr = *(DWORD*)(dwGameAddr+MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET1);
		PGAME_STRUCT_ARRAY_POINT ptheDoorObjAddr = (PGAME_STRUCT_ARRAY_POINT)(dwGameAddr+MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET2);


		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] base addr = 0x%p array addr = 0x%p\r\n",dwGameAddr,ptheDoorObjAddr->str_dwBeginAddr);
		csRet += szPrint;

		for (DWORD dwGameDoorAddr=ptheDoorObjAddr->str_dwBeginAddr; dwGameDoorAddr<ptheDoorObjAddr->str_dwEndAddr1; dwGameDoorAddr+=4)
		{
			DWORD dwTempGameDoorAddr = *(DWORD*)dwGameDoorAddr;
			if( dwTempGameDoorAddr != 0 )
			{
				WORD wClass = *(WORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___);

				// 遍历采集物品，遍历门对象
				if ( Dwtype!=-1 &&   Dwtype !=wClass)
					continue;

				if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_DOOR___ || wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_DOOR2___ )
				{
					try
					{

						DWORD dwLinkAddr = *(DWORD*)(dwTempGameDoorAddr+MMOCLIENT_REVERSAL_DOOR_IN_ROOM_LINK_OFFSET___);
						char* pszLinkStr = "";

						if( dwLinkAddr != 0 )
						{
							pszLinkStr = (char*)dwLinkAddr;

							csRet += GetSuoYouShiXianObjInfo("门对象",dwTempGameDoorAddr,wClass);
						}


					}
					catch (...)
					{

					}
				}
				else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_COLLECT___)
				{
					csRet += GetSuoYouShiXianObjInfo("采集对象",dwTempGameDoorAddr,wClass);
				}
				/*else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_STATIC___ )
				{
					csRet += GetSuoYouShiXianObjInfo("发光对象",dwTempGameDoorAddr,wClass);
				}
				else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_PLAY1___ )
				{
					csRet += GetSuoYouShiXianObjInfo("玩家对象",dwTempGameDoorAddr,wClass);
				}
				else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_NPC___ )
				{
					csRet += GetSuoYouShiXianObjInfo("NPC对象",dwTempGameDoorAddr,wClass);
				}
				else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_ROCK___ )
				{
					//[LOG] 石块对象 addr = 0x2FB74D48 addr2 = 0x5D6D16D8 id = 0xC0002237 local id = 0x000004A7 room = 3 class = 0x00000595 packet id = 0x00000000 name = SceneObject.HardRock5 name = SceneObject.HardRock5 x = 2802.998779 y = 2746.079590 z = 250.036377
					//[LOG] 石块对象 addr = 0x2FB7B108 addr2 = 0x5D6D1398 id = 0xC0002235 local id = 0x000004AA room = 3 class = 0x00000595 packet id = 0x552BF600 name = SceneObject.HardRock4 name = SceneObject.HardRock4 x = 2771.323975 y = 2736.880859 z = 249.926147
					//[LOG] 石块对象 addr = 0x2FB63C08 addr2 = 0x5D6D2238 id = 0xC000223E local id = 0x000004AB room = 3 class = 0x00000595 packet id = 0x552B96F0 name = SceneObject.HardRock3 name = SceneObject.HardRock3 x = 2760.863037 y = 2734.797607 z = 250.494736
					//[LOG] 石块对象 addr = 0x2FB64538 addr2 = 0x5D6D2098 id = 0xC000223D local id = 0x000004AE room = 3 class = 0x00000595 packet id = 0x00000000 name = SceneObject.HardRock7 name = SceneObject.HardRock7 x = 2802.814697 y = 2735.222168 z = 249.947968
					//[LOG] 石块对象 addr = 0x2FB6F018 addr2 = 0x5D6D1A18 id = 0xC0002239 local id = 0x000004AF room = 3 class = 0x00000595 packet id = 0x552BF0B8 name = SceneObject.HardRock2 name = SceneObject.HardRock2 x = 2767.158203 y = 2747.059082 z = 250.079407
					//[LOG] 石块对象 addr = 0x2FB84838 addr2 = 0x5D75B378 id = 0xC0002232 local id = 0x000004B0 room = 3 class = 0x00000595 packet id = 0x00000000 name = SceneObject.HardRock8 name = SceneObject.HardRock8 x = 2813.031982 y = 2735.775635 z = 249.892014
					//[LOG] 石块对象 addr = 0x2FBDC368 addr2 = 0x5D6D2A58 id = 0xC000236C local id = 0x000004BA room = 3 class = 0x00000595 packet id = 0x00000000 name = SceneObject.HardRock6 name = SceneObject.HardRock6 x = 2815.690430 y = 2746.470215 z = 249.979019
					//[LOG] 石块对象 addr = 0x2FBDABC8 addr2 = 0x5D6D2BF8 id = 0xC000236D local id = 0x000004BD room = 3 class = 0x00000595 packet id = 0x552BEB70 name = SceneObject.HardRock1 name = SceneObject.HardRock1 x = 2757.336670 y = 2744.193359 z = 250.349960
					csRet += GetSuoYouShiXianObjInfo("石块对象",dwTempGameDoorAddr,wClass);
				}
				else if( wClass == MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_BUCKET___ )
				{
					//[LOG] 桶对象 addr = 0x57248A88 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x0000028B room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_BucketHint4 x = 167.481079 y = 278.819061 z = 23.143614
					//[LOG] 桶对象 addr = 0x5B02A548 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x0000028F room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_BucketHint3 x = 161.620224 y = 306.006042 z = 23.626951
					//[LOG] 桶对象 addr = 0x5B059E08 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x00000293 room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_BucketHint1 x = 130.214279 y = 315.956085 z = 23.373713
					//[LOG] 桶对象 addr = 0x5B06DBE8 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x00000294 room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_BucketHint2 x = 144.297363 y = 330.796997 z = 23.402502
					//[LOG] 桶对象 addr = 0x5492BFC8 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x0000038D room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_bucket_launcher2 x = 144.219116 y = 330.758392 z = 23.287764
					//[LOG] 桶对象 addr = 0x5B013778 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x0000038E room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_bucket_launcher4 x = 167.440338 y = 278.757294 z = 23.210886
					//[LOG] 桶对象 addr = 0x5B025E78 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x0000038F room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_bucket_launcher1 x = 130.194550 y = 315.939789 z = 23.388668
					//[LOG] 桶对象 addr = 0x5B059238 addr2 = 0x00000000 id = 0xFFFFFFFF local id = 0x00000390 room = 3 class = 0x00000510 packet id = 0x00000000 name = 未知 name = 100491_bucket_launcher3 x = 161.541977 y = 305.967438 z = 23.574936

					//此对象不会消失，可以重复动作N次，一直到完成提示
					csRet += GetSuoYouShiXianObjInfo("桶对象",dwTempGameDoorAddr,wClass);		// 包含字符串 "BucketHint"，为桶对象 或者 "bucket_launcher"
				}
				else
				{
					csRet += GetSuoYouShiXianObjInfo("其他对象",dwTempGameDoorAddr,wClass);
				}*/
			}
		}

	}
	catch (...)
	{

	}

	return csRet;
}


void InitDoorLinkInfo()
{
}




CString GetInFbInfo()
{
	CString csRet;

	try
	{
		MMOActor theActor;
		theActor.ReadActor();	// 获取对象的封包id

		DWORD dwObjAddr2 = GetObjAddr(theActor.m_dwActorPacketId);		// 获取对象的另一个地址

		// dd [[[[[0x0DAC8EF0 ]+0xc8]+0x1C]+0x214]+0x00002044]
		// dd [[[[[0x0DAC8EF0 ]+0xc8]+0x1C]+0x214]+0x000020E0]
		DWORD dwInFbInfoAddr = *(DWORD*)(dwObjAddr2+MMOCLIENT_REVERSAL_IN_FB_INFO_OFFSET1);

		PGAME_STRUCT_ARRAY_POINT ptheInFbInfoArrayAddr = PGAME_STRUCT_ARRAY_POINT(dwInFbInfoAddr+MMOCLIENT_REVERSAL_IN_FB_INFO_OFFSET2___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] addr = 0x%p\r\n",dwInFbInfoAddr);
		csRet += szPrint;

		for (DWORD dwCurInfbInfoAddr=ptheInFbInfoArrayAddr->str_dwBeginAddr; dwCurInfbInfoAddr<ptheInFbInfoArrayAddr->str_dwEndAddr1; dwCurInfbInfoAddr+=4)
		{
			DWORD dwFbId = *(DWORD*)dwCurInfbInfoAddr;


			DWORD dwInfbInfoAddr2 = GetInFbInfoAddr(dwFbId);

			assert(dwInfbInfoAddr2);
			if( dwInfbInfoAddr2 != 0 )
			{
				char* pszFbName = (char*)*(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_NAME_OFFSET___);

				/*++
				凡事类型为 8,23,16的，都不判断等级，8和23的表现主线任务的副本，16大图标的副本且需求等级为0的
				fbtype = 8 name = [易怒的野猪王]			
				fbtype = 23 name = [危险的商路]
				fbtype = 16 name = 肆虐的幼年蓝速龙
				--*/
				DWORD dwFbType = *(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_TYPE_OFFSET___);		// 选择副本的时候发包需要的数据
				int nDiffLevel = *(int*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_DIFF_LEVEL_OFFSET__);	// 副本挑战等级
				int nLevel = *(int*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_LEVEL_OFFSET___);
				DWORD dwMapId = *(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_MAP_ID_OFFSET___);		// 地图模板id


				sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p map id = 0x%p level = %d difflevel = %d fbtype = %d 副本完成 = %d name = %s\r\n",
					dwInfbInfoAddr2,dwFbId,dwMapId,nLevel,nDiffLevel,dwFbType,IsInTaskFb(dwFbId),pszFbName);
				OutputDebugStringA(szPrint);

				csRet += szPrint;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	try
	{
		csRet += "*****************************暴力搜索****************************\r\n";

		char szPrint[256] = {0};

		for (DWORD i=1; i<=0x00085000 ;i++)
		{
			DWORD dwInfbInfoAddr2 = GetInFbInfoAddr(i);
			if( dwInfbInfoAddr2 != 0 )
			{
				char* pszFbName = (char*)*(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_NAME_OFFSET___);
				DWORD dwFbType = *(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_TYPE_OFFSET___);		// 选择副本的时候发包需要的数据
				int nDiffLevel = *(int*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_DIFF_LEVEL_OFFSET__);	// 副本挑战等级
				int nLevel = *(int*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_LEVEL_OFFSET___);
				DWORD dwMapId = *(DWORD*)(dwInfbInfoAddr2+MMOCLIENT_REVERSAL_FB_INFO_MAP_ID_OFFSET___);		// 地图模板id
				sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p map id = 0x%p level = %d difflevel = %d fbtype = %d name = %s\r\n",dwInfbInfoAddr2,i,dwMapId,nLevel,nDiffLevel,dwFbType,pszFbName);
				OutputDebugStringA(szPrint);

				csRet += szPrint;
			}
		}
	}
	catch (...)
	{

	}

	return csRet;
}






CString GetTaskFbInfo()
{
	CString csRet;

	csRet += "主线任务副本： \r\n";

	try
	{
		DWORD dwTaskInfoAddr = GetMianBanTaskFbInfoBaseAddr(1);		// 副本 "CHubEntryConfigInfo"   王立副本 "CHubEntryConfigKingQuestInfo"



		if( dwTaskInfoAddr != 0 )
		{
			for (int i=0; i<3; i++)		// 暂定于3
			{
				PTASK_FB_INFO ptheFbInfoArray = (PTASK_FB_INFO)dwTaskInfoAddr;

				for (DWORD dwTaskZhangAddr = ptheFbInfoArray->theTaskArray.str_dwBeginAddr; dwTaskZhangAddr<ptheFbInfoArray->theTaskArray.str_dwEndAddr1; )
				{
					PZHANG_TASK_FB_INFO ptheZhangTaskFbInfoAddr = (PZHANG_TASK_FB_INFO)dwTaskZhangAddr;

					char szPrint[256] = {0};

					if( IsInTaskFb(ptheZhangTaskFbInfoAddr->dwTaskFbId) == 1 && ptheZhangTaskFbInfoAddr->nShowFlag == 1 )
					{
						sprintf_s(szPrint,"[LOG] addr = 0x%p %s 序号：%d 副本id：0x%p 城镇id：0x%p show flag = %d 剧情任务已完成!\r\n",
							dwTaskZhangAddr,
							ptheZhangTaskFbInfoAddr->pszZhangStr,		// 章节的字符串
							ptheZhangTaskFbInfoAddr->nZhangTaskIndex,	// 章节内任务的序号
							ptheZhangTaskFbInfoAddr->dwTaskFbId,		// 任务副本id
							ptheZhangTaskFbInfoAddr->dwCityId,			// 任务副本所在的城镇id
							ptheZhangTaskFbInfoAddr->nShowFlag);		// 1为显示出来的主线任务副本，2为正常副本
					}
					else if( IsInTaskFb(ptheZhangTaskFbInfoAddr->dwTaskFbId) == 1 && ptheZhangTaskFbInfoAddr->nShowFlag == 2 )
					{
						sprintf_s(szPrint,"[LOG] addr = 0x%p %s 序号：%d 副本id：0x%p 城镇id：0x%p show flag = %d 副本挑战获得奖杯，可解锁下一难度副本\r\n",
							dwTaskZhangAddr,
							ptheZhangTaskFbInfoAddr->pszZhangStr,		// 章节的字符串
							ptheZhangTaskFbInfoAddr->nZhangTaskIndex,	// 章节内任务的序号
							ptheZhangTaskFbInfoAddr->dwTaskFbId,		// 任务副本id
							ptheZhangTaskFbInfoAddr->dwCityId,			// 任务副本所在的城镇id
							ptheZhangTaskFbInfoAddr->nShowFlag);		// 1为显示出来的主线任务副本，2为正常副本
					}
					else
					{
						sprintf_s(szPrint,"[LOG] addr = 0x%p %s 序号：%d 副本id：0x%p 城镇id：0x%p show flag = %d \r\n",
							dwTaskZhangAddr,
							ptheZhangTaskFbInfoAddr->pszZhangStr,		// 章节的字符串
							ptheZhangTaskFbInfoAddr->nZhangTaskIndex,	// 章节内任务的序号
							ptheZhangTaskFbInfoAddr->dwTaskFbId,		// 任务副本id
							ptheZhangTaskFbInfoAddr->dwCityId,			// 任务副本所在的城镇id
							ptheZhangTaskFbInfoAddr->nShowFlag);		// 1为显示出来的主线任务副本，2为正常副本
					}

					csRet += szPrint;
					OutputDebugStringA(szPrint);

					dwTaskZhangAddr += sizeof(ZHANG_TASK_FB_INFO);
				}

				dwTaskInfoAddr += sizeof(TASK_FB_INFO);
			}

		}
	}
	catch (...)
	{

	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// hook 加载城镇地图和加载副本地图完成后执行的代码地址
void PrintLoadMap()
{
	try
	{
		MMOActor theActor;
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 进入地图 : %s\r\n",theActor.GetRoleInMap());
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{

	}
}

DWORD GetLoadMapEndHook()
{
	return MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK+GetCryGameBaseAddr();
}
_declspec(naked) void HookLoadMap()
{
	__asm
	{
		pushfd
			pushad

			call PrintLoadMap

			popad
			popfd

			MOV EDX,DWORD PTR DS:[EAX]
		MOV ECX,EAX
			MOV EAX,DWORD PTR DS:[EDX+0x24]

		mov eax, GetLoadMapEndHook
			call eax
			add eax, 7
			jmp eax
	}
}

DWORD GetLoadMapEndHook2()
{
	return MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK2+GetCryGameBaseAddr();
}
_declspec(naked) void HookLoadMap2()
{
	__asm
	{
		pushfd
			pushad

			call PrintLoadMap

			popad
			popfd

			MOV ECX,EBX
			MOV DWORD PTR SS:[EBP-0x8C],ESI

			push eax
			mov eax, GetLoadMapEndHook2
			call eax
			mov edx, eax
			pop eax
			add edx, 8
			jmp edx
	}
}





void HookLoadMapCallEnd()
{
	Hook(MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK+GetCryGameBaseAddr(),HookLoadMap);
	Hook(MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK2+GetCryGameBaseAddr(),HookLoadMap2);
}





////////////////////////////////////////////////////////////////////////////////////
//HOOK 到精彩镜头


DWORD g_dwOldJingcaiAddr=0;
void PirntJingcai(DWORD dwSocketHeadId)
{
	if (dwSocketHeadId==0x56F)
	{
#if _DEBUG
		OutputDebugStringA("[LOG] 到精彩镜头");
#endif
	}


}



_declspec(naked) void HookJingcaiHook()
{
	__asm
	{
		mov edi,[esp+8]
		mov edi,[edi]
		pushfd
			pushad

			push edi
			call PirntJingcai
			add esp,4
			popad
			popfd
			mov ebp,esp
			sub esp,0x30
			mov edi, g_dwOldJingcaiAddr
			add edi, 5
			jmp edi
	}
}
void HookJingcai()
{
	g_dwOldJingcaiAddr=UPDATES_MMOCLIENT_REVERSAL_HIGHIGHT_HOOK+GetCryGameBaseAddr();
	Hook(g_dwOldJingcaiAddr,HookJingcaiHook);
}




//////////////////////////////////////////////////////////////////////////
// 遍历邮件



void PrintMail(DWORD lpSubTreeRoot)
{
	try
	{
		DWORD dwMailObjAddr = *(DWORD*)(lpSubTreeRoot/*+MMOCLIENT_REVERSAL_MAIL_ADDR_OFFSET___*/);

		if( dwMailObjAddr != 0 )
		{
			DWORD dwMailId1 = *(DWORD*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_ID_OFFSET1___);
			DWORD dwMailId2 = *(DWORD*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_ID_OFFSET2___);


			// 获取单个邮件的具体信息
			LONGLONG	llMailId = *(LONGLONG*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_ID_OFFSET1___);
			extern bool MMO_GetMailItemInfo(LONGLONG llMailId);
			MMO_GetMailItemInfo(llMailId);		// 获取邮件信息后，需要等待服务器返回包才有具体邮件的物品信息

			char* pszMailSender = (char*)*(DWORD*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_SENDER_OFFSET___);
			char* pszMailTitle = (char*)*(DWORD*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_TITLE_OFFSET___);
			int nGold = *(int*)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_GOLD_OFFSET___);

			CString csTxt;

			PGAME_STRUCT_ARRAY_POINT ptheMailItemAddr = (PGAME_STRUCT_ARRAY_POINT)(dwMailObjAddr+MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_OFFSET___);

			for(DWORD dwMailItemAddr = ptheMailItemAddr->str_dwBeginAddr;dwMailItemAddr<ptheMailItemAddr->str_dwEndAddr1; dwMailItemAddr+=MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_SIZE___)
			{
				DWORD dwItemId = *(DWORD*)(dwMailItemAddr+MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_ID_OFFSET___);
				int nCount = (int)*(WORD*)(dwMailItemAddr+MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_COUNT_OFFSET___);
				char szPrint2[256];
				sprintf_s(szPrint2,"Item id = 0x%p count = %d ",dwItemId,nCount);
				csTxt += szPrint2;
			}


			char szPrint[256];
			sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p 0x%p sender = %s title = %s %s Gold = %d\r\n",lpSubTreeRoot,dwMailId1,dwMailId2,pszMailSender,pszMailTitle,csTxt,nGold);
			OutputDebugStringA(szPrint);
			csRet += szPrint;


		}
	}
	catch (...)
	{

	}
}


CString GetMailInfo()
{
	csRet = "";

	try
	{

		// [[[[0xDAC8EF0]+0xC8]+0x1C]+0x214]

		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

		if( dwGameAddr2 != 0 )
		{
			DWORD dwMailAddr = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_MAIL_OFFSET1);
			dwMailAddr += MMOCLIENT_REVERSAL_MAIL_OFFSET2___;

			PGAME_STRUCT_ARRAY_POINT ptheMailAddr = PGAME_STRUCT_ARRAY_POINT(dwMailAddr);

			for(DWORD dwMailItemAddr = ptheMailAddr->str_dwBeginAddr;dwMailItemAddr<ptheMailAddr->str_dwEndAddr1; dwMailItemAddr+=sizeof(dwMailItemAddr))
			{
				PrintMail(dwMailItemAddr);
			}

		}
	}
	catch (...)
	{
		assert(false);
	}

	return csRet;
}

////////////////////////////////////////////////////////////////////////// 
// 遍历采集物品


// dd [[[[0x257A028]+0x74]+0x04]+0x0C]
CString GetCollectObjAddr2()
{
	CString csRet; 

	try
	{
		/*
		DWORD dwCollectAddr = *(DWORD*)MMOCLIENT_REVERSAL_GAME_OBJ_ADDR;
		dwCollectAddr = *(DWORD*)(dwCollectAddr+MMOCLIENT_REVERSAL_COLLECT_VFUN_ADDR_OFFSET1);
		dwCollectAddr = *(DWORD*)(dwCollectAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_OFFSET1___);
		PGAME_STRUCT_ARRAY_POINT ptheCollectAddrPoint  = (PGAME_STRUCT_ARRAY_POINT)(dwCollectAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_OFFSET2___);



		char szPrint[256] = {0};

		for (DWORD dwCollectObjNodeAddr = ptheCollectAddrPoint->str_dwBeginAddr; dwCollectObjNodeAddr < ptheCollectAddrPoint->str_dwEndAddr1; dwCollectObjNodeAddr +=4 )
		{
		DWORD dwCollectObjNodeAddr2 = *(DWORD*)(dwCollectObjNodeAddr);
		DWORD dwCollectObjAddr = *(DWORD*)(dwCollectObjNodeAddr2+MMOCLIENT_REVERSAL_COLLECT_OBJ_NODE_ADDR_OFFSET___);

		int nFlag = 0; //*(int*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_COLLECT_ZHEN_DE_DUI_XIANG_OFFSET___);

		if( nFlag == 0 )
		{
		DWORD dwCollectId = *(DWORD*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_ID_OFFSET___);

		DWORD dwCollectRoomIdAddr = *(DWORD*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_COLLECT_ROOM_INDEX_OFFSET1___);

		MMO_Vector3* ptheVector = (MMO_Vector3*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_XYZ_OFFSET___);

		char* pszObjName = "未知";

		try
		{
		DWORD dwNameAddr = *(DWORD*)(dwCollectObjAddr+0x44);

		if( dwNameAddr != 0 )
		{
		pszObjName = (char*)dwNameAddr;
		}
		else
		{
		dwNameAddr = *(DWORD*)(dwCollectObjAddr+0x158);
		if( dwNameAddr != 0 )
		{
		dwNameAddr = *(DWORD*)dwNameAddr;

		if( dwNameAddr != 0 )
		{
		pszObjName = (char*)dwNameAddr;
		}
		}
		}

		}
		catch (...)
		{

		}

		int nRoomIndex = -1;
		char* pszCollectName = "";
		if( dwCollectRoomIdAddr != 0 )
		{
		// 采集对象
		nRoomIndex = *(DWORD*)(dwCollectRoomIdAddr+MMOCLIENT_REVERSAL_TRAVERSE_COLLECT_ROOM_INDEX_OFFSET2___);
		//pszCollectName = (char*)*(DWORD*)(dwCollectRoomIdAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_NAME_OFFSET___);

		sprintf_s(szPrint,"[LOG]addr = 0x%p addr2 = 0x%p id = 0x%p room = %d x = %f y = %f z = %f name = %s\r\n",dwCollectObjNodeAddr,dwCollectObjAddr,dwCollectId,nRoomIndex,
		ptheVector->fx,ptheVector->fy,ptheVector->fz,pszObjName);
		OutputDebugStringA(szPrint);

		csRet += szPrint;
		}
		else
		{
		// 剥皮对象

		// 此ID为怪物对象的id
		DWORD dwMonsterPacketId = *(DWORD*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_COLLECT_OBJ_MONSTER_PACKET_ID_OFFSET___);

		DWORD dwColletItemAddr = *(DWORD*)(dwCollectObjAddr+MMOCLIENT_REVERSAL_COLLECT_ITEM_ADDR_OFFSET___);

		if( dwMonsterPacketId != 0 && dwColletItemAddr != 0 )
		{
		sprintf_s(szPrint,"[LOG] addr = 0x%p addr2 = 0x%p id = 0x%p packet id = 0x%p x = %f y = %f z = %f name = %s\r\n",dwCollectObjNodeAddr,dwCollectObjAddr,dwCollectId,dwMonsterPacketId,
		ptheVector->fx,ptheVector->fy,ptheVector->fz,pszObjName);
		OutputDebugStringA(szPrint);

		csRet += szPrint;
		}
		}
		}
		}
		*/
	}
	catch (...)
	{

	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
//  获取商品信息

CString GetShopInfo(int nNpcIndex)
{
	CString csRet;

	try
	{
		DWORD dwShopInfoAddr = GetEquipInfoAddr(nNpcIndex);

		char szPrint[256];


		if( dwShopInfoAddr != 0 )
		{
			sprintf_s(szPrint,"[LOG] i = %d addr = 0x%p\r\n",nNpcIndex,dwShopInfoAddr);
			csRet += szPrint;

			PGAME_STRUCT_ARRAY_POINT ptheShopInfo = (PGAME_STRUCT_ARRAY_POINT)(dwShopInfoAddr+MMOCLIENT_REVERSAL_GET_SHOP_INFO_OFFSET___);


			for (DWORD dwTempShopInfoAddr = ptheShopInfo->str_dwBeginAddr; dwTempShopInfoAddr < ptheShopInfo->str_dwEndAddr1; dwTempShopInfoAddr += sizeof(dwTempShopInfoAddr))
			{
				DWORD dwTempShopInfoAddr1 = *(DWORD*)dwTempShopInfoAddr;
				DWORD dwShopId = *(DWORD*)(dwTempShopInfoAddr1+MMOCLIENT_REVERSAL_SHOP_INFO_SHOP_ID_OFFSET___);
				DWORD dwItemId = *(DWORD*)(dwTempShopInfoAddr1+MMOCLIENT_REVERSAL_SHOP_INFO_ITEM_ID_OFFSET___);
				int nCount = *(int*)(dwTempShopInfoAddr1+MMOCLIENT_REVERSAL_SHOP_INFO_ITEM_COUNT_OFFSET___);

				DWORD dwItemInfoAddr = GetTemplateItemAddr(dwItemId);

				char* pszItemName = "";
				if( dwItemInfoAddr != 0 )
				{
					pszItemName = (char*)*(DWORD*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___);
				}
				sprintf_s(szPrint,"[LOG] addr = 0x%p shop id = 0x%p Item id = 0x%p count = %d name = %s\r\n",
					dwTempShopInfoAddr1,
					dwShopId,
					dwItemId,
					nCount,
					pszItemName
					);
				OutputDebugStringA(szPrint);
				csRet += szPrint;
			}
		}
	}
	catch (...)
	{

	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// 遍历锻造物品

CString GetCraftInfo()
{
	CString csRet;

	try
	{
		char szPrint[0x1024];

		for (DWORD i=0; i<0x30000; i++)
		{
			DWORD dwCraftInfoAddr = GetCraftInfoAddr(i);

			if( dwCraftInfoAddr != 0 )
			{
				// 锻造id
				DWORD dwCraftId = *(DWORD*)(dwCraftInfoAddr+MMOCLIENT_REVERSAL_CRAFT_INFO_CRAFT_ID_OFFSET___);

				// 锻造成功后的物品id
				DWORD dwItemId = *(DWORD*)(dwCraftInfoAddr+MMOCLIENT_REVERSAL_CRAFT_INFO_ITEM_ID_OFFSET___);

				// 获取物品模板
				DWORD dwItemInfoAddr = GetTemplateItemAddr(dwItemId);
				if( dwItemInfoAddr != 0 )
				{
					char* pszItemName = (char*)*(DWORD*)(dwItemInfoAddr+MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___);


					PGAME_STRUCT_ARRAY_POINT ptheMaterial = PGAME_STRUCT_ARRAY_POINT(dwCraftInfoAddr+MMOCLIENT_REVERSAL_CRAFT_INF0_MATERIAL_OFFSET);

					char szMaterialPrint[256];

					CString csMaterial;

					// 锻造装备所需要的材料
					for (DWORD dwMaterialAddr = ptheMaterial->str_dwBeginAddr; dwMaterialAddr < ptheMaterial->str_dwEndAddr1; dwMaterialAddr+=MMOCLIENT_REVERSAL_MATERIAL_SIZE___)
					{
						PMMO_MATERIAL_ITEM ptheMaterialItem = (PMMO_MATERIAL_ITEM)dwMaterialAddr;

						sprintf_s(szMaterialPrint,sizeof(szMaterialPrint)," material id = 0x%p count = %d",ptheMaterialItem->dwMaterialItemId,ptheMaterialItem->nCount);

						csMaterial += szMaterialPrint;

					}

					sprintf_s(szPrint,sizeof(szPrint),"[LOG] addr = 0x%p craft id = 0x%p item id = 0x%p name = %s %s\r\n",dwCraftInfoAddr,dwCraftId,dwItemId,pszItemName,csMaterial);
					OutputDebugStringA(szPrint);
					csRet += szPrint;
				}
			}
		}
	}
	catch (...)
	{

	}

	return csRet;
}

//////////////////////////////////////////////////////////////////////////
// 调和 制作物品


CString GetManuFactureInfo()
{
	CString csRet;

	try
	{
		char szPrint[256];
		for(int i=0; i<0x2000; i++)
		{
			DWORD dwManufactureInfoAddr = GetManufactureInfoAddr(i);
			if( dwManufactureInfoAddr != 0 )
			{
				// 调和物品 的 id，这是所于调和发包要用到的id
				DWORD dwManuFactrueItemId = *(DWORD*)(dwManufactureInfoAddr+MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_ID_OFFSET___);
				char* pszMamufactureItemName = (char*)*(DWORD*)(dwManufactureInfoAddr+MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_NAME_OFFSET___);
				// 调和完成的 物品id，这是制作后得到的物品id
				DWORD dwItemId = *(DWORD*)(dwManufactureInfoAddr+MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_ITEM_ID_OFFSET___);
				PGAME_STRUCT_ARRAY_POINT ptheMaterialAddr = (PGAME_STRUCT_ARRAY_POINT)(dwManufactureInfoAddr+MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_MAIERSAL_OFFSET___);

				CString csTxt;

				for (DWORD dwMaterialAddr = ptheMaterialAddr->str_dwBeginAddr; dwMaterialAddr < ptheMaterialAddr->str_dwEndAddr1; dwMaterialAddr+=sizeof(MMO_MANU_MATERIAL))
				{
					PMMO_MANU_MATERIAL ptheManuMaieral = (PMMO_MANU_MATERIAL)dwMaterialAddr;

					sprintf_s(szPrint,"材料id = 0x%p count = %d ; ",ptheManuMaieral->dwMaterialId,ptheManuMaieral->nMaterialCount);
					csTxt += szPrint;
				}

				sprintf_s(szPrint,"[LOG] Manu addr = 0x%p Id = 0x%p name = %s Item id = 0x%p 需要%s\r\n",dwManufactureInfoAddr,dwManuFactrueItemId,pszMamufactureItemName,dwItemId,csTxt);
				OutputDebugStringA(szPrint);
				csRet += szPrint;
			}
		}
	}
	catch (...)
	{

	}
	return csRet;
}

//////////////////////////////////////////////////////////////////////////

CString GetFarmSeedInfo()
{
	CString csTxt;

	for (DWORD i=0; i<0x10000; i++)
	{
		DWORD dwFarmSeedAddr = GetFarmSeedAddr(i);

		if( dwFarmSeedAddr != 0 )
		{
			try
			{
				char* pszFarmSeedName = (char*)*(DWORD*)(dwFarmSeedAddr+MMOCLIENT_REVERSAL_FARM_SEED_NAME_OFFSET___);
				char szPrint[1024];
				sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p %s\r\n",dwFarmSeedAddr,i,pszFarmSeedName);
				csTxt += szPrint;
			}
			catch (...)
			{

			}

		}
	}

	return csTxt;
}

//////////////////////////////////////////////////////////////////////////
// 遍历农场采集点

// 采集点的序号，从0开始数； 采集点的等级

CString GetFarmFacilityInfo()
{
	CString csTxt;

	try
	{

		// dd [[[[0x0175EF20+0C370000]+0xc8]+0x1C]+0x214]+0x00003F60+0x28
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		dwGameAddr2 = dwGameAddr2+MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET1;
		dwGameAddr2 += MMOCLIENT_REVERSAL_CUR_FARM_FACILITY_LEVEL_OFFSET3___;


		// dd [[0x0175EF20+0C370000]+0xC8]+0xD0+0x8+0x28
		// 这段数据需要角色在农场内遍历
		// 		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		// 		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		// 		dwGameAddr2 += MMOCLIENT_REVERSAL_CUR_FARM_FACILITY_LEVEL_OFFSET1___;
		// 		dwGameAddr2 += MMOCLIENT_REVERSAL_CUR_FARM_FACILITY_LEVEL_OFFSET2___;
		// 		dwGameAddr2 += MMOCLIENT_REVERSAL_CUR_FARM_FACILITY_LEVEL_OFFSET3___;

		PMMO_CUR_FARM_FACILITY ptheCurFarmFacility = (PMMO_CUR_FARM_FACILITY)(dwGameAddr2);
		char szPrint[256];
		/*++
		采集点的排序分别为：
		矿场=0
		鱼塘=1
		虫栖地=2
		育菇木床=3
		蜂房=4
		草药田=5	// 草药田的时间要另外遍历，此处的草药田的时间不顶用
		通过遍历到的等级，从遍历采集点信息中找到对象
		--*/
		for (int i=0; i<6; i++)
		{
			// 通过排列序号和采集点等级，获取采集点id
			DWORD dwFarmId = GetFarmFacilityId(i,ptheCurFarmFacility->dwCurFarmFacilityLevel);

			sprintf_s(szPrint,"[LOG] Index = %d level = %d id = 0x%p time = %d\r\n",i,ptheCurFarmFacility->dwCurFarmFacilityLevel,
				dwFarmId,GetFarmCpTime(dwFarmId,ptheCurFarmFacility->nFarmTime));
			OutputDebugStringA(szPrint);

			ptheCurFarmFacility++;
			csTxt += szPrint;
		}
	}
	catch (...)
	{

	}

	for (DWORD i=0; i<0x1000; i++)
	{
		DWORD dwFarmFacilityAddr = GetFarmFacilityInfoAddr(i);

		if( dwFarmFacilityAddr != 0 )
		{
			try
			{
				DWORD dwFarmFacilityId = *(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_ID_OFFSET___);
				DWORD dwFarmFacilityLevel = *(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_LEVEL_OFFSET___);
				DWORD dwFarmFacilityPoint = *(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_POINT_OFFSET___);
				DWORD dwFarmFacilityYing = *(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_YING_OFFSET___);
				char* pszFarmFacilityName = (char*)*(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_NAME_OFFSET___);
				char* pszFarmFacilityDesc = (char*)*(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_DESC_OFFSET___);
				DWORD dwRoleLevel = *(DWORD*)(dwFarmFacilityAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_ROLE_LEVEL_OFFSET___);

				char szPrint[1024];
				sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p name = %s 采集点等级 = %d 精算点 = %d 绑定金 = %d 所需等级 = %d 描述 = %s\r\n",
					dwFarmFacilityAddr,
					dwFarmFacilityId,
					pszFarmFacilityName,
					dwFarmFacilityLevel,
					dwFarmFacilityPoint,
					dwFarmFacilityYing,
					dwRoleLevel,
					pszFarmFacilityDesc
					);
				csTxt += szPrint;
			}
			catch (...)
			{

			}
		}
	}

	return csTxt;
}

//////////////////////////////////////////////////////////////////////////
// 获取农场采集点状态

// 遍历草药田
CString GetFarmFacilityState()
{
	CString csTxt;
	try
	{
		// dd [[[[[0xDAC9EF0]+0xc8]+0x1C]+0x214]+0x00003F60]
		DWORD dwGameBaseAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameBaseAddr = *(DWORD*)(dwGameBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameBaseAddr = *(DWORD*)(dwGameBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameBaseAddr = *(DWORD*)(dwGameBaseAddr+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		dwGameBaseAddr = dwGameBaseAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET1;

		char szPrint[256];
		PMMO_FARM_FACILITY_STATE ptheFarmFacilityState = (PMMO_FARM_FACILITY_STATE)(dwGameBaseAddr+MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET2);
		for (int i=0; i<3; i++)
		{
			/*++
			开启状态 = 1 表示此草药田可以种植; 开启状态 = 0 表示此草药田为开启
			种植物品id = 0 表示此处没有种植物品，需要种植 如果有值，则是种子的id（遍历种子信息中的id）
			--*/
			sprintf_s(szPrint,"[LOG] 草药田%d: 开启状态 = %d 种植物品id = %p time = %d\r\n",i+1,ptheFarmFacilityState->nFarmFlag,ptheFarmFacilityState->dwSeedId,GetCollectTime(i,dwGameBaseAddr));
			OutputDebugStringA(szPrint);
			csTxt += szPrint;
			ptheFarmFacilityState++;
		}
	}
	catch (...)
	{

	}

	return csTxt;
}

//////////////////////////////////////////////////////////////////////////
// 获取补给箱物品

CString GetRequireCurSlotSupplysAll()
{
	CString csTxt;

	try
	{
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

		// dd [[[[[[[[0x01928EA0+0C370000]+0x000000CC]+0x1C]+0x214]+0x0C]+0xA4]+0x14+0x4]+0x18]
		// dd [[[[[[[[0x01928EA0+0C370000]+0x000000CC]+0x1C]+0x214]+0x0C]+0x8C]+0x18+0x4]+0x18]
		DWORD dwRequireSlotAddr = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET1___);
		dwRequireSlotAddr = *(DWORD*)(dwRequireSlotAddr+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET2___);
		dwRequireSlotAddr =  *(DWORD*)(dwRequireSlotAddr+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET3___+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET4___);
		dwRequireSlotAddr = *(DWORD*)(dwRequireSlotAddr+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET5___);

		DWORD dwBeginRequirSlotAddr = dwRequireSlotAddr+MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET6___;

		char szPrint[256] = {0};
		DWORD dwCurRequirSlotAddr = *(DWORD*)dwBeginRequirSlotAddr;

		while ( dwCurRequirSlotAddr != dwBeginRequirSlotAddr )
		{
			PMMO_REQUIRE_SLOT ptheRequreSlot = (PMMO_REQUIRE_SLOT)dwCurRequirSlotAddr;

			sprintf_s(szPrint,"[LOG] Slot Index = %d Item id = 0x%p count = %d\r\n",
				ptheRequreSlot->ptheCurSlot->nSlotIndex,
				ptheRequreSlot->ptheCurSlot->dwItemId,
				ptheRequreSlot->ptheCurSlot->nCount);

			OutputDebugStringA(szPrint);
			csTxt += szPrint;

			dwCurRequirSlotAddr = *(DWORD*)dwCurRequirSlotAddr;
		}

	}
	catch (...)
	{

	}
	return csTxt;
}

//////////////////////////////////////////////////////////////////////////
// 遍历角色buff状态值



void ConvertUtf8ToGBK(const char* strUtf8,char *_outAscii) 
{
	try
	{
		int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL,0); 
		WCHAR * wszGBK = new WCHAR[len+1]; 
		memset(wszGBK, 0, len * 2 + 2); 
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, wszGBK, len); 

		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
		char *szGBK=new char[len + 1]; 
		memset(szGBK, 0, len + 1); 
		WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL); 

		strcpy_s(_outAscii, len, szGBK);
		delete[] szGBK; 
		delete[] wszGBK; 
	}
	catch (...)
	{
		assert(false);
	}
} 

CString g_csBuffStr;

void PrintBuff(DWORD lpSubTreeRoot)
{
	try
	{
		char szBuffer[256] = "";
		DWORD dwBuffAddr = *(DWORD*)(lpSubTreeRoot+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET3___);
		if( dwBuffAddr == 0 )
			return ;

		DWORD dwActorPacketId = *(DWORD*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_ACTOR_PACKET_ID_OFFSET___);
		int	  nBuffFlag = *(int*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_FLAG_OFFSET);
		//if( nBuffFlag != -1 )
		{
			DWORD dwBuffInfoAddr = *(DWORD*)(dwBuffAddr+MMOCLIENT_REVERSAL_ROLE_BUFF_INFO_ADDR_OFFSET);

			if( dwBuffInfoAddr != 0 )
			{
				DWORD dwBuffInfoId = *(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_ID_OFFSET___);
				char* pszBuffInfoDesc = (char*)*(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_DESC_OFFSET___);

				if( pszBuffInfoDesc == NULL )
				{
					pszBuffInfoDesc = szBuffer;
				}

				char* pszBuffInfoName = (char*)*(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_NAME_OFFSET___);

				if( pszBuffInfoName == NULL )
				{
					pszBuffInfoName = szBuffer;
				}
				char szBuffInfoName[256] = {0};
				if( pszBuffInfoName != NULL )
				{
					ConvertUtf8ToGBK(pszBuffInfoName,szBuffInfoName);
				}

				char szPrint[1024] = {0};
				sprintf_s(szPrint,sizeof(szPrint),"[LOG] addr = 0x%p buff addr = 0x%p 0x%p 对象id = 0x%p buff id = 0x%p flag = 0x%p name = %s desc = %s\r\n",
					lpSubTreeRoot,dwBuffAddr,dwBuffInfoAddr,dwActorPacketId,dwBuffInfoId,nBuffFlag,szBuffInfoName,pszBuffInfoDesc);
				OutputDebugStringA(szPrint);

				g_csBuffStr += szPrint;
			}

		}
	}
	catch (...)
	{

	}
}

CString GetRoleBuffState()
{
	g_csBuffStr = "";

	try
	{
		// dd [[[[[[0x01955820 +0C370000]+0x000000D0 ]+0x0000001C]+0x214]+0x10]+0x18]
		// dd [[[[[[0x01956820 +0C370000]+0x000000D0 ]+0x0000001C]+0x214]+0x10]+0x28]
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_ADDR_OFFSET___);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET1);


		DWORD dwBuffAddr1 = dwGameAddr2+MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET2___;

		{
			NODE	theNode = {0};
			_ReadProcessMemory(dwBuffAddr1,&theNode,sizeof(theNode));
			Traverse(theNode.str_dwNextAddrParent,dwBuffAddr1,PrintBuff);
		}

		DWORD dwBuffAddr2 = dwGameAddr2+MMOCLIENT_REVERSAL_TIAN_QI_BUFF_STATE_OFFSET___;

		{
			NODE	theNode = {0};
			_ReadProcessMemory(dwBuffAddr2,&theNode,sizeof(theNode));
			Traverse(theNode.str_dwNextAddrParent,dwBuffAddr2,PrintBuff);
		}
	}
	catch (...)
	{
		assert(false);
	}

	return g_csBuffStr;
}

//////////////////////////////////////////////////////////////////////////
// 遍历buff info



DWORD GetBuffInfoAddr(DWORD dwBuffInfoId)
{
	DWORD dwBuffInfoAddr = 0;
	try
	{
		DWORD dwGetBuffInfoAddrCall = MMOCLIENT_REVERSAL_GET_BUFF_INFO_ADDR_CALL+GetCryGameBaseAddr();
		__asm
		{
			push dwBuffInfoId
				mov eax, dwGetBuffInfoAddrCall
				call eax
				add esp, 4
				mov dwBuffInfoAddr, eax
		}
	}
	catch (...)
	{

	}
	return dwBuffInfoAddr;
}

CString GetBuffInfo()
{
	CString csTxt;

	try
	{
		for (int i=0; i<0x05000000; i++)
		{
			DWORD dwBuffInfoAddr = GetBuffInfoAddr(i);

			if( dwBuffInfoAddr != 0 )
			{
				DWORD dwBuffInfoId = *(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_ID_OFFSET___);
				char* pszBuffInfoName = (char*)*(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_NAME_OFFSET___);
				char szBuffInfoName[256] = {0};
				if( pszBuffInfoName != NULL )
				{
					ConvertUtf8ToGBK(pszBuffInfoName,szBuffInfoName);
				}
				char* pszBuffInfoDesc = (char*)*(DWORD*)(dwBuffInfoAddr+MMOCLIENT_REVERSAL_BUFF_INFO_DESC_OFFSET___);

				if( pszBuffInfoDesc != NULL )
				{
					char szPrint[256];
					sprintf_s(szPrint,"[LOG] addr = 0x%p buff id = 0x%p name = %s desc = %s\r\n",dwBuffInfoAddr,dwBuffInfoId,szBuffInfoName,pszBuffInfoDesc);
					OutputDebugStringA(szPrint);
					csTxt += szPrint;
				}
			}
		}
	}
	catch (...)
	{

	}

	return csTxt;
}

//////////////////////////////////////////////////////////////////////////
// 遍历礼包

CString g_csGiftBagInfo;

void PrintGiftBagInfo(DWORD lpSubTreeRoot)
{
	try
	{
		PMMO_GIFT_BAG_INFO ptheGiftBagInfo = (PMMO_GIFT_BAG_INFO)lpSubTreeRoot;

		DWORD	dwGiftBagAddr = *(DWORD*)(ptheGiftBagInfo->dwGiftBagAddr+MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET3___);
		DWORD	dwGetGiftBagFlag = *(DWORD*)(ptheGiftBagInfo->dwGiftBagAddr+MMOCLIENT_REVERSAL_GIFT_BAG_GET_FLAG_OFFSET___);
		char* pszGiftBagName = (char*)*(DWORD*)(dwGiftBagAddr+MMOCLIENT_REVERSAL_GIFT_BAG_INFO_NAME_OFFSET___);
		char* pszGiftBagDesc = (char*)*(DWORD*)(dwGiftBagAddr+MMOCLIENT_REVERSAL_GIFT_BAG_INFO_DESC_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] addr = 0x%p id = 0x%p get flag = %d name = %s desc = %s\r\n",lpSubTreeRoot,ptheGiftBagInfo->dwGiftBagId,dwGetGiftBagFlag,pszGiftBagName,pszGiftBagDesc);
		OutputDebugStringA(szPrint);

		g_csGiftBagInfo += szPrint;
	}
	catch (...)
	{

	}

}

CString GetGiftBagInfo()
{
	g_csGiftBagInfo = "";
	try
	{
		// [[[[[0x0C370000+0x01974DB0]+0xD0]+0x1C]+0x214]+0x0000215C]
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET1);
		dwGameAddr2 += MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET2___;

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] addr = 0x%p \r\n",dwGameAddr2);
		OutputDebugStringA(szPrint);


		NODE	theNode = {0};
		_ReadProcessMemory(dwGameAddr2,&theNode,sizeof(theNode));
		Traverse(theNode.str_dwNextAddrParent,dwGameAddr2,PrintGiftBagInfo);
	}
	catch (...)
	{

	}
	return g_csGiftBagInfo;
}

//////////////////////////////////////////////////////////////////////////
// 遍历等级信息

DWORD GetPlayerLevelUpInfoAddr(int nLevel)
{
	DWORD	dwPlayerLevelUpInfoAddr = 0;

	try
	{
		DWORD dwPlayerLevelUpInfoCall = MMOCLIENT_REVERSAL_PLAYER_LEVEL_UP_INFO_CALL+GetCryGameBaseAddr();
		__asm
		{
			push nLevel
				mov eax, dwPlayerLevelUpInfoCall
				call eax
				add esp, 4
				mov dwPlayerLevelUpInfoAddr, eax
		}

	}
	catch (...)
	{
		assert(false);
	}

	return dwPlayerLevelUpInfoAddr;
}

CString GetCPlayerLevelUpInfo()
{
	CString csTxt;

	try
	{
		char szPrint[256];
		for (int i=0; i<=55; i++)
		{
			DWORD dwPlayerLevelUpInfoAddr = GetPlayerLevelUpInfoAddr(i);
			if( dwPlayerLevelUpInfoAddr != 0 )
			{
				int nLevelMaxExp = *(int*)(dwPlayerLevelUpInfoAddr+MMOCLIENT_REVERSAL_PLAYER_LEVEL_UP_INFO_MAX_EXP_OFFSET___);
				sprintf_s(szPrint,"[LOG] addr = 0x%p level = %d max exp = %d\r\n",dwPlayerLevelUpInfoAddr,i,nLevelMaxExp);
				OutputDebugStringA(szPrint);

				csTxt += szPrint;
			}
		}

	}
	catch (...)
	{

	}

	return csTxt;
}


//////////////////////////////////////////////////////////////////////////
// 遍历王立


CString g_csKingQuestInfo;

CString GetKingQuestInfo()
{
	g_csKingQuestInfo = "";

	try
	{
		char szPrint[1024];

		DWORD dwGetKingQuestInfoAddr = GetKingQuestInfoAddr(2);		// 2王立一星和2星  3 王立三星和四星

		if( dwGetKingQuestInfoAddr != 0 )
		{
			PTASK_FB_INFO ptheKingQuestInfo = (PTASK_FB_INFO)dwGetKingQuestInfoAddr;

			for (DWORD dwKingQuestInfoAddr = ptheKingQuestInfo->theTaskArray.str_dwBeginAddr; dwKingQuestInfoAddr < ptheKingQuestInfo->theTaskArray.str_dwEndAddr1; dwKingQuestInfoAddr+=sizeof(KING_QUEST_INFO))
			{
				PKING_QUEST_INFO ptheKingQuestInfoAddr = (PKING_QUEST_INFO)dwKingQuestInfoAddr;

				/*++

				title == null 的是王立精英

				--*/
				sprintf_s(szPrint,sizeof(szPrint),"[LOG] addr = 0x%p king quest id = 0x%p city = 0x%p fb title = %s\r\n",
					dwKingQuestInfoAddr,ptheKingQuestInfoAddr->dwKingQuestId,ptheKingQuestInfoAddr->dwCityId,ptheKingQuestInfoAddr->pszFbTitle);

				OutputDebugStringA(szPrint);
				g_csKingQuestInfo += szPrint;

			}

		}

		dwGetKingQuestInfoAddr = GetKingQuestInfoAddr(3);

		if( dwGetKingQuestInfoAddr != 0 )
		{
			PTASK_FB_INFO ptheKingQuestInfo = (PTASK_FB_INFO)dwGetKingQuestInfoAddr;

			for (DWORD dwKingQuestInfoAddr = ptheKingQuestInfo->theTaskArray.str_dwBeginAddr; dwKingQuestInfoAddr < ptheKingQuestInfo->theTaskArray.str_dwEndAddr1; dwKingQuestInfoAddr+=sizeof(KING_QUEST_INFO))
			{
				PKING_QUEST_INFO ptheKingQuestInfoAddr = (PKING_QUEST_INFO)dwKingQuestInfoAddr;

				sprintf_s(szPrint,sizeof(szPrint),"[LOG] addr = 0x%p king quest id = 0x%p city = 0x%p fb title = %s\r\n",
					dwKingQuestInfoAddr,ptheKingQuestInfoAddr->dwKingQuestId,ptheKingQuestInfoAddr->dwCityId,ptheKingQuestInfoAddr->pszFbTitle);

				OutputDebugStringA(szPrint);
				g_csKingQuestInfo += szPrint;

			}

		}
	}
	catch (...)
	{

	}


	return g_csKingQuestInfo;
}
