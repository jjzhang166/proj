#pragma once
#include "../gwlrbase.h"
#include <string>

#pragma pack(push)
#pragma pack (1)

typedef struct
{
	int		nSayType;
	char	pszSayBuf[MMOCLIENT_REVERSAL_PACKET_BUF_SIZE___*4];
}MMO_SAY_MESSAGE,*PMMO_SAY_MESSAGE;

//三维向量，可以用来表示坐标等
struct MMO_Vector3{
	float	fx;
	float	fy;
	float	fz;
};
typedef MMO_Vector3 MMO_Direction3;

//二维向量，可以用来表示方向等
//dir是direction即方向的缩写
struct MMO_Direction2{
	float	dir_x;
	float	dir_y;
};

// 遍历到的角色坐标的结构体
struct MMO_ActorVector
{
	MMO_Direction3  dir_1;
	float			fx;
	MMO_Direction3  dir_2;
	float			fy;
	MMO_Direction3	dir_3;
	float			fz;
};


typedef struct  
{
	DWORD	str_dwUnKnow;
	DWORD	str_dwNextAddrParent;
	DWORD	str_dwNextAddrLeft;
	DWORD	str_dwNextAddrRight;
}NODE,*PNODE;

typedef struct
{
	NODE	theNode;
	DWORD	dwObjId;
	DWORD	dwObjAddr;
}MMO_ACTOR,*PMMO_ACTOR;

typedef	struct
{
	DWORD				str_dwBeginAddr;
	DWORD				str_dwEndAddr1;
	DWORD				str_dwEndAddr2;
	DWORD				str_dwCommandAddr;
}GAME_STRUCT_ARRAY_POINT,*PGAME_STRUCT_ARRAY_POINT;

typedef	struct
{
	DWORD				str_dwBeginAddr;
	DWORD				str_dwEndAddr1;
	DWORD				str_dwEndAddr2;
}GAME_STRUCT_ARRAY_POINT2,*PGAME_STRUCT_ARRAY_POINT2;

typedef struct
{
	DWORD	dwVFunAddr;
	DWORD	dwTaskType;
	DWORD	dwZero1;
	DWORD	dwZero2;
	DWORD	dwUnKnow1;
	GAME_STRUCT_ARRAY_POINT	theTaskArray;
	DWORD	dwZero3;
}TASK_FB_INFO,*PTASK_FB_INFO;


typedef struct  
{
	DWORD	dwUnKnow1;
	int		nZhangIndex;	// 章节序号
	char*	pszZhangStr;	// 章节字符串
	DWORD	dwTaskFbId;		// 任务副本id
	int		nZhangTaskIndex;	// 章节内任务的序号
	GAME_STRUCT_ARRAY_POINT2	theZhangTaskJieSuoArray;	// 任务解锁副本对应关系
	int		x;
	int		y;
	DWORD	dwCityId;		// 任务副本所在城镇id
	int		nShowFlag;		// 是否显示的标志
	char	szEnterInstance[0x14];
	char*	pszEnterInstance;
	char	szLevelType[0x14];
	char*	pszLevelType;
}ZHANG_TASK_FB_INFO,*PZHANG_TASK_FB_INFO;


typedef struct
{
	DWORD dwUnKnow1;
	DWORD dwUnKnow2;
	DWORD dwKingQuestId;
	DWORD dwUnKnow3;
	DWORD dwUnKnow4;
	DWORD dwCityId;
	char* pszFbTitle;
	char  szBuffer1[4*5];
	char* pszBuffer1;
	char  szBuffset2[4*5];
	char* pszBuffer2;
}KING_QUEST_INFO,*PKING_QUEST_INFO;	// 王立副本遍历


//////////////////////////////////////////////////////////////////////////
// 发包结构体
typedef struct		// 封包头
{
	WORD	wPacketMsgId;
	WORD	wPacketMsgType;
	DWORD	dwPacketMsgLen;
	DWORD	dwPacketZero1;
	DWORD	dwPacketZero2;
}MMO_PACKET_HEADER,*PMMO_PACKET_HEADER;

