#include "stdafx.h"
#include "FuncObjC.h"
#include "facility/GOffset.h"
#include "Facility/GAddr.h"
#include "Facility/GForeach.h"
#include "Common/MemOper.h"
#include "Facility/GFuncCall.h"
#include <boost/bind.hpp>
#include "Facility/GClass.h"
#include "Facility/GMatch.h"
#include "Common/Toolhelp.h"
#include "CfgStructC.h"
#include "Facility/GPlayer.h"
#include "Net/PocoMore.h"
#include <protect/Protect.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Communicate/CG_Defines.h>
#include <Communicate/CGL_Defines.h>
#include <Common/CmnException.h>
#include "GloFuncs.h"
#include "DllApp.h"

FuncObj_NormalC* g_con_account = nullptr;

FuncObj_NormalC::FuncObj_NormalC() : Connection(kSelfIpStr, P_Port_Game), session_oper_(*this), 
	coordinate_(*this), timer_set_match_end_(1000 * 60 * 5 + 1000 * 15, false)
{
	assert(!g_con_account);
	g_con_account = this;
	test_dll_ = nullptr;
	inject_ = nullptr;
	game_state_ = GType::enGameState_Invalid;
	auto_run_script_ = false;
	game_ser_idx_ = 0;
	game_ac_type_ = kGAT_Normal;
	timer_set_match_end_.SetTimerStart(false);
}

bool FuncObj_NormalC::OnModifyAccount( P_Modify_Account& modify_account )
{
	account_ = modify_account.account_;
	password_ = modify_account.password_;
	game_ser_name_ = modify_account.service_name_;
	auto_run_script_ = modify_account.auto_run_script_;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "Game.dll收到游戏帐号：" << account_ << " 密码：" << password_ << " 服务器：" << game_ser_name_;

	return true;
}

bool FuncObj_NormalC::InitGame()
{
	LOG_O_EMPTY_MSG_FAST(Log_trace);
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

void FuncObj_NormalC::ContinueOnClick( IoTimerPtr& timer )
{
	using namespace GFuncCall;
	if (IsEnterContinueOnClick())
	{
		//发送键盘消息或直接调用游戏call，以使能够跳过点击继续场景
		LeftBtClick(770, 88);

		if (IsEnterContinueOnClick())
		{
			if (timer)
				timer->Continue();
			else
			{
				timer_continue_on_click_ = CreateIoTimer(1000, true, boost::bind(&FuncObj_NormalC::ContinueOnClick,
					this, _1));
			}
		}
		else
		{
			if (timer_continue_on_click_)
			{
				timer_continue_on_click_->Cancel();
				timer_continue_on_click_.reset();
			}
		}
	}
	else
	{
		if (timer_continue_on_click_)
		{
			timer_continue_on_click_->Cancel();
			timer_continue_on_click_.reset();
		}
	}
}

void FuncObj_NormalC::TrainerProcedure()
{
	using namespace GFuncCall;
	if (IsExistTrainer())
	{
		//TrainerSelectScene(nullptr);
	}
	else
	{
		CreateTrainerProcedure(nullptr);
	}
}

void FuncObj_NormalC::TrainerSelectScene( IoTimer* timer, std::size_t trainer_idx )
{
	using namespace GFuncCall;
	if (IsTrainerSelectScene())
	{
		LuaThrdData::GetMe().LuaObjCall<void>("EnterSelectTrainer");

		if (IsTrainerSelectScene())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(500, boost::bind(&FuncObj_NormalC::TrainerSelectScene,
					this, _1, trainer_idx));
			}
		}
	}
}

void FuncObj_NormalC::CreateTrainerProcedure( IoTimer* timer )
{
	LOG_O_EMPTY_MSG_FAST(Log_trace);
}

