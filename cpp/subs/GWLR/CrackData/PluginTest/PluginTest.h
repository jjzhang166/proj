// PluginTest.h : main header file for the PluginTest DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CPluginTestApp
// See PluginTest.cpp for the implementation of this class
//

class CPluginTestApp : public CWinApp
{
public:
	CPluginTestApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:
	CDialog*			main_dlg_;
};

extern CPluginTestApp theApp;