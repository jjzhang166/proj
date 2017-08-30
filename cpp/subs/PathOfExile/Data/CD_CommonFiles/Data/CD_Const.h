#pragma once
/*
@author zhp
@date 2017/1/24 17:21
*/
#include <ProtocolX/P_DefinesX.h>
#include <string>

//state name
const std::wstring CD_StateNamePreGameState = L"PreGameState";
const std::wstring CD_StateNameSelectCharacterState = L"SelectCharacterState";
const std::wstring CD_StateNameCreateCharacterState = L"CreateCharacterState";		//创建角色的时候才有这个状态
const std::wstring CD_StateNameInGameState = L"InGameState";
const std::wstring CD_StateNameAreaLoadingState = L"AreaLoadingState";		//加载地图
const std::wstring CD_StateNameEscapeState = L"EscapeState";				//EscapeMenu弹出的时候
const std::wstring CD_StateNameLoginState = L"LoginState";

//怪物名
#ifdef VER_GJ_
const pt_cwsz kMonTransitionableName[] = {
	L"Azmeri Ancestor",
	L"Doedre the Vile",
	L"Lunaris, Eternal Moon",
	L"Solaris, Eternal Sun",
	L"Threshold of Anger",
	L"Threshold of Hatred",
	L"Threshold of Wrath"
};
#else
const pt_cwsz kMonTransitionableName[] = {
	L"阿兹莫里先人",
	L"恶毒者德瑞",
	L"纯净射手",
	L"纯净剑卫",
	L"纯净守卫", 
	L"硫磺疾行者",
	L"派蒂",
	L"赤龙之炎威尔翰",
	L"荣耀之刃史丹提努斯",
	L"威吓者雷拳奥克塔维亚",
	L"禁卫军",
	L"黑影薛朗",
	L"审判者马雷格罗",
	L"重塑之布鲁特斯",
	L"奇才马雷格罗",
	L"月影守卫者-暮色",
	L"日耀守卫者-黎明",
	L"古灵使徒斗士",
};
#endif

//自动选择网关
const std::wstring CD_AutoSelectGateway = L"Auto-select Gateway";

//特殊对象名
const std::wstring CD_RenderName_CityPortalDoor = L"城镇传送门";
const std::wstring CD_RenderName_YiJieDoor = L"异界之门";

const int CD_kMaxStrLen = 0x100;
const int CD_kMaxNameLen = 50;
const int CD_kMaxFilePathLen = 0x100;

//职业类型
enum enCD_ProfessionType : pt_byte{
	kPT_GuiZu = 0,					//贵族
	kPT_YeManRen = 1,				//野蛮人
	kPT_YouXia = 2,					//游侠
	kPT_NvWu = 3,					//女巫
	kPT_JueDouZhe = 4,				//决斗者
	kPT_ShengTangWuSeng = 5,		//圣堂武僧
	kPT_AnYing = 6,					//暗影
	kPT_Mask = 7,					//总共7个职业
};

//地图系数
const int CD_kMapRatio = 0x17;
//> 0可通过
enum enCD_MapDataFlag : pt_byte{
	//如果为0表示该点无数据
	kMDF_EmptyPoint = 0,		//门一开始也是0，打开后就变成5(也有可能是其他值，得根据当时的地形来决定)
	kMDF_Mountain = 1,			//怀疑是山
	kMDF_Grass = 2,				//草地
	kMDF_LikeGrass = 3,			//比草地要稀疏的地
	kMDF_GroundFlat = 5,		//地面平坦
	kMDF_MaxMask = 0xf,			//最多不会超过0xf
};

