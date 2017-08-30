#pragma once
/*
@author zhp
@date 2016/2/5 9:34
@purpose add the depend library
*/
#include <config/targetver.h>

#ifdef _DEBUG
#pragma comment(lib, "libeay32_d.lib")
#pragma comment(lib, "ssleay32_d.lib")
#pragma comment(lib, "public.lib")
#else
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
	#ifdef _LOG_OUTPUT_EMPTY
	#pragma comment(lib, "public_LE.lib")
	#elif defined(_LOG_OUT_BY_LEVEL)
	#pragma comment(lib, "public_LI.lib")
	#else
	#pragma comment(lib, "public_LE.lib")
	#endif
#endif
#pragma comment(lib, "crypt32.lib")
