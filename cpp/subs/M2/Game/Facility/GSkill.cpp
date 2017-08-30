#include "stdafx.h"
#include "GSkill.h"
#include "GPlayer.h"
#include "../DllApp.h"

bool GDbPassiveSkillMgr::RebuildAll()
{
	if (game_passive_skills_)
		return true;
	return __super::RebuildAll();
}

bool GDbPassiveSkillMgr::RebuildAllImpl()
{
	passive_skills_.clear();
	auto& db_mgr = CD_Interface::GetDbMgr();
	auto db_passive_skill = db_mgr.GetDbPassiveSkill();
	if (IsUnValidPtr(db_passive_skill))
	{
		assert(false);
		return false;
	}
	auto db_tbl =  db_passive_skill->GetDbTbl();
	if (IsUnValidPtr(db_tbl))
	{
		assert(false);
		return false;
	}
	if (db_tbl->tbl_items_.empty())
	{
		assert(false);
		return false;
	}

	for (auto& v : db_tbl->tbl_items_)
	{
		auto ch_name = CD_SafeName(v.ch_name_);
		if (!ch_name)
			continue;
		if (StdEx::TStrLen(ch_name) <= 0)
			continue;
		passive_skills_.insert(std::make_pair(std::wstring(ch_name), &v));
		if (!v.professions_.empty())
		{
			for (auto& type : v.professions_)
			{
				if (!type.IsValid())
				{
					assert(false);
					goto LABLE_HERE;
				}
			}
			started_passive_skills_.push_back(&v);
		LABLE_HERE:
			(void*)0;
		}
	}
	assert(started_passive_skills_.size() == started_passive_skills_.max_size());

	auto db_graph = db_mgr.GetDbPassiveSkillGraph();
	if (!db_graph)
	{
		assert(false);
		return false;
	}
	auto tbl_graph = db_graph->GetDbTbl();
	if (!tbl_graph)
	{
		assert(false);
		return false;
	}
	if (tbl_graph->hash_passive_skills_.size() < 500)
	{
		assert(false);
		return false;
	}
	game_passive_skills_ = &tbl_graph->hash_passive_skills_;
	return true;
}

GDbPassiveSkillMgr::GDbPassiveSkillMgr()
{
	game_passive_skills_ = nullptr;
}

stCD_TblItemPassiveSkill* GDbPassiveSkillMgr::FindPassiveSkill(const std::wstring& skill_name) const
{
	auto it = passive_skills_.find(skill_name);
	if (it == passive_skills_.end()) return nullptr;
	return it->second;
}

stCD_TblItemPassiveSkill* GDbPassiveSkillMgr::FindPassiveSkill(pt_word skill_id) const
{
	if (!game_passive_skills_)
	{
		assert(false);
		return nullptr;
	}
	auto it = game_passive_skills_->find(skill_id);
	if (it == game_passive_skills_->end()) return nullptr;
	return it->second.tbl_item_;
}

bool GDbPassiveSkillMgr::GetPassiveSkill(const std::wstring& skill_name, GDbPassiveSkill& out) const
{
	auto ptr = FindPassiveSkill(skill_name);
	if (!ptr) return false;
	out.passive_db_ = ptr;
	if (!out.UpdateImpl())
	{
		assert(false);
		return false;
	}
	return true;
}

stCD_TblItemPassiveSkill* GDbPassiveSkillMgr::FindStartedPassiveSkill(enCD_ProfessionType profession_type) const
{
	for (auto passive_skill : started_passive_skills_)
	{
		for (auto& type : passive_skill->professions_)
		{
			if (type.tbl_item_->profession_type_ == profession_type)
				return passive_skill;
		}
	}
	assert(false);
	return nullptr;
}

stCD_TblItemPassiveSkill* GDbPassiveSkillMgr::FindStartedPassiveSkill(const std::wstring& profession_name) const
{
	for (auto passive_skill : started_passive_skills_)
	{
		for (auto& type : passive_skill->professions_)
		{
			if (type.tbl_item_->profession_ch_name_ == profession_name)
				return passive_skill;
		}
	}
	assert(false);
	return nullptr;
}

