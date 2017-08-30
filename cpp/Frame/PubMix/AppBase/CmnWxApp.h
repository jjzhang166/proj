#pragma once
/*
@author zhp
@date 2016/11/14 16:52
@purpose for wxApp
*/
#include "Cmn_App.h"
#include "FrameEvents.h"
#include <wx/wxprec.h>


//wxInitializer的析构函数会调用wxUninitialize，且注意。
class Cmn_App : public wxApp, public Cmn_AppBase{
protected:
	Cmn_App();

public:
	bool DoExit() override;

protected:
	void BindFrame(lua_State* l) override;

private:
	using Cmn_AppBase::DoInit;
	bool DoRun() override;

private:
	bool OnInit() override final;
	int  OnExit() override final;
	void OnIoService(FE_IoServiceEvent& event);
	bool OnExceptionInMainLoop() override;
	void ActiveIoService() override final;

private:
	wxLocale*		wx_locale_;

private:
	DECLARE_EVENT_TABLE();
};

class Cmn_WxAppWithInitor : public Cmn_AppBase{
protected:
	Cmn_WxAppWithInitor(Cmn_ThrdMgrBase::ThrdMgrSink* sink = nullptr);

protected:
	bool OnInit_() override;
	int OnExit_() override;
	void BindFrame(lua_State* l) override;

private:
	bool		is_wx_inited_;
};
