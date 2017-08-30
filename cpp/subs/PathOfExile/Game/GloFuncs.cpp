#include "stdafx.h"
#include <GloFuncs.h>
#include <Common/CmnDllApi.h>
#include "Facility/GPlayer.h"

TimeDuration& GetUpdateTimeDuration()
{
	static TimeDuration td(1000);
	return td;
}

TimeDuration& GetSendTimeDuration()
{
	//发包间隔最好不要小于1秒
	static TimeDuration td(1500);
	return td;
}

int G_MessageBoxTimeoutA( LPCSTR lpText, LPCSTR lpCaption, DWORD dwMilliseconds )
{
	return MessageBoxTimeout(nullptr, lpText, lpCaption, MB_OKCANCEL, 0, dwMilliseconds);
}
