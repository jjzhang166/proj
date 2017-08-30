#include "stdafx.h"
#include "GClass.h"
#include <Net/IoTimer.h>
#include <Common/EnsureCleanup.h>
#include "GameObjMgr.h"
#include "GPlayer.h"
#include "GNpc.h"
#include "GMap.h"
#include "GItem.h"
#include <Scmd.h>
#include <Public/Script/LuaScript.h>

using namespace GClass;
GItemContT GAutoUpDateGem::all_gem_;
bool GOpenVisitor::VisitNpc(const GNpc& npc, bool is_pre_open, enOpenVisitorRes& open_res)
{
	const auto& name = npc.GetCnName();
	if (name.empty())
	{
		assert(false);
		return false;
	}
	bool is_opened = false;
	auto npc_name = GbkToWideChar(name);
	if (is_pre_open)
	{
		auto& gpm = GPlayerMe::GetMe();
		if (!gpm.CloseAllPanel(false))
		{
			assert(false);
			return false;
		}
		if (GInterface::EqualNpcPanelTitle(is_opened, npc_name))
		{
			open_res = kOVR_Opened;
			return true;
		}
		if (is_opened)
		{
			if (!gpm.CloseNpcPanel())
			{
				assert(false);
				return false;
			}
		}
		return true;
	}
	bool other_npc_panel_open = false;
	if (!TimerDo(40, 1000 * 30, [&is_opened, &npc_name, &other_npc_panel_open](){
		if (GInterface::EqualNpcPanelTitle(is_opened, npc_name))
			return true;
		if (GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown) > CD_kInGameNormalShowDlgMaxCnt)
			return true;
		if (is_opened)
		{
			other_npc_panel_open = true;
			return true;
		}
		return false;
	}))
	{
		assert(false);
		return false;
	}
	if (is_opened && other_npc_panel_open)
		return false;
	return true;
}

