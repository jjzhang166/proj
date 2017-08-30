#include "stdafx.h"
#include <AppMix/GameComponent.h>
#include <Common/LogOut.h>
#include <AssetSys/AS_Sys.h>
#include <AppMix/AppExecEnv.h>
#include <Common/UsefulFunc.h>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(CmnGameConnectionOper){
	bufcall::class_<CmnGameConnectionOper>("CmnGameConnectionOper")
	.def("OnConnectDone_", &CmnGameConnectionOper::OnConnectDone_)
	.def("SetAcInfo_", &CmnGameConnectionOper::SetAcInfo_);
}
//////////////////////////////////////////////////////////////////////////
Cmn_GameDllApp::Cmn_GameDllApp()
{
	is_poll_ = false;
	is_log_dbgview_ = false;
	is_on_exited_ = false;
	game_dll_data_ = nullptr;
	login_dll_data_ = nullptr;
}

void Cmn_GameDllApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<stAsSerSessionInfo>("stAsSerSessionInfo")
			.def_readonly("ac", &stAsSerSessionInfo::ac_)
			.def_readonly("pwd", &stAsSerSessionInfo::pwd_)
			.def_readonly("area", &stAsSerSessionInfo::area_)
			.def_readonly("ser", &stAsSerSessionInfo::ser_),

			class_<Cmn_GameDllApp, Cmn_DllRunApp>("Cmn_GameDllApp")
			.def("GetAssetDirString", &Cmn_GameDllApp::GetAssetDirString)
			.def("GetAcInfo", &Cmn_GameDllApp::GetAcInfo)
		];
}

CmnGameConnectionC* GameAppFactory::CreateGameConnection(pt_word dest_port)
{
	return nullptr;
}

CmnGameConnectionC* GameAppFactory::CreateLoginConnection(pt_word dest_port)
{
	return nullptr;
}

GameConnectionOper* GameAppFactory::CreateGameConnectionOper(GameConnectionC& connection)
{
	return nullptr;
}

LoginConnectionOper* GameAppFactory::CreateLoginConnectionOper(LoginConnectionC& connection)
{
	return nullptr;
}

CmnGameScriptEcho* GameAppFactory::CreateScriptEcho(GameConnectionC& connection)
{
	return nullptr;
}

ScriptRunRAII* GameAppFactory::CreateScriptRunRAII()
{
	return nullptr;
}

CmnGameLuaThrdData* GameAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new CmnGameLuaThrdData(lstate);
}

LuaCfgParser* GameAppFactory::CreateLuaCfgParser(GameConnectionC& connection)
{
	return nullptr;
}

Cmn_DllData* GameAppFactory::CreateGameDllData()
{
	return nullptr;
}

Cmn_DllData* GameAppFactory::CreateLoginDllData()
{
	return nullptr;
}

CA_T<CA_AsTimerAction> GameAppFactory::MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action)
{
	assert(false);
	return nullptr;
}

bool Cmn_GameDllApp::OnInit_()
{
	assert(game_dll_data_ || login_dll_data_);
	if (!app_factory_)
	{
		app_factory_ = CreateAppFactory();
		if (!app_factory_)
		{
			assert(false);
			return false;
		}
	}
	auto& factory = GetAppFactory();
	//assert(false);
	LogToDbgviewHelper log_dbgview;
	BOOST_SCOPE_EXIT_ALL(&log_dbgview){
		log_dbgview.ClearHolder();
	};
	if (!__super::OnInit_())
		return false;
	if (login_dll_data_)
	{
		login_connection_.reset(factory.CreateLoginConnection(login_dll_data_->port_));
		if (login_connection_)
		{
			login_connection_->ConnectAsync();
			if (!login_connection_->WaitForConnectDone())
			{
				assert(false);
				return false;
			}
		}
	}
	
	if (game_dll_data_ && !login_connection_ && !game_connection_)
	{
		game_connection_.reset(factory.CreateGameConnection(game_dll_data_->port_));
		if (game_connection_)
		{
			game_connection_->ConnectAsync();
			if (!game_connection_->WaitForConnectDone())
			{
				assert(false);
				return false;
			}
		}
	}
	CmnGameConnectionC* conne = nullptr;
	if (login_connection_)
		conne = login_connection_.get();
	else
		conne = game_connection_.get();
	if (!conne)
	{
		assert(false);
		return false;
	}
	auto& ac_info = conne->GetAcInfo();
	if (ac_info.ac_.empty())
	{
		assert(false);
		return false;
	}
	auto log_pre_str = ac_info.ac_;
	log_pre_str += "-";
	if (!ac_info.area_.empty())
	{
		log_pre_str += ac_info.area_;
		log_pre_str += "-";
	}
	log_pre_str += ac_info.ser_;
	log_dbgview.SetDbgViewPreString(log_pre_str);
	return true;
}

