#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

//0x501==XP, 0x0600==win7
#ifndef _WIN32_WINNT
#error _WIN32_WINNT 必须已定义
#endif

//必须在预编译器里面定义
//#define _WIN32_WINNT 0x502
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs
#define WINDOWS_ENABLE_CPLUSPLUS
#define STRSAFE_NO_DEPRECATE

#define _WINSOCK_DEPRECATED_NO_WARNINGS		//error C4996: 'WSAAddressToStringA': Use WSAAddressToStringW() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings

/*影响std::tuple等模板的参数个数*/
#if defined(_VARIADIC_MAX)
#  error _VARIADIC_MAX 最好自己来定义
#  undef _VARIADIC_MAX
#endif /* !defined(_VARIADIC_MAX) */
#define _VARIADIC_MAX 10
/*函数参数最大个数*/
#define CMN_ARITY_MAX 15

#ifdef _DEBUG
#define _WG_TEST
#else
//vmp开关
//#define __VMP__
/**/
#ifdef __RELEASE_LOG_INFO_
#define _LOG_OUT_BY_LEVEL Log_info
#endif
/**/
#endif

#include <SDKDDKVer.h>

//lua使用json时要定义此宏
#define _LUA_USE_JSON
//lua使用poco解析http字符串时要定义此宏
#define _LUA_USE_POCO_PARSE_HTTP_STR
//lua使用多字符串匹配要定义此宏
#define _LUA_USE_MULTI_STR_MATCH
//version of dx input
#define DIRECTINPUT_VERSION 0x0800

//使用windows API，因为Poco库有个unwindows.h文件，把windows API都给undef了
#define POCO_NO_UNWINDOWS
//diable some warnings
#define _SCL_SECURE_NO_WARNINGS
/**/
#ifdef POD_TO_CLASS
#define POD_TO_CLASS_UNWIND(code) code
#define POD_TO_CLASS_COMMA ,
#else
#define POD_TO_CLASS_UNWIND(code)
#define POD_TO_CLASS_COMMA
#endif
#define POD_TO_CLASS_VIRTUAL POD_TO_CLASS_UNWIND(virtual)
/**/
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//////////////////////////////////////////////////////////////////////////
//************************处理警告****************************************
//处理类似这样的警告：warning LNK4099: PDB 'lib.pdb' was not found with
//#pragma warning(disable:4099)
//////////////////////////////////////////////////////////////////////////
//************wxLua
#include <luabind_wx/wxlua_config.h>
//////////////////////////////////////////////////////////////////////////