bool GOpenVisitor::VisitMainSubPanelNormal(const GameObjBase& obj, bool is_pre_open, enOpenVisitorRes& open_res, pt_dword vtbl)
{
	if (is_pre_open)
	{
		if (GInterface::MainSubUiShowState((pt_pcvoid)vtbl, kTUSF_Shown))
		{
			open_res = kOVR_Opened;
			return true;
		}
		auto& gpm = GPlayerMe::GetMe();
		if (!gpm.CloseAllPanel())
		{
			assert(false);
			return false;
		}
		return true;
	}
	if (!TimerDo(40, 1000 * 30, [vtbl](){
		if (GInterface::MainSubUiShowState((pt_pcvoid)vtbl, kTUSF_Shown))
			return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GOpenVisitor::WaitCrossMap(const GClass::stLastMapInfo& last_map_info, GAreaTransition* last_area_transition)
{
	IoTimer::lock_guard locker;
	auto& map = GMap::GetMe();
	if (!TimerDo(800, 1000 * 60, [&map, &last_map_info](){
		if (!map.Update())
		{
			assert(false);
			return true;
		}
		return map.HasChangedMap(last_map_info);
	}))
	{
		assert(false);
		return false;
	}
	if (!last_area_transition)
		return true;
	if (last_map_info.last_map_name_ != map.GetMapName())
		return true;
	last_area_transition->ResetPos();
	auto& astar = G_AStar::GetMe();
	auto last_area_id = last_map_info.last_area_id_;
	if (last_area_id < 0)
	{
		assert(false);
		last_area_id = last_area_transition->GetAreaId();
		if (last_area_id < 0)
		{
			assert(false);
			return false;
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	LOG_O(Log_debug) << "杀到房间-------------" << last_area_transition->GetCnName() << ",id:" << last_area_transition->GetGameId();
	GameObjMgrPtr obj_mgr_transition(new GameObjMgr(kGOT_AreaTransition));
	FilterGameObjNotTheId filter_id(obj_mgr_transition->GetFilter(), last_area_transition->GetGameId());
	if (!obj_mgr_transition)
	{
		assert(false);
		return false;
	}
	auto last_name = last_area_transition->GetCnName();
	auto it = std::find_if(begin(kRoomConnectFilter), end(kRoomConnectFilter), [&last_name](const std::string& str){ return last_name == str; });
	if (it == end(kRoomConnectFilter))
	{
		//进入门后，有时候门没有刷出来，同步等待一下
		TimerDo(800, 1000 * 7, [&obj_mgr_transition, &gpm, player_pos](){
			if (!obj_mgr_transition->RebuildAll())
			{
				assert(false);
				return false;
			}
			obj_mgr_transition->SortByPos(*player_pos);
			auto the_new_transition = obj_mgr_transition->GetFirstObj();
			if (!the_new_transition)
				return false;
			return !!the_new_transition && the_new_transition->GetAreaId() == gpm.GetAreaId();
		});
	}
	else
	{
		if (!obj_mgr_transition->RebuildAll())
		{
			assert(false);
			return false;
		}
	}
	obj_mgr_transition->SortByPos(*player_pos);
	auto the_new_transition = obj_mgr_transition->GetFirstObj();
	AreaVertexGuess vertex_rhs;
	int new_area_id = gpm.GetAreaId();
	assert(new_area_id >= 0);
	if (the_new_transition && (the_new_transition->GetAreaId() == new_area_id || new_area_id < 0 ))
	{
		the_new_transition->ResetPos();
		if (new_area_id < 0)
		{
			assert(false);
			new_area_id = the_new_transition->GetAreaId();
		}
		vertex_rhs.SetTransitionable(the_new_transition);
		vertex_rhs.SetPos(*player_pos);
	}
	else
	{
		new_area_id = gpm.GetAreaId();
		vertex_rhs.SetPos(*player_pos);
	}
	if (new_area_id < 0)
	{
		assert(false);
		return false;
	}
	if (new_area_id == last_area_id)
	{
		assert(false);
		return false;
	}
	AreaVertexGuess vertex_lhs;
	auto tra_guess = GMapTraMgr::GetMe().GetTraGuess();
	if (tra_guess.FindAreaById(last_area_transition->GetGameId()))
		vertex_lhs.SetAreaState(kAPS_NotPass);
	auto last_new_rhs = tra_guess.FindAreaById(new_area_id);
	if (last_new_rhs)
	{
		vertex_rhs.SetAreaState(kAPS_NotPass);
		auto last_transitionable = last_new_rhs->GetTransitionable();
		vertex_rhs.SetTransitionable(last_transitionable);
	}
		
	vertex_lhs.SetTransitionable(last_area_transition->shared_from_this());
	vertex_lhs.SetAreaId(last_area_id);
	vertex_lhs.SetPos(last_map_info.last_pos_);
	vertex_rhs.SetAreaId(new_area_id);
	GMapTraMgr::GetMe().AddTransitionableConnect(vertex_lhs, vertex_rhs);
	return true;
}

bool GOpenVisitor::VisitAreaTransition(GAreaTransition& area_transition, bool is_pre_open, enOpenVisitorRes& open_res)
{
	if (is_pre_open)
	{
		area_transition.RememberLastMapInfo();
		return true;
	}	
	return WaitCrossMap(area_transition.GetLastMapInfo(), &area_transition);
}

bool GOpenVisitor::VisitWorldItem(const GWorldItem& world_item, bool is_pre_open, enOpenVisitorRes& open_res)
{
	const auto& name = world_item.GetCnName();
	if (name.empty())
	{
		assert(false);
		return false;
	}

	auto& inventory = GInventoryMgr::GetMe();
	if (is_pre_open)
	{
		auto item = world_item.GetWorldItem();
		if (!item)
		{
			assert(false);
			return false;
		}
		if (!inventory.NeedTakeupTheDropItem(item))
		{
			if (!MyLuaThrdData::GetPickUpFilter().DoFilt(*item))
			{
				open_res = kOVR_NoNeedOpen;
				return true;
			}
		}
		return true;
	}
	auto item = world_item.GetWorldItem();
	if (!item)
	{
		assert(false);
		return false;
	}
	LOG_O(Log_debug) << "拾取物品:" << item->GetItemName();
	auto main_bag = inventory.GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	auto pos_info = main_bag->FindFreePosBySize(item->GetItemSize(), main_bag->GetRetainFilterPos(item.get()));
	if (pos_info.AllLessZero())
	{
		if (!main_bag->AdjustPack())
		{
			assert(false);
			return false;
		}
		auto pos_info = main_bag->FindFreePosBySize(item->GetItemSize(), main_bag->GetRetainFilterPos(item.get()));
		if (pos_info.AllLessZero())
		{
			LuasScriptEchoC::TriggerEvent(kEventName_BagIsFull);
			if (!inventory.AutoClearMainBagBySize(item->GetItemSize()))
			{
				LOG_O(Log_error) << "拾取物品之背包没有可存放的空间，物品名称:" << name;
				open_res = kOVR_NoNeedOpen;
				return true;
			}
		}
	}
	auto id = item->GetItemId();
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_WorldItem));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	if (!TimerDo(40, 1000 * 2, [&inventory, &world_item, &obj_mgr, &id](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		return obj_mgr->FindById(id) == nullptr;
	}))
	{
		LOG_O(Log_debug) << "捡取物品失败，名字:" << world_item.GetCnName();
		return false;
	}
	if (item->IsEquip())
		MyLuaThrdData::AutoWearSomeEquips();
	else if (item->IsGem())
		MyLuaThrdData::AutoWearSomeGem();
	return true;
}

bool GOpenVisitor::VisitChestItem(const GChest& chest, bool is_pre_open, enOpenVisitorRes& open_res)
{
	if (is_pre_open)
	{
		if (chest.IsOpened())
			open_res = kOVR_Opened;
		return true;
	}
	assert(!chest.GetCnName().empty());
#ifdef _DEBUG
	/*auto& gpm = GPlayerMe::GetMe();
	auto pos = gpm.GetPos();
	if (!pos)
	{
		assert(false);
		return false;
	}
	auto world_item_pos = world_item.GetPos();
	if (!world_item_pos)
	{
		assert(false);
		return false;
	}
	auto distance = pos->Distance(*world_item.GetPos());
	LOG_O(Log_debug) << "开箱子:" << world_item.GetCnName() << ",Id:" << world_item.GetGameId() << ",距离:" << distance;*/
#endif
	auto id = chest.GetGameId();
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Chest));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	if (!TimerDo(40, 1000 * 2, [id, &obj_mgr
	](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		auto chest_obj = obj_mgr->FindById(id);
		if (!chest_obj)
			return true;
		return chest_obj->IsOpened();
	}))
	{
#ifdef _DEBUG
		//LOG_O(Log_debug) << "开箱子失败:" << world_item.GetCnName() << ",Id:" << world_item.GetGameId() << ",距离:" << distance;
#endif
		//assert(false);
		return false;
	}
	return true;
}

bool GOpenVisitor::VisitTranstionable(const GTranstionable& transitionable, bool is_pre_open, enOpenVisitorRes& open_res)
{
	const auto& name = transitionable.GetCnName();
	if (name.empty())
	{
		assert(false);
		return false;
	}
	if (is_pre_open)
	{
		/*if (transitionable.GetOpenState())
			open_res = kOVR_Opened;*/
		return true;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Transitonable));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	auto id = transitionable.GetGameId();
	if (!TimerDo(40, 1000 * 3, [&obj_mgr, &id](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		return obj_mgr->FindById(id) == nullptr;
	}))
	{
		LOG_O(Log_error) << "等待打开门失败：" << transitionable.GetCnName();
		//assert(false);
		return false;
	}
	GPlayerMe::GetMe().EraseRememberId(transitionable);
	return true;
}

GClass::stLastMapInfo::stLastMapInfo()
{
	last_world_ = nullptr;
	last_world_area_ = nullptr;
	last_area_id_ = -1;
}

void GClass::stLastMapInfo::clear()
{
	last_world_ = nullptr;
	last_world_area_ = nullptr;
	last_map_name_.clear();
	last_area_id_ = -1;
}

namespace GClass{
	bool operator==(const stLastMapInfo& lhs, const stLastMapInfo& rhs){
		if (lhs.last_area_id_ < 0 || rhs.last_area_id_ < 0)
			return false;
		return lhs.last_area_id_ == rhs.last_area_id_
			&& lhs.last_map_size_ == rhs.last_map_size_
			&& lhs.last_world_ == rhs.last_world_
			&& lhs.last_world_area_ == rhs.last_world_area_ && lhs.last_map_name_ == rhs.last_map_name_;
	}

	bool operator!=(const stLastMapInfo& lhs, const stLastMapInfo& rhs){
		return !(lhs == rhs);
	}

	pt_dword stBasePro::Checksum() const
	{
		pt_dword checksum = level_;
		checksum += power_ << 5;
		checksum += smart_ << 8;
		checksum += brain_ << 11;
		return checksum;
	}
}

//////////////////////////////////////////////////////////////////////////
class _GHNTL_Continue : public GHandleNpcTalkLabel::Handler{
public:
	enTalkToNpcRes PreHandle(const std::wstring& talk_label) override{
		/*auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(false);
		if (talk_res < kTTNR_Succeed)
			return talk_res;*/
		if (!GInterface::NpcPanelSubUiShowState(kNPIC_TalkPanel, kTUSF_Shown))
			return kTTNR_NotNeedHandle;
		GInterface::TalkToNpc(kNPIC_TalkPanel, kNTL_Cotinue, true);
		return kTTNR_NotNeedHandle;
	}
};

class _GHNTL_Bye : public GHandleNpcTalkLabel::Handler{
public:
	enTalkToNpcRes PreHandle(const std::wstring& talk_label) override{
		auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(false);
		if (talk_res < kTTNR_Succeed)
			return talk_res;
		if (!GInterface::NpcPanelSubUiShowState(kNPIC_Normal, kTUSF_Shown))
			return kTTNR_NotNeedHandle;
		GInterface::TalkToNpc(kNPIC_Normal, kNTL_Bye, true);
		return kTTNR_NotNeedHandle;
	}
	enTalkToNpcRes Handle(const std::wstring& talk_label) override{
		return kTTNR_Succeed;
	}
};

//////////////////////////////////////////////////////////////////////////
GHandleNpcTalkLabel::Handler g_handle_lable_default;
_GHNTL_Continue	g_handle_lable_continue;
_GHNTL_Bye	g_handle_lable_bye;
//////////////////////////////////////////////////////////////////////////
GHandleNpcTalkLabel::GHandleNpcTalkLabel()
{
	handlers_[kNTL_Cotinue] = &g_handle_lable_continue;
	handlers_[kNTL_Bye] = &g_handle_lable_bye;
}

enTalkToNpcRes GHandleNpcTalkLabel::PreHandleLabel(const std::wstring& talk_label)
{
	auto it = handlers_.find(talk_label);
	if (it == handlers_.end())
	{
		LOG_O(Log_debug) << "没有该[" << talk_label << "]npc talk label，默认去处理";
		return g_handle_lable_default.PreHandle(talk_label);
	}
	return it->second->PreHandle(talk_label);
}

enTalkToNpcRes GHandleNpcTalkLabel::HandleLabel(const std::wstring& talk_label)
{
	auto it = handlers_.find(talk_label);
	if (it == handlers_.end())
	{
		return g_handle_lable_default.Handle(talk_label);
	}
	return it->second->Handle(talk_label);
}

enTalkToNpcRes GHandleNpcTalkLabel::HandleCotinueImpl(bool wait_menu_panel)
{
	if (GInterface::NpcPanelSubUiShowState(kNPIC_TalkPanel, kTUSF_Shown))
	{
		while (true)
		{
			auto talk_res = GInterface::TalkToNpc(kNPIC_TalkPanel, kNTL_Cotinue, false);
			if (talk_res < kTTNR_Succeed)
				return talk_res;
			bool is_break = false;
			if (!TimerDo(50, 1000 * 60, [&is_break](){
				if (GInterface::NpcPanelSubUiShowState(kNPIC_TalkPanel, kTUSF_Shown))
					return true;
				if (GInterface::NpcPanelSubUiShowState(kNPIC_Normal, kTUSF_Shown))
				{
					is_break = true;
					return true;
				}
				if (GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown) > CD_kInGameNormalShowDlgMaxCnt)
				{
					is_break = true;
					return true;
				}
				return false;
			}))
			{
				assert(false);
				return kTTNR_Error;
			}
			if (is_break)
				break;
		}
	}
	if (!wait_menu_panel)
		return kTTNR_Succeed;
	if (!TimerDo(50, 1000 * 60, [](){
		if (GInterface::NpcPanelSubUiShowState(kNPIC_Normal, kTUSF_Shown))
			return true;
		if (GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown) > CD_kInGameNormalShowDlgMaxCnt)
			return true;
		return false;
	}))
	{
		assert(false);
		return kTTNR_Error;
	}
	return kTTNR_Succeed;
}

