#ifndef LUACOM_H
#define LUACOM_H


#define LUACOM_VERSION   "LuaCOM 1.3b 2005-01-06"
#define LUACOM_COPYRIGHT "Copyright (C) 1998-2005 Tecgraf, PUC-Rio"
#define LUACOM_AUTHORS   "V. Almendra & R. Cerqueira & F. Mascarenhas"

#ifndef LUACOM_API
//#define LUACOM_API __declspec(dllexport)
#define LUACOM_API extern
#endif

enum
{
  LUACOM_AUTOMATION,
  LUACOM_NOAUTOMATION,
  LUACOM_REGISTER,
  LUACOM_UNREGISTER,
  LUACOM_AUTOMATION_ERROR
};

#ifdef __cplusplus
extern "C"
{
#endif

#include <lua/lua.h>


LUACOM_API void luacom_open(lua_State *L);
LUACOM_API void luacom_close(lua_State *L);

LUACOM_API int luacom_IDispatch2LuaCOM(lua_State *L, void *pdisp_arg);

LUACOM_API int luacom_detectAutomation(lua_State *L, int argc, char *argv[]);


#ifdef __cplusplus
}
#endif

//如果想开启html help，就把下面的给注释掉
#define NO_HTMLHELP

#ifndef NO_HTMLHELP
#pragma comment(lib, "Htmlhelp.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "luacomd.lib")
#else
#pragma comment(lib, "luacom.lib")
#endif



#endif
