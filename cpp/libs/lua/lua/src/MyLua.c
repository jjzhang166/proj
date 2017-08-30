#include "MyLua.h"


int luaMy_tblsort(lua_State *L)
{
	int sort(lua_State *L);
	return sort(L);
}

int luaMy_tostring(lua_State *L)
{
	int luaB_tostring(lua_State *L);
	return luaB_tostring(L);
}

int luaMy_DbgTraceback(lua_State *L)
{
	int db_errorfb(lua_State *L);
	return db_errorfb(L);
}
