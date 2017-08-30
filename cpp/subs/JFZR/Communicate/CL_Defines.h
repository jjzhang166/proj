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
//登陆程序选择服务区的窗口类名
const LPCTSTR kLoginSelectServiceWndClsName = _T("e649e249_0000442e_00004f2d_0018c024");
//登陆程序的主窗口名字
const LPCTSTR kLoginMainWndName = _T("疾风之刃登录程序");
//登陆器所在相对路径
const LPCTSTR kRelativeClient = _T("TCLS\\");
//登陆器的名称
const LPCTSTR kLoginClientName = _T("Client.exe");

//验证码另存为的目标名字
const LPCTSTR kCaptchaPicNewName = _T("NewCaptcha.png");

//控制台发给game模块一个固定的字符串
const char* const kStableStr_ConsoleToLogin = 
#ifdef __PROJ_JFZR_
	"D6AFE121-2CFC-4BFA-1A36-6A955A65A36D";
#elif defined(__PROJ_JFZR_SM_)
	"B45131B3-7E81-4CC3-27B1-F37C1139980A";
#elif defined(__PROJ_JFZR_TWO_)
	"1FD993D5-B761-458A-3F03-8FFB0466FE3D";
#endif

//login operator
const P_ObjId kObjId_LoginOper = DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_(1);