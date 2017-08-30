
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
const pt_dword kEludeMonHp = 50;
const DWORD kMonsterInvalidPacketId = std::numeric_limits<DWORD>::max();
//game object type
enum enGameObjType{
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
	kGOT_Movement,			//可移动对象
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
	kOVR_Opened,
};

enum enTalkToNpcRes{
	kTTNR_NpcIsClosed,
	kTTNR_HaveNotTheLable,
	kTTNR_Error,
	kTTNR_Succeed,
	kTTNR_NotNeedHandle,
};

enum enItemFilterType{
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
	KIFT_Bind,
	KIFT_Group,
	KIFT_ThreeColorLink,
	KIFT_Emboitement,
	kIFT_ItemProp,
	kIFT_Equip,
	kIFT_Level,
	KIFT_InvalidGem,
	kIFT_Max,
};

enum enGradeValue{
	kGV_Invalid,
	kGV_EquipColor,
	kGV_SlotLink,
	kGV_SlotColor,
	kGV_Quality,
	kGV_Weapon,
	kGV_Prop
};

enum enFilterType{
	kFT_PickUp,
	kFT_Sell,
	kFT_Store,
	KFT_Again,
};

enum enOpenChestFlag{
	kOCF_Enable = 1 << 1,
	kOCF_OnlyShowName = 1 << 2,
};

enum enGroupPropflag{
	kGPF_BaseProp = 1 << 0,
	kGPF_SubjoinProp = 1 << 1,
	kGPF_AllProp = kGPF_BaseProp | kGPF_SubjoinProp,
};

enum enLoginAccountRes{
	kLAR_AcBaned,
	kLAR_AcError,
	kLAR_PwdError,
	kLAR_Timeout,
	kLAR_WaitLoginRes,
	kLAR_Error,
	kLAR_Succeed,
};

//通过一次后，就不再通过了。
enum enAreaPassState{
	kAPS_NotPass,		//不可以再通过
	kAPS_CanPass,		//可以通过
	kAPS_FristPass,		//第一次通过
};
//////////////////////////////////////////////////////////////////////////
//**************模态对话框提示字符。
const std::wstring kDlgModalStr_LoginError = L"Login Error";
const std::wstring kDlgModalStr_PwdError = L"The password is not correct for this account.";
const std::wstring kDlgModalStr_AcError = L"The specified account does not exist.";
const std::wstring kDlgModalStr_AcBaned = L"Your account has been banned by an administrator.";
//////////////////////////////////////////////////////////////////////////

//移动开箱子
const int kMoveOpenChestDistance = 60;
//移动拾取物品最大距离
const int kMovePickupItemDistance = 60;
//移动开门距离。门的距离一定不能小，否则会有不开门而直接放技能的问题
const int kMoveOpenTranstionableDistance = 70;
//默认找到对象距离
const pt_dword kOpenObjValidDistance = 1000000;
//药瓶最大数量
const pt_dword kFlaskMaxCnt = 5;
//九件套
const pt_dword kEmboitementCnt = 9;
//a*
const int kAStar_RectifyRadius = 50;
const int kAStar_CompressRadius = 50;

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

enum enTerrainParserFlag{
	kTPF_Cmp = 1 << 1,
	kTPF_Strstr = 1 << 2,
	kTPF_ObjDbPath = 1 << 3,
};

enum enInventoryItemChecksum{
	kIIC_ItemCnt = 0,
	kIIC_Pos = 8,
	kIIC_ItemName = 12,
	kIIC_ItemId = 15,
	kIIC_Weight = 17,
	kIIC_ItemInventoryType = 21,
	kIIC_BasePro = 24,
	kIIC_ItemValues = 32,
	kIIC_TipsFormatStr = 36,
	kIIC_Price = 40,
	kIIC_ItemTips = 48,
	kIIC_InventoryType = 60,
};
const int kDummyChecksumBagWidth = 6;
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



#ifdef VER_GJ_
const std::string kNoAutoOpenObj[] = {
	"Trial of Ascendancy",
	"Undying Blockage",
	"Thaumetic Seal",
	"Medicine Chest",
	"Strange Glyph Wall",
	"Tolman's Bracelet",	
	"Maligaro's Spike",
	"Golden Hand",
	"Thaumetic Sulphite",
	"Oak's Amulet",
	"Kraityn's Amulet",
	"Alira's Amulet",
	"Allflame",
	"Chitus' Plum",	
	"The Eye of Fury",
	//"The Black Core",
	"Hinekor's Hair",
	"Tukohama's Tooth",
	"Bestel's Manuscript",
	"Miasmeters",
	"Cradle of Purity",
	"Valako's Jaw",
	"The Black Flag",
	"Maligaro's Map",
	"Map Device",
	"Silver Locket",
	"Firefly",
	"Moon Orb",
	"Sun Orb",
	"Statue of The Sisters",
	"Locked Door",
	"Switch",
	"Portal",
	"Sign of Purity",
	//"Fortress Gate",
	//"Eye of Conquest",
	"Ignition Switch",
	"Secret Passage",
	"Ankh of Eternity",
	"Timed Crank",
	"Basilisk Acid",
};

