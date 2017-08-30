// AutoUpdate.h : main header file for the AutoUpdate DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <AppBase/CmnWxDllApp.h>

// CAutoUpdateApp
// See AutoUpdate.cpp for the implementation of this class
//

class CAutoUpdateApp : public Cmn_DllApp{
public:
	CAutoUpdateApp();

private:
	bool OnInit_() override;

};
CMN_MY_APP_DECLARE(CAutoUpdateApp);