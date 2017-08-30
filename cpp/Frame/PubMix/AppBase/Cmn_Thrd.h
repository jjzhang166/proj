#pragma once
/*
@author zhp
@date 2016/11/6 15:47
@purpose for thread base
*/

#include "../DependSelfLib.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <CmnMix/Cmn_IoService.h>
#include <set>
#include <typeinfo>
#include <CmnMix/Cmn_LuaOper.h>
#include <boost/enable_shared_from_this.hpp>
#include <wx/thread.h>
#include <boost/thread/shared_mutex.hpp>


class Cmn_Threadable;
typedef boost::shared_ptr<Cmn_Threadable>	ThreadablePtr;
typedef boost::weak_ptr<Cmn_Threadable>	ThreadableWeakPtr;
class Cmn_Threadable : public boost::enable_shared_from_this<Cmn_Threadable>{
	friend class Cmn_ThrdMgrBase;
	typedef boost::shared_ptr<IoService::work> IoWorkPtr;

public:
	//IoService::work work(GetIoService());	可以保证work生命结束前，io_s_的run不会退出。
	class ThrdIoService : public IoService{
	private:
		friend class Cmn_Threadable;
		explicit ThrdIoService(Cmn_Threadable& thrd);
		

	public:
		template<typename T>
		void post(const T& f){
			thrd_.ActiveIoService();
			return __super::post(f);
		}
		template<typename T>
		void dispatch(const T& f){
			thrd_.ActiveIoService();
			return __super::dispatch(f);
		}

	private:
		Cmn_Threadable&		thrd_;
	};
	typedef boost::shared_ptr<ThrdIoService> ThrdIoServicePtr;
	typedef unsigned long ThrdIdT;

protected:
	Cmn_Threadable();

public:
	virtual ~Cmn_Threadable(){}
	ThrdIoService& GetIoService();
	ThrdIoServicePtr GetIoServicePtr() const;
	bool HasIoService() const;
	static Cmn_Threadable& GetCurThreadable();
	virtual bool IsIoThrd() const;
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

	//thread operate
public:
	virtual ThrdIdT GetThrdId() const = 0;
	virtual bool IsThrdAlive() const = 0;
	virtual bool Pause() = 0;
	virtual bool Resume() = 0;
	void EnableFroceTerminate(bool force_teminate);
	LuaThrdData* GetLuaVm() const;
	lua_State* lua() const;
	luabind::object& GetMyPermanents();

public:
	static LuaThrdData& GetMyLuaVm();
	//由其他线程调用
	luabind::object DoShell(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	void DoShell(const luabind::object& fn);
	//Shell Main
	static luabind::object ShellM(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	static luabind::object ShellM(const std::string& lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	//IO线程
	static luabind::object ShellIo(const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	static luabind::object ShellIo(const std::string& lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	//Shell到目标
	static luabind::object Shell(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	static luabind::object Shell(ThrdIdT dst_thrd_id, const std::string& lua_cmd, const char* cmd_name = nullptr, bool sync = false);
	//异步执行，在同一线程中
	static void Shell(const luabind::object& fn);

	//由其他线程执行
public:
	virtual bool DoInit() = 0;
	virtual bool DoRun() = 0;
	virtual bool DoExit();

	//只能由执行线程执行
protected:
	virtual bool OnInit_();
	virtual bool OnRun_() = 0;
	virtual int  OnExit_();

protected:
	virtual LuaThrdData* CreateLuaVm();
	virtual void LuaVmSetting(LuaThrdData* lua_vm) const;
	virtual void BindFrame(lua_State* l);
	virtual void ActiveIoService() = 0;

private:
	static luabind::object __ShellM_c__(const char* lua_cmd);
	static luabind::object __ShellM_cc__(const char* lua_cmd, const char* cmd_name);
	static luabind::object __ShellM_cb__(const char* lua_cmd, bool sync);
	static luabind::object __ShellM_ccb__(const char* lua_cmd, const char* cmd_name, bool sync);
	static luabind::object __ShellIo_c__(const char* lua_cmd);
	static luabind::object __ShellIo_cc__(const char* lua_cmd, const char* cmd_name);
	static luabind::object __ShellIo_cb__(const char* lua_cmd, bool sync);
	static luabind::object __ShellIo_ccb__(const char* lua_cmd, const char* cmd_name, bool sync);
	static luabind::object __Shell_ic__(ThrdIdT dst_thrd_id, const char* lua_cmd);
	static luabind::object __Shell_icc__(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name);
	static luabind::object __Shell_icb__(ThrdIdT dst_thrd_id, const char* lua_cmd, bool sync);
	static luabind::object __Shell_iccb__(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name, bool sync);
	static void __Shell_o__(const luabind::object& fn);

protected:
	LuaThrdData*			my_lvm_;
	bool					force_terminate_;

private:
	ThrdIoServicePtr		io_s_;
	wxCriticalSection		io_s_section_;
	IoWorkPtr				io_s_dlt_work_;
	luabind::object			my_permanents_;
};

class Cmn_ThrdMgrBase{
public:
	typedef boost::shared_mutex SharedMutexT;
	typedef boost::shared_lock<SharedMutexT> ReadLockT;
	typedef boost::unique_lock<SharedMutexT> WriteLockT;
	class ThrdMgrSink{
	public:
		virtual void PreKill(const ThreadablePtr& thrd, const Cmn_Threadable::ThrdIoServicePtr& thrd_io_service) = 0;
	};

protected:
	Cmn_ThrdMgrBase(ThrdMgrSink* sink = nullptr);

public:
	Cmn_Threadable::ThrdIdT AddThrd(const ThreadablePtr& thrd, bool is_do_fun = true, bool is_do_init = true);
	bool KillThrd(Cmn_Threadable::ThrdIdT thrd_id);
	ThreadablePtr FindThrd(Cmn_Threadable::ThrdIdT thrd_id);
	int KillAll();
	ThreadablePtr GetIoThrd();

private:
	void RecycleThrd();

protected:
	SharedMutexT				thrds_mutex_;
	std::set<ThreadablePtr>		thrds_;

private:
	ThrdMgrSink*				sink_;
};

class Cmn_ThrdBase : public Cmn_Threadable{
protected:
	Cmn_ThrdBase();

public:
	bool DoInit() override;

protected:
	bool OnRun_() override;
	const std::type_info* OnEntry();

private:
	const std::type_info* PreRun_CatchImpl();
	void ProtectOnRun();
	bool InnerRepeatDoIoRun_();
	void InnerThreadDoSomeErr_();

protected:
	bool		is_inited_;
	bool		is_poll_;
};
