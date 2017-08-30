#include "stdafx.h"
#include <AppMix/OnlyGameComponent.h>
#include <Common/LogOut.h>
#include <Actions/CA_Action.h>
#include <boost/thread.hpp>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(GameConnectionOper){
	bufcall::class_<GameConnectionOper, CmnGameConnectionOper>("GameConnectionOper")
	.def("B22B5A47CE8C0", &GameConnectionOper::OnCheckInfo_)
	.def("OnSetLuaCfgStr_", &GameConnectionOper::OnSetLuaCfgStr_)
	.def("ForegroundTheWindow", &GameConnectionOper::ForegroundTheWindow);
}

BC_REG_CLS_DEFINE(CmnGameScriptEcho){
	bufcall::class_<CmnGameScriptEcho, LuasScriptEchoC>("CmnGameScriptEcho")
	;
}
//////////////////////////////////////////////////////////////////////////
GameConnectionC::GameConnectionC(Cmn_GameDllApp& app, pt_word dest_port) : CmnGameConnectionC(app, dest_port)
{
	script_echo_ = nullptr;
	cfg_parser_ = nullptr;
}

GameConnectionOper* GameConnectionC::CreateConnectionOper()
{
	return Cmn_GameDllApp::GetApp().GetAppFactory().CreateGameConnectionOper(*this);
}

bool GameConnectionC::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	if (!end_date_time_)
	{
		assert(false);
		return false;
	}
	if (!addr_datas_)
	{
		assert(false);
		return false;
	}
	if (!str_datas_)
	{
		assert(false);
		return false;
	}
	Cmn_GameDllApp::ShellIo(LuaSerialize::ToLuaStringCall("s_cmncfg:OnS_GameConnected"), nullptr, true);
	return true;
}

GameConnectionC::~GameConnectionC()
{
	delete script_echo_;
	script_echo_ = nullptr;
	delete cfg_parser_;
	cfg_parser_ = nullptr;
}

bool GameConnectionC::PreConnect()
{
	if (!__super::PreConnect())
		return false;
	assert(!script_echo_);
	script_echo_ = Cmn_GameDllApp::GetApp().GetAppFactory().CreateScriptEcho(*this);
	if (!script_echo_)
	{
		assert(false);
		return false;
	}
	return true;
}

CmnGameScriptEcho* GameConnectionC::GetScriptEcho() const
{
	assert(script_echo_);
	return script_echo_;
}

LuaCfgParser& GameConnectionC::GetLuaCfgParser()
{
	if (cfg_parser_)
		return *cfg_parser_;
	cfg_parser_ = Cmn_GameDllApp::GetApp().GetAppFactory().CreateLuaCfgParser(*this);
	assert(cfg_parser_);
	return *cfg_parser_;
}

const luabind::object& GameConnectionC::GetConsoleCfg() const
{
	assert(console_cfg_.is_valid());
	return console_cfg_;
}

void GameConnectionC::SetLuaSerialize(const std::string& lua_str)
{
	assert(!lua_str.empty());
	using namespace luabind;
	object empty_tbl = newtable(LuaThrdData::GetMe().lua());
	console_cfg_ = LuaSerialize::PlutoUnpersist(empty_tbl, lua_str);
	if (!console_cfg_.is_valid())
	{
		assert(false);
		Disconnect();
		return;
	}
	auto& parser = GetLuaCfgParser();
	try{
		if (!parser.ParseLuaCfg(console_cfg_))
		{
			assert(false);
			Disconnect();
			return;
		}
	}
	catch (...){
		assert(false);
		Disconnect();
		throw;
	}
	GetApp().OnSetConsoleCfg(console_cfg_);
}

void GameConnectionC::OnDisconnect(IoConnectBase* connect)
{
	{
		luabind::object().swap(console_cfg_);
	}
	__super::OnDisconnect(connect);
	LOG_O(Log_debug) << "准备关闭进程，等待2秒";
	::Sleep(2000);
	throw CmnCloseProcessException("rehuie1oqsdfcfdae");
}

void GameConnectionC::GameLoginSucceed()
{
	if (!connect_oper_)
	{
		assert(false);
		return;
	}
	static bool exec_once = false;
	if (exec_once)
	{
		assert(false);
		return;
	}
	exec_once = true;
	if (!connect_oper_->ToCall(BC_MakeP("GameLoginSucceed")))
	{
		assert(false);
		return;
	}
}

