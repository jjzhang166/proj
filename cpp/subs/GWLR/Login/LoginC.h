#pragma once
/*
@author zhp
@date 2014/4/9 11:05
@purpose func object of login helper
*/
#include "stdafx.h"
#include "Net/client.h"
#include "Common/UsefulClass.h"
#include <Net/JsonCall.h>
#include "Lua_l.h"
#include "../Console/CfgStruct.h"
#include "Common/CrackCaptcha.h"
#include <Net/IoTimer.h>

class LoginC;
class LoginOperC : public JsonEchoBase{
public:
	LoginOperC( LoginC& login_c );
	//验证字符串
	bool CheckLoginStr( const std::string& console_str );

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	LoginC&				login_c_;
};

class LoginC : public Connection, public Singleton<LoginC, Singleton_Instance>{
public:
	LoginC();

public:
	bool Init();
	//get json echo
	JsonEcho& GetJsonEcho();

	//for communication
public:
	//设置帐号和密码
	void SetAccount_Password( const char* account, const char* password );
	//设置要进入的服务器
	void SetJoinedService( int pos_x, int pos_y );
	//设置答码方式
	void SetCrackCaptchaMode( int ccm, const char* account, const char* password, const char* remote_ip, WORD remote_port );
	//设置游戏目录
	void SetGamePath( const std::string& game_path );
	//开始登陆游戏
	void BeginLogin();

public:
	//查找安全检查子进程
	DWORD Do_CheckSafeSubProcessId();
	//打开内存映射文件
	bool OpenMemFileMap();
	//注入Login helper模块
	bool InjectLoginHelper( DWORD process_id );
	//安全检查的事件对象是否存在
	bool Do_SafeCheckEventExist();
	//输入帐号
	bool InputAccount();
	//输入密码
	bool InputPassword();
	//登陆游戏
	bool LoginGame();
	//处理登录结果
	bool HandleLoginResult();
	//处理验证码
	bool HandleCaptcha();
	//选择服务器
	bool SelectService();
	//确认选择
	bool SelectedService();
	//关闭登陆广告
	void StartCloseLoginAdvert();
	void EndCloseLoginAdvert();

protected:
	bool OnConnected( IoConnectBase* connect );
	void OnConnectFailed();
	bool OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size );

private:
	HWND GetAcWnd();
	void DMTest();

private:
	class DaMaTu_FIFA : public DaMaTu{
		const char* SoftKey() const;
	};

	//组件
private:
	JsonEcho				json_echo_;
	Lua_l					lua_l_;

private:
	std::string				account_;
	std::string				password_;
	CPoint					service_pos_;					//要选择的服务器的坐标
	stCrackCaptchaMode		crack_captcha_mode_;			//答码方式

	//答码方式
private:
	ICrackCaptcha*			crack_captcha_;
	LocalCrackCaptcha		local_crack_captcha_;			//本地答码
	DaMaTu_FIFA				da_ma_tu_;						//答码兔
	LoginOperC				login_oper_;
	IoTimerPtr				close_login_advert_;
	bool					is_dmtest_;
};