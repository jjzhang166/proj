#include "StdAfx.h"
#include "UnPacket.h"
#include "jfzrbase.h"
#include "jfzrpacketstruct.h"
#include <sstream>

UnPacket	g_theUnPacket;

void UnPacketFun(LPVOID lpPraram, int nlen)
{
	g_theUnPacket.UnPacketFun(lpPraram);
}

_declspec(naked) void HookUnPacketAddr()
{
	__asm
	{
		pushad
		pushfd
	}
	
	__asm
	{
		push ebx
		push dword ptr [ebp-0x3c]
		lea eax, UnPacketFun
		call eax
		add esp, 8
	}

	__asm
	{
		popfd
		popad

		MOV EAX,DWORD PTR SS:[EBP-0x38]
		SUB EAX,DWORD PTR SS:[EBP-0x3C]

		mov eax, JFZR_REVERSAL_RECV_UN_PACKET_HOOK
		add eax, 6
		jmp eax
	}
}

UnPacket::UnPacket(void) : PacketString((const BYTE *)"")
{
}

UnPacket::UnPacket(const BYTE* lpPacket) : PacketString(lpPacket)
{
	
}

UnPacket::~UnPacket(void)
{
}

bool UnPacket::HookUnPacket(void)
{
	Hook(JFZR_REVERSAL_RECV_UN_PACKET_HOOK,HookUnPacketAddr);
	return true;
}

bool UnPacket::UnHookUnPacket(void)
{
	unsigned char szCode[] = {JFZR_REVERSAL_RECV_UN_PACKET_CODE};
	_WriteProcessMemory(JFZR_REVERSAL_RECV_UN_PACKET_HOOK,szCode,6);
	return false;
}

char szSendRecvData[0x40960] = {0};
char szRecvPrint[0x41960] = {0};
bool UnPacket::UnPacketFun(LPVOID lpPraram)
{
	BYTE* pszRecvPacket = (BYTE*)lpPraram;

	DWORD dwlen = *(WORD*)pszRecvPacket;

	WORD wMethodType = *(WORD*)(pszRecvPacket+2);

	if( wMethodType != 0x55E0 )
	{
		return true;
	}

	if( dwlen > 0x40960 )
	{
		wsprintfA(szRecvPrint,"[JFZR] recv 接收到的封包太大:%d",dwlen);
		OutputDebugStringA(szRecvPrint);
		assert(false);
		return false;
	}
	
	WORD wUrlSendType = *(WORD*)(pszRecvPacket+4);

	switch( wUrlSendType )
	{ 
	case 0x0000706C:	//0x7069:		// "Apple::Protocol::FromServer::SessionSystem::AllowSwitchChannel"     "94d50b73864f5682d3e091ef5d884559"
		{
			return UnPacket_AllowSwitchChannel(pszRecvPacket);
		}
		break;
	case 0x00007E0B: //0x8900:		// "Apple::Protocol::FromServer::SessionSystem::ChannelList"		"2bdfa59b5e93cdb9d4a1313aebd0ae25"
		{
			return UnPacket_ChannelList(pszRecvPacket);
		}
		break;
	case 0x00003B88:	//0xAC70:		// "PartialListPlayersStaticInfo"		"82de8827ee0d06331d1b1d583fb0d2ba"
		{
			return UnPacket_PartialListPlayersStaticInfo(pszRecvPacket);
		}
		break;
	case 0x0000004F:	//0x00005AF0:	查询拍卖物品
		{
			return UnPacket_RequestProductList(pszRecvPacket);		// "Apple::Protocol::FromServer::MarketSystem::RequestProductList"   "90220ab243e8bade2776266fea9260db"
		}
		break;
	case 0x0000930F://0x000042E3:
		{
			return UnPacket_RequestMyProductList(pszRecvPacket);	// "Apple::Protocol::FromServer::MarketSystem::RequestMyProductList"	"31bc866842a5439e308a170bfa24473b"
		}
		break;
	case 0x000095D3://0x00001163:
		{
			return UnPacket_MailList(pszRecvPacket);		// "Apple::Protocol::FromServer::MailSystem::MailList"		"3d894648a534f8205210c4aaad92974e"
		}
		break;
	case 0x000004CE://0x00008239:
		{
			return UnPacket_CreatePlayer(pszRecvPacket);		// "Apple::Protocol::FromServer::PlayerManagementSystem::CreatePlayer"		"b3283ba862b85d061ba4f8cd99ed17a5"
		}
		break;
	case 0x0000DC0F:		// 0x0000858F:
		{
			return UnPacket_EndPartialListPlayers(pszRecvPacket);		// "EndPartialListPlayers"     "53b7ee7068b502dd9a867a7e963062a9"
		}
		break;
	case 0x0000BFD3:			// "Apple::Protocol::FromServer::Message"		"ba9fd69709fdc831983da9deeb49c647"
		{
			return UnPacket_Message(pszRecvPacket);				// 消息值
		}
		break;
	case 0x00002887:			// "Apple::Protocol::FromServer::MarketSystem::RegisterProduct"			"3d7f0e591dd14f39d41ce8e3e57e9b28"
		{
			return UnPacket_RegisterProduct(pszRecvPacket);		// 挂物品到拍卖收到的确认信息
		}
		break;
	case 0x00005380:			// "Apple::Protocol::FromServer::MarketSystem::BuyProduct"		"79deee93b8fa134771559f02d165f17f"
		{
			return UnPacket_BuyProduct(pszRecvPacket);		// 购买拍卖物品的返回确认包
		}
		break;

	case 0x0000E8AD:			// "Apple::Protocol::FromServer::MarketSystem::UnregisterProduct"		"eff027c40b2e8ebba2a10782bb63dc75"
		{
			return UnPacket_UnregisterProduct(pszRecvPacket);		//	 取消我挂在拍卖的物品收到的确认包
		}
		break;
	case 0x0000050D:		// "FromServer::StageSystem::StartStageFailed"	 "d313982cc304fdb314419d2c0cae263a"
		{
			return UnPacket_StartStageFailed(pszRecvPacket);
		}
		break;
	case 0x0000CB66:		// Apple::Protocol::FromServer::MailSystem::MailContents    2197cd5a4edff09f1559c717d18ee605
		{
			return UnPacket_MailContents(pszRecvPacket);		// 接受邮件物品后，更新此邮件内容
		}
		break;
	case 0x0000074C:		// Apple::Protocol::FromServer::MailSystem::GetAttachments	329656d9f5a5817482ee735c01652a7e
		{
			return UnPacket_GetAttachments(pszRecvPacket);		// 进入游戏后，在一定时间无法移动邮件物品
		}
		break;
	case 0x00006D87:		// Apple::Protocol::FromServer::MarketSystem::RequestProductInfo	27a58432f813df2395f896dc77bf6d59
		{
			return UnPacket_RequestProductInfo(pszRecvPacket);
		}
		break;
	case 0x0000DE1C:	//  "Apple::Protocol::FromServer::QuestSystem::AcceptQuestResult"   "faac064cc47448f533db91322a3d16e4"
		{
			return UnPacket_AcceptQuestResult(pszRecvPacket);
		}
		break;
// 	case 0x0000A631:		// Apple::Protocol::ToServer::SpecialAttackSystem::ActivateSpecialAttack		激活特殊攻击
// 	case 0x0000801C:		// Apple::Protocol::Both::AntiHackingSystem::RawData
// 	case 0x0000DFC1:		// Apple::Protocol::FromServer::AnnonceSystem::WorldNoticeByUser		世界聊天消息
// 	case 0x00007BA5:		// Apple::Protocol::FromServer::ActorSystem::PlayerInfoChange			玩家信息变化
// 	case 0x000044D0:		// Apple::Protocol::FromServer::ShopSystem::GetRepairPrice
// 	case 0x00000001:
// 	case 0x00003AE5:		// Apple::Protocol::Both::StatusSystem::ChangeStatusBundleByFloat
// 	case 0x00004E3E:		// Apple::Protocol::FromServer::ActorSystem::DurabilityChange			耐久变化
// 	case 0x0000DA5E:		// Apple::Protocol::FromServer::QuestSystem::UpdateQuestMarks			启动房间序号id，禁用房间序号id，怪哉
// 		{
// 
// 		}
// 		break;
// 	case 0x0000E9FA://0x00004AE9:		// Apple::Protocol::FromServer::QuestSystem::ActiveQuestChanged		 "ce48d875f79c5b8e08df5eaf65c1abfd"
// 		{
// 			//MessageBox(NULL,"ActiveQuestChanged","",MB_OK);
// 		}
	case 0x000057B6:	// "8758da9ba83be6d8d94a09cf26af11b2"
		{
			//return UnPacket_Login(pszRecvPacket);
		}
		break;
	case 0x00008A17:	// "97cd2d91540399d862ba54fd2a7c0034"	"Apple::Protocol::FromServer::ShopSystem::SellItemResult"
		{
			//return UnPacket_SellItemResult(pszRecvPacket);
		}
		break;	
	case 0x00006D54:	//  "bd1cca3fb041b5a1acc2f523ac85b8a6"	"FromServer::StageSystem::CreateNpc"
		{
			return UnPacket_CreateNpc(pszRecvPacket);
		}
		break;
	case 0x0000F704:	// "0820e252bfbc80a17c85e238466bc65b"	"Apple::Protocol::FromServer::ActorSystem::CreatePopInfoNpc"
		{
			//return UnPacket_CreatePopInfoNpc(pszRecvPacket);
		}
		break;
	case 0x0000D999:		// "FromServer::StageSystem::PlayerDead"     "c85a0cd95fabae899231e8e33d33d831"
		{
			return UnPacket_PlayerDead(pszRecvPacket);
		}
		break;
	case 0x0000487D:		// "156560dcd0fcee4201d7fbace468b5d2"	"FromServer::StageSystem::ChangeVillage"
		{
			return UnPacket_ChangeVillage(pszRecvPacket);
		}
		break;
	case 0x00001DD9:	// "b136f214af569b58119e615f038f5680"	"FromServer::StageSystem::CreatePlayer"
		{
			
		}
		break;
// 	case 0x00002465:	// "c37fbd403882d58098930b40a2e4d4a2"	"Apple::Protocol::FromServer::ActorSystem::PlayerInfoChange"
// 		{
// 			//assert(false);
// 		}
// 		break;
	default:
		{
			for (DWORD i = 0 ; i < dwlen ; i++)
			{
				wsprintfA(szSendRecvData+i*3,"%02X ",pszRecvPacket[i]);
			}
			szSendRecvData[dwlen*3] = 0;

			wsprintfA(szRecvPrint,"[JFZR] recv %s",szSendRecvData);
			OutputDebugStringA(szRecvPrint);
		}
	}

	return true;
}

