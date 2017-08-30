#include "StdAfx.h"
#include "MMOPacket.h"
#include <assert.h>
#include "GameStructs.h"
#include "../public/UsefulFns.h"

byte g_bSendBuf[0x48452];



// 发包函数，用于程序发包，在hook send函数的下一层，hook点不会拦截到从这个函数发出的封包
bool MMO_SendPrivate::MMO_send( const void* pSendBuf, int nLen, WORD wMethod )
{
	bool bRet = false;
	try
	{
		if( pSendBuf == NULL )
		{
			return false;
		}

		if( nLen <= 0 )
		{
			return false;
		}

		if( wMethod <= 0 )
		{
			return false;
		}

		// dd [[[[[[0x024EDA60]+0x04]+0x0C]+0x40]+0x4]+0x1C]
// 		DWORD dwSocket = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
// 		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_GAME_OFFSET1);
// 		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_GAME_OFFSET2);
// 		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET3);
// 		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET4);
// 		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET5);

		DWORD dwSocket = *(DWORD*)(MMOCLIENT_REVERSAL_SOCKET_BASE_ADDR+GetCryGameBaseAddr());
		dwSocket = *(DWORD*)(dwSocket+MMOLCINET_REVERSAL_SOCKET_OFFSET1);
		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET2);
		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET3);


		if( dwSocket == 0 )
		{
			return false;
		}

		memset(g_bSendBuf,0,sizeof(g_bSendBuf));

		*(WORD*)g_bSendBuf = wMethod;

		memcpy(g_bSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___,pSendBuf,nLen);


		DWORD dwSendCall2Addr = MMOCLIENT_REVERSAL_SEND_CALL2+GetCryGameBaseAddr();
		__asm
		{
			lea eax, g_bSendBuf
			push eax
			mov ecx, dwSocket
			//push eax
			mov eax, dwSendCall2Addr
			call eax
			mov bRet, al
		}
	}
	catch (...)
	{
		assert(false);
	}

	return !bRet;
}

// 发包函数的最顶层，如果调用此函数，那么hook send的点会拦截到从此函数发出的封包
bool MMO_hooksend( const void* pSendBuf, int nLen, WORD wMethod )
{
	bool bRet = false;
	try
	{
		if( pSendBuf == NULL )
		{
			return false;
		}

		if( nLen <= 0 )
		{
			return false;
		}

		if( wMethod <= 0 )
		{
			return false;
		}

		DWORD dwSocket = *(DWORD*)MMOCLIENT_REVERSAL_SOCKET_BASE_ADDR+GetCryGameBaseAddr();
		dwSocket = *(DWORD*)(dwSocket+MMOLCINET_REVERSAL_SOCKET_OFFSET1);
		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET2);

		if( dwSocket == 0 )
		{
			return false;
		}

		memset(g_bSendBuf,0,sizeof(g_bSendBuf));

		*(WORD*)g_bSendBuf = wMethod;

		memcpy(g_bSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___,pSendBuf,nLen);

		DWORD dwSendCallAddr = MMOCLIENT_REVERSAL_SEND_CALL+GetCryGameBaseAddr();
		__asm
		{
			lea eax, g_bSendBuf
			push eax
			mov ecx, dwSocket
			mov eax, dwSendCallAddr
			call eax
			mov bRet, al
		}
	}
	catch (...)
	{
		assert(false);
	}

	return !bRet;
}