typedef struct
{
	LONGLONG	llDynamic;
	MMO_Vector3	dst_pos;	//目标坐标
	DWORD	dwUnKnow2;
	DWORD	dwUnKnow3;
	MMO_Direction2	dir;	//移动的方向
}MMO_WALK,*PMMO_WALK;

typedef struct
{
	MMO_Vector3		theDoorCoord;		// 门的坐标
	char			szDoorCmd[64];
}MMO_PACKET_GOTO_DOOR,*PMMO_PACKET_GOTO_DOOR;

typedef struct
{
	DWORD	dwUnknow1;
	DWORD	dwUnKnow2;
	DWORD	dwFbId;		// 副本id
	int		nFbDiffLevel;	// 副本挑战等级
	DWORD	dwFbType;	// 副本类型
}GO_TO_FB1,*PGO_TO_FB1;


typedef struct
{
	DWORD	dwFbId;		// 副本id
	DWORD	dwMode;
}GO_TO_FB_CHOOSEMODE,*PGO_TO_FB_CHOOSEMODE;



typedef struct  
{
	DWORD	dwKey1;
	DWORD	dwKey2;
	byte	bScrItemTabIndex;	// 被移动的物品在哪个栏
	WORD	wScrItemSlotIndex;	// 被移动的物品在第几格
	byte	bDstItemTabIndex;	// 移动到物品在哪个栏
	WORD	wDstItemSlotIndex;	// 移动到物品在第几格
}MMO_ITEM_MOVE_TO,*PMMO_ITEM_MOVE_TO;

typedef struct  
{
	DWORD	dwMoveItemKey1;		// 被移动的物品key
	DWORD	dwMoveItemKey2;
	byte	bScrItemTabIndex;	// 被移动的物品在哪个栏
	WORD	wScrItemSlotIndex;	// 被移动的物品在第几格
	WORD	wMoveItmeCount;		// 移动物品的数量
	byte	bDstItemTabIndex;	// 移动到物品在哪个栏
	WORD	wDstItemSlotIndex;	// 移动到物品在第几格
	DWORD	dwToItemKey1;		// 移动到物品的key
	DWORD	dwToItemKey2;
}MMO_ITEM_MOVE_TO_COUNT,*PMMO_ITEM_MOVE_TO_COUNT;

typedef struct
{
	int		nItemSlotIndex;
	DWORD	dwItemId;
	DWORD	dwItemCount;
}MMO_DO_BU_JI_XIANG,*PMMO_DO_BU_JI_XIANG;		// 补给箱


typedef struct  
{
	int nItemTabIndex;
	int nItemSlotIndex;
	int nItemCount;
}MMO_SELL_ITEM,*PMMO_SELL_ITEM;			// 出售物品

typedef struct
{
	LONGLONG	llMailId;
	byte		bFlag;
	int			nUnKnow;
	int			nItemTabIndex;
}MMO_GET_MAIL_ITEM,*PMMO_GET_MAIL_ITEM;

typedef struct
{
	byte		bUnknow;
	LONGLONG	llMailId;
}MMO_GET_MAIL_ITEM_INFO,*PMMO_GET_MAIL_ITEM_INFO;

typedef struct
{
	int		nMailDeleteFlag;
	LONGLONG llMailId;
}MMO_DELETE_MAIL,*PMMO_DELETE_MAIL;

typedef struct
{
	DWORD dwBuyItemFlag;
	DWORD dwShopId;
	int	  nCount;
	DWORD dwFFFFFFFF1;
	DWORD dwFFFFFFFF2;
}MMO_BUY_ITEM,*PMMO_BUY_ITEM;

