
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 声明的结构体
#ifndef JFZR_STRUCT_91044C184E5B46508DDB073DA8DEFC42
#define JFZR_STRUCT_91044C184E5B46508DDB073DA8DEFC42
// Source code here

typedef struct  
{
	DWORD	str_dwNextAddrLeft;
	DWORD	str_dwNextAddrParent;
	DWORD	str_dwNextAddrRight;
}NODE,*PNODE;

typedef struct
{
	DWORD	str_dwUnKnowAddr;
	DWORD	str_dwBinaryTreeNullMapAddr;			// 指向二叉树空节点
	DWORD	str_dwCount;							// 二叉树有效节点个数，不包括空节点
	DWORD	str_dwUnKnowAddr2;
}BINARY_TREE_MAP,*PBINARY_TREE_MAP;

typedef	struct
{
	DWORD				str_dwBeginAddr;
	DWORD				str_dwEndAddr1;
	DWORD				str_dwEndAddr2;
	DWORD				str_dwCommandAddr;
}GAME_STRUCT_ARRAY_POINT,*PGAME_STRUCT_ARRAY_POINT;

typedef struct  
{
	FLOAT		fx;
	FLOAT		fz;
	FLOAT		fy;
}XYZ;


typedef struct
{
	union
	{
		wchar_t*	wstr_ptr_id;
		wchar_t		wstr_id[8];	
	};
	DWORD	str_dwStrLen;
	DWORD	str_dwStrLen_;
	DWORD	str_dwUnKnow1;
}GAME_STRING_ID,*PGAME_STRING_ID;

typedef struct
{
	union
	{
		wchar_t*	wstr_ptr_id;
		wchar_t		wstr_id[8];	
	};
	DWORD	str_dwStrLen;
	DWORD	str_dwStrLen_;
	DWORD	str_dwUnKnow1;
	DWORD	str_dwUnKnow2;
}GAME_STRING_ID_,*PGAME_STRING_ID_;


typedef struct
{
	DWORD	str_dwVFunAddr;		// 虚函数
	union
	{
		wchar_t*	wstr_ptr_id;
		wchar_t		wstr_id[8];	
	};
	DWORD	str_dwStrLen;
	DWORD	str_dwStrLen_;
	DWORD	str_dwUnKnow1;
}GAME_STRING_VF_ID,*PGAME_STRING_VF_ID;



extern wchar_t* GetString(PGAME_STRING_ID ptheGameString);
extern wchar_t* GetVString(PGAME_STRING_VF_ID ptheGameString);
extern wchar_t* GetString_(PGAME_STRING_ID_ ptheGameString);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 职业任务
typedef struct  
{
	DWORD	str_dwNextAddrLeft;
	DWORD	str_dwNextAddrParent;
	DWORD	str_dwNextAddrRight;
	DWORD	str_dwid;						// 任务id
	DWORD	str_dwTaskObjAddr;				// 任务对象
	DWORD	str_dwTaskObjPointAddr;			// 指向任务对象
	bool	str_bFlag;
	DWORD	str_dwUnKnow;
}TASK_OBJ,*PTASK_OBJ;						// 遍历任务二叉树时，节点对象

typedef struct		// Apple::StaticDatabase::Quest
{
	GAME_STRING_VF_ID	str_theTaskId;				// 任务名称id
	GAME_STRUCT_ARRAY_POINT		str_theStageIds;	// 任务所在区域  10.30不删档测试字段变化，并减少掉一个
	GAME_STRING_VF_ID	str_theMainTitleId;			// 主标题英文映射
	GAME_STRING_VF_ID	str_theSubTitleId;			// 副标题英文映射
	DWORD				str_dwCategory;				// 任务类别	==>    4,普通任务之副本杀怪任务  5,主线任务  6,普通任务之收集材料任务 其它未知
	DWORD				str_dwLevel;				// 任务等级
	GAME_STRING_VF_ID	str_theBeginDescId;			// 查看任务时，右窗口的标题
	GAME_STRING_VF_ID	str_theEndDescId;			// 查看任务时，右窗口的内容
}TASK_INFO,*PTASK_INFO;

typedef struct  
{
	GAME_STRING_ID		str_theAcceptTaskNpcId;		// 接任务NPC id
	GAME_STRING_ID		str_theCompleteNpcId;		// 交任务NPC id
}ACCEPT_COMPLETE,*PACCEPT_COMPLETE;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theCollectItemNameId;		// 需要收集物品的名称id
	DWORD				str_dwCollectItemCount;			// 需要收集物品的数量
}COLLECT_ITEM,*PCOLLECT_ITEM;							// 收集物品

typedef struct	// Apple::Condition
{
	DWORD				str_dwVFunAddr;
	union  
	{
		DWORD			str_dwClasses;
		DWORD			str_dwClassesArray[4];
	};
	union  
	{
		FLOAT			str_fLevel;
		FLOAT			str_fLevelArray[2];
	};
	DWORD				str_dwGold;
	union  
	{
		DWORD			str_dwRequiredTierAttackPower;
		DWORD			str_dwRequiredTierAttackPowerArray[2];
	};
	DWORD				str_dwAwakening;		// +0x28
	//DWORD				str_dwPvpPoint;

	BINARY_TREE_MAP				str_thePvpWinCounts;
	BINARY_TREE_MAP				str_thePvpLoseCounts;
	BINARY_TREE_MAP				str_thePvpTotalCounts;
	BINARY_TREE_MAP				str_theItems;						// 收集物品
	BINARY_TREE_MAP				str_theLicenses;					// +0x6C
	GAME_STRUCT_ARRAY_POINT		str_theInacceptableQuests;
	DWORD						str_dwInacceptableWhenCompleted;	// 11.30新增数据段
	GAME_STRUCT_ARRAY_POINT		str_thePrecedingQuests;				// 上一个任务
	GAME_STRUCT_ARRAY_POINT		str_thePrecedingQuestsTypeOr;

	DWORD				str_dwDifficulty;
	DWORD				str_dwCompleteType;

	GAME_STRUCT_ARRAY_POINT		str_theNpcConditions;				// 击杀怪物
	GAME_STRUCT_ARRAY_POINT		str_thePvpConditions;
	BINARY_TREE_MAP				str_theStageConditions;				// 连击
	GAME_STRUCT_ARRAY_POINT		str_theUseItems;
	GAME_STRUCT_ARRAY_POINT		str_theEnhanceItems;
	BINARY_TREE_MAP				str_theEquipmentSetInfos;
	BINARY_TREE_MAP				str_theStatusConditions;

	DWORD				str_dwInventoryCapacity;
	DWORD				str_dwPlayerBankCapacity;
	DWORD				str_dwAccountBankCapacity;
}ACCPET_CONDITION,COMPLETE_CONDITION,*PACCPET_CONDITION,*PCOMPLETE_CONDITION;		// 接收条件，完成条件