//****************过房间不需要检测新房间 --国际
const std::string kRoomConnectFilter[] = {
	"Sanctum of Innocence",
	"Caldera of The King",
	"The Black Core",
	"The Black Heart",
	"Cathedral Apex",
	"Tukohama's Keep",
	"Prison Rooftop",
	"The Brine King's Throne",
	"Maligaro's Workshop",
	"Portal",
	"The Spawning Ground",
	"The Fortress Encampment",
	"Arakaali's Web",
	"The Cauldron",
	"The Sky Shrine",
	"Arena",
	"Maligaro's Misery",
	"Shavronne's Sorrow",
	"Doedre's Despair",
	"The Black Heart"
};
#else
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
	"不灭之火",
	"缎带之轴",
	"神塔之钥",
	"迪虚瑞特的红旗",
	"愤怒之眼",
	"纯净之印",
	"征服之眼",
	"点火开关",
	"马雷格罗的地图",
	"上锁的门",
	"阿兹莫里的神殿",
	"萤火虫",
	//"阿拉卡力的巨网",
	"蛇怪酸液"
};

//****************过房间不需要检测新房间 --国服
const std::string kRoomConnectFilter[] = {
	"织网者巢穴",
	//"传送门",
	"神权之塔(顶层)",
	"熔岩之环",
	"黑灵之核",
	"黑灵之心",
	"纯净圣地",
	"教堂之尖",
	"图克哈玛的要塞",
	"禁灵之狱(顶层)",
	"引火者山谷",  //监狱大门地图的房间
	"割裂者之径",
	"噬魂者山谷",
	"海王皇座",
	"雷格罗的工作间",
	"旧森林营地",
	"巨型锅炉",
	"天空神殿",
	"马雷格罗的苦痛",
	"薛朗的悔恨",
	"德瑞的绝望",
	"阿拉卡力的巨网"
};
#endif

//////////////////////////////////////////////////////////////////////////
//***************lua键值
const std::string kLuaKeyNameItemType = "物品类型";
const std::string kLuaKeyNamePropInfo = "属性信息";
const std::string kLuaKeyItemLevel = "物品等级";

//****************属性名
const std::string kPropQualityName = "品质";



//***************对象名
#ifdef VER_GJ_			//国际服
const std::string kObjName_Waypoint = "Waypoint";
const std::string kObjName_Storage = "Stash";
const std::string kObjName_Identify = "Scroll of Wisdom";	//知识卷轴
const std::string kObjName_PortalIdentify = "Portal Scroll"; //传送卷轴
const std::string kObjName_DefaultSkill = "Default Attack";  //普通攻击
const std::string kObjName_NpcNoVisit = "Navali";
const std::string kObjName_RegretBook = "Book of Regrets";
const std::string kObjName_SkillBook = "Book of Skill";
const std::string kObjName_MapDevice = "Map Device";
const std::string kObjName_HideoutMapDevice = "Map Device";
const std::string kObjName_LabyrinthDevice = "Labyrinth Activation Device";		//迷宫激活设备
const std::string kObjName_LabyrinthTrialAtCity = "Ascendancy Plaque";			//第3章城镇试练面板
const std::string kObjName_LabyrinthTrialAtFb = "Trial of Ascendancy Plaque";
const std::string kObjName_JinGuShiXiang = "禁锢石像";
#else					//国服
const std::string kObjName_Waypoint = "传送点";
const std::string kObjName_Storage = "仓库";
const std::string kObjName_Identify = "知识卷轴"; 
const std::string kObjName_PortalIdentify = "传送卷轴";
const std::string kObjName_DefaultSkill = "普通攻击";
const std::string kObjName_RegretBook = "后悔之书";
const std::string kObjName_SkillBook = "技能之书";
const std::string kObjName_NpcNoVisit = "娜瓦莉";
const std::string kObjName_MapDevice = "地图装置";
const std::string kObjName_HideoutMapDevice = "黄金制图仪";
const std::string kObjName_LabyrinthTrialAtCity = "试炼盘";			//第3章城镇试练面板
const std::string kObjName_LabyrinthTrialAtFb = "Trial of Ascendancy Plaque";
const std::string kObjName_LabyrinthDevice = "Labyrinth Activation Device";		//迷宫激活设备
const std::string kObjName_JinGuShiXiang = "禁锢石像";
#endif