void GameConnectionC::ModifyConsoleCfg(const std::string& key, const luabind::object& value)
{
	if (!value.is_valid())
		return ModifyConsoleCfg(key, kEmptyStr, false);
	//用pluto的话，控制台那边不好实现。
	const auto& str = LuaSerialize::ToString(value);
	return ModifyConsoleCfg(key, str, false);
}

void GameConnectionC::ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string)
{
	if (key.empty())
	{
		assert(false);
		return;
	}
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	if (as_string)
	{
		std::string v = "[[";
		v += value;
		v += "]]";
		oper->ToCall(BC_MakeP("OnModifyConsoleCfg") << key << v);
	}
	else
		oper->ToCall(BC_MakeP("OnModifyConsoleCfg") << key << value);
}

CA_T<CA_AsTimerAction> OnlyGameAppFactory::MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action)
{
	return CA_Factory::MakeAsTimerAction(decoration, timer_action, Cmn_OnlyGameDllApp::GetApp().as_timer_action_time_);
}

void Cmn_OnlyGameDllApp::SendGameLoginSucceed()
{
	auto p = GetGameConnection();
	if (!p)
	{
		assert(false);
		return;
	}
	p->GameLoginSucceed();
}

OnlyGameAppFactory* Cmn_OnlyGameDllApp::CreateAppFactory()
{
	return new OnlyGameAppFactory;
}

Cmn_OnlyGameDllApp& Cmn_OnlyGameDllApp::GetApp()
{
	return Cmn_OnlyGameDllApp::GetAppBase().CastAs<Cmn_OnlyGameDllApp>();
}

void Cmn_OnlyGameDllApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_OnlyGameDllApp, Cmn_GameDllApp>("Cmn_OnlyGameDllApp")
			.def("AddScriptExecEnv", &Cmn_OnlyGameDllApp::_AddScriptExecEnv_1_)
			.def("AddScriptExecEnv", &Cmn_OnlyGameDllApp::_AddScriptExecEnv_2_)
			.def("EraseScriptExecEnv", &Cmn_OnlyGameDllApp::_EraseScriptExecEnv_)
			.def("GetGameTopHwnd", &Cmn_OnlyGameDllApp::_GetGameTopHwnd_),

			def("SetMaxGameRoles", &Cmn_OnlyGameDllApp::SetMaxGameRoles_),
			def("AutoCreateSelectRole", &Cmn_OnlyGameDllApp::AutoCreateSelectRole_)
		];
	CmnGameScriptEcho::RegForLua(l);
	if (ch_role_name_)
		RandomCombineStr::RegForLua(l);

	CSH_Handler::RegForLua(l);
}

bool Cmn_OnlyGameDllApp::OnInit_()
{
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
	game_dll_data_ = factory.CreateGameDllData();
	if (!game_dll_data_)
	{
		assert(false);
		return false;
	}
	if (!__super::OnInit_())
		return false;
	return true;
}

GameConnectionC* Cmn_OnlyGameDllApp::GetConnection()
{
	return GetGameConnection();
}

void Cmn_OnlyGameDllApp::OnSetConsoleCfg(const luabind::object& console_cfg)
{
	__super::OnSetConsoleCfg(console_cfg);
	game_roles_.clear();
	using namespace luabind;
	object roles_info = console_cfg["rolesInfo"];
	if (LUA_TTABLE != type(roles_info))
	{
		assert(false);
		return;
	}
	auto& factory = GetAppFactory();
	for (auto& v : roles_info)
	{
		GameRoleInfoPtr role_info(factory.CreateGameRoleInfo());
		if (!role_info)
		{
			assert(false);
			return;
		}
		if (!role_info->SerializeR(v))
		{
			assert(false);
			continue;
		}
		game_roles_.push_back(role_info);
	}
}

OnlyGameAppFactory& Cmn_OnlyGameDllApp::GetAppFactory() const
{
	return __super::GetAppFactory().CastAs<OnlyGameAppFactory>();
}

