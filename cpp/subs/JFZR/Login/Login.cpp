// Login.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Common/Hooker.h"
#include "LAddr.h"
#include "Common/UsefulFunc.h"
#include <atlpath.h>
#include "Protocol/P_Defines.h"
#include "Common/LogOut.h"
#include <Communicate/CL_Defines.h>
#include "LoginC.h"

bool InitCodeHooker();
extern bool InitGame()
{
	if ( !InitCodeHooker() )
		return false;
	return true;
}

void* Hooker_GainCaptchaPic( IHooker& hooker )
{	
	const CPath captcha_src = GetModulePath(nullptr);
	CString str_file_name;
	CPath captcha_src_tmp;
	for ( int i = 0;i != 10; ++i )
	{
		if ( 0 == i )
			str_file_name = _T("ui\\res\\pic.png");
		else
		{
			str_file_name.Format(_T("ui\\res\\pic%d.png"), i);
		}
		captcha_src_tmp = captcha_src;
		captcha_src_tmp += str_file_name;
		if (captcha_src_tmp.FileExists())
			break;
	}
	if (captcha_src_tmp.FileExists())
	{
		VERIFY(CopyFile(captcha_src_tmp, captcha_src + kCaptchaPicNewName, FALSE));
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "copyÑéÖ¤ÂëÍ¼Æ¬";
	}

	return hooker.GetCodeOrig();
}

void* Hooker_GetPrivateProfileString( IHooker& hooker )
{
	auto ambient = hooker.GetAmbient();
	if ( !ambient )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( ambient->esp_ < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	const wchar_t** pp_app_name = (const wchar_t**)(ambient->esp_ + 4);
	const wchar_t** pp_key_name = (const wchar_t**)(ambient->esp_ + 8);
	if ( (int)*pp_app_name < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if ( (int)*pp_key_name < (WORD)-1 )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	static const std::wstring kStr39 = L"39";
	static const std::wstring kStrText = L"text";
	if ( kStr39 == *pp_app_name && kStrText == *pp_key_name )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "key name:" << *pp_key_name;
		GetIoService().post([](){
			auto ls = LoginC::GetInstance();
			if ( !ls )
			{
				assert(false);
				return;
			}
			ls->HandleServiceFull();
		});
	}
	return hooker.GetCodeOrig();
}

bool InitCodeHooker()
{
	using namespace LAddr;
	auto& hooker_factory = HookerFactory::GetMe();
	//game main wnd
	auto kernel32 = LoadLibrary(_T("Kernel32.dll"));
	assert(kernel32);
	if ( !kernel32 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "TCLS.dll¼ÓÔØÊ§°Ü";
		return false;
	}
	auto lib_func_addr = GetProcAddress(kernel32, "GetPrivateProfileStringW");
	if ( !lib_func_addr )
	{
		assert(false);
		return false;
	}
	static auto hooker_gain_CaptchaPic = hooker_factory.CreateCodeHooker<tagGetPrivateProfileString
	>((void*)lib_func_addr,
		Hooker_GetPrivateProfileString, false);

	return true;
}