bool MMO_gamehooksend( const void* pSendBuf)
{
	bool bRet = false;
	try
	{
		if( pSendBuf == NULL )
		{
			return false;
		}

		DWORD dwSocket = *(DWORD*)MMOCLIENT_REVERSAL_SOCKET_BASE_ADDR+GetCryGameBaseAddr();
		dwSocket = *(DWORD*)(dwSocket+MMOLCINET_REVERSAL_SOCKET_OFFSET1);
		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET2);
		dwSocket = *(DWORD*)(dwSocket+MMOCLIENT_REVERSAL_SOCKET_OFFSET3);

		if( dwSocket == 0 )
		{
			return false;
		}

		//*(LONGLONG*)((DWORD)pSendBuf+0x10) = GetDynamicValue();
		//*(LONGLONG*)((DWORD)pSendBuf+0x18) = GetRand(*(LONGLONG*)(g_bSendBuf+0x10));

		DWORD dwSendCall2Addr = MMOCLIENT_REVERSAL_SEND_CALL2+GetCryGameBaseAddr();
		__asm
		{
			mov eax, pSendBuf
			push eax
			mov ecx, dwSocket
			mov eax, dwSendCall2Addr
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


bool MMO_say(char* pszSayBuf,int nSayType)
{
	MMO_SAY_MESSAGE theSayMessage;
	memset(&theSayMessage,0,sizeof(theSayMessage));
	theSayMessage.nSayType = nSayType;
	memcpy_s(theSayMessage.pszSayBuf,sizeof(theSayMessage.pszSayBuf),pszSayBuf,strlen(pszSayBuf)+1);
	return MMO_hooksend(&theSayMessage,sizeof(theSayMessage),MMOCLIENT_REVERSAL_PACKET_SAY_METHOD___);
}

// 方向w走路时的一个动态值
LONGLONG GetDynamicValue()
{
	LONGLONG llWalkDynamic = 0;
// 	try
// 	{
// 		__asm
// 		{
// 			mov edx,dword ptr ds:[MMOCLIENT_REVERSAL_GAME_ADDR]
// 			mov ecx,dword ptr ds:[edx+MMOCLIENT_REVERSAL_WALK_DYNAMEIC_OFFSET1]
// 			mov eax,dword ptr ds:[ecx]
// 			mov edx,dword ptr ds:[eax+MMOCLIENT_REVERSAL_WALK_DYNAMEIC_OFFSET2]
// 			call edx
// 			mov edx,dword ptr ds:[eax]
// 			mov ecx,eax
// 			mov eax,dword ptr ds:[edx+MMOCLIENT_REVERSAL_WALK_DYNAMEIC_OFFSET3]
// 			call eax
// 			mov DWORD PTR [llWalkDynamic], eax
// 			mov DWORD PTR [llWalkDynamic+4], edx
// 		}
// 	}
// 	catch (...)
// 	{
// 		assert(false);
// 	}

	return llWalkDynamic;
}

// 走路封包
bool MMO_walk(float fx, float fy, float fz)
{
	MMO_WALK theWalk = {0};
	theWalk.llDynamic = GetDynamicValue();
	theWalk.dst_pos.fx = fx;
	theWalk.dst_pos.fy = fy;
	theWalk.dst_pos.fz = fz;
	theWalk.dir.dir_x = -0.798296f;
	theWalk.dir.dir_y =  0.602266f;

	return MMO_sendT(theWalk, MMOCLIENT_REVERSAL_PACKET_MSG_ID_WALK___);
}

// 接受任务
bool MMO_accepttask(DWORD dwTaskId)
{
	MM0_ACCEPT_TASK	theAccpetTask = {0};

	theAccpetTask.dwTaskId = dwTaskId;
	theAccpetTask.nFlag = 0;

	return MMO_sendT(theAccpetTask,MMOCLIENT_REVERSAL_PACKET_MSG_ID_ACCEPT_TASK___);
}

// 完成任务
bool MMO_completetask(DWORD dwTaskId)
{
	return MMO_sendT(dwTaskId,MMOCLIENT_REVERSAL_PACKET_MSG_ID_COMPLETE_TASK___);
}

// 放弃任务
bool MMO_giveuptask(DWORD dwTaskId)
{
	return MMO_sendT(dwTaskId,0x00000E03);
}

// 打开NPC
bool MMO_OpenNpc(DWORD dwNpcId,int nOpenNpcType)
{
	byte bCode[] = {0x02 ,0x01 ,0x00 ,0x00 ,0x00 ,0xAC ,0x00 ,0x00 ,0x10 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	try
	{
		*(DWORD*)(bCode+5) = dwNpcId;
		*(int*)(bCode+9) = nOpenNpcType;
	}
	catch (...)
	{
		return false;
	}
	
	return MMO_sendT(bCode,16,MMOCLIENT_REVERSAL_PACKET_MSG_ID_OPEN_NPC___);
}

// 副本过图封包
bool MMO_FbGuoTu( const MMO_Vector3& theDoorCoord,char* pszGuoTuCmd)
{
	MMO_PACKET_GOTO_DOOR theGotoDoor = {0};

	theGotoDoor.theDoorCoord.fx = theDoorCoord.fx;
	theGotoDoor.theDoorCoord.fy = theDoorCoord.fy;
	theGotoDoor.theDoorCoord.fz = theDoorCoord.fz;
	memcpy_s(theGotoDoor.szDoorCmd,sizeof(theGotoDoor.szDoorCmd),pszGuoTuCmd,strlen(pszGuoTuCmd)+1);

	return MMO_sendT(theGotoDoor,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GUO_TU___);
}

// 城镇过图封包
bool MMO_CityGuoTu(char* pszGuoTuCmd)
{
	return MMO_sendT(pszGuoTuCmd,strlen(pszGuoTuCmd)+1,MMOCLIENT_REVERSAL_PACKET_MSG_ID_CITY_GUO_TU___);
}


// 进入副本封包1  
// 参数一： 副本id
// 参数二： 副本挑战等级
// 参数三： 副本类型
bool MMO_GotoFb1(DWORD dwFbId,int nDiffLevel,DWORD dwFbType)
{
	GO_TO_FB1 theGoToFb1 = {0};
	theGoToFb1.dwUnknow1 = 2;
	theGoToFb1.dwUnKnow2 = 0;
	theGoToFb1.dwFbId = dwFbId;
	theGoToFb1.nFbDiffLevel = nDiffLevel;
	theGoToFb1.dwFbType = dwFbType;
	
	return MMO_sendT(theGoToFb1,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB1___);
}



// 进入副本封包2
bool MMO_GotoFb2(DWORD dwFbId,DWORD dwType)
{
	if( dwFbId == 0 )
	{
		return false;
	}

	if( dwType == 0 )
	{
		return false;
	}
	
	byte bszCode[64] = {0x69 ,0x87 ,0x01 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00};

	try
	{
		*(DWORD*)(bszCode) = dwFbId;
		*(DWORD*)(bszCode+4) = dwType;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB2___);
}

// 进入副本封包3
bool MMO_GotoFb3(DWORD dwType)
{
	byte bszCode[] = {0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	try
	{
		*(DWORD*)(bszCode) = dwType;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB3___);
}

//dwmode 0为狩猎卷进入的，1为银币进入

bool MMO_GotoFbChooseMode(DWORD dwFbId,DWORD dwMode)
{
	GO_TO_FB_CHOOSEMODE theGoToChooseMode = {0};
	theGoToChooseMode.dwFbId=dwFbId;
	theGoToChooseMode.dwMode=dwMode;


	return MMO_sendT(theGoToChooseMode,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_CHOOSEMODE___);
}


// 物品移动到某处
bool MMO_ItmeMoveTo( const MMO_ITEM_MOVE_TO& theItemMoveTo )
{
	return MMO_sendT(theItemMoveTo,MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_TO___);
}

// 指定物品数量移动到某处
bool MMO_ItmeCountMoveTo( const MMO_ITEM_MOVE_TO_COUNT& theItemMoveTo)
{
	return MMO_sendT(theItemMoveTo,MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_COUNT_TO___);
}

// 移动指定物品数量到一个空格里
bool MMO_ItmeCountMoveToEmpty( const MMO_ITEM_MOVE_TO_COUNT& theItemMoveTo)
{
	return MMO_sendT(theItemMoveTo,MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_COUNT_TO_EMPTY___);
}



// 获取补给箱物品
// 物品id，通过模板可以获取
// 物品背包内第几格
// 获取物品的数量
bool MMO_DoBuJiXiang(DWORD dwItemId,int nItemSlotIndex,DWORD dwItemCount)
{
	MMO_DO_BU_JI_XIANG theBuJiXiang = { 0 };

	theBuJiXiang.nItemSlotIndex = nItemSlotIndex;
	theBuJiXiang.dwItemId = dwItemId;
	theBuJiXiang.dwItemCount = dwItemCount;

	return MMO_sendT(theBuJiXiang,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_ITEM___);
}

// 出售物品
bool MMO_SellItem(int nItemTabIndex,int nItemSlotIndex,int nItemCount)
{
	MMO_SELL_ITEM theSellItem = { 0 };

	theSellItem.nItemTabIndex = nItemTabIndex;
	theSellItem.nItemSlotIndex = nItemSlotIndex;
	theSellItem.nItemCount = nItemCount;

	return MMO_sendT(theSellItem,MMOCLIENT_REVERSAL_PACKET_MSG_ID_SELL_ITEM___);
}

// 制作回复药
bool MMO_facture()
{
	byte bszCode[] = {0x02 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_FACTURE___);
}

// 调和制作物品 
// 参数1: 遍历调和物品的id，非物品id本身  
// 参数2: 背包=1 仓库=2  用于制作完物品是直接放到仓库还是背包的选择
bool MMO_ManuFactureItem(DWORD dwManuFactureId,DWORD dwTabIndex)
{
	byte bszCode[] = {0x02 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	*(DWORD*)(bszCode) = dwManuFactureId;
	*(DWORD*)(bszCode+4) = dwTabIndex;

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_FACTURE___);
}

// 获取邮件物品
bool MMO_GetMailItem(LONGLONG llMailId,int nItemTabIndex)
{
	MMO_GET_MAIL_ITEM theMailItem = { 0 };

	theMailItem.llMailId = llMailId;
	theMailItem.bFlag = 0;
	theMailItem.nUnKnow = -1;
	theMailItem.nItemTabIndex = nItemTabIndex;		// 0 表示收件到背包， 1表示收件到仓库

	return MMO_sendT(theMailItem,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_MAIL_ITEM___);
}

// 打开邮箱，获取邮件信息
bool MMO_OpenMail()
{
	byte bszCode[] = {0x00 ,0x00 ,0x00 ,0xFF};

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_OPEN_MAIL___);
}

// 打开当个邮件获取邮件的具体信息
bool MMO_GetMailItemInfo(LONGLONG llMailId)
{
	MMO_GET_MAIL_ITEM_INFO theMailItemInfo = {0};
	theMailItemInfo.llMailId = llMailId;

	return MMO_sendT(theMailItemInfo,MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_MAIL_ITEM_INFO___);
}

// 测试无效，但是游戏也只发了这一个封包，可能跟界面有关系
bool MMO_GetMailInfo(LONGLONG llMailId)
{
	return MMO_sendT(llMailId,0x00001B05);
}


// 删除邮件
bool MMO_DeleteMail(LONGLONG llMailId)
{
	MMO_DELETE_MAIL theDelteMail = { 0 };

	theDelteMail.nMailDeleteFlag = 1;
	theDelteMail.llMailId = llMailId;

	return MMO_sendT(theDelteMail,MMOCLIENT_REVERSAL_PACKET_MSG_ID_DELETE_MAIL___);
}


bool MMO_TaskNpcChat(DWORD dwTaskId)
{
	byte bszCode[] = {0x02 ,0x00 ,0x00 ,0x00 ,0xBD ,0x0B ,0x00 ,0x00};

	try
	{
		*(DWORD*)(bszCode+4) = dwTaskId;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_TASK_NPC_CHAT___);
}

bool MMO_ChooseRole(DWORD dwRoleId)
{
	return MMO_sendT(dwRoleId,0x00000007);
}

bool MMO_DeleteRole(DWORD dwRoleId)
{
	return MMO_sendT(dwRoleId,0x00000005);
}

bool MMO_ChooseLine(DWORD dwLineId)
{
	return MMO_sendT(dwLineId,0x00000051);
}

bool MMO_RetutnChooseRole(DWORD dwRoleId)
{
	return MMO_sendT(dwRoleId,0x00000011);
}

bool MMO_BuyItem(DWORD dwShopItemId, int nCount,int nBuyShopId)
{
	MMO_BUY_ITEM theBuyItem = { 0 };
	theBuyItem.dwBuyItemFlag = nBuyShopId;
	theBuyItem.dwShopId = dwShopItemId;
	theBuyItem.nCount = nCount;
	theBuyItem.dwFFFFFFFF1 = -1;
	theBuyItem.dwFFFFFFFF2 = -1;

	return MMO_sendT(theBuyItem,0x00000F01);
}

bool MMO_CraftItem(DWORD dwCraftId)
{
	return MMO_sendT(dwCraftId,0x00000903);
}

// 升级武器
bool MMO_UpEquip( const MMO_SEND_UP_EQUIP_PACKET& theUpEquip)
{
	return MMO_sendT(theUpEquip,MMOCLIENT_REVERSAL_PACKET_MSG_ID_UP_EQUIP___);
}

bool MMO_CreateRole( const char* pszRoleName)
{
	byte bszCode[] = { 0x31 ,0x32 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
					   0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
 					   0x00 ,0x01 ,0x00 ,0x01 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x39 ,0x3D ,0x48 ,0x00 ,0x96, \
					   0x96 ,0x96 ,0x00 ,0x32 ,0x00 ,0x00 ,0x00 ,0x60 ,0x71 ,0x7B ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
					   0x00 ,0xFF
	};

	try
	{
		if( strlen(pszRoleName) > 20 )
		{
			return false;
		}
		memcpy_s(bszCode,20,pszRoleName,strlen(pszRoleName)+1);
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode), MMOCLIENT_REVERSAL_PACKET_MSG_ID_CREATE_ROLE___);
}

bool MMO_LeaveZhuangYuan()
{
	DWORD dwLevae = 1;
	return MMO_sendT(dwLevae,0x0000000F);
}

// dd [[[[0x01926D00+0C370000]+0x000000CC]+0x0000001C]+0x214]+0x00004030 
LONGLONG GetZhuangYuanId()	// 获取角色自身庄园id
{
	DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
	dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

	LONGLONG llZhuangYuanId = *(LONGLONG*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_ZHUANG_YUAN_ID_OFFSET);

	assert(llZhuangYuanId);

	return llZhuangYuanId;
}



bool MMO_GotoZhuangYuan(LONGLONG llZhuangYuanId)
{
	byte bszCode[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
	try
	{
		*(LONGLONG*)(bszCode+4) = llZhuangYuanId;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),0x00000033);
}

bool MMO_UpCollect(int nCollectIndex)
{
	byte bszCode[] = { 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
	try
	{
		*(int*)(bszCode+4) = nCollectIndex;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),0x000002F2);
}

// 拆解素材
bool MMO_Dismantling( const MMO_DISMANTLING& theDismantling)
{
	return MMO_sendT(theDismantling,0x00003121);
}

// 接受公会副本任务  参数一：从收包处获取
bool MMO_AcceptGongHuiTask(DWORD dwTaskFbLevelType,DWORD dwGongHuiTaskId)
{
	MMO_ACCEPT_GONG_HUI_TASK theTaskAddr = {0};

	try
	{
		theTaskAddr.dwTaskFlag = MMOCLIENT_SOCIATY_TASK_FLAG;
		theTaskAddr.dwTaskFbId = dwGongHuiTaskId;
		theTaskAddr.dwUnKnow = MMOCLIENT_SOCIATY_TASK_UNKNOWN_DATA;
	}
	catch (...)
	{
		assert(false);
		return false;
	}

	return MMO_sendT(theTaskAddr,MMOCLIENT_SOCIATY_TASK_MSG_ID);
}

bool MMO_AcceptQianShouTask(DWORD dwQianShouFbId)
{
	MMO_ACCEPT_GONG_HUI_TASK theTaskAddr = {0};

	try
	{
		theTaskAddr.dwTaskFlag = 4;
		theTaskAddr.dwTaskFbLevel = 0x00000191;
		theTaskAddr.dwTaskFbId = dwQianShouFbId;
		theTaskAddr.dwUnKnow = 0x000075B6;
	}
	catch (...)
	{
		assert(false);
		return false;
	}

	return MMO_sendT(theTaskAddr,0x00000035);
}



// 查询拍卖物品信息
bool MMO_ReqItemBytype(byte bTabIndex, byte bTypeIndex,byte bSlotIndex)
{
	byte bszCode[] = { 0x04 ,0x00 ,0x00 ,0x00 ,0x01 ,0x01 ,0x01 ,0x00 ,0x00 ,0xE7 ,0x03 ,0x09 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	static int nReqCount = 0;
	nReqCount++;
	try
	{
		*(int*)bszCode = nReqCount;		// 请求拍卖次数的累加
		*(int*)(bszCode+4) = bTabIndex;		// 普通物品 = 1 装备 = 2 宠物装备 = 4
		*(int*)(bszCode+5) = bTypeIndex;
		*(int*)(bszCode+6) = bSlotIndex;
	}
	catch (...)
	{
		return false;
	}

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_PACKET_MSG_ID_REQ_ITEM_BY_TYPE___);
}

bool MMO_ReqItemBytype(const char* pszReqItemName,DWORD dwItemId)
{
	static int nReqCount = 0;
	nReqCount++;

	MMO_REQ_ITEM_BY_SEND_STRUCT theReqItem = { 0 };

	if( strlen(pszReqItemName) >= sizeof(theReqItem.szSearchName) )
	{
		assert(false);
		return false;
	}

	theReqItem.nReqCount = nReqCount;
	theReqItem.bTabIndex = 0;
	theReqItem.bTypeIndex = 0;
	theReqItem.bSlotIndex = 0;
	theReqItem.wUnKnow1 = 0;
	theReqItem.wGuDingValue1 = 0x03E7;
	theReqItem.wGuDingValue2 = 0x0009;
	memcpy_s(theReqItem.szSearchName,sizeof(theReqItem.szSearchName),pszReqItemName,strlen(pszReqItemName)+1);		// 这个关键字的作用并不大
	theReqItem.bItemIdCount = 1;		// 搜索的物品id数量
	theReqItem.dwItemId[0] = dwItemId;	// 万能虫网
	return MMO_sendT(theReqItem,MMOCLIENT_REVERSAL_PACKET_MSG_ID_REQ_ITEM_BY_TYPE___);
}



// 查询拍卖物品信息后，翻页
bool MMO_GetSearchItemListByPage(int nPageIndex)
{
	MMO_GET_SEARCH_ITEM_LIST_BY_PAGE theSearchItemPage = { 0 };
	theSearchItemPage.nTabIndex = 4;
	theSearchItemPage.nPageIndex = nPageIndex;
	theSearchItemPage.nPageCount = 8;

	return MMO_sendT(theSearchItemPage,0x0000150A);
}

// 拍卖购买物品
bool MMO_ReqBuyItem( const MMO_REQ_BUY_ITEM& theReqBuyItem)
{
	return MMO_sendT(theReqBuyItem,0x00001505);
}

// 拍卖刷新我的拍卖物品
bool MMO_GetMySellDetail()
{
	byte bszCode[] = { 0x8 ,0x00 ,0x00 ,0x00};
	return MMO_sendT(bszCode,sizeof(bszCode),0x00001511);
}

// 上架拍卖物品
bool MMO_ShangJiaAuctionItem( const MMO_SHANG_JIA_AUCTION_ITEM& theAuctionItem)
{
	return MMO_sendT(theAuctionItem,MMOCLIENT_REVERSAL_PACKET_MSG_ID_SHANG_JIA_AUCTION_ITEM___);
}

// 邀请组队
bool MMO_InviteTeam(DWORD dwActorPacketId)
{
	return MMO_sendT(dwActorPacketId,0x00000701);
}


// 申请组队
bool MMO_JoinTeam(DWORD dwActorPacketId,char* pszTeamPwd)
{
	MMO_JOIN_TEAM theJoinTeam = { 0 };
	theJoinTeam.dwActorPacketId = dwActorPacketId;
	memcpy_s(theJoinTeam.szTeamPwd,sizeof(theJoinTeam.szTeamPwd),pszTeamPwd,strlen(pszTeamPwd)+1);

	return MMO_sendT(theJoinTeam,0x00000707);
}

// 退出队伍
bool MMO_ReqQuitTeam()
{
	byte bszCode[] = { 0x00 ,0x00 ,0x00 ,0x00};
	return MMO_sendT(bszCode,sizeof(bszCode),0x0000070C);
}

// 请离队伍
bool MMO_ReqKickMember(DWORD dwActorPacketId)
{
	return MMO_sendT(dwActorPacketId,0x0000070F);
}

// 创建一个密码队伍
bool MMO_CreateTeamReq(char* pszTeamPwd)
{
	byte bszCode[] = {	0xC7 ,0xA7 ,0xE1 ,0xF7 ,0xB8 ,0xB1 ,0xB1 ,0xBE ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
						0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
						0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
						0x00 ,0x00 ,0x00 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x31 ,0x00, \
						0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
						0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xC3 ,0xBE, \
						0x79 ,0x00 ,0x01 ,0x00 ,0x37 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
	};
	try
	{
		memcpy_s(bszCode+0x3E,8,pszTeamPwd,strlen(pszTeamPwd));
	}
	catch (...)
	{
		
	}

	return MMO_sendT(bszCode,sizeof(bszCode),0x0000071E);
}

bool MMO_ServerLineLaunch(int nServerLineIndex)
{
	return MMO_sendT(nServerLineIndex,0x00000055);
}

// 请求获取公会任务，千狩，喵喵
bool MMO_RequestEntrustList(/*DWORD dwFbTypeIndex,*/DWORD dwFbKeyFlag)
{
	MMO_REQUEST_ENTRUST_LIST theEntrustList = { 0 };
	theEntrustList.dwFbKeyFlag = dwFbKeyFlag;

	return MMO_sendT(theEntrustList,0x0000003D);
}

// 请求获取激运副本信息
bool MMO_GetInstanceEnterData()
{
	DWORD dwReqGetInstanceEnterDataFlag = 1;
	return MMO_sendT(dwReqGetInstanceEnterDataFlag,0x00004201);
}

// 坐气球前往梅杰波尔坦
bool MMO_ZuoQiQiuQianWangMeiJieBoErTan()
{
	byte bszCode[] = {0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00};

	return MMO_sendT(bszCode,sizeof(bszCode),0x00002401);
}

// 放弃关卡
bool MMO_OnReturnTown_OK()
{
	byte bszCode[] = {0x01 ,0x00 ,0x00 ,0x00 };

	return MMO_sendT(bszCode,sizeof(bszCode),0x0000000F);
}

// 种植种子 参数为物品种子id
bool MMO_FramSeed(DWORD dwFramSeedId,int nFramIndex)
{
	byte bszCode[] = {0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00};

	*(DWORD*)(bszCode) = dwFramSeedId;
	*(int*)(bszCode+4) = nFramIndex;

	return MMO_sendT(bszCode,sizeof(bszCode),0x000002FB);
}

// 发送邮件
bool MMO_SendMail( const MMO_SEND_MAIL& theSendMail)
{
	return MMO_sendT(theSendMail,0x00001B09);
}

// 领取礼包
bool MMO_GetGfitBag(DWORD dwGfitBagId)
{
	return MMO_sendT(dwGfitBagId,0x00002601);
}

// 整理背包
bool MMO_TidyInventoryItem(PMMO_TIDY_ITEM theTidyItem)
{
	return MMO_sendT(*theTidyItem,0x0000300B);
}

// 新手训练-离开教学
bool MMO_XinShouXunLian_LiKaiJiaoXue()
{
	byte bszCode[] = { 0x02 ,0x00 ,0x00 ,0x00 };
	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_XIN_SHOU_XUN_LIAN_LI_KAI_JIAO_XUE___);
}

// 进入教学副本
bool MMO_JinRuJiaoXueFb(DWORD dwJiaoXueFbId )
{
	byte bszCode[] = { 0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};

	try
	{
		*(DWORD*)(bszCode+4) = dwJiaoXueFbId;
	}
	catch (...)
	{
		
	}

	return MMO_sendT(bszCode,sizeof(bszCode),MMOCLIENT_REVERSAL_JIN_RU_JIAO_XUE_FB___);
}



// 获取剩余可领取周狩猎券
int GetWeekCreditCount()
{
	int nWeekCreditCount = 0;

	try
	{
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}

		dwGameAddr2 += MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET2___;

		int nWeekCreditIndex = 2;

		DWORD dwWeekCreditInfoCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_WEEK_CREDIT_INFO_CALL;
		DWORD dwWeekCreditCountCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_CALL;
		__asm
		{
			mov esi, dwGameAddr2

			lea edx, nWeekCreditIndex
			push edx

			mov ecx, esi
			mov eax, dwWeekCreditInfoCall
			call eax

			cmp eax, esi
			je EXIT

			mov eax, dword ptr [eax+MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET___]
			test eax, eax
			je EXIT

			mov ecx, eax
			mov eax, dwWeekCreditCountCall
			call eax
			mov nWeekCreditCount, eax
EXIT:
		}
	}
	catch (...)
	{
		assert(false);
		return -1;
	}

	return nWeekCreditCount;
}

