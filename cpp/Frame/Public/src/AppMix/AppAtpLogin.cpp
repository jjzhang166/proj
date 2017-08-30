#include "stdafx.h"
#include <AppMix/AppAtpLogin.h>
#include <CmnMix/Cmn_LogOut.h>
#include <AtpMiX/AtpLogin.h>
#include <Common/UsefulClass.h>
#include <CmnMix/Cmn_Toolhelp.h>

//////////////////////////////////////////////////////////////////////////
const wchar_t* const kDaMaPicRelativePath = L"TenProtect\\ui\\res\\";
//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(AtpLoginConnectionOper){
	bufcall::class_<AtpLoginConnectionOper, LoginConnectionOper>("AtpLoginConnectionOper")
		;
}

AtpLoginConnectionOper::AtpLoginConnectionOper(LoginConnectionC& connection) : LoginConnectionOper(connection)
{
}

AtpAppLogin& AtpAppLogin::GetApp()
{
	return __super::GetApp().CastAs<AtpAppLogin>();
}

AtpAppLogin::AtpAppLogin()
{
	login_client_main_wnd_ = nullptr;
}

AtpAppLogin::~AtpAppLogin()
{
}

AtpAppLoginFactory& AtpAppLogin::GetAppFactory() const
{
	return __super::GetAppFactory().CastAs<AtpAppLoginFactory>();
}

AtpAppLoginFactory* AtpAppLogin::CreateAppFactory()
{
	return new AtpAppLoginFactory;
}

void AtpAppLogin::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)[
		class_<AtpAppLogin, Cmn_OnlyLoginDllApp>("AtpAppLogin")
	];
}

void AtpAppLogin::BeginLogin()
{
	ClearPreLogin_();
	if (!WaitSafeCheckDone())
	{
		LOG_O(Log_debug) << "等待安全检查失败";
		assert(false);
		return;
	}

	auto commu_info = Atp_MemFileMap::GetMe().ReadAsCommu();
	if (!commu_info)
	{
		assert(false);
		return;
	}
	SetIoTimer1(100, [commu_info, this](IoTimer& timer){
		CheckToRelogin();
		if (commu_info->pwd_input_error_)
		{
			LOG_O(Log_debug) << "帐号或密码错误";
			auto connect = GetConnection();
			if (!connect)
			{
				assert(false);
				return;
			}
			connect->SendAcPwdErr();
			return;
		}
		else if (commu_info->ac_freeze_)
		{
			LOG_O(Log_debug) << "帐号被冻结";
			auto connect = GetConnection();
			if (!connect)
			{
				assert(false);
				return;
			}
			connect->SendFreeze();
			return;
		}
		timer.Continue();
	});

	if (!BeginInputPwd())
	{
		LOG_O(Log_debug) << "输入密码失败";
		assert(false);
		return;
	}
	if (!LoginGame())
	{
		LOG_O(Log_debug) << "登陆游戏失败";
		assert(false);
		return;
	}
	if (!StartGame())
	{
		LOG_O(Log_debug) << "开始游戏失败";
		assert(false);
		return;
	}
}

bool AtpAppLogin::WaitSafeCheckDone()
{
	LOG_O(Log_trace) << "开始等待安全检查完成";
	auto& file_map = Atp_MemFileMap::GetMe();
	if (!TimerDo(100, 1000 * 60 * 10, [&file_map](){
		return file_map.OpenFileMap();
	}))
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "已打开文件映射对象";
	if (!TimerDo(100, 1000 * 60 * 10, [&file_map, this](){
		CheckToRelogin();
		auto commu = file_map.ReadAsCommu();
		if (!commu)
		{
			assert(false);
			return false;
		}
		return commu->safe_check_done_;
	}))
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "安全检查已完成，等待结束";
	return true;
}

bool AtpAppLogin::BeginInputPwd()
{
	LOG_O(Log_trace) << "开始输入密码";
	if (LOBYTE(GetKeyState(VK_CAPITAL)))
		//关闭大小写锁
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
	auto info = Atp_MemFileMap::GetMe().WriteAsCommu();
	if (!info)
	{
		assert(false);
		return false;
	}
	auto main_wnd = GetLoginClientMainWnd();
	assert(main_wnd);
	ForegroundWnd(main_wnd);

	auto& ac_info = GetAcInfo();
	StdEx::TStrCpy(info->pwd_, ac_info.pwd_.c_str(), ac_info.pwd_.size() + 1);
	info->begin_input_pwd_ = true;
	info->end_input_pwd_ = false;
	if (!TimerDo(100, 1000 * 60 * 5, [this, info, main_wnd](){
		CheckToRelogin();
		if (info->input_pwd_has_except_)
		{
			ForegroundWnd(main_wnd);
			::Sleep(80);
			keybd_event(VK_DOWN, 0, 0, 0);
			keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
			::Sleep(100);
			info->input_pwd_has_except_ = false;
		}
		return info->end_input_pwd_;
	}))
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "密码输入完毕";
	return true;
}

WXHWND AtpAppLogin::GetLoginClientMainWnd() const
{
	if (login_client_main_wnd_)
		return login_client_main_wnd_;
	EnumPidWnd enum_wnd;
	EnumPidWnd::FindInfo find_info;
	find_info.m_bIsWindowVisible = TRUE;
	find_info.pid_ = ::wxGetProcessId();
	_tcscpy_s(find_info.str_szWndName, GetLoginClientWndTitle());
	login_client_main_wnd_ = enum_wnd.FindTheWnd(find_info);
	return login_client_main_wnd_;
}

