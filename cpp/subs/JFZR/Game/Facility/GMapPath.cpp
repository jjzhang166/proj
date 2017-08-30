#include "stdafx.h"
#include "GMapPath.h"
#include "GClass.h"
#include "GMap.h"

bool GSP_Resolver::FillVillages()
{
	using namespace GClass;
	auto& allowed_mgr = AllowedVillageCloneMapMgr::GetMe();
	if ( !allowed_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto& map_mgr = GStageTemplateMgr::GetMe();
	if ( !map_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	stages_.clear();
	auto& iter = allowed_mgr.TraverseVillage();
	if ( !iter )
	{
		assert(false);
		return false;
	}
	StagePtr stage_tmp;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		const auto& it_value = iter->CurItem();
		stage_tmp.reset(new GSP_Stage);
		assert(stage_tmp);
		stage_tmp->stage_id_ = it_value.GetMapId();
		
		stages_.push_back((stage_tmp));
	}
	if (stages_.empty())
	{
		assert(false);
		return false;
	}

	static const wchar_t* the_village_id[][2] = {
		{ L"Ep02Village01", L"WarpGate03" }
	};
	GSP_Gate gate_tmp;
	for ( auto& stage : stages_ )
	{
		assert(stage);
		auto stage_template = map_mgr.FindStage(stage->stage_id_);
		if ( !stage_template )
		{
			assert(false);
			return false;
		}
		if ( !stage_template->IsTown() )
		{
			assert(false);
			return false;
		}
		auto& gate_links_cont = stage_template->GetGateLinksInfo().GetCont();
		if (gate_links_cont.empty())
		{
			assert(false);
			continue;
		}
		int warning_the_stage = -1;
		for ( int i = 0;i != _countof(the_village_id); ++i )
		{
			if ( stage->stage_id_ == the_village_id[i][0] )
			{
				warning_the_stage = i;
				break;
			}
		}
		for ( const auto& gate_link : gate_links_cont )
		{
			const auto& gate_mgr_cont = gate_link.GetGateLinkMgr().GetCont();
			if (gate_mgr_cont.empty())
			{
				assert(false);
				continue;
			}
			for ( const auto& gate : gate_mgr_cont )
			{
				auto stage_dst = FindStage(gate.gate_dst_.stage_id_);
				if ( !stage_dst )
					continue;
				if ( warning_the_stage >= 0 )
				{
					if ( gate.gate_src_.warp_gate_id_ == the_village_id[warning_the_stage][1] )
						continue;
				}
				assert(gate.gate_src_.stage_id_ == stage->stage_id_);
				assert(gate.gate_dst_.stage_id_ != stage->stage_id_);
				if ( stage.get() == stage_dst )
				{
					assert(false);
					continue;
				}
				gate_tmp.wrap_gate_id_ = gate.gate_src_.warp_gate_id_;
				gate_tmp.next_stage_ = stage_dst;
				stage->wrap_gates_.push_back(gate_tmp);
			}
		}
	}
	return true;
}

bool GSP_Resolver::VillageMove( const std::wstring& village_id_src, const std::wstring& village_id_dst, GSP_Stage::StepsT& steps_out )
{
	if ( village_id_src == village_id_dst )
		return true;
	auto village_src = FindStage(village_id_src);
	if ( !village_src )
	{
		if ( !FillVillages() )
		{
			assert(false);
			return false;
		}
		village_src = FindStage(village_id_src);
		if ( !village_src )
		{
			assert(false);
			return false;
		}
	}
	auto village_dst = FindStage(village_id_dst);
	if ( !village_dst )
	{
		if ( !FillVillages() )
		{
			assert(false);
			return false;
		}
		village_src = FindStage(village_id_src);
		if ( !village_src )
		{
			assert(false);
			return false;
		}
		village_dst = FindStage(village_id_dst);
		if ( !village_dst )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "village dst is null, dst village->" << village_id_dst << "\tvillage src->" << village_id_src;
			assert(false);
			return false;
		}
	}
	return village_src->MoveTo(village_dst, steps_out);
}