enTalkToNpcRes GHandleNpcTalkLabel::Handler::PreHandle(const std::wstring& talk_label)
{
	auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(true);
	if (talk_res < kTTNR_Succeed && talk_res != kTTNR_HaveNotTheLable)
	{
		assert(false);
		return talk_res;
	}
	talk_res = GInterface::TalkToNpc(kNPIC_Normal, talk_label, false);
	if (kTTNR_HaveNotTheLable == talk_res)
	{
		LOG_O(Log_error) << "对话NPC之没有该标签：" << talk_label;
	}
	return talk_res;
}

enTalkToNpcRes GHandleNpcTalkLabel::Handler::Handle(const std::wstring& talk_label)
{
	bool is_ret = true;
	if (!TimerDo(50, 1000 * 60, [&is_ret](){
		if (GInterface::NpcPanelSubUiShowState(kNPIC_TalkPanel, kTUSF_Shown))
		{
			is_ret = false;
			return true;
		}
		if (GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown) > CD_kInGameNormalShowDlgMaxCnt)
			return true;
		return false;
	}))
	{
		assert(false);
		return kTTNR_Error;
	}
	if (is_ret)
		return kTTNR_Succeed;
	auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(true);
	if (talk_res < kTTNR_Succeed && talk_res != kTTNR_HaveNotTheLable)
	{
		assert(false);
		return talk_res;
	}
	return kTTNR_Succeed;
}

