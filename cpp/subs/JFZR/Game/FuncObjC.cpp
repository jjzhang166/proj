#include "stdafx.h"
#include "FuncObjC.h"
#include "facility/GOffset.h"
#include "Facility/GAddr.h"
#include "Facility/GForeach.h"
#include "Common/MemOper.h"
#include "Facility/GFuncCall.h"
#include <boost/bind.hpp>
#include "Facility/GClass.h"
#include "Common/Toolhelp.h"
#include "CfgStructC.h"
#include "Facility/GPlayer.h"
#include "Socket/PocoMore.h"
#include <protect/Protect.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Communicate/CG_Defines.h>
#include <Communicate/CGL_Defines.h>
#include "Facility/GPlayer.h"
#include "Facility/GameObjMgr.h"
#include <random>
#include <Common/CmnException.h>
#include "EventsImpl.h"
#include "EventsName.h"
#include "Facility/GCloneMap.h"
#include "Facility/GMap.h"

FuncObj_NormalC* g_con_account = nullptr;

FuncObj_NormalC::FuncObj_NormalC() : Connection_Aes(kSelfIpStr, P_Port_Game), glua_(this), json_echo_(glua_, *this), session_oper_(*this), lua_script_(glua_, *this),
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
	selected_role_ = false;
}

TimeDuration& GetGlobalTimeDuration()
{
	static TimeDuration td(500);
	return td;
}

bool FuncObj_NormalC::OnModify( P_MsgNT msg_num, void* param, size_t param_size )
{
	switch (msg_num)
	{
	case P_Modify_Account::value:
		{
			if ( sizeof(P_Modify_Account) != param_size )
				break;
			return OnModifyAccount(*(P_Modify_Account*)param);
		}
	default:
		break;
	}
	return __super::OnModify(msg_num, param, param_size);
}

bool FuncObj_NormalC::OnModifyAccount( P_Modify_Account& modify_account )
{
	account_ = modify_account.account_;
	password_ = modify_account.password_;
	auto_run_script_ = modify_account.auto_run_script_;
	LOG_O_EMPTY_MSG_FAST(Log_trace);

	return true;
}