typedef struct
{
	LONGLONG	llUpEquipSeiral;			// 遍历物品时的物品序列号
	byte		bSoltTableIndex;		// 装备栏=3 背包栏=0 仓库栏=2
	WORD		wSoltIndex;				// 槽的序号，即第几格，从0开始
	DWORD		dwEquipItemId;			// 升级后的物品id
}MMO_SEND_UP_EQUIP_PACKET,*PMMO_SEND_UP_EQUIP_PACKET;

typedef struct
{
	DWORD	dwItemKey1;
	DWORD	dwItemKey2;
	byte	bItemTabIndex;
	WORD	wItemSlotIndex;
	WORD	wItemCount;
}MMO_DISMANTLING_,*PMMO_DISMANTLING_;

typedef struct
{
	int					nDismantlingCount;
	MMO_DISMANTLING_	theDismantling[20];
}MMO_DISMANTLING,*PMMO_DISMANTLING;

typedef struct
{
	DWORD	dwTaskFlag;
	DWORD	dwTaskFbLevel;
	DWORD	dwTaskFbId;
	DWORD	dwUnKnow;
}MMO_ACCEPT_GONG_HUI_TASK,*PMMO_ACCEPT_GONG_HUI_TASK;

typedef struct
{
	int nTabIndex;		// 选择的哪个选项
	int nPageIndex;		// 第一页 为 8 第二页为 16 依次类推
	int nPageCount;		// 没有变过，猜测为请求数量
}MMO_GET_SEARCH_ITEM_LIST_BY_PAGE,*PMMO_GET_SEARCH_ITEM_LIST_BY_PAGE;

typedef struct
{
	DWORD	dwReqItemId;	// 拍卖物品id
	DWORD	dwReqItemKey;	// 收包中和拍卖物品id一起key
	int		nPrice;			// 价钱
	DWORD	dwItemId;		// 物品id
	int		nItemCount;		// 购买数量
}MMO_REQ_BUY_ITEM,*PMMO_REQ_BUY_ITEM;

typedef struct
{
	byte	bTabIndex;		// 物品栏  背包=0 仓库=2
	WORD	wSlotIndex;		// 格子序号，即第几格
	DWORD	dwItemId;		// 物品id
	WORD	wItemCount;		// 物品数量
	int		nPrice1;		// 起拍价
	int		nPrice2;		// 一口价
	int		nTimeFalg;		// 拍卖时间 2小时=0 8小时=1 24小时=2
}MMO_SHANG_JIA_AUCTION_ITEM,*PMMO_SHANG_JIA_AUCTION_ITEM;

typedef struct
{
	DWORD	dwActorPacketId;
	char	szTeamPwd[9];
}MMO_JOIN_TEAM,*PMMO_JOIN_TEAM;


typedef struct
{
	DWORD dwFbKeyFlag;			// 公会副本 = 0x0000985E 喵喵副本 = 0x000075C2  千狩副本 = 0x000075B6	
//	DWORD dwFbTypeIndex;		// 公会副本 = 1 千狩副本 = 4 喵喵副本 = 3
}MMO_REQUEST_ENTRUST_LIST,*PMMO_REQUEST_ENTRUST_LIST;

typedef struct
{
	DWORD		dwUnKnow1;
	char		szMailActorName[0x15];	// 收件人名字
	char		szUnKnow1[0x0C];
	char		szMailTitle[0x19];		// 邮件标题
	char		szUnKnow2[0x27];
	char		szMailData[0x258];		// 邮件内容
	int			nGold;					// 邮寄金币
	int			nAttachmemtCount;		// 附件数量
	struct
	{
		byte	bTableIndex;		// 背包=0 仓库=2	
		WORD	wSlotIndex;			// 物品所在格子序号
		LONGLONG llItemSerial;
		int		nItemCount;			// 物品数量
		WORD	wUnKnow;
	}theAttachmemt[2];		// 附件的数量固定在5
}MMO_SEND_MAIL,*PMMO_SEND_MAIL;	

