
// CheckServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <AppBase/CmnCWinApp.h>
#include <AppBase/CmnWxThrd.h>

// CCheckServerApp:
// See CheckServer.cpp for the implementation of this class
//

class MyThrdIo : public Cmn_ThrdIo{
};


class CCheckServerApp : public Cmn_CWinApp{
public:
	CCheckServerApp();

// Overrides
public:
	bool OnInit_() override;
	int OnExit_() override;
};

CMN_MY_APP_DECLARE(CCheckServerApp);
