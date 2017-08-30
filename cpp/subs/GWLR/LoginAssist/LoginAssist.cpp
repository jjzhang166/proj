// LoginAssist.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>
#include "Addr.h"
#include <Common/EnsureCleanup.h>
#include <Communicate/LLH_Defines.h>

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
#ifndef _LOG_OUTPUT_EMPTY
	OutputDebugStringA("flog:安全模块检查完成");
#endif
	auto& file_map = LLH_MemFileMap::GetMe();
	auto commu = file_map.WriteAsCommu();
	if ( !commu )
	{
		//LOG_O(Log_debug) << "commu is zero";
		return;
	}
	commu->safe_check_done_ = true;
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

void OnLoginFailed( const wchar_t* login_str )
{
	static bool enter_once = false;
	if (enter_once)
	{
#ifndef _LOG_OUTPUT_EMPTY
		OutputDebugStringW(L"flog:已经进入了");
#endif
		return;
	}
	if ( !login_str )
		return;
	enter_once = true;
	auto enter_ptr = &enter_once;
	auto func = [enter_ptr](){
		*enter_ptr = false;
	};
	CleanAtScopeExit<decltype(func)> at_exit(func);
		
/*
#ifndef _LOG_OUTPUT_EMPTY
	wchar_t szbuffer[MAX_PATH];
	swprintf_s(szbuffer, L"flog:OnLoginFailed,字符是:%s", login_str);
	OutputDebugStringW(szbuffer);
#endif*/

	const wchar_t* kPwdError = L"密码不正确";
	const wchar_t* kFreezeAc = L"您的QQ暂时无法登录";
	const wchar_t* kFreezeAc2 = L"已经冻结";

	auto it_appear = wcsstr(login_str, kPwdError);
	if (it_appear)
	{

#ifndef _LOG_OUTPUT_EMPTY
		OutputDebugStringW(L"flog:密码不正确");
#endif
		
		auto& file_map = LLH_MemFileMap::GetMe();
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

	it_appear = wcsstr(login_str, kFreezeAc);
	if ( !it_appear )
		it_appear = wcsstr(login_str, kFreezeAc2);
	if (it_appear)
	{
#ifndef _LOG_OUTPUT_EMPTY
		OutputDebugStringW(L"flog:冻结帐号了");
#endif
		auto& file_map = LLH_MemFileMap::GetMe();
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

			push [esp+0x28]
		lea eax, OnLoginFailed
			call eax
			add esp, 4
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

void OnCheckLoginSucceed( DWORD flag )
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << flag;
	if ( 0 != flag )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "登陆成功";
		auto& file_map = LLH_MemFileMap::GetMe();
		auto commu = file_map.WriteAsCommu();
		if ( !commu )
		{
			LOG_O(Log_debug) << "commu is zero";
			return;
		}
		commu->login_succeed_ = true;
	}*/
}

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

bool InitCodeHooker()
{
	//return true;
	//安全检查完成
	{
		//2016/5/30 14:16
		/*
		$-F6     38024F18      56                    PUSH ESI
		$-F5     38024F19      8BF1                  MOV ESI, ECX
		$-F3     38024F1B      57                    PUSH EDI
		$-F2     38024F1C      837E 04 00            CMP DWORD PTR DS:[ESI+4], 0
		$-EE     38024F20      74 54                 JE SHORT SecureCh.38024F76
		$-EC     38024F22      8B7C24 0C             MOV EDI, DWORD PTR SS:[ESP+C]
		$-E8     38024F26      85FF                  TEST EDI, EDI
		$-E6     38024F28      74 4C                 JE SHORT SecureCh.38024F76
		$-E4     38024F2A      6A FF                 PUSH -1
		$-E2     38024F2C      FF76 14               PUSH DWORD PTR DS:[ESI+14]
		$-DF     38024F2F      FF15 DC300338         CALL DWORD PTR DS:[<&KERNEL32.WaitForSin>; kernel32.WaitForSingleObject
		$-D9     38024F35      837E 08 00            CMP DWORD PTR DS:[ESI+8], 0
		$-D5     38024F39      75 3B                 JNZ SHORT SecureCh.38024F76
		$-D3     38024F3B      8B47 08               MOV EAX, DWORD PTR DS:[EDI+8]
		$-D0     38024F3E      3B47 0C               CMP EAX, DWORD PTR DS:[EDI+C]
		$-CD     38024F41      75 1E                 JNZ SHORT SecureCh.38024F61
		$-CB     38024F43      68 04010000           PUSH 104
		$-C6     38024F48      8D47 10               LEA EAX, DWORD PTR DS:[EDI+10]
		$-C3     38024F4B      68 74380338           PUSH SecureCh.38033874                   ; UNICODE "安全检查完成"
		$-BE     38024F50      50                    PUSH EAX
		$-BD     38024F51      C746 08 01000000      MOV DWORD PTR DS:[ESI+8], 1
		$-B6     38024F58      FF15 9C320338         CALL DWORD PTR DS:[<&MSVCRT.wcsncpy>]    ; msvcrt.wcsncpy
		$-B0     38024F5E      83C4 0C               ADD ESP, 0C
		$-AD     38024F61      8B4E 1C               MOV ECX, DWORD PTR DS:[ESI+1C]
		$-AA     38024F64      85C9                  TEST ECX, ECX
		$-A8     38024F66      74 05                 JE SHORT SecureCh.38024F6D
		$-A6     38024F68      8B01                  MOV EAX, DWORD PTR DS:[ECX]
		$-A4     38024F6A      57                    PUSH EDI
		$-A3     38024F6B      FF10                  CALL DWORD PTR DS:[EAX]
		$-A1     38024F6D      FF76 14               PUSH DWORD PTR DS:[ESI+14]
		$-9E     38024F70      FF15 E8300338         CALL DWORD PTR DS:[<&KERNEL32.SetEvent>] ; kernel32.SetEvent
		$-98     38024F76      5F                    POP EDI
		$-97     38024F77      5E                    POP ESI
		$-96     38024F78      C2 0400               RETN 4
		$-93     38024F7B      8BC1                  MOV EAX, ECX
		$-91     38024F7D      8360 04 00            AND DWORD PTR DS:[EAX+4], 0
		$-8D     38024F81      C700 AC380338         MOV DWORD PTR DS:[EAX], SecureCh.380338A>
		$-87     38024F87      C3                    RETN
		$-86     38024F88      8361 04 00            AND DWORD PTR DS:[ECX+4], 0
		$-82     38024F8C      C701 AC380338         MOV DWORD PTR DS:[ECX], SecureCh.380338A>
		$-7C     38024F92      C3                    RETN
		$-7B     38024F93      8B5424 04             MOV EDX, DWORD PTR SS:[ESP+4]
		$-77     38024F97      33C0                  XOR EAX, EAX
		$-75     38024F99      85D2                  TEST EDX, EDX
		$-73     38024F9B      74 06                 JE SHORT SecureCh.38024FA3
		$-71     38024F9D      6A 01                 PUSH 1
		$-6F     38024F9F      8951 04               MOV DWORD PTR DS:[ECX+4], EDX
		$-6C     38024FA2      58                    POP EAX
		$-6B     38024FA3      C2 0400               RETN 4
		$-68     38024FA6      C2 0400               RETN 4
		$-65     38024FA9      C2 0800               RETN 8
		$-62     38024FAC      55                    PUSH EBP
		$-61     38024FAD      8BEC                  MOV EBP, ESP
		$-5F     38024FAF      81EC 20040000         SUB ESP, 420
		$-59     38024FB5      56                    PUSH ESI
		$-58     38024FB6      8BF1                  MOV ESI, ECX
		$-56     38024FB8      837E 04 00            CMP DWORD PTR DS:[ESI+4], 0
		$-52     38024FBC      74 50                 JE SHORT SecureCh.3802500E
		$-50     38024FBE      83A5 E0FBFFFF 00      AND DWORD PTR SS:[EBP-420], 0
		$-49     38024FC5      57                    PUSH EDI
		$-48     38024FC6      B9 07010000           MOV ECX, 107
		$-43     38024FCB      33C0                  XOR EAX, EAX
		$-41     38024FCD      8DBD E4FBFFFF         LEA EDI, DWORD PTR SS:[EBP-41C]
		$-3B     38024FD3      6A 64                 PUSH 64
		$-39     38024FD5      F3:AB                 REP STOS DWORD PTR ES:[EDI]
		$-37     38024FD7      58                    POP EAX
		$-36     38024FD8      8985 E8FBFFFF         MOV DWORD PTR SS:[EBP-418], EAX
		$-30     38024FDE      8985 ECFBFFFF         MOV DWORD PTR SS:[EBP-414], EAX
		$-2A     38024FE4      68 04010000           PUSH 104
		$-25     38024FE9      8D85 F0FBFFFF         LEA EAX, DWORD PTR SS:[EBP-410]
		$-1F     38024FEF      68 74380338           PUSH SecureCh.38033874                   ; UNICODE "安全检查完成"
		$-1A     38024FF4      50                    PUSH EAX
		$-19     38024FF5      FF15 9C320338         CALL DWORD PTR DS:[<&MSVCRT.wcsncpy>]    ; msvcrt.wcsncpy
		$-13     38024FFB      8B4E 04               MOV ECX, DWORD PTR DS:[ESI+4]
		$-10     38024FFE      83C4 0C               ADD ESP, 0C
		$-D      38025001      8D85 E0FBFFFF         LEA EAX, DWORD PTR SS:[EBP-420]
		$-7      38025007      50                    PUSH EAX
		$-6      38025008      E8 0BFFFFFF           CALL SecureCh.38024F18
		$-1      3802500D      5F                    POP EDI
		$ ==>    3802500E      33C0                  XOR EAX, EAX
		$+2      38025010      5E                    POP ESI
		$+3      38025011      C9                    LEAVE
		$+4      38025012      C3                    RETN
		$+5      38025013      33C0                  XOR EAX, EAX
		$+7      38025015      C2 0800               RETN 8
		$+A      38025018      E9 00000000           JMP SecureCh.3802501D
		$+F      3802501D      A1 68330338           MOV EAX, DWORD PTR DS:[<&WS2_32.#19>]
		$+14     38025022      A3 B49A0438           MOV DWORD PTR DS:[38049AB4], EAX
		$+19     38025027      C3                    RETN
		//89??????????68????????8d??????????68????????50ff??????????8b????83????8d??????????50e8????????5f
		*/
		auto tcls = LoadLibrary(GetModulePath(nullptr) + _T("SecureCheck.dll"));
		assert(tcls);
		if ( !tcls )
		{
			return false;
		}
		Hook((DWORD)tcls + g_o_HookSafeCheckDone, HookCheckModule);
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
#ifndef _LOG_OUTPUT_EMPTY
	OutputDebugStringA("flog:login helper hook成功");
#endif
	return true;
}