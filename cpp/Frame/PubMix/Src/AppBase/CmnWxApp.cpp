#include "stdafx.h"
#include <AppBase/CmnWxApp.h>
#include <CmnMix/Cmn_LogOut.h>


BEGIN_EVENT_TABLE(Cmn_App, wxApp)
EVT_FE_IO_SERVICE(wxID_ANY, Cmn_App::OnIoService)
END_EVENT_TABLE()

Cmn_App::Cmn_App()
{
	is_inited_ = true;
}

bool Cmn_App::OnInit()
{
	wx_locale_ = new wxLocale(wxLANGUAGE_DEFAULT);
	if (!__super::OnInit())
	{
		assert(false);
		return false;
	}
	if (!DoInit())
	{
		assert(false);
		return false;
	}
	return OnInit_();
}

int Cmn_App::OnExit()
{
	auto res = OnExit_();
	if (wx_locale_)
	{
		delete wx_locale_;
		wx_locale_ = nullptr;
	}
	__super::OnExit();
	return res;
}

void Cmn_App::OnIoService(FE_IoServiceEvent& event)
{
	__super::OnIoService();
}

bool Cmn_App::OnExceptionInMainLoop()
{
	try{
		CmnExceptionBase::CatchDelayException();
		LOG_O(Log_debug) << "Cmn_App之收到自定义的异常";
		GetIoService().post([](){
			if (CmnExceptionBase::GetDelayException())
			{
				try{
					throw CmnExitIoThreadException("不应该会捕获到这个异常的1");
				}
				DO_CATCH_ALL_EXCEPTION({
					CmnExceptionBase::HandleDelayException();
					assert(false);
				})
			}
			else{
				//刚才还有异常，现在莫名其妙地没有了
				assert(false);
			}
		});
		return true;
	} DO_CATCH_ALL_EXCEPTION({
		LOG_O(Log_debug) << "Cmn_App之捕获到未知异常";
		return __super::OnExceptionInMainLoop();
	})
}

void Cmn_App::ActiveIoService()
{
	QueueEvent(new FE_IoServiceEvent(wxID_ANY));
}

bool Cmn_App::DoRun()
{
	return true;
}

bool Cmn_App::DoExit()
{
	if (!__super::DoExit())
	{
		assert(false);
		return false;
	}
	ExitMainLoop();
	return true;
}

void Cmn_App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_App, Cmn_AppBase>("Cmn_App")
		];
}

//////////////////////////////////////////////////////////////////////////

Cmn_WxAppWithInitor::Cmn_WxAppWithInitor(Cmn_ThrdMgrBase::ThrdMgrSink* sink) : Cmn_AppBase(sink)
{
	is_wx_inited_ = false;
}

bool Cmn_WxAppWithInitor::OnInit_()
{
	if (!is_wx_inited_)
	{
		is_wx_inited_ = wxInitialize();
		if (!is_wx_inited_)
		{
			assert(false);
			return false;
		}
	}
	return __super::OnInit_();
}

int Cmn_WxAppWithInitor::OnExit_()
{
	auto res = __super::OnExit_();
	if (is_wx_inited_)
		wxUninitialize();
	return res;
}

void Cmn_WxAppWithInitor::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_WxAppWithInitor, Cmn_AppBase>("Cmn_WxAppWithInitor")
		];
}
