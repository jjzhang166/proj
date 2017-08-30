#include "stdafx.h"
#include "FuncObjs/FuncObjInter.h"
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <Common/UsefulFunc.h>
#include <boost/pool/singleton_pool.hpp>
#include "Common/LogOut.h"
#include <Common/CmnException.h>
#include <Common/UsefulClass.h>
#include <boost/scope_exit.hpp>
#include <Common/CmnDllApi.h>
#include <AppBase/Cmn_App.h>

class SingletonPool : public boost::singleton_pool<FuncObj_Inter::ExecFnType,
	sizeof(FuncObj_Inter::ExecFnType)>{
public:
	template<typename T>
	static FuncObj_Inter::ExecFnType* Construct( T param ){
		auto func = (FuncObj_Inter::ExecFnType*)malloc();
		if ( !func )
			return nullptr;
		return new (func) FuncObj_Inter::ExecFnType(param);
	}
	static void Destroy( FuncObj_Inter::ExecFnType* func ){
		if ( !func )
			return;
		func->FuncObj_Inter::ExecFnType::~ExecFnType();
		free(func);
	}
};
bool SafeExecNewWndProc( FuncObj_Inter::ExecFnType* func, LPARAM lParam ){
	try{
		assert(func);
		BOOST_SCOPE_EXIT_ALL(=){
			if (lParam)
				SingletonPool::Destroy(func);
		};
		(*func)();
		return true;
	}
	DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "senwpde1->" << e.what();
		assert(false);
	})
	DO_CATCH_ALL_EXCEPTION({
		LOG_O(Log_fatal) << "senwpde2";
		assert(false);
	})
	return false;	
}

bool SafeExecNewWndProc_Dump( FuncObj_Inter::ExecFnType* func, LPARAM lParam ){
	bool res = false;
	__try{
		res = SafeExecNewWndProc(func, lParam);
	}
	__except (HandleCrash(EXCEPTION_EXECUTE_HANDLER)){
		auto f = [](){
			LOG_O(Log_debug) << "SafeExecNewWndProc_Dump之出现了C++未能捕获的SEH异常";
		};
		f();
		assert(false);
	}
	return res;
}

bool SafeExecNewWndProc_Final( FuncObj_Inter::ExecFnType* func, LPARAM lParam ){
	try{
		if (Cmn_AppBase::GetAppBase().IsEnableDump())
			return SafeExecNewWndProc_Dump(func, lParam);
		else
			return SafeExecNewWndProc(func, lParam);
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "senwpde2";
		//assert(false);
	})
	return false;
}
//hook wnd
WNDPROC old_wnd_proc_ = nullptr;
LRESULT CALLBACK NewWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( FuncObj_Inter::kMsgNotify == message )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "游戏被hook住的主窗口在处理注入线程请求的事件";*/
		auto func = (FuncObj_Inter::ExecFnType*)wParam;
		if ( !IsUnValidPtr(func) )
			return SafeExecNewWndProc_Final(func, lParam);
		assert(false);
	}
	return CallWindowProc(old_wnd_proc_, hWnd, message, wParam, lParam);
}

FuncObj_Inter::FuncObj_Inter( FuncObj_Inter* sink, FuncObj* parent ) : 
	FuncObj_IInter(parent)
{
	state_ = false;
	sink_ = sink;
	assert(sink);
}

void FuncObj_Inter::Close()
{
	if (state_)
	{
		state_ = false;
		if (sink_->state_)
			sink_->AsyncExec(boost::bind(&FuncObj_Inter::Close, sink_));
	}
}

void FuncObj_Inter::Decode( P_Main* p_main, pt_dword msg_size )
{
	if ( !p_main )
	{
		assert(false);
		return;
	}

	if ( !state_ )
	{
		free(p_main);
		return;
	}
	if ( msg_size == p_main->GetTotalLength() )
	{
		if (p_main->Decode(*this))
		{
			free(p_main);
			return;
		}
	}
	LOG_O_EMPTY_MSG_FAST(Log_error);
	free(p_main);
	Close();
}

bool FuncObj_Inter::OperEncode( P_FuncObjOper oper, const void* param, pt_dword param_size,
							   FuncObj* msg_owner )
{
	if ( !state_ )
		return false;

	auto mem_size = P_Main::CalcNeedMemory(msg_owner ? *msg_owner : *this, oper, param, param_size);
	if ( 0 == mem_size )
		return false;
	auto p_main = (P_Main*)malloc(mem_size);
	assert(p_main);
	if ( !p_main->Encode(*this, oper, param, param_size, mem_size) )
	{
		free(p_main);
		Close();
		return false;
	}
	sink_->AsyncExec(boost::bind(&FuncObj_Inter::Decode, sink_, p_main, mem_size));
	return true;
}

bool FuncObj_Inter::GetState()
{
	return state_;
}

void FuncObj_Inter::SetState( bool state )
{
	state_ = state;
}

FuncObj_Inject::FuncObj_Inject( FuncObj_Inter* sink, FuncObj* parent ) : FuncObj_Inter(sink, parent)
{
}

