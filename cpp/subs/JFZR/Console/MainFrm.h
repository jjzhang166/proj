
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"
#include "DlgUnion.h"
#include <Common/UsefulClass.h>

class CMainFrame : public CFrameWnd, public Singleton<CMainFrame, Singleton_Instance>
{	
public:
	CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Operations
public:
	//开始检测帐号名是否正确
	void BeginCheckAcValid();
	//设置验证服务验证通过的帐号和密码
	void SetValidAcInfo( const std::string& ac_pwd );

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;

public:
	afx_msg void OnMenuLogin();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnMenuReg();
	afx_msg void OnMenuPay();
	DECLARE_MESSAGE_MAP()

private:
	IoTimerPtr					timer_ac_check_;
	DlgLogin					dlg_login_;
	DlgReg						dlg_reg_;
	DlgPay						dlg_pay_;
	std::string					ac_pwd_;
};