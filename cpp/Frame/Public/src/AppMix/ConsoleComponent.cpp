#include "stdafx.h"
#include <AppMix/ConsoleComponent.h>
#include <Net/CmnLogin.h>
#include <AssetSys/AS_Sys.h>
#include <AssetSys/AS_Dir.h>
#include <CmnMix/Cmn_Toolhelp.h>
#include <Protocol/HijackXmlCfg.h>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(CmnConsoleSessionOper){
	bufcall::class_<CmnConsoleSessionOper>("CmnConsoleSessionOper")
	.def("OnAttachAcInfo_", &CmnConsoleSessionOper::OnAttachAcInfo_)
	.def("OnAcPwdErr", &CmnConsoleSessionOper::OnAcPwdErr_)
	.def("OnBacAc", &CmnConsoleSessionOper::OnBacAc_)
	.def("OnFreeze", &CmnConsoleSessionOper::OnFreeze_)
	.def("SetStateAtDisconnect", &CmnConsoleSessionOper::SetStateAtDisconnect_);
}

BC_REG_CLS_DEFINE(ConsoleSessionOper){
	bufcall::class_<ConsoleSessionOper, CmnConsoleSessionOper>("ConsoleSessionOper")
	.def("GameLoginSucceed", &ConsoleSessionOper::GameLoginSucceed)
	.def("OnModifyConsoleCfg", &ConsoleSessionOper::OnModifyConsoleCfg_);
}

BC_REG_CLS_DEFINE(ConsoleLoginSessionOper){
	bufcall::class_<ConsoleLoginSessionOper, CmnConsoleSessionOper>("ConsoleLoginSessionOper")
	.def("OnRelogin", &OnRelogin_);
}

BC_REG_CLS_DEFINE(CmnConsoleScriptEcho){
	bufcall::class_<CmnConsoleScriptEcho, LuasScriptEchoS>("CmnConsoleScriptEcho")
	;
}
//////////////////////////////////////////////////////////////////////////

void Cmn_ThrdIo_Console::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	CmnDoLogin::RegForLua(l);
}

bool Cmn_ThrdIo_Console::DoInit()
{
	if (!__super::DoInit())
		return false;
	//必须先初始化IoService
	this->GetIoService();
	return true;
}

bool Cmn_ThrdIo_Console::OnInit_()
{
	auto& app = GetApp();
	auto& factory = app.GetAppFactory();
	app_ui_oper_.reset(factory.CreateIoUiOper(*this));
	assert(app_ui_oper_);
	if (!__super::OnInit_())
		return false;
	if (!my_lvm_)
	{
		assert(false);
		return false;
	}
	login_info_mgr_.reset(factory.CreateLoginInfoMgr());
	if (!login_info_mgr_)
	{
		assert(false);
		return false;
	}
	login_pri_data_.reset(new CmnPriLuaData(my_lvm_->lua()));
	if (!login_pri_data_)
	{
		assert(false);
		return false;
	}
	login_info_mgr_->SetLuaData(login_pri_data_.get());
	console_server_.reset(factory.CreateConsoleServer());
	assert(console_server_);
	console_login_server_.reset(factory.CreateConsoleLoginServer());
	assert(console_login_server_);
	return true;
}

int Cmn_ThrdIo_Console::OnExit_()
{
	//server最好放到最先释放
	if (console_server_)
	{
		console_server_->DisconnectAll();
		console_server_.reset();
	}
	if (console_login_server_)
	{
		console_login_server_->DisconnectAll();
		console_login_server_.reset();
	}
	login_info_mgr_.reset();
	login_pri_data_.reset();
	app_ui_oper_.reset();
	return __super::OnExit_();
}

Cmn_ThrdIo_Console::Cmn_ThrdIo_Console()
{
	is_just_create_thrd_ = false;
	stack_size_ = 0;
}

Cmn_ThrdIo_Console::~Cmn_ThrdIo_Console()
{
}

ConsoleServer* ConsoleAppFactory::CreateConsoleServer()
{
	return new ConsoleServer(WxLuaApp_Console::GetApp().port_game_);
}

ConsoleAppUiOperIo* Cmn_ThrdIo_Console::GetAppUiOper() const
{
	return app_ui_oper_.get();
}

Cmn_ThrdIo_Console::ConsoleServerPtr Cmn_ThrdIo_Console::GetConsoleServer() const
{
	return console_server_;
}

ConsoleLoginServer* ConsoleAppFactory::CreateConsoleLoginServer()
{
	return new ConsoleLoginServer(WxLuaApp_Console::GetApp().port_login_);
}

Cmn_ThrdIo_Console::ConsoleLoginServerPtr Cmn_ThrdIo_Console::GetConsoleLoginServer() const
{
	return console_login_server_;
}

bool Cmn_ThrdIo_Console::DoRun()
{
	assert(!IsAlive());
	auto err = Create(stack_size_);
	if (err == wxTHREAD_NO_ERROR)
	{
		is_just_create_thrd_ = true;
		::GetIoService().post([this](){
			this->Resume();
			is_just_create_thrd_ = false;
		});
		return true;
	}
	return false;
}

bool Cmn_ThrdIo_Console::IsThrdAlive() const
{
	if (is_just_create_thrd_)
		return true;
	return __super::IsThrdAlive();
}

CmnConsoleLoginInfoMgrBase* Cmn_ThrdIo_Console::GetLoginInfoMgr()
{
	return login_info_mgr_.get();
}

WxLuaApp_Console& Cmn_ThrdIo_Console::GetApp()
{
	return WxLuaApp_Console::GetAppBase().CastAs<WxLuaApp_Console>();
}

LuaThrdData* Cmn_ThrdIo_Console::CreateLuaVm()
{
	auto& app = GetApp();
	auto& factory = app.GetAppFactory();
	return factory.CreateIoThrdLuaVm();
}

bool WxLuaApp_Console::OnInit_()
{
	if (!CheckSingletonHandle(console_singleton_str_))
	{
		assert(!"进程必须唯一");
		return false;
	}
	assert(!app_factory_);
	app_factory_ = CreateAppFactory();
	if (!app_factory_)
	{
		assert(false);
		return false;
	}
	if (!exec_env_)
	{
		exec_env_ = GetAppFactory().CreateExecEnv();
		if (!exec_env_)
		{
			assert(false);
			return false;
		}
		if (!exec_env_->Init())
		{
			assert(false);
			return false;
		}
	}
	auto& factory = GetAppFactory();
	AddMainThrd();
	app_ui_oper_ = factory.CreateAppUiOper(*this);
	assert(app_ui_oper_);
	ThreadablePtr io_thrd(factory.CreateIoThrd());
	if (!io_thrd)
	{
		assert(false);
		return false;
	}
	AddThrd(io_thrd, true);
	if (!__super::OnInit_())
		return false;
	return true;
}

WxLuaApp_Console::WxLuaApp_Console() : version_game_dll_(kGameDllVersion), version_login_dll_(kLoginDllVersion),
	console_singleton_str_("EC1C05BC-1109-42D4-BA9E-0336CEA737A2")
{
	is_add_main_thred_ = false;
	app_ui_oper_ = nullptr;
#ifdef _DEBUG
	ac_item_key_use_uuid_ = false;
#else
	ac_item_key_use_uuid_ = true;
#endif
	port_game_ = P_Port_Game;
	port_login_ = P_Port_Login;
	time_login2game_succeed_ = 1000 * 60 * 6;
	time_game_login_succeed_ = 1000 * 60 * 5;
	glo_auto_run_script_ = true;
	exec_env_ = nullptr;
	ac_type_ = 0;
	game_dll_name_ = "gl.dll";
}