typedef struct
{
	DWORD				str_dwVFunAddr;
	DWORD				str_dwNpcConditionType;
	GAME_STRING_ID		str_theKey;							// NPC id
	DWORD				str_dwValue;						// 需要击杀的数量
	DWORD				str_dwBackAttackCount;
	DWORD				str_dwSpecialAttackUseCount;
	DWORD				str_dwDeadlySkillUseCount;
	GAME_STRUCT_ARRAY_POINT		str_theSkillUsageCounts;
	GAME_STRUCT_ARRAY_POINT		str_theApprovedStages;		// 怪物所在副本地图
	BINARY_TREE_MAP				str_theNpcKillBySkill;
}NPC_CONDITION,*PNPC_CONDITION;								// 攻击NPC对象

typedef struct	// Apple::StaticDatabase::Quest::Reward
{
	DWORD				str_dwVFunAddr;
	DWORD				str_dwExpRate;
	DWORD				str_dwBonusExp;		// 11.30不删档改变字段
	DWORD				str_dwGold;
	DWORD				str_dwRemoveGold;
	BINARY_TREE_MAP		str_theBuffsToGive;
	BINARY_TREE_MAP		str_theBuffsToRemove;
	BINARY_TREE_MAP		str_theItemsToGive;
	BINARY_TREE_MAP		str_theItemsToGiveMail;		// 11.30不删档新增字段
	BINARY_TREE_MAP		str_theItemsToRemove;
	BINARY_TREE_MAP		str_theSelectableItems;				// 选择奖励物品
	BINARY_TREE_MAP		str_theLicensesToGive;
	BINARY_TREE_MAP		str_theLicensesToRemove;
	GAME_STRING_VF_ID	str_theChangeClass;
	DWORD				str_dwMakeAwaken;
	DWORD				str_dwAwakenPoint;
	GAME_STRUCT_ARRAY_POINT		str_theUnlockBasicEquipSlots;
	GAME_STRING_ID		str_theTitle;				// 11.30结构体改变
	DWORD				str_dwAccountMileage;		// 11.30不删档新增字段
	FLOAT				str_fTierAttackPower;		// 11.30不删档新增字段
}QUEST_REWARD,*PQUEST_REWARD;					//任务奖励

typedef struct
{
	GAME_STRING_VF_ID			str_theVillageId;
	struct  
	{
		FLOAT	x;
		FLOAT	z;
		FLOAT	y;
	}str_thePosition;
	struct  
	{
		FLOAT	x;
		FLOAT	z;
		FLOAT	y;
	}str_theDirection;
}TELEPORTLOCATION,*PTELEPORTLOCATION;			// TeleportLocation




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 已接任务
typedef	struct  
{
	NODE				str_theNode;
	DWORD				str_dwUnknow;
	GAME_STRING_ID		str_theGameTaskId;
}RECEIVED_TASK,*PRECEIVED_TASK;

typedef struct
{
	DWORD				str_dwVFun;
	GAME_STRING_ID		str_theGameNpcType;
	GAME_STRING_ID		str_theGameNpcNameId;		// 任务需要消灭NPC的名称id
	DWORD				str_dwConditionsCount;		// 需要消灭NPC对象的个数
}CONDITIONS_NPC_TASK,*PCONDITIONS_NPC_TASK;			// 消灭NPC任务对象

typedef struct
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theType;
	GAME_STRING_ID		str_theKey;						// 任务需要消灭NPC的名称id
	DWORD				str_dwValue;					// 需要消灭NPC对象的个数
	DWORD				str_dwBackAttackCount;						// 背后攻击次数
	DWORD				str_dwSpecialAttackUseCount;				// 特殊攻击使用数
	DWORD				str_dwDeadlySkillUseCount;					// 致命的技能使用数
	GAME_STRUCT_ARRAY_POINT			str_theSkillUsageCounts;		// 技能的使用数量
	GAME_STRUCT_ARRAY_POINT			str_theStages;					// 任务阶段，即所属地图
	BINARY_TREE_MAP					str_theNpcKillBySkill;			// 使用技能杀死怪物
}REMAIN_NPC_CONDITION,*PREMAIN_NPC_CONDITION;						// RemainNpcCondition

typedef struct
{
	GAME_STRING_VF_ID		str_theId;
	DWORD					str_dwConditions;
	BYTE					str_bCompletable;
	BYTE					str_bZeor1[3];
	struct
	{
		DWORD						str_dwVFunAddr2;
		GAME_STRUCT_ARRAY_POINT		str_theRemainUseItems;
		GAME_STRUCT_ARRAY_POINT		str_theRemainKillNpcInfos;			// 剩余杀怪数量
		GAME_STRUCT_ARRAY_POINT		str_theRemainPvpConditions;
		BINARY_TREE_MAP				str_theRemainStageIds;
		GAME_STRUCT_ARRAY_POINT		str_theRemainEnhance;
		BINARY_TREE_MAP				str_theRemainStatus;
	}str_theRemainCondition;

	DWORD					str_dwNullAddr;
	struct
	{
		DWORD				str_dwOidActiveCondition1;
		DWORD				str_dwOidActiveCondition2;
	}str_theOidActiveCondition;
}ACTIVE_CONDITION,*PACTIVE_CONDITION;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stage模板
typedef struct  
{
	DWORD						str_dwVFunAddr;
	GAME_STRUCT_ARRAY_POINT		str_theWarpGeteLinkInfosNode;
}WARP_GATE_LINK_INFOS_NODE,*PWARP_GATE_LINK_INFOS_NODE;	//WarpGateLinkInfos

