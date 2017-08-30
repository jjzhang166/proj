// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <boost/thread.hpp>
#include <cassert>
#include "protect/Protect.h"
#include "LoginC.h"
#include "Common/LogOut.h"
#include "LoginAssistS.h"

boost::thread& GetImplThread(){
	static boost::thread t;
	return t;
}

//全局初始化
extern bool InitGame();

void IoRun(){
	VMP_BEGIN;
	//init log
	LogDbgviewInstanceHelper log_dbgview_creator;
	LogXmlInstanceHelper log_xml_creator(_T("Login.dll"), _T("LogLogin"));

	//LoginAssistServer ser;
	boost::shared_ptr<LoginC> con_account(new LoginC);
	con_account->ConnectAsync();
	if ( !InitGame() )
	{
		assert(false);
		return;
	}
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
			ModuleHelper::GetMe().SetSelfModuleHandle(hModule);
			boost::thread t([](){
				PreIoRun(&IoRun);
			});
			GetImplThread().swap(t);
			break;
		}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

