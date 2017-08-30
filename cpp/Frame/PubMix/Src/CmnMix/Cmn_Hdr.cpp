#include "stdafx.h"
#include <CmnMix/Cmn_Hdr.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <CommonX/CmnX_DllApi.h>

void chMB(PCSTR szMsg)
{
	char szTitle[MAX_PATH];
	GetModuleFileNameA(NULL, szTitle, _countof(szTitle));
	MessageBoxTimeout(GetActiveWindow(), szMsg, szTitle, MB_OK, 0, 1000 * 20);
}

void chFAIL(PSTR szMsg)
{
	chMB(szMsg);
	DebugBreak();
}

void chASSERTFAIL(LPCSTR file, int line, PCSTR expr) {
	char sz[2*MAX_PATH];
	wsprintfA(sz, "File %s, line %d : %s", file, line, expr);
	chFAIL(sz);
}

void chSETDLGICONS(HWND hWnd, int idi) {
	SendMessage(hWnd, WM_SETICON, ICON_BIG,  (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
}