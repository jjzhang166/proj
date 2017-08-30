#include "stdafx.h"
#include "DllApp.h"
#include <Common/UsefulClass.h>

CMN_MY_DLLAPP_DEFINE(App);

bool App::OnInit_()
{
#ifdef _DEBUG
	//wxSetEnv(wxT("__MyZhpAcItemKey__"), "11ab");
#endif
	if (!__super::OnInit_())
		return false;
	return true;
}

App::App()
{

}

void App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<App, AtpAppLogin>("App"),
			def("GetMyApp", &GetMyApp)
		];
}

MyAppFactory* App::CreateAppFactory(){
	return new MyAppFactory;
}

pt_cwsz App::GetLoginClientWndTitle() const
{
	return L"流放之路登录程序";
}

stVec2Int App::GetCaptchaInputPos() const
{
	return stVec2Int(586, 439);
}

bool App::EnteredServiceScene() const
{
	struct stHelper{
		stHelper(const App& app){
			_tcscpy_s(find_info_.str_szClassName, _T("Internet Explorer_Hidden"));
			find_info_.pid_ = ::wxGetProcessId();
			but_info_.but_thrd_id_ = ::GetWindowThreadProcessId(app.GetLoginClientMainWnd(), nullptr);
		}

		EnumPidWnd::FindInfo	find_info_;
		EnumPidWnd::stButInfo	but_info_;
	};
	static stHelper helper(*this);
	EnumPidWnd enum_wnd;
	return !!enum_wnd.FindTheWndBut(helper.find_info_, helper.but_info_);
}

DaMaTu* MyAppFactory::CreateDaMaTu()
{
	struct MyDaMaTu : DaMaTu{
		const char* SoftKey() const override{
			return "3e6f191aa757b73fab904d9ec32ae821";
		}
	};
	return new MyDaMaTu;
}
