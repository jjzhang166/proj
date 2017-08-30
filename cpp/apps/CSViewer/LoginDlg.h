#pragma once
/*
@author zhp
@date 2014/4/27
@purpose for login dialog
*/
#include "stdafx.h"
#include "Common/UsefulClass.h"

class LoginDlg : public CDialog, public Singleton<LoginDlg, Singleton_Instance>{
public:
	LoginDlg();
	~LoginDlg();

public:
	void SetLogining( bool logining );
	void LoginSucceed();
	const CString& GetAc() const;
	const CString& GetAcPwd() const;
	void SetWndText( const std::string& wnd_text );

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

private:
	CEdit					edit_ac_;
	CEdit					edit_pwd_;

private:
	CString					str_ac_;
	CString					str_pwd_;
	CString					wnd_text_old_;

private:
	bool					is_logining_;
};