bool FuncObj_NormalC::InitGame()
{
	LOG_O_EMPTY_MSG_FAST(Log_trace);
	if ( !glua_.InitScript(_T("game.dll"), _T("main.lua")) )
		return false;

	di_press_key_ptr_.reset(new DI_PressKeyPtr::element_type);

	//初始化RSA密钥
	if ( !GenRsaKey(ModuleHelper::GetMe().GetSelfModuleHandle(), public_key_, private_key_, VMP_PROTECT_STR(kRSA_PriPassphrase_CtoS)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "rsa key init failed";
		return false;
	}
	assert(public_key_);
	public_key_->SetEncoding(ICryptoStr::ENC_BASE64);

	if (timer_check_respond_)
	{
		assert(false);
		return false;
	}
	timer_check_respond_ = CreateIoTimer(10 * 60 * 1000, []( IoTimerPtr& ){
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "检测到窗口无响应";
		assert(false);
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->Disconnect();
	}, true);
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

bool FuncObj_NormalC::OnExec( P_MsgNT msg_num, void* param, size_t param_size )
{
	switch (msg_num)
	{
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
	return __super::OnExec(msg_num, param, param_size);
}

void FuncObj_NormalC::CloseAllUi()
{

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
	test_dll_ = LoadLibraryA(load_lib.str_dll_full_file_path_);
	if (test_dll_)
	{
		typedef void (*FuncT)( DWORD );
		auto fn = (FuncT)GetProcAddress(test_dll_, "SetSendObjAddr");
		if (fn)
		{
			extern void* GAddr::g_pSendObjAddr;
			fn((DWORD)GAddr::g_pSendObjAddr);
		}
	}
	if ( !test_dll_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "dll:" << load_lib.str_dll_full_file_path_ << "加载失败";
	}
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
		LOG_O(Log_debug) << "已经初始化过FuncObj_InterInjectComposite了";
		return false;
	}
	inject_ = new InjectGWndComposite(this, game_main_wnd);
	LOG_O(Log_debug) << "初始化FuncObj_InterInjectComposite，HWND:" << (DWORD)game_main_wnd;
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
	return Exec(P_Exec_CallLuaFuncResult::value, &msg_res, sizeof(msg_res));
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

DI_PressKeyPtr FuncObj_NormalC::GetDI_PressKeyPtr()
{
	return di_press_key_ptr_;
}

void FuncObj_NormalC::PressDxinputKey( BYTE di_key )
{
	LOG_ENTER_FUNC;
	if (di_press_key_ptr_)
	{
		stDxinput_PressKey dxinput_key = { di_key, true };
		WriteLock write_lock(di_press_key_ptr_->second);
		di_press_key_ptr_->first.push(dxinput_key);
	}
	else
	{
		LOG_O(Log_debug) << "di_press_key_ptr_还未创建";
	}
}

SessionOperC& FuncObj_NormalC::GetSessionOper()
{
	return session_oper_;
}

void FuncObj_NormalC::BanAccount()
{
	session_oper_.ThisMemCall(nullptr, "BanAccount", nullptr);
}

bool FuncObj_NormalC::PreConnect()
{
	if ( !__super::PreConnect() )
		return false;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "等待验证信息";

	VMP_BEGIN_EX;
	//等待接收成功信息
	WaitUntil([this](){
		return login_succeed_;
	}, 5 * 60 * 1000);

	if ( !login_succeed_ )
	{
		LOG_O(Log_debug) << "login_succeed竟然还未初始化";
		return false;
	}
	if ( !*login_succeed_ )
	{
		LOG_O(Log_debug) << "login_succeed验证未通过";
		return false;
	}

	//等待接收结束日期
	WaitUntil([this](){
		return end_date_time_;
	}, 30 * 1000);

	if ( !end_date_time_ )
	{
		LOG_O(Log_debug) << "end_date_time_竟然还未初始化";
		return false;
	}
	Poco::DateTime time_now;
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "time now->" << Poco::DateTimeFormatter::format(time_now, Poco::DateTimeFormat::SORTABLE_FORMAT)
		<< "\tend date time->" << Poco::DateTimeFormatter::format(*end_date_time_, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	if ( time_now >= *end_date_time_ )
	{
		LOG_O(Log_debug) << "time_now大于到期时间";
		return false;
	}
	VMP_END;

	LOG_O(Log_trace) << "验证信息接收成功";
	return true;
}

void FuncObj_NormalC::SetAddrDatas( const AddrDatasT& addr_datas )
{
	using namespace GAddr;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置关键数据, data[0]->" << addr_datas[0];
	assert(1 == addr_datas.size());
	g_pSendCallSocket = addr_datas[0];

}

JECoorTransImpl& FuncObj_NormalC::GetCoorTrans()
{
	return coor_trans_;
}

void FuncObj_NormalC::SetGameState( GType::enGameState game_state )
{
	using namespace GType;
	if ( game_state < kGS_Invalid || game_state > kGS_InGame )
	{
		assert(false);
		return;
	}
	static bool auto_run_script_at_first = false;
	if ( !auto_run_script_at_first )
	{
		if ( kGS_SelectChannel == game_state )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "进入到选择频道场景";
			auto_run_script_at_first = true;
			if (auto_run_script_)
			{
				GetSessionOper().ThisMemCall(nullptr,
					"StartRunScript", nullptr);
			}
			if (timer_check_login_valid_)
			{
				timer_check_login_valid_->Cancel();
				timer_check_login_valid_.reset();
			}
		}
	}

	static bool auto_login_next = false;
	if ( !auto_login_next )
	{
		if ( kGS_SelectRole == game_state )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "开始登陆下一个帐号";
			auto_login_next = true;
			VERIFY(GetSessionOper().ThisMemCall(nullptr, "GameLoginSucceed", nullptr));
		}
	}

	game_state_ = game_state;
	switch (game_state)
	{
	case GType::kGS_SelectChannel:
		{
			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if (events_mgr)
				events_mgr->ActiveEvent(kPEN_SelectChannel);
		}
		break;
	case GType::kGS_SelectRole:
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "进入到选择角色场景";
			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if (events_mgr)
				events_mgr->ActiveEvent(kPEN_SelectRole);
		}
		break;
	case GType::kGS_InGame:
		{
			if (timer_check_respond_)
			{
				timer_check_respond_->Cancel();
				timer_check_respond_.reset();
			}
			GPlayerSelf::GetMe().Update();
			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if (events_mgr)
			{
				auto& cmn_event = events_mgr->FindByName(kPEN_RoleInGame);
				if (cmn_event)
					cmn_event->ExecAsyc();
				/*auto& timer_event = events_mgr->FindByName(kPPEN_KeepScriptRunningTimer);
				if (timer_event)
					timer_event->Discard();*/
			}
		}
		break;
	default:
		break;
	}
}

GType::enGameState FuncObj_NormalC::GetGameState() const
{
	return game_state_;
}

bool FuncObj_NormalC::ChooseChannel()
{
	using namespace GClass;
	using namespace GType;
	if (channel_id_.empty())
	{
		assert(false);
		return false;
	}
	if ( kGS_SelectChannel != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "当前游戏状态是->" << game_state_ << "\t无法选择频道";
		assert(false);
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "选择频道->" << channel_id_;
	CMDS_Msg_RequestSwitchChannel msg;
	msg.channel_name_ = channel_id_;
	auto& tcp_send = TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	GetLuaScript().OutputScriptInfo("选择了频道");

	GetIoService().post([](){
		auto ambient = &GAmbient::GetMe();
		static auto fn_cond = [ambient](){
			return ambient->GetChooseRoleCond().CondtionTrue();
		};
		ambient->GetChooseRoleCond().ResetCondition();
		WaitUntil(fn_cond, 60 * 1000 * 10);
		if ( !ambient->GetChooseRoleCond().CondtionTrue() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "选择频道超时";
			//assert(false);
			return;
		}
	});
	return true;
}

void FuncObj_NormalC::SetChannelId( const std::wstring& channel_id )
{
	assert(!channel_id.empty());
	channel_id_ = channel_id;
}

boost::optional<bool> FuncObj_NormalC::CreateRole( const std::wstring& role_name, GType::enJobCategory job )
{
	using namespace GClass;
	boost::optional<bool> res;
	if (role_name.empty())
	{
		assert(false);
		return res;
	}
	CMDS_Msg_CreatePlayer msg;
	msg.role_name_ = role_name;
	msg.job_category_ = job;
	on_create_player_result_.reset();
	auto& tcp_send = TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return res;
	}
	WaitUntil([]() -> bool{
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
			return true;
		return nor->on_create_player_result_;
	}, 1000 * 60 * 10);
	if ( !on_create_player_result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "创建角色结果超时";
		assert(false);
		return res;
	}
	if ( CMDR_OnCreatePlayer::kSucceed == on_create_player_result_ )
	{
		/*auto& aps = GAllPlayersStaticInfo::GetMe();
		WaitUntil([&aps]() ->bool {
			return !aps.RolesInfoEmpty();
		}, 1000 * 60 * 10);
		if (aps.RolesInfoEmpty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "创建角色之没有角色信息到来，超时失败";
			assert(false);
			return false;
		}*/

		/*if ( !GAmbient::GetMe().UntilChooseRole(1000 * 10) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "直到选择角色界面超时";
			assert(false);
			return false;
		}*/

		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return res;
		}
		if ( !nor->WaitUntilGameState(GType::kGS_SelectRole, 1000 * 60 * 8 ) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "创建角色之等待选择角色场景超时失败";
			assert(false);
			return res;
		}
		res = true;
		return res;
	}
	else if ( CMDR_OnCreatePlayer::kNameIsUsed == on_create_player_result_ || CMDR_OnCreatePlayer::kWordProhibit == on_create_player_result_ )
	{
		res = false;
		return res;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "创建角色返回未预期结果值：" << on_create_player_result_;
		assert(false);
	}
	return res;
}

