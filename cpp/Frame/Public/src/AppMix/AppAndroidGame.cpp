#include "stdafx.h"
#include <AppMix/AppAndroidGame.h>
#include <Script/DmOper.h>
#include <Script/StateResolver.h>
#include <Script/FileSystem.h>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(AndroidLoginConnectionOper){
	bufcall::class_<AndroidLoginConnectionOper, LoginConnectionOper>("AndroidLoginConnectionOper")
		.def("SetMemuEnvPath_", &AndroidLoginConnectionOper::SetMemuEnvPath_);
}

BC_REG_CLS_DEFINE(AndroidGameConnectionOper){
	bufcall::class_<AndroidGameConnectionOper, GameConnectionOper>("AndroidGameConnectionOper")
		;
}
//////////////////////////////////////////////////////////////////////////
AndroidGameAppFactory* AndroidGameDllApp::CreateAppFactory()
{
	return new AndroidGameAppFactory;
}

AndroidGameAppFactory& AndroidGameDllApp::GetAppFactory() const
{
	assert(app_factory_);
	return *static_cast<AndroidGameAppFactory*>(app_factory_);
}

AndroidGameConnection* AndroidGameAppFactory::CreateGameConnection(pt_word dest_port)
{
	assert(dest_port);
	return new AndroidGameConnection(AndroidGameDllApp::GetApp(), dest_port);
}

AndroidLoginConnection* AndroidGameAppFactory::CreateLoginConnection(pt_word dest_port)
{
	assert(dest_port);
	return new AndroidLoginConnection(AndroidGameDllApp::GetApp(), dest_port);
}

AndroidGameConnectionOper* AndroidGameAppFactory::CreateGameConnectionOper(GameConnectionC& connection)
{
	return new AndroidGameConnectionOper(connection);
}

AndroidLoginConnectionOper* AndroidGameAppFactory::CreateLoginConnectionOper(LoginConnectionC& connection)
{
	return new AndroidLoginConnectionOper(connection);
}

CmnGameScriptEcho* AndroidGameAppFactory::CreateScriptEcho(GameConnectionC& connection)
{
	return new CmnGameScriptEcho(connection);
}

ScriptRunRAII* AndroidGameAppFactory::CreateScriptRunRAII()
{
	return new ScriptRunRAII;
}

LuaCfgParser* AndroidGameAppFactory::CreateLuaCfgParser(GameConnectionC& connection)
{
	return new LuaCfgParser;
}

MEmuExecEnv* AndroidGameAppFactory::CreateMEmuExecEnv()
{
	return new MEmuExecEnv;
}

Cmn_GameDllData* AndroidGameAppFactory::CreateGameDllData()
{
	return new Cmn_GameDllData;
}

Cmn_LoginDllData* AndroidGameAppFactory::CreateLoginDllData()
{
	return new Cmn_LoginDllData;
}

AndroidGameLuaThrdData* AndroidGameAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new AndroidGameLuaThrdData(lstate);
}

AndroidGameDllApp::AndroidGameDllApp()
{
	memu_exec_env_ = nullptr;
	is_use_dm_ = true;
	is_use_state_node_ = true;
	is_use_filesystem_ = true;
}

void AndroidGameDllApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<AndroidGameDllApp, Cmn_OnlyGameDllApp>("AndroidGameDllApp")
		];
	CmnGameScriptEcho::RegForLua(l);
	if (is_use_dm_)
		DmOper::RegForLua(l);
	if (is_use_state_node_)
		StateNode::RegForLua(l);
	if (is_use_filesystem_)
		RegFileSystemToLua(l);
}

AndroidGameDllApp& AndroidGameDllApp::GetApp()
{
	return AndroidGameDllApp::GetAppBase().CastAs<AndroidGameDllApp>();
}

MEmuExecEnv& AndroidGameDllApp::GetExecEnv()
{
	if (memu_exec_env_)
		return *memu_exec_env_;
	memu_exec_env_ = GetAppFactory().CreateMEmuExecEnv();
	assert(memu_exec_env_);
	return *memu_exec_env_;
}