//当前地图难度
enum enCD_kMapDifficulty : int{
	kMD_Invalid = 0,	//顺序不要变
	kMD_Normal,			//一般
	kMD_Cruel,			//残酷
	kMD_Merciless,		//无情
	KMD_Max,
};
//////////////////////////////////////////////////////////////////////////
const pt_csz CD_kAttrInfoMonster = "Monster";
const pt_csz CD_kAttrInfoPlayer = "Player";
const pt_csz CD_kAttrInfoClientNpc = "ClientNPC";
const pt_csz CD_kAttrInfoNpc = "NPC";
const pt_csz CD_kAttrInfoMinimapIcon = "MinimapIcon";
const pt_csz CD_kAttrInfoAreaTransition = "AreaTransition";
const pt_csz CD_kAttrInfoBase = "Base";		//用于取物品的基本属性
const pt_csz CD_kAttrInfoQuality = "Quality";
const pt_csz CD_kAttrInfoStack = "Stack";
const pt_csz CD_kAttrInfoAttributeRequirements = "AttributeRequirements";
const pt_csz CD_kAttrInfoMods = "Mods";
const pt_csz CD_kAttrInfoSockets = "Sockets";
const pt_csz CD_kAttrInfoArmour = "Armour";
const pt_csz CD_kAttrInfoSkillGem = "SkillGem";
const pt_csz CD_kAttrInfoRender = "Render";
const pt_csz CD_kAttrInfoPositioned = "Positioned";
const pt_csz CD_kAttrInfoLife = "Life";
const pt_csz CD_kAttrInfoActor = "Actor";
const pt_csz CD_kAttrInfoPathfinding = "Pathfinding";
const pt_csz CD_kAttrInfoChest = "Chest";
const pt_csz CD_kAttrInfoWorldItem = "WorldItem";
const pt_csz CD_kAttrInfoTransitionable = "Transitionable";
const pt_csz CD_kAttrInfoPortal = "Portal";		//使用传送卷轴，点击传送门后，被传送到的地方。其RenderName:城镇传送门
const pt_csz CD_kAttrInfoUsable = "Usable";
const pt_csz CD_kAttrInfoCharges = "Charges";
const pt_csz CD_kAttrInfoFlask = "Flask";
const pt_csz CD_kAttrInfoTargetable = "Targetable";
const pt_csz CD_kAttrInfoDiesAfterTime = "DiesAfterTime";		//需要保护的怪物有这个属性
const pt_csz CD_kAttrInfoTriggerableBlockage = "TriggerableBlockage";		//凡是有这个属性的，打开门后，会更改地图数据为可通过的。
const pt_csz CD_kAttrInfoMovement = "Movement";

//////////////////////////////////////////////////////////////////////////
const pt_cwsz CD_kMetaMonster = L"/Monsters";
const pt_cwsz CD_kMetaPlayer = L"/Characters";
const pt_cwsz CD_kMetaNpc = L"/NPC";
const pt_cwsz CD_kMetaWaypoint = L"/Waypoint";
const pt_cwsz CD_kMetaAreaTransition = L"/AreaTransition";
const pt_cwsz CD_kMetaItems = L"/Items";
const pt_cwsz CD_kMetaStash = L"Objects/Stash";							//Metadata/MiscellaneousObjects/Stash
const pt_cwsz CD_kMetaNoticeBoard = L"Objects/NoticeBoard";				//Metadata/MiscellaneousObjects/NoticeBoard
const pt_cwsz CD_kMetaNoticeBoardPvP = L"Objects/NoticeBoardPvP";		//Metadata/MiscellaneousObjects/NoticeBoardPvP
const pt_cwsz CD_kMetaWorldItem = L"/WorldItem";			//Metadata/MiscellaneousObjects/WorldItem
const pt_cwsz CD_kMetaChest = L"/Chests/";				//Metadata/Chests/ProdWounded1
const pt_cwsz CD_kMetaMapDevice = L"MiscellaneousObjects/MapDevice";
const pt_cwsz CD_kMetMappingDevice = L"Hideouts/Objects/StrDexIntMappingDevice";
const pt_cwsz CD_kMetBellyArenaTransition = L"/Belly/Objects/BellyArenaTransition";
const pt_cwsz CD_kMetMaligaroOrrery = L"QuestObjects/Act7/MaligaroOrrery";
const pt_cwsz CD_kMetKaruiSlave = L"Monsters/KaruiSlave/KaruiSlave";
//////////////////////////////////////////////////////////////////////////
//******************物品相关
const int CD_kBagItemArraySideSize = 50;
//背包数组最大尺寸
const int CD_kBagItemArrayMaxSize = CD_kBagItemArraySideSize * CD_kBagItemArraySideSize;
//宝石槽最大个数
const int CD_kGemSlotMaxCnt = 6;
//身上瓶子最大数量
const int CD_kBodyFlaskMaxCnt = 5;

//宝石插槽颜色
enum enCD_GemSlotColor{
	kGSC_Red = 1,		//红色
	kGSC_Green = 2,		//绿色
	kGSC_Blue = 3,		//蓝色
	KGSC_White = 4,		//白色
	kGSC_Invalid = 5,	//无效
};