GDbPassiveSkill::GDbPassiveSkill()
{
	skill_id = 0;
	passive_db_ = nullptr;
}

bool GDbPassiveSkill::UpdateImpl()
{
	if (IsUnValidPtr(passive_db_))
	{
		assert(false);
		return false;
	}

	skill_id = passive_db_->skill_id_;
	ch_name_.assign(CD_SafeName(passive_db_->ch_name_));
	assert(skill_id > 0);
	return true;
}

void GDbPassiveSkill::DebugInfo() const
{
	LOG_O(Log_debug) << "passive-name" << ch_name_ << ",Skill_Id-" << skill_id;
}

GDbActiveSkillMgr::GDbActiveSkillMgr()
{

}

bool GDbActiveSkillMgr::RebuildAllImpl()
{
	auto skillDb = CD_Interface::GetDbMgr().GetDbActiveSkills();
	if (IsUnValidPtr(skillDb))
	{
		assert(false);
		return false;
	}

	auto tab = skillDb->GetDbTbl();
	if (IsUnValidPtr(tab))
	{
		assert(false);
		return false;
	}

	if (!tab->tbl_items_.IsValid())
	{
		assert(false);
		return false;
	}
	GDbActiveSkill tmpskill;
	for (auto& i : tab->tbl_items_)
	{
		tmpskill.skill_db_ = &i;
		if (!tmpskill.UpdateImpl())
		{
			assert(false);
			return false;
		}
		skillDb_.push_back(tmpskill);
	}

	return true;
}

void GDbActiveSkillMgr::DebugInfo() const
{
	if (skillDb_.empty())
		return;

	for (auto& i : skillDb_)
		i.DebugInfo();
}

bool GDbActiveSkillMgr::RebuildAll()
{
	if (!skillDb_.empty())
		return true;

	return __super::RebuildAll();
}



bool GDbActiveSkill::UpdateImpl()
{
	if (IsUnValidPtr(skill_db_))
	{
		assert(false);
		return false;
	}

	ch_name_.assign(CD_SafeName(skill_db_->ch_name_));

	return true;
}

void GDbActiveSkill::DebugInfo() const
{
	LOG_O(Log_debug) << "Skill_Db_Name-" << ch_name_;
}

GDbActiveSkill::GDbActiveSkill()
{
	skill_db_ = nullptr;
}

GSkillMgr::GSkillMgr()
{

}

bool GSkillMgr::RebuildAllImpl()
{
	skills_.clear();
	CD_Interface::TraMySkill([this](stCD_SkillInfo* skill_info){
		GSkillObj skill_tmp;
		if (!skill_tmp.SetSkillInfo(skill_info))
		{
			assert(false);
			return false;
		}
		if (!skill_tmp.Update())
		{
			assert(false);
			return false;
		}

		skills_.push_back(skill_tmp);
		return true;
	});
	return true;
}

void GSkillMgr::DebugInfo() const
{
	if (skills_.empty())
		return;

	for (auto& i : skills_)
		i.DebugInfo();
}

const GSkillObj* GSkillMgr::FindByName(const std::string& skill_name) const
{
	assert(!skill_name.empty());
	for (auto& v : skills_)
	{
		if (v.GetSkillName() == skill_name)
			return &v;
	}
	return nullptr;
}

const GSkillObj* GSkillMgr::FindById(int skill_id) const
{
	//assert(skill_id > 0);
	for (auto& v : skills_)
	{
		if (v.GetSkillId() == skill_id)
			return &v;
	}
	return nullptr;
}

