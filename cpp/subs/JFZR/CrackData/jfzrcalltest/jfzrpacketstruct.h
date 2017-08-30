//////////////////////////////////////////////////////////////////////////
//	封包所使用的结构体定义处
//	by:reversal
//////////////////////////////////////////////////////////////////////////

#ifndef JFZR_PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A
#define JFZR_PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A

#include "jfzrstruct.h"

typedef	struct  
{
	WORD	wPacketLen;
	WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
	WORD	wUrlType;				// 请求实现方法具体类型
}PACKET_HEADER,*PPACKET_HEADER;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD	dwSkillHash;
	WORD	wParam;
	WORD	wassociative_array;
	DWORD	dwsize;
	struct  
	{
		WORD	witem_first_0Len;
		std::wstring	stritem_first_0;
		WORD	witem_second_0Len;
		std::wstring	stritem_second_0;
	}theParam[10];
	WORD	wSelectedSkillTabIndex;
	FLOAT	fMaxDt;
}SEND_ACTION_SYSTEM_USESKILL,*PSEND_ACTION_SYSTEM_USESKILL;

typedef struct  
{
	PACKET_HEADER	theHeader;
	DWORD	dwSerial;
	DWORD	dwActionSerial;
	DWORD	dwActionHash;
	BYTE	bExistParams;
	struct
	{
		WORD	wParams;
		WORD	wassociative_array;
		DWORD	dwsize;
		struct
		{
			WORD	witem_first_0Len;
			std::wstring	stritem_first_0;
			WORD	witem_second_0Len;
			std::wstring	stritem_second_0;
		}theParams[10];
	}theExistParams;
	FLOAT	fActionStartPosition_X;
	FLOAT	fActionStartPosition_Z;
	FLOAT	fActionStartPosition_Y;
	WORD	wDirectionX;
	WORD	wDirectionY;
	WORD	wMovingDirectionX;
	WORD	wMovingDirectionY;
	FLOAT	fClientAbsoluteTime;
	DWORD	dwRandomSeed;
	DWORD	dwPrevActionSerial;
	DWORD	dwPrevActionPlayTime;
	BYTE	bSkipValidation1;
	DWORD	dwSkillId;
	WORD	wSkillLevel;
	BYTE	bSkipValidation2;
}SKILL_ACTION_SYNC_PACKET,*PSKILL_ACTION_SYNC_PACKET;

typedef struct
{
	WORD	wItem_i;
	WORD	wHitInfo;
	DWORD	dwProjectileSerial;
	DWORD	dwDefenderSerial;
	DWORD	dwAttackerSerial;
	WORD	wHitResultLen;
	std::wstring	strHitResult;
	DWORD	dwHitType;
	DWORD	dwDeltaHp;				// 单次攻击伤害值取反+1
	DWORD	dwRandomSeed;
	WORD	wHitProjectileInfo;
	WORD	wAppleHitProjectileInfo;
	FLOAT	fAttackPowerRate;
	FLOAT	fDamagePlusRateToSuperArmor;
	FLOAT	fDamagePlusRateForCounter;
	WORD	wPhysicalAttribute;
	//WORD	wPhysicalAttributeLen;
	//std::wstring 	strPhysicalAttribute;
	WORD	wElementalAttributeLen;
	std::wstring	strElementalAttribute;
	DWORD	dwEventOptionSerial;
	DWORD	dwEventOptionDamageReduceRate;
	BYTE	bIsCounterAttack;
	BYTE	bIsHitSuperArmor;
	DWORD	dwProjectileHash;
	BYTE	bFirstHit;
	DWORD	dwActionHash;
	DWORD	dwSkillHash;
	DWORD	dwSkillLevel;
	FLOAT	fProjectileScalex;
	FLOAT	fProjectileScaley;
	FLOAT	fProjectileScalez;
	DWORD	dwTimeStamp;
}SEND_REQUEST_PROJECTILE_HIT_BODY,*PSEND_REQUEST_PROJECTILE_HIT_BODY;		// RequestProjectileHit

typedef struct  
{
	PACKET_HEADER	theHeader;
	DWORD	dwType;
	DWORD	dwCount;
	DWORD	dwDefenderSerial;
	DWORD	dwAttackerSerial;
}SEND_ACTOR_SYSTEM_ACTIVITY,*PSEND_ACTOR_SYSTEM_ACTIVITY;

