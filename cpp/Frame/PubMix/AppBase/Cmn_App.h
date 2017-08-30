#pragma once
/*
@author zhp
@date 2016/11/6 16:03
@purpose for application base
*/
#include "Cmn_Thrd.h"
#include "../DependSelfLib.h"
#include <boost/preprocessor/cat.hpp>
#include <boost/shared_ptr.hpp>
#include <AssetSys/AS_Node.h>
#include <CmnMix/Cmn_LuaFile.h>
//////////////////////////////////////////////////////////////////////////
//Cmn_AppBase的对象最好是new出来的。
#define CMN_MY_APP_DECLARE(t)	t& GetMyApp()
#define CMN_MY_WX_APP_DECLARE(t)	\
	wxDECLARE_APP(t);	\
	CMN_MY_APP_DECLARE(t)
/**/
#define CMN_MY_APP_DEFINE(t)	\
	t& GetMyApp(){ \
		struct stHelper{	\
			stHelper() : app_(new t){}		\
			boost::shared_ptr<t>	app_; };	\
			static stHelper	app;		\
		return *app.app_; }	\
	struct BOOST_PP_CAT(_CALL_FIRST_MY_APP_, __LINE__){	BOOST_PP_CAT(_CALL_FIRST_MY_APP_, __LINE__)(){ GetMyApp(); } };	\
	BOOST_PP_CAT(_CALL_FIRST_MY_APP_, __LINE__)		BOOST_PP_CAT(__CALL_FIRST_MY_APP_, __LINE__)
#define CMN_MY_WX_APP_DEFINE(t)	\
	wxIMPLEMENT_APP(t);	\
	t& GetMyApp(){	\
		return wxGetApp();	\
	}
#define CMN_MY_DLLAPP_DEFINE(t)	\
	int __stdcall DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID _Reserved)	\
	{	return GetMyApp().DllMain((HINSTANCE)hModule, ul_reason_for_call);	}	\
	CMN_MY_APP_DEFINE(t)
/**/
//////////////////////////////////////////////////////////////////////////
class CmnLoginConnectCfg;
class Cmn_AppBaseFactory{
private:
	Cmn_AppBaseFactory(const Cmn_AppBaseFactory&) = delete;
	Cmn_AppBaseFactory& operator = (const Cmn_AppBaseFactory&) = delete;

public:
	Cmn_AppBaseFactory();
	virtual ~Cmn_AppBaseFactory(){}
	template<typename T>
	T* CastTo(){
#ifdef _DEBUG
		return dynamic_cast<T*>(this);
#else
		return static_cast<T*>(this);
#endif
	}
	template<typename T>
	T& CastAs(){
		return *CastTo<T>();
	}

public:
	virtual Cmn_LuaCfgResolver* CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag);
	virtual CmnLoginConnectCfg* CreateCheckServerConnection(const std::string& svr_ip);
	virtual LuaThrdData* CreateAppLuaVm(lua_State* lstate);
};

class Cmn_AppBase : public Cmn_ThrdBase, public Cmn_ThrdMgrBase{
protected:
	Cmn_AppBase(Cmn_ThrdMgrBase::ThrdMgrSink* sink = nullptr);

public:
	~Cmn_AppBase();
	static Cmn_AppBase& GetAppBase();
	AS_NodePtr GetScriptDir() const;
	const char* GetScriptDirName() const;
	AS_NodePtr GetCfgDir() const;
	AS_NodePtr GetAssetDir() const;
	Cmn_LuaCfgResolver* GetLuaCfgResolver() const;
	const boost::filesystem::path& GetSelfModulePath() const;

public:
	ThrdIdT GetThrdId() const override final;
	bool IsThrdAlive() const override final;
	bool DoRun() override;
	bool Pause() override final;
	bool Resume() override final;

	//配置相关
public:
	//是否允许dump
	bool IsEnableDump() const;
	//设置是否dump
	void SetEnableDump(bool is_enable_dump);

	//static
public:
	//默认Shell到Io线程
	static luabind::object Shell(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	static Cmn_AppBaseFactory& GetAppFactory();

protected:
	bool OnInit_() override;
	int  OnExit_() override;
	void OnIoService();
	using Cmn_ThrdBase::DoInit;
	using Cmn_ThrdBase::DoRun;
	void AddMainThrd();
	virtual Cmn_AppBaseFactory* CreateAppFactory();

protected:
	void BindFrame(lua_State* l) override;
	void ActiveIoService() override;
	using Cmn_ThrdBase::OnEntry;

private:
	bool InitAS();
	LuaThrdData* CreateLuaVm() override;
	bool IsDebug() const;

private:
	static luabind::object __Shell_c__(const char* lua_cmd);
	static luabind::object __Shell_cc__(const char* lua_cmd, const char* cmd_name);
	static luabind::object __Shell_cb__(const char* lua_cmd, bool async);
	static luabind::object __Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool async);

protected:
	boost::filesystem::path		self_module_path_;
	bool						is_log_dbgview_;
	bool						is_log_console_;
	bool						is_add_main_thred_;

	bool						is_enable_dump_;
	ThrdIdT						main_thrd_id_;
	HANDLE						main_thrd_handle_;
	Cmn_AppBaseFactory*			app_factory_;
	const char*					cfg_name_;
	const char*					script_dir_name_;
	const char*					asset_dir_name_;

private:
	const char*					log_file_name_;

private:
	boost::shared_ptr<Cmn_LuaCfgResolver>	lua_cfg_relover_;
};

class LuaCfgParser{
	LuaCfgParser(const LuaCfgParser&) = delete;
	LuaCfgParser& operator = (const LuaCfgParser&) = delete;
public:
	LuaCfgParser();
	virtual ~LuaCfgParser(){}
	virtual bool ParseLuaCfg(const luabind::object& lua_cfg);

public:
	bool	auto_run_;
};
