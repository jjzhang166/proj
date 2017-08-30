// Console.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Console.h"
#include <CmnMix/Cmn_LogOut.h>
#include <luabind/raw_call_policy.hpp>
#include <Script/FileSystem.h>
#include <AssetSys/AS_Sys.h>

CMN_MY_WX_APP_DEFINE(CConsoleApp);

CConsoleApp::CConsoleApp()
{
	//SetStarterProcessCommand("--dcs --nologo --nopatch");
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
	if (!__super::ModifySomeGameFiles())
	{
		assert(false);
		return false;
	}
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
	//////////////////////////////////////////////////////////////////////////
	if (!game_dir_asnode_ || !game_dir_starter_asnode_)
	{
		assert(false);
		return false;
	}
	auto path_game_root = game_dir_asnode_->GetAbsolutePath_();
	DeleteFileOrDirectory(path_game_root, L"TenproxyLog");
	DeleteFileOrDirectory(path_game_root, L"logs");
	auto path_tmp = path_game_root;
	path_tmp /= L"TQM";
	CreateExeAsDirectory(path_tmp, L"TQMCenter.exe");

	path_tmp = game_dir_starter_asnode_->GetAbsolutePath_();
	DeleteFileOrDirectory(path_tmp, L"tlog");
	//CreateExeAsDirectory(path_tmp, L"TPSvc.exe");			//删了重启就不正常
	//CreateExeAsDirectory(path_tmp, L"Assistant.exe");		//删了就不正常
	CreateExeAsDirectory(path_tmp, L"bugreport.exe");
	path_tmp /= L"Tenio/TenioDL";
	CreateExeAsDirectory(path_tmp, L"TenioDL.exe");

	path_tmp = path_game_root;
	path_tmp /= L"qbclient";
	CreateExeAsDirectory(path_tmp, L"qbclient.exe");

	path_tmp = path_game_root;
	path_tmp /= L"components";
	CreateExeAsDirectory(path_tmp, L"PCMgr.dat");
	RenameFileOrDirectory(path_tmp, L"boostup");
	path_tmp /= L"tclsqmfix";
	CreateExeAsDirectory(path_tmp, L"TclsQmFix.exe");
	return true;
}

pt_dword CConsoleApp::GetMaxConnectedCntByAcType(pt_dword ac_type) const
{
	switch (ac_type)
	{
	case 0:
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 30;
	default:
		assert(false);
		break;
	}
	return 0;
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