bool AtpAppLogin::LoginGame()
{
	LOG_O(Log_trace) << "开始登陆游戏";
	PressReturnKey();
	while (true)
	{
		if (EnteredServiceScene())
			break;
		CheckToRelogin();
		HandleAdvert();
		HandleCaptcha();
		DummySleep(200);
	}
	LOG_O(Log_trace) << "登陆游戏成功";
	return true;
}

AtpAppLogin::enHandleCaptchaRes AtpAppLogin::HandleCaptcha()
{
	if (!crack_captcha_)
	{
		assert(false);
		return kHCR_Error;
	}
	using namespace boost::filesystem;
	path pic_dir = GetModulePath(nullptr);
	pic_dir /= kDaMaPicRelativePath;
	auto pic_path = pic_dir / L"pic.png";
	if (!exists(pic_path))
	{
		pic_path = pic_dir / L"pic1.png";
		if (!exists(pic_path))
			return kHCR_HasNotCaptcha;
	}
	std::string captcha;
	if (!crack_captcha_->GetResult(pic_path.string(), captcha))
		return kHCR_CaptchaFailed;
	if (captcha.empty())
	{
		assert(false);
		return kHCR_CaptchaFailed;
	}
	remove(pic_path);
	auto main_wnd = GetLoginClientMainWnd();
	assert(main_wnd);
	const auto& the_pos = GetCaptchaInputPos();
	::SendMessage(main_wnd, WM_LBUTTONDOWN, 1, MAKELPARAM(the_pos.x, the_pos.y));
	::SendMessage(main_wnd, WM_LBUTTONUP, 0, MAKELPARAM(the_pos.x, the_pos.y));

	LOG_O(Log_trace) << "开始答码：" << captcha;
	const auto kCaptchaCnt = captcha.size();
	for (std::size_t i = 0; i < kCaptchaCnt; ++i)
		::SendMessage(main_wnd, WM_CHAR, captcha[i], MAKELPARAM(captcha[i], WM_KEYDOWN));

	ForegroundWnd(main_wnd);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	return kHCR_HandleSucceed;
}

void AtpAppLogin::ClearPreLogin_()
{
	using namespace boost::filesystem;
	path pic_dir = GetModulePath(nullptr);
	pic_dir /= kDaMaPicRelativePath;
	auto pic_path = pic_dir / L"pic.png";
	if (exists(pic_path))
		remove(pic_path);
	pic_path = pic_dir / L"pic1.png";
	if (exists(pic_path))
		remove(pic_path);
}

void AtpAppLogin::HandleAdvert()
{

}

bool AtpAppLogin::StartGame()
{
	LOG_O(Log_trace) << "开始启动游戏";
	while (EnteredServiceScene())
	{
		PressReturnKey();
		DummySleep(2000);
		if (!GetConnection())
			break;
	}
	//LOG_O(Log_trace) << "启动游戏成功";
	return true;
}

void AtpAppLogin::PressReturnKey()
{
	auto main_wnd = GetLoginClientMainWnd();
	assert(main_wnd);
	ForegroundWnd(main_wnd);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

bool AtpAppLogin::PreBeginLogin()
{
	if (!__super::PreBeginLogin())
	{
		assert(false);
		return false;
	}
	if (!TimerDo(500, 1000 * 60 * 10, [this](){
		if (GetLoginClientMainWnd())
			return true;
		return false;
	}))
	{
		LOG_O(Log_debug) << "没有找到登陆进行的主窗口";
		assert(false);
		return false;
	}
	auto& ac_info = GetAcInfo();
	LOG_O(Log_trace) << "开始登陆游戏，帐号：" << ac_info.ac_ << ",服务器：" << ac_info.ser_
		<< "，登陆进程的主窗口句柄：" << GetLoginClientMainWnd();
	return true;
}

void AtpAppLogin::ReloginAc()
{
	auto c = GetConnection();
	if (c)
		c->ReloginAc();
}

AtpLoginConnection* AtpAppLogin::GetConnection()
{
	return (AtpLoginConnection*)__super::GetConnection();
}

bool AtpAppLogin::IsTASLoginValid() const
{
	std::vector<DWORD> the_pids;
	CToolhelp::DoProcessIdUntilAsName(CPath(_T("TASLogin.exe")), [&the_pids](DWORD pid){
		the_pids.push_back(pid);
		return false;
	});
	if (the_pids.size() != 1)
		return false;
	auto the_pid = the_pids[0];
	auto my_pid = GetCurrentProcessId();
	if (CToolhelp::GetParentProcessID(the_pid) != my_pid)
		return false;
	return true;
}

void AtpAppLogin::CheckToRelogin()
{
	if (IsTASLoginValid())
		return;
	LOG_O(Log_debug) << "TASLogin不正常了";
	ReloginAc();
	DummySleep(1000 * 60 * 30);
}

AtpLoginConnectionOper* AtpAppLoginFactory::CreateLoginConnectionOper(LoginConnectionC& connection)
{
	return new AtpLoginConnectionOper(connection);
}

DaMaTu* AtpAppLoginFactory::CreateDaMaTu()
{
	struct MyDaMaTu : DaMaTu{
		const char* SoftKey() const override{
			return "3e6f191aa757b73fab904d9ec32ae821";
		}
	};
	return new MyDaMaTu;
}

AtpLoginConnection* AtpAppLoginFactory::CreateLoginConnection(pt_word dest_port)
{
	assert(dest_port);
	return new AtpLoginConnection(AtpAppLogin::GetApp(), dest_port);
}

AtpLoginConnection::AtpLoginConnection(Cmn_GameDllApp& app, pt_word dest_port) : LoginConnectionC(app, dest_port)
{
}
