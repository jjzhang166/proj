#pragma once
/*
@author zhp
@date 2016/4/11 21:40
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include <Windows.h>
#include "CD_StructsBase.h"

#pragma pack(push)
#pragma pack(1)
#define CD_ZERO_MSG(t) t(){ std::memset(this, 0, sizeof(*this)); }
//////////////////////////////////////////////////////////////////////////
//MMO_SAY_MESSAGE
struct stCD_Msg_SayMsg : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_SayMsg)
	enum{
		kMsgId = 0x101	//MMOCLIENT_REVERSAL_PACKET_SAY_METHOD___
	};
	enum enSayType{
		kNormal = 1,
		kTeam = 3,	//队伍
	};
	template<typename T>
	void serialize(T& stm){
		BC_OriStrArray<decltype(msg_buffer_)&> tmp = msg_buffer_;
		stm & say_type_ & tmp;
		BC_OriCont<decltype(array_)&> ar_tmp = array_;
		stm & array_size_ & ar_tmp;
	}

	enSayType		say_type_;
	char			msg_buffer_[0x195 - 4];
	int				array_size_;
	char			array_[0xa];
};
static_assert(offsetof(stCD_Msg_SayMsg, array_size_) == 0x195, "");

struct stCD_Msg_MoveVillage : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_MoveVillage)
	enum{
		kMsgId = 0x00000042	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_CITY_GUO_TU___
	};
	template<typename T>
	void serialize(T& stm){
		BC_OriStrArray<decltype(village_name_)&> tmp = village_name_;
		stm & tmp;
	}

	char	village_name_[50];
};

//MMO_PACKET_GOTO_DOOR
struct stCD_Msg_MoveRoom : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_MoveRoom)
	enum{
		kMsgId = 0x0000025B	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GUO_TU___
	};
	template<typename T>
	void serialize(T& stm){
		stm & theDoorCoord & BC_OriStrArray<decltype(szDoorCmd)&>(szDoorCmd);
	}

	stCD_Vector3	theDoorCoord;		// 门的坐标
	char			szDoorCmd[0x10b + 1];
};

//MMO_REQ_ITEM_BY_SEND_STRUCT
struct stCD_Msg_ReqItem : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_ReqItem)
	enum{
		kMsgId = 0x00001507	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_REQ_ITEM_BY_TYPE___
	};
	template<typename T>
	void serialize(T& stm){
		BC_OriStrArray<decltype(szSearchName)&> tmp = szSearchName;
		stm & nReqCount & bTabIndex & bTypeIndex & bSlotIndex & wUnKnow1 & wGuDingValue1 & wGuDingValue2 & byUnKonw;
		stm & tmp & bItemIdCount & dwItemId[0];
	}

	int		nReqCount;		// 累加请求
	byte	bTabIndex;
	byte	bTypeIndex;
	byte	bSlotIndex;
	WORD	wUnKnow1;
	WORD	wGuDingValue1;	// 000003E7
	WORD	wGuDingValue2;	// 00000009
	byte    byUnKonw;
	char	szSearchName[0x20];		// 搜索拍卖关键字
	byte	bItemIdCount;		// 搜索的物品id数量
	DWORD	dwItemId[1];
};

struct stCD_Msg_SendMail : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_SendMail)
	enum{
		kMsgId = 0x00001B09
	};
	template<typename T>
	void serialize(T& stm){
		stm & dwUnKnow1 & BC_OriStrArray<decltype(szMailActorName)&>(szMailActorName);
		stm & BC_OriStrArray<decltype(szMailTitle)&>(szMailTitle)
			& BC_OriStrArray<decltype(szMailData)&>(szMailData)
			& nGold & items_ & ukn_;
	}
	struct stItemInfo{
		template<typename T>
		void serialize(T& stm){
			stm & bTableIndex
				& wSlotIndex
				& llItemSerial
				& nItemCount
				& wUnKnow;
		}

		pt_byte	bTableIndex;		// 背包=0 仓库=2	
		pt_word	wSlotIndex;			// 物品所在格子序号
		pt_dint llItemSerial;
		pt_word	nItemCount;			// 物品数量
		int		wUnKnow;			//==0
	};		// 附件的数量固定在5
	static_assert(sizeof(stItemInfo) == 0x11, "");

	DWORD		dwUnKnow1;				//==0
	char		szMailActorName[0x25 - 4];	// 收件人名字
	char		szMailTitle[0x65 - 0x25];		// 邮件标题
	char		szMailData[0x2bd - 0x65];		// 邮件内容
	int			nGold;					// 邮寄金币
	stCD_MsgArray<stItemInfo, int, 2>		items_;
	p_mem_placeholder(0x3f7 - sizeof(stCD_MsgArray<stItemInfo, int, 2>) - 0x2c1);
	pt_byte		ukn_;		//==0
};
static_assert(offsetof(stCD_Msg_SendMail, szMailTitle) == 0x25, "");
static_assert(offsetof(stCD_Msg_SendMail, szMailData) == 0x65, "");
static_assert(offsetof(stCD_Msg_SendMail, nGold) == 0x2bd, "");
static_assert(offsetof(stCD_Msg_SendMail, items_) == 0x2c1, "");
static_assert(offsetof(stCD_Msg_SendMail, ukn_) == 0x3f7, "");

struct stCD_Msg_AddFriend : stCD_CheckMsgAtRuntime{
	CD_ZERO_MSG(stCD_Msg_AddFriend)
	enum{
		kMsgId = 0x00001601		//MMOCLIENT_REVERSAL_PACKET_SYN_ADD_FRIENT_NAME___
	};
	template<typename T>
	void serialize(T& stm){
		BC_OriStrArray<decltype(name_)&> tmp = name_;
		stm & tmp;
	}

	char	name_[0x15];
};

//////////////////////////////////////////////////////////////////////////
//////////////////////*******以下继承自stCD_CheckMsgAsPOD******************
BC_DEFINE_POD(stCD_Msg_CreateRole) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_CreateRole)
	enum{
		kMsgId = 0x00000003
	};
	pt_byte		ukn_[16 * 4 + 2];
};

//MMO_WALK
BC_DEFINE_POD(stCD_Msg_Walk) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_Walk)
	enum{
		kMsgId = 0x00000286	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_WALK___
	};

	pt_dint			llDynamic;
	stCD_Vector3	dst_pos;	//目标坐标
	DWORD			dwUnKnow2;
	DWORD			dwUnKnow3;
	stCD_Direction2	dir;	//移动的方向
};

//MM0_ACCEPT_TASK
BC_DEFINE_POD(stCD_Msg_AcceptTask) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_AcceptTask)
	enum{
		kMsgId = 0xE01	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_ACCEPT_TASK___
	};
	int		task_id_;
	int		nFlag;
};

BC_DEFINE_POD(stCD_Msg_CommitTask) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_CommitTask)
	enum{
		kMsgId = 0xE04	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_COMPLETE_TASK___
	};
	int		task_id_;
};

//MMO_DO_BU_JI_XIANG
BC_DEFINE_POD(stCD_Msg_GainSupplyItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_GainSupplyItem)
	enum{
		kMsgId = 0x00000D01	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_ITEM___
	};
	int		slog_key_;
	int		dwItemId;
	int		dwItemCount;
};

//MMO_GET_MAIL_ITEM
BC_DEFINE_POD(stCD_Msg_GainMailItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_GainMailItem)
	enum{
		kMsgId = 0x00001B07	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_MAIL_ITEM___
	};
	pt_dint		llMailId;
	pt_byte		bFlag;
	int			nUnKnow;		//==-1
	int			nItemTabIndex;
	int			ukn_;	//==2
};

//MMO_ACCEPT_GONG_HUI_TASK
BC_DEFINE_POD(stCD_Msg_AcceptSociatyTask) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_AcceptSociatyTask)
	enum{
		kMsgId = 0x00000035	//MMOCLIENT_SOCIATY_TASK_MSG_ID
	};
	DWORD	dwTaskFlag;
	DWORD	dwTaskFbLevel;
	DWORD	dwTaskFbId;
	DWORD	dwUnKnow;
};

//MMO_WEEK_CREDIT
BC_DEFINE_POD(stCD_Msg_WeekCredit) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_WeekCredit)
	enum{
		kMsgId = 0x00002801	//MMOCLIENT_REVERSAL_GET_WEEK_CREDIT_COUNT___
	};
	CD_En_WeekCreditIndex dwWeekCreditIndex;			// 周狩猎券序号 2
	int	  nWeekCreditCount1;			// 取狩猎券数量
	int	  nHasReceiveWeekCreditCount;	// 已取数量
	int   nWeekCreditCount2;			// 取狩猎券数量
	p_mem_placeholder(0x1a - 4 * 4);

	//准备领取指定的数量
	bool GainCnt(int cnt);
};

BC_DEFINE_POD(stCD_Msg_GiveUpTask) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_GiveUpTask)
	enum{
		kMsgId = 0x00000E03
	};
	int		task_id_;
};

//GO_TO_FB1
BC_DEFINE_POD(stCD_Msg_MoveFb1) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_MoveFb1)
	enum{
		kMsgId = 0x19	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB1___
	};
	DWORD	dwUnknow1;
	DWORD	dwUnKnow2;
	int		dwFbId;			// 副本id
	int		nFbDiffLevel;	// 副本挑战等级
	int		dwFbType;		// 副本类型
};

//GO_TO_FB2
BC_DEFINE_POD(stCD_Msg_MoveFb2) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_MoveFb2)
	enum{
		kMsgId = 0x21	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB2___
	};
	int		dwFbId;			// 副本id
	int		dwFbType;		// 副本类型
	int		ukn_zero_;
};

BC_DEFINE_POD(stCD_Msg_MoveFb3) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_MoveFb3)
	enum{
		kMsgId = 0x23	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_FB3___
	};
	int		dwFbType;		// 副本类型
};

//GO_TO_FB_CHOOSEMODE
BC_DEFINE_POD(stCD_Msg_ChooseFbMode) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ChooseFbMode)
	enum{
		kMsgId = 0X2F	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GOTO_CHOOSEMODE___
	};
	int		fb_id_;		// 副本id
	int		dwMode;		//0是狩猎券，1是银币
};

//MMO_ITEM_MOVE_TO
BC_DEFINE_POD(stCD_Msg_MoveItemTo) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_MoveItemTo)
	enum{
		kMsgId = 0x000003004	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_TO___
	};
	pt_dint	item_serial_;
	pt_byte	bScrItemTabIndex;	// 被移动的物品在哪个栏
	WORD	wScrItemSlotIndex;	// 被移动的物品在第几格
	pt_byte	bDstItemTabIndex;	// 移动到物品在哪个栏
	WORD	wDstItemSlotIndex;	// 移动到物品在第几格
};

struct stCD_Msg_MoveItemBase : stCD_CheckMsgAsPOD{
	pt_dint	item_serial_;		// 被移动的物品key
	pt_byte	bScrItemTabIndex;	// 被移动的物品在哪个栏
	WORD	wScrItemSlotIndex;	// 被移动的物品在第几格
	WORD	wMoveItmeCount;		// 移动物品的数量
	pt_byte	bDstItemTabIndex;	// 移动到物品在哪个栏
	WORD	wDstItemSlotIndex;	// 移动到物品在第几格
};
//MMO_ITEM_MOVE_TO_COUNT
BC_DEFINE_POD(stCD_Msg_MoveItemCountTo) : stCD_Msg_MoveItemBase{
	CD_ZERO_MSG(stCD_Msg_MoveItemCountTo)
	enum{
		kMsgId = 0x000003008	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_COUNT_TO___
	};
	
	pt_dint to_item_serial_;		// 移动到物品的key
};

//MMO_ITEM_MOVE_TO_COUNT
BC_DEFINE_POD(stCD_Msg_MoveItemCountToEmpty) : stCD_Msg_MoveItemBase{
	enum{
		kMsgId = 0x00000300A	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_MOVE_COUNT_TO_EMPTY___
	};
};

BC_DEFINE_POD(stCD_Msg_MakeFactureItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_MakeFactureItem)
	enum{
		kMsgId = 0x00001001	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_FACTURE___
	};
	int		facture_id_;
	int		tab_idx_;
	int		ukn_;
};

//MMO_SELL_ITEM
BC_DEFINE_POD(stCD_Msg_SellItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_SellItem)
	enum{
		kMsgId = 0x00000F03	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_SELL_ITEM___
	};
	int nItemTabIndex;
	int nItemSlotIndex;
	int nItemCount;
};

//MMO_GET_MAIL_ITEM_INFO
BC_DEFINE_POD(stCD_Msg_GetMailItemInfo) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_GetMailItemInfo)
	enum{
		kMsgId = 0x00001B05	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_GET_MAIL_ITEM_INFO___
	};
	pt_byte		ukn_;
	pt_dint		mail_id_;
};

BC_DEFINE_POD(stCD_Msg_OpenMail) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_OpenMail)
	enum{
		kMsgId = 0x00001B03	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_OPEN_MAIL___
	};
	pt_word	ukn_zero_;
	pt_byte	ukn_zero_1_;
	pt_byte	flag_;		//==0xff
	pt_byte	ukn_zero_2_;
};

//MMO_DELETE_MAIL
BC_DEFINE_POD(stCD_Msg_DeleteMail) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_DeleteMail)
	enum{
		kMsgId = 0x00001B29	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_DELETE_MAIL___
	};
	int		nMailDeleteFlag;
	LONGLONG llMailId;
};

BC_DEFINE_POD(stCD_Msg_DoNpcChat) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_DoNpcChat)
	enum{
		kMsgId = 0x00002402	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_TASK_NPC_CHAT___
	};
	int		ukn_;
	int		task_id_;
};

BC_DEFINE_POD(stCD_Msg_ChooseRole) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ChooseRole)
	enum{
		kMsgId = 0x00000007
	};
	int		role_id_;
};

BC_DEFINE_POD(stCD_Msg_DeleteRole) : stCD_Msg_ChooseRole{
	enum{
		kMsgId = 0x00000005
	};
};

BC_DEFINE_POD(stCD_Msg_ChooseLine) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ChooseLine)
	enum{
		kMsgId = 0x00000051
	};
	pt_word		line_id_;
};

//MMO_BUY_ITEM
BC_DEFINE_POD(stCD_Msg_BuyItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_BuyItem)
	enum{
		kMsgId = 0x00000F01
	};
	DWORD dwBuyItemFlag;
	DWORD dwShopId;
	int	  nCount;
	DWORD dwFFFFFFFF1;
	DWORD dwFFFFFFFF2;
};

BC_DEFINE_POD(stCD_Msg_CraftItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_CraftItem)
	enum{
		kMsgId = 0x00000903
	};
	int		craft_id_;
	int		ukn_zero_;
};

//MMO_SEND_UP_EQUIP_PACKET
BC_DEFINE_POD(stCD_Msg_LvlUpEquip) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_LvlUpEquip)
	enum{
		kMsgId = 0x0000310D		//MMOCLIENT_REVERSAL_PACKET_MSG_ID_UP_EQUIP___
	};
	LONGLONG	llUpEquipSeiral;			// 遍历物品时的物品序列号
	byte		bSoltTableIndex;		// 装备栏=3 背包栏=0 仓库栏=2
	WORD		wSoltIndex;				// 槽的序号，即第几格，从0开始
	DWORD		dwEquipItemId;			// 升级后的物品id
	int			ukn_zero_;
};
static_assert(sizeof(stCD_Msg_LvlUpEquip) == 19, "");

BC_DEFINE_POD(stCD_Msg_OpenNpc) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_OpenNpc)
	enum{
		kMsgId = 0x00001401	//MMOCLIENT_REVERSAL_PACKET_MSG_ID_OPEN_NPC___
	};

	pt_byte	ukn_value_;		//一直=2
	int		ukn_1_;			//一直=1
	int		npc_id_;
	int		npc_type_;
	int		ukn_zero_;
};

BC_DEFINE_POD(stCD_Msg_EnterFarm) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_EnterFarm)
	enum{
		kMsgId = 0x00000033
	};
	int		ukn_;
	pt_dint	farm_id_;
};

BC_DEFINE_POD(stCD_Msg_UpCollect) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_UpCollect)
	enum{
		kMsgId = 0x000002F2
	};
	int		ukn_;
	int		collect_idx_;
};

//MMO_GET_SEARCH_ITEM_LIST_BY_PAGE
BC_DEFINE_POD(stCD_Msg_SearchItemList) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_SearchItemList)
	enum{
		kMsgId = 0x0000150A
	};
	int nTabIndex;		// 选择的哪个选项
	int nPageIndex;		// 第一页 为 8 第二页为 16 依次类推
	int nPageCount;		// 没有变过，猜测为请求数量
};

//MMO_REQ_BUY_ITEM
BC_DEFINE_POD(stCD_Msg_ReqBuyItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ReqBuyItem)
	enum{
		kMsgId = 0x00001505
	};
	DWORD	dwReqItemId;	// 拍卖物品id
	DWORD	dwReqItemKey;	// 收包中和拍卖物品id一起key
	int		nPrice;			// 价钱
	DWORD	dwItemId;		// 物品id
	int		nItemCount;		// 购买数量
};

//点击出售选项卡会触发此消息。
BC_DEFINE_POD(stCD_Msg_RefreshMySellList) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_RefreshMySellList)
	enum{
		kMsgId = 0x00001511
	};
	pt_dint ukn_;	//每次点击购买选项卡，此数值会加1.
};

//MMO_SHANG_JIA_AUCTION_ITEM
BC_DEFINE_POD(stCD_Msg_ShangJiaItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ShangJiaItem)
	enum{
		kMsgId = 0x00001501		//MMOCLIENT_REVERSAL_PACKET_MSG_ID_SHANG_JIA_AUCTION_ITEM___
	};
	byte	bTabIndex;		// 物品栏  背包=0 仓库=2
	WORD	wSlotIndex;		// 格子序号，即第几格
	DWORD	dwItemId;		// 物品id
	WORD	wItemCount;		// 物品数量
	int		nPrice1;		// 起拍价
	int		nPrice2;		// 一口价
	pt_byte	nTimeFalg;		// 拍卖时间 2小时=0 8小时=1 24小时=2
};

//MMO_REQUEST_ENTRUST_LIST
BC_DEFINE_POD(stCD_Msg_QuestEntrustList) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_QuestEntrustList)
	enum{
		kMsgId = 0x0000003D
	};
	DWORD dwFbKeyFlag;			// 公会副本 = 0x0000985E 喵喵副本 = 0x000075C2  千狩副本 = 0x000075B6	
};

BC_DEFINE_POD(stCD_Msg_ZuoQiQiu) : stCD_CheckMsgAsPOD{
	stCD_Msg_ZuoQiQiu(){
		ukn1_ = 0;
		ukn2_ = 1;
		ukn3_ = 1;
		ukn4_ = 1;
	}
	enum{
		kMsgId = 0x00002401
	};
	int		ukn1_;
	int		ukn2_;
	int		ukn3_;
	int		ukn4_;
};

BC_DEFINE_POD(stCD_Msg_GiveUpFb) : stCD_CheckMsgAsPOD{
	stCD_Msg_GiveUpFb(){
		ukn_ = 1;
	}
	enum{
		kMsgId = 0x0000000F
	};
	int	ukn_;
	int	ukn_1_[2];
};

BC_DEFINE_POD(stCD_Msg_FarmSeed) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_FarmSeed)
	enum{
		kMsgId = 0x000002FB
	};
	int		seed_id_;
	int		farm_idx_;
};

BC_DEFINE_POD(stCD_Msg_GetGiftBag) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_GetGiftBag)
	enum{
		kMsgId = 0x00002601
	};
	int		gift_bag_id_;
};

BC_DEFINE_POD(stCD_Msg_XinShouXunLian) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_XinShouXunLian)
	enum{
		kMsgId = 0x000000A2		//MMOCLIENT_REVERSAL_XIN_SHOU_XUN_LIAN_LI_KAI_JIAO_XUE___   MMOCLIENT_REVERSAL_JIN_RU_JIAO_XUE_FB___
	};
	int		ukn_1_;
	int		ukn_2_;
	int		ukn_3_[3];
	p_mem_placeholder(25 - 20);
};
static_assert(sizeof(stCD_Msg_XinShouXunLian) == 25, "");

//SYN_COOR_DINATES
BC_DEFINE_POD(stCD_Msg_SYN_COOR_DINATES) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_SYN_COOR_DINATES)
	enum{
		kMsgId = 0x00000292		//MMOCLIENT_REVERSAL_PACKET_SYN_COOR_DINATES___
	};
	LONGLONG llSysRandom;
	stCD_Vector3	ptheAcrot;
	DWORD	dwZeor1;
	DWORD	dwZeor2;
	stCD_Direction2	theDir1;
	stCD_Direction2	theDir2;
};

BC_DEFINE_POD(stCD_Msg_RequestDeal) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_RequestDeal)
	enum{
		kMsgId = 0x00001102
	};
	int		dst_role_id_;
};

BC_DEFINE_POD(stCD_Msg_AcceptDeal) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_AcceptDeal)
	enum{
		kMsgId = 0x00001902
	};
	int		deal_key_;
	bool	no_accept_;		//true表示取消，false表示接受
	int		ukn1_;			//==0
};

//MMO_DEAL_MOVE_ITEM
BC_DEFINE_POD(stCD_Msg_DealMoveItem) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_DealMoveItem)
	enum{
		kMsgId = 0x00001106
	};
	WORD	wScrItemTabIndex;	// 背包=0 仓库素材=0x0A 普通仓库=2
	WORD	wScrItemSlotIndex;	// 背包或仓库栏格子，从0开始计数
	WORD	wMoveItemCount;		// 移动物品的数量
	WORD	wDealSlotaIndex;	// 交易栏的格子，从0开始计数
};

BC_DEFINE_POD(stCD_Msg_LockDeal) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_LockDeal)
	enum{
		kMsgId = 0x00001110
	};
	pt_byte		lock_deal_idx_;
};

//确认交易
BC_DEFINE_POD(stCD_Msg_EnterDeal) : stCD_Msg_LockDeal{
};
static_assert(sizeof(stCD_Msg_EnterDeal) == 1, "");
static_assert(std::is_base_of<stCD_CheckMsgAsPOD, stCD_Msg_EnterDeal>::value == true, "");

BC_DEFINE_POD(stCD_Msg_DealGold) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_DealGold)
	enum{
		kMsgId = 0x0000110C
	};
	int		gold_;
};
static_assert(sizeof(stCD_Msg_DealGold) == 4, "");

BC_DEFINE_POD(stCD_Msg_UnLockKingFb) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_UnLockKingFb)
	enum{
		kMsgId = 0x00005115
	};
	int		king_fb_id_;
};
static_assert(sizeof(stCD_Msg_UnLockKingFb) == 4, "");

//章节奖励
BC_DEFINE_POD(stCD_Msg_ChapterReward) : stCD_CheckMsgAsPOD{
	CD_ZERO_MSG(stCD_Msg_ChapterReward)
	enum{
		kMsgId = 0x00005128
	};
	pt_byte		ukn_;			//==1
	int			chapter_idx_;	//
};
static_assert(sizeof(stCD_Msg_ChapterReward) == 5, "");
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
