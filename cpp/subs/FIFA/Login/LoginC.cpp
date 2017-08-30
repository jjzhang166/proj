#include "stdafx.h"
#include "LoginC.h"
#include "Net/IoTimer.h"
#include "P_LoginMsg.h"
#include "Common/Toolhelp.h"
#include "atlpath.h"
#include <Communicate/CL_Defines.h>
#include <Communicate/CGL_Defines.h>
#include <protect/Protect.h>
#include <Communicate/LLH_Defines.h>
#include <wx/wxprec.h>

LoginC::LoginC() : Connection(kSelfIpStr, P_Port_Login), json_echo_(LuaThrdData::AsRef<LuaVM>(), *this), login_oper_(*this)
{
	crack_captcha_ = &local_crack_captcha_;
	is_dmtest_ = false;
	is_input_pwd_ = false;
	is_handle_select_sever_ = false;
	handle_cnt_ = 0;
}

bool LoginC::Init()
{
	VMP_BEGIN_EX;
	if ( !CheckAssistVersion() )
	{
		assert(false);

		SetIoTimer(10 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			throw CmnExitIoThreadException("le");
			VMP_END;
		});
	}
	VMP_END;
	return true;
}

JsonEcho& LoginC::GetJsonEcho()
{
	return json_echo_;
}

void LoginC::BeginLogin()
{
	/*SetIoTimer(20, [=]( IoTimer* timer ){
		assert(timer);
		if ( 0 == this->Do_CheckSafeSubProcessId() )
			timer->Continue();
	});*/
	/*SetIoTimer(5 * 1000, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->InputAccount() )
			timer->Continue();
	});*/
	SetIoTimer(50, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->OpenMemFileMap() )
			timer->Continue();
	});
}

bool LoginC::InputAccount()
{
	if (account_.empty())
		return false;

	struct Helper{
		POINT		pt;
		Helper( LoginC* login_c ){			
			pt.x = LuaThrdData::GetMe().LuaObjCallByDlt("Login_GetAccountCtrlPosX", 0);
			pt.y = LuaThrdData::GetMe().LuaObjCallByDlt("Login_GetAccountCtrlPosY", 0);
		}
	};

	//HWND hWnd = ::FindWindow(kLoginMainClsName, kLoginMainWndName);

	// 输入账号
	HWND hWnd = GetAcWnd();
	if ( !::IsWindow(hWnd) )
		return false;
	//置顶窗口
	ForegroundWnd(hWnd);

	static Helper helper(this);

	::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));

	for ( int i = 0; i < 20; ++i )
	{
		::PostMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_DELETE, 0);
		::PostMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_BACK, 0);
	}

	for ( auto it = account_.begin(); it != account_.end(); ++it )
	{
		::PostMessage(hWnd, WM_IME_CHAR, *it, 0);
	}

	SetIoTimer(1000, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->InputPassword() )
			timer->Continue();
	});
	SetIoTimer(500, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->HandleLoginResult() )
			timer->Continue();
	});
	return true;
}

void LoginC::SetAccount_Password( const char* account, const char* password )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置帐号和密码，帐号是：" << account;
	account_ = account;
	password_ = password;
}

void LoginC::SetJoinedService( int pos_x, int pos_y )
{
	service_pos_.SetPoint(pos_x, pos_y);
}

