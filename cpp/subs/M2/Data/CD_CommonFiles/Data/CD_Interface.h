#pragma once
/*
@author zhp
@date 2017/2/4 11:18
@purpose for interface
*/
#include "CD_Structs.h"
#include <luabind/luabind.hpp>
#include "CD_ProtocolS.h"
#include "CD_ProtocolR.h"
#include <CmnMix/Cmn_Hooker.h>
#include <CmnMix/Cmn_UsefulClass.h>
#include "CD_UsefulFns.h"

namespace CD_Interface{
	typedef stCD_SkillMgr::Fn_TraSkill Fn_TraSkill;
	//////////////////////////////////////////////////////////////////////////
	//功能函数
	void RegSomeForLua(lua_State* l);
	stCD_Map* GetMap();
	stCD_RoleMe* GetRoleMe();
	stCD_BagMgr* GetBagMgr();
	stCD_UiMgr* GetUiMgr();
	stCD_QuestMgr* GetQuestMgr();
	stCD_MailMgr& GetMailMgr();
	stCD_LightService* GetLightService();
	stCD_UICharacterListDialog* GetUICharacterListDialog();
	stCD_PartyMgr* GetPartyMgr();
	stCD_MapInformationService* GetMapInformationService();
	//////////////////////////////////////////////////////////////////////////
	//遍历
	int TraSkill(const Fn_TraSkill& fn);
	//////////////////////////////////////////////////////////////////////////
	//hooks
	class Hooks : public Singleton<Hooks, Singleton_Instance>{
	public:
		void ClearAllHooks();

	public:
		bool DoHooks();

	public:

	private:
	};
	//////////////////////////////////////////////////////////////////////////
	//test
	void RegTestForLua(lua_State* l);
	//////////////////////////////////////////////////////////////////////////
	//debug
	void RegDbgInfoForLua(lua_State* l);
	void DbgMap();
	void DbgRoleMe();
	void DbgGameObjs();
	void DbgGlobalUtility();
	void DbgBagItems(enCD_InventoryType inventory_type);
	void DbgSkills();
	void DbgUi(enCD_TraUiShowFlag show_flag);
	void DbgFindUi(enCD_TraUiShowFlag show_flag, pt_csz class_name);
	void DbgQuest();
	void DbgSkillSlot();
	void DbgPortal();
	void DbgAction();
	void DbgBodyEquips();
	void DbgMails();
	void DbgLightService();
	void DbgCharacterList();
	void DbgParty();
	void DbgMapInformationService();
	//////////////////////////////////////////////////////////////////////////
}