const GSP_Stage* GSP_Resolver::FindStage( const std::wstring& stage_id )
{
	auto it = std::find_if(stages_.begin(), stages_.end(), [&stage_id]( const GSP_Stage::StagePtr& stage ){
		return stage_id == stage->stage_id_;
	});
	if ( it != stages_.end() )
		return it->get();
	return nullptr;
}

bool GSP_Stage::MoveTo( const GSP_Stage* dst_stage, StepsT& steps_out ) const
{
	if ( !dst_stage )
	{
		assert(false);
		return false;
	}
	steps_out.clear();
	if ( this == dst_stage )
		return true;
	StepsT cur_steps;
	GSP_Step step_tmp;
	for ( const auto& gate : wrap_gates_ )
	{
		assert(cur_steps.empty());
		if ( 1 == steps_out.size() )
			break;
		
		step_tmp.step_gate_ = &gate;
		step_tmp.step_stage_ = this;
		cur_steps.push_back(step_tmp);
		gate.next_stage_->MoveToNext(cur_steps, dst_stage, steps_out);
	}
	return !steps_out.empty();
}

void GSP_Stage::MoveToNext( StepsT& cur_steps, const GSP_Stage* dst_stage, StepsT& steps_out ) const
{
	GSP_Step step_tmp;
	if ( this == dst_stage )
	{
		steps_out = cur_steps;
		assert(!steps_out.empty());
		step_tmp.step_gate_ = nullptr;
		step_tmp.step_stage_ = this;
		steps_out.push_back(step_tmp);
	}
	else
	{
		auto it_end = cur_steps.end();
		auto it = std::find_if(cur_steps.begin(), it_end, [this]( const GSP_Step& step ){
			return this == step.step_stage_;
		});
		if ( it == it_end )
		{
			for ( const auto& gate : wrap_gates_ )
			{
				const auto kRealStepsCnt = steps_out.size();
				if ( kRealStepsCnt > 0 )
				{
					if ( cur_steps.size() >= kRealStepsCnt - 1 )
						break;
				}
				step_tmp.step_gate_ = &gate;
				step_tmp.step_stage_ = this;
				cur_steps.push_back(step_tmp);
				gate.next_stage_->MoveToNext(cur_steps, dst_stage, steps_out);
			}
		}
	}
	assert(!cur_steps.empty());
	cur_steps.pop_back();
}

GSP_StageEx::GSP_StageEx()
{
	is_dst_stage_ = false;
	visited_cnt_ = 0;
}

