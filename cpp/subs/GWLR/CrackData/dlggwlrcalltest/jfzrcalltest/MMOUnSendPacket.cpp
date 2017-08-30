#include "StdAfx.h"
#include "MMOUnSendPacket.h"
#include "MMOClientTraverse.h"
#include "public/MMOPacket.h"
#include "public/UsefulFns.h"
#include "public/GameStructs.h"

MMOUnSendPacket theMmoUnSendPacket;

MMOUnSendPacket::MMOUnSendPacket(void)
{
}

MMOUnSendPacket::~MMOUnSendPacket(void)
{
}


bool PrintSend(void* pszSendBuf,void* pCallBack)
{
	return theMmoUnSendPacket.MmoPrintSend(pszSendBuf,pCallBack);
}


DWORD GetSendCallJmpAddr()
{
	return MMOCLIENT_REVERSAL_SEND_CALL+GetCryGameBaseAddr();
}

_declspec(naked) void HookSend()
{
	__asm
	{
		pushfd
		pushad

		mov eax, dword ptr[esp+0x24]
		push eax
		mov eax, dword ptr[esp+0x24+8]
		push eax
		call PrintSend
		add esp, 8

		test al,al
		je NOT_SEND_PACKET

		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH ESI
		MOV ESI,ECX

		mov eax, GetSendCallJmpAddr
		call eax
		add eax, 0x06
		jmp eax

NOT_SEND_PACKET:

		popad
		popfd
		sete al
		retn 4
	}
}

