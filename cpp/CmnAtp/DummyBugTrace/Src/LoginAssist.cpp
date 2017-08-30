// LoginAssist.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <CmnMix/Cmn_LogOut.h>
#include "LogicHandler.h"
#include <CmnMix/Cmn_EnsureCleanup.h>
#include <AtpMix/AtpLogin.h>
#include <CmnMix/Cmn_Hooker.h>
#include <boost/thread.hpp>

void Hook(DWORD dwHookAddr,LPVOID pfun)
{
	try
	{
		unsigned char code = 0xe9;
		DWORD dwDoHookAddr = (DWORD)(void*)pfun;
		DWORD dwcAddr = dwDoHookAddr - dwHookAddr - 1 - 4;

		// 判断当前地址是否被hook
		unsigned char readcode;
		ReadProcessMemory((void*)-1,(void*)dwHookAddr,&readcode,sizeof(readcode),NULL);

		if( readcode == code )
		{
			DWORD	dwReadAddr = 0;
			ReadProcessMemory((void*)-1,(void*)(dwHookAddr+1),&dwReadAddr,sizeof(dwReadAddr),NULL);

			if( dwReadAddr == dwcAddr )
			{
				return ;
			}
		}

		WriteProcessMemory((void*)-1,(void*)dwHookAddr,&code,1,NULL);

		WriteProcessMemory((void*)-1,(void*)(dwHookAddr+1),&dwcAddr,4,NULL);
	}
	catch (...)
	{
		MessageBoxA(NULL,"哒哒哒","啊啊啊",MB_OK);
	}
}

void* Hooker_CheckDone(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto& file_map = Atp_MemFileMap::GetMe();
	auto commu = file_map.WriteAsCommu();
	if (!commu)
	{
		LOG_O(Log_error) << "commu is zero";
		return hooker.GetCodeOrig();
	}
	if (commu->safe_check_done_)
		return hooker.GetCodeOrig();
	commu->safe_check_done_ = true;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "安全模块检查完成";
	return hooker.GetCodeOrig();
}

void OnLoginFailed(const wchar_t* login_str, UINT char_cnt)
{
	static bool enter_once = false;

	if (enter_once)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "已经进入了";
		return;
	}
	if (!login_str || char_cnt <= 0)
		return;
	enter_once = true;
	auto enter_ptr = &enter_once;
	auto func = [enter_ptr](){
		*enter_ptr = false;
	};
	CleanAtScopeExit<decltype(func)> at_exit(func);
	
	
	/*
#ifndef _LOG_OUTPUT_EMPTY
	CStringW str_output;
	str_output.Format(L"flog:OnLoginFailed,字符个数是%d,%s", char_cnt, login_str);
	OutputDebugStringW(str_output);
#endif
*/

	static const wchar_t kPwdError[] = L"密码不正确";
	static const wchar_t kFreezeAc[] = L"您的QQ暂时无法登录";
	static const wchar_t kFreezeAc2[] = L"已经冻结";

	const auto kLoginStrEnd = login_str + char_cnt;
	auto it = std::search(login_str, kLoginStrEnd, kPwdError, kPwdError + _countof(kPwdError) - 1);
	if ( it != kLoginStrEnd )
	{
		//LOG_ENTER_FUNC;
		//LOG_O(Log_trace) << "密码错误,密码是->" << login_str;
#ifndef _LOG_OUTPUT_EMPTY
		CStringW str_tmp;
		str_tmp.Format(L"flog:密码不正确，密码是->%s", login_str);

		OutputDebugStringW(str_tmp);
#endif		
		auto& file_map = Atp_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
#ifndef _LOG_OUTPUT_EMPTY
			OutputDebugString(_T("flog:commu error"));
#endif
			return;
		}
		commu->pwd_input_error_ = true;
		return;
	}
	it = std::search(login_str, kLoginStrEnd, kFreezeAc, kFreezeAc + _countof(kFreezeAc) - 1);
	if (it == kLoginStrEnd)
		it = std::search(login_str, kLoginStrEnd, kFreezeAc2, kFreezeAc2 + _countof(kFreezeAc2) - 1);
	if ( it != kLoginStrEnd )
	{
#ifndef _LOG_OUTPUT_EMPTY
		CStringW str_tmp;
		str_tmp.Format(L"flog:冻结帐号了->%s", login_str);

		OutputDebugStringW(str_tmp);
#endif
		auto& file_map = Atp_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
#ifndef _LOG_OUTPUT_EMPTY
			OutputDebugString(_T("flog:commu error"));
#endif
			return;
		}
		commu->ac_freeze_ = true;
		return;
	}
}

