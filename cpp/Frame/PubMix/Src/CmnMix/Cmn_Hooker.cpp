#include "stdafx.h"
#include <CmnMix/Cmn_Hooker.h>
#include <HideClasses/APIHook.h>
#include <HideClasses/CDetours.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <CommonX/CmnX_DllApi.h>
#include <CommonX/CmnX_HookerMore.h>

IHooker::~IHooker()
{

}

PROC IHooker::GetApiOrig()
{
	return nullptr;
}

void* IHooker::GetHookedCodeAddr()
{
	return nullptr;
}

void* IHooker::GetCodeOrig()
{
	return nullptr;
}

HookerPtr HookerFactory::CreateApiHooker( PCSTR pszCalleeModName, PCSTR pszFuncName, 
										 PROC pfnHook, bool is_resume )
{
	HookerPtr hooker(new CAPIHook(pszCalleeModName, pszFuncName, pfnHook, GetModuleHandle(nullptr)));
	if ( !hooker->IsValid() )
	{
		return HookerPtr();
	}
	hooker->SetResume(is_resume);
	return hooker;
}

HookerPtr HookerFactory::CreateCodeHooker( const char* hooker_name, void* hooked_addr, void* detour_addr, bool is_resume )
{
	HookerPtr hooker(new CDetours(hooker_name, hooked_addr, detour_addr, GetCurrentThreadId()));
	if ( !hooker->IsValid() )
	{
		return HookerPtr();
	}
	hooker->SetResume(is_resume);
	return hooker;
}

HookerFactory::HookerFactory()
{

}

HookerFactory& HookerFactory::GetMe()
{
	static HookerFactory hooker_factory;
	return hooker_factory;
}

void HookerPtrStored::StoreHook( const HookerPtr& hook_ptr )
{
	if ( !hook_ptr )
	{
		assert(false);
		return;
	}
	hooks_.insert(hook_ptr);
}

void HookerPtrStored::Clear()
{
	hooks_.clear();
}

void SetDetourHook_(const char* hooker_name, void* hooked_addr, const void* detour_addr, void** code_orig)
{
	CDetours impl(hooker_name, hooked_addr, (void*)detour_addr, GetCurrentThreadId(), code_orig);
	static_cast<IHooker&>(impl).SetResume(false);
}

HookSomeThing& HookSomeThing::GetMe()
{
	static HookSomeThing me;
	return me;
}

void HookSomeThing::SetMachineCodeBased(const std::string& machine_code_based)
{
	assert(!machine_code_based.empty());
	assert(machine_code_based_.empty());
	machine_code_based_ = machine_code_based;
}

void* HookSomeThing::_Hooker_ForAntiCpuid_(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto code_orig = (unsigned short*)hooker.GetCodeOrig();
	auto& hook_some = HookSomeThing::GetMe();
	auto& machine_code_based = hook_some.machine_code_based_;
	if (machine_code_based.empty())
	{
		assert(false);
		return code_orig;
	}
	assert(code_orig);
	//0xa20f==cpuid
	assert(*code_orig == 0xa20f);
	auto res = hook_some.DoAntiCpuid(ambient);
	if (res == kACR_RetPassCpuid)
		return code_orig + 1;
	else
		return code_orig;
}

pt_dword HookSomeThing::GetDummyCpuid(pt_dword cpuid, int idx) const
{
	auto max_cnt = machine_code_based_.size();
	if (max_cnt <= 0)
	{
		assert(false);
		return cpuid;
	}
	auto cstr = machine_code_based_.c_str();
	cpuid <<= idx;
	pt_dword cnt = 0;
	while (cnt < max_cnt)
	{
		cpuid += *(cstr + idx % max_cnt);
		++idx;
		++cnt;
	}
	return cpuid;
}

HookSomeThing::enAntiCpuidRes HookSomeThing::DoAntiCpuid(JmpHookerAmbient& ambient)
{
	/*
	处理器序列号一共96位，最高32位就是处理器签名，通过执行EAX=1的CPUID指令获得，
	其余的64位在执行EAX=3的CPUID指令后，中间32位在EDX中，最低32位在ECX中。
	顺便提一句，AMD所有的CPU都没有提供过处理器序列号的功能。
	*/
	//获取CPU的Vendor ID
	if (0 == ambient.eax_)
	{
		ambient.Cpuid(0);
		ambient.eax_ = GetDummyCpuid(ambient.eax_, 0);
		return kACR_RetPassCpuid;
	}
	//处理器签名（Processor Signiture）和功能（Feature）位
	else if (1 == ambient.eax_)
	{
		ambient.Cpuid(1);
		ambient.eax_ = GetDummyCpuid(ambient.eax_, 1);
		return kACR_RetPassCpuid;
	}
	else if (3 == ambient.eax_)
	{
		ambient.Cpuid(3);
		ambient.ecx_ = GetDummyCpuid(ambient.ecx_, 3);
		ambient.edx_ = GetDummyCpuid(ambient.edx_, 3);
		return kACR_RetPassCpuid;
	}
	return kACR_RetNormal;
}

void JmpHookerAmbient::Cpuid(int eax_param)
{
	pt_dword the_eax = 0, the_ebx = 0, the_ecx = 0, the_edx = 0;
	__asm{
		mov eax, eax_param
		cpuid
		mov the_eax, eax
		mov the_ebx, ebx
		mov the_ecx, ecx
		mov the_edx, edx
	}
	eax_ = the_eax;
	ebx_ = the_ebx;
	ecx_ = the_ecx;
	edx_ = the_edx;
}

JmpHookerAmbient::JmpHookerAmbient()
{
	ZeroMemory(this, sizeof(*this));
}

void JmpHookerAmbient::AssignRegisterABCD(const JmpHookerAmbient& rhs)
{
	eax_ = rhs.eax_;
	ebx_ = rhs.ebx_;
	ecx_ = rhs.ecx_;
	edx_ = rhs.edx_;
}
