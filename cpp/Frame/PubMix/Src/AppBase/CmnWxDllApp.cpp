#include "stdafx.h"
#include <AppBase/CmnWxDllApp.h>
#include <wx/wxprec.h>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/thread/thread.hpp>


bool Cmn_DllApp::DllMain(HINSTANCE hModule, int ul_reason_for_call)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		wxSetInstance(hModule);
		if (!DoInit())
		{
			LOG_O(Log_debug) << "初始化失败";
			//assert(false);
			return false;
		}
		if (!DoRun())
		{
			assert(false);
			return false;
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (!DoExit())
		{
			assert(false);
			return false;
		}
		break;
	default:
		assert(false);
		return false;
	}
	return true;
}

Cmn_DllApp::Cmn_DllApp() : Cmn_WxAppWithInitor(this)
{
	is_add_main_thred_ = false;
}

bool Cmn_DllApp::DoExit()
{
	if (!__super::DoExit())
		return false;

	return 0 == OnExit_();
}

bool Cmn_DllApp::DoRun()
{
	return OnRun_();
}

bool Cmn_DllApp::DoInit()
{
	if (!__super::DoInit())
		return false;
	return OnInit_();
}

bool Cmn_DllApp::OnRun_()
{
	return true;
}

void Cmn_DllApp::PreKill(const ThreadablePtr& thrd, const Cmn_Threadable::ThrdIoServicePtr& thrd_io_service)
{
	assert(thrd);
	thrd->EnableFroceTerminate(true);
	if (thrd->IsThrdAlive() && thrd_io_service)
	{
		auto thrd_id = thrd->GetThrdId();
		/*
		如果是强制杀死的线程，那么就得保存thrd_io_service的备份，防止boost::asio::io_service调用析构函数，因为该函数会阻塞DLL_PROCESS_DETACH。
		如果正确未抛出CmnExitIoThreadException异常，那么便会造成thrd_io_service的内存泄露，但是总好过进程卡死。
		*/
		auto io_service_cpy = thrd_io_service;
		thrd->GetIoService().post([thrd_id, io_service_cpy](){
			LOG_O(Log_debug) << "DllMain要释放线程了，线程ID：" << thrd_id;
			throw CmnExitIoThreadException("DllMain exit");
		});
		wxMilliSleep(500);
	}
}

void Cmn_DllApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_DllApp, Cmn_WxAppWithInitor>("Cmn_DllApp")
		];
}

bool Cmn_DllRunApp::IsIoThrd() const
{
	return true;
}

void Cmn_DllRunApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<Cmn_DllRunApp, Cmn_DllApp>("Cmn_DllRunApp")
		];
}

Cmn_DllRunAppFactory* Cmn_DllRunApp::CreateAppFactory()
{
	return new Cmn_DllRunAppFactory;
}

bool Cmn_DllRunApp::DoRun()
{
	boost::thread::attributes thrd_attr;
	thrd_attr.set_stack_size(io_thread_stack_size_);
	boost::thread t(thrd_attr, [this](){
		auto res = OnEntry();
		if (res)
		{
			assert(false);
			return;
		}
	});
	return true;
}

bool Cmn_DllRunApp::OnRun_()
{
	return Cmn_WxAppWithInitor::OnRun_();
}

bool Cmn_DllRunApp::DoInit()
{
	if (!Cmn_WxAppWithInitor::DoInit())
		return false;
	return true;
}

bool Cmn_DllRunApp::DoExit()
{
	if (!Cmn_WxAppWithInitor::DoExit())
		return false;
	return true;
}

Cmn_DllRunApp::Cmn_DllRunApp()
{
	is_add_main_thred_ = true;
	io_thread_stack_size_ = 0;
}

LuaThrdData* Cmn_DllRunAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new LuaThrdData(lstate);
}