_declspec(naked) void HookCheckLoginResult()
{
	__asm
	{
		pushfd
		pushad
		mov ebp, esp
		push [ebp+0x2c]
		push [ebp+0x28]
		lea eax, OnLoginFailed
		call eax
		add esp, 8
	}

	__asm
	{
		popad
		popfd

		mov edi,edi
		push ebp
		mov ebp, esp

		mov eax, g_pCheckLoginFailed
		add eax, 5
		jmp eax
	}
}

bool InitCodeHooker(const stAtpDummyBugTrace& param)
{
	//return true;
	if (!param.h_safe_check_done_)
	{
		assert(false);
		return false;
	}
	//安全检查完成
	{
		auto tcls = LoadLibrary(GetModulePath(nullptr) + _T("SecureCheck.dll"));
		assert(tcls);
		if ( !tcls )
		{
			return false;
		}
		HOOKER_SetCodeHooker(Hooker_CheckDone, (char*)tcls + param.h_safe_check_done_, Hooker_CheckDone);
	}
	//登陆失败的判断
	{
		auto oleaut32 = LoadLibrary(_T("oleaut32.dll"));
		assert(oleaut32);
		if (!oleaut32)
			return false;
		g_pCheckLoginFailed = GetProcAddress(oleaut32, "SysAllocStringLen");
		if (!g_pCheckLoginFailed)
			return false;
		Hook((DWORD)g_pCheckLoginFailed, HookCheckLoginResult);
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "login helper hook成功";
	return true;
}
//////////////////////////////////////////////////////////////////////////
const UINT_PTR kPwdTimerId = 0x5555;
UINT_PTR g_hTimer = 0;
//HWND g_wnd_edit_ac = nullptr;
CRITICAL_SECTION g_cs;
KBDLLHOOKSTRUCT pKdb = { 0 };

void InputPwd(HOOKPROC next_proc, char c)
{
	//LOG_O(Log_trace) << "InputPwd,Char:" << c;
	const auto& vk_info = Ascii2VK(c);
	if (vk_info.first)
	{
		std::memset(&pKdb, 0, sizeof(pKdb));
		pKdb.vkCode = VK_SHIFT;
		//LOG_O(Log_trace) << "VK_SHIFT begin down";
		next_proc(HC_ACTION, WM_KEYDOWN, (LPARAM)&pKdb);
		//LOG_O(Log_trace) << "VK_SHIFT end down";
	}
	std::memset(&pKdb, 0, sizeof(pKdb));
	pKdb.vkCode = vk_info.second;
	//LOG_O(Log_trace) << "begin down->" << (char)pKdb.vkCode;
	next_proc(HC_ACTION, WM_KEYDOWN, (LPARAM)&pKdb);

	std::memset(&pKdb, 0, sizeof(pKdb));
	pKdb.vkCode = vk_info.second;
	//LOG_O(Log_trace) << "begin up->" << (char)pKdb.vkCode;
	next_proc(HC_ACTION, WM_KEYUP, (LPARAM)&pKdb);
	//LOG_O(Log_trace) << "end up->" << (char)pKdb.vkCode;

	if (vk_info.first)
	{
		std::memset(&pKdb, 0, sizeof(pKdb));
		pKdb.vkCode = VK_SHIFT;
		//LOG_O(Log_trace) << "VK_SHIFT begin up->" << (char)pKdb.vkCode;
		next_proc(HC_ACTION, WM_KEYUP, (LPARAM)&pKdb);
		//LOG_O(Log_trace) << "VK_SHIFT end up->" << (char)pKdb.vkCode;
	}
}
void DebugTimer(){
	LOG_O(Log_trace) << "login pwd timer";
}

void DebugMsgHook(HOOKPROC hook){
	LOG_O(Log_trace) << "得到MsgHook的地址：" << hook;
}

void CALLBACK InputPwdTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	auto& file_map = Atp_MemFileMap::GetMe();
	auto info = file_map.WriteAsCommu();
	if (!info)
	{
		assert(false);
		return;
	}
	if (!info->begin_input_pwd_)
		return;
	if (info->end_input_pwd_)
		return;
	if (info->input_pwd_has_except_)
		return;
	auto& gs = GlobalSetter::GetMe();
	if (!gs.params_.o_msg_hook_proc_)
		return;
	
	EnterCriticalSection(&g_cs);
	bool has_except = true;
	__try
	{
		static bool init_once = false;
		static HOOKPROC game_hook_call_back = nullptr;
		if (!init_once)
		{
			auto dwModuleHanlde = (DWORD)GetModuleHandle(_T("fszwd.dat"));
			if (!dwModuleHanlde)
			{
				assert(false);
				__leave;
			}
			game_hook_call_back = (HOOKPROC)(dwModuleHanlde + gs.params_.o_msg_hook_proc_);
#ifndef _LOG_OUTPUT_EMPTY
			DebugMsgHook(game_hook_call_back);
#endif
			init_once = true;
		}
		if (!game_hook_call_back)
		{
			assert(false);
			__leave;
		}
		DebugTimer();
		::SendMessage(hwnd, WM_KILLFOCUS, 0, 0);
		ForegroundWnd(hwnd);
		::SetFocus(hwnd);

		has_except = true;

		for (int i = 0;i != _countof(info->pwd_); ++i)
		{
			if (!info->pwd_[i])
				break;
			//输入密码前必须先点一下其他窗口（比如帐号窗口）才行，否则会抛出异常的
			InputPwd(game_hook_call_back, info->pwd_[i]);
		}		
		info->end_input_pwd_ = true;

		has_except = false;
	}
	__finally{
		if (has_except)
		{
			info->input_pwd_has_except_ = true;
#ifndef _LOG_OUTPUT_EMPTY
			OutputDebugString(_T("flog:InputPwdTimer输入密码时发生异常"));
#endif
			::Sleep(200);
			//KillTimer(hwnd, idEvent);
		}
		LeaveCriticalSection(&g_cs);
	}	
}

