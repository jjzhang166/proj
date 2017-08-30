// Console.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Console.h"
#include <CmnMix/Cmn_LogOut.h>
#include <luabind/raw_call_policy.hpp>
#include <Script/FileSystem.h>
#include <AssetSys/AS_Sys.h>

CMN_MY_WX_APP_DEFINE(CConsoleApp);

CConsoleApp::CConsoleApp() : AtpAppConsole(L"PathOfExile.exe")
{
}

bool CConsoleApp::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	return true;
}

int CConsoleApp::OnExit_()
{
	return __super::OnExit_();
}

void CConsoleApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<CConsoleApp, AtpAppConsole>("CConsoleApp")
			.def("Test", &CConsoleApp::Test, raw_call(_2)),
			def("GetMyApp", &GetMyApp)
		];
	RegFileSystemToLua(l);
}

void CConsoleApp::Test(lua_State* l)
{
	lua_pushvalue(l, -1);
}

MyAppFactory* CConsoleApp::CreateAppFactory()
{
	return new MyAppFactory;
}

bool CConsoleApp::ModifySomeGameFiles()
{
	using namespace boost::filesystem;
	path ten_cfg_path = GetMyDocumentsFolderPath();
	if (ten_cfg_path.empty())
	{
		assert(false);
		return false;
	}
	ten_cfg_path /= L"My Games/Path of Exile/tencent_Config.ini";
	auto asset_dir = GetAssetDir();
	if (!asset_dir)
	{
		assert(false);
		return false;
	}
	auto as_ten_cfg = asset_dir->FindPosterity(L"tencent_Config.ini");
	if (!as_ten_cfg)
	{
		assert(false);
		return false;
	}
	if (!as_ten_cfg->FileSameContent(ten_cfg_path.string()))
	{
		if (!as_ten_cfg->SaveToDisk(ten_cfg_path))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

AppLuaThrdData::AppLuaThrdData(lua_State* lstate) : LuaThrdData(lstate)
{

}

MyThrdIo* MyAppFactory::CreateIoThrd()
{
	return new MyThrdIo;
}

LuaThrdData* MyAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new AppLuaThrdData(lstate);
}

LuaThrdData* MyAppFactory::CreateIoThrdLuaVm()
{
	return new IoLuaThrdData;
}

ConsoleLoginMgr* MyAppFactory::CreateConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper)
{
	return new ConsoleLoginMgrImpl(ui_oper);
}

bool ConsoleLoginMgrImpl::HandleAttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& session)
{
#ifdef _DEBUG
	CancelLogin(item_key);
	return true;
#else
	return __super::HandleAttachSessionGame(pid, item_key, session);
#endif
}

ConsoleLoginMgrImpl::ConsoleLoginMgrImpl(ConsoleAppUiOperIo& ui_oper) : ConsoleLoginMgr(ui_oper)
{

}
