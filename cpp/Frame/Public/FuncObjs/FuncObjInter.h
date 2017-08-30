#pragma once
/*
@author zhp
@date 2014-1-01
@purpose 相同进程的多个进程间通信使用
*/
#include "stdafx.h"
#include "FuncObj.h"
#include <boost/function.hpp>
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>

//可用于console
//interface internal
class FuncObj_IInter : public FuncObj{
public:
	FuncObj_IInter(FuncObj* parent);
};

class FuncObj_Inter : public FuncObj_IInter{
protected:
	FuncObj_Inter(FuncObj_Inter* sink, FuncObj* parent);

public:
	typedef FunctionVoid ExecFnType;
	//通知消息
	static const UINT kMsgNotify = MAXWORD - 1111;

public:
	virtual void Close();
	//异步执行
	virtual bool AsyncExec(const ExecFnType& async_func) = 0;
	//检验状态
	bool GetState();
	//设置状态
	void SetState(bool state);

protected:
	bool OperEncode(P_FuncObjOper oper, const void* param, pt_dword param_size, 
		FuncObj* msg_owner) override;

private:
	//decode
	void Decode(P_Main* p_main, pt_dword msg_size);

protected:
	bool				state_;
	FuncObj_Inter*		sink_;
};

//只能有一个实例
//这个对象在游戏主窗口所属的线程中执行
class FuncObj_GWnd : public FuncObj_Inter{
public:
	FuncObj_GWnd( HWND hwnd, FuncObj_Inter* sink );
	~FuncObj_GWnd();

public:
	//new_hwnd为空，则重置成之前的
	bool ResetHwnd( HWND new_hwnd );
	bool AsyncExec(const ExecFnType& async_func) override;
	//同步调用
	template<typename ResultT, typename FuncT>
	bool SyncExec( ResultT& res, const FuncT& func ){
		ExecFnType fn = [&res, &func](){
			res = func();
		};
		return SyncExec(fn);
	}
	bool SyncExec( const ExecFnType& func );
	//get the wnd handle
	HWND GetWndHandle() const;
	//让这个窗口显示在最前
	void ForegroundWindow() const;
	DWORD GetWndThrdId() const;

protected:
	HWND		hwnd_;
	DWORD		thread_id_;
};

//只能有一个实例
//这个对象在game.dll开辟的线程中执行
class FuncObj_Inject : public FuncObj_Inter{
protected:
	FuncObj_Inject( FuncObj_Inter* sink, FuncObj* parent );

public:
	bool AsyncExec(const ExecFnType& async_func) override;

protected:
	//virtual FuncObj_GWnd* CreateGWnd();

protected:
};

class FuncObj_InterInjectComposite : public FuncObj_Inject{
public:
	FuncObj_InterInjectComposite( FuncObj* parent, HWND hwnd );
	FuncObj_GWnd& GetInterGWnd();
	void DeleteSelf();

protected:
	FuncObj_GWnd			gwnd_;
};