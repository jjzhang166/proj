#include "stdafx.h"
#include <AppBase/Cmn_Thrd.h>
#include <boost/scope_exit.hpp>
#include <AppBase/Cmn_App.h>
#include <CmnMix/Cmn_Exception.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <CmnMix/Cmn_LogOut.h>
#include <pluto/pluto.h>
#include <CmnMix/Cmn_PocoMore.h>

Cmn_ThrdBase::Cmn_ThrdBase()
{
	is_inited_ = false;
	is_poll_ = false;
}

bool Cmn_ThrdBase::DoInit()
{
	return true;
}

const std::type_info* Cmn_ThrdBase::PreRun_CatchImpl(){
	//VMP_BEGIN_EX;
	try
	{
		if (is_inited_)
		{
			OnRun_();
		}
		else
		{
			is_inited_ = true;
			if (!OnInit_())
			{
				assert(false);
				return nullptr;
			}
			BOOST_SCOPE_EXIT_ALL(this){
				OnExit_();
			};
			OnRun_();
		}		
	} DO_CATCH_THE_EXCEPTION(const CmnCastException& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_CastFailed->" << e.what();
		DoExit();
		return &typeid(e);
	}) DO_CATCH_THE_EXCEPTION(const CmnExitIoThreadException& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_ExitIoThread->" << e.what();
		DoExit();
	}) DO_CATCH_THE_EXCEPTION(const CmnCloseProcessException& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_CloseProcess->" << e.what();
		/*stCloseWndHelper close_hlp;
		auto app_ptr = AfxGetApp();
		if (app_ptr)
		{
			auto main_wnd = app_ptr->GetMainWnd();
			if (main_wnd)
			{
				main_wnd->SendMessage(WM_CLOSE);
				close_hlp.but_hwnds_.push_back(main_wnd->m_hWnd);
			}
		}
		close_hlp.process_id_ = GetCurrentProcessId();
		EnumWindows(stCloseWndHelper::EnumWindowsProc__, (LPARAM)&close_hlp);*/
		//exit(0);
		DoExit();
		return &typeid(e);
	}) DO_CATCH_THE_EXCEPTION(const CmnTerminateProcessException& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_TerminateProcess->" << e.what();
		DoExit();
		TerminateSelfProcess();
		assert(false);
		return &typeid(e);
	}) DO_CATCH_THE_EXCEPTION(const CmnExceptionBase& e, {
		const auto typeid_ptr = &typeid(e);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "CmnExceptionBase[" << typeid_ptr->name() << "]->" << e.what();
		DoExit();
		assert(false);
		return typeid_ptr;
	}) DO_CATCH_THE_EXCEPTION(const char* str, {
		assert(str);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "str exception->" << str;
		DoExit();
	}) DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "std unknown exception->" << e.what();
		DoExit();
	})
	DO_CATCH_ALL_EXCEPTION({
		LOG_O(Log_debug) << "出现了未知C++异常";
		DoExit();
	})
	//VMP_END;
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "正常退出线程，线程ID:" << GetThrdId();*/
	return nullptr;
}

bool Cmn_ThrdBase::OnRun_()
{
	ProtectOnRun();
	return true;
}

void Cmn_ThrdBase::ProtectOnRun()
{
	//VMP_BEGIN_EX;
	while (InnerRepeatDoIoRun_())
	{
		if (is_poll_)
			break;
	};
	//VMP_END;
}

bool Cmn_ThrdBase::InnerRepeatDoIoRun_()
{
	//VMP_BEGIN_EX;
	try
	{
		InnerThreadDoSomeErr_();
	}
	DO_CATCH_THE_EXCEPTION(const CmnReDoIoRunException& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_ReDoIoRun->" << e.what();
		return true;
	})
	//VMP_END;
	return false;
}

void Cmn_ThrdBase::InnerThreadDoSomeErr_()
{
	//VMP_BEGIN_EX;
	try
	{
		if (is_poll_)
			GetIoService().Poll();
		else
			GetIoService().Run();
		return;
	}
	DO_CATCH_THE_EXCEPTION(const CmnLetIoThreadDoErr& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "kET_LetIoThreadDoSomeErr->" << e.what();
	})

	while (true)
	{
		GetIoService().PollOne();
	}
	//VMP_END;
}