// 获取已领取的周狩猎券数量
int GetHasReceiveWeekCreditCount()
{
	int nHasReceiveWeekCreditCount = 0;

	try
	{
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}

		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET1);
		if( IsUnValidPtr(dwGameAddr2) )
		{
			assert(false);
			return -1;
		}

		dwGameAddr2 += MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET2___;

		int nWeekCreditIndex = 2;

		DWORD dwWeekCreditInfoCall = GetCryGameBaseAddr()+MMOCLIENT_REVERSAL_WEEK_CREDIT_INFO_CALL;

		__asm
		{
			mov esi, dwGameAddr2

			lea edx, nWeekCreditIndex
			push edx

			mov ecx, esi
			mov eax, dwWeekCreditInfoCall
			call eax

			cmp eax, esi
			je EXIT

			mov eax, dword ptr [eax+MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET___]
			test eax, eax
			je EXIT

			mov eax, dword ptr [eax+MMOCLIENT_REVERSAL_WEEK_CREDIT_HAS_RECEIVE_COUNT_OFFSET___]

			mov nHasReceiveWeekCreditCount ,eax
EXIT:
		}
	}
	catch (...)
	{
		assert(false);
		return -1;
	}

	return nHasReceiveWeekCreditCount;
}


// 获取周狩猎券  参数为数量  获取前先判断当前拥有的狩猎券，如果为0时获取，不因为没有用完的狩猎券是留不到第2天的
// 返回值为false时，一是给的数量大于可领取的数量，二是没有可以领取的狩猎券
bool MMO_GetWeekCreditCount( int nWeekCredietCount, MMO_WEEK_CREDIT& theWeekCredit )
{
	ZeroMemory(&theWeekCredit, sizeof(theWeekCredit));

	if( 0 >= GetWeekCreditCount() || GetWeekCreditCount() < nWeekCredietCount )
	{
		return false;
	}

	theWeekCredit.dwWeekCreditIndex = 2;
	theWeekCredit.nWeekCreditCount1 = nWeekCredietCount;
	theWeekCredit.nHasReceiveWeekCreditCount = GetHasReceiveWeekCreditCount();
	if( theWeekCredit.nHasReceiveWeekCreditCount == -1 )
	{
		assert(false);
		return false;
	}
	theWeekCredit.nWeekCreditCount2 = nWeekCredietCount;
	return true;
}


