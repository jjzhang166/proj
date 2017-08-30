#pragma once
#include <Net/PocoMore.h>
#include <functional>

TimeDuration& GetUpdateTimeDuration();
TimeDuration& GetSendTimeDuration();
void OutputScriptInfo( const std::string& str );
int G_MessageBoxTimeoutA( LPCSTR lpText, LPCSTR lpCaption, DWORD dwMilliseconds );
