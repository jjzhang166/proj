#pragma once
/*
@author zhp
@date 2016/4/11 21:38
*/
#include <ProtocolX/P_DefinesX.h>

pt_cdword kInvalidDwValue = (pt_dword)-1;
pt_cint kInvalidIntValue = (pt_int)-1;
pt_cdword kInvalidPacketId = (pt_dword)-1;
pt_cdword kInvalidLocalId = (pt_dword)-1;
//名字类字符串最大长度
pt_cdword kMaxNameStrLen = 250;
pt_cdword kMaxItemDbCnt = 0x20000;
pt_cdword kMaxTaskDbId = 0x2000;
/*
const int kMinFbId = 0x00018000;
const int kMaxFbId = 0x00045000;*/
pt_cdword kMaxFbDbId = 0x00850000;
const int kMaxFarmDbId = 0x1000;
const int kMaxShopDbId = 0x200;
const int kMaxSeedDbId = kMaxItemDbCnt;
const int kMaxCraftDbId = 0x60000;
const int kMaxMenufactureDbId = 0x2000;
const int kMaxBuffDbId = 0x05000000;


//角色最大等级
const int kMaxRoleLvl = 50;
//角色名字缓冲区长度
const int kRoleBufferMaxSize = 0x20;

//游戏当中发包缓冲区最大长度
pt_cdword kMaxSendBufferSizeOfGame = 0x00080000;
//发包缓冲区最大长度
pt_cdword kMaxSendBufferSize = kMaxSendBufferSizeOfGame / 2;
//主消息ID
pt_cword kMainMsgId = 1;
//////////////////////////////////////////////////////////////////////////
//ui
const char* const kRoleListButtons[6] = {
	"root1.rolelist.item1.btnBg.mc_role",
	"root1.rolelist.item2.btnBg.mc_role",
	"root1.rolelist.item3.btnBg.mc_role",
	"root1.rolelist.item4.btnBg.mc_role",
	"root1.rolelist.item5.btnBg.mc_role",
	"root1.rolelist.item6.mc_norole"
};
//MMOCLINET_UI_BTN_STARGAME
const pt_csz kUiStr_StartGame = "root1.btn_startgame"; //开始游戏按钮
//MMOCLINET_UI_CR_BTN_MC_RAND
const pt_csz kUiStr_RandomSkin = "root1.panel.down_mc.randombtn"; // 随机皮肤和脸型
//MMOCLINET_UI_CR_BTN_MC_NEXTSTEP
const pt_csz kUiStr_CreateRoleNextStep = "root1.btn_nextstep";	//这是选择属性的下一步
//MMOCLINET_UI_CR_BTN_CREATEOK
const pt_csz kUiStr_CreateRoleOk = "root1.btn_CreateRoleOk";	//创建完成按钮
//MMOCLINET_UI_CR_BTN_OLDPLAY
const pt_csz kUiStr_IamOlPlayer = "root1.playerSetting.oldplayer"; //我的老猎人
//MMOCLINET_UI_CN_EDIT_CREATENAME
const pt_csz kUiStr_CreateRole_InputNameEdit = "root1.mc_creatroleinput.input";	//创建角色的时候，输入的名字
//////////////////////////////////////////////////////////////////////////
// 城镇to城镇的字符串
//MMOCLIENT_REVERSAL_XI_MEI_LUN_SHAN_LU_TO_LONG_ZHI_HU
const pt_csz kMoveCmd_XiMeiLun2LongZhiHu = "Hub1ToHub2Triigger";		// 希美伦山路 	==> 龙趾湖	Hub1ToHub2Triigger
//MMOCLIENT_REVERSAL_LONG_ZHI_HU_TO_XI_MEI_LUN_SHAN_LU
const pt_csz kMoveCmd_LongZhiHu2XiMeiLun = "Teleporter_To_Hub001";		// 龙趾湖		==> 希美伦山路	Teleporter_To_Hub001
//MMOCLIENT_REVERSAL_XI_MEI_LUN_SHAN_LU_TO_MI_DE_LA_CUN
const pt_csz kMoveCmd_XiMeiLun2MiLaDeCun = "HubToVillageTrigger";		// 希美伦山路 	==> 米拉德村	HubToVillageTrigger
//MMOCLIENT_REVERSAL_MI_DE_LA_CUN_TO_XI_MEI_LUN_SHAN_LU
const pt_csz kMoveCmd_MiLaDe2XiMeiLun = "VillageToHubTrigger";		// 米拉德村		==> 希美伦山路	VillageToHubTrigger
//MMOCLIENT_REVERSAL_LONG_ZHI_HU_TO_MEI_JIE_BO_ER_TAN
const pt_csz kMoveCmd_LongZhiHu2MeiJie = "Teleporter_To_Village02";	// 龙趾湖		==> 梅杰波尔坦	Teleporter_To_Village02
//MMOCLIENT_REVERSAL_MEI_JIE_BO_ER_TAN_TO_LONG_ZHI_HU
const pt_csz kMoveCmd_MeiJie2LongZhi = "Village2_To_Hub2_Trigger";	// 梅杰波尔坦	==> 龙趾湖	Village2_To_Hub2_Trigger
//MMOCLIENT_REVERSAL_MEI_JIE_BO_ER_TAN_TO_YAO_YUE_LIN
const pt_csz kMoveCmd_MeiJie2RaoYueLin = "Village2_To_Hub3_Trigger";	// 梅杰波尔坦	==> 耀月林	Village2_To_Hub3_Trigger
//MMOCLIENT_REVERSAL_YAO_YUE_LIN_TO_MEI_JIE_BO_ER_TAN
const pt_csz kMoveCmd_RaoYueLin2MeiJie = "TeleporterToVillage02";		// 耀月林		==> 梅杰波尔坦	TeleporterToVillage02
//银白隘口到耀月林
const pt_csz kMoveCmd_YinBaiAiKou2YaoYueLin = "TeleportToHub03";
//耀月林到银白隘口
const pt_csz kMoveCmd_YaoYueLin2YinBaiAiKou = "TeleporterToHub04";
//////////////////////////////////////////////////////////////////////////
const pt_csz kD3d9DllName = "CryRenderD3D9.dll";