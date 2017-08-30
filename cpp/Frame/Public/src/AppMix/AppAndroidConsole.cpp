#include "stdafx.h"
#include <AppMix/AppAndroidConsole.h>

//////////////////////////////////////////////////////////////////////////
BC_REG_CLS_DEFINE(AndroidLoginSessionOper){
	bufcall::class_<AndroidLoginSessionOper, ConsoleLoginSessionOper>("AndroidLoginSessionOper")
		;
}

BC_REG_CLS_DEFINE(AndroidGameSessionOper){
	bufcall::class_<AndroidGameSessionOper, ConsoleSessionOper>("AndroidGameSessionOper")
		;
}
//////////////////////////////////////////////////////////////////////////

void AndroidThrdIo_Console::OnSetGamePath(const std::string& game_path)
{

}

AndroidApp_Console& AndroidThrdIo_Console::GetApp()
{
	return AndroidApp_Console::GetAppBase().CastAs<AndroidApp_Console>();
}

int AndroidThrdIo_Console::OnExit_()
{
	return __super::OnExit_();
}

void AndroidThrdIo_Console::AddCheckCloseMemu(const std::string& menu_process_info)
{
	if (menu_process_info.empty())
	{
		assert(false);
		return;
	}
	if (check_close_menu_set_.find(menu_process_info) != check_close_menu_set_.end())
		return;
	check_close_menu_set_.insert(menu_process_info);
	if (!timer_check_close_memu_)
	{
		auto& app = AndroidApp_Console::GetApp();
		timer_check_close_memu_ = CreateIoTimer(1000 * 4, true, [this, &app](const IoTimerPtr& timer){
			if (check_close_menu_set_.empty())
				return;
			auto set_tmp = check_close_menu_set_;
			app.GetIoService().post([&app, set_tmp](){
				app.DoCheckClosedMenu(set_tmp);
			});

			assert(timer);
			timer->Continue();
		});
		if (!timer_check_close_memu_)
		{
			assert(false);
			return;
		}
	}
	if (!timer_check_close_memu_->IsClocking())
	{
		timer_check_close_memu_->Continue();
	}
}

void AndroidThrdIo_Console::EraseCheckCloseMemus(const VmNamesT& erased_menu)
{
	if (erased_menu.empty())
	{
		assert(false);
		return;
	}
	auto old_size = check_close_menu_set_.size();
	for (auto& v : erased_menu)
	{
		check_close_menu_set_.erase(v);
	}
	if (old_size == check_close_menu_set_.size())
		return;
	auto io_oper = GetAppUiOper();
	if (!io_oper)
	{
		assert(false);
		return;
	}
	io_oper->Login(kEmptyStr, kEmptyStr, false);
}

void AndroidThrdIo_Console::EraseCheckCloseMemu(const std::string& erased_menu)
{
	if (erased_menu.empty())
	{
		assert(false);
		return;
	}
	auto old_size = check_close_menu_set_.size();
	check_close_menu_set_.erase(erased_menu);
	if (old_size == check_close_menu_set_.size())
		return;
	auto io_oper = GetAppUiOper();
	if (!io_oper)
	{
		assert(false);
		return;
	}
	io_oper->Login(kEmptyStr, kEmptyStr, false);
}

const AndroidThrdIo_Console::VmNamesT& AndroidThrdIo_Console::GetEraseCheckCloseMemus() const
{
	return check_close_menu_set_;
}

AndroidConsoleAppFactory& AndroidApp_Console::GetAppFactory() const
{
	assert(app_factory_);
	return *static_cast<AndroidConsoleAppFactory*>(app_factory_);
}

AndroidConsoleAppFactory* AndroidApp_Console::CreateAppFactory()
{
	return new AndroidConsoleAppFactory;
}

AndroidAppUiOperMain* AndroidConsoleAppFactory::CreateAppUiOper(WxLuaApp_Console& app)
{
	auto& the_app = static_cast<AndroidApp_Console&>(app);
	return new AndroidAppUiOperMain(the_app, the_app.GetExecEnv());
}

AndroidAppUiOperIo* AndroidConsoleAppFactory::CreateIoUiOper(Cmn_ThrdIo_Console& io_thrd)
{
	return new AndroidAppUiOperIo(static_cast<AndroidThrdIo_Console&>(io_thrd));
}

