#include "stdafx.h"
#include "Common/CrackCaptcha.h"
#include "CrackCaptcha/CrackCaptchaAPI.h"
#include <cassert>
#include "Common/UsefulFunc.h"
#include <Common/CmnDllApi.h>

#pragma comment(lib, "CrackCaptchaAPI.lib")

ICrackCaptcha::~ICrackCaptcha()
{

}

bool DaMaTu::Init()
{
	//LibraryLoader::LoadTheLibrary(_T("CrackCaptchaAPI.dll"));
	return true;
}

bool DaMaTu::GetResult( const std::string& str_pic_dir, std::string& str_result )
{
	if ( account_.empty() || password_.empty() )
	{
		MessageBoxTimeout(nullptr, "打码兔帐号或密码不能为空", nullptr, MB_OK, 0, 1000 * 30);
		return false;
	}

	char szVCodeText[64] = {0};
	const unsigned long kVTypeId = 23;
	auto nRet = ::D2File(
		SoftKey(),
		account_.c_str(),
		password_.c_str(),
		str_pic_dir.c_str(),
		60,
		kVTypeId,
		szVCodeText);
	::ReportResult(kVTypeId, nRet);
	if( nRet > 0 )
	{
		str_result = szVCodeText;
		return true;
	}
	std::string prompt_str = "打码兔返回错误:";
	prompt_str += std::to_string(nRet);
	prompt_str += " 帐号：";
	prompt_str += account_;
	prompt_str += " 密码：";
	prompt_str += password_;
	MessageBoxTimeout(nullptr, prompt_str.c_str(), nullptr, MB_OK, 0, 1000 * 20);
	return false;
}

void DaMaTu::SetAccountPassword( const char* account, const char* password )
{
	if ( account && password )
	{
		account_ = account;
		password_ = password;
		return;
	}

	assert(false);
}

bool DaMaTu::GetResultFromBuff( const void* pic_data, const pt_dword pic_size, std::string& str_result )
{
	if ( account_.empty() || password_.empty() )
		return false;
	if ( IsUnValidPtr(pic_data) || pic_size <= 0 )
	{
		assert(false);
		return false;
	}
	char szVCodeText[64] = {0};
	const unsigned long kVTypeId = 23;
	auto nRet = ::D2Buf(
		SoftKey(),
		account_.c_str(),
		password_.c_str(),
		pic_data, pic_size,
		15,
		kVTypeId,
		szVCodeText);
	::ReportResult(kVTypeId, nRet);
	if( nRet > 0 )
	{
		str_result = szVCodeText;
		return true;
	}
	return false;
}

bool LocalCrackCaptcha::Init()
{
	auto lib = LibraryLoader::LoadTheLibrary(kQxAqDllName);
	if ( !lib )
		return false;
	
	set_server_info_ = (SetServerInfoT)LibFuncLoader::GetTheProcAddress(lib, "SetServerInfo");
	assert(set_server_info_);
	if ( !set_server_info_ )
		return false;

	send_image_ = (SendImageT)LibFuncLoader::GetTheProcAddress(lib, "SendImage");
	assert(send_image_);
	if ( !send_image_ )
		return false;

	get_answer_ = (GetAnswerT)LibFuncLoader::GetTheProcAddress(lib, "GetAnswer");
	assert(get_answer_);
	if ( !get_answer_ )
		return false;

	set_server_info_(remote_ip_.c_str(), remote_port_);
	return true;
}

LocalCrackCaptcha::~LocalCrackCaptcha()
{
	//::FreeLibrary(kQxAqDllName);
}

LocalCrackCaptcha::LocalCrackCaptcha() : remote_ip_(kSelfIpStr)
{
	set_server_info_ = nullptr;
	send_image_ = nullptr;
	get_answer_ = nullptr;

	remote_port_ = 0;
}

bool LocalCrackCaptcha::GetResult( const std::string& str_pic_dir, std::string& str_result )
{
	if ( send_image_ && get_answer_ )
	{
		const int kTime_TimeOut = 10 * 60;

		CStringA send_img_ret = send_image_(str_pic_dir.c_str(), kTime_TimeOut);
		if ( !send_img_ret )
		{
			assert(false);
			return false;
		}

		//超时
		static const std::string kTimeOut("-3");
		//暂无答案
		static const std::string kNoResult("-2");
		//服务器连接失败
		static const std::string kCanNotConnet("-1");

		const char* get_ans_ret = nullptr;
		do 
		{
			DummySleep(2000);
			get_ans_ret = get_answer_(send_img_ret);
		} while ( get_ans_ret && kCanNotConnet != get_ans_ret && kTimeOut != get_ans_ret && kNoResult == get_ans_ret );
		
		if ( get_ans_ret && kCanNotConnet != get_ans_ret && kNoResult != get_ans_ret && kTimeOut != get_ans_ret )
		{
			str_result = get_ans_ret;
			return true;
		}
		return false;
	}

	assert(false);
	return false;
}

void LocalCrackCaptcha::SetServiceInfo( const char* remote_ip, WORD remote_port )
{
	if (remote_ip)
		remote_ip_ = remote_ip;
	remote_port_ = remote_port;
}

bool LocalCrackCaptcha::GetResultFromBuff( const void* pic_data, const pt_dword pic_size, std::string& str_result )
{
	//assert(false);
	return false;
}