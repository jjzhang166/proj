
// Console.h : main header file for the Console application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <Socket/server.h>
#include <Protocol/P_BaseStruct.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>

// CConsoleApp:
// See Console.cpp for the implementation of this class
//
class CheckLoginC;
class CConsoleApp : public CWinApp
{
public:
	CConsoleApp();
	//设置验证服务器连接成功与否
	void SetCheckSvrConnected( bool succeed );

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void EngrossLpk();
	void UnEngrossLpk();

// Implementation
public:
	void SetCheckLogin( boost::shared_ptr<CheckLoginC> check_login );
	boost::shared_ptr<CheckLoginC> GetCheckLogin();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	boost::weak_ptr<CheckLoginC>			check_login_;
	boost::optional<bool>					check_svr_connect_succeed_;			//验证服务器是否连接成功
	CryptoStrCompose::SubCryptoT				public_key_;
	CryptoStrCompose::SubCryptoT				private_key_;
	HANDLE										engross_lpk_;
};

extern CConsoleApp theApp;
