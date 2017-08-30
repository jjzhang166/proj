#include "stdafx.h"
#include <AppBase/CmnCWinApp.h>
#include <wx/wxprec.h>


//////////////////////////////////////////////////////////////////////////
Cmn_CWinApp::Cmn_CWinApp()
{
	is_inited_ = true;
}

bool Cmn_CWinApp::DoRun()
{
	return true;
}

bool Cmn_CWinApp::DoExit()
{
	if (!__super::DoExit())
	{
		assert(false);
		return false;
	}
	VERIFY(this->PostThreadMessage(kIoServiceMsgId, 0, 1));
	return true;
}

BOOL Cmn_CWinApp::InitInstance()
{
	if (!__super::InitInstance())
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	wxSetInstance(m_hInstance);
	wxApp::m_nCmdShow = m_nCmdShow;
	if (!DoInit())
	{
		assert(false);
		return FALSE;
	}
	return OnInit_();
}

int Cmn_CWinApp::ExitInstance()
{
	auto res = OnExit_();
	__super::ExitInstance();
	return res;
}

void Cmn_CWinApp::ActiveIoService()
{
	VERIFY(this->PostThreadMessage(kIoServiceMsgId, 0, 0));
}

BOOL Cmn_CWinApp::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == kIoServiceMsgId)
	{
		assert(pMsg->hwnd == 0 && pMsg->wParam == 0);
		if (pMsg->lParam == 0)
			OnIoService();
		else if (pMsg->lParam == 1)
			AfxPostQuitMessage(0);
		else
		{
			assert(false);
		}
		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

void Cmn_CWinApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_CWinApp, Cmn_WxAppWithInitor>("Cmn_CWinApp")
		];
}