void Cmn_OnlyGameDllApp::AddGameRolesInfo(const GameRolesKeyCont& roles_key)
{
	GameRoleInfoCont game_roles_tmp;
	game_roles_.swap(game_roles_tmp);
	auto& factory = GetAppFactory();
	for (auto& v : roles_key)
	{
		for (auto& role : game_roles_tmp)
		{
			assert(role);
			if (role->role_key_ == v)
			{
				game_roles_.push_back(role);
				goto LABLE_CONTINUE;
			}
		}
		goto LABLE_NEXT;
	LABLE_CONTINUE:
		continue;
	LABLE_NEXT:
		GameRoleInfoPtr role_info(factory.CreateGameRoleInfo());
		if (!role_info)
		{
			assert(false);
			return;
		}
		role_info->role_key_ = v;
		role_info->role_state_ = CmnGameRoleInfo::kRS_Free;
		game_roles_.push_back(role_info);
	}
	//////////////////////////////////////////////////////////////////////////
	//***********for test
	/*DateTimeLocal time;
	time += Poco::Timespan(60, 0);
	for (auto& v : game_roles_)
	{
	v->role_state_ = CmnGameRoleInfo::kRS_TimeRestricted;
	v->time_restricted_ = time;
	}*/
	//////////////////////////////////////////////////////////////////////////
	SaveGameRolesInfo();
}

void Cmn_OnlyGameDllApp::SetGameRoleInfo(const std::string& role_key, CmnGameRoleInfo::enRoleState role_state)
{
	if (role_key.empty())
	{
		assert(false);
		return;
	}
	if (role_state == CmnGameRoleInfo::kRS_TimeRestricted)
	{
		assert(!"时间限制的，要用另一个函数");
		return;
	}
	for (auto& v : game_roles_)
	{
		assert(v);
		if (v->role_key_ == role_key)
		{
			if (v->role_state_ == role_state)
				return;
			v->role_state_ = role_state;
			SaveGameRolesInfo();
			return;
		}
	}
	auto& factory = GetAppFactory();
	GameRoleInfoPtr role_info(factory.CreateGameRoleInfo());
	if (!role_info)
	{
		assert(false);
		return;
	}
	role_info->role_key_ = role_key;
	role_info->role_state_ = role_state;
	game_roles_.push_back(role_info);
	SaveGameRolesInfo();
}

void Cmn_OnlyGameDllApp::SetGameRoleInfo(const std::string& role_key, const DateTimeLocal& time_restricted)
{
	if (role_key.empty())
	{
		assert(false);
		return;
	}
	GameRoleInfoPtr role_info;
	for (auto& v : game_roles_)
	{
		assert(v);
		if (v->role_key_ == role_key)
		{
			role_info = v;
			break;
		}
	}
	if (!role_info)
	{
		auto& factory = GetAppFactory();
		role_info.reset(factory.CreateGameRoleInfo());
		if (!role_info)
		{
			assert(false);
			return;
		}
		role_info->role_key_ = role_key;
	}
	role_info->role_state_ = CmnGameRoleInfo::kRS_TimeRestricted;
	role_info->time_restricted_ = time_restricted;
	SaveGameRolesInfo();
}

void Cmn_OnlyGameDllApp::SaveGameRolesInfo()
{
	using namespace luabind;
	auto& lua_thrd = LuaThrdData::GetMe();
	auto lua = lua_thrd.lua();
	assert(lua);
	object lua_roles(newtable(lua));
	int idx = 1;
	for (auto& v : game_roles_)
	{
		object tbl(newtable(lua));
		if (!v->SerializeW(tbl))
		{
			assert(false);
			continue;
		}
		lua_roles[idx] = tbl;
		++idx;
	}
	ModifyConsoleCfg("rolesInfo", lua_roles);
}

void Cmn_OnlyGameDllApp::SetMaxGameRoles(pt_dword max_game_roles)
{
	max_game_roles_ = std::max(max_game_roles, (pt_dword)1);
}