typedef struct 
{
	DWORD				str_dwVFunAddr;
	DWORD				str_dwStageType;			// 城镇=1,副本=0
	GAME_STRING_ID		str_theId;					// +8
	GAME_STRING_ID		str_theStartingSectorId;	// +24
	GAME_STRING_ID		str_theEnteringWarpGateId;	// +40	10.30不删档新增数据
	GAME_STRING_VF_ID	str_theName;				// +5C
	GAME_STRING_VF_ID	str_theClearConditionDesc;	// +7C
	DWORD				str_dwRecommandLevel[2];	// +9C
	DWORD				str_dwEnterLevelRange[2];	// +A4	20150316

	DWORD				str_dwRecommandDPS;			// 10.30不删档新增数据  dps 攻击
	DWORD				str_dwExperiencedDifficulty;
	XYZ					str_theStartingDirection;	// +AC
	GAME_STRUCT_ARRAY_POINT		str_theWarpGateLinkInfos;		// +B8  传送门连接信息
	BINARY_TREE_MAP				str_theSectors;					// +CC  当前地图的房间
	GAME_STRUCT_ARRAY_POINT		str_theRandomRoute;				// +D8
	GAME_STRUCT_ARRAY_POINT		str_theUniqueDropListForShowing;
	GAME_STRUCT_ARRAY_POINT		str_theClearBuffListForShowing;	// +F8
	DWORD						str_dwClearExp;						// +108
	DWORD						str_dwClearGold;
	GAME_STRING_ID				str_theClearMedal;					// +110
	BINARY_TREE_MAP				str_theStageObjectNpcs;				// BOOS
	BINARY_TREE_MAP				str_theStageFailObjectNpcs;			// +13C
	GAME_STRUCT_ARRAY_POINT		str_theClearConditions;
	GAME_STRUCT_ARRAY_POINT		str_theFailConditions;				// 
	FLOAT						str_fStartingPositionX;
	FLOAT						str_fStartingPositionY;
	DWORD						str_dwMinimumLevelToGetLicense;
	DWORD						str_dwGetResurrectionSicknessWhenFailed;
	GAME_STRING_ID_				str_theMinimapPath;					// +17C
	GAME_STRING_ID_				str_theFishingSiteMinimapPath;		// 1230新增
	GAME_STRUCT_ARRAY_POINT		str_theCameraEffectPaths;
	DWORD						str_dwLevel;		// 
	DWORD						str_dwFatigue;		// 消耗疲劳值
	BYTE						str_bEnableStageFailBuff;
	BYTE						str_bIsRestartable;
	WORD						str_wUnKnow1;
	BINARY_TREE_MAP				str_theMonsterStatusBonus;
	BINARY_TREE_MAP				str_theMonsterStatusBonusByDelta;
	GAME_STRING_ID				str_theBgmCue;
	GAME_STRING_ID				str_theBossBgmCue;
	BINARY_TREE_MAP				str_theAmbientSoundEvents;
	FLOAT						str_fStartingRectangleX;
	FLOAT						str_fStartingRectangleY;
	// ...
}SDB_STAGE_INFO,*PSDB_STAGE_INFO;		// StageInfo

typedef struct
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theStageId;			// id
	GAME_STRING_ID		str_theSectorId;		// 房间序号
	GAME_STRING_ID		str_theWarpGateId;		// 门
	BYTE				str_bEnable;			// 是否启用 1为可通过，0为不可通过
	BYTE				str_bUnknow1;
	WORD				str_wUnknow2;
}WARP_GATE,*PWARP_GATE;

typedef struct  
{
	WARP_GATE	str_theWarpGate1;
	WARP_GATE	str_theWarpGate2;
}WARP_GATE_LINK,&PWARP_GATE_LINK;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theId_;
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID_		str_theSectorPath;
	GAME_STRING_ID		str_theId;
	DWORD				str_dwSectorResetType;				// 房间重置类型
	GAME_STRUCT_ARRAY_POINT		str_theSectorLimitPoints;
	GAME_STRUCT_ARRAY_POINT		str_theRestrictions;
	GAME_STRUCT_ARRAY_POINT		str_theLocations;
	BYTE						str_bIsBossSector;			// 是否为BOOS房间
	DWORD						str_dwMinimapOffset[2];
	DWORD						str_dwMinimapSize[2];
	BINARY_TREE_MAP				str_theWarpGates;			// +88 房间传送门	
	GAME_STRUCT_ARRAY_POINT		str_theStageStartWarpGates;	// stage开始传送门
	GAME_STRUCT_ARRAY_POINT		str_thePopInfos;
	GAME_STRUCT_ARRAY_POINT		str_theFixedNpcs;			// 城镇NPC
	GAME_STRUCT_ARRAY_POINT		str_theInteractiveNpcs;		
	GAME_STRUCT_ARRAY_POINT		str_theFixedNpcsByQuest;
	GAME_STRUCT_ARRAY_POINT		str_theBuffFloors;
	GAME_STRUCT_ARRAY_POINT		str_theStartingRedWall;
	GAME_STRUCT_ARRAY_POINT		str_theStartingBlueWall;
	GAME_STRUCT_ARRAY_POINT		str_theAdditionalNpcPopInfos;
	GAME_STRING_ID				str_theReverbPreset;
	struct  
	{
		DWORD					str_dwVFunAddr;
		GAME_STRING_ID			str_theCinematicDialogId;
		GAME_STRUCT_ARRAY_POINT	str_theCreateItems;
	}theClearEventInfo;		// +144
	struct  
	{
		DWORD					str_dwVFunAddr;
		GAME_STRING_ID			str_theWarpGateId;
		DWORD					str_dwDelayTime;
	}theAutoWarpGateAfterSectorClearInfo;		// +174
	struct  
	{
		DWORD					str_dwVFunAddr;
		GAME_STRING_ID			str_theUnknow1;
	}theUnKnow1;
	XYZ							str_theSectorCenterPosition;
	XYZ							str_theSectorRebirthPosition;
	XYZ							str_theSectorRebirthDirection;
	XYZ							str_theSectorEscortPosition;
	struct  
	{
		DWORD					str_dwVFunAddr;
		struct  
		{
			DWORD				str_dwVFunAddr;
			GAME_STRING_ID		str_theEnterMessage;
		}theEnterMessage;
		struct  
		{
			DWORD				str_dwVFunAddr;
			GAME_STRING_ID		str_theClearMessage;
		}theClearMessage;
		GAME_STRUCT_ARRAY_POINT		str_theTimeMessage;
		GAME_STRUCT_ARRAY_POINT		str_theMonsterMessage;
	}theSectorMessageInfo;
}SDB_SECTOR,*PSDB_SECTOR;		// Sector