void FuncObj_NormalC::TrainerSelectedScene( IoTimer* timer )
{
	return;
	using namespace GFuncCall;
	if (IsTrainerSelectedScene())
	{
		//首次教练选定场景，会有每日任务dlg弹出
		CloseAllUi();
		//进一步动作，做赛事还是其他？
		ReadyState_DistributeTask();
		return;
	}

	if (timer)
		timer->Continue();
	else
	{
		SetIoTimer(500, boost::bind(&FuncObj_NormalC::TrainerSelectedScene,
			this, _1));
	}
}

void FuncObj_NormalC::CloseAllUi()
{

}

void FuncObj_NormalC::ReadyState_DistributeTask()
{

}

bool FuncObj_NormalC::OnTestLoadLib( P_Exec_TestLoadLib& load_lib )
{
	if ( !PathFileExistsA(load_lib.str_dll_full_file_path_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "加载的DLL：" << load_lib.str_dll_full_file_path_ << "不存在";
		return true;
	}
	if (test_dll_)
		FreeLibrary(test_dll_);
	test_dll_ = LoadLibraryA(load_lib.str_dll_full_file_path_);
	if ( !test_dll_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "dll:" << load_lib.str_dll_full_file_path_ << "加载失败";
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

FuncObj_GWnd* FuncObj_NormalC::GetInterGWnd()
{
	return inject_ ? &inject_->GetInterGWnd() : nullptr;
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
	if ( !game_main_wnd )
		return false;
	inject_ = new FuncObj_InterInjectComposite(this, game_main_wnd);
	LOG_O(Log_debug) << "初始化FuncObj_InterInjectComposite，HWND:" << (DWORD)game_main_wnd;
	return true;
}

bool FuncObj_NormalC::OnCallLuaFunction( P_Exec_CallLuaFunction& call_lua_func )
{
	LOG_ENTER_FUNC;
	auto func_res = LuaThrdData::GetMe().LuaObjCallByDlt(call_lua_func.call_func_name, std::string("glua_.Call没有执行成功"), call_lua_func.func_param);
	P_Exec_CallLuaFuncResult msg_res;
	strcpy_s(msg_res.func_result, func_res.c_str());
	strcpy_s(msg_res.call_func_name, call_lua_func.call_func_name);
	strcpy_s(msg_res.func_param, call_lua_func.func_param);
	LOG_O(Log_info) << "LUA函数名：" << msg_res.call_func_name << "。参数：" << msg_res.func_param << "。结果：" << msg_res.func_result;
	return UserMsg(P_Exec_CallLuaFuncResult::value, &msg_res, sizeof(msg_res));
}

FuncObj_GWnd* FuncObj_NormalC::GetGWnd()
{
	auto func_obj_normal = GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "FuncObj_NormalC实例还未创建";
		return nullptr;
	}
	auto gwnd = func_obj_normal->GetInterGWnd();
	if ( !gwnd )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "Gwnd还未创建";
	}
	return gwnd;
}

void FuncObj_NormalC::SetGameState( GType::enGameState game_state )
{
	using namespace GType;
	if ( game_state_ == game_state )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "游戏状态相同，设置个毛，状态：" << game_state;
		return;
	}
	//比赛结束之前不准进入教练选定状态
	if ( GType::enGameState_TrainerSelected == game_state && game_state_ > GType::enGameState_TrainerSelected )
		return;
	if ( !CanChangeMatchingState(game_state) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "can not change matching state, src->" << game_state_ << "\tdst->" << game_state;
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置游戏状态，之前：" << game_state_ << "\t之后：" << game_state; 
	game_state_ = game_state;
	if ( GType::enGameState_MatchEnd_Back == game_state )
	{
		auto inject = GetInterInject();
		if (inject)
		{
			inject->AsyncExec([](){
				IGMatchMgr::GetMe().MatchMode()->EnteredRoom();
			});
		}
	}

	static bool auto_run_script_at_first = false;
	if ( !auto_run_script_at_first )
	{
		if ( GType::enGameState_TrainerSelect == game_state_ || GType::enGameState_CreateCoach == game_state_ )
		{
			auto_run_script_at_first = true;
			if (auto_run_script_)
			{
				GetSessionOper().ToCall(BC_MakeP("StartRunScript"));
			}
		}
	}

	//比赛房间中超时
	const std::size_t kTimeAtMatchRoom = 1000 * 60 * 3;
	if ( enGameState_Managers == game_state_ || enGameState_Managers_Contract == game_state_ || enGameState_Managers_Computer == game_state_ ||
		enGameState_Managers_Friend == game_state_ || enGameState_Managers_Random == game_state_ )
	{
		auto timer = GPlayer::GetMe().GetTimer_AtMatchRoom(kTimeAtMatchRoom);
		if (timer)
			if ( !timer->IsClocking() )
			{
				timer->Continue();
			}
	}
	else
	{
		auto& gplayer = GPlayer::GetMe();
		auto timer = gplayer.GetTimer_AtMatchRoom(kTimeAtMatchRoom);
		if (timer)
		{
			timer->Cancel();
		}
		gplayer.ResetTimeoutAtMatchRoom();
	}

	if ( enGameState_NoticeDlg == game_state_ )
	{
		GetSessionOper().ToCall(BC_MakeP("OnGameState")<<(int)game_state_);
	}
}

GType::enGameState FuncObj_NormalC::GetGameState()
{
	return game_state_;
}

void FuncObj_NormalC::LuaContinueOnClick()
{
	ContinueOnClick(IoTimerPtr());
}

bool FuncObj_NormalC::IsEnterContinueOnClick()
{
	return GType::enGameState_PressOnClick == game_state_;
}

void FuncObj_NormalC::CloseNoticeDialog( IoTimerPtr& timer )
{
	if (IsNoticeDlgOfMosttop())
	{
		if (timer)
		{
			timer->Continue();
		}
		else
		{
			timer_close_notice_dlg_ = CreateIoTimer(1500, true, boost::bind(&FuncObj_NormalC::CloseNoticeDialog,
				this, _1));
		}

		if (IsNoticeDlgOfMosttop())
		{
			LuaThrdData::GetMe().LuaObjCall<void>("CloseNoticeDlg");
		}
		else
		{
			if (timer_close_notice_dlg_)
			{
				timer_close_notice_dlg_->Cancel();
				timer_close_notice_dlg_.reset();
			}
		}
	}
	else
	{
		if (timer_close_notice_dlg_)
		{
			timer_close_notice_dlg_->Cancel();
			timer_close_notice_dlg_.reset();
		}
	}
}

void FuncObj_NormalC::LuaCloseNoticeDialog()
{
	CloseNoticeDialog(IoTimerPtr());
}

bool FuncObj_NormalC::IsNoticeDlgOfMosttop()
{
	return GType::enGameState_NoticeDlg == game_state_;
}

void FuncObj_NormalC::LuaTrainerSelectScene( std::size_t trainer_idx )
{
	TrainerSelectScene(nullptr, trainer_idx);
}

bool FuncObj_NormalC::IsTrainerSelectScene()
{
	return GType::enGameState_TrainerSelect == game_state_;
}

void FuncObj_NormalC::LuaTrainerSelectedScene()
{
	TrainerSelectedScene(nullptr);
}

void FuncObj_NormalC::CloseTaskDlg( IoTimer* timer )
{
	if (IsTaskDlgShown())
	{
		LuaThrdData::GetMe().LuaObjCall<void>("HandleCloseTaskDlg");

		if (IsTaskDlgShown())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1500, boost::bind(&FuncObj_NormalC::CloseTaskDlg,
					this, _1));
			}
		}
	}
}

