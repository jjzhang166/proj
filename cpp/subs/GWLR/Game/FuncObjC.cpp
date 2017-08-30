#include "stdafx.h"
#include "FuncObjC.h"
#include "Common/MemOper.h"
#include "Facility/GFuncCall.h"
#include <boost/bind.hpp>
#include "Facility/GClass.h"
#include "Common/Toolhelp.h"
#include "CfgStructC.h"
#include "Facility/GPlayer.h"
#include "Net/PocoMore.h"
#include <protect/Protect.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Communicate/CG_Defines.h>
#include <Communicate/CGL_Defines.h>
#include "Facility/GPlayer.h"
#include <random>
#include <Common/CmnException.h>
#include "EventsImpl.h"
#include "EventsName.h"
#include "Facility/GameObjMgr.h"
#include <Common/EnsureCleanup.h>
#include "Facility/GConst.h"
#include "Facility/GMonster.h"
#include <dinput.h>
#include "HandleUiScene.h"
#include <Common/CmnDllApi.h>
#include <CD_CommonFiles/Data/CD_ProtocolS.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include "GLua/Scmd.h"
#include "Facility/GRecvMsgMgr.h"

FuncObj_NormalC* g_con_account = nullptr;

FuncObj_NormalC::FuncObj_NormalC() : Connection(kSelfIpStr, P_Port_ForGame), glua_(this), json_echo_(glua_, *this), session_oper_(*this), lua_script_(glua_, *this),
	coor_trans_(*this)
{
	assert(!g_con_account);
	g_con_account = this;
	test_dll_ = nullptr;
	inject_ = nullptr;
	auto_run_script_ = false;
	game_ser_idx_ = 0;
	game_ac_type_ = kGAT_Normal;
	game_state_ = GType::kGS_Invalid;
	multi_hit_rate_ = kMultiHitRate_Min;
	console_process_id_ = 0;
	cur_delete_role_idx_ = -1;
	change_ac_at_delete_one_role_ = true;
	ser_line_ = -1;
	is_you_hua_ = true;
}

bool FuncObj_NormalC::OnModifyAccount( P_Modify_Account& modify_account )
{
	account_ = modify_account.account_;
	password_ = modify_account.password_;
	game_ser_name_ = modify_account.service_name_;
	auto_run_script_ = modify_account.auto_run_script_;
	LOG_O_EMPTY_MSG_FAST(Log_trace);

	return true;
}

bool FuncObj_NormalC::InitGame()
{
	return true;
}

FuncObj_NormalC* FuncObj_NormalC::GetInstance()
{
	return g_con_account;
}

FuncObj_NormalC::~FuncObj_NormalC()
{
	assert(g_con_account);
	g_con_account = nullptr;
	Clear();
}

bool FuncObj_NormalC::OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size )
{
	switch (msg_num)
	{
	case P_Modify_Account::value:
		{
			if ( sizeof(P_Modify_Account) != param_size )
				break;
			return OnModifyAccount(*(P_Modify_Account*)param);
		}
	case P_Exec_TestLoadLib::value:
		{
			if ( sizeof(P_Exec_TestLoadLib) != param_size )
				break;
			return OnTestLoadLib(*(P_Exec_TestLoadLib*)param);
		}
	case P_Exec_TestUnloadLib::value:
		{
			if ( sizeof(P_Exec_TestUnloadLib) != param_size )
				break;
			return OnTestUnloadLib(*(P_Exec_TestUnloadLib*)param);
		}
	case P_Exec_CallLuaFunction::value:
		{
			if ( sizeof(P_Exec_CallLuaFunction) != param_size )
				break;
			return OnCallLuaFunction(*(P_Exec_CallLuaFunction*)param);
		}
	default:
		break;
	}
	return __super::OnUserMsg(msg_num, param, param_size);
}


bool FuncObj_NormalC::OnTestLoadLib( P_Exec_TestLoadLib& load_lib )
{
	if ( !PathFileExistsA(load_lib.str_dll_full_file_path_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "加载的DLL：" << load_lib.str_dll_full_file_path_ << "不存在";
		return true;
	}
	if (test_dll_)
		FreeLibrary(test_dll_);
	test_dll_ = LibraryLoader::LoadTheLibrary(load_lib.str_dll_full_file_path_);
	
	return true;
}

bool FuncObj_NormalC::OnTestUnloadLib( P_Exec_TestUnloadLib& unload_lib )
{
	if (test_dll_)
	{
		auto gwnd = GetInterGWnd();
		if (gwnd)
		{
			gwnd->AsyncExec([this](){
				LOG_O_EMPTY_MSG_FAST(Log_trace);
				if (test_dll_)
				{
					FreeLibrary(test_dll_);
					test_dll_ = nullptr;
					LOG_O(Log_trace) << "游戏主线程成功释放测试DLL";
				}
			});
		}
		else
		{
			FreeLibrary(test_dll_);
			test_dll_ = nullptr;
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "注入线程释放测试DLL成功";
		}
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "不存在已加载的测试DLL";
	}
	return true;
}

FuncObj_Inject* FuncObj_NormalC::GetInterInject()
{
	return inject_;
}

FuncObj_NormalC::InjectedGWnd* FuncObj_NormalC::GetInterGWnd()
{
	if (inject_)
		return &inject_->GetInterGWnd();
	return nullptr;
}

void FuncObj_NormalC::Clear()
{
	if (inject_)
	{
		inject_->DeleteSelf();
		inject_ = nullptr;
	}
}

bool FuncObj_NormalC::InitInter( HWND game_main_wnd )
{
	LOG_ENTER_FUNC;
	if (inject_)
	{
		//LOG_O(Log_debug) << "已经初始化过FuncObj_InterInjectComposite了";
		return false;
	}
	if ( !game_main_wnd )
		return false;
	inject_ = new InjectGWndComposite(this, game_main_wnd);
	LOG_O(Log_debug) << "初始化FuncObj_InterInjectComposite，HWND:" << std::hex << (DWORD)game_main_wnd << std::dec;
	
	if ( !inject_ )
	{
		assert(false);
		return false;
	}
	bool is_executed = false;
	inject_->GetInterGWnd().SyncExec(is_executed, [&is_executed](){
		return true;
	});
	if ( !is_executed )
	{
		assert(false);
		Disconnect_Prompt("未知错误1");
		return false;
	}
/*
#ifndef _DEBUG
	ShowWindow(game_main_wnd, SW_MINIMIZE);
#endif*/
	return true;
}

GLuaC& FuncObj_NormalC::GetGLua()
{
	return glua_;
}

bool FuncObj_NormalC::OnCallLuaFunction( P_Exec_CallLuaFunction& call_lua_func )
{
	LOG_ENTER_FUNC;
	auto func_res = glua_.Call<std::string>(call_lua_func.call_func_name, "glua_.Call没有执行成功", call_lua_func.func_param);
	P_Exec_CallLuaFuncResult msg_res;
	strcpy_s(msg_res.func_result, func_res.c_str());
	strcpy_s(msg_res.call_func_name, call_lua_func.call_func_name);
	strcpy_s(msg_res.func_param, call_lua_func.func_param);
	LOG_O(Log_debug) << "LUA函数名：" << msg_res.call_func_name << "。参数：" << msg_res.func_param << "。结果：" << msg_res.func_result;
	return UserMsg(P_Exec_CallLuaFuncResult::value, &msg_res, sizeof(msg_res));
}

FuncObj_NormalC::InjectedGWnd* FuncObj_NormalC::GetGWnd()
{
	auto func_obj_normal = GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "FuncObj_NormalC实例还未创建";
		return nullptr;
	}
	auto gwnd = func_obj_normal->GetInterGWnd();
	if ( !gwnd )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "Gwnd还未创建";
	}
	return gwnd;
}

