// Console.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Console.h"
#include <CmnMix/Cmn_LogOut.h>
#include <luabind/raw_call_policy.hpp>
#include <Script/FileSystem.h>
#include <AssetSys/AS_Sys.h>
#include "resource.h"
#include "MapOper.h"

CMN_MY_WX_APP_DEFINE(CConsoleApp);
#ifdef VER_GJ3_
const pt_cwsz kConfigFileName = L"beta_Config.ini";
#else
const pt_cwsz kConfigFileName = L"production_Config.ini";
#endif

BC_REG_CLS_DEFINE(MySessionOper){
	bufcall::class_<MySessionOper, ConsoleSessionOper>("MySessionOper")
		.def("OnSetMapData", &MySessionOper::OnSetMapData)
		.def("OnSetPlayerPos", &OnSetPlayerPos);
}

void MySessionOper::OnSetMapData(const CG_MapBitset& bset, const stSizeInt& map_size, const std::string& map_name)
{
	auto oper = GetDlgMapOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	auto mfc_map = oper->GetMfcMap();
	if (!mfc_map)
		return;
	mfc_map->SetMapData(bset, map_size, map_name);
}

DlgMapOper* MySessionOper::GetDlgMapOper()
{
	auto oper = GetMyApp().GetAcOperMgr().Find(GetSession().GetKey());
	if (!oper)
	{
		assert(false);
		return nullptr;
	}
	return oper->GetDlgMapOper();
}

void MySessionOper::OnSetPlayerPos(const stVec2Int& player_pos)
{
	auto oper = GetDlgMapOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	auto mfc_map = oper->GetMfcMap();
	if (!mfc_map)
		return;
	mfc_map->SetPlayerPos(player_pos);
}

CConsoleApp::CConsoleApp()
{
	/*AddGameFileOverwrite({ kSFM_AtGamePathStarter, L"vclink.dll", kFM_FindChild },
	{ kSFM_AtSysAsset, L"Login.dll", kFM_FindPosterity });*/
	//SetStarterProcessCommand("--dcs --nologo --nopatch");
	SetStarterProcessCommand("--dcs --nologo");
	AddAcOperSink(&my_ac_oper_mgr_);
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
			class_<CConsoleApp, WxLuaApp_Console>("CConsoleApp")
			.def("Test", &CConsoleApp::Test, raw_call(_2))
			.def("OnOpenMfcDlg", &CConsoleApp::OnOpenMfcDlg),
			def("GetMyApp", &GetMyApp)
		];
	RegFileSystemToLua(l);
}

void CConsoleApp::Test(lua_State* l)
{
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
	ten_cfg_path /= L"My Games/Path of Exile/";
	ten_cfg_path /= kConfigFileName;
	auto asset_dir = GetAssetDir();
	if (!asset_dir)
	{
		assert(false);
		return false;
	}
	auto as_ten_cfg = asset_dir->FindPosterity(kConfigFileName);
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

boost::filesystem::path CConsoleApp::GetStarterRalativePath() const
{
	return L"Client.exe";
}

void CConsoleApp::OnOpenMfcDlg()
{
	auto first = my_ac_oper_mgr_.GetFrist();
	if (!first)
	{
		assert(false);
		return;
	}
	first->Show(true);
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

MySessionOper* MyAppFactory::CreateGameSessionOper(ConsoleSessionS& session)
{
	return new MySessionOper(session);
}

MyConsoleSession* MyAppFactory::CreateGameSession(ConsoleServer& server)
{
	return new MyConsoleSession(server);
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