bool FuncObj_NormalC::IsTaskDlgShown()
{
	return IsTrainerSelectedScene();
}

bool FuncObj_NormalC::IsTrainerSelectedScene()
{
	return GType::enGameState_TrainerSelected == game_state_;
}

void FuncObj_NormalC::LuaCloseTaskDlg()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
	{
		if ( GType::enGameState_TrainerSelected == func_obj_normal->GetGameState() )
			CloseTaskDlg(nullptr);
	}
}

bool GWndExecSync( const std::function<bool()>& func )
{
	//return func();
	auto gwnd = FuncObj_NormalC::GetGWnd();
	if ( !gwnd )
	{
		assert(false);
		return false;
	}	
	bool res = false;
	if ( !gwnd->SyncExec(res, func) )
	{
		assert(false);
		return false;
	}
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

void FuncObj_NormalC::CloseLevelUpDlg( int milliseconds, int times )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "发送升级包了，表示升级了，但不知道会有没有升级对话框弹出";
	SetIoTimer((std::size_t)milliseconds, times, [](std::size_t){
		PressEsc();
	});
}

void FuncObj_NormalC::ForegroundTheWindow() const
{
	auto gwnd = GetGWnd();
	if (gwnd)
	{
		gwnd->ForegroundWindow();
	}
}