JsonEcho& FuncObj_NormalC::GetJsonEcho()
{
	return json_echo_;
}

void SessionOperC::ForegroundTheWindow()
{
	auto gwnd = func_obj_normal_.GetGWnd();
	if (gwnd)
	{
		gwnd->ForegroundWindow();
	}
	else
		assert(false);
}

const std::string& FuncObj_NormalC::GetAccount() const
{
	return account_;
}

SessionOperC& FuncObj_NormalC::GetSessionOper()
{
	return session_oper_;
}

void FuncObj_NormalC::BanAccount()
{
	session_oper_.ToCall(BC_MakeP("BanAccount"));
}

bool FuncObj_NormalC::PreConnect()
{
	if ( !__super::PreConnect() )
		return false;
	return true;
}

void FuncObj_NormalC::SetAddrDatas( const AddrDatasT& addr_datas )
{
	LOG_ENTER_FUNC;
	for ( auto& v : addr_datas )
	{
		LOG_O(Log_trace) << "设置关键数据->" << v;
	}
	//kAddrDatasAdditional;
	/*assert(1 == addr_datas.size());
	if ( kStaticAddrData != addr_datas[0] )
	{
		assert(false);
		GetIoService().post([](){
			throw CmnExitIoThreadException("ivsad");
		});
	}*/
}

BufCallCoorTransImpl& FuncObj_NormalC::GetCoorTrans()
{
	return coor_trans_;
}

void FuncObj_NormalC::SetGameState( GType::enGameState game_state )
{
	/*
	不要加判断，否则后面的更新函数执行不到了。
	if (game_state == game_state_)
		return;*/
	using namespace GType;
	auto old_state = game_state_;
	//LOG_ENTER_FUNC;
	//LOG_O(Log_trace) << "SetGameState3,old" << old_state << " new:" << game_state_;
	game_state_ = game_state;
	DoInitByGameState(old_state, game_state);
	GPlayerMe::GetMe().ClrMySellList();
}

GType::enGameState FuncObj_NormalC::GetGameState() const
{
	return game_state_;
}

void FuncObj_NormalC::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);
	Clear();
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "断开连接，关闭进程";
	/*GetIoService().post([](){
		throw CmnCloseProcessException("fdjklac");
	});*/
	GetIoService().post([](){
		throw CmnTerminateProcessException();
	});
}

