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

bool InitCodeHooker()
{
	using namespace LAddr;
	//auto& hooker_factory = HookerFactory::GetMe();
	/*//game main wnd
	auto tcls = LoadLibrary(GetModulePath(nullptr) + _T("Tenio\\TCLS.dll"));
	assert(tcls);
	if ( !tcls )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "TCLS.dll¼ÓÔØÊ§°Ü";
		return false;
	}
	static auto hooker_gain_CaptchaPic = hooker_factory.CreateCodeHooker<tagGainCaptchaPic
	>((void*)((char*)tcls + g_o_GainCaptchaPic),
		Hooker_GainCaptchaPic, false);*/

	//É¾³ý´ðÂëÍ¼Æ¬
	CPath chaptcha_pic = GetModulePath(nullptr) + kDaMaPicRelativePath;
	chaptcha_pic.Append(_T("pic.png"));
	::DeleteFile(chaptcha_pic);
	chaptcha_pic.RemoveFileSpec();
	chaptcha_pic.Append(_T("pic1.png"));
	::DeleteFile(chaptcha_pic);
	return true;
}