bool LoginC::InputPassword()
{
	is_input_pwd_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_input_pwd_ = false;
	};
	if (LOBYTE(GetKeyState(VK_CAPITAL)))
		//关闭大小写锁
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}

	/*struct Helper{
		POINT		pt;
		Helper( LoginC* login_c ){			
			pt.x = login_c->lua_l_.Call<long>("Login_GetPasswordCtrlPosX", 0);
			pt.y = login_c->lua_l_.Call<long>("Login_GetPasswordCtrlPosY", 0);
		}
	};

	//HWND hWnd = ::FindWindow(kLoginMainClsName, kLoginMainWndName);
	// 输入密码 
	FINDWINDOW	theWnd = { 0 };
	memset(&theWnd, 0, sizeof(theWnd));
	_tcscpy_s(theWnd.str_szExeFile, _T("TASLogin.exe"));
	_tcscpy_s(theWnd.str_szClassName, _T("Edit"));
	theWnd.str_x = 148;
	theWnd.str_y = 23;
	theWnd.str_GWL_STYLE = 0x94010020;
	EnumPidWnd theEnumPidWnd;
	HWND hWnd = theEnumPidWnd.FindWindow(&theWnd);

	if ( !::IsWindow(hWnd) )
		return false;
	static Helper helper(this);
	::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
	DummySleep(500);
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));
	
	
	//输入密码
	//InputKeybdEvent(password_);
	for (auto v : password_)
	{
		::SendMessage(hWnd, WM_KILLFOCUS, 0, 0);
		ForegroundWnd(hWnd);
		::SetFocus(hWnd);
		InputKeybdEventChar(v);
	}
	*/
	auto info = LLH_MemFileMap::GetMe().WriteAsCommu();
	if (!info)
	{
		assert(false);
		return false;
	}
	StdEx::TStrCpy(info->pwd_, password_.c_str(), _countof(info->pwd_));
	info->begin_input_pwd_ = true;
	info->end_input_pwd_ = false;
	if (!TimerDoWaitUntil(500, 1000 * 60 * 3, [info](){
		return info->end_input_pwd_;
	}, kFunc_DoNothing))
	{
		assert(false);
		return false;
	}
	
	SetIoTimer(2000, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->LoginGame() )
			timer->Continue();
	});
	return true;
}

bool LoginC::LoginGame()
{
	LOG_ENTER_FUNC;
	
	HWND hWnd = ::FindWindow(nullptr, kLoginMainWndName);
	if ( !::IsWindow(hWnd) )
	{
		LOG_O(Log_trace) << "点击登陆游戏，失败";
		return false;
	}
	LOG_O(Log_trace) << "点击登陆游戏，成功";

	struct Helper{
		POINT		pt;
		Helper( LoginC* login_c ){			
			pt.x = LuaThrdData::GetMe().LuaObjCallByDlt("Login_BeginGameBtX", 0);
			pt.y = LuaThrdData::GetMe().LuaObjCallByDlt("Login_BeginGameBtY", 0);
		}
	};
	static Helper helper(this);

	::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));

	SetIoTimer(1000, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->HandleCaptcha() )
			timer->Continue();
	});

	StartCloseLoginAdvert();
	return true;
}

bool LoginC::SelectService()
{
	HWND hWnd = ::FindWindow(nullptr, kLoginMainWndName);
	if ( !::IsWindow(hWnd) )
		return false;

	::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(service_pos_.x, service_pos_.y));
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(service_pos_.x, service_pos_.y));

	SetIoTimer(1000, [=]( IoTimer* timer ){
		assert(timer);
		if ( !this->SelectedService() )
			timer->Continue();
	});
	return true;
}

bool LoginC::SelectedService()
{
	struct Helper{
		POINT		pt;
		Helper( LoginC* login_c ){			
			pt.x = LuaThrdData::GetMe().LuaObjCallByDlt("Login_SelectedServicePosX", 0);
			pt.y = LuaThrdData::GetMe().LuaObjCallByDlt("Login_SelectedServicePosY", 0);
		}
	};
	
	static Helper helper(this);
	static bool is_find_wnd = false;

	HWND hWnd = ::FindWindow(nullptr, kLoginMainWndName);
	if ( !::IsWindow(hWnd) )
	{
		if (is_find_wnd)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "确定选择服务区了，关闭socket";
			Disconnect();
			return true;
		}
		return false;
	}
	ForegroundWnd(hWnd);
	is_find_wnd = true;
	/*for ( int i = 0; i != 10; ++i )
	{
		::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
		::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));
	}

	Disconnect();
	return true;*/

	::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
	::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));
	return false;
}

