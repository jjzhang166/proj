#include "stdafx.h"
#include <AppMix/ConsoleUiOper.h>
#include <AppMix/ConsoleComponent.h>
#include <boost/filesystem/operations.hpp>
#include <Common/LogOut.h>
#include <boost/algorithm/string.hpp>

bool ConsoleAppUiOperMain::Login(const std::string& key, const std::string& game_path, bool is_add_to_first)
{
	if (!boost::filesystem::exists(game_path))
	{
		wxString msg(game_path);
		msg.Append(wxT("不是有效路径"));
		wxMessageBox(msg, wxMessageBoxCaptionStr, wxOK);
		return false;
	}
	if (IsLogined(key))
	{
		assert(false);
		return false;
	}
	auto io = GetUiOperIo();
	if (!io)
		return false;
	auto& mgr = io->GetLoginMgr();
	if (mgr.AddLoginItem(key, is_add_to_first))
	{
		auto t_io = GetThrdIo();
		if (!t_io)
		{
			assert(false);
			return false;
		}
		t_io->GetIoService().post([key, io, game_path, is_add_to_first](){
			io->Login(key, game_path, is_add_to_first);
		});
		return true;
	}
	return false;
}

bool ConsoleAppUiOperMain::CancelLogin(const std::string& key)
{
	auto io = GetUiOperIo();
	if (!io)
		return false;
	auto& mgr = io->GetLoginMgr();
	if (mgr.CancelLogin(key))
	{
		auto t_io = GetThrdIo();
		if (!t_io)
		{
			assert(false);
			return false;
		}
		t_io->GetIoService().post([key, io](){
			io->CancelLogin(key);
		});
		return true;
	}
	return false;
}

bool ConsoleAppUiOperMain::Run(const std::string& key, const std::string& script_name, const std::string& lua_serialize)
{
	if (script_name.empty())
	{
		assert(false);
		return false;
	}
	auto io = GetThrdIo();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session)
	{
		assert(false);
		return false;
	}
	auto oper = GetUiOperIo();
	if (!oper)
	{
		assert(false);
		return false;
	}
	io->GetIoService().post([=](){
		oper->Run(key, script_name, lua_serialize);
	});
	return true;
}

bool ConsoleAppUiOperMain::Stop(const std::string& key)
{
	auto io = GetThrdIo();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session)
	{
		assert(false);
		return false;
	}
	auto oper = GetUiOperIo();
	if (!oper)
	{
		assert(false);
		return false;
	}
	io->GetIoService().post([=](){
		oper->Stop(key);
	});
	return true;
}

bool ConsoleAppUiOperMain::CloseGame(const std::string& key)
{
	auto io = GetThrdIo();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto io_oper = GetUiOperIo();
	if (!io_oper)
	{
		assert(false);
		return false;
	}
	io->GetIoService().post([key, io_oper](){
		io_oper->CloseGame(key);
	});
	return true;
}

bool ConsoleAppUiOperMain::ShowGameWnd(const std::string& key)
{
	auto io = GetThrdIo();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto io_oper = GetUiOperIo();
	if (!io_oper)
	{
		assert(false);
		return false;
	}
	io->GetIoService().post([key, io_oper](){
		io_oper->ShowGameWnd(key);
	});
	return true;
}

bool ConsoleAppUiOperMain::IsLogined(const std::string& key) const
{
	auto& io_t = WxLuaApp_Console::GetAppBase().GetIoThrd();
	if (!io_t)
	{
		assert(false);
		return false;
	}
	auto& io_thrd = io_t->CastAs<Cmn_ThrdIo_Console>();
	auto& ser_ptr = io_thrd.GetConsoleServer();
	if (!ser_ptr)
	{
		assert(false);
		return false;
	}
	if (ser_ptr->FindSessionByKey(key))
		return true;
	return false;
}

Cmn_ThrdIo_Console* ConsoleAppUiOperMain::GetThrdIo()
{
	auto& io_t = WxLuaApp_Console::GetAppBase().GetIoThrd();
	if (!io_t)
	{
		assert(false);
		return nullptr;
	}
	return io_t->CastTo<Cmn_ThrdIo_Console>();
}

ConsoleAppUiOperIo* ConsoleAppUiOperMain::GetUiOperIo()
{
	auto thrd_io = GetThrdIo();
	if (!thrd_io)
		return nullptr;
	return thrd_io->GetAppUiOper();
}