bool FuncObj_NormalC::CreateRandNameRole( GType::enJobCategory job )
{
	using namespace GType;
	using namespace GClass;
	if ( kGS_SelectRole != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法创建角色，当前状态是->" << game_state_ << "\t不是选择角色状态";
		assert(false);
		return false;
	}
	const auto& str_role_name = Random_StrW(6, 8);
	/*std::wstring str_role_name;
	static int create_cnt = 0;
	if ( create_cnt++ <= 0 )
		str_role_name = L"12345";
	else
		str_role_name = Random_StrW(6, 8);*/
	auto res = CreateRole(str_role_name, job);
	if ( !res || !*res )
	{
		session_oper_.ThisMemCall(nullptr, "OnCreateRoleFailed", nullptr);
	}
	return res && *res;
	/*while (true)
	{
		const auto& str_role_name = Random_StrW(6, 8);
		if (CreateRole(str_role_name, job))
			return true;
		if ( !on_create_player_result_ )
		{
			assert(false);
			return false;
		}
		if ( CMDR_OnCreatePlayer::kNameIsUsed == on_create_player_result_ )
			continue;
		else
		{
			assert(false);
			return false;
		}
	}
	assert(false);
	return true;*/
}

bool FuncObj_NormalC::SelectRole( const std::wstring& role_name )
{
	using namespace GType;
	using namespace GClass;
	if ( kGS_SelectRole != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法选择角色，当前状态是->" << game_state_ << "\t不是选择角色状态";
		assert(false);
		return false;
	}
	if (role_name.empty())
	{
		assert(false);
		return false;
	}
	if ( !GAllPlayersStaticInfo::GetMe().FindByRoleName(role_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "所有角色信息里未找到角色名->" << role_name;
		assert(false);
		return false;
	}

	CMDS_Msg_SelectPlayer msg;
	msg.role_name_ = role_name;
	auto& tcp_send = TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	GetIoService().post([role_name](){
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "等待进入到游戏";
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "选择角色之开始等待触发kPEN_RoleInGame";
		if ( !event_mgr->WaitTouchEvent(kPEN_RoleInGame, 1000 * 60 * 10) )
		{
			LOG_O(Log_trace) << "选择角色之等待触发kPEN_RoleInGame，超时失败";
			return;
		}
		LOG_O(Log_trace) << "选择角色之等待触发kPEN_RoleInGame，完成，已进入游戏->" << role_name;
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
		}
		nor->lua_script_.OutputScriptInfo("已进入游戏");
		::Sleep(2000);
		auto& gm = GMap::GetMe();
		if ( !gm.Update() )
		{
			assert(false);
			return;
		}
		if ( !gm.IsTown() )
		{
			auto& gr = GCloneMap::GetMe();
			if ( !gr.Update() )
			{
				assert(false);
				return;
			}
		}
		if ( !GameObjMgr::GetMe().RebuildUpdateAll() )
		{
			assert(false);
			return;
		}
		if ( !GPlayerSelf::GetMe().Update() )
		{
			assert(false);
			return;
		}
	});	
	
	/*if ( !GAmbient::GetMe().UntilCityUiNow(1000 * 60 * 10) )
	{
		LOG_O(Log_error) << "选择角色超时失败";
		assert(false);
		return false;
	}*/
	
	return true;
}