typedef struct
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theId;
	FLOAT				str_fPositionX;
	FLOAT				str_fPositionY;
	XYZ					str_theDirection;
	XYZ					str_theScale;
	struct  
	{
		DWORD			str_dwVFunAddr;	
		DWORD			str_dwStamps;
		GAME_STRUCT_ARRAY_POINT	str_thefixedNpcMovement;
	}theMovementInfo;
}FIXD_NPC,*PFIXD_NPC;		// FixedNpc

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 遍历物品模板信息
typedef struct
{
	GAME_STRING_VF_ID		str_theId;						// +4
	GAME_STRING_VF_ID		str_theName;					// +20
	GAME_STRING_VF_ID		str_theDesc;					// +40
	DWORD					str_dwGrade;					// +60	装备强化等级
	DWORD					str_dwCategory;					// 类型
	DWORD					str_dwItemLevel;				// 装备等级
	DWORD					str_dwMaxStackCount;			// 最大叠加数量
	DWORD					str_dwMaxPossessionLimit;		// 最大拥有限制
	GAME_STRING_ID_			str_theIconPath;				// +74
	BINARY_TREE_MAP			str_theIconPathByGrade;			// +94
	GAME_STRING_ID			str_theUITextColor;				// +A4
	BYTE					str_bDisassemblable;			// +C0
	BYTE					str_bQuickSlotable;
	BYTE					str_bNoDropFromNpc;
	BYTE					str_bNoDropFromStageClear;
	BYTE					str_bAutoUseWhenPickUp;
	BYTE					str_bAutoPickUp;
	BYTE					str_bIsPickableWhenStableAction;
	BYTE					str_bCanSellTotheShop;			// 是否可以卖给商店
	BYTE					str_bReusable;
	BYTE					str_bPcCafeExclusive;
	WORD					str_wZeor1;
	DWORD					str_dwBindType;					// +CC  绑定类型
	GAME_STRING_ID			str_theMarketCategory;			// +D0	市场分类，拍卖分类需要用到
	GAME_STRING_ID			str_thePriceCategory;			// +EC  作用分类 即:装备，材料，任务物品等	CONSUME:消耗品  EQUIPMENT:装备
	GAME_STRING_ID			str_theSoundCategory;			// +108 物品类型 护甲 即:皮革,布甲,重甲等   QUEST:鉴定书 ARMOR_HEAVYARMOR:重甲
	GAME_STRING_ID			str_theDropModelCategory;		// +124 装备使用部位类型 即:上半身,手部等	QUEST:鉴定书
	DWORD					str_dwInventoryTab;				// +140 在背包栏所属分类  普通=0,消耗品=1 详细数据见遍历物品
	DWORD					str_dwSellPrice;				// +144

	BINARY_TREE_MAP			str_theStatusCondition;			// 20150317

	struct
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwExpirationType;			// 物品过期类型
		struct
		{
			DWORD				str_dwVFunAddr;
			DWORD				str_dwYear;
			DWORD				str_dwMonth;
			DWORD				str_dwDay;
			DWORD				str_dwHour;
			DWORD				str_dwMinute;
		}theDateInfo;
	}theExpireInfo;		// +148		过期信息
	struct
	{
		DWORD				str_dwVFunAddr;
		BYTE				str_bEnable;			// 启用标志
		BYTE				str_bZeor1;				// 内存对齐
		WORD				str_wZeor2;				// 内存对齐
		DWORD				str_dwEquipPart;		// 装备使用部位，即发包穿装备时需要用到的 EquipSlot
		DWORD				str_dwLimitLevel[2];	// 限制等级
		DWORD				str_dwLimitMinLevelMinus;	// 10.30不删档增加新的字段
		FLOAT				str_fMaxDurability;		// 最大耐久
		BYTE				str_bIdentified;		// 鉴定
		BYTE				str_bSealed;			// 封印
		BYTE				str_bIsTemplateItem;	// 是否可以交易 Rradable 1:可卖商店的普通装备  0:极品装备不可卖商店(不是100%确定)
		BYTE				str_bZeor4;
		GAME_STRING_ID		str_theEquipCategory;	// 装备类别 即:皮革,布甲,重甲等
		BINARY_TREE_MAP		str_theStatus;			// 物品属性
		BINARY_TREE_MAP		str_theSkillBonus;		// 技能增加
		DWORD				str_dwEquipableType;	// 装备能力类型 +0x5C
		GAME_STRUCT_ARRAY_POINT		str_theMeshPaths;
		GAME_STRING_ID_				str_theEffectPath;
		BINARY_TREE_MAP				str_theMeshPathByGrade;  // +0x90
		BINARY_TREE_MAP				str_theMeshPathByClass;
		GAME_STRUCT_ARRAY_POINT		str_theEventOptionList;
		GAME_STRUCT_ARRAY_POINT		str_theMasteryStatusIds;
		GAME_STRUCT_ARRAY_POINT		str_theSubEquipParts;
		GAME_STRUCT_ARRAY_POINT		str_theHideMeshTypes;
		DWORD						str_dwColorChannelCount;
		GAME_STRUCT_ARRAY_POINT		str_theChannelColors;
		GAME_STRING_ID				str_theEquipmentSetInfoId;
		GAME_STRUCT_ARRAY_POINT		str_theOverrideProjectileByTag;	// +0x120
		GAME_STRUCT_ARRAY_POINT		str_theSkillCoolDownInfo;
		GAME_STRING_ID				str_theAdditionalUnsealItemType; // +0x140
		struct
		{
			DWORD				str_dwVFunAddr;
			DWORD				str_dwBaitSlotCount;
			BINARY_TREE_MAP		str_theStatus;
			GAME_STRUCT_ARRAY_POINT	str_theFishingEventOptions;
		}theFishingInfo;	// 1230新增
	}theEquipInfo;		// +168		装备信息

	struct  
	{
		DWORD						str_dwVFunAddr;
		BINARY_TREE_MAP				str_thePeriodStatus;
		BINARY_TREE_MAP				str_thePeriodSkillBonus;
		GAME_STRUCT_ARRAY_POINT		str_thePeriodEventOptionList;
		GAME_STRING_ID				str_thePeriodOptionExclusiveClassId;	// 10.30不删档新增数据段
		struct
		{
			DWORD					str_dwVFunAddr;
			DWORD					str_dwExpirationType;
			struct
			{
				DWORD				str_dwVFunAddr;
				DWORD				str_dwYear;
				DWORD				str_dwMonth;
				DWORD				str_dwDay;
				DWORD				str_dwHour;
				DWORD				str_dwMinute;
			}theDate;
		}theExpire;
	}theExtendInfo;			// +2EC		扩展信息

	
	struct 
	{
		DWORD				str_dwVFunAddr;
		BYTE				str_bEnable;						// 启用标志
		BYTE				str_bCanUseInVillage;				// 是否可以再城镇中使用
		WORD				str_wZeor1;
		GAME_STRING_ID		str_theCooldownCategory;			// 鉴定书：识别(Identify)
		DWORD				str_dwUseEffect;					// 使用效果
		DWORD				str_dwTargetLimitItemLevel[2];		// 限制目标物品等级	鉴定书：鉴定物品等级
		DWORD				str_dwTargetLimitGrade[2];			// 限制目标可精炼等级
		DWORD				str_dwLimitLevel[2];				// 限制等级
		DWORD				str_dwLimitTierAttackPower[2];		// 限制攻击强度
		BINARY_TREE_MAP		str_theParams;
		GAME_STRUCT_ARRAY_POINT		str_thePlayerInfoStates;
		GAME_STRUCT_ARRAY_POINT		str_theUnKnow2;
		GAME_STRUCT_ARRAY_POINT		str_theBuffs;
		DWORD						str_dwUsableType;
		GAME_STRING_ID				str_theCinematicDialogInfoId;
		GAME_STRING_ID				str_theAcceptQuestId;
		GAME_STRUCT_ARRAY_POINT		str_theExceptedBuffIds;
	}theUsableInfo;		// +35C		可用信息

	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwEnable;					// 启用标志
		DWORD				str_dwEnhanceType;
		DWORD				str_dwInitialEnhanceLevel;
		BINARY_TREE_MAP		str_theEnhanceFactors;
	}theEnhanceInfo;		// +3E4		加强信息
	BYTE			str_bAutoComplete;
	BYTE			str_bIsCash;
	BYTE			str_bCanBeCombineOutput;
}SDB_ITEM_INFO,*PSDB_ITEM_INFO;		// ItemInfo




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 技能模板
typedef struct  
{
	NODE			str_theNode;
	DWORD			str_dwId;
	DWORD			str_dwObjAddr;
	DWORD			str_dwObjAddrPoint;
	DWORD			str_dwUnKnowFlag;
}SDB_INDEX,*PSDB_INDEX;