ConsoleAppUiOperMain::ConsoleAppUiOperMain(WxLuaApp_Console& app) : my_app_(app)
{

}

bool ConsoleAppUiOperMain::CloseLoginSession(const std::string& key) const
{
	auto& io_t = WxLuaApp_Console::GetAppBase().GetIoThrd();
	if (!io_t)
	{
		assert(false);
		return false;
	}
	auto& io_thrd = io_t->CastAs<Cmn_ThrdIo_Console>();
	auto& ser_ptr = io_thrd.GetConsoleLoginServer();
	if (!ser_ptr)
	{
		assert(false);
		return false;
	}
	auto ptr = ser_ptr->FindSessionByKey(key);
	if (ptr)
	{
		io_thrd.GetIoService().post([ptr](){
			if (ptr->ObjValid())
				ptr->Disconnect();
		});
		return true;
	}
	return false;
}

bool ConsoleAppUiOperMain::DoLoginOne(const std::string& key, const stAcSerInfo& ac_info)
{
	auto& app = WxLuaApp_Console::GetApp();
	if (!app.PreLoginGame())
	{
		assert(false);
		return false;
	}
	if (!app.game_dir_starter_asnode_)
	{
		assert(false);
		return false;
	}
	auto process_cmd = app.GetGameClientStarterPath() + " " + app.GetStarterProcessCommand();
	auto pid = app.GetExecEnv().EnvCreateProcess(process_cmd, key);
	auto io_oper = GetUiOperIo();
	if (!io_oper)
	{
		assert(false);
		return false;
	}
	auto io = GetThrdIo();
	if (!io)
	{
		assert(false);
		return false;
	}
	io->GetIoService().post([io_oper, pid, key](){
		auto process_info = key;
		process_info += std::to_string(pid);
		io_oper->OnLoginedOne(process_info, pid, key);
	});
	if (0 == pid)
	{
		assert(false);
		return false;
	}
	return true;
}

bool ConsoleAppUiOperIo::Login(const std::string& key, const std::string& game_path, bool is_add_to_first)
{
	auto& mgr = GetLoginMgr();
	if (mgr.GetWaitAttachCnt() >= 1)
		return false;
	if (mgr.GetLoginingCnt() >= GetMaxLoginingCnt())
		return false;
	if (!mgr.HasFreeItem())
		return false;
	auto login_info = thrd_io_.GetLoginInfoMgr();
	if (!login_info)
		return false;
	if (!login_info->FindFreeOne())
		return false;
	auto item_key = mgr.GetFreeItemKey();
	if (item_key.empty())
	{
		//assert(false);
		return false;
	}
	return DoLoginOne(item_key);
}

bool ConsoleAppUiOperIo::CancelLogin(const std::string& key)
{
	return true;
}

bool ConsoleAppUiOperIo::Run(const std::string& key, const std::string& script_name, const std::string& lua_serialize)
{
	assert(!script_name.empty());
	auto io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session)
	{
		assert(false);
		return false;
	}
	if (!session->ObjValid())
		return false;
	auto console_s = boost::static_pointer_cast<ConsoleSessionS>(session);
	if (!console_s)
	{
		assert(false);
		return false;
	}
	auto oper = console_s->GetSessionOper();
	if (!oper)
	{
		assert(false);
		return false;
	}
	oper->SetLuaSerialize(lua_serialize);
	auto script = console_s->GetScriptEcho();
	if (!script)
	{
		assert(false);
		return false;
	}
	return script->RunLuaScript(script_name, lua_serialize);
}

bool ConsoleAppUiOperIo::Stop(const std::string& key)
{
	auto io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session)
	{
		assert(false);
		return false;
	}
	if (!session->ObjValid())
		return false;
	auto console_s = boost::static_pointer_cast<ConsoleSessionS>(session);
	if (!console_s)
	{
		assert(false);
		return false;
	}
	auto script = console_s->GetScriptEcho();
	if (!script)
	{
		assert(false);
		return false;
	}
	return script->TerminateScript();
}

bool ConsoleAppUiOperIo::CloseGame(const std::string& key)
{
	auto io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session || !session->ObjValid())
		return true;
	session->Disconnect();
	return true;
}

