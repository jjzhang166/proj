// LoginAssist.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>
#include "Addr.h"
#include <Common/EnsureCleanup.h>
#include <Communicate/LLH_Defines.h>
#include <CmnMix/Cmn_Hooker.h>
#include <boost/thread.hpp>
#include "Addr.h"

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

void CheckModule()
{
	
}

void* Hooker_CheckDone(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto& file_map = LLH_MemFileMap::GetMe();
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

	/*bool InitMsgHook();
	if (!InitMsgHook())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "init code hooker error";
		return FALSE;
	}*/
	return hooker.GetCodeOrig();
}


_declspec(naked) void HookCheckModule()
{
	__asm
	{
		pushfd
			pushad

			lea eax, CheckModule
			call eax
	}

	__asm
	{
		popad
			popfd

			XOR EAX,EAX
			POP ESI
			LEAVE
			RETN
	}
}

void OnLoginFailed( const wchar_t* login_str, UINT char_cnt )
{
	static bool enter_once = false;

	if (enter_once)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "已经进入了";
		return;
	}
	if ( !login_str || char_cnt <= 0 )
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
	str_output.Format(L"zhp:OnLoginFailed,字符个数是%d,%s", char_cnt, login_str);
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
		auto& file_map = LLH_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
#ifndef _LOG_OUTPUT_EMPTY
			OutputDebugString(_T("zhp:commu error"));
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
		str_tmp.Format(L"zhp:冻结帐号了->%s", login_str);

		OutputDebugStringW(str_tmp);
#endif
		auto& file_map = LLH_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
#ifndef _LOG_OUTPUT_EMPTY
			OutputDebugString(_T("zhp:commu error"));
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

/*
void OnCheckLoginSucceed( DWORD flag )
{
	/ *LOG_ENTER_FUNC;
	LOG_O(Log_trace) << flag;
	if ( 0 != flag )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "登陆成功";
		auto& file_map = LLH_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
			LOG_O(Log_error) << "commu is zero";
			return;
		}
		commu->login_succeed_ = true;
	}* /
}
*/

/*
_declspec(naked) void HookCheckLoginSucceed()
{
	__asm
	{
		pushfd
			pushad
			push eax
			lea eax, OnCheckLoginSucceed
			call eax
			add esp, 4
	}

	__asm
	{
		popad
			popfd

			POP ESI
			LEAVE
			RETN 0xC
	}
}
*/

void* Hooker_SetUnhooks(IHooker& hooker, JmpHookerAmbient& ambient)
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "Hooker_SetUnhooks";
	return hooker.GetCodeOrig();
}

