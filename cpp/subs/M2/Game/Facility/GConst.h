
#pragma once
/*
@author zhp
@date 2017/2/5 22:36
@purpose for game const variable
*/
#include <string>
#include <CD_CommonFiles/Data/CD_Interface.h>

//角色名最大长度
const pt_dword kRoleNameMaxCnt = 20;
const int kCellMaxCnt = 8;	//物品所占格子数量
const DWORD kMonsterInvalidPacketId = std::numeric_limits<DWORD>::max();
//game object type
enum GameObjType{
	kGOT_Npc,
	kGOT_Monster,
	kGOT_Player,			//玩家
	kGOT_Waypoint,			//传送点
	kGOT_AreaTransition,	//传送门
	kGOT_Stash,				//仓库
	kGOT_NoticeBoard,		//告示板
	kGOT_NoticeBoardPvP,	//活动告示板
	kGOT_Chest,				//箱子 流放者的遗体
	kGOT_WorldItem,			//掉落物		
	kGOT_Transitonable,		//可传送对象
	kGOT_Max,
	kGOT_Invalid,
};

//物品类别,item category
enum enItemCate{
	kICATE_Usable,
	kICATE_Flask,
	kICATE_Decoration,	//饰品
	kICATE_Ring,
	kICATE_Armor,		//防具
	kICATE_Weapon,
	kICATE_Gem,
	kICATE_Quest,		//任务物品
	kICATE_Other,
	kICATE_Invalid,
};

//武器组合类别，顺序不要随便改变
enum enWeaponCompositeCate{
	kWCC_OneHand = 0,		//单手，左、右手都可以装备，可以同时装备两个任意单手
	kWCC_TwoHand,		//双手，只能左手装备，右手不能装备任何东西
	kWCC_Restricted,	//限定位置的，只能放在左手，或只能放在右手，假如是此装备是放在左手的，右手只能装备指定装备
	kWCC_Invalid,
	kWCC_Max,
};

enum enMoveItemToRes{
	kMITR_MoveToThereFailed,
	kMITR_CanNotBePutted,	//背包不允许放入该物品
	kMITR_Fulled,			//已满
	kMITR_Failed,
	kMITR_Succeed,
};

enum enTakeoffGemRes{
	kTOGR_HasNotGem,
	kTOGR_Fulled,
	kTOGR_Failed,
	kTOGR_Succeed,
};

enum enInsideGemRes{
	kIGR_CanNotBeInsided,
	kIGR_DiffColor,
	kIGR_Failed,
	kIGR_Succeed,
};

enum enPutItemDownRes{
	kPIDR_CanNotBePutted,
	kPIDR_Failed,
	kPIDR_Succeed,
};

//顺序不要变
enum enOpenVisitorRes{
	kOVR_NoNeedOpen,
	kOVR_Openning,
	kOVR_Error,
	//以下是成功
	kOVR_Succeed,
	kOVR_NotVisitAgain,
	kOVR_Opened,
};

enum enTalkToNpcRes{
	kTTNR_NpcIsClosed,
	kTTNR_HaveNotTheLable,
	kTTNR_Error,
	kTTNR_Succeed,
	kTTNR_NotNeedHandle,
};

enum en_ItemFilterType
{
	kIFT_QuestItem,
	kIFT_BlackName,
	kIFT_WhiteName,
	kIFT_Slot,
	kIFT_Colour,
	kIFT_Quality,
	kIFT_Type,
	kIFT_GemLink,
	kIFT_ItemCnt,
	KIFT_Level,
	KIFT_Prop,
	KIFT_Bind,
	KIFT_Group,
	KIFT_ThreeColorLink,
	KIFT_Emboitement,
	kIFT_Max,
};

enum en_NpcMonster
{
	kNM_Invalid,
	kNM_BeatFailure,
	kNM_Help,
	kNM_Visit,
};

