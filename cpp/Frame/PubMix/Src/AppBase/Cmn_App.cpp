#include "stdafx.h"
#include <AppBase/Cmn_App.h>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/filesystem/operations.hpp>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <AssetSys/AS_Sys.h>
#include <AssetSys/AS_Dir.h>
#include <wx/convauto.h>
#include <AppBase/CmnWxThrd.h>
#include <wx/wxprec.h>
//////////////////////////////////////////////////////////////////////////

Cmn_AppBase* ptr_app_base_ = nullptr;
static void _AlwaysSet__(){
	std::locale::global(std::locale("", std::locale::all & ~std::locale::numeric));
}
Cmn_AppBase::Cmn_AppBase(Cmn_ThrdMgrBase::ThrdMgrSink* sink) : Cmn_ThrdMgrBase(sink)
{
	main_thrd_handle_ = nullptr;
	if (ptr_app_base_)
	{
		assert(false);
		return;
	}
	ptr_app_base_ = this;
	//////////////////////////////////////////////////////////////////////////
	_AlwaysSet__();
#ifdef _DEBUG
	is_enable_dump_ = true;
#else
	is_enable_dump_ = false;
#endif
	wxConvAuto::SetFallbackEncoding(wxFONTENCODING_SYSTEM);
	//////////////////////////////////////////////////////////////////////////
	main_thrd_id_ = 0;
	log_file_name_ = nullptr;
	is_log_dbgview_ = true;
	is_log_console_ = false;
	is_add_main_thred_ = true;
	is_poll_ = true;
	app_factory_ = nullptr;
	cfg_name_ = "Cfg";
	//script_dir_name_ = "script_c";
	script_dir_name_ = "脚本";
	asset_dir_name_ = "asset";
	//////////////////////////////////////////////////////////////////////////		
}

Cmn_AppBase::~Cmn_AppBase()
{
	if (!ptr_app_base_)
	{
		assert(false);
		return;
	}
	ptr_app_base_ = nullptr;
	delete app_factory_;
	app_factory_ = nullptr;
	assert(!main_thrd_handle_);
}

Cmn_AppBase& Cmn_AppBase::GetAppBase()
{
	assert(ptr_app_base_);
	return *ptr_app_base_;
}

bool Cmn_AppBase::IsEnableDump() const
{
	return is_enable_dump_;
}

void Cmn_AppBase::SetEnableDump(bool is_enable_dump)
{
	is_enable_dump_ = is_enable_dump;
}