AndroidThrdIo_Console* AndroidConsoleAppFactory::CreateIoThrd()
{
	return new AndroidThrdIo_Console;
}

AndroidLoginSessionS* AndroidConsoleAppFactory::CreateLoginSession(ConsoleLoginServer& server)
{
	return new AndroidLoginSessionS(server);
}

AndroidConsoleSessionS* AndroidConsoleAppFactory::CreateGameSession(ConsoleServer& server)
{
	return new AndroidConsoleSessionS(server);
}

AndroidGameSessionOper* AndroidConsoleAppFactory::CreateGameSessionOper(ConsoleSessionS& session)
{
	return new AndroidGameSessionOper(session);
}

AndroidLoginSessionOper* AndroidConsoleAppFactory::CreateLoginSessionOper(ConsoleLoginSessionS& session)
{
	return new AndroidLoginSessionOper(session);
}

MEmuExecEnv* AndroidConsoleAppFactory::CreateExecEnv()
{
	return new MEmuExecEnv;
}

AndroidApp_Console::AndroidApp_Console()
{
	time_login2game_succeed_ = 1000 * 60 * 7;
	time_game_login_succeed_ = 1000 * 15;
	memu_exec_env_ = nullptr;
}

void AndroidApp_Console::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<AndroidApp_Console, WxLuaApp_Console>("WxLuaApp_Console")
			.def("DetectGamePath", &AndroidApp_Console::DetectGamePath)
		];
}

void AndroidApp_Console::OnSetGamePath(const std::string& game_path, bool is_reset)
{
	if (game_path_ != game_path)
	{
		__super::OnSetGamePath(game_path, is_reset);
		auto& thrd = GetIoThrd();
		if (!thrd)
		{
			assert(false);
			return;
		}
		auto thrd_io = thrd->CastTo<AndroidThrdIo_Console>();
		if (!thrd_io)
		{
			assert(false);
			return;
		}
		thrd_io->GetIoService().post([thrd_io, game_path](){
			thrd_io->OnSetGamePath(game_path);
		});
		auto& env = GetExecEnv();
		if (!env.DmRegsvr32())
		{
			assert(false);
			return;
		}
		if (!env.SetMEmuPath(game_path))
		{
			wxString msg(game_path);
			msg.append(wxT("路径错误"));
			wxMessageBox(msg, wxMessageBoxCaptionStr, wxOK);
		}
		else
		{
			env.AdbStartServer();
		}
		return;
	}
	__super::OnSetGamePath(game_path, is_reset);
}

MEmuExecEnv& AndroidApp_Console::GetExecEnv() const
{
	return __super::GetExecEnv().CastAs<MEmuExecEnv>();
}

AndroidApp_Console& AndroidApp_Console::GetApp()
{
	return AndroidApp_Console::GetAppBase().CastAs<AndroidApp_Console>();
}

void AndroidApp_Console::DoCheckClosedMenu(const AndroidThrdIo_Console::VmNamesT& menus)
{
	if (menus.empty())
	{
		assert(false);
		return;
	}
	MEmuExecEnv::VmsOper vms;
	auto& exec_env = GetExecEnv();
	if (!exec_env.Manager_ListRunningVms(vms))
	{
		assert(false);
		return;
	}
	AndroidThrdIo_Console::VmNamesT erased;
	for (auto& v : menus)
	{
		if (!vms.ExistByName(v))
			erased.insert(v);
	}
	if (erased.size() == menus.size())
	{
		auto io = GetIoThrd_();
		if (io)
		{
			io->GetIoService().post([io, erased](){
				io->EraseCheckCloseMemus(erased);
			});
		}
		return;
	}
	vms.vms_.clear();
	exec_env.Manager_ListVms(vms);
	for (auto& v : menus)
	{
		if (!vms.ExistByName(v))
		{
			assert(false);
			erased.insert(v);
		}
	}
	if (erased.empty())
		return;
	auto io = GetIoThrd_();
	if (!io)
		return;
	io->GetIoService().post([io, erased](){
		io->EraseCheckCloseMemus(erased);
	});
}

AndroidThrdIo_Console* AndroidApp_Console::GetIoThrd_()
{
	auto io = __super::GetIoThrd_();
	if (!io)
		return nullptr;
	return static_cast<AndroidThrdIo_Console*>(io);
}

