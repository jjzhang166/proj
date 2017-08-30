#pragma once
/*
@author zhp
@date 2014/4/9 11:05
@purpose func object of login helper
*/
#include "stdafx.h"
#include "socket/client.h"
#include "Common/UsefulClass.h"
#include "Script/JsonCall.h"
#include "Lua_l.h"
#include "../Console/CfgStruct.h"
#include "Common/CrackCaptcha.h"

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

class LoginC : public Connection_Aes, public Singleton<LoginC, Singleton_Instance>{
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
	//设置大区坐标
	void SetSerRegion( int pos_x, int pos_y );
	//设置要进入的服务器
	void SetJoinedService( int pos_x, int pos_y );
	//设置答码方式
	void SetCrackCaptchaMode( int ccm, const char* account, const char* password, const char* remote_ip, WORD remote_port );
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
	//选择大区(电信/网通)
	bool SelectSerRegion();
	//选择服务器
	bool SelectService();
	//服务器已经爆满
	bool HandleServiceFull();
	//确认选择
	bool SelectedService();
	//寻找帐号窗口
	HWND FindAcWnd();

protected:
	bool OnConnected( IoConnectBase* connect );

private:
	class DaMaTu_JFZR : public DaMaTu{
		const char* SoftKey() const;
	};

	//组件
private:
	JsonEcho				json_echo_;
	Lua_l					lua_l_;

private:
	std::string				account_;
	std::string				password_;
	CPoint					ser_region_pos_;				//服务大区坐标
	CPoint					service_pos_;					//要选择的服务器的坐标
	stCrackCaptchaMode		crack_captcha_mode_;			//答码方式

	//答码方式
private:
	ICrackCaptcha*			crack_captcha_;
	LocalCrackCaptcha		local_crack_captcha_;			//本地答码
	DaMaTu_JFZR					da_ma_tu_;						//答码兔
	LoginOperC				login_oper_;
};