// stdafx.cpp : source file that includes just the standard includes
// PluginTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <CmnMix/Cmn_UsefulFunc.h>

bool GWndExecSync( const std::function<bool()>& func ){
	return func();
}