typedef struct
{
	WORD	wTabSoltIndex;		// 仓库=2 背包=0
	byte	bUnKnow1;			// 为0
	WORD	wSoltCount;			// 有效格子的数量，从0开始
	WORD	wItemCount;			// 物品个数
	struct
	{
		LONGLONG	llkey;
		byte		bUnKnow;
		int			nSloltIndex;
	}theTidyItemInfo[1000];		// 假设仓库最大为1000个
}MMO_TIDY_ITEM,*PMMO_TIDY_ITEM;



typedef struct
{
	DWORD dwWeekCreditIndex;			// 周狩猎券序号 2
	int	  nWeekCreditCount1;			// 取狩猎券数量
	int	  nHasReceiveWeekCreditCount;	// 已取数量
	int   nWeekCreditCount2;			// 取狩猎券数量
}MMO_WEEK_CREDIT,*PMMO_MMO_WEEK_CREDIT;

typedef struct
{
	LONGLONG llSysRandom;
	MMO_Vector3	ptheAcrot;
	DWORD	dwZeor1;
	DWORD	dwZeor2;
	MMO_Direction2	theDir1;
	MMO_Direction2	theDir2;
}SYN_COOR_DINATES,*PSYN_COOR_DINATES;	// 同步坐标封包

typedef struct
{
	WORD	wScrItemTabIndex;	// 背包=0 仓库素材=0x0A 普通仓库=2
	WORD	wScrItemSlotIndex;	// 背包或仓库栏格子，从0开始计数
	WORD	wMoveItemCount;		// 移动物品的数量
	WORD	wDealSlotaIndex;	// 交易栏的格子，从0开始计数
}MMO_DEAL_MOVE_ITEM,*PMMO_DEAL_MOVE_ITEM,MMO_RECV_DEAL_MOVE_ITEM_RET,*PMMO_RECV_DEAL_MOVE_ITEM_RET;


//////////////////////////////////////////////////////////////////////////
// 收包结构体
typedef struct
{
	MMO_PACKET_HEADER	thePacketHeader;
	DWORD				dwTaskId;
	DWORD				dwTaskZero;
}MMO_ACCEPT_TASK,*PMMO_ACCEPT_TASK;			// 接受任务


typedef struct
{
	MMO_PACKET_HEADER	thePacketHeader;
	DWORD				dwTaskId;
}MMO_GIVE_UP_TASK,*PMMO_GIVE_UP_TASK;			// 放弃任务

typedef struct
{
	MMO_PACKET_HEADER	thePacketHeader;
	int		nSize;
	DWORD	nRetValue;			// nRetValue = 0， nRetError = -1 表示提取附件失败
	DWORD	dwMailId1;
	DWORD	dwMailId2;
	int		nRetError;			// nRetValue = 0x29， nRetError = -2 表示提取附件失败:超出携带上限
}MM0_ACCEPT_MAIL_RET_VALUE,*PMM0_ACCEPT_MAIL_RET_VALUE;

typedef struct
{
	DWORD	dwRolePacketId;
	int		nError;		// 0表示可以正常进入副本  非0表示不可进入副本，如激运副本疲劳状态，公会委托副本已经打完，注意：公会副本可以打3次，但是每个副本只能打一次
	//int		nRet;		// 1表示普通副本返回值和激运副本返回值  2表示公会副本返回值 
}MMO_GAME_IN_FB_RET_PACKET,*PMMO_GAME_IN_FB_RET_PACKET;

typedef struct
{
	DWORD dwUnKnow1;
	DWORD dwUnKnow2;	
	DWORD dwRetValue;		// 测试服务器已满返回值=0x73 放弃副本返回值=0x6A
	DWORD dwRolePacketId;	// 角色对象的id
	char  szRoleName[20];	// 角色名字
}MMO_FB_SERVER_FULL_RET,*PMMO_FB_SERVER_FULL_RET;