//物品颜色，物品名字的颜色
enum enCD_ItemColor{
	kIC_Non = -1,
	kIC_White = 0,	//白
	kIC_Blue,		//蓝
	kIC_Yellow,		//黄
	kIC_Gold,		//金
	KIC_Max,
};

//物品需求属性，力量、敏捷智力
enum enCD_ItemRequireAttr{
	kIRA_Power = 0,	//力量
	kIRA_Smart,		//敏捷
	kIRA_Brain,		//智力
	kIRA_Max,
};

//背包结构类型
enum enCD_InventoryType{
	kIT_Invalid = 0,			//枚举顺序不要改变
	kIT_MainBag = 1,			//人物背包
	kIT_BodyArmour,				//衣服盔甲
	kIT_WeaponLeft,				//左手武器，只能遍历出当前选择的那套武器
	kIT_WeaponRight,			//右手武器
	kIT_Helm,					//头盔
	kIT_Necklace,				//项链
	kIT_RingLeft,				//左手戒指
	kIT_RingRight,				//右手戒指
	kIT_Glove,					//手套
	kIT_Shoe,					//鞋子
	kIT_Belt,					//腰带
	kIT_Flasks,					//瓶子，5个瓶子都是这个索引
	kIT_Cursor1,
	kIT_Map1,
	kIT_WeaponLeftHide,			//左手武器，当前没有选择的那个左手武器
	kIT_WeaponRightHide,		//右手武器，当前没有选择的那个右手武器
	kIT_AltarSmall1,
	kIT_AltarSmall2,
	kIT_AltarMedium1,
	kIT_AltarMedium2,
	kIT_AltarMedium3,
	kIT_AltarLarge1,
	kIT_AltarLarge2,
	kIT_AltarLarge3,
	kIT_BodyEquiqMax,
	//仓库
	kIT_StashTab_1 = 31,		//打开仓库的时候才有现在的这些 2
	kIT_StashTab_2,	
	kIT_StashTab_3,	
	kIT_StashTab_4,
	kIT_Max,
};

enum enCD_AttrInventoryEquipType{
	kAIET_WeaponLeft = 0,
	kAIET_WeaponRight,
	kAIET_BodyArmour,		//衣服盔甲
	kAIET_Helm,				//头盔
	kAIET_Necklace_Glove,	//项链、手，这是个bug
	kAIET_Shoe,				//鞋子
	kAIET_Ukn,
	kAIET_RingLeft,			//左手戒指
	kAIET_RingRight,		//右手戒指
	kAIET_Belt,				//腰带
	kAIET_Max,
};

//游戏里并没有下面的枚举常量，自己可随意定义数值
enum enCD_ItemType{
	kITYPE_Invalid = 0,
	kITYPE_LifeFlask,				//生命药剂
	kITYPE_ManaFlask,				//魔力药剂
	kITYPE_HybridFlask,				//复合药剂
	kITYPE_UtilityFlask,			//功能药剂
	kITYPE_UtilityFlaskCritical,	//暴击药剂
	kITYPE_StackableCurrency,		//可堆叠通货
	kITYPE_ActiveSkillGem,		//主动技能宝石
	kITYPE_SupportSkillGem,		//辅助技能宝石
	kITYPE_Helmet,				//头部
	kITYPE_Amulet,				//项链
	kITYPE_Belt,				//腰带
	kITYPE_Ring,				//戒指
	kITYPE_Boots,				//鞋子
	kITYPE_Gloves,				//手套
	kITYPE_BodyArmour,			//胸甲
	kITYPE_Shield,				//盾			--右手
	kITYPE_Currency,			//通货
	kITYPE_Claw,				//爪			--单手  左右手都可以带  不可以与法杖搭配
	kITYPE_Dagger,				//匕首		--单手  左右手都可以带
	kITYPE_Wand,				//法杖		--单手  只能与盾配合 或者同类型的配合
	kITYPE_QuestItem,			//任务物品
	kITYPE_OneHandSword,		//单手剑
	kITYPE_ThrustingOneHandSword,	//细剑
	kITYPE_OneHandAxe,			//单手斧
	kITYPE_OneHandMace,			//单手锤		--单手  左右手都可以带
	kITYPE_Bow,					//弓
	kITYPE_Staff,				//长杖		--双手
	kITYPE_TwoHandSword,		//双手剑
	kITYPE_TwoHandAxe,			//双手斧
	kITYPE_TwoHandMace,			//双手锤
	kITYPE_Quiver,				//箭袋
	kITYPE_SmallRelic,			//小型圣物
	kITYPE_MediumRelic,			//中型圣物
	kITYPE_LargeRelic,			//大型圣物
	kITYPE_Sceptre,				//短杖
	kITYPE_Map,					//异界地图
	kITYPE_FishingRod,			//鱼竿
	kITYPE_MapFragment,			//地图碎片
	kITYPE_HideoutDoodad,		//藏身处装饰
	kITYPE_Microtransaction,	//商城物品
	kITYPE_Jewel,				//珠宝
	kITYPE_DivinationCard,		//命运卡
	kITYPE_LabyrinthItem,		//迷宫物品
	kITYPE_LabyrinthTrinket,	//迷宫饰品
	kITYPE_LabyrinthMapItem,	//异界迷宫物品
	kITYPE_Breachstone,			//裂隙之石
	kITYPE_Leaguestone,			//联盟石
	kITYPE_Unarmed,				//没有武器的  --- 特殊的表里没有名字
	kITYPE_Max,
};

