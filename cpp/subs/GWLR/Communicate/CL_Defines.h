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
const LPCTSTR kLoginMainWndName = _T("怪物猎人Online登录程序");
//登陆器所在相对路径
const LPCTSTR kRelativeClient = _T("TCLS\\");
//登陆器的名称
const LPCTSTR kLoginClientName = _T("Client.exe");

//验证码另存为的目标名字
const LPCTSTR kCaptchaPicNewName = _T("NewCaptcha.png");

//控制台发给game模块一个固定的字符串
#ifdef __PROJ_GWLR_MB_
const char* const kStableStr_ConsoleToLogin = 
	/*kStableStr_ConsoleToLogin_mb*/"5635ee5f-4d4d-41e1-8543-8e952b00aa79";
#elif (__PROJ_GWLR_XE_)
const char* const kStableStr_ConsoleToLogin = 
	/*kStableStr_ConsoleToLogin_xe*/"aca51561-5dce-4f12-b1f9-2e8244d566a8";
#elif (__PROJ_GWLR_MBP_)
const char* const kStableStr_ConsoleToLogin = 
	/*kStableStr_ConsoleToLogin_mbp*/"44d68105-4f81-4671-8104-cf4d411318c2";
#endif
//login operator
const P_ObjId kObjId_LoginOper = DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_(1);