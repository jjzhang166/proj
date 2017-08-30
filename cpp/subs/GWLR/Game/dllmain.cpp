// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <TlHelp32.h>
#include <boost/thread.hpp>
#include "FuncObjC.h"
#include <cassert>
#include "protect/Protect.h"
#include <Communicate/CGL_Defines.h>
#include <Common/EnsureCleanup.h>
#include <CommonX/CmnX_HookerMore.h>

//全局初始化
extern bool InitCodeHooker( HookerPtrStored& hooks );
extern bool InitCodeHooker_Pre( HookerPtrStored& hooks );
extern void EndOfRun();

std::shared_ptr<LogXmlInstanceHelper> g_log_xml_helper;
std::shared_ptr<LogDbgviewInstanceHelper> g_log_dbg_view;
HookerPtrStored g_hooks_;

//io_service run
void IoRun(){
	VMP_BEGIN_EX;
	//init log
	g_log_dbg_view.reset(new LogDbgviewInstanceHelper);
	if ( !g_log_dbg_view )
	{
		assert(false);
		return;
	}
	boost::shared_ptr<FuncObj_NormalC> con_account(new FuncObj_NormalC);

	BOOST_SCOPE_EXIT_ALL(&){
		//SupendProcess(true);
		g_log_xml_helper.reset();
		LogSinkInitor_Xml::FlushToTempFile();
		EndOfRun();
		g_hooks_.Clear();
	};
	if ( !con_account->InitGame() )
		return;
	con_account->ConnectAsync();
	GetIoService().post([](){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return;
		if ( !InitCodeHooker(g_hooks_) )
		{
			norc->Disconnect_Prompt("非常严重的错误");
			return;
		}
		if ( !norc->WaitForConnectDone() )
			return;		
		if ( !g_log_dbg_view )
			return;
		auto log_pre_str = norc->account_;
		log_pre_str += "-";
		log_pre_str += norc->game_ser_name_;
		g_log_dbg_view->SetDbgViewPreString(log_pre_str);

		g_log_xml_helper.reset(new LogXmlInstanceHelper(GlobalSetter::GetMe().GetSelfModuleHandle(), _T("LogGame")));
		if ( !g_log_xml_helper )
		{
			assert(false);
			return;
		}
		g_log_xml_helper->SetPreFileName(log_pre_str);
	});
	//恢复游戏线程
	//SupendProcess(false);
	ProtectDoIoRun();
	
	VMP_END;
}

void ThreadRunStaticQueue()
{
	CmnX_AntiHookCheck::GetRealMe().AddThreadWhiteList(::GetCurrentThreadId());
	GetStaticTaskQueue().RunTasks();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			auto h_real = LoadLibraryA("lpka.dll");

			auto& gs = GlobalSetter::GetMe();
			gs.SetSelfModuleHandle(hModule);
#ifdef _DEBUG
			gs.SetEnableDump(true);
#endif
			auto load_thread_id = ::GetCurrentThreadId();
			CmnX_AntiHookCheck::SetRealMudule(h_real);
			auto& anti_crc = CmnX_AntiHookCheck::GetRealMe();
			anti_crc.AddThreadWhiteList(load_thread_id);
			if ( !InitCodeHooker_Pre(g_hooks_) )
			{
				assert(false);
				return FALSE;
			}
			anti_crc.EraseThreadWhiteList(load_thread_id);

			//break;
			boost::thread t([](){
				CmnX_AntiHookCheck::GetRealMe().AddThreadWhiteList(::GetCurrentThreadId());
				/*auto the_type_info = PreIoRun(&IoRun);
				if ( !the_type_info )
					return;*/
				PreIoRun(&IoRun);
				static_assert(sizeof(LONG) == 4, "");
				//if ( *the_type_info == typeid(CmnCloseProcessException) )
				{
					auto& gs = GlobalSetter::GetMe();
					if( gs.GetGameMainWnd() && ::IsWindow(gs.GetGameMainWnd()) )
					{
						POINT pt;
						pt.x = 443;
						pt.y = 426;

						::Sleep(500);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONDOWN, 1, MAKELPARAM(pt.x, pt.y));
						::Sleep(300);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));

						::Sleep(500);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONDOWN, 1, MAKELPARAM(pt.x, pt.y));
						::Sleep(300);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));

						::Sleep(1000);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONDOWN, 1, MAKELPARAM(pt.x, pt.y));
						::Sleep(500);
						::SendMessage(gs.GetGameMainWnd(), WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));

						::Sleep(1000 * 6);
					}
					TerminateSelfProcess();
				}
			});
			gs.GetIoThread().swap(t);

			//static task queue
			boost::thread t_static_queue(ThreadRunStaticQueue);
			break;
		}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}