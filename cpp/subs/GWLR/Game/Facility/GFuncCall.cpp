#include "stdafx.h"
#include "GFuncCall.h"
#include "GClass.h"
#include "FuncObjC.h"
#include "GloFuncs.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CmnMix/Cmn_MemOper.h>

bool IsTheFbIdJuQingDone(int fb_id)
{
	auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
	task_fb_info.Update();
	return task_fb_info.IsTheFbIdJuQingDone(fb_id);
}

bool IsTheFbIdJuQingTask( int fb_id )
{
	auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
	task_fb_info.Update();
	return task_fb_info.IsTheFbIdJuQingTask(fb_id);
}

int GFC_GetShangJinCompleteCount()
{
	int res = 0;
	if ( !GWndExecSyncT(res, [](){
		DO_EXCEPTION_TRY;
		return CD_FuncCall::GetShangJinCompleteCnt();
		DO_CATCH_ALL_MSG_BOX_CODE(
			return 0;);
	}) )
	{
		assert(false);
	}
	return res;	
}

int GFC_GetShangJinTotalCount()
{
	int res = 0;
	if ( !GWndExecSyncT(res, [](){
		DO_EXCEPTION_TRY;
		return CD_FuncCall::GetShangJinTotalCnt();
		DO_CATCH_ALL_MSG_BOX_CODE(
			return 0;);
	}) )
	{
		assert(false);
	}
	return res;
}

int GFC_GetShangJinTaskStatus( DWORD dwShangJinTaskId )
{
	int res = 0;
	if ( !GWndExecSyncT(res, [dwShangJinTaskId](){
		DO_EXCEPTION_TRY;
		return CD_FuncCall::GetShangJinTaskState(dwShangJinTaskId);
		DO_CATCH_ALL_MSG_BOX_CODE(
			return 0;);
	}) )
	{
		assert(false);
	}
	return res;
}

int GFC_GetMsgRoleCityRoomIndex()
{
	int res = -1;
	if ( !GWndExecSyncT(res, [](){
		DO_EXCEPTION_TRY;
		auto minimap = CD_FuncCall::FindUiMng_Minimap();
		if (!minimap)
		{
			assert(false);
			return -1;
		}
		return minimap->room_num_;
		DO_CATCH_ALL_MSG_BOX_CODE(
			return -1;);
	}) )
	{
		assert(false);
	}
	return res;
}

void GFC_TidyInventoryItem(CD_En_TabCategory nTabSoltIndex)
{
	GWndExecSync([nTabSoltIndex](){
		DO_EXCEPTION_TRY;
		CD_FuncCall::TidyInventoryItem(nTabSoltIndex);
		DO_CATCH_ALL_MSG_BOX_CODE(
			return false;);
		return true;
	});
}

LONGLONG GFC_GetSynCoorDinatesRandom()
{
	LONGLONG res = 0;
	GWndExecSyncT(res, [](){
		DO_EXCEPTION_TRY;
		return CD_FuncCall::GetSynCoorDinatesRandom();
		DO_CATCH_ALL_MSG_BOX_CODE(
			return LONGLONG(0););
	});
	return res;
}

//ÊÇ·ñ¿ªÆôÎÞµÐ
volatile bool g_is_enable_wu_di_ = true;
void GFC_EnableWuDi( bool enable_wu_di )
{
	g_is_enable_wu_di_ = enable_wu_di;
}

stCD_UiCtrl* GFC_Ui_TraUi(const char* cls_name)
{
	stCD_UiCtrl* res = nullptr;
	GWndExecSyncT(res, [=](){
		DO_EXCEPTION_TRY;
		return CD_FuncCall::FindUiCtrlByFullClsName(cls_name);
		DO_CATCH_ALL_MSG_BOX_CODE(
			return res;);
	});
	return res;
}

bool GFC_Ui_ClickButton( const char* cls_name )
{
	return CD_FuncCall::UiCtrl_LeftBtClickByFullClsName(cls_name);
}

bool GFC_Ui_PutString( const char* cls_name, const wchar_t* the_str )
{
	bool res = false;
	GWndExecSyncT(res, [=](){
		DO_EXCEPTION_TRY;
		auto obj = CD_FuncCall::FindUiCtrlByFullClsName(cls_name);
		if (!obj)
			return false;
		return CD_FuncCall::UiCtrlEdit_PutString(obj, the_str);
		DO_CATCH_ALL_MSG_BOX_CODE(
			return res;);
	});
	return res;
}