int Cmn_GameDllApp::OnExit_()
{
	is_on_exited_ = true;
	if (game_connection_)
	{
		if (game_connection_->ObjValid())
			game_connection_->Disconnect();
		game_connection_.reset();
	}
	if (login_connection_)
	{
		if (login_connection_->ObjValid())
			login_connection_->Disconnect();
		login_connection_.reset();
	}
	if (game_dll_data_)
	{
		game_dll_data_->clear();
		delete game_dll_data_;
		game_dll_data_ = nullptr;
	}
	if (login_dll_data_)
	{
		login_dll_data_->clear();
		delete login_dll_data_;
		login_dll_data_ = nullptr;
	}
	return __super::OnExit_();
}

CmnGameConnectionC* Cmn_GameDllApp::GetGameConnection()
{
	return game_connection_.get();
}

CmnGameConnectionC* Cmn_GameDllApp::GetLoginConnection()
{
	return login_connection_.get();
}

void Cmn_GameDllApp::OnLoginDisconnect_()
{
	if (is_on_exited_)
		return;
	login_connection_.reset();
	if (!game_dll_data_)
		return;
	if (!game_connection_)
		game_connection_.reset(GetAppFactory().CreateGameConnection(game_dll_data_->port_));
	assert(game_connection_);
	if (game_connection_)
	{
		game_connection_->ConnectAsync();
		if (!game_connection_->WaitForConnectDone())
		{
			assert(false);
			return;
		}
	}
}

const std::string& Cmn_GameDllApp::GetAcItemKey() const
{
	return ac_item_key_;
}

Cmn_GameDllApp::~Cmn_GameDllApp()
{
	if (game_dll_data_)
	{
		delete game_dll_data_;
		game_dll_data_ = nullptr;
	}
	if (login_dll_data_)
	{
		delete login_dll_data_;
		login_dll_data_ = nullptr;
	}
}

GameAppFactory& Cmn_GameDllApp::GetAppFactory() const
{
	assert(app_factory_);
	return *static_cast<GameAppFactory*>(app_factory_);
}

GameAppFactory* Cmn_GameDllApp::CreateAppFactory()
{
	return new GameAppFactory;
}

Cmn_GameDllApp& Cmn_GameDllApp::GetApp()
{
	return Cmn_GameDllApp::GetAppBase().CastAs<Cmn_GameDllApp>();
}

Cmn_DllData& Cmn_GameDllApp::GetGameDllData() const
{
	assert(game_dll_data_);
	return *game_dll_data_;
}

Cmn_DllData& Cmn_GameDllApp::GetLoginDllData() const
{
	assert(login_dll_data_);
	return *login_dll_data_;
}

const stAsSerSessionInfo& Cmn_GameDllApp::GetAcInfo() const
{
	return ac_info_;
}

void Cmn_GameDllApp::SetAcInfo(const stAsSerSessionInfo& ac_info)
{
	if (ac_info_.login_process_info_ != ac_info.login_process_info_)
		ac_info_ = ac_info;
}

std::string Cmn_GameDllApp::GetAssetDirString() const
{
	auto node = GetAssetDir();
	if (!node)
	{
		assert(false);
		return kEmptyStr;
	}
	return node->GetAbsolutePath_().string();
}

void Cmn_GameDllApp::ModifyConsoleCfg(const std::string& key, const luabind::object& value)
{
	auto game_s = GetGameConnection();
	if (!game_s)
	{
		assert(false);
		return;
	}
	return game_s->ModifyConsoleCfg(key, value);
}

void Cmn_GameDllApp::ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string)
{
	auto game_s = GetGameConnection();
	if (!game_s)
	{
		assert(false);
		return;
	}
	return game_s->ModifyConsoleCfg(key, value, as_string);
}

void Cmn_GameDllApp::OnSetConsoleCfg(const luabind::object& console_cfg)
{
}

