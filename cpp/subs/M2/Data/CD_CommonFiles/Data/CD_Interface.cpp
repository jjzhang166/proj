#include "stdafx.h"
#include "CD_Interface.h"
#include <CmnMix/Cmn_LogOut.h>

using namespace CD_Interface;

void CD_Interface::RegTestForLua(lua_State* l)
{
	/*using namespace luabind;
	module(l, "CD_Interface")[
	];*/
}

void CD_Interface::RegDbgInfoForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "CD_Interface")[
		class_<enCD_TraUiShowFlag>("enCD_TraUiShowFlag")
		.enum_("")
		[
			value("kTUSF_Show", kTUSF_Show),
			value("kTUSF_UnShow", kTUSF_UnShow)
		],
		def("DbgMap", &DbgMap),
		def("DbgRoleMe", &DbgRoleMe),
		def("DbgGameObjs", &DbgGameObjs),
		def("DbgGlobalUtility", &DbgGlobalUtility),
		def("DbgBagItems", &DbgBagItems),
		def("DbgSkills", &DbgSkills),
		def("DbgUi", &DbgUi),
		def("DbgQuest", &DbgQuest),
		def("DbgFindUi", &DbgFindUi),
		def("DbgSkillSlot", &DbgSkillSlot),
		def("DbgPortal", &DbgPortal),
		def("DbgAction", &DbgAction),
		def("DbgBodyEquips", &DbgBodyEquips),
		def("DbgMails", &DbgMails),
		def("DbgLightService", &DbgLightService),
		def("DbgCharacterList", &DbgCharacterList),
		def("DbgParty", &DbgParty),
		def("DbgMapInformationService", &DbgMapInformationService)
	];
}

stCD_RoleMe* CD_Interface::GetRoleMe()
{
	auto m = GetMap();
	if (!m)
	{
		assert(false);
		return nullptr;
	}
	return m->GetRoleMe();
}

void CD_Interface::DbgRoleMe()
{
	auto p = GetRoleMe();
	if (p)
		p->DebugInfo();
}

stCD_Map* CD_Interface::GetMap()
{
	return stCD_Map::GetInstance();
}

void CD_Interface::DbgMap()
{
	auto m = GetMap();
	if (m)
		m->DebugInfo();
}

void CD_Interface::DbgGameObjs()
{
	auto m = GetMap();
	if (!m)
		return;
	m->DebugGameObjs();
}

void CD_Interface::DbgGlobalUtility()
{
	stCD_GlobalUtility::DebugInfo();
}

stCD_BagMgr* CD_Interface::GetBagMgr()
{
	return stCD_BagMgr::GetInstance();
}

void CD_Interface::RegSomeForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<enCD_InventoryType>("enCD_InventoryType")
		.enum_("")
		[
			value("kIT_Equip", kIT_Equip),
			value("kIT_Fashion", kIT_Fashion),
			value("kIT_Call", kIT_Call),
			value("kIT_Material", kIT_Material),
			value("kIT_Life", kIT_Life),
			value("kIT_Task", kIT_Task),
			value("kIT_Gem", kIT_Gem),
			value("kIT_Other", kIT_Other)
		]
	];
}

void CD_Interface::DbgBagItems(enCD_InventoryType inventory_type)
{
	auto bag = GetBagMgr();
	if (!bag)
		return;
	bag->DebugInfo(inventory_type);
}

void CD_Interface::DbgSkills()
{
	auto p = GetRoleMe();
	if (p)
		p->DebugSkills();
}

int CD_Interface::TraSkill(const Fn_TraSkill& fn)
{
	auto role = GetRoleMe();
	if (!role)
	{
		assert(false);
		return 0;
	}
	return role->skill_mgr_.TraSkill(fn);
}

void CD_Interface::DbgUi(enCD_TraUiShowFlag show_flag)
{
	auto ui_mgr = GetUiMgr();
	if (!ui_mgr)
		return;
	ui_mgr->DebugInfo(show_flag);
}

stCD_UiMgr* CD_Interface::GetUiMgr()
{
	return stCD_UiMgr::GetInstance();
}

stCD_QuestMgr* CD_Interface::GetQuestMgr()
{
	return stCD_QuestMgr::GetInstance();
}

void CD_Interface::DbgQuest()
{
	auto q = GetQuestMgr();
	if (q)
		q->DebugInfo();
}

void CD_Interface::DbgFindUi(enCD_TraUiShowFlag show_flag, pt_csz class_name)
{
	auto ui_mgr = GetUiMgr();
	if (!ui_mgr)
		return;
	auto ui = ui_mgr->FindUi(show_flag, class_name);
	if (!ui)
		return;
	ui->DebugInfo(0);
}

void CD_Interface::DbgSkillSlot()
{
	auto ui_mgr = GetUiMgr();
	if (!ui_mgr)
		return;
	auto ui = ui_mgr->GetUiMainPlayerInfo();
	if (!ui)
		return;
	ui->DebugSlotInfo();
}

void CD_Interface::DbgPortal()
{
	auto m = GetMap();
	if (!m)
		return;
	m->DebugPortal();
}

void CD_Interface::DbgAction()
{
	auto p = GetRoleMe();
	if (!p)
		return;
	p->action_mgr_.DebugAction();
}

void CD_Interface::DbgBodyEquips()
{
	auto role = GetRoleMe();
	if (!role)
		return;
	role->body_equip_mgr_.DebugInfo();
}

void CD_Interface::DbgMails()
{
	GetMailMgr().DebugInfo();
}

stCD_MailMgr& CD_Interface::GetMailMgr()
{
	return stCD_MailMgr::GetMe();
}

stCD_LightService* CD_Interface::GetLightService()
{
	return stCD_LightService::GetInstance();
}

void CD_Interface::DbgLightService()
{
	auto l = GetLightService();
	if (l)
		l->DebugInfo();
}

stCD_UICharacterListDialog* CD_Interface::GetUICharacterListDialog()
{
	auto ui_mgr = GetUiMgr();
	if (!ui_mgr)
		return nullptr;
	return ui_mgr->GetUICharacterListDialog();
}

void CD_Interface::DbgCharacterList()
{
	auto ui = GetUICharacterListDialog();
	if (ui)
		ui->DebugInfo();
}

stCD_PartyMgr* CD_Interface::GetPartyMgr()
{
	return stCD_PartyMgr::GetInstance();
}

void CD_Interface::DbgParty()
{
	auto p = GetPartyMgr();
	if (p)
		p->DebugInfo();
}

stCD_MapInformationService* CD_Interface::GetMapInformationService()
{
	return stCD_MapInformationService::GetInstance();
}

void CD_Interface::DbgMapInformationService()
{
	auto m = GetMapInformationService();
	if (m)
		m->DebugInfo();
}