bool ConsoleAppUiOperIo::ShowGameWnd(const std::string& key)
{
	auto io = GetIoThrd();
	if (!io)
	{
		assert(false);
		return false;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return false;
	}
	auto session = server->FindSessionByKey(key);
	if (!session || !session->ObjValid())
		return false;
	auto sess = boost::static_pointer_cast<ConsoleSessionS>(session);
	if (!sess)
	{
		assert(false);
		return false;
	}
	return sess->ForegroundTheWindow();
}

ConsoleLoginMgr& ConsoleAppUiOperIo::GetLoginMgr()
{
	if (login_mgr_)
		return *login_mgr_;
	login_mgr_ = WxLuaApp_Console::GetApp().GetAppFactory().CreateConsoleLoginMgr(*this);
	assert(login_mgr_);
	return *login_mgr_;
}

ConsoleAppUiOperIo::ConsoleAppUiOperIo(Cmn_ThrdIo_Console& thrd_io) : thrd_io_(thrd_io)
{
	login_mgr_ = nullptr;
}

ConsoleAppUiOperMain* ConsoleAppUiOperIo::GetAppOperMain()
{
	auto app = WxLuaApp_Console::GetAppBase().CastTo<WxLuaApp_Console>();
	if (!app)
	{
		assert(false);
		return nullptr;
	}
	auto res = app->GetAppUiOper();
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return static_cast<ConsoleAppUiOperMain*>(res);
}

WxLuaApp_Console* ConsoleAppUiOperIo::GetApp()
{
	auto app = WxLuaApp_Console::GetAppBase().CastTo<WxLuaApp_Console>();
	if (!app)
	{
		assert(false);
		return nullptr;
	}
	return app;
}

Cmn_ThrdIo_Console* ConsoleAppUiOperIo::GetIoThrd()
{
	auto app = GetApp();
	if (!app)
		return nullptr;
	return app->GetIoThrd_();
}

int ConsoleAppUiOperIo::GetMaxLoginingCnt() const
{
	return 1;
}

ConsoleAppUiOperIo::~ConsoleAppUiOperIo()
{
	delete login_mgr_;
}

bool ConsoleAppUiOperIo::DoLoginOne(const std::string& key)
{
	assert(!key.empty());
	auto oper_main = GetAppOperMain();
	if (!oper_main)
	{
		assert(false);
		return false;
	}
	auto& mgr = GetLoginMgr();
	auto ac_info = mgr.FindAcItemInfo(key);
	if (!ac_info)
	{
		assert(false);
		return false;
	}
	auto ac_info_tmp = *ac_info;
	WxLuaApp_Console::GetAppBase().GetIoService().post([oper_main, key, ac_info_tmp](){
		if (!oper_main->DoLoginOne(key, ac_info_tmp))
		{
			assert(false);
		}
	});
	return true;
}

bool ConsoleAppUiOperIo::OnLoginedOne(const std::string& process_info, long pid, const std::string& item_key)
{
	auto& mgr = GetLoginMgr();
	if (pid == 0)
	{
		mgr.RecycleFreeItemKey(item_key);
		return false;
	}
	if (!mgr.AttachProcessInfo(process_info, pid, item_key))
	{
		assert(false);
		return false;
	}
	return true;
}

ConsoleLoginMgr::ItemInfo::ItemInfo(const std::string& item_key) : item_key_(item_key)
{
	target_process_id_ = 0;
	state_ = kNil;
}

void ConsoleLoginMgr::ItemInfo::FreeLoginTimer()
{
	if (timer_login_)
	{
		timer_login_->Cancel();
		timer_login_.reset();
	}
}

bool ConsoleLoginMgr::AddLoginItem(const std::string& item_key, bool add_to_first)
{
	if (item_key.empty())
	{
		assert(false);
		return false;
	}
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.item_key_ == item_key)
			return false;
	}
	ItemInfo item(item_key);
	if (add_to_first)
		login_infos_.push_front(item);
	else
		login_infos_.push_back(item);

	//LUA会主动告知的
	//Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_GetAcItemInfo", item_key));
	return true;
}

bool ConsoleLoginMgr::CancelLogin(const std::string& item_key)
{
	LockGuard l(login_mutex_);
	auto it = login_infos_.begin(), it_end = login_infos_.end();
	for (; it != it_end; ++it )
	{
		if (it->item_key_ == item_key)
			break;
	}
	if (it == it_end)
		return false;
	if (it->state_ != ItemInfo::kLogining)
	{
		assert(!it->timer_login_);
		assert(it->session_login_.expired());
		login_infos_.erase(it);
		return true;
	}
	it->FreeLoginTimer();
	if (!it->session_login_.expired())
	{
		auto ptr = it->session_login_.lock();
		if (ptr)
		{
			ptr->Disconnect();
		}
	}
	login_infos_.erase(it);
	return true;
}