bool Cmn_GameDllApp::DoInit()
{
	if (!__super::DoInit())
		return false;
	ac_item_key_ = AppExecEnvBase::GetEnvAcItemKey();
	if (ac_item_key_.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

luabind::object Cmn_GameDllApp::GetConsoleCfg()
{
	return luabind::object();
}

CmnGameConnectionOper::CmnGameConnectionOper(CmnGameConnectionC& connection)
	: BufCall_SocketExcept(&connection, kFuncObjTemplateId_GameOrLogin), connection_(connection)
{

}

void CmnGameConnectionOper::OnConnectDone_(bool is_connected)
{
	assert(!connection_.notify_connected_done_);
	connection_.notify_connected_done_ = is_connected;
}

void CmnGameConnectionOper::SetAcInfo_(const stAsSerSessionInfo& ac_info)
{
	assert(!ac_info.ac_.empty());
	assert(connection_.ac_info_.ac_.empty());
	connection_.ac_info_ = ac_info;
	Cmn_GameDllApp::GetApp().SetAcInfo(ac_info);
}

CmnGameConnectionOper* CmnGameConnectionC::GetConnectionOper() const
{
	return connect_oper_;
}

void CmnGameConnectionC::SendAcPwdErr()
{
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	oper->ToCall(BC_MakeP("OnAcPwdErr"));
}

void CmnGameConnectionC::SendBanAc()
{
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	oper->ToCall(BC_MakeP("OnBacAc"));
}

void CmnGameConnectionC::SendFreeze()
{
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	oper->ToCall(BC_MakeP("OnFreeze"));
}

CmnGameConnectionC::CmnGameConnectionC(Cmn_GameDllApp& app, pt_word dest_port) : Connection(kSelfIpStr, dest_port), my_app_(app)
{
	connect_oper_ = nullptr;
}

bool CmnGameConnectionC::PreConnect()
{
	if (!__super::PreConnect())
		return false;
	connect_oper_ = CreateConnectionOper();
	if (!connect_oper_)
	{
		assert(false);
		return false;
	}
	return true;
}

CmnGameConnectionC::~CmnGameConnectionC()
{
	delete connect_oper_;
}

Cmn_GameDllApp& CmnGameConnectionC::GetApp()
{
	return my_app_;
}

bool CmnGameConnectionC::ConnectDone()
{
	if (!connect_oper_)
		return false;
	long curProcessId = wxGetProcessId();
	if (!connect_oper_->ToCall(BC_MakeP("OnAttachAcInfo_") << curProcessId << GetApp().GetAcItemKey()))
		return false;
	if (!WaitUntil(kToServerWaitTime, [this](){
		if (notify_connected_done_)
			return true;
		if (!this->ObjValid())
			return true;
		return false;
	}))
		return false;
	if (!CheckOptionalBoolRes(notify_connected_done_))
		return false;
	return __super::ConnectDone();
}

bool CmnGameConnectionC::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	return true;
}

const stAsSerSessionInfo& CmnGameConnectionC::GetAcInfo() const
{
	return ac_info_;
}

void CmnGameConnectionC::SetStateAtDisconnect(const char* final_state)
{
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	oper->ToCall(BC_MakeP("SetStateAtDisconnect") << final_state);
}

void CmnGameConnectionC::ModifyConsoleCfg(const std::string& key, const luabind::object& value)
{
	assert(false);
}

void CmnGameConnectionC::ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string)
{
	assert(false);
}

CmnGameRoleInfo::CmnGameRoleInfo(const std::string& role_key) : role_key_(role_key)
{
	role_state_ = kRS_Free;
}

CmnGameRoleInfo::CmnGameRoleInfo()
{
	role_state_ = kRS_Free;
}

bool CmnGameRoleInfo::SerializeR(const luabind::object& role_info)
{
	using namespace luabind;
	assert(LUA_TTABLE == type(role_info));
	role_key_ = LuaObjCast(role_info["key"], kEmptyStr);
	if (role_key_.empty())
	{
		assert(false);
		return false;
	}
	role_state_ = (enRoleState)LuaObjCast(role_info["state"], (int)role_state_);
	if (role_state_ != kRS_TimeRestricted)
		return true;
	auto str_time_restricted = LuaObjCast(role_info["timed"], kEmptyStr);
	if (str_time_restricted.empty())
	{
		assert(false);
		return false;
	}
	if (!time_restricted_.FromFormatString(str_time_restricted))
	{
		assert(false);
		return false;
	}
	return true;
}

bool CmnGameRoleInfo::SerializeW(luabind::object& role_info) const
{
	using namespace luabind;
	assert(LUA_TTABLE == type(role_info));
	role_info["key"] = role_key_;
	role_info["state"] = (int)role_state_;
	if (role_state_ != kRS_TimeRestricted)
		return true;
	const auto& str = time_restricted_.FormatString();
	if (str.empty())
	{
		assert(false);
		return false;
	}
	role_info["timed"] = str;
	return true;
}

Cmn_DllData::Cmn_DllData()
{
	port_ = 0;
}

void Cmn_DllData::clear()
{

}

Cmn_DllData::~Cmn_DllData()
{

}