void WxLuaApp_Console::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<ConsoleAppUiOper>("ConsoleAppUiOper")
			.def("Login", &ConsoleAppUiOper::Login)
			.def("CancelLogin", &ConsoleAppUiOper::CancelLogin)
			.def("Run", &ConsoleAppUiOper::Run)
			.def("Stop", &ConsoleAppUiOper::Stop)
			.def("CloseGame", &ConsoleAppUiOper::CloseGame)
			.def("ShowGameWnd", &ConsoleAppUiOper::ShowGameWnd),
			
			class_<ConsoleAppUiOperMain, ConsoleAppUiOper>("ConsoleAppUiOperMain")
			.def("IsLogined", &ConsoleAppUiOperMain::IsLogined)
			.def("CloseLoginSession", &ConsoleAppUiOperMain::CloseLoginSession),

			class_<WxLuaApp_Console, WxLuaApp>("WxLuaApp_Console")
			.def("GetAppUiOper", &WxLuaApp_Console::GetAppUiOper)
			.def("OnSetGamePath", &WxLuaApp_Console::OnSetGamePath)
			.def("GenAcItemKey", &WxLuaApp_Console::GenAcItemKey)
			.def("SetAcItemInfo", &WxLuaApp_Console::SetAcItemInfo)
			.def("OnFrameCreated", &WxLuaApp_Console::OnFrameCreated)
			.def("SetGameSessionCfg", &WxLuaApp_Console::SetGameSessionCfg)
			.def("GetDirFileNames", &WxLuaApp_Console::GetDirFileNames)
			.def("CanLoginGameAc", &WxLuaApp_Console::CanLoginGameAc)
			.def("ParseMainCfg", &WxLuaApp_Console::ParseMainCfg)
			.def("SetCrackCaptchaDmt", &WxLuaApp_Console::SetCrackCaptchaDmt)
			.def("SetCrackCaptchaLocal", &WxLuaApp_Console::SetCrackCaptchaLocal)
			.def("AddAreaSer2Id", &WxLuaApp_Console::AddAreaSer2Id)
			.def("ParseAreaSer2Id", &WxLuaApp_Console::ParseAreaSer2Id)
			.def("OnUpdateAcItem", &WxLuaApp_Console::OnUpdateAcItem)
			.def("OnLuaAddAc", &WxLuaApp_Console::OnLuaAddAc)
			.def("OnLuaDeleteAc", &WxLuaApp_Console::OnLuaDeleteAc),

			class_<enCrackCaptchaMode>("enCrackCaptchaMode")
			.enum_("")
			[
				value("kCCMode_DaMaTu", kCCMode_DaMaTu),
				value("kCCMode_Local", kCCMode_Local)
			],
			//////////////////////////////////////////////////////////////////////////
			class_<ConsoleAcCfgMgr::stAcItemInfo, ConsoleAcCfgMgr::AcItemInfoPtrT>("ConsoleAcCfgItemInfo")
			.def(constructor<>())
			.def(constructor<const std::string&, const std::string&, const std::string&, const std::string&>())
			.def_readonly("ac_", &ConsoleAcCfgMgr::stAcItemInfo::ac_)
			.def_readonly("pwd_", &ConsoleAcCfgMgr::stAcItemInfo::pwd_)
			.def_readonly("area_", &ConsoleAcCfgMgr::stAcItemInfo::area_)
			.def_readonly("ser_", &ConsoleAcCfgMgr::stAcItemInfo::ser_),

			class_<ConsoleAcCfgMgr::AcItemInfoIterator, IteratorBase, ConsoleAcCfgMgr::AcItemInfoIteratorPtr>("ConsoleAcCfgMgrIterator")
			.def("CurItem", &ConsoleAcCfgMgr::AcItemInfoIterator::CurItem),

			class_<ConsoleAcCfgMgr>("ConsoleAcCfgMgr")
			.def(constructor<>())
			.def("Iterator", &ConsoleAcCfgMgr::Iterator)
			.def("Load", &ConsoleAcCfgMgr::Load)
			.def("Save", &ConsoleAcCfgMgr::Save)
			.def("Add", &ConsoleAcCfgMgr::Add)
			//////////////////////////////////////////////////////////////////////////
		];
}

WxLuaApp_Console::~WxLuaApp_Console()
{
	delete app_ui_oper_;
	delete exec_env_;
}

ConsoleAppUiOperMain* WxLuaApp_Console::GetAppUiOper() const
{
	return app_ui_oper_;
}

void WxLuaApp_Console::OnSetGamePath(const std::string& game_path, bool is_reset)
{
	using namespace boost::filesystem;
	game_dir_asnode_.reset();
	game_path_ = game_path;
	game_client_starter_path_ = game_path;
	if (game_path.empty())
		return;
	path path_game_dir = game_path;
	if (!exists(path_game_dir))
	{
		assert(false);
		return;
	}
	auto& as_sys = AS_Sys::GetMe();
	auto root_node = as_sys.GetRoot();
	if (!root_node)
	{
		assert(false);
		return;
	}
	static const path kPathGameRoot = L"__GamePathRoot__";
	auto game_path_root = root_node->FindChild(kPathGameRoot);
	if (game_path_root)
		game_path_root->clear();
	else
	{
		game_path_root.reset(new AS_MemoryDir);
		if (!game_path_root)
		{
			assert(false);
			return;
		}
		game_path_root->SetPath(kPathGameRoot);
		if (!root_node->Add(game_path_root))
		{
			assert(false);
			return;
		}
	}
	if (!is_directory(path_game_dir))
		path_game_dir = path_game_dir.parent_path();
	game_dir_asnode_.reset(new AS_DiskDir);
	if (!game_dir_asnode_)
	{
		assert(false);
		return;
	}
	game_dir_asnode_->SetPath(path_game_dir);
	if (!game_path_root->Add(game_dir_asnode_))
	{
		assert(false);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	if (!DiscoverGameClientStarterPath())
	{
		//assert(false);
		return;
	}
	if (!is_reset)
		return;
	if (!game_dir_asnode_)
	{
		assert(false);
		return;
	}
	auto game_dir_path = game_dir_asnode_->GetAbsolutePath_();
	CToolhelp::TeminateAllProcessAsPath(game_dir_path.native().c_str());
	try{
		if (!PreResolveGameFiles())
		{
			assert(false);
			return;
		}
		if (!ResolveGameFiles(game_dir_path))
		{
			assert(false);
			return;
		}
		if (!ModifySomeGameFiles())
		{
			assert(false);
			return;
		}
	} DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_O(Log_debug) << e.what();
		assert(false);
	})
	//////////////////////////////////////////////////////////////////////////
}

AS_NodePtr WxLuaApp_Console::GetGameDll() const
{
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	node = node->GetSysDir();
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	static boost::filesystem::path game_dll_path(GetGameDllName());
	node = node->FindChild(game_dll_path);
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	return node;
}

const char* WxLuaApp_Console::GetGameDllName() const
{
	return game_dll_name_;
}

std::string WxLuaApp_Console::GenAcItemKey(const std::string& ac, const std::string& area, const std::string& ser)
{
	assert(!ac.empty());
	std::string str_mix(ac);
	str_mix += area;
	str_mix += ser;
	if (ac_item_key_use_uuid_)
		return GenUuidStr(str_mix);
	else
		return str_mix;
}

Cmn_ThrdIo_Console* WxLuaApp_Console::GetIoThrd_()
{
	auto& io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return nullptr;
	}
	return io->CastTo<Cmn_ThrdIo_Console>();
}

void WxLuaApp_Console::SetAcItemInfo(const std::string& item_key, 
	const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser)
{
	if (item_key.empty() || ac.empty())
	{
		assert(false);
		return;
	}
	stAcSerInfo ac_info;
	ac_info.ac_ = ac;
	ac_info.pwd_ = pwd;
	ac_info.area_ = area;
	ac_info.ser_ = ser;
	auto io = GetIoThrd_();
	if (!io)
	{
		assert(false);
		return;
	}
	io->GetIoService().post([io, ac_info, item_key](){
		auto oper = io->GetAppUiOper();
		if (!oper)
		{
			assert(false);
			return;
		}
		oper->GetLoginMgr().AddAcItemInfo(item_key, ac_info);
	});
}

void WxLuaApp_Console::LoginByItemKey(const std::string& item_key, bool add_to_first)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_Login", item_key, add_to_first));
}

ConsoleAppFactory& WxLuaApp_Console::GetAppFactory() const
{
	assert(app_factory_);
	return *static_cast<ConsoleAppFactory*>(app_factory_);
}

WxLuaApp_Console& WxLuaApp_Console::GetApp()
{
	return WxLuaApp_Console::GetAppBase().CastAs<WxLuaApp_Console>();
}

ConsoleAppFactory* WxLuaApp_Console::CreateAppFactory()
{
	return new ConsoleAppFactory;
}

void WxLuaApp_Console::OnFrameCreated()
{
	if (!end_date_time_.empty())
	{
		ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_SetEndDateTime", end_date_time_));
	}
	auto& version_info = CmnConsoleCS_Connection::GetVersionInfo();
	if (!version_info.str_version_.empty())
	{
		ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_SetVersion", version_info.str_version_));
	}
	TimerDoMinutes();
	auto io_thrd = GetIoThrd();
	if (!io_thrd)
	{
		assert(false);
		return;
	}
	//主线程的计时器起不到计时的作用，必须在IO线程里计时，因为主线程里不会阻塞调用GetIoService().Run()。
	io_thrd->GetIoService().post([this](){
		SetIoTimer1(1000 * 60 * 5, [this](IoTimer& timer){
			this->GetIoService().post([this](){
				this->TimerDoMinutes();
			});
			timer.Continue();
		});
	});	
}

