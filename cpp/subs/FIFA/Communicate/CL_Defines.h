#pragma once
/*
@author zhp
@date 2014/6/15 15:15
@purpose console.exe/login.dll之间的交互
*/
#include <WTypes.h>
#include <Protocol/P_Defines.h>

//登陆程序的主窗口类名
const LPCTSTR kLoginMainClsName = _T("TWINCONTROL");
//登陆程序的主窗口名字
const LPCTSTR kLoginMainWndName = _T("FIFA Online 3登录程序");
//登陆器所在相对路径
const LPCTSTR kRelativeClient = _T("TCLS\\");
//登陆器的名称
const LPCTSTR kLoginClientName = _T("Client.exe");

//验证码另存为的目标名字
const LPCTSTR kCaptchaPicNewName = _T("NewCaptcha.png");

//控制台发给game模块一个固定的字符串
const char* const kStableStr_ConsoleToLogin = 
	"572BF63C-69618-4215-3hE9-0841E231244fa";

//login operator
const P_ObjId kObjId_LoginOper = DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_(1);