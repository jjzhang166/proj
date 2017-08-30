#include "stdafx.h"
#include "GloFuncs.h"
#include "Facility/GFuncCall.h"

void KeyClick( DWORD v_k )
{
	using namespace GFuncCall;
	GWndExecSync([v_k](){
		PressKeyImpl(v_k, WM_KEYDOWN);
		return true;
	});
	DummySleep(100);
	GWndExecSync([v_k](){
		PressKeyImpl(v_k, WM_KEYUP);
		return true;
	});
}

void LeftBtClick( int x, int y )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "×ó¼üµã»÷,x:" << x << " y:" << y;
	using namespace GFuncCall;
	GWndExecSync([=](){
		LeftBtClickImpl(x, y);
		return true;
	});
	DummySleep(40);
}

void PressEsc()
{
	KeyClick(VK_ESCAPE);
}

void PressReturn()
{
	KeyClick(VK_RETURN);
}

void PressBackSpace()
{
	KeyClick(VK_BACK);
}
