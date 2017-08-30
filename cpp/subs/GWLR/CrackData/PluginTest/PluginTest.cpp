// PluginTest.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PluginTest.h"
#include <cassert>
#include <CmnMix/Cmn_UsefulClass.h>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/scope_exit.hpp>
#include <CmnMix/Cmn_Exception.h>
#include "DlgMain.h"
#include "CD_Lua.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CPluginTestApp

BEGIN_MESSAGE_MAP(CPluginTestApp, CWinApp)
END_MESSAGE_MAP()


// CPluginTestApp construction

CPluginTestApp::CPluginTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	main_dlg_ = nullptr;
}


// The one and only CPluginTestApp object

CPluginTestApp theApp;


void ThreadRunFunc()
{
	BOOST_SCOPE_EXIT_ALL(&){
		LogSinkInitor::CloseAllLogOutput();
	};
	try{
		BOOST_SCOPE_EXIT_ALL(&){
			theApp.main_dlg_ = nullptr;
		};
		DlgMain main_dlg;
		theApp.main_dlg_ = &main_dlg;
		main_dlg.DoModal();
	}
	DO_CATCH_THE_EXCEPTION(const CmnExceptionBase& e, {
		LOG_O(Log_fatal) << "Òì³££º" << e.what();
		assert(false);
	})
}

// CPluginTestApp initialization
BOOL CPluginTestApp::InitInstance()
{
	if ( !CWinApp::InitInstance() )
	{
		assert(false);
		return FALSE;
	}

	auto& gs = GlobalSetter::GetMe();
	gs.SetSelfModuleHandle(AfxGetModuleState()->m_hCurrentInstanceHandle);
	gs.SetEnableDump(true);

	LogDbgviewInstanceHelper log_dbgview_creator;
	log_dbgview_creator.ClearHolder();

	if ( !CD_LuaVM::GetMe().InitScript(gs.GetSelfModuleHandle(), _T("main")) )
	{
		assert(false);
		return FALSE;
	}

	boost::thread new_t(ThreadRunFunc);
	gs.GetIoThread().swap(new_t);
	return TRUE;
}

int CPluginTestApp::ExitInstance()
{
	auto& gs = GlobalSetter::GetMe();
	auto& the_thdr = gs.GetIoThread();
	if (the_thdr.joinable())
	{
		if ( theApp.main_dlg_ && IsWindow(theApp.main_dlg_->GetSafeHwnd() ) )
			theApp.main_dlg_->SendMessage(WM_CLOSE);
		CD_LuaVM::ReleaseMe();

		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		TerminateThread(the_thdr.native_handle(), 0);
	}
	else
		assert(false);

	/*auto thdr_handle = the_thdr.native_handle();
	if (thdr_handle)
	{
		DWORD exit_code = 0;
		if (GetExitCodeThread(thdr_handle, &exit_code))
		{
			if ( STILL_ACTIVE == exit_code )
			{
				if ( theApp.main_dlg_ && IsWindow(theApp.main_dlg_->GetSafeHwnd() ) )
					theApp.main_dlg_->PostMessage(WM_CLOSE);
				the_thdr.detach();
				if ( WAIT_TIMEOUT == WaitForSingleObject(thdr_handle, 1000 * 10) )
				{
					if (GetExitCodeThread(thdr_handle, &exit_code))
					{
						if ( STILL_ACTIVE == exit_code )
						{
							int i = 0;
						}
					}
					assert(false);
					TerminateThread(thdr_handle, -1);
				}
			}
		}
		else
			assert(false);
	}
	else
		assert(false);*/
	return CWinApp::ExitInstance();
}
