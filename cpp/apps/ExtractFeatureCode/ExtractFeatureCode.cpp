
// ExtractFeatureCode.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ExtractFeatureCode.h"
#include "ExtractFeatureCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CExtractFeatureCodeApp::CExtractFeatureCodeApp()
{
	is_add_main_thred_ = false;
}

CMN_MY_APP_DEFINE(CExtractFeatureCodeApp);

bool CExtractFeatureCodeApp::OnInit_()
{
	if (!__super::OnInit_())
		return false;

	CExtractFeatureCodeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}
	return FALSE;
}

