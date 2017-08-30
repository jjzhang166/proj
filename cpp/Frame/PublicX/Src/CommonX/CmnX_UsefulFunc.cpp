#include <stdafx.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <cassert>
#include <CommonX/CmnX_HookerMore.h>
#include <CommonX/CmnX_DllApi.h>

bool Cmnx_CpyMem( void* dst_addr, const void* src_addr, unsigned int mem_size )
{
	if ( mem_size <= 0 )
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dst_addr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(src_addr))
	{
		assert(false);
		return false;
	}
	auto dst_c = (char*)dst_addr;
	auto src_c = (const char*)src_addr;
	for ( auto i = 0; i != mem_size; ++i, ++dst_c, ++src_c )
	{
		*dst_c = *src_c;
	}
	return true;
}

bool Cmnx_MemSetValue( void* dst_addr, unsigned int mem_size, unsigned char the_value )
{
	if ( mem_size <= 0 )
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dst_addr))
	{
		assert(false);
		return false;
	}
	auto dst_c = (unsigned char*)dst_addr;
	for ( auto i = 0; i != mem_size; ++i, ++dst_c )
	{
		*dst_c = the_value;
	}
	return true;
}

bool Cmnx_CpyMemRecycle( void* dst_addr, unsigned int dst_buffer_size, const void* src_addr, unsigned int mem_size )
{
	if ( mem_size <= 0 || dst_buffer_size <= 0 )
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dst_addr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(src_addr))
	{
		assert(false);
		return false;
	}
	auto dst_c = (char*)dst_addr;
	auto src_c = (const char*)src_addr;
	for ( auto i = 0; i != dst_buffer_size; ++i, ++dst_c )
	{
		*dst_c = *(src_c + i % mem_size);
	}
	return true;
}

SECURITY_ATTRIBUTES* GetNullDACL()
{
	struct Hlp{
		Hlp(){
			SecAttr.nLength = sizeof(SecAttr);
			SecAttr.bInheritHandle = FALSE;
			SecAttr.lpSecurityDescriptor = &SecDesc;

			InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(&SecDesc, TRUE, 0, FALSE);
		}
		SECURITY_ATTRIBUTES SecAttr;
		SECURITY_DESCRIPTOR SecDesc;
	};
	static Hlp hlp;

	return &hlp.SecAttr;
}

bool SetHook(const char* hooker_name, void* the_hooked_addr, const void* new_jmp_addr, unsigned char old_code[5])
{
	if (IsUnValidPtr(the_hooked_addr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(new_jmp_addr))
	{
		assert(false);
		return false;
	}
	unsigned char code_buff[5];
	static_assert(sizeof(code_buff) == 5, "");

	if (old_code)
	{
		SIZE_T read_size = 0;
		if (!::ReadProcessMemory((void*)-1, the_hooked_addr, old_code, _countof(code_buff), &read_size))
		{
			assert(false);
			return false;
		}
		if (read_size != sizeof(code_buff))
		{
			assert(false);
			return false;
		}
		if (0xe9 == old_code[0])
		{
			assert(false);
			return false;
		}
	}

	code_buff[0] = 0xe9;
	*(DWORD*)&(code_buff[1]) = (DWORD)new_jmp_addr - (DWORD)the_hooked_addr - sizeof(code_buff);

	return WriteMemory_AntiCrc(hooker_name, the_hooked_addr, code_buff, _countof(code_buff));
}

bool WriteMemory_AntiCrc(const char* hooker_name, void* dest, const void* src, size_t size)
{
	if (!CmnX_AntiHookCheck::GetRealMe().AddHookCodeInfo(hooker_name, dest, size))
	{
		assert(false);
		return false;
	}
	return WriteMemory(dest, src, size);
}

bool WriteMemory(void* dest, const void* src, size_t size)
{
	auto the_func = ::GetWriteProcessMemory();
	if (!the_func)
	{
		assert(false);
		return false;
	}
	SIZE_T out_size = 0;
	if (!the_func(INVALID_HANDLE_VALUE, dest, src, size, &out_size))
	{
		assert(false);
		return false;
	}
	if (size != out_size)
	{
		assert(false);
		return false;
	}
	return true;
}

bool Details::CmnTimerDoImpl_(pt_dword every_sleep, pt_dword time_out, SleepFunctionT fn_sleep,
	const FunctionBool& func_do){
	if (!fn_sleep)
	{
		assert(false);
		return false;
	}
	/*if (every_sleep > time_out)
	{
		assert(false);
		return false;
	}*/
	pt_dint cur = clock();
	pt_dint end_time = cur + time_out;
	for (; cur < end_time; cur = clock())
	{
		if (func_do())
			return true;
		fn_sleep(every_sleep);
	}
	return false;
}

namespace Details{
	bool stTimerDoByPolicy<tagTimerDoSleep>::TimerDoImpl(pt_dword every_sleep, pt_dword time_out, const FunctionBool& func_do)
	{
		return CmnTimerDoImpl_(every_sleep, time_out, ::Sleep, func_do);
	}
}