bool GSkillObj::UpdateImpl()
{
	if (!game_skill_info_)
	{
		assert(false);
		return false;
	}
	skill_name_.clear();
	skill_id_ = game_skill_info_->skill_id_;
	auto active_skill = game_skill_info_->GetActiveSkillDb();
	if (active_skill)
	{
		skill_name_ = WideCharToGbk(CD_SafeName(active_skill->ch_name_));
		if (skill_name_.empty())
		{
			assert(false);
			return false;
		}
	}
	stCD_SkillAttrValueCont attr_values;
	game_skill_info_->GetSkillAttrValues(attr_values);
	if (attr_values.empty()) return true;
	for (auto& attr_value : attr_values)
	{
		if (attr_value.value_type_ == kSAVT_CallerMaxCnt)
		{
			if (attr_value.values_.empty())
				continue;
			caller_max_cnt_ = attr_value.values_[0];
		}
	}
	return true;
}

bool GSkillObj::SetSkillInfo(stCD_SkillInfo* skill_info)
{
	assert(skill_info);
	game_skill_info_ = skill_info;
	return true;
}

void GSkillObj::DebugInfo() const
{
	LOG_O(Log_debug) << "skill_name::" << skill_name_ << ",skill_id:" << skill_id_;
}

GSkillObj::GSkillObj()
{
	skill_id_ = 0;
	game_skill_info_ = nullptr;
	caller_max_cnt_ = 0;
}

GAutoUseSkill::GAutoUseSkill()
{

}

void GAutoUseSkill::AddFireSkill(const std::string& skill_name, float time_gap, int fire_cnt, float hp_min_limit, float hp_max_limit, float mp_min_limit, float mp_max_limit)
{
	auto it = std::find_if(fire_skill_cont_.begin(), fire_skill_cont_.end(), [&skill_name](const stUseSkillInfo& skill_info){return skill_name == skill_info.skill_name_; });
	if (it != fire_skill_cont_.end())
	{
		LOG_O(Log_warning) << "已有该技能配置，技能名：" << skill_name;
		assert(false);
		return;
	}
	stUseSkillInfo tmp;
	tmp.skill_name_.assign(skill_name);
	tmp.skill_consume_min_mp_ = mp_min_limit;
	tmp.skill_consume_max_mp_ = mp_max_limit;
	tmp.skill_consume_min_hp_ = hp_min_limit;
	tmp.skill_consume_max_hp_ = hp_max_limit;
	tmp.use_gap_time_.SetDuration(time_gap * 1000);
	tmp.use_gap_time_.UpdateTime();
	tmp.use_count_ = fire_cnt;
	//tmp.fire_distance_ = fire_cnt;
	fire_skill_cont_.push_back(tmp);
}

void GAutoUseSkill::ClearAllUseSkill()
{
	fire_skill_cont_.clear();
}

const std::string& GAutoUseSkill::GetOneUseSkill(int& count)
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return kObjName_DefaultSkill;
	}
	
	if (fire_skill_cont_.empty())
	{
		LOG_O(Log_debug) << "自动获取技能之没有配置 使用默认技能:" << kObjName_DefaultSkill;
		return kObjName_DefaultSkill;
	}

	auto cur_mp = gpm.GetCurMp();
	if (cur_mp <= 0)
		return kObjName_DefaultSkill;
	auto max_mp = gpm.GetMaxMp();
	if (max_mp <= 0)
	{
		assert(false);
		return kObjName_DefaultSkill;
	}
	auto cur_rate_mp = (float)cur_mp / (float)max_mp;
	auto cur_rate_hp = (float)gpm.GetCurHp() / (float)gpm.GetMaxHp();

	auto& skill_mgr = GSkillMgr::GetMe();
	if (!skill_mgr.RebuildAll())
	{
		assert(false);
		return kObjName_DefaultSkill;
	}
	for (auto& i : fire_skill_cont_)
	{
		if (i.skill_consume_min_hp_ > cur_rate_hp ||i.skill_consume_max_hp_ < cur_rate_hp)
			continue;
		if (i.skill_consume_min_mp_ > cur_rate_mp || i.skill_consume_max_mp_ < cur_rate_mp)
			continue;
		if (i.use_gap_time_.CalcLeftDuration() > 0)
			continue;
		i.use_gap_time_.UpdateTime();
		if(!skill_mgr.FindByName(i.skill_name_))
			continue;
		count = i.use_count_;
		return i.skill_name_;
	}
	return kObjName_DefaultSkill;
}