const std::type_info* Cmn_ThrdBase::OnEntry()
{
	__try{
		return PreRun_CatchImpl();
	}
	__except (HandleCrash(EXCEPTION_EXECUTE_HANDLER)){
		auto f = [](){
			LOG_O(Log_debug) << "Cmn_ThrdBase::OnEntry之出现了C++未能捕获的SEH异常";
		};
		f();
		assert(false);
	}
	return nullptr;
}

Cmn_Threadable::ThrdIoService& Cmn_Threadable::GetIoService()
{
	if (io_s_)
		return *io_s_;
	wxCriticalSectionLocker locker(io_s_section_);
	if (io_s_)
		return *io_s_;
	io_s_.reset(new ThrdIoService(*this));
	assert(io_s_);
	auto& res = *io_s_;
	//work在析构的时候会stop  IoService。
	io_s_dlt_work_.reset(new IoService::work(res));
	return res;
}

bool Cmn_Threadable::OnInit_()
{
	my_lvm_ = CreateLuaVm();
	if (my_lvm_)
	{
		LuaVmSetting(my_lvm_);
		assert(my_lvm_->lstate_);
		LuaThrdData::RegLuabind(my_lvm_->lstate_);
		BindFrame(my_lvm_->lstate_);
		LuaThrdData::SetMe(my_lvm_);
	}
	return true;
}

LuaThrdData& Cmn_Threadable::GetMyLuaVm()
{
	return LuaThrdData::GetMe();
}

Cmn_Threadable::Cmn_Threadable()
{
	my_lvm_ = nullptr;
	force_terminate_ = false;
}

luabind::object Cmn_Threadable::DoShell(const char* lua_cmd, const char* cmd_name, bool sync)
{
	if (!lua_cmd)
	{
		assert(false);
		return luabind::object();
	}
	if (!IsThrdAlive())
	{
		assert(false);
		return luabind::object();
	}
	if (!my_lvm_)
	{
		assert(false);
		return luabind::object();
	}
	auto& app_me = Cmn_AppBase::GetAppBase();
	const auto& cur_thrd = app_me.FindThrd(GetThrdId());
	if (!cur_thrd)
	{
		assert(false);
		return luabind::object();
	}
	const auto& called_thrd = app_me.FindThrd(wxThread::GetCurrentId());
	if (!called_thrd)
	{
		assert(false);
		return luabind::object();
	}
	assert(called_thrd->IsThrdAlive());
	if (sync && cur_thrd == called_thrd)
		return my_lvm_->DoBuffer(lua_cmd, strlen(lua_cmd), cmd_name ? cmd_name : "Shell");
	ThreadableWeakPtr weak_self = cur_thrd;
	std::shared_ptr<std::string> str_cmd(new std::string(lua_cmd));
	std::shared_ptr<std::string> str_cmd_name;
	if (cmd_name)
		str_cmd_name.reset(new std::string(cmd_name));
	if (sync)
	{
		if (!called_thrd->my_lvm_)
		{
			assert(false);
			return luabind::object();
		}
		typedef boost::shared_ptr<std::string> CallResStrT;
		CallResStrT the_res_str;
		ThreadableWeakPtr notified(called_thrd);
		boost::shared_ptr<bool> is_receive(new bool(false));
		assert(is_receive);
		boost::weak_ptr<bool> weak_receive(is_receive);
		this->GetIoService().post([weak_self, str_cmd, str_cmd_name, notified, weak_receive, &the_res_str](){
			CallResStrT res_str;
			if (notified.expired())
			{
				assert(false);
				return;
			}
			auto notified_ptr = notified.lock();
			if (!notified_ptr)
			{
				assert(false);
				return;
			}
			if (weak_receive.expired())
			{
				assert(false);
				return;
			}
			{
				BOOST_SCOPE_EXIT_ALL(&notified_ptr, &res_str, &weak_receive, &the_res_str){
					notified_ptr->GetIoService().post([res_str, weak_receive, &the_res_str](){
						if (weak_receive.expired())
						{
							assert(false);
							return;
						}
						auto ptr_receive = weak_receive.lock();
						if (!ptr_receive)
						{
							assert(false);
							return;
						}
						the_res_str = res_str;
						assert(false == *ptr_receive);
						*ptr_receive = true;
					});
				};
				assert(str_cmd);
				assert(!weak_self.expired());
				auto& self = *weak_self.lock();
				assert(self.my_lvm_);
				auto call_res = self.my_lvm_->DoBuffer(str_cmd->c_str(), str_cmd->size(), str_cmd_name ? str_cmd_name->c_str() : "Shell");
				if (call_res.is_valid())
				{
					res_str.reset(new std::string(LuaSerialize::PlutoPersist(self.GetMyPermanents(), call_res)));
				}
			}
		});
		auto& io_ser = called_thrd->GetIoService();
		while (io_ser.RunOne()){
			if (*is_receive)
				break;
		}
		if (!the_res_str || the_res_str->empty())
			return luabind::object();
		return LuaSerialize::PlutoUnpersist(called_thrd->GetMyPermanents(), *the_res_str);
	}
	else
	{
		this->GetIoService().post([weak_self, str_cmd, str_cmd_name](){
			assert(str_cmd);
			assert(!weak_self.expired());
			auto& self = *weak_self.lock();
			assert(self.my_lvm_);
			self.my_lvm_->DoBuffer(str_cmd->c_str(), str_cmd->size(), str_cmd_name ? str_cmd_name->c_str() : "Shell");
		});
		return luabind::object();
	}	
}

