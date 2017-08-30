#pragma once
/*
@author zhp
@date 2017/4/3 15:36
@purpose for android
*/
#include "ConsoleComponent.h"
#include "AndroidExecEnv.h"

//////////////////////////////////////////////////////////////////////////
class AndroidApp_Console;
class AndroidAppUiOperIo;
class AndroidAppUiOperMain;
class AndroidLoginSessionS;
class AndroidConsoleSessionS;
class AndroidGameSessionOper;
class AndroidLoginSessionOper;
class AndroidConsoleAppFactory;
class AndroidApp_Console;
class AndroidThrdIo_Console;
//////////////////////////////////////////////////////////////////////////
class AndroidAppUiOperMain : public ConsoleAppUiOperMain{
public:
	typedef MEmuExecEnv::VmsOper VmsOper;
	typedef std::set<std::string> VmNamesT;

public:
	AndroidAppUiOperMain(AndroidApp_Console& app, MEmuExecEnv& exec_env);
	void GetIo_NewLoginProcessInfo(const std::string& item_key, const VmNamesT& black_name_list);
	AndroidThrdIo_Console* GetThrdIo();
	AndroidAppUiOperIo* GetUiOperIo();

private:
	MEmuExecEnv&		exec_env_;
	VmsOper				vms_;
	VmsOper				running_vms_;
};

class AndroidAppUiOperIo : public ConsoleAppUiOperIo{
public:
	AndroidAppUiOperIo(AndroidThrdIo_Console& thrd_io);
	AndroidAppUiOperMain* GetAppOperMain();
	AndroidApp_Console* GetApp();
	AndroidThrdIo_Console* GetIoThrd();

public:
	bool OnLoginedOne(const std::string& process_info, long pid, const std::string& item_key) override;

protected:
	int GetMaxLoginingCnt() const override;
	bool DoLoginOne(const std::string& key) override;
};
//////////////////////////////////////////////////////////////////////////
//**************opers
class AndroidLoginSessionOper : public ConsoleLoginSessionOper{
public:
	AndroidLoginSessionOper(ConsoleLoginSessionS& session);

protected:
	void OnS_AttachLoginSession(const std::string& item_key) override;

private:
	BC_REG_CLS_DECLARE(AndroidLoginSessionOper);
};

class AndroidGameSessionOper : public ConsoleSessionOper{
public:
	AndroidGameSessionOper(ConsoleSessionS& session);

private:
	void OnS_AttachGameSession(const std::string& item_key) override;

private:
	BC_REG_CLS_DECLARE(AndroidGameSessionOper);
};
//////////////////////////////////////////////////////////////////////////
//***********sessions
class AndroidLoginSessionS : public ConsoleLoginSessionS{
public:
	AndroidLoginSessionS(Server& server);

protected:
	bool OnConnected(IoConnectBase* connect) override;
	void OnS_LoginDisconnect() override;
};

class AndroidConsoleSessionS : public ConsoleSessionS{
public:
	AndroidConsoleSessionS(Server& server);
	static AndroidThrdIo_Console& GetIoThrd();

protected:
	void OnDisconnect(IoConnectBase* connect) override;
	void OnS_GameDisconnect() override;
};
//////////////////////////////////////////////////////////////////////////
class AndroidThrdIo_Console : public Cmn_ThrdIo_Console{
public:
	typedef AndroidAppUiOperMain::VmNamesT VmNamesT;

public:
	virtual void OnSetGamePath(const std::string& game_path);
	void AddCheckCloseMemu(const std::string& menu_process_info);
	void EraseCheckCloseMemus(const VmNamesT& erased_menu);
	void EraseCheckCloseMemu(const std::string& erased_menu);
	const VmNamesT& GetEraseCheckCloseMemus() const;
	static AndroidApp_Console& GetApp();

protected:
	int OnExit_() override;

private:
	IoTimerPtr		timer_check_close_memu_;
	VmNamesT		check_close_menu_set_;
};

class AndroidConsoleAppFactory : public ConsoleAppFactory{
public:
	AndroidThrdIo_Console* CreateIoThrd() override;
	AndroidAppUiOperMain* CreateAppUiOper(WxLuaApp_Console& app) override;
	AndroidAppUiOperIo* CreateIoUiOper(Cmn_ThrdIo_Console& io_thrd) override;
	AndroidLoginSessionS* CreateLoginSession(ConsoleLoginServer& server) override;
	AndroidConsoleSessionS* CreateGameSession(ConsoleServer& server) override;
	AndroidGameSessionOper* CreateGameSessionOper(ConsoleSessionS& session) override;
	AndroidLoginSessionOper* CreateLoginSessionOper(ConsoleLoginSessionS& session) override;
	MEmuExecEnv* CreateExecEnv() override;
};

class AndroidApp_Console : public WxLuaApp_Console{
public:
	AndroidApp_Console();
	~AndroidApp_Console();
	AndroidConsoleAppFactory& GetAppFactory() const;
	MEmuExecEnv& GetExecEnv() const;
	void DoCheckClosedMenu(const AndroidThrdIo_Console::VmNamesT& menus);
	AndroidThrdIo_Console* GetIoThrd_();
	static AndroidApp_Console& GetApp();

protected:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	void OnSetGamePath(const std::string& game_path, bool is_reset) override;
	AndroidConsoleAppFactory* CreateAppFactory() override;

private:
	std::string DetectGamePath() const;

private:
	MEmuExecEnv*		memu_exec_env_;
};
//////////////////////////////////////////////////////////////////////////