bool InitCodeHooker()
{
	//return true;
	//安全检查完成
	{
		/*
		$ ==>    6542E3DE      8B3D 10B34465                MOV EDI, DWORD PTR DS:[<&MSVCR90.wcsncpy>; MSVCR90.wcsncpy
		$+6      6542E3E4      BB 01000000                  MOV EBX, 1
		$+B      6542E3E9      8DA424 00000000              LEA ESP, DWORD PTR SS:[ESP]
		$+12     6542E3F0      8B46 18                      MOV EAX, DWORD PTR DS:[ESI+18]
		$+15     6542E3F3      83F8 5F                      CMP EAX, 5F
		$+18     6542E3F6      0F84 A9000000                JE SecureCh.6542E4A5
		$+1E     6542E3FC      73 1A                        JNB SHORT SecureCh.6542E418
		$+20     6542E3FE      68 E0D44465                  PUSH SecureCh.6544D4E0                   ; UNICODE "正在取消扫描"
		$+25     6542E403      68 20C04565                  PUSH SecureCh.6545C020                   ; UNICODE "安全检查进行中..."
		$+2A     6542E408      FF15 D4B24465                CALL DWORD PTR DS:[<&MSVCR90._wcsicmp>]  ; MSVCR90._wcsicmp
		$+30     6542E40E      83C4 08                      ADD ESP, 8
		$+33     6542E411      85C0                         TEST EAX, EAX
		$+35     6542E413      74 03                        JE SHORT SecureCh.6542E418
		$+37     6542E415      015E 18                      ADD DWORD PTR DS:[ESI+18], EBX
		$+3A     6542E418      8B4E 18                      MOV ECX, DWORD PTR DS:[ESI+18]
		$+3D     6542E41B      68 04010000                  PUSH 104
		$+42     6542E420      8D5424 24                    LEA EDX, DWORD PTR SS:[ESP+24]
		$+46     6542E424      68 20C04565                  PUSH SecureCh.6545C020                   ; UNICODE "安全检查进行中..."
		$+4B     6542E429      52                           PUSH EDX
		$+4C     6542E42A      894C24 24                    MOV DWORD PTR SS:[ESP+24], ECX
		$+50     6542E42E      C74424 28 64000000           MOV DWORD PTR SS:[ESP+28], 64
		$+58     6542E436      FFD7                         CALL EDI
		$+5A     6542E438      83C4 0C                      ADD ESP, 0C
		$+5D     6542E43B      837E 04 00                   CMP DWORD PTR DS:[ESI+4], 0
		$+61     6542E43F      74 4F                        JE SHORT SecureCh.6542E490
		$+63     6542E441      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
		$+66     6542E444      6A FF                        PUSH -1
		$+68     6542E446      50                           PUSH EAX
		$+69     6542E447      FF15 28B04465                CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; kernel32.WaitForSingleObject
		$+6F     6542E44D      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
		$+73     6542E451      75 3D                        JNZ SHORT SecureCh.6542E490
		$+75     6542E453      8B4C24 18                    MOV ECX, DWORD PTR SS:[ESP+18]
		$+79     6542E457      3B4C24 1C                    CMP ECX, DWORD PTR SS:[ESP+1C]
		$+7D     6542E45B      75 17                        JNZ SHORT SecureCh.6542E474
		$+7F     6542E45D      68 04010000                  PUSH 104
		$+84     6542E462      8D5424 24                    LEA EDX, DWORD PTR SS:[ESP+24]
		$+88     6542E466      68 D0D44465                  PUSH SecureCh.6544D4D0                   ; UNICODE "安全检查完成"
		$+8D     6542E46B      52                           PUSH EDX
		$+8E     6542E46C      895E 08                      MOV DWORD PTR DS:[ESI+8], EBX
		$+91     6542E46F      FFD7                         CALL EDI
		$+93     6542E471      83C4 0C                      ADD ESP, 0C
		$+96     6542E474      8B4E 1C                      MOV ECX, DWORD PTR DS:[ESI+1C]
		$+99     6542E477      85C9                         TEST ECX, ECX
		$+9B     6542E479      74 0B                        JE SHORT SecureCh.6542E486
		$+9D     6542E47B      8B01                         MOV EAX, DWORD PTR DS:[ECX]
		$+9F     6542E47D      8B00                         MOV EAX, DWORD PTR DS:[EAX]
		$+A1     6542E47F      8D5424 10                    LEA EDX, DWORD PTR SS:[ESP+10]
		$+A5     6542E483      52                           PUSH EDX
		$+A6     6542E484      FFD0                         CALL EAX
		$+A8     6542E486      8B4E 14                      MOV ECX, DWORD PTR DS:[ESI+14]
		$+AB     6542E489      51                           PUSH ECX
		$+AC     6542E48A      FF15 2CB04465                CALL DWORD PTR DS:[<&KERNEL32.SetEvent>] ; kernel32.SetEvent
		$+B2     6542E490      68 04010000                  PUSH 104
		$+B7     6542E495      FF15 60B14465                CALL DWORD PTR DS:[<&KERNEL32.Sleep>]    ; kernel32.Sleep
		$+BD     6542E49B      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
		$+C1     6542E49F    ^ 0F84 4BFFFFFF                JE SecureCh.6542E3F0
		$+C7     6542E4A5      8B8C24 34040000              MOV ECX, DWORD PTR SS:[ESP+434]
		$+CE     6542E4AC      5F                           POP EDI                                  ; 03C978F7
		$+CF     6542E4AD      5E                           POP ESI                                  ; 03C978F7
		$+D0     6542E4AE      5B                           POP EBX                                  ; 03C978F7
		$+D1     6542E4AF      33CC                         XOR ECX, ESP
		$+D3     6542E4B1      33C0                         XOR EAX, EAX
		$+D5     6542E4B3      E8 E0880100                  CALL SecureCh.65446D98
		$+DA     6542E4B8      8BE5                         MOV ESP, EBP
		$+DC     6542E4BA      5D                           POP EBP                                  ; 03C978F7
		$+DD     6542E4BB      C2 0400                      RETN 4
		$+E0     6542E4BE      CC                           INT3
		$+E1     6542E4BF      CC                           INT3
		$+E2     6542E4C0      837E 04 00                   CMP DWORD PTR DS:[ESI+4], 0
		$+E6     6542E4C4      74 54                        JE SHORT SecureCh.6542E51A
		$+E8     6542E4C6      85FF                         TEST EDI, EDI
		$+EA     6542E4C8      74 50                        JE SHORT SecureCh.6542E51A
		$+EC     6542E4CA      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
		$+EF     6542E4CD      6A FF                        PUSH -1
		$+F1     6542E4CF      50                           PUSH EAX
		$+F2     6542E4D0      FF15 28B04465                CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; kernel32.WaitForSingleObject
		$+F8     6542E4D6      837E 08 00                   CMP DWORD PTR DS:[ESI+8], 0
		$+FC     6542E4DA      75 3E                        JNZ SHORT SecureCh.6542E51A
		$+FE     6542E4DC      8B4F 08                      MOV ECX, DWORD PTR DS:[EDI+8]
		$+101    6542E4DF      3B4F 0C                      CMP ECX, DWORD PTR DS:[EDI+C]
		$+104    6542E4E2      75 1E                        JNZ SHORT SecureCh.6542E502
		$+106    6542E4E4      68 04010000                  PUSH 104
		$+10B    6542E4E9      8D57 10                      LEA EDX, DWORD PTR DS:[EDI+10]
		$+10E    6542E4EC      68 D0D44465                  PUSH SecureCh.6544D4D0                   ; UNICODE "安全检查完成"
		$+113    6542E4F1      52                           PUSH EDX
		$+114    6542E4F2      C746 08 01000000             MOV DWORD PTR DS:[ESI+8], 1
		$+11B    6542E4F9      FF15 10B34465                CALL DWORD PTR DS:[<&MSVCR90.wcsncpy>]   ; MSVCR90.wcsncpy
		$+121    6542E4FF      83C4 0C                      ADD ESP, 0C								;要hook这里
		$+124    6542E502      8B4E 1C                      MOV ECX, DWORD PTR DS:[ESI+1C]
		$+127    6542E505      85C9                         TEST ECX, ECX
		$+129    6542E507      74 07                        JE SHORT SecureCh.6542E510
		$+12B    6542E509      8B01                         MOV EAX, DWORD PTR DS:[ECX]
		$+12D    6542E50B      8B10                         MOV EDX, DWORD PTR DS:[EAX]
		$+12F    6542E50D      57                           PUSH EDI
		$+130    6542E50E      FFD2                         CALL EDX
		$+132    6542E510      8B46 14                      MOV EAX, DWORD PTR DS:[ESI+14]
		$+135    6542E513      50                           PUSH EAX
		$+136    6542E514      FF15 2CB04465                CALL DWORD PTR DS:[<&KERNEL32.SetEvent>] ; kernel32.SetEvent
		$+13C    6542E51A      C3                           RETN
		*/
		auto tcls = LoadLibrary(GetModulePath(nullptr) + _T("SecureCheck.dll"));
		assert(tcls);
		if ( !tcls )
		{
			return false;
		}
		//Hook((DWORD)tcls + g_o_HookSafeCheckDone, HookCheckModule);
		HOOKER_SetCodeHooker(Hooker_CheckDone, (char*)tcls + g_o_HookSafeCheckDone,
			Hooker_CheckDone);
	}
	//登陆失败的判断
	{
		auto oleaut32 = LoadLibrary(_T("oleaut32.dll"));
		assert(oleaut32);
		if ( !oleaut32 )
			return false;
		g_pCheckLoginFailed = GetProcAddress(oleaut32, "SysAllocStringLen");
		if ( !g_pCheckLoginFailed )
			return false;
		Hook((DWORD)g_pCheckLoginFailed, HookCheckLoginResult);
	}
	//登陆成功的判断
	/*{
		auto ssoc = LoadLibrary(_T("SSOCommon.dll"));
		if ( !ssoc )
			return false;
		Hook((DWORD)ssoc + g_o_login_succeed, HookCheckLoginSucceed);
	}*/
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "login helper hook成功";
	/*struct tagUnhooks{};
	static auto hooker_unhook_hooks = HookerFactory::GetMe().HOOKER_CreateCodeHooker(tagUnhooks, (char*)UnhookWindowsHookEx,
		Hooker_SetUnhooks, true);*/
	return true;
}