bool Cmn_AppBase::DoRun()
{
	auto res = OnEntry();
	if (res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool Cmn_AppBase::OnInit_()
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
	_AlwaysSet__();
	if (0 == main_thrd_id_)
		main_thrd_id_ = wxThread::GetCurrentId();
	if (main_thrd_handle_ == nullptr)
	{
		main_thrd_handle_ = ::OpenThread(THREAD_ALL_ACCESS, FALSE, main_thrd_id_);
		if (!main_thrd_handle_)
		{
			assert(false);
			return false;
		}
	}
	if (is_add_main_thred_)
		AddMainThrd();
	if (!InitAS())
	{
		assert(false);
		return false;
	}
	if (log_file_name_)
	{
		LogToXmlHelper log_xml(log_file_name_);
		log_xml.ClearHolder();
	}
	if (is_log_dbgview_)
	{
		LogToDbgviewHelper log_dbgview;
		log_dbgview.ClearHolder();
	}
	if (is_log_console_)
	{
		LogToConsoleHelper log_console;
		log_console.ClearHolder();
	}
	if (!__super::OnInit_())
	{
		assert(false);
		return false;
	}
	/*if (lua_cfg_relover_)
	{
		lua_cfg_relover_->Resolve();
	}*/
	return true;
}

int Cmn_AppBase::OnExit_()
{
	auto res = __super::OnExit_();
	KillAll();
	main_thrd_id_ = 0;
	if (lua_cfg_relover_)
	{
		//不需要最后保存，因为每修改个值，都会自动保存一次的，最后退出前保存反而有可能出问题。
		//lua_cfg_relover_->SaveAll();
		lua_cfg_relover_.reset();
	}
	if (main_thrd_handle_)
	{
		CloseHandle(main_thrd_handle_);
		main_thrd_handle_ = nullptr;
	}
	else
		assert(false);
	return res;
}

Cmn_AppBase::ThrdIdT Cmn_AppBase::GetThrdId() const
{
	assert(main_thrd_id_ != 0);
	return main_thrd_id_;
}

bool Cmn_AppBase::IsThrdAlive() const
{
	return main_thrd_id_ != 0;
}

void Cmn_AppBase::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			def("Shell", &Cmn_AppBase::__Shell_c__),
			def("Shell", &Cmn_AppBase::__Shell_cc__),
			def("Shell", &Cmn_AppBase::__Shell_cb__),
			def("Shell", &Cmn_AppBase::__Shell_ccb__)
		];

	class_<Cmn_AppBase> bind_app("Cmn_AppBase");

	lua_cfg_relover_.reset(app_factory_->CreateLuaCfgResolver(Cmn_LuaCfgResolver::kEncrypt));
	if (lua_cfg_relover_)
	{
		Cmn_LuaCfgResolver::RegForLua(l);
		bind_app.def("GetLuaCfgResolver", &Cmn_AppBase::GetLuaCfgResolver);
	}
	module(l)
		[
			bind_app
			.def("GetScriptDirName", &Cmn_AppBase::GetScriptDirName)
			.def("IsDebug", &Cmn_AppBase::IsDebug)
		];
}

luabind::object Cmn_AppBase::Shell(const char* lua_cmd, const char* cmd_name, bool sync)
{
	return ShellIo(lua_cmd, cmd_name, sync);
}

bool Cmn_AppBase::InitAS()
{
	auto self_module = wxGetInstance();
	if (!self_module)
	{
		assert(false);
		return false;
	}
	const auto& self_module_name = wxGetFullModuleName(self_module);
	boost::filesystem::path module_path(self_module_name.c_str());
	self_module_path_ = module_path;
	self_module_path_.remove_filename();
	//ASset Sys File->assf
	module_path.replace_extension("assf");
	bool is_file_exist = boost::filesystem::exists(module_path);
	if (!is_file_exist)
	{
		module_path = self_module_path_ / "../" / module_path.stem() / "as.assf";
		is_file_exist = boost::filesystem::exists(module_path);
	}
	if (is_file_exist)
	{
		std::ifstream istm(module_path.string(), std::ios_base::in | std::ios_base::binary);
		if (!istm)
		{
			assert(false);
			return false;
		}
		AS_Sys::GetMe().SetMe(istm, self_module_path_.string());
	}
	else
	{
		const auto& res_data = LoadRcdataRes(self_module, wxT("assf"));
		if (!res_data.first || res_data.second <= 0)
		{
			assert(false);
			return false;
		}
		using namespace std;
		std::stringstream stm(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		stm.write((const char*)res_data.first, res_data.second);
		AS_Sys::GetMe().SetMe(stm, self_module_path_.string());
	}
	return true;
}

const char* Cmn_AppBase::GetScriptDirName() const
{
	assert(script_dir_name_);
	return script_dir_name_;
}

void Cmn_AppBase::OnIoService()
{
	OnEntry();
}

void Cmn_AppBase::ActiveIoService()
{

}

AS_NodePtr Cmn_AppBase::GetScriptDir() const
{
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	node = node->FindChild(GetScriptDirName());
	assert(node);
	return node;
}

luabind::object Cmn_AppBase::__Shell_c__(const char* lua_cmd)
{
	return Shell(lua_cmd);
}

luabind::object Cmn_AppBase::__Shell_cc__(const char* lua_cmd, const char* cmd_name)
{
	return Shell(lua_cmd, cmd_name);
}

luabind::object Cmn_AppBase::__Shell_cb__(const char* lua_cmd, bool async)
{
	return Shell(lua_cmd, nullptr, async);
}

luabind::object Cmn_AppBase::__Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool async)
{
	return Shell(lua_cmd, cmd_name, async);
}

