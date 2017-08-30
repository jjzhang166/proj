#include "stdafx.h"
#include <AppMix/AppAtpConsole.h>
#include <CmnMix/Cmn_Toolhelp.h>
#include <AssetSys/AS_Sys.h>

//////////////////////////////////////////////////////////////////////////
static const boost::filesystem::path kGameFileTASLogin = L"TenProtect/TASLogin.exe";
static const boost::filesystem::path kGameFileTenioDL = L"Tenio/TenioDL/TenioDL.exe";
//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(AtpConsoleLoginSessionOper){
	bufcall::class_<AtpConsoleLoginSessionOper, ConsoleLoginSessionOper>("AtpConsoleLoginSessionOper")
	;
}

AtpAppConsoleFactory* AtpAppConsole::CreateAppFactory()
{
	return new AtpAppConsoleFactory;
}

AtpAppConsole& AtpAppConsole::GetApp()
{
	return __super::GetApp().CastAs<AtpAppConsole>();
}

void AtpAppConsole::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)[
		class_<AtpAppConsole, WxLuaApp_Console>("AtpAppConsole")	
	];
}

AtpAppConsole::AtpAppConsole()
{
	using namespace boost::filesystem;
	AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"TenProtect/BugTrace.dll", kFM_Find },
	{ kSFM_AtSysAsset, L"DummyBugTrace.dll", kFM_Find });
	AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"TenProtect/BugTrace_.dll", kFM_Find },
	{ kSFM_AtSysAsset, L"BugTrace.dll", kFM_Find });
	AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"CrackCaptchaAPI.dll", kFM_FindChild },
	{ kSFM_AtSysAsset, L"CrackCaptchaAPI.dll", kFM_FindChild });
	//////////////////////////////////////////////////////////////////////////
	AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"d3d9.dll", kFM_FindChild },
	{ kSFM_AtSysAsset, L"d3d9_profile.dll", kFM_FindChild });
	AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"TenProtect/SSOSocket.dll", kFM_Find },
	{ kSFM_AtSysAsset, L"Login.dll", kFM_FindPosterity });
	AddD3d9Cfg(kDM_Profile, { kSFM_AtGamePathStarter, L"Client.exe", kFM_FindChild },
	{ kSFM_AtGamePathStarter, "TenProtect/SSOSocket.dll", kFM_Find });
}

bool AtpAppConsole::PreLoginGame()
{
	if (!__super::PreLoginGame())
	{
		assert(false);
		return false;
	}
	if (!CloseTheGameProcess(kGameFileTASLogin))
	{
		assert(false);
		return false;
	}
	if (!CloseTheGameProcess(kGameFileTenioDL))
	{
		assert(false);
		return false;
	}
	return true;
}

boost::filesystem::path AtpAppConsole::GetStarterRalativePath() const
{
	return L"TCLS\\Client.exe";
}

AtpConsoleLoginSessionOper::AtpConsoleLoginSessionOper(ConsoleLoginSessionS& session) : ConsoleLoginSessionOper(session)
{

}

AtpConsoleLoginSessionOper* AtpAppConsoleFactory::CreateLoginSessionOper(ConsoleLoginSessionS& session)
{
	return new AtpConsoleLoginSessionOper(session);
}

AtpConsoleLoginSession* AtpAppConsoleFactory::CreateLoginSession(ConsoleLoginServer& server)
{
	return new AtpConsoleLoginSession(server);
}

AtpUiOperMain* AtpAppConsoleFactory::CreateAppUiOper(WxLuaApp_Console& app)
{
	return new AtpUiOperMain(app);
}

AtpConsoleLoginSession::AtpConsoleLoginSession(Server& server) : ConsoleLoginSessionS(server)
{

}

AtpUiOperMain::AtpUiOperMain(WxLuaApp_Console& app) : ConsoleAppUiOperMain(app)
{

}

bool AtpUiOperMain::DoLoginOne(const std::string& key, const stAcSerInfo& ac_info)
{
	auto& app = my_app_.CastAs<AtpAppConsole>();
	auto ser_id = app.FindSerId(ac_info.area_, ac_info.ser_);
	if (!ser_id)
	{
		assert(!"不支持该服务器，无法登陆");
		return false;
	}
	auto abs_path = app.game_dir_starter_asnode_->GetAbsolutePath_();
	abs_path /= L"config/LoginQ.dat";
	WriteQqLoginRecord(ac_info.ac_, *ser_id, abs_path.string());
	return __super::DoLoginOne(key, ac_info);
}