extern bool InitGame();
bool FuncObj_NormalC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	P_Exec_GameProccessInfo msg;
	msg.self_process_id_ = GetCurrentProcessId();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "current process id->" << msg.self_process_id_;
	msg.login_client_proccess_id_ = msg.self_process_id_;
	//LOG_O(Log_trace) << "parent process id->" << msg.login_client_proccess_id_;
	if ( !UserMsg(P_Exec_GameProccessInfo::value, &msg, sizeof(msg)) )
		return false;
	if ( !glua_.InitScript(GlobalSetter::GetMe().GetSelfModuleHandle(), _T("main")) )
		return false;
	VMP_BEGIN_EX;
	if ( !CheckAssistVersion() )
	{
		assert(false);

		SetIoTimer(25 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			throw CmnExitIoThreadException("ge");
			VMP_END;
		});
	}
	//////////////////////////////////////////////////////////////////////////
	LOG_O(Log_trace) << "等待验证信息";

	//等待接收成功信息
	if ( !WaitUntil([this](){
		return login_succeed_;
	}, 5 * 60 * 1000) )
	{
		assert(false);
		DoSomeError();
	}

	if ( !login_succeed_ )
	{
		LOG_O(Log_debug) << "login_succeed竟然还未初始化";
		DoSomeError();
	}
	if ( !*login_succeed_ )
	{
		LOG_O(Log_debug) << "login_succeed验证未通过";
		DoSomeError();
	}

	//等待接收结束日期
	if ( !WaitUntil([this](){
		return end_date_time_;
	}, 30 * 1000) )
	{
		assert(false);
		DoSomeError();
	}

	if ( !end_date_time_ )
	{
		LOG_O(Log_debug) << "end_date_time_竟然还未初始化";
		DoSomeError();
	}
	DateTimeLocal time_now;
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "time now->" << Poco::DateTimeFormatter::format(time_now, Poco::DateTimeFormat::SORTABLE_FORMAT)
		<< "\tend date time->" << Poco::DateTimeFormatter::format(*end_date_time_, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	if ( time_now >= *end_date_time_ )
	{
		LOG_O(Log_debug) << "time_now大于到期时间";
		DoSomeError();
	}
	SetIoTimer(1000 * 60 * 5, [this]( IoTimer* timer ){
		DateTimeLocal time_now;
		if ( time_now >= *end_date_time_ )
		{
			assert(false);
			throw CmnCloseProcessException("tmollda");
		}
		assert(timer);
		timer->Continue();
	});

	if ( !WaitUntil([this](){
		return console_process_id_ != 0;
	}, 5 * 60 * 1000) )
	{
		assert(false);
		DoSomeError();
	}

	//only test
	//VERIFY(GetSessionOper().ThisMemCall(nullptr, "GameLoginSucceed", nullptr));

	if ( !WaitUntil([=](){
		return !account_.empty();
	}, 1000 * 60) )
		return false;
	LOG_O(Log_trace) << "验证信息接收成功";
	VMP_END;
	//////////////////////////////////////////////////////////////////////////
	if ( !::InitGame() )
		return false;
	InitInter(GlobalSetter::GetMe().GetGameMainWnd());
	handle_at_hitting_.reset(new CmnPriLuaData(glua_.lua()));
	handler_after_entered_fb_.reset(new CmnPriLuaData(glua_.lua()));
	handlers_on_script_stoped_.reset(new CmnPriLuaData(glua_.lua()));
	GPlayerMe::GetMe().InitItemBlackWhiteList();
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool FuncObj_NormalC::WaitUntilGameState( GType::enGameState game_state, std::size_t time_out )
{
	if ( game_state == game_state_ )
		return true;
	WaitUntil([game_state, this](){
		return game_state == game_state_;
	}, time_out);
	return game_state == game_state_;
}

void FuncObj_NormalC::Test()
{
	GetCoorTrans().GainOtherSidePrivateItems();
}

LuasScriptEchoC_Impl& FuncObj_NormalC::GetLuaScript()
{
	return lua_script_;
}

int FuncObj_NormalC::GetMultiHitRate() const
{
	return multi_hit_rate_;
}

void FuncObj_NormalC::LeftButtonClick( int pos_x, int pos_y )
{
	GWndExecSync([=](){
		return CD_FuncCall::HandleMouseMsg(pos_x, pos_y, CD_En_MouseOper::kLeftBtDown);
	});	
	DummySleep(100);
	GWndExecSync([=](){
		return CD_FuncCall::HandleMouseMsg(pos_x, pos_y, CD_En_MouseOper::kLeftBtUp);
	});
}

std::shared_ptr<CmnPriLuaData>& FuncObj_NormalC::GetHandleAtHitting()
{
	return handle_at_hitting_;
}

bool FuncObj_NormalC::SendChooseRole( DWORD role_id, int role_idx )
{
	using namespace GType;
	if ( role_idx >= _countof(kRoleListButtons) )
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	on_ser_line_.reset();
	//////////////////////////////////////////////////////////////////////////
#ifdef ROLE_USE_PACKET_
	stCD_Msg_ChooseRole msg;
	msg.role_id_ = role_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
#else
	auto& ui_mgr = UiSceneMgr::GetMe();
	ui_mgr.ClearUiStateHandlers();
	ui_mgr.ReplaceUiStateHandler(kUS_StartGame, [role_idx](){
		LOG_O(Log_trace) << "处理开始游戏状态";
		if ( !GFC_Ui_ClickButton(kRoleListButtons[role_idx]) )
		{
			LOG_O(Log_debug) << "点击UI失败：" << kRoleListButtons[role_idx];
			return false;
		}
		DummySleep(1000);
		return GFC_Ui_ClickButton(kUiStr_StartGame);
	});
	if ( !ui_mgr.DoWaitHandleUntil(&UiSceneMgr::DoHandleUiState_SelectRole, true, 1000 * 60) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "未能成功处理选择角色";
		return false;
	}
#endif
	//////////////////////////////////////////////////////////////////////////

	if ( !WaitUntil([this](){
		if (on_ser_line_)
			return true;
		if ( kGS_InGame == game_state_ )
			return true;
		return false;
	}, 1000 * 60 * 2) )
	{
		Disconnect_Prompt("等待选择服务器线路超时失败");
		return false;
	}
	if ( kGS_InGame == game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "不需要选择游戏线路，竟然莫名进入到游戏了";
		return true;
	}
	if ( !on_ser_line_ )
	{
		assert(false);
		return false;
	}
	
	auto kLinesSize = (int)on_ser_line_->lines_info_.size();
	if ( ser_line_ < 0 || ser_line_ >= kLinesSize )
	{
		if ( ser_line_ >= kLinesSize )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "没有这样的服务器线路：" << ser_line_ + 1 << " 最多只有这些个线路：" << kLinesSize << " 所以默认选择最空闲服务器线路";
		}
		auto it = std::min_element(on_ser_line_->lines_info_.begin(), on_ser_line_->lines_info_.end(), []( const CMDR_PlayerLine::stLineInfo& lhs, const CMDR_PlayerLine::stLineInfo& rhs ){
			return lhs.line_lvl_ < rhs.line_lvl_;
		});
		if ( it == on_ser_line_->lines_info_.end() )
		{
			assert(false);
			return false;
		}
		return SendChooseLine(it->line_idx_);
	}
	else
	{
		return SendChooseLine(on_ser_line_->lines_info_[ser_line_].line_idx_);
	}
}

bool FuncObj_NormalC::SendChooseLine( DWORD line_id )
{
	stCD_Msg_ChooseLine msg;
	msg.line_id_ = (pt_word)line_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	return true;
}

void FuncObj_NormalC::SetRolesList( const std::shared_ptr<CMDR_PlayerList>& player_list )
{
	players_list_ = player_list;
	cur_players_list_ = players_list_;
}

bool FuncObj_NormalC::SelectRoleByName( const std::string& role_name )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "选择角色进入游戏，角色名：" << role_name;
	if (role_name.empty())
	{
		assert(false);
		return false;
	}
	if ( !cur_players_list_ )
	{
		assert(false);
		return false;
	}
	int role_idx = 0;
	auto it = cur_players_list_->players_info_.begin();
	for ( ; it != cur_players_list_->players_info_.end(); ++it, ++role_idx )
	{
		if ( it->player_name_ == role_name )
			break;
	}
	if ( it == cur_players_list_->players_info_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "选择指定名字的角色之没有指定的角色名，角色名：" << role_name;
		return false;
	}
	return SelectRoleByIdx(role_idx);
}

bool FuncObj_NormalC::SelectRoleByIdx( int role_idx )
{
	using namespace GType;
	if ( role_idx < 0 )
	{
		assert(false);
		return false;
	}
	if ( !cur_players_list_ )
	{
		assert(false);
		return false;
	}
	
	role_idx = std::min(role_idx, (int)cur_players_list_->players_info_.size() - 1);
	if ( role_idx < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有角色可选";
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (gpm.is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	gpm.is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&gpm){
		gpm.is_active_cross_map_ = false;
	};

	if ( !SendChooseRole(cur_players_list_->players_info_[role_idx].player_id_, role_idx) )
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if ( kGS_InGame == game_state_ )
			return true;
		return false;
	}, 1000 * 60 * 5) )
	{
		Disconnect_Prompt("选择角色后，等待进入游戏超时失败");
		return false;
	}
	/*if ( !WaitCrossedGraph(1000 * 60 * 5) )
		return false;*/

	ClrRolesList();

	return scmd::GetMe().WaitRoleCanMove(1000 * 60 * 10);
}

void FuncObj_NormalC::ClrRolesList()
{
	players_list_.reset();
}

const std::shared_ptr<CMDR_PlayerList>& FuncObj_NormalC::GetRolesList() const
{
	return players_list_;
}

