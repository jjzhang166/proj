#include "stdafx.h"
#include "GFuncCall.h"
#include "GAddr.h"
#include "GOffset.h"
#include "GForeach.h"
#include "GClass.h"
#include "FuncObjC.h"
#include "../../CrackData/dlgfifacalltest/command/UsefulFuncs.h"

using namespace GAddr;
using namespace GOffset;
using namespace GForeach;
using namespace GType;
using namespace GClass;

void GFuncCall::CloseNoticeDlg()
{

}

bool GFuncCall::IsExistTrainer()
{
	return false;
}

bool GFuncCall::IsExistValidTrainer()
{
	return false;
}

bool GFuncCall::IsTrainerSelectedScene()
{
	return false;
}

void* GFuncCall::MallocOfGame( size_t size )
{
	return g_pGMalloc.Call<void*>(size);
}

void* GFuncCall::CallocOfGame( size_t n, size_t size )
{
	return g_pGCalloc.Call<void*>(n, size);
}

void GFuncCall::FreeOfGame( void* p )
{
	g_pGFree.Call<void>(p);
}

void* GFuncCall::GetSllReadHandle()
{
	/*void* ssl_handle = 0;
	MemOperApplyR(ssl_handle, GAddr::g_pGameBase, GOffset::g_o_HttpRecvSllHandle);
	return ssl_handle;*/

	return (void*)g_hSslHandle;
}

void GFuncCall::PressKeyImpl( DWORD v_k, UINT msg )
{
	DO_EXCEPTION_TRY;
	::PressKey(v_k, msg);
	DO_CATCH_ALL_MSG_BOX_CODE(
		return;);
}

void GFuncCall::LeftBtClickImpl( int x, int y )
{
	DO_EXCEPTION_TRY;
	::FIFA_MouseLeftClick(x, y);
	DO_CATCH_ALL_MSG_BOX_CODE(
		return;);
}