//使用类型
enum enCD_UsableType{
	kUT_Invalid = 0,
	kUT_UseItem = 1,		//点击右键就能直接使用
	kUT_UseItemTo,			//使用物品作用于
	kUT_Ukn1,
	kUT_Ukn2,
	kUT_Ukn3,
	kUT_Ukn4,
	kUT_Max,
};
static_assert(7 == kUT_Max, "");

//枚举顺序和数值不要随便改变
enum enCD_FlaskType{
	kFT_Invalid = 0,
	kFT_Hp,				//Metadata/Items/Flasks/FlaskLife11
	kFT_Mp,				//Metadata/Items/Flasks/FlaskMana11
	kFT_Hybrid,			//复合型药剂，即加血又加蓝，Metadata/Items/Flasks/FlaskHybrid2
	kFT_Utility,		//增加属性的，主要增加移动速度吧,Metadata/Items/Flasks/FlaskUtility6
	kFT_Max,
};

enum enCD_ValueType{
	kVT_Hp = 0xd,
	kVT_Mp = 0xf,
	kVT_Time = 0x12,		//比如多少秒内回复一定的生命，kFVI_Time的value / 10 = 秒。也可以是持续时间，比如持续多少秒
};

enum enCD_SendItemOperFlag : pt_byte{
	kSIOF_Normal = 0,
};
//////////////////////////////////////////////////////////////////////////
//********************GameLogic的OperateCommand
const pt_cwsz CD_kLOC_Destroy = L"destroy";
const pt_cwsz CD_kLOC_Remaining = L"remaining";
//////////////////////////////////////////////////////////////////////////
//*********************文件db路径
const pt_cwsz CD_kFileDbPath_Words = L"Data/Words.dat";
const pt_cwsz CD_kFileDbPath_PassiveSkill = L"Data/PassiveSkills.dat";
const pt_cwsz CD_kFileDbPath_PassiveSkillGraph = L"Metadata/PassiveSkillGraph.psg";
const pt_cwsz CD_kFileDbPath_Quest = L"Data/Quest.dat";
const pt_cwsz CD_kFileDbPath_QuestStates = L"Data/QuestStates.dat";
const pt_cwsz CD_kFileDbPath_ActiveSkills = L"Data/ActiveSkills.dat";
const pt_cwsz CD_kFileDbPath_BaseItemTypes = L"Data/BaseItemTypes.dat";
const pt_cwsz CD_kFileDbPath_ItemClasses = L"Data/ItemClasses.dat";
const pt_cwsz CD_kFileDbPath_SkillGems = L"Data/SkillGems.dat";
const pt_cwsz CD_kFileDbPath_GemTags = L"Data/GemTags.dat";
const pt_cwsz CD_kFileDbPath_GrantedEffects = L"Data/GrantedEffects.dat";
const pt_cwsz CD_kFileDbPath_GrantedEffectsPerLevel = L"Data/GrantedEffectsPerLevel.dat";		//结构并没有分析出全部，因为没必要
const pt_cwsz CD_kFileDbPath_WorldAreas = L"Data/WorldAreas.dat";
const pt_cwsz CD_kFileDbPath_LabyrinthTrials = L"Data/LabyrinthTrials.dat";
const pt_cwsz CD_kFileDbPath_Labyrinths = L"Data/Labyrinths.dat";
const pt_cwsz CD_kFileDbPath_LabyrinthAreas = L"Data/LabyrinthAreas.dat";		//魔幻迷宫
const pt_cwsz CD_kFileDbPath_LabyrinthSection = L"Data/LabyrinthSection.dat";
const pt_cwsz CD_kFileDbPath_LabyrinthSectionLayout = L"Data/LabyrinthSectionLayout.dat";
const pt_cwsz CD_kFileDbPath_LabyrinthExclusionGroups = L"Data/LabyrinthExclusionGroups.dat";
const pt_cwsz CD_kFileDbPath_LabyrinthNodeOverrides = L"Data/LabyrinthNodeOverrides.dat";
const pt_cwsz CD_kFileDbPath_Difficulties = L"Data/Difficulties.dat";
const pt_cwsz CD_kFileDbPath_MapPins = L"Data/MapPins.dat";			//地图表，用于组合WorldArea
const pt_cwsz CD_kFileDbPath_MinimapIcons = L"Data/MinimapIcons.dat";
const pt_cwsz CD_kFileDbPath_NPCs = L"Data/NPCs.dat";
const pt_cwsz CD_kFileDbPath_NPCShop = L"Data/NPCShop.dat";
const pt_cwsz CD_kFileDbPath_NPCAudio = L"Data/NPCAudio.dat";
const pt_cwsz CD_kFileDbPath_NPCTalk = L"Data/NPCTalk.dat";
const pt_cwsz CD_kFileDbPath_NPCTextAudio = L"Data/NPCTextAudio.dat";
const pt_cwsz CD_kFileDbPath_MapConnections = L"Data/MapConnections.dat";
const pt_cwsz CD_kFileDbPath_Tags = L"Data/Tags.dat";
const pt_cwsz CD_kFileDbPath_DropPool = L"Data/DropPool.dat";
const pt_cwsz CD_kFileDbPath_Characters = L"Data/Characters.dat";
const pt_cwsz CD_kFileDbPath_NPCMaster = L"Data/NPCMaster.dat";
const pt_cwsz CD_kFileDbPath_NPCMasterExperiencePerLevel = L"Data/NPCMasterExperiencePerLevel.dat";
const pt_cwsz CD_kFileDbPath_Mods = L"Data/Mods.dat";
const pt_cwsz CD_kFileDbPath_ModType = L"Data/ModType.dat";
const pt_cwsz CD_kFileDbPath_ModSellPriceTypes = L"Data/ModSellPriceTypes.dat";
const pt_cwsz CD_kFileDbPath_Stats = L"Data/Stats.dat";
const pt_cwsz CD_kFileDbPath_AchievementItems = L"Data/AchievementItems.dat";
const pt_cwsz CD_kFileDbPath_CurrencyItems = L"Data/CurrencyItems.dat";		//可流通的物品
const pt_cwsz CD_kFileDbPath_Topologies = L"Data/Topologies.dat";
const pt_cwsz CD_kFileDbPath_SoundEffects = L"Data/SoundEffects.dat";
const pt_cwsz CD_kFileDbPath_Chests = L"Data/Chests.dat";
const pt_cwsz CD_kFileDbPath_WeaponTypes = L"Data/WeaponTypes.dat";
const pt_cwsz CD_kFileDbPath_MonsterResistances = L"Data/MonsterResistances.dat";
const pt_cwsz CD_kFileDbPath_MonsterTypes = L"Data/MonsterTypes.dat";
const pt_cwsz CD_kFileDbPath_MonsterVarieties = L"Data/MonsterVarieties.dat";
const pt_cwsz CD_kFileDbPath_BloodTypes = L"Data/BloodTypes.dat";
const pt_cwsz CD_kFileDbPath_ClientStrings = L"Data/ClientStrings.dat";
const pt_cwsz CD_kFileDbPath_ItemVisualIdentity = L"Data/ItemVisualIdentity.dat";
const pt_cwsz CD_kFileModel_ArmoursShields = L"Art/Models/Items/Armours/Shields/ShieldInt/ShieldI";
const pt_cwsz CD_kFileDbPath_Environments = L"Data/Environments.dat";
const pt_cwsz CD_kFileDbPath_Music = L"Data/Music.dat";
const pt_cwsz CD_kFileDbPath_BuffDefinitions = L"Data/BuffDefinitions.dat";
const pt_cwsz CD_kFileDbPath_SkillStatDescriptions = L"Metadata/StatDescriptions/skillpopup_stat_filters.txt";
const pt_cwsz CD_kFileDbPath_MinionSpellSkillStat = L"Metadata/StatDescriptions/minion_spell_skill_stat_descriptions.txt";
const pt_cwsz CD_kFileDbPath_CraftingBenchOptions = L"Data/CraftingBenchOptions.dat";
const pt_cwsz CD_kFileDbPath_ChestEffects = L"Data/ChestEffects.dat";
const pt_cwsz CD_kFileDbPath_Strongboxes = L"Data/Strongboxes.dat";
//////////////////////////////////////////////////////////////////////////
//*****************技能相关
//最下面格子的类型
enum enCD_DownInventoryType{
	kDIT_Invalid = -1,
	kDIT_Flask1 = 0,	//瓶子1
	kDIT_Flask2,
	kDIT_Flask3,
	kDIT_Flask4,
	kDIT_Flask5,
	kSGT_LeftBt,		//鼠标左键
	kSGT_MiddleBt,
	kSGT_RightBt,
	kSGT_KeyQ,
	kSGT_KeyW,
	kSGT_KeyE,
	kSGT_KeyR,
	kSGT_KeyT,
};

