#include "stdafx.h"
#include "GInterface.h"
#include "GProtocolR.h"
#include "GItem.h"
#include "GPlayer.h"
#include <DllApp.h>
#include "GMap.h"

namespace Detail{
	bool DoTouchTargetObj(enCD_SkillId flag, pt_dword net_id, enCD_TouchTargetSmallFlag small_flag){
		return GWndExecSync([=](){
			return CD_Interface::DoTouchTargetObj(flag, net_id, small_flag);
		});
	}
}

bool GInterface::OpenObj(enCD_SkillId skill_id, pt_dword net_id, enCD_TouchTargetSmallFlag flag)
{
	if (!WaitCanUseSkill())
	{
		assert(false);
		return false;
	}
	return ::Detail::DoTouchTargetObj((enCD_SkillId)skill_id, net_id, flag);
	/*stCDS_OpenObject msg;
	msg.dst_obj_id_ = net_id;
	msg.skill_id_ = (enCD_SkillId)skill_id;
	if (!GWndExecSync([&msg](){
		auto player_entity = CD_Interface::GetPlayerEntity();
		if (!player_entity)
		{
			assert(false);
			return false;
		}
		auto actor = player_entity->GetActor();
		if (!actor)
		{
			assert(false);
			return false;
		}
		msg.skill_use_cnt_ = actor->IncUseSkillCnt();
		return GInterface::Send(msg);
	}))
	{
		assert(false);
		return false;
	}
	return true;*/
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
	//if (check_recv_msg)
	//{
	//	GRecvMsgHandler::GetMe().BeginNpcPanelOper();
	//}
	DummySleep(1200);

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
		if (!TimerDo(60, 1000 * 60, [panel, &talk_label](){
			return GWndExecSync([panel, talk_label](){
				auto npc_panel = CD_Interface::GetNpcPanel(true);
				if (!npc_panel)
					return true;
				return !NpcPanelSubUiShowState(panel, kTUSF_Shown) && !npc_panel->FindPressEventParam(panel, talk_label);
			});
		}))
		{
			assert(false);
			return talk_res;
		}
		//if (!WaitNpcPanelsubUiShowState(panel, kTUSF_UnShown, 1000 * 60))
		//{
		//	assert(false);
		//	return talk_res;
		//}
		//auto& msg_mgr = GRecvMsgHandler::GetMe();
		//if (!msg_mgr.WaitNpcPanelRes())
		//{
		//	assert(false);
		//	return talk_res;
		//}
		//if (!msg_mgr.GetNpcPanelRes())
		//{
		//	assert(false);
		//	return talk_res;
		//}
	}
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
		//assert(false);
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
	if (!WaitMainSubUiShowState((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_died_panel), kTUSF_Shown, 1000 * 60 * 2, false))
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
	/*if (!WaitMainSubUiShowState((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_died_panel), kTUSF_UnShown, 1000 * 60 * 2, false))
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
#ifdef /*VER_GF_*/_NODIFNED
	return GWndExecSync([&dst_pos](){
		return CD_Interface::PlayerMove(dst_pos);
		/*auto player = CD_Interface::GetPlayerEntity();
		if (!player)
		{
			assert(false);
			return false;
		}
		auto actor = player->GetActor();
		if (!actor)
		{
			assert(false);
			return false;
		}
		return actor->DummyMsg_OnMove(false, dst_pos);*/
	});
#elif /*VER_GJ_*/_DEFINED
	return GInterface::GetSendSkillTimeDuration().TimeToDoSync([&dst_pos](){
		return GWndExecSync([&dst_pos](){
			auto player = CD_Interface::GetPlayerEntity();
			if (!player)
			{
				assert(false);
				return false;
			}
			auto actor = player->GetActor();
			if (!actor)
			{
				assert(false);
				return false;
			}
			//中止上次技能，可以避免一直跑路，而点不了游戏的BUG。
			actor->TerminateCurSkill();
			if (!actor->UseSkill(CD_Interface::GetSkillIdMove(), &dst_pos))
			{
				assert(false);
				return false;
			}
			if (!actor->using_skill_timed_)
			{
				LOG_O(Log_debug) << "使用技能后using_skill_timed_竟然为空，难道是没有移动成功？";
				return CD_Interface::PlayerMove(dst_pos);
			}
			return true;
		});
	});