bool WxLuaApp_Console::RunScript(const std::string& item_key)
{
	assert(!item_key.empty());
	ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_RunScript", item_key));
	return true;
}

void WxLuaApp_Console::SetGameSessionCfg(const std::string& item_key, const std::string& lua_serialize)
{
	auto io = GetIoThrd_();
	if (!io)
	{
		assert(false);
		return;
	}
	io->GetIoService().post([io, item_key, lua_serialize](){
		auto server = io->GetConsoleServer();
		if (!server)
		{
			assert(false);
			return;
		}
		auto session = server->FindSessionByKey(item_key);
		if (!session || !session->ObjValid())
			return;
		auto s = boost::static_pointer_cast<ConsoleSessionS>(session);
		if (!s)
		{
			assert(false);
			return;
		}
		auto oper = s->GetSessionOper();
		if (!oper)
		{
			assert(false);
			return;
		}
		oper->SetLuaSerialize(lua_serialize);
	});
}

int WxLuaApp_Console::GetDirFileNames(const luabind::object& out_tbl, const std::string& dir_path, const std::string& file_extension, bool res_has_extension)
{
	using namespace luabind;
	if (LUA_TTABLE != type(out_tbl))
	{
		assert(false);
		return -1;
	}
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return -1;
	}
	node = node->FindPosteritySmart(dir_path);
	if (!node)
	{
		assert(false);
		return -1;
	}
	if (!node->Refresh(true))
	{
		assert(false);
		return -1;
	}
	auto iter = node->Iterator(false);
	if (!iter)
	{
		assert(false);
		return -1;
	}
	auto tbl_idx = table_maxn(out_tbl) + 1;
	int cnt = 0;
	for (iter->First(); !iter->IsDone(); iter->Next())
	{
		auto cur_item = iter->CurItem();
		if (!cur_item)
		{
			assert(false);
			return -1;
		}
		auto& the_path = cur_item->GetPath();
		if (!file_extension.empty())
		{
			if (!the_path.has_extension())
				continue;
			if (the_path.extension() != file_extension)
				continue;
		}
		if (res_has_extension)
		{
			out_tbl[tbl_idx++] = the_path.filename().string();
		}
		else
		{
			out_tbl[tbl_idx++] = the_path.filename().stem().string();
		}
		++cnt;
	}
	return cnt;
}

bool WxLuaApp_Console::CanLoginGameAc() const
{
	if (game_path_.empty())
	{
		wxMessageBox(wxT("还未选择目录！"), wxT("登陆错误"), wxOK);
		return false;
	}
	if (crack_captcha_info_.SameMode(kCCMode_DaMaTu))
	{
		if (crack_captcha_info_.GetDmtAc().empty())
		{
			wxMessageBox(wxT("尚未配置打码兔帐号"), wxT("登陆错误"), wxOK);
			return false;
		}
		if (crack_captcha_info_.GetDmtPwd().empty())
		{
			wxMessageBox(wxT("尚未配置打码兔密码"), wxT("登陆错误"), wxOK);
			return false;
		}
	}
	else if (crack_captcha_info_.SameMode(kCCMode_Local))
	{
		if (crack_captcha_info_.GetLocalIp().empty())
		{
			wxMessageBox(wxT("尚未配置本地答题IP"), wxT("登陆错误"), wxOK);
			return false;
		}
		if (crack_captcha_info_.GetLocalPort() == 0)
		{
			wxMessageBox(wxT("尚未配置本地答题端口"), wxT("登陆错误"), wxOK);
			return false;
		}
	}
	return true;
}

void WxLuaApp_Console::ParseMainCfg(const luabind::object& main_cfg)
{
	OnSetGamePath(LuaObjCast(main_cfg["gamePath"], kEmptyStr), false);

	enCrackCaptchaMode ccmode = LuaObjCast(main_cfg["enCrackCaptchaMode"], kCCMode_DaMaTu);
	if (ccmode == kCCMode_DaMaTu)
	{
		crack_captcha_info_.SetDmt(LuaObjCast(main_cfg["ccDmtAc"], kEmptyStr),
			LuaObjCast(main_cfg["ccDmtPwd"], kEmptyStr));
	}
	else if (ccmode == kCCMode_Local)
	{
		crack_captcha_info_.SetLocal(LuaObjCast(main_cfg["ccLocalIp"], kEmptyStr),
			LuaObjCast(main_cfg["ccLocalPort"], pt_word(0)));
	}
	else
	{
		assert(false);
		return;
	}
}

AppExecEnvBase& WxLuaApp_Console::GetExecEnv() const
{
	assert(exec_env_);
	return *exec_env_;
}

const std::string& WxLuaApp_Console::GetGameClientStarterPath() const
{
	return game_client_starter_path_;
}

void WxLuaApp_Console::OnLoginFailed(const std::string& item_key, const stAsSerSessionInfo& ac_info)
{
	auto main_oper = GetAppUiOper();
	if (!main_oper)
	{
		assert(false);
		return;
	}
	main_oper->CancelLogin(item_key);
}

void WxLuaApp_Console::TimerDoMinutes()
{
	ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_TimerDo"));
}

bool WxLuaApp_Console::DiscoverGameClientStarterPath()
{
	game_dir_starter_asnode_.reset();
	if (game_path_.empty())
		return false;
	if (!game_dir_asnode_)
	{
		assert(false);
		return false;
	}
	if (starter_relative_path_.empty())
	{
		starter_relative_path_ = GetStarterRalativePath();
		if (starter_relative_path_.empty())
		{
			assert(false);
			return false;
		}
	}
	auto starter_node = game_dir_asnode_->Find(starter_relative_path_);
	if (!starter_node)
	{
		assert(false);
		return false;
	}
	game_dir_starter_asnode_ = starter_node->Parent(true);
	if (!game_dir_starter_asnode_)
	{
		assert(false);
		return false;
	}
	game_client_starter_path_ = starter_node->GetAbsolutePath_().string();
	return true;
}

bool WxLuaApp_Console::ModifySomeGameFiles()
{
	return true;
}

void WxLuaApp_Console::AddGameFileDelete(const stPathFindingInfo& game_path)
{
	if (game_path.path_.empty())
	{
		assert(false);
		return;
	}
	game_files_delete_.push_back(game_path);
}

void WxLuaApp_Console::AddGameFileRename(const stPathFindingInfo& game_path, const std::string& new_name)
{
	if (game_path.path_.empty())
	{
		assert(false);
		return;
	}
	if (new_name.empty())
	{
		assert(false);
		return;
	}
	game_files_rename_.push_back(std::make_pair(game_path, new_name));
}

void WxLuaApp_Console::AddGameFileRenameToDir(const stPathFindingInfo& game_path, const std::string& new_name)
{
	if (game_path.path_.empty())
	{
		assert(false);
		return;
	}
	if (new_name.empty())
	{
		assert(false);
		return;
	}
	game_files_rename_to_dir_.push_back(std::make_pair(game_path, new_name));
}

void WxLuaApp_Console::AddGameFileOverwrite(const stPathFindingInfo& game_path, const stPathFindingInfo& src_file_path)
{
	if (game_path.path_.empty())
	{
		assert(false);
		return;
	}
	if (src_file_path.path_.empty())
	{
		assert(false);
		return;
	}
	game_files_overwrite_.push_back(std::make_pair(game_path, src_file_path));
}

void WxLuaApp_Console::AddD3d9Cfg(enD3d9Mode d3d9_mode, const stPathFindingInfo& game_file_path, const stPathFindingInfo& inject_file_path)
{
	if (game_file_path.path_.empty())
	{
		assert(false);
		return;
	}
	if (inject_file_path.path_.empty())
	{
		assert(false);
		return;
	}
	d3d9_configs_.push_back({ d3d9_mode, game_file_path, inject_file_path });
}

void WxLuaApp_Console::SetCrackCaptchaDmt(const std::string& ac, const std::string& pwd)
{
	crack_captcha_info_.SetDmt(ac, pwd);
}

void WxLuaApp_Console::SetCrackCaptchaLocal(const std::string& ip, pt_word port)
{
	crack_captcha_info_.SetLocal(ip, port);
}

