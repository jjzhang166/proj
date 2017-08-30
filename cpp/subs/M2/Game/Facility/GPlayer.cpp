#include "stdafx.h"
#include "GPlayer.h"
#include "GClass.h"
#include "GConst.h"
#include <dinput.h>
#include <string>
#include <CommonX/CmnX_Math.h>
#include "GMap.h"
#include "GNpc.h"
#include "GType.h"
#include "GProtocolR.h"
#include "../Scmd.h"
#include <Script/LuaScript.h>
#include "GActions.h"


GPlayer::GPlayer() : GameObjBase(kGOT_Player)
{

}

GPlayerMe::GPlayerMe()
{
	is_moving_ = false;
	is_openning_ = false;
	difficulty_ = kMD_Normal;
	avoid_skill_deep_ = 0;
	moving_dst_pos_ = nullptr;
	auto_open_chest_ = false;
	auto_pickup_item_ = false;
	auto_open_waypoint_ = false;
	auto_open_transitionable_ = false;
	auto_lvl_up_skill_gem_by_default_ = false;
	on_tra_map_over_deep_ = 0;
}

bool GPlayerMe::UpdateImpl()
{
	auto entity = CD_Interface::GetPlayerEntity();
	if (!entity)
	{
		assert(false);
		return false;
	}
	if (!SetGameEntity(entity))
	{
		assert(false);
		return false;
	}
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}
	auto cur_player = entity->GetPlayer();
	if (IsUnValidPtr(cur_player))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(cur_player->name_))
	{
		assert(false);
		return false;
	}
	SetName(cur_player->name_.GetStdString());

	auto attrdb = cur_player->GetAttrDb();
	if (IsUnValidPtr(attrdb))
	{
		assert(false);
		return false;
	}

	job_name_.assign(CD_SafeName(attrdb->profession_name_));
	if (job_name_.empty())
	{
		assert(false);
		return false;
	}

	job_id_ = attrdb->id_;
	if (job_id_ <= 0)
	{
		assert(false);
		return false;
	}

	base_pro_.level_ = cur_player->level_;
	if (base_pro_.level_ <= 0)
	{
		assert(false);
		return false;
	}

	base_pro_.power_ = cur_player->power_;
	if (base_pro_.power_ <= 0)
	{
		assert(false);
		return false;
	}

	base_pro_.smart_ = cur_player->smart_;
	if (base_pro_.smart_ <= 0)
	{
		assert(false);
		return false;
	}
	
	base_pro_.brain_ = cur_player->brain_;
	if (base_pro_.brain_ <= 0)
	{
		assert(false);
		return false;
	}
	difficulty_ = cur_player->difficulty_;
	if (difficulty_ <= 0)
	{
		assert(false);
		return false;
	}

	auto path = CD_Interface::GetPlayerPathfinding();
	if (IsUnValidPtr(path))
	{
		assert(false);
		return false;
	}
	is_moving_ = path->is_moving_;

	auto life_ptr = CD_Interface::GetPlayerLife();
	if (IsUnValidPtr(life_ptr))
	{
		assert(false);
		return false;
	}
	hp_ = life_ptr->hp_;
	mp_ = life_ptr->mp_;
	shield_ = life_ptr->shield_;
	if (hp_.max_value_ < 0 ||
		mp_.max_value_ < 0 ||
		shield_.max_value_ < 0
		)
	{
		assert(false);
		return false;
	}
	return true;
}

void GPlayerMe::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_info) << "level-" << base_pro_.level_ << ",力量-" << base_pro_.power_ << ",智力-" << base_pro_.brain_ << ",敏捷" << base_pro_.smart_
		<< ",job_name" << job_name_;
}

bool GPlayerMe::SendUseSkill(pt_word skill_id, const stCD_VecInt& dst_pos)
{
	stCDS_UseSkillEnd skill_end;
	/*if (!GInterface::Send(skill_end))
	{
		assert(false);
		return false;
	}*/
	stCDS_UseSkill skill_msg;
	skill_msg.dst_pos_ = dst_pos;
	skill_msg.skill_id_ = skill_id;
	if (!GInterface::SendSpan(GInterface::GetSendSkillTimeDuration(), skill_msg))
	{
		assert(false);
		return false;
	}
	BOOST_SCOPE_EXIT_ALL(&skill_end){
		if (!GInterface::Send(skill_end))
		{
			assert(false);
			return;
		}
	};
	DummySleep(50);
	return true;
}

void GPlayerMe::TestDebug(const std::string& str)
{
	LOG_O(Log_debug) << " player TestPaclDebig";
	//inventory_mgr_.RebuildAll();
	//auto& inventory = GInventoryMgr::GetMe();
	//inventory.RebuildAll();
	//inventory.AdjustPack();
	//inventory_mgr_.DebugInfo();

	//quest_mgr_.RebuildAll();
	//quest_mgr_.DebugInfo();
	
	//auto& bagMgr = GInventoryMgr::GetMe();


}

bool GPlayerMe::UseSkill(const std::string& skill_name, const stCD_VecInt& dst_pos)
{
	auto& mgr = GSkillMgr::GetMe();
	auto skill = mgr.FindByName(skill_name);
	if (!skill)
	{
		if (!mgr.RebuildAll())
		{
			assert(false);
			return false;
		}
		skill = mgr.FindByName(skill_name);
		if (!skill)
		{
			assert(false);
			return false;
		}
	}
	auto skill_tmp = *skill;
	return UseSkill(skill_tmp, dst_pos);
}

bool GPlayerMe::UseSkill(const GSkillObj& skill, const stCD_VecInt& dst_pos)
{
	return SendUseSkill(skill.GetSkillId(), dst_pos);
}

int GPlayerMe::GetCurHp() const
{
	return hp_.cur_value_;
}

int GPlayerMe::GetCurMp() const
{
	return mp_.cur_value_;
}

