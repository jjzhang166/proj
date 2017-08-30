#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 存放函数地址或全局变量的地址
*/
#include "Common/MemOper.h"
#include "../../CrackData/jfzrcalltest/jfzrbase.h"
#include "../../CrackData/jfzrcalltest/jfzrstruct.h"
#include "../../CrackData/jfzrcalltest/JFZRPacket.h"
#include "../../CrackData/jfzrcalltest/jfzrpacketstruct.h"

//g_f_s_->stdcall function
//g_f_c_->cdecl function
//g_f_t_->thiscall function
//g_f_a_->其他调用约束的函数
//g_v_->虚函数表
//g_sa_->单字节字符串
//g_sw_->宽字节字符串
namespace GAddr{
	/*
	发送http包
	@1:对象地址
	@2:指向发送内容(ASCII字符串)
	@3:封包有效数据长度
	*/
	//////////////////////////////////////////////////////////////////////////
	//发包套接字,socket
	extern unsigned int						g_pSendCallSocket /*=JFZR_REVERSAL_SEND_PACKET_SOCKET_ADDR*/;
	//const unsigned int						g_pSendCallSocket = JFZR_REVERSAL_SEND_PACKET_SOCKET_ADDR;
	//通过HOOK此处获取发包CALL的加密对象，任务对象的对象地址
	const unsigned int						g_pHookGetAllmMemorySendObjAddr = JFZR_REVERSAL_SEND_ARGC3_ARGC4_HOOK_ADDR;
	//发包call地址
	const unsigned int						g_pSendCallAddr = JFZR_REVERSAL_SEND_PUSH_ARGC3_ARGC4_CALL;
	//发包call之明文发包之地址
	const unsigned int						g_pOriginalSendCallAddr = JFZR_REVERSAL_SEND_ADDR_CALL;
	//发包CALL之前申请对象
	const unsigned int						g_pSendCallConstructObj = JFZR_REVERSAL_SEND_ALLOC_OBJ_CALL;
	//发包CALL之后释放对象
	const unsigned int						g_pSendCallDestroyObj = JFZR_REVERSAL_SEND_FREE_OBJ_CALL;
	//游戏内部申请内存
	const unsigned int						g_pGameAlloc = JFZR_REVERSAL_ALLMMEMORY_ALLOC_ADDR;
	//游戏内部释放内存
	const unsigned int						g_pGameFree = JFZR_REVERSAL_ALLMMEMORY_FREE_ADDR;
	//游戏内部复制宽字符串
	const unsigned int						g_pGameStrCpy = JFZR_REVERSAL_GAME_MEMCPY_CALL;
	//走路命令call
	const unsigned int						g_pWalkCallAddr = JFZR_REVERSAL_ZOULU_CALL;
	//房间里的所有NPC、怪物、活物等，hook这个地址得到这个对象
	const unsigned int						g_pHookGainCreatureInRoom = JFZR_REVERSAL_LOCAL_OBJ_ADDR_HOOK;
	//角色对象/玩家地址
	const unsigned int						g_pRoleObjAddr = JFZR_REVERSAL_ROLE_OBJ_ADDR;
	//人物基址
	const unsigned int						g_pRoleBaseAddr = JFZR_REVERSAL_ROLE_BASE_ADDR;
	//角色运行时序号
	const auto								g_pRoleSkillRunSerialAddr = (DWORD*)JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR;
	//随机种子
	const unsigned int						g_pRandomSeekCall = JFZR_REVERSAL_RANDOMSEED_CALL;
	//通过hook得到房间对象，当前房间的序号、门等
	const unsigned int						g_pHookGainRoomObj = JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK;
	const unsigned int						g_pHookGainTownRoomObj = JFZR_REVERSAL_LOCAL_CITY_OBJ_HOOK;
	//得到副本难度等级
	//const unsigned int						g_pGetCloneMapDiffiLevel = JFZR_REVERSAL_FB_SEACH_CALL;
	//地图对象
	//const unsigned int						g_pGameMapObj_ = JFZR_REVERSAL_ROLE_WHERE_THE_MAP_HOOK;
	//游戏对象析构函数
	//const unsigned int						g_pGameObjDestroy = JFZR_REVERSAL_NPC_OBJ_DESTRUCTOR_HOOK;
	// 判断当前角色是在城镇或者副本，且副本等级，及副本内所有房间数据
	const unsigned int						g_pCurMapObjHook = JFZR_REVERSAL_LOCAL_MAP_OBJ_HOOK;
	//中英文映射
	const unsigned int						g_pEn2ChMapAddr = JFZR_REVERSAL_CHINESE_ENGLISH_MAP_ADDR;
	//当前职业所有任务
	const unsigned int						g_pCurJobAllTask = JFZR_REVERSAL_TASK_OBJ_ADDR1;
	//local all obj map
	const unsigned int						g_pHookLocalAllObjMapAddr = JFZR_REVERSAL_ALLOBJ_MAP_FIND_ADDR_HOOK;
	//技能模板
	const unsigned int						g_pSkillTemplate = JFZR_REVERSAL_SDB_SKILL_ADDR;
	//hook收包
	const unsigned int						g_pHookRecvPacket = JFZR_REVERSAL_RECV_UN_PACKET_HOOK;
	//物品模板
	const unsigned int						g_pItemTemplate = JFZR_REVERSAL_SDB_ITEM_INFO_ADDR;
	//地图模板
	const unsigned int						g_pStageTemplate = JFZR_REVERSAL_SDB_STAGE_INFO_ADDR;
	//游戏环境条件
	const unsigned int						g_pGameAmbientCondition = JFZR_REVERSAL_GAME_CONDITION_HOOK;
	//商品模板
	const unsigned int						g_pShopTemplate = JFZR_REVERSAL_SHOP_INFO_SELL_ITEM_ADDR;
	//hook发包
	const unsigned int						g_pHookGameSendCall = JFZR_REVERSAL_SEND_ADDR_PRE_CALL_HOOK;
	/*
	用来得到主窗口句柄
	hook Call CreateWindowEx的下一个指令
	*/
	//const unsigned int						g_pHookForGainMainWndHandle_ = JFZR_REVERSAL_CREATE_WINDOWN_EX_HOOK;	// 窗口句柄
	//game wnd proc call
	const unsigned int						g_pGameWndProcCall_ = JFZR_REVERSAL_WND_PROC_CALL;
	//class info
	const unsigned int						g_pClassInfoTemplate = JFZR_REVERSAL_CLASS_INFO_ADDR;
	//职业class name
	const unsigned int						g_pJobClassNameTemplate = JFZR_REVERSAL_CLASS_CLASSTYPE_ADDR;
	//region template
	const unsigned int						g_pRegionStageMapTemplate = JFZR_REVERSAL_REGION_INFO_ADDR;
	//掉线
	//const unsigned int						g_pGameOffline = JFZR_REVERSAL_TCPCHANNEL_CLOSE_HOOK___;
	//skill timestamp
	const unsigned int						g_pSkillTimestamp = JFZR_REVERSAL_TIME_STAMP_HOOK;
	//pre action play time
	const unsigned int						g_pPreActionPlayTime = JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_HOOK;
	//get local room pos
	const unsigned int						g_pGetLocalRoomPos = JFZR_REVERSAL_GET_LOCAL_ROOM_POSITION_HOOK;
	//无敌
	const unsigned int						g_pWuDiAddr = JFZR_REVERSAL_WUDI_CODE_ADDR___;
	//////////////////////////////////////////////////////////////////////////
	const MemOper<Oper_ThisCall>			g_pSendCallConstructObj_ = g_pSendCallConstructObj;
	const MemOper<Oper_ThisCall>			g_pSendCallDestroyObj_ = g_pSendCallDestroyObj;
	const MemOper<Oper_ThisCall>			g_pSendCallAddr_ = g_pSendCallAddr;
	const MemOper<Oper_Cdecl>				g_pOriginalSendCallAddr_ = g_pOriginalSendCallAddr;
	const MemOper<Oper_Cdecl>				g_pRandomSeekCall_ = g_pRandomSeekCall;
	const MemOper<Oper_ThisCall>			g_pGameWndProcCal = g_pGameWndProcCall_;
	//const MemOper<Oper_ThisCall>			g_pGetFbUnknownObj = g_pGetCloneMapDiffiLevel;
	//////////////////////////////////////////////////////////////////////////
	//发包对象
	extern void*							g_pSendObjAddr;
	//local object addr
	extern void*							g_pLocalObjAddr;
	//local room object addr
	extern void*							g_pLocalRoomObjAddr;
	//local town room object addr
	extern void*							g_pLocalTownRoomObjAddr;
	//map object
	//extern void*							g_pGameMapObjAddr;
	//当前地图(城镇或副本)
	extern void*							g_pCurMapObjAddr;
	//local all obj map
	extern void*							g_pLocalAllObjMapAddr;
	//skill timestamp
	extern void*							g_pSkillTimestampValue;
	//过图有效数据
	extern DWORD							g_through_room_valid_data_x;
	extern DWORD							g_through_room_valid_data_y;
	extern bool								g_through_room_dummy_exist;
	//pre action play time addr
	extern void*							g_pPreActionPlayTimeAddr;
	//local room pos
	extern void*							g_pLocalRoomPos;
	//////////////////////////////////////////////////////////////////////////
}