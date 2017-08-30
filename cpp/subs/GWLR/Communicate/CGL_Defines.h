#pragma once
/*
@author zhp
@date 2014/6/15 15:12
@purpose console.exe/game.dll/login.dll之间的交互
*/
//////////////////////////////////////////////////////////////////////////
//*******************在这里声明服务器addr数据和str数据**********************
//addr data
enum enCheckAddrDataIndex{
	kCADI_LastestVersion = 1,		//最新的版本号
};
//str data
enum enCheckStrDataIndex{
	kCSDI_LpkbUuid = 1,				//lpkb的uuid
	kCSDI_ConsoleUuidMb,			//console的uuid，漫步
	kCSDI_ConsoleUuidXe,			//console的uuid，小二
};
//////////////////////////////////////////////////////////////////////////
//版本字符串
//版本号
const unsigned short kAssistVersion = 0x002e;
#ifdef __PROJ_GWLR_MB_
const char* const kAssistVersionUuidGenerator =
	/*kAssistVersionUuidGenerator_mb*/"3e47abaa-85e1-4f78-8796-b04cc8813e7c";
#elif (__PROJ_GWLR_XE_)
const char* const kAssistVersionUuidGenerator =
	/*kAssistVersionUuidGenerator_xe*/"ca523e4a-a907-433c-bd86-a99c8b52f404";
#elif (__PROJ_GWLR_MBP_)
const char* const kAssistVersionUuidGenerator =
	/*kAssistVersionUuidGenerator_mbp*/"607fb79d-a020-4f5d-8c77-9b2b898349ea";
#endif
//版本uuid异或常量
const unsigned char kAssistVersionXor = 0xc4;
//////////////////////////////////////////////////////////////////////////
//kAcceptableGameConnectionCnt里面加起来的数量最多不能超过kGameConnectionMaxCnt
//每个帐号可接受的连接数量
static const int kAcceptableGameConnectionCnt[] = {
	2, 1, 1, 5, 1
};
//连接数量
struct ConnectCntInfo{
	//最大连接数量
	static const int kGameConnectionMaxCnt = 30;
	//不在kAcceptableGameConnectionCnt此列的，也就是超出的，全部设为此连接数
	static const unsigned int kDefConnectCntAtOverflow = 2;
	static auto ConnectItemCnt() -> decltype(kAcceptableGameConnectionCnt)&{
		return kAcceptableGameConnectionCnt;
	}
};
//////////////////////////////////////////////////////////////////////////

//版本事件对象名
const char* const kAssistVersionEventObj = "_ComAtlStUads_";

//创建版本校验
bool CreateCheck_AssistVersion();
//进行版本校验
bool CheckAssistVersion();

//游戏模块名字
#define P_INJECTED_GAME_NAME _T("MHOClient.exe")

const unsigned short P_Port_ForGame = 35316;
const unsigned short P_Port_ForLogin = 35323;

//game do some
#define GAME_MODULE_NAME "gdoso"
#define LOGIN_MODULE_NAME "lgdos"