typedef struct
{
	PACKET_HEADER		theHeader;
	WORD				wWarpGateIdLen;
	std::wstring		strWarpGateId;
	FLOAT				fAttackSpeed;
	FLOAT				fMoveSpeed;
	FLOAT				fEvadeRating;
	BYTE				bIsDummyExist;
	DWORD				dwValidationData_Y;
	DWORD				dwValidationData_X;
}SEND_CHANGE_SECTOR,*PSEND_CHANGE_SECTOR;

typedef struct
{
	WORD			wApple_DateTime;
	WORD			wYear;
	WORD			wMonth;
	WORD			wDay;
	DWORD			dwHour;
	DWORD			dwMinute;
	DWORD			dwSecond;
	DWORD			dwMilliSec;
	BYTE			IsInfinity;
}APPLE_DATA_TIME,*PAPPLE_DATA_TIME;

typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD	wRequestHitInfos;
	WORD	warray;
	DWORD	dwsize;
}SEND_SKILL_HIT_HEADER,*PSEND_SKILL_HIT_HEADER;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD	dwStageHash;
	DWORD	dwDifficulty;
	BYTE	bIsFirstPlay;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
	BYTE			bIsRestart;
	BYTE			bUseIntrusionItem;
	BYTE			bIsMaxLevelExpendTutorialPlay;
}SEND_STAGE_SYSTEM_START_STAGE,*PSEND_STAGE_SYSTEM_START_STAGE;


typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wPlayerIdLen;
	std::wstring	strPlayerId;
	BYTE			bStepIn;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
}SEND_STEP_IN_WARP_GATE,*PSEND_STEP_IN_WARP_GATE;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSerial;
	XYZ				thePosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
}SEND_DIRECTION_SYNC_PACKET,*PSEND_DIRECTION_SYNC_PACKET;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSerial;
}SEND_PICK_UP_ITEM,*PSEND_PICK_UP_ITEM;

typedef	struct
{
	PACKET_HEADER	theHeader;
	BYTE			bAlone;
	DWORD			dwSumOfDeltaHp;
}SEND_RETURN_VILLAGE,*PSEND_RETURN_VILLAGE;


typedef	struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwStageHash;
	DWORD			dwDifficulty;
	BYTE			bIsFirstPlay;
	DWORD			dwWarpGateId;
	BYTE			bIsRestart;
	BYTE			bUseIntrusionItem;
}SEND_START_STAGE,*PSEND_START_STAGE;


typedef	struct
{
	PACKET_HEADER	theHeader;
	BYTE			bIsEquipmentRepair;
}SEND_REPAIR_ITEM,*PSEND_REPAIR_ITEM;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSerial;
	DWORD			dwActionSerial;
	DWORD			dwActionHash;
	BYTE			bExistParams;
	WORD			wParams;
	WORD			wassociative_array;
	DWORD			dwsize;
	struct
	{
		WORD			witem_first_iLen;
		std::wstring	stritem_first_i;
		WORD			witem_second_iLen;
		std::wstring	stritem_second_i;
	}Item[10];
	XYZ				theActionStartPosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
	FLOAT			fClientAbsoluteTime;
	DWORD			dwRandomSeed;
	DWORD			dwPrevActionSerial;
	FLOAT			fPrevActionPlayTime;
	BYTE			bSkipValidation;
}SEND_ACTION_SYNC_PACKET,*PSEND_ACTION_SYNC_PACKET;



typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSerial;
	DWORD			dwActionSerial;
	DWORD			dwActionHash;
	BYTE			bExistParams;		// 是否存在参数的标志 存在=1 不存在=0
	WORD			wParams;
	WORD			wassociative_array;
	DWORD			dwsize;
	struct
	{
		WORD			witem_first_iLen;
		std::wstring	stritem_first_i;
		WORD			witem_second_iLen;
		std::wstring	stritem_second_i;
	}Item[10];
	XYZ				theActionStartPosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
	FLOAT			fClientAbsoluteTime;
}SEND_VILLAGE_ACTION_SYNC_PACKET,*PSEND_VILLAGE_ACTION_SYNC_PACKET;



typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wEquipTabLen;
	std::wstring	strEquipTab;
	WORD			wEquipSlot;
	WORD			wInventoryTabLen;
	std::wstring	strInventoryTab;
	WORD			wInvenSlot;
}SEND_EQUIP_ITEM,*PSEND_EQUIP_ITEM;


typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wEquipTabLen;
	std::wstring	strEquipTab;
	WORD			wEquipSlot;
	WORD			wInventoryTabLen;
	std::wstring	strInventoryTab;
	WORD			wInvenSlot;
}SEND_UN_EQUIP_ITEM,*PSEND_UN_EQUIP_ITEM;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwUsingTabIndex;		// 鉴定书所在背包栏序号
	WORD			wUsingSlotIndex;		// 鉴定书位置
	DWORD			dwTargetTabIndex;		// 被鉴定物品所在背包栏序号
	WORD			wTargetSlotIndex;		// 被鉴定的物品位置
}SEND_USE_FOR_ITEM,*PSEND_USE_FOR_ITEM;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwFixedNpcHash;
	WORD			wSellId;
	WORD			wCount;
}SEND_BUY_ITEM,*PSEND_BUY_ITEM;


typedef	struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwFixedNpcHash;
	DWORD			dwTabCategory;
	DWORD			dwSlotIndex;
	DWORD			dwCount;
}SEND_SELL_ITEM,*PSEND_SELL_ITEM;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSkillSet;			// 11.30 新增数据段
	WORD			wRequestLearnSkillList;
	WORD			warray;
	DWORD			dwsize;
	struct
	{
		WORD		witem_i;
		WORD		wpair;
		DWORD		dwkey;
		DWORD		dwvalue;
	}Item[1];		// 此结构只适合学一个技能
}SEND_LEARN_SKILL_MULTIPLE,*PSEND_LEARN_SKILL_MULTIPLE;

typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
}SEND_CHANGE_VILLGAE,*PSEND_CHANGE_VILLGAE;

typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wPlayerIdLen;
	std::wstring	strPlayerId;
}SEND_SELECT_PLAYER,*PSEND_SELECT_PLAYER;


typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wChannelIdLen;
	std::wstring	strChannelId;
}SEND_REQUEST_SWITCH_CHANNEL,*PSEND_REQUEST_SWITCH_CHANNEL;


typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wChannelIdLen;
	std::wstring	strChannelId;
	WORD			wAccountIdLen;
	std::wstring	strAccountId;
	WORD			wSessionKeyLen;
	std::wstring	strSessionKey;
}SEND_SWITCH_CHANNEL,*PSEND_SWITCH_CHANNEL;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwType;
	DWORD			dwSystemMailType;
	WORD			wTitleLen;
	std::wstring	strTitle;
	LONGLONG		llOidReceiver;
	WORD			wReceiverIdLen;
	std::wstring	strReceiverId;
	WORD			wMessageLen;
	std::wstring	strMessage;
	LONGLONG		llGold;
	DWORD			dwOidMailItemList;
	WORD			wItems;
	WORD			warray;
	DWORD			dwsize;
}SEND_MAIL_SEND_HEADER,*PSEND_MAIL_SEND_HEADER;

typedef struct
{
	WORD		witem_i;
	WORD		wApple_SendingMailItem;
	WORD		wInventoryTabLen;
	std::wstring	strInventoryTab;
	DWORD			dwItemSlotIndex;
	DWORD			dwCount;
}SEND_MAIL_SEND_BODY,*PSEND_MAIL_SEND_BODY;

typedef struct
{
	WORD		wExpireDate;
	APPLE_DATA_TIME	theAppleDateTime;
}SEND_MAIL_SEND_TAIL,*PSEND_MAIL_SEND_TAIL;

typedef struct  
{
	PACKET_HEADER	theHeader;
	LONGLONG		llMailSerial;
	BYTE			bIsFirst;
}SEND_GETATTACHMEMTS,*PSEND_GETATTACHMEMTS;


typedef struct
{
	PACKET_HEADER	theHeader;
	LONGLONG		llMailSerial;
	BYTE			bIsFirst;
}SEND_MAIL_REMOVE,*PSEND_MAIL_REMOVE;

typedef struct
{
	PACKET_HEADER	theHeader;
	BYTE			bIgnoreFistPlay;
	BYTE			bSwitchChannelInVillage;
}SEND_JOIN_VILLAGE,*PSEND_JOIN_VILLAGE;

typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
	DWORD			dwRewardItemId;
	DWORD			dwInventoryTabIndex;
	DWORD			dwInventorySlotIndex;
}SEND_ACCEPT,*PSEND_ACCEPT;

typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
	WORD			wRewardItemIdLen;
	std::wstring	strRewardItemId;

}SEND_COMPLETE,*PSEND_COMPLETE;

typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
}SEND_GIVEUP,*PSEND_GIVEUP;

typedef struct 
{
	PACKET_HEADER	theHeader;
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
	DWORD			dwState;
}SEND_TELEPORT_TO_VILLAGE,*PSEND_TELEPORT_TO_VILLAGE;

typedef struct  
{
	PACKET_HEADER	theHeader;
	DWORD			dwTabCategory;
	DWORD			dwSlotIndex;
	WORD			wIsAvatarItem;
}SEND_DISASSEMBLY,*PSEND_DISASSEMBLY;

typedef struct  
{
	PACKET_HEADER	theHeader;
	DWORD			dwRewardIndex;
}SEND_STAGE_REWARD,*PSEND_STAGE_REWARD;

typedef struct  
{
	PACKET_HEADER	theHeader;
	DWORD			dwOnePageProductCount;		// 每页请求个数，这个游戏固定是10
	DWORD			dwPageNum;					// 请求第多少页
	WORD			wCategoryLen;
	std::wstring	strCategory;
	DWORD			dwGrade;					// 初级=1 中级=2 高级=4 稀有=8 传说=0x10 全部选上，则是所有的总和=0x1F
	BYTE			bIsUseable;
	DWORD			dwMinLv;
	DWORD			dwMaxLv;
	DWORD			dwMinEnhantLevel;
	DWORD			dwMaxEnhantLevel;
	WORD			wKeyWordLen;
	std::wstring	strKeyWord;					// 搜索关键字
	DWORD			dwOrderType;
	WORD			wOptionList;				// 当OptionList给DWORD类型，即可导致游戏掉线
	WORD			wassociative_array;
	DWORD			dwsize;						// 这个数组数量给0即可
}SEND_REQUEST_PRODUCT_LIST,*PSEND_REQUEST_PRODUCT_LIST;

typedef struct
{
	PACKET_HEADER	theHeader;
	LONGLONG		llOidMarket;
	WORD			wCount;
}SEND_BUY_PRODUCT,*PSEND_BUY_PRODUCT;

typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wCategoryLen;
	std::wstring	strCategory;
	DWORD			dwItemTabCategory;
	DWORD			dwItemSlotIndex;
	LONGLONG		llPrice;
	DWORD			dwPeriod;			// 有效期，有1天，3天，5天，7天
	DWORD			dwCount;
}SEND_REGISTER_PRODUCT,*PSEND_REGISTER_PRODUCT;

typedef struct
{
	PACKET_HEADER	theHeader;
}SEND_REQUEST_MY_PRODUCT_LIST,*PSEND_REQUEST_MY_PRODUCT_LIST;

typedef struct
{
	PACKET_HEADER	theHeader;
	LONGLONG		llOidMarket;
}SEND_UNREGISTER_PRODUCT,*PSEND_UNREGISTER_PRODUCT;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD		dwItemTabCategory;
	DWORD		dwItemSlotIndex;
}SEND_REQUEST_PRODUCT_INFO,*PSEND_REQUEST_PRODUCT_INFO;


typedef struct
{
	PACKET_HEADER	theHeader;
	int				nItemPackageTabIndex;		// 物品栏
	WORD			wItemPackageSlotIndex;		// 物品在第几格
	int				nRequireItemTabIndex;		// 配合打开物品需要的物品  -1表示没有
	WORD			wRequireItemSlotIndex;		// 物品在第几格，如果不存在物品，则置0
	int				nCount;
}SEND_OPEN_ITEM_PACKAGE,*PSEND_OPEN_ITEM_PACKAGE;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwMasterSerial;
	DWORD			dwRequestSerial;
	WORD			wNpcIdLen;
	std::wstring	strNpcId;
	XYZ				thePosition;
	XYZ				theDirection;
	WORD			wTeamLen;
	std::wstring	strTeam;
	BYTE			bIsControllable;
	BYTE			bDropItemWhenDie;
	BYTE			bGiveExpWhenDie;
	BYTE			bIsStageObject;
	BYTE			bUninfluencedByQuest;
	WORD			wDefaultActionIdLen;
	std::wstring	strDefaultActionId;
	DWORD			dwNpcType;
}SEND_CREATE_NPC_BY_CLIENT,*PSEND_CREATE_NPC_BY_CLIENT;