bool Cmn_OnlyGameDllApp::AutoCreateSelectRole()
{
	GameRoleInfoPtr free_role;
	DateTimeLocal time_now;
	for (auto& v : game_roles_)
	{
		assert(v);
		if (v->role_state_ == CmnGameRoleInfo::kRS_Free)
		{
			free_role = v;
			break;
		}
		else if (v->role_state_ == CmnGameRoleInfo::kRS_TimeRestricted)
		{
			if (v->time_restricted_ < time_now)
			{
				//最好不要改变状态
				//v->role_state_ = CmnGameRoleInfo::kRS_Free;
				free_role = v;
				break;
			}
		}
	}
	if (free_role)
	{
		return SelectEnterToGame(*free_role);
	}
	else
	{
		if (game_roles_.size() >= max_game_roles_)
		{
			for (auto& v : game_roles_)
			{
				assert(v);
				if (v->role_state_ == CmnGameRoleInfo::kRS_TimeRestricted)
				{
					if (!free_role || v->time_restricted_ < free_role->time_restricted_)
						free_role = v;
				}
			}
			std::string str_time_restricted;
			if (free_role)
			{
				str_time_restricted = std::to_string(free_role->time_restricted_.GetEpochTime());
			}
			else
			{
				time_now += Poco::Timespan(365, 0, 0, 0, 0);
				str_time_restricted = std::to_string(time_now.GetEpochTime());
			}
			ModifyConsoleCfg("timeRestricted", str_time_restricted, false);
			SetStateAtDisconnect("stateRolesUseout");
			LOG_O(Log_info) << "角色刷完了";
			SetIoTimer(1000 * 5, [this](IoTimer*){
				auto c = GetGameConnection();
				if (c)
					c->Disconnect();
			});
			return false;
		}

		while (true)
		{
			const auto& role_name = _RandomGameRoleNameImpl_();
			auto create_res = CreateEnterToGame(role_name);
			if (create_res == kCmnCRR_Succeed)
			{
				//并不需要保存角色信息
				//SaveGameRolesInfo();
				return true;
			}
			else if (create_res == kCmnCRR_NameErr)
				continue;
			else
			{
				LOG_O(Log_error) << "创建角色失败:" << role_name;
				assert(!"创建角色失败");
				return false;
			}
		}
	}
	assert(false);
	return false;
}

void Cmn_OnlyGameDllApp::SetStateAtDisconnect(const char* final_state)
{
	auto c = GetGameConnection();
	if (!c)
	{
		assert(false);
		return;
	}
	c->SetStateAtDisconnect(final_state);
}

enCmnCreateRoleRes Cmn_OnlyGameDllApp::CreateEnterToGame(const std::string& role_name)
{
	assert(false);
	return kCmnCRR_Error;
}

bool Cmn_OnlyGameDllApp::SelectEnterToGame(const CmnGameRoleInfo& role_info)
{
	assert(false);
	return false;
}

void Cmn_OnlyGameDllApp::SetMaxGameRoles_(pt_dword max_game_roles)
{
	return Cmn_OnlyGameDllApp::GetApp().SetMaxGameRoles(max_game_roles);
}

bool Cmn_OnlyGameDllApp::AutoCreateSelectRole_()
{
	return Cmn_OnlyGameDllApp::GetApp().AutoCreateSelectRole();
}

Cmn_OnlyGameDllApp::Cmn_OnlyGameDllApp()
{
	ch_role_name_ = true;
	max_game_roles_ = 1;
	as_timer_action_time_ = 70;
	event_init_thread_ = nullptr;
	init_thread_stack_size_ = 0;
	init_thread_valid_ = false;
	RandomCombineStr::GetDefMe().SetTotalLen_MinMax(4, 20);
}

std::string Cmn_OnlyGameDllApp::_RandomGameRoleNameImpl_()
{
	std::string res;
	auto& maker = RandomCombineStr::GetDefMe();
	res = maker.GenRandomStr(kEmptyStr);
	if (res.empty())
		res = RandomGameRoleName(maker.GetMinLen(), maker.GetMaxLen());
	return res;
}

Cmn_GameDllData& Cmn_OnlyGameDllApp::GetGameDllData() const
{
	return __super::GetGameDllData().CastAs<Cmn_GameDllData>();
}

GameConnectionC* OnlyGameAppFactory::CreateGameConnection(pt_word dest_port)
{
	assert(dest_port);
	return new GameConnectionC(Cmn_OnlyGameDllApp::GetApp(), dest_port);
}

GameConnectionOper* OnlyGameAppFactory::CreateGameConnectionOper(GameConnectionC& connection)
{
	return new GameConnectionOper(connection);
}

CmnGameScriptEcho* OnlyGameAppFactory::CreateScriptEcho(GameConnectionC& connection)
{
	return new CmnGameScriptEcho(connection);
}

ScriptRunRAII* OnlyGameAppFactory::CreateScriptRunRAII()
{
	return new ScriptRunRAII;
}

LuaCfgParser* OnlyGameAppFactory::CreateLuaCfgParser(GameConnectionC& connection)
{
	return new LuaCfgParser;
}

Cmn_GameDllData* OnlyGameAppFactory::CreateGameDllData()
{
	return new Cmn_GameDllData;
}

GameDllLuaThrdData* OnlyGameAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new GameDllLuaThrdData(lstate);
}

CmnGameRoleInfo* OnlyGameAppFactory::CreateGameRoleInfo()
{
	return new CmnGameRoleInfo;
}