ConsoleLoginMgr::ConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper) : ui_oper_(ui_oper)
{

}

bool ConsoleLoginMgr::HasFreeItem() const
{
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.state_ == ItemInfo::kNil)
			return true;
	}
	return true;
}

bool ConsoleLoginMgr::AttachProcessInfo(const std::string& process_info, long pid, const std::string& item_key)
{
	if (process_info.empty())
	{
		assert(false);
		return false;
	}
	LockGuard l(login_mutex_);
	if (HasTheProcessInfo(process_info))
	{
		assert(false);
		return false;
	}
	ItemInfo* item = nullptr;
	for (auto& v : login_infos_)
	{
		if (v.item_key_ == item_key)
		{
			if (v.state_ == ItemInfo::kWaitAttachPid)
			{
				item = &v;
				break;
			}
			else
			{
				assert(false);
			}
		}
	}
	if (!item)
		return false;
	assert(item->target_process_info_.empty());
	assert(!item->timer_login_);
	assert(item->session_login_.expired());
	item->target_process_id_ = pid;
	item->target_process_info_ = process_info;
	item->state_ = ItemInfo::kLogining;
	item->timer_login_ = CreateIoTimer(WxLuaApp_Console::GetApp().time_login2game_succeed_, true, 
		[item_key, this](const IoTimerPtr&){
		LOG_O(Log_debug) << "登陆超时";
		this->Timeout(item_key);
	});
	return true;
}

bool ConsoleLoginMgr::HasTheProcessInfo(const std::string& process_info) const
{
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.target_process_info_ == process_info)
			return true;
	}
	return false;
}

void ConsoleLoginMgr::Timeout(const std::string& item_key)
{
	CancelLogin(item_key);
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_CancelLogin", item_key));
	auto io = ui_oper_.GetIoThrd();
	if (!io)
	{
		assert(false);
		return;
	}
	auto& ser = io->GetConsoleServer();
	if (!ser)
	{
		assert(false);
		return;
	}
	if (ser->FindSessionByKey(item_key))
		return;
	WxLuaApp_Console::LoginByItemKey(item_key, false);
}

std::string ConsoleLoginMgr::GetFreeItemKey()
{
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.state_ == ItemInfo::kNil)
		{
			v.state_ = ItemInfo::kWaitAttachPid;
			return v.item_key_;
		}
	}
	return std::string();
}

bool ConsoleLoginMgr::AttachSessionLogin(long pid, const std::string& item_key, CmnConsoleSession& login_session)
{
	assert(!item_key.empty());
	LockGuard l(login_mutex_);
	ItemInfo* item = nullptr;
	for (auto& v : login_infos_)
	{
		if (v.item_key_ == item_key)
		{
			if (v.state_ == ItemInfo::kLogining)
			{
				//assert(v.target_process_id_ == pid);		//可以注释掉
				item = &v;
				break;
			}
			else
			{
				assert(false);
			}
		}
	}
	if (!item)
		return false;
	if (!item->session_login_.expired())
		return false;
	login_session.SetLoginProcessInfo(item->target_process_info_);
	auto shared = login_session.shared_from_this();
	assert(shared);
	item->session_login_ = boost::static_pointer_cast<Session>(shared);
	return true;
}

void ConsoleLoginMgr::AddAcItemInfo(const std::string& item_key, const stAcSerInfo& ac_info)
{
	ac_info_[item_key] = ac_info;
}

const stAcSerInfo* ConsoleLoginMgr::FindAcItemInfo(const std::string& item_key) const
{
	auto it = ac_info_.find(item_key);
	if (it == ac_info_.end()) return nullptr;
	return &it->second;
}

bool ConsoleLoginMgr::AttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& session)
{
	assert(!item_key.empty());
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.item_key_ == item_key)
		{
			session.SetLoginProcessInfo(v.target_process_info_);
		}
	}
	return HandleAttachSessionGame(pid, item_key, session);
}

int ConsoleLoginMgr::GetLoginingCnt() const
{
	return GetTheStateCnt(ItemInfo::kLogining);
}