typedef struct			// 玩家链表结构
{
	DWORD dwZeor1;
	DWORD dwZero2;
	DWORD dwPlayerId;
	char  szPlayerName[0x20];
	byte  bChosseFlag;
	int	  nPlayerLevel;
	byte  bszUnKnow[0x501 - 0x31];
}MM0_PLAYER_LIST,*PMM0_PLAYER_LIST;


typedef struct
{
	int nRet;
	int nRetParamer;
}MMO_RECV_DELETE_PLAYER,*PMMO_RECV_DELETE_PLAYER;


typedef struct
{
	WORD wLineIndex;
	byte bLineLevel;
	byte bUnKnow;
}MMO_GAME_LINE,*PMMO_GAME_LINE;

typedef struct
{
	DWORD	dwReqItemById;		// 拍卖的物品id，非物品id，需要查表确认物品id
	DWORD	dwReqItemByKey;		// 操作拍卖物品需要用到的key值
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	char	szReqItemByPlayerName[0x20];
	byte	bFlag;
	DWORD	dwUnKnow3;
	int		nReqItemByPrice1;		// 起拍价
	int		nReqItemByPrice2;		// 一口价
	int		nReqItemByPrice3;		// 竞拍价
	byte	bUnKnowCode[0x0AE];
}MM0_RECV_REQ_ITEM_BY_TYPE,*PMM0_RECV_REQ_ITEM_BY_TYPE;

typedef struct
{
	DWORD	dwRet;		// 返回值
	int		nDoCount;	// 计数
	DWORD	dwUnKnow;	// 不清楚，不知道
	int		nReqItemCount;	// 物品的总数量，页数需要除以8，如果存在余数，需要商+1
	WORD	wCurPageCount;	// 当前页的拍卖物品数量
}MMO_RECV_REQ_ITEM_BY_HEADER,*PMMO_RECV_REQ_ITEM_BY_HEADER;


typedef struct
{
	DWORD	dwRetValue;		// 返回值 = 6，邀请组队被拒绝
	DWORD	dwZeor1;
	DWORD	dwZeor2;
	char	szActorName[0x20];		// 拒绝组队的对象名字
}MMO_INVITE_TEAM_FAIL,*PMMO_INVITE_TEAM_FAIL;

typedef struct
{
	DWORD	dwActorPacketId;
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	char	szTeamMemberName[0x20];
	int		nMemberLevel;
	byte	bCode[0x3c];
}MMO_TEAM_MEMBER_INFO,*PMMO_TEAM_MEMBER_INFO;

typedef struct
{
	DWORD	dwTeamId;
	char	szTeamTitle[0x28];		// 标题
	int		nMaxMemberCount;		// 队伍最大成员数量
	DWORD	dwZeor1;
	WORD	wFlag;
	char	szTeamPwd[0x20];		// 队伍密码
	byte	bUnKnowCode[0x3E];
	int		nMemberCount;			// 当前队伍成员数量
	MMO_TEAM_MEMBER_INFO	theTeamMemberInfo[4];
}MMO_TEAM_INFO,*PMMO_TEAM_INFO;

typedef struct
{
	DWORD	dwFbType;		// 副本类型  喵喵=0x00000065 千狩=0x000000C9
	WORD	wCount;
	DWORD	dwTaskFbId[0x32];
}MMO_TASK_FB_INFO_,*PMMO_TASK_FB_INFO_;

typedef struct
{
	DWORD	dwRetValue;
	//DWORD	dwTaskFbType;		// 01表示公会副本  04 表示千狩副本  03 表示喵喵副本
	WORD	wCount;
}MMO_TASK_FB_INFO,*PMMO_TASK_FB_INFO;

typedef struct
{
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
	DWORD	dwItemId[50];
}MMO_REQ_ITEM_BY_SEND_STRUCT,*PMMO_REQ_ITEM_BY_SEND_STRUCT;

typedef struct
{
	DWORD dwBuyItemRet;		// 购买成功 = 0
	DWORD dwBuyReqItemId;	// 购买的物品拍卖id
}MMO_RECV_REQ_BUY_ITEM_RET,*PMMO_RECV_REQ_BUY_ITEM_RET;