AS_NodePtr Cmn_AppBase::GetCfgDir() const
{
	if (!cfg_name_)
	{
		assert(false);
		return nullptr;
	}
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	auto sys_dir = node->GetSysDir();
	if (!sys_dir)
	{
		assert(false);
		return nullptr;
	}
	node = sys_dir->FindChild(cfg_name_);
	if (node)
		return node;
	node.reset(new AS_DiskDir);
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	node->SetPath(cfg_name_);
	if (!sys_dir->Add(node))
	{
		assert(false);
		return nullptr;
	}
	return node;
}

Cmn_LuaCfgResolver* Cmn_AppBaseFactory::CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag)
{
	return nullptr;
}

Cmn_LuaCfgResolver* Cmn_AppBase::GetLuaCfgResolver() const
{
	return lua_cfg_relover_.get();
}

void Cmn_AppBase::AddMainThrd()
{
	if (0 == main_thrd_id_)
		main_thrd_id_ = wxThread::GetCurrentId();
	ThreadablePtr auto_pointer(this, [](Cmn_AppBase*){
		//do not delete
	});
	AddThrd(auto_pointer, false, false);
}

Cmn_AppBaseFactory* Cmn_AppBase::CreateAppFactory()
{
	return new Cmn_AppBaseFactory;
}

Cmn_AppBaseFactory& Cmn_AppBase::GetAppFactory()
{
	auto factory = Cmn_AppBase::GetAppBase().app_factory_;
	assert(factory);
	return *factory;
}

LuaThrdData* Cmn_AppBase::CreateLuaVm()
{
	return GetAppFactory().CreateAppLuaVm(nullptr);
}

const boost::filesystem::path& Cmn_AppBase::GetSelfModulePath() const
{
	assert(!self_module_path_.empty());
	return self_module_path_;
}

AS_NodePtr Cmn_AppBase::GetAssetDir() const
{
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return nullptr;
	}
	static const boost::filesystem::path kPathAsset(asset_dir_name_);
	node = node->FindChild(kPathAsset);
	assert(node);
	return node;
}

bool Cmn_AppBase::IsDebug() const
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

bool Cmn_AppBase::Pause()
{
	if (!main_thrd_handle_)
	{
		assert(false);
		return false;
	}
	auto cnt = ::SuspendThread(main_thrd_handle_);
	if (cnt == (pt_dword)-1)
	{
		assert(false);
		return false;
	}
	return true;
}

bool Cmn_AppBase::Resume()
{
	if (!main_thrd_handle_)
	{
		assert(false);
		return false;
	}
	auto cnt = ::ResumeThread(main_thrd_handle_);
	if (cnt == pt_dword(-1))
	{
		assert(false);
		return false;
	}
	return true;
}

Cmn_AppBaseFactory::Cmn_AppBaseFactory()
{

}

CmnLoginConnectCfg* Cmn_AppBaseFactory::CreateCheckServerConnection(const std::string& svr_ip)
{
	assert(false);
	return nullptr;
}

LuaThrdData* Cmn_AppBaseFactory::CreateAppLuaVm(lua_State* lstate)
{
	return nullptr;
}

bool LuaCfgParser::ParseLuaCfg(const luabind::object& lua_cfg)
{
	using namespace luabind;
	auto_run_ = object_cast<bool>(lua_cfg["autoRun"]);

	/*auto script = object_cast<std::string>(lua_cfg["script"]);
	LOG_O(Log_trace) << "lua cfg script:" << script;
	for (iterator it(lua_cfg), it_end; it != it_end; ++it)
	{
	object v = *it;
	object key = it.key();

	LOG_O(Log_trace) << "lua cfg key:" << object_cast<std::string>(key) << " value:" << LuaSerialize::ToString(v);
	}*/
	return true;
}

LuaCfgParser::LuaCfgParser()
{
	auto_run_ = true;
}