// 选择频道时取值发包
bool UnPacket::UnPacket_AllowSwitchChannel(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_AllowSwitchChannel*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwResult;
		WORD	wChannelIdLen;
		std::wstring	strChannelId;
		WORD	wAccountIdLen;
		std::wstring	strAccountId;
		WORD	wSessionKeyLen;
		std::wstring	strSessionKey;
		WORD	wStageServerAddresses;
		WORD	warray;
		DWORD	dwsize;
		struct  
		{
			WORD	witem_i;
			WORD	wAllM_Net_Address;
			WORD	wIpLen;
			std::string	strIp;
			WORD	wPort;
		}ITEM[100];
		WORD	wPrevChannelIdLen;
		std::wstring	strPrevChannelId;
	}AllowSwitchChannel;

	try
	{
		*this = (pszRecvPacket);
		*this >> AllowSwitchChannel.wPacketLen;
		*this >> AllowSwitchChannel.wMethodType;
		*this >> AllowSwitchChannel.wUrlType;
		*this >> AllowSwitchChannel.dwResult;
		*this >> AllowSwitchChannel.wChannelIdLen;
		*this >> AllowSwitchChannel.strChannelId;		// 频道id
		*this >> AllowSwitchChannel.wAccountIdLen;
		*this >> AllowSwitchChannel.strAccountId;		// QQ
		*this >> AllowSwitchChannel.wSessionKeyLen;
		*this >> AllowSwitchChannel.strSessionKey;		// 进入频道需要用到的key值
		*this >> AllowSwitchChannel.wStageServerAddresses;
		*this >> AllowSwitchChannel.warray;
		*this >> AllowSwitchChannel.dwsize;

		for (DWORD i = 0; i < AllowSwitchChannel.dwsize; i++)
		{
			*this >> AllowSwitchChannel.ITEM[i].witem_i;
			*this >> AllowSwitchChannel.ITEM[i].wAllM_Net_Address;
			*this >> AllowSwitchChannel.ITEM[i].wIpLen;
			*this >> AllowSwitchChannel.ITEM[i].strIp;
			*this >> AllowSwitchChannel.ITEM[i].wPort;
		}

		*this >> AllowSwitchChannel.wPrevChannelIdLen;
		*this >> AllowSwitchChannel.strPrevChannelId;

		assert(AllowSwitchChannel.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

		JFZRActionSystem theActionSystem;
		theActionSystem.SwitchChannel(	AllowSwitchChannel.strChannelId.c_str(),
			AllowSwitchChannel.strAccountId.c_str(),
			AllowSwitchChannel.strSessionKey.c_str());
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

// 频道链表
bool UnPacket::UnPacket_ChannelList(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_ChannelList*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		WORD	wChannels;
		WORD	warray;
		DWORD	dwsize;
		struct  
		{
			WORD	witem_i;
			WORD	wApple_ChannelElementForClient;
			DWORD	dwChannelNumber;
			WORD	wChannelIdLen;
			std::wstring	strChannelId;			// 频道id 如： "channel_03_01"
			WORD	wTitleLen;	
			std::wstring	strTitle;				// 标题 如：频道1
			BYTE	bEnabled;						// 激活状态  1为可登陆  0为不可登陆
			DWORD	dwCapacity;						// 可进入频道最大人数
			DWORD	dwPlayerCount;					// 实际玩家人数
			DWORD	dwMinPvpPoint;
			DWORD	dwMaxPvpPoint;
			DWORD	dwChannelPropertyId;
		}CHANNELS[100];
	}ChannelList;

	try
	{
		*this = (pszRecvPacket);
		*this >> ChannelList.wPacketLen;
		*this >> ChannelList.wMethodType;
		*this >> ChannelList.wUrlType;
		*this >> ChannelList.wChannels;
		*this >> ChannelList.warray;
		*this >> ChannelList.dwsize;

		for (DWORD i = 0; i<ChannelList.dwsize ; i++)
		{
			*this >> ChannelList.CHANNELS->witem_i;
			*this >> ChannelList.CHANNELS->wApple_ChannelElementForClient;
			*this >> ChannelList.CHANNELS->dwChannelNumber;
			*this >> ChannelList.CHANNELS->wChannelIdLen;
			*this >> ChannelList.CHANNELS->strChannelId;
			*this >> ChannelList.CHANNELS->wTitleLen;
			*this >> ChannelList.CHANNELS->strTitle;
			*this >> ChannelList.CHANNELS->bEnabled;
			*this >> ChannelList.CHANNELS->dwCapacity;
			*this >> ChannelList.CHANNELS->dwPlayerCount;
			*this >> ChannelList.CHANNELS->dwMinPvpPoint;
			*this >> ChannelList.CHANNELS->dwMaxPvpPoint;
			*this >> ChannelList.CHANNELS->dwChannelPropertyId;
		}
	}
	catch (...)
	{
		assert(false);
	}

	
	return true;
}

// 进入选择角色界面需要的角色信息
bool UnPacket::UnPacket_PartialListPlayersStaticInfo(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_PartialListPlayersStaticInfo*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		WORD	wAccountIdLen;
		std::wstring	strAccountId;			// QQ
		WORD			wStaticInfo;
		WORD			wApple_StaticPlayerInfo;
		WORD			wPlayerNameLen;
		std::wstring    strPlayerName;			// 玩家姓名
		LONGLONG		lPlayerSerial;
		WORD			wPlayerGenderLen;
		std::wstring	strPlayerGender;		// 玩家性别
		WORD			wClassTypeLen;
		std::wstring	strClassType;			// 职业类型，包括分支  GLADIBERSERKER 收到的都是大写
		WORD			wCreatedDateTime;
		WORD			wApple_DateTime;
		WORD			wYear;
		WORD			wMonth;
		WORD			wDay;
		WORD			wHour;
		DWORD			dwMinute;
		DWORD			dwSecond;
		DWORD			dwMilliSec;
		BYTE			bIsInfinity;
		BYTE			bIsDeleted;				// 角色是否在删除状态  00 正常状态， 01 删除状态
	}PartialListPlayersStaticInfo;

	try
	{
		*this = (pszRecvPacket);
		*this >> PartialListPlayersStaticInfo.wPacketLen;
		*this >> PartialListPlayersStaticInfo.wMethodType;
		*this >> PartialListPlayersStaticInfo.wUrlType;
		*this >> PartialListPlayersStaticInfo.wAccountIdLen;
		*this >> PartialListPlayersStaticInfo.strAccountId;
		*this >> PartialListPlayersStaticInfo.wStaticInfo;
		*this >> PartialListPlayersStaticInfo.wApple_StaticPlayerInfo;
		*this >> PartialListPlayersStaticInfo.wPlayerNameLen;
		*this >> PartialListPlayersStaticInfo.strPlayerName;
		*this >> PartialListPlayersStaticInfo.lPlayerSerial;
		*this >> PartialListPlayersStaticInfo.wPlayerGenderLen;
		*this >> PartialListPlayersStaticInfo.strPlayerGender;
		*this >> PartialListPlayersStaticInfo.wClassTypeLen;
		*this >> PartialListPlayersStaticInfo.strClassType;
		*this >> PartialListPlayersStaticInfo.wCreatedDateTime;
		*this >> PartialListPlayersStaticInfo.wApple_DateTime;
		*this >> PartialListPlayersStaticInfo.wYear;
		*this >> PartialListPlayersStaticInfo.wMonth;
		*this >> PartialListPlayersStaticInfo.wDay;
		*this >> PartialListPlayersStaticInfo.wHour;
		*this >> PartialListPlayersStaticInfo.dwMinute;
		*this >> PartialListPlayersStaticInfo.dwSecond;
		*this >> PartialListPlayersStaticInfo.dwMilliSec;
		*this >> PartialListPlayersStaticInfo.bIsInfinity;
		*this >> PartialListPlayersStaticInfo.bIsDeleted;
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

// 接受查询拍卖数据封包
bool UnPacket::UnPacket_RequestProductList(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_RequestProductList*********************");

	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwResult;
		DWORD	dwTotalPage1;			// 显示总共多少页
		DWORD	dwTotalPage2;
		DWORD	dwTotalPage3;
		WORD	wCurrentServerDate;
		APPLE_DATA_TIME	theDataTime1;
		WORD			wProducts;
		WORD			warray;
		DWORD			dwsize;
		struct  
		{
			WORD			witem_i;
			WORD			wApple_MarketProductInfo;
			LONGLONG		llOidMarket;				// 拍卖物品的序号
			WORD			wSellerNameLen;
			std::wstring	strSellerName;				// 物品所属玩家名称
			WORD			wCategoryLen;
			std::wstring	strCategory;
			WORD			wItemNameLen;
			std::wstring	strItemName;				// 物品名称
			DWORD			dwMinLevel;
			DWORD			dwMaxLevel;
			WORD			wItemId;
			std::wstring	strItemId;					// 物品id
			DWORD			dwSellingCount;				// 出售数量
			DWORD			dwDurability;
			DWORD			dwGrade;
			DWORD			dwEnhanceLevel;
			WORD			wIncreaseStatusLen;
			std::wstring	strIncreaseStatus;
			WORD			wIdentifiedStatusLen;
			std::wstring	strIdentifiedStatus;
			WORD			wEventOptions;
			std::wstring	strEventOptions;
			WORD			wEnchantIdLen;
			std::wstring	strEnchantId;
			WORD			wTierEnchantIdLen;
			std::wstring	strTierEnchantId;
			BYTE			bIsSealed;
			BYTE			bIsUnidentified;
			BYTE			bIsTradable;
			BYTE			bIsCash;
			WORD			wExpirationTime;
			APPLE_DATA_TIME	theDataTime2;
			LONGLONG		llPrice;					// 物品价格
			WORD			wExpireDate;
			APPLE_DATA_TIME	theDataTime3;
			DWORD			dwBindType;
			WORD			wExtendInfoLen;
			std::wstring	strExtendInfo;
		}theItem[100];
	}RequestProductList;

	try
	{
		*this = (pszRecvPacket);
		*this >> RequestProductList.wPacketLen;
		*this >> RequestProductList.wMethodType;
		*this >> RequestProductList.wUrlType;
		*this >> RequestProductList.dwResult;
		*this >> RequestProductList.dwTotalPage1;
		*this >> RequestProductList.dwTotalPage2;
		*this >> RequestProductList.dwTotalPage3;
		*this >> RequestProductList.wCurrentServerDate;
		*this >> RequestProductList.theDataTime1.wApple_DateTime;
		*this >> RequestProductList.theDataTime1.wYear;
		*this >> RequestProductList.theDataTime1.wMonth;
		*this >> RequestProductList.theDataTime1.wDay;
		*this >> RequestProductList.theDataTime1.dwHour;
		*this >> RequestProductList.theDataTime1.dwMinute;
		*this >> RequestProductList.theDataTime1.dwSecond;
		*this >> RequestProductList.theDataTime1.dwMilliSec;
		*this >> RequestProductList.theDataTime1.IsInfinity;
		*this >> RequestProductList.wProducts;
		*this >> RequestProductList.warray;
		*this >> RequestProductList.dwsize;

		for (DWORD i=0; i<RequestProductList.dwsize; i++)
		{
			*this >> RequestProductList.theItem[i].witem_i;
			*this >> RequestProductList.theItem[i].wApple_MarketProductInfo;
			*this >> RequestProductList.theItem[i].llOidMarket;
			*this >> RequestProductList.theItem[i].wSellerNameLen;
			*this >> RequestProductList.theItem[i].strSellerName;
			*this >> RequestProductList.theItem[i].wCategoryLen;
			*this >> RequestProductList.theItem[i].strCategory;
			*this >> RequestProductList.theItem[i].wItemNameLen;
			*this >> RequestProductList.theItem[i].strItemName;
			*this >> RequestProductList.theItem[i].dwMinLevel;
			*this >> RequestProductList.theItem[i].dwMaxLevel;
			*this >> RequestProductList.theItem[i].wItemId;
			*this >> RequestProductList.theItem[i].strItemId;
			*this >> RequestProductList.theItem[i].dwSellingCount;
			*this >> RequestProductList.theItem[i].dwDurability;
			*this >> RequestProductList.theItem[i].dwGrade;
			*this >> RequestProductList.theItem[i].dwEnhanceLevel;
			*this >> RequestProductList.theItem[i].wIncreaseStatusLen;
			*this >> RequestProductList.theItem[i].strIncreaseStatus;
			*this >> RequestProductList.theItem[i].wIdentifiedStatusLen;
			*this >> RequestProductList.theItem[i].strIdentifiedStatus;
			*this >> RequestProductList.theItem[i].wEventOptions;
			*this >> RequestProductList.theItem[i].strEventOptions;
			*this >> RequestProductList.theItem[i].wEnchantIdLen;
			*this >> RequestProductList.theItem[i].strEnchantId;
			*this >> RequestProductList.theItem[i].wTierEnchantIdLen;
			*this >> RequestProductList.theItem[i].strTierEnchantId;
			*this >> RequestProductList.theItem[i].bIsSealed;
			*this >> RequestProductList.theItem[i].bIsUnidentified;
			*this >> RequestProductList.theItem[i].bIsTradable;
			*this >> RequestProductList.theItem[i].bIsCash;
			*this >> RequestProductList.theItem[i].wExpirationTime;
			*this >> RequestProductList.theItem[i].theDataTime2.wApple_DateTime;
			*this >> RequestProductList.theItem[i].theDataTime2.wYear;
			*this >> RequestProductList.theItem[i].theDataTime2.wMonth;
			*this >> RequestProductList.theItem[i].theDataTime2.wDay;
			*this >> RequestProductList.theItem[i].theDataTime2.dwHour;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMinute;
			*this >> RequestProductList.theItem[i].theDataTime2.dwSecond;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMilliSec;
			*this >> RequestProductList.theItem[i].theDataTime2.IsInfinity;
			*this >> RequestProductList.theItem[i].llPrice;
			*this >> RequestProductList.theItem[i].wExpireDate;
			*this >> RequestProductList.theItem[i].theDataTime3.wApple_DateTime;
			*this >> RequestProductList.theItem[i].theDataTime3.wYear;
			*this >> RequestProductList.theItem[i].theDataTime3.wMonth;
			*this >> RequestProductList.theItem[i].theDataTime3.wDay;
			*this >> RequestProductList.theItem[i].theDataTime3.dwHour;
			*this >> RequestProductList.theItem[i].theDataTime3.dwMinute;
			*this >> RequestProductList.theItem[i].theDataTime3.dwSecond;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMilliSec;
			*this >> RequestProductList.theItem[i].theDataTime3.IsInfinity;
			*this >> RequestProductList.theItem[i].dwBindType;
			*this >> RequestProductList.theItem[i].wExtendInfoLen;
			*this >> RequestProductList.theItem[i].strExtendInfo;
		}
	}
	catch (...)
	{
		assert(false);
	}


	return true;
}

// 接受查询我的拍卖数据封包
bool UnPacket::UnPacket_RequestMyProductList(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_RequestMyProductList*********************");

	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwResult;
		WORD	wCurrentServerDate;
		APPLE_DATA_TIME	theDataTime1;
		WORD			wProducts;
		WORD			warray;
		DWORD			dwsize;
		struct  
		{
			WORD			witem_i;
			WORD			wApple_MarketProductInfo;
			LONGLONG		llOidMarket;				// 我的拍卖物品序号，可以用来操作取消拍卖
			WORD			wSellerNameLen;
			std::wstring	strSellerName;
			WORD			wCategoryLen;
			std::wstring	strCategory;
			WORD			wItemNameLen;
			std::wstring	strItemName;				// 物品名称
			DWORD			dwMinLevel;
			DWORD			dwMaxLevel;
			WORD			wItemId;
			std::wstring	strItemId;					// 物品id
			DWORD			dwSellingCount;				// 物品数量
			DWORD			dwDurability;
			DWORD			dwGrade;
			DWORD			dwEnhanceLevel;
			WORD			wIncreaseStatusLen;
			std::wstring	strIncreaseStatus;
			WORD			wIdentifiedStatusLen;
			std::wstring	strIdentifiedStatus;
			WORD			wEventOptions;
			std::wstring	strEventOptions;
			WORD			wEnchantIdLen;
			std::wstring	strEnchantId;
			WORD			wTierEnchantIdLen;
			std::wstring	strTierEnchantId;
			BYTE			bIsSealed;
			BYTE			bIsUnidentified;
			BYTE			bIsTradable;
			BYTE			bIsCash;
			WORD			wExpirationTime;
			APPLE_DATA_TIME	theDataTime2;
			LONGLONG		llPrice;					// 物品价格
			WORD			wExpireDate;
			APPLE_DATA_TIME	theDataTime3;
			DWORD			dwBindType;
			WORD			wExtendInfoLen;
			std::wstring	strExtendInfo;
		}theItem[100];
	}RequestProductList;

	try
	{
		*this = (pszRecvPacket);
		*this >> RequestProductList.wPacketLen;
		*this >> RequestProductList.wMethodType;
		*this >> RequestProductList.wUrlType;
		*this >> RequestProductList.dwResult;
		*this >> RequestProductList.wCurrentServerDate;
		*this >> RequestProductList.theDataTime1.wApple_DateTime;
		*this >> RequestProductList.theDataTime1.wYear;
		*this >> RequestProductList.theDataTime1.wMonth;
		*this >> RequestProductList.theDataTime1.wDay;
		*this >> RequestProductList.theDataTime1.dwHour;
		*this >> RequestProductList.theDataTime1.dwMinute;
		*this >> RequestProductList.theDataTime1.dwSecond;
		*this >> RequestProductList.theDataTime1.dwMilliSec;
		*this >> RequestProductList.theDataTime1.IsInfinity;
		*this >> RequestProductList.wProducts;
		*this >> RequestProductList.warray;
		*this >> RequestProductList.dwsize;

		for (DWORD i=0; i<RequestProductList.dwsize; i++)
		{
			*this >> RequestProductList.theItem[i].witem_i;
			*this >> RequestProductList.theItem[i].wApple_MarketProductInfo;
			*this >> RequestProductList.theItem[i].llOidMarket;
			*this >> RequestProductList.theItem[i].wSellerNameLen;
			*this >> RequestProductList.theItem[i].strSellerName;
			*this >> RequestProductList.theItem[i].wCategoryLen;
			*this >> RequestProductList.theItem[i].strCategory;
			*this >> RequestProductList.theItem[i].wItemNameLen;
			*this >> RequestProductList.theItem[i].strItemName;
			*this >> RequestProductList.theItem[i].dwMinLevel;
			*this >> RequestProductList.theItem[i].dwMaxLevel;
			*this >> RequestProductList.theItem[i].wItemId;
			*this >> RequestProductList.theItem[i].strItemId;
			*this >> RequestProductList.theItem[i].dwSellingCount;
			*this >> RequestProductList.theItem[i].dwDurability;
			*this >> RequestProductList.theItem[i].dwGrade;
			*this >> RequestProductList.theItem[i].dwEnhanceLevel;
			*this >> RequestProductList.theItem[i].wIncreaseStatusLen;
			*this >> RequestProductList.theItem[i].strIncreaseStatus;
			*this >> RequestProductList.theItem[i].wIdentifiedStatusLen;
			*this >> RequestProductList.theItem[i].strIdentifiedStatus;
			*this >> RequestProductList.theItem[i].wEventOptions;
			*this >> RequestProductList.theItem[i].strEventOptions;
			*this >> RequestProductList.theItem[i].wEnchantIdLen;
			*this >> RequestProductList.theItem[i].strEnchantId;
			*this >> RequestProductList.theItem[i].wTierEnchantIdLen;
			*this >> RequestProductList.theItem[i].strTierEnchantId;
			*this >> RequestProductList.theItem[i].bIsSealed;
			*this >> RequestProductList.theItem[i].bIsUnidentified;
			*this >> RequestProductList.theItem[i].bIsTradable;
			*this >> RequestProductList.theItem[i].bIsCash;
			*this >> RequestProductList.theItem[i].wExpirationTime;
			*this >> RequestProductList.theItem[i].theDataTime2.wApple_DateTime;
			*this >> RequestProductList.theItem[i].theDataTime2.wYear;
			*this >> RequestProductList.theItem[i].theDataTime2.wMonth;
			*this >> RequestProductList.theItem[i].theDataTime2.wDay;
			*this >> RequestProductList.theItem[i].theDataTime2.dwHour;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMinute;
			*this >> RequestProductList.theItem[i].theDataTime2.dwSecond;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMilliSec;
			*this >> RequestProductList.theItem[i].theDataTime2.IsInfinity;
			*this >> RequestProductList.theItem[i].llPrice;
			*this >> RequestProductList.theItem[i].wExpireDate;
			*this >> RequestProductList.theItem[i].theDataTime3.wApple_DateTime;
			*this >> RequestProductList.theItem[i].theDataTime3.wYear;
			*this >> RequestProductList.theItem[i].theDataTime3.wMonth;
			*this >> RequestProductList.theItem[i].theDataTime3.wDay;
			*this >> RequestProductList.theItem[i].theDataTime3.dwHour;
			*this >> RequestProductList.theItem[i].theDataTime3.dwMinute;
			*this >> RequestProductList.theItem[i].theDataTime3.dwSecond;
			*this >> RequestProductList.theItem[i].theDataTime2.dwMilliSec;
			*this >> RequestProductList.theItem[i].theDataTime3.IsInfinity;
			*this >> RequestProductList.theItem[i].dwBindType;
			*this >> RequestProductList.theItem[i].wExtendInfoLen;
			*this >> RequestProductList.theItem[i].strExtendInfo;
		}
	}
	catch (...)
	{
		assert(false);
	}


	return true;
}


