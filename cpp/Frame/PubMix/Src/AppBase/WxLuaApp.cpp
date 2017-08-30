#include "stdafx.h"
#include <AppBase/WxLuaApp.h>
#include <CmnMix/Cmn_LogOut.h>
#include <luabind_wx/bindwx_base.h>
#include <luabind_wx/bindwx.h>
#include <wxbind/include/wxbinddefs.h>
WXLUA_DECLARE_BIND_ALL


//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(WxLuaApp, Cmn_App)
EVT_LUA_PRINT(wxID_ANY, WxLuaApp::OnLua)
EVT_LUA_ERROR(wxID_ANY, WxLuaApp::OnLua)
EVT_LUA_DEBUG_HOOK(wxID_ANY, WxLuaApp::OnLua)
EVT_LUA_CREATION(wxID_ANY, WxLuaApp::OnLua)
END_EVENT_TABLE()

WxLuaApp::WxLuaApp()
{

}

bool WxLuaApp::OnInit_()
{
	WXLUA_IMPLEMENT_BIND_ALL
		wxLuaState::sm_wxAppMainLoop_will_run = true;
	wxlState = wxLuaState(this, wxID_ANY);
	if (!wxlState.Ok())
	{
		assert(false);
		return false;
	}
	if (!__super::OnInit_())
		return false;

	return true;
}

LuaThrdData* WxLuaApp::CreateLuaVm()
{
	return GetAppFactory().CreateAppLuaVm(wxlState.GetLuaState());
}

int WxLuaApp::OnExit_()
{
	ClearAllLuaBase();
	auto res = __super::OnExit_();
	wxlState.CloseLuaState(true);
	wxlState.Destroy();
	return res;
}

void WxLuaApp::OnLua(wxLuaEvent& event)
{
	HandleLuaMessage(event.GetString(), event.GetEventType());
}

void _HandlePosLuaMsg_(){
	if (CmnExceptionBase::GetDelayException())
	{
		try{
			try{
				throw CmnExitIoThreadException("不应该会捕获到这个异常的2");
			}
			DO_CATCH_ALL_EXCEPTION({
				CmnExceptionBase::HandleDelayException();
				assert(false);
			})
		} DO_CATCH_THE_EXCEPTION(const CmnLuaException& e, {
			LOG_O(Log_debug) << "自定义lua异常信息->" << e.what();
		})
	}
	else{
		//刚才还有异常，现在莫名其妙地没有了
		assert(false);
	}	
}
void WxLuaApp::HandleLuaMessage(const wxString &msg, int type)
{
	if (wxEVT_LUA_PRINT == type)
	{
		LOG_O(Log_info) << msg;
	}
	else if (wxEVT_LUA_ERROR == type)
	{
		auto exit_exception = CmnExceptionBase::GetDelayException();
		if (exit_exception)
		{
			LOG_O(Log_debug) << "从wxlua处接收到了自定义异常";
			GetIoService().post(_HandlePosLuaMsg_);
		}
		else
		{
			LOG_O(Log_debug) << "wxlua出现了异常:" << msg;
		}
	}
	else if (wxEVT_LUA_CREATION == type)
	{
		//LOG_O(Log_trace) << "wxEVT_LUA_CREATION : " << msg;
	}
	else if (wxEVT_LUA_DEBUG_HOOK == type)
	{
		LOG_O(Log_debug) << "wxEVT_LUA_DEBUG_HOOK : " << msg;
	}
	else
	{
		LOG_O(Log_trace) << "wxlua event type : " << type << " msg : " << msg;
	}
}

void WxLuaApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<WxLuaApp, Cmn_App>("WxLuaApp")
			.luabase("wx.wxApp")
		];
	AddLuaBase(LuaBindWxPolicy::InitWxLua<int>(l, this));
	Luabind_Wx_All(l);
}

WxLuaAppFactory& WxLuaApp::GetAppFactory() const
{
	assert(app_factory_);
	return *static_cast<WxLuaAppFactory*>(app_factory_);
}

WxLuaAppFactory* WxLuaApp::CreateAppFactory()
{
	return new WxLuaAppFactory;
}

LuaThrdData* WxLuaAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new LuaThrdData(lstate);
}