int ConsoleLoginMgr::GetTheStateCnt(ItemInfo::enState state) const
{
	LockGuard l(login_mutex_);
	int cnt = 0;
	for (auto& v : login_infos_)
	{
		if (v.state_ == state)
			++cnt;
	}
	return cnt;
}

int ConsoleLoginMgr::GetWaitAttachCnt() const
{
	return GetTheStateCnt(ItemInfo::kWaitAttachPid);
}

void ConsoleLoginMgr::RecycleFreeItemKey(const std::string& item_key)
{
	LockGuard l(login_mutex_);
	for (auto& v : login_infos_)
	{
		if (v.state_ == ItemInfo::kWaitAttachPid)
		{
			v.state_ = ItemInfo::kNil;
			return;
		}
	}
	assert(false);
}

ConsoleLoginMgr::~ConsoleLoginMgr()
{

}

bool ConsoleLoginMgr::HandleAttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& session)
{
	assert(!session.GetAcInfo().login_process_info_.empty());
	if (!CancelLogin(item_key))
		return false;
	return true;
}

bool ConsoleAcCfgMgr::stAcItemInfo::ParseTails(const StringItemT& tails)
{
	return true;
}

ConsoleAcCfgMgr::stAcItemInfo::~stAcItemInfo()
{

}

bool ConsoleAcCfgMgr::stAcItemInfo::ReadLine(const std::string& str_line)
{
	StringItemT vec_seg_tag;
	static const std::string kTags("  ，,\t");
	boost::split(vec_seg_tag, str_line, boost::is_any_of(kTags), boost::token_compress_on);
	auto size = vec_seg_tag.size();
	if (size < 4)
		return false;

	ac_ = vec_seg_tag[0].c_str();
	pwd_ = vec_seg_tag[1].c_str();
	area_ = vec_seg_tag[2].c_str();
	ser_ = vec_seg_tag[3].c_str();
	if (size == 4)
		return true;
	StringItemT tails;
	for (decltype(size) i = 4; i < size; ++i)
	{
		tails.push_back(vec_seg_tag[i]);
	}
	return ParseTails(tails);
}

bool ConsoleAcCfgMgr::stAcItemInfo::WriteLine(std::ostream& ostm) const
{
	ostm << ac_ << ',' << pwd_ << ',' << area_ << ',' << ser_;
	return WriteTails(ostm);
}

bool ConsoleAcCfgMgr::stAcItemInfo::WriteTails(std::ostream& ostm) const
{
	return true;
}

ConsoleAcCfgMgr::stAcItemInfo::stAcItemInfo()
{

}

ConsoleAcCfgMgr::stAcItemInfo::stAcItemInfo(const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser) :
	ac_(ac), pwd_(pwd), area_(area), ser_(ser)
{

}

bool ConsoleAcCfgMgr::Load(const std::string& full_file_name)
{
	if (!boost::filesystem::exists(full_file_name))
	{
		assert(false);
		return false;
	}
	ac_items_.clear();

	std::ifstream fstm(full_file_name);
	std::string str_line;
	auto& factory = WxLuaApp_Console::GetApp().GetAppFactory();
	while (fstm.good())
	{
		std::getline(fstm, str_line);
		if (str_line.empty())
			continue;
		auto ptr = factory.CreateAcCfgItemInfo();
		AcItemInfoPtrT cfg_tmp(ptr);
		if (!cfg_tmp)
		{
			assert(false);
			return false;
		}
		if (!ptr->ReadLine(str_line))
			return false;
		ac_items_.push_back(cfg_tmp);
	}
	return true;
}

bool ConsoleAcCfgMgr::Save(const std::string& full_file_name) const
{
	std::ofstream oftm(full_file_name, std::ios_base::out | std::ios_base::trunc);
	for (const auto& ac_cfg : ac_items_)
	{
		if (!ac_cfg->WriteLine(oftm))
		{
			assert(false);
			return false;
		}
		oftm << '\n';
	}
	return true;
}

const ConsoleAcCfgMgr::AcItemInfosT& ConsoleAcCfgMgr::GetAcItems() const
{
	return ac_items_;
}

void ConsoleAcCfgMgr::Add(const AcItemInfoPtrT& ac_item)
{
	if (!ac_item)
	{
		assert(false);
		return;
	}
	ac_items_.push_back(ac_item);
}

ConsoleAcCfgMgr::AcItemInfoIteratorPtr ConsoleAcCfgMgr::Iterator()
{
	return MakeIterator<AcItemInfoTraits>(ac_items_);
}