CSH_HandlerPtr OnlyGameAppFactory::CreateSceneRecycleHandler(const luabind::object& tbl)
{
	return CSH_MakeHandler<CSH_RecycleHandler, CSH_tagCont>(tbl);
}
GameConnectionOper::GameConnectionOper(GameConnectionC& connection) : CmnGameConnectionOper(connection)
{

}

void GameConnectionOper::OnCheckInfo_(const DateTimeLocal& end_date_time,
	const std::vector<DWORD>& addr_datas, const std::vector<std::string>& str_datas, pt_csz version)
{
	auto& app = connection_.GetApp();
	if (app.GetGameDllData().dll_version_ != version || DateTimeLocal() >= end_date_time)
	{
		assert(false);
		auto shared = connection_.shared_from_this();
		GetIoService().post([shared](){
			if (shared->ObjValid())
				shared->DoDisconnect();
		});
		return;
	}
	auto& connect = GetConnection();
	connect.end_date_time_ = end_date_time;
	connect.addr_datas_ = addr_datas;
	connect.str_datas_ = str_datas;
}

GameConnectionC& GameConnectionOper::GetConnection() const
{
	return *static_cast<GameConnectionC*>(&connection_);
}

void GameConnectionOper::OnSetLuaCfgStr_(const std::string& lua_serialize)
{
	GetConnection().SetLuaSerialize(lua_serialize);
}

void GameConnectionOper::ForegroundTheWindow()
{
	auto& data = Cmn_OnlyGameDllApp::GetApp().GetGameDllData();
	if (!data.game_top_hwnd_)
	{
		assert(false);
		return;
	}
	ForegroundWnd(data.game_top_hwnd_);
}

Cmn_GameDllData::Cmn_GameDllData()
{
	dll_version_ = kGameDllVersion;
	game_top_hwnd_ = nullptr;
	port_ = P_Port_Game;
}

static int __AddScriptExecEnv_Index__(lua_State *L){
	using namespace luabind;
	object key(from_stack(L, -1));
	auto env_cont = (const Cmn_GameDllData::ScriptExecEnvContT*)lua_touserdata(L, lua_upvalueindex(1));
	for (auto& info : static_cast<const Cmn_GameDllData::ScriptExecEnvContT&>(*env_cont))
	{
		const object& v = info.exec_env_[key];
		if (LUA_TNIL == type(v))
			continue;
		v.push(L);
		return 1;
	}
	return 0;
}
bool Cmn_GameDllData::AddScriptExecEnv(const char* key, const luabind::object& exec_env)
{
	using namespace luabind;
	if (!exec_env.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(exec_env))
	{
		assert(false);
		return false;
	}
	if (!script_exec_env_.is_valid())
	{
		auto lstate = exec_env.interpreter();
		script_exec_env_ = newtable(lstate);
		object mt = newtable(lstate);
		setmetatable(script_exec_env_, mt);
		lua_pushlightuserdata(lstate, &script_exec_env_array_);
		lua_pushcclosure(lstate, __AddScriptExecEnv_Index__, 1);
		object fn(from_stack(lstate, -1));
		lua_pop(lstate, 1);
		mt["__index"] = fn;
	}
	if (key)
	{
		for (auto& v : script_exec_env_array_)
		{
			if (v.key_ == key)
			{
				assert(false);
				return false;
			}
			if (v.exec_env_ == exec_env)
				return false;
		}
	}
	else
	{
		for (auto& v : script_exec_env_array_)
		{
			if (v.exec_env_ == exec_env)
				return false;
		}
	}
	script_exec_env_array_.push_back(stScriptExecEnvInfo());
	auto& back = script_exec_env_array_.back();
	if (key)
		back.key_ = key;
	back.exec_env_ = exec_env;
	return true;
}

void Cmn_GameDllData::FreeScriptExecEnv()
{
	luabind::object().swap(script_exec_env_);
	script_exec_env_array_.clear();
}

bool Cmn_GameDllData::EraseScriptExecEnv(const char* key)
{
	if (!key)
	{
		assert(false);
		return false;
	}
	auto it = script_exec_env_array_.begin(), it_end = script_exec_env_array_.end();
	for (; it != it_end; ++it)
	{
		if (it->key_ == key)
			break;
	}
	if (it == it_end)
		return false;
	script_exec_env_array_.erase(it);
	return true;
}

void Cmn_GameDllData::clear()
{
	FreeScriptExecEnv();
}

bool Cmn_OnlyGameDllApp::_AddScriptExecEnv_1_(const luabind::object& exec_env)
{
	return GetGameDllData().AddScriptExecEnv(nullptr, exec_env);
}