bool UnPacket::UnPacket_MailList(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_MailList*********************");

	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwResult;
		WORD	wList;
		WORD	warray;
		DWORD	dwsize;
		struct  
		{
			WORD			witem_i;
			WORD			wApple_MailHeader;
			LONGLONG		llMailSerial;			// 序号，用于接受邮件时需要
			WORD			wTitleLen;
			std::wstring	strTitle;
			WORD			wSenderIdLen;
			std::wstring	strSenderId;
			BYTE			bIsRead;
			LONGLONG		llGold;					// 金币	= 0，则表示没有金币
			WORD			wSampleItemIdLen;		// 如果 = 1，则表示没有图标没有物品，也可以表示附件没有物品
			std::wstring	strSampleItemId;		// 邮件图标物品
			BYTE			bHadAttchmentGot;
			WORD			wTypeLen;
			std::wstring	strType;				// 邮件类型 分系统(SYSTEM)和玩家(NORMAL)
			WORD			wExpireDate;
			APPLE_DATA_TIME	theDataTime;
			BYTE			bIsSampleItemSealed;
			WORD			wSampleItemGradeLen;
			std::wstring	strSampleItemGrade;
		}Item[100];
	}theMailList;

	try
	{
		*this = (pszRecvPacket);
		*this >> theMailList.wPacketLen;
		*this >> theMailList.wMethodType;
		*this >> theMailList.wUrlType;
		*this >> theMailList.dwResult;
		*this >> theMailList.wList;
		*this >> theMailList.warray;
		*this >> theMailList.dwsize;

		for (DWORD i=0; i<theMailList.dwsize; i++)
		{
			*this >> theMailList.Item[i].witem_i;
			*this >> theMailList.Item[i].wApple_MailHeader;
			*this >> theMailList.Item[i].llMailSerial;
			*this >> theMailList.Item[i].wTitleLen;
			*this >> theMailList.Item[i].strTitle;
			*this >> theMailList.Item[i].wSenderIdLen;
			*this >> theMailList.Item[i].strSenderId;
			*this >> theMailList.Item[i].bIsRead;
			*this >> theMailList.Item[i].llGold;
			*this >> theMailList.Item[i].wSampleItemIdLen;
			*this >> theMailList.Item[i].strSampleItemId;
			*this >> theMailList.Item[i].bHadAttchmentGot;
			*this >> theMailList.Item[i].wTypeLen;
			*this >> theMailList.Item[i].strType;
			*this >> theMailList.Item[i].wExpireDate;
			*this >> theMailList.Item[i].theDataTime.wApple_DateTime;
			*this >> theMailList.Item[i].theDataTime.wYear;
			*this >> theMailList.Item[i].theDataTime.wMonth;
			*this >> theMailList.Item[i].theDataTime.wDay;
			*this >> theMailList.Item[i].theDataTime.dwHour;
			*this >> theMailList.Item[i].theDataTime.dwMinute;
			*this >> theMailList.Item[i].theDataTime.dwSecond;
			*this >> theMailList.Item[i].theDataTime.dwMilliSec;
			*this >> theMailList.Item[i].theDataTime.IsInfinity;
			*this >> theMailList.Item[i].bIsSampleItemSealed;
			*this >> theMailList.Item[i].wSampleItemGradeLen;
			*this >> theMailList.Item[i].strSampleItemGrade;

		}

	}
	catch (...)
	{
		assert(false);
	}


	return true;
}


