#pragma once
/*
@author zhp
@date 2016/11/14 17:11
@purpose for wx thread
*/
#include "Cmn_Thrd.h"
#include <wx/thread.h>

class Cmn_WxThrd : public Cmn_ThrdBase, protected wxThread{
public:
	Cmn_WxThrd();
	ThrdIdT GetThrdId() const override final;
	bool IsThrdAlive() const override;
	bool Pause() override final;
	bool Resume() override final;

public:
	bool DoRun() override;
	bool DoExit() override;

private:
	ExitCode Entry() override final;
	void ActiveIoService() override final;
};

class Cmn_ThrdIo : public Cmn_WxThrd{
public:
	bool IsIoThrd() const;

public:
	//默认Shell到主线程
	static luabind::object Shell(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);

private:
	static luabind::object __Shell_c__(const char* lua_cmd);
	static luabind::object __Shell_cc__(const char* lua_cmd, const char* cmd_name);
	static luabind::object __Shell_cb__(const char* lua_cmd, bool sync);
	static luabind::object __Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool sync);

protected:
	void BindFrame(lua_State* l) override;
	void LuaVmSetting(LuaThrdData* lua_vm) const override;
};

class Cmn_Thrd : public Cmn_WxThrd{
public:
	//默认Shell到Io线程
	static luabind::object Shell(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);

private:
	static luabind::object __Shell_c__(const char* lua_cmd);
	static luabind::object __Shell_cc__(const char* lua_cmd, const char* cmd_name);
	static luabind::object __Shell_cb__(const char* lua_cmd, bool sync);
	static luabind::object __Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool sync);

protected:
	void BindFrame(lua_State* l) override;
	void LuaVmSetting(LuaThrdData* lua_vm) const override;
};