typedef struct
{
	int		nRet;		// 返回值=1
	DWORD	dwFbId;		// 激运副本id
	DWORD	dwUnKnow;		
	int		nFbCount;	// 剩余激运副本次数
	int		nShuaXinTime;	// 剩余刷新副本时间
}MMO_RECV_JI_YUN_FB,*PMMO_RECV_JI_YUN_FB;

typedef struct
{
	WORD	wRet;		// 打开成功返回值=2
	struct
	{
		char	szUnKnow[3];
		int		nOpenFlag;		
		DWORD	dwObjPacketId;	// 0下标为npc的id，1下标为玩家的id
		DWORD	dwUnKnow2;
		DWORD	dwUnKnow3;
	}theOpenNpcRet[2];
}MMO_RECV_OPEN_NPC_RET,*PMMO_RECV_OPEN_NPC_RET;

typedef struct
{
	DWORD	dwRetValue;		// 上架成功返回值=0
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
}MMO_RECV_SHANG_JIA_ITEM_RET,*PMMO_RECV_SHANG_JIA_ITEM_RET;

typedef struct
{
	int		nRet;			// 成功=0 不可出售=8
	DWORD	dwSellItemId;	// 成功后有出售物品的id
}MMO_RECV_SELL_ITEM_RET,*PMMO_RECV_SELL_ITEM_RET;

typedef struct
{
	int		nRet;		// 背包已满，无法提交任务 = 3，无法接赏金任务的返回值 = 1
	DWORD	dwTaskId;	// 提交任务的id
}MMO_RECV_COMPLETE_TASK_GAIL_RET,*PMMO_RECV_COMPLETE_TASK_GAIL_RET;

typedef struct
{
	DWORD	dwActorPacketId;
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	DWORD	dwUnKnow3;
	DWORD	dwUnKnow4;
	DWORD	dwPoisoningFeebHpFlag;	// 中毒持续减血标志 如果为0则表示炎热天气或者寒冷天气导致的持续减血  如果不为0，则表示中毒
}MMO_RECV_FEEB_HP_RET,*PMMO_RECV_FEEB_HP_RET;


typedef struct
{
	byte	bRet;			// 返回值=1
	WORD	wUnKnow;
	DWORD	dwItemId;		// 物品id
	byte	bUnKnow;
	WORD	wSlotIndex;		// 物品所在格子
}MMO_RECV_USE_ITEM_RET,*PMMO_RECV_USE_ITEM_RET;

typedef struct
{
	LONGLONG	llItemKey;			// 遍历物品时的key1和key2
	byte		bUseItemTableIndex;	// 背包=0
	WORD		wItemSlotIndex;		// 物品所在槽
}MMO_RECV_USE_ITEM_RET2,*PMMO_RECV_USE_ITEM_RET2;

typedef struct
{
	int		nRet;			// 成功返回0 购买数量已达上限 0x20
	DWORD	dwUnKnow;
	DWORD	dwShopItemId;	// 商品id，非物品id
	int		nCount;			// 购买数量
}MMO_RECV_BUY_ITEM_RET,*PMMO_RECV_BUY_ITEM_RET;

typedef struct
{
	int		nRet;			// 返回值为1，其余不用管
	DWORD	dwCollectObjPacketId;	// 当前被采集的对象id
	DWORD	dwUnKnow1;
	DWORD	dwUNKnow2;
	DWORD	dwCollectItemId;		// 采集得到的物品id
	DWORD	dwUnKnow3;
}MMO_RECV_COLLECT_ITEM_BAG_FULL_RET,*PMMO_RECV_COLLECT_ITEM_BAG_FULL_RET;

typedef struct
{
	DWORD dwMoveDealGoldRoleId;
	int	  nMoveDealGoldValue;
}MMO_RECV_MOVE_DEAL_GOLD_RET,*PMMO_RECV_MOVE_DEAL_GOLD_RET;