void Cmn_Threadable::DoShell(const luabind::object& fn)
{
	assert(wxThread::GetCurrentId() == GetThrdId());
	using namespace luabind;
	if (!fn.is_valid() || LUA_TFUNCTION != type(fn))
	{
		assert(false);
		return;
	}
	GetIoService().post([fn](){
		LuaObjCall<void>(fn);
	});
}

luabind::object Cmn_Threadable::Shell(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name, bool sync)
{
	auto& dst_thrd = Cmn_AppBase::GetAppBase().FindThrd(dst_thrd_id);
	if (!dst_thrd)
	{
		assert(false);
		return luabind::object();
	}
	return dst_thrd->DoShell(lua_cmd, cmd_name, sync);
}

luabind::object Cmn_Threadable::Shell(ThrdIdT dst_thrd_id, const std::string& lua_cmd, const char* cmd_name /*= nullptr*/, bool sync /*= false*/)
{
	return Shell(dst_thrd_id, lua_cmd.c_str(), cmd_name, sync);
}

void Cmn_Threadable::Shell(const luabind::object& fn)
{
	GetCurThreadable().DoShell(fn);
}

bool Cmn_Threadable::IsIoThrd() const
{
	return false;
}

luabind::object Cmn_Threadable::ShellM(const char* lua_cmd, const char* cmd_name, bool sync)
{
	auto& app = Cmn_AppBase::GetAppBase();
	auto& main_thrd = app.FindThrd(app.GetThrdId());
	if (!main_thrd)
	{
		assert(false);
		return luabind::object();
	}
	return main_thrd->DoShell(lua_cmd, cmd_name, sync);
}

luabind::object Cmn_Threadable::ShellM(const std::string& lua_cmd, const char* cmd_name /*= nullptr*/, bool sync /*= false*/)
{
	return ShellM(lua_cmd.c_str(), cmd_name, sync);
}

luabind::object Cmn_Threadable::ShellIo(const char* lua_cmd, const char* cmd_name, bool sync)
{
	auto& io_thrd = Cmn_AppBase::GetAppBase().GetIoThrd();
	if (!io_thrd)
	{
		assert(false);
		return luabind::object();
	}
	return io_thrd->DoShell(lua_cmd, cmd_name, sync);
}

luabind::object Cmn_Threadable::ShellIo(const std::string& lua_cmd, const char* cmd_name /*= nullptr*/, bool sync /*= false*/)
{
	return ShellIo(lua_cmd.c_str(), cmd_name, sync);
}