const std::string& FuncObj_NormalC::GetAccount() const
{
	return account_;
}

DI_PressKeyPtr& FuncObj_NormalC::GetDI_PressKeyPtr()
{
	return di_press_key_ptr_;
}

void FuncObj_NormalC::PressDxinputKey( BYTE di_key )
{
	if (di_press_key_ptr_)
	{
		stDxinput_PressKey dxinput_key = { di_key, true };
		{
			WriteLock write_lock(di_press_key_ptr_->second);
			di_press_key_ptr_->first.push(dxinput_key);
		}		
		if (!WaitUntil(1000 * 2, [this](){
			if ( !di_press_key_ptr_ )
				return true;
			if (di_press_key_ptr_->first.empty())
				return true;
			return false;
		}) )
		{
			//assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "timeout to press dx key";
			return;
		}
		if ( !di_press_key_ptr_ )
		{
			assert(false);
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "DX按下成功，key:" << (int)di_key;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "di_press_key_ptr_还未创建";
	}
}

bool FuncObj_NormalC::NeedReplaceSendContent()
{
	return false;
	if ( GType::enGameState_CreateCoach == game_state_ )
	{
		auto& gplayer = GPlayer::GetMe();
		if ( GType::enCC_SubState_InputName == gplayer.GetCC_SubState() )
			return true;
	}
	return false;
}

bool FuncObj_NormalC::ReplaceSendContent( GType::stHttpSendCall_Content& send_content )
{
	HTTPRequest_RFB request;
	if ( !request.ReadFromBuffer(send_content.str_http_content) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "ReadFromBuffer错误";
		return false;
	}
	const auto& uri = request.getURI();

	//创建教练之输入教练名字
	const char* const kInputCoachName = "/coach/new/reserve";
	if ( uri == kInputCoachName )
	{
		Json::Value msg;
		msg["name"] = GPlayer::GetMe().GenCoachName();
		GClass::HttpPackageSend::ReplaceMsg_Json(uri.c_str(), msg, send_content);
		return true;
	}
	return false;
}

void FuncObj_NormalC::InputCoachName( char c )
{
	auto gwnd = GetGWnd();
	if ( !gwnd )
		return;
	auto hWnd = gwnd->GetWndHandle();
	/*LPARAM lParam = 1;
	const UINT VK_1 = 0x31;
	lParam += MapVirtualKey(VK_1, MAPVK_VK_TO_VSC) << 16;
	::PostMessage(hWnd, WM_KEYDOWN, VK_1, lParam);
	lParam += 1 << 30;
	lParam += 1 << 31;
	::PostMessage(hWnd, WM_KEYUP, VK_1, lParam);*/
	::PostMessage(hWnd, WM_CHAR, c, MAKELPARAM(c, WM_KEYDOWN));
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
	using namespace GAddr;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置关键数据";
	
	//SSL_read
	/*MO_SslRead addr_ssl_read(addr_datas[0]);
	g_pSSL_read.swap(addr_ssl_read);*/
}

GCoordinateC& FuncObj_NormalC::GetCoordinate()
{
	return coordinate_;
}

bool FuncObj_NormalC::CanNormalTradeProcedure()
{
	using namespace GType;
	if ( game_state_ < enGameState_Managers )
		return false;
	Poco::DateTime time_now;
	auto time_span = time_now - time_normal_trade_;
	//1分钟交易一次
	if ( time_span.totalMinutes() >= 1 )
	{
		time_normal_trade_ = time_now;
		return true;
	}
	return false;
}

bool FuncObj_NormalC::CanStoredTradeProcedure()
{
	using namespace GType;
	if ( game_state_ < enGameState_Managers )
		return false;
	Poco::DateTime time_now;
	auto time_span = time_now - time_normal_trade_;
	//30秒交易一次
	if ( time_span.totalSeconds() >= 30 )
	{
		time_normal_trade_ = time_now;
		return true;
	}
	return false;
}

LuasScriptEchoC& FuncObj_NormalC::GetLuaScript()
{
	if (lua_script_)
		return *lua_script_;
	lua_script_.reset(new LuasScriptEchoC(*this));
	return *lua_script_;
}

bool FuncObj_NormalC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	P_Exec_GameProccessInfo msg;
	msg.self_process_id_ = GetCurrentProcessId();
	msg.login_client_proccess_id_ = msg.self_process_id_;
	if ( !UserMsg(P_Exec_GameProccessInfo::value, &msg, sizeof(msg)) )
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
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "等待验证信息";

	//等待接收成功信息
	WaitUntil(5 * 60 * 1000, [this](){
		return !!login_succeed_;
	});

	if ( !login_succeed_ )
	{
		LOG_O(Log_error) << "login_succeed竟然还未初始化";
		return false;
	}
	if ( !*login_succeed_ )
	{
		LOG_O(Log_error) << "login_succeed验证未通过";
		return false;
	}

	//等待接收结束日期
	WaitUntil(30 * 1000, [this](){
		return !!end_date_time_;
	});

	if ( !end_date_time_ )
	{
		LOG_O(Log_debug) << "end_date_time_竟然还未初始化";
		return false;
	}
	DateTimeLocal time_now;
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "time now->" << Poco::DateTimeFormatter::format(time_now, Poco::DateTimeFormat::SORTABLE_FORMAT)
		<< "\tend date time->" << Poco::DateTimeFormatter::format(*end_date_time_, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	if ( time_now >= *end_date_time_ )
	{
		LOG_O(Log_error) << "time_now大于到期时间";
		return false;
	}
	SetIoTimer(1000 * 60 * 5, [this]( IoTimer* timer ){
		DateTimeLocal time_now;
		if ( time_now >= *end_date_time_ )
		{
			throw CmnTerminateProcessException("fjdkacv");
		}
		assert(timer);
		timer->Continue();
	});

	if (!WaitUntil(1000 * 60, [=](){
		return !account_.empty();
	}) )
		return false;
	LOG_O(Log_trace) << "验证信息接收成功";
	VMP_END;
	//////////////////////////////////////////////////////////////////////////
	//初始化
	GetJsonEcho();
	GetLuaScript();
	GetGLua().LuaObjCall<void>("RegForChineseScript");

	if ( !stPlayersData::GetMe().Load() )
	{
		LOG_O(Log_error) << "players data加载失败";
		return false;
	}

	di_press_key_ptr_.reset(new DI_PressKeyPtr::element_type);

	//初始化RSA密钥
	/*if (!rsa_.Init())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "rsa key init failed";
		return false;
	}
	assert(rsa_.public_key_);
	rsa_.public_key_->SetEncoding(ICryptoStr::ENC_BASE64);*/

	InitInter(GetMyApp().game_main_wnd_);
	//////////////////////////////////////////////////////////////////////////
	return true;
}

void FuncObj_NormalC::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);
	Clear();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "断开连接，中止进程";
	GetIoService().post([](){
		throw CmnTerminateProcessException();
	});
}