bool UnPacket::UnPacket_CreatePlayer(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_CreatePlayer*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD	dwResult;				// dwResult = 0 表示创建角色成功，随后会重新收到所有角色信息；如果 dwResult = 6 表示角色名已被使用
	}theCreatePlayer;

	try
	{
		*this = (pszRecvPacket);
		*this >> theCreatePlayer.wPacketLen;
		*this >> theCreatePlayer.wMethodType;
		*this >> theCreatePlayer.wUrlType;
		*this >> theCreatePlayer.dwResult;
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



bool UnPacket::UnPacket_EndPartialListPlayers(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_EndPartialListPlayers*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		WORD			wAccountIdLen;
		std::wstring	strAccountId;
		WORD			wSelectedPlayerIdLen;
		std::wstring	strSelectedPlayerId;
		DWORD			dwConnectionSerial;
		DWORD			dwSendingType;
	}theEndPartialListPlayers;

	try
	{
		*this = (pszRecvPacket);
		*this >> theEndPartialListPlayers.wPacketLen;
		*this >> theEndPartialListPlayers.wMethodType;
		*this >> theEndPartialListPlayers.wUrlType;
		*this >> theEndPartialListPlayers.wAccountIdLen;
		*this >> theEndPartialListPlayers.strAccountId;
		*this >> theEndPartialListPlayers.wSelectedPlayerIdLen;
		*this >> theEndPartialListPlayers.strSelectedPlayerId;
		*this >> theEndPartialListPlayers.dwConnectionSerial;
		*this >> theEndPartialListPlayers.dwSendingType;

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_Message(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_Message*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD			dwResult;
		WORD			wSenderIdLen;
		std::wstring	wstrSenderId;
		WORD			wCommand;
		WORD			wParams;
		WORD			warray;
		DWORD			dwsize;
		struct  
		{
			WORD		witemLen;
			std::string	stritem;
		}ITEM[10];
		DWORD			dwRepeat;
		DWORD			dwIntervalSecond;
		DWORD			dwDisplaySesond;
	}theMessage;

	try
	{
		*this = (pszRecvPacket);
		*this >> theMessage.wPacketLen;
		*this >> theMessage.wMethodType;
		*this >> theMessage.wUrlType;
		*this >> theMessage.dwResult;			// 成功返回值为0
		*this >> theMessage.wSenderIdLen;	
		*this >> theMessage.wstrSenderId;
		*this >> theMessage.wCommand;
		*this >> theMessage.wParams;
		*this >> theMessage.warray;
		*this >> theMessage.dwsize;
		for (DWORD i=0; i<theMessage.dwsize; i++)
		{
			*this >> theMessage.ITEM[i].witemLen;
			*this >> theMessage.ITEM[i].stritem;		// 当i=0时，stritem="Mail_OnArrived" 邮件到达的意思
		}
		*this >> theMessage.dwRepeat;
		*this >> theMessage.dwIntervalSecond;
		*this >> theMessage.dwDisplaySesond;

	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

bool UnPacket::UnPacket_RegisterProduct(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_RegisterProduct*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD			dwResult;
		WORD			wBlockExpiration;
		APPLE_DATA_TIME	theDataTime;
	}theRegisterProduct;

	try
	{
		*this = (pszRecvPacket);
		*this >> theRegisterProduct.wPacketLen;
		*this >> theRegisterProduct.wMethodType;
		*this >> theRegisterProduct.wUrlType;
		*this >> theRegisterProduct.dwResult;			// 成功返回值=0x0 已超出账号的每日交易次数=0x0D 已超出角色交易次数=0x0E  登陆游戏后，在一定时间内无法移动物品=0x11
		*this >> theRegisterProduct.wBlockExpiration;	
		*this >> theRegisterProduct.theDataTime.wApple_DateTime;
		*this >> theRegisterProduct.theDataTime.wYear;
		*this >> theRegisterProduct.theDataTime.wMonth;
		*this >> theRegisterProduct.theDataTime.wDay;
		*this >> theRegisterProduct.theDataTime.dwHour;
		*this >> theRegisterProduct.theDataTime.dwMinute;
		*this >> theRegisterProduct.theDataTime.dwSecond;
		*this >> theRegisterProduct.theDataTime.dwMilliSec;
		*this >> theRegisterProduct.theDataTime.IsInfinity;

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_BuyProduct(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_BuyProduct*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD			dwResult;
		WORD			wBlockExpiration;
		APPLE_DATA_TIME	theDataTime;
	}theBuyProduct;

	try
	{
		*this = (pszRecvPacket);
		*this >> theBuyProduct.wPacketLen;
		*this >> theBuyProduct.wMethodType;
		*this >> theBuyProduct.wUrlType;
		*this >> theBuyProduct.dwResult;			// 成功返回值为0
		*this >> theBuyProduct.wBlockExpiration;	
		*this >> theBuyProduct.theDataTime.wApple_DateTime;
		*this >> theBuyProduct.theDataTime.wYear;
		*this >> theBuyProduct.theDataTime.wMonth;
		*this >> theBuyProduct.theDataTime.wDay;
		*this >> theBuyProduct.theDataTime.dwHour;
		*this >> theBuyProduct.theDataTime.dwMinute;
		*this >> theBuyProduct.theDataTime.dwSecond;
		*this >> theBuyProduct.theDataTime.dwMilliSec;
		*this >> theBuyProduct.theDataTime.IsInfinity;

	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_UnregisterProduct(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_UnregisterProduct*********************");
	struct
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		DWORD			dwResult;
	}theUnregisterProduct;

	try
	{
		*this = (pszRecvPacket);
		*this >> theUnregisterProduct.wPacketLen;
		*this >> theUnregisterProduct.wMethodType;
		*this >> theUnregisterProduct.wUrlType;
		*this >> theUnregisterProduct.dwResult;			// 成功返回值为0
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_StartStageFailed(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_StartStageFailed*********************");
	try
	{
		RECV_START_STAGE_FAILED	theStartStageFailed;
		*this = (pszRecvPacket);
		*this >> theStartStageFailed.theHeader.wPacketLen;
		*this >> theStartStageFailed.theHeader.wMethodType;
		*this >> theStartStageFailed.theHeader.wUrlType;
		*this >> theStartStageFailed.wPlayerIds;
		*this >> theStartStageFailed.warray;
		*this >> theStartStageFailed.dwsize;
		for (DWORD i=0; i<theStartStageFailed.dwsize; i++)
		{
			*this >> theStartStageFailed.thePlayerIds[i].wPlayerIdsLen;
			*this >> theStartStageFailed.thePlayerIds[i].strPlayerIds;
		}
		*this >> theStartStageFailed.wBlockExpiration;
		*this >> theStartStageFailed.theDateTime.wApple_DateTime;
		*this >> theStartStageFailed.theDateTime.wYear;
		*this >> theStartStageFailed.theDateTime.wMonth;
		*this >> theStartStageFailed.theDateTime.wDay;
		*this >> theStartStageFailed.theDateTime.dwHour;
		*this >> theStartStageFailed.theDateTime.dwMinute;
		*this >> theStartStageFailed.theDateTime.dwSecond;
		*this >> theStartStageFailed.theDateTime.dwMilliSec;
		*this >> theStartStageFailed.theDateTime.IsInfinity;
		*this >> theStartStageFailed.dwResult;

		assert(theStartStageFailed.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


bool UnPacket::UnPacket_MailContents(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_MailContents*********************");
	try
	{
		RECV_MAIL_CONTENTS	theMailContents;

		*this = (pszRecvPacket);
		*this >> theMailContents.theHeader.wPacketLen;
		*this >> theMailContents.theHeader.wMethodType;
		*this >> theMailContents.theHeader.wUrlType;
		*this >> theMailContents.dwResult;
		*this >> theMailContents.wContents;
		*this >> theMailContents.wApple_MailContents;
		*this >> theMailContents.llMailSerial;
		*this >> theMailContents.wMessageLen;
		*this >> theMailContents.strMessage;
		*this >> theMailContents.wItems;
		*this >> theMailContents.warray;
		*this >> theMailContents.dwsize;			
	
		//assert(theMailContents.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_GetAttachments(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_GetAttachments*********************");
	try
	{
		RECV_GET_ATTACHMENTS	theGetAttachments;

		*this = (pszRecvPacket);
		*this >> theGetAttachments.theHeader.wPacketLen;
		*this >> theGetAttachments.theHeader.wMethodType;
		*this >> theGetAttachments.theHeader.wUrlType;
		*this >> theGetAttachments.dwResult;
		*this >> theGetAttachments.llMailSerial;
		*this >> theGetAttachments.wBlockExpiration;
		*this >> theGetAttachments.theDateTime.wApple_DateTime;
		*this >> theGetAttachments.theDateTime.wYear;
		*this >> theGetAttachments.theDateTime.wMonth;
		*this >> theGetAttachments.theDateTime.wDay;
		*this >> theGetAttachments.theDateTime.dwHour;
		*this >> theGetAttachments.theDateTime.dwMinute;
		*this >> theGetAttachments.theDateTime.dwSecond;
		*this >> theGetAttachments.theDateTime.dwMilliSec;
		*this >> theGetAttachments.theDateTime.IsInfinity;
			
		assert(theGetAttachments.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



bool UnPacket::UnPacket_RequestProductInfo(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_RequestProductInfo*********************");
	try
	{
		RECV_REQUEST_PRODUCT_INFO	theRequestProductInfo;

		*this = (pszRecvPacket);
		*this >> theRequestProductInfo.theHeader.wPacketLen;
		*this >> theRequestProductInfo.theHeader.wMethodType;
		*this >> theRequestProductInfo.theHeader.wUrlType;
		*this >> theRequestProductInfo.dwResult;
		*this >> theRequestProductInfo.wProduct;
		*this >> theRequestProductInfo.wApple_ProductStatistics;
		*this >> theRequestProductInfo.llAveragePrice;
		*this >> theRequestProductInfo.dwCount;
		*this >> theRequestProductInfo.llLowerPrice;

		assert(theRequestProductInfo.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}


bool UnPacket::UnPacket_Login(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_Login*********************");
	try
	{
		RECV_LOGIN	theLogin;

		*this = (pszRecvPacket);
		*this >> theLogin.theHeader.wPacketLen;
		*this >> theLogin.theHeader.wMethodType;
		*this >> theLogin.theHeader.wUrlType;
		*this >> theLogin.dwResult;
		*this >> theLogin.wAccountIdLen;
		*this >> theLogin.strAccountId;
		*this >> theLogin.dwAge;
		*this >> theLogin.dwShutdown;
		*this >> theLogin.dwKickTime;
		*this >> theLogin.bIsGm;
		*this >> theLogin.bIsSpecificAccount;
		*this >> theLogin.bPlayerNotCreated;
		*this >> theLogin.wWorldNameLen;
		*this >> theLogin.strWorldName;
		*this >> theLogin.wWorldIdLen;
		*this >> theLogin.strWorldId;
		*this >> theLogin.llOidAccount;
		*this >> theLogin.bIsReturnee;
		*this >> theLogin.bIsMembership;
		*this >> theLogin.wReturnExpiration;
		*this >> theLogin.theReturnExpirationDataTime.wApple_DateTime;
		*this >> theLogin.theReturnExpirationDataTime.wYear;
		*this >> theLogin.theReturnExpirationDataTime.wMonth;
		*this >> theLogin.theReturnExpirationDataTime.wDay;
		*this >> theLogin.theReturnExpirationDataTime.dwHour;
		*this >> theLogin.theReturnExpirationDataTime.dwMinute;
		*this >> theLogin.theReturnExpirationDataTime.dwSecond;
		*this >> theLogin.theReturnExpirationDataTime.dwMilliSec;
		*this >> theLogin.theReturnExpirationDataTime.IsInfinity;
		*this >> theLogin.wMembershipExpiration;
		*this >> theLogin.theMembershipExpirationDataTime.wApple_DateTime;
		*this >> theLogin.theMembershipExpirationDataTime.wYear;
		*this >> theLogin.theMembershipExpirationDataTime.wMonth;
		*this >> theLogin.theMembershipExpirationDataTime.wDay;
		*this >> theLogin.theMembershipExpirationDataTime.dwHour;
		*this >> theLogin.theMembershipExpirationDataTime.dwMinute;
		*this >> theLogin.theMembershipExpirationDataTime.dwSecond;
		*this >> theLogin.theMembershipExpirationDataTime.dwMilliSec;
		*this >> theLogin.theMembershipExpirationDataTime.IsInfinity;
		*this >> theLogin.wCreated;
		*this >> theLogin.theCreatedDataTime.wApple_DateTime;
		*this >> theLogin.theCreatedDataTime.wYear;
		*this >> theLogin.theCreatedDataTime.wMonth;
		*this >> theLogin.theCreatedDataTime.wDay;
		*this >> theLogin.theCreatedDataTime.dwHour;
		*this >> theLogin.theCreatedDataTime.dwMinute;
		*this >> theLogin.theCreatedDataTime.dwSecond;
		*this >> theLogin.theCreatedDataTime.dwMilliSec;
		*this >> theLogin.theCreatedDataTime.IsInfinity;
		*this >> theLogin.wBlockExpiration;
		*this >> theLogin.theBlockExpirationDataTime.wApple_DateTime;
		*this >> theLogin.theBlockExpirationDataTime.wYear;
		*this >> theLogin.theBlockExpirationDataTime.wMonth;
		*this >> theLogin.theBlockExpirationDataTime.wDay;
		*this >> theLogin.theBlockExpirationDataTime.dwHour;
		*this >> theLogin.theBlockExpirationDataTime.dwMinute;
		*this >> theLogin.theBlockExpirationDataTime.dwSecond;
		*this >> theLogin.theBlockExpirationDataTime.dwMilliSec;
		*this >> theLogin.theBlockExpirationDataTime.IsInfinity;
		*this >> theLogin.dwBlockType;
		*this >> theLogin.wBlockReasonLen;
		*this >> theLogin.strBlockReason;
		*this >> theLogin.dwBlockGrade;
		*this >> theLogin.wBlockGradeExpiration;
		*this >> theLogin.theBlockGradeExpirationDataTime.wApple_DateTime;
		*this >> theLogin.theBlockGradeExpirationDataTime.wYear;
		*this >> theLogin.theBlockGradeExpirationDataTime.wMonth;
		*this >> theLogin.theBlockGradeExpirationDataTime.wDay;
		*this >> theLogin.theBlockGradeExpirationDataTime.dwHour;
		*this >> theLogin.theBlockGradeExpirationDataTime.dwMinute;
		*this >> theLogin.theBlockGradeExpirationDataTime.dwSecond;
		*this >> theLogin.theBlockGradeExpirationDataTime.dwMilliSec;
		*this >> theLogin.theBlockGradeExpirationDataTime.IsInfinity;

		assert(theLogin.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



bool UnPacket::UnPacket_SellItemResult(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_SellItemResult*********************");
	try
	{
		RECV_SELL_ITEM_RESULT	theSellItemResult;

		*this = (pszRecvPacket);
		*this >> theSellItemResult.theHeader.wPacketLen;
		*this >> theSellItemResult.theHeader.wMethodType;
		*this >> theSellItemResult.theHeader.wUrlType;
		*this >> theSellItemResult.dwResult;
		*this >> theSellItemResult.wBlockExpiration;
		*this >> theSellItemResult.theBlockExpirationDataTime.wApple_DateTime;
		*this >> theSellItemResult.theBlockExpirationDataTime.wYear;
		*this >> theSellItemResult.theBlockExpirationDataTime.wMonth;
		*this >> theSellItemResult.theBlockExpirationDataTime.wDay;
		*this >> theSellItemResult.theBlockExpirationDataTime.dwHour;
		*this >> theSellItemResult.theBlockExpirationDataTime.dwMinute;
		*this >> theSellItemResult.theBlockExpirationDataTime.dwSecond;
		*this >> theSellItemResult.theBlockExpirationDataTime.dwMilliSec;
		*this >> theSellItemResult.theBlockExpirationDataTime.IsInfinity;

		assert(theSellItemResult.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

bool UnPacket::UnPacket_BuyItemResult(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_BuyItemResult*********************");
	try
	{
		RECV_SELL_ITEM_RESULT	theBuyItemResult;

		*this = (pszRecvPacket);
		*this >> theBuyItemResult.theHeader.wPacketLen;
		*this >> theBuyItemResult.theHeader.wMethodType;
		*this >> theBuyItemResult.theHeader.wUrlType;
		*this >> theBuyItemResult.dwResult;
		*this >> theBuyItemResult.wBlockExpiration;
		*this >> theBuyItemResult.theBlockExpirationDataTime.wApple_DateTime;
		*this >> theBuyItemResult.theBlockExpirationDataTime.wYear;
		*this >> theBuyItemResult.theBlockExpirationDataTime.wMonth;
		*this >> theBuyItemResult.theBlockExpirationDataTime.wDay;
		*this >> theBuyItemResult.theBlockExpirationDataTime.dwHour;
		*this >> theBuyItemResult.theBlockExpirationDataTime.dwMinute;
		*this >> theBuyItemResult.theBlockExpirationDataTime.dwSecond;
		*this >> theBuyItemResult.theBlockExpirationDataTime.dwMilliSec;
		*this >> theBuyItemResult.theBlockExpirationDataTime.IsInfinity;

		assert(theBuyItemResult.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

extern DWORD g_dwValidationData_X;		// 过图验证数据x
extern DWORD g_dwValidationData_Y;		// 过图验证数据y
extern BYTE  g_bIsDummyExist;

// 创建静态物品和不可打NPC
bool UnPacket::UnPacket_CreateNpc(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_CreateNpc*********************");
	try
	{
		RECV_CREATE_NPC	theCreateNpc;

		DWORD i = 0;
		*this = (pszRecvPacket);
		*this >> theCreateNpc.theHeader.wPacketLen;
		*this >> theCreateNpc.theHeader.wMethodType;
		*this >> theCreateNpc.theHeader.wUrlType;
		*this >> theCreateNpc.wNpcIdLen;
		*this >> theCreateNpc.strNpcId;
		*this >> theCreateNpc.wTeamIdLen;
		*this >> theCreateNpc.strTeamId;
		*this >> theCreateNpc.wDefaultActionIdLen;
		*this >> theCreateNpc.strDefaultActionId;
		*this >> theCreateNpc.wLocationIdLen;
		*this >> theCreateNpc.strLocationId;
		*this >> theCreateNpc.dwSerial;
		*this >> theCreateNpc.dwMasterSerial;
		*this >> theCreateNpc.dwRequestSerial;
		*this >> theCreateNpc.dwOwnerSerial;
		*this >> theCreateNpc.dwHp;
		*this >> theCreateNpc.dwMp;
		*this >> theCreateNpc.dwMaxHp;
		*this >> theCreateNpc.dwMaxMp;
		*this >> theCreateNpc.dwRandomSeed;
		*this >> theCreateNpc.dwLevel;
		*this >> theCreateNpc.dwMiscIndex;

		*this >> theCreateNpc.fPositionX;
		*this >> theCreateNpc.fPositionY;

// 		theCreateNpc.fPositionX = 74.679825f;
// 		theCreateNpc.fPositionY = 44.203293f;
// 		*this << theCreateNpc.fPositionX;
// 		*this << theCreateNpc.fPositionY;

		*this >> theCreateNpc.fDirectionX;
		*this >> theCreateNpc.fDirectionZ;
		*this >> theCreateNpc.fDirectionY;

		*this >> theCreateNpc.fScaleX;
		*this >> theCreateNpc.fScaleZ;
		*this >> theCreateNpc.fScaleY;

		*this >> theCreateNpc.bIsDummy;
		*this >> theCreateNpc.bDropItemWhenDie;
		*this >> theCreateNpc.bGiveExpWhenDie;
		*this >> theCreateNpc.bRequestFromClient;
		*this >> theCreateNpc.bRequestFromPlayer;
		*this >> theCreateNpc.bAttachEffectWhenCreated;
		*this >> theCreateNpc.bIsControllable;
		*this >> theCreateNpc.bIsStageObject;
		*this >> theCreateNpc.bIsStageFailObject;
		*this >> theCreateNpc.bUninfluencedByQuest;
		*this >> theCreateNpc.bIsIntrusionNpcDropItem;
		*this >> theCreateNpc.bIsCreateByClient;

		if( theCreateNpc.bIsDummy == 1 )
		{
			// 获取验证数据，城镇里面是不用的，且不可以再城镇计算，如果在城镇计算了，则进入副本的时候，需要清0处理
			if( wcscmp(theCreateNpc.strTeamId.c_str(),L"NPC") == 0 )
			{
				__asm
				{
					fld dword ptr[theCreateNpc.fPositionX]
					mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
					call eax
					mov dword ptr[g_dwValidationData_X], eax
					fld dword ptr[theCreateNpc.fPositionY]
					mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
					call eax
					mov dword ptr[g_dwValidationData_Y], eax
				}

				g_bIsDummyExist = 1;
			}

			OutputDebugString("[JFZR] 存在ValidationData！！！！！！！！！！！！！！");
		}
// 		else if( wcscmp(theCreateNpc.strTeamId.c_str(),L"NPC") == 0 )
// 		{
// 			theMonsterObj.SetUnHitNpcId(theCreateNpc.strNpcId.c_str());
// 		}
	
		
		// 包太大，还未完全解析，不过后面的已经不重要了
		return true;

		*this >> theCreateNpc.wStatus;
		*this >> theCreateNpc.wApple_Logic_EncryptedStatusContainer;
		*this >> theCreateNpc.wStringContainer;
		*this >> theCreateNpc.wStringContainerarray;
		*this >> theCreateNpc.dwStringContainersize;	assert(!theCreateNpc.dwStringContainersize);
		for (i=0; i<theCreateNpc.dwStringContainersize; i++)
		{
			*this >> theCreateNpc.theStringContainer[i].wStringContainerLen;
			*this >> theCreateNpc.theStringContainer[i].strStringContainer;
		}
		

		*this >> theCreateNpc.wFloatContainer;
		*this >> theCreateNpc.wFloatContainerarray;
		*this >> theCreateNpc.dwFloatContainersize;
		for (i=0; i<theCreateNpc.dwFloatContainersize; i++)
		{
			*this >> theCreateNpc.theFloatContainer[i].fFloatContainerItem_i;
		}

		*this >> theCreateNpc.wIntContainer;
		*this >> theCreateNpc.wIntContainerarray;
		*this >> theCreateNpc.wIntContainersize;	assert(!theCreateNpc.wIntContainersize);
		for (i=0; i<theCreateNpc.wIntContainersize; i++)
		{
			*this >> theCreateNpc.theIntContainer[i].dwIntContainerItem_i;
		}


		*this >> theCreateNpc.wBoolContainer;
		*this >> theCreateNpc.wBoolContainerarray;
		*this >> theCreateNpc.dwBoolContainersize;	assert(!theCreateNpc.dwBoolContainersize);
		for (i=0; i<theCreateNpc.dwBoolContainersize; i++)
		{
			*this >> theCreateNpc.theBoolContainer[i].bBoolContainer;
		}

		*this >> theCreateNpc.wVectorContainer;
		*this >> theCreateNpc.wVectorContainerarray;
		*this >> theCreateNpc.dwVectorContainersize;	assert(!theCreateNpc.dwVectorContainersize);
		for (i=0; i<theCreateNpc.dwVectorContainersize; i++)
		{
			// 未知
		}

		*this >> theCreateNpc.wKeyDataContainer;
		*this >> theCreateNpc.wKeyDataContainerarray;
		*this >> theCreateNpc.dwKeyDataContainersize;
		for (i=0; i<theCreateNpc.dwKeyDataContainersize; i++)
		{
			*this >> theCreateNpc.theKeyDataContainer[i].wKeyDataContainer_itme_i;
			*this >> theCreateNpc.theKeyDataContainer[i].wApple_Logic_KeyData;
			*this >> theCreateNpc.theKeyDataContainer[i].dwHashKey;
			*this >> theCreateNpc.theKeyDataContainer[i].wStatusTypeLen;
			*this >> theCreateNpc.theKeyDataContainer[i].strStatusType;
			*this >> theCreateNpc.theKeyDataContainer[i].dwIndex;
		}


		*this >> theCreateNpc.wBuffs;
		*this >> theCreateNpc.wApple_BuffContainer;
		*this >> theCreateNpc.wList;
		*this >> theCreateNpc.wListarray;
		*this >> theCreateNpc.dwListsize;
		for (i=0; i<theCreateNpc.dwListsize; i++)
		{
			*this >> theCreateNpc.theList[i].wListItem_i;
			*this >> theCreateNpc.theList[i].wApple_BuffInstance;
			*this >> theCreateNpc.theList[i].wBuffIdLen;
			*this >> theCreateNpc.theList[i].strBuffId;

		}
		*this >> theCreateNpc.dwLevel;
		*this >> theCreateNpc.dwSerial;
		*this >> theCreateNpc.dwCasterSerial;
		*this >> theCreateNpc.dwCasterOwnerSerial;
		*this >> theCreateNpc.dwTargetSerial;
		*this >> theCreateNpc.wEndTime;

		*this >> theCreateNpc.theEndTime.wApple_DateTime;
		*this >> theCreateNpc.theEndTime.wYear;
		*this >> theCreateNpc.theEndTime.wMonth;
		*this >> theCreateNpc.theEndTime.wDay;
		*this >> theCreateNpc.theEndTime.dwHour;
		*this >> theCreateNpc.theEndTime.dwMinute;
		*this >> theCreateNpc.theEndTime.dwSecond;
		*this >> theCreateNpc.theEndTime.dwMilliSec;
		*this >> theCreateNpc.theEndTime.IsInfinity;

		*this >> theCreateNpc.fDuration;
		*this >> theCreateNpc.llOidBuff;
		*this >> theCreateNpc.dwRandomSeed;
		*this >> theCreateNpc.dwNextSerial;
	
		assert(theCreateNpc.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 创建副本内可打的NPC
bool UnPacket::UnPacket_CreatePopInfoNpc(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_CreatePopInfoNpc*********************");
	try
	{
		RECV_CREATE_POP_INFO_NPC	theCreatePopInfoNpc;

		*this = (pszRecvPacket);
		*this >> theCreatePopInfoNpc.theHeader.wPacketLen;
		*this >> theCreatePopInfoNpc.theHeader.wMethodType;
		*this >> theCreatePopInfoNpc.theHeader.wUrlType;
		*this >> theCreatePopInfoNpc.dwIndex;
		*this >> theCreatePopInfoNpc.dwSerial;
		*this >> theCreatePopInfoNpc.dwStageMasterSerial;
		*this >> theCreatePopInfoNpc.dwRandomSeed;
		*this >> theCreatePopInfoNpc.dwMiscIndex;
		*this >> theCreatePopInfoNpc.wBuffs;
		*this >> theCreatePopInfoNpc.wApple_BuffContainer;
		*this >> theCreatePopInfoNpc.wList;
		*this >> theCreatePopInfoNpc.wListarray;
		*this >> theCreatePopInfoNpc.dwListsize;

		for (DWORD i=0; i<theCreatePopInfoNpc.dwListsize; i++)
		{
			*this >> theCreatePopInfoNpc.theList.wListitem_i;
			*this >> theCreatePopInfoNpc.theList.wApple_BuffInstance;
			*this >> theCreatePopInfoNpc.theList.wBuffIdLen;
			*this >> theCreatePopInfoNpc.theList.strBuffId;
		}
		*this >> theCreatePopInfoNpc.dwLevel;
		*this >> theCreatePopInfoNpc.dwSerial2;
		*this >> theCreatePopInfoNpc.dwCasterSerial;
		*this >> theCreatePopInfoNpc.dwCasterOwnerSerial;
		*this >> theCreatePopInfoNpc.dwTargetSerial;
		*this >> theCreatePopInfoNpc.wEndTime;
		*this >> theCreatePopInfoNpc.theEndTime.wApple_DateTime;
		*this >> theCreatePopInfoNpc.theEndTime.wYear;
		*this >> theCreatePopInfoNpc.theEndTime.wMonth;
		*this >> theCreatePopInfoNpc.theEndTime.wDay;
		*this >> theCreatePopInfoNpc.theEndTime.dwHour;
		*this >> theCreatePopInfoNpc.theEndTime.dwMinute;
		*this >> theCreatePopInfoNpc.theEndTime.dwSecond;
		*this >> theCreatePopInfoNpc.theEndTime.dwMilliSec;
		*this >> theCreatePopInfoNpc.theEndTime.IsInfinity;
		*this >> theCreatePopInfoNpc.fDuration;
		*this >> theCreatePopInfoNpc.llOidBuff;
		*this >> theCreatePopInfoNpc.dwRandomSeed2;
		*this >> theCreatePopInfoNpc.dwNextSerial;


		assert(theCreatePopInfoNpc.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 创建副本内可打的NPC
bool UnPacket::UnPacket_AcceptQuestResult(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_AcceptQuestResult*********************");
	try
	{
		ACCEPT_QUEST_RESULT	theAcceptQuestResult;

		*this = (pszRecvPacket);
		*this >> theAcceptQuestResult.theHeader.wPacketLen;
		*this >> theAcceptQuestResult.theHeader.wMethodType;
		*this >> theAcceptQuestResult.theHeader.wUrlType;
		*this >> theAcceptQuestResult.dwResult;
		*this >> theAcceptQuestResult.wQuestIdLen;
		*this >> theAcceptQuestResult.strQuestId;
		*this >> theAcceptQuestResult.wBlockExpiration;
		*this >> theAcceptQuestResult.theAppleDataTime.wApple_DateTime;
		*this >> theAcceptQuestResult.theAppleDataTime.wYear;
		*this >> theAcceptQuestResult.theAppleDataTime.wMonth;
		*this >> theAcceptQuestResult.theAppleDataTime.wDay;
		*this >> theAcceptQuestResult.theAppleDataTime.dwHour;
		*this >> theAcceptQuestResult.theAppleDataTime.dwMinute;
		*this >> theAcceptQuestResult.theAppleDataTime.dwSecond;
		*this >> theAcceptQuestResult.theAppleDataTime.dwMilliSec;
		*this >> theAcceptQuestResult.theAppleDataTime.IsInfinity;


		assert(theAcceptQuestResult.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}



bool UnPacket::UnPacket_PlayerDead(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_PlayerDead*********************");
	try
	{
		PLAYER_DEAD	thePlayerDead;

		*this = (pszRecvPacket);
		*this >> thePlayerDead.theHeader.wPacketLen;
		*this >> thePlayerDead.theHeader.wMethodType;
		*this >> thePlayerDead.theHeader.wUrlType;
		*this >> thePlayerDead.dwSerial;
	
		assert(thePlayerDead.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}

// 城镇过图返回值
bool UnPacket::UnPacket_ChangeVillage(const BYTE* pszRecvPacket)
{
	DEBUG_PRINT("*********************Recv_UnPacket_ChangeVillage*********************");
	try
	{
		ACCEPT_CHANGE_VILLAGE	theChangeVillage;

		*this = (pszRecvPacket);
		*this >> theChangeVillage.theHeader.wPacketLen;
		*this >> theChangeVillage.theHeader.wMethodType;
		*this >> theChangeVillage.theHeader.wUrlType;
		*this >> theChangeVillage.dwResult;
		*this >> theChangeVillage.wVillageIdLen;
		*this >> theChangeVillage.strVillageId;
		*this >> theChangeVillage.wSectorIdLen;
		*this >> theChangeVillage.strSectorId;
		*this >> theChangeVillage.dwTimeStamp;
		*this >> theChangeVillage.bIsMaxLevelExpendTutorialPlay;
		*this >> theChangeVillage.wBlockExpiration;
		*this >> theChangeVillage.theAppleDataTime.wApple_DateTime;
		*this >> theChangeVillage.theAppleDataTime.wYear;
		*this >> theChangeVillage.theAppleDataTime.wMonth;
		*this >> theChangeVillage.theAppleDataTime.wDay;
		*this >> theChangeVillage.theAppleDataTime.dwHour;
		*this >> theChangeVillage.theAppleDataTime.dwMinute;
		*this >> theChangeVillage.theAppleDataTime.dwSecond;
		*this >> theChangeVillage.theAppleDataTime.dwMilliSec;
		*this >> theChangeVillage.theAppleDataTime.IsInfinity;


		assert(theChangeVillage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	}
	catch (...)
	{
		assert(false);
	}
	return true;
}