typedef struct
{
	PACKET_HEADER	theHeader;
	int				nTabIndex;
	WORD			wSlotIndex;
}SEND_USE_ITEM,*PSEND_USE_ITEM;

typedef struct
{
	PACKET_HEADER		theHeader;
	WORD				wSerials;
	WORD				warray;
	DWORD				dwsize;
	struct
	{
		DWORD			dwSerial;
	}theSerial[100];

	FLOAT				fMaxDt;
	FLOAT				fClientAbsoluteTime;

	WORD				wSumOfDeltaHps;
	WORD				wassociative_array;
	DWORD				dwassociativesize;

	struct
	{
		DWORD			dwitem_first;
		DWORD			dwitem_second;
	}theSumOfDeltaHps[100];
}SEND_DESTROY_ACTORS,*PSEND_DESTROY_ACTORS;

typedef struct
{
	PACKET_HEADER		theHeader;
	DWORD				dwSerial;
	DWORD				dwDeltaHp;
	DWORD				dwReason;
	DWORD				dwCauserSerial;
	DWORD				dwEventOptionSerial;
}REQUEST_CHANGE_HP,*PREQUEST_CHANGE_HP;

///////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwResult;		// 进入游戏，一定时间内无法移动邮件物品 = 6
	LONGLONG		llMailSerial;
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theDateTime;
}RECV_GET_ATTACHMENTS,*PRECV_GET_ATTACHMENTS;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwResult;		// = 0
	WORD			wContents;
	WORD			wApple_MailContents;
	LONGLONG		llMailSerial;
	WORD			wMessageLen;
	std::wstring	strMessage;
	WORD			wItems;
	WORD			warray;
	DWORD			dwsize;		// dwsize = 0，表示没有附件物品，dwsize>=1表示有附件存在，大于1的时候，只有在鼠标点击查看的时候会存在，接受附件后，此包依然会到，但是dwsize=0
	// ...
}RECV_MAIL_CONTENTS,*PRECV_MAIL_CONTENTS;

typedef struct  
{
	PACKET_HEADER	theHeader;
	WORD			wPlayerIds;
	WORD			warray;
	DWORD			dwsize;
	struct
	{
		WORD			wPlayerIdsLen;
		std::wstring	strPlayerIds;
	}thePlayerIds[10];
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theDateTime;
	DWORD			dwResult;			// 已超出入场次数上线，无法进入副本=6   被限制进入副本=0x0F
}RECV_START_STAGE_FAILED,*PRECV_START_STAGE_FAILED;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwResult;			// 成功 = 0
	WORD			wProduct;
	WORD			wApple_ProductStatistics;
	LONGLONG		llAveragePrice;		// 拍卖行平均价
	DWORD			dwCount;			// 最近4周交易次数
	LONGLONG		llLowerPrice;		// 拍卖行最低价
}RECV_REQUEST_PRODUCT_INFO,*PRECV_REQUEST_PRODUCT_INFO;

typedef struct
{
	PACKET_HEADER	theHeader;  
	DWORD			dwResult;		//  登陆失败=0x0B  登陆成功=0
	WORD			wAccountIdLen;
	std::wstring	strAccountId;
	DWORD			dwAge;
	DWORD			dwShutdown;
	DWORD			dwKickTime;
	BYTE			bIsGm;
	BYTE			bIsSpecificAccount;
	BYTE			bPlayerNotCreated;
	WORD			wWorldNameLen;
	std::wstring	strWorldName;
	WORD			wWorldIdLen;
	std::wstring	strWorldId;
	LONGLONG		llOidAccount;
	BYTE			bIsReturnee;
	BYTE			bIsMembership;
	WORD			wReturnExpiration;
	APPLE_DATA_TIME	theReturnExpirationDataTime;
	WORD			wMembershipExpiration;
	APPLE_DATA_TIME	theMembershipExpirationDataTime;
	WORD			wCreated;
	APPLE_DATA_TIME	theCreatedDataTime;
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theBlockExpirationDataTime;
	DWORD			dwBlockType;
	WORD			wBlockReasonLen;
	std::wstring	strBlockReason;
	DWORD			dwBlockGrade;
	WORD			wBlockGradeExpiration;
	APPLE_DATA_TIME	theBlockGradeExpirationDataTime;
}RECV_LOGIN,*PRECV_LOGIN;