bool FuncObj_NormalC::IsMatchingState( GType::enGameState game_state )
{
	using namespace GType;
	if ( game_state < enGameState_Match_Wait )
		return false;
	if ( game_state > enGameState_MatchEnd_Back )
		return false;
	return true;
}

bool FuncObj_NormalC::CanChangeMatchingState( GType::enGameState game_state ) const
{
	using namespace GType;
	if (IsMatchingState(game_state_))
	{
		if (IsMatchingState(game_state))
			return true;
		if ( enGameState_MatchEnd_Back == game_state_ || enGameState_MatchEnd == game_state_ )
			return true;
		return false;
	}
	return true;
}

void FuncObj_NormalC::ElapseTimers()
{
	return;
}

TimeElapse& FuncObj_NormalC::GetSetMatchTimer()
{
	return timer_set_match_end_;
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

GLuaC& FuncObj_NormalC::GetGLua()
{
	return LuaThrdData::AsRef<GLuaC>();
}

JsonEcho& FuncObj_NormalC::GetJsonEcho()
{
	if (json_echo_)
		return *json_echo_;
	json_echo_.reset(new JsonEcho(GetGLua(), *this));
	return *json_echo_;
}

SessionOperC::SessionOperC( FuncObj_NormalC& func_obj_normal ) : BufCall_SocketExcept(&func_obj_normal, kObjIdNormal),
	func_obj_normal_(func_obj_normal)
{

}

void SessionOperC::GetPlayersInfo()
{
	auto& gplayer = GPlayer::GetMe();
	const auto& players = gplayer.GetAllPlayers();

	Json::Value json_players;
	Json::Value json_player_tmp;
	for ( const auto& player : players )
	{
		json_player_tmp["_id"] = player.id_;
		json_player_tmp["grade"] = player.grade_;
		json_player_tmp["price"] = player.price_;
		json_player_tmp["life"] = player.life_;
		json_player_tmp["position"] = player.player_pos_;
		json_player_tmp["spid"] = player.spid_;
		json_player_tmp["name"] = player.GetPlayerName();
		json_players.append(json_player_tmp);
	}

	ToCall(BC_MakeP("SetPlayersInfo")<<json_players);
}

void SessionOperC::GetItemsInfo()
{
	auto& gplayer = GPlayer::GetMe();
	const auto& items_info = gplayer.GetAllItems();

	Json::Value json_items;
	Json::Value json_item_tmp;
	for ( const auto& item : items_info )
	{
		json_item_tmp["id"] = item.id_;
		json_item_tmp["itemid"] = item.itemid_;
		json_item_tmp["remains"] = item.remains_;
		json_item_tmp["expired"] = item.expired_;
		json_items.append(json_item_tmp);
	}

	ToCall(BC_MakeP("SetItemsInfo")<<json_items);
}

void SessionOperC::SetLoginSucceed( bool succeed )
{
	VMP_BEGIN_EX;
	func_obj_normal_.login_succeed_ = succeed;
	VMP_END;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "验证登陆结果->" << succeed;
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

void GCoordinateC::OnQuestSoldInfo()
{
	LOG_O(Log_trace) << "OnQuestSoldInfo";
	auto& gp = GPlayer::GetMe();
	if (gp.NeedRefreshSelfOffserList())
		gp.RefreshSelfOfferList();
	Json::Value sold_players;
	if ( !gp.GenSoldPlayersInfo(sold_players, true) )
		return;
	if (sold_players.empty())
		return;
	SetCurRoute(GetSourceRoute());
	nor_.coordinate_.ToCall(BC_MakeP("OnSoldInfo")<<sold_players);
}

void GCoordinateC::OnSoldInfo( const Json::Value& sold_players )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "接收到拍卖的信息";
	GPlayer::GetMe().AddOtherOffers(sold_players);
}

void SessionOperC::SetGameAcType( int game_ac_type )
{
	func_obj_normal_.game_ac_type_ = (enGameAcType)game_ac_type;

	auto& gp = GPlayer::GetMe();
	gp.ClearOtherSoldInfo();
	//请求对方的拍卖信息
	func_obj_normal_.GetCoordinate().QuestSoldInfo();
}

void GCoordinateC::OnCancelSale( const Json::Value& sold_players )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "游戏中接收到取消拍卖的信息";
	GPlayer::GetMe().DeleteOtherOffers(sold_players);
}