bool GPlayerMe::SendTakeupItem(const GItemBase& item)
{
	auto item_id = item.GetItemId();
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
	return true;
}
bool GPlayerMe::SendPutDownItem(const stCD_VecInt& dst_pos, const enCD_InventoryType inventory_type)
{
	if (dst_pos.x < 0 || dst_pos.y < 0 )
	{
		assert(false);
		return false;
	}
	if (inventory_type <= kIT_Invalid || inventory_type > kIT_Max + 30)
	{
		assert(false);
		return false;
	}

	stCDS_PutdownItem msg;
	msg.dst_inventory_type_ = inventory_type;
	msg.dst_left_top_idx_	= dst_pos;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::TakeupItem(enCD_InventoryType type, const stCD_VecInt& pos)
{
	auto bag = GInventoryMgr::GetMe().GetPackBase(type);
	if (!bag)
	{
		assert(false);
		return false;
	}
	auto item = bag->FindItemByPos(pos);
	//auto item = bag->FindItemById(item_id);
	if (!item)
	{
		assert(false);
		return false;
	}
	if (GInterface::MouseAttachedItem())
	{
		assert(false);
		return false;
	}
	if (!SendTakeupItem(*item))
	{
		assert(false);
		return false;
	}
	IoTimer::lock_guard locker;
	if (!TimerDo(50, 1000 * 60 * 2, [](){
		return GInterface::MouseAttachedItem();
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::PutDownItem( const stCD_VecInt& dst_pos,enCD_InventoryType dst_inventory_type)
{
	auto& inventory_mgr_ = GInventoryMgr::GetMe();
	auto bag = inventory_mgr_.GetPackBase(dst_inventory_type);
	if (!bag)
	{
		assert(false);
		return false;
	}
	auto item = bag->FindItemByPos(dst_pos);
	if (item)
		bag->SetLastPosId(item->GetItemId());
	if (!GInterface::MouseAttachedItem())
	{
		assert(false);
		return false;
	}

	if (!SendPutDownItem(dst_pos, dst_inventory_type))
	{
		assert(false);
		return false;
	}
	return WaitDstHasItem(*bag, dst_pos);
}

bool GPlayerMe::CloseNpcPanel()
{
	if (!TimerDo(40, 1000 * 60, GInterface::CloseNpcPanel))
	{
		assert(false);
		return false;
	}
	return true;
}

void GPlayerMe::RememberObjs(const GameObjMgrBase& obj_mgr)
{
	remembered_objs_ += obj_mgr;
	remembered_objs_.ResetAll();
}

void GPlayerMe::ClearRememberedObjs()
{
	remembered_objs_.clear();
}

const GameObjMgrBase& GPlayerMe::GetRememberedObjs() const
{
	return remembered_objs_;
}

bool GPlayerMe::MoveMapByWaypoint(enCD_kMapDifficulty difficulty, const std::string& dst_map_name)
{
	if (dst_map_name.empty())
	{
		assert(false);
		return false;
	}
	if (difficulty <= kMD_Invalid || difficulty >= KMD_Max)
	{
		assert(false);
		return false;
	}
	
	auto& map = GMap::GetMe();
	if (!GWndExecSync([this, &map](){
		if (!Update())
		{
			assert(false);
			return false;
		}
		if (!map.Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return false;
	}
	if (difficulty == difficulty_ && map.GetMapName() == dst_map_name)
	{
		assert(false);
		return false;
	}
	GClass::stLastMapInfo last_map_info;
	if (!map.BeginChangeMap(last_map_info))
	{
		assert(false);
		return false;
	}	
	assert(!dst_map_name.empty());
	auto w_dst_map_name = GbkToWideChar(dst_map_name);
	if (!GWndExecSync([&w_dst_map_name, difficulty](){
		auto dlg = CD_Interface::GetDlgWayPoint();
		if (!dlg)
		{
			assert(false);
			return false;
		}
		stCD_DlgWaypoint::stWaypointInfo item_info;
		if (!dlg->FindWaypoint(difficulty, w_dst_map_name, item_info))
		{
			assert(false);
			return false;
		}
		assert(item_info.map_);
		assert(item_info.waypoint_);
		if (0 == item_info.map_->connections_num_)
		{
			assert(!"没打开传送点吧？");
			return false;
		}
		if (!item_info.map_->TheMapHasWaypoint(*item_info.waypoint_))
		{
			assert(!"目标地图并没有传送点");
			return false;
		}
		auto dst_area = item_info.map_->GetTransferWolrdItem(*item_info.waypoint_);
		if (!dst_area)
		{
			assert(false);
			return false;
		}
		if (IsUnValidPtr(dst_area->ch_map_name_))
		{
			assert(false);
			return false;
		}
		return item_info.map_->CallChangeMap(item_info.waypoint_);
	}))
	{
		assert(false);
		return false;
	}
	return GOpenVisitor::WaitCrossMap(last_map_info, nullptr);
}

void GPlayerMe::TestChangeMap(int difficulty, const std::string& dst_map_name)
{
	MoveMapByWaypoint((enCD_kMapDifficulty)difficulty, dst_map_name);
}

bool GPlayerMe::PressEscOnceToClosePanel(int* cur_dlg_cnt)
{
	if (!CloseEscapeMenu())
	{
		assert(false);
		return false;
	}
	auto old_cnt = 0;
	if (!GWndExecSync([&old_cnt](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			assert(false);
			return false;
		}
		old_cnt = main_ui->GetDlgCnt(kTUSF_Shown);
		if (old_cnt <= CD_kInGameNormalShowDlgMaxCnt)
			return true;
		if (!main_ui->EscapePanel())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return false;
	}
	if (old_cnt <= CD_kInGameNormalShowDlgMaxCnt)
		return true;
	if (!TimerDo(40, 1000 * 60, [cur_dlg_cnt, old_cnt](){
		auto new_cnt = GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown);
		if (new_cnt < old_cnt)
		{
			if (cur_dlg_cnt)
				*cur_dlg_cnt = new_cnt;
			return true;
		}
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::OpenEscapeMenu()
{
	if (EscapeMenuOpened())
		return true;
	if (!CloseAllPanel())
	{
		assert(false);
		return false;
	}
	if (!GWndExecSync([](){
		if (!CD_Interface::SwitchToEscapeState())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return false;
	}
	if (!TimerDo(40, 1000 * 60, [this](){
		if (EscapeMenuOpened()) return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::CloseEscapeMenu()
{
	if (!EscapeMenuOpened())
		return true;
	if (!PressEscapeMenuItem(kEMB_Continue))
	{
		assert(false);
		return false;
	}
	if (!TimerDo(40, 1000 * 60, [this](){
		if (EscapeMenuOpened()) return false;
		return true;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::EscapeMenuOpened() const
{
	return GWndExecSync([](){
		auto ptr = CD_Interface::GetSceneHolder();
		if (!ptr)
		{
			assert(false);
			return false;
		}
		return ptr->TopStateIsEscape();
	});
}

bool GPlayerMe::PressEscapeMenuItem(enCD_EscapeMenuButton bt_idx)
{
	return GWndExecSync([bt_idx](){
		if (!CD_Interface::PressEscapeBtItem(bt_idx))
		{
			assert(false);
			return false;
		}
		return true;
	});
}

bool GPlayerMe::CloseAllPanel(bool close_npc_panel)
{
	int dlg_cnt = GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown);;
	if (dlg_cnt <= CD_kInGameNormalShowDlgMaxCnt)
	{
		if (!CloseEscapeMenu())
		{
			assert(false);
			return false;
		}
		if (close_npc_panel && !CloseNpcPanel())
		{
			assert(false);
			return false;
		}
		return true;
	}
	for (int cnt = 0; cnt != 100; ++cnt)
	{
		if (!PressEscOnceToClosePanel(&dlg_cnt))
		{
			assert(false);
			return false;
		}
		if (dlg_cnt <= CD_kInGameNormalShowDlgMaxCnt)
		{
			if (close_npc_panel && !CloseNpcPanel())
			{
				assert(false);
				return false;
			}
			return true;
		}
	}
	assert(false);
	return false;
}

void GPlayerMe::TestCloseUi(int type)
{
	switch (type)
	{
	case 1:
		CloseAllPanel();
		break;
	case 2:
		OpenEscapeMenu();
		break;
	case 3:
		CloseEscapeMenu();
		break;
	default:
		assert(false);
		break;
	}
}

bool GPlayerMe::TalkToNpc(const std::string& talk_label)
{
	if (talk_label.empty())
	{
		assert(false);
		return false;
	}
	if (!GWndExecSync([](){
		auto npc_panel = CD_Interface::GetNpcPanel(true);
		if (!npc_panel)
		{
			//assert(false);
			return false;
		}
		return true;
	}))
	{
		//assert(!"npc面板没打开吧？");
		LOG_O(Log_debug) << "npc面板没打开，无法对话npc，label:" << talk_label;
		return false;
	}
	//必须要先清空面板
	if (!CloseAllPanel(false))
	{
		assert(false);
		return false;
	}
	auto& handle_label = GHandleNpcTalkLabel::GetMe();
	auto w_talk_label = GbkToWideChar(talk_label);
	auto talk_res = handle_label.PreHandleLabel(w_talk_label);
	if (talk_res < kTTNR_Succeed)
		return false;
	else if (talk_res == kTTNR_NotNeedHandle)
		return true;
	talk_res = handle_label.HandleLabel(w_talk_label);
	return kTTNR_Succeed == talk_res;
}

void GPlayerMe::TestTalkToNpc(const std::string& talk_label)
{
	TalkToNpc(talk_label);
}

bool GPlayerMe::SmartMoveMap(const std::string& dst_map_name, enCD_kMapDifficulty diff_lvl /*= kMD_Invalid*/, bool is_use_item /*= false*/)
{
	if (dst_map_name.empty())
	{
		assert(false);
		return false;
	}
	if (diff_lvl <= kMD_Invalid || diff_lvl >= KMD_Max)
	{
		assert(false);
		return false;
	}
	auto& resolver = GSP_Resolver::GetMe();
	auto& way = GWayPointDlgMgr::GetMe();
	if (!way.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& cur_map = GMap::GetMe();
	if (!cur_map.Update())
	{
		assert(false);
		return false;
	}
	auto cur_map_diff = GetDiffLvl();
	if (cur_map_diff <= kMD_Invalid)
	{
		assert(false);
		return false;
	}
	if (cur_map.GetMapName() == dst_map_name && diff_lvl == cur_map_diff)
	{
		LOG_O(Log_debug) << "最短路径目标地图等于当前地图 : " << dst_map_name << "难度等级" << cur_map_diff << ":" << diff_lvl;
		return true;
	}
	GSP_Resolver::DequeHelp short_path;
	if (!resolver.GetDijkLatelyPath(dst_map_name, GetDiffLvl(), short_path))
	{
		assert(false);
		return false;
	}
	if (short_path.empty())
	{
		LOG_O(Log_error) << "获得最短路径失败";
		assert(false);
		return false;
	}

	while (!short_path.empty())
	{
		GSP_Resolver::stMoveHelp move_help = short_path.front();
		cur_map.Update();
		if (move_help.end_name_.empty())
		{
			short_path.pop_front();
			continue;
		}
		if (!Update())
		{
			assert(false);
			return false;
		}
		if (cur_map.GetMapName() == dst_map_name && GetDiffLvl() == diff_lvl)
			break;
		if (move_help.type_ == kGOT_Waypoint)
		{
			if (MoveToOpenObj(nullptr, true, kObjName_Waypoint) < kOVR_Succeed)
			{
				assert(false);
				return false;
			}
			if (!MoveMapByWaypoint(move_help.difficulty_, move_help.end_name_))
			{
				assert(false);
				return false;
			}
		}
		else if (move_help.type_ == kGOT_AreaTransition)
		{
			if (MoveToOpenObj(nullptr, true, move_help.end_name_) < kOVR_Succeed)
			{
				assert(false);
				return false;
			}
		}
		if (!TimerDo(50, 1000 * 60, [&cur_map, &move_help](){
			if (!cur_map.Update())
			{
				assert(false);
				return false;
			}
			return move_help.ori_map_ == cur_map.GetMapName();
		}))
		{
			assert(false);
			return false;
		}
		short_path.pop_front();
	}
	return true;
}

bool GPlayerMe::NpcItemToInventory(const GItemBase& item, const stCD_VecInt& dst_pos, enCD_InventoryType inventory_type)
{
	auto item_id = item.GetItemId();
	if (dst_pos.x < 0 || dst_pos.y < 0)
	{
		assert(false);
		return false;
	}
	if (inventory_type < kIT_MainBag || inventory_type > kIT_Flasks)
	{
		assert(false);
		return false;
	}
	auto& inventory_mgr = GInventoryMgr::GetMe();
	auto bag = inventory_mgr.GetPackBase(inventory_type);
	if (!bag)
	{
		assert(false);
		return false;
	}

	stCDS_NpcInventoryItemPutToSelfInventory msg;
	msg.dst_inventory_type_ = inventory_type;
	msg.dst_pos_ = dst_pos;
	msg.item_id_ = item_id;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	return WaitDstHasItem(*bag, dst_pos);
}

bool GPlayerMe::WaitDstHasItem(GPackageBase& packet, const stCD_VecInt& dst_pos)
{
	IoTimer::lock_guard locker;
	if (!TimerDo(30, 1000 * 60 * 2, [&dst_pos, &packet](){
		packet.Update();
		auto item = packet.FindItemByPos(dst_pos);
		if (item)
			return (packet.GetLastPosId() == 0 ) || (item->GetItemId() != packet.GetLastPosId());
		return false;
		//return !!packet.GetItemByPos(dst_pos) && packet;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

static bool Takeoff_InsideGemImpl(const GItemBase& equip, pt_dword slot_idx, bool is_takeoff){
	auto& msg_mgr = GRecvMsgHandler::GetMe();
	msg_mgr.BeginItemOper();
	if (is_takeoff)
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
	}
	if (!msg_mgr.WaitItemOperRes())
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.GetItemOperRes())
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::SendTakeoffGem(const GItemBase& equip, pt_dword slot_idx)
{
	return Takeoff_InsideGemImpl(equip, slot_idx, true);
}

bool GPlayerMe::SendInsideGem(const GItemBase& equip, pt_dword slot_idx)
{
	return Takeoff_InsideGemImpl(equip, slot_idx, false);
}

bool GPlayerMe::TakeoffGem(enCD_InventoryType type, const stCD_VecInt& pos, int index)
{
	if (index < 0 || index >= CD_kGemSlotMaxCnt)
	{
		assert(false);
		return false;
	}
	auto& inventory_mgr_ = GInventoryMgr::GetMe();
	auto bag = inventory_mgr_.GetPackBase(type);
	if (!bag)
	{
		assert(false);
		return false;
	}
	auto item  = bag->FindItemByPos(pos);
	if (!item)
	{
		assert(false);
		return false;
	}
	if (!SendTakeoffGem(*item, index))
	{
		assert(false);
		return false;
	}

	return true;
}

bool GPlayerMe::InsideGem(enCD_InventoryType type, pt_dword item_id, int index)
{
	if (index < 0 || index >= CD_kGemSlotMaxCnt)
	{
		assert(false);
		return false;
	}
	auto& inventory_mgr_ = GInventoryMgr::GetMe();
	auto bag = inventory_mgr_.GetPackBase(type);
	if (!bag)
	{
		assert(false);
		return false;
	}
	auto equip_item = bag->FindItemById(item_id);
	if (!equip_item)
	{
		assert(false);
		return false;
	}
	if (!SendInsideGem(*equip_item, index))
	{
		assert(false);
		return false;
	}

	return true;

}

bool GPlayerMe::SendUseFlask(const GItemBase& flask)
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
	stCDS_UseFlask msg;
	msg.item_idx_ = idx;
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.WaitItemOperRes())
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.GetItemOperRes())
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::SendUseItem(const GItemBase& item)
{
	if (kIT_MainBag != item.GetInventoryType())
	{
		assert(false);
		return false;
	}
	auto& msg_mgr = GRecvMsgHandler::GetMe();
	msg_mgr.BeginItemOper();
	stCDS_UseItem msg;
	msg.inventory_type_ = item.GetInventoryType();
	msg.item_id_ = item.GetItemId();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.WaitItemOperRes())
	{	
		assert(false);
		return false;
	}
	//这个记得屏蔽了！
	//if (!msg_mgr.GetItemOperRes())
	//{
	//	assert(false);
	//	return false;
	//}
	return true;
}

bool GPlayerMe::SendUseItemTo(const GItemBase& item_src, const GItemBase& item_dst)
{
	auto& msg_mgr = GRecvMsgHandler::GetMe();
	msg_mgr.BeginItemOper();
	stCDS_UseItemTo msg;
	msg.inventory_type_src_ = item_src.GetInventoryType();
	msg.item_id_src_ = item_src.GetItemId();
	msg.inventory_type_dst_ = item_dst.GetInventoryType();
	msg.item_id_dst_ = item_dst.GetItemId();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.WaitItemOperRes())
	{
		assert(false);
		return false;
	}
	//if (!msg_mgr.GetItemOperRes())
	//{
	//	assert(false);
	//	return false;
	//}
	return true;
}

bool GPlayerMe::UseItem(const GItemBase& item)
{
	return SendUseItem(item);
}

bool GPlayerMe::UseItem(const GItemBase& item_src, const GItemBase& item_dst)
{
	return SendUseItemTo(item_src, item_dst);
}

GameObjBasePtrT GPlayerMe::FindNearestObj(GameObjType obj_type)
{
	GameObjMgrPtr obj_mgr(new GameObjMgr(obj_type));
	if (!obj_mgr)
	{
		assert(false);
		return nullptr;
	}
	if (!GWndExecSync([&obj_mgr, this](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return nullptr;
	}
	
	auto pos = GetPos();
	if (!pos)
	{
		assert(false);
		return nullptr;
	}
	obj_mgr->SortByWeightDistance(*pos);
	return obj_mgr->GetFirstObj();
}

GameObjBasePtrT GPlayerMe::FindNearestObj(const std::string& obj_name)
{
	auto obj_mgr_ptr = new GameObjMgr;
	GameObjMgrPtr mgr(obj_mgr_ptr);
	if (!mgr)
	{
		assert(false);
		return nullptr;
	}
	obj_mgr_ptr->IncludeLifedFilter();
	obj_mgr_ptr->IncludeOpenableFilter();
	if (!GWndExecSync([&mgr, this](){
		if (!mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return nullptr;
	}
	auto pos = GetPos();
	if (!pos)
	{
		assert(false);
		return nullptr;
	}
	mgr->SortByWeightDistance(*pos);
	return mgr->FindByName(obj_name);
}

void GPlayerMe::TestAvoidPos(bool enable)
{
	GMap::GetMe().EnableAvoidSkill(enable);
}

stCD_VecInt GPlayerMe::GetCurMovingDstPos()
{
	stCD_VecInt res;
	if (!moving_dst_pos_)
		return res;
	ReadLock lock(moving_dst_pos_mutex_);
	if (!moving_dst_pos_)
		return res;
	res = *moving_dst_pos_;
	return res;
}

bool GPlayerMe::UseEatFlask(const GItemBase& flask)
{
	if (!flask.IsFlask())
	{
		assert(false);
		return false;
	}
	if (flask.GetInventoryType() != kIT_Flasks)
	{
		assert(false);
		return false;
	}


	if (!SendUseFlask(flask))
	{
		assert(false);
		return false;
	}
	return true;
}



bool GPlayerMe::SendMergeItem(const GItemBase& dst_item)
{
	if (dst_item.GetItemMaxStack() <= 0)
	{
		assert(false);
		return false;
	}
	
	stCDS_MergeItem msg;
	msg.dst_item_id_ = dst_item.GetItemId();
	msg.dst_inventory_type_ = dst_item.GetInventoryType();
	if (!GInterface::Send(msg))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::MergeItem( const GItemBase& dst_item)
{
	if (dst_item.GetItemMaxStack() <= 0)
	{
		assert(false);
		return false;
	}

	auto item_cout = dst_item.GetItemCnt();
	auto item_pos = dst_item.GetCurItemPos();
	auto& inventory =  GInventoryMgr::GetMe();
	auto dst_bag = inventory.GetPackBase(dst_item.GetInventoryType());
	if (!dst_bag)
	{
		assert(false);
		return false;
	}

	if (!SendMergeItem(dst_item))
	{
		assert(false);
		return false;
	}
	if (!TimerDo(50, 1000 * 60 * 2, [&item_pos,&item_cout, &dst_bag](){
		dst_bag->Update();
		auto item = dst_bag->FindItemByPos(item_pos);
		if (!item)
			return false;
		return item->GetItemCnt() > item_cout;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::SendDestroy(const GItemBase& dst_item)
{
	auto& msg_mgr = GRecvMsgHandler::GetMe();
	msg_mgr.BeginItemOper();
	stCDS_OperCommand msg;
	msg.sub_msg_id_ = kOCSMID_DestroyItem;
	if (!CD_Interface::Send(msg))
	{
		assert(false);
		return false;
	}

	if (!msg_mgr.WaitItemOperRes())
	{
		assert(false);
		return false;
	}
	if (!msg_mgr.GetItemOperRes())
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::DestroyItem(const GItemBase& item, bool need_takeup)
{
	if (item.IsQuestItem())
	{
		LOG_O(Log_error) << "该物品是任务物品，不能摧毁，物品名:" << item.GetItemName();
		return false;
	}
	if (need_takeup)
	{
		auto pack_type = item.GetInventoryType();
		auto item_pos = item.GetCurItemPos();
		if (item_pos.AllLessZero())
		{
			assert(false);
			return false;
		}
		if (!TakeupItem(pack_type, item.GetCurItemPos()))
		{
			assert(false);
			return false;
		}
		//DummySleep(200);
	}
	else
	{
		if (!GInterface::MouseAttachedItem())
		{
			assert(false);
			return false;
		}
	}

	if (!SendDestroy(item))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::WaitRoleCanMove(pt_dword timeout)
{
	auto& gmap = GMap::GetMe();
	if (!gmap.Update())
	{
		assert(false);
		return false;
	}
	if (!Update())
	{
		assert(false);
		return false;
	}
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return false;
	}
	auto& path_mark = GPathMark::GetMe();
	stCD_VecInt out_pos;
	if (!path_mark.GetNearestPos(true, *my_pos, out_pos))
	{
		assert(false);
		return false;
	}
	assert(out_pos != *my_pos);
	auto old_my_pos = *my_pos;
	stCDS_UseSkill skill_msg;
	skill_msg.dst_pos_ = out_pos;
	skill_msg.skill_id_ = CD_Interface::GetSkillIdMove();
	//skill_msg.skill_id_ = CD_kSkillId_NormalHit;
	stCDS_UseSkillEnd skill_end;
	if (!TimerDo(600, timeout, [&out_pos, &old_my_pos, my_pos, &skill_msg, &skill_end](){
		GWndExecSync([&skill_msg, &skill_end](){
			if (!GInterface::Send(skill_msg))
			{
				assert(false);
				return false;
			}
			if (!GInterface::Send(skill_end))
			{
				assert(false);
				return false;
			}
			return true;
		});
		return old_my_pos != *my_pos;
	}))
	{
		assert(false);
		return false;
	}
	return true;
	
}

bool GPlayerMe::AcceptAnyQuest()
{
	auto& gsp_resolver = GSP_Resolver::GetMe();
	if (!gsp_resolver.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& map = GMap::GetMe();
	if (!map.Update())
	{
		assert(false);
		return false;
	}
	auto& way_point = GWayPointDlgMgr::GetMe();
	if (!way_point.RebuildAll())
	{
		assert(false);
		return false;
	}
	if (!gsp_resolver.IsTownByMapName(map.GetMapName(), GetDiffLvl()) && !UseWayToTown())
	{
		auto dst_town_name = gsp_resolver.GetCurChapterTownName(map.GetChapter());
		if (!SmartMoveMap(dst_town_name, GetDiffLvl(), false))
		{
			assert(false);
			return false;
		}
	}
	GSP_Resolver::MapNameCont all_towm;
	gsp_resolver.GetAllTownName(all_towm);
	if (all_towm.empty())
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	{
		auto& cur_map = map.GetMapName();
		auto it = std::find_if(all_towm.begin(), all_towm.end(), [&cur_map](const std::string& map_name){
			return cur_map == map_name;
		});
		if (it != all_towm.end())
		{
			all_towm.erase(it);
			it = all_towm.begin();
			if (it == all_towm.end())
				all_towm.push_back(cur_map);
			else
				all_towm.insert(it, cur_map);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Npc));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterGameObjNeedVisitable need_visit(obj_mgr->GetFilter());
	auto tester_any_obj = factory.MakeTesterAnyObj(obj_mgr);
	if (!tester_any_obj)
	{
		assert(false);
		return false;
	}
	auto tra_full_map = factory.MakeTraFullMap(consumer, tester_any_obj, false);
	if (!tra_full_map)
	{
		assert(false);
		return false;
	}
	tester_any_obj->SetParent(consumer);
	bool is_find = false;
	GameObjBasePtrT the_npc;
	for (auto& town : all_towm)
	{
		if (!Update())
		{
			assert(false);
			return false;
		}
		if (!way_point.IsOpenedWayPoint(town, GetDiffLvl()))
			continue;
		if (!SmartMoveMap(town, GetDiffLvl()))
		{
			LOG_O(Log_error) << "移动到其他城镇接任务失败:" << town;
			return false;
		}
		if (!map.Update())
		{
			assert(false);
			return false;
		}
		consumer->Add(tra_full_map);
		const auto& run_res = consumer->Run();
		if (run_res != tester_any_obj)
			continue;
		the_npc = tester_any_obj->GetFirstObj();
		if (!the_npc)
		{
			assert(false);
			return false;
		}
		break;
	}
	if (!the_npc)
	{
		LOG_O(Log_warning) << "没有接到任意任务";
		return false;
	}
	if (MoveToOpenObj(nullptr, false, the_npc) < kOVR_Succeed)
	{
		LOG_O(Log_error) << "接任意任务之移动到NPC失败：" << the_npc->GetCnName();
		return false;
	}	
	if (!GInterface::WaitNpcPanelsubUiShowState(kNPIC_TalkPanel, kTUSF_Shown, 1000 * 20))
	{
		assert(false);
		return false;
	}
	if (!TalkToNpc(CD_kNTL_CotinueA))
	{
		//assert(false);
		return false;
	}
	if (!TalkToNpc(CD_kNTL_ByeA))
	{
		//assert(false);
		return false;
	}
	DummySleep(2000);
	return true;
}

bool GPlayerMe::MoveOtherMapByWayPoint(enCD_kMapDifficulty difficulty, const std::string& dst_map_name)
{
	return MoveMapByWaypoint(difficulty, dst_map_name);
}

bool GPlayerMe::KillToKillTheObj(const GameObjBasePtrT& dst_obj)
{
	if (!dst_obj)
	{
		assert(false);
		return false;
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto open_objs = factory.MakeOpenObjs(consumer);
	CA_ActionPtr open_objs_tester;
	if (open_objs)
		open_objs_tester = open_objs->Tester();
	auto action_kill = factory.MakeKillMonster(dst_obj);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, open_objs_tester);
	const auto& run_res = consumer->Run();
	if (!run_res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::TestPlayerMove(const stCD_VecInt& dst_pos)
{
	return GInterface::PlayerMove(dst_pos);
}

bool GPlayerMe::PreKillMonster()
{
	if (auto_lvl_up_skill_gem_by_default_)
	{
		auto lvl_up_skill_gem_cnt = GInterface::PressAllLvlUpSkillGem();
		if (lvl_up_skill_gem_cnt > 0)
		{
			//LOG_O(Log_trace) << "自动升级技能宝石数量：" << lvl_up_skill_gem_cnt;
		}
	}
	MyLuaThrdData::AutoUseUtilityFlask();
	GPassiveSkillResolver::GetMe().AutoLvlUpPassiveSkills();
	//改变脚本的命令，暂时写在这吧。
	LuasScriptEchoC::ChangeRunningScript(-1);
	return true;
}

void GPlayerMe::OnMonsterDied(pt_dword entity_id)
{
}

bool GPlayerMe::OnTraMapOver()
{
	if (on_tra_map_over_deep_ > 0)
		return false;
	++on_tra_map_over_deep_;
	BOOST_SCOPE_EXIT_ALL(&){
		--on_tra_map_over_deep_;
	};
	auto& gmap = GMap::GetMe();
	auto cur_map_areas_info = gmap.GetCurAreaInfo();
	if (!cur_map_areas_info)
		return false;
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return false;
	}
	auto& astar = G_AStar::GetMe();
	auto my_area_id = astar.GetAreaId(*my_pos);
	if (my_area_id < 0)
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	struct stHelper{
		GameObjBasePtrT		area_transition_;
		float				weight_;
	};
	GMapAreaShortestPathsCont<stHelper> map_area_transitions;
	auto area_names_name_list = MakeNameListRef(cur_map_areas_info->area_names_);
	for (auto& area_transition : remembered_objs_.GetObjs())
	{
		if (!area_names_name_list.IsIn(area_transition->GetCnName()))
			continue;
		stHelper tmp = { area_transition, area_transition->WeightDistance(*my_pos) };
		tmp.weight_ += (my_area_id == astar.GetAreaId(area_transition->GetBackupPos()) ? 0 : 1) * 1000;
		map_area_transitions.push_back(tmp);
	}
	if (map_area_transitions.empty())
	{
		assert(!"是不是没记录下传送门？");
		return false;
	}
	//改变排序方式的时候，一定要考虑下stHelper.weight_必须是没有变化的，否则会出莫名其妙的问题。
	std::sort(map_area_transitions.begin(), map_area_transitions.end(), [my_pos]
		(const stHelper& lhs, const stHelper& rhs){
		return lhs.weight_ < rhs.weight_;
	});
	//////////////////////////////////////////////////////////////////////////
	GMap::AreaStepsType area_steps;
	for (auto& v : map_area_transitions)
	{
		gmap.GenSortedAreaSteps(v.area_transition_, area_steps);
		if (!area_steps.empty())
			break;
	}
	if (area_steps.empty())
		return false;
	return DoMoveToTheAreaByStep(area_steps.front());
}

bool GPlayerMe::DoMoveToTheAreaByStep(const stAreaVertex& vertex)
{
	if (vertex.transitionable_)
	{
		assert(!vertex.transitionable_->GetOwner());
		LOG_O(Log_trace) << "移动到房间：" << vertex.transitionable_->GetCnName();
		return KillToOpenObj(vertex.transitionable_);
	}
	if (vertex.transitionable_pos_.AllLess(1))
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "移动到房间之先通过坐标移动：" << vertex.transitionable_pos_;
	if (!SmartMove(vertex.transitionable_pos_))
	{
		LOG_O(Log_debug) << "先移动到目标点失败：" << vertex.transitionable_pos_;
		return false;
	}
	GameObjMgrPtr obj_mgr_transition(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr_transition)
	{
		assert(false);
		return false;
	}
	if (!GWndExecSync([&obj_mgr_transition, this](){
		if (!obj_mgr_transition->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return false;
	}
	
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return false;
	}
	obj_mgr_transition->SortByPos(*my_pos);
	auto the_new_transition = obj_mgr_transition->GetFirstObj();
	if (!the_new_transition)
	{
		LOG_O(Log_debug) << "先移动到目标点之，移动到目标点了，但未发现门对象";
		return false;
	}
	const_cast<stAreaVertex&>(vertex).transitionable_ = the_new_transition;
	BOOST_SCOPE_EXIT_ALL(&the_new_transition){
		the_new_transition->ResetPos();
		the_new_transition->SetOwner(nullptr);
	};
	return KillToOpenObj(the_new_transition);
}

bool GPlayerMe::UseWayToTown()
{
	if (!GInventoryMgr::GetMe().UseItem(kObjName_PortalIdentify))
	{
		//LOG_O(Log_error) << "使用传送卷轴失败";
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	if (!TimerDo(50, 1000 * 60, [&obj_mgr](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		auto obj = obj_mgr->GetFirstObj();
		if (!obj)
			return false;
		return true;
	}))
	{
		LOG_O(Log_error) << "使用传送卷轴 没有发现传送门";
		assert(false);
		return false;
	}
	if (!KillToOpenObj(obj_mgr->GetFirstObj()))
	{
		assert(false);
		return false;
	}
	return true;
}

GameObjBasePtrT GPlayerMe::FindNearestMonsterObj(const std::string& obj_name)
{
	if (obj_name.empty())
	{
		assert(false); 
		return nullptr;
	}
	GameObjMgrPtr mgr(new GameObjMgr(kGOT_Monster));
	if (!mgr)
	{
		assert(false);
		return nullptr;
	}
	FilterGameObjAlive filter(mgr->GetFilter());
	if (!GWndExecSync([&mgr, this](){
		if (!mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return nullptr;
	}	
	auto pos = GetPos();
	if (!pos)
	{
		assert(false);
		return nullptr;
	}
	mgr->SortByWeightDistance(*pos);
	return mgr->FindByName(obj_name);
}

bool GPlayerMe::SmartMove(const stCD_VecInt& dst_pos)
{
	auto action = GA_Factory::GetMe().MakeMoveToNearly(dst_pos);
	if (!action)
	{
		assert(false);
		return false;
	}
	return action->Run() >= CA_IAction::kRR_Succeed;
}

GameObjBasePtrT GPlayerMe::MoveToTheObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const GameObjBasePtrT& obj)
{
	auto action = GA_Factory::GetMe().MakeMoveToObj(consumer, clear_mark_pos, obj);
	if (!action)
	{
		assert(false);
		return nullptr;
	}
	const auto& run_res = action->Run();
	if (!run_res)
	{
		assert(obj);
		LOG_O(Log_debug) << "移动到对象失败，对象名：" << obj->GetCnName();
		//assert(false);
		return nullptr;
	}
	return action->GetTheObj();
}

GameObjBasePtrT GPlayerMe::MoveToTheObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const std::string& obj_name, const GameObjMgrPtr& obj_mgr)
{
	auto action = GA_Factory::GetMe().MakeMoveToObj(consumer, clear_mark_pos, obj_name, obj_mgr);
	if (!action)
	{
		assert(false);
		return nullptr;
	}
	const auto& run_res = action->Run();
	if (!run_res)
	{
		assert(false);
		return nullptr;
	}
	return action->GetTheObj();
}

enOpenVisitorRes GPlayerMe::MoveToOpenObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const GameObjBasePtrT& obj)
{
	if (!obj)
	{
		assert(false);
		return kOVR_Error;
	}
	enOpenVisitorRes open_res = PreOpenObj(obj);
	if (open_res < kOVR_Succeed)
	{
		if (kOVR_Error == open_res)
			LOG_O(Log_error) << "移动并打开对象失败，对象名:" << obj->GetCnName();
		return open_res;
	}
	else if (kOVR_Opened == open_res)
		return open_res;
	assert(!is_openning_);
	is_openning_ = true;
	BOOST_SCOPE_EXIT_ALL(this){
		is_openning_ = false;
	};
	
	for (int i = 0; i < 3; ++i)
	{
		//DummySleep(40);
		//必须使用obj，不能使用返回的新对象。
		if (!MoveToTheObj(consumer, clear_mark_pos, obj))
		{
			LOG_O(Log_debug) << "移动到对象失败：" << obj->GetCnName();
			//assert(false);
			return kOVR_Error;
		}
		//延迟不能随便去掉，否则太快，服务器不认可
		//DummySleep(100);
		if (!obj->DoOpen())
		{
			assert(false);
			return kOVR_Error;
		}
		if (kOVR_NotVisitAgain != open_res)
		{
			open_res = kOVR_Succeed;
			GOpenVisitor visitor;
			if (!obj->OpenVisit(visitor, false, open_res))
			{
				//assert(!"是没有正常打开对象吗");
				continue;
			}
			if (kOVR_Succeed != open_res)
			{
				assert(false);
				return kOVR_Error;
			}
		}
		return open_res;
	}
	LOG_O(Log_debug) << "等待打开对象失败：" << obj->GetCnName();
	return kOVR_Error;
}

enOpenVisitorRes GPlayerMe::MoveToOpenObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const std::string& obj_name, const GameObjMgrPtr& obj_mgr)
{
	auto the_obj = MoveToTheObj(consumer, clear_mark_pos, obj_name, obj_mgr);
	if (!the_obj)
	{
		assert(false);
		return kOVR_Error;
	}
	return MoveToOpenObj(consumer, false, the_obj);
}

bool GPlayerMe::KillToOpenObj(const GameObjBasePtrT& dst_obj)
{
	if (!dst_obj)
	{
		assert(false);
		return false;
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto producer = factory.MakeProducer(consumer);
	if (!producer)
	{
		assert(false);
		return false;
	}
	auto open_objs = factory.MakeOpenObjs(consumer);
	auto kill_in_range = factory.MakeKillInRange(consumer, open_objs);
	if (!kill_in_range)
	{
		assert(false);
		return false;
	}
	producer->Add(kill_in_range->Tester());
	if (open_objs)
	{
		open_objs->AddBlackNameList(dst_obj->GetGameId());
		producer->Add(open_objs->Tester());
	}

	GA_SmartConsumerWeakT weak_consumer = consumer;
	auto action_move_to_open = factory.MakeLambda([this, dst_obj, weak_consumer](){
		if (MoveToOpenObj(weak_consumer.lock(), false, dst_obj) >= kOVR_Succeed)
			return CA_IAction::kRR_Succeed;
		return CA_IAction::kRR_Failed;
	});
	if (!action_move_to_open)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_move_to_open, producer);
	const auto& run_res = consumer->Run();
	if (!run_res)
	{
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillToOpenObj(const std::string& dst_name)
{
	if (dst_name.empty())
	{
		assert(false);
		return false;
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto producer = factory.MakeProducer(consumer);
	if (!producer)
	{
		assert(false);
		return false;
	}
	
	auto open_objs = factory.MakeOpenObjs(consumer);
	auto kill_in_range = factory.MakeKillInRange(consumer, open_objs);
	if (!kill_in_range)
	{
		assert(false);
		return false;
	}
	producer->Add(kill_in_range->Tester());
	//位置不能变
	GFilterGameObjPtr filter_name;
	if (open_objs)
	{
		auto obj_mgr = open_objs->GetObjMgr();
		if (!obj_mgr)
		{
			assert(false);
			return false;
		}
		filter_name.reset(new FilterGameObjNotName(obj_mgr->GetFilter(), dst_name));
		producer->Add(open_objs->Tester());
	}
	GA_SmartConsumerWeakT weak_consumer = consumer;
	auto action_move_to_open = factory.MakeLambda([this, dst_name, weak_consumer](){
		if (MoveToOpenObj(weak_consumer.lock(), true, dst_name) >= kOVR_Succeed)
			return CA_IAction::kRR_Succeed;
		return CA_IAction::kRR_Failed;
	});
	if (!action_move_to_open)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_move_to_open, producer);
	auto run_res = consumer->Run();
	if (!run_res)
	{
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillToTheObj(const GameObjBasePtrT& dst_obj)
{
	if (!dst_obj)
	{
		assert(false);
		return false;
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto producer = factory.MakeProducer(consumer);
	if (!producer)
	{
		assert(false);
		return false;
	}
	
	auto open_objs = factory.MakeOpenObjs(consumer);
	auto kill_in_range = factory.MakeKillInRange(consumer, open_objs);
	if (!kill_in_range)
	{
		assert(false);
		return false;
	}
	producer->Add(kill_in_range->Tester());
	if (open_objs)
	{
		open_objs->AddBlackNameList(dst_obj->GetGameId());
		producer->Add(open_objs->Tester());
	}

	auto action_move = factory.MakeMoveToObj(consumer, false, dst_obj);
	if (!action_move)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_move, producer);
	const auto& run_res = consumer->Run();
	if (!run_res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillToTheObj(const std::string& dst_name)
{
	if (dst_name.empty())
	{
		assert(false);
		return false;
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto producer = factory.MakeProducer(consumer);
	if (!producer)
	{
		assert(false);
		return false;
	}

	auto open_objs = factory.MakeOpenObjs(consumer);
	//位置不能变
	GFilterGameObjPtr filter_name;
	if (open_objs)
	{
		auto obj_mgr = open_objs->GetObjMgr();
		if (!obj_mgr)
		{
			assert(false);
			return false;
		}
		filter_name.reset(new FilterGameObjNotName(obj_mgr->GetFilter(), dst_name));
		producer->Add(open_objs->Tester());
	}
	auto tester_name = factory.MakeTesterByName(dst_name);
	if (!tester_name)
	{
		assert(false);
		return false;
	}
	auto action_kill = factory.MakeKillUntil(consumer, true, tester_name);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, producer);
	consumer->AddTester(tester_name);
	tester_name->SetParent(consumer);
	auto run_res = consumer->Run();
	if (run_res != tester_name)
	{
		assert(false);
		return false;
	}
	auto the_dst_obj = tester_name->GetTheObj();
	if (!the_dst_obj)
	{
		assert(false);
		return false;
	}
	consumer->clear();
	action_kill = factory.MakeMoveToObj(consumer, false, the_dst_obj);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	auto kill_in_range = factory.MakeKillInRange(consumer, open_objs);
	if (!kill_in_range)
	{
		assert(false);
		return false;
	}
	producer->Add(kill_in_range->Tester());

	consumer->AddEnsure(action_kill, producer);
	run_res = consumer->Run();
	if (!run_res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillToKillTheObj(const std::string& dst_name)
{
	if (dst_name.empty())
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Monster));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr->RebuildAll())
	{
		assert(false);
		return false;
	}
	auto the_dst_obj = obj_mgr->FindByName(dst_name);
	if (the_dst_obj)
	{
		if (the_dst_obj->GetCurHp() <= 0)
		{
			FilterGameObjAlive filter_alive(obj_mgr->GetFilter());
			if (!obj_mgr->RebuildAll())
			{
				assert(false);
				return false;
			}
			the_dst_obj = obj_mgr->FindByName(dst_name);
			if (!the_dst_obj)
			{
				LOG_O(Log_warning) << "杀到杀死目标之怪物已经死亡了：" << dst_name;
				return true;
			}
		}
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto open_objs = factory.MakeOpenObjs(consumer);
	CA_ActionPtr open_objs_tester;
	if (open_objs)
		open_objs_tester = open_objs->Tester();
	FilterGameObjAlive filter_alive(obj_mgr->GetFilter());
	FilterGameObjHittable filter_hittable(obj_mgr->GetFilter());
	auto tester_name = factory.MakeTesterByName(dst_name, obj_mgr);
	if (!tester_name)
	{
		assert(false);
		return false;
	}
	auto action_kill = factory.MakeKillUntil(consumer, true, tester_name);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, open_objs_tester);
	consumer->AddTester(tester_name);
	tester_name->SetParent(consumer);
	auto run_res = consumer->Run();
	if (run_res != tester_name)
	{
		assert(false);
		return false;
	}
	the_dst_obj = tester_name->GetTheObj();
	if (!the_dst_obj)
	{
		assert(false);
		return false;
	}

	consumer->clear();
	action_kill = factory.MakeKillMonster(the_dst_obj);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, open_objs_tester);
	run_res = consumer->Run();
	if (!run_res)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillToTheRoom(const std::string& dst_name)
{
	if (dst_name.empty())
	{
		assert(false);
		return false;
	}
	auto& gmap = GMap::GetMe();
	auto vertex = gmap.FindAreaByName(dst_name);
	if (!vertex.transitionable_)
		return KillToOpenObj(dst_name);
	vertex = gmap.FindConnectedRoom(vertex);
	if (!vertex.IsValid())
	{
		assert(false);
		return false;
	}
	if (!Update())
	{
		assert(false);
		return false;
	}
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return false;
	}
	GMap::AreaStepsType steps;
	if (!gmap.GenAreaSteps(*my_pos, vertex.GetValidPos(), steps))
		return false;
	if (steps.empty())
		return true;
	for (auto& step : steps)
	{
		if (!DoMoveToTheAreaByStep(step))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GPlayerMe::KillToKillNpc(const std::string& dst_name)
{
	if (dst_name.empty())
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Monster));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr->RebuildAll())
	{
		assert(false);
		return false;
	}
	auto the_dst_obj = obj_mgr->FindByName(dst_name);
	if (the_dst_obj)
	{
		if (the_dst_obj->GetCurHp() <= 0)
		{
			FilterGameObjAlive filter_alive(obj_mgr->GetFilter());
			if (!obj_mgr->RebuildAll())
			{
				assert(false);
				return false;
			}
			the_dst_obj = obj_mgr->FindByName(dst_name);
			if (!the_dst_obj)
			{
				LOG_O(Log_warning) << "杀到杀死NPC怪之NPC怪已经死亡了：" << dst_name;
				return true;
			}
		}
	}
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto open_objs = factory.MakeOpenObjs(consumer);
	//位置不能变
	GFilterGameObjPtr filter_name;
	CA_ActionPtr open_objs_tester;
	if (open_objs)
	{
		auto obj_mgr = open_objs->GetObjMgr();
		if (!obj_mgr)
		{
			assert(false);
			return false;
		}
		filter_name.reset(new FilterGameObjNotName(obj_mgr->GetFilter(), dst_name));
		open_objs_tester = open_objs->Tester();
	}
	auto kill_npc_monster = factory.MakeKillNpcMonster(consumer, dst_name);
	if (!kill_npc_monster)
	{
		assert(false);
		return false;
	}
	auto tester_look_die = factory.MakeTesterLookMonsterDie(dst_name);
	if (!tester_look_die)
	{
		assert(false);
		return false;
	}
	auto action_kill = factory.MakeKillUntil(consumer, true, tester_look_die);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, open_objs_tester);
	consumer->AddTester(tester_look_die);
	consumer->AddTester(kill_npc_monster->Tester());
	tester_look_die->SetParent(consumer);
	const auto& run_res = consumer->Run();
	if (run_res != tester_look_die)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::KillAllMonsters()
{
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	auto open_objs = factory.MakeOpenObjs(consumer);
	CA_ActionPtr open_objs_tester;
	if (open_objs)
		open_objs_tester = open_objs->Tester();
	auto action_kill = factory.MakeKillUntil(consumer, false, nullptr);
	if (!action_kill)
	{
		assert(false);
		return false;
	}
	consumer->AddEnsure(action_kill, open_objs_tester);
	return consumer->Run();
}

enOpenVisitorRes GPlayerMe::PreOpenObj(const GameObjBasePtrT& obj)
{
	if (!obj)
	{
		assert(false);
		return kOVR_Error;
	}
	if (is_openning_)
		return kOVR_Openning;
	enOpenVisitorRes open_res = kOVR_Succeed;
	GOpenVisitor visitor;
	if (!obj->OpenVisit(visitor, true, open_res))
	{
		assert(false);
		return kOVR_Error;
	}
	return open_res;
}