LONGLONG GetSynCoorDinatesRandom()
{
	DWORD dwGameAddr = MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr();
	LONGLONG llSynRandom = 0;
	try
	{
		__asm
		{
			MOV EDX,DWORD PTR DS:[dwGameAddr]		// 照抄汇编代码
			MOV EDX,DWORD PTR [EDX]
			MOV ECX,DWORD PTR DS:[EDX+0x4]
			MOV EAX,DWORD PTR DS:[ECX]
			MOV EDX,DWORD PTR DS:[EAX+0x38]
			CALL EDX
			MOV EDX,DWORD PTR DS:[EAX]
			MOV ECX,EAX
			MOV EAX,DWORD PTR DS:[EDX+0x228]
			CALL EAX
			mov DWORD PTR [llSynRandom], eax
			mov dword ptr [llSynRandom+4], edx
		}
	}
	catch (...)
	{
		assert(false);
	}

	return llSynRandom;
}

bool MMO_SynCoorDinates( const SYN_COOR_DINATES& theSynCoorDinates)
{
	if( theSynCoorDinates.llSysRandom == 0 )
	{
		return false;
	}
	return MMO_sendT(theSynCoorDinates, MMOCLIENT_REVERSAL_PACKET_SYN_COOR_DINATES___);
}



bool MMO_AddFrient( const char* pszFrientName)
{
	char		szFrientName[0x15] = {0};		// 好友名字

	if( strlen(szFrientName) >= 0x15 )
	{
		assert(false);
		return false;
	}

	memcpy_s(szFrientName,sizeof(szFrientName),pszFrientName,strlen(pszFrientName)+1);

	return MMO_sendT(szFrientName,MMOCLIENT_REVERSAL_PACKET_SYN_ADD_FRIENT_NAME___);
}


