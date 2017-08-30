#pragma once
/*
@author zhp
@date 2014/8/1 5:25
@purpose for game const variable
*/
#include <string>

//角色名最大长度,不要超过19
const int kRoleNameMaxLen = 19;
const int kRoleNameMinLen = 4;
const int kMaxTaskId = 0x2000;
const int kItemTemplateMax = 0x20000;
const int kCraftMaxId = 0x60000;
const int kManuFactureMaxId = 0x2000;
const int kJingSuanDianMax = 0x1000;
const int kCurJingSuanDianMax = 6;
const int kFarmSeedMax = kItemTemplateMax;
const int kCaoYaoTianMax = 3;
//邮寄最大数量
const int kSendMailMaxCnt = 10;
//邮寄等级限制
const int kSendMailMinRoleLvl = 20;
//狩猎券证书最大数量
const int kShouLieQuanMax = 30;
//交易最大格子数量
const int kDealMaxGridCnt = 30;
//请求交易有效时间，单位秒
const int kQuestDealValidTime = 15 - 1;
//当面交易的等级限制
const int kRoleMinLvl_FaceToFaceDeal = 20;

//城镇名
const std::string kVillageName_XiMeiLunShanLu = "希美伦山路";
const std::string kVillageName_LongZhiHu = "龙趾湖";
const std::string kVillageName_MiLaDeCun = "米拉德村";
const std::string kVillageName_MeiJiePoErTan = "梅杰波尔坦";
const std::string kVillageName_YaoYueLin = "耀月林";
const std::string kVillageName_YinBaiAiKou = "银白隘口";
//const std::string kVillageName_LieTaunYingDi = "猎团营地";
/*
梅杰波尔坦的房间：
王立猎团营地：1
艾露居留地：2
*/

//副本名
const std::string kFbName_BeginDream = "[梦的起点]";

const int kMinFbId = 0x00018000;
const int kMaxFbId = 0x00045000;

const DWORD kMonsterInvalidPacketId = std::numeric_limits<DWORD>::max();
const int kInvalidTaskId = -1;
const int kInvalidFbId = -1;

const std::string kHandleAtHitting = "2A239250BBAB";
const std::string kHandleAfterEnterFb = "0BED2498D256";
const std::string kHandlersOnScriptStoped = "2D613C6E329B4CD0816FB";

//npc name
const std::string kNpcName_Sociaty = "看板娘莎夏";

//拍卖页面条数
const int kAuctionPageCnt = 8;

//交易相关
//拍卖行人物等级限制
const int kRoleLvlLimit_Auction = 25;

//最大房间号
const int kMaxRoomId = 20;

//物品黑名单/白名单
const char* const kItemBlackWhiteList_PriTblName = "97CCFE37";
//物品黑名单
const std::string kItemBlackListName = "CC19DA40";
//物品白名单
const std::string kItemWhiteListName = "0E9D1F23";

//房间到距离的权重
const double kRoomNum2DistWeight = 0x9fffff9;

//消息返回的错误码
//普通副本的返回值
//行动力不足
const int kMsgRetError_XingDongLiBuZu = 300007;
//
const int kMsgRetError_DoneIt = 360046;

const int kLianQiCaoFullValue = 100;

const char* const kFbSubName_ShiLian = "试练";
const char* const kFbSubName_TiaoZhan = "挑战";
const char* const kFbSubName_JiXian = "极限";

const int kZhanWeiZhiMin = 1000000;

//buff name
const std::string kBuffName_Hot = "炎热";
const std::string kBuffName_Cold = "寒冷";
const std::string kBuffName_LengYin = "冷饮";
const std::string kBuffName_ReYin = "热饮";
const std::string kBuffName_AoYiJueXing = "奥义觉醒";
const std::string kBuffName_NiRen = "泥人";
const std::string kBuffName_RanShao = "燃烧";
const std::string kBuffName_ErChou = "恶臭";
const std::string kBuffName_SnowMan = "雪人";
const std::string kBuffName_ChanRao = "缠绕";
//buff desc
const std::string kBuffDesc_Hot = "你感觉到炎热";
const std::string kBuffDesc_Cold = "你感觉到寒冷";
//const std::string kBuffDesc_RanShao = "每秒钟损失一定的生命值";

//庄园副本ID
const int kSelfFarmFbId = 0x00027165;

//草药田
const std::string kFarmGaoShiPai = "庄园告示牌";

//物品名
const std::string kItemName_JingSuan = "精算";
//古文书
const std::string kItemName_GuWenShu = "古文书·一";
const int kUseCnt_Unlock_KingCream = 5;

//////////////////////////////////////////////////////////////////////////
//*********************瞬移、坐标相关**************************************
//这些数值都不能轻易改变，否则会有问题
//过图时间间隔
const int kSendMoveRoomSpan = 5 * 1000;
//不打怪时的寻路最小距离限制
const double kFindPathMinDistLimitNormal = 4;
//打怪时的寻路最小距离限制
const double kFindPathMinDistLimitOnHitting = 10;
//设置坐标危险距离
const double kFindPathMaxDangerDist = kFindPathMinDistLimitOnHitting + 10;
//寻路中，坐标与目标坐标的差距不能大于这个数
const double kFindingPathRoleMonsterMaxDist = 5;
//设置坐标后，等待游戏发过房间包的时间
const int kWaitSendMoveRoolAfterSetPos = 1000 * 2;
//////////////////////////////////////////////////////////////////////////
//攻击，高度判断失败次数上限
const int kMonsterMostHighFaileMaxCnt = 200;

//非空闲动作的等待时间
const int kUnFreeToFreeWaitTime = 1000 * 10;
//BOSS最高高度限制
const int kMaxPosZHigh = 6;
//BOSS最低高度限制，这个是允许打桃毛兽王最低的值，不要轻易改动
const int kMinPosZHigh = -12;
//Z高度不对时延迟时间
const int kSleepTimeAtPosZError = 700;
//过图之等待角色可移动时间
const int kWaitTime_RoleCanMove = 1000 * 60 * 15;
//新手训练地图模板
const int kXinShouXunLianTemplateMapId = 1402;
//////////////////////////////////////////////////////////////////////////
//******************pause id**********************************************
//游戏数据不正常
extern const void* const kPauseId_GameDataErr_;
DEFINE_THE_PAUSE_ID(kPauseId_GameDataErr, kPauseId_GameDataErr_);
//角色死亡了
extern const void* const kPauseId_RoleDead_;
DEFINE_THE_PAUSE_ID(kPauseId_RoleDead, kPauseId_RoleDead_);
//过图失败
extern const void* const kPauseId_PassMapErr_;
DEFINE_THE_PAUSE_ID(kPauseId_PassMapErr, kPauseId_PassMapErr_);
//更换运行脚本
extern const void* const kPauseId_ChangeCurRunScript_;
DEFINE_THE_PAUSE_ID(kPauseId_ChangeCurRunScript, kPauseId_ChangeCurRunScript_);
//进行删除角色
extern const void* const kPauseId_DoDeleteRole_;
DEFINE_THE_PAUSE_ID(kPauseId_DoDeleteRole, kPauseId_DoDeleteRole_);
//取消交易
extern const void* const kPauseId_CancelDeal_;
DEFINE_THE_PAUSE_ID(kPauseId_CancelDeal, kPauseId_CancelDeal_);
//////////////////////////////////////////////////////////////////////////
//数据错误默认返回值
const int kDefRetValueAtDataErr = 1;