typedef struct
{
	GAME_STRING_VF_ID		str_theId;
	GAME_STRING_VF_ID		str_theName;
	DWORD					str_dwNeedTarget;						// 需要目标
	DWORD					str_dwMaxRangeOnTarget;					// 最大目标范围
	GAME_STRING_VF_ID		str_theDesc;							// 技能说明
	DWORD					str_dwElementalAttribute;				// 元素属性  对应封包攻击时 "FIRE"  ElementalAttribute
	DWORD					str_dwTargetTypes;						// 目标类型
	DWORD					str_dwSkillType;						// 技能类型 : 主动技能=0;被动技能=1;特殊技=2;
	DWORD					str_dwTargetInfo[6];					// 目标信息
	GAME_STRUCT_ARRAY_POINT		str_theSkillInfoPerLevels;			// 技能各等级详细信息
	GAME_STRUCT_ARRAY_POINT		str_thePrecedingSkills;				// 前置技能, 学习技能时要用来做判断的
	GAME_STRUCT_ARRAY_POINT		str_theSecondaryAwakeningSkills;	// 二次觉醒技能
	BYTE					str_bEnableAutoTarget;					// 自动使用目标
	BYTE					str_OneClickUse;						// 点击使用
	WORD					str_wZeor1;
	FLOAT					str_fAutoTargetDistanceRangeX;			// 自动目标范围
	FLOAT					str_fAutoTargetDistanceRangeY;	
	FLOAT					str_fAutoTargetAngleRangeX;				// 自动目标三角范围
	FLOAT					str_fAutoTargetAngleRangeY;
	BYTE					str_bAutoTargetingIsOnlyFirstAction;
	BYTE					str_bEnableAOEAutoTarget;	
	WORD					str_wZeor2;
	FLOAT					str_fAOEAutoTargetDistanceRangeX;
	FLOAT					str_fAOEAutoTargetDistanceRangeY;
	FLOAT					str_fAOEAutoTargetAngleRangeX;
	FLOAT					str_fAOEAutoTargetAngleRangeY;
	GAME_STRING_ID			str_theIconPath;
	DWORD					str_dwZeor1;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwEnable;
		GAME_STRING_ID		str_theScriptFuncName;		// 脚本函数的名字
	}theSkillActivateCondition;
	struct
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theUnKnow;
	}theUnKnow1;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwEnable;				// 启用
		DWORD				str_dwLevel;
		DWORD				str_dwRadius;				// 半径范围
		DWORD				str_dwMaxRange;				// 最大范围
		DWORD				str_dwCheckSectorLimit;
		GAME_STRING_VF_ID	str_theAoeVisualId;			// 效果范围视觉id
	}theAoeInfo;
	GAME_STRUCT_ARRAY_POINT	str_theOverrideAoeInfos;	// 覆盖效果范围信息
	DWORD					str_dwEnableSkillUseWhenCooldown;		// 使技能冷却时使用
	GAME_STRING_ID			str_theCommandTag;						// 命令标签
	BYTE					str_bIsDeadlySkill;						// 是否为致命技能
	DWORD					str_dwDeadlySkillFullGaugeSpending;
	GAME_STRING_ID			str_theDeadlySkillId;
	BYTE					str_bAffectedByAwaken;
	BYTE					str_bIsAwakeningSkill;
	BYTE					str_bBeAwakenToLearn;
	BYTE					str_bIsGuildSkill;
	GAME_STRING_ID_			str_theMoviePath;
	struct  
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theInstructionDesc;
	}theInstructionDesc;
	GAME_STRUCT_ARRAY_POINT	str_theIncompatibleSkills;
	DWORD					str_dwPvpPrecooldown;
	DWORD					str_dwPvpAttackPowerRate;
}SDB_SKILL,*PSDB_SKILL;

