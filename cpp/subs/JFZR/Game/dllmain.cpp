// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <TlHelp32.h>
#include <boost/thread.hpp>
#include "FuncObjC.h"
#include <cassert>
#include "protect/Protect.h"
#include <Common/CmnException.h>

boost::thread			g_game_thread;
boost::thread& GetImplThread(){
	return g_game_thread;
}

//全局初始化
extern bool InitGame();

//io_service run
void IoRun(){
	VMP_BEGIN_EX;
	//init log
	LogDbgviewInstanceHelper log_dbgview_creator;
	LogXmlInstanceHelper log_xml_creator(_T("Game.dll"), _T("LogGame"));	
#ifdef _DEBUG
	if ( !LoadLibrary(_T("sxs.dll")) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "sxs.dll加载失败";
	}
#endif
	boost::shared_ptr<FuncObj_NormalC> con_account(new FuncObj_NormalC);
	con_account->BeginCheckLoginFailed();
	if ( !InitGame() )
		return;
	if ( !con_account->InitGame() )
		return;
	con_account->ConnectAsync();
	//恢复游戏线程
	SupendProcess(false);
	ProtectDoIoRun();
	VMP_END;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//LoadLibrary(_T("d3d9_feifa.dll"));
			ModuleHelper::GetMe().SetSelfModuleHandle(hModule);
			boost::thread t([](){
				PreIoRun(&IoRun);
			});
			GetImplThread().swap(t);
			break;
		}
	case DLL_THREAD_ATTACH:
		//OutputDbgString("OutputDbgString thread attach");
		break;
	case DLL_THREAD_DETACH:
		//OutputDbgString("OutputDbgString thread dattach");
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}