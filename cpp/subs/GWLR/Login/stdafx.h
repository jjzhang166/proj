// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "../prever.h"
#include <config/targetver.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <afxwin.h>         // MFC core and standard components

#include <config/AdjustWin32.h>

#pragma comment(lib, "CrackCaptchaAPI.lib")

const TCHAR* const kDaMaPicRelativePath = _T("TenProtect\\ui\\res\\");
