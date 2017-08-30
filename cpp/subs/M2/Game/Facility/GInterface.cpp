#include "stdafx.h"
#include "GInterface.h"
#include "GProtocolR.h"
#include "GItem.h"
#include "GPlayer.h"
#include "../DllApp.h"
#include "GMap.h"

namespace Detail{
	bool DoTouchTargetObj(enCD_SkillId flag, pt_dword net_id, enCD_TouchTargetSmallFlag small_flag){
		return GWndExecSync([=](){
			return CD_Interface::DoTouchTargetObj(flag, net_id, small_flag);
		});
	}
}

bool GInterface::OpenObj(pt_dword net_id)
{
	return ::Detail::DoTouchTargetObj(kSI_OpenObj, net_id, kTTSF_Normal);
}

bool GInterface::OpenObj(enCD_SkillId skill_id, pt_dword net_id)
{
	return ::Detail::DoTouchTargetObj((enCD_SkillId)skill_id, net_id, kTTSF_Normal);
}

bool GInterface::CloseNpcPanel()
{
	bool direct_res = false;
	if (!GWndExecSync([&direct_res](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			assert(false);
			return false;
		}
		auto npc_panel = main_ui->GetNpcPanel(true);
		if (!npc_panel)
		{
			direct_res = true;
			return true;
		}
		return main_ui->CloseNpcPanel();
	}))
	{
		assert(false);
		return false;
	}
	if (direct_res) return true;
	direct_res = false;
	if (!TimerDo(30, 1000 * 60 * 2, [&direct_res](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			assert(false);
			return true;
		}
		auto npc_panel = main_ui->GetNpcPanel(true);
		if (!npc_panel)
		{
			direct_res = true;
			return true;
		}
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return direct_res;
}

int GInterface::GetInGameMainUiDlgCnt(enCD_TraUiShownFlag show_flag)
{
	int res = 0;
	GWndExecSync([show_flag, &res](){
		if (!CD_Interface::CloseSomeBoringUi())
		{
			assert(false);
			res = CD_kInGameNormalShowDlgMaxCnt + 100;
			return false;
		}
		res = CD_Interface::GetInGameMainUiDlgCnt(show_flag);
		return true;
	});
	return res;
}

bool GInterface::EqualNpcPanelTitle(bool& panel_opened, const std::wstring& npc_title)
{
	return GWndExecSync([&npc_title, &panel_opened](){
		auto npc_panel = CD_Interface::GetNpcPanel(true);
		if (!npc_panel)
			return false;
		panel_opened = true;
		return npc_panel->EqualTitleLabel(npc_title);
	});
}

bool GInterface::MainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag)
{
	return GWndExecSync([=](){
		return CD_Interface::MainSubUiShowState(vtbl, show_flag);
	});
}

enTalkToNpcRes GInterface::TalkToNpc(enCD_NpcPanelItemsContainer panel, const std::wstring& talk_label, bool check_recv_msg)
{
	if (check_recv_msg)
	{
		GRecvMsgHandler::GetMe().BeginNpcPanelOper();
	}
	enTalkToNpcRes talk_res = kTTNR_Error;
	if (!GWndExecSync([&talk_label, panel, &talk_res](){
		auto npc_panel = CD_Interface::GetNpcPanel(true);
		if (!npc_panel)
		{
			talk_res = kTTNR_NpcIsClosed;
			return false;
		}
		if (!npc_panel->PressItem(panel, talk_label))
		{
			talk_res = kTTNR_HaveNotTheLable;
			return false;
		}
		return true;
	}))
	{
		return talk_res;
	}
	if (check_recv_msg)
	{
		auto& msg_mgr = GRecvMsgHandler::GetMe();
		if (!msg_mgr.WaitNpcPanelRes())
		{
			assert(false);
			return talk_res;
		}
		if (!msg_mgr.GetNpcPanelRes())
		{
			assert(false);
			return talk_res;
		}
	}
	
	DummySleep(150);
	talk_res = kTTNR_Succeed;
	return talk_res;
}

bool GInterface::NpcPanelSubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag)
{
	return GWndExecSync([=](){
		return CD_Interface::NpcPanelSubUiShowState(panel, show_flag);
	});
}

TimeDuration& GInterface::GetSendSkillTimeDuration()
{
	static TimeDuration td(200);
	return td;
}

bool GInterface::MouseAttachedItem()
{
	return GWndExecSync([](){
		return !!CD_Interface::GetMouseAttachItem();
	});
}

GItemBasePtr GInterface::GetMouseAttachItem()
{
	GItemBasePtr out_item;
	if (!GWndExecSync([&out_item](){
		auto item = CD_Interface::GetMouseAttachItem();
		if (!item)
			return false;
		out_item = GItemMgrBase().CreateItem(*item, 0, nullptr, nullptr);
		if (!out_item)
		{
			assert(false);
			return false;
		}
		return out_item->Update();
	}))
	{
		assert(false);
		return nullptr;
	}
	return out_item;
}

