#pragma once
/*
@author zhp
@date 2014/9/9 17:14
@purpose for struct switch
*/
#include "GClass.h"

struct stSdbItemTemplate{
	GClass::stGameStringEx	str_theId;						// +4
	GClass::stGameStringEx	str_theName;					// +20
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
	bool					str_bCanSellTotheShop;			// 是否可以卖给商店
	BYTE					str_bReusable;
	BYTE					str_bPcCafeExclusive;
	WORD					str_wZeor1;
	DWORD					str_dwBindType;					// 绑定类型
	GClass::stGameString			str_theMarketCategory;			// +D0	市场分类，应该是拍卖分类
	GAME_STRING_ID			str_thePriceCategory;			// +EC  作用分类 即:装备，材料，任务物品等	CONSUME:消耗品
	GClass::stGameString	str_theSoundCategory;			// +108 物品类型 护甲 即:皮革,布甲,重甲等   QUEST:鉴定书
	GAME_STRING_ID			str_theDropModelCategory;		// +124 装备使用部位类型 即:上半身,手部等	QUEST:鉴定书
	GType::enTabCategory					str_dwInventoryTab;				// +140 在背包栏所属分类  普通=0,消耗品=1 详细数据见遍历物品
	DWORD					str_dwSellPrice;				// 1230新增

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
		bool				str_bEnable;			// 启用标志
		BYTE				str_bZeor1;				// 内存对齐
		WORD				str_wZeor2;				// 内存对齐
		GType::enEquipPart	str_dwEquipPart;		// 装备使用部位，即发包穿装备时需要用到的 EquipSlot
		DWORD				str_dwLimitLevel[2];	// 限制等级
		DWORD				str_dwLimitMinLevelMinus;	// 10.30不删档增加新的字段
		FLOAT				str_fMaxDurability;		// 最大耐久
		bool				str_bIdentified;		// 鉴定
		bool				str_bSealed;			// 封印
		BYTE				str_bIsTemplateItem;	// 是否可以交易 Rradable 1:可卖商店的普通装备  0:极品装备不可卖商店(不是100%确定)
		BYTE				str_bZeor4;
		GClass::stGameString		str_theEquipCategory;	// 装备类别 即:皮革,布甲,重甲等
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
		bool				str_bCanUseInVillage;				// 是否可以再城镇中使用
		WORD				str_wZeor1;
		GClass::stGameString		str_theCooldownCategory;			// 鉴定书：识别(Identify),Unseal
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
};
static_assert(sizeof(stSdbItemTemplate) == sizeof(SDB_ITEM_INFO), "");