// RecordMap.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "RecordMap.h"
#include <boost/thread.hpp>
#include <CmnMix/Cmn_UsefulClass.h>
#include <CmnMix/Cmn_LogOut.h>
#include "RECM_Lua.h"
#include "RecMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CRecordMapApp::CRecordMapApp()
{
	main_dlg_ = nullptr;
	is_add_main_thred_ = false;
}

CMN_MY_APP_DEFINE(CRecordMapApp);

// The one and only CRecordMapApp object

bool CRecordMapApp::OnInit_()
{
	if (!__super::OnInit_())
	{
		assert(false);
		return false;
	}
	ThreadablePtr io_thrd(new MyThrdIo);
	assert(io_thrd);
	AddThrd(io_thrd, true);
	return true;
}

int CRecordMapApp::OnExit_()
{
	if (main_dlg_ && IsWindow(main_dlg_->GetSafeHwnd()))
		main_dlg_->SendMessage(WM_CLOSE);
	return __super::OnExit_();
}

LuaThrdData* MyThrdIo::CreateLuaVm()
{
	return new RECM_LuaVM;
}

bool MyThrdIo::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	this->GetIoService().post([](){
		BOOST_SCOPE_EXIT_ALL(&){
			GetMyApp().main_dlg_ = nullptr;
		};
		RecordMap main_dlg;
		GetMyApp().main_dlg_ = &main_dlg;
		main_dlg.DoModal();
	});
	return true;
}
