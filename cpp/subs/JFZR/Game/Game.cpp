// Game.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Facility/GAddr.h"
#include "Protocol/P_Defines.h"
#include "FuncObjC.h"
#include "Facility/GClass.h"
#include "Protocol/P_BaseStruct.h"
#include <set>
#include "Facility/GFuncCall.h"
#include <openssl/ssl.h>
#include "Facility/GameObjMgr.h"
#include "Facility/GRoom.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GMap.h"
#include "Facility/GRecvMsgMgr.h"
#include "Facility/GCloneMap.h"
#include <Common/UsefulFunc.h>
//////////////////////////////////////////////////////////////////////////
//API hook
//////////////////////////////////////////////////////////////////////////
using namespace GAddr;
bool InitCodeHooker();
extern bool InitGame()
{
	if ( !InitCodeHooker() )
		return false;
	return true;
}

//for gain send object addr
void* Hooker_GainSendObjAddr( IHooker& hooker )
{
	if (g_pSendObjAddr)
		return hooker.GetCodeOrig();
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "for gain send object";
	g_pSendObjAddr = (void*)ambient->esi_;
	assert(g_pSendObjAddr);
	return hooker.GetCodeOrig();
}

//for gain creature in room
void* Hooker_GainCreatureInRoom( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto new_addr = (void*)ambient->ecx_;
	if ( new_addr != g_pLocalObjAddr )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "for gain creature in room";*/
		g_pLocalObjAddr = new_addr;
		assert(g_pLocalObjAddr);
		/*GetIoService().dispatch([](){
			auto& gameobj_mgr = GameObjMgr::GetMe();
			gameobj_mgr.ResetForceUpdate();
			VERIFY(gameobj_mgr.RebuildUpdateAll());
		});*/
	}
	assert(g_pLocalObjAddr);
	return hooker.GetCodeOrig();
}

//for gain room object
void* Hooker_GainRoomObject( IHooker& hooker )
{
	using namespace GClass;
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto new_addr = (void*)ambient->edi_;
	if ( g_pLocalRoomObjAddr != new_addr )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "for gain room object";*/
		g_pLocalRoomObjAddr = new_addr;
		assert(g_pLocalRoomObjAddr);

		/*GetIoService().dispatch([](){
			VERIFY(GCloneMap::GetMe().Update());
			//GAmbient::GetMe().HandleEnteredFb();
		});*/
	}
	return hooker.GetCodeOrig();
}

//for gain town room object
void* Hooker_GainTownRoomObject( IHooker& hooker )
{
	using namespace GClass;
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto new_addr = (void*)ambient->edi_;
	if ( g_pLocalTownRoomObjAddr != new_addr )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "for gain room object";*/
		g_pLocalTownRoomObjAddr = new_addr;
		assert(g_pLocalTownRoomObjAddr);

		/*GetIoService().dispatch([](){
			VERIFY(GCloneMap::GetMe().Update());
			//GAmbient::GetMe().HandleEnteredFb();
		});*/
	}
	return hooker.GetCodeOrig();
}

//gain game map object
/*
void* Hooker_GainGameMapObject( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto new_addr = (void*)ambient->eax_;
	if ( g_pGameMapObjAddr != new_addr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "for gain game map object";
		g_pGameMapObjAddr = new_addr;
		assert(g_pGameMapObjAddr);
	}
	return hooker.GetCodeOrig();
}*/

//游戏对象析构
/*
void* Hooker_GameObjectDestroy( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	void* game_obj_addr = (void*)ambient->ecx_;
	if ( !game_obj_addr )
	{
		//assert(false);
		return hooker.GetCodeOrig();
	}
	GetIoService().dispatch([game_obj_addr](){
		auto& gameobj_mgr = GameObjMgr::GetMe();
		gameobj_mgr.ResetForceUpdate();
		gameobj_mgr.OnGameObjDestroy(game_obj_addr);
	});
	return hooker.GetCodeOrig();
}
*/

//获得当前地图对象
void* Hooker_GainCurMapObj( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	void* cur_map_obj_addr = (void*)ambient->ecx_;
	if ( !cur_map_obj_addr )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( cur_map_obj_addr != g_pCurMapObjAddr )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "获得当前地图对象";*/
		g_pCurMapObjAddr = cur_map_obj_addr;
		/*GetIoService().dispatch([](){
			auto& map = GMap::GetMe();
			VERIFY(map.Update());
		});*/
	}
	return hooker.GetCodeOrig();
}