//普通技能类型，也可能是普通技能的ID
//技能特效的ID吧，比如装备了弓箭，如果改变这个数值，就只有拉弓放箭的动作，而没有箭矢的特效流过了。
enum enCD_WeaponSkillType{
	kWST_Zhua = 0x7,				//爪
	kWST_BiShou = 0x8,			//匕首
	kWST_SingleShortJian = 0xa,	//单手剑、锈剑、赤红短刃、法杖
	kWST_SingleFu = 0xc,		//单手斧
	kWST_SingleChui = 0xd,		//单手锤
	kWST_GongJian = 0xe,		//弓箭
	kWST_LongZhang = 0xf,	//长杖
	kWST_DoubleJian = 0x10,	//双手剑
	kWST_DoubleFu = 0x11,	//双手斧
	kWST_DoubleChui = 0x12,	//双手锤
	kWST_SingleZhang = 0x21,		//单手锤，影语短杖
	kWST_EmptyWeapon = 0x25,		//角色默认的数值，空着手时的数值
};
//好像跟左右手开打有关
enum enCD_SkillTypeValue{
	kSTV_Normal = 0x30,
};

const pt_word CD_kSkillId_NormalHit = 0x4000;		//普通攻击

//*******buff名
//冰封怪有这两个buff
const std::wstring CD_kBuffEnName_hidden_monster_disable_minions = L"hidden_monster_disable_minions";
const std::wstring CD_kBuffEnName_visual_display_buff_not_hidden = L"visual_display_buff_not_hidden";
const std::wstring CD_kBuffEnName_cannot_be_damaged = L"cannot_be_damaged";
//冰封怪解冻后有这个buff
const std::wstring CD_kBuffEnName_monolith_monster_freedom = L"monolith_monster_freedom";
//冰封怪的boss
const std::wstring CD_kBuffEnName_monster_rare_effect_buff = L"monster_rare_effect_buff";
//////////////////////////////////////////////////////////////////////////
enum enCD_SkillId : pt_word{
	kSI_OpenObj = 0x266,
};

