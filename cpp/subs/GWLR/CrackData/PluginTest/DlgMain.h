#pragma once


// DlgMain dialog

class DlgMain : public CDialog
{
	DECLARE_DYNAMIC(DlgMain)

public:
	DlgMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgMain();

// Dialog Data
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnBtTest();
	afx_msg void OnBtRunScript();
	DECLARE_MESSAGE_MAP()
};
