// RecordMap.h : main header file for the RecordMap DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <AppBase/CmnCWinApp.h>
#include <AppBase/CmnWxThrd.h>



class MyThrdIo : public Cmn_ThrdIo{
private:
	bool OnInit_() override;
	LuaThrdData* CreateLuaVm() override;
};

class CRecordMapApp : public Cmn_CWinApp{
public:
	CRecordMapApp();

// Overrides
private:
	bool OnInit_() override;
	int OnExit_() override;

public:
	CDialog*			main_dlg_;
};
CMN_MY_APP_DECLARE(CRecordMapApp);