// 请求交易
bool MMO_Request_Deal(DWORD dwDealRoleId)
{
	return MMO_sendT(dwDealRoleId,0x00001102);
}

// 接受交易请求
bool MMO_Accept_Deal(DWORD dwRequest_DealKey)
{
	return MMO_sendT(dwRequest_DealKey,0x00001902);
}

// 移动物品到交易栏
bool MMO_Deal_Move_Item( const MMO_DEAL_MOVE_ITEM& theDealMoveItem )
{
	return MMO_sendT(theDealMoveItem,0x00001106);
}

// 锁定交易栏
bool MMO_Lock_Deal()
{
	int nLockDealIndex = 2;
	return MMO_sendT(nLockDealIndex,0x00001110);
}

// 确认交易
bool MMO_Enter_Deal()
{
	int nEnterDealIndex = 3;
	return MMO_sendT(nEnterDealIndex,0x00001110);
}

// 交易金币
bool MMO_Deal_Gold(int nGold)
{
	return MMO_sendT(nGold,0x0000110C);
}

// 解锁王立精英副本
bool MMO_UnLock_KingFb(DWORD dwKingFbId)
{
	return MMO_sendT(dwKingFbId,0x00005115);
}

















////////////////////////////////////////////////////////////////////////////////////////////
// 
// 016882A2    8B0D E0E43102   MOV ECX,DWORD PTR DS:[0x231E4E0]         ; MHOClien.02320BF8
// 016882A8    56              PUSH ESI
// 016882A9    E8 F23EE1FF     CALL MHOClien.0149C1A0