void GPassiveSkillResolver::SetNodesInfo(const luabind::object& job_skill_nodes)
{
	using namespace luabind;
	job_nodes_.clear();
	need_regen_ = true;
	if (type(job_skill_nodes) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	for (iterator it_job(job_skill_nodes), it_end; it_job != it_end; ++it_job)
	{
		object key_job_name = it_job.key();
		auto job_name = LuaObjCast(key_job_name, kEmptyStr);
		if (job_name.empty())
		{
			LOG_O(Log_error) << "配置技能树之必须要有职业名";
			continue;
		}
		object tbl_nodes = *it_job;
		if (LUA_TTABLE != type(tbl_nodes))
		{
			LOG_O(Log_error) << "配置技能树之技能结点信息必须是表类型";
			continue;
		}
		auto job_name_w = GbkToWideChar(job_name);
		auto& job_nodes = job_nodes_[job_name_w];
		object the_nodes = tbl_nodes[1];
		//大节点
		if (LUA_TTABLE == type(the_nodes))
		{
			for (auto& v : the_nodes)
			{
				const auto& skill_name = LuaObjCast(v, kEmptyStr);
				if (skill_name.empty())
				{
					LOG_O(Log_error) << "配置技能树之大技能节点中技能名有为空的：" << job_name;
					continue;
				}
				job_nodes.big_nodes_.push_back(GbkToWideChar(skill_name));
			}
		}
		the_nodes = tbl_nodes[2];
		//小节点
		if (LUA_TTABLE == type(the_nodes))
		{
			for (auto& v : the_nodes)
			{
				const auto& skill_name = LuaObjCast(v, kEmptyStr);
				if (skill_name.empty())
				{
					LOG_O(Log_error) << "配置技能树之小技能节点中技能名有为空的：" << job_name;
					continue;
				}
				job_nodes.small_nodes_.push_back(GbkToWideChar(skill_name));
			}
		}
	}
}

const GPassiveSkillResolver::stNodesInfo* GPassiveSkillResolver::GetCurJobNodesInfo() const
{
	auto& gpm = GPlayerMe::GetMe();
	/*if (!gpm.Update())
	{
		assert(false);
		return nullptr;
	}*/
	auto& job_name = gpm.GetJobName();
	if (job_name.empty())
	{
		assert(false);
		return nullptr;
	}
	if (job_name != last_job_name_)
	{
		need_regen_ = true;
		last_job_name_ = job_name;
	}
	auto it = job_nodes_.find(job_name);
	if (it == job_nodes_.end())
	{
		LOG_O(Log_error) << "没有当前职业的天赋树配置信息";
		return nullptr;
	}
	return &it->second;
}

const GPassiveSkillResolver::stNodesInfo* GPassiveSkillResolver::ReGenCurJobNodes()
{
	if (!GenGraphOnce())
	{
		assert(false);
		return nullptr;
	}
	auto cur_nodes = GetCurJobNodesInfo();
	if (!cur_nodes)
	{
		assert(false);
		return nullptr;
	}
	if (!need_regen_) return cur_nodes;

	resolver_.TraEdges([this, cur_nodes](stCD_TblItemPassiveSkill* lhs, stCD_TblItemPassiveSkill* rhs, int& weight){
		if (cur_nodes->ExistSmallNode(lhs) || cur_nodes->ExistSmallNode(rhs))
			weight = kEdgeWeightLoved;
		else
			weight = kEdgeWeightDef;
		return true;
	});

	need_regen_ = false;
	return cur_nodes;
}

bool GPassiveSkillResolver::GenGraphOnce()
{
	if (resolver_.VertexCount() > 0)
		return true;
	LOG_O(Log_trace) << "开始初始化天赋树";
	stCD_DbTblPassiveSkillGraph* tbl_graph = nullptr;
	GWndExecSyncT(tbl_graph, []() -> stCD_DbTblPassiveSkillGraph*{
		auto db_graph = CD_Interface::GetDbMgr().GetDbPassiveSkillGraph();
		if (!db_graph)
		{
			assert(false);
			return nullptr;
		}
		return db_graph->GetDbTbl();
	});
	if (!tbl_graph)
	{
		assert(false);
		return false;
	}
	for (auto& vertex_pair : tbl_graph->vertices_)
	{
		auto game_vertex = vertex_pair.second;
		if (!game_vertex)
		{
			assert(false);
			continue;
		}
		auto passive_skill = game_vertex->passive_skill_.tbl_item_;
		if (!passive_skill)
			continue;
		if (StdEx::TStrLen(CD_SafeName(passive_skill->ch_name_)) <= 0)
			continue;
		resolver_.AddVertex(passive_skill);
	}
	for (auto& vertex_pair : tbl_graph->vertices_)
	{
		auto game_vertex = vertex_pair.second;
		if (!game_vertex)
		{
			assert(false);
			continue;
		}
		auto passive_skill = game_vertex->passive_skill_.tbl_item_;
		if (!passive_skill)
			continue;
		if (StdEx::TStrLen(CD_SafeName(passive_skill->ch_name_)) <= 0)
			continue;
		for (auto edge : game_vertex->edges_in_)
		{
			if (!edge)
			{
				assert(false);
				continue;
			}
			if (!edge->from_)
			{
				assert(false);
				continue;
			}
			if (!edge->to_)
			{
				assert(false);
				continue;
			}
			auto passive_skill_from = edge->from_->passive_skill_.tbl_item_;
			if (!passive_skill_from)
				continue;
			if (StdEx::TStrLen(CD_SafeName(passive_skill_from->ch_name_)) <= 0)
				continue;
			auto passive_skill_to = edge->to_->passive_skill_.tbl_item_;
			if (!passive_skill_to)
				continue;
			if (StdEx::TStrLen(CD_SafeName(passive_skill_to->ch_name_)) <= 0)
				continue;
			if (resolver_.Exist(passive_skill_from, passive_skill_to))
				continue;
			resolver_.AddEdge(passive_skill_from, passive_skill_to, kEdgeWeightDef);
		}
	}
	if (resolver_.VertexCount() <= 0)
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "初始化天赋树成功";
	return true;
}

GPassiveSkillResolver::GPassiveSkillResolver()
{
	need_regen_ = true;
}

bool GPassiveSkillResolver::GenOncePath(StepsContType& steps)
{
	GPassiveSkillGraphMgr graph_mgr;
	if (!graph_mgr.Update())
	{
		assert(false);
		return false;
	}
	auto cur_points = graph_mgr.GetPassiveSkillPoints();
	if (cur_points <= 0)
		return false;
	auto cur_nodes = ReGenCurJobNodes();
	if (!cur_nodes)
		return false;
	auto nearest_pair = CalcVertexPorts(graph_mgr, cur_nodes->big_nodes_);
	if (nearest_pair.first == nearest_pair.second)
		return false;
	if (!resolver_.Resolve(nearest_pair.first, nearest_pair.second, steps))
	{
		assert(false);
		return false;
	}
	if (steps.empty())
	{
		assert(false);
		return false;
	}
	steps.pop_front();
	if (steps.empty())
	{
		assert(false);
		return false;
	}
	auto step_size = (int)steps.size();
	if (step_size <= cur_points)
		return true;
	step_size -= cur_points;
	while (step_size > 0)
	{
		--step_size;
		steps.pop_back();
	}
	if (steps.empty())
	{
		assert(false);
		return false;
	}
	assert(steps.size() == cur_points);
	return true;
}

GPassiveSkillResolver::PassiveSkillPair GPassiveSkillResolver::CalcVertexPorts(GPassiveSkillGraphMgr& graph_mgr,
	const SkillNameCont& nodes) const
{
	PassiveSkillPair res;
	if (nodes.empty())
	{
		assert(false);
		return res;
	}
	if (last_job_name_.empty())
	{
		assert(false);
		return res;
	}
	auto& db_passive_skill_mgr = GDbPassiveSkillMgr::GetMe();
	if (!db_passive_skill_mgr.RebuildAll())
	{
		assert(false);
		return res;
	}
	auto started_skill = db_passive_skill_mgr.FindStartedPassiveSkill(last_job_name_);
	if (!started_skill)
	{
		assert(false);
		return res;
	}
	auto& learned_skills = graph_mgr.GetLearnedSkillIds();
	learned_skills.push_back(started_skill->skill_id_);
	BOOST_SCOPE_EXIT_ALL(&learned_skills){
		learned_skills.pop_back();
	};
	//////////////////////////////////////////////////////////////////////////
	struct stHelp{
		stCD_TblItemPassiveSkill*	free_skill_;
		stCD_TblItemPassiveSkill*	learned_skill_;
		int							weight_;
	};
	typedef CmnBufferVector<stHelp, 250> NodesSkillCont;

	NodesSkillCont nodes_skill;
	for (auto& node : nodes)
	{
		auto skill = db_passive_skill_mgr.FindPassiveSkill(node);
		if (!skill)
		{
			LOG_O(Log_error) << "配置天赋树之不存在这样的技能名：" << node;
			continue;
		}
		for (auto skill_id : learned_skills)
		{
			if (skill_id == skill->skill_id_)
				goto LABLE_CONTINUE;
		}
		{
			nodes_skill.push_back({ skill, nullptr, std::numeric_limits<int>::max() });
		}
	LABLE_CONTINUE:
		(void*)0;
	}
	if (nodes_skill.empty())
		return res;
	//////////////////////////////////////////////////////////////////////////
	StepsWeightContType steps;
	for (auto& node : nodes_skill)
	{
		steps.clear();
		if (!resolver_.Resolve<CSP_PolicyVertexWeight>(node.free_skill_, steps))
		{
			assert(false);
			continue;
		}
		if (steps.empty())
			continue;
		for (auto& step : steps)
		{
			if (step.vertex_ == node.free_skill_)
				continue;
			for (auto skill_id : learned_skills)
			{
				if (skill_id == step.vertex_->skill_id_)
				{
					node.learned_skill_ = step.vertex_;
					node.weight_ = step.weight_;
					goto LABLE_BREAK;
				}
			}
			continue;
		LABLE_BREAK:
			break;
		}
	}
	std::sort(nodes_skill.begin(), nodes_skill.end(), [](const stHelp& lhs, const stHelp& rhs){
		return lhs.weight_ < rhs.weight_;
	});
	for (auto& node : nodes_skill)
	{
		if (node.free_skill_ && node.learned_skill_)
		{
			res.first = node.learned_skill_;
			res.second = node.free_skill_;
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return res;
}

int GPassiveSkillResolver::AutoLvlUpPassiveSkills()
{
	StepsContType steps;
	int cnt = 0;
	while (true)
	{
		steps.clear();
		if (!GenOncePath(steps))
			break;
		assert(!steps.empty());
		for (auto step : steps)
		{
			assert(step);
			if (!GInterface::LvlUpPassiveSkill(step->skill_id_))
			{
				assert(false);
				goto LABLE_BREAK;
			}
			++cnt;
		}
		continue;
	LABLE_BREAK:
		break;
	}
	return cnt;
}

void GPassiveSkillResolver::DebugInfo(const std::wstring& first_node, const std::wstring& last_node)
{
	auto& db_mgr = GDbPassiveSkillMgr::GetMe();
	if (!db_mgr.RebuildAll())
	{
		assert(false);
		return;
	}
	auto node_frist = db_mgr.FindPassiveSkill(first_node);
	if (!node_frist)
	{
		assert(false);
		return;
	}
	auto node_last = db_mgr.FindPassiveSkill(last_node);
	if (!node_last)
	{
		assert(false);
		return;
	}
	need_regen_ = true;
	auto cur_nodes = ReGenCurJobNodes();
	if (!cur_nodes)
		return;
	StepsContType steps;
	if (!resolver_.Resolve(node_frist, node_last, steps))
	{
		assert(false);
		return;
	}
	for (auto step : steps)
	{
		LOG_O(Log_debug) << step->ch_name_;
	}
	return;
}

void GPassiveSkillResolver::Init()
{
	if (job_nodes_.empty())
	{
		if (!GenGraphOnce())
		{
			assert(false);
			return;
		}
	}
	else
	{
		if (!ReGenCurJobNodes())
		{
			assert(false);
			return;
		}
	}	
}

bool GPassiveSkillResolver::stNodesInfo::ExistSmallNode(stCD_TblItemPassiveSkill* passive_skill) const
{
	for (auto& v : small_nodes_)
	{
		if (v == passive_skill->ch_name_)
			return true;
	}
	return false;
}

GPassiveSkillGraphMgr::GPassiveSkillGraphMgr()
{
	passive_skill_points_ = 0;
}

bool GPassiveSkillGraphMgr::UpdateImpl()
{
	auto logic = CD_Interface::GetGameLogic();
	if (!logic)
	{
		assert(false);
		return false;
	}
	learned_skill_ids_ = logic->passive_skill_.learned_skill_id_cont_;
	passive_skill_points_ = logic->passive_skill_.GetTalentPoint();
	assert(passive_skill_points_ >= 0 && passive_skill_points_ < 500);
	return true;
}

bool GBuffMgr::UpdateImpl()
{
	buff_items_.clear();
	if (!GWndExecSync([this](){
		auto player_entity = CD_Interface::GetPlayerEntity();
		if (!player_entity)
		{
			assert(false);
			return false;
		}
		auto life = player_entity->GetLife();
		if (!life)
		{
			assert(false);
			return false;
		}
		for (auto buff : life->buff_cont_)
		{
			if (IsUnValidPtr(buff))
			{
				assert(false);
				return false;
			}
			buff_items_.push_back(stBuffItem());
			buff_items_.back().buff_name_ = WideCharToGbk(CD_SafeName(buff->GetBuffName()));
		}
		CD_Interface::TraCallerBuff([this](stCD_SkillInfo* skill_info, stCD_UiBuffItem* ui_item){
			assert(skill_info);
			assert(ui_item);
			auto active_skill = skill_info->GetActiveSkillDb();
			if (!active_skill)
				return true;
			buff_items_.push_back(stBuffItem());
			auto& the_item = buff_items_.back();
			the_item.buff_name_ = WideCharToGbk(CD_SafeName(active_skill->ch_name_));
			for (auto id : ui_item->entity_ids_)
				the_item.buff_entity_ids_.push_back(id);
			return true;
		});
		return true;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

const GBuffMgr::stBuffItem* GBuffMgr::FindBuff(const GSkillObj& skill_obj) const
{
	auto& skill_name = skill_obj.GetSkillName();
	if (skill_name.empty())
		return nullptr;
	for (auto& buff : buff_items_)
	{
		auto pos = buff.buff_name_.find(skill_name);
		if (pos != std::string::npos)
			return &buff;
	}
	return nullptr;
}

bool GBuffMgr::ExistEntityId(pt_dword entity_id) const
{
	for (auto& v : buff_items_)
	{
		for (auto& id : v.buff_entity_ids_)
		{
			if (id == entity_id)
				return true;
		}
	}
	return false;
}