//这个pt_char不要改，因为要判断负数的
enum enCD_TouchTargetSmallFlag : pt_char{
	//kTTSF_Normal = 0x8,			//原先是8，现在变成0了，2017/8/7 22:56
	kTTSF_Normal = 0,
	//kTTSF_WithLeftCtrl = 0xc,		//按着ctrl键移动和打开对象都是此值，原先是0xc，现在是4
	kTTSF_WithLeftCtrl = 4,
};

//是个byte
enum enCD_TransitionableState : pt_byte{
	kTranState_Block = 1,		//阻塞着的，不可通过的
	kTranState_Opened,			//可通过的，已打开的。对于某些传送门，比如 怨忿之窟深处，原先是1，当触发了boss，然后把门隐藏掉后，此值就变成2了。
};
//////////////////////////////////////////////////////////////////////////
//**************UI相关
const int CD_kUiNextStateInfoMax = 0xe;
//正常游戏状态下，默认显示的对话框数量
#ifdef VER_GJ3_
const int CD_kInGameNormalShowDlgMaxCnt = 19;
#elif VER_GF_
const int CD_kInGameNormalShowDlgMaxCnt = 19;
#endif
//EscapeMenu的button
enum enCD_EscapeMenuButton{
	kEMB_Continue = 1,		//继续游戏
	kEMB_Setting,			//设定
	kEMB_RetSelect,			//回到角色选择画面
	kEMB_LeaveGame,			//离开游戏
};
#ifdef VER_GJ_			//国际服
const std::string CD_kNTL_CotinueA = "Continue";
const std::wstring CD_kNTL_Cotinue = L"Continue";
const std::string CD_kNTL_ByeA = "Goodbye";
const std::wstring CD_kNTL_Bye = L"Goodbye";
const std::string CD_KNTL_BuyA = "Purchase Items";
const std::wstring CD_KNTL_Buy = L"Purchase Items";
const std::string CD_KNTL_SellA = "Sell Items";
const std::wstring CD_KNTL_Sell = L"Sell Items";
const std::wstring CD_kBtTitle_SkillAllTutorials = L"skip all tutorials";
#else					//国服
const std::string CD_kNTL_CotinueA = "继续";
const std::wstring CD_kNTL_Cotinue = L"继续";
const std::string CD_kNTL_ByeA = "再会";
const std::wstring CD_kNTL_Bye = L"再会";
const std::string CD_KNTL_BuyA = "购买物品";
const std::wstring CD_KNTL_Buy = L"购买物品";
const std::string CD_KNTL_SellA = "贩卖物品";
const std::wstring CD_KNTL_Sell = L"贩卖物品";
const std::wstring CD_kBtTitle_SkillAllTutorials = L"跳过所有指南";
#endif

