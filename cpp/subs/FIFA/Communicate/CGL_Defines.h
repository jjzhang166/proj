#pragma once
/*
@author zhp
@date 2014/6/15 15:12
@purpose console.exe/game.dll/login.dll之间的交互
*/

//版本uuid异或常量
const unsigned char kAssistVersionXor = 0xa3;
//版本事件对象名
const char* const kAssistVersionEventObj = "_ComAtlStUads_";

//创建版本校验
bool CreateCheck_AssistVersion();
//进行版本校验
bool CheckAssistVersion();

//游戏模块名字
#define P_INJECTED_GAME_NAME _T("fifazf.exe")

//版本有效截止日期
const int kVersionEndTime_Year = 2017;
const int kVersionEndTime_Month = 11;
const int kVersionEndTime_Day = 1;

const unsigned short kAssistVersion = 0x0059;
#ifdef __PROJ_JIN_GANG_
const char* const kAssistVersionUuidGenerator = "23C92F9F-F1264-46EE-9E1E-82A23195BE7fd";
const char* const g_call_fn_name_set_login_succeed = "pldsc";
const char* const g_call_fn_name_set_ep = "aghks";
const char* const g_call_fn_name_set_trainer_name = "lkers";
const char* const g_call_fn_name_set_accout_wpd = "ercvh";

const unsigned int kGameConnectionMaxCnt = 20;
#elif defined(__PROJ_XIAO_ER_)
const char* const kAssistVersionUuidGenerator =	"36B2388DF-F299-4196-8AAD-22B9E7EB51Dc2";
const char* const g_call_fn_name_set_login_succeed = "jiolds";
const char* const g_call_fn_name_set_ep = "uioa";
const char* const g_call_fn_name_set_trainer_name = "werdx";
const char* const g_call_fn_name_set_accout_wpd = "okmys";

const unsigned int kGameConnectionMaxCnt = 20;
#endif

//////////////////////////////////////////////////////////////////////////
//kAcceptableGameConnectionCnt里面加起来的数量最多不能超过kGameConnectionMaxCnt
//每个帐号可接受的连接数量
static const int kAcceptableGameConnectionCnt[] = {
	kGameConnectionMaxCnt
};
//连接数量
struct ConnectCntInfo{
	//最大连接数量
	static const int kGameConnectionMaxCnt = kGameConnectionMaxCnt;
	//不在kAcceptableGameConnectionCnt此列的，也就是超出的，全部设为此连接数
	static const unsigned int kDefConnectCntAtOverflow = 0;
	static auto ConnectItemCnt() -> decltype(kAcceptableGameConnectionCnt)&{
		return kAcceptableGameConnectionCnt;
	}
};
//////////////////////////////////////////////////////////////////////////