bool WxLuaApp_Console::ResolveGameFiles(const boost::filesystem::path& game_dir_path)
{
	using namespace boost::filesystem;
	if (!game_dir_asnode_)
	{
		assert(false);
		return false;
	}
	for (auto& v : game_files_delete_)
	{
		auto game_file = SmartFindTheFile(v);
		if (!game_file)
			continue;
		game_file->DeleteDisks();
	}

	for (auto& v : game_files_rename_)
	{
		auto game_file = SmartFindTheFile(v.first);
		if (!game_file)
			continue;
		auto old_file_path = game_file->GetAbsolutePath_();
		boost::system::error_code err;
		auto new_file_name = old_file_path.parent_path();
		new_file_name /= v.second;
		rename(old_file_path, new_file_name, err);
#ifdef _DEBUG
		if (err)
			LOG_O(Log_debug) << "rename_error:" << err.message();
#endif
	}

	for (auto& v : game_files_rename_to_dir_)
	{
		auto game_file = SmartFindTheFile(v.first);
		if (!game_file)
			continue;
		if (game_file->GetType() == AS_Node::kDir)
			continue;
		auto old_file_path = game_file->GetAbsolutePath_();
		game_file->DeleteDisks();
		create_directory(old_file_path);
	}

	for (auto& v : game_files_overwrite_)
	{
		auto my_file = SmartFindTheFile(v.second);
		if (!my_file)
		{
			assert(false);
			continue;
		}
		auto game_file = SmartFindTheFile(v.first);
		if (!game_file)
		{
			auto the_dir = GetTheFileDirByMode(v.first.file_dir_);
			if (!the_dir)
			{
				assert(false);
				continue;
			}
			auto path_tmp = the_dir->GetAbsolutePath_();
			path_tmp /= v.first.path_;
			if (!my_file->SaveToDisk(path_tmp, AS_Node::kSF_Unknown))
			{
				assert(false);
			}
			continue;
		}
		const auto& game_path = game_file->GetAbsolutePath_().string();
		if (my_file->FileSameContent(game_path))
			continue;
		my_file->SaveToDisk(game_path, AS_Node::kSF_Unknown);
	}

	for (auto& v : d3d9_configs_)
	{
		auto game_file = SmartFindTheFile(v.game_file_path_);
		if (!game_file)
		{
			assert(false);
			continue;
		}
		auto injected_file = SmartFindTheFile(v.inject_file_path_);
		if (!injected_file)
		{
			assert(false);
			continue;
		}
		auto injected_file_path = injected_file->GetAbsolutePath_();
		if (injected_file_path.empty())
		{
			assert(false);
			continue;
		}

		auto game_file_path = game_file->GetAbsolutePath_();
		auto game_file_name = game_file_path.filename();
		if (game_file_name.empty())
		{
			assert(false);
			continue;
		}
		game_file_path = game_file_path.parent_path();

		if (v.d3d9_mode_ == kDM_Profile)
		{
			stHijackProfile profile;
			profile.injected_game_name_ = game_file_name.string();
			game_file_path /= stHijackProfile::GetCfgFileName();
			profile.SetProfile(game_file_path.string());
			profile.inject_dll_full_path_ = injected_file_path.string();
			if (!profile.Save())
			{
				assert(false);
			}
		}
		else if (v.d3d9_mode_ == kDM_Xml)
		{
			stHijackDllCfg xml_cfg;
			xml_cfg.injected_game_name_ = game_file_name.native().c_str();
			xml_cfg.inject_dll_full_path_ = injected_file_path.native().c_str();
			game_file_path /= stHijackDllCfg::GetCfgFileName();
			XmlCfg::Save(xml_cfg, game_file_path.string());
		}
		else
		{
			assert(false);
		}
	}
	return true;
}

AS_NodePtr WxLuaApp_Console::SmartFindTheFile(const stPathFindingInfo& path_info) const
{
	auto dir = GetTheFileDirByMode(path_info.file_dir_);
	if (!dir)
	{
		assert(false);
		return nullptr;
	}
	return FindTheFileByFindMode(dir, path_info.path_, path_info.find_mode_);
}

AS_NodePtr WxLuaApp_Console::GetTheFileDirByMode(enFileDirLocation mode) const
{
	if (mode == kSFM_AtGamePathRoot)
		return game_dir_asnode_;
	else if (mode == kSFM_AtGamePathStarter)
		return game_dir_starter_asnode_;
	else if (mode == kSFM_AtSystem)
	{
		auto root = AS_Sys::GetMe().GetRoot();
		if (!root)
		{
			assert(false);
			return nullptr;
		}
		auto sys_dir = root->GetSysDir();
		if (!sys_dir)
		{
			assert(false);
			return nullptr;
		}
		return sys_dir;
	}
	else if (mode == kSFM_AtSysAsset)
		return GetAssetDir();
	else
	{
		assert(false);
	}
	return nullptr;
}

AS_NodePtr WxLuaApp_Console::FindTheFileByFindMode(const AS_NodePtr& file_dir, const boost::filesystem::path& file_path, enFindMode find_mode)
{
	assert(file_dir);
	if (find_mode == kFM_Find)
		return file_dir->Find(file_path);
	else if (find_mode == kFM_FindChild)
		return file_dir->FindChild(file_path);
	else if (find_mode == kFM_FindPosterity)
		return file_dir->FindPosterity(file_path);
	assert(false);
	return nullptr;
}

bool WxLuaApp_Console::CloseTheGameProcess(const boost::filesystem::path& file_path) const
{
	if (!game_dir_starter_asnode_)
	{
		assert(false);
		return false;
	}
	auto game_file = game_dir_starter_asnode_->FindChild(file_path);
	if (!game_file)
	{
		assert(false);
		return false;
	}
	CPath cpath_tmp = game_file->GetAbsolutePath_().native().c_str();
	CToolhelp::TeminateAllProcess(cpath_tmp);
	return true;
}

bool WxLuaApp_Console::PreLoginGame()
{
	if (starter_relative_path_.empty())
	{
		starter_relative_path_ = GetStarterRalativePath();
		if (starter_relative_path_.empty())
		{
			assert(false);
			return false;
		}
	}
	if (!CloseTheGameProcess(starter_relative_path_.filename()))
	{
		assert(false);
		return false;
	}
	return true;
}

bool WxLuaApp_Console::PreResolveGameFiles()
{
	if (game_exe_name_.empty())
	{
		if (game_path_.empty())
		{
			assert(false);
			return false;
		}
		game_exe_name_ = game_path_;
		game_exe_name_ = game_exe_name_.filename();
		if (game_exe_name_.empty())
		{
			assert(false);
			return false;
		}
		AddGameFileOverwrite({ kSFM_AtGamePathRoot, L"d3d9.dll", kFM_FindChild },
		{ kSFM_AtSysAsset, L"d3d9_profile.dll", kFM_FindChild });
		AddD3d9Cfg(kDM_Profile, { kSFM_AtGamePathRoot, game_exe_name_, kFM_FindChild },
		{ kSFM_AtSystem, GetGameDllName(), kFM_FindChild });		//Game.dll, gl->GameLogic.
	}
	return true;
}

boost::filesystem::path WxLuaApp_Console::GetStarterRalativePath() const
{
	assert(false);
	return boost::filesystem::path();
}

ConsoleAppUiOperMain* ConsoleAppFactory::CreateAppUiOper(WxLuaApp_Console& app)
{
	return new ConsoleAppUiOperMain(app);
}

ConsoleAppUiOperIo* ConsoleAppFactory::CreateIoUiOper(Cmn_ThrdIo_Console& io_thrd)
{
	return new ConsoleAppUiOperIo(io_thrd);
}

LuaThrdData* ConsoleAppFactory::CreateIoThrdLuaVm()
{
	return new LuaThrdData;
}

Cmn_ThrdIo_Console* ConsoleAppFactory::CreateIoThrd()
{
	return new Cmn_ThrdIo_Console;
}

ConsoleSessionS* ConsoleAppFactory::CreateGameSession(ConsoleServer& server)
{
	return new ConsoleSessionS(server);
}

ConsoleLoginSessionS* ConsoleAppFactory::CreateLoginSession(ConsoleLoginServer& server)
{
	return new ConsoleLoginSessionS(server);
}

ConsoleSessionOper* ConsoleAppFactory::CreateGameSessionOper(ConsoleSessionS& session)
{
	return new ConsoleSessionOper(session);
}

ConsoleLoginSessionOper* ConsoleAppFactory::CreateLoginSessionOper(ConsoleLoginSessionS& session)
{
	return new ConsoleLoginSessionOper(session);
}

CmnConsoleLoginInfoMgrBase* ConsoleAppFactory::CreateLoginInfoMgr()
{
	//MaxCnt最好跟其他数字不一样，以防破解者更改内存
	return new CmnConsoleLoginInfoMgr<101, 0, 100>;
}