bool LoginC::HandleCaptcha()
{
	++handle_cnt_;
	//判断是否进入到选择服务区场景
	HWND hWnd = FindServiceWnd();
	if (hWnd)
		//不需要填验证码
	{
		EndCloseLoginAdvert();
		is_handle_select_sever_ = true;
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "登陆到选择服务器界面";
		SetIoTimer(1000, [=]( IoTimer* timer ){
			assert(timer);
			if ( !this->SelectedService() )
				timer->Continue();
		});
		DMTest();
		return true;
	}
	else
	{
		if (!is_handle_select_sever_ && handle_cnt_ >= 3)
		{
			is_handle_select_sever_ = true;
			LOG_O(Log_debug) << "长时间未进入选择服务器界面，怀疑是有广告弹框了";

			//先关闭弹框
			int x = 865;
			int y = 264;
			auto hWnd = ::FindWindow(nullptr, kLoginMainWndName);
			if (::IsWindow(hWnd))
			{
				LOG_O(Log_debug) << "先关掉广告弹框";
				::SendMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(x, y));
				::SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				DummySleep(500);
				hWnd = FindServiceWnd();
				if (hWnd)
				{
					//已经进入到选择服务界面了，不需要关闭进程了
					is_handle_select_sever_ = false;
					return false;
				}
				else
				{
					DummySleep(2000);
					hWnd = FindServiceWnd();
					if (hWnd)
					{
						//已经进入到选择服务界面了，不需要关闭进程了
						is_handle_select_sever_ = false;
						return false;
					}
				}
			}

			if (!is_handle_select_sever_)
			{
				login_oper_.ThisMemCall(nullptr, "LoginUnRespond", nullptr);
				return true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/*FINDWINDOW	theWnd;
	memset(&theWnd,0,sizeof(theWnd));
	theWnd.str_hParWnd = ::FindWindow(kLoginMainClsName, kLoginMainWndName);
	_tcscpy_s(theWnd.str_szExeFile, kLoginClientName);
	_tcscpy_s(theWnd.str_szClassName, kLoginMainClsName);
	theWnd.str_x = 526;
	theWnd.str_y = 307;
	theWnd.str_GWL_STYLE = 0x0d6020000;
	theWnd.str_GWL_EXSTYLE = 0x00080000;
	theWnd.str_GCL_STYLE = 0x8;

	EnumPidWnd theEnumPidWnd;
	hWnd = theEnumPidWnd.FindWindow(&theWnd);

	if(hWnd)
	{
		CPath chaptcha_pic = GetModulePath(nullptr) + kCaptchaPicNewName;
		if ( !chaptcha_pic.FileExists() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "指定的验证码图片还不存在:\t" << (LPCSTR)CT2CA(chaptcha_pic);
			return false;
		}
		std::string captcha;
		if (crack_captcha_->GetResult((LPCSTR)CT2CA(chaptcha_pic), captcha))
		{
			struct Helper{
				POINT		pt;
				Helper( LoginC* login_c ){			
					pt.x = login_c->lua_l_.Call<long>("Login_GetCaptchaInputPosX", 0);
					pt.y = login_c->lua_l_.Call<long>("Login_GetCaptchaInputPosY", 0);
				}
			};
			static Helper helper(this);

			::PostMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
			::PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));

			const auto kCaptchaCnt = captcha.size();
			for ( std::size_t i = 0; i < kCaptchaCnt; ++i )
			{
				::PostMessage(hWnd, WM_CHAR, captcha[i], MAKELPARAM(captcha[i], WM_KEYDOWN));
			}

			::PostMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
			::PostMessage(hWnd, WM_KEYUP, VK_RETURN, 0);

			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "输入验证码：" << captcha;
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "答码失败";
		}
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "没有找到验证码对话框,重新LoginGame";
		SetIoTimer(2000, [=]( IoTimer* timer ){
			assert(timer);
			if ( !this->LoginGame() )
				timer->Continue();
		});
		return true;
	}*/

	//////////////////////////////////////////////////////////////////////////
	//CPath chaptcha_pic = GetModulePath(nullptr) + kCaptchaPicNewName;
	CPath chaptcha_pic = GetModulePath(nullptr) + kDaMaPicRelativePath + _T("pic.png");

	if ( !chaptcha_pic.FileExists() )
	{
		CPath chaptcha_pic1 = GetModulePath(nullptr) + kDaMaPicRelativePath + _T("pic1.png");
		if ( !chaptcha_pic1.FileExists() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "没有找到验证码图片,重新输入密码和LoginGame";
			SetIoTimer(2000, [=]( IoTimer* timer ){
				assert(timer);
				if ( !this->InputPassword() )
				{
					if (GetAcWnd())
						timer->Continue();
					else
					{
						SetIoTimer(2000, [=]( IoTimer* timer ){
							assert(timer);
							if ( !this->LoginGame() )
								timer->Continue();
						});
					}
				}
			});
			DMTest();
			return true;
		}
		chaptcha_pic = chaptcha_pic1;
	}
	if (handle_cnt_ > 0)
		--handle_cnt_;
	std::string captcha;
	if (crack_captcha_->GetResult((LPCSTR)CT2CA(chaptcha_pic), captcha))
	{
		VERIFY(::DeleteFile(chaptcha_pic));
		struct Helper{
			POINT		pt;
			Helper( LoginC* login_c ){			
				pt.x = LuaThrdData::GetMe().LuaObjCallByDlt("Login_GetCaptchaInputPosX", 0);
				pt.y = LuaThrdData::GetMe().LuaObjCallByDlt("Login_GetCaptchaInputPosY", 0);
			}
		};
		static Helper helper(this);

		auto kMainWnd = ::FindWindow(nullptr, kLoginMainWndName);
		if ( !kMainWnd || !::IsWindow(kMainWnd) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "输入验证码的时候，竟然找不到主窗口";
			return false;
		}
		::PostMessage(kMainWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(helper.pt.x, helper.pt.y));
		::PostMessage(kMainWnd, WM_LBUTTONUP, 0, MAKELPARAM(helper.pt.x, helper.pt.y));

		const auto kCaptchaCnt = captcha.size();
		for ( std::size_t i = 0; i < kCaptchaCnt; ++i )
		{
			::PostMessage(kMainWnd, WM_CHAR, captcha[i], MAKELPARAM(captcha[i], WM_KEYDOWN));
		}

		::PostMessage(kMainWnd, WM_KEYDOWN, VK_RETURN, 0);
		::PostMessage(kMainWnd, WM_KEYUP, VK_RETURN, 0);

		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "输入验证码：" << captcha;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "答码失败";
		DMTest();
	}
	//////////////////////////////////////////////////////////////////////////
	return false;
}

void LoginC::SetCrackCaptchaMode( int ccm, const char* account, const char* password, const char* remote_ip, WORD remote_port )
{
	crack_captcha_mode_.ccm_ = (stCrackCaptchaMode::enCrackCaptchaMode)ccm;
	if ( account && password )
	{
		crack_captcha_mode_.account_ = account;
		crack_captcha_mode_.password_ = password;
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "设置答码方式，ccm:" << crack_captcha_mode_.ccm_ << "\taccount:" << crack_captcha_mode_.account_ << 
			"\tpassword:" << crack_captcha_mode_.password_;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "设置答码方式，ccm:" << crack_captcha_mode_.ccm_;
	}

	if (remote_ip)
	{
		crack_captcha_mode_.remote_ip_ = remote_ip;
		crack_captcha_mode_.remote_port_ = remote_port;
	}

	switch (crack_captcha_mode_.ccm_)
	{
	case stCrackCaptchaMode::kCCM_Local:
		{
			local_crack_captcha_.SetServiceInfo(crack_captcha_mode_.remote_ip_.c_str(), crack_captcha_mode_.remote_port_);
			crack_captcha_ = &local_crack_captcha_;
			break;
		}
	case stCrackCaptchaMode::kCCM_DaMaTu:
		{
			da_ma_tu_.SetAccountPassword(crack_captcha_mode_.account_.c_str(), crack_captcha_mode_.password_.c_str());
			crack_captcha_ = &da_ma_tu_;
			break;
		}
	default:
		break;
	}

	if ( !crack_captcha_->Init() )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "答码初始化失败";
	}
}

DWORD LoginC::Do_CheckSafeSubProcessId()
{
	assert(false);
	struct Helper{
		Helper(){
			cur_process_id_ = ::GetCurrentProcessId();
			CString safe_check_exe = GetModulePath(nullptr);
			safe_check_exe += _T("TenProtect\\TASLogin.exe");
			CPath path_tmp(safe_check_exe);
			path_safe_check_exe_ = path_tmp;
		}
		CPath		path_safe_check_exe_;
		DWORD		cur_process_id_;
	};
	static Helper helper;
	const DWORD kCurProcessId = helper.cur_process_id_;

	DWORD res = 0;
	CToolhelp::DoProcessIdUntil(helper.path_safe_check_exe_, [kCurProcessId, &res]( DWORD process_id ){
		if ( kCurProcessId == CToolhelp::GetParentProcessID(process_id) )
		{
			res = process_id;
			return true;
		}
		return false;
	});
	if ( 0 != res )
	{
		LOG_ENTER_FUNC;
		if ( !InjectLoginHelper(res) )
		{
			LOG_O(Log_error) << "注入LoginH.dll失败";
			return res;
		}
		LOG_O(Log_trace) << "注入LoginH.dll成功";
		/*SetIoTimer(500, [=]( IoTimer* timer ){
			assert(timer);
			if ( !this->Do_SafeCheckEventExist() )
				timer->Continue();
		});*/
	}
	return res;
}

bool LoginC::Do_SafeCheckEventExist()
{
	assert(false);
	struct Helper{
		Helper(){
			/*auto str_src = GenUuidStr(kLogin_SafeCheckEventName);
			str_event_name_ = CA2CT(str_src.c_str());*/
		}

		CString			str_event_name_;
	};
	static Helper helper;
	//打开事件对象
	auto event_handle = ::OpenEvent(DELETE, FALSE, helper.str_event_name_);
	if (event_handle)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "安全检查事件对象打开成功";
		::CloseHandle(event_handle);
		SetIoTimer(2 * 1000, [=]( IoTimer* timer ){
			assert(timer);
			if ( !this->InputAccount() )
				timer->Continue();
		});
		return true;
	}
	return false;
}