bool FuncObj_NormalC::SendCreateRole( const std::string& role_name )
{
	using namespace GType;
	on_hook_create_role_.reset();

#ifdef ROLE_USE_PACKET_
	stCD_Msg_CreateRole msg;
	msg.ukn_ = { 0x31 ,0x32 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
		0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
		0x00 ,0x01 ,0x00 ,0x01 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x39 ,0x3D ,0x48 ,0x00 ,0x96, \
		0x96 ,0x96 ,0x00 ,0x32 ,0x00 ,0x00 ,0x00 ,0x60 ,0x71 ,0x7B ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, \
		0x00, 0xFF
	};
	memcpy_s(msg.ukn_,20,role_name.c_str(),role_name.size());
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
#else
	auto& ui_mgr = UiSceneMgr::GetMe();
	ui_mgr.ClearUiStateHandlers();
	ui_mgr.ReplaceUiStateHandler(kUS_OldPlayer, [](){
		LOG_O(Log_trace) << "处理我是老猎人状态";
		auto res = GFC_Ui_ClickButton(kUiStr_IamOlPlayer);
		DummySleep(1000);
		return res;
	});
	ui_mgr.ReplaceUiStateHandler(kUS_RandomSkin, [](){
		LOG_O(Log_trace) << "处理随机皮肤和脸型状态";
		if (!GFC_Ui_ClickButton(kUiStr_RandomSkin))
		{
			LOG_O(Log_debug) << "点击失败：" << kUiStr_RandomSkin;
			return false;
		}
		DummySleep(1000);
		auto res = GFC_Ui_ClickButton(kUiStr_CreateRoleNextStep);
		DummySleep(1000);
		return res;
	});
	const auto& wstr_role_name = GbkToWideChar(role_name);
	ui_mgr.ReplaceUiStateHandler(kUS_CreateRoleOk, [wstr_role_name, &ui_mgr](){
		LOG_O(Log_trace) << "处理完成创建状态";
		if (!GFC_Ui_PutString(kUiStr_CreateRole_InputNameEdit, wstr_role_name.c_str()))
		{
			LOG_O(Log_debug) << "点击失败：" << kUiStr_CreateRole_InputNameEdit;
			return false;
		}
		DummySleep(1000);
		auto res = GFC_Ui_ClickButton(kUiStr_CreateRoleOk);
		if (res)
			ui_mgr.SetBreakAtDoUntil(true);
		DummySleep(1000);
		return res;
	});
	ui_mgr.ReplaceUiStateHandler(kUS_CreateRoleBt6, [](){
		LOG_O(Log_trace) << "处理第六个创建新角色按钮状态";
		auto res = GFC_Ui_ClickButton(kRoleListButtons[_countof(kRoleListButtons) - 1]);
		DummySleep(1000);
		return res;
	});
	if ( !ui_mgr.DoWaitHandleUntil(&UiSceneMgr::DoHandleUiState_SelectRole, true, 1000 * 20) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "未能成功创建角色1";
		return false;
	}
	if ( !ui_mgr.DoWaitHandleUntil(&UiSceneMgr::DoHandleUiState_SelectRole, false, 1000 * 20) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "未能成功创建角色2";
		return false;
	}
#endif

	return WaitUntil([this](){
		if (on_hook_create_role_)
			return true;
		return false;
	}, 1000 * 2);
}

bool FuncObj_NormalC::CreateTheRole( const std::string& role_name )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "开始创建角色，角色名：" << role_name;
	if (role_name.empty())
	{
		assert(false);
		return false;
	}
	on_create_player_.reset();
	players_list_.reset();

	assert(role_name.size() <= kRoleNameMaxLen);
	if ( !SendCreateRole(role_name) )
	{
		//assert(false);
		return false;
	}
	WaitUntil([](){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return true;
		if (norc->on_create_player_)
			return true;
		return false;
	}, 1000 * 60 * 2);
	if ( !on_create_player_ )
	{
		Disconnect_Prompt("创建角色之长时间未收到创建结果");
		DummySleep(2000);
		return false;
	}

	if ( CMDR_OnCreatePlayer::kEC_Reged == on_create_player_->res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "创建角色失败，角色名已被注册";
		return false;
	}
	else if ( CMDR_OnCreatePlayer::kEC_InvalidChar == on_create_player_->res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "角色名含有非法字符，角色名：" << role_name;
		return false;
	}
	else if ( CMDR_OnCreatePlayer::kEC_CntLimit == on_create_player_->res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "创建角色失败，角色数量已达到上限";
		return false;
	}
	else if ( 0 != on_create_player_->res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "创建角色失败，未知原因：" << on_create_player_->res_;
		return false;
	}

	if ( !WaitUntil([=](){
		if (players_list_)
			return true;
		return false;
	}, 1000 * 60 * 5) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "虽然创建成功了，但是长时间未收到角色信息";
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "创建角色成功，角色名：" << role_name;
	return true;
}

void FuncObj_NormalC::SetCreateRoleRes( const std::shared_ptr<CMDR_OnCreatePlayer>& on_create_role )
{
	on_create_player_ = on_create_role;
}

bool FuncObj_NormalC::AutoCreateRole()
{
	using namespace GClass;
	std::string str_role_name = GRoleCnNameMaker::GetMe().GenRandomStr(kEmptyStr);
	if (str_role_name.empty())
		str_role_name = Random_Str(kRoleNameMinLen, kRoleNameMaxLen);
	int err_cnt = 0;
	while ( !CreateTheRole(str_role_name) )
	{
		++err_cnt;
		if ( err_cnt > 30 )
		{
			Disconnect_Prompt("创建角色失败次数过多");
			return false;
		}
		if ( !on_create_player_ )
		{
			/*LOG_ENTER_FUNC;
			LOG_O(Log_error) << "等待创建角色失败";
			return false;*/
			str_role_name = GRoleCnNameMaker::GetMe().GenRandomStr(kEmptyStr);
			if (str_role_name.empty())
				str_role_name = Random_Str(kRoleNameMinLen, kRoleNameMaxLen);
			continue;
		}
		if ( CMDR_OnCreatePlayer::kEC_Reged == on_create_player_->res_ )
		{
			str_role_name = GRoleCnNameMaker::GetMe().GenRandomStr(kEmptyStr);
			if (str_role_name.empty())
				str_role_name = Random_Str(kRoleNameMinLen, kRoleNameMaxLen);
			continue;
		}
		if ( CMDR_OnCreatePlayer::kEC_InvalidChar == on_create_player_->res_ )
		{
			str_role_name = GRoleCnNameMaker::GetMe().GenRandomStr(kEmptyStr);
			if (str_role_name.empty())
				str_role_name = Random_Str(kRoleNameMinLen, kRoleNameMaxLen);
			continue;
		}
		else if ( CMDR_OnCreatePlayer::kEC_CntLimit == on_create_player_->res_ )
		{
			GetSessionOper().ToCall(BC_MakeP("SetRolesUseOut"));
			return false;
		}
		else
			return false;
	}
	return true;
}

