#pragma once
/*
@author zhp
@date 2016/3/24 18:22
@purpose add lib
*/
#include <config/targetver.h>
#include <config/AdjustWin32.h>

#ifdef _DEBUG
#pragma comment(lib, "PubMix.lib")
#else
	#ifdef _LOG_OUTPUT_EMPTY
	#pragma comment(lib, "PubMix_LE.lib")
	#elif defined(_LOG_OUT_BY_LEVEL)
	#pragma comment(lib, "PubMix_LI.lib")
	#else
	#pragma comment(lib, "PubMix_LE.lib")
	#endif
#endif