bool Cmn_OnlyGameDllApp::_AddScriptExecEnv_2_(const char* key, const luabind::object& exec_env)
{
	return GetGameDllData().AddScriptExecEnv(key, exec_env);
}

bool Cmn_OnlyGameDllApp::_EraseScriptExecEnv_(const char* key)
{
	return GetGameDllData().EraseScriptExecEnv(key);
}

long Cmn_OnlyGameDllApp::_GetGameTopHwnd_() const
{
	return (long)GetGameDllData().game_top_hwnd_;
}

GameConnectionC* Cmn_OnlyGameDllApp::GetGameConnection()
{
	auto res = __super::GetGameConnection();
	if (!res)
		return nullptr;
	return res->CastTo<GameConnectionC>();
}

bool Cmn_OnlyGameDllApp::ActiveInitThread()
{
	if (event_init_thread_)
	{
		if (!SetEvent(event_init_thread_))
		{
			assert(false);
			return false;
		}
	}
	else
	{
		init_thread_valid_ = true;
		event_init_thread_ = ::CreateEvent(GetNullDACL(), FALSE, TRUE, nullptr);
		if (!event_init_thread_)
		{
			assert(false);
			return false;
		}
		boost::thread::attributes thrd_attr;
		thrd_attr.set_stack_size(init_thread_stack_size_);
		boost::thread t(thrd_attr, [this](){
			while (event_init_thread_ && init_thread_valid_)
			{
				auto res = ::WaitForSingleObject(event_init_thread_, INFINITE);
				if (res == WAIT_ABANDONED)
				{
					assert(false);
					break;
				}
				else if (res != WAIT_OBJECT_0)
				{
					assert(false);
					break;
				}
				if (!init_thread_valid_)
					break;
				OnInitThreadRun();
			}
			if (event_init_thread_)
			{
				::CloseHandle(event_init_thread_);
				event_init_thread_ = nullptr;
			}
			init_thread_valid_ = false;
		});
	}
	return true;
}

void Cmn_OnlyGameDllApp::OnInitThreadRun()
{
	assert(false);
}

void Cmn_OnlyGameDllApp::CloseInitThread()
{
	if (event_init_thread_ && init_thread_valid_)
	{
		init_thread_valid_ = false;
		ActiveInitThread();
	}
}

int Cmn_OnlyGameDllApp::OnExit_()
{
	CloseInitThread();
	return OnExit_();
}

std::string Cmn_OnlyGameDllApp::RandomGameRoleName(pt_dword min_cnt, pt_dword max_cnt) const
{
	return Random_Str(min_cnt, max_cnt);
}

luabind::object Cmn_OnlyGameDllApp::GetConsoleCfg()
{
	auto c = GetGameConnection();
	if (c)
		return c->GetConsoleCfg();
	assert(false);
	return luabind::object();
}

CmnGameScriptEcho::CmnGameScriptEcho(GameConnectionC& parent) : LuasScriptEchoC(parent), connection_(parent)
{

}

ScriptRunRAII* CmnGameScriptEcho::CreateRunRAII()
{
	return Cmn_GameDllApp::GetApp().GetAppFactory().CreateScriptRunRAII();
}

bool CmnGameScriptEcho::OperResult(enOperResult result)
{
	return ToCall(BC_MakeP("OperResult") << result);
}

bool CmnGameScriptEcho::RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize)
{
	connection_.SetLuaSerialize(lua_serialize);
	return __super::RunLuaScript(lua_file_name, lua_serialize);
}

void CmnGameScriptEcho::PreCallScriptFn(const luabind::object& script_func)
{
	using namespace luabind;
	auto& data = Cmn_OnlyGameDllApp::GetApp().GetGameDllData();
	if (data.script_exec_env_.is_valid())
	{
		auto lstate = data.script_exec_env_.interpreter();
		script_func.push(lstate);
		//保证平衡堆栈，弹出上面的压栈
		detail::stack_pop pop(lstate, 1);
		lua_getfenv(lstate, -1);
		object old_env(from_stack(lstate, -1));
		lua_pop(lstate, 1);
		data.AddScriptExecEnv(nullptr, old_env);
		//设置执行环境
		data.script_exec_env_.push(lstate);
		lua_setfenv(lstate, -2);
	}
}

luabind::object CmnGameScriptEcho::GetConsoleCfg() const
{
	return Cmn_GameDllApp::GetApp().GetConsoleCfg();
}
