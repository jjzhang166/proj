#pragma once
/*
@author zhp
@date 2016/2/5 9:17
@purpose add lib
*/
#include <config/targetver.h>

#ifdef _DEBUG
#pragma comment(lib, "PublicX_d.lib")
#else
#pragma comment(lib, "PublicX.lib")
#endif

#pragma comment(lib, "Advapi32.lib")