bool LoginC::InjectLoginHelper( DWORD process_id )
{
	assert(false);
	//auto str_dll = GlobalSetter::GetMe().GetSelfModuleFullFilePath() + _T("LoginH.dll");
	//return !!InjectLib(process_id, str_dll);
	return false;
}

bool LoginC::OpenMemFileMap()
{
	auto& file_map = LLH_MemFileMap::GetMe();
	if (!file_map.OpenFileMap())
	{
		//LOG_O(Log_trace) << "等待打开内存映射文件";
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "打开内存映射文件成功";
	SetIoTimer(500, [this]( IoTimer* timer ){
		assert(timer);
		auto& file_map = LLH_MemFileMap::GetMe();
		auto commu = file_map.ReadAsCommu();
		if ( !commu )
		{
			assert(false);
			timer->Continue();
			return;
		}
		if ( !commu->safe_check_done_ )
		{
			timer->Continue();
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "safe check done->" << commu->safe_check_done_;

		DummySleep(300);
		SetIoTimer(500, [this]( IoTimer* timer ){
			if ( !InputAccount() )
				timer->Continue();

		});
	});
	return true;
}

bool LoginC::HandleLoginResult()
{
	auto& file_map = LLH_MemFileMap::GetMe();
	auto commu = file_map.ReadAsCommu();
	if ( !commu )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "commu为空";
		return false;
	}
	if (commu->pwd_input_error_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "帐号或密码输入错误";
		bool res = false;
		login_oper_.ThisMemCallSync(nullptr, "AcOrPwdErr", nullptr, JsonEcho::MakeResult(res, res));
		
		//throw CmnExitIoThreadException("帐号或密码输入错误");
		return true;
	}
	if (commu->ac_freeze_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "帐号被冻结";
		bool res = false;
		login_oper_.ThisMemCallSync(nullptr, "AcFreeze", nullptr, JsonEcho::MakeResult(res, res));

		//throw CmnExitIoThreadException("帐号被冻结");
		return true;
	}
	/*if (commu->login_succeed_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "登录成功";
		SetIoTimer(1000, [=]( IoTimer* timer ){
			assert(timer);
			if ( !this->SelectedService() )
				timer->Continue();
		});
		return true;
	}*/
	return false;
}

HWND LoginC::GetAcWnd()
{
	EnumPidWnd::FindInfo theWnd;
	memset(&theWnd, 0, sizeof(theWnd));
	_tcscpy_s(theWnd.str_szExeFile, _T("TASLogin.exe"));
	_tcscpy_s(theWnd.str_szClassName, _T("Edit"));
	theWnd.str_x = 148;
	theWnd.str_y = 23;
	theWnd.str_GWL_STYLE = 0x940120ef;
	EnumPidWnd theEnumPidWnd;
	auto hwnd = theEnumPidWnd.FindTheWnd(theWnd);
	if (hwnd && ::IsWindow(hwnd))
		return hwnd;
	return nullptr;
}

bool LoginC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	if ( !Init() )
		return false;

	P_Exec_BindLoginPid msg;
	msg.login_pid_ = GetCurrentProcessId();

	return UserMsg(kLoginMsg_BindLoginPid, &msg, sizeof(msg));
}