CmnConsoleCS_Connection* ConsoleAppFactory::CreateCheckServerConnection(const std::string& svr_ip)
{
	return new CmnConsoleCS_Connection(svr_ip);
}

CmnConsoleScriptEcho* ConsoleAppFactory::CreateScriptEcho(ConsoleSessionS& session)
{
	return new CmnConsoleScriptEcho(session);
}

LuaCfgParser* ConsoleAppFactory::CreateLuaCfgParser(ConsoleSessionS& session)
{
	return new LuaCfgParser;
}

Cmn_LuaCfgResolver* ConsoleAppFactory::CreateLuaCfgResolverImpl_(Cmn_LuaCfgResolver::enTag tag)
{
	auto& app = WxLuaApp_Console::GetApp();
	auto& cfg = app.GetCfgDir();
	if (!cfg)
	{
		assert(false);
		return nullptr;
	}
	auto lua_vm = app.GetLuaVm();
	if (!lua_vm)
	{
		assert(false);
		return nullptr;
	}
	return new Cmn_LuaCfgResolver(cfg, lua_vm->lua(), tag);
}

Cmn_LuaCfgResolver* ConsoleAppFactory::CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag){
	return CreateLuaCfgResolverImpl_(
#ifdef _DEBUG
		Cmn_LuaCfgResolver::kNormal
#else
		Cmn_LuaCfgResolver::kEncrypt
#endif
		);
}

ConsoleAcCfgMgr::stAcItemInfo* ConsoleAppFactory::CreateAcCfgItemInfo()
{
	return new ConsoleAcCfgMgr::stAcItemInfo;
}

ConsoleLoginMgr* ConsoleAppFactory::CreateConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper)
{
	return new ConsoleLoginMgr(ui_oper);
}

AppExecEnvBase* ConsoleAppFactory::CreateExecEnv()
{
	return new AppExecEnvBase;
}

void CmnConsoleLoginInfoMgrBase::DoCloseTheSession(const BindAcInfoT& the_clr_ac)
{
	if (the_clr_ac.empty())
		return;
	auto& app = WxLuaApp_Console::GetApp();
	auto io = app.GetIoThrd_();
	if (!io)
	{
		assert(false);
		return;
	}
	io->GetIoService().post([the_clr_ac, io, &app](){
		auto mgr = io->GetLoginInfoMgr();
		if (!mgr)
		{
			assert(false);
			return;
		}
		auto server = io->GetConsoleServer();
		if (!server)
		{
			assert(false);
			return;
		}
		for (auto& v : the_clr_ac)
		{
			const auto& the_key = app.GenAcItemKey(v.first, kEmptyStr, v.second);
			auto session = server->FindSessionByKey(the_key);
			if (session && session->ObjValid())
			{
				session->Disconnect();
			}
		}
		mgr->RectifyAcceptMaxCnt();
	});
}

pt_dword CmnConsoleLoginInfoMgrBase::GetMaxConnectedCntByAcType(pt_dword ac_type) const
{
	return WxLuaApp_Console::GetApp().GetMaxConnectedCntByAcType(ac_type);
}

LuaCfgParser& ConsoleSessionS::GetLuaCfgParser()
{
	if (cfg_parser_)
		return *cfg_parser_;
	cfg_parser_ = GetApp().GetAppFactory().CreateLuaCfgParser(*this);
	assert(cfg_parser_);
	return *cfg_parser_;
}

ConsoleLoginSessionS::ConsoleLoginSessionS(Server& server) : CmnConsoleSession(server)
{
	state_at_disconnect_ = "stateWaitLogined";
}

ConsoleLoginSessionOper* ConsoleLoginSessionS::CreateSessionOper()
{
	return WxLuaApp_Console::GetApp().GetAppFactory().CreateLoginSessionOper(*this);
}

void ConsoleLoginSessionS::OnDisconnect(IoConnectBase* connect)
{
	__super::OnDisconnect(connect);
	if (key_.empty())
		return;
	OnS_LoginDisconnect();
}

bool ConsoleLoginSessionS::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	if (!session_oper_)
	{
		assert(false);
		return false;
	}
	auto& app = GetApp();
	if (!session_oper_->ToCall(BC_MakeP("B22B5A47CE8C0") << app.version_login_dll_))
		return false;
	auto& crack_catpcha = app.crack_captcha_info_;
	if (crack_catpcha.SameMode(kCCMode_DaMaTu))
	{
		auto& ac = crack_catpcha.GetDmtAc();
		if (!ac.empty())
			session_oper_->ToCall(BC_MakeP("SetDmtCrackCaptcha") << ac << crack_catpcha.GetDmtPwd());
	}
	else if (crack_catpcha.SameMode(kCCMode_Local))
	{
		auto& ip = crack_catpcha.GetLocalIp();
		if (!ip.empty())
			session_oper_->ToCall(BC_MakeP("SetLocalCrackCaptcha") << ip << crack_catpcha.GetLocalPort());
	}
	if (!SendSomethingAtConnected())
	{
		assert(false);
		return false;
	}
	session_oper_->ToCall(BC_MakeP("BeginLogin"));
	return true;
}

void ConsoleLoginSessionS::OnS_LoginDisconnect()
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_LoginDisconnect", key_, state_at_disconnect_));
}

bool ConsoleLoginSessionS::SendSomethingAtConnected()
{
	return true;
}

ConsoleSessionS::ConsoleSessionS(Server& server) : CmnConsoleSession(server)
{
	state_at_disconnect_ = "stateNil";
	is_reconnect_at_disconnect_ = true;
	is_reconnect_to_first_ = false;
	script_echo_ = nullptr;
	cfg_parser_ = nullptr;
}

ConsoleSessionOper* ConsoleSessionS::CreateSessionOper()
{
	return WxLuaApp_Console::GetApp().GetAppFactory().CreateGameSessionOper(*this);
}

bool ConsoleSessionS::OnConnected(IoConnectBase* connect)
{
	if (lua_cfg_info_.lua_cfg_serialize_.empty())
	{
		assert(false);
		return false;
	}
	auto mgr = CmnLoginInfoMgrBase::GetInstance();
	assert(mgr);
	auto login_mgr = mgr->GetDerived<CmnLoginInfoMgrBase>();
	assert(login_mgr);
	auto ac_pwd = login_mgr->FindFreeOne();
	if (!ac_pwd)
	{
		assert(false);
		return true;
	}
	const DateTimeLocal& time_now = G_GetSerCurTime();
	if (ac_pwd->TestTimeEnd(time_now))
	{
		assert(false);
		return false;
	}
	if (!__super::OnConnected(connect))
		return false;
	if (!ac_pwd->AddBindGameAcInfo(ac_info_.ac_, ac_info_.area_ + ac_info_.ser_))
	{
		assert(false);
		return false;
	}
	if (!ac_pwd->IsTheLoginSucceed())
	{
		assert(false);
		return false;
	}
	auto& the_end_date_time = ac_pwd->GetUseEndDayTime();
	if (!the_end_date_time)
	{
		assert(false);
		return false;
	}

	assert(session_oper_);
	{
		const auto& end_date_time = *the_end_date_time;
		const auto& addr_datas = ac_pwd->GetAddrDatas();
		const auto& str_datas = ac_pwd->GetStrDatas();
		auto& app = GetApp();
		if (!session_oper_->ToCall(BC_MakeP("B22B5A47CE8C0") << end_date_time << addr_datas << str_datas << app.version_game_dll_))
			return false;
	}
	if (!session_oper_->ToCall(BC_MakeP("OnSetLuaCfgStr_") << lua_cfg_info_.lua_cfg_serialize_))
		return false;
	auto& app = WxLuaApp_Console::GetApp();
	if (!app.GetAcOperSinks().empty())
	{
		auto item_key = key_;
		app.GetIoService().post([item_key, this](){
			for (auto sink : WxLuaApp_Console::GetApp().GetAcOperSinks())
			{
				sink->OnAcConnected(this, item_key);
			}
		});
	}
	auto ac_bind_info = ac_pwd->GetBindGameAcInfo();
	if (ac_bind_info.size() > mgr->GetMaxConnectedCntByAcType(app.ac_type_))
	{
		assert(false);
		return false;
	}
	return true;
}