LONGLONG GetRand(LONGLONG llDynamic)
{
	LONGLONG llRet = 0;
	int nrandvalue = rand();

	DWORD dwRandAddr = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_RAND_BASE_ADDR+GetCryGameBaseAddr());	// // MOV ECX,DWORD PTR SS:[ESP+0x14]

	__asm
	{
		mov eax, nrandvalue

		AND EAX,0xFFFF
		CDQ
		MOV ECX,EAX
		mov edx,dword ptr[llDynamic+4]
		MOV ESI,EDX
		
		mov eax, dwRandAddr
		mov edi, dword ptr[eax+MMOCLIENT_REVERSAL_GAME_RAND_OFFSET1___]		// MOV EBP,DWORD PTR DS:[ECX+0x164]
		MOV EAX,edi
		CDQ
		XOR EDX,EDX
		OR EDX,DWORD PTR SS:[llDynamic]	// OR EDX,DWORD PTR SS:[ESP+0x3C]
		XOR EBX,EBX
		OR EAX,EBX
		SHLD EDX,EAX,0x10
		SHL EAX,0x10
		OR ECX,EAX
		
		mov dword ptr[llRet], ecx	// MOV DWORD PTR SS:[ESP+0x21C],ECX
		OR ESI,EDX
		INC edi

		mov eax, dwRandAddr
		MOV DWORD PTR DS:[EAX+MMOCLIENT_REVERSAL_GAME_RAND_OFFSET1___],edi

		MOV DWORD PTR SS:[llRet+4],ESI	// MOV DWORD PTR SS:[ESP+0x220],ESI
	}

	return llRet;
}