//////////////////////////////////////////////////////////////////////////
enum class enCD_EntityAttr_Player{
	kEAP_Positioned = 0,
	kEAP_Stats,
	kEAP_Pathfinding,
	kEAP_Life,
	kEAP_Animated,
	kEAP_Player,		//5
	kEAP_Inventories,
	kEAP_Actor,
	kEAP_Render,
	kEAP_Targetable,		//可选中
	kEntityAttrMaxCnt,		//总共有10个
};

enum class enCD_EntityAttr_Npc{
	kEAN_Positioned = 0,
	kEAN_Animated,
	kEAN_BaseEvents,		//触发事件
	kEAN_NPC,
	kEAN_MinimapIcon,
	kEAN_Render,		//5
	kEAN_Targetable,
	kEAN_ClientNPC,			//有这个属性才可以访问该npc
	kEntityAttrMaxCnt,		//总共有8个
};

//传送点/传送门
/*
Metadata/Effects/Environment/waypoint/new/WaypointEffect		//传送点周围的特效，它的坐标是不到传送点，但很接近的地方。
Metadata/MiscellaneousObjects/Waypoint			//传送点正中央
Metadata/MiscellaneousObjects/AreaTransition	//传送门，比如从 狮眼守望 到 暮光海滩 的传送门
*/
enum class enCD_EntityAttr_Waypoint{
	kEAW_Positioned = 0,
	kEAW_BaseEvents,
	kEAW_Animated,
	kEAW_MinimapIcon,
	kEAW_Render,		//4
	kEAW_Targetable,
	kEntityAttrMaxCnt,
};

//传送门
enum class enCD_EntityAttr_AreaTransition{
	kEAAT_Positioned = 0,
	kEAAT_BaseEvents,
	kEAAT_Animated,
	kEAAT_AreaTransition,
	kEAAT_Stats,		//4
	kEAAT_ObjectMagicProperties,
	kEAAT_Render,
	kEAAT_Targetable,
	kEntityAttrMaxCnt,		//8
};

//可传送的对象
enum class enCD_EntityAttr_Transitionable{
	kPositioned = 0,
	kBaseEvents,
	kAnimated,
	kTransitionable,
	kMinimapIcon,		//4
	kEAAT_Render,
	kEAAT_Targetable,
	kEntityAttrMaxCnt,	//7
};

