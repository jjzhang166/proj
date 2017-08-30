#pragma once
/*
@author zhp
@date 2016/12/5 14:54
@purpose for dependence
*/
#include "wxlua_config.h"
#include <luabind/luabind.hpp>
#include <wx/wxprec.h>

#ifdef _DEBUG
#pragma comment(lib, "luabind_wxd.lib")
#else
#pragma comment(lib, "luabind_wx.lib")
#endif
