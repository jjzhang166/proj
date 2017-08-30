#pragma once
/*
@author zhp
@date 2016/11/14 17:46
@purpose for MFC, CWinApp，兼容代码，最好用wxApp
*/
#include "CmnWxApp.h"
#include <afxwin.h>


//CWinApp、Cmn_AppBase，继承的顺序不能颠倒
class Cmn_CWinApp : public CWinApp, public Cmn_WxAppWithInitor{
protected:
	Cmn_CWinApp();

public:
	bool DoExit() override;

protected:
	using Cmn_WxAppWithInitor::DoInit;
	bool DoRun() override;
	void BindFrame(lua_State* l) override;

private:
	BOOL InitInstance() override final;
	int ExitInstance() override final;
	void ActiveIoService() override final;
	BOOL PreTranslateMessage(MSG* pMsg) override final;
	//异常并未处理
	//bool OnExceptionInMainLoop() override;

private:
	static const unsigned int	kIoServiceMsgId = 55555;
};