#endif
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
	LOG_O(Log_info) << "创建角色，大区：" << game_area << ",角色名：" << role_name;
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
			LOG_O(Log_warning) << "创建的名字无效";
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

static enLoginAccountRes _CheckLoginAcRes(){
	auto select_state = CD_Interface::GetSelectCharacterState();
	if (select_state)
		return kLAR_Succeed;
	auto pre_state = CD_Interface::GetPreGameState();
	if (!pre_state)
	{
		assert(false);
		return kLAR_Error;
	}
	auto dlg_modal = pre_state->GetDlgModal();
	if (!dlg_modal)
		return kLAR_WaitLoginRes;
	if (!dlg_modal->EqualDlgTitle(kDlgModalStr_LoginError))
	{
		dlg_modal->DebugInfo();
		assert(false);
		return kLAR_Error;
	}
	if (dlg_modal->EqualDlgContent(kDlgModalStr_PwdError))
	{
		//LOG_O(Log_debug) << "密码错误";
		return kLAR_PwdError;
	}
	if (dlg_modal->EqualDlgContent(kDlgModalStr_AcError))
	{
		//LOG_O(Log_debug) << "帐号不存在";
		return kLAR_AcError;
	}
	if (dlg_modal->EqualDlgContent(kDlgModalStr_AcBaned))
	{
		//LOG_O(Log_debug) << "帐号已疯";
		return kLAR_AcBaned;
	}
	dlg_modal->DebugInfo();
	assert(false);
	return kLAR_Error;
}