AndroidApp_Console::~AndroidApp_Console()
{
	delete memu_exec_env_;
	memu_exec_env_ = nullptr;
}

bool AndroidApp_Console::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	return true;
}

std::string AndroidApp_Console::DetectGamePath() const
{
	using namespace boost::filesystem;
	wxString str;
	if (!wxGetEnv(wxT("MEmu_Path"), &str))
	{
		//assert(false);
		return kEmptyStr;
	}
	path p(str);
	p /= "MEmu\\MEmuConsole.exe";
	if (!exists(p))
	{
		assert(false);
		return kEmptyStr;
	}
	return p.string();
}

AndroidAppUiOperMain::AndroidAppUiOperMain(AndroidApp_Console& app, MEmuExecEnv& exec_env)
	: ConsoleAppUiOperMain(app), exec_env_(exec_env)
{

}

void AndroidAppUiOperMain::GetIo_NewLoginProcessInfo(const std::string& item_key, const VmNamesT& black_name_list)
{
	assert(!item_key.empty());
	VmsOper vms;
	if (!exec_env_.Manager_ListVms(vms))
	{
		assert(false);
		return;
	}

	if (vms != vms_)
	{
		vms_.swap(vms);
	}
	vms.vms_.clear();
	if (!exec_env_.Manager_ListRunningVms(vms))
	{
		assert(false);
		return;
	}
	if (vms != running_vms_)
	{
		running_vms_.swap(vms);
	}
	//////////////////////////////////////////////////////////////////////////
	struct stHelper{
		const MEmuExecEnv::stVmItem*	item_;
		bool	is_running_;
	};
	std::vector<stHelper> hlp;
	for (auto& v : vms_.vms_)
	{
		hlp.push_back({ &v, false });
	}
	for (auto& v : hlp)
	{
		if (running_vms_.ExistByItem(*v.item_))
		{
			v.is_running_ = true;
		}
	}
	auto oper_io = GetUiOperIo();
	if (!oper_io)
	{
		assert(false);
		return;
	}
	auto& login_mgr = oper_io->GetLoginMgr();
	const MEmuExecEnv::stVmItem* item = nullptr;
	for (auto& v : hlp)
	{
		if (v.is_running_)
			continue;
		if (login_mgr.HasTheProcessInfo(v.item_->vm_name_))
			continue;
		if (black_name_list.find(v.item_->vm_name_) != black_name_list.end())
			continue;
		item = v.item_;
		break;
	}
	long login_pid = 0;
	std::string vm_name;
	if (item && exec_env_.ModifyVm(item->vm_name_))
	{
		vm_name = item->vm_name_;
		LOG_O(Log_trace) << "启动Memu：" << item->vm_name_ << " item_key:" << item_key;
		login_pid = exec_env_.Memu_Create(item->vm_name_, item_key);
		//LOG_O(Log_trace) << "启动Menu结束，pid:" << login_pid;
	}
	oper_io = GetUiOperIo();
	if (!oper_io)
	{
		assert(false);
		return;
	}
	auto io_thrd = GetThrdIo();
	if (!io_thrd)
	{
		assert(false);
		return;
	}
	io_thrd->GetIoService().post([io_thrd, vm_name, login_pid, oper_io, item_key](){
		if (login_pid != 0)
		{
			io_thrd->EraseCheckCloseMemu(vm_name);
		}
		oper_io->OnLoginedOne(vm_name, login_pid, item_key);
	});
}

AndroidAppUiOperIo* AndroidAppUiOperMain::GetUiOperIo()
{
	auto res = __super::GetUiOperIo();
	if (!res)
		return nullptr;
	return static_cast<AndroidAppUiOperIo*>(res);
}

AndroidThrdIo_Console* AndroidAppUiOperMain::GetThrdIo()
{
	auto io = __super::GetThrdIo();
	if (!io)
		return nullptr;
	return static_cast<AndroidThrdIo_Console*>(io);
}

AndroidAppUiOperMain* AndroidAppUiOperIo::GetAppOperMain()
{
	auto res = __super::GetAppOperMain();
	if (!res)
		return nullptr;
	return static_cast<AndroidAppUiOperMain*>(res);
}