typedef struct
{
	DWORD					str_dwVFunAddr;
	DWORD					str_dwLevel;				// 技能等级
	DWORD					str_dwMinLevelToLearn;		// 学习技能需要的角色等级
	DWORD					str_dwSpToLearn;			// 学习技能需要的sp点
	DWORD					str_dwGoldToLearn;			// 学习技能所需金币
	DWORD					str_dwAwakeningPointToLearn;// 学习技能所需觉醒点 
	DWORD					str_dwTierAttackPowerToLearn;	// 10.30不删档新增字段
	GAME_STRING_ID			str_theActionId;
	FLOAT					str_fConsumeHp;				// 消耗hp
	FLOAT					str_fConsumeMp;				// 消耗mp
	FLOAT					str_fConsumeRage;			// 消耗愤怒
	FLOAT					str_fConsumeBloodPoint;		// 消耗血值
	FLOAT					str_fAvailablePeriod;		// 使用时间
	FLOAT					str_fCooldown;				// 冷却时间
	FLOAT					str_fReusingInterval;		// 重用间隔
	FLOAT					str_fAttackPowerRate;		// 攻击速度，封包伤害攻击需要用到
	GAME_STRUCT_ARRAY_POINT		str_theActionIdList;
	BINARY_TREE_MAP				str_theDescParams;			// 攻击力和攻击次数
	BINARY_TREE_MAP				str_theParams;
	GAME_STRUCT_ARRAY_POINT		str_theIncreaseStatus;		// 增加状态
	GAME_STRING_ID				str_theOverrideActionId;
	GAME_STRUCT_ARRAY_POINT		str_theEventOptionIds;
	GAME_STRUCT_ARRAY_POINT		str_theReduceCooldowns;
	DWORD						str_dwIncreaseStatusByEquipCategory;
	GAME_STRING_VF_ID			str_theUnKnow1;
	DWORD						str_dwUnKnow1[3];
}SKILL_INFO_PER_LEVEL,*PSKILL_INFO_PER_LEVEL;		// SkillInfoPerLevel


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 商品模板
typedef struct
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theId;
	DWORD				str_dwShopType;
	GAME_STRUCT_ARRAY_POINT		str_theTargetVillages;
	GAME_STRING_ID				str_theArenaId;
	GAME_STRUCT_ARRAY_POINT		str_theSellItems;
	GAME_STRUCT_ARRAY_POINT		str_theSkillTrainClasses;
	BINARY_TREE_MAP				str_theCraftItems;
	DWORD						str_dwItemCategoryInfos[5];	// 未知数据类型
}SDB_SHOP_INFO,*PSDB_SHOP_INFO;

typedef struct			// Apple::Sdb::ShopInfo::SellItem
{
	DWORD				str_dwVFunAddr;
	DWORD				str_dwId;				// 十六进制id
	GAME_STRING_ID		str_theOutputItemId;	// 物品id
	struct
	{
		DWORD			str_dwVFunAddr;
		DWORD			str_dwGold;				// 金币
		DWORD			str_dwFriendPoint;		// 友情点
		DWORD			str_dwGuildCoin;		// 公会硬币
		DWORD			str_dwMileage;			// 10.30新增数据段
		GAME_STRUCT_ARRAY_POINT			str_theCrests;		// 其他要求
	}thePriceInfo;	// PriceInfo
	DWORD				str_dwRequiredLevelRange[2];			// 要求等级范围
	DWORD				str_dwRequiredTierAttackPowerRange[2];	// 要求攻击强度范围
	DWORD				str_dwRequiredGuildLevelRange[2];		// 要求公会等级范围
	GAME_STRING_ID		str_theOverrideSealed;
	GAME_STRING_ID		str_theItemCategory;
	LONGLONG			str_llVisibleClassType;				// 10.30 新增字段
	DWORD				str_dwRegistTime[4];				// 10.30 新增字段
	DWORD				str_dwUnRegistTime[4];				// 10.30 新增字段
	DWORD				str_dwRepeatTimes[4];				// 10.30 新增字段
	LONGLONG			str_llLockType;						// 10.30 新增字段
}SELL_ITEM,*PSELL_ITEM;		// SellItem

