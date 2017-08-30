#include "stdafx.h"
#include <mfc/MfcAcOper.h>

bool MfcAcOperMgr::Add(UINT id_template, const stAcSerInfo& ac_info)
{
	auto ac_item_key = WxLuaApp_Console::GetApp().GenAcItemKey(ac_info.ac_, ac_info.area_, ac_info.ser_);
	auto it = ac_opers_.find(ac_item_key);
	if (it != ac_opers_.end())
	{
		assert(false);
		return false;
	}
	MfcAcOperBasePtr ac_oper(MakeAcOper());
	if (!ac_oper)
	{
		assert(false);
		return false;
	}
	struct stHelp{
		stHelp(){
			AfxWinInit(wxGetInstance(), nullptr, nullptr, wxApp::m_nCmdShow);
		}
	};
	static stHelp hlp;
	if (!ac_oper->Init(id_template, ac_item_key, ac_info))
	{
		assert(false);
		return false;
	}
	ac_opers_[ac_item_key] = ac_oper;
	return true;
}

MfcAcOperBase* MfcAcOperMgr::MakeAcOper()
{
	return new MfcAcOperBase;
}

MfcAcOperBase* MfcAcOperMgr::Find(const std::string& ac_item_key) const
{
	auto it = ac_opers_.find(ac_item_key);
	if (it == ac_opers_.end())
		return nullptr;
	return it->second.get();
}

bool MfcAcOperBase::Init(UINT id_template, const std::string& ac_item_key, const stAcSerInfo& ac_info)
{
	if (!Create(id_template))
	{
		assert(false);
		return false;
	}
	ac_info_ = ac_info;
	ac_item_key_ = ac_item_key;
	return true;
}

MfcAcOperBase::~MfcAcOperBase()
{

}

bool MfcAcOperBase::RunScript(const std::string& script_name)
{
	Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_RunTheScript", ac_item_key_, script_name));
	return true;
}

void MfcAcOperBase::UpdateUi()
{
	if (IsWindowVisible())
	{
		Cmn_App::ShellM(LuaSerialize::ToLuaStringCall("mainfrm:OnS_UpdateItem", ac_item_key_));
	}
}

void MfcAcOperBase::ModifyAcItemData(const std::string& item_key, const std::string& key, const std::string& value)
{
	WxLuaApp_Console::GetApp().ModifyAcItemData(item_key, key, value);
}

void MfcAcOperBase::OnUpdateUi(const luabind::object& item_data)
{

}

bool MfcAcOperBase::Show(bool is_show)
{
	if (is_show)
	{
		if (IsWindowVisible())
			return true;
		if (ShowWindow(SW_SHOW))
		{
			assert(false);
			return false;
		}
		UpdateUi();
		return true;
	}
	else
	{
		if (!IsWindowVisible())
			return true;
		if (!ShowWindow(SW_HIDE))
		{
			assert(false);
			return false;
		}
		return true;
	}
}

ConsoleSessionS* MfcAcOperBase::GetSession()
{
	auto io = WxLuaApp_Console::GetApp().GetIoThrd_();
	if (!io)
	{
		assert(false);
		return nullptr;
	}
	auto server = io->GetConsoleServer();
	if (!server)
	{
		assert(false);
		return nullptr;
	}
	auto session = server->FindSessionByKey(ac_item_key_);
	if (!session)
	{
		assert(false);
		return nullptr;
	}
	return session->CastTo<ConsoleSessionS>();
}

ConsoleSessionOper* MfcAcOperBase::GetSessionOper()
{
	auto s = GetSession();
	if (s)
		return s->GetSessionOper();
	return nullptr;
}

const std::string& MfcAcOperBase::GetAcItemKey() const
{
	return ac_item_key_;
}

MfcAcOperMgr::~MfcAcOperMgr()
{

}

void MfcAcOperMgr::OnUpdateAcItemData(int ac_item_idx, const std::string& item_key, const luabind::object& item_data)
{
	auto dlg = Find(item_key);
	if (dlg && dlg->IsWindowVisible())
	{
		dlg->OnUpdateUi(item_data);
	}
}

void MfcAcOperMgr::OnAddAc(const stAcSerInfo& ac_info)
{
	if (!Add(GetAcOperDlgTemplateId(), ac_info))
		assert(false);
}

void MfcAcOperMgr::OnDeleteAc(const std::string& item_key)
{
	auto it = ac_opers_.find(item_key);
	if (it == ac_opers_.end())
		return;
	it->second->DestroyWindow();
	ac_opers_.erase(it);
}

void MfcAcOperMgr::OnAcConnected(ConsoleSessionS* session, const std::string& item_key)
{
	auto dlg = Find(item_key);
	if (dlg)
	{
		dlg->Show(true);
	}
}

void MfcAcOperMgr::OnAcDisconnect(const std::string& item_key)
{
	auto dlg = Find(item_key);
	if (dlg)
	{
		dlg->Show(false);
	}
}

MfcAcOperBase* MfcAcOperMgr::GetFrist() const
{
	if (ac_opers_.empty())
	{
		assert(false);
		return nullptr;
	}
	return ac_opers_.begin()->second.get();
}

MfcAcOperMgr::MfcAcOperMgr()
{
}