bool FuncObj_NormalC::RetSelectRole()
{
	using namespace GType;
	if ( kGS_SelectRole == game_state_ )
		return true;
	if ( kGS_InGame != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "并没有在游戏中，无法返回到选择角色界面";
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if ( !gpm.GiveUpFb() )
		return false;
	/*
	gpm.Update();
	auto role_packet_id = gpm.GetPacketId();*/
	auto old_gs = GetGameState();
	game_state_ = kGS_Invalid;
	bool is_resume = true;
	BOOST_SCOPE_EXIT_ALL(this, &is_resume, old_gs){
		if (is_resume)
		{
			game_state_ = old_gs;
			//LOG_O(Log_trace) << "SetGameState1,old" << old_gs << " new:" << game_state_;
		}
		else
		{
			//LOG_O(Log_trace) << "SetGameState2,old" << old_gs << " new:" << game_state_;
		}
	};
	/*if ( !GWndExecSync([role_packet_id](){
		return ::MMO_RetutnChooseRole(role_packet_id);
	}) )
	{
		return false;
	}*/
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "开始返回选择角色界面";
	for ( int i = 0; i != 12; ++i )
	{
		DxPressKeyEsc();
		DummySleep(800);
		LeftButtonClick(504, 397);
		if (scmd::GetMe().WaitForSelectableRole(1000))
		{
			is_resume = false;
			return true;
		}
	}
	LOG_O(Log_error) << "返回角色界面失败";
	return false;
}

void FuncObj_NormalC::InitOnceAfterInGame()
{
	using namespace GClass;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "InitOnceAfterInGame";
	CraftDbMgr::GetMe().Update();
	GManufactureDbMgr::GetMe().Update();
	ShopDb::GetMe().Update();
	MapDbMgr::GetMe().Update();
	ItemDbMgr::GetMe().RebuildAll();
	GTaskDbMgr::GetMe().RebuildAll();
	GClass::FarmDbMgr::GetMe().Update();
	GClass::FarmSeedDbMgr::GetMe().Update();
	GClass::GTaskFbInfo::GetMe().Update();

	coor_trans_.GainOtherSidePrivateItems();
}

void FuncObj_NormalC::ExecAfterInGame()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "ExecAfterInGame";
	auto& gpm = GPlayerMe::GetMe();
	gpm.ExecAtEveryRoleEnterGame();
	GameObjMgr::GetMe().RebuildAll();
	GMailMgr::GetMe().RebuildAll();
	GMonster_HitPart::GetMonsterHitPart().ClrAll();
}

void FuncObj_NormalC::DxPressKey( DWORD virtual_key, int delay_time )
{
	BOOST_SCOPE_EXIT_ALL(=){
		DxKeyUpDown(virtual_key, false);
	};
	DxKeyUpDown(virtual_key, true);
	DummySleep(delay_time);
}

void FuncObj_NormalC::DxPressKeyZ()
{
	DxPressKey(DIK_Z, 100);
}

void FuncObj_NormalC::DxPressKeyW()
{
	DxPressKey(DIK_W, 130);
}

void FuncObj_NormalC::DxPressKeyS()
{
	DxPressKey(DIK_S, 130);
}

void FuncObj_NormalC::DxPressKeyA()
{
	DxPressKey(DIK_A, 130);
}

void FuncObj_NormalC::DxPressKeyD()
{
	DxPressKey(DIK_D, 130);
}

void FuncObj_NormalC::DxPressKeyQ()
{
	DxPressKey(DIK_Q, 100);
}

void FuncObj_NormalC::DxPressKeySpace()
{
	DxPressKey(DIK_SPACE, 100);
}

void FuncObj_NormalC::DxPressKeyEsc()
{
	DxPressKey(DIK_ESCAPE, 100);
}

void FuncObj_NormalC::ExecAfterCrossMap()
{
	auto& gpm = GPlayerMe::GetMe();
	if ( !gpm.is_active_cross_map_ )
		gpm.OnCrossMap();
}

void FuncObj_NormalC::DoInitByGameState( GType::enGameState old_state, GType::enGameState new_state )
{
	using namespace GType;
	static bool is_entered = false;
	//LOG_O(Log_trace) << "DoInitByGameState";
	if (is_entered)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "DoInitByGameState已经进入了";
		return;
	}
	is_entered = true;
	auto entered_ptr = &is_entered;
	BOOST_SCOPE_EXIT_ALL(entered_ptr){
		*entered_ptr = false;
	};
	DoInitAfterGameState(new_state, false);

	if ( kGS_InGame == new_state )
	{
		if ( kGS_SelectRole == old_state )
		{
			static bool init_once = false;
			if ( !init_once )
			{
				init_once = true;
				InitOnceAfterInGame();
			}
			ExecAfterInGame();
			ExecAfterCrossMap();
		}
		else if ( kGS_InGame == old_state )
		{
			ExecAfterCrossMap();
		}
	}
}

enGameAcType FuncObj_NormalC::SwitchGameAcType() const
{
	switch (game_ac_type_)
	{
	case kGAT_Normal:
		return kGAT_Stored;
	case kGAT_Stored:
		return kGAT_Normal;
	default:
		assert(false);
		return kGAT_Invalid;
	}
}

bool FuncObj_NormalC::AutoCreateOrSelectRole()
{
	using namespace GType;
	if ( kGS_SelectRole != game_state_ )
	{
		assert(false);
		return false;
	}
	on_create_player_.reset();
	if ( !session_oper_.ToCall(BC_MakeP("AutoCreateOrSelectRole")) )
	{
		assert(false);
		return false;
	}
	return WaitUntil([this](){
		if ( kGS_InGame == game_state_ || on_create_player_ )
			return true;
		return false;
	}, 1000 * 60 * 10);
}

std::shared_ptr<CmnPriLuaData>& FuncObj_NormalC::GetHandleAfterEnteredFb()
{
	return handler_after_entered_fb_;
}

