#include "stdafx.h"
#include <AppMix/OnlyLoginComponent.h>
#include <Common/UsefulFunc.h>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(LoginConnectionOper){
	bufcall::class_<LoginConnectionOper, CmnGameConnectionOper>("LoginConnectionOper")
	.def("B22B5A47CE8C0", &LoginConnectionOper::OnCheckInfo_)
	.def("SetLocalCrackCaptcha", &LoginConnectionOper::SetLocalCrackCaptcha_)
	.def("SetDmtCrackCaptcha", &LoginConnectionOper::SetDmtCrackCaptcha_)
	.def("BeginLogin", &LoginConnectionOper::BeginLogin_);
}
//////////////////////////////////////////////////////////////////////////
OnlyLoginAppFactory* Cmn_OnlyLoginDllApp::CreateAppFactory()
{
	return new OnlyLoginAppFactory;
}

Cmn_OnlyLoginDllApp& Cmn_OnlyLoginDllApp::GetApp()
{
	return Cmn_OnlyLoginDllApp::GetAppBase().CastAs<Cmn_OnlyLoginDllApp>();
}

void Cmn_OnlyLoginDllApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_OnlyLoginDllApp, Cmn_GameDllApp>("Cmn_OnlyLoginDllApp")
		];
}

bool Cmn_OnlyLoginDllApp::OnInit_()
{
	if (!app_factory_)
	{
		app_factory_ = CreateAppFactory();
		if (!app_factory_)
		{
			assert(false);
			return false;
		}
	}
	auto& factory = GetAppFactory();
	login_dll_data_ = factory.CreateLoginDllData();
	if (!login_dll_data_)
	{
		assert(false);
		return false;
	}
	if (!__super::OnInit_())
		return false;
	return true;
}

LoginConnectionC* Cmn_OnlyLoginDllApp::GetConnection()
{
	auto res = GetLoginConnection();
	if (!res) return nullptr;
	return res->CastTo<LoginConnectionC>();
}

Cmn_LoginDllData& Cmn_OnlyLoginDllApp::GetLoginDllData() const
{
	return __super::GetGameDllData().CastAs<Cmn_LoginDllData>();
}

void Cmn_OnlyLoginDllApp::__SetLocalCrackCaptcha__(const char* remote_ip, pt_word remote_port)
{
	assert(!crack_captcha_);
	auto ptr = new LocalCrackCaptcha;
	crack_captcha_ = ptr;
	if (!crack_captcha_)
	{
		assert(false);
		return;
	}
	ptr->SetServiceInfo(remote_ip, remote_port);
}

void Cmn_OnlyLoginDllApp::__SetDmtCrackCaptcha__(const char* account, const char* password)
{
	assert(!crack_captcha_);
	auto ptr = GetAppFactory().CreateDaMaTu();
	crack_captcha_ = ptr;
	if (!crack_captcha_)
	{
		//assert(false);
		return;
	}
	ptr->SetAccountPassword(account, password);
}

void Cmn_OnlyLoginDllApp::__BeginLogin__()
{
	assert(!is_begin_login_);
	auto& ac_info = GetAcInfo();
	if (ac_info.ac_.empty())
	{
		assert(false);
		return;
	}
	if (ac_info.pwd_.empty())
	{
		assert(false);
		return;
	}
	if (!crack_captcha_)
	{
		assert(false);
		return;
	}
	is_begin_login_ = true;
}

Cmn_OnlyLoginDllApp::Cmn_OnlyLoginDllApp()
{
	crack_captcha_ = nullptr;
	is_begin_login_ = false;
}

void Cmn_OnlyLoginDllApp::BeginLogin()
{
	assert(false);
}

bool Cmn_OnlyLoginDllApp::PreBeginLogin()
{
	if (!is_begin_login_)
	{
		assert(false);
		return false;
	}
	return true;
}

Cmn_OnlyLoginDllApp::~Cmn_OnlyLoginDllApp()
{
	if (crack_captcha_)
		delete crack_captcha_;
}

OnlyLoginAppFactory& Cmn_OnlyLoginDllApp::GetAppFactory() const
{
	return __super::GetAppFactory().CastAs<OnlyLoginAppFactory>();
}

LoginConnectionC* OnlyLoginAppFactory::CreateLoginConnection(pt_word dest_port)
{
	assert(dest_port);
	return new LoginConnectionC(Cmn_OnlyLoginDllApp::GetApp(), dest_port);
}

LoginConnectionOper* OnlyLoginAppFactory::CreateLoginConnectionOper(LoginConnectionC& connection)
{
	return new LoginConnectionOper(connection);
}

Cmn_LoginDllData* OnlyLoginAppFactory::CreateLoginDllData()
{
	return new Cmn_LoginDllData;
}

DaMaTu* OnlyLoginAppFactory::CreateDaMaTu()
{
	return nullptr;
}

LoginConnectionC::LoginConnectionC(Cmn_GameDllApp& app, pt_word dest_port) : CmnGameConnectionC(app, dest_port)
{

}

void LoginConnectionC::OnDisconnect(IoConnectBase* connect)
{
	auto& app = GetApp();
	app.GetIoService().post([&app](){
		app.OnLoginDisconnect_();
	});
	return __super::OnDisconnect(connect);
}

bool LoginConnectionC::PreConnect()
{
	return __super::PreConnect();
}

bool LoginConnectionC::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	GetIoService().post([](){
		auto& app = Cmn_OnlyLoginDllApp::GetApp();
		if (!app.PreBeginLogin())
		{
			assert(false);
			return;
		}
		app.BeginLogin();
	});
	return true;
}

LoginConnectionOper::LoginConnectionOper(LoginConnectionC& connection) : CmnGameConnectionOper(connection)
{

}

void LoginConnectionOper::OnCheckInfo_(pt_csz version)
{
	auto& app = connection_.GetApp();
	if (app.GetLoginDllData().dll_version_ != version)
	{
		assert(false);
		auto shared = connection_.shared_from_this();
		GetIoService().post([shared](){
			if (shared->ObjValid())
				shared->DoDisconnect();
		});
	}
}

void LoginConnectionOper::SetLocalCrackCaptcha_(const char* remote_ip, pt_word remote_port)
{
	Cmn_OnlyLoginDllApp::GetApp().__SetLocalCrackCaptcha__(remote_ip, remote_port);
}

void LoginConnectionOper::SetDmtCrackCaptcha_(const char* account, const char* password)
{
	Cmn_OnlyLoginDllApp::GetApp().__SetDmtCrackCaptcha__(account, password);
}

void LoginConnectionOper::BeginLogin_()
{
	Cmn_OnlyLoginDllApp::GetApp().__BeginLogin__();
}

Cmn_LoginDllData::Cmn_LoginDllData()
{
	port_ = P_Port_Login;
	dll_version_ = kLoginDllVersion;
}

LoginConnectionOper* LoginConnectionC::CreateConnectionOper()
{
	return Cmn_GameDllApp::GetApp().GetAppFactory().CreateLoginConnectionOper(*this);
}

void LoginConnectionC::ReloginAc()
{
	auto oper = GetConnectionOper();
	if (oper)
	{
		oper->ToCall(BC_MakeP("OnRelogin"));
	}
}