bool GInterface::MouseAttachedItemCanPutTo(enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y)
{
	return GWndExecSync([=](){
		auto item = CD_Interface::GetMouseAttachItem();
		if (!item)
			return false;
		return CD_Interface::CanPutItemToTheInventory(item, dst_inventory_type, dst_pos_x, dst_pos_y);
	});
}

bool GInterface::CanPutItemToTheInventory(enCD_InventoryType src_inventory_type, pt_dword item_id,
	enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y)
{
	return GWndExecSync([=](){
		return CD_Interface::CanPutItemToTheInventory(src_inventory_type, item_id, dst_inventory_type, dst_pos_x, dst_pos_y);
	});
}

bool GInterface::WaitNpcPanelsubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag, pt_dword timeout)
{
	if (!TimerDo(50, timeout, [=](){
		if (GInterface::NpcPanelSubUiShowState(panel, show_flag))
			return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GInterface::Revive(enCD_ReviveAtPosition at_position)
{
	if (!WaitMainSubUiShowState((pt_pcvoid)g_va_ui_dlg_died_panel, kTUSF_Shown, 1000 * 60 * 2, false))
	{
		assert(false);
		return false;
	}
	::Sleep(500);
	auto& gmap = GMap::GetMe();
	auto old_map_name = gmap.GetMapName();
	if (!GWndExecSync([=](){
		return CD_Interface::Revive(at_position);
	}))
	{
		assert(false);
		return false;
	}
	/*if (!WaitMainSubUiShowState((pt_pcvoid)g_va_ui_dlg_died_panel, kTUSF_UnShown, 1000 * 60 * 2, false))
	{
		assert(false);
		return false;
	}*/
	auto& gpm = GPlayerMe::GetMe();
	if (!TimerDo<tagTimerDoSleep>(50, 1000 * 60 * 2, [&gpm](){
		gpm.Update();
		return gpm.GetCurHp() > 0;
	}))
	{
		assert(false);
		return false;
	}
	::Sleep(500);
	LuasScriptEchoC::TriggerEvent(kEventName_Die);
	if (old_map_name != gmap.GetMapName())
	{
		LOG_O(Log_debug) << "角色死亡后，地图变了，原先地图：" << old_map_name << ",新地图：" << gmap.GetMapName();
		//LuasScriptEchoC::ChangeRunningScript(-1, true);
		LuasScriptEchoC::TriggerEvent(kEventName_Die_ChangeMap);
	}
	return true;
}

bool GInterface::WaitMainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag, pt_dword timeout, bool is_dummy_sleep)
{
	FunctionBool fn = [=](){
		return GInterface::MainSubUiShowState(vtbl, show_flag);
	};
	bool res = false;
	if (is_dummy_sleep)
		res = TimerDo(70, timeout, fn);
	else
		res = TimerDo<tagTimerDoSleep>(70, timeout, fn);
	if (!res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GInterface::PlayerMove(const stCD_VecInt& dst_pos)
{
	return GWndExecSync([&dst_pos](){
		return CD_Interface::PlayerMove(dst_pos);
	});
}

bool GInterface::WaitPreGameState(pt_dword timeout)
{
	if (!TimerDo(500, timeout, GInterface::IsPreGameState))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GInterface::IsPreGameState()
{
	return GWndExecSync([](){
		return !!CD_Interface::GetPreGameState();
	});
}

int GInterface::PressAllLvlUpSkillGem()
{
	int cnt = 0;
	if (!GWndExecSyncT(cnt, CD_Interface::PressAllLvlUpSkillGem))
	{
		assert(false);
	}
	return cnt;
}

bool GInterface::ChooseKillTheMonster()
{
	DummySleep(1000 * 5);
	auto res = GWndExecSync(CD_Interface::ChooseKillTheMonster);
	DummySleep(2000);
	return res;
}

bool GInterface::PressInventoryItem(const GPackageBase& package, const GItemBase& item, enCD_PressItemWithKey key_state)
{
	auto grid_panel = package.GetUiGridPanel();
	if (!grid_panel)
	{
		assert(false);
		return false;
	}
	auto item_id = item.GetItemId();
	return GWndExecSync([grid_panel,item_id, key_state](){
		return grid_panel->PressGridItem(key_state, item_id);
	});
}

stCD_UiGridPanel* GInterface::FindGridPanelFromDlgProp(enCD_InventoryType inventory_type)
{
	stCD_UiGridPanel* res = nullptr;
	GWndExecSyncT(res, [inventory_type](){
		return CD_Interface::FindGridPanelFromDlgProp(inventory_type);
	});
	return res;
}

bool GInterface::LvlUpPassiveSkill(pt_word skill_id)
{
	GPassiveSkillGraphMgr graph_mgr;
	if (!graph_mgr.Update())
	{
		assert(false);
		return false;
	}
	auto old_points = graph_mgr.GetPassiveSkillPoints();
	if (old_points <= 0)
	{
		assert(false);
		return false;
	}
	stCDS_LvlUpPassiveSkill msg;
	msg.skill_id_ = skill_id;
	if (!Send(msg))
	{
		assert(false);
		return false;
	}
	auto new_points = old_points;
	if (!TimerDo(100, 1000 * 60 * 2, [&graph_mgr, &new_points, old_points](){
		if (!graph_mgr.Update())
		{
			assert(false);
			return true;
		}
		new_points = graph_mgr.GetPassiveSkillPoints();
		return new_points != old_points;
	}))
	{
		assert(false);
		return false;
	}
	return old_points != new_points;
}

bool GInterface::EnterGame(const std::string& role_name)
{
	if (role_name.empty())
	{
		assert(false);
		return false;
	}
	auto& gmap = GMap::GetMe();
	if (!gmap.BeginChangeMap())
	{
		assert(false);
		return false;
	}
	auto w_role_name = GbkToWideChar(role_name);
	if (!GWndExecSync([&w_role_name](){
		auto select_top = CD_Interface::GetSelectRoleTop();
		if (!select_top)
		{
			//assert(false);
			return false;
		}
		stCD_stCD_UiRoleListItem* selet_role = nullptr;
		select_top->TraRoleItems([&w_role_name, &selet_role](stCD_stCD_UiRoleListItem* role){
			if (role->role_name_.c_str() == w_role_name)
			{
				selet_role = role;
				return false;
			}
			return true;
		});
		if (!selet_role)
		{
			assert(false);
			return false;
		}
		if (!select_top->EnterGame(selet_role))
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		//assert(false);
		return false;
	}
	return TimerDo(100, 1000 * 60 * 10, [&gmap](){
		return gmap.HasChangedMap();
	});
}

static bool PressEnterToCreateRoleState(){
	bool ret_direct = false;
	if (!GWndExecSync([&ret_direct](){
		auto c_state = CD_Interface::GetCreateCharacterState();
		if (c_state)
		{
			ret_direct = true;
			return true;
		}
		auto s_state = CD_Interface::GetSelectCharacterState();
		if (!s_state)
		{
			assert(false);
			return false;
		}
		return s_state->PressCreateRole();
	}))
	{
		assert(false);
		return false;
	}
	if (ret_direct)
		return true;
	if (!TimerDo(100, 1000 * 60 * 2, [](){
		return GWndExecSync([](){
			return !!CD_Interface::GetCreateCharacterState();
		});
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

enCmnCreateRoleRes GInterface::CreateRole(const std::string& game_area, enCD_ProfessionType profession_type, const std::string& role_name)
{
	if (game_area.empty())
	{
		assert(false);
		return kCmnCRR_Error;
	}
	if (role_name.empty())
	{
		assert(false);
		return kCmnCRR_Error;
	}
	if (role_name.size() > kRoleNameMaxCnt)
	{
		assert(false);
		return kCmnCRR_NameErr;
	}
	if (!PressEnterToCreateRoleState())
	{
		assert(false);
		return kCmnCRR_Error;
	}
	auto& gmap = GMap::GetMe();
	if (!gmap.BeginChangeMap())
	{
		assert(false);
		return kCmnCRR_Error;
	}
	auto& msg_handler = GRecvMsgHandler::GetMe();
	msg_handler.BeginCreateRole();
	auto w_area_name = GbkToWideChar(game_area), w_role_name = GbkToWideChar(role_name);
	if (!GWndExecSync([&w_area_name, &w_role_name, profession_type](){
		auto c_state = CD_Interface::GetCreateCharacterState();
		if (!c_state)
		{
			assert(false);
			return false;
		}
		const stCD_GameArea* select_game_area = nullptr;
		for (auto& area : c_state->game_areas_)
		{
			if (w_area_name == area.area_name_.c_str())
			{
				select_game_area = &area;
				break;
			}
		}
		if (!select_game_area)
		{
			assert(false);
			return false;
		}
		if (!c_state->SelectGameArea(*select_game_area))
		{
			assert(false);
			return false;
		}
		if (!c_state->SelectCharacter(profession_type))
		{
			assert(false);
			return false;
		}
		if (!c_state->InputRoleName(w_role_name))
		{
			assert(false);
			return false;
		}
		if (!c_state->PressButton(kCRBI_Ok))
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return kCmnCRR_Error;
	}
	enCmnCreateRoleRes res = kCmnCRR_Succeed;
	if (!TimerDo(100, 1000 * 60 * 10, [&res, &msg_handler, &gmap](){
		if (!msg_handler.GetCreateRoleRes())
		{
			res = kCmnCRR_NameErr;
			return true;
		}
		return gmap.HasChangedMap();
	}))
	{
		assert(false);
		return kCmnCRR_Error;
	}
	return res;
}

bool GInterface::IsInGameState()
{
	return GWndExecSync([](){
		return !!CD_Interface::GetInGameState();
	});
}