enLoginAccountRes GInterface::LoginAccount(const std::wstring& ac, const std::wstring& pwd, const std::wstring& gateway)
{
	if (!GWndExecSync([&](){
		auto login_state = CD_Interface::GetLoginState();
		if (!login_state)
		{
			assert(false);
			return false;
		}
		auto pre_state = CD_Interface::GetPreGameState();
		if (!pre_state)
		{
			assert(false);
			return false;
		}
		if (!pre_state->CloseDlgModal())
		{
			assert(false);
			return false;
		}
		if (!login_state->InputLoginInfo(ac, pwd, gateway))
		{
			assert(false);
			return false;
		}
		if (!login_state->PressBtLogin())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return kLAR_Error;
	}
	auto login_res = kLAR_Error;
	if (!TimerDo(1000, 1000 * 60 * 2, [&login_res](){
		GWndExecSyncT(login_res, _CheckLoginAcRes);
		if (login_res != kLAR_WaitLoginRes)
			return true;
		return false;
	}))
	{
		assert(false);
		return kLAR_Timeout;
	}
	return login_res;
}

bool GInterface::IsLoginState()
{
	return GWndExecSync([](){
		return !!CD_Interface::GetLoginState();
	});
}


bool GInterface::WaitCanUseSkill()
{
	return true;
	if (!TimerDo(25, 1000 * 60 * 2, CanUseSkill))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GInterface::CanUseSkill()
{
	return GWndExecSync([](){
		auto entity = CD_Interface::GetPlayerEntity();
		if (!entity)
		{
			assert(false);
			return false;
		}
		auto actor = entity->GetActor();
		if (!actor)
		{
			assert(false);
			return false;
		}
		return actor->CanUseSkill();
	});
}

bool GInterface::TakeupItem(const GItemBase& item)
{
	auto pack = item.GetPackage();
	if (!pack)
	{
		assert(false);
		return false;
	}
	return GWndExecSync([&](){
		auto grid_panel = pack->GetUiGridPanel();
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		if (!grid_panel->PressGridItem(kPIWK_Normal, item.GetItemId()))
		{
			assert(false);
			return false;
		}
		return true;
	});
	/*auto item_id = item.GetItemId();
	if (item_id <= 0)
	{
		assert(false);
		return false;
	}
	auto inventory_type = item.GetInventoryType();
	if (inventory_type <= kIT_Invalid || inventory_type > kIT_Max + 30)
	{
		assert(false);
		return false;
	}
	stCDS_TakeupItem msg;
	msg.inventory_type_ = inventory_type;
	msg.item_id_ = item_id;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	return true;*/
}

bool GInterface::PutItemDownTo(const GPackageBase& packet, const stCD_VecInt& dst_pos)
{
	if (dst_pos.x < 0 || dst_pos.y < 0)
	{
		assert(false);
		return false;
	}
	return GWndExecSync([&](){
		auto grid_panel = packet.GetUiGridPanel();
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		return grid_panel->PutItemDown(dst_pos);
	});
	/*
	if (inventory_type <= kIT_Invalid || inventory_type > kIT_Max + 30)
	{
		assert(false);
		return false;
	}

	stCDS_PutdownItem msg;
	msg.dst_inventory_type_ = inventory_type;
	msg.dst_left_top_idx_ = dst_pos;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	return true;*/
}

bool GInterface::Takeoff_InsideGemImpl(const GItemBase& equip, pt_dword slot_idx, bool is_takeoff)
{
	//auto& msg_mgr = GRecvMsgHandler::GetMe();
	//msg_mgr.BeginItemOper();
	auto packet = equip.GetPackage();
	if (!packet)
	{
		assert(false);
		return false;
	}
	auto old_bag_checksum = GInventoryMgr::GetMe().BagInventoriesChecksum();
	if (!GWndExecSync([&packet, &equip, slot_idx, is_takeoff](){
		auto grid_panel = packet->GetUiGridPanel();
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		if (is_takeoff)
		{
			if (!grid_panel->TakeoffGem(equip.GetItemId(), slot_idx))
			{
				assert(false);
				return false;
			}
		}
		else
		{
			if (!grid_panel->InsideGem(equip.GetItemId(), slot_idx))
			{
				assert(false);
				return false;
			}
		}
		return true;
	}))
	{
		assert(false);
		return false;
	}
	/*if (is_takeoff)
	{
		stCDS_TakeoffGem msg;
		msg.inventory_type_ = equip.GetInventoryType();
		msg.equip_item_id_ = equip.GetItemId();
		msg.slot_idx_ = slot_idx;
		if (!GInterface::Send(msg))
		{
			assert(false);
			return false;
		}
	}
	else
	{
		stCDS_InsideGem msg;
		msg.inventory_type_ = equip.GetInventoryType();
		msg.equip_item_id_ = equip.GetItemId();
		msg.slot_idx_ = slot_idx;
		if (!GInterface::Send(msg))
		{
			assert(false);
			return false;
		}
	}*/
	return GPlayerMe::GetMe().WaitDstItem(nullptr, old_bag_checksum);
	//if (!msg_mgr.WaitItemOperRes())
	//{
	//	assert(false);
	//	return false;
	//}
	//if (!msg_mgr.GetItemOperRes())
	//{
	//	assert(false);
	//	return false;
	//}
	//return true;
}

bool GInterface::UseFlask(const GItemBase& flask)
{
	if (kIT_Flasks != flask.GetInventoryType())
	{
		assert(false);
		return false;
	}
	auto idx = flask.GetItemGridInfo().GetLeftX();
	if (idx >= CD_kBodyFlaskMaxCnt || idx < 0)
	{
		assert(false);
		return false;
	}
	auto& msg_mgr = GRecvMsgHandler::GetMe();
	msg_mgr.BeginItemOper();
	if (!GWndExecSync([idx](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			assert(false);
			return false;
		}
		auto ui_bottom = main_ui->GetUiBottom();
		if (!ui_bottom)
		{
			assert(false);
			return false;
		}
		auto flask_mgr = ui_bottom->GetFlasksMgr();
		if (!flask_mgr)
		{
			assert(false);
			return false;
		}
		if (!flask_mgr->UseFlask(idx))
		{
			assert(false);
			return false;
		}
		return true;
	}))
		return false;
	/*stCDS_UseFlask msg;
	msg.item_idx_ = idx;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}*/
	//if (!msg_mgr.WaitItemOperRes())
	//{
	//	assert(false);
	//	return false;
	//}
	//判断返回结果没鸟用。
	/*if (!msg_mgr.GetItemOperRes())
	{
	assert(false);
	return false;
	}*/
	return true;
}

bool GInterface::UseItem(const GItemBase& item)
{
	if (kIT_MainBag != item.GetInventoryType())
	{
		assert(false);
		return false;
	}
	auto packet = item.GetPackage();
	if (!packet)
	{
		assert(false);
		return false;
	}
	auto old_bag_checksum = GInventoryMgr::GetMe().BagInventoriesChecksum();
	/*stCDS_UseItem msg;
	msg.inventory_type_ = item.GetInventoryType();
	msg.item_id_ = item.GetItemId();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}*/
	if (!GWndExecSync([&packet, &item](){
		auto grid_panel = packet->GetUiGridPanel();
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		return grid_panel->UseItem(item.GetItemId(), item.GetItemEntity());
	}))
	{
		assert(false);
		return false;
	}
	return GPlayerMe::GetMe().WaitDstHasItem(nullptr, old_bag_checksum);
}

bool GInterface::UseItemTo(const GItemBase& item_src, const GItemBase& item_dst)
{
	auto old_bag_checksum = GInventoryMgr::GetMe().BagInventoriesChecksum();
	/*stCDS_UseItemTo msg;
	msg.inventory_type_src_ = item_src.GetInventoryType();
	msg.item_id_src_ = item_src.GetItemId();
	msg.inventory_type_dst_ = item_dst.GetInventoryType();
	msg.item_id_dst_ = item_dst.GetItemId();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}*/
	if (!GWndExecSync([&item_src, &item_dst](){
		auto gl = CD_Interface::GetGameLogic();
		if (!gl)
		{
			assert(false);
			return false;
		}
		return gl->UseItemTo(item_src.GetInventoryType(), item_src.GetItemId(), item_dst.GetInventoryType(), item_dst.GetItemId());
	}))
	{
		assert(false);
		return false;
	}
	auto src_bag = item_src.GetPackage();
	return GPlayerMe::GetMe().WaitDstHasItem(src_bag ? src_bag.get() : nullptr, old_bag_checksum);
}

bool GInterface::MergeItem(const GItemBase& dst_item)
{
	if (dst_item.GetItemMaxStack() <= 0)
	{
		assert(false);
		return false;
	}

	/*stCDS_MergeItem msg;
	msg.dst_item_id_ = dst_item.GetItemId();
	msg.dst_inventory_type_ = dst_item.GetInventoryType();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}*/
	auto pack = dst_item.GetPackage();
	if (!pack)
	{
		assert(false);
		return false;
	}
	return GWndExecSync([&pack, &dst_item](){
		auto grid_panel = pack->GetUiGridPanel();
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		return grid_panel->MergeItem(dst_item.GetItemId());
	});
}

bool GInterface::TopStateSameName(const std::wstring& state_name)
{
	return GWndExecSync([&state_name](){
		return CD_Interface::TopStateSameName(state_name);
	});
}

bool GInterface::OperateCommand(const wchar_t* command)
{
	return GWndExecSync([command](){
		auto logic = CD_Interface::GetGameLogic();
		if (!logic)
		{
			assert(false);
			return false;
		}
		stCD_SafeGameWString the_cmd;
		the_cmd.assign(command);
		return logic->OperateCommand(&the_cmd);
	});
}

int GInterface::GetRemaining()
{
	auto& app = GetMyApp();
	app.remaining_.reset();
	if (!OperateCommand(CD_kLOC_Remaining))
	{
		assert(false);
		return 0;
	}
	if (!TimerDo<tagTimerDoSleep>(50, 2000, [&app](){
		if (app.remaining_)
			return true;
		return false;
	}))
	{
		LOG_O(Log_debug) << "GetRemaining超时";
		return -1;
	}
	assert(app.remaining_);
	return *app.remaining_;
}

bool GInterface::IsCreateStateWithEmptyRole()
{
	return GWndExecSync([](){
		/*if (!CD_Interface::GetPreGameState())
			return false;
		if (!CD_Interface::GetCreateCharacterState())
			return false;
		if (!CD_Interface::GetSelectCharacterState())
			return false;
		return true;*/
		auto c = CD_Interface::GetCreateCharacterState();
		if (!c)
			return false;
		return c->IsSelectCharacterAtBottom();
	});
}
