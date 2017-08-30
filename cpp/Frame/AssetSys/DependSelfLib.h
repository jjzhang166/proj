#pragma once
/*
@author zhp
@date 2016/2/5 9:34
@purpose add the depend library
*/
#ifdef _DEBUG
#pragma comment(lib, "AssetSysd.lib")
#pragma comment(lib, "lua51d.lib")
#pragma comment(lib, "luabindd.lib")
#else
#pragma comment(lib, "AssetSys.lib")
#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "luabind.lib")
#endif