void FuncObj_NormalC::AfterRunScript()
{
	using namespace luabind;
	cur_delete_role_idx_ = -1;

	if (handle_at_hitting_)
		handle_at_hitting_->SetLuaObj(kHandleAtHitting, luabind::object());
	if (handler_after_entered_fb_)
		handler_after_entered_fb_->SetLuaObj(kHandleAfterEnterFb, luabind::object());
	if (handlers_on_script_stoped_)
	{
		auto handle_tbl = handlers_on_script_stoped_->GetLuaObj(kHandlersOnScriptStoped);
		if ( handle_tbl.is_valid() && LUA_TTABLE == type(handle_tbl) )
		{
			for ( iterator it(handle_tbl), it_end; it != it_end; ++it )
			{
				object tbl_item = *it;
				if ( LUA_TFUNCTION != type(tbl_item) )
				{
					assert(false);
					continue;
				}
				CallFunction(tagNoResult(), tbl_item);
			}
		}
		handlers_on_script_stoped_->SetLuaObj(kHandlersOnScriptStoped, luabind::object());
	}
}

void FuncObj_NormalC::DxPressKeyGrave()
{
	DxPressKey(DIK_GRAVE, 100);
}

void FuncObj_NormalC::DoInitAfterGameState( GType::enGameState new_state, bool force_succeed_notify )
{
	using namespace GType;
	//////////////////////////////////////////////////////////////////////////
	static bool auto_run_script_at_first = false;
	static bool notify_login_succeed = false;
	if ( !auto_run_script_at_first )
	{
		if ( kGS_SelectRole == new_state )
		{
			//////////////////////////////////////////////////////////////////////////
			/*typedef void (*MyUseGDIWidthCache)();
			auto the_lpk_func = (MyUseGDIWidthCache)LibFuncLoader::GetTheProcAddress(LibraryLoader::LoadTheLibrary("lpk.dll"), "MyUseGDIWidthCache");
			assert(the_lpk_func);
			if (the_lpk_func)
				the_lpk_func();*/
			BeginHandleRubThrd();
			//////////////////////////////////////////////////////////////////////////
			auto_run_script_at_first = true;
			if (auto_run_script_)
			{
				//太快了运行脚本不好
				SetIoTimer(1000 * 6, []( IoTimer* ){
					auto norc = FuncObj_NormalC::GetInstance();
					if ( !norc )
						return;
					norc->GetSessionOper().ToCall(BC_MakeP("StartRunScript"));
				});
			}
			notify_login_succeed = true;
			VERIFY(GetSessionOper().ToCall(BC_MakeP("GameLoginSucceed")));
			/*if (timer_check_login_valid_)
			{
				timer_check_login_valid_->Cancel();
				timer_check_login_valid_.reset();
			}*/
		}
	}
	if ( !notify_login_succeed && force_succeed_notify )
	{
		notify_login_succeed = true;
		VERIFY(GetSessionOper().ToCall(BC_MakeP("GameLoginSucceed")));
	}
	//////////////////////////////////////////////////////////////////////////
}

void FuncObj_NormalC::DxKeyUpDown( DWORD virtual_key, bool is_down )
{
	GWndExecSync([=](){
		CD_FuncCall::PressKey(virtual_key, is_down);
		return true;
	});	
}

void FuncObj_NormalC::SetStrDatas( const StrDatasT& str_datas )
{
	LOG_ENTER_FUNC;
	for ( auto& v : str_datas )
	{
		LOG_O(Log_trace) << "game收到str data:" << v;
	}
}

void FuncObj_NormalC::Disconnect_Prompt( const std::string& prompt_str )
{
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
		throw CmnTerminateProcessException("iueqpd;ac.");
	if ( !prompt_str.empty() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << prompt_str;
		std::string msg_prompt = prompt_str;
		msg_prompt += '\n';
		msg_prompt += "是否要关闭游戏";
		if ( IDCANCEL == G_MessageBoxTimeoutA(msg_prompt.c_str(), nullptr, 1000 * 8) )
			return;
	}

	/*LOG_O(Log_trace) << "想要关闭连接：" << prompt_str;
	return;*/

	nor->Disconnect();
	/*SetIoTimer(1000 * 60, []( IoTimer* ){
		throw CmnCloseProcessException("rehuioqsdfc");
	});*/
}

void FuncObj_NormalC::Disconnect()
{
	__super::Disconnect();
}

std::shared_ptr<CmnPriLuaData>& FuncObj_NormalC::GetHandlersAtScriptStoped()
{
	return handlers_on_script_stoped_;
}

bool FuncObj_NormalC::DeleteRole()
{
	if ( cur_delete_role_idx_ < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "当前没有要删除的角色";
		return false;
	}
	if ( !cur_players_list_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有角色列表信息";
		return false;
	}
	if ( cur_delete_role_idx_ >= (int)cur_players_list_->players_info_.size() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法删除角色，因为要删除的角色索引不正确，角色索引：" << cur_delete_role_idx_ + 1 << " 而只有这些个角色：" << cur_players_list_->players_info_.size();
		return false;
	}
	while ( !SendDeleteRole(cur_players_list_->players_info_[cur_delete_role_idx_].player_id_) )
	{
		if ( !on_delete_role_ )
			return false;
		if ( CMDR_OnDeleteRole::kEC_Protected == on_delete_role_->res_ )
		{
			if ( on_delete_role_->protect_time_ < 0 )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "等待时间不能小于0秒，而等待时间：" << on_delete_role_->protect_time_;
				return false;
			}
			else if ( on_delete_role_->protect_time_ > 0 )
				DummySleep(1000 * on_delete_role_->protect_time_);
			continue;
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "删除角色失败，未知原因：" << on_delete_role_->res_;
			return false;
		}
	}
	return true;
}

bool FuncObj_NormalC::SendDeleteRole( DWORD role_id )
{
	on_delete_role_.reset();
	stCD_Msg_DeleteRole msg;
	msg.role_id_ = role_id;
	if (!CD_FuncCall::SendT(msg))
		return false;
	if ( !WaitUntil([this](){
		if (on_delete_role_)
			return true;
		return false;
	}, 1000 * 60 * 10) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "删除角色等待超时";
		return false;
	}
	if ( !on_delete_role_ )
	{
		assert(false);
		return false;
	}
	if ( CMDR_OnDeleteRole::kEC_Succeed == on_delete_role_->res_ || CMDR_OnDeleteRole::kEC_Deleting == on_delete_role_->res_ )
		return true;
	return false;
}


bool GWndExecSync( const std::function<bool()>& func )
{
	bool res = false;
	GWndExecSyncT(res, func);
	return res;
}

bool MakeTransRoute( P_CoorPacketToT& out_route, enGameAcType game_ac_type, int ser_id )
{
	assert(game_ac_type >= kGAT_Normal && game_ac_type < kGAT_Invalid);
	while ( !out_route.empty() )
		out_route.pop();
	P_WayToNextCoor way_tmp;
	way_tmp.method_ = kWTNC_ByType;
	way_tmp.coor_type_ = kDST_Normal;
	out_route.push(way_tmp);
	way_tmp.coor_type_ = kPCT_Any;
	out_route.push(way_tmp);
	way_tmp.coor_type_ = MakeTransRouteType(game_ac_type, ser_id);
	out_route.push(way_tmp);
	return true;
}

