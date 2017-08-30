#pragma once
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
const WORD kSqlPort = 54987;
//////////////////////////////////////////////////////////////////////////
// DlgMgrTbl dialog
class DlgMgrTbl : public CDialog
{
	DECLARE_DYNAMIC(DlgMgrTbl)

public:
	DlgMgrTbl(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgMgrTbl();

// Dialog Data
	enum { IDD = IDD_DLG_CREATE_AND_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//创建数据库表
	afx_msg void OnBtCreateDbTbl();
	//导入表数据
	afx_msg void OnBtImportDbData();
	//合并数据库
	afx_msg void OnBtCombineDb();
	DECLARE_MESSAGE_MAP()

private:
	void SetWndText(const char* text);
	void SetWndText(const CString& text);

private:
	CString		str_db_old_;
	CString		str_db_new_;
};