typedef struct
{	
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theId;
	DWORD				str_dwCount;
}CREST_INFO,*PCREST_INFO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
typedef struct  
{
	DWORD					str_dwVFunAddr;
	GAME_STRING_ID			str_theId;				// 各职业分支id
	GAME_STRING_ID			str_theApDisplayType;
	GAME_STRING_VF_ID		str_theClassName;		// 中英文映射
	GAME_STRING_ID_			str_theModelPath;
	GAME_STRING_ID_			str_theVillageSimplifiedModelPath;
	GAME_STRING_ID_			str_theStageSimplifiedModelPath;
	XYZ						str_theScale;
	BYTE					str_theIsCreateableClass;		// 可创建角色职业 = 1 ，不可创建角色职业 = 0
	FLOAT					str_fJumpHeight;
	FLOAT					str_fJumpGravity;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwType;
		FLOAT				str_fRadius;
		FLOAT				str_fHeight;
		XYZ					str_theBoxSize;
	}theCollisionShape;
	GAME_STRING_ID			str_theSkinType;		// +F0
	GAME_STRING_ID			str_theActionListId;				// 职业类型id
	GAME_STRING_ID			str_theStageDefaultActionId;		// 副本默认状态
	GAME_STRING_ID			str_theVillageDefaultActionId;		// 城镇默认状态
	GAME_STRING_ID			str_theLevelUpActionId;
	GAME_STRING_ID			str_theClassChangeActionId;			// +17C
	GAME_STRING_ID			str_theClassSelectSoundEventId;
	GAME_STRING_ID			str_theClassCreateSoundEventId;
	BINARY_TREE_MAP			str_theDefaultStatus;						// +1D0	 默认角色状态
	GAME_STRUCT_ARRAY_POINT	str_theEquipCategories;						// +1E0  装备类型， 即职业可穿戴的装备类型
	FLOAT					str_fSpecialAttackTargetingAngleRange[2];
	BINARY_TREE_MAP			str_theSkillCategories;						// +1F8  职业分支
	BINARY_TREE_MAP			str_theSpecialAttackSettings;				// +208  特殊攻击设置
	GAME_STRUCT_ARRAY_POINT	str_theEffects;
	GAME_STRUCT_ARRAY_POINT	str_theDeadlySkillReadyEffects;
	BINARY_TREE_MAP			str_theDefaultMeshInfos;		// +238
	GAME_STRUCT_ARRAY_POINT	str_theDefaultBuffInfos;
	GAME_STRUCT_ARRAY_POINT	str_theDefaultSkills;			// +258  默认技能
	GAME_STRUCT_ARRAY_POINT	str_theAwakenDefaultSkills;		// +268  默认觉醒技能
	BINARY_TREE_MAP			str_theDefaultEquipment;		// +278	 默认装备
	BINARY_TREE_MAP			str_theDefaultQuickSlot;		// +288  默认快捷栏技能
	GAME_STRING_ID			str_theDefaultQuest;			// +298	 默认任务id	
	GAME_STRING_ID			str_theDefaultStage;			// +2B0	 默认副本id
	struct  
	{
		DWORD				str_dwVFunAddr;
		struct  
		{
			DWORD			str_dwVFunAddr;
			GAME_STRING_ID	str_theBrief;
		}theBrief;
		GAME_STRING_ID		str_theDifficulty;
	}theDescription;		// +2D0
	GAME_STRING_ID			str_theStartModeForStage;					// +310		副本模式为攻击模式
	GAME_STRING_ID			str_theStartModeForVillage;					// +32C		城镇模式为和平模式
	GAME_STRUCT_ARRAY_POINT	str_theRandomModeChange;
	GAME_STRING_ID_			str_theCriticalHitSoundPath;
	GAME_STRUCT_ARRAY_POINT	str_theOverrideActions;
	struct
	{
		DWORD				str_dwVFunAddr;
		BINARY_TREE_MAP		str_theCustomizingPartInfos;
		GAME_STRUCT_ARRAY_POINT	str_theSelectableSkinColors;
	}theCustomizingInfo;												// +388
	struct
	{
		DWORD				str_dwVFunAddr;
		BINARY_TREE_MAP		str_theCustomizingPartDatas;
		DWORD				str_dwSelectedSkinColorIndex;
	}theCustomizingData;												// +3AC
	FLOAT					str_fWeaponDurabilityDecreaseRate;
	FLOAT					str_fArmorDurabilityDecreaseRate;
	FLOAT					str_fAccessoryDurabilityDecreaseRate;
	struct
	{
		DWORD				str_dwVFunAddr;
		BYTE				str_bUseHpInsteadOfMp;
		GAME_STRUCT_ARRAY_POINT		str_theAddBuffs;
		GAME_STRUCT_ARRAY_POINT		str_theRemoveBuffs;
		GAME_STRUCT_ARRAY_POINT		str_theBuffListToBeErased;
	}theClassAwakenInfo;		// +3D0
	struct  
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theGaugeType;
		DWORD				str_dwGaugeMaxValue;
	}theSpecialGaugeInfo;		// +408
}SDB_CLASS_INFO,*PSDB_CLASS_INFO;

typedef struct
{
	NODE					str_theNode;
	GAME_STRING_ID			str_theId;
	struct
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theId;
		GAME_STRING_VF_ID	str_theName;
		BINARY_TREE_MAP		str_theSkills;
	}theSkillCategory;
	DWORD					str_dwUnKnow1;
}SDB_SKILL_CATEGORY,*PSDB_SKILL_CATEGORY;

typedef struct
{
	NODE					str_theNode;
	GAME_STRING_ID			str_theId;
	GAME_STRING_ID			str_theId2;
	DWORD					str_dwMaxSkillLevel;
	DWORD					str_dwUnKnow2;
}SKILL_CATEGORY,*PSKILL_CATEGORY;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 副本和城镇之间的映射关系
typedef struct
{
	DWORD					str_dwVFunAddr;
	GAME_STRING_ID			str_theId;
	GAME_STRING_VF_ID		str_theName;
	GAME_STRING_ID_			str_theWorldMapImagePath;
	GAME_STRING_ID_			str_theStageSelectBackgroundImagePath;
	GAME_STRUCT_ARRAY_POINT		str_theStageIds;					// 副本和城镇映射
	GAME_STRUCT_ARRAY_POINT		str_theStageGroups;
	DWORD						str_dwMinLevelToEnter;
}SDB_REGION_INFO,*PSDB_REGION_INFO;		// Apple::Sdb::RegionInfo



// 角色静态信息
typedef struct
{
	GAME_STRING_ID				str_thePlayerName;
	DWORD						str_dwSerial;

	struct
	{
		DWORD						str_dwVFunAddr;
		DWORD						str_dwUnKnow1;
		struct
		{
			DWORD					str_dwVFunAddr;
			GAME_STRING_ID			str_thePlayerName;
			LONGLONG				str_llPlayerSerial;
			DWORD					str_dwPlayerGender;
			DWORD					str_dwClassType;			// 剑斗士 = 1,斩狂 = 3, 控魔师 = 9 和创建角色时的ClassType相同
			DWORD					str_dwCreatedDateTime[4];
			BYTE					str_bIsDeleted;
			DWORD					str_dwUnKnow2;
		}theStaticPlayerInfo;
		struct
		{
			DWORD					str_dwVFunAddr;
			DWORD					str_dwGameLv;
			DWORD					str_dwGameXp;
			DWORD					str_dwUnKnow3[2];
			DWORD					str_dwGold;
			DWORD					str_dwFriendPoint;
			BYTE					str_bFriendApplyDenyOption;
			DWORD					str_dwFriendApplyDenyOption;
			DWORD					str_dwResurrectionStoneCount_PcCafe;
			DWORD					str_dwResurrectionStoneCount_Daily;
			DWORD					str_dwResurrectionStoneCount;
			DWORD					str_dwTalentLv;
			DWORD					str_dwFatigue;
			DWORD					str_dwEventFatigue;
			DWORD					str_dwPremiumFatigue;
			DWORD					str_dwMembershipFatigue;
			DWORD					str_dwSpentFatigue;
			DWORD					str_dwTierAttackPower;
			// ....
		}theDynamicPlayerInfo;
		// ....
	}thePlayerInfo;

}PLAYER_INFO,*PPLAYER_INFO;