typedef struct
{
	DWORD dwDoDealRoleId;		// 操作交易的对象id
	byte  bDoDealIndex;			// 1 表示取消锁定 2 表示操作的锁定交易 3表示操作的确认交易
}MMO_RECV_DEAL_STATUS_RET,*PMMO_RECV_DEAL_STATUS_RET;

typedef struct
{
	DWORD dwDealFailRet;		// 0x0029001E 物品超出携带上限  0x0029000B 对方已经在交易中
	DWORD dwDealRoleId;			// 交易对象的id
}MMO_RECV_DEAL_FAIL_RET,*PMMO_RECV_DEAL_FAIL_RET;

#define MMOCLIENT_REVERSAL_SEND_MAIL_ANQUAN_TIME_RET___				0x00046CE4		// 安全时间
#define MMOCLIENT_REVERSAL_SEND_MAIL_ROLE_NAME_NOT_FOUND_RET___		0x00046CD4		// 角色名不存在
#define MMOCLIENT_REVERSAL_SEND_MAIL_PIN_FAN_RET___					0x00046CE2		// 发送邮件频繁
typedef struct
{
	int		nRet;		// 返回值测试时一直为1
	DWORD	dwErrorValue;	// 封包错误值
}MMO_RECV_SEND_MAIL_RET2,*PMMO_RECV_SEND_MAIL_RET2;





typedef struct
{
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	DWORD	dwRolePacketId;
	DWORD	dwUnKnow3;
	DWORD	dwUnKnow4;
	DWORD	dwUnKnow5;
	MMO_Direction2	theRoleDirection;
	MMO_Vector3		theRoleVector;
}GUO_TU_RET,*PGUO_TU_RET;

typedef struct
{
	DWORD dwRequestDealRoleId;		// 请求交易的对象id		注意：谁请求，就谁在前，这个包请求和接受通用的
	DWORD dwByRequestDealRoleId;	// 同意交易的对象id
}MMO_RECV_REQUEST_DEAL_RET,*PMMO_RECV_REQUEST_DEAL_RET;

typedef struct
{
	DWORD dwRequestDealFailValue;		// 返回值
	DWORD dwUnKnow1[3];
	byte  bUnKnow2;
	DWORD dwUnKnow3;
	char  szRoleName[21];
}MMO_RECV_REQUEST_DEAL_FIAL_RET,*PMMO_RECV_REQUEST_DEAL_FIAL_RET;


//////////////////////////////////////////////////////////////////////////
// 遍历物品
typedef struct
{
	DWORD	dwItemAddr;
	DWORD	dwZero1;
	DWORD	dwZero2;
}MM0_ITEM,*PMM0_ITEM;

typedef struct
{
	LONGLONG	llDynamic;		// +0x10
	LONGLONG	llRand;			// +0x18
	MMO_Direction2	dir_zero;	// +0x20
	MMO_Direction2	dir;		// +0x28
	MMO_Vector3		scr_pos;	// +0x30
	WORD			wzero1;		// +0x3c
	MMO_Vector3		dir_unknow1;// +0x3e
	WORD			wflag;		// +0x4a
	DWORD			dwzero1[17];
	WORD	wUnKnow1;			// +0x90
	WORD	wUnKnow2;			// +0x92
	float	fHitTime;			// +0x94 攻击时的一个速度值，砍的慢的动作，小数值会大
	float	fUnKnow2;			// +0x98
	WORD	wUnKnow3;			// +0x9a
	DWORD	dwZeor2[20];
}MMO_HIT,*PMMO_HIT;

typedef struct  
{
	DWORD	dwTaskId;
	int		nFlag;
}MM0_ACCEPT_TASK,*PMM0_ACCEPT_TASK;

typedef struct
{
	NODE	theNode;
	DWORD	dwTaskId;
	DWORD	dwTaskAddr;
}MMO_RECEIVED_TASK_POINT,*PMMO_RECEIVED_TASK_POINT;