void FuncObj_NormalC::OnCreatePlayerResult( CMDR_OnCreatePlayer::enResult result )
{
	on_create_player_result_ = result;
}

bool FuncObj_NormalC::IsSelectedRole() const
{
	return selected_role_;
}

void FuncObj_NormalC::SetSelectedRole( bool selected_role )
{
	selected_role_ = selected_role;
}

bool FuncObj_NormalC::AutoCreateOrSelectRole()
{
	lua_script_.OutputScriptInfo("创建或选择角色");
	GetIoService().post([this](){
		using namespace GType;
		if ( kGS_SelectRole != game_state_ )
		{
			assert(false);
			return false;
		}
		if ( !session_oper_.ThisMemCall(nullptr, "AutoCreateOrSelectRole", nullptr) )
		{
			assert(false);
			return false;
		}
		return true;
	});
	auto event_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !event_mgr )
	{
		assert(false);
		return false;
	}
	return true;
	//return kGS_InGame == game_state_;
}

bool FuncObj_NormalC::RetChooseRoleScene()
{
	using namespace GType;
	using namespace GClass;
	if ( kGS_InGame != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "角色并未进入游戏，无法返回到选择角色界面";
		//assert(false);
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "开始返回选择角色界面";
	selected_role_ = false;
	if ( !UntilShowGameMenuByPressEsp() )
		return false;
	//::Sleep(1000);
	if ( !ButtonClick(464, 330) )
	{
		assert(false);
		return false;
	}
	
	//SetGameState(kGS_Invalid);
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "返回到角色场景之开始等待返回到角色选择界面";
	if ( !GAmbient::GetMe().UntilChooseRole(60 * 1000 * 10) )
	{
		LOG_O(Log_error) << "等待超时，未返回到角色选择界面";
		SetGameState(kGS_InGame);
		assert(false);
		return false;
	}*/
	auto event_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !event_mgr )
	{
		assert(false);
		return false;
	}
	auto cmn_timer = event_mgr->FindByName(kPPEN_WaitRetSelectRole);
	if (cmn_timer)
		return true;
	cmn_timer = event_mgr->CreateTimerEvent(1000 * 10, []( ICmnEvent& cmn_event ) -> bool {
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return true;
		}
		if (nor->IsSelectedRole())
		{
			cmn_event.Discard();
			return true;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "再一次尝试返回到角色选择界面";
		if ( !nor->RetChooseRoleScene() )
			return true;
		return false;
	}, kPPEN_WaitRetSelectRole);
	if ( !cmn_timer )
	{
		assert(false);
		return false;
	}
	cmn_timer->OwnershipLvlUp();
	cmn_timer->SetUnNamed();
	cmn_timer->ExecAsyc();

	/*LOG_O(Log_info) << "等待返回角色界面";
	GetIoService().post([](){
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "返回到选择角色之开始等待触发选择角色事件";
		if ( !event_mgr->WaitTouchEvent(kPEN_SelectRole, 1000 * 60 * 10) )
		{
			LOG_O(Log_trace) << "返回到选择角色之等待触发选择角色事件，超时失败，再次尝试返回到角色界面";
			return;
		}
		LOG_O(Log_trace) << "返回到选择角色之等待触发选择角色事件，完成";
	});*/
	return true;
}

