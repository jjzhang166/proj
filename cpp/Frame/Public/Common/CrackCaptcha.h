#pragma once
/*
@author zhp
@date 2014/4/9 23:32
@purpose for crack chaptcha
*/
#include "stdafx.h"
#include <string>
#include "../DependSelfLib.h"

class ICrackCaptcha
{
public:
	virtual ~ICrackCaptcha();
	//初始化
	virtual bool Init() = 0;
	//得到结果
	virtual bool GetResult( const std::string& str_pic_dir, std::string& str_result ) = 0;
	virtual bool GetResultFromBuff( const void* pic_data, const pt_dword pic_size, std::string& str_result ) = 0;
};

//答码兔
class DaMaTu : public ICrackCaptcha{
public:
	//设置帐号和密码
	void SetAccountPassword( const char* account, const char* password );

public:
	virtual bool Init();
	virtual bool GetResult( const std::string& str_pic_dir, std::string& str_result );
	bool GetResultFromBuff( const void* pic_data, const pt_dword pic_size, std::string& str_result );

protected:
	virtual const char* SoftKey() const = 0;

private:
	std::string				account_;
	std::string				password_;
};

//本地答题
class LocalCrackCaptcha : public ICrackCaptcha{
public:
	LocalCrackCaptcha();
	virtual ~LocalCrackCaptcha();
	//设置服务信息
	void SetServiceInfo( const char* remote_ip, WORD remote_port );

public:
	virtual bool Init();
	virtual bool GetResult( const std::string& str_pic_dir, std::string& str_result );
	bool GetResultFromBuff( const void* pic_data, const pt_dword pic_size, std::string& str_result );

private:
	typedef void (__stdcall*SetServerInfoT)( const char* str_ip, DWORD port );
	typedef const char* (__stdcall*SendImageT)( const char* str_pic_path, DWORD time_out );
	typedef const char* (__stdcall*GetAnswerT)( const char* str_send_image_ret );

private:
	SetServerInfoT				set_server_info_;
	SendImageT					send_image_;
	GetAnswerT					get_answer_;

	std::string					remote_ip_;
	WORD						remote_port_;
};