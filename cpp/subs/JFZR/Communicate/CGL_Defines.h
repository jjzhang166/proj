#pragma once
/*
@author zhp
@date 2014/6/15 15:12
@purpose console.exe/game.dll/login.dll之间的交互
*/

//版本字符串
#ifdef __PROJ_JFZR_
//版本号
const unsigned short kAssistVersion = 0x001;
const char* const kAssistVersionUuidGenerator =
	"6D7AF229-1FF7-432B-9154-79AB2A5EF988";
//版本uuid异或常量
const unsigned char kAssistVersionXor = 0xd1;
//连接数量
const unsigned int kGameConnectionMaxCnt = 99999;
#elif defined(__PROJ_JFZR_SM_)
//版本号
const unsigned short kAssistVersion = 0x001;
const char* const kAssistVersionUuidGenerator =
	"9E3AC972-718F-4C00-913B-9594386B10ED";
//版本uuid异或常量
const unsigned char kAssistVersionXor = 0xa3;
//连接数量
const unsigned int kGameConnectionMaxCnt = 1;
#elif defined(__PROJ_JFZR_TWO_)
//版本号
const unsigned short kAssistVersion = 0x001;
const char* const kAssistVersionUuidGenerator =
	"E8DC12A9-09C1-443F-1F17-3A74463BECC4";
//版本uuid异或常量
const unsigned char kAssistVersionXor = 0x3f;
//连接数量
const unsigned int kGameConnectionMaxCnt = 2;
#endif

//版本事件对象名
const char* const kAssistVersionEventObj = "_ComAtlStUads_";

//创建版本校验
bool CreateCheck_AssistVersion();
//进行版本校验
bool CheckAssistVersion();

//游戏模块名字
#define P_INJECTED_GAME_NAME _T("JFZR.exe")