void ConsoleSessionS::OnDisconnect(IoConnectBase* connect)
{
	if (timer_login_succeed_)
	{
		timer_login_succeed_->Cancel();
		timer_login_succeed_.reset();
	}
	__super::OnDisconnect(connect);
	auto& io = GetIoThrd();
	auto mgr = io.GetLoginInfoMgr();
	if (!mgr)
	{
		assert(false);
		return;
	}
	auto ser_mix = ac_info_.area_ + ac_info_.ser_;
	auto ac_pwd = mgr->FindByAc_Ser(ac_info_.ac_, ser_mix);
	if (ac_pwd)
	{
		if (!ac_pwd->DeleteTheBindGameAcInfo(ac_info_.ac_, ser_mix))
		{
			assert(false);
		}
		ac_pwd = nullptr;
	}
	{
		luabind::object().swap(lua_cfg_info_.lua_cfg_);
	}
	if (key_.empty())
		return;
	assert(!state_at_disconnect_.empty());
	OnS_GameDisconnect();
	auto io_oper = GetIoUiOper();
	if (!io_oper)
	{
		assert(false);
		return;
	}
	if (is_reconnect_at_disconnect_)
	{
		WxLuaApp_Console::LoginByItemKey(key_, is_reconnect_to_first_);
		auto& app = GetApp();
		app.GetIoService().post([&app, io_oper](){
			auto& io = app.GetIoThrd();
			if (io)
			{
				io->GetIoService().post([io_oper](){
					io_oper->Login(kEmptyStr, kEmptyStr, false);
				});
			}
		});
	}
	else
	{
		auto& thrd = GetIoThrd();
		thrd.GetIoService().post([io_oper](){
			io_oper->Login(kEmptyStr, kEmptyStr, false);
		});
	}
	auto& app = WxLuaApp_Console::GetApp();
	if (!app.GetAcOperSinks().empty())
	{
		auto item_key = key_;
		app.GetIoService().post([item_key](){
			for (auto sink : WxLuaApp_Console::GetApp().GetAcOperSinks())
			{
				sink->OnAcDisconnect(item_key);
			}
		});
	}
}

ConsoleSessionS::~ConsoleSessionS()
{
	delete script_echo_;
	script_echo_ = nullptr;
	delete cfg_parser_;
	cfg_parser_ = nullptr;
	if (timer_login_succeed_)
	{
		timer_login_succeed_->Cancel();
		timer_login_succeed_.reset();
	}
}

CmnConsoleScriptEcho* ConsoleSessionS::GetScriptEcho() const
{
	assert(script_echo_);
	return script_echo_;
}

bool ConsoleSessionS::InitOnce()
{
	if (!__super::InitOnce())
		return false;
	assert(!script_echo_);
	auto& app = WxLuaApp_Console::GetApp();
	script_echo_ = app.GetAppFactory().CreateScriptEcho(*this);
	if (!script_echo_)
	{
		assert(false);
		return false;
	}
	assert(!timer_login_succeed_);
	auto shared_this = shared_from_this();
	timer_login_succeed_ = CreateIoTimer(app.time_game_login_succeed_, true, [shared_this](const IoTimerPtr&){
		LOG_O(Log_debug) << "登陆未收到成功，判定登陆超时";
		CMN_ASSERT(false);
		if (shared_this)
			shared_this->DoDisconnect();
	});
	return true;
}

ConsoleSessionOper* ConsoleSessionS::GetSessionOper() const
{
	assert(session_oper_);
	return static_cast<ConsoleSessionOper*>(session_oper_);
}

const ConsoleSessionS::stLuaCfgInfo& ConsoleSessionS::GetLuaCfgInfo() const
{
	return lua_cfg_info_;
}

bool ConsoleSessionS::ForegroundTheWindow()
{
	if (!session_oper_)
	{
		assert(false);
		return false;
	}
	return session_oper_->ToCall(BC_MakeP("ForegroundTheWindow"));
}

void ConsoleSessionS::OnS_GameDisconnect()
{
	assert(!state_at_disconnect_.empty());
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_GameDisconnect", key_, state_at_disconnect_));
}

void ConsoleSessionS::OnModifyConsoleCfg(pt_csz key, pt_csz value)
{
	auto& my_key = GetKey();
	ModifyConsoleCfgImpl(my_key, key, value);
}

void ConsoleSessionS::ModifyConsoleCfgImpl(const std::string& item_key, pt_csz key, pt_csz value)
{
	assert(key);
	assert(value);
	auto run_str = LuaSerialize::ToLuaStringCall("mainfrm:OnS_GetItemData", item_key);
	run_str += '.';
	run_str += key;
	run_str += '=';
	run_str += value;
	run_str += '\n';
	run_str += LuaSerialize::ToLuaStringCall("mainfrm:OnS_UpdateItem", item_key);
	//LOG_O(Log_trace) << "MidifyConsoleCfg,run_str:" << run_str;
	Cmn_App::ShellM(run_str);
}

ConsoleLoginSessionS* ConsoleLoginServer::CreateSession()
{
	return WxLuaApp_Console::GetApp().GetAppFactory().CreateLoginSession(*this);
}

bool ConsoleLoginServer::OnConnected(IoConnectBase* connect)
{
	return __super::OnConnected(connect);
}

void ConsoleLoginServer::OnDisconnect(IoConnectBase* connect)
{
	return __super::OnDisconnect(connect);
}

ConsoleLoginServer::ConsoleLoginServer(pt_word host_port) : Server(host_port)
{

}

ConsoleServer::ConsoleServer(pt_word host_port) : Server(host_port)
{

}

ConsoleSessionS* ConsoleServer::CreateSession()
{
	return WxLuaApp_Console::GetApp().GetAppFactory().CreateGameSession(*this);
}

bool ConsoleServer::OnConnected(IoConnectBase* connect)
{
	return __super::OnConnected(connect);
}

void ConsoleServer::OnDisconnect(IoConnectBase* connect)
{
	return __super::OnDisconnect(connect);
}

void CmnConsoleSession::SetLoginProcessInfo(const std::string& process_info)
{
	ac_info_.login_process_info_ = process_info;
}

CmnConsoleSession::CmnConsoleSession(Server& server) : Session(server)
{
	session_oper_ = nullptr;
}

bool CmnConsoleSession::InitOnce()
{
	if (!__super::InitOnce())
		return false;
	auto& app = WxLuaApp_Console::GetAppBase().CastAs<WxLuaApp_Console>();
	auto& iot = app.GetIoThrd();
	if (!iot)
	{
		assert(false);
		return false;
	}
	auto& thrd_io = iot->CastAs<Cmn_ThrdIo_Console>();
	auto login_mgr = thrd_io.GetLoginInfoMgr();
	if (!login_mgr)
	{
		assert(false);
		return false;
	}
	if (!login_mgr->FindFreeOne())
		return false;
	session_oper_ = CreateSessionOper();
	if (!session_oper_)
	{
		assert(false);
		return false;
	}
	return true;
}

CmnConsoleSession::~CmnConsoleSession()
{
	delete session_oper_;
}

bool CmnConsoleSession::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	if (key_.empty())
	{
		assert(false);
		return false;
	}
	if (!session_oper_)
	{
		assert(false);
		return false;
	}
	if (ac_info_.ac_.empty())
	{
		assert(false);
		return false;
	}
	session_oper_->ToCall(BC_MakeP("SetAcInfo_") << ac_info_);
	return true;
}

bool CmnConsoleSession::ConnectDone()
{
	if (!session_oper_)
	{
		assert(false);
		return false;
	}
	if (!__super::ConnectDone())
		return false;
	if (!session_oper_->ToCall(BC_MakeP("OnConnectDone_") << true))
		return false;
	return true;
}

CmnConsoleSessionOper* CmnConsoleSession::GetSessionOper() const
{
	return session_oper_;
}

CmnConsoleSessionOper::CmnConsoleSessionOper(CmnConsoleSession& session)
	: BufCall_SocketExcept(&session, kFuncObjTemplateId_GameOrLogin), session_(session)
{

}

bool CmnConsoleSessionOper::OnAttachAcInfo_(long pid, const std::string& ac_item_key)
{
	auto& the_key = session_.GetKey();
	if (!the_key.empty())
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	session_.SetKey(ac_item_key);
	if (the_key.empty())
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	auto& login_mgr = session_.GetConsoleLoginMgr();
	auto ac_info = login_mgr.FindAcItemInfo(the_key);
	if (!ac_info)
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	static_cast<stAcSerInfo&>(session_.ac_info_) = *ac_info;
	if (session_.ac_info_.login_process_info_.empty())
	{
		LOG_O(Log_debug) << "登陆进程信息竟然为空，怀疑是没有经过正常的登陆流程连接到控制台的";
	}
	return DoExecConnectDone();
}