GAutoEatHpMp::GAutoEatHpMp(float hp_rate, float mp_rate)
{
	last_hp_rate_  = 0;
	last_hp_speed_ = 0;
	last_mp_rate_ = 0;
	last_mp_speed_ = 0;
	hp_rate_ = hp_rate;
	mp_rate_ = mp_rate;
} 

bool GAutoEatHpMp::CalcHpIndex(int& hp_index)
{
	auto& gmp = GPlayerMe::GetMe();
	auto cur_hp = gmp.GetCurHp();
	auto max_hp = gmp.GetMaxHp();
	if (cur_hp >= max_hp)
		return false;
	if (max_hp <= 0)
	{
		assert(false);
		return false;
	}
	auto cur_rate_hp = (float)cur_hp / (float)max_hp;
	if (cur_rate_hp >= hp_rate_)
		return false;
	static TimeDuration td(800);
	if (td.CalcLeftDuration() > 0)
		return false;
	td.UpdateTime();
	return CalcIdxImpl(last_hp_rate_, last_hp_speed_, cur_rate_hp, kFT_Hp, hp_index);
}

bool GAutoEatHpMp::CalcMpIndex(int& mp_index)
{
	auto& gmp = GPlayerMe::GetMe();
	auto cur_mp = gmp.GetCurMp();
	auto max_mp = gmp.GetMaxMp();
	if (cur_mp >= max_mp)
		return false;
	if (max_mp <= 0)
	{
		assert(false);
		return false;
	}
	auto cur_rate_mp = (float)cur_mp / (float)max_mp;
	if (cur_rate_mp >= mp_rate_)
		return false;
	static TimeDuration td(2000);
	if (td.CalcLeftDuration() > 0)
		return false;
	td.UpdateTime();
	return CalcIdxImpl(last_mp_rate_, last_mp_speed_, cur_rate_mp, kFT_Mp, mp_index);
}

bool GAutoEatHpMp::CalcIdxImpl(float& last_rate, int& last_speed, float cur_rate, enCD_FlaskType flask_type, int& out_index)
{
	auto& inventory = GInventoryMgr::GetMe();
	auto flask_bag = inventory.GetPackBase(kIT_Flasks);
	if (!flask_bag)
	{
		assert(false);
		return false;
	}
	auto speed_cont = flask_bag->GetFlaskSpeedCont();
	if (!speed_cont)
	{
		assert(false);
		return false;
	}
	auto it = speed_cont->find(flask_type);
	if (it == speed_cont->end())
	{
		//LOG_O(Log_error) << "没有可补充的药剂了:"<<flask_type;
		return false;
	}
	auto& set = it->second;
	if (set.empty())
	{
		LOG_O(Log_error) << "没有可以补充的药剂了:"<<flask_type;
		return false;
	}

	//上次补血不足 这次选个大瓶的
	if (cur_rate <= last_rate)
	{
		for (auto& v : set)
		{
			out_index = v.index_;
			if (last_speed < v.speed_)
			{
				out_index = v.index_;
				last_speed = v.speed_;
				break;
			}
		}
	}
	else
	{
		out_index = set.begin()->index_;
		last_speed = set.begin()->speed_;
	}

	last_rate = cur_rate;
	return true;
}

void GAutoEatHpMp::SetRateInfo(float hp_rate, float mp_rate)
{
	assert(hp_rate > 0 && hp_rate < 1);
	assert(mp_rate > 0 && mp_rate < 1);
	hp_rate_ = hp_rate;
	mp_rate_ = mp_rate;
}

bool GAutoEatHpMp::CalcUtilityIndex(int& utility_index)
{
	static TimeDuration td(1000);
	if (td.CalcLeftDuration() > 0)
		return false;
	td.UpdateTime();
	float tmp_last = 0.9f;
	float tmp_rate = 1.0f;
	int  tmp_last_rete=1;
	return CalcIdxImpl(tmp_last, tmp_last_rete, tmp_rate, kFT_Utility, utility_index);
}

void GAutoUpDateGem::AddAutoGemLink(const std::string& main_gem, pt_dword min_lvl, pt_dword max_lvl, const luabind::object& tbl)
{
	using namespace luabind;
	if (main_gem.empty())
		return;
	if (type(tbl) != LUA_TTABLE)
		return;
	stMainGemInfo tmp;
	tmp.max_lvl_ = max_lvl;
	tmp.min_lvl_ = min_lvl;
	tmp.main_Gem_ = main_gem;
	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TSTRING)
		{
			LOG_O(Log_error) << "增加宝石连接之参数必须是宝石名字";
			assert(false);
			continue;
		}
		tmp.assist_gem_.push_back(LuaObjCast(v,kEmptyStr));
	}
	map_gem_.push_back(tmp);
	return;
}

void GAutoUpDateGem::ClearAllGemLink()
{
	map_gem_.clear();
}

