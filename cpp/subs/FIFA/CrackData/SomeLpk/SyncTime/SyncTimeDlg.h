// SyncTimeDlg.h : header file
//

#if !defined(AFX_SYNCTIMEDLG_H__BA746F53_9C87_4C73_AAC2_F101719A0B99__INCLUDED_)
#define AFX_SYNCTIMEDLG_H__BA746F53_9C87_4C73_AAC2_F101719A0B99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSyncTimeDlg dialog

class CSyncTimeDlg : public CDialog
{
// Construction
public:
	BOOL SyncSystemClock(CTime tmNow);
	BOOL GetInternetTime(CTime* pTime, CString strServer);
	CSyncTimeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSyncTimeDlg)
	enum { IDD = IDD_SYNCTIME_DIALOG };
	CComboBox	m_cmbServer;
	CString	m_info;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyncTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSyncTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnSync();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DWORD dwDely;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNCTIMEDLG_H__BA746F53_9C87_4C73_AAC2_F101719A0B99__INCLUDED_)