void Cmn_Threadable::BindFrame(lua_State* l)
{
	using namespace luabind;
	module(l)
		[
			def("ShellM", &Cmn_Threadable::__ShellM_c__),
			def("ShellM", &Cmn_Threadable::__ShellM_cc__),
			def("ShellM", &Cmn_Threadable::__ShellM_cb__),
			def("ShellM", &Cmn_Threadable::__ShellM_ccb__),
			def("ShellIo", &Cmn_Threadable::__ShellIo_c__),
			def("ShellIo", &Cmn_Threadable::__ShellIo_cc__),
			def("ShellIo", &Cmn_Threadable::__ShellIo_cb__),
			def("ShellIo", &Cmn_Threadable::__ShellIo_ccb__),
			def("Shell", &Cmn_Threadable::__Shell_ic__),
			def("Shell", &Cmn_Threadable::__Shell_icc__),
			def("Shell", &Cmn_Threadable::__Shell_icb__),
			def("Shell", &Cmn_Threadable::__Shell_iccb__),
			def("Shell", &Cmn_Threadable::__Shell_o__),

			class_<IteratorBase>("IteratorBase")
			.def("First", &IteratorBase::First)
			.def("Next", &IteratorBase::Next)
			.def("IsDone", &IteratorBase::IsDone)
		];
	luaopen_pluto(l);
	DateTimeLocal::RegForLua(l);
}

int Cmn_Threadable::OnExit_()
{
	io_s_dlt_work_.reset();
	if (io_s_)
		GetIoService().stop();
	if (my_permanents_.is_valid())
	{
		luabind::object().swap(my_permanents_);
	}
	return 0;
}

bool Cmn_Threadable::DoExit()
{
	if (io_s_)
		GetIoService().stop();
	return true;
}

void Cmn_Threadable::LuaVmSetting(LuaThrdData* lua_vm) const
{
	lua_vm->SetScriptDirName(Cmn_AppBase::GetAppBase().GetScriptDirName());
}

LuaThrdData* Cmn_Threadable::CreateLuaVm()
{
	return nullptr;
}

Cmn_Threadable& Cmn_Threadable::GetCurThreadable()
{
	auto& app_base = Cmn_AppBase::GetAppBase();
	auto thrd = app_base.FindThrd(wxThread::GetCurrentId());
	if (!thrd)
	{
		thrd = app_base.GetIoThrd();
		assert(thrd);
	}
	return *thrd;
}

void Cmn_Threadable::EnableFroceTerminate(bool force_teminate)
{
	force_terminate_ = force_teminate;
}

bool Cmn_Threadable::HasIoService() const
{
	return !!io_s_;
}

luabind::object Cmn_Threadable::__ShellM_c__(const char* lua_cmd)
{
	return ShellM(lua_cmd);
}

luabind::object Cmn_Threadable::__ShellM_cc__(const char* lua_cmd, const char* cmd_name)
{
	return ShellM(lua_cmd, cmd_name);
}

luabind::object Cmn_Threadable::__ShellM_cb__(const char* lua_cmd, bool sync)
{
	return ShellM(lua_cmd, nullptr, sync);
}

luabind::object Cmn_Threadable::__ShellM_ccb__(const char* lua_cmd, const char* cmd_name, bool sync)
{
	return ShellM(lua_cmd, cmd_name, sync);
}

luabind::object Cmn_Threadable::__ShellIo_c__(const char* lua_cmd)
{
	return ShellIo(lua_cmd);
}

luabind::object Cmn_Threadable::__ShellIo_cc__(const char* lua_cmd, const char* cmd_name)
{
	return ShellIo(lua_cmd, cmd_name);
}

luabind::object Cmn_Threadable::__ShellIo_cb__(const char* lua_cmd, bool sync)
{
	return ShellIo(lua_cmd, nullptr, sync);
}

luabind::object Cmn_Threadable::__ShellIo_ccb__(const char* lua_cmd, const char* cmd_name, bool sync)
{
	return ShellIo(lua_cmd, cmd_name, sync);
}

luabind::object Cmn_Threadable::__Shell_ic__(ThrdIdT dst_thrd_id, const char* lua_cmd)
{
	return Shell(dst_thrd_id, lua_cmd);
}

luabind::object Cmn_Threadable::__Shell_icc__(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name)
{
	return Shell(dst_thrd_id, lua_cmd, cmd_name);
}

luabind::object Cmn_Threadable::__Shell_icb__(ThrdIdT dst_thrd_id, const char* lua_cmd, bool sync)
{
	return Shell(dst_thrd_id, lua_cmd, nullptr, sync);
}

luabind::object Cmn_Threadable::__Shell_iccb__(ThrdIdT dst_thrd_id, const char* lua_cmd, const char* cmd_name, bool sync)
{
	return Shell(dst_thrd_id, lua_cmd, cmd_name, sync);
}