void LoginC::StartCloseLoginAdvert()
{
	//关闭广告弹窗问题太多，去掉吧。
	return;
	if (is_input_pwd_)
		return;
	LOG_O(Log_trace) << "开始关闭广告弹窗";
	if ( !close_login_advert_ )
	{
		close_login_advert_ = CreateIoTimer(300, true, [this](IoTimerPtr timer){
			StartCloseLoginAdvert();
			timer->Continue();
		});
	}
	EnumPidWnd::FindInfo theWnd;
	_tcscpy_s(theWnd.str_szExeFile, _T("Client.exe"));
	/*_tcscpy_s(theWnd.str_szClassName, _T("TWINCONTROL"));
	theWnd.str_x = 518;
	theWnd.str_y = 325;*/
	theWnd.str_GWL_STYLE = 0x94090000;
	theWnd.str_GWL_EXSTYLE = 0x00080000;
	theWnd.str_GCL_STYLE = 0x4008;
	EnumPidWnd theEnumPidWnd;
	HWND hWnd = theEnumPidWnd.FindTheWnd(theWnd);
	if (hWnd && !FindServiceWnd())
	{
		auto ac_wnd = GetAcWnd();
		if (ac_wnd)
		{
			LOG_O(Log_trace) << "找到了广告弹框";
			//assert(false);
			//::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			int x = 864;
			int y = 270;
			::SendMessage(hWnd, WM_LBUTTONDOWN, 1, MAKELPARAM(x, y));
			::SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
		}
	}

	//下面的可能没用
	EnumPidWnd::FindInfo theWnd2;
	auto parent_hwnd = theWnd2.str_hParWnd;
	theWnd2.str_hParWnd = nullptr;
	_tcscpy_s(theWnd2.str_szClassName, _T("Progman"));
	_tcscpy_s(theWnd2.str_szWndName, _T("Program Manager"));
	_tcscpy_s(theWnd2.str_szExeFile, _T("Client.exe"));
	theWnd2.str_x = 0;
	theWnd2.str_y = 0;
	theWnd2.str_GWL_STYLE = 0x96000000;
	theWnd2.str_GWL_EXSTYLE = 0x80;
	theWnd2.str_GCL_STYLE = 0x8;
	EnumPidWnd theEnumPidWnd2;
	hWnd = theEnumPidWnd2.FindTheWnd(theWnd2);
	if (hWnd)
	{
		LOG_O(Log_debug) << "发现了新的广告窗口";
		::PostMessage(hWnd,WM_CLOSE,NULL,NULL);
	}

	::PostMessage(parent_hwnd, WM_KEYDOWN, VK_RETURN, 0);
	::PostMessage(parent_hwnd, WM_KEYUP, VK_RETURN, 0);
}