typedef struct	// SellItemResult
{
	PACKET_HEADER	theHeader;
	DWORD			dwResult;	// 成功=0，被限制=6
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theBlockExpirationDataTime;
}RECV_SELL_ITEM_RESULT,*PRECV_SELL_ITEM_RESULT;


typedef struct	// BuyItemResult
{
	PACKET_HEADER	theHeader;
	DWORD			dwResult;	// 成功=0，被限制=6
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theBlockExpirationDataTime;
}RECV_BUY_ITEM_RESULT,*PRECV_BUY_ITEM_RESULT;


typedef struct
{
	PACKET_HEADER	theHeader;
	WORD			wNpcIdLen;
	std::wstring	strNpcId;
	WORD			wTeamIdLen;
	std::wstring	strTeamId;
	WORD			wDefaultActionIdLen;
	std::wstring	strDefaultActionId;
	WORD			wLocationIdLen;
	std::wstring	strLocationId;
	DWORD			dwSerial;
	DWORD			dwMasterSerial;
	DWORD			dwRequestSerial;
	DWORD			dwOwnerSerial;
	DWORD			dwHp;
	DWORD			dwMp;
	DWORD			dwMaxHp;
	DWORD			dwMaxMp;
	DWORD			dwRandomSeed;
	DWORD			dwLevel;
	DWORD			dwMiscIndex;
	FLOAT			fPositionX;
	FLOAT			fPositionY;
	FLOAT			fDirectionX;
	FLOAT			fDirectionZ;
	FLOAT			fDirectionY;
	FLOAT			fScaleX;
	FLOAT			fScaleZ;
	FLOAT			fScaleY;
	BYTE			bIsDummy;
	BYTE			bDropItemWhenDie;
	BYTE			bGiveExpWhenDie;
	BYTE			bRequestFromClient;
	BYTE			bRequestFromPlayer;
	BYTE			bAttachEffectWhenCreated;
	BYTE			bIsControllable;
	BYTE			bIsStageObject;
	BYTE			bIsStageFailObject;
	BYTE			bUninfluencedByQuest;
	BYTE			bIsIntrusionNpcDropItem;
	BYTE			bIsCreateByClient;
	WORD			wStatus;
	WORD			wApple_Logic_EncryptedStatusContainer;

	WORD			wStringContainer;
	WORD			wStringContainerarray;
	DWORD			dwStringContainersize;		// assert(!dwStringContainersize);  当有数据的时候，断言
	struct
	{
		WORD			wStringContainerLen;
		std::wstring	strStringContainer;
	}theStringContainer[50];				// 猜想是此结构

	WORD			wFloatContainer;
	WORD			wFloatContainerarray;
	DWORD			dwFloatContainersize;
	struct
	{
		FLOAT		fFloatContainerItem_i;
	}theFloatContainer[50];

	WORD			wIntContainer;
	WORD			wIntContainerarray;
	WORD			wIntContainersize;
	struct
	{
		DWORD		dwIntContainerItem_i;
	}theIntContainer[50];

	WORD			wBoolContainer;
	WORD			wBoolContainerarray;
	DWORD			dwBoolContainersize;		// assert(!dwBoolContainersize);  当有数据的时候，断言
	struct
	{
		BYTE		bBoolContainer;
	}theBoolContainer[50];					// 猜想是此结构

	WORD			wVectorContainer;
	WORD			wVectorContainerarray;
	DWORD			dwVectorContainersize;		// assert(!dwBoolContainersize);  当有数据的时候，断言

	WORD			wKeyDataContainer;
	WORD			wKeyDataContainerarray;
	DWORD			dwKeyDataContainersize;
	struct
	{
		WORD		wKeyDataContainer_itme_i;
		WORD		wApple_Logic_KeyData;
		DWORD		dwHashKey;
		WORD		wStatusTypeLen;
		std::wstring	strStatusType;
		DWORD			dwIndex;
	}theKeyDataContainer[50];

	WORD			wBuffs;
	WORD			wApple_BuffContainer;
	WORD			wList;
	WORD			wListarray;
	DWORD			dwListsize;
	struct  
	{
		WORD		wListItem_i;
		WORD		wApple_BuffInstance;
		WORD		wBuffIdLen;
		std::wstring	strBuffId;
	}theList[50];

	DWORD			dwLevel2;
	DWORD			dwSerial2;
	DWORD			dwCasterSerial;
	DWORD			dwCasterOwnerSerial;
	DWORD			dwTargetSerial;
	WORD			wEndTime;
	APPLE_DATA_TIME	theEndTime;
	FLOAT			fDuration;
	LONGLONG		llOidBuff;
	DWORD			dwRandomSeed2;
	DWORD			dwNextSerial;
}RECV_CREATE_NPC,*PRECV_CREATE_NPC;


typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwIndex;
	DWORD			dwSerial;
	DWORD			dwStageMasterSerial;
	DWORD			dwRandomSeed;
	DWORD			dwMiscIndex;
	WORD			wBuffs;
	WORD			wApple_BuffContainer;
	WORD			wList;
	WORD			wListarray;
	DWORD			dwListsize;
	struct
	{
		WORD		wListitem_i;
		WORD		wApple_BuffInstance;
		WORD		wBuffIdLen;
		std::wstring	strBuffId;
	}theList;
	DWORD			dwLevel;
	DWORD			dwSerial2;
	DWORD			dwCasterSerial;
	DWORD			dwCasterOwnerSerial;
	DWORD			dwTargetSerial;
	WORD			wEndTime;
	APPLE_DATA_TIME	theEndTime;
	FLOAT			fDuration;
	LONGLONG		llOidBuff;
	DWORD			dwRandomSeed2;
	DWORD			dwNextSerial;
}RECV_CREATE_POP_INFO_NPC,*PRECV_CREATE_POP_INFO_NPC;

typedef struct
{
	PACKET_HEADER		theHeader;
	DWORD				dwResult;		// 0表示成功 0x12表示限制 
	WORD				wQuestIdLen;
	std::wstring		strQuestId;
	WORD				wBlockExpiration;
	APPLE_DATA_TIME		theAppleDataTime;
}ACCEPT_QUEST_RESULT,*PACCEPT_QUEST_RESULT;

typedef struct
{
	PACKET_HEADER		theHeader;
	DWORD				dwResult;		// 0表示成功 1表示限制 
	WORD				wVillageIdLen;
	std::wstring		strVillageId;
	WORD				wSectorIdLen;
	std::wstring		strSectorId;
	DWORD				dwTimeStamp;
	BYTE				bIsMaxLevelExpendTutorialPlay;
	WORD				wBlockExpiration;
	APPLE_DATA_TIME		theAppleDataTime;	// 被限制到的时间
}ACCEPT_CHANGE_VILLAGE,*PACCEPT_CHANGE_VILLAGE;

typedef struct
{
	PACKET_HEADER	theHeader;
	DWORD			dwSerial;		// 死亡角色序号，如果组队状态，则需要判断
}PLAYER_DEAD,*PPLAYER_DEAD;


typedef struct
{
	int		str_nFatigue;				// 正常疲劳值	==> 疲劳值
	int		str_nEventFatigue;			// 事件疲劳值
	int		str_nPremiumFatigue;		// 不知道
	int		str_nMembershipFatigue;		// 会员疲劳值	==> VIP疲劳值
	int		str_nSpentFatigue;			// 已刷的疲劳值
}ROLE_FATIGUE,*PROLE_FATIGUE;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theWarp;
	struct
	{
		DWORD			str_dwVFunAddr;
		GAME_STRING_ID	str_theId;
		FLOAT			str_fPositionX;
		FLOAT			str_fPositionY;
		FLOAT			str_fDirectionX;
		FLOAT			str_fDirectionZ;
		FLOAT			str_fDirectionY;
	}theWarpGate;
}SDB_WARP_GATE,*PSDB_WARP_GATE;





#endif  //_CLASSA_H_