void MsgThread(){
	auto& gs = GlobalSetter::GetMe();
	while (true)
	{
		if (gs.params_.edit_pwd_pos_x_ || gs.params_.edit_pwd_pos_y_ || gs.params_.edit_pwd_gwl_style_)
			break;
		::Sleep(500);
	}

	EnumPidWnd::FindInfo theWnd;
	theWnd.m_bIsWindowVisible = TRUE;
	_tcscpy_s(theWnd.str_szExeFile, _T("TASLogin.exe"));
	_tcscpy_s(theWnd.str_szClassName, _T("Edit"));
	theWnd.pid_ = GetCurrentProcessId();
	
	theWnd.str_x = gs.params_.edit_pwd_pos_x_;
	theWnd.str_y = gs.params_.edit_pwd_pos_y_;
	theWnd.str_GWL_STYLE = gs.params_.edit_pwd_gwl_style_;

	HWND pwd_wnd = nullptr;
	EnumPidWnd theEnumPidWnd;
	while (true)
	{
		pwd_wnd = theEnumPidWnd.FindTheWnd(theWnd);
		if (pwd_wnd && ::IsWindow(pwd_wnd))
			break;
		::Sleep(1000);
	}
	if (!pwd_wnd)
	{
		assert(false);
		return;
	}

	/*theWnd.str_x = 0;
	theWnd.str_y = 0;
	theWnd.str_GWL_STYLE = 0;
	while (true)
	{
		g_wnd_edit_ac = theEnumPidWnd.FindTheWndBut(theWnd, my_pid, pwd_wnd);
		if (g_wnd_edit_ac && ::IsWindow(g_wnd_edit_ac))
			break;
		::Sleep(1000);
	}*/
	g_hTimer = SetTimer(pwd_wnd, kPwdTimerId, 800, InputPwdTimer);
	assert(g_hTimer);
	LOG_O(Log_trace) << "SetTimer终于成功了，timer句柄：" << g_hTimer;
}

bool InitMsgHook(){
	boost::thread t(MsgThread);
	InitializeCriticalSection(&g_cs);
	return true;
}