typedef struct  
{
	DWORD				str_dwClassType;		// 这里就是对应的class类型
	GAME_STRING_ID		str_theClass;			// 比如剑魔，斩狂，剑斗士的英文大写类型名 如： "WARRIOR" "GLADIBERSERKER" 
}CALSS_CLASSTYPE_,*PCALSS_CLASSTYPE_;

typedef struct
{
	CALSS_CLASSTYPE_ theClass[20];		// 这里大小未知，但是给20够了
}CALSS_CLASSTYPE,*PCALSS_CLASSTYPE;

typedef struct
{
	NODE			str_theNode;
	DWORD			str_dwNullAddr;
	GAME_STRING_ID_	str_theId;
	DWORD			str_dwVFunAddr;
	DWORD			str_dwLevel;
}FB_LEVEL,*PFB_LEVEL;

typedef struct 
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theSectorId;
	GAME_STRING_VF_ID	str_theMapId;
}MAP_INFO,*PMAP_INFO;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID_		str_theId;
}BOOS_INFO,*PBOOS_INFO;

typedef struct 
{
	DWORD			str_dwVFunAddr;
	DWORD			str_dwObjAddr;
	DWORD			str_dwObjAddrPoint;
	GAME_STRING_ID	str_theId;
	DWORD			str_dwLevel;
	DWORD			str_dwUnKnow2;
	BINARY_TREE_MAP	str_theAlreadyClearSector;		// 已经被清理掉的副本，包括刚打完开门后的房间
}MAP_DATA,*PMAP_DATA;

typedef struct
{
	NODE			str_theNode;
	GAME_STRING_ID	str_theId;
}CLEAR_SECTOR,*PCLEAR_SECTOR;		// 被清空的房间

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theTaskNpcId;
	DWORD				str_dwNpcTaskObjAddr;
	DWORD				str_dwNpcTaskObjPoint;
	DWORD				str_dwAccpetFlag;			// 一个未知的标志
}TASK_NPC_OBJ,*PTASK_NPC_OBJ;

typedef struct  
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theId;
}CAN_ACCPET_NPC_TASK,CAN_COMPLETE_NPC_TASK,*PCAN_ACCPET_NPC_TASK,*PCAN_COMPLETE_NPC_TASK;

typedef struct  
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theId;
	DWORD				str_dwLevel;
	DWORD				str_dwBonusPoint;
	GAME_STRUCT_ARRAY_POINT		str_theEventOptions;
	LONGLONG					str_llOidSkill;
	DWORD						str_dwBonusCooldown;
	DWORD						str_dwSkillId;
}APPLE_SKILL,*PAPPLE_SKILL;

typedef struct
{
	NODE	theNode;
	DWORD	dwFlag;
	GAME_STRUCT_ARRAY_POINT	theSkillArrayAddr;
	bool    bUnKnow;
	bool	bFlag;
}SKILL_OBJ_ADDR,*PSKILL_OBJ_ADDR;


typedef struct		// Apple::Item
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theItemId;					// 物品id
	GAME_STRING_ID_		str_thePrefixOptionName;		// 物品名称
	LONGLONG			str_llSerial;					// 物品序号
	DWORD				str_dwItemObjAddr;				// 物品模板对象
	DWORD				str_dwItemObjPoint;				// 物品模板对象指针对象
	DWORD				str_dwStackCount;				// 物品存在数量
	GAME_STRING_ID		str_theGradeId;					// 物品品质
	FLOAT				str_fDurability;				// 物品耐久
	GAME_STRUCT_ARRAY_POINT		str_theIncreaseStatusList;		// +74 装备属性
	GAME_STRUCT_ARRAY_POINT		str_theEventOptions;			// +84
	GAME_STRUCT_ARRAY_POINT		str_theIdentifiedOptions;		// +94 鉴定属性
	DWORD						str_dwUnKnow1;
	struct
	{
		DWORD					str_dwVFunAddr;
		DWORD					str_dwVersion;
		DWORD					str_dwUnBindCount;
		DWORD					str_dwReidentifyCount;
		DWORD					str_dwEventOptionChangeCount;
		DWORD					str_dwAddBonusLevel;
		DWORD					str_dwExpirationTime[4];
		GAME_STRUCT_ARRAY_POINT	str_theMasteryStatusIds;
		DWORD					str_dwRebalanceVersion;		// 10.30新增数据段
	}theExtendData;	// +A8
	DWORD						str_dwUnKnow2;		// 10.30新增未知数据
	BYTE						str_bIsSealed;
	BYTE						str_bIsUnidentified;
	BYTE						str_bIsTradable;
	DWORD						str_dwGoldValue;
	BINARY_TREE_MAP				str_theStatus;					// +E8	装备基本属性
	BINARY_TREE_MAP				str_theEnhanceFactors;
	// ...
}APPLE_ITEM,*PAPPLE_ITEM;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theStatusId;
	FLOAT				str_fValue;
}ITEM_STATUS_VALUE,*PITEM_STATUS_VALUE;

typedef struct
{
	DWORD				str_dwVFunAddr;
	GAME_STRING_ID		str_theStatusId;
	DWORD				str_dwIncreaseType;
	FLOAT				str_fValue;
	FLOAT				str_fRelativeRate;
}ITEM_STATUS_VALUE_,*PITEM_STATUS_VALUE_;		// "Apple::IncreaseStatus"

typedef void (*PFUN_PRINT)(DWORD lpSubTreeRoot);

void Traverse( DWORD lpSubTreeRoot, DWORD dwNullNode, PFUN_PRINT pFunPrint);

typedef struct
{
	NODE		theNode;
	DWORD		dwSerial;
	DWORD		dwLocalObjAddr;
	DWORD		dwLocalObjPointAddr;
}PREV_ACTION_PLAY_TIME_1,*PPREV_ACTION_PLAY_TIME_1;

typedef struct
{
	DWORD		dwLocalObjAddr;
	DWORD		dwSerial;
	BINARY_TREE_MAP	theMapAddr;
}PREV_ACTION_PLAY_TIME_2,*PPREV_ACTION_PLAY_TIME_2;

typedef struct
{
	NODE		theNode;
	DWORD		dwUnKnow1;
	DWORD		dwLocalObjAddr;
	DWORD		dwLocalObjPointAddr;
}PREV_ACTION_PLAY_TIME_3,*PPREV_ACTION_PLAY_TIME_3;

#endif  //_CLASSA_H_