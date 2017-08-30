
// CSViewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include <string>
#include "CheckServerC.h"
#include "stdafx.h"
#include <AppBase/CmnCWinApp.h>
#include <AppBase/CmnWxThrd.h>
#include "resource.h"


// CCSViewerApp:
// See CSViewer.cpp for the implementation of this class
//

class MyThrdIo : public Cmn_ThrdIo{
};

class MyAppFactory : public Cmn_AppBaseFactory{
	CmnLoginConnectCfg* CreateCheckServerConnection(const std::string& svr_ip) override;
};

class CCSViewerApp : public Cmn_CWinApp{
public:
	CCSViewerApp();

	//interfaces
public:
	const std::string& GetUserAc() const;
	const std::string& GetUserPwd() const;
	//设置用户信息
	void SetUserInfo( const std::string& user_ac, const std::string& user_pwd );

private:
	bool OnInit_() override;
	int OnExit_() override;
	LuaThrdData* CreateLuaVm() override;
	Cmn_AppBaseFactory* CreateAppFactory() override;

public:
	std::string						user_ac_;
	std::string						user_pwd_;
};
CMN_MY_APP_DECLARE(CCSViewerApp);