void SessionOperC::GetMyOfferList()
{
	auto& gp = GPlayer::GetMe();
	gp.RefreshSelfOfferList();
	Json::Value sold_players;
	if ( !gp.GenAllSoldPlayersInfo(sold_players) )
		return;
	if (sold_players.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "我的出售列表为空";
		return;
	}
	ToCall(BC_MakeP("SetMyOfferList")<<sold_players);
}

void SessionOperC::GetOtherOfferItems()
{
	auto& gp = GPlayer::GetMe();
	Json::Value sold_players;
	if ( !gp.GenOtherOfferItemsInfo(sold_players) )
		return;
	if (sold_players.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "其他号的出售项为空";
		return;
	}
	ToCall(BC_MakeP("SetOtherOfferItems")<<sold_players);
}

void SessionOperC::StableString( const std::string& stable_str )
{
	VMP_BEGIN_EX;
	if ( stable_str != GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToGame)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "固定的字符串不匹配，隔断时间关闭套接字";
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
	/*
	VMP_BEGIN_EX;
	assert(func_obj_normal_.public_key_);
	auto en_str = func_obj_normal_.public_key_->Encrypt(VMP_PROTECT_STR(kCheckPubKey_ConsoleToGame));
	if ( en_str != encode_str )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "验证公钥失败";
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
		LOG_O(Log_info) << "验证公钥成功";
	}
	VMP_END;
	*/
}