void LoginC::EndCloseLoginAdvert()
{
	if (close_login_advert_)
	{
		close_login_advert_->Cancel();
		close_login_advert_.reset();
	}
}

void LoginC::SetGamePath( const std::string& game_path )
{
	auto the_path = game_path + "\\CrashDumps\\*.dmp";
	std::system(("del /f /s /q \"" + the_path + "\"").c_str());
	std::system("del /f /s /q C:\\Users\\administrator\\appData\\local\\temp\\*.tmp");
}

void LoginC::DMTest()
{
	/*using namespace Gdiplus;
	int s_width = 4 * 11.5;
	const auto dm_pic1 = LoadRcdataRes(GlobalSetter::GetMe().GetSelfModuleHandle(), _T("DM_TEST"));
	
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, dm_pic1.second);  
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
	memcpy(pmem, dm_pic1.first, dm_pic1.second);  
	IStream* pstm = nullptr;  
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
	// load from stream  
	auto pimg = Gdiplus::Image::FromStream(pstm);  
	// free/release stuff  
	GlobalUnlock(m_hMem);
	pstm->Release();  

	Gdiplus::Font font(_T("Arial"), 10, Gdiplus::FontStyleBoldItalic);


	auto pgra = Graphics::FromImage(pimg);

	pgra->DrawString()*/

	if (is_dmtest_)
		return;
	//百分率
	const double the_rate = 
#ifdef __PROJ_JIN_GANG_
	0.17;
#elif defined(__PROJ_XIAO_ER_)	//给小二的版本
	0.28;
#elif defined(__PROJ_OUTSIDE_)	//市场版
	0.30;
#elif defined(__PROJ_CLIENT_4_)
	0.40;
#endif

	const int kMax = 10000;
	if ( RandomNum(1, kMax) > kMax * the_rate )
	{
		is_dmtest_ = true;
		return;
	}
	const auto dm_pic = LoadRcdataRes(wxGetInstance(), _T("DM_TEST"));
	std::string res = "error";
	is_dmtest_ = crack_captcha_->GetResultFromBuff(dm_pic.first, dm_pic.second, res);
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "dmtest->" << res;
}