P_CoorPacketToT MakeTransRoute( enGameAcType game_ac_type, int ser_id )
{
	P_CoorPacketToT out_route;
	VERIFY(MakeTransRoute(out_route, game_ac_type, ser_id));
	return out_route;
}

const P_CoorPacketToT& GetTransRouteAny()
{
	static P_CoorPacketToT out_route;
	if ( !out_route.empty() )
		return out_route;
	P_WayToNextCoor way_tmp;
	way_tmp.method_ = kWTNC_ByType;
	way_tmp.coor_type_ = kDST_Normal;
	out_route.push(way_tmp);
	way_tmp.coor_type_ = kPCT_Any;
	out_route.push(way_tmp);
	way_tmp.coor_type_ = kPCT_Any;
	out_route.push(way_tmp);
	return out_route;
}

SessionOperC::SessionOperC( FuncObj_NormalC& func_obj_normal ) : BufCall_SocketExcept(&func_obj_normal, kObjIdNormal),
	func_obj_normal_(func_obj_normal)
{

}

void SessionOperC::SetLoginSucceed( bool succeed )
{
	VMP_BEGIN_EX;
	func_obj_normal_.login_succeed_ = succeed;
	VMP_END;
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "验证登陆结果->" << succeed;
}

void SessionOperC::SetEndDateTime( std::time_t end_time )
{
	VMP_BEGIN_EX;
	Poco::Timestamp poco_timestamp = Poco::Timestamp::fromEpochTime(end_time);
	func_obj_normal_.end_date_time_ = poco_timestamp;
	VMP_END;
}

void SessionOperC::SetAddrDatas( const Json::Value& addr_datas )
{
	if ( addr_datas.isNull() || !addr_datas.isArray() )
		return;
	FuncObj_NormalC::AddrDatasT datas;
	for ( const auto& json_addr : addr_datas )
	{
		if ( json_addr.isNull() || !json_addr.isInt() )
			return;
		datas.push_back(json_addr.asUInt());
	}

	func_obj_normal_.SetAddrDatas(datas);
}

void SessionOperC::SetGameSerIdx( int ser_idx )
{
	func_obj_normal_.game_ser_idx_ = ser_idx;
}

void SessionOperC::SetGameAcType( int game_ac_type )
{
	func_obj_normal_.game_ac_type_ = (enGameAcType)game_ac_type;
}

