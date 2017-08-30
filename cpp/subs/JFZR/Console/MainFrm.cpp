
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Console.h"

#include "MainFrm.h"
#include "Common/UsefulFunc.h"
#include "protect/Protect.h"
#include "TabPages.h"
#include "SafeCheck.h"
#include <Communicate/CGL_Defines.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_LOGIN, &CMainFrame::OnMenuLogin)
	ON_COMMAND(ID_MENU_REG, &CMainFrame::OnMenuReg)
	ON_COMMAND(ID_MENU_PAY, &CMainFrame::OnMenuPay)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VMP_BEGIN_EX;
	auto& safe_check = SafeCheckLpk::GetMe();
	if ( !safe_check.CreateCheckLpkFileMap() )
	{
		assert(false);
		return -1;
	}
	if ( !VMP_CRC_VALID() || VMP_IS_DEBUGGER() )
	{
		SetIoTimer(10 * 1000, []( IoTimer* timer ){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"is on debug or crc"
#else
				"iodoc"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}
	safe_check.WriteValue(::GetCurrentProcessId());
	LoadLibrary(_T("lpkb.dll"));

	if (SafeCheckLpk::SafeCheckLpkValid())
		//if (false)
	{
		SetIoTimer(20 * 1000, []( IoTimer* timer ){
			//return;
#ifdef _DEBUG
			return;
#endif
			VMP_BEGIN_EX;
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "check lpk";
			const auto& kLpkFullPath = GetModulePath(nullptr) + VMP_PROTECT_STR("lpkb.dll");
			bool check_failed = true;
			if (GetModuleHandle(kLpkFullPath))
			{
				check_failed = false;
			}
			if ( check_failed /*|| 0 == FindAtomA(VMP_PROTECT_STR("lpkb.dll"))*/ )
			{
				const char* const kThrowStr =
#ifdef _DEBUG
					"can not find lpk.dll class"
#else
					"cnflc"
#endif
					;
				throw CmnExitIoThreadException(kThrowStr);
			}
			assert(timer);
			timer->Continue();
			VMP_END;
		});
	}
	else
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "校验lpk uuid不成功";
		SetIoTimer(35 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"check safe lpk failed";
#else
				"cslf"
#endif
				;
			throw CmnExitIoThreadException(VMP_PROTECT_STR(kThrowStr));
			VMP_END;
		});
	}
	
	if ( !VMP_CRC_VALID() || VMP_IS_DEBUGGER() )
	{
		SetIoTimer(10 * 1000, []( IoTimer* timer ){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"is on debug or crc"
#else
				"iodoc"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}
	//校验版本
	if ( !CreateCheck_AssistVersion() )
	{
		assert(false);
		SetIoTimer(30 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"create check version failed";
#else
				"ccvf"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	VMP_END;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME;
	/*cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU;*/

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	//设置窗口尺寸
	cs.cx = 0x309 + 120 * 3 + 40;
	cs.cy = 0x177;

	//更改类名
	{
		WNDCLASS wc;
		::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wc);
		wc.lpszClassName = VMP_PROTECT_STR(kMainWndClsName);
		AfxRegisterClass(&wc);
		cs.lpszClass = wc.lpszClassName;
	}
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnMenuLogin()
{
	dlg_login_.DoModal();
}


void CMainFrame::OnMenuReg()
{
	dlg_reg_.DoModal();
}


void CMainFrame::OnMenuPay()
{
	dlg_pay_.DoModal();
}

void CMainFrame::OnClose()
{
	auto account_list = PageAccount_List::GetInstance();
	if (account_list)
	{
		account_list->SaveAllAccountCfg();
		account_list->SaveAllSpecificCfg();
	}
	auto page_ac = PageAccount::GetInstance();
	if (page_ac)
	{
		page_ac->SaveConfig();
	}
	__super::OnClose();
}

void CMainFrame::BeginCheckAcValid()
{
	VMP_BEGIN_EX;
	if (timer_ac_check_)
		timer_ac_check_->Cancel();
	timer_ac_check_ = CreateIoTimer(5 * 60 * 1000, []( IoTimerPtr& timer ){
		VMP_BEGIN_EX;
		if ( !timer )
		{
			assert(false);
			return;
		}
		auto main_frm = CMainFrame::GetInstance();
		if ( !main_frm )
		{
			assert(false);
			return;
		}
		const auto& single_cfg = stSingletonCfg::GetMe();
		std::string str_ac = CT2CA(single_cfg.authen_account_ ), str_pwd = CT2CA(single_cfg.authen_password_);
		std::string str_ac_pwd = str_ac + str_pwd;
		if ( GenUuidStr(str_ac_pwd) != main_frm->ac_pwd_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "帐号不对";
			assert(false);
			const char* kThrowStr = 
#ifdef _DEBUG
				"ac not matched"
#else
				"anm"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		}
		timer->Continue();
		VMP_END;
	}, true);
	VMP_END;
}

CMainFrame::~CMainFrame()
{
	if (timer_ac_check_)
	{
		timer_ac_check_->Cancel();
	}
}

void CMainFrame::SetValidAcInfo( const std::string& ac_pwd )
{
	VMP_BEGIN_EX;
	ac_pwd_ = ac_pwd;
	VMP_END;
}