bool GSP_StageEx::MoveTo( StepsT& steps_out )
{
	steps_out.clear();
	if ( !MoveToImpl(steps_out) )
	{
		assert(false);
		steps_out.clear();
		return false;
	}
	auto it = steps_out.rbegin();
	auto it_end = steps_out.rend();
	for ( ; it != it_end; ++it )
	{
		if ( !it->is_pass_ )
			break;
	}
	auto erase_cnt = std::distance(steps_out.rbegin(), it);
	while (erase_cnt)
	{
		steps_out.pop_back();
		--erase_cnt;
	}
	if (steps_out.empty())
	{
		if ( !MoveToDst(steps_out) )
		{
			assert(false);
			return false;
		}
	}
	else
	{
		if ( !steps_out.back().step_stage_->MoveToDst(steps_out) )
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GSP_StageEx::GenRecall( const GSP_StageEx& dest_stage, GSP_StepEx& recall_step ) const
{
	for ( const auto& gate : dest_stage.wrap_gates_ )
	{
		if ( gate.next_stage_ == this )
		{
			recall_step.step_stage_ = &dest_stage;
			recall_step.step_gate_ = &gate;
			recall_step.is_pass_ = dest_stage.visited_cnt_ > 0;
			return true;
		}
	}
	assert(false);
	return false;
}

bool GSP_StageEx::MoveToImpl( StepsT& steps_out )
{
	++visited_cnt_;
	GSP_StepEx step_tmp;
	step_tmp.step_stage_ = this;
	GSP_StepEx recall_step;
	for ( auto& gate : wrap_gates_ )
	{
		if ( !gate.next_stage_ )
		{
			assert(false);
			continue;
		}
		if (gate.next_stage_->visited_cnt_)
			continue;
		if (gate.next_stage_->is_dst_stage_)
			continue;
		step_tmp.is_pass_ = std::find_if(steps_out.begin(), steps_out.end(), [this]( const GSP_StepEx& the_step ){
			return the_step.step_stage_ == this;
		}) != steps_out.end();
		step_tmp.step_gate_ = &gate;
		steps_out.push_back(step_tmp);
		if ( !gate.next_stage_->MoveToImpl(steps_out) )
			return false;
		if ( !GenRecall(*gate.next_stage_, recall_step) )
			return false;
		recall_step.is_pass_ = std::find_if(steps_out.begin(), steps_out.end(), [&recall_step]( const GSP_StepEx& the_step ){
			return the_step.step_stage_ == recall_step.step_stage_;
		}) != steps_out.end();
		steps_out.push_back(recall_step);
	}
	return true;
}

bool GSP_StageEx::MoveToDst( StepsT& steps_out ) const
{
	assert(visited_cnt_);
	GSP_StepEx step_tmp;
	step_tmp.is_pass_ = true;
	step_tmp.step_stage_ = this;
	for ( auto& gate : wrap_gates_ )
	{
		assert(gate.next_stage_);
		if (gate.next_stage_->is_dst_stage_)	
		{
			step_tmp.step_gate_ = &gate;
			if ( !steps_out.empty() )
			{
				assert(steps_out.back().step_stage_ == this);
				steps_out.pop_back();
			}
			step_tmp.is_pass_ = std::find_if(steps_out.begin(), steps_out.end(), [this]( const GSP_StepEx& the_step ){
				return the_step.step_stage_ == this;
			}) != steps_out.end();
			steps_out.push_back(step_tmp);
			return true;
		}
	}

	StepsT cur_steps, min_steps;
	for ( const auto& gate : wrap_gates_ )
	{
		assert(cur_steps.empty());
		if ( 1 == min_steps.size() )
			break;

		step_tmp.step_gate_ = &gate;
		cur_steps.push_back(step_tmp);
		if ( !gate.next_stage_->MoveToDstImpl(cur_steps, min_steps) )
			return false;
	}

	if ( !steps_out.empty() )
	{
		assert(min_steps.empty() || steps_out.back().step_stage_ == min_steps.front().step_stage_);
		steps_out.pop_back();
	}
	for ( auto& cur_step : min_steps )
	{
		cur_step.is_pass_ = std::find_if(steps_out.begin(), steps_out.end(), [&cur_step]( const GSP_StepEx& the_step ){
			return the_step.step_stage_ == cur_step.step_stage_;
		}) != steps_out.end();
		steps_out.push_back(cur_step);
	}
	return true;
}

bool GSP_StageEx::MoveToDstImpl( StepsT& cur_steps, StepsT& min_steps ) const
{
	assert(visited_cnt_);
	GSP_StepEx step_tmp;
	step_tmp.step_stage_ = this;
	step_tmp.is_pass_ = true;
	for ( auto& gate : wrap_gates_ )
	{
		assert(gate.next_stage_);
		if (gate.next_stage_->is_dst_stage_)	
		{
			min_steps = cur_steps;
			assert(!min_steps.empty());
			step_tmp.step_gate_ = &gate;
			min_steps.push_back(step_tmp);
			assert(!cur_steps.empty());
			cur_steps.pop_back();
			return true;
		}
	}

	auto it_end = cur_steps.end();
	auto it = std::find_if(cur_steps.begin(), it_end, [this]( const GSP_StepEx& step ){
		return this == step.step_stage_;
	});
	if ( it == it_end )
	{
		for ( const auto& gate : wrap_gates_ )
		{
			const auto kRealStepsCnt = min_steps.size();
			if ( kRealStepsCnt > 0 )
			{
				if ( cur_steps.size() >= kRealStepsCnt - 1 )
					break;
			}
			step_tmp.step_gate_ = &gate;
			cur_steps.push_back(step_tmp);
			VERIFY(gate.next_stage_->MoveToDstImpl(cur_steps, min_steps));
		}
	}

	assert(!cur_steps.empty());
	cur_steps.pop_back();
	return true;
}

bool GSP_ResolverEx::StageMove( const GStageTemplate& stage_template, const std::wstring& sector_src, GSP_StageEx::StepsT& steps_out )
{
	//const auto& kStartingStage = stage_template.GetStartingSectorId();
	assert(!sector_src.empty());
	auto stage_src = FindStage(sector_src);
	if ( !stage_src )
	{
		if ( !FillStages(stage_template) )
		{
			assert(false);
			return false;
		}
		stage_src = FindStage(sector_src);
		if ( !stage_src )
		{
			assert(false);
			return false;
		}
	}
	return stage_src->MoveTo(steps_out);
}

GSP_StageEx* GSP_ResolverEx::FindStage( const std::wstring& stage_id )
{
	auto it = std::find_if(stages_.begin(), stages_.end(), [&stage_id]( const GSP_StageEx& stage ){
		return stage_id == stage.stage_id_;
	});
	if ( it != stages_.end() )
		return &*it;
	return nullptr;
}

bool GSP_ResolverEx::FillStages( const GStageTemplate& stage_template )
{
	stages_.clear();
	GSP_StageEx stage_tmp;
	const auto& sectors_mgr = stage_template.GetSectors();
	for ( const auto& sector_item : sectors_mgr.GetAllNodes() )
	{
		stage_tmp.is_dst_stage_ = sector_item.IsBossRoom();
		stage_tmp.stage_id_ = sector_item.GetSectorId();
		stages_.push_back(stage_tmp);
	}
	if (stages_.empty())
	{
		assert(false);
		return false;
	}
	auto& gate_links_cont = stage_template.GetGateLinksInfo().GetCont();
	if (gate_links_cont.empty())
	{
		//assert(false);
		return true;
	}
	GSP_GateEx gate_tmp;
	for ( auto& stage : stages_ )
	{		
		for ( const auto& gate_link : gate_links_cont )
		{
			const auto& gate_mgr_cont = gate_link.GetGateLinkMgr().GetCont();
			if (gate_mgr_cont.empty())
			{
				assert(false);
				continue;
			}
			for ( const auto& gate : gate_mgr_cont )
			{
				if ( gate.gate_src_.sector_id_ != stage.stage_id_ )
					continue;
				auto stage_dst = FindStage(gate.gate_dst_.sector_id_);
				if ( !stage_dst )
				{
					assert(false);
					continue;
				}
				assert(gate.gate_src_.sector_id_ == stage.stage_id_);
				assert(gate.gate_dst_.sector_id_ != stage.stage_id_);
				if ( &stage == stage_dst )
				{
					assert(false);
					continue;
				}
				gate_tmp.wrap_gate_id_ = gate.gate_src_.warp_gate_id_;
				gate_tmp.next_stage_ = stage_dst;
				stage.wrap_gates_.push_back(gate_tmp);
				if ( std::find_if(stage_dst->wrap_gates_.begin(), stage_dst->wrap_gates_.end(), [&gate]( const GSP_GateEx& the_gate ){
					return the_gate.wrap_gate_id_ == gate.gate_dst_.warp_gate_id_;
				} ) 
					== stage_dst->wrap_gates_.end() )
				{
					gate_tmp.wrap_gate_id_ = gate.gate_dst_.warp_gate_id_;
					gate_tmp.next_stage_ = &stage;
					stage_dst->wrap_gates_.push_back(gate_tmp);
				}
			}
		}
	}
	return true;
}
