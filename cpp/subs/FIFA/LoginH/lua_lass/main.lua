--dofile==每次都要编译。loadfile==只编译(加载)文件一次
--require不会多次加载同一个文件。

__g_lua_files__ = {
	"Logic"
};

--load all lua files for initial
LoadAndReloadLuaFiles(LoginH_LuaInst());