void SessionOperC::StableString( const std::string& stable_str )
{
	VMP_BEGIN_EX;
	if ( stable_str != GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToGame)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "固定的字符串不匹配，隔断时间关闭套接字";
		assert(false);
		SetIoTimer(5 * 1000, []( IoTimer* ){
			const char* const kThrowStr = 
#ifdef _DEBUG
				"固定的字符串不匹配，抛出异常"
#else
				VMP_PROTECT_STR("239")
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "固定字符串匹配成功";
	}
	VMP_END;
}

void SessionOperC::SetMultiHit( int multi_hit_rate )
{
	assert(multi_hit_rate >= 1);
	func_obj_normal_.multi_hit_rate_ = multi_hit_rate - 1;
}

bool SessionOperC::CreateRandNameRole()
{
	if (scmd::GetMe().AutoCreateRole())
		return true;
	return false;
}

bool SessionOperC::SelectRole(pt_csz role_name)
{
	return func_obj_normal_.SelectRoleByName(role_name);
}

void SessionOperC::SetRightBtHit( bool is_right_hit )
{
	GPlayerMe::GetMe().SetRightBtHit(is_right_hit);
}

void SessionOperC::CheckConsoleUuid( const std::string& str_uuid, DWORD console_process_id )
{
	func_obj_normal_.console_uuid_ = str_uuid;
	func_obj_normal_.console_process_id_ = console_process_id;

	return;

	SetIoTimer(1000 * 2, [this, console_process_id, str_uuid]( IoTimer* ){
		if ( console_process_id == 0 )
		{
			func_obj_normal_.DoSomeError();
		}
		if (func_obj_normal_.console_uuid_.empty())
		{
			func_obj_normal_.DoSomeError();
		}

		const char* the_lua_func_name = 
#ifdef __PROJ_GWLR_MB_
		"g_str_MakeMatchSubStrmb";
#elif (__PROJ_GWLR_XE_)
		"g_str_MakeMatchSubStrxe";
#elif (__PROJ_GWLR_MBP_)
		"g_str_MakeMatchSubStrpmb";
#endif

#ifndef _DEBUG
		const auto& the_lua_uuid = ObjectCast(func_obj_normal_.GetGLua().GetGlobalLuaObj(the_lua_func_name), kEmptyStr);
		if ( the_lua_uuid != func_obj_normal_.console_uuid_ )
		{
			assert(false);
			func_obj_normal_.DoSomeError();
		}
#endif
		/*GetIoService().post([=](){
			CToolhelp tool_hlp(TH32CS_SNAPMODULE, console_process_id);
			MODULEENTRY32 module_info = { 0 };
			module_info.dwSize = sizeof(module_info);
			tool_hlp.ModuleFind(_T("Console.exe"), &module_info);
			if ( GenUuidStrOfFile(std::string(CT2CA(module_info.szExePath))) != str_uuid )
			{
				assert(false);
				func_obj_normal_.DoSomeError();
			}
		});*/
	});
}

void SessionOperC::SetStrDatas( const Json::Value& str_datas )
{
	if ( str_datas.isNull() || !str_datas.isArray() )
		return;
	FuncObj_NormalC::StrDatasT datas;
	for ( const auto& json_str : str_datas )
	{
		if ( json_str.isNull() || !json_str.isString() )
			return;
		datas.push_back(json_str.asString());
	}

	func_obj_normal_.SetStrDatas(datas);
}

void SessionOperC::SetOpenVillagePath( bool open_village_path )
{
	GPlayerMe::GetMe().is_open_village_path_ = open_village_path;
}

void SessionOperC::GetMySellList()
{
	Json::Value j_value;
	GPlayerMe::GetMe().GenMySellListToJson(j_value);
	if (j_value.empty())
		return;
	ToCall(BC_MakeP("SetMyOfferList") << j_value);
}

void SessionOperC::GetOtherSellInfo()
{
	Json::Value j_value;
	GPlayerMe::GetMe().GenOthersSellToJson(j_value);
	if (j_value.empty())
		return;
	ToCall(BC_MakeP("SetOtherOfferItems") << j_value);
}

BC_REG_CLS_DEFINE(SessionOperC){
	bufcall::class_<SessionOperC>("SessionOperC")
	.def("SetLoginSucceed", &SessionOperC::SetLoginSucceed)
	.def("4AA7E8C2E7CFE703", &SessionOperC::SetEndDateTime)
	.def("sadsex", &SessionOperC::SetAddrDatas)
	.def("ssdtaseqdc", &SessionOperC::SetStrDatas)
	.def("SetGameSerIdx", &SessionOperC::SetGameSerIdx)
	.def("SetGameAcType", &SessionOperC::SetGameAcType)
	.def("SetMatchTime", &SessionOperC::StableString)
	.def("SetMultiHit", &SessionOperC::SetMultiHit)
	.def("CreateRandNameRole", &SessionOperC::CreateRandNameRole)
	.def("SelectRole", &SessionOperC::SelectRole)
	.def("SetRightBtHit", &SessionOperC::SetRightBtHit)
	.def("SetOpenVillagePath", &SessionOperC::SetOpenVillagePath)
	.def("SetYouHua", &SessionOperC::SetYouHua)
	.def("ForegroundTheWindow", &SessionOperC::ForegroundTheWindow)
	.def("SetHitM_Info", &SessionOperC::CheckConsoleUuid)
	.def("GetMySellList", &SessionOperC::GetMySellList)
	.def("GetOtherSellInfo", &SessionOperC::GetOtherSellInfo)
	.def("GetRunInfo", &SessionOperC::GetRunInfo);
}

void SessionOperC::SetYouHua(bool is_you_hua)
{
	func_obj_normal_.is_you_hua_ = is_you_hua;
}

void SessionOperC::GetRunInfo(int cnt)
{
	ToCall(BC_MakeP("SetRunInfo") << cnt << LuasScriptEchoC::IsRunning() << GRecvMsgMgr::GetMe().GetLastRecvTime());
}

FuncObj_NormalC::InjectGWndComposite::InjectGWndComposite( FuncObj* parent, HWND hwnd ) : FuncObj_Inject(&injected_gwnd_, parent), injected_gwnd_(hwnd, this)
{
	SetState(true);
	injected_gwnd_.SetState(true);
}

LRESULT FuncObj_NormalC::InjectedGWnd::HandleWndMsg_( UINT message, WPARAM wParam, LPARAM lParam )
{
	assert(false);
	return 0;
}

void FuncObj_NormalC::InjectedGWnd::HandleWndMsg( UINT message, WPARAM wParam, LPARAM lParam )
{
	HandleWndMsg_(message, wParam, lParam);
}

void FuncObj_NormalC::InjectedGWnd::ButtonClick( int x, int y )
{
	LONG pos_xy = MAKELPARAM(x,y);
	HandleWndMsg(WM_LBUTTONDOWN, 1, pos_xy);
	::Sleep(100);
	HandleWndMsg(WM_LBUTTONUP, 0, pos_xy);

	HandleWndMsg(WM_LBUTTONDOWN, 1, pos_xy);
	::Sleep(100);
	HandleWndMsg(WM_LBUTTONUP, 0, pos_xy);
}

void FuncObj_NormalC::InjectedGWnd::KeyClick( DWORD virtual_key )
{
	LPARAM lParam = 1;
	lParam += MapVirtualKey(virtual_key, MAPVK_VK_TO_VSC) << 16;
	HandleWndMsg(WM_KEYDOWN, virtual_key, lParam);
	lParam += 1 << 30;
	lParam += 1 << 31;
	::Sleep(100);
	HandleWndMsg(WM_KEYUP, virtual_key, lParam);
}

FuncObj_NormalC::InjectedGWnd& FuncObj_NormalC::InjectGWndComposite::GetInterGWnd()
{
	return injected_gwnd_;
}

void FuncObj_NormalC::InjectGWndComposite::DeleteSelf()
{
	injected_gwnd_.SyncExec(boost::bind(&FuncObj_GWnd::Close, &injected_gwnd_));
	Close();
	GetIoService().post([this](){
		delete this;
	});
}

FuncObj_NormalC::InjectedGWnd::InjectedGWnd( HWND hwnd, FuncObj_Inter* sink ) : FuncObj_GWnd(hwnd, sink)
{

}

BufCallCoorTransImpl::BufCallCoorTransImpl( FuncObj_NormalC& func_obj_normal )
	: BufCallCoorTrans(&func_obj_normal),
	nor_(func_obj_normal)
{

}

void BufCallCoorTransImpl::GainOtherSidePrivateItems()
{
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnGainOtherSidePrivateItems"));
}

void BufCallCoorTransImpl::OnGainOtherSidePrivateItems()
{
	Json::Value j_value;
	GPlayerMe::GetMe().GenPrivateToJson(j_value);
	if ( j_value.isNull() || j_value.empty() )
		return;
	SetCurRoute(GetSourceRoute());
	ToCall(BC_MakeP("OnAddPrivateItems")<<j_value);
}

void BufCallCoorTransImpl::OnAddPrivateItems( const Json::Value& j_value )
{
	GPlayerMe::GetMe().AddOtherSideItemsFromJson(j_value);
}

void BufCallCoorTransImpl::SendAddPrivateItems( const Json::Value& j_value )
{
	if ( j_value.isNull() || j_value.empty() )
		return;
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnAddPrivateItems")<<j_value);
}

void BufCallCoorTransImpl::OnErasePrivateItems( const Json::Value& j_value )
{
	GPlayerMe::GetMe().EraseOtherSideItemsFromJson(j_value);
}

void BufCallCoorTransImpl::SendErasePrivateItems_OtherSide( const Json::Value& j_value )
{
	if ( j_value.isNull() || j_value.empty() )
		return;
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnErasePrivateItems")<<j_value);
}

void BufCallCoorTransImpl::SendErasePrivateItems_AllSide( const Json::Value& j_value )
{
	if ( j_value.isNull() || j_value.empty() )
		return;
	SetCurRoute(GetTransRouteAny());
	ToCall(BC_MakeP("OnErasePrivateItems")<<j_value);
}

BC_REG_CLS_DEFINE(BufCallCoorTransImpl){
	bufcall::class_<BufCallCoorTransImpl>("BufCallCoorTransImpl")
	.def("OnGainOtherSidePrivateItems", &BufCallCoorTransImpl::OnGainOtherSidePrivateItems)
	.def("OnAddPrivateItems", &BufCallCoorTransImpl::OnAddPrivateItems)
	.def("OnErasePrivateItems", &BufCallCoorTransImpl::OnErasePrivateItems);
}