AndroidGameDllApp::~AndroidGameDllApp()
{
	delete memu_exec_env_;
	memu_exec_env_ = nullptr;
}

bool AndroidGameDllApp::OnInit_()
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
	login_dll_data_ = factory.CreateLoginDllData();
	if (!login_dll_data_)
	{
		assert(false);
		return false;
	}
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

Cmn_LoginDllData& AndroidGameDllApp::GetLoginDllData() const
{
	return __super::GetGameDllData().CastAs<Cmn_LoginDllData>();
}

Cmn_GameDllData& AndroidGameDllApp::GetGameDllData() const
{
	return __super::GetGameDllData().CastAs<Cmn_GameDllData>();
}

AndroidGameLuaThrdData::AndroidGameLuaThrdData(lua_State* lstate) : GameDllLuaThrdData(lstate)
{

}

void AndroidGameLuaThrdData::BindScriptCommand(luabind::module_& mod)
{
	__super::BindScriptCommand(mod);
	using namespace luabind;
	mod[
		def("CopyAssetFileToShareDir", &CopyAssetFileToShareDir),
			def("InstallApk", &InstallApk),
			def("PmInstallApk", &PmInstallApk),
			def("UnInstallApk", &UnInstallApk),
			def("StartApp", &StartApp),
			def("StopApp", &StopApp)
	];
}

