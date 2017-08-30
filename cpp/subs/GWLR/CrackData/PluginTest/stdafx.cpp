// stdafx.cpp : source file that includes just the standard includes
// PluginTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <CmnMix/Cmn_UsefulFunc.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>

bool GWndExecSync( const std::function<bool()>& func ){
	return func();
}

bool CD_FuncCall::SendImplSync(stCD_MsgBase& msg)
{
	return SendImpl(msg);
}
void __stdcall DummySleep(unsigned long milli_seconds){
	::Sleep(milli_seconds);
}