enum class enCD_EntityAttr_Monster{
	kEAM_Positioned = 0,
	kEAM_Stats,
	kEAM_ObjectMagicProperties,
	kEAM_Pathfinding,
	kEAM_Life,
	kEAM_Animated,		//5
	kEAM_Inventories,
	kEAM_Actor,
	kEAM_Monster,
	kEAM_Render,
	kEAM_Targetable,
	kEntityAttrMaxCnt,		//总共有11个
};

//非人类，比如仓库、告示板、活动告示板等
enum class enCD_EntityAttr_NoHuman{
	kPositioned = 0,
	kBaseEvents,
	kAnimated,
	kRender,
	kTargetable,
	kEntityAttrMaxCnt,
};

//告示板
enum class enCD_EntityAttr_NoticeBoard{
	kPositioned = 0,
	kBaseEvents,
	kAnimated,
	kRender,
	Targetable,
	kEntityAttrMaxCnt,
};

//箱子、木桶、流放者的遗体
//恐喙鸟巢有9个属性
enum class enCD_EntityAttr_Chest{
	kPositioned = 0,
	kBaseEvents,
	kAnimated,
	kChest,
	kStats,
	kObjectMagicProperties,
	kRender,
	kTargetable,
	kEntityAttrMaxCnt,		//8
};

//掉落物品
enum class enCD_EntityAttr_WorldItem{
	kPositioned = 0,
	kAnimated,
	kWorldItem,
	kRender,
	kTargetable,
	kEntityAttrMaxCnt,		//5
};
//////////////////////////////////////////////////////////////////////////
enum enCD_EnterMapUknId{
};
//////////////////////////////////////////////////////////////////////////
//****************下面的不是游戏中的数据
enum enCD_TraGemSkillFlag{
	kTGSF_Gem = 1 << 0,				//遍历宝石
	kTGSF_Inside = 1 << 1,			//遍历镶嵌到装备里的宝石
	kTGSF_ActiveSkill = 1 << 2,		//遍历主动技能
	kTGSF_PassiveSkill = 1 << 3,	//遍历被动技能
};

enum enCD_TraUiShownFlag : int{
	kTUSF_Shown = 1 << 0,							//显示
	kTUSF_UnShown = 1 << 1,							//不显示
	kTUSF_AllNode = 1 << 2,							//遍历所有树结点
	kTUSF_ShowHide = kTUSF_Shown | kTUSF_UnShown,	//显示和不显示的都遍历
	kTUSF_All = kTUSF_ShowHide | kTUSF_AllNode,
};

enum enCD_TraUiFilterType{
	kTUFT_ByVtbl = 0,
	kTUFT_ByAddr,
	kTUFT_AllSubNodes,				//遍历所有树结点
};

//枚举常量的数字不要随便改动
enum enCD_NpcPanelItemsContainer{
	kNPIC_Normal = 1,		//正常的容器
	kNPIC_TalkPanel,		//有 继续 按键的面板
};

enum enCD_RightMostPanel{
	kRMP_QuestLvlState = 1,		//任务、技能状态面板
	kRMP_Ukn,
};

enum enCD_ReviveAtPosition{
	kRAP_AtCity,
	kRAP_AtRecord,
	kRAP_AtDied,		//在死亡点复活
};

enum enCD_PressItemWithKey{
	kPIWK_Shift,		//VK_SHIFT
	kPIWK_Control,		//VK_CONTROL
	kPIWK_Menu,			//VK_MENU
	kPIWK_Normal,
};

enum enCD_CreateRoleBtIdx{
	kCRBI_Cancel = 0,
	kCRBI_Ok,
	kCRBI_Ukn,
	kCRBI_Max = 3,
};

enum enCD_SkillAttrValueType{
	kSAVT_CallerMaxCnt,		//比如 最多拥有 6 只僵尸。
};

enum enCD_SocketType{
	kST_PreGameState,
	kST_InGameState,
	kST_Ukn,
};

enum enCD_GetLabyrinthTrialFlag{
	kGLT_Completed = 1 << 0,
	kGLT_UnCompleted = 1 << 1,
	kGLT_All = kGLT_Completed | kGLT_UnCompleted,
};
//////////////////////////////////////////////////////////////////////////
