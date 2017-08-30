#include "stdafx.h"
#include <CommonX/CmnX_DllApi.h>
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
//#include <Common/LogOut.h>

LibFuncLoader::LibFuncLoader( const char* str_library, const char* func_name )
{
	the_func_addr_ = nullptr;
	LibraryLoader ll(str_library);
	the_func_addr_ = GetTheProcAddress(ll.GetLibrary(), func_name);
	assert(the_func_addr_);
}

LibFuncLoader::LibFuncLoader( HMODULE hlib, const char* func_name )
{
	the_func_addr_ = nullptr;
	assert(hlib);
	the_func_addr_ = GetTheProcAddress(hlib, func_name);
	assert(the_func_addr_);
}

LibFuncLoader::LibFuncLoader( const LibraryLoader& lib_loader, const char* func_name )
{
	the_func_addr_ = GetTheProcAddress(lib_loader.GetLibrary(), func_name);
	if ( !the_func_addr_ )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "ÎÞ·¨loadº¯Êý£º" << func_name;*/
		assert(the_func_addr_);
	}
}

HMODULE LibraryLoader::LoadTheLibrary( const char* str_library )
{
	typedef HMODULE (WINAPI *LoadLibraryT)( _In_  LPCSTR lpFileName );
	static LoadLibraryT func_load = nullptr;
	if ( !func_load )
	{
		func_load = (LoadLibraryT)GetProcAddress(::LoadLibraryA("Kernel32.dll"), "LoadLibraryA");
		assert(func_load);
	}
	assert(str_library);
	return func_load(str_library);
}

HMODULE LibraryLoader::LoadTheLibrary( const wchar_t* str_library )
{
	typedef HMODULE (WINAPI *LoadLibraryT)( _In_  LPCWSTR lpFileName );
	static LoadLibraryT func_load = nullptr;
	if ( !func_load )
	{
		func_load = (LoadLibraryT)GetProcAddress(GetLib_Kernel32().GetLibrary(), "LoadLibraryW");
		assert(func_load);
	}
	assert(str_library);
	return func_load(str_library);
}

LibraryLoader::LibraryLoader( const char* str_library )
{
	assert(str_library);
	the_library_ = LoadTheLibrary(str_library);
	assert(the_library_);
}

HMODULE LibraryLoader::GetLibrary() const
{
	return the_library_;
}

const LibraryLoader& LibraryLoader::GetLib_Kernel32()
{
	static LibraryLoader the_lib("Kernel32.dll");
	return the_lib;
}

const LibraryLoader& LibraryLoader::GetLib_User32()
{
	static LibraryLoader the_lib("user32.dll");
	return the_lib;
}

const LibraryLoader& LibraryLoader::GetLib_Ntdll()
{
	static LibraryLoader the_lib("ntdll.dll");
	return the_lib;
}

FARPROC LibFuncLoader::GetTheProcAddress( HMODULE hlib, const char* func_name )
{
	typedef FARPROC (WINAPI* GetProcAddressT)(_In_  HMODULE hModule, _In_  LPCSTR lpProcName);
	static GetProcAddressT func_get_proc_addr = nullptr;
	if ( !func_get_proc_addr )
	{
		func_get_proc_addr = (GetProcAddressT)GetProcAddress(LibraryLoader::GetLib_Kernel32().GetLibrary(), "GetProcAddress");
		assert(func_get_proc_addr);
	}
	assert(hlib);
	assert(func_name);
	return func_get_proc_addr(hlib, func_name);
}

WriteProcessMemoryT GetWriteProcessMemory()
{
	static LibFuncLoaderX<WriteProcessMemoryT>	write_process_memory_(LibraryLoader::GetLib_Kernel32(), "WriteProcessMemory");
	return write_process_memory_.GetFunc();
}