bool FuncObj_Inject::AsyncExec( const ExecFnType& async_func )
{
	if ( !state_ )
		return false;

	GetIoService().post(async_func);
	return true;
}

FuncObj_GWnd::FuncObj_GWnd( HWND hwnd, FuncObj_Inter* sink ) : FuncObj_Inter(sink, nullptr)
{
	hwnd_ = hwnd;
	thread_id_ = 0;
	assert(IsWindow(hwnd_));
	if ( !old_wnd_proc_ )
	{
		old_wnd_proc_ = (WNDPROC)::SetWindowLongPtr(hwnd_, GWLP_WNDPROC,
			(INT_PTR)NewWndProc);
		if ( !old_wnd_proc_ )
		{
			MessageBoxTimeout(nullptr, nullptr, "gwnderr", MB_OK, 0, 1000 * 20);
			throw CmnCloseProcessException("gwnderr");
		}
		LOG_O(Log_trace) << "old wnd proc:" << std::hex << (DWORD)old_wnd_proc_ << " new wnd proc:" << std::hex << (DWORD)NewWndProc << std::dec;
		DWORD process_id = 0;
		thread_id_ = GetWindowThreadProcessId(hwnd_, &process_id);
		assert(0 != thread_id_);
	}
}

bool FuncObj_GWnd::AsyncExec( const ExecFnType& async_func )
{
	if ( !state_ || !IsWindow(hwnd_) )
		return false;
	if ( thread_id_ == GetCurrentThreadId() )
	{
		async_func();
		return true;
	}
	return !!::PostMessage(hwnd_, kMsgNotify, (WPARAM)SingletonPool::Construct(async_func), !!"需要删除ExecFnType对象指针");
}

FuncObj_GWnd::~FuncObj_GWnd()
{
	if ( hwnd_ && old_wnd_proc_ )
	{
		(WNDPROC)::SetWindowLongPtr(hwnd_, GWLP_WNDPROC,
			(INT_PTR)old_wnd_proc_);
		old_wnd_proc_ = nullptr;
	}
}

bool FuncObj_GWnd::SyncExec( const ExecFnType& func )
{
	if ( !state_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "state不对";
		return false;
	}
	if ( !IsWindow(hwnd_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "不是窗口, hwnd->" << hwnd_;
		return false;
	}

	if ( thread_id_ == GetCurrentThreadId() )
	{
		func();
		return true;
	}
	return !!::SendMessage(hwnd_, kMsgNotify, (WPARAM)&func, !"不需要删除ExecFnType对象指针");
}

HWND FuncObj_GWnd::GetWndHandle() const
{
	return hwnd_;
}

void FuncObj_GWnd::ForegroundWindow() const
{
	ForegroundWnd(hwnd_);
}

bool FuncObj_GWnd::ResetHwnd( HWND new_hwnd )
{
	if ( hwnd_ && old_wnd_proc_ )
	{
		(WNDPROC)::SetWindowLongPtr(hwnd_, GWLP_WNDPROC,
			(INT_PTR)old_wnd_proc_);
		old_wnd_proc_ = nullptr;
	}
	else
		assert(false);
	if ( new_hwnd != nullptr )
		hwnd_ = new_hwnd;
	thread_id_ = 0;
	assert(IsWindow(hwnd_));
	if ( !old_wnd_proc_ )
	{
		old_wnd_proc_ = (WNDPROC)::SetWindowLongPtr(hwnd_, GWLP_WNDPROC,
			(INT_PTR)NewWndProc);
		if ( !old_wnd_proc_ )
		{
			MessageBoxTimeout(nullptr, nullptr, "gwnderr341", MB_OK, 0, 1000 * 10);
			throw CmnCloseProcessException("gwnderr321");
		}
		LOG_O(Log_trace) << "old wnd proc:" << std::hex << (DWORD)old_wnd_proc_ << " new wnd proc:" << std::hex << (DWORD)NewWndProc << std::dec;
		DWORD process_id = 0;
		thread_id_ = GetWindowThreadProcessId(hwnd_, &process_id);
		assert(0 != thread_id_);
		return 0 != thread_id_;
	}
	return false;
}

DWORD FuncObj_GWnd::GetWndThrdId() const
{
	return thread_id_;
}

FuncObj_IInter::FuncObj_IInter( FuncObj* parent )
	: FuncObj(parent, kFuncObjTemplateId_Inter)
{

}

FuncObj_InterInjectComposite::FuncObj_InterInjectComposite(FuncObj* parent, HWND hwnd)
	: FuncObj_Inject(&gwnd_, parent), gwnd_(hwnd, this)
{
	SetState(true);
	gwnd_.SetState(true);
}

FuncObj_GWnd& FuncObj_InterInjectComposite::GetInterGWnd()
{
	return gwnd_;
}

void FuncObj_InterInjectComposite::DeleteSelf()
{
	gwnd_.SyncExec(boost::bind(&FuncObj_GWnd::Close, &gwnd_));
	Close();
	GetIoService().post([this](){
		delete this;
	});
}