bool FuncObj_NormalC::UntilShowGameMenuByPressEsp()
{
	using namespace GType;
	using namespace GClass;
	if ( kGS_InGame != game_state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "角色并未进入游戏，无法返回到选择角色界面";
		assert(false);
		return false;
	}
	auto& ambient = GAmbient::GetMe();
	bool esc_valid = true;
	if ( !TimerDoWaitUntil([&ambient, this, &esc_valid](){
		if (IsSelectedRole())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "按ESC返回选择角色之已经选择过角色，并且又换到了新的角色的游戏场景中";
			esc_valid = false;
			return true;
		}
		if ( GType::kGS_SelectRole == GetGameState() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "按ESC返回选择角色之已经到了选择角色场景了";
			esc_valid = false;
			return true;
		}
		return ambient.UntilOpenedGameMenu(500);
	}, [&ambient](){
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->KeyClick(VK_ESCAPE);
	}, 1500, 1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开游戏菜单失败";
		return false;
	}
	if ( !esc_valid )
		return false;
	return true;
}

bool FuncObj_NormalC::ButtonClick( int x, int y )
{
	auto injected_gwnd = GetGWnd();
	if ( !injected_gwnd )
	{
		assert(false);
		return false;
	}
	injected_gwnd->AsyncExec([injected_gwnd, x, y](){
		injected_gwnd->ButtonClick(x, y);
	});
	return true;
}