bool MMOUnSendPacket::MmoPrintSend(void* pszSendBuf,void* pCallBack)
{
	try
	{
		WORD wMsgId = *(WORD*)pszSendBuf;
		switch(wMsgId)
		{
		case 0x00:
		case 0x28d:
		case 0x504:
		case 0x286:
		case 0x251:
		case 0x291:
		case 0x2A9:
		case 0x293:
		case 0x2c5:
			{
				return true;
			}
		}
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG]zhp call = 0x%p msg id = %p",pCallBack,wMsgId);
		OutputDebugStringA(szPrint);
		switch(wMsgId)
		{
		case MMOCLIENT_REVERSAL_PACKET_SYN_COOR_DINATES___:		//	 同步坐标封包
			{
				/*sprintf_s(szPrint,"[LOG] call = 0x%p msg id = %p",pCallBack,wMsgId);
				OutputDebugString(szPrint);*/
// 				return false;
			}
			break;

		default:
			{
				/*sprintf_s(szPrint,"[LOG] call = 0x%p msg id = %p",pCallBack,wMsgId);
				OutputDebugString(szPrint);*/

				switch (wMsgId)
				{
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_HIT___:		
					{
						return theMmoUnSendPacket.MmoPacketHit(pszSendBuf);
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_WALK___:
					{
						return true;
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_GUO_TU___:		// 拦截游戏的过图封包
					{
						return theMmoUnSendPacket.MmoPacketGuoTu(pszSendBuf);
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_CITY_GUO_TU___:	// 城镇过图封包
					{
						return theMmoUnSendPacket.MmoPacketCityGuoTu(pszSendBuf);
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_TO___:
					{
						return true;
					}
 				case 0x00000C01:
					{
						sprintf_s(szPrint,"[LOG] call = 0x%p msg id = %p collect id = 0x%p",pCallBack,wMsgId,*(DWORD*)((DWORD)pszSendBuf+0x10));
						OutputDebugStringA(szPrint);
					}
					break;
				case 0x00000251:		// 角色动作同步
					{
						
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_FACTURE___:		// 制作物品
					{
						
					}
					break;	
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_BUY_REQ_ITEM___:			// 购买拍卖物品
					{	
						
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_SHANG_JIA_AUCTION_ITEM___:		// 拍卖上架物品
					{
						
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_REQ_ITEM_BY_TYPE___:
					{
						return theMmoUnSendPacket.MmoPacketReqItembyType(pszSendBuf);
					}
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_UP_EQUIP___:		// 升级装备
					{
						return false;
					}
					break;
				case MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB2___:
					{
						DWORD dwFbId = *(DWORD*)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
						sprintf_s(szPrint,"[LOG] 开始猎守副本 id = 0x%p",dwFbId);
						OutputDebugStringA(szPrint);
					}
					break;
				case 0x00000705:
				case 0x00000033:
				case 0x0000000F:
					{
						
					}
					break;
				default:
					{
						
					}
				}
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

void MMOUnSendPacket::MmoHookSend(void)
{
	Hook(MMOCLIENT_REVERSAL_SEND_CALL+GetCryGameBaseAddr(),HookSend);
}

void MMOUnSendPacket::MmoUnHookSend(void)
{
// 	0070CEB0    56                   PUSH ESI
// 	0070CEB1    8BF1                 MOV ESI,ECX
// 	0070CEB3    83BE 24010000 00     CMP DWORD PTR DS:[ESI+0x124],0x0

	byte bCode[] = {0x55 ,0x8B ,0xEC ,0x56 ,0x8B};
	_WriteProcessMemory(MMOCLIENT_REVERSAL_SEND_CALL+GetCryGameBaseAddr(),bCode,sizeof(bCode));
}

bool MMOUnSendPacket::MmoPacketHit(void* pszSendBuf)
{
//	PMMO_HIT ptheHit = NULL;
	try
	{
		OutputDebugStringA("[LOG] 攻击封包!");
//		MMOActor theActor;
//		theActor.ReadActor();
//		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
// 		MMO_Hit(pszSendBuf,theActor.m_thePos,theActor.m_theDir);
//		return false;

		
		
//		ptheHit = (PMMO_HIT)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

// 		ptheHit->llDynamic = GetDynamicValue();
// 		ptheHit->llRand = GetRand(ptheHit->llDynamic);
// 
// 		ptheHit->dir.dir_x = theActor.m_theDir.dir_x;
// 		ptheHit->dir.dir_y = theActor.m_theDir.dir_y;
// 
// 		ptheHit->scr_pos.fx = theActor.m_thePos.fx;
// 		ptheHit->scr_pos.fy = theActor.m_thePos.fy;
// 		ptheHit->scr_pos.fz = theActor.m_thePos.fz;
// 
// 		ptheHit->wUnKnow1 = 0x2C2;
// 		ptheHit->fHitTime = 0.1572972f;
// 		ptheHit->wUnKnow3 = 0x204;;

// 		static bool bFalg = false;
// 
// 		if( bFalg == false )
// 		{
// 			//bFalg = true;
// 			mmo_copy(pszSendBuf);
// 		}

// 		for (int i=0; i<100; i++)
// 		{
// 			MMO_gamehooksend(pszSendBuf);
// 		}
		
		//return false;
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}


bool MMOUnSendPacket::MmoPacketGuoTu(void* pszSendBuf)
{
	try
	{
		PMMO_PACKET_GOTO_DOOR ptheGotoDoor = (PMMO_PACKET_GOTO_DOOR)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 过图封包 : coord = %f %f %f cmd = %s",
			ptheGotoDoor->theDoorCoord.fx,
			ptheGotoDoor->theDoorCoord.fy,
			ptheGotoDoor->theDoorCoord.fz,
			ptheGotoDoor->szDoorCmd);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		
	}

	return true;
}


bool MMOUnSendPacket::MmoPacketCityGuoTu(void* pszSendBuf)
{
	try
	{
		MMOActor theActor;
		char* pszCityGuoTuCmd = (char*)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 城镇过图封包 cmd = %s name = %s",pszCityGuoTuCmd,theActor.GetRoleInMap());
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{

	}

	return true;
}



bool MMOUnSendPacket::MmoPacketReqItembyType(void* pszSendBuf)
{
	try
	{
		char szPrint[256];

		sprintf_s(szPrint,"[LOG] MmoPacketReqItembyType %d %d %d",
		*(byte*)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+4),
		*(byte*)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+5),
		*(byte*)((DWORD)pszSendBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+6)
		);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// hook 收包
MMOUnRecvPacket theMmoUnRecvPacket;

MMOUnRecvPacket::MMOUnRecvPacket(void)
{

}

MMOUnRecvPacket::~MMOUnRecvPacket(void)
{

}

void TestRecv(void* pszRecvBuf)
{
	PMMO_PACKET_HEADER pthePacketHeader = (PMMO_PACKET_HEADER)pszRecvBuf;

	try
	{
		for (DWORD i = 0; i<(DWORD)pthePacketHeader->dwPacketMsgLen; i+=1 )
		{
			DWORD dwValue = *(DWORD*)((DWORD)pszRecvBuf+i+sizeof(MMO_PACKET_HEADER));
			if( dwValue == 0x058E3460 )
			{
				char szPrint[256] = {0};
				sprintf_s(szPrint,"[LOG] 搜索到的封包id: 0x%p 偏移:0x%p",pthePacketHeader->wPacketMsgId,i+sizeof(MMO_PACKET_HEADER));
				OutputDebugStringA(szPrint);
			}
		}
	}
	catch (...)
	{
		
	}
}


bool PrintRecv(void* pszRecvBuf, int nLen)
{
// 	byte bCmdType = *(byte*)pszRecvBuf;
// 
// 	if( bCmdType != 0 )
// 	{
// 		return true;
// 	}
// 
// 	*(int*)&pszRecvBuf +=1;
// 
// 	if( nLen == 0 )
// 		nLen = *(int*)((DWORD)pszRecvBuf+0x4);
// 
// 
// 	WORD wMssageId = *(WORD*)pszRecvBuf;

	WORD wMssageId = *(WORD*)pszRecvBuf;

	nLen = *(int*)((DWORD)pszRecvBuf+0x4);

	//TestRecv(pszRecvBuf);
	

	switch(wMssageId)
	{
// 	case 0x508:		// 副本过滤封包
 	case 0x506:
// 	case 0x280:
// 	case 0x2d1:
// 	case 0x2ea:
// 	case 0x281:
// 	case 0x402:
// 	case 0x290:
// 	case 0x504:
// 	case 0x264:
// 	case 0x295:
// 	case 0x274:
// 	case 0x26d:
// 	case 0x403:
// 	case 0x2701:
		{

		}
		break;
 	case 0x2ef:			// 城镇过滤封包
 	case 0x509:
 	case 0x104:
	case 0x25e:
	case 0x261:
	case 0x3043:
	case 0x1702:
	case 0x1727:
	case 0x263:
	case 0x299:
	case 0x3403:
	case 0x2162:
	case 0x2161:
	case 0x288:
	case 0x28A:
	case 0x262:
	case 0x505:
	case 0x2d1:
	case 0x403:
	case 0x402:
		{
			//*(WORD*)pszRecvBuf = 0;
		}
		break;
	case 0x56F:
#if _DEBUG
		OutputDebugStringA("[LOG] 到精彩镜头");
#endif
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_ROLE_HP_STATE_RET___:		// 所有对象的状态包   
						
		{
			//*(WORD*)pszRecvBuf = 0;
			int nRet = (int)(*(WORD*)((DWORD)pszRecvBuf+0x10));
			if( 
// 				nRet == 0x1F ||
//  			nRet == 0x20 ||
				nRet == 0x2B 
//				nRet == 0x34 

// 				nRet == 0x38 || 
// 				nRet == 0x44 ||
// 				nRet == 0x48 || 
// 				nRet == 0x4C 

// 				nRet == 0x5c || 
// 				nRet == 0x56 ||
// 				nRet == 0x88 || 
// 				nRet == 0xBB || 
// 				nRet == 0xE6 || 
// 				nRet == 0x114 
				)
			{
				char* pszRecvBuffer[256] = {0};
				memcpy(pszRecvBuffer,(char*)((DWORD)pszRecvBuf+0x10+0x4),0x2a);
				//memset(pszRecvBuffer,0,0x2A);

				byte bRet = *(byte*)((DWORD)pszRecvBuf+0x3E);
				*(byte*)((DWORD)pszRecvBuf+0x3E) = 0;

				DWORD dwRet = *(DWORD*)((DWORD)pszRecvBuf+0x10+0x4);

				char szPrint[256] = {0};
				sprintf_s(szPrint,"[LOG] 收到 recv id = 0x%p len = 0x%p ret = 0x%p ret2 = 0x%p %s ret = %d",wMssageId,nLen,nRet,dwRet,pszRecvBuffer,bRet);
				OutputDebugStringA(szPrint);

				//*(WORD*)pszRecvBuf = 0;
			}
			else
			{
				char szPrint[256] = {0};
				sprintf_s(szPrint,"[LOG] 清空 recv id = 0x%p len = 0x%p ret = 0x%p",wMssageId,nLen,nRet);
				//OutputDebugStringA(szPrint);
				
			}
		
		}
		break;
	case 0x274:
	case 0x295:
	case 0x504:
	case 0x280:
	case 0x281:
	case 0x26D:
	case 0x264:
	case 0xD03:
	case 0x25F:
	case 0x2EA:
	case 0x2e9:
	case 0x258:
	case 0x290:
		{
			
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_PLAYER_LIST___:			// 角色链表返回值
		{
			return theMmoUnRecvPacket.MmoRecvPlayerList(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_DELETE_PLAYER_RET___:	// 删除角色返回报
		{
			return theMmoUnRecvPacket.MmoRecvDeletePlayer(pszRecvBuf);
		}
		break;
	case 0x2d3:
		{
			//OutputDebugStringA("[LOG] 攻击包0x2d3");
		}
		break;
	case 0x29d:	// 销毁对象
		{

		}
		break;
	//case 0x2A1:
	//case 0x258:
		{
			
// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[LOG] 清空 recv id = 0x%p len = 0x%p",wMssageId,nLen);
// 			OutputDebugStringA(szPrint);
// 
//  			*(WORD*)pszRecvBuf = 0;
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_MSG_MOSTER_SIWANG___:		// 死亡包
		{
			char szPrint[256] = {0};
		
			DWORD packet_id = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_MSG_MOSTER_SIWANG_PACKET_ID);

			sprintf_s(szPrint,"[LOG] 死亡对象id ：0x%p",	packet_id);
			OutputDebugStringA(szPrint);

			//*(DWORD*)((DWORD)pszRecvBuf+0x18) = 0x100014AC;
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_MSG_PLAYER_SIWANG___:
		{
			char szPrint[256] = {0};
			DWORD packet_id = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);
			sprintf_s(szPrint,"[LOG] 死亡玩家id ：0x%p",	packet_id);
			OutputDebugStringA(szPrint);
			//assert(false);
		}
		break;
	case MMOCLIENT_REVERSAL_ACCEPT_TASK_MSG_ID___:
		{
			return theMmoUnRecvPacket.MmoAcceptTask(pszRecvBuf);		// 接受任务返回值
		}
		break;
	case MMOCLIENT_REVERSAL_GIVE_UP_TASK_MSG_ID___:
		{
			return theMmoUnRecvPacket.MmoGiveUpTask(pszRecvBuf);		// 放弃任务返回值
		}
		break;
	case MMOCLIENT_REVERSAL_ACCEPT_MAIL_VALUE_RET_MSG_ID___:			// 接受邮件的返回值
		{
			return theMmoUnRecvPacket.MmoAcceptMailRetValue(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_LINE___:		// 选择角色后，线路的返回值
		{
			return theMmoUnRecvPacket.MmoRecvPlayerLine(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_CREATE_ROLE_RET___:	// 创建角色的返回值
		{
			return theMmoUnRecvPacket.MmoRecvCreateRole(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_IN_FB_RET___:		// 进入副本的返回值
		{
			return theMmoUnRecvPacket.MmoRecvDoFbRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MSG_FB_SERVER_FULL_RET___:	// 副本服务器爆满的返回值
		{
			return theMmoUnRecvPacket.MmoRecvFbServerFullRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_REQ_ITEM_BY_TYPE___:			// 查询拍卖物品返回值
		{
			return theMmoUnRecvPacket.MmoRecvReqItemBytype(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_REQ_ITEM_BY_TYPE_MY___:			// 刷新我的拍卖返回值
		{
			return theMmoUnRecvPacket.MmoRecvMyReqItemBytype(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_INVITE_TEAM_FAIL___:			// 邀请组队的返回值 ==》 组队失败：xx拒绝了你的组队邀请
		{
			return theMmoUnRecvPacket.MmoRecvInviteTeamFail(pszRecvBuf);
		}
	case MMOCLIENT_REVERSAL_PACKET_RECV_TEAM_INFO___:			// 在角色不是队长的情况下申请组队和创建队伍返回值  ==》 第一次获取队伍信息会有此返回包
		{
			return theMmoUnRecvPacket.MmoRecvTeamInfo(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_JOIN_TEAM_INFO___:			// 在角色为队长的情况下，有新加入的队员返回值
		{
			return theMmoUnRecvPacket.MmoRecvJoinTeamInfo(pszRecvBuf);
		}
		break;	
	case MMOCLIENT_REVERSAL_PACKET_RECV_LEAVE_TEAM___:		// 有队员离开队伍  
		{
			return theMmoUnRecvPacket.MmoRecvleaveTeam(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_BE_LEAVE_TEAM___:	// 有队员被踢出队伍
		{
			return theMmoUnRecvPacket.MmoRecvBeleaveTeam(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_DISS_TEAM___ :		// 队长退出队伍
		{
			return theMmoUnRecvPacket.MmoRecvDisTeam(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_TEAM_ERROR___:		// 组队的错误码
		{
			return theMmoUnRecvPacket.MmoRecvTeamRetError(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_TASK_FB_INFO___:	// 公会任务副本，千狩副本，喵喵副本的返回值
		{
			return theMmoUnRecvPacket.MmoRecvTaskFbInfo(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_AN_QUAN_BAO_HU_TIME___:			// 安全保护时间	
		{
			int nTime = *(int*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

			// 操作拍卖，买卖商店，等需要时间限制的，都会返回这个包
			char szPrint[256] = {0};
			sprintf_s(szPrint,"[LOG] recv 您现在处于安全保护时间内，请稍后尝试此操作。剩余%d秒",nTime);
			OutputDebugStringA(szPrint);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_REQ_BUY_ITEM_RET___:			// 购买拍卖物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvReqBuyItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_JI_YUN_FB_RET___:			// 激运
		{
			return theMmoUnRecvPacket.MmoRecvJiYunFbRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_OPEN_NPC_RET___:			// 打开npc返回值
		{
			return theMmoUnRecvPacket.MmoRecvOpenNpcRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_SHANG_JIA_ITEM_RET___:		// 上架拍卖物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvShangJiaAuctionItemRet(pszRecvBuf);
		}
		break;	
	case MMOCLIENT_REVERSAL_PACKET_RECV_SELL_ITEM_RET___:	// 出售物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvSellItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_COMPLETE_TASK_RET___:		// 提交任务成功的返回值
		{
			return theMmoUnRecvPacket.MmoRecvCompleteTaskRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_COMPLETE_TASK_FAIL_RET___:			// 提交任务失败的返回值
		{
			return theMmoUnRecvPacket.MmoRecvCompleteTaskFailRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_NPC_CHAT_RET___:		// 交谈任务npc返回值
		{
			return theMmoUnRecvPacket.MmoRecvTaskNpcChatRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_FRAM_SEED_RET___:		// 种植种子的返回值
		{
			return theMmoUnRecvPacket.MmoRecvFramSeedRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_FEEB_HP_RET___:			// 持续减血的返回包，用来判断是否中毒
		{
			return theMmoUnRecvPacket.MmoRecvFeebHpRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_SEND_MAIL_RET___:		// 发送邮件的返回值
		{
			return theMmoUnRecvPacket.MmoRecvSendMailRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_USE_ITEM_RET___:			// 使用物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvUseItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_USE_ITEM_RET2___:		// 使用物品后没有剩余物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvUseItemRet2(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_BUY_ITEM_RET___:		// 购买物品的返回值
		{
			return theMmoUnRecvPacket.MmoRecvBuyItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_GET_GFIT_BAG_RET___:	// 领取礼包
		{
			return theMmoUnRecvPacket.MmoRecvGetGfitBagRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_COLLECT_ITEM_BAG_FULL_RET___:	// 采集物品背包已满的返回值
		{
			return theMmoUnRecvPacket.MmoRecvCollectItemBagFullRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_TIDY_INVENTORY_ITEM_RET___:		// 整理背包或仓库的返回值
		{
			return theMmoUnRecvPacket.MmoRecvTidyInventoryItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_SEND_MAIL_RET2___:			// 发送邮件频繁的返回值
		{
			return theMmoUnRecvPacket.MmoRecvSendMailRet2(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_GUOTU_RET___:
		{
			return theMmoUnRecvPacket.MmoRecvSendGuoTuRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_REQUEST_DEAL_RET___:	// 请求交易返回值 成功
		{
			return theMmoUnRecvPacket.MmoRecvRequestDealRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_REQUEST_DEAL_FIAL_RET___:	// 请求交易返回值 拒绝
		{
			return theMmoUnRecvPacket.MmoRecvRequestDealFailRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_ACCEPT_DEAL_RET___:		// 接受到请求交易的返回值
		{
			return theMmoUnRecvPacket.MmoRecvAcceptDealRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_QUXIAO_DEAL_RET___:	// 取消交易返回值
		{
			return theMmoUnRecvPacket.MmoRecvQuXiaoDealRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_FAIL_RET1___:	// 双方都确认交易后的返回值 交易失败 物品超出携带上限 对方已经在交易中
		{
			return theMmoUnRecvPacket.MmoRecvDealFailRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MOVE_DEAL_ITEM_RET___:	// 移动物品到交易栏成功后的返回值
		{
			return theMmoUnRecvPacket.MmoRecvMoveDealItemRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_MOVE_DEAL_GOLD_RET___:	// 交易金币的返回值
		{
			return theMmoUnRecvPacket.MmoRecvMoveDealGoldRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_STATUS_RET___:		// 交易状态返回值 ==》 锁定交易栏的返回值 对方锁定交易栏的返回值 对方确认交易的返回值认交易的返回值
		{
			return theMmoUnRecvPacket.MmoRecvDealStatusRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_SUCESS_RET___:	// 双方都确认交易后的返回值 交易结束
		{
			return theMmoUnRecvPacket.MmoRecvDealSucessRet(pszRecvBuf);
		}
		break;
	case MMOCLIENT_REVERSAL_PACKET_RECV_EMIAL_INFO_RET___:
		{
			char szPrint[256] = {0};

			DWORD packet_id = *(DWORD*)((DWORD)pszRecvBuf+0x14);

			sprintf_s(szPrint,"[LOG] 返回邮件信息id ：0x%p",	packet_id);
			OutputDebugStringA(szPrint);
			
		}
		break;
	default:
		{
			char szPrint[256] = {0};
			sprintf_s(szPrint,"[LOG] recv id = 0x%p len = 0x%p",wMssageId,nLen);
			OutputDebugStringA(szPrint);
		}
	}
	return true;
}



_declspec(naked) void HookRecv()
{
	__asm
	{

		pushfd
		pushad

		push 0
		push edi
		call PrintRecv
		add esp, 8

		popad
		popfd

		pop edi
		pop esi
		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN
	}
}


void MMOUnRecvPacket::MmoHookRecv()
{
	Hook(MMOCLIENT_REVERSAL_RECV_HOOK+GetCryGameBaseAddr(),HookRecv);
}

void MMOUnRecvPacket::MmoUnHookRecv()
{
	byte bCode[] = {0x5F ,0x5E ,0x5B ,0x8B ,0xE5 };
	_WriteProcessMemory(MMOCLIENT_REVERSAL_RECV_HOOK+GetCryGameBaseAddr(),bCode,5);

}

bool MMOUnRecvPacket::MmoAcceptTask(void* pszRecvBuf)
{
	PMMO_ACCEPT_TASK ptheAcceptTask = (PMMO_ACCEPT_TASK)pszRecvBuf;
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] 接受任务: 0x%p",ptheAcceptTask->dwTaskId);
	OutputDebugStringA(szPrint);

	return true;
}

bool MMOUnRecvPacket::MmoGiveUpTask(void* pszRecvBuf)
{
	PMMO_GIVE_UP_TASK ptheGiveUpTask = (PMMO_GIVE_UP_TASK)pszRecvBuf;
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] 放弃任务: 0x%p",ptheGiveUpTask->dwTaskId);
	OutputDebugStringA(szPrint);

	return true;
}

bool MMOUnRecvPacket::MmoAcceptMailRetValue(void* pszRecvBuf)
{
	PMM0_ACCEPT_MAIL_RET_VALUE ptheMailRet = (PMM0_ACCEPT_MAIL_RET_VALUE)pszRecvBuf;

	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] 接受邮件返回值: id = 0x%p 0x%p ret = %d %d",ptheMailRet->dwMailId1,ptheMailRet->dwMailId2,ptheMailRet->nRetValue,ptheMailRet->nRetError);
	OutputDebugStringA(szPrint);
	return true;
}



bool MMOUnRecvPacket::MmoRecvPlayerList(void* pszRecvBuf)
{
	try
	{
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 人物收包地址=%p 结构大小=%p",(DWORD)pszRecvBuf,sizeof(MM0_PLAYER_LIST));
		OutputDebugStringA(szPrint);

		int nPlayerCount = (int)(*(WORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PLAYER_LIST_COUNT___));

		PMM0_PLAYER_LIST pthePlayerList = PMM0_PLAYER_LIST((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PLAYER_INFO_OFFSET___);
		for(int i=0; i<nPlayerCount; i++)
		{
			sprintf_s(szPrint,"[LOG] player id = 0x%p level = %d name = %s\r\n",pthePlayerList->dwPlayerId,pthePlayerList->nPlayerLevel,pthePlayerList->szPlayerName);
			OutputDebugStringA(szPrint);
			pthePlayerList++;
		}
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}


bool MMOUnRecvPacket::MmoRecvDeletePlayer(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_DELETE_PLAYER ptheDeletePlayer = (PMMO_RECV_DELETE_PLAYER)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);

		/*++
			当 ptheDeletePlayer->nRet = 0 时，则表示删除成功
			当 ptheDeletePlayer->nRet = 2 时，则表示删除失败，ptheDeletePlayer->nRetParamer = n，此返回包表示当前处于登陆保护状态，此n为剩余秒

		--*/
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] delete ret = 0x%p paramer = 0x%p\r\n",ptheDeletePlayer->nRet,ptheDeletePlayer->nRetParamer);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}


bool MMOUnRecvPacket::MmoRecvPlayerLine(void* pszRecvBuf)
{
	try
	{
		int nLineCount = (int)*(WORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);

		PMMO_GAME_LINE ptheGameLine = (PMMO_GAME_LINE)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___+sizeof(WORD));

		char szPrint[256];
		for (int i=0; i<nLineCount; i++)
		{
			// 流畅 = 1， 良好 = 2，黄色繁忙 = 3，红色繁忙 = 4
			
			char szLineLevelStr[256] = {0};

			switch (ptheGameLine->bLineLevel)
			{
			case 1:
				{
					memcpy_s(szLineLevelStr,sizeof(szLineLevelStr),"流畅",strlen("流畅")+1);
				}
				break;
			case 2:
				{
					memcpy_s(szLineLevelStr,sizeof(szLineLevelStr),"良好",strlen("良好")+1);
				}
				break;
			case 3:
				{
					memcpy_s(szLineLevelStr,sizeof(szLineLevelStr),"黄色繁忙",strlen("黄色繁忙")+1);
				}
				break;
			case 4:
				{
					memcpy_s(szLineLevelStr,sizeof(szLineLevelStr),"红色繁忙",strlen("红色繁忙")+1);
				}
				break;
			default:
				{
					memcpy_s(szLineLevelStr,sizeof(szLineLevelStr),"未知",strlen("未知")+1);
				}
			}
			sprintf_s(szPrint,"[LOG] game line Index = %d level = %d %s\r\n",ptheGameLine->wLineIndex,ptheGameLine->bLineLevel,szLineLevelStr);
			OutputDebugStringA(szPrint);

			ptheGameLine++;
		}
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvCreateRole(void* pszRecvBuf)
{
	try
	{
		DWORD dwRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 创建角色返回值: 0x%p",dwRet);   // 成功 = 0， 已经被注册 = 4
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}



// 解析拍卖物品封包
bool MMOUnRecvPacket::MmoRecvReqItemBytype(void* pszRecvBuf)
{
	try
	{
		char szPrint[256];
		PMMO_RECV_REQ_ITEM_BY_HEADER ptheRecvReqItemByHeader = (PMMO_RECV_REQ_ITEM_BY_HEADER)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		sprintf_s(szPrint,"[LOG] 拍卖物品 ret = 0x%p 物品总数量 = %d 当前页物品数量 = %d",ptheRecvReqItemByHeader->dwRet,ptheRecvReqItemByHeader->nReqItemCount,ptheRecvReqItemByHeader->wCurPageCount);
		OutputDebugStringA(szPrint);
		if( ptheRecvReqItemByHeader->wCurPageCount != 0 )
		{
			PMM0_RECV_REQ_ITEM_BY_TYPE ptheReqItenByType = (PMM0_RECV_REQ_ITEM_BY_TYPE)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+sizeof(MMO_RECV_REQ_ITEM_BY_HEADER));

			
			for (int i=0; i<ptheRecvReqItemByHeader->wCurPageCount; i++)
			{
				/*++
					游戏处理解密逻辑

					建立一张游戏的表，key值 = dwReqItemById 和 dwReqItemByKey
					recv获取对拍卖物品信息时，首先查表，如果没有，则调用解密函数 GetReqItemAddr 随后加入到表中，游戏针对每一个物品都进行一次解密，之后再次访问到此对象时，则不在解密
				--*/
				DWORD dwItemAddr = GetReqItemAddr((DWORD)ptheReqItenByType);
				assert(dwItemAddr);
				DWORD dwItemId = 0;
				int nItemCount = 0;

				if( dwItemAddr != 0 )
				{
					nItemCount = *(int*)(dwItemAddr+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_COUNT_OFFSET___);

					DWORD dwItemAddr2 = *(DWORD*)(dwItemAddr+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET3___);
					if( dwItemAddr2 != 0 )
					{
						dwItemId = *(DWORD*)(dwItemAddr2+MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___);
					}
					
				}

				sprintf_s(szPrint,"[LOG] addr = 0x%p req id = 0x%p key = 0x%p Item Id = 0x%p count = %d by name = %s 起拍价: %d 一口价: %d 竞拍价: %d",
							dwItemAddr,
							ptheReqItenByType->dwReqItemById,
							ptheReqItenByType->dwReqItemByKey,
							dwItemId,
							nItemCount,		// 如果此物品不存在叠加数量，则遍历出来的是0，但是此物品是存在的，仅存在一个
							ptheReqItenByType->szReqItemByPlayerName,
							ptheReqItenByType->nReqItemByPrice1,
							ptheReqItenByType->nReqItemByPrice2,
							ptheReqItenByType->nReqItemByPrice3);
				OutputDebugStringA(szPrint);

				ptheReqItenByType++;
			}
		}
		else
		{
			OutputDebugStringA("[LOG] 不存在拍卖物品!");
		}
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}



// 解析我的拍卖物品封包
bool MMOUnRecvPacket::MmoRecvMyReqItemBytype(void* pszRecvBuf)
{
	try
	{
		int nCount = (int)*(WORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+MMOCLIENT_REVERSAL_PACKET_RECV_MSG_MY_REQ_COUNT_OFFSET___);
		if( nCount != 0 )
		{
			PMM0_RECV_REQ_ITEM_BY_TYPE ptheReqItenByType = (PMM0_RECV_REQ_ITEM_BY_TYPE)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+MMOCLIENT_REVERSAL_PACKET_RECV_MSG_MY_REQ_DATA_OFFSET___);

			char szPrint[256];
			for (int i=0; i<nCount; i++)
			{
				DWORD dwItemAddr = GetReqItemAddr((DWORD)ptheReqItenByType);
				DWORD dwItemId = 0;
				int nItemCount = 0;

				if( dwItemAddr != 0 )
				{
					nItemCount = *(int*)(dwItemAddr+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_COUNT_OFFSET___);

					DWORD dwItemAddr2 = *(DWORD*)(dwItemAddr+MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET3___);
					if( dwItemAddr2 != 0 )
					{
						dwItemId = *(DWORD*)(dwItemAddr2+MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___);
					}

				}

				sprintf_s(szPrint,"[LOG] req id = 0x%p key = 0x%p Item Id = 0x%p count = %d by name = %s 起拍价: %d 一口价: %d 竞拍价: %d",
					ptheReqItenByType->dwReqItemById,
					ptheReqItenByType->dwReqItemByKey,
					dwItemId,
					nItemCount,
					ptheReqItenByType->szReqItemByPlayerName,
					ptheReqItenByType->nReqItemByPrice1,
					ptheReqItenByType->nReqItemByPrice2,
					ptheReqItenByType->nReqItemByPrice3);
				OutputDebugStringA(szPrint);

				ptheReqItenByType++;
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

// 邀请组队的返回值 ==》 组队失败：xx拒绝了你的组队邀请
bool MMOUnRecvPacket::MmoRecvInviteTeamFail(void* pszRecvBuf)
{
	try
	{
		PMMO_INVITE_TEAM_FAIL ptheInviteTeamFail = (PMMO_INVITE_TEAM_FAIL)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 组队失败：%s 拒绝了你的组队邀请!",ptheInviteTeamFail->szActorName);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		
	}
	return true;
}


// 在角色不是队长的情况下申请组队和创建队伍返回值  ==》 第一次获取队伍信息会有此返回包
bool MMOUnRecvPacket::MmoRecvTeamInfo(void* pszRecvBuf)
{
	try
	{
		PMMO_TEAM_INFO ptheTeamInfo = (PMMO_TEAM_INFO)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 创建队伍成功 ==> team title = %s pwd = %s member count = %d",ptheTeamInfo->szTeamTitle,ptheTeamInfo->szTeamPwd,ptheTeamInfo->nMemberCount);
		OutputDebugStringA(szPrint);
		for (int i=0; i<ptheTeamInfo->nMemberCount; i++ )
		{
			sprintf_s(szPrint,"[LOG] team member name = %s id = 0x%p level = %d",ptheTeamInfo->theTeamMemberInfo[i].szTeamMemberName,ptheTeamInfo->theTeamMemberInfo[i].dwActorPacketId,ptheTeamInfo->theTeamMemberInfo[i].nMemberLevel);
			OutputDebugStringA(szPrint);
		}
	}
	catch (...)
	{

	}
	return true;
}

// 在角色为队长的情况下，有新加入的队员返回值
bool MMOUnRecvPacket::MmoRecvJoinTeamInfo(void* pszRecvBuf)
{
	try
	{
		PMMO_TEAM_MEMBER_INFO ptheTeamInfo = (PMMO_TEAM_MEMBER_INFO)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 有新成员加入 ==> team member name = %s id = 0x%p level = %d",ptheTeamInfo->szTeamMemberName,ptheTeamInfo->dwActorPacketId,ptheTeamInfo->nMemberLevel);
		OutputDebugStringA(szPrint);

	}
	catch (...)
	{

	}
	return true;
}

// 有队员离开队伍
bool MMOUnRecvPacket::MmoRecvleaveTeam(void* pszRecvBuf)
{
	try
	{
		DWORD dwLeaveActorPacketId = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 有队员离开队伍 ==> id = 0x%p",dwLeaveActorPacketId);
		OutputDebugStringA(szPrint);

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 有队员被队长踢出队伍
bool MMOUnRecvPacket::MmoRecvBeleaveTeam(void* pszRecvBuf)
{
	try
	{
		DWORD dwLeaveActorPacketId = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 有队员被队长踢出队伍 ==> id = 0x%p",dwLeaveActorPacketId);
		OutputDebugStringA(szPrint);

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 队长退出队伍
bool MMOUnRecvPacket::MmoRecvDisTeam(void* pszRecvBuf)
{
	try
	{
		OutputDebugStringA("[LOG] 队长退出队伍!!!");
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 组队的错误返回值
bool MMOUnRecvPacket::MmoRecvTeamRetError(void* pszRecvBuf)
{
	try
	{
		DWORD dwTeamError1 = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		DWORD dwTeamError2 = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+4);
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 队伍操作错误码 ==> 0x%p 0x%p",dwTeamError1,dwTeamError2);
		OutputDebugStringA(szPrint);

		/*++
			你没有队伍，不能退出队伍	返回值： 6 和 0x0C
			创建队伍失败，你已经存在队伍	返回值： 5 和 5
			你已经存在队伍，不可加入别人的队伍	返回值： 3 和 5
			组队失败，该队伍不存在	返回值： 3 和 0x0C
			组队失败，密码验证错误	返回值： 3 和 0x10
			你不是队长，或者没有队伍，无法踢出队员	返回值：7 和 0x0C
			邀请组队失败，对方已经有队伍	返回值：1 和 9
		--*/
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 公会任务副本，千狩副本，喵喵副本
bool MMOUnRecvPacket::MmoRecvTaskFbInfo(void* pszRecvBuf)
{
	try
	{
		PMMO_TASK_FB_INFO ptheTaskFbInfo = (PMMO_TASK_FB_INFO)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256];

// 		switch(ptheTaskFbInfo->dwTaskFbType)
// 		{
// 		case MMOCLIENT_REVERSAL_PACKET_MSG_ID_GONG_HUI_INDEX___:
// 			{
// 				sprintf_s(szPrint,"[LOG] 公会副本 ==> task fb type = 0x%p count = %d ",ptheTaskFbInfo->dwTaskFbType,ptheTaskFbInfo->wCount);
// 			}
// 			break;
// 		case MMOCLIENT_REVERSAL_PACKET_MSG_ID_QIAN_SHOU_INDEX___:
// 			{
// 				sprintf_s(szPrint,"[LOG] 千狩副本 ==> task fb type = 0x%p count = %d ",ptheTaskFbInfo->dwTaskFbType,ptheTaskFbInfo->wCount);
// 			}
// 			break;
// 		case MMOCLIENT_REVERSAL_PACKET_MSG_ID_MIAO_MIAO_INDEX___:
// 			{
// 				sprintf_s(szPrint,"[LOG] 喵喵副本 ==> task fb type = 0x%p count = %d ",ptheTaskFbInfo->dwTaskFbType,ptheTaskFbInfo->wCount);
// 			}
// 			break;
// 		default:
// 			{
// 				sprintf_s(szPrint,"[LOG] 未知副本 ==> task fb type = 0x%p count = %d ",ptheTaskFbInfo->dwTaskFbType,ptheTaskFbInfo->wCount);
// 			}
// 		}

		sprintf_s(szPrint,"[LOG] 查询副本返回值 ==> fb ret = 0x%p count = %d ",ptheTaskFbInfo->dwRetValue,ptheTaskFbInfo->wCount);
		
		OutputDebugStringA(szPrint);

		PMMO_TASK_FB_INFO_	ptheTaskFbInfo_ = (PMMO_TASK_FB_INFO_)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___+sizeof(MMO_TASK_FB_INFO));

		for (int i=0; i<ptheTaskFbInfo->wCount; i++ )
		{
			for (int j=0; j<ptheTaskFbInfo_->wCount; j++)
			{
				sprintf_s(szPrint,"[LOG] task fb id = 0x%p fb type = 0x%p",ptheTaskFbInfo_->dwTaskFbId[j],ptheTaskFbInfo_->dwFbType);	
				OutputDebugStringA(szPrint);
			}
			ptheTaskFbInfo_++;
		}
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvReqBuyItemRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_REQ_BUY_ITEM_RET ptheRecvReqItemRet = (PMMO_RECV_REQ_BUY_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);
		
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 拍卖购买物品返回值 ==> ret = 0x%p id = 0x%p",ptheRecvReqItemRet->dwBuyItemRet,ptheRecvReqItemRet->dwBuyReqItemId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


bool MMOUnRecvPacket::MmoRecvJiYunFbRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_JI_YUN_FB ptheRecvJiYunFb = (PMMO_RECV_JI_YUN_FB)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___);

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 激运副本返回值 ==> ret = 0x%p id = 0x%p count = %d time = %d",
						ptheRecvJiYunFb->nRet,
						ptheRecvJiYunFb->dwFbId,
						ptheRecvJiYunFb->nFbCount,
						ptheRecvJiYunFb->nShuaXinTime);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvDoFbRet(void* pszRecvBuf)
{
	try
	{
		PMMO_GAME_IN_FB_RET_PACKET ptheGameInFbRetPacket = (PMMO_GAME_IN_FB_RET_PACKET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 副本操作返回值 ==> role packet id = 0x%p error = 0x%p",
					ptheGameInFbRetPacket->dwRolePacketId,ptheGameInFbRetPacket->nError);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



bool MMOUnRecvPacket::MmoRecvFbServerFullRet(void* pszRecvBuf)
{
	try
	{
		// 只要此包出现，必然失败
		PMMO_FB_SERVER_FULL_RET ptheFbServerFullRet = (PMMO_FB_SERVER_FULL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 进入副本失败的返回值 ==> role packet id = 0x%p error = 0x%p name = %s",
			ptheFbServerFullRet->dwRolePacketId,ptheFbServerFullRet->dwRetValue,ptheFbServerFullRet->szRoleName);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvOpenNpcRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_OPEN_NPC_RET ptheOpenNpcRet = (PMMO_RECV_OPEN_NPC_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 打开NPC返回值 ==> ret = 0x%p npc id = 0x%p player id = 0x%p",
							ptheOpenNpcRet->wRet,ptheOpenNpcRet->theOpenNpcRet[0].dwObjPacketId,ptheOpenNpcRet->theOpenNpcRet[1].dwObjPacketId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 上架拍卖物品返回值
bool MMOUnRecvPacket::MmoRecvShangJiaAuctionItemRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_SHANG_JIA_ITEM_RET ptheShangJiaItemRet = (PMMO_RECV_SHANG_JIA_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 上架物品返回值 ==> ret1 = 0x%p ret2 = 0x%p ret3 = 0x%p",
			ptheShangJiaItemRet->dwRetValue,ptheShangJiaItemRet->dwUnKnow1,ptheShangJiaItemRet->dwUnKnow2);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvSellItemRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_SELL_ITEM_RET ptheSellItemRet = (PMMO_RECV_SELL_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 出售物品的返回值 ==> ret = 0x%p Sell Item Id = 0x%p",
			ptheSellItemRet->nRet,ptheSellItemRet->dwSellItemId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvCompleteTaskRet(void* pszRecvBuf)
{
	try
	{
		// 提交任务成功的返回值为提交任务的id
		DWORD dwCompleteTaskRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 提交任务成功的返回值 ==> ret = 0x%p",dwCompleteTaskRet);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvCompleteTaskFailRet(void* pszRecvBuf)
{
	try
	{
		// 提交任务失败的返回值 ， 接赏金任务已达上限的返回值
		PMMO_RECV_COMPLETE_TASK_GAIL_RET ptheCompleteTaskFailRet = (PMMO_RECV_COMPLETE_TASK_GAIL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 提交任务失败的返回值 ==> ret = 0x%p task id = 0x%p",ptheCompleteTaskFailRet->nRet,ptheCompleteTaskFailRet->dwTaskId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvTaskNpcChatRet(void* pszRecvBuf)
{
	try
	{
		// 交谈完成任务返回值 : 当前任务的id
		DWORD dwChatNpcRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 交谈完成任务的返回值 ==> task id = 0x%p",dwChatNpcRet);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvFramSeedRet(void* pszRecvBuf)
{
	try
	{
		// 种植种子的返回值  成功=0 已经种子=0x14
		DWORD dwFramSeedRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 种植种子的返回值 ==> ret = 0x%p",dwFramSeedRet);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvFeebHpRet(void* pszRecvBuf)
{
	try
	{
		// 中毒，寒冷，炎热
		PMMO_RECV_FEEB_HP_RET ptheFeebHpRet = (PMMO_RECV_FEEB_HP_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 角色所在恶劣环境的返回值 ==> id = 0x%p ret = 0x%p 0x%p 0x%p 0x%p 0x%p",
			ptheFeebHpRet->dwActorPacketId,
			ptheFeebHpRet->dwUnKnow1,
			ptheFeebHpRet->dwUnKnow2,
			ptheFeebHpRet->dwUnKnow3,
			ptheFeebHpRet->dwUnKnow4,
			ptheFeebHpRet->dwPoisoningFeebHpFlag
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvSendMailRet(void* pszRecvBuf)
{
	try
	{
		// 发送成功返回值=0
		DWORD dwSendMailRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 发送邮件的返回值 ==> ret = 0x%p",dwSendMailRet);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvUseItemRet(void* pszRecvBuf)
{
	try
	{
		// 使用物品返回值
		PMMO_RECV_USE_ITEM_RET ptheUseItemRet = (PMMO_RECV_USE_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 使用物品后剩余返回值 ==> ret = %d Item id = 0x%p slot = %d",ptheUseItemRet->bRet,ptheUseItemRet->dwItemId,ptheUseItemRet->wSlotIndex);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvUseItemRet2(void* pszRecvBuf)
{
	try
	{
		// 使用物品返回值
		PMMO_RECV_USE_ITEM_RET2 ptheUseItemRet = (PMMO_RECV_USE_ITEM_RET2)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 使用物品后没有物品的返回值 ==> key1 = 0x%p key2 = 0x%p Slot = %d ",(DWORD)ptheUseItemRet->llItemKey,(DWORD)(ptheUseItemRet->llItemKey>>32),ptheUseItemRet->wItemSlotIndex);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvBuyItemRet(void* pszRecvBuf)
{
	try
	{
		// 使用物品返回值
		PMMO_RECV_BUY_ITEM_RET ptheBuyItemRet = (PMMO_RECV_BUY_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 购买物品的返回值 ==> ret = 0x%p shop id = 0x%p count = %d",ptheBuyItemRet->nRet,ptheBuyItemRet->dwShopItemId,ptheBuyItemRet->nCount);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvGetGfitBagRet(void* pszRecvBuf)
{
	try
	{
		// 领取礼包的返回值
		DWORD dwGfitBagId = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 领取礼包的返回值 ==> id = 0x%p",dwGfitBagId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvCollectItemBagFullRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_COLLECT_ITEM_BAG_FULL_RET ptheBagFullRet =(PMMO_RECV_COLLECT_ITEM_BAG_FULL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 采集物品背包已满的返回值 ==> ret = 0x%p 采集的对象id = 0x%p 采集得到的物品id = 0x%p",ptheBagFullRet->nRet,ptheBagFullRet->dwCollectObjPacketId,ptheBagFullRet->dwCollectItemId);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool MMOUnRecvPacket::MmoRecvTidyInventoryItemRet(void* pszRecvBuf)
{
	try
	{
		// 返回的封包和发送的结构是一样的，更新最新的物品位置
		WORD wTabSlotIndex = *(WORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 整理仓库或背包的返回值 ==> ret = 0x%p",wTabSlotIndex);		// 这个值如果为2，则说明整理仓库成功，如果为0则说整理背包成功
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


bool MMOUnRecvPacket::MmoRecvSendMailRet2(void* pszRecvBuf)
{
	try
	{
		// 返回的封包和发送的结构是一样的，更新最新的物品位置
		PMMO_RECV_SEND_MAIL_RET2 ptheSendMailRet2 = (PMMO_RECV_SEND_MAIL_RET2)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
		char szPrint[256] = {0};

		switch (ptheSendMailRet2->dwErrorValue)
		{
		case 0x00046CE4:
			{
				sprintf_s(szPrint,"[LOG] 发送邮件失败返回值 ==> ret = 0x%p 安全时间 error = 0x%p",ptheSendMailRet2->nRet,ptheSendMailRet2->dwErrorValue);		// 发送邮件频繁返回值 = 1	
			}
			break;
		case 0x00046CD4:
			{
				sprintf_s(szPrint,"[LOG] 发送邮件失败返回值 ==> ret = 0x%p 不存在的角色名 error = 0x%p",ptheSendMailRet2->nRet,ptheSendMailRet2->dwErrorValue);		// 发送邮件频繁返回值 = 1	
			}
			break;
		case 0x00046CE2:
			{
				sprintf_s(szPrint,"[LOG] 发送邮件失败返回值 ==> ret = 0x%p 发送邮件频繁 error = 0x%p",ptheSendMailRet2->nRet,ptheSendMailRet2->dwErrorValue);		// 发送邮件频繁返回值 = 1	
			}
			break;
		default:
			{
				sprintf_s(szPrint,"[LOG] 发送邮件失败返回值 ==> ret = 0x%p 未知错误码 error = 0x%p",ptheSendMailRet2->nRet,ptheSendMailRet2->dwErrorValue);		// 发送邮件频繁返回值 = 1	
			}
		}

		
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


bool MMOUnRecvPacket::MmoRecvSendGuoTuRet(void* pszRecvBuf)
{
	try
	{
		PGUO_TU_RET ptheGuoTuRet = (PGUO_TU_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 过图返回包 ==> packet id = 0x%p dir x = %f y = %f vector x = %f y = %f z = %f",	
																								ptheGuoTuRet->dwRolePacketId,
																								ptheGuoTuRet->theRoleDirection.dir_x,
																								ptheGuoTuRet->theRoleDirection.dir_y,
																								ptheGuoTuRet->theRoleVector.fx,
																								ptheGuoTuRet->theRoleVector.fy,
																								ptheGuoTuRet->theRoleVector.fx
																								);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



// 请求交易返回值 接受
// 同意接受对方请求交易的返回值
bool MMOUnRecvPacket::MmoRecvRequestDealRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_REQUEST_DEAL_RET ptheRequestDealRet = (PMMO_RECV_REQUEST_DEAL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 请求交易返回值 与 同意接受对方请求交易的返回值 成功 ==> request id = 0x%p by request id = 0x%p",	
			ptheRequestDealRet->dwRequestDealRoleId,
			ptheRequestDealRet->dwByRequestDealRoleId
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


// 请求交易返回值 拒绝
bool MMOUnRecvPacket::MmoRecvRequestDealFailRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_REQUEST_DEAL_FIAL_RET ptheRequestDealFailRet = (PMMO_RECV_REQUEST_DEAL_FIAL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 请求交易返回值 拒绝 ==> ret = 0x%p role name = %s",	
			ptheRequestDealFailRet->dwRequestDealFailValue,
			ptheRequestDealFailRet->szRoleName
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


// 接受到请求交易的返回值
bool MMOUnRecvPacket::MmoRecvAcceptDealRet(void* pszRecvBuf)
{
	try
	{
		DWORD dwAcceptDealValue = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 接受到请求交易的返回值 ret = 0x%p",dwAcceptDealValue);
		OutputDebugStringA(szPrint);

		MMO_Accept_Deal(dwAcceptDealValue);	// 接受请求交易
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 取消交易返回值
bool MMOUnRecvPacket::MmoRecvQuXiaoDealRet(void* pszRecvBuf)
{
	try
	{
		DWORD dwQuXiaoDealRoleId = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 取消交易返回值 ret = 0x%p",dwQuXiaoDealRoleId);	// 谁取消，就是谁的id，如果为0，则表示交易失败
		OutputDebugStringA(szPrint);

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 移动物品到交易栏成功后的返回值
bool MMOUnRecvPacket::MmoRecvMoveDealItemRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_DEAL_MOVE_ITEM_RET ptheDealMoveItemRet = (PMMO_RECV_DEAL_MOVE_ITEM_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 移动物品到交易栏成功后的返回值 ==> scr tab = 0x%d slot = %d Item count = %d Deal slot = %d",	
				ptheDealMoveItemRet->wScrItemTabIndex,
				ptheDealMoveItemRet->wScrItemSlotIndex,
				ptheDealMoveItemRet->wMoveItemCount,
				ptheDealMoveItemRet->wDealSlotaIndex
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


// 交易金币的返回值
bool MMOUnRecvPacket::MmoRecvMoveDealGoldRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_MOVE_DEAL_GOLD_RET ptheMoveDealGoldRet = (PMMO_RECV_MOVE_DEAL_GOLD_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 交易金币的返回值 ==> Role id = 0x%p Gold = %d",	
			ptheMoveDealGoldRet->dwMoveDealGoldRoleId,
			ptheMoveDealGoldRet->nMoveDealGoldValue
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


// 交易状态返回值 ==》 锁定交易栏的返回值 对方锁定交易栏的返回值 对方确认交易的返回值 确认交易的返回值
bool MMOUnRecvPacket::MmoRecvDealStatusRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_DEAL_STATUS_RET ptheDealStatusRet = (PMMO_RECV_DEAL_STATUS_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 交易状态返回值 ==》 锁定交易栏的返回值 对方锁定交易栏的返回值 对方确认交易的返回值 确认交易的返回值 ==> do deal role id = 0x%p, deal index = %d",	
					ptheDealStatusRet->dwDoDealRoleId,
					ptheDealStatusRet->bDoDealIndex
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}




// 双方都确认交易后的返回值 交易失败 物品超出携带上限
bool MMOUnRecvPacket::MmoRecvDealFailRet(void* pszRecvBuf)
{
	try
	{
		PMMO_RECV_DEAL_FAIL_RET ptheDealFailRet = (PMMO_RECV_DEAL_FAIL_RET)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 双方都确认交易后的返回值 交易失败 物品超出携带上限 ==> ret = 0x%p Deal Role id = 0x%p",	
					ptheDealFailRet->dwDealFailRet,
					ptheDealFailRet->dwDealRoleId
			);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 双方都确认交易后的返回值 交易结束
bool MMOUnRecvPacket::MmoRecvDealSucessRet(void* pszRecvBuf)
{
	try
	{
		DWORD dwDealSucessRet = *(DWORD*)((DWORD)pszRecvBuf+MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] 双方都确认交易后的返回值 交易结束 ret = 0x%p",dwDealSucessRet);
		OutputDebugStringA(szPrint);

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}