bool CmnConsoleSessionOper::DoExecConnectDone()
{
	if (!session_.ConnectDone())
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	return true;
}

CmnConsoleSession& CmnConsoleSessionOper::GetSession() const
{
	return session_;
}

WxLuaApp_Console& CmnConsoleSession::GetApp()
{
	return WxLuaApp_Console::GetAppBase().CastAs<WxLuaApp_Console>();
}

Cmn_ThrdIo_Console& CmnConsoleSession::GetIoThrd()
{
	auto& io = GetApp().GetIoThrd();
	assert(io);
	return io->CastAs<Cmn_ThrdIo_Console>();
}

ConsoleAppUiOperMain* CmnConsoleSession::GetAppUiOper()
{
	return GetApp().GetAppUiOper();
}

ConsoleAppUiOperIo* CmnConsoleSession::GetIoUiOper()
{
	return GetIoThrd().GetAppUiOper();
}

ConsoleLoginMgr& CmnConsoleSession::GetConsoleLoginMgr()
{
	auto io = GetIoUiOper();
	assert(io);
	return io->GetLoginMgr();
}

bool CmnConsoleSession::DoConnectDone()
{
	auto& io = GetIoThrd();
	auto mgr = io.GetLoginInfoMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	if (!mgr->FindFreeOne())
	{
		assert(false);
		auto shared = shared_from_this();
		io.GetIoService().post([shared](){
			if (shared->ObjValid())
				shared->DoDisconnect();
		});
		return true;
	}
	return true;
}

const stAsSerSessionInfo& CmnConsoleSession::GetAcInfo() const
{
	return ac_info_;
}

void CmnConsoleSession::SetStateAtDisconnect(const char* final_state)
{
	assert(final_state);
	state_at_disconnect_ = final_state;
}

ConsoleSessionOper::ConsoleSessionOper(ConsoleSessionS& session) : CmnConsoleSessionOper(session)
{
	is_done_connection_done_ = false;
	connection_done_valid_cnt_ = 0;
	first_set_lua_serialize_ = true;
}

bool ConsoleSessionOper::OnAttachAcInfo_(long pid, const std::string& ac_item_key)
{
	auto& mgr = session_.GetConsoleLoginMgr();
	if (!mgr.AttachSessionGame(pid, ac_item_key, session_))
	{
		assert(false);
		return false;
	}
	OnS_AttachGameSession(ac_item_key);
	return __super::OnAttachAcInfo_(pid, ac_item_key);
}

bool ConsoleSessionOper::DoExecConnectDone()
{
	if (is_done_connection_done_)
		return true;
	++connection_done_valid_cnt_;
	if (connection_done_valid_cnt_ < 2)
		return true;
	is_done_connection_done_ = true;
	return __super::DoExecConnectDone();
}

void ConsoleSessionOper::SetLuaSerialize(const std::string& lua_str)
{
	assert(!lua_str.empty());
	using namespace luabind;
	auto& session = GetSession();
	session.lua_cfg_info_.lua_cfg_serialize_ = lua_str;
	object empty_tbl = newtable(LuaThrdData::GetMe().lua());
	session.lua_cfg_info_.lua_cfg_ = LuaSerialize::PlutoUnpersist(empty_tbl, lua_str);
	if (!session.lua_cfg_info_.lua_cfg_.is_valid())
	{
		assert(false);
		session.Disconnect();
		return;
	}
	auto& parser = session.GetLuaCfgParser();
	try{
		if (!parser.ParseLuaCfg(session.lua_cfg_info_.lua_cfg_))
		{
			assert(false);
			session.Disconnect();
			return;
		}
	}
	catch (...){
		assert(false);
		session.Disconnect();
		throw;
	}

	if (first_set_lua_serialize_)
	{
		first_set_lua_serialize_ = false;
		DoExecConnectDone();
	}
}

ConsoleSessionS& ConsoleSessionOper::GetSession() const
{
	return static_cast<ConsoleSessionS&>(session_);
}

void ConsoleSessionOper::GameLoginSucceed()
{
	auto& session = GetSession();
	CMN_ASSERT(session.timer_login_succeed_ && session.timer_login_succeed_->IsClocking());
	if (session.timer_login_succeed_)
	{
		session.timer_login_succeed_->Cancel();
		session.timer_login_succeed_.reset();
	}
	auto& app = WxLuaApp_Console::GetApp();
	if (app.glo_auto_run_script_)
	{
		auto& parser = session.GetLuaCfgParser();
		if (parser.auto_run_)
		{
			if (!WxLuaApp_Console::RunScript(session.GetKey()))
			{
				assert(false);
				return;
			}
		}
	}

	auto io_oper = session_.GetIoUiOper();
	if (!io_oper)
	{
		assert(false);
		return;
	}
	io_oper->Login(kEmptyStr, kEmptyStr, false);
}

void ConsoleSessionOper::OnS_AttachGameSession(const std::string& item_key)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_AttachGameSession", item_key));
}

void ConsoleSessionOper::OnModifyConsoleCfg_(pt_csz key, pt_csz value)
{
	if (!key)
	{
		assert(false);
		return;
	}
	if (value)
	{
		if (0 == *value)
			value = nullptr;
	}
	value = value ? value : "nil";
	GetSession().OnModifyConsoleCfg(key, value);
}

ConsoleLoginSessionOper::ConsoleLoginSessionOper(ConsoleLoginSessionS& session) : CmnConsoleSessionOper(session)
{

}

bool ConsoleLoginSessionOper::OnAttachAcInfo_(long pid, const std::string& ac_item_key)
{
	auto& mgr = session_.GetConsoleLoginMgr();
	if (!mgr.AttachSessionLogin(pid, ac_item_key, session_))
	{
		assert(false);
		return false;
	}
	OnS_AttachLoginSession(ac_item_key);
	return __super::OnAttachAcInfo_(pid, ac_item_key);
}

void ConsoleLoginSessionOper::OnS_AttachLoginSession(const std::string& item_key)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_AttachLoginSession", item_key));
}

void ConsoleLoginSessionOper::OnAcPwdErr_()
{
	__super::OnAcPwdErr_();
	auto item_key = session_.GetKey();
	auto ac_info = session_.GetAcInfo();
	WxLuaApp_Console::GetApp().GetIoService().post([item_key, ac_info](){
		WxLuaApp_Console::GetApp().OnLoginFailed(item_key, ac_info);
	});
}

void ConsoleLoginSessionOper::OnBacAc_()
{
	__super::OnBacAc_();
	auto item_key = session_.GetKey();
	auto ac_info = session_.GetAcInfo();
	WxLuaApp_Console::GetApp().GetIoService().post([item_key, ac_info](){
		WxLuaApp_Console::GetApp().OnLoginFailed(item_key, ac_info);
	});
}

void ConsoleLoginSessionOper::OnFreeze_()
{
	__super::OnFreeze_();
	auto item_key = session_.GetKey();
	auto ac_info = session_.GetAcInfo();
	WxLuaApp_Console::GetApp().GetIoService().post([item_key, ac_info](){
		WxLuaApp_Console::GetApp().OnLoginFailed(item_key, ac_info);
	});
}

void ConsoleLoginSessionOper::OnRelogin_()
{
	session_.Disconnect();
	auto item_key = session_.GetKey();
	WxLuaApp_Console::GetApp().ReloginAc(item_key);
}

void CmnConsoleSessionOper::OnAcPwdErr_()
{
	session_.SetStateAtDisconnect("stateAcPwdErr");
	session_.Disconnect();
}

void CmnConsoleSessionOper::OnBacAc_()
{
	session_.SetStateAtDisconnect("stateBan");
	session_.Disconnect();
}

void CmnConsoleSessionOper::OnFreeze_()
{
	session_.SetStateAtDisconnect("stateFreeze");
	session_.Disconnect();
}

void CmnConsoleSessionOper::SetStateAtDisconnect_(const char* final_state)
{
	if (final_state)
	{
		if (*final_state == 0)
			final_state = nullptr;
	}
	final_state = final_state ? final_state : "stateNil";
	session_.SetStateAtDisconnect(final_state);
}

CmnConsoleCS_Connection::CmnConsoleCS_Connection(const std::string& check_server_ip) : CmnDoLogin(check_server_ip)
{

}