bool FuncObj_NormalC::KeyClick( DWORD virtual_key )
{
	auto injected_gwnd = GetGWnd();
	if ( !injected_gwnd )
	{
		assert(false);
		return false;
	}
	injected_gwnd->AsyncExec([injected_gwnd, virtual_key](){
		injected_gwnd->KeyClick(virtual_key);
	});
	return true;
}

void FuncObj_NormalC::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);
	Clear();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "断开连接，关闭进程";
	GetIoService().post([](){
		throw CmnTerminateProcessException();
	});
}

bool FuncObj_NormalC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	P_Exec_GameProccessInfo msg;
	msg.self_process_id_ = GetCurrentProcessId();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "current process id->" << msg.self_process_id_;
	msg.login_client_proccess_id_ = CToolhelp::GetParentProcessID(msg.self_process_id_);
	LOG_O(Log_trace) << "parent process id->" << msg.login_client_proccess_id_;
	if ( !Exec(P_Exec_GameProccessInfo::value, &msg, sizeof(msg)) )
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
	VMP_END;
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
	GetSessionOper().ThisMemCall(nullptr, "GameLoginSucceed", nullptr);
}

LuasScriptEchoC_Impl& FuncObj_NormalC::GetLuaScript()
{
	return lua_script_;
}

void FuncObj_NormalC::BeginCheckLoginFailed()
{
	assert(!timer_check_login_valid_);
	timer_check_login_valid_ = CreateIoTimer(1000 * 5, []( IoTimerPtr& timer ){
		HWND hWnd = ::FindWindow(nullptr, _T("INFORMATION"));
		if (hWnd)
		{
			DWORD dwProcessId = 0;
			GetWindowThreadProcessId(hWnd, &dwProcessId);

			if( GetCurrentProcessId() == dwProcessId )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "TP检测，login failed";
				//assert(false);
				auto nor = FuncObj_NormalC::GetInstance();
				if (nor)
					nor->Disconnect();
			}
		}
		timer->Continue();
	}, true);
	assert(timer_check_login_valid_);
}

bool FuncObj_NormalC::SelectRoleA( const std::string& role_name )
{
	return SelectRole(GbkToWideChar(role_name));
}

bool GWndExecSync( const std::function<bool()>& func )
{
	auto gwnd = FuncObj_NormalC::GetGWnd();
	if ( !gwnd )
	{
		assert(false);
		return false;
	}
	//return func();
	bool res = false;
	if ( !gwnd->SyncExec(res, func) )
	{
		assert(false);
		return false;
	}
	return res;
}

bool MakeTransRoute( P_CoorPacketHeadT& out_route, enGameAcType game_ac_type )
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
	way_tmp.coor_type_ = game_ac_type;
	out_route.push(way_tmp);
	return true;
}

P_CoorPacketHeadT MakeTransRoute( enGameAcType game_ac_type )
{
	P_CoorPacketHeadT out_route;
	VERIFY(MakeTransRoute(out_route, game_ac_type));
	return out_route;
}

SessionOperC::SessionOperC( FuncObj_NormalC& func_obj_normal ) : JsonEchoBase(func_obj_normal.GetGLua(), func_obj_normal, kObjIdNormal),
	func_obj_normal_(func_obj_normal)
{

}

bool SessionOperC::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

void SessionOperC::SetLoginSucceed( bool succeed )
{
	VMP_BEGIN_EX;
	func_obj_normal_.login_succeed_ = succeed;
	VMP_END;
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "验证登陆结果->" << succeed;
}

