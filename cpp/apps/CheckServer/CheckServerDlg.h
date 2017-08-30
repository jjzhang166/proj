
// CheckServerDlg.h : header file
//

#pragma once
#include "CheckServerS.h"
#include "DataHandlerS.h"
#include <Common/UsefulClass.h>
#include "Resource.h"

// CCheckServerDlg dialog
class CCheckServerDlg : public CDialog, public Singleton<CCheckServerDlg, Singleton_Instance>
{
// Construction
public:
	CCheckServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHECKSERVER_DIALOG };

public:
	//得到扣除天数
	static int CalcDecrementMinutes();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//创建表、导入旧表
	afx_msg void OnBtCreate_Import();
	//执行扣除
	afx_msg void OnBtMinusMinutes();
	afx_msg void OnBnClickedRadioAsDay();
	afx_msg void OnBnClickedRadioAsMinute();
	afx_msg void OnBtSetVersion();
	DECLARE_MESSAGE_MAP()

private:
	CheckServer::SelfPtr				check_server_;
	DbSingleton::SelfPtr				db_singleton_;
};
