
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
#include <Net/PocoMore.h>

// CConsoleApp:
// See Console.cpp for the implementation of this class
//
class CheckLoginC;
class CConsoleApp : public CWinApp
{
public:
	CConsoleApp();
	//设置验证服务器连接成功与否

// Overrides
public:
	virtual BOOL InitInstance();
	BOOL InitImpl();
	virtual int ExitInstance();
	void EngrossLpk();
	void UnEngrossLpk();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CryptoStrCompose::SubCryptoT				public_key_;
	CryptoStrCompose::SubCryptoT				private_key_;
	HANDLE										engross_lpk_;
	bool			is_auto_run_;
	DateTimeLocal	fatigue_end_to_clear_time_;				//疲劳值到期清空时间
};

extern CConsoleApp theApp;