bool AndroidGameLuaThrdData::InstallApk(const char* apk_name, const char* package_name)
{
	if (!apk_name || !package_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto& env = app.GetExecEnv();
	MEmuExecEnv::StringContT packages;
	if (env.AdbListPackages(app.adb_device_info_, packages) <= 0)
		return false;
	for (auto& v : packages)
	{
		if (v == package_name)
			return true;
	}
	auto node = app.GetAssetDir();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->FindPosteritySmart(apk_name);
	if (!node)
	{
		assert(false);
		return false;
	}
	const auto& file_full_path = node->GetAbsolutePath_();
	const auto& file_full_name = file_full_path.string();
	return env.AdbInstallApk(app.adb_device_info_, file_full_name);
}

bool AndroidGameLuaThrdData::PmInstallApk(const char* apk_name, const char* package_name)
{
	if (!apk_name || !package_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto& env = app.GetExecEnv();
	MEmuExecEnv::StringContT packages;
	if (env.AdbListPackages(app.adb_device_info_, packages) <= 0)
		return false;
	for (auto& v : packages)
	{
		if (v == package_name)
			return true;
	}
	return env.AdbPmInstallApk(app.adb_device_info_, apk_name);
}

bool AndroidGameLuaThrdData::StartApp(const char* package_name, const char* activity_name)
{
	if (!package_name || !activity_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto& env = app.GetExecEnv();
	return env.AdbStartApp(app.adb_device_info_, package_name, activity_name);
}

bool AndroidGameLuaThrdData::UnInstallApk(const char* package_name)
{
	if (!package_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto& env = app.GetExecEnv();
	return env.AdbUnInstallApk(app.adb_device_info_, package_name);
}

bool AndroidGameLuaThrdData::StopApp(const char* package_name)
{
	if (!package_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto& env = app.GetExecEnv();
	auto& ac_info = app.GetAcInfo();
	return env.AdbStopApp(app.adb_device_info_, package_name);
}

bool AndroidGameLuaThrdData::CopyAssetFileToShareDir(const char* file_name, const char* new_file_name)
{
	if (!file_name || !new_file_name)
	{
		assert(false);
		return false;
	}
	auto& app = AndroidGameDllApp::GetApp();
	auto node = app.GetAssetDir();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->FindPosteritySmart(file_name);
	if (!node)
	{
		assert(false);
		return false;
	}
	auto& ac_info = app.GetAcInfo();
	auto& env = app.GetExecEnv();
	MEmuExecEnv::stShareDirInfo	dir_info;
	if (!env.GetSharedDirInfo(ac_info.login_process_info_, dir_info))
	{
		assert(false);
		return false;
	}
	boost::filesystem::path new_file_path(dir_info.dir_download_);
	new_file_path /= new_file_name;
	if (!node->SaveToDisk(new_file_path, AS_Node::kSF_NoOverwrite))
	{
		assert(false);
		return false;
	}
	return true;
}

AndroidLoginConnectionOper::AndroidLoginConnectionOper(LoginConnectionC& connection) : LoginConnectionOper(connection)
{

}

void AndroidLoginConnectionOper::SetMemuEnvPath_(const std::string& path)
{
	AndroidGameDllApp::GetApp().GetExecEnv().SetMEmuPath(path);
}

AndroidGameConnectionOper::AndroidGameConnectionOper(GameConnectionC& connection) : GameConnectionOper(connection)
{

}

AndroidLoginConnection::AndroidLoginConnection(AndroidGameDllApp& app, pt_word dest_port) : LoginConnectionC(app, dest_port)
{

}

bool AndroidLoginConnection::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	auto& app = AndroidGameDllApp::GetApp();
	auto& exec_env = app.GetExecEnv();
	app.memu_adb_ip_info_ = exec_env.GetAdbIpInfo(ac_info_.login_process_info_);
	if (app.memu_adb_ip_info_.first.empty())
	{
		assert(false);
		return false;
	}
	app.adb_device_info_ = app.memu_adb_ip_info_.first;
	app.adb_device_info_ += ':';
	app.adb_device_info_ += std::to_string(app.memu_adb_ip_info_.second);
	EnumPidWnd::FindInfo theWnd;
	theWnd.str_GWL_STYLE = 0x96020000;
	theWnd.str_GCL_STYLE = 8;
	theWnd.m_bIsWindowVisible = TRUE;
	_tcscpy_s(theWnd.str_szClassName, wxT("Qt5QWindowIcon"));
	theWnd.pid_ = wxGetProcessId();
	EnumPidWnd theEnumPidWnd;
	auto& data = app.GetGameDllData();
	if (!TimerDoWaitUntil(1000, 1000 * 100, [&data](){
		return data.game_top_hwnd_ != nullptr;
	}, [&data, &theEnumPidWnd, &theWnd](){
		auto hwnd = theEnumPidWnd.FindTheWnd(theWnd);
		if (hwnd)
			data.game_top_hwnd_ = hwnd;
	}))
	{
		assert(false);
		return false;
	}
	if (!exec_env.AdbStartServer(true))
	{
		assert(false);
		return false;
	}
	if (!TimerDo(2000, 1000 * 60 * 5, [&app, &exec_env](){
		exec_env.AdbConnect(app.adb_device_info_);
		static const std::string kIpInfo = app.adb_device_info_ + "\tdevice";
		return exec_env.AdbHasTheDeviceInfo(kIpInfo);
	}))
	{
		assert(false);
		return false;
	}
	auto shared_this = shared_from_this();
	GetIoService().post([shared_this](){
		shared_this->DoDisconnect();
	});
	return true;
}

AndroidGameConnection::AndroidGameConnection(AndroidGameDllApp& app, pt_word dest_port) : GameConnectionC(app, dest_port)
{

}

bool AndroidGameConnection::OnConnected(IoConnectBase* connect)
{
	GameLoginSucceed();
	return __super::OnConnected(connect);
}

void AndroidGameConnection::OnDisconnect(IoConnectBase* connect)
{
	auto& app = GetApp();
	auto& data = app.GetGameDllData();
	if (!ac_info_.login_process_info_.empty() && data.game_top_hwnd_)
	{
		::PostMessage(data.game_top_hwnd_, WM_CLOSE, NULL, NULL);
		auto& exec_env = app.GetExecEnv();
		if (!exec_env.Manager_CloseMenu(ac_info_.login_process_info_))
		{
			assert(false);
		}
		GetIoService().post([](){
			throw CmnExitIoThreadException();
		});
	}
	__super::OnDisconnect(connect);
}

AndroidGameDllApp& AndroidGameConnection::GetApp()
{
	return static_cast<AndroidGameDllApp&>(my_app_);
}