//////////////////////////////////////////////////////////////////////////
//***************地图名
#ifdef VER_GJ_
const std::string kMapName_JueWangYanTan = "The Twilight Strand";  //绝望岩滩
const std::string kMapName_CangShenChu = "Your Hideout";
const pt_cwsz kMapName_ShiYanShouWang = L"Lioneye's Watch";		//狮眼守望
const pt_cwsz kMapName_NanBuSenLin = L"The Southern Forest";
const pt_cwsz kMapName_SaEnChenFeiXu = L"The City of Sarn";
const pt_cwsz kMapName_TongDao = L"Passageway";
const pt_cwsz kMapName_CitySarn = L"The City of Sarn";  //萨恩废墟
const std::string kMapName_Aspirants = "Aspirants' Plaza"; 
const std::string kMapName_Hifhgate = "Highgate";
const std::string kMapName_BellyBeast = "The Belly of the Beast Level 2";
const std::string kMapName_DeHuanJing = "Daresso's Dream";

//*****************怪物名
const pt_cwsz kMonName_WaErChaoLing = L"Vaal Oversoul";  
//const pt_cwsz kMonName_DominusAscendant = L"Dominus Ascendant";  
const pt_cwsz kMonName_DominusHigh = L"Dominus, High Templar";  
const pt_cwsz kMonName_Weaver = L"The Weaver"; 

//****************Buff名
const std::string kBuffName_GracePeriod = "Grace Period";
const std::string kBuffName_SummonRagingSpirit = "Summon Raging Spirit";
//任务名字
const std::string kTaskName_Labyrinth = "The Lord's Labyrinth";		//过滤掉试炼迷宫任务

#else
const std::string kMapName_JueWangYanTan = "绝望岩滩";
const std::string kMapName_CangShenChu = "藏身处 - ";
const pt_cwsz kMapName_ShiYanShouWang = L"狮眼守望";
const pt_cwsz kMapName_NanBuSenLin = L"南部森林";
const pt_cwsz kMapName_SaEnChenFeiXu = L"萨恩城废墟";
const pt_cwsz kMapName_TongDao = L"通道";
const pt_cwsz kMapName_CitySarn = L"萨恩城废墟"; 
const std::string kMapName_Hifhgate = "统治者之殿";
const std::string kMapName_BellyBeast = "巨兽沼泽 2 层";
const std::string kMapName_DeHuanJing = "德瑞索的幻境";
const std::string kMapName_YULingZhiShi = "育灵之室";
const std::string kMapName_laboratory = "圣堂教团实验室";
const std::string kMapName_GangwanBridgey = "港湾大桥";

const std::string kMapName_Aspirants = "试炼者广场";	//试炼地图
const pt_cwsz kMonName_WaErChaoLing = L"瓦尔超灵";  
const pt_cwsz kMonName_DominusHigh = L"Dominus, High Templar";
const pt_cwsz kMonName_Weaver = L"The Weaver";
const std::string kMonName_HateFlag = "憎恨旗帜";
const std::string kMonName_parasite = "寄生虫";
const std::string kMonName_LaSaEgg = "瑞斯拉萨的卵";

//****************Buff名
const std::string kBuffName_GracePeriod = "游戏保护未开启";
const std::string kBuffName_SummonRagingSpirit = "召唤愤怒狂灵";
//任务名字
const std::string kTaskName_Labyrinth = "帝王试炼迷宫";		//过滤掉试炼迷宫任务
#endif

//地图位置信息
#ifdef VER_GJ_
const std::string kMapPosName_UpLouTi = "Stairs_up";
const std::string kMapPosName_DownLouTi = "Stairs1_down";
#else
const std::string kMapPosName_UpLouTi = "Stairs_up";
const std::string kMapPosName_DownLouTi = "Stairs1_down";
#endif

//////////////////////////////////////////////////////////////////////////
//***************任务名
const std::string kTaskName_StartQuest = "初始任务";

const std::string kMapName_Coast = "The Coast [Update]";  //暂时不知道该地图用处 会出问题 先过滤

//////////////////////////////////////////////////////////////////////////
//**************事件相关
const std::string kEventName_Die = "死亡事件";
const std::string kEventName_Die_ChangeMap = "死亡后变更地图事件";
const std::string kEventName_BagIsFull = "背包已满事件";
const std::string kEventName_ReviveFront = "死亡复活前事件";

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
//////////////////////////////////////////////////////////////////////////
