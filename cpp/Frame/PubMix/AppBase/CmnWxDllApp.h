#pragma once
/*
@author zhp
@date 2016/11/16 0:40
@purpse for dll,该文件中的内容原以为可以与CmnWxApp.h中的内容放到一起，最好不要，因为初始化问题。
*/
#include "CmnWxApp.h"


class Cmn_DllApp : public Cmn_WxAppWithInitor, public Cmn_ThrdMgrBase::ThrdMgrSink{
protected:
	Cmn_DllApp();

public:
	bool DllMain(HINSTANCE hModule, int ul_reason_for_call);

protected:
	bool OnRun_() override;
	void BindFrame(lua_State* l) override;

	//不建议在下面的函数内做事，所以是private声明
private:
	bool DoInit() override;
	bool DoRun() override;
	bool DoExit() override;
	void PreKill(const ThreadablePtr& thrd, const Cmn_Threadable::ThrdIoServicePtr& thrd_io_service) override final;
};

class Cmn_DllRunAppFactory : public Cmn_AppBaseFactory{
public:
	LuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
};

class Cmn_DllRunApp : public Cmn_DllApp{
public:
	Cmn_DllRunApp();
	bool IsIoThrd() const override;

protected:
	bool OnRun_() override;
	void BindFrame(lua_State* l) override;
	Cmn_DllRunAppFactory* CreateAppFactory() override;

	//不建议在下面的函数内做事
protected:
	bool DoInit() override;
	bool DoRun() override;
	bool DoExit() override;

protected:
	std::size_t			io_thread_stack_size_;
};