void LoginC::OnDisconnect( IoConnectBase* con )
{
	__super::OnDisconnect(con);
	//throw CmnExitIoThreadException("fjdpqkjmd");
}

HWND LoginC::FindServiceWnd()
{
	auto hwnd = ::FindWindow(_T("NetSpeedDetectorModuleWindow"), _T("NetSpeedDetectorModuleWindow"));
	if (hwnd &&::IsWindow(hwnd))
		return hwnd;
	return nullptr;
}

LoginOperC::LoginOperC( LoginC& login_c ) : login_c_(login_c), JsonEchoBase(LuaThrdData::AsRef<Lua_l>(), login_c, kObjId_LoginOper)
{

}

bool LoginOperC::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

bool LoginOperC::CheckLoginStr( const std::string& console_str )
{
	VMP_BEGIN_EX;
	bool res = false;
	if ( console_str != GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToLogin)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "验证登陆字符串失败";
		SetIoTimer(1 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			const auto* const kThrowStr = 
#ifdef _DEBUG
				"check login str error"
#else
				"clse"
#endif
				;
			assert(false);
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
		res = false;
	}
	res = true;
	ThisMemCall(nullptr, "oclvr", &JsonEcho::MakeParams(res));
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "验证登陆字符串成功";
	return res;
	VMP_END;
}

const char* LoginC::DaMaTu_FIFA::SoftKey() const
{
	return "3e6f191aa757b73fab904d9ec32ae821";
}
