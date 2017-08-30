#include "stdafx.h"
#include "GClass.h"
#include <Net/IoTimer.h>
#include <Common/EnsureCleanup.h>
#include "GameObjMgr.h"
#include "GPlayer.h"
#include "GNpc.h"
#include "GMap.h"
#include "GItem.h"
#include <Game/Scmd.h>

using namespace GClass;

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
	if (!TimerDo(40, 1000 * 60, [&is_opened, &npc_name](){
		if (GInterface::EqualNpcPanelTitle(is_opened, npc_name))
			return true;
		if (GInterface::GetInGameMainUiDlgCnt(kTUSF_Shown) > CD_kInGameNormalShowDlgMaxCnt)
			return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	/*if (!is_opened)
	{
		assert(false);
		return false;
	}*/
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
	if (!TimerDo(40, 1000 * 60, [vtbl](){
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
	auto& map = GMap::GetMe();
	if (!TimerDo(800, 1000 * 60 * 6, [&map, &last_map_info](){
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
	auto& astar = G_AStar::GetMe();
	auto last_area_id = astar.GetAreaId(last_area_transition->GetBackupPos());
	if (last_area_id < 0)
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr_transition(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr_transition)
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&obj_mgr_transition, &gpm](){
		if (!obj_mgr_transition->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!gpm.Update())
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
	
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	obj_mgr_transition->SortByPos(*player_pos);
	auto the_new_transition = obj_mgr_transition->GetFirstObj();
	stAreaVertex vertex_rhs;
	int new_area_id = -1;
	if (the_new_transition)
	{
		new_area_id = astar.GetAreaId(the_new_transition->GetBackupPos());
		vertex_rhs.transitionable_ = the_new_transition;
	}
	else
	{
		new_area_id = astar.GetAreaId(*player_pos);
		vertex_rhs.transitionable_pos_ = *player_pos;
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
	stAreaVertex vertex_lhs;
	vertex_lhs.transitionable_ = last_area_transition->shared_from_this();
	map.AddAreaConnection(vertex_lhs, vertex_rhs);
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

bool GOpenVisitor::VisitChestItem(const GChest& world_item, bool is_pre_open, enOpenVisitorRes& open_res)
{
	if (is_pre_open)
	{
		//更新的时候就过滤掉了
		auto the_state = world_item.GetChestState();
		if (!the_state)
		{
			assert(false);
			return false;
		}
		if (*the_state)
		{
			open_res = kOVR_Opened;
		}
		return true;
	}
	assert(!world_item.GetCnName().empty());
	auto chest_state = world_item.GetChestState();
	if (!chest_state)
	{
		assert(false);
		return false;
	}
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
	if (!TimerDo(40, 1000 * 2, [chest_state](){
		return *chest_state;
	}))
	{
#ifdef _DEBUG
		//LOG_O(Log_debug) << "开箱子失败:" << world_item.GetCnName() << ",Id:" << world_item.GetGameId() << ",距离:" << distance;
#endif
		//assert(false);
		return false;
	}
	//DummySleep(kOpenObjTimeSpan);
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
		if (transitionable.GetOpenState())
			open_res = kOVR_Opened;
		return true;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Transitonable));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	auto id = transitionable.GetGameId();
	if (!TimerDo(40, 1000 * 5, [&obj_mgr, &id](){
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
	//DummySleep(kOpenObjTimeSpan);
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
}

//////////////////////////////////////////////////////////////////////////
class _GHNTL_Continue : public GHandleNpcTalkLabel::Handler{
public:
	enTalkToNpcRes PreHandle(const std::wstring& talk_label) override{
		/*auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(false);
		if (talk_res < kTTNR_Succeed)
			return talk_res;*/
		GInterface::TalkToNpc(kNPIC_TalkPanel, kNTL_Cotinue, false);
		return kTTNR_NotNeedHandle;
	}
};

class _GHNTL_Bye : public GHandleNpcTalkLabel::Handler{
public:
	enTalkToNpcRes PreHandle(const std::wstring& talk_label) override{
		auto talk_res = GHandleNpcTalkLabel::HandleCotinueImpl(false);
		if (talk_res < kTTNR_Succeed)
			return talk_res;
		GInterface::TalkToNpc(kNPIC_Normal, kNTL_Bye, false);
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

GExcept_AvoidSkill::GExcept_AvoidSkill() : CmnGeneralRunException(kEmptyStr)
{

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
	static TimeDuration td(2000);
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
	static TimeDuration td(2000);
	if (td.CalcLeftDuration() > 0)
		return false;
	td.UpdateTime();
	float tmp_last = 0.9f;
	float tmp_rate = 1.0f;
	int  tmp_last_rete=1;
	return CalcIdxImpl(tmp_last, tmp_last_rete, tmp_rate, kFT_Utility, utility_index);
}

void GAutoUpDateGem::AddAutoGemLink(const std::string& main_gem, const luabind::object& tbl)
{
	using namespace luabind;
	if (main_gem.empty())
		return;

	if (type(tbl) != LUA_TTABLE)
		return;

	map_gem_[main_gem].clear();
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
		map_gem_[main_gem].push_back(object_cast<std::string>(v));
	}
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
	GItemContT all_gem;
	auto& inventory = GInventoryMgr::GetMe();
	inventory.GetAllPackGem(all_gem);
	if (all_gem.empty())
		return false;
	for (auto it = all_gem.begin(); it != all_gem.end();)
	{
		if (!(*it)->CanWear())
		{
			it = all_gem.erase(it);
			continue;
		}
		++it;
	}
	if (all_gem.empty())
	{
		LOG_O(Log_debug) << "没有要镶嵌的宝石";
		return false;
	}
	for (auto& i : map_gem_)
	{
		auto it = std::find_if(all_gem.begin(), all_gem.end(), [&i](const GItemBasePtr& gem){return gem->GetItemName() == i.first; });
		if (it == all_gem.end())
			continue;
		std::vector<stGemInfo>  tmp_vec;
		stGemInfo tmp_main_gem;
		tmp_main_gem.gem_color_ = (*it)->GetItemGemColor();
		tmp_main_gem.gem_name_.assign((*it)->GetItemName());
		tmp_main_gem.gem_main_ = true;
		tmp_vec.push_back(tmp_main_gem);
		all_gem.erase(it);
		if (!i.second.empty())
		{
			for (auto& v : i.second)
			{
				auto it_assist = std::find_if(all_gem.begin(), all_gem.end(), [v](const GItemBasePtr& gem){return gem->GetItemName() == v; });
				if (it_assist == all_gem.end())
					continue;
				tmp_main_gem.gem_color_ = (*it_assist)->GetItemGemColor();
				tmp_main_gem.gem_name_.assign((*it_assist)->GetItemName());
				tmp_main_gem.gem_main_ = false;
				all_gem.erase(it_assist);
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
		//LOG_O(Log_debug) << "没有找到配置表中要连接的宝石";
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

//#ifdef _DEBUG
//	for (auto& vv : gem_pos)
//		LOG_O(Log_debug) << "宝石名:" << vv.gem_name_ << ",仓库:" << vv.dst_inventory_ << ",索引:" << vv.index_;
//#endif
	return true;
}

void GAutoUpDateGem::FindSetCanWearEquip(GClass::VcSlotInfoConT&  solt_info, std::vector<GClass::stGemInfo> gem, GemDstPos& gem_pos)
{
	auto value = gem.size();
	GemDstPos result_pos;
	for (auto& solt : solt_info)
	{
		//if (solt.link_value_ < (int)value)
		//	break;
		GemDstPos tmp_index;
		auto gem_it = std::for_each(gem.begin(), gem.end(), [this, &tmp_index, &solt](const stGemInfo tmp){
			for (auto& km : solt.solt_color_)
			{
				if (km.is_use_)
					continue;

				auto it = std::find_if(tmp_index.begin(), tmp_index.end(), [&km, &solt](stGemPosInfo value){return km.index_ == value.index_ && value.dst_inventory_ == solt.dst_inventory_; });
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
		});
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
	}

	gem_pos.insert(gem_pos.end(),result_pos.begin(), result_pos.end());
	return;
 }


GEnsureAvoidSkill::GEnsureAvoidSkill()
{
	auto& map = GMap::GetMe();
	old_enabled_ = map.IsEnableAvoidSkill();
	if (!map.Update())
	{
		assert(false);
		return;
	}
	map.EnableAvoidSkill(true);
}

GEnsureAvoidSkill::~GEnsureAvoidSkill()
{
	GMap::GetMe().EnableAvoidSkill(old_enabled_);
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
		assert(false);
		return 0;
	}
	auto it = item_color_score_.find(color);
	if (it != item_color_score_.end())
		the_value += it->second;
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