enum en_GradeValue
{
	kGV_Invalid,
	kGV_EquipColor,
	kGV_SlotLink,
	kGV_SlotColor,
	kGV_Quality,
};

//移动开箱子
const int kMoveOpenChestDistance = 40;
//移动拾取物品最大距离
const int kMovePickupItemDistance = 50;
//移动开门距离。门的距离一定不能小，否则会有不开门而直接放技能的问题
const int kMoveOpenTranstionableDistance = 60;
//打开对象延迟，延迟必须得有，否则太快，服务器就没反应了
//const pt_dword kOpenObjTimeSpan = 600;
//默认找到对象距离
const pt_dword kOpenObjValidDistance = 1000000;
//药瓶最大数量
const pt_dword kFlaskMaxCnt = 5;
//九件套
const pt_dword kEmboitementCnt = 9;

//使用技能结果，枚举顺序不要变
enum enUseSkillRes{
	kUSR_Error,
	kUSR_DstNotValid,
	kUSR_Succeed,
};

enum enStopQuestReason{
	kSQR_Invalid,
	kSQR_AcceptNewQuest,
};
	
//////////////////////////////////////////////////////////////////////
//默认仓库页
const std::string kStashPageDefault[4] = { "1", "2", "3", "4"};
const stCD_VecInt kInvalidVecInt(-1, -1);
const stSizeInt kItemMaxSize(2,4);
//////////////////////////////////////////////////////////////////////////
//***************npc talk label
const std::wstring kNTL_Cotinue = CD_kNTL_Cotinue;
const std::wstring kNTL_Bye = CD_kNTL_Bye;
const std::wstring kNTL_Buy = CD_KNTL_Buy;
const std::wstring kNTL_Sell = CD_KNTL_Sell;
const std::string kNoAutoOpenObj[] = {
"试炼盘",
"升华试炼",
"不朽屏障",
"托麦迪克之印",
"医药箱",
"刻着雕纹的奇怪墙壁",
"迪虚瑞特的封印",
"托尔曼",
"托尔曼的手镯",
"马雷格罗的尖刺",
"黄金之手",
"托麦迪克亚硫酸",
"欧克的古灵之记",
"克雷顿的古灵之记",
"阿莉亚的古灵之记",
"缎带之轴",
"神塔之钥",
"迪虚瑞特的红旗",
"愤怒之眼"
};
//////////////////////////////////////////////////////////////////////////
//***************对象名
const std::string kObjName_Waypoint = "传送点";
const std::string kObjName_Storage = "仓库";
const std::string kObjName_Identify = "知识卷轴"; 
const std::string kObjName_PortalIdentify = "传送卷轴";
const std::string kObjName_DefaultSkill = "普通攻击";
const std::string kObjName_RegretBook = "后悔之书";
const std::string kObjName_SkillBook = "技能之书";
const std::string kObjName_NpcNoVisit = "娜瓦莉";
const std::string kObjName_MapDevice = "地图装置";
//////////////////////////////////////////////////////////////////////////
//***************地图名
const std::string kMapName_JueWangYanTan = "绝望岩滩";
//////////////////////////////////////////////////////////////////////////
//***************任务名
const std::string kTaskName_StartQuest = "初始任务";

//////////////////////////////////////////////////////////////////////////
//**************事件相关
const std::string kEventName_Die = "死亡事件";
const std::string kEventName_Die_ChangeMap = "死亡后变更地图事件";
const std::string kEventName_BagIsFull = "背包已满事件";
//////////////////////////////////////////////////////////////////////////
const enCD_ItemType kEmboitementType[] = {
	kITYPE_Helmet,				//头部
	kITYPE_Amulet,				//项链
	kITYPE_Belt,				//腰带
	kITYPE_Ring,				//戒指
	kITYPE_Boots,				//鞋子
	kITYPE_Gloves,				//手套
	kITYPE_BodyArmour,			//胸甲
	kITYPE_TwoHandSword
};