GAutoUpDateGem::GAutoUpDateGem()
{

}
bool GAutoUpDateGem::CalcNeedLinkGem(GemLinkCnt& vec_link)
{
	//GItemContT all_gem;
	all_gem_.clear();
	auto& inventory = GInventoryMgr::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	inventory.GetAllPackGem(all_gem_);
	if (all_gem_.empty())
		return false;
	for (auto it = all_gem_.begin(); it != all_gem_.end();)
	{
		if (!(*it)->CanWear())
		{
			it = all_gem_.erase(it);
			continue;
		}
		++it;
	}
	if (all_gem_.empty())
	{
		LOG_O(Log_debug) << "没有要镶嵌的宝石";
		return false;
	}
	auto role_lvl = gpm.GetRoleLvl();
	for (auto& i : map_gem_)
	{
		if (role_lvl < i.min_lvl_ || role_lvl > i.max_lvl_)
			continue;
		auto it = std::find_if(all_gem_.begin(), all_gem_.end(), [&i](const GItemBasePtr& gem){return gem->GetItemName() == i.main_Gem_; });
		if (it == all_gem_.end())
			continue;
		std::vector<stGemInfo>  tmp_vec;
		stGemInfo tmp_main_gem;
		tmp_main_gem.gem_color_ = (*it)->GetItemGemColor();
		tmp_main_gem.gem_name_.assign((*it)->GetItemName());
		tmp_main_gem.gem_main_ = true;
		tmp_vec.push_back(tmp_main_gem);
		//all_gem.erase(it);
		if (!i.assist_gem_.empty())
		{
			for (auto& v : i.assist_gem_)
			{
				auto it_assist = std::find_if(all_gem_.begin(), all_gem_.end(), [v](const GItemBasePtr& gem){return gem->GetItemName() == v; });
				if (it_assist == all_gem_.end())
					continue;
				tmp_main_gem.gem_color_ = (*it_assist)->GetItemGemColor();
				tmp_main_gem.gem_name_.assign((*it_assist)->GetItemName());
				tmp_main_gem.gem_main_ = false;
				//all_gem.erase(it_assist);
				tmp_vec.push_back(tmp_main_gem);
			}
		}
		vec_link.push_back(tmp_vec);
	}
	if (!vec_link.empty())
		std::sort(vec_link.begin(), vec_link.end(), [](const GemConT& lhs, const GemConT& rhs){return lhs.size() > rhs.size(); });
	return true;
}

bool GAutoUpDateGem::CalcAllGemPos(GemDstPos& gem_pos)
{
	//计算出需要连接的宝石
	GemLinkCnt  vec_link;
	CalcNeedLinkGem(vec_link);
	if (vec_link.empty())
	{
		return false;
	}

	GClass::VcSlotInfoConT  all_solt;
	auto& inventory = GInventoryMgr::GetMe();
	inventory.GetAllGemSlotInfo(all_solt);
	if (all_solt.empty())
	{
		LOG_O(Log_debug) << "身上装备没有带宝石槽的装备";
		return false;
	}
	for (auto& gem_link : vec_link)
		FindSetCanWearEquip(all_solt, gem_link, gem_pos);
	return true;
}

void GAutoUpDateGem::FindSetCanWearEquip(GClass::VcSlotInfoConT&  solt_info, std::vector<GClass::stGemInfo> gem, GemDstPos& gem_pos)
{
	//先找到能镶嵌主宝石的槽子！然后再匹配辅助宝石
	assert(!all_gem_.empty());
	auto value = gem.size();
	GemDstPos result_pos;
	for (auto& solt : solt_info)
	{
		if (gem.empty())
			break;
		auto main_gem = gem[0];
		GemDstPos tmp_index;
		auto fn = [this, &tmp_index, &solt](const stGemInfo tmp){
			auto it_gem = std::find_if(all_gem_.begin(), all_gem_.end(), [&tmp](const GItemBasePtr& gem_ptr){return gem_ptr->GetItemName() == tmp.gem_name_; });
			if (it_gem == all_gem_.end())
				return;
			for (auto& km : solt.solt_color_)
			{
				if (km.is_use_)
					continue;
				auto it = std::find_if(tmp_index.begin(), tmp_index.end(), [&tmp, &km, &solt](stGemPosInfo value){return value.gem_name_ == tmp.gem_name_ || (km.index_ == value.index_ && value.dst_inventory_ == solt.dst_inventory_); });
				if (it != tmp_index.end())
					continue;
				if (km.color_ == tmp.gem_color_ || km.color_ == KGSC_White || tmp.gem_color_ == KGSC_White)
				{
					stGemPosInfo tmp_pos;
					tmp_pos.index_ = km.index_;
					tmp_pos.gem_name_ = tmp.gem_name_;
					tmp_pos.dst_inventory_ = solt.dst_inventory_;
					tmp_index.push_back(tmp_pos);
					break;
				}
			}
		};
		fn(main_gem);
		if (tmp_index.empty())
			continue;
		//gem.erase(gem.begin());
		std::for_each (gem.begin(), gem.end(), fn);
		if (tmp_index.size() >= value)
		{
			result_pos.clear();
			result_pos.assign(tmp_index.begin(), tmp_index.end());
			break;
		}
		if (tmp_index.size() > result_pos.size())
		{
			result_pos.clear();
			result_pos.assign(tmp_index.begin(), tmp_index.end());
		}
	}
	if (result_pos.empty())
		return;
	for (auto& v : result_pos)
	{
		for_each(solt_info.begin(), solt_info.end(), [&v](stGemSoltInfo& tmp){
			if (tmp.dst_inventory_ == v.dst_inventory_)
			{
				for (auto& m : tmp.solt_color_)
				{
					if (m.index_ == v.index_)
						m.is_use_ = true;
				}
			}
		});
		auto it_gem = std::find_if(all_gem_.begin(), all_gem_.end(), [&v](const GItemBasePtr& gem_ptr){return gem_ptr->GetItemName() == v.gem_name_; });
		if (it_gem != all_gem_.end())
			all_gem_.erase(it_gem);
		else
			assert(false);
	}
	gem_pos.insert(gem_pos.end(),result_pos.begin(), result_pos.end());
	return;
 }

GAutoUpDateGem::stMainGemInfo* GAutoUpDateGem::FindGemGroup(const std::string& gem_name)
{
	if (map_gem_.empty())
		return nullptr;
	for (auto& i : map_gem_)
	{
		if (i.main_Gem_ == gem_name)
			return &i;
		if (i.assist_gem_.empty())
			continue;
		auto it = std::find_if(i.assist_gem_.begin(), i.assist_gem_.end(), [&gem_name](const std::string& name){return gem_name == name; });
		if (it == i.assist_gem_.end())
			continue;
		return &i;
	}
	return nullptr;
}

