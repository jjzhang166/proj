#pragma once
#include <Net/PocoMore.h>
#include "Facility/GTypes.h"
#include <functional>

template<typename ResultT, typename FuncT>
bool GWndExecSyncT( ResultT& res, const FuncT& func )
{
	/*res = func();
	return true;*/
	auto gwnd = FuncObj_NormalC::GetGWnd();
	if ( !gwnd || !::IsWindow(gwnd->GetWndHandle()) )
	{
		//assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "游戏发生错误，关掉游戏jfdka";
		GetIoService().post([](){
			FuncObj_NormalC::Disconnect_Prompt("未知错误2");
		});
		//throw CmnCloseProcessException("jfdc,/");
		return false;
	}
	if ( !gwnd->SyncExec(res, func) )
	{
		bool is_executed = false;
		gwnd->SyncExec(is_executed, [&is_executed](){
			return true;
		});
		if ( !is_executed )
		{
			//assert(false);
			VERIFY(gwnd->ResetHwnd(nullptr));
			if (gwnd->SyncExec(res, func))
				return true;
			assert(false);
			FuncObj_NormalC::Disconnect_Prompt("未知错误3");
		}
		//assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "同步执行返回假";
		return false;
	}
	return true;
}
TimeDuration& GetUpdateTimeDuration();
TimeDuration& GetSendTimeDuration();
bool SetItemQualityImpl(int item_quality, CD_En_ItemQuality& out_item_quality);
int Offset2DllAddr( int offset );
void DxKeyUpDown( DWORD virtual_key, bool is_down );
void OutputScriptInfo( const std::string& str );
int G_MessageBoxTimeoutA( LPCSTR lpText, LPCSTR lpCaption, DWORD dwMilliseconds );
class GItem;
int PackageSomeItemToStore( const std::string& item_name, int cnt, const std::function<bool(const GItem&)>& item_filter );
int StoreSomeItemToPackage(const std::string& item_name, int cnt, const std::function<bool(const GItem&)>& item_filter);
//////////////////////////////////////////////////////////////////////////
void BeginHandleRubThrd();
void EnablePassRoom(bool enable);
//////////////////////////////////////////////////////////////////////////