BC_REG_CLS_DEFINE(SessionOperC){
	bufcall::class_<SessionOperC>("SessionOperC")
	.def("GetPlayersInfo", &SessionOperC::GetPlayersInfo)
	.def("GetItemsInfo", &SessionOperC::GetItemsInfo)
	.def(g_call_fn_name_set_login_succeed, &SessionOperC::SetLoginSucceed)
	.def("SetEndDateTime", &SessionOperC::SetEndDateTime)
	.def("sads", &SessionOperC::SetAddrDatas)
	.def("SetGameSerIdx", &SessionOperC::SetGameSerIdx)
	.def("SetGameAcType", &SessionOperC::SetGameAcType)
	.def("GetMyOfferList", &SessionOperC::GetMyOfferList)
	.def("GetOtherOfferItems", &SessionOperC::GetOtherOfferItems)
	.def("SetMatchTime", &SessionOperC::StableString)
	.def("SetPlayerInfo", &SessionOperC::CheckRsaPubKey);
}

GCoordinateC::GCoordinateC( FuncObj_NormalC& func_obj_normal )
	: BufCallCoorTrans(&func_obj_normal),
	nor_(func_obj_normal)
{

}

void GCoordinateC::QuestSoldInfo()
{
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnQuestSoldInfo"));
}

void GCoordinateC::CancelSale_OtherSide( const Json::Value& sold_players )
{
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnCancelSale")<<sold_players);
}

BC_REG_CLS_DEFINE(GCoordinateC){
	bufcall::class_<GCoordinateC>("GCoordinateC")
	.def("OnQuestSoldInfo", &GCoordinateC::OnQuestSoldInfo)
	.def("OnSoldInfo", &GCoordinateC::OnSoldInfo)
	.def("OnCancelSale", &GCoordinateC::OnCancelSale);
}

void GCoordinateC::SellPlayersSwitch( const Json::Value& sold_players )
{
	if (sold_players.empty())
		return;
	auto other_game_ac_type = nor_.SwitchGameAcType();
	if ( kGAT_Invalid == other_game_ac_type )
	{
		assert(false);
		return;
	}
	SetCurRoute(MakeTransRoute(other_game_ac_type, nor_.game_ser_idx_));
	ToCall(BC_MakeP("OnSoldInfo")<<sold_players);
}
