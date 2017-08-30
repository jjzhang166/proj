#include "stdafx.h"
#include <HideClasses/CDetours.h>
#include <CmnMix/Cmn_EnsureCleanup.h>
#include <CmnMix/Cmn_Toolhelp.h>
#include <detours/detours.h>
#include <cassert>
#include <CommonX/CmnX_HookerMore.h>

HMODULE WINAPI Detoured()
{
	return nullptr;
}

class DetourAbort{
public:
	DetourAbort( void** orig = nullptr){ 
		is_exec_ = true; 
		orig_ = orig;
	}
	~DetourAbort(){ 
		if (is_exec_) 
		{
			assert(false);
			DetourTransactionAbort(); 
			if (orig_)
			{
				*orig_ = nullptr;
			}
		}
	}

	void**			orig_;
	bool			is_exec_;
};

bool DetourUpDateOneThread( unsigned int thread_id )
{
	CEnsureCloseHandle thandle(OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread_id));
	if ( NO_ERROR !=
		DetourUpdateThread(thandle) )
		return false;
	return true;
}
bool DetourUpDateAllThread()
{
	const auto kCurrentThreadId = GetCurrentThreadId();
	const auto process_id = GetCurrentProcessId();
	CToolhelp th(TH32CS_SNAPTHREAD, process_id);
	THREADENTRY32 te = { sizeof(te) };
	for ( BOOL fOk = th.ThreadFirst(&te); fOk; fOk = th.ThreadNext(&te) ) {
		// Only parse threads of the given process
		if ( te.th32OwnerProcessID == process_id && kCurrentThreadId != te.th32ThreadID ) {
			return DetourUpDateOneThread(te.th32ThreadID);
		}
	}
	return true;
}
CDetours::CDetours(const char* hooker_name, void* hooked_addr, void* detour_addr, unsigned int update_thread_id, void** code_orig)
{
	hooked_addr_ = hooked_addr;
	hooked_addr_tmp_ = hooked_addr;
	assert(hooked_addr);
	detour_addr_ = detour_addr;
	assert(detour_addr);
	update_thread_id_ = update_thread_id;
	trampoline_addr_ = nullptr;
	is_resume_ = true;
	//////////////////////////////////////////////////////////////////////////
	auto& anti_hook = CmnX_AntiHookCheck::GetRealMe();
	if ( !anti_hook.AddHookCodeInfo(hooker_name, hooked_addr) )
	{
		assert(false);
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	DetourAbort detour_abort(&hooked_addr_tmp_);
	if ( NO_ERROR != DetourTransactionBegin() )
		return;
	
	if (code_orig)
	{
		auto res = DetourAttachEx(&(PVOID&)hooked_addr_tmp_, detour_addr_,
			&(PDETOUR_TRAMPOLINE&)*code_orig, nullptr, nullptr);
		if (NO_ERROR != res)
			return;
		trampoline_addr_ = *code_orig;
	}
	else
	{
		auto res = DetourAttachEx(&(PVOID&)hooked_addr_tmp_, detour_addr_,
			&(PDETOUR_TRAMPOLINE&)trampoline_addr_, nullptr, nullptr);
		if (NO_ERROR != res)
			return;
	}	
	assert(trampoline_addr_);

	if (update_thread_id_)
	{
		if ( update_thread_id_ != GetCurrentThreadId() && !DetourUpDateOneThread(update_thread_id_) )
			return;
	} 
	else
	{
		if ( !DetourUpDateAllThread() )
			return;
	}

	if ( NO_ERROR != DetourTransactionCommit() )
		return;

	detour_abort.is_exec_ = false;
}

CDetours::~CDetours()
{
	if ( !is_resume_ )
		return;
	DetourAbort detour_abort;
	if ( NO_ERROR != DetourTransactionBegin() )
		return;

	if ( NO_ERROR !=
		DetourDetach(&(PVOID&)hooked_addr_tmp_, detour_addr_) )
		return;

	if (update_thread_id_)
	{
		if ( update_thread_id_ != GetCurrentThreadId() && !DetourUpDateOneThread(update_thread_id_) )
			return;
	} 
	else
	{
		if ( !DetourUpDateAllThread() )
			return;
	}

	if ( NO_ERROR != DetourTransactionCommit() )
		return;

	detour_abort.is_exec_ = false;
}

bool CDetours::IsValid()
{
	return !!hooked_addr_;
}

void CDetours::SetResume( bool is_resume )
{
	is_resume_ = is_resume;
}

void* CDetours::GetHookedCodeAddr()
{
	return hooked_addr_;
}

void* CDetours::GetCodeOrig()
{
	return trampoline_addr_;
}