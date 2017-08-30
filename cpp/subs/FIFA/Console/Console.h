
// Console.h : main header file for the Console application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <Net/server.h>
#include <Protocol/P_BaseStruct.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <Net/PocoMore.h>
#include <AppBase/CmnCWinApp.h>
#include <AppBase/CmnWxThrd.h>



class MyThrdIo : public Cmn_ThrdIo{
private:
	LuaThrdData* CreateLuaVm() override;
	bool OnRun_() override;
};

class MyAppFactory : public Cmn_AppBaseFactory{
	CmnLoginConnectCfg* CreateCheckServerConnection(const std::string& svr_ip) override;
};
class CheckLoginC;
class CConsoleApp : public Cmn_CWinApp
{
public:
	CConsoleApp();

// Overrides
private:
	bool OnInit_() override;
	int OnExit_() override;
	bool InitImpl();
	LuaThrdData* CreateLuaVm() override;
	Cmn_AppBaseFactory* CreateAppFactory() override;

public:
	bool			is_auto_run_;
	DateTimeLocal	last_time_12runconsole_;
	mutable bool	is_exit_;
};

CMN_MY_APP_DECLARE(CConsoleApp);