BOOL WINAPI WriteProcessMemory_( _In_ HANDLE hProcess, _In_ LPVOID lpBaseAddress, _In_ LPCVOID lpBuffer, _In_ SIZE_T nSize, _Out_ SIZE_T *lpNumberOfBytesWritten )
{
	auto the_func = GetWriteProcessMemory();
	if ( !the_func )
	{
		assert(false);
		return FALSE;
	}
	return the_func(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

ReadProcessMemoryT GetReadProcessMemory()
{
	static LibFuncLoaderX<ReadProcessMemoryT>	read_process_memory_(LibraryLoader::GetLib_Kernel32(), "ReadProcessMemory");
	return read_process_memory_.GetFunc();
}

BOOL WINAPI ReadProcessMemory_( _In_ HANDLE hProcess, _In_ LPCVOID lpBaseAddress, _Out_ LPVOID lpBuffer, _In_ SIZE_T nSize, _Out_ SIZE_T *lpNumberOfBytesRead )
{
	auto the_func = GetReadProcessMemory();
	if ( !the_func )
	{
		assert(false);
		return FALSE;
	}
	return the_func(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

bool ReadMemory( const void* base_addr, void* out_buffer, SIZE_T read_size )
{
	SIZE_T copy_size = 0;
	if ( !ReadProcessMemory_(INVALID_HANDLE_VALUE, base_addr, out_buffer, read_size, &copy_size) )
	{
		assert(false);
		return false;
	}
	if ( copy_size != read_size )
	{
		assert(false);
		return false;
	}
	return true;
}

MessageBoxTimeoutT GetMessageBoxTimeout()
{
	static LibFuncLoaderX<MessageBoxTimeoutT> the_func(LibraryLoader::GetLib_User32(), "MessageBoxTimeoutA");
	return the_func.GetFunc();
}

int MessageBoxTimeout( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds )
{
	auto the_func = GetMessageBoxTimeout();
	if ( !the_func )
	{
		assert(false);
		return IDOK;
	}
	return the_func(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
}

NtQueryInformationThreadT GetNtQueryInformationThread()
{
	static NtQueryInformationThreadT the_func = nullptr;
	if (the_func)
		return the_func;
	NtQueryInformationThreadT* ptr_fn = &the_func;
	TimerDo<tagTimerDoSleep>(1, 1000 * 60, [ptr_fn](){
		LibFuncLoaderX<NtQueryInformationThreadT>	nt_query_information(LibraryLoader::GetLib_Ntdll(), "NtQueryInformationThread");
		auto fn = nt_query_information.GetFunc();
		if (fn)
		{
			*ptr_fn = fn;
			return true;
		}
		return false;
	});
	if (!the_func)
	{
		assert(false);
		return nullptr;
	}
	return the_func;
}

NtQueryInformationProcessT GetNtQueryInformationProcess()
{
	static LibFuncLoaderX<NtQueryInformationProcessT>	the_func(LibraryLoader::GetLib_Ntdll(), "NtQueryInformationProcess");
	return the_func.GetFunc();
}

BOOL Get_GetVersionExW(LPOSVERSIONINFOW lpVersionInformation)
{
	typedef BOOL(WINAPI*GetVersionExW_)(_Inout_ LPOSVERSIONINFOW lpVersionInformation);
	static LibFuncLoaderX<GetVersionExW_> the_func(LibraryLoader::GetLib_Kernel32(), "GetVersionExW");
	auto func = the_func.GetFunc();
	if ( !func )
		return FALSE;
	return func(lpVersionInformation);
}

NtCreateThreadEx GetNtCreateThreadEx()
{
	static LibFuncLoaderX<NtCreateThreadEx>	the_func(LibraryLoader::GetLib_Ntdll(), "NtCreateThreadEx");
	return the_func.GetFunc();
}

NtQueryObject GetNtQueryObject()
{
	static LibFuncLoaderX<NtQueryObject>	the_func(LibraryLoader::GetLib_Ntdll(), "NtQueryObject");
	return the_func.GetFunc();
}

ULONG GetNtObjectHandleCount(HANDLE ObjectHandle)
{
	if (!ObjectHandle)
		return 0;
	auto fn = GetNtQueryObject();
	if (!fn)
	{
		assert(false);
		return 0;
	}
	OBJECT_BASIC_INFORMATION info = { 0 };
	if (!NT_SUCCESS(fn(ObjectHandle, ObjectBasicInformation, &info, sizeof(info), nullptr)))
		assert(false);
	return info.HandleCount;
}