void SessionOperC::SetEndDateTime( Poco::UInt32 timestamp_lower, Poco::UInt32 timestamp_upper )
{
	VMP_BEGIN_EX;
	Poco::Timestamp poco_timestamp = Poco::Timestamp::fromEpochTime((time_t)MakeInt64(timestamp_lower, timestamp_upper));
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

void SessionOperC::CheckRsaPubKey( const std::string& encode_str )
{
	assert(false);
	/*VMP_BEGIN_EX;
	assert(func_obj_normal_.public_key_);
	auto en_str = func_obj_normal_.public_key_->Encrypt(VMP_PROTECT_STR(kCheckPubKey_ConsoleToGame));
	if ( en_str != encode_str )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "验证公钥失败";
		assert(false);
		SetIoTimer(60 * 1000, []( IoTimer* ){
			const char* const kThrowStr = 
#ifdef _DEBUG
				"验证公钥失败，抛出异常"
#else
				VMP_PROTECT_STR("3409")
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "验证公钥成功";
	}
	VMP_END;*/
}

Json::Value SessionOperC::GetSkillsInfo()
{
	Json::Value json_array;
	auto gps = GPlayerSelf::GetInstance();
	if ( !gps )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "GPlayerSelf不存在";
		return json_array;
	}
	auto& skill_mgr = gps->GetSkillMgr();
	const auto& skills = skill_mgr.GetSkills();
	if (skills.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "尚没有技能";
		return json_array;
	}
	Json::Value json_tmp;
	for ( const auto& skill_obj : skills )
	{
		if ( !skill_obj.ConvertToJson(json_tmp) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "技能conver to json失败";
			assert(false);
			return Json::Value();
		}
		json_array.append(json_tmp);
	}
	assert(!json_array.empty());
	return json_array;
}

Json::Value SessionOperC::GetGameObjsInfo()
{
	Json::Value json_array;
	auto& gameobj_mgr = GameObjMgr::GetMe();
	const auto& gameobjs = gameobj_mgr.GetAllGameObjs();
	if (gameobjs.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "游戏对象为空";
		return json_array;
	}
	Json::Value json_tmp;
	for ( const auto& gameobj : gameobjs )
	{
		if ( !gameobj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "获取游戏对象的时候，发现有为空的游戏对象";
			assert(false);
			return Json::Value();
		}
		if ( !gameobj->ConverToJson(json_tmp) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "游戏对象convert to game obj failed";
			assert(false);
			return Json::Value();
		}
		json_array.append(json_tmp);
	}
	assert(!json_array.empty());
	return json_array;
}

Json::Value SessionOperC::GetAllItems()
{
	Json::Value json_array;
	auto gps = GPlayerSelf::GetInstance();
	if ( !gps )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "GPlayerSelf不存在";
		return json_array;
	}
	auto& packet = gps->GetPacket();
	GItemMgr* item_mgrs[] = {
		&packet.GetTabNormal(), &packet.GetTabConsume(), &packet.GetTabTask(),
		&packet.GetTabMaterial(), &packet.GetTabAvatar(), &packet.GetTabEquiped(),
		&packet.GetTabAvatarEquiped()
	};
	Json::Value json_tmp;
	for ( auto i = 0; i != _countof(item_mgrs); ++i )
	{
		const auto& items = item_mgrs[i]->GetItems();
		auto tab_category = (Json::UInt)item_mgrs[i]->GetTabCategory();
		for ( const auto& item : items )
		{
			if ( !item.ConverToJson(json_tmp) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "item convert to json failed";
				assert(false);
				return Json::Value();
			}
			json_tmp["tab_cate"] = tab_category;
			json_array.append(json_tmp);
		}
	}
	assert(!json_array.empty());
	return json_array;
}

Json::Value SessionOperC::GetRoomInfo()
{
	Json::Value json_room;
	auto room = GCloneMap::GetInstance();
	if ( !room )
		return json_room;
	if ( !room->ConverToJson(json_room) )
	{
		assert(false);
		return Json::Value();
	}
	return json_room;
}

bool SessionOperC::CreateRandNameRole()
{
	using namespace GType;
	const enJobCategory kJobs[] = {
		kJC_JianDouShi, /*kJC_RenWuZhe, */kJC_KongMoShi
	};
	using namespace std;
	uniform_int_distribution<> gr;
	return func_obj_normal_.CreateRandNameRole(kJobs[gr(GetDltRandomDevice()) % _countof(kJobs)]);
}