luabind::object& Cmn_Threadable::GetMyPermanents()
{
	if (my_permanents_.is_valid())
		return my_permanents_;
	if (my_lvm_)
	{
		luabind::newtable(my_lvm_->lua()).swap(my_permanents_);
	}
	else
	{
		assert(false);
	}
	return my_permanents_;
}

LuaThrdData* Cmn_Threadable::GetLuaVm() const
{
	return my_lvm_;
}

void Cmn_Threadable::__Shell_o__(const luabind::object& fn)
{
	return Shell(fn);
}

Cmn_Threadable::ThrdIoServicePtr Cmn_Threadable::GetIoServicePtr() const
{
	return io_s_;
}

lua_State* Cmn_Threadable::lua() const
{
	auto lua_vm = GetLuaVm();
	if (!lua_vm)
		return nullptr;
	return lua_vm->lua();
}

Cmn_Threadable::ThrdIdT Cmn_ThrdMgrBase::AddThrd(const ThreadablePtr& thrd, bool is_do_fun, bool is_do_init)
{
	if (!thrd)
	{
		assert(false);
		return 0;
	}
	if (is_do_init && !thrd->DoInit())
	{
		assert(false);
		return 0;
	}
	if (is_do_fun && !thrd->DoRun())
	{
		assert(false);
		return 0;
	}
	if (!thrd->IsThrdAlive())
	{
		assert(false);
		return 0;
	}
	RecycleThrd();
	auto thrd_id = thrd->GetThrdId();
	auto old = FindThrd(thrd_id);
	if (old)
	{
		assert(false);
		if (!KillThrd(thrd_id))
		{
			assert(false);
			return 0;
		}
	}
	WriteLockT locker(thrds_mutex_);
	thrds_.insert(thrd);
	return thrd_id;
}

ThreadablePtr Cmn_ThrdMgrBase::FindThrd(Cmn_Threadable::ThrdIdT thrd_id)
{
	ReadLockT locker(thrds_mutex_);
	for (auto& v : thrds_)
	{
		if (v->GetThrdId() == thrd_id)
		{
			return v;
		}
	}
	return nullptr;
}

bool Cmn_ThrdMgrBase::KillThrd(Cmn_Threadable::ThrdIdT thrd_id)
{
	auto thrd = FindThrd(thrd_id);
	if (!thrd)
		return false;
	bool res = false;
	{
		ReadLockT locker(thrds_mutex_);
		if (sink_)
			sink_->PreKill(thrd, thrd->io_s_);
		res = thrd->DoExit();
	}
	WriteLockT locker(thrds_mutex_);
	thrds_.erase(thrd);
	return res;
}

void Cmn_ThrdMgrBase::RecycleThrd()
{
	std::set<ThreadablePtr> tmp;
	{
		ReadLockT locker(thrds_mutex_);
		for (auto& v : thrds_)
		{
			if (!v->IsThrdAlive())
			{
				tmp.insert(v);
				v->DoExit();
			}
		}
	}
	{
		WriteLockT locker(thrds_mutex_);
		for (auto& v : tmp)
		{
			thrds_.erase(v);
		}
	}
}

int Cmn_ThrdMgrBase::KillAll()
{
	ThreadablePtr cur_thrd = FindThrd(wxThread::GetCurrentId());
	int cnt = 0;
	{
		ReadLockT locker(thrds_mutex_);
		for (auto& v : thrds_)
		{
			if (v == cur_thrd)
				continue;
			if (sink_)
				sink_->PreKill(v, v->io_s_);
			v->DoExit();
			++cnt;
		}
	}

	WriteLockT locker(thrds_mutex_);
	thrds_.clear();
	if (cur_thrd)
		thrds_.insert(cur_thrd);
	return cnt;
}

ThreadablePtr Cmn_ThrdMgrBase::GetIoThrd()
{
	ReadLockT locker(thrds_mutex_);
	for (auto& v : thrds_)
	{
		if (v->IsIoThrd())
			return v;
	}
	return nullptr;
}

Cmn_ThrdMgrBase::Cmn_ThrdMgrBase(ThrdMgrSink* sink)
{
	sink_ = sink;
}

Cmn_Threadable::ThrdIoService::ThrdIoService(Cmn_Threadable& thrd) : thrd_(thrd)
{

}