bool CmnConsoleCS_Connection::OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix,
	pt_csz new_cookie, const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, 
	const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	if (res)
	{
		auto& app = WxLuaApp_Console::GetApp();
		if (app.end_date_time_.empty())
		{
			DateTimeLocal end_time(Poco::Timestamp::fromEpochTime(end_day_time));
			auto str_end_time = end_time.FormatString();
			app.GetIoService().post([&app, str_end_time](){
				if (app.end_date_time_.empty())
				{
					app.end_date_time_ = str_end_time;
				}
				else
				{
					assert(false);
				}
			});
		}
		app.ac_type_ = ac_type;
	}
	return __super::OnLoginRes(res, info, ac_pwd_machine_mix, new_cookie, end_day_time, ser_cur_time, addr_datas, str_addr_datas, ac_type);
}

CmnConsoleScriptEcho::CmnConsoleScriptEcho(ConsoleSessionS& parent) : LuasScriptEchoS(parent), session_(parent)
{

}

bool CmnConsoleScriptEcho::OutputScriptInfo(const std::string& script_info)
{
	WxLuaApp_Console::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_OutputScriptInfo", session_.GetKey(), script_info));
	return true;
}

bool CmnConsoleScriptEcho::OperResult(enOperResult result)
{
	bool is_start_script = kBeginRun == result;
	WxLuaApp_Console::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_ScriptState", session_.GetKey(), is_start_script));
	return true;
}

WxLuaApp_Console::stCrackCaptchaInfo::stCrackCaptchaInfo()
{
	port_ = 0;
}

void WxLuaApp_Console::stCrackCaptchaInfo::SetDmt(const std::string& ac, const std::string& pwd)
{
	mode_ = kCCMode_DaMaTu;
	str_buffer_ = ac;
	str_buffer1_ = pwd;
}

void WxLuaApp_Console::stCrackCaptchaInfo::SetLocal(const std::string& ip, pt_word port)
{
	mode_ = kCCMode_Local;
	str_buffer_ = ip;
	port_ = port;
}

bool WxLuaApp_Console::stCrackCaptchaInfo::SameMode(enCrackCaptchaMode mode) const
{
	return mode_ == mode;
}

const std::string& WxLuaApp_Console::stCrackCaptchaInfo::GetDmtAc() const
{
	if (mode_ != kCCMode_DaMaTu)
	{
		assert(false);
		return kEmptyStr;
	}
	return str_buffer_;
}

const std::string& WxLuaApp_Console::stCrackCaptchaInfo::GetDmtPwd() const
{
	if (mode_ != kCCMode_DaMaTu)
	{
		assert(false);
		return kEmptyStr;
	}
	return str_buffer1_;
}

const std::string WxLuaApp_Console::stCrackCaptchaInfo::GetLocalIp() const
{
	if (mode_ != kCCMode_Local)
	{
		assert(false);
		return kEmptyStr;
	}
	return str_buffer_;
}

pt_word WxLuaApp_Console::stCrackCaptchaInfo::GetLocalPort() const
{
	if (mode_ != kCCMode_Local)
	{
		assert(false);
		return 0;
	}
	return port_;
}

void WxLuaApp_Console::AddAreaSer2Id(const std::string& area_name, const std::string& ser_name, pt_dword ser_id)
{
	if (ser_name.empty())
	{
		assert(false);
		return;
	}
	if (0 == ser_id)
	{
		assert(false);
		return;
	}
	for (auto& v : ser2id_cont_)
	{
		if (v.area_name_ == area_name && v.ser_name_ == ser_name)
		{
			assert(false);
			return;
		}
		if (v.ser_id_ == ser_id)
		{
			assert(false);
			return;
		}
	}
	ser2id_cont_.push_back({ area_name, ser_name, ser_id });
}

void WxLuaApp_Console::ParseAreaSer2Id(const luabind::object& tbl)
{
	using namespace luabind;
	if (!tbl.is_valid() || type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		auto area_name = LuaObjCast(it.key(), kEmptyStr);
		object sers = *it;
		if (type(sers) != LUA_TTABLE)
		{
			assert(false);
			continue;
		}
		for (iterator it_ser(sers), it_ser_end; it_ser != it_ser_end; ++it_ser)
		{
			AddAreaSer2Id(area_name, LuaObjCast(it_ser.key(), kEmptyStr),
				LuaObjCast(*it_ser, pt_dword(0)));
		}
	}
}

boost::optional<pt_dword> WxLuaApp_Console::FindSerId(const std::string& area_name, const std::string& ser_name) const
{
	boost::optional<pt_dword> res;
	for (auto& v : ser2id_cont_)
	{
		if (v.area_name_ == area_name && v.ser_name_ == ser_name)
		{
			res = v.ser_id_;
			return res;
		}
	}
	assert(false);
	return res;
}

void WxLuaApp_Console::SetStarterProcessCommand(const std::string& command)
{
	starter_process_command_ = command;
}

const std::string& WxLuaApp_Console::GetStarterProcessCommand() const
{
	return starter_process_command_;
}

WxLuaApp_Console::AcOperSinkCont& WxLuaApp_Console::GetAcOperSinks()
{
	return ac_oper_sinks_;
}

void WxLuaApp_Console::AddAcOperSink(AcOperSink* sink)
{
	assert(sink);
	ac_oper_sinks_.push_back(sink);
}

void WxLuaApp_Console::OnUpdateAcItem(int ac_item_idx, const std::string& item_key, const luabind::object& item_data)
{
	for (auto sink : ac_oper_sinks_)
	{
		sink->OnUpdateAcItemData(ac_item_idx, item_key, item_data);
	}
}

void WxLuaApp_Console::ModifyAcItemData(const std::string& item_key, const std::string& key, const std::string& value)
{
	ConsoleSessionS::ModifyConsoleCfgImpl(item_key, key.c_str(), value.c_str());
}

void WxLuaApp_Console::OnLuaAddAc(const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser)
{
	stAcSerInfo ac_info;
	ac_info.ac_ = ac;
	ac_info.pwd_ = pwd;
	ac_info.area_ = area;
	ac_info.ser_ = ser;
	for (auto sink : GetAcOperSinks())
	{
		sink->OnAddAc(ac_info);
	}
}

void WxLuaApp_Console::OnLuaDeleteAc(const std::string& item_key)
{
	for (auto sink : GetAcOperSinks())
	{
		sink->OnDeleteAc(item_key);
	}
}

pt_dword WxLuaApp_Console::GetMaxConnectedCntByAcType(pt_dword ac_type) const
{
	return 100;
}

void WxLuaApp_Console::CreateExeAsDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& exe_file_name)
{
	using namespace boost::filesystem;
	auto path_tmp = path_dir;
	path_tmp /= exe_file_name;
	if (!exists(path_tmp))
	{
		assert(false);
		return;
	}
	if (is_directory(path_tmp))
		return;
	auto file_tmp = exe_file_name.stem();
	file_tmp.concat(L"_");
	file_tmp.replace_extension(exe_file_name.extension());
	auto path_tmp1 = path_dir;
	path_tmp1 /= file_tmp;
	boost::system::error_code err;
	if (exists(path_tmp1))
	{
		remove(path_tmp1, err);
		if (err)
		{
			LOG_O(Log_debug) << err.message();
			assert(false);
			return;
		}
	}
	rename(path_tmp, path_tmp1);
	create_directory(path_tmp, err);
	if (err)
	{
		LOG_O(Log_debug) << err.message();
		assert(false);
	}
}

void WxLuaApp_Console::DeleteFileOrDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& file_name)
{
	using namespace boost::filesystem;
	auto path_tmp = path_dir;
	path_tmp /= file_name;
	if (!exists(path_tmp))
		return;
	boost::system::error_code err;
	remove_all(path_tmp, err);
	if (err)
	{
		LOG_O(Log_debug) << err.message();
		assert(false);
	}
}

void WxLuaApp_Console::RenameFileOrDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& file_name)
{
	using namespace boost::filesystem;
	auto path_tmp = path_dir;
	path_tmp /= file_name;
	if (!exists(path_tmp))
		return;
	auto file_name_tmp = file_name.stem();
	file_name_tmp.concat(L"_");
	file_name_tmp.replace_extension(file_name.extension());
	auto path_tmp1 = path_dir;
	path_tmp1 /= file_name_tmp;
	boost::system::error_code err;
	if (exists(path_tmp1))
	{
		remove_all(path_tmp1, err);
		if (err)
		{
			LOG_O(Log_debug) << err.message();
			assert(false);
			return;
		}
	}
	rename(path_tmp, path_tmp1, err);
	if (err)
	{
		LOG_O(Log_debug) << err.message();
		assert(false);
	}
}

void WxLuaApp_Console::ReloginAc(const std::string& item_key)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_CancelLogin", item_key));
	LoginByItemKey(item_key, false);
}

AcOperSink::~AcOperSink()
{

}
