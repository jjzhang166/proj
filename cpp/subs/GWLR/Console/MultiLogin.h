#pragma once
/*
@author zhp
@date 2015/6/25 19:40
@purpose for multiple login
*/
#include "DlgUnion.h"
#include "CheckLoginC.h"

enum enListMultiLogin{
	kLML_Ac,
	kLML_Pwd,
	kLML_State,
};

class ListMultiLogin : public CListCtrl, public IDlgAddLoginSink{
public:
	int FindByAc( const CString& ac );
	CString GetItemAccount( int item ) const;

protected:
	void OnAdd( DlgAddLoginAc& dlg );
	virtual void PreSubclassWindow();
	//œÏ”¶”“º¸down
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//ÃÌº”’ ∫≈
	afx_msg void OnMenuAddAccount();
	//…æ≥˝’ ∫≈
	afx_msg void OnMenuDeleteAccount();
	DECLARE_MESSAGE_MAP()
};

class DlgMultiLogin : public CDialog, public Singleton<DlgMultiLogin, Singleton_Instance>{
public:
	DlgMultiLogin();
	virtual void OnOK();
	void OnLoginInfo(const std::string& res);

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();	
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	ListMultiLogin		list_multi_login_;
	CString				ac_tmp_;
	CString				pwd_tmp_;
};