//local all obj map
void* Hooker_GainLocalAllObjMap( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	void* local_all_obj_map = (void*)ambient->ecx_;
	if ( !local_all_obj_map )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( local_all_obj_map != g_pLocalAllObjMapAddr )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "获得当前地图所有对象的映射基址";*/
		g_pLocalAllObjMapAddr = local_all_obj_map;
	}
	return hooker.GetCodeOrig();
}

//hook收包
void* Hooker_RecvPacket( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( ambient->ebx_ < sizeof(CMDR_HeaderBase) )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( ambient->ebp_ < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	const auto packet_ptr = *(const void**)(ambient->ebp_ - 0x3c);
	if ( (DWORD)packet_ptr < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto& recv_mgr = GRecvMsgMgr::GetMe();
	WORD ret_offset = recv_mgr.RecvPacket(packet_ptr/*, ambient->ebx_*/);
	return (char*)hooker.GetCodeOrig() + ret_offset;
}

//游戏环境条件
void* Hooker_AmbientCondition( IHooker& hooker )
{
	using namespace GClass;
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	const auto game_str = (const char**)(ambient->esp_ + 8);
	if ( !game_str )
		return hooker.GetCodeOrig();
	if ( (int)game_str < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	
	std::shared_ptr<std::string> str(new std::string(*game_str));
	if ( !str )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	GetIoService().dispatch([str](){
		auto& handle_game_ambient = GAmbient::GetMe();
		handle_game_ambient.HandleAmbient_String(*str);
	});
	return hooker.GetCodeOrig();
}

//HOOK游戏中的发包call
void* Hooker_GameSendCall( IHooker& hooker )
{
	using namespace GClass;
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( ambient->eax_ < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto cmd_header = (CMDR_HeaderBase**)(ambient->eax_ + 8);
	if ( (int)cmd_header < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto& recv_mgr = GHookGameSendMsgMgr::GetMe();
	WORD ret_offset = recv_mgr.HandleMsg(*cmd_header);
	return (char*)hooker.GetCodeOrig() + ret_offset;
}

//HOOK所有发包call
void* Hooker_AllSendCall( IHooker& hooker )
{
	using namespace GClass;
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( ambient->esp_ < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	const auto unknwon_ptr = *(const char**)(ambient->esp_ + 8);
	if ( (int)unknwon_ptr < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}

	auto cmd_header = *(const CMDR_HeaderBase**)(unknwon_ptr + 8);
	if ( (int)cmd_header < (WORD)-1 )
	{
		//assert(false);
		return hooker.GetCodeOrig();
	}
	auto& recv_mgr = GHookAllSendMsgMgr::GetMe();
	WORD ret_offset = recv_mgr.HandleMsg(cmd_header);
	return (char*)hooker.GetCodeOrig() + ret_offset;
}

HookerPtr g_hooker_CreateWindowExW;

HWND WINAPI Hooker_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
							   DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent,
							   HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	typedef HWND (WINAPI *CreateWindowExW_Type)(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
		DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent,
		HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
	if ( !g_hooker_CreateWindowExW )
	{
		assert(false);
		return 0;
	}
	auto old_create_window = (CreateWindowExW_Type)g_hooker_CreateWindowExW->GetCodeOrig();
	if ( !old_create_window )
	{
		assert(false);
		return 0;
	}
	//::Sleep(10000);
	HWND hwnd = old_create_window(dwExStyle,lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
	if ( !hwnd )
	{
		assert(false);
		return 0;
	}
	if (IsUnValidPtr(lpClassName))
		return hwnd;
	static const std::wstring kClsName = L"JFZR";
	if ( kClsName == lpClassName )
	{
		LOG_O(Log_trace) << "hook create window, class name->" << lpClassName;
		GetIoService().post([hwnd](){
			auto func_obj_normal = FuncObj_NormalC::GetInstance();
			if ( !func_obj_normal )
			{
				assert(false);
				return;
			}
			if ( !func_obj_normal->InitInter(hwnd) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "初始化func obj inter失败";
			}
		});
	}
	return hwnd;
}

//for gain main wnd handle
/*
void* Hooker_GainMainWndHandle( IHooker& hooker )
{
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
		return hooker.GetCodeOrig();
	auto inject = nor->GetInterInject();
	if (inject)
	{
		return hooker.GetCodeOrig();
	}
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "通过hook code得到主窗口句柄";
	
	auto hwnd = (HWND)ambient->eax_;
	if ( !hwnd )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	GetIoService().post([hwnd](){
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
		{
			assert(false);
			return;
		}
		if ( !func_obj_normal->InitInter(hwnd) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "初始化func obj inter失败";
		}
	});
	return hooker.GetCodeOrig();
}*/

//游戏掉线
/*void* Hooker_GameOffline( IHooker& hooker )
{
	if (FuncObj_NormalC::GetInstance())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "游戏掉线了";
	}
	assert(false);
	GetIoService().post([](){
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)	
			nor->Disconnect();
	});
	return hooker.GetCodeOrig();
}*/

void* Hooker_GainSkillTimestamp( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if (ambient)
	{
		g_pSkillTimestampValue = (void*)ambient->ecx_;
		assert(g_pSkillTimestampValue);
	}
	else
		assert(false);
	return hooker.GetCodeOrig();
}

void* Hooker_PreActionPlayTime( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	using namespace GAddr;
	if ( (int)g_pPreActionPlayTimeAddr != ambient->ecx_ )
		g_pPreActionPlayTimeAddr = (void*)ambient->ecx_;
	return hooker.GetCodeOrig();
}

void* Hooker_GetLocalRoomPos( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	using namespace GAddr;
	if ( (int)g_pLocalRoomPos != ambient->ecx_ )
		g_pLocalRoomPos = (void*)ambient->ecx_;
	return hooker.GetCodeOrig();
}

//Init code hooker
bool InitCodeHooker()
{
	//return true;
	using namespace GAddr;
	auto& hooker_factory = HookerFactory::GetMe();
	//game main wnd
	/*static auto hooker_gain_main_wnd_handle = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookForGainMainWndHandle_>(
		Hooker_GainMainWndHandle, false);*/
	g_hooker_CreateWindowExW = hooker_factory.CreateCodeHooker(GetProcAddress(LoadLibrary(_T("user32.dll")), "CreateWindowExW"), Hooker_CreateWindowExW);
	static auto hooker_hook_gain_send_obj = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookGetAllmMemorySendObjAddr>(
		Hooker_GainSendObjAddr, false);
	static auto hooker_hook_gain_creature_in_room = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookGainCreatureInRoom>(
		Hooker_GainCreatureInRoom, false);
	static auto hooker_hook_gain_room_obj = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookGainRoomObj>(
		Hooker_GainRoomObject, false);
	static auto hooker_hook_gain_town_room_obj = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookGainTownRoomObj>(
		Hooker_GainTownRoomObject, false);
	/*static auto hooker_hook_gain_game_map_obj = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pGameMapObj_>(
		Hooker_GainGameMapObject, false);*/
	/*static auto hooker_hook_game_obj_destroy = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pGameObjDestroy>(
		Hooker_GameObjectDestroy, false);*/
	static auto hooker_hook_cur_map_obj_addr = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pCurMapObjHook>(
		Hooker_GainCurMapObj, false);
	static auto hooker_local_all_obj_map = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookLocalAllObjMapAddr>(
		Hooker_GainLocalAllObjMap, false);
	static auto hooker_recv_packet = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookRecvPacket>(
		Hooker_RecvPacket, false);
	static auto hooker_ambient_condition = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pGameAmbientCondition>(
		Hooker_AmbientCondition, false);
	static auto hooker_game_send_call = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pHookGameSendCall>(
		Hooker_GameSendCall, false);
	static auto hooker_all_send_call = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pOriginalSendCallAddr>(
		Hooker_AllSendCall, false);
	/*static auto hooker_game_offline = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pGameOffline>(
		Hooker_GameOffline, false);*/
	static auto hooker_gain_skill_timestamp = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pSkillTimestamp>(
		Hooker_GainSkillTimestamp, false);
	static auto hooker_pre_action_play_time = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pPreActionPlayTime>(
		Hooker_PreActionPlayTime, false);
	static auto hooker_get_local_room_pos = hooker_factory.CreateCodeHooker<
		(void*)(int)g_pGetLocalRoomPos>(
		Hooker_GetLocalRoomPos, false);
	return true;
}