HWND g_hPwd = nullptr;
const UINT_PTR kPwdTimerId = 0x1000;
UINT_PTR g_hTimer = 0;
CRITICAL_SECTION g_cs;
KBDLLHOOKSTRUCT pKdb = { 0 };

void InputPwd(HOOKPROC next_proc, char c)
{
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
VOID CALLBACK InputPwdTimer(_In_  HWND hwnd, _In_  UINT uMsg, _In_  UINT_PTR idEvent, _In_  DWORD dwTime)
{
	auto& file_map = LLH_MemFileMap::GetMe();
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
	EnterCriticalSection(&g_cs);
	__try
	{
		static bool init_once = false;
		if (!init_once)
		{
			auto dwModuleHanlde = (DWORD)GetModuleHandle(_T("fszwd.dat"));
			if (!dwModuleHanlde)
			{
				assert(false);
				__leave;
			}
			game_hook_call_back = (HOOKPROC)(dwModuleHanlde + 0x1460BC);
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
		//DoLBtDown(game_hook_call_back);

		for (int i = 0;i != _countof(info->pwd_); ++i)
		{
			if (!info->pwd_[i])
				break;
			InputPwd(game_hook_call_back, info->pwd_[i]);
		}		
		info->end_input_pwd_ = true;
		//KillTimer(g_hPwd, kPwdTimerId);
	}
	__finally{
		LeaveCriticalSection(&g_cs);
	}	
}
bool hook_executed = false;
HHOOK g_hHookId = nullptr;
LRESULT CALLBACK GetMsgProc(__in  int code, __in  WPARAM wParam, __in  LPARAM lParam)
{
	LOG_O(Log_trace) << "GetMsgProc";
	if (code == HC_ACTION)
	{
		if (!g_hPwd)
		{
			LOG_O(Log_trace) << "init g_hPwd";
			auto pMsg = (MSG*)lParam;
			if (pMsg)
			{
				DWORD process_id = 0;
				GetWindowThreadProcessId(pMsg->hwnd, &process_id);
				if (process_id == GlobalSetter::GetMe().GetCurProcessId())
				{
					LONG dwStyle = GetWindowLong(pMsg->hwnd, GWL_STYLE);
					if (dwStyle & ES_PASSWORD && !(dwStyle & ES_MULTILINE))
					{
						g_hPwd = pMsg->hwnd;
						if (!g_hTimer)
							g_hTimer = SetTimer(g_hPwd, kPwdTimerId, 800, InputPwdTimer);
						assert(g_hTimer);
						if (g_hHookId)
						{
							hook_executed = true;
							UnhookWindowsHookEx(g_hHookId);
							g_hHookId = nullptr;
						}
					}
				}				
			}
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

void MsgThread(){
	EnumPidWnd::FindInfo theWnd;
	_tcscpy_s(theWnd.str_szExeFile, _T("TASLogin.exe"));
	_tcscpy_s(theWnd.str_szClassName, _T("Edit"));
	theWnd.str_x = 148;
	theWnd.str_y = 23;
	theWnd.str_GWL_STYLE = 0x94010020;	
	theWnd.pid_ = GetCurrentProcessId();
	while (!hook_executed)
	{
		EnumPidWnd theEnumPidWnd;
		HWND hWnd = theEnumPidWnd.FindTheWnd(theWnd);
		if (hWnd && ::IsWindow(hWnd))
		{
			/*if (g_hHookId)
			{
				LOG_O(Log_trace) << "Unhook res:" << UnhookWindowsHookEx(g_hHookId);
			}
			auto t_id = GetWindowThreadProcessId(hWnd, nullptr);
			auto& gs = GlobalSetter::GetMe();
			g_hHookId = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, gs.GetSelfModuleHandle(), t_id);
			if (!g_hHookId)
			{
				assert(false);
				return;
			}
			LOG_O(Log_trace) << "SetWindowHook res:" << (void*)g_hHookId;
			break;
			::Sleep(1000);*/
			if (!g_hTimer)
				g_hTimer = SetTimer(hWnd, kPwdTimerId, 500, InputPwdTimer);
			assert(g_hTimer);
			if (g_hTimer)
				break;
		}
		::Sleep(1000);
	}
	LOG_O(Log_trace) << "SetTimer终于成功了";
}

bool InitMsgHook(){
	boost::thread t(MsgThread);
	/*auto& gs = GlobalSetter::GetMe();
	g_hHookId = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, gs.GetSelfModuleHandle(), 0);
	if (!g_hHookId)
	{
		assert(false);
		return false;
	}*/
	InitializeCriticalSection(&g_cs);
	return true;
}