struct stFbDoorLink{
	int				template_map_id_;	//模板地图ID
	int				room_id_src_;		//源房间号
	std::string		door_name_src_;		//源房间中的门的名字
	int				room_id_dst_;		//目标房间号
};

typedef struct
{
	DWORD dwMaterialItemId;
	int	  nCount;
}MMO_MATERIAL_ITEM,*PMMO_MATERIAL_ITEM;

typedef struct
{
	DWORD	dwVFunAddr;
	GAME_STRUCT_ARRAY_POINT	theUpEquipMaterialInfo;
	DWORD	dwUnKnow1;
	DWORD	dwUpEquipItemId;
	DWORD	dwUnKnow2[6];
}MMO_UP_EQUIP_ITEM_MATERIAL_INFO,*PMMO_UP_EQUIP_ITEM_MATERIAL_INFO;

typedef struct		// 攻击部位的结构体
{
	byte	bUnKnowFloat[0x20];		// 以怪物的头为前，分左右
	MMO_Vector3		theRangeRight;	// 怪物部位右边
	MMO_Vector3		theRangeLeft;	// 怪物部位左边
	DWORD			dwUnKnow1;
	DWORD			dwUnKnow2;
	int				dwBuWeiId;		// 部位id
	byte	bUnKnowCode[0x34];
}MMO_BU_WEI_OBJ_STRUCT,*PMMO_BU_WEI_OBJ_STRUCT;

typedef struct
{
	DWORD	dwUnKnow1;
	int		nFarmTime;		// 种植时间
	DWORD	dwUnKnow3;
	DWORD	dwCurFarmFacilityLevel;	// 当前采集点等级
	DWORD	dwUnKnow4;
}MMO_CUR_FARM_FACILITY,*PMMO_CUR_FARM_FACILITY;

typedef struct
{
	int		nFarmFlag;		// 草药田是否开启的标志 开启=1，未开启=0
	DWORD	dwSeedId;		// 种植的种子id
	int		nUnKnow;
}MMO_FARM_FACILITY_STATE,*PMMO_FARM_FACILITY_STATE;

typedef struct
{
	DWORD	dwMaterialId;		// 需要的材料id
	DWORD	dwUnKnow[6];
	int		nMaterialCount;		// 需要的材料数量
}MMO_MANU_MATERIAL,*PMMO_MANU_MATERIAL;


typedef struct
{
	int		nSlotIndex;		// 物品所在补给箱第几格从1开始
	DWORD	dwItemId;		// 物品的id
	int		nCount;			// 物品的数量
	/*++
	$ ==>    >00000002
	$+4      >00001771
	$+8      >00000001
	--*/
}MMO_REQUIRE_CUR_SLOT,*PMMO_REQUIRE_CUR_SLOT;

typedef struct  
{
	DWORD dwNextNode;
	DWORD dwPreNode;
	PMMO_REQUIRE_CUR_SLOT	ptheCurSlot;
}MMO_REQUIRE_SLOT,*PMMO_REQUIRE_SLOT;


typedef struct
{
	DWORD	dwVFunAddr;
	GAME_STRUCT_ARRAY_POINT	theRewardItemAddr;		// 奖励物品的遍历结构体
}MMO_TASK_REWARD_ITEM,*PMMO_TASK_REWARD_ITEM;

typedef struct
{
	DWORD	dwVFunAddr;		
	DWORD	dwItemAddr;		// 物品模板的地址
	int		nCount;			// 奖励物品的数量
}MMO_TASK_REWARD_ITEM_INFO,*PMMO_TASK_REWARD_ITEM_INFO;

typedef struct
{
	NODE	theNode;		
	DWORD	dwGiftBagId;	// 礼包的id
	DWORD	dwGiftBagAddr;
}MMO_GIFT_BAG_INFO,*PMMO_GIFT_BAG_INFO;

#pragma pack(pop)
