#pragma once
/*
@author zhp
@date 2017/19/30 19:30
@purpose some defines
*/
#include "stdafx.h"
#include <ProtocolX/P_DefinesX.h>
#include <AtpMiX/AtpLogin.h>

extern void*			g_pCheckLoginFailed;
//////////////////////////////////////////////////////////////////////////
class IHooker;
struct JmpHookerAmbient;
//////////////////////////////////////////////////////////////////////////

struct GlobalSetter{
	static GlobalSetter& GetMe();
	void SetSelfModuleHandle(HMODULE hModule){
		hModule_ = hModule;
	}
	void SetIoThreadId(DWORD thrd_id){
		thrd_id_ = thrd_id;
	}
	DWORD GetCurProcessId(){
		return ::GetCurrentProcessId();
	}

	HMODULE				hModule_;
	DWORD				thrd_id_;
	stAtpDummyBugTrace	params_;
};

BOOL DllMainImpl(HMODULE hModule, DWORD dwReason, PVOID pvReserved);
void SetInitParam(const stAtpDummyBugTrace& param);
//特征码信息相关
void AsmInfo();
bool InitMsgHook();
bool InitCodeHooker(const stAtpDummyBugTrace& param);
void* Hooker_CheckDone(IHooker& hooker, JmpHookerAmbient& ambient);
void OnLoginFailed(const wchar_t* login_str, UINT char_cnt);
void InputPwd(HOOKPROC next_proc, char c);
void CALLBACK InputPwdTimer(_In_  HWND hwnd, _In_  UINT uMsg, _In_  UINT_PTR idEvent, _In_  DWORD dwTime);
void MsgThread();
