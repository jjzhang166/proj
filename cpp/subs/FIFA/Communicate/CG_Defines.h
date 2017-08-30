#pragma once
/*
@author zhp
@date 2014/6/15 15:10
@purpose console.exe与game.dll之间的交互
*/
#include <Protocol/P_Defines.h>

//控制台发给game模块一个固定的字符串
const char* const kStableStr_ConsoleToGame = 
#ifdef __PROJ_JIN_GANG_
	"C5144231-9538-42257-9236-221DaB3353E2f6";
#elif defined(__PROJ_XIAO_ER_)
	"70275346-1A48-43129-8390-3311B92737F18";
#endif
//控制台发给game模块，用来效验证公钥有效性
//const char* const kCheckPubKey_ConsoleToGame = "44C62BB7-D69C-469A-BD80-3DE8F4DF4BDA";

//服务器上接收的关键数据，加上这个值才是真正有效的
//const int kAddrDatasAdditional = 10;

enum enGameAcType{
	kGAT_Normal = 0,
	kGAT_Stored,
	kGAT_Invalid,
};

enum enDstServerType{
	kDST_Normal = 1,
	kDST_Invalid,
};
inline P_CoorTypeT MakeTransRouteType( enGameAcType game_ac_type, int game_ser_id ){
	return game_ac_type + (game_ser_id << 4);
}