bool AndroidAppUiOperIo::OnLoginedOne(const std::string& process_info, long pid, const std::string& item_key)
{
	if (!__super::OnLoginedOne(process_info, pid, item_key))
		return false;
	auto app = GetApp();
	if (!app)
	{
		assert(false);
		return false;
	}
	auto& game_dll = app->GetGameDll();
	if (!game_dll)
	{
		assert(false);
		return false;
	}
	const auto& the_path = game_dll->GetAbsolutePath_();
	const auto& game_dll_path = the_path.native();
	if (!InjectLib(pid, game_dll_path.c_str()))
	{
		assert(false);
		return false;
	}
	return true;
}

AndroidApp_Console* AndroidAppUiOperIo::GetApp()
{
	auto res = __super::GetApp();
	if (!res)
		return nullptr;
	return static_cast<AndroidApp_Console*>(res);
}

AndroidAppUiOperIo::AndroidAppUiOperIo(AndroidThrdIo_Console& thrd_io) : ConsoleAppUiOperIo(thrd_io)
{

}

int AndroidAppUiOperIo::GetMaxLoginingCnt() const
{
	return 6;
}

AndroidThrdIo_Console* AndroidAppUiOperIo::GetIoThrd()
{
	auto io = __super::GetIoThrd();
	if (!io)
		return nullptr;
	return static_cast<AndroidThrdIo_Console*>(io);
}

bool AndroidAppUiOperIo::DoLoginOne(const std::string& key)
{
	auto io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto oper_main = GetAppOperMain();
	if (!oper_main)
	{
		assert(false);
		return false;
	}
	const auto& black_name_list = io->GetEraseCheckCloseMemus();
	AndroidApp_Console::GetAppBase().GetIoService().post([oper_main, key, black_name_list](){
		oper_main->GetIo_NewLoginProcessInfo(key, black_name_list);
	});
	return true;
}

AndroidLoginSessionOper::AndroidLoginSessionOper(ConsoleLoginSessionS& session) : ConsoleLoginSessionOper(session)
{

}

void AndroidLoginSessionOper::OnS_AttachLoginSession(const std::string& item_key)
{
	auto& memu = GetSession().GetAcInfo().login_process_info_;
	assert(!memu.empty());
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_AttachLoginSession", item_key, memu));
}

AndroidGameSessionOper::AndroidGameSessionOper(ConsoleSessionS& session) : ConsoleSessionOper(session)
{

}

void AndroidGameSessionOper::OnS_AttachGameSession(const std::string& item_key)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_AttachGameSession", item_key, GetSession().GetAcInfo().login_process_info_));
}

AndroidLoginSessionS::AndroidLoginSessionS(Server& server) : ConsoleLoginSessionS(server)
{

}

bool AndroidLoginSessionS::OnConnected(IoConnectBase* connect)
{
	if (!__super::OnConnected(connect))
		return false;
	auto oper = GetIoUiOper();
	if (!oper)
	{
		assert(false);
		return false;
	}
	GetIoService().post([oper](){
		oper->Login(kEmptyStr, kEmptyStr, false);
	});
	auto& app = WxLuaApp_Console::GetApp();
	if (!session_oper_->ToCall(BC_MakeP("SetMemuEnvPath_") << app.game_path_))
		return false;
	return true;
}

void AndroidLoginSessionS::OnS_LoginDisconnect()
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_LoginDisconnect", key_, state_at_disconnect_, ""));
}

AndroidConsoleSessionS::AndroidConsoleSessionS(Server& server) : ConsoleSessionS(server)
{

}

void AndroidConsoleSessionS::OnDisconnect(IoConnectBase* connect)
{
	if (!ac_info_.login_process_info_.empty())
	{
		auto& io = GetIoThrd();
		auto menu = ac_info_.login_process_info_;
		io.GetIoService().post([&io, menu](){
			io.AddCheckCloseMemu(menu);
		});
	}
	return __super::OnDisconnect(connect);
}

AndroidThrdIo_Console& AndroidConsoleSessionS::GetIoThrd()
{
	return static_cast<AndroidThrdIo_Console&>(__super::GetIoThrd());
}

void AndroidConsoleSessionS::OnS_GameDisconnect()
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_GameDisconnect", key_, state_at_disconnect_, ""));
}
