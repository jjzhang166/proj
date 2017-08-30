#pragma once
/*
@author zhp
@date 2016/2/22 1:05
@purpose for image/pe/load dll from memory...
*/
#include <WinNT.h>

class CmnImageHlp{
public:
	static PIMAGE_NT_HEADERS AnalyaisImage( HMODULE hModule );
	static DWORD GetPEImageSize( HMODULE hModule );
};