bool SessionOperC::SelectRole( const std::string& role_name )
{
	return func_obj_normal_.SelectRole(GbkToWideChar(role_name));
}

void SessionOperC::GetMyOfferList()
{

}

void SessionOperC::GetOtherOfferItems()
{

}

bool JECoorTransImpl::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

FuncObj_NormalC::InjectGWndComposite::InjectGWndComposite( FuncObj* parent, HWND hwnd ) : FuncObj_Inject(&injected_gwnd_, parent), injected_gwnd_(hwnd, this)
{
	SetState(true);
	injected_gwnd_.SetState(true);
}

LRESULT FuncObj_NormalC::InjectedGWnd::HandleWndMsg_( UINT message, WPARAM wParam, LPARAM lParam )
{
	using namespace GClass;
	using namespace GOffset;
	static const GWnd* game_wnd = nullptr;
	if (game_wnd)
		return game_wnd->WndProc(message, wParam, lParam);
	auto hwnd = GetWndHandle();
	assert(hwnd);
	auto unknown_addr = (int)GetWindowLong(hwnd, GWL_USERDATA);
	if ( !unknown_addr )
	{
		assert(false);
		return -1;
	}
	game_wnd = (const GWnd*)(unknown_addr + g_o_game_wnd_obj_addr);
	return game_wnd->WndProc(message, wParam, lParam);
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

JECoorTransImpl::JECoorTransImpl( FuncObj_NormalC& func_obj_normal )
	: JECoorTrans(func_obj_normal.GetGLua(), func_obj_normal),
	nor_(func_obj_normal)
{

}

bool JECoorTransImpl::GainPriProductsInfo()
{
	auto& gps = GPlayerSelf::GetMe();
	const auto& role_uuid = gps.GetMyRoleUuid();
	if (role_uuid.is_nil())
	{
		assert(false);
		return false;
	}
	Json::Value out_json;
	if ( !gps.GenPriProductsInfo(out_json) )
		return false;
	Json::Value json_uuid;
	VERIFY(role_uuid.ToJson(json_uuid));
	SetCurRoute(GetSourceRoute());
	return ThisMemCall(nullptr, "OnProductsInfo", &JsonEcho::MakeParams(json_uuid, out_json));
}

void JECoorTransImpl::OnProductsInfo( const Json::Value& json_uuid, const Json::Value& json_v )
{
	using namespace GType;
	CmnUuid role_uuid;
	if ( !role_uuid.FromJson(json_uuid) )
	{
		assert(false);
		return;
	}
	stProductInfo product_tmp;
	auto& role_products_info = GPlayerSelf::GetMe().GetOthersProductInfo();
	auto it = role_products_info.find(role_uuid);
	if ( it == role_products_info.end() )
	{
		auto& products = role_products_info[role_uuid];
		for ( auto& json_product : json_v )
		{
			if ( !product_tmp.FromJson(json_product) )
			{
				assert(false);
				return;
			}
			products.insert(product_tmp);
		}
	}
	else
	{
		it->second.clear();
		for ( auto& json_product : json_v )
		{
			if ( !product_tmp.FromJson(json_product) )
			{
				assert(false);
				return;
			}
			it->second.insert(product_tmp);
		}
	}
}

bool JECoorTransImpl::GainProductsInfo( int ac_type )
{
	if ( kGAT_Normal < ac_type || kGAT_Invalid > ac_type )
	{
		assert(false);
		return false;
	}
	auto& gps = GPlayerSelf::GetMe();
	const auto& role_uuid = gps.GetMyRoleUuid();
	if (role_uuid.is_nil())
	{
		assert(false);
		return false;
	}
	Json::Value out_json;
	if ( !gps.GenProductsInfoByAcType(out_json, enGameAcType(ac_type)) )
		return false;
	Json::Value json_uuid;
	VERIFY(role_uuid.ToJson(json_uuid));
	SetCurRoute(GetSourceRoute());
	return ThisMemCall(nullptr, "OnProductsInfo", &JsonEcho::MakeParams(json_uuid, out_json));
}