void mmo_copy(void* pSendBuf)
{
	memcpy(g_bSendBuf,pSendBuf,sizeof(g_bSendBuf));
}


bool MMO_Hit(void* pSendBuf, const MMO_Vector3& pos, const MMO_Direction2& dir)
{
	try
	{

		

		MMO_HIT theHit = {0};
// 		theHit.llDynamic = GetDynamicValue();
// 		theHit.llRand = GetRand(theHit.llDynamic);
// 		theHit.dir.dir_x = dir.dir_x;
// 		theHit.dir.dir_y = dir.dir_y;
// 		theHit.scr_pos.fx = pos.fx;
// 		theHit.scr_pos.fy = pos.fy;
// 		theHit.scr_pos.fz = pos.fz;
// 		theHit.wzero1 = 1;
// 		theHit.dir_unknow1.fx = 0.0f;
// 		theHit.dir_unknow1.fy = 1.0f;
// 		theHit.dir_unknow1.fz = 1.0f;
// 		theHit.wflag = 1;
// 		theHit.wUnKnow1 = 0x2C2;
// 		theHit.wUnKnow2 = 0;
// 		theHit.fHitTime = 0.1572972f;
// 		theHit.fUnKnow2 = 1.0f;
// 		theHit.wUnKnow3 = 0x204;

// 		*(LONGLONG*)(g_bSendBuf+0x10) = GetDynamicValue();
// 		*(LONGLONG*)(g_bSendBuf+0x18) = GetRand(*(LONGLONG*)(g_bSendBuf+0x10));

		return MMO_hooksend(g_bSendBuf,sizeof(theHit),MMOCLIENT_REVERSAL_PACKET_MSG_ID_HIT___);
		//memcpy_s((byte*)pSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___,sizeof(theHit),&theHit,sizeof(theHit));

		//return MMO_sendT(theHit,MMOCLIENT_REVERSAL_PACKET_MSG_ID_HIT___);
		return MMO_hooksend(pSendBuf,sizeof(theHit),MMOCLIENT_REVERSAL_PACKET_MSG_ID_HIT___);


		/*
		DWORD dwDynamic1 = (DWORD)theHit.llDynamic;
		DWORD dwDynamic2 = 0;
		DWORD dwRand1 = (DWORD)theHit.llRand;
		DWORD dwRand2 = 0;

		__asm
		{
			mov eax, dword ptr[theHit.llDynamic+4]
			mov dwDynamic2, eax
			mov eax, dword ptr[theHit.llRand+4]
			mov dwRand2, eax
		}

		char szPrint[256] = {0};
		sprintf_s(szPrint,sizeof(szPrint),"[LOG] %p%p %p%p",dwDynamic2,dwDynamic1,dwRand2,dwRand1);
		OutputDebugStringA(szPrint);
		*/
	}
	catch (...)
	{
		assert(false);
	}

	
	return false;
}