GFilterRect::GFilterRect(const stVec2Int& left_bottom, const stVec2Int& right_top) : rect_(left_bottom, right_top)
{

}

bool GFilterRect::DoFilt(const FittedType& obj) const
{
	return rect_.IsIn(obj);
}

void GFilterRect::SetRect(const stVec2Int& left_bottom, const stVec2Int& right_top)
{
	rect_.SetRect(left_bottom, right_top);
}

bool GScene_PreGameState::InScene() const
{
	return GInterface::IsPreGameState();
}

bool GScene_PreGameState::Handle()
{
	LOG_O(Log_info) << "开始处理选择角色场景";
	return __super::Handle();
}

bool GScene_InGameState::InScene() const
{
	return GInterface::IsInGameState();
}

bool GScene_InGameState::Handle()
{
	LOG_O(Log_info) << "开始处理游戏内场景";
	return __super::Handle();
}

pt_int GEquipScore::CalcItemGoodValue(const GItemBase& item) const
{
	pt_int the_value = 0;
	auto color = item.GetItemColor();
	if (color < 0 || color >= KIC_Max)
	{
		//assert(false);
		return 0;
	}
	auto it = item_color_score_.find(color);
	if (it != item_color_score_.end())
		the_value += it->second;

	the_value += CalcPropScore(item);
	return the_value;
}

pt_int GEquipScore::CalcItemGoodValue(const GItemEquipBase& item) const
{
	pt_int the_value = 0;
	auto cur_link_cnt = item.GetMaxGemLinkValue();
	cur_link_cnt = std::min((int)cur_link_cnt, CD_kGemSlotMaxCnt);
	auto it = item_link_score_.find(cur_link_cnt);
	if (it != item_link_score_.end())
		the_value += it->second;
	for (auto& i : item.GetSlot())
	{
		auto it_red = item_gem_solt_red_.find(i.slot_color_);
		if (it_red != item_gem_solt_red_.end())
			the_value += it_red->second;
	}
	return the_value;
}

pt_int GEquipScore::CalcItemGoodValue(const GItemFlask& item) const
{
	return item.GetHpMpSpeed();
}

pt_int GEquipScore::CalcItemGoodValue(const GItemWeapon& item) const
{
	auto item_type = item.GetItemType();
	auto it = item_type_score_.find(item_type);
	if (it == item_type_score_.end())
		return 0;
	return it->second;
}

void GEquipScore::SetLinkScore(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
			continue;

		auto link_cnt = LuaObjCast(v[1], 0);
		auto grade_value = LuaObjCast(v[2], 0);
		item_link_score_[(pt_int)link_cnt] = grade_value;
	}
}

void GEquipScore::SetGemSlotRed(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
			continue;

		auto gem_color = LuaObjCast(v[1], 0);
		auto grade_value = LuaObjCast(v[2], 0);
		item_gem_solt_red_[gem_color] = grade_value;
	}
}

void GEquipScore::SetColorSocre(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
			continue;

		auto item_color = LuaObjCast(v[1], 0);
		auto grade_value = LuaObjCast(v[2], 0);
		item_color_score_[item_color] = grade_value;
	}
}

void GEquipScore::SetQuality(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	item_quality_ratio_ = LuaObjCast(lua_obj[1], 0);
}

void GEquipScore::PrintItemWeightInfo(const GItemBase& item)
{
	pt_int the_value = 0;
	auto color = item.GetItemColor();
	if (color < 0 || color >= KIC_Max)
	{
		assert(false);
		return;
	}
	auto it = item_color_score_.find(color);
	if (it != item_color_score_.end())
		LOG_O(Log_info) << "物品颜色评分: " << it->second;
	PrintItemPropScore(item.GetItemBaseProp(), item);
	PrintItemPropScore(item.GetItemSubjoinProp(), item);
	return;
}

void GEquipScore::PrintItemWeightInfo(const GItemEquipBase& item)
{
	pt_int the_value = 0;
	auto cur_link_cnt = item.GetMaxGemLinkValue();
	cur_link_cnt = std::min((int)cur_link_cnt, CD_kGemSlotMaxCnt);
	auto it = item_link_score_.find(cur_link_cnt);
	if (it != item_link_score_.end())
		LOG_O(Log_info) << cur_link_cnt << "连槽子评分: " << it->second;
	struct stSlot
	{
		typedef std::map<enCD_GemSlotColor, std::string>  SlotConT;
		stSlot(){
			slot_name_[kGSC_Red] 	= "红色槽子";
			slot_name_[kGSC_Green] 	= "绿色槽子";
			slot_name_[kGSC_Blue] 	= "蓝色槽子";
			slot_name_[KGSC_White] 	= "白色槽子";
		}
		SlotConT slot_name_;
	};
	static stSlot	slot_name;
	std::map<enCD_GemSlotColor, int>	tmp_slot;
	for (auto& i : item.GetSlot())
	{
		auto it_red = item_gem_solt_red_.find(i.slot_color_);
		if (it_red != item_gem_solt_red_.end())
			tmp_slot[i.slot_color_] += it_red->second;
	}
	for (auto& i : tmp_slot)
		LOG_O(Log_info) << slot_name.slot_name_[i.first] <<"总分值 "<<":" << i.second;
}

void GEquipScore::PrintItemWeightInfo(const GItemFlask& item)
{
	LOG_O(Log_info) << "药剂速率评分:" << item.GetHpMpSpeed();
}

void GEquipScore::PrintItemWeightInfo(const GItemWeapon& item)
{
	int value = 0;
	auto it = item_type_score_.find(item.GetItemType());
	if (it != item_type_score_.end())
		value = it->second;
	LOG_O(Log_info) << "武器类型评分: " << value;
}

void GEquipScore::SetWeaponSocre(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
			continue;

		enCD_ItemType item_type = LuaObjCast(v[1], kITYPE_Invalid);
		auto socre_value = LuaObjCast(v[2], 0);
		item_type_score_[item_type] = socre_value;
	}
}

void GEquipScore::SetItemPropScore(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TFUNCTION)
	{
		LOG_O(Log_error) << "设置属性评分参数必须是回调函数";
		assert(false);
		return;
	}
	prop_score_fn_ = lua_obj;
}

int GEquipScore::CalcPropScore(const GItemBase& item) const
{
	using namespace luabind;
	auto lua_l = prop_score_fn_.interpreter();
	if (!lua_l)
	{
		assert(false);
		return 0;
	}
	auto calc_prop = kGPF_BaseProp;
	if (calc_prop_)
		calc_prop = kGPF_AllProp;
	auto tbl = item.GroupPropTable(lua_l, calc_prop);
	if (type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return 0;
	}
	return LuaObjCallByDlt(prop_score_fn_, 0, tbl);
}

void GEquipScore::PrintItemPropScore(const GItemTipsPanel& prop_panel, const GItemBase& item)
{
	using namespace luabind;
	auto item_prop_ = prop_panel.GetProp();
	if (item_prop_.empty())
		return;
	auto lua_l = prop_score_fn_.interpreter();
	if (!lua_l)
	{
		assert(false);
		return;
	}
	for (auto& i : item_prop_)
	{
		if (!i.first || !i.second)
			continue;
		object tbl = newtable(lua_l);
		tbl[kLuaKeyNameItemType] = item.GetItemType();
		tbl[kLuaKeyItemLevel] = item.GetItemLvl();
		object tbl_prop = newtable(lua_l);
		tbl_prop[i.first->GetName()] = i.second->GetValue(lua_l);
		tbl[kLuaKeyNamePropInfo] = tbl_prop;
		int value = LuaObjCallByDlt(prop_score_fn_, 0, tbl);
		LOG_O(Log_info) << "属性:" << "[" << i.first->GetName() << ", " << value << "]";
	}
}

bool GKeepBuyItem::Parse(const std::string& item_name, const std::string& town_name, const std::string& npc_name)
{
	using namespace luabind;
	auto& cur_player_name = GPlayerMe::GetMe().GetCnName();
	if (cur_player_name != player_name_)
		finish_item_.clear();
	stDstNpcItem tmp;
	tmp.item_name_ = item_name;
	tmp.npc_name_ = npc_name;
	tmp.map_name_ = town_name;
	auto it = std::find_if(item_pathway_.begin(), item_pathway_.end(), [&tmp](const stDstNpcItem& info){ return info == tmp; });
	if (it == item_pathway_.end())
		item_pathway_.push_back(tmp);
	player_name_ = cur_player_name;
	return true;
}

bool GKeepBuyItem::CanKeepBuyItemCnt(const std::string& item_name, int& item_cnt)
{
	auto& cur_map = GMap::GetMe().GetMapName();
	auto it = std::find_if(item_pathway_.begin(), item_pathway_.end(), [&cur_map, &item_name](const stDstNpcItem& tmp){ return item_name == tmp.item_name_; });
	if (it == item_pathway_.end())
	{
		LOG_O(Log_error) << "没有配置该物品的获取方式,物品名:" << item_name;
		return false;
	}
	auto it_finish = std::find_if(finish_item_.begin(), finish_item_.end(), [&item_name](const std::string& name){return item_name == name; });
	if (it_finish != finish_item_.end())
	{
		LOG_O(Log_info) << "该物品已经存在 不需要再次购买 :" << item_name;
		return false;
	}
	PackagesContT packages;
	auto& gpm = GPlayerMe::GetMe();
	if (!GPackageMgrBase().NewPackage(kIT_MainBag, kIT_Flasks, packages))
	{
		assert(false);
		return false;
	}
	pt_dword cur_cnt = 0;
	for (auto& i : packages)
	{
		cur_cnt += i->GetItemCntByName(item_name);
		if (cur_cnt >= (pt_dword)item_cnt)
		{
			finish_item_.push_back(item_name);
			return false;
		}
	}
	if (!GInventoryMgr::GetMe().OpenStoreAndUpdate(false))
	{
		assert(false);
		return false;
	}
	auto& stash_mgr = GPackStashMgr::GetMe();
	stash_mgr.reset();
	cur_cnt += stash_mgr.GetItemCntByName(item_name);
	if (cur_cnt >= (pt_dword)item_cnt)
	{
		finish_item_.push_back(item_name);
		return false;
	}
	gpm.CloseAllPanel();
	item_cnt -= cur_cnt;
	return true;
}

int GKeepBuyItem::KeepBuyItem(const std::string& item_name, int cnt)
{
	if (item_pathway_.empty() || cnt <= 0)
	{
		assert(false);
		return 0;
	}
	int buy_cnt = 0;
	auto& gpm = GPlayerMe::GetMe();
	auto& cur_map_name = GMap::GetMe().GetMapName();
	for (auto& i : item_pathway_)
	{
		if (item_name != i.item_name_)
			continue;
		if (cur_map_name != i.map_name_)
		{
			auto& way = GWayPointDlgMgr::GetMe();
			if (!way.RebuildAll())
			{
				assert(false);
				return 0;
			}
			if (!way.IsOpenedWayPoint(i.map_name_, gpm.GetDiffLvl()))
			{
				LOG_O(Log_warning) << "目标城镇还未打开，无法购买物品，物品名：" << item_name << "，目标城镇：" << i.map_name_;
				//assert(false);
				return buy_cnt;
			}
			if (!gpm.SmartMoveMap(i.map_name_, gpm.GetDiffLvl()))
			{
				assert(false);
				return 0;
			}
		}
		if (!GInventoryMgr::GetMe().OpenStoreAndUpdate(true))
		{
			assert(false);
			return buy_cnt;
		}
		gpm.CloseAllPanel();
		buy_cnt = GInventoryMgr::GetMe().BuyItemByNpc(i.npc_name_, i.item_name_, cnt);
		if (buy_cnt == cnt)
		{
			finish_item_.push_back(i.item_name_);
			continue;
		}
		break;
	}
	return buy_cnt;
}

GUseSkill_gf::GUseSkill_gf(pt_word skill_id, const stCD_VecInt& dst_pos) : dst_pos_(dst_pos)
{
	skill_id_ = skill_id;
}

GUseSkill_gf::GUseSkill_gf()
{
	skill_id_ = 0;
}

void GUseSkill_gf::UseSkill(bool is_timer)
{
	assert(false);
	if (0 == skill_id_)
	{
		assert(false);
		return;
	}
	if (!dst_pos_.AllGreaterZero())
	{
		assert(false);
		return;
	}
	stCDS_UseSkill skill_msg;
	skill_msg.dst_pos_ = dst_pos_;
	skill_msg.skill_id_ = skill_id_;
	if (is_timer)
	{
		GInterface::GetSendSkillTimeDuration().TimeToDoSync([&skill_msg, this](){
			return GWndExecSync([&skill_msg, this](){
				IncSendSkillCnt();
				return GInterface::Send(skill_msg);
			});			
		});
	}
	else
	{
		GInterface::GetSendSkillTimeDuration().UpdateTime();
		GWndExecSync([&skill_msg, this](){
			IncSendSkillCnt();
			return GInterface::Send(skill_msg);
		});
	}
}

void GUseSkill_gf::SkillUsing(bool is_timer, const stCD_VecInt& dst_pos)
{
	assert(false);
	assert(dst_pos_.AllGreaterZero());
	stCDS_SkillUsing msg;
	msg.dst_pos_ = dst_pos;
	dst_pos_ = dst_pos;
	if (is_timer)
	{
		GInterface::GetSendSkillTimeDuration().TimeToDoSync([&msg, this](){
			return GWndExecSync([&msg, this](){
				IncSendSkillCnt();
				return GInterface::Send(msg);
			});
		});
	}
	else
	{
		GInterface::GetSendSkillTimeDuration().UpdateTime();
		GWndExecSync([&msg, this](){
			IncSendSkillCnt();
			return GInterface::Send(msg);
		});
	}
}

void GUseSkill_gf::UseSkillEnd(pt_dword timer)
{
	assert(false);
	stCDS_UseSkillEnd skill_end;
	BOOST_SCOPE_EXIT_ALL(&skill_end, this){
		GWndExecSync([&skill_end, this](){
			IncSendSkillCnt();
			return GInterface::Send(skill_end);
		});
	};
	if (timer)
		DummySleep(timer);
}

bool GUseSkill_gf::IsValid() const
{
	return 0 != skill_id_;
}

void GUseSkill_gf::SetSkillId(pt_word skill_id)
{
	skill_id_ = skill_id;
}

void GUseSkill_gf::SetDstPos(const stCD_VecInt& dst_pos)
{
	dst_pos_ = dst_pos;
}

void GUseSkill_gf::IncSendSkillCnt()
{
	GWndExecSync([](){
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
		actor->IncUseSkillCnt();
		return true;
	});
}

GUseSkill_gj::GUseSkill_gj(pt_word skill_id, const stCD_VecInt& dst_pos) : dst_pos_(dst_pos)
{
	skill_id_ = skill_id;
}

GUseSkill_gj::GUseSkill_gj()
{
	skill_id_ = 0;
}

void GUseSkill_gj::UseSkill(bool is_timer)
{
	if (is_timer)
	{
		GInterface::GetSendSkillTimeDuration().TimeToDoSync([this](){
			UseSkillImpl();
			return true;
		});
	}
	else
	{
		GInterface::GetSendSkillTimeDuration().UpdateTime();
		UseSkillImpl();
	}
}

void GUseSkill_gj::UseSkillImpl()
{
	if (!GInterface::WaitCanUseSkill())
	{
		assert(false);
		return;
	}
	if (0 == skill_id_)
	{
		assert(false);
		return;
	}
	if (!dst_pos_.AllGreaterZero())
	{
		assert(false);
		return;
	}
	if (!GWndExecSync([this](){
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
		return actor->UseSkill(skill_id_, &dst_pos_);
/*
#ifdef VER_GJ_
		stCDS_UseSkill msg;
		msg.skill_id_ = skill_id_;
		msg.dst_pos_ = dst_pos_;
		msg.skill_use_cnt_ = actor->IncUseSkillCnt();
		return GInterface::Send(msg);
#else
		assert(false);
		return false;
#endif*/
	}))
		assert(false);
}

void GUseSkill_gj::SkillUsing(bool is_timer, const stCD_VecInt& dst_pos)
{
	dst_pos_ = dst_pos;
	if (is_timer)
	{
		GInterface::GetSendSkillTimeDuration().TimeToDoSync([this](){
			UseSkillImpl();
			return true;
		});
	}
	else
	{
		GInterface::GetSendSkillTimeDuration().UpdateTime();
		UseSkillImpl();
	}
}

void GUseSkill_gj::UseSkillEnd(pt_dword timer)
{
	//这里一定不要使用技能，否则会出异常的，因为有DummySleep
	/*BOOST_SCOPE_EXIT_ALL(this){
		GInterface::GetSendSkillTimeDuration().UpdateTime();
		UseSkillImpl();
	};
	if (timer)
		DummySleep(timer);*/
}

bool GUseSkill_gj::IsValid() const
{
	return 0 != skill_id_;
}

void GUseSkill_gj::SetSkillId(pt_word skill_id)
{
	skill_id_ = skill_id;
}

void GUseSkill_gj::SetDstPos(const stCD_VecInt& dst_pos)
{
	dst_pos_ = dst_pos;
}

bool GScene_LoginState::InScene() const
{
	return GInterface::IsLoginState();
}

bool GScene_LoginState::Handle()
{
	LOG_O(Log_info) << "开始处理登陆场景";
	return __super::Handle();
}

GChecksumType stItemSlot::Checksum()
{	
	GChecksumType checksum = index_;
	checksum |= slot_color_ << 4;
	checksum <<= kIIC_Pos;
	if (gem_Item_)
		checksum += gem_Item_->CheckSum(kDummyChecksumBagWidth);
	return checksum;
}
