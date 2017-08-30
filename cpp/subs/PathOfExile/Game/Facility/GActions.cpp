#include "stdafx.h"
#include "GActions.h"
#include "GPlayer.h"
#include "GMap.h"
#include "GObjFilter.h"
#include <DllApp.h>
#include "GProtocolR.h"

CA_RunRes GA_TraFullMap::OnRun()
{
	CA_RunRes run_res;
	clear_mark_pos_cnt_ = 0;
	while (true)
	{
		run_res = __super::OnRun();
		if (run_res >= kRR_Succeed)
			return run_res;
		if (!MakeAssistActions())
		{
			assert(false);
			return kRR_Failed;
		}

		do_other_res_ = kDOR_Failed;
		assert(ensure_tester_);
		run_res = ensure_tester_->Run();
		if (ensure_tester_->IsTesterError(run_res))
			continue;
		if (do_other_res_ == kDOR_Continue)
			continue;
		else if (do_other_res_ == kDOR_Failed)
			return kRR_Failed;
		else if (do_other_res_ == kDOR_Over)
		{
			//最后再执行一次
			run_res = __super::OnRun();
			if (run_res >= kRR_Succeed)
				return run_res;
			break;
		}
	}
	LOG_O(Log_debug) << "遍历全图之已全部遍历完毕";
	return kRR_Succeed;
}

GA_TraFullMap::GA_TraFullMap(bool clear_mark_pos, float tra_map_rate, int remaining)
{
	do_other_res_ = kDOR_Failed;
	clear_mark_pos_cnt_ = 0;
	clear_mark_pos_ = clear_mark_pos;
	tra_map_rate_ = tra_map_rate;
	remaining_ = remaining;
}

GA_TraFullMap::enDoOtherRes GA_TraFullMap::DoOther()
{
	auto& path_mark = GPathMark::GetMe();
	if (tra_map_rate_ >= 0)
	{
		auto cur_map_rate = path_mark.GetCurMovableRate();
		if (cur_map_rate >= tra_map_rate_)
		{
			LOG_O(Log_trace) << "已达地图遍历百分比,cur_map_rate:" << cur_map_rate << ",tra_map_rate:" << tra_map_rate_;
			return kDOR_Over;
		}
	}
	if (remaining_ >= 0)
	{
		auto cur_remaining = GInterface::GetRemaining();
		if (cur_remaining >= 0 && cur_remaining <= remaining_)
		{
			LOG_O(Log_trace) << "剩余怪物数量已满足,cur_remaining:" << cur_remaining << ",remaining:" << remaining_;
			return kDOR_Over;
		}
	}
	auto& map = GMap::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&map, &gpm](){
		if (!map.Update())
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
		return kDOR_Failed;
	}
	auto my_pos = gpm.GetPos();
	if (!my_pos)
	{
		assert(false);
		return kDOR_Failed;
	}
	stCD_VecInt suggest_pos;
	if (!path_mark.GetSuggestPos(*my_pos, suggest_pos))
	{
		LOG_O(Log_debug) << "遍历全图之没有建议的坐标了";
		if (gpm.OnTraMapOver())
		{
			DummySleep(1000);
			return kDOR_Continue;
		}
		++clear_mark_pos_cnt_;
		if (clear_mark_pos_cnt_ > (int)clear_mark_pos_)
			return kDOR_Over;
		LOG_O(Log_debug) << "遍历全图之第一遍没完成，清空MarkPos，再次遍历，已遍历次数：" << clear_mark_pos_cnt_;
		map.ClearAllMarkInfo();
		DummySleep(1000);
		return kDOR_Continue;
	}
	if (action_move_to_)
		action_move_to_->SetDstPos(suggest_pos);
	else
	{
		action_move_to_ = GA_Factory::GetMe().MakeMoveTo(suggest_pos);
		if (!action_move_to_)
		{
			assert(false);
			return kDOR_Failed;
		}
		action_move_to_->SetParent(shared_from_this());
	}
	LOG_O(Log_trace) << "遍历全图之得到了建议坐标：" << suggest_pos;
	action_move_to_->Run();
	//一定要先标记记忆，否则有可能会产生跑来跑去的BUG。
	//上面的注释是之前的理解，放到后面标记即可，因为移动失败时会标记的。
	path_mark.MarkPos(suggest_pos);
	return kDOR_Continue;
}

bool GA_TraFullMap::MakeAssistActions()
{
	if (ensure_tester_)
	{
		ensure_tester_->clear();
		auto consumer = Consumer();
		if (consumer)
			ensure_tester_->AddEnsure(consumer);
		ensure_tester_->AddEnsure(shared_from_this());
		return true;
	}
	auto action_do_other = GA_Factory::MakeLambda([this](){
		do_other_res_ = DoOther();
		return kRR_Succeed;
	});
	if (!action_do_other)
	{
		assert(false);
		return false;
	}
	ensure_tester_ = GA_Factory::MakeAction<CA_EnsureTester>();
	if (!ensure_tester_)
	{
		assert(false);
		return false;
	}
	ensure_tester_->Add(action_do_other);
	auto consumer = Consumer();
	if (consumer)
		ensure_tester_->AddEnsure(consumer);
	ensure_tester_->AddEnsure(shared_from_this());
	return true;
}

GA_MoveTo::GA_MoveTo(const stCD_VecInt& dst_pos) : dst_pos_(dst_pos)
{

}

GA_MoveTo::enMoveRes GA_MoveTo::SmartMove(bool is_small_astar)
{
	auto& io_s = GetIoService();
	io_s.PollOne();
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return kMR_Error;
	}
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return kMR_Error;
	}
	PosContT path_pos;
	auto pos_src = *player_pos;
	//LOG_O(Log_debug) << "开始计算A星，起始坐标：" << pos_src << ",目标坐标：" << old_dst_pos << "距离：" << pos_src.Distance(old_dst_pos);
	if (!GenPath(is_small_astar, pos_src, path_pos))
	{
		//assert(false);
		return kMR_AStarError;
	}
	/*if (path_pos.empty())
	{
		assert(false);
		return kMR_Succeed;
	}*/
	for (auto it = path_pos.begin(), it_end = path_pos.end(); it < it_end; ++it)
	{
		io_s.PollOne();
		auto& the_pos = *it;
		auto mov_res = MoveTo(the_pos);
		if (mov_res < kMR_Succeed)
		{
			auto& path_mark = GPathMark::GetMe();
			path_mark.MarkPos(dst_pos_);
			path_mark.MarkPos(the_pos);
			if (mov_res != kMR_PlayerMoveError)
				return mov_res;
			if (!obj_mgr_)
			{
				obj_mgr_.reset(new GameObjMgr(kGOT_Transitonable));
				if (!obj_mgr_)
				{
					assert(false);
					return mov_res;
				}
			}
			if (!GWndExecSync([&gpm, this](){
				if (!obj_mgr_->RebuildAll())
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
				return mov_res;
			}
			player_pos = gpm.GetPos();
			if (!player_pos)
			{
				assert(false);
				return kMR_Error;
			}
			obj_mgr_->SortByPos(*player_pos);
			auto the_obj = obj_mgr_->GetFirstObj();
			if (the_obj)
			{
				if (the_obj->Distance(dst_pos_) <= 0)
					return mov_res;
				if (the_obj->Distance(*player_pos) > 15)
				{
					obj_mgr_->SortByPos(the_pos);
					the_obj = obj_mgr_->GetFirstObj();
					assert(the_obj);
					if (the_obj->Distance(the_pos) > 10)
						the_obj.reset();
				}
				if (the_obj && the_obj->MovingDoTryOpen())
				{
					if (TryOpenObj(the_obj, is_small_astar))
						return kMR_DistError;
				}				
			}
			LOG_O(Log_debug) << "移动失败,enMoveRes：" << mov_res;
			//assert(false);
			return mov_res;
		}
	}
	return kMR_Succeed;
}

GA_MoveTo::enMoveRes GA_MoveTo::MoveTo(const stCD_VecInt& dst_pos)
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return kMR_Error;
	}
	if (player_pos->Distance(dst_pos) > kValidDistance)
	{
		LOG_O(Log_debug) << "一次性移动的距离过远:" << player_pos->Distance(dst_pos);
		//assert(false);
		return kMR_DistError;
	}
	auto old_pos = *player_pos;
	enMoveRes mov_res = kMR_Succeed;
	if (!GInterface::PlayerMove(dst_pos))
	{
		if (!G_AStar::GetMe().CanLineMoveTo(old_pos, dst_pos))
		{
			LOG_O(Log_debug) << "目标点不能移动过去：" << dst_pos;
			mov_res = kMR_PlayerMoveError;
		}
	}
#ifdef VER_GF_
	auto skill_id = CD_Interface::GetSkillIdMove();
	GUseSkill use_skill(skill_id, dst_pos);
	use_skill.UseSkill(true);
	BOOST_SCOPE_EXIT_ALL(&use_skill){
		use_skill.UseSkillEnd(0);
	};
#endif
	//GRecvMsgHandler::GetMe().BeginMoveSkillOper(skill_id);
	//////////////////////////////////////////////////////////////////////////
	bool failed = false;
	if (!TimerDo(10, 10 * 1000, [&failed](){
		return OnTimerDo(&failed);
	}))
	{
		LOG_O(Log_debug) << "出现了游戏里的bug？卡来卡去的那种？";
		return kMR_Succeed;
	}
	//////////////////////////////////////////////////////////////////////////
	if (failed)
		return kMR_Error;
	player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return kMR_Error;
	}
	if (player_pos->Distance(dst_pos) > kValidDistance)
	{
		LOG_O(Log_debug) << "移动到目标的误差太大:" << player_pos->Distance(dst_pos);
		return kMR_DistError;
	}
	/*if (mov_res == kMR_Succeed && old_pos.Distance(dst_pos) > 10)
	{
		if (old_pos == *player_pos)
		{
			LOG_O(Log_debug) << "移动失败，坐标没有变化";
			return kMR_PlayerMoveError;
		}
	}*/
	return mov_res;
}

void GA_MoveTo::SetDstPos(const stCD_VecInt& dst_pos)
{
	dst_pos_ = dst_pos;
}

bool GA_MoveTo::OnTimerDo(bool* failed)
{
	auto& gpm = GPlayerMe::GetMe();
	struct stHelp{
		stHelp(){
			auto ptr = new GameObjMgr;
			obj_mgr_remembered.reset(ptr);
			if (!obj_mgr_remembered)
			{
				assert(false);
				return;
			}
			ptr->AddFilterType(kGOT_Npc);
			ptr->AddFilterType(kGOT_Transitonable);
			ptr->IncludeNoHumanFilter();
		}

		GameObjMgrPtr obj_mgr_remembered;
	};
	static stHelp hlp;
	auto obj_mgr = hlp.obj_mgr_remembered;
	if (!obj_mgr)
	{
		assert(false);
		return true;
	}
	obj_mgr->clear();
	if (!GWndExecSync([&gpm, obj_mgr](){
		if (!obj_mgr->RebuildAll())
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
		if (failed)
			*failed = true;
		assert(false);
		return true;
	}
	//本来想在此处设置对象的AreaId的，可是考虑有些门或许真的与角色所在房间不一样，而又能够遍历到的情况，所以就没搞了。
	/*for (auto& obj : obj_mgr->GetObjs())
	{

	}*/
	gpm.RememberObjs(*obj_mgr);
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (failed)
			*failed = true;
		assert(false);
		return true;
	}
	if (!GPathMark::GetMe().MarkPos(*player_pos))
	{
		assert(false);
	}
	//不需要判断收包，否则像是国际服很卡的情况，就会有可能返回失败
	/*if (GRecvMsgHandler::GetMe().GetMoveSkillRes() == kUSR_DstNotValid)
	{
		if (failed)
			*failed = true;
	}*/
	return !gpm.IsMoving();
}

CA_RunRes GA_MoveTo::OnRun()
{
	GetIoService().PollOne();
	auto& gpm = GPlayerMe::GetMe();
	auto& map = GMap::GetMe();
	if (!GWndExecSync([&gpm, &map](){
		if (!gpm.Update())
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
		return kRR_Failed;
	}
		
	auto my_pos = gpm.GetPos();
	if (!my_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	int dist_err_cnt = 0, player_move_err_cnt = 0, astar_err_cnt = 0;
	stCD_VecInt old_my_pos;
	while (true)
	{
		MoveToArea(true);
		old_my_pos = *my_pos;
		auto mov_res = SmartMove(player_move_err_cnt > 0);
		if (mov_res != kMR_AStarError)
			astar_err_cnt = 0;
		if (mov_res != kMR_DistError)
			dist_err_cnt = 0;
		if (mov_res != kMR_PlayerMoveError)
			player_move_err_cnt = 0;

		if (mov_res >= kMR_Succeed)
			return kRR_Succeed;
		else if (mov_res == kMR_Error)
			return kRR_Failed;
		else if (mov_res == kMR_DistError)
		{
			++dist_err_cnt;
			if (dist_err_cnt >= 4)
				return kRR_Failed;
			continue;
		}
		if (!gpm.Update())
		{
			assert(false);
			return kRR_Failed;
		}
		my_pos = gpm.GetPos();
		if (!my_pos)
		{
			assert(false);
			return kRR_Failed;
		}
		if (mov_res == kMR_PlayerMoveError)
		{
			/*if (my_pos->Distance(old_my_pos) >= 8)
			{
				continue;
			}*/
			++player_move_err_cnt;
			if (player_move_err_cnt <= 3)
				continue;
			LOG_O(Log_debug) << "PlayerMove移动失败次数过多";
			return kRR_Failed;
		}
		assert(mov_res == kMR_AStarError);
		++astar_err_cnt;
		if (astar_err_cnt > 10)
		{
			LOG_O(Log_debug) << "astar失败次数过多";
			return kRR_Failed;
		}
		if (!MoveToArea(false))
			return kRR_Failed;
	}
	assert(false);
	return kRR_Failed;
}

bool GA_MoveTo::GenPath(bool is_small_astar, const stCD_VecInt& pos_src, PosContT& out_pos_info)
{
	G_AStar::PosContT pos_info_tmp;
	if (!GenPathImpl(is_small_astar, pos_src, pos_info_tmp))
	{
		LOG_O(Log_debug) << "A星寻路之生成路径失败，源坐标：" << pos_src << ",目标坐标：" << dst_pos_ << ",is_small_astar:" << is_small_astar;
		return false;
	}
	out_pos_info.reserve(pos_info_tmp.size());
	for (auto& v : pos_info_tmp)
		out_pos_info.push_back(v);
	return true;
}

bool GA_MoveTo::GenPathImpl(bool is_small_astar, const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info)
{
	if (is_small_astar)
	{
		auto small_astar = GMap::GetMe().GetSmallAStar(pos_src);
		if (!small_astar)
		{
			assert(false);
			return false;
		}
		if (!small_astar->GenFindPath(pos_src, dst_pos_, out_pos_info))
		{
			LOG_O(Log_debug) << "SmallAstar计算路径失败1";
			return false;
		}
		return true;
	}
	if (!G_AStar::GetMe().GenFindPath(pos_src, dst_pos_, out_pos_info))
	{
		LOG_O(Log_debug) << "A星计算路径失败1";
		return false;
	}
	return true;
}

bool GA_MoveTo::TryOpenObj(const GameObjBasePtrT& the_obj, bool is_small_astar)
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	if (is_small_astar)
	{
		if (!TryMoveTo(*player_pos, the_obj))
			return false;
	}
	LOG_O(Log_debug) << "移动时发现有门要开，先去开门";
	if (gpm.MoveToOpenObj(Consumer(), the_obj) < kOVR_Succeed)
		return false;
	return true;
}

bool GA_MoveTo::TryMoveTo(const stCD_VecInt& pos, const GameObjBasePtrT& the_obj)
{
	assert(the_obj);
	auto dst_pos = the_obj->GetPos();
	if (!dst_pos)
	{
		assert(false);
		return false;
	}
	auto small_astar = GMap::GetMe().GetSmallAStar(pos);
	if (!small_astar)
		return false;
	SmallAStar::PosContT pos_cont;
	if (!small_astar->GenFindPath(pos, *dst_pos, pos_cont))
		return false;
	return true;
}

bool GA_MoveTo::MoveToArea(bool pre_smart_move)
{
	if (dst_pos_.AnyLess(1)) return false;
	return GPlayerMe::GetMe().EnsureMoveToTheArea([this](GAreaStepsType& steps){
		auto& tra_policy = GMapTraMgr::GetMe().GetTraGuess();
		//if (!tra_policy)
		//{
		//	assert(false);
		//	return false;
		//}
		auto my_pos = GPlayerMe::GetMe().GetPos();
		if (!my_pos)
		{
			assert(false);
			return false;
		}
		assert(dst_pos_.AnyGreater(0));
		tra_policy.GenAreaSteps(*my_pos, dst_pos_, steps);
		if (steps.empty())
			return false;
		return true;
	});
}

GA_MoveToObjByObj::GA_MoveToObjByObj(const GameObjBasePtrT& game_obj)
	: game_obj_(game_obj)
{
	//clear_mark_pos_ = clear_mark_pos;
	if (game_obj)
	{
		obj_mgr_ = game_obj->GetOwner();
		if (!obj_mgr_)
		{
			obj_mgr_.reset(new GameObjMgr(game_obj->GetObjType()));
			if (!obj_mgr_)
			{
				assert(false);
				return;
			}
		}
		tester_any_obj_ = GA_Factory::GetMe().MakeTesterAnyObj(obj_mgr_);
		assert(tester_any_obj_);
	}
	else
		assert(false);
}

CA_RunRes GA_MoveToObjByObj::OnRun()
{
	if (!game_obj_)
	{
		assert(false);
		return kRR_Failed;
	}
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed;
	}
	if (!tester_any_obj_)
	{
		assert(false);
		return kRR_Failed;
	}
	FilterGameObjId	filter_id(obj_mgr_->GetFilter(), game_obj_->GetGameId());
	auto dst_pos = game_obj_->GetPos();
	if (!dst_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	auto& factory = GA_Factory::GetMe();
	if (action_move_to_)
		action_move_to_->SetDstPos(*dst_pos);
	else
	{
		action_move_to_ = factory.MakeMoveToNearly(*dst_pos);
		if (!action_move_to_)
		{
			assert(false);
			return kRR_Failed;
		}
	}	
	if (as_timer_action_)
		as_timer_action_->Add(action_move_to_);
	else
	{
		as_timer_action_ = factory.MakeAsTimerAction(action_move_to_, tester_any_obj_);
		if (!as_timer_action_)
		{
			assert(false);
			return kRR_Failed;
		}
		tester_any_obj_->SetConsumer(Consumer());
	}
	auto run_res = as_timer_action_->Run();
	if (run_res == tester_any_obj_)
	{
		game_obj_ = tester_any_obj_->GetFirstObj();
		if (!game_obj_)
		{
			assert(false);
			return kRR_Failed;
		}
		dst_pos = game_obj_->GetPos();
		if (!dst_pos)
		{
			assert(false);
			return kRR_Failed;
		}
		action_move_to_->SetDstPos(*dst_pos);
		return action_move_to_->Run();
	}
	return HandleRunRes(game_obj_, run_res);
	/*if (!tra_full_map_)
	{
		tra_full_map_ = factory.MakeTraFullMap(Consumer(), tester_any_obj_, clear_mark_pos_);
		if (!tra_full_map_)
		{
			assert(false);
			return kRR_Failed;
		}
	}
	as_timer_action_->Add(tra_full_map_);
	tester_any_obj_->clear();
	run_res = as_timer_action_->Run();
	if (run_res.error_action_ != tester_any_obj_)
		return kRR_Failed;
	game_obj_ = tester_any_obj_->GetFirstObj();
	if (!game_obj_)
	{
		assert(false);
		return kRR_Failed;
	}
	dst_pos = game_obj_->GetPos();
	if (!dst_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	action_move_to_->SetDstPos(*dst_pos);
	return HandleRunRes(game_obj_, action_move_to_->Run());*/
}

GameObjBasePtrT GA_MoveToObjByObj::GetTheObj() const
{
	return game_obj_;
}

GA_MoveToObjByObj::enRunRes GA_MoveToObjByObj::HandleRunRes(const GameObjBasePtrT& dst_obj, const CA_RunRes& run_res)
{
	if (run_res != kRR_Failed)
		return run_res;
	if (!dst_obj)
	{
		assert(false);
		return kRR_Failed;
	}
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	LOG_O(Log_debug) << "MoveToTheObj之找到了对象, 但是移动到该目标时返回失败,对象名：" << dst_obj->GetCnName();
	if (!dst_obj->IsTransitonable())
		return kRR_Failed;
	if (dst_obj->Distance(*player_pos) > 25)
		return kRR_Failed;
	auto obj_pos = dst_obj->GetPos();
	if (!obj_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	if (G_AStar::GetMe().TestCanMove(*obj_pos))
		return kRR_Failed;
	return kRR_Succeed;
}

CA_T<GA_MoveTo> GA_Factory::MakeMoveTo(const stCD_VecInt& dst_pos)
{
	return CA_T<GA_MoveTo>(new GA_MoveTo(dst_pos));
}

CA_ActionPtr GA_Factory::MakeTraFullMap(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do,
	bool clear_mark_pos, float tra_map_rate, int remaining)
{
	if (!action_tra_do)
	{
		assert(false);
		return nullptr;
	}
	CA_ActionPtr res(new GA_TraFullMap(clear_mark_pos, tra_map_rate, remaining));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->Add(action_tra_do);
	res->SetConsumer(consumer);
	return res;
}

CA_ActionPtr GA_Factory::MakeTraGuessArea(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do, bool clear_mark_pos)
{
	if (!action_tra_do)
	{
		assert(false);
		return nullptr;
	}
	CA_ActionPtr res(new GA_TraGuessArea(clear_mark_pos));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->Add(action_tra_do);
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_TesterAnyObj> GA_Factory::MakeTesterAnyObj(const GameObjMgrPtr& obj_mgr)
{
	if (!obj_mgr)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_TesterAnyObj> res(new GA_TesterAnyObj(obj_mgr));
	return res;
}

CA_T<GA_TesterAnyPosByName> GA_Factory::MakeTesterAnyObj(const std::string& pos_name)
{
	if (pos_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_TesterAnyPosByName> res(new GA_TesterAnyPosByName(pos_name));
	return res;
}

CA_T<CA_AsTimerAction> GA_Factory::MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action)
{
	return GetMyApp().GetAppFactory().MakeAsTimerAction(decoration, timer_action);
}

CA_T<GA_MoveToObjByObj> GA_Factory::MakeMoveToObj(const GA_SmartConsumerT& consumer, const GameObjBasePtrT& obj)
{
	if (!obj)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_MoveToObjByObj> res(new GA_MoveToObjByObj(obj));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_MoveToObjByName> GA_Factory::MakeMoveToObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const std::string& obj_name, const GameObjMgrPtr& obj_mgr, bool remem_find)
{
	if (obj_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_MoveToObjByName> res(new GA_MoveToObjByName(clear_mark_pos, obj_name, obj_mgr,remem_find));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_MoveToPosByName> GA_Factory::MakeMoveToPos(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const std::string& obj_name)
{
	if (obj_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_MoveToPosByName> res(new GA_MoveToPosByName(clear_mark_pos, obj_name));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_KillBase> GA_Factory::MakeKillMonster(const GameObjBasePtrT& game_obj)
{
	if (!game_obj)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_KillBase> res(new GA_DangerKill);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetDstObj(game_obj);
	return res;
}

CA_T<GA_TesterNearest> GA_Factory::MakeTesterNearest(const GameObjBasePtrT& dst_obj)
{
	if (!dst_obj)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_TesterNearest> res(new GA_TesterNearest(dst_obj));
	return res;
}

CA_ActionPtr GA_Factory::MakeKillMonsters(const GameObjMgrPtr& obj_mgr)
{
	CA_ActionPtr res(new GA_KillMonsters(obj_mgr));
	return res;
}

CA_ActionPtr GA_Factory::MakeKillUntil(const GA_SmartConsumerT& consumer, bool clear_mark_pos, 
	float tra_map_rate, int remaining, const CA_ActionPtr& fast_tester)
{
	CA_ActionPtr res(new GA_KillUntil(clear_mark_pos, tra_map_rate, remaining, fast_tester));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_TesterByName> GA_Factory::MakeTesterByName(const std::string& obj_name, const GameObjMgrPtr& obj_mgr)
{
	if (obj_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_TesterByName> res(new GA_TesterByName(obj_name, obj_mgr));
	return res;
}

CA_T<GA_TesterInRange> GA_Factory::MakeTesterInRange(const CA_T<GA_OpenObjs>& open_objs)
{
	CA_T<GA_TesterInRange> res(new GA_TesterInRange(open_objs));
	return res;
}

CA_T<GA_OpenObjs> GA_Factory::MakeOpenObjs(const CA_ActionPtr& consumer)
{
	static CA_WeakT<GA_OpenObjs> single_open_objs;
	if (!single_open_objs.expired())
		return nullptr;
	if (!consumer)
	{
		assert(false);
		return nullptr;
	}
	CmnStaticVector<enGameObjType, kGOT_Max> objs_type;
	auto& gpm = GPlayerMe::GetMe();
	if (gpm.auto_open_waypoint_)
		objs_type.push_back(kGOT_Waypoint);
	if (gpm.auto_open_transitionable_)
		objs_type.push_back(kGOT_Transitonable);
	if (gpm.auto_open_chest_ & kOCF_Enable)
		objs_type.push_back(kGOT_Chest);
	if (gpm.auto_pickup_item_)
		objs_type.push_back(kGOT_WorldItem);
	if (objs_type.empty())
		return nullptr;
	auto obj_mgr_ptr = new GameObjMgr;
	GameObjMgrPtr obj_mgr(obj_mgr_ptr);
	if (!obj_mgr_ptr)
	{
		assert(false);
		return nullptr;
	}
	for (auto t : objs_type)
		obj_mgr_ptr->AddFilterType(t);
	CA_T<GA_OpenObjs> res(new GA_OpenObjs(obj_mgr));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	single_open_objs = res;
	return res;
}

CA_T<GA_KillInRange> GA_Factory::MakeKillInRange(const CA_ActionPtr& consumer, const CA_T<GA_OpenObjs>& open_objs)
{
	if (!consumer)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_KillInRange> tmp(new GA_KillInRange(open_objs));
	if (!tmp)
	{
		assert(false);
		return nullptr;
	}
	tmp->SetConsumer(consumer);
	return tmp;
}

GA_SmartConsumerT GA_Factory::MakeSmartConsumer()
{
	return MakeAction<CA_SmartConsumer>();
}

CA_T<GA_KillNpcMonster> GA_Factory::MakeKillNpcMonster(const CA_ActionPtr& consumer, const std::string& npc_name)
{
	if (!consumer || npc_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_KillNpcMonster> res(new GA_KillNpcMonster(npc_name));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	res->SetConsumer(consumer);
	return res;
}

CA_T<GA_LookMonsterDie> GA_Factory::MakeTesterLookMonsterDie(const std::string& monster_name)
{
	if (monster_name.empty())
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_LookMonsterDie> res(new GA_LookMonsterDie(monster_name));
	return res;
}

CA_T<GA_TesterDanger> GA_Factory::MakeTesterDanger(const GameObjMgrPtr& obj_mgr)
{
	if (!obj_mgr)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_TesterDanger> res(new GA_TesterDanger(obj_mgr));
	return res;
}

CA_T<GA_DangerMoveTo> GA_Factory::MakeDangerMoveTo(const stCD_VecInt& dst_pos, const GameObjMgrPtr& obj_mgr)
{
	if (!obj_mgr)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_DangerMoveTo> res(new GA_DangerMoveTo(dst_pos, obj_mgr));
	return res;
}

CA_T<GA_SkillBase> GA_Factory::MakeSkill(const std::string& skill_name)
{
	return CA_T<GA_SkillBase>(new GA_SkillToHittable(skill_name));
}

CA_T<GA_Skills> GA_Factory::MakeSkills()
{
	return CA_T<GA_Skills>(new GA_Skills);
}

CA_T<GA_SaferMoveTo> GA_Factory::MakeSaferMoveTo(const GameObjMgrPtr& obj_mgr)
{
	if (!obj_mgr)
	{
		assert(false);
		return nullptr;
	}
	CA_T<GA_SaferMoveTo> res(new GA_SaferMoveTo(obj_mgr));
	return res;
}

CA_T<GA_SkillBase> GA_Factory::MakeCorpseSkill(const std::string& skill_name)
{
	return CA_T<GA_SkillBase>(new GA_SkillToCorpse(skill_name));
}

CA_T<GA_MoveToNearly> GA_Factory::MakeMoveToNearly(const stCD_VecInt& dst_pos)
{
	return CA_T<GA_MoveToNearly>(new GA_MoveToNearly(dst_pos));
}

GA_TesterAnyObj::GA_TesterAnyObj(const GameObjMgrPtr& obj_mgr) : obj_mgr_(obj_mgr)
{
	assert(obj_mgr);
}

GameObjBasePtrT& GA_TesterAnyObj::GetFirstObj()
{
	return first_obj_;
}

bool GA_TesterAnyObj::Test()
{
	if (first_obj_)
	{
		//assert(false);
		return true;
	}
	if (!obj_mgr_)
	{
		assert(false);
		return true;
	}
	if (!obj_mgr_->RebuildAll())
	{
		assert(false);
		return true;
	}
	if (obj_mgr_->empty())
		return false;
	first_obj_ = obj_mgr_->GetFirstObj();
	return true;
}

void GA_TesterAnyObj::clear()
{
	first_obj_.reset();
}

GA_MoveToObjByName::GA_MoveToObjByName(bool clear_mark_pos, const std::string& obj_name,
	const GameObjMgrPtr& obj_mgr, bool remem_find)
	: obj_name_(obj_name), obj_mgr_(obj_mgr)
{
	clear_mark_pos_ = clear_mark_pos;
	assert(!obj_name.empty());
	remem_find_ = remem_find;
}

CA_RunRes GA_MoveToObjByName::OnRun()
{
	if (!dst_obj_)
	{
		dst_obj_ = GPlayerMe::GetMe().GetRememberedObjs().FindByName(obj_name_);
		if (dst_obj_ && remem_find_)
		{
			auto action = GA_Factory::GetMe().MakeMoveToObj(Consumer(), dst_obj_);
			if (!action)
			{
				assert(false);
				return kRR_Failed;
			}
			return action->Run();
		}
		if (!obj_mgr_)
		{
			auto obj_mgr_ptr = new GameObjMgr();
			obj_mgr_.reset(obj_mgr_ptr);
			if (!obj_mgr_)
			{
				assert(false);
				return kRR_Failed;
			}
			obj_mgr_ptr->IncludeOpenableFilter();
			obj_mgr_ptr->IncludeLifedFilter();
		}
		if (!tester_any_obj_)
		{
			tester_any_obj_ = GA_Factory::GetMe().MakeTesterAnyObj(obj_mgr_);
			if (!tester_any_obj_)
			{
				assert(false);
				return kRR_Failed;
			}
			tester_any_obj_->SetConsumer(Consumer());
		}
		if (!action_detect_)
		{
			auto tra_policy = GMapTraMgr::GetMe().GetCurPolicy();
			if (!tra_policy)
			{
				assert(false);
				return kRR_Failed;
			}
			auto area = tra_policy->FindAreaByName(obj_name_);
			if (area)
			{
				LOG_O(Log_debug) << "找到记录目标(" << obj_name_ << ")位置-----------" << area->GetValidPos();
				if (action_move_to_)
					action_move_to_->SetDstPos(area->GetValidPos());
				else
				{
					action_move_to_ = GA_Factory::GetMe().MakeMoveToNearly(area->GetValidPos());
					if (!action_move_to_)
					{
						assert(false);
						return kRR_Failed;
					}
				}
				action_detect_ = action_move_to_;
			}
			else
			{
				LOG_O(Log_debug) << "没有找到记录目标(" << obj_name_ << ")------------";
				action_detect_ = tra_policy->MakeTraMap(Consumer(), tester_any_obj_, clear_mark_pos_);
				if (!action_detect_)
				{
					assert(false);
					return kRR_Failed;
				}
			}
		}
		FilterGameObjName filter_name(obj_mgr_->GetFilter(), obj_name_);
		if (!as_timer_action_)
		{
			as_timer_action_ = GA_Factory::GetMe().MakeAsTimerAction(action_detect_, tester_any_obj_);
			if (!as_timer_action_)
			{
				assert(false);
				return kRR_Failed;
			}
		}
		auto run_res = as_timer_action_->Run();
		if (run_res)
		{
			if (!obj_mgr_->RebuildAll())
			{
				assert(false);
				return kRR_Failed;
			}
			dst_obj_ = obj_mgr_->FindByName(obj_name_);
		}
		else if (run_res == tester_any_obj_)
		{
			dst_obj_ = tester_any_obj_->GetFirstObj();
		}
		else
			return kRR_Failed;
		if (!dst_obj_)
		{
			assert(false);
			return kRR_Failed;
		}
	}	
	auto dst_pos = dst_obj_->GetPos();
	if (!dst_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	if (action_move_to_)
		action_move_to_->SetDstPos(*dst_pos);
	else
	{
		action_move_to_ = GA_Factory::GetMe().MakeMoveToNearly(*dst_pos);
		if (!action_move_to_)
		{
			assert(false);
			return kRR_Failed;
		}
	}
	return GA_MoveToObjByObj::HandleRunRes(dst_obj_, action_move_to_->Run());
}

GameObjBasePtrT GA_MoveToObjByName::GetTheObj() const
{
	return dst_obj_;
}

GA_KillMonsters::GA_KillMonsters(const GameObjMgrPtr& obj_mgr) : obj_mgr_(obj_mgr)
{
	if (!obj_mgr_)
	{
		obj_mgr_.reset(new GameObjMgr(kGOT_Monster));
		if (!obj_mgr_)
		{
			assert(false);
			return;
		}
	}
	filter_alive_.reset(new FilterGameObjHittable(obj_mgr_->GetFilter()));
	filter_hittable_.reset(new FilterGameObjAlive(obj_mgr_->GetFilter()));
	filter_black_name_list_.reset(FilterGameObj_MakeLambda([this](const GameObjBasePtrT& obj){
		if (black_name_list_.empty()) return true;
		assert(obj);
		auto obj_id = obj->GetGameId();
		for (auto& v : black_name_list_)
		{
			if (v.obj_->GetGameId() == obj_id)
				return false;
		}
		return true;
	}, obj_mgr_->GetFilter()));	
}

CA_RunRes GA_KillMonsters::OnRun()
{
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed;
	}
	DecBlackNameList();
	auto& gpm = GPlayerMe::GetMe();
	CA_RunRes run_res;
	while (true)
	{
		if (!GWndExecSync([&gpm, this](){
			if (!gpm.Update())
			{
				assert(false);
				return false;
			}
			if (!obj_mgr_->RebuildAll())
			{
				assert(false);
				return false;
			}
			return true;
		}))
		{
			assert(false);
			return kRR_Failed;
		}
		auto player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return kRR_Failed;
		}
		obj_mgr_->SortByWeightDistance(*player_pos);
		auto the_obj = obj_mgr_->GetFirstObj();
		if (!the_obj)
			return kRR_Succeed;
		//LOG_O(Log_trace) << "KillMonsters，准备杀死这个怪：" << the_obj->GetCnName() << ",id:" << the_obj->GetGameId();
		//////////////////////////////////////////////////////////////////////////
		if (action_kill_monster_)
			action_kill_monster_->SetDstObj(the_obj);
		else
		{
			action_kill_monster_ = GA_Factory::GetMe().MakeKillMonster(the_obj);
			if (!action_kill_monster_)
			{
				assert(false);
				return kRR_Failed;
			}
		}
		if (action_tester_nearest_)
			action_tester_nearest_->SetDstObj(the_obj);
		else
		{
			action_tester_nearest_ = GA_Factory::GetMe().MakeTesterNearest(the_obj);
			if (!action_tester_nearest_)
			{
				assert(false);
				return kRR_Failed;
			}
			action_tester_nearest_->SetConsumer(Consumer());
		}
		if (!as_timer_action_)
		{
			as_timer_action_ = GA_Factory::GetMe().MakeAsTimerAction(action_kill_monster_, action_tester_nearest_);
			if (!as_timer_action_)
			{
				assert(false);
				return kRR_Failed;
			}
		}
		run_res = as_timer_action_->Run();
		/*if (run_res == action_tester_nearest_)
			continue;		//此时不能清空黑名单，否则会有死循环
		else */
		if (run_res < kRR_Succeed)
			AddBlackNameList(the_obj);
		else
			DecBlackNameList();
		//////////////////////////////////////////////////////////////////////////
	}
	assert(false);
	return kRR_Failed;
}

GA_KillMonsters::~GA_KillMonsters()
{
	filter_hittable_.reset();
	filter_alive_.reset();
	filter_black_name_list_.reset();
}

CA_ActionPtr GA_KillMonsters::Tester()
{
	if (other_tester_)
		return other_tester_;
	else
	{
		CA_ActionWeakPtr weak_this = shared_from_this();
		other_tester_ = GA_Factory::MakeLambda<CA_FastTester>([weak_this, this](){
			if (weak_this.expired())
				return false;
			if (!obj_mgr_)
			{
				assert(false);
				return false;
			}
			if (!obj_mgr_->RebuildAll())
			{
				assert(false);
				return false;
			}
			return !!obj_mgr_->GetFirstObj();
		});
	}
	return other_tester_;
}

void GA_KillMonsters::DecBlackNameList()
{
	if (black_name_list_.empty())
		return;
	CmnBufferVector<stObjFailedInfo, 500> obj_cont_tmp;
	for (auto& v : black_name_list_)
		obj_cont_tmp.push_back(v);
	black_name_list_.clear();
	if (!GWndExecSync([this](){
		if (!obj_mgr_)
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return;
	}
	for (auto& v : obj_cont_tmp)
	{
		if (v.failed_cnt_ < 0)
		{
			black_name_list_.push_back(v);
			continue;
		}
		assert(v.failed_cnt_ > 0);
		auto the_new_obj = obj_mgr_->FindById(v.obj_->GetGameId());
		if (!the_new_obj)
		{
			black_name_list_.push_back(v);
			continue;
		}
		auto dist = the_new_obj->Distance(v.obj_->GetBackupPos());
		if (dist < 35)		//目标位置没什么变化，很可能怪物与角色由门阻隔着的
		{
			black_name_list_.push_back(v);
			continue;
		}
		--v.failed_cnt_;
		if (v.failed_cnt_ == 0)
			continue;
		black_name_list_.push_back(v);
	}
}

void GA_KillMonsters::AddBlackNameList(const GameObjBasePtrT& obj)
{
	assert(obj);
	auto game_id = obj->GetGameId();
	obj->ResetPos();
	obj->SetOwner(nullptr);
	for (auto& v : black_name_list_)
	{
		if (v.obj_->GetGameId() == game_id)
		{
			v.obj_ = obj;
			if (v.failed_cnt_ > 0)
				++v.failed_cnt_;
			return;
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	if (GA_MoveTo::TryMoveTo(*player_pos, obj))
		black_name_list_.push_back({ obj, 1 });
	else
	{
		black_name_list_.push_back({ obj, -1 });
		GMap::GetMe().UpdateArea(*player_pos);
	}
}

GA_TesterNearest::GA_TesterNearest(const GameObjBasePtrT& dst_obj)
{
	SetDstObj(dst_obj);
}

void GA_TesterNearest::SetDstObj(const GameObjBasePtrT& dst_obj)
{
	if (!dst_obj)
	{
		assert(false);
		return;
	}
	dst_obj_ = dst_obj;
	obj_mgr_ = dst_obj->GetOwner();
	if (!obj_mgr_)
	{
		assert(false);
		return;
	}
}

bool GA_TesterNearest::Test()
{
	if (!obj_mgr_ || !dst_obj_)
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
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
	obj_mgr_->SortByWeightDistance(*player_pos);
	auto the_obj = obj_mgr_->GetFirstObj();
	if (!the_obj)
		return false;		//这里不能返回真，否则可能出现跑来跑去的BUG
	//即使是当前的怪，坐标也有可能会有大的改变
	/*if (the_obj->GetGameId() == dst_obj_->GetGameId())
		return false;*/
	return the_obj->WeightDistance(*player_pos) - dst_obj_->WeightDistance(*player_pos) <=
		-25;		//这个数值不能太小，否则就有可能出现跑来跑去的BUG
}

GA_KillUntil::GA_KillUntil(bool clear_mark_pos, float tra_map_rate, int remaining, const CA_ActionPtr& fast_tester) 
	: fast_tester_(fast_tester)
{
	clear_mark_pos_ = clear_mark_pos;
	tra_map_rate_ = tra_map_rate;
	remaining_ = remaining;
}

CA_RunRes GA_KillUntil::OnRun()
{
	if (!as_timer_action_)
	{
		auto& factory = GA_Factory::GetMe();
		auto action_kill_monsters = factory.MakeKillMonsters();
		if (!action_kill_monsters)
		{
			assert(false);
			return kRR_Failed;
		}
		auto not_action = factory.MakeNot(action_kill_monsters);
		if (!not_action)
		{
			assert(false);
			return kRR_Failed;
		}
		auto tra_full_map = factory.MakeTraFullMap(Consumer(), not_action, clear_mark_pos_, tra_map_rate_, remaining_);
		if (!tra_full_map)
		{
			assert(false);
			return kRR_Failed;
		}
		if (fast_tester_)
		{
			as_timer_action_ = factory.MakeAsTimerAction(tra_full_map, fast_tester_);
			if (!as_timer_action_)
			{
				assert(false);
				return kRR_Failed;
			}
			if (!fast_tester_->Consumer())
				fast_tester_->SetConsumer(Consumer());
		}
		else
			as_timer_action_ = tra_full_map;
	}
	const auto& run_res = as_timer_action_->Run();
	if (fast_tester_ && run_res != fast_tester_)
	{
		LOG_O(Log_debug) << "KillUntil之没有找到目标";
		//assert(false);
		return kRR_Failed;
	}
	return kRR_Succeed;
}

GA_TesterByName::GA_TesterByName(const std::string& obj_name, const GameObjMgrPtr& obj_mgr)
	: obj_name_(obj_name), obj_mgr_(obj_mgr)
{
	assert(!obj_name.empty());
	if (!obj_mgr_)
	{
		auto ptr = new GameObjMgr;
		obj_mgr_.reset(ptr);
		if (!obj_mgr_)
		{
			assert(false);
			return;
		}
		ptr->IncludeOpenableFilter();
		ptr->IncludeLifedFilter();
	}
}

bool GA_TesterByName::Test()
{
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr_->RebuildAll())
	{
		assert(false);
		return false;
	}
	the_obj_ = obj_mgr_->FindByName(obj_name_);
	if (the_obj_)
		return true;
	return false;
}

GameObjBasePtrT GA_TesterByName::GetTheObj() const
{
	return the_obj_;
}

GA_OpenObjs::GA_OpenObjs(const GameObjMgrPtr& obj_mgr)
	: obj_mgr_(obj_mgr)
{
	open_res_ = kOVR_Error;
	if (!obj_mgr)
	{
		assert(false);
		return;
	}
	filter_black_name_list_.reset(new FilterGameObjBlackNameList(obj_mgr_->GetFilter(), black_name_list_));
	filter_rec_open_obj_.reset(FilterGameObj_MakeLambda([this](const GameObjBasePtrT& obj){
		if (rec_open_obj_.empty()) return true;
		assert(obj);
		auto the_id = obj->GetGameId();
		for (auto& v : rec_open_obj_)
		{
			assert(v.open_obj_);
			if (v.open_obj_->GetGameId() == the_id)
				return false;
		}
		/*if (glo_black_name_list_.IsIn(the_id))
			return false;*/
		return true;
	}, obj_mgr_->GetFilter()));
}

GA_OpenObjs::~GA_OpenObjs()
{
	filter_black_name_list_.reset();
	filter_rec_open_obj_.reset();
}

bool GA_OpenObjs::Test()
{
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	if (!tester_in_range_)
	{
		tester_in_range_ = GA_Factory::GetMe().MakeTesterInRange(CA_T<GA_OpenObjs>(shared_from_this(), this));
		if (!tester_in_range_)
		{
			assert(false);
			return nullptr;
		}
		auto consumer = Consumer();
		if (!consumer)
		{
			assert(false);
			return nullptr;
		}
		tester_in_range_->SetConsumer(consumer);
		tester_in_range_->SetParent(consumer);
	}
	if (!tester_in_range_->obj_mgr_)
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!tester_in_range_->obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
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
	obj_mgr_->SortByWeightDistance(*player_pos);
	for (auto& obj : obj_mgr_->GetObjs())
	{
		if (obj->Distance(*player_pos) > obj->GetCanOpenedDistance())
			continue;
		if (!obj->MovingDoTryOpen())
		{
			black_name_list_.Add(obj->GetGameId());
			continue;
		}
		auto open_res = gpm.PreOpenObj(obj);
		if (open_res == kOVR_Succeed)
		{
			if (!product_impl_)
			{
				auto action_open_obj = GA_Factory::MakeLambda([this](){
					if (DoOpenObjs()) return kRR_Succeed;
					return kRR_Failed;
				});
				if (!action_open_obj)
				{
					assert(false);
					return false;
				}
				product_impl_ = GA_Factory::GetMe().MakeAsTimerAction(action_open_obj, tester_in_range_);
				if (!product_impl_)
				{
					assert(false);
					return false;
				}
			}
			obj->ResetPos();
			obj->SetOwner(nullptr);
			stOpenObjInfo open_obj;
			open_obj.open_obj_ = obj;
			rec_open_obj_.push_back(open_obj);
			continue;
		}
		else if (open_res == kOVR_Openning || open_res == kOVR_Opened)
			continue;
		black_name_list_.Add(obj->GetGameId());
	}
	if (!rec_open_obj_.empty())
	{
		if (obj_openning_)
		{
			auto the_obj = CalcFirstOpenObj();
			if (!the_obj)
				return false;
			if (obj_openning_ == the_obj)
				return false;
			assert(obj_openning_->GetGameId() != the_obj->GetGameId());
			if (obj_openning_->WeightDistance(*player_pos) - the_obj->WeightDistance(*player_pos) <= 25)
				return false;
		}
		//有任何怪物都应该去中止打开对象
		if (GA_TesterInRange::HasTheObjs(tester_in_range_->obj_mgr_))
		{
			if (!obj_mgr_->FindObjInCircleByType(kGOT_Transitonable, *player_pos, 40))
				return false;
		}
		return true;
	}
	return false;
}

void GA_OpenObjs::AddBlackNameList(pt_dword obj_id)
{
	black_name_list_.Add(obj_id);
}

GameObjMgrPtr GA_OpenObjs::GetObjMgr() const
{
	assert(obj_mgr_);
	return obj_mgr_;
}

bool GA_OpenObjs::CanKillTheObj(const GameObjBasePtrT& obj) const
{
	if (!must_do_obj_)
		return true;
	assert(obj);
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	auto pos_tmp = *player_pos;
	auto distance_delta = must_do_obj_->Distance(pos_tmp) - obj->Distance(pos_tmp);
	if (distance_delta < 20)
		return false;
	return true;
}

bool GA_OpenObjs::DoOpenObjs()
{
	if (!tester_open_)
	{
		auto action_open = CA_Factory::MakeLambda([this](){
			open_res_ = GPlayerMe::GetMe().MoveToOpenObj(Consumer(), obj_openning_);
			return kRR_Succeed;
		});
		tester_open_ = GA_Factory::GetMe().MakeAsTimerAction(action_open, Tester());
		if (!tester_open_)
		{
			assert(false);
			return false;
		}
	}
	assert(!obj_openning_);
	while (true)
	{
		obj_openning_ = CalcFirstOpenObj();
		if (!obj_openning_) break;
		for (auto& rec_obj : rec_open_obj_)
		{
			if (rec_obj.open_obj_ == obj_openning_)
			{
				++rec_obj.open_cnt_;
				rec_obj.timer_.SetDuration(2000 * rec_obj.open_cnt_);
				rec_obj.timer_.UpdateTime();
				LOG_O(Log_trace) << "去打开对象：" << obj_openning_->GetCnName() 
					<< ",记录的对象个数：" << rec_open_obj_.size() << ",打开对象次数：" << rec_obj.open_cnt_;
				break;
			}
		}
		if (!obj_openning_->CanBreakOpenObj())
			must_do_obj_ = obj_openning_;
		BOOST_SCOPE_EXIT_ALL(this){
			must_do_obj_.reset();
			obj_openning_.reset();
		};
		open_res_ = kOVR_Error;		
		tester_open_->Run();
		if (open_res_ >= kOVR_Succeed)
		{
			EraseTheObj(obj_openning_);
			obj_openning_->CloseObject();
			continue;
		}
		else if (open_res_ == kOVR_Openning)
			continue;
		EraseTheObj(obj_openning_);
		black_name_list_.Add(obj_openning_->GetGameId());
	}	
	return true;
}

void GA_OpenObjs::EraseTheObj(const GameObjBasePtrT& obj)
{
	assert(obj);
	auto game_id = obj->GetGameId();
	auto it = std::find_if(rec_open_obj_.begin(), rec_open_obj_.end(), [game_id](const stOpenObjInfo& the_obj){
		assert(the_obj.open_obj_);
		return the_obj.open_obj_->GetGameId() == game_id;
	});
	if (it != rec_open_obj_.end())
		rec_open_obj_.erase(it);
}

GameObjBasePtrT GA_OpenObjs::CalcFirstOpenObj()
{
	CmnBufferVector<GameObjBasePtrT, 100> rec_open_obj_tmp;
	for (auto& v : rec_open_obj_)
	{
		if (v.open_cnt_ <= 0)
		{
			rec_open_obj_tmp.push_back(v.open_obj_);
			continue;
		}
		if (v.timer_.IsTimeout())
		{
			rec_open_obj_tmp.push_back(v.open_obj_);
			continue;
		}
	}
	auto obj_size = rec_open_obj_tmp.size();
	if (obj_size == 0)
		return nullptr;
	else if (obj_size == 1)
		return rec_open_obj_tmp.back();
	auto& gpm = GPlayerMe::GetMe();
	/*if (!GWndExecSync([this, &gpm](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return nullptr;
	}*/
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return nullptr;
	}
	GameObjBase::SortAssist<true>(rec_open_obj_tmp, *player_pos);

	GameObjBasePtrT the_res_obj;
	for (auto it = rec_open_obj_tmp.begin(), it_end = rec_open_obj_tmp.end(); it != it_end; ++it)
	{
		auto& obj = *it;
		assert(obj);
#ifdef _DEBUG
		auto is_safe = false;
		BOOST_SCOPE_EXIT_ALL(&is_safe){
			if (!is_safe)
			{
				assert(!"怎么会有异常？");
			}
		};
#endif
		auto open_res = gpm.PreOpenObj(obj);
#ifdef _DEBUG
		is_safe = true;
#endif
		if (open_res == kOVR_Succeed)
		{
			the_res_obj = obj;
			break;
		}
		else if (open_res == kOVR_Opened)
		{
			EraseTheObj(obj);
			continue;
		}
		else if (open_res != kOVR_Openning)
		{
			black_name_list_.Add(obj->GetGameId());
			EraseTheObj(obj);
			continue;
		}
	}
	return the_res_obj;
}

GA_TesterInRange::GA_TesterInRange(const CA_T<GA_OpenObjs>& action_open_objs) : action_open_objs_(action_open_objs)
{
	terminate_flag_ = enTerminateFlag(kTF_ReRun | kTF_ToParent);
	obj_mgr_.reset(new GameObjMgr(kGOT_Monster));
	if (!obj_mgr_)
	{
		assert(false);
		return;
	}
	filter_alive_.reset(new FilterGameObjAlive(obj_mgr_->GetFilter()));
	filter_hittable_.reset(new FilterGameObjHittable(obj_mgr_->GetFilter()));
}

bool GA_TesterInRange::Test()
{
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
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
	if (!HasTheObjs(obj_mgr_))
		return false;
	auto the_obj = obj_mgr_->GetFirstObj();
	if (the_obj && !action_open_objs_.expired())
	{
		auto open_objs = action_open_objs_.lock();
		assert(open_objs);
		if (!open_objs->CanKillTheObj(the_obj))
			return false;
	}
	//but_obj_ = the_obj;
	return true;
}

GA_TesterInRange::~GA_TesterInRange()
{
	filter_alive_.reset();
	filter_hittable_.reset();
}

bool GA_TesterInRange::HasTheObjs(const GameObjMgrPtr& obj_mgr)
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	return obj_mgr->HasObjInCircle(*player_pos, GA_TesterDanger::danger_distance_, kMonsterMaxCnt);
}

bool GA_TesterInRange::HasTheObjs(const GameObjMgrPtr& obj_mgr, const GameObjBasePtrT& but_obj)
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	GameObjsBufferCont objs;
	obj_mgr->GetInCircleObjs(*player_pos, GA_TesterDanger::danger_distance_, kMonsterMaxCnt + 1, objs);
	auto objs_size = objs.size();
	if (objs_size > kMonsterMaxCnt)
		return true;
	else if (objs_size < kMonsterMaxCnt)
		return false;
	if (but_obj)
	{
		auto id = but_obj->GetGameId();
		for (auto& v : objs)
		{
			if (v->GetGameId() == id)
				return false;
		}
	}
	return true;
}

void GA_TesterInRange::SetButObj(const GameObjBasePtrT& but_obj)
{
	assert(false);
	//but_obj_ = but_obj;
}

GA_KillInRange::GA_KillInRange(const CA_T<GA_OpenObjs>& action_open_objs) : action_open_objs_(action_open_objs){}

bool GA_KillInRange::Test()
{
	if (!obj_mgr_)
	{
		obj_mgr_.reset(new GameObjMgr(kGOT_Monster));
		if (!obj_mgr_)
		{
			assert(false);
			return false;
		}
		filter_alive_.reset(new FilterGameObjHittable(obj_mgr_->GetFilter()));
		filter_hittable_.reset(new FilterGameObjAlive(obj_mgr_->GetFilter()));
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
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
	if (!GA_TesterInRange::HasTheObjs(obj_mgr_))
		return false;
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	obj_mgr_->SortByWeightDistance(*player_pos);
	auto the_obj = obj_mgr_->GetFirstObj();
	if (!the_obj)
	{
		assert(false);
		return false;
	}
	if (!action_open_objs_.expired())
	{
		auto open_objs = action_open_objs_.lock();
		assert(open_objs);
		if (!open_objs->CanKillTheObj(the_obj))
			return false;
	}
	if (action_kill_monster_)
		action_kill_monster_->SetDstObj(the_obj);
	else
	{
		auto& factory = GA_Factory::GetMe();
		action_kill_monster_ = factory.MakeKillMonster(the_obj);
		if (!action_kill_monster_)
		{
			assert(false);
			return false;
		}
		if (action_open_objs_.expired())
			product_impl_ = action_kill_monster_;
		else
		{
			auto open_objs = action_open_objs_.lock();
			if (!open_objs)
			{
				assert(false);
				return false;
			}
			auto as_timer_action = factory.MakeAsTimerAction(action_kill_monster_, open_objs->Tester());
			if (!as_timer_action)
			{
				assert(false);
				return false;
			}
			product_impl_ = as_timer_action;
		}
	}
	return true;
}

GA_KillInRange::~GA_KillInRange()
{
	filter_hittable_.reset();
	filter_alive_.reset();
}

GA_KillNpcMonster::~GA_KillNpcMonster()
{
	filter_visit_.reset();
	filter_name_.reset();
}

GA_KillNpcMonster::GA_KillNpcMonster(const std::string& npc_name)
{
	exec_once_ = false;
	if (npc_name.empty())
	{
		assert(false);
		return;
	}
	obj_mgr_.reset(new GameObjMgr(kGOT_Npc));
	if (!obj_mgr_)
	{
		assert(false);
		return;
	}
	filter_name_.reset(new FilterGameObjName(obj_mgr_->GetFilter(), npc_name));
	filter_visit_.reset(new FilterGameObjNeedVisitable(obj_mgr_->GetFilter()));
}

bool GA_KillNpcMonster::Test()
{
	if (exec_once_)
		return false;
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr_->RebuildAll())
	{
		assert(false);
		return false;
	}
	the_obj_ = obj_mgr_->GetFirstObj();
	if (!the_obj_)
		return false;
	if (!product_impl_)
	{
		product_impl_ = GA_Factory::MakeLambda([this](){
			assert(the_obj_);
			exec_once_ = true;
			auto& gpm = GPlayerMe::GetMe();
			if (!gpm.KillToOpenObj(the_obj_))
				return kRR_Failed;
			if (!gpm.TalkToNpc(CD_kNTL_CotinueA))
			{
				assert(false);
				return kRR_Failed;
			}
			if (!GInterface::ChooseKillTheMonster())
			{
				assert(false);
			}
			return kRR_Succeed;
		});
		if (!product_impl_)
		{
			assert(false);
			return false;
		}
	}
	return true;
}

GA_LookMonsterDie::GA_LookMonsterDie(const std::string& monster_name) : monster_name_(monster_name)
{
	has_the_obj_ = false;
	if (monster_name.empty())
	{
		assert(false);
		return;
	}
	obj_mgr_.reset(new GameObjMgr(kGOT_Monster));
}

bool GA_LookMonsterDie::Test()
{
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr_->RebuildAll())
	{
		assert(false);
		return false;
	}
	const auto& the_obj = obj_mgr_->FindByName(monster_name_);
	if (has_the_obj_)
	{
		if (the_obj)
			the_obj_ = the_obj;
		else
			return true;
		return the_obj->GetCurHp() <= 0;
	}
	if (the_obj)
	{
		has_the_obj_ = true;
		assert(!the_obj_);
		the_obj_ = the_obj;
		if (the_obj->GetCurHp() <= 0)
			return true;
	}
	return false;
}

GameObjBasePtrT GA_LookMonsterDie::GetTheObj() const
{
	return the_obj_;
}

float GA_TesterDanger::danger_distance_ = 50;

void GA_TesterDanger::SetDangerDistance(float screen_rate)
{
	if (screen_rate < 0)
	{
		LOG_O(Log_error) << "设置全局危险距离之屏幕比率不能为负";
		assert(false);
		return;
	}
	danger_distance_ = ScreenRate2Distance(screen_rate);
}

GA_TesterDanger::GA_TesterDanger(const GameObjMgrPtr& obj_mgr) : obj_mgr_(obj_mgr)
{
	but_danger_distance_ = -1;
}

void GA_TesterDanger::SetButObj(const GameObjBasePtrT& but_obj, float but_danger_distance)
{
	but_obj_ = but_obj;
	but_danger_distance_ = but_danger_distance;
}

bool GA_TesterDanger::Test()
{
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	auto action_skills = GA_SkillBase::GetSkills();
	if (!action_skills)
	{
		assert(false);
		return false;
	}
	auto cur_skill = action_skills->GetCurSkill();
	if (!cur_skill)
	{
		assert(false);
		return false;
	}
	FilterGameObjAlive filter_alive(obj_mgr_->GetFilter());
	FilterGameObjHittable filter_hittable(obj_mgr_->GetFilter());
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
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
	auto max_hp = gpm.GetMaxHp();
	if (max_hp > 0)
	{
		auto cur_hp = gpm.GetCurHp();
		auto cur_rate_hp = (float)cur_hp / (float)max_hp;
		if (cur_rate_hp * 100 >= kEludeMonHp)
			return false;
	}
	GameObjsBufferCont objs;
	obj_mgr_->GetInCircleObjs(*player_pos, cur_skill->GetDangerDistance(), 2, objs);
	if (!but_obj_ || but_danger_distance_ <= 0)
		return !objs.empty();
	if (objs.size() > 1)
		return true;
	GameObjBasePtrT the_obj;
	if (!objs.empty())
	{
		the_obj = objs.front();
		if (the_obj->GetGameId() != but_obj_->GetGameId())
			return true;
	}
	else
	{
		the_obj = obj_mgr_->FindById(but_obj_->GetGameId());
		if (!the_obj)
			return false;
	}
	assert(the_obj);
	if (the_obj->Distance(*player_pos) > but_danger_distance_)
		return false;
	auto dst_pos = the_obj->GetPos();
	if (!dst_pos)
	{
		assert(false);
		return false;
	}
	G_AStar::PosContT pos_info_tmp;
	if (!G_AStar::GetMe().GenFindPath(*player_pos, but_danger_distance_, *dst_pos, pos_info_tmp))
		return false;
	return !pos_info_tmp.empty();
}

float GA_TesterDanger::ScreenRate2Distance(float screen_rate)
{
	return (float)kDistanceOfScreen * screen_rate;
}

CA_RunRes GA_DangerKill::OnRun()
{
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed;
	}
	if (!dst_obj_)
	{
		assert(false);
		return kRR_Failed;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.PreKillMonster())
	{
		assert(false);
		return kRR_Failed;
	}
	auto action_skills = GA_SkillBase::GetSkills();
	if (!action_skills)
	{
		assert(false);
		return kRR_Failed;
	}
	FilterGameObjAlive filter_alive(obj_mgr_->GetFilter());
	FilterGameObjHittable filter_hittable(obj_mgr_->GetFilter());
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return kRR_Failed;
	}
	auto obj_id = dst_obj_->GetGameId();
	auto cur_skill_old = action_skills->GetCurSkill();
	BOOST_SCOPE_EXIT_ALL(&cur_skill_old, &action_skills){
		action_skills->SetCurSkill(cur_skill_old);
	};
	if (!action_skills->GenCurSkill())
	{
		//assert(false);
		return kRR_Failed;
	}
	if (!HandleDanger())
	{
		//assert(false);
		return kRR_Failed;
	}
	int danger_cnt = 0;
	bool is_kill_other = false;
	GameObjBasePtrT dst_obj_tmp;
	int disappear_cnt = 0;
	while (true)
	{
		if (dst_obj_)
			dst_obj_->ResetPos();
		auto hit_res = HitOnce(action_skills);
#ifdef _DEBUG
		/*if (dst_obj_)
		{
			auto cur_skill = action_skills->GetCurSkill();
			if (cur_skill)
			{
				LOG_O(Log_trace) << "DangerKill，怪：" << dst_obj_->GetCnName() 
					<< ",id:" << dst_obj_->GetGameId() << ",hit_res:" << hit_res << ",技能名：" << cur_skill->GetSkillName();
			}
		}*/
#endif
		if (kHR_Error == hit_res)
		{
			//assert(false);
			return kRR_Failed;
		}
		else if (kHR_Danger == hit_res)
		{
			if (!HandleDanger())
			{
				//assert(false);
				return kRR_Failed;
			}
		}
		if (!GWndExecSync([&gpm, this](){
			if (!gpm.Update())
			{
				assert(false);
				return false;
			}
			if (!obj_mgr_->RebuildAll())
			{
				assert(false);
				return false;
			}
			return true;
		}))
		{
			assert(false);
			return kRR_Failed;
		}

		dst_obj_tmp = obj_mgr_->FindById(obj_id);
		if (dst_obj_tmp)
		{
			dst_obj_ = dst_obj_tmp;
			disappear_cnt = 0;
		}
		else
		{
			if (!obj_mgr_only_->RebuildAll())
			{
				assert(false);
				return kRR_Failed;
			}
			dst_obj_tmp = obj_mgr_only_->GetFirstObj();
			if (dst_obj_tmp)
			{
				if (dst_obj_tmp->GetCurHp() <= 0)
					return kRR_Succeed;
				dst_obj_ = dst_obj_tmp;
				disappear_cnt = 0;
			}
			else
			{
				++disappear_cnt;
				if (disappear_cnt > 1)
				{
					LOG_O(Log_debug) << "怪物不见了的次数过多，把怪物当作死亡看待：" << dst_obj_->GetCnName();
					return kRR_Succeed;
				}
				dst_obj_->ResetPos();
				LOG_O(Log_debug) << "怪物竟然不见了，难道是死亡了？怪物名：" << dst_obj_->GetCnName();
			}
		}
		if (is_kill_other)
		{
			danger_cnt = 0;
			is_kill_other = false;
		}
		else if (kHR_Danger == hit_res)
		{
			++danger_cnt;
			if (danger_cnt >= kDangerHandleCnt)
			{
				auto player_pos = gpm.GetPos();
				if (!player_pos)
				{
					assert(false);
					return kRR_Failed;
				}
				obj_mgr_->SortByPos(*player_pos);
				is_kill_other = true;
				dst_obj_ = obj_mgr_->GetFirstObj();
				if (!dst_obj_)
				{
					assert(false);
					return kRR_Failed;
				}
			}
		}
		if (!action_skills->GenCurSkill())
		{
			//assert(false);
			return kRR_Failed;
		}
	}
	assert(!"执行不到这");
	return kRR_Failed;
}

GA_DangerKill::enHitRes GA_DangerKill::HitOnce(const CA_T<GA_Skills>& action_skills)
{
	assert(dst_obj_);
	assert(action_skills);
	if (!as_timer_action_)
	{
		auto& factory = GA_Factory::GetMe();
		assert(!tester_danger_);
		tester_danger_ = factory.MakeTesterDanger(obj_mgr_);
		if (!tester_danger_)
		{
			assert(false);
			return kHR_Error;
		}
		assert(!as_timer_action_);
		as_timer_action_ = CA_Factory::MakeAsTimerAction(action_skills, tester_danger_, kTestDangerTime);
		if (!as_timer_action_)
		{
			assert(false);
			return kHR_Error;
		}
	}
	action_skills->SetDstObj(dst_obj_);
	assert(as_timer_action_);
	const auto& run_res = as_timer_action_->Run();
	if (run_res == tester_danger_)
		return kHR_Danger;
	else if (run_res < kRR_Succeed)
	{
		LOG_O(Log_debug) << "DangerHitOnce失败";
		//assert(false);
		return kHR_Error;
	}
	return kHR_Succeed;
}

bool GA_DangerKill::HandleDanger()
{
	if (!safer_move_to_)
	{
		safer_move_to_ = GA_Factory::GetMe().MakeSaferMoveTo(obj_mgr_);
		if (!safer_move_to_)
		{
			LOG_O(Log_debug) << "SaferMoveTo Failed";
			//assert(false);
			return false;
		}
	}
	if (GMap::GetMe().GetMapName() == kMapName_DeHuanJing)
		return true;
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
	auto max_hp = gpm.GetMaxHp();
	if (max_hp > 0)
	{
		auto cur_hp = gpm.GetCurHp();
		auto cur_rate_hp = (float)cur_hp / (float)max_hp;
		if (cur_rate_hp * 100 >= kEludeMonHp)
			return true;
	}
	const auto& run_res = safer_move_to_->Run();
	if (run_res < kRR_Succeed)
	{
		LOG_O(Log_debug) << "HandleDanger Failed";
		//assert(false);
		return false;
	}
	return true;
}

GA_DangerKill::GA_DangerKill()
{
	obj_mgr_only_.reset(new GameObjMgr(kGOT_Monster));
	assert(obj_mgr_only_);
	filter_only_obj_.reset(FilterGameObj_MakeLambda([this](const GameObjBasePtrT& obj){
		if (!dst_obj_) return false;
		return dst_obj_->GetGameId() == obj->GetGameId();
	}, obj_mgr_only_->GetFilter()));
}

GA_DangerKill::~GA_DangerKill()
{
	filter_only_obj_.reset();
}

void GA_Targetable::SetDstObj(const GameObjBasePtrT& dst_obj)
{
	if (dst_obj)
	{
		assert(dst_obj);
		dst_obj_ = dst_obj;
		obj_mgr_ = dst_obj->GetOwner();
		assert(obj_mgr_);
	}
	else
	{
		dst_obj_.reset();
		obj_mgr_.reset();
	}
	
}

GA_DangerMoveTo::GA_DangerMoveTo(const stCD_VecInt& dst_pos, const GameObjMgrPtr& obj_mgr) : GA_MoveTo(dst_pos), obj_mgr_(obj_mgr)
{
	assert(obj_mgr);
}

template<typename DangerT>
static bool _BuildDanger(DangerT& danger, GameObjMgrPtr& obj_mgr, bool set_dst_danger){
	auto action_skills = GA_SkillBase::GetSkills();
	if (!action_skills)
	{
		assert(false);
		return false;
	}
	auto cur_skill = action_skills->GetCurSkill();
	if (!cur_skill)
	{
		assert(false);
		return false;
	}
	auto danger_distance = cur_skill->GetDangerDistance();
	for (auto& obj : obj_mgr->GetObjs())
	{
		auto obj_pos = obj->GetPos();
		if (!obj_pos)
		{
			assert(false);
			continue;
		}
		danger.AddPosition(*obj_pos, danger_distance);
	}
	if (set_dst_danger)
		danger.SetDstDangerDistance(cur_skill->GetSkillDistance());
	return true;
}

bool GA_DangerMoveTo::GenPathImpl(bool is_small_astar, const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info)
{
	out_pos_info.clear();
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	if (is_small_astar)
	{
		auto small_astar = GMap::GetMe().GetSmallAStar(pos_src);
		if (!small_astar)
		{
			assert(false);
			return false;
		}
		AStarDangerT<SmallAStar> danger(*small_astar, *player_pos);
		if (!_BuildDanger(danger, obj_mgr_, true))
		{
			assert(false);
			return false;
		}
		if (!small_astar->GenFindPath(*player_pos, dst_pos_, out_pos_info, danger))
		{
			LOG_O(Log_debug) << "SmallAstar计算路径失败3";
			return false;
		}
		return true;
	}
	auto& astar = G_AStar::GetMe();
	AStarDangerT<G_AStar> danger(astar, *player_pos);
	if (!_BuildDanger(danger, obj_mgr_, true))
	{
		assert(false);
		return false;
	}
	if (!astar.GenFindPath(*player_pos, dst_pos_, out_pos_info, danger))
	{
		LOG_O(Log_debug) << "A星计算路径失败2";
		return false;
	}
	return true;
}

void GA_DangerMoveTo::SetObjMgr(const GameObjMgrPtr& obj_mgr)
{
	assert(obj_mgr);
	obj_mgr_ = obj_mgr;
}

GA_Skill::GA_Skill(const std::string& skill_name) : skill_name_(skill_name), time_span_(1000 * 2), cd_(600)
{
	assert(!skill_name.empty());
	hp_rate_min_ = (float)0.01;
	hp_rate_max_ = 1;
	mp_rate_min_ = (float)0.1;
	mp_rate_max_ = 1;
	danger_distance_ = 15;
	skill_distance_ = 25;
	priority_ = std::numeric_limits<int>::max();
	ensure_kill_ = false;
	use_direct_ = false;
}

CA_RunRes GA_Skill::OnRun()
{
	if (!dst_obj_)
		return kRR_Succeed;
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed;
	}
	auto run_res = RunTheSkills(pre_skills_);
	if (!run_res)
		return run_res;
	if (!dst_obj_)
		return kRR_Succeed;
	if (use_direct_)
		return UseSkillDirect();
	else
	{
		//这个函数最好不要改写dst_obj_，否则在角色死亡后，就不会移动到目标了。
		run_res = PreMove();
		if (!run_res)
			return run_res;
		if (!dst_obj_)
		{
			assert(false);
			return kRR_Succeed;
		}
		auto dst_pos = dst_obj_->GetPos();
		if (!dst_pos)
		{
			assert(false);
			return kRR_Failed;
		}
		if (action_move_to_)
		{
			action_move_to_->SetDstPos(*dst_pos);
			action_move_to_->SetObjMgr(obj_mgr_);
		}
		else
		{
			auto& factory = GA_Factory::GetMe();
			action_move_to_ = factory.MakeDangerMoveTo(*dst_pos, obj_mgr_);
			if (!action_move_to_)
			{
				assert(false);
				return kRR_Failed;
			}
		}
		const auto& run_res = action_move_to_->Run();
		if (run_res < kRR_Succeed)
			return run_res;
	}	
	run_res = DoUseSkill();
	if (!run_res)
		return run_res;

	run_res = RunTheSkills(suffix_skills_);
	return run_res;
}

CA_ActionPtr GA_Skill::SetTimeSpan(float seconds)
{
	if (seconds < 0)
	{
		LOG_O(Log_error) << "释放间隔之参数不能为负";
		assert(false);
		return shared_from_this();
	}
	pt_dword time_span = seconds * (float)1000;
	time_span_.SetDuration(time_span);
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetCD(float seconds)
{
	if (seconds < 0)
	{
		LOG_O(Log_error) << "技能CD之参数不能为负";
		assert(false);
		return shared_from_this();
	}
	pt_dword cd = seconds * (float)1000;
	cd = std::max(cd, (pt_dword)50);
	cd_.SetDuration(cd);
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetHp(float hp_rate_min, float hp_rate_max)
{
	if (hp_rate_min < 0 || hp_rate_min > 1)
	{
		LOG_O(Log_error) << "血值区间之下限必须在0到1之间";
		assert(false);
		return shared_from_this();
	}
	if (hp_rate_max < 0 || hp_rate_max > 1)
	{
		LOG_O(Log_error) << "血值区间之上限必须在0到1之间";
		assert(false);
		return shared_from_this();
	}
	if (hp_rate_min > hp_rate_max)
	{
		LOG_O(Log_error) << "血值区间之下限必须小于等于上限";
		assert(false);
		return shared_from_this();
	}
	hp_rate_min_ = hp_rate_min;
	hp_rate_max_ = hp_rate_max;
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetMp(float mp_rate_min, float mp_rate_max)
{
	if (mp_rate_min < 0 || mp_rate_min > 1)
	{
		LOG_O(Log_error) << "蓝值区间之下限必须在0到1之间";
		assert(false);
		return shared_from_this();
	}
	if (mp_rate_max < 0 || mp_rate_max > 1)
	{
		LOG_O(Log_error) << "蓝值区间之上限必须在0到1之间";
		assert(false);
		return shared_from_this();
	}
	if (mp_rate_min > mp_rate_max)
	{
		LOG_O(Log_error) << "蓝值区间之下限必须小于等于上限";
		assert(false);
		return shared_from_this();
	}
	mp_rate_min_ = mp_rate_min;
	mp_rate_max_ = mp_rate_max;
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetSkillDistance(float danger_screen_rate, float skill_screen_rate)
{
	if (danger_screen_rate < 0)
	{
		LOG_O(Log_error) << "施法距离之[危险距离屏幕比率]参数不能为负";
		assert(false);
		return shared_from_this();
	}
	if (skill_screen_rate < 0)
	{
		LOG_O(Log_error) << "施法距离之[技能距离屏幕比率]参数不能为负";
		assert(false);
		return shared_from_this();
	}
	if (danger_screen_rate > skill_screen_rate)
	{
		LOG_O(Log_error) << "施法距离之[危险距离屏幕比率]参数必须小于等于[技能距离屏幕比率]参数";
		assert(false);
		return shared_from_this();
	}
	danger_distance_ = GA_TesterDanger::ScreenRate2Distance(danger_screen_rate);
	skill_distance_ = GA_TesterDanger::ScreenRate2Distance(skill_screen_rate);
	return shared_from_this();
}

void GA_SkillBase::ClearSkills()
{
	if (skills_)
		skills_->clear();
}

void GA_SkillBase::AddSkill(GA_SkillBase& skill)
{
	if (!skills_)
	{
		skills_ = GA_Factory::GetMe().MakeSkills();
		if (!skills_)
		{
			assert(false);
			return;
		}
	}
	skills_->AddSkill(CA_T<GA_SkillBase>(skill.shared_from_this(), &skill));
}

bool GA_SkillToHittable::SkillUsingImpl()
{
	if (!obj_mgr_ || !dst_obj_)
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	stCD_VecInt the_pos;
	if (!GWndExecSync([&gpm, this, &the_pos](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		dst_obj_ = obj_mgr_->FindById(dst_obj_->GetGameId());
		if (!dst_obj_)
			return true;
		auto use_skill = GetUseSkill();
		if (!use_skill)
		{
			assert(false);
			return false;
		}

		auto dst_pos = dst_obj_->GetPos();
		if (!dst_pos)
		{
			assert(false);
			return false;
		}
		if (gpm.Distance(*dst_pos) > GA_MoveTo::kValidMaxDistance)
		{
			LOG_O(Log_warning) << "使用技能之距离变得过远:" << gpm.Distance(*dst_pos);
			return false;
		}
		the_pos = *dst_pos;
		return true;
	}))
	{
		//assert(false);
		return false;
	}
	if (dst_obj_)
	{
		auto use_skill = GetUseSkill();
		if (!use_skill)
		{
			assert(false);
			return false;
		}
		use_skill->SkillUsing(false, the_pos);
	}

	return true;
}

CA_RunRes GA_SkillToHittable::BeginUseSkill()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		dst_obj_ = obj_mgr_->FindById(dst_obj_->GetGameId());
		if (!dst_obj_)
			return true;
		auto dst_pos = dst_obj_->GetPos();
		if (!dst_pos)
		{
			assert(false);
			return false;
		}
		if (gpm.Distance(*dst_pos) > GA_MoveTo::kValidMaxDistance)
		{
			LOG_O(Log_warning) << "使用技能之距离过远:" << skill_name_ << ",目标名:" << dst_obj_->GetCnName()
				<< ",距离：" << gpm.Distance(*dst_pos);
			return false;
		}
		auto use_skill = GetUseSkill();
		if (!use_skill)
		{
			assert(false);
			return false;
		}
		use_skill->SetDstPos(*dst_pos);
		return true;
	}))
	{
		//assert(false);
		return kRR_Failed;
	}
	if (dst_obj_)
	{
		auto use_skill = GetUseSkill();
		if (!use_skill)
		{
			assert(false);
			return kRR_Failed;
		}
		use_skill->UseSkill(false);
	}

	return kRR_Succeed;
}

int GA_Skill::Weight() const
{
	return priority_;
}

CA_ActionPtr GA_Skill::SetPriority(int priority)
{
	priority_ = priority;
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetEnsureKill()
{
	ensure_kill_ = true;
	return shared_from_this();
}

CA_ActionPtr GA_Skill::SetUseDirect()
{
	use_direct_ = true;
	return shared_from_this();
}

CA_RunRes GA_SkillToHittable::DoUseSkill()
{
	auto left_time = GInterface::GetSendSkillTimeDuration().CalcLeftDuration();
	if (left_time > 0)
	{
		if (ensure_kill_)
			::Sleep(left_time);
		else
			DummySleep(left_time);
	}
	auto run_res = BeginUseSkill();
	if (run_res < kRR_Succeed)
		return run_res;
	if (!dst_obj_)
		return kRR_Succeed;
	cd_.UpdateTime();
	auto use_skill = GetUseSkill();
	if (!use_skill)
	{
		assert(false);
		return kRR_Failed;
	}
	BOOST_SCOPE_EXIT_ALL(&){
		use_skill->UseSkillEnd(0);
		time_span_.UpdateTime();
	};

	auto& io_s = GetIoService();
	while (true)
	{
		io_s.PollOne();
		run_res = SkillUsing();
		if (run_res < kRR_Succeed)
			return run_res;
		if (!dst_obj_)
			return kRR_Succeed;
		if (cd_.DoTimeout())
			return kRR_Succeed;
	}
	assert(false);
	return kRR_Failed;
}

CA_ActionPtr GA_Skill::AddPreSkill(const std::string& skill_name)
{
	if (skill_name.empty())
	{
		LOG_O(Log_error) << "增加前置技能之技能名不能为空:" << skill_name;
		assert(false);
		return shared_from_this();
	}
	if (skill_name == skill_name_)
	{
		LOG_O(Log_error) << "增加前置技能之技能名不能是自身:" << skill_name;
		assert(false);
		return shared_from_this();
	}
	for (auto& v : pre_skills_)
	{
		if (v.skill_name_ == skill_name)
		{
			LOG_O(Log_error) << "增加前置技能之存在同名技能名：" << skill_name;
			assert(false);
			return shared_from_this();
		}
	}
	pre_skills_.push_back(stSkillInfo());
	pre_skills_.back().skill_name_ = skill_name;
	return shared_from_this();
}

CA_ActionPtr GA_Skill::AddSuffixSkill(const std::string& skill_name)
{
	if (skill_name.empty())
	{
		LOG_O(Log_error) << "增加后置技能之技能名不能为空:" << skill_name;
		assert(false);
		return shared_from_this();
	}
	if (skill_name == skill_name_)
	{
		LOG_O(Log_error) << "增加后置技能之技能名不能是自身:" << skill_name;
		assert(false);
		return shared_from_this();
	}
	for (auto& v : suffix_skills_)
	{
		if (v.skill_name_ == skill_name)
		{
			LOG_O(Log_error) << "增加后置技能之存在同名技能名：" << skill_name;
			assert(false);
			return shared_from_this();
		}
	}
	suffix_skills_.push_back(stSkillInfo());
	suffix_skills_.back().skill_name_ = skill_name;
	return shared_from_this();
}

CA_RunRes GA_Skill::RunTheSkills(SkillInfoCont& skills)
{
	if (skills.empty())
		return kRR_Succeed;
	auto action_skills = GetSkills();
	if (!action_skills)
	{
		assert(false);
		return kRR_Failed;
	}
	float cur_hp_rate = 0, cur_mp_rate = 0;
	if (!GA_Skills::CalcHpMpInfo(cur_hp_rate, cur_mp_rate))
	{
		assert(false);
		return false;
	}
	auto& skill_mgr = GSkillMgr::GetMe();
	auto& buff_mgr = GBuffMgr::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	CA_T<GA_SkillBase> the_skill;
	for (auto& skill : skills)
	{
		if (skill.skill_.expired())
		{
			the_skill = action_skills->FindSkill(skill.skill_name_);
			if (!the_skill)
			{
				LOG_O(Log_warning) << "不存在此技能名：" << skill.skill_name_;
				assert(false);
				continue;
			}
			skill.skill_ = the_skill;
		}
		else
		{
			the_skill = skill.skill_.lock();
			if (!the_skill)
			{
				assert(false);
				continue;
			}
		}
		if (!the_skill->CanFire(skill_mgr, buff_mgr, cur_hp_rate, cur_mp_rate))
			continue;
		auto old_cur_skill = action_skills->GetCurSkill();
		BOOST_SCOPE_EXIT_ALL(&old_cur_skill, &action_skills){
			action_skills->SetCurSkill(old_cur_skill);
		};
		action_skills->SetCurSkill(the_skill);
		the_skill->SetDstObj(dst_obj_);
		const auto& run_res = the_skill->Run();
		if (run_res < kRR_Succeed)
			return run_res;
	}
	return kRR_Succeed;
}

bool GA_Skill::CanFire(const GSkillMgr& skill_mgr, const GBuffMgr& buff_mgr, float cur_hp_rate, float cur_mp_rate) const
{
	if (hp_rate_min_ > cur_hp_rate)
		return false;
	if (hp_rate_max_ < cur_hp_rate)
		return false;
	if (mp_rate_min_ > cur_mp_rate)
		return false;
	if (mp_rate_max_ < cur_mp_rate)
		return false;
	if (!time_span_.IsTimeout())
		return false;
	auto skill_obj = skill_mgr.FindByName(skill_name_);
	if (!skill_obj)
		return false;
	use_skill_.SetSkillId(skill_obj->GetSkillId());
	auto buff = buff_mgr.FindBuff(*skill_obj);
	if (!buff)
		return true;
	auto caller_max_cnt = skill_obj->GetCallerMaxCnt();
	if (caller_max_cnt <= 0)
		return false;		//已经存在buff了，则不必使用该技能
	if (buff->buff_entity_ids_.size() >= caller_max_cnt)
		return false;
	return true;
}

CA_ActionPtr GA_Skill::AddPreSkills(const luabind::object& skill_names)
{
	using namespace luabind;
	if (type(skill_names) != LUA_TTABLE)
	{
		LOG_O(Log_error) << "必须是表类型或字符串类型";
		assert(false);
		return shared_from_this();
	}
	for (auto& v : skill_names)
	{
		auto skill_name = LuaObjCast(v, kEmptyStr);
		AddPreSkill(skill_name);
	}
	return shared_from_this();
}

CA_ActionPtr GA_Skill::AddSuffixSkills(const luabind::object& skill_names)
{
	using namespace luabind;
	if (type(skill_names) != LUA_TTABLE)
	{
		LOG_O(Log_error) << "必须是表类型或字符串类型";
		assert(false);
		return shared_from_this();
	}
	for (auto& v : skill_names)
	{
		auto skill_name = LuaObjCast(v, kEmptyStr);
		AddSuffixSkill(skill_name);
	}
	return shared_from_this();
}

CA_RunRes GA_Skill::UseSkillDirect()
{
	auto left_time = GInterface::GetSendSkillTimeDuration().CalcLeftDuration();
	if (left_time > 0)
	{
		if (ensure_kill_)
			::Sleep(left_time);
		else
			DummySleep(left_time);
	}
	if (!DoUseSkillDirect())
	{
		assert(false);
		return kRR_Failed;
	}
	auto use_skill = GetUseSkill();
	if (!use_skill)
	{
		assert(false);
		return kRR_Failed;
	}
	use_skill->UseSkillEnd(0);
	return kRR_Succeed;
}

CA_RunRes GA_Skill::PreMove()
{
	assert(dst_obj_);
	assert(obj_mgr_);
	auto& gpm = GPlayerMe::GetMe();
	bool need_kill = false;
	if (!GWndExecSync([&gpm, this, &need_kill](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		auto dst_obj_tmp = obj_mgr_->FindById(dst_obj_->GetGameId());
		if (!dst_obj_tmp)
			return true;
		auto dst_pos = dst_obj_tmp->GetPos();
		if (!dst_pos)
		{
			assert(false);
			return false;
		}
		auto distance = gpm.Distance(*dst_pos);
		if (distance <= danger_distance_)
			return true;
		distance -= skill_distance_;
		if (distance <= 15)
			need_kill = true;
		return true;
	}))
	{
		assert(false);
		return kRR_Failed;
	}
	if (need_kill)
		return UseSkillDirect();
	return kRR_Succeed;
}

GUseSkill* GA_Skill::GetUseSkill()
{
	if (use_skill_.IsValid())
		return &use_skill_;
	auto& skill_mgr = GSkillMgr::GetMe();
	auto skill_obj = skill_mgr.FindByName(skill_name_);
	if (!skill_obj)
	{
		if (!GWndExecSync([&skill_mgr](){
			return skill_mgr.RebuildAll();
		}))
		{
			assert(false);
			return nullptr;
		}
		skill_obj = skill_mgr.FindByName(skill_name_);
		if (!skill_obj)
		{
			assert(false);
			return nullptr;
		}
	}
	use_skill_.SetSkillId(skill_obj->GetSkillId());
	return &use_skill_;
}

CA_T<GA_Skills> GA_SkillBase::skills_;

void GA_Skills::AddSkill(const CA_T<GA_SkillBase>& skill)
{
	if (!skill)
	{
		assert(false);
		return;
	}
	auto& skill_name = skill->GetSkillName();
	for (auto& v : skills_)
	{
		if (v->GetSkillName() == skill_name)
		{
			LOG_O(Log_error) << "添加技能之已经存在同名技能了，忽略此次添加：" << skill_name;
			return;
		}
	}
	skills_.push_back(skill);
	std::sort(skills_.begin(), skills_.end(), [](const CA_T<GA_SkillBase>& lhs, const CA_T<GA_SkillBase>& rhs){
		return lhs->Weight() < rhs->Weight();
	});
}

void GA_Skills::clear()
{
	skills_.clear();
}

CA_RunRes GA_Skills::OnRun()
{
	if (!dst_obj_)
	{
		assert(false);
		return kRR_Failed;
	}
	if (!cur_skill_)
	{
		assert(false);
		return kRR_Failed;
	}
	cur_skill_->SetDstObj(dst_obj_);
	return cur_skill_->Run();
}

const CA_T<GA_SkillBase>& GA_Skills::GetCurSkill() const
{
	return cur_skill_;
}

void GA_Skills::SetCurSkill(const CA_T<GA_SkillBase>& cur_skill)
{
	cur_skill_ = cur_skill;
}

bool GA_Skills::GenCurSkill()
{
	float cur_hp_rate = 0, cur_mp_rate = 0;
	if (!CalcHpMpInfo(cur_hp_rate, cur_mp_rate))
	{
		assert(false);
		return false;
	}
	auto& skill_mgr = GSkillMgr::GetMe();
	auto& buff_mgr = GBuffMgr::GetMe();
	CA_T<GA_SkillBase> cur_skill;
	for (auto& skill : skills_)
	{
		if (skill->CanFire(skill_mgr, buff_mgr, cur_hp_rate, cur_mp_rate))
		{
			cur_skill = skill;
			break;
		}
	}
	if (!cur_skill)
	{
		LOG_O(Log_debug) << "没有找到合适的技能来释放，是不是没有配置正确技能？";
		return false;
	}
	cur_skill_ = cur_skill;
	return true;
}

CA_T<GA_SkillBase> GA_Skills::FindSkill(const std::string& skill_name) const
{
	for (auto& v : skills_)
	{
		if (v->GetSkillName() == skill_name)
			return v;
	}
	return nullptr;
}

bool GA_Skills::CalcHpMpInfo(float& cur_hp_rate, float& cur_mp_rate)
{
	auto& gpm = GPlayerMe::GetMe();
	//skill_mgr必须要更新，这个一定不能删
	auto& skill_mgr = GSkillMgr::GetMe();
	if (!GWndExecSync([&gpm, &skill_mgr](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (!skill_mgr.RebuildAll())
		{
			assert(false);
			return false;
		}
		if (!GBuffMgr::GetMe().Update())
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
	auto max_mp = gpm.GetMaxMp();
	if (max_mp <= 0)
	{
		assert(false);
		return false;
	}
	auto max_hp = gpm.GetMaxHp();
	if (max_hp <= 0)
	{
		assert(false);
		return false;
	}
	cur_mp_rate = (float)gpm.GetCurMp() / (float)max_mp;
	if (cur_mp_rate < 0)
	{
		assert(false);
		return false;
	}
	cur_hp_rate = (float)gpm.GetCurHp() / (float)max_hp;
	if (cur_hp_rate < 0)
	{
		assert(false);
		return false;
	}
	return true;
}

GA_SaferMoveTo::GA_SaferMoveTo(const GameObjMgrPtr& obj_mgr)
	: GA_DangerMoveTo(stCD_VecInt(), obj_mgr)
{
}

bool GA_SaferMoveTo::GenPathImpl(bool is_small_astar, const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info)
{
	out_pos_info.clear();
	if (!obj_mgr_)
	{
		assert(false);
		return false;
	}
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	if (is_small_astar)
	{
		auto small_astar = GMap::GetMe().GetSmallAStar(pos_src);
		if (!small_astar)
		{
			assert(false);
			return false;
		}
		AStarDangerT<SmallAStar> danger(*small_astar, *player_pos);
		if (!_BuildDanger(danger, obj_mgr_, false))
		{
			assert(false);
			return false;
		}
		AStarSaferT<AStarDangerT<SmallAStar> > safer(danger);
		if (!small_astar->GenFindPath(*player_pos, safer, out_pos_info, danger))
		{
			LOG_O(Log_debug) << "SmallAstar计算路径失败4";
			return false;
		}
	}
	else
	{
		auto& astar = G_AStar::GetMe();
		AStarDangerT<G_AStar> danger(astar, *player_pos);
		if (!_BuildDanger(danger, obj_mgr_, false))
		{
			assert(false);
			return false;
		}
		AStarSaferT<AStarDangerT<G_AStar> > safer(danger);
		if (!astar.GenFindPath(*player_pos, safer, out_pos_info, danger))
		{
			LOG_O(Log_debug) << "A星计算路径失败3";
			return false;
		}
	}	
	if (!out_pos_info.empty())
		SetDstPos(out_pos_info.back());
	return true;
}

bool GA_SaferMoveTo::MoveToArea(bool pre_smart_move)
{
	if (pre_smart_move)
		return true;
	return __super::MoveToArea(pre_smart_move);
}

GA_SkillDecorator::GA_SkillDecorator(const CA_T<GA_SkillBase>& decoration) : decoration_(decoration)
{
	assert(decoration);
}

const std::string& GA_SkillDecorator::GetSkillName() const
{
	assert(decoration_);
	return decoration_->GetSkillName();
}

float GA_SkillDecorator::GetDangerDistance() const
{
	assert(decoration_);
	return decoration_->GetDangerDistance();
}

float GA_SkillDecorator::GetSkillDistance() const
{
	assert(decoration_);
	return decoration_->GetSkillDistance();
}

int GA_SkillDecorator::Weight() const
{
	assert(decoration_);
	return decoration_->Weight();
}

bool GA_SkillDecorator::CanFire(const GSkillMgr& skill_mgr, const GBuffMgr& buff_mgr, float cur_hp_rate, float cur_mp_rate) const
{
	assert(decoration_);
	return decoration_->CanFire(skill_mgr, buff_mgr, cur_hp_rate, cur_mp_rate);
}

CA_ActionPtr GA_SkillDecorator::SetTimeSpan(float seconds)
{
	assert(decoration_);
	decoration_->SetTimeSpan(seconds);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetCD(float seconds)
{
	assert(decoration_);
	decoration_->SetCD(seconds);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetHp(float hp_rate_min, float hp_rate_max)
{
	assert(decoration_);
	decoration_->SetHp(hp_rate_min, hp_rate_max);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetMp(float mp_rate_min, float mp_rate_max)
{
	assert(decoration_);
	decoration_->SetMp(mp_rate_min, mp_rate_max);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetSkillDistance(float danger_screen_rate, float skill_screen_rate)
{
	assert(decoration_);
	decoration_->SetSkillDistance(danger_screen_rate, skill_screen_rate);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetPriority(int priority)
{
	assert(decoration_);
	decoration_->SetPriority(priority);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetEnsureKill()
{
	assert(decoration_);
	decoration_->SetEnsureKill();
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::SetUseDirect()
{
	assert(decoration_);
	decoration_->SetUseDirect();
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::AddPreSkill(const std::string& skill_name)
{
	assert(decoration_);
	decoration_->AddPreSkill(skill_name);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::AddPreSkills(const luabind::object& skill_names)
{
	assert(decoration_);
	decoration_->AddPreSkills(skill_names);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::AddSuffixSkill(const std::string& skill_name)
{
	assert(decoration_);
	decoration_->AddSuffixSkill(skill_name);
	return shared_from_this();
}

CA_ActionPtr GA_SkillDecorator::AddSuffixSkills(const luabind::object& skill_names)
{
	assert(decoration_);
	decoration_->AddSuffixSkills(skill_names);
	return shared_from_this();
}

CA_RunRes GA_SkillDecorator::OnRun()
{
	assert(decoration_);
	return decoration_->Run();
}

GA_SkillToCorpse::GA_SkillToCorpse(const std::string& skill_name) : GA_Skill(skill_name)
{

}

void GA_SkillToCorpse::SetDstObj(const GameObjBasePtrT& dst_obj)
{
	if (!obj_mgr_)
	{
		obj_mgr_.reset(new GameObjMgr(kGOT_Monster));
		if (!obj_mgr_)
		{
			assert(false);
			return;
		}
	}
}

CA_RunRes GA_SkillToCorpse::OnRun()
{
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed;
	}
	FilterGameObjDied filter_died(obj_mgr_->GetFilter());
	auto& skill_mgr = GSkillMgr::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, &skill_mgr, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		//if (!skill_mgr.RebuildAll())
		//{
		//	assert(false);
		//	return false;
		//}
		if (!obj_mgr_->RebuildAll())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return kRR_Failed;
	}
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return kRR_Failed;
	}
	obj_mgr_->SortByPos(*player_pos);
	dst_obj_ = obj_mgr_->GetFirstObj();
	if (!dst_obj_)
		return kRR_Succeed;
	//避免与主技能相冲突，避免来回移动的问题
	if (dst_obj_->Distance(*player_pos) > skill_distance_)
		return kRR_Succeed;
	FilterGameObjId filter_id(obj_mgr_->GetFilter(), dst_obj_->GetGameId());
	return __super::OnRun();
}

CA_RunRes GA_SkillToCorpse::DoUseSkill()
{
	if (!DoUseSkillDirect())
	{
		assert(false);
		return kRR_Failed;
	}
	return kRR_Succeed;
}

bool GA_SkillToCorpse::DoUseSkillDirect()
{
	if (!time_span_.DoTimeout())
		return true;
	if (!dst_obj_)
	{
		assert(false);
		return false;
	}
	auto skill_obj = GSkillMgr::GetMe().FindByName(skill_name_);
	if (!skill_obj)
	{
		assert(false);
		return false;
	}
	dst_obj_->SetOpenSkillId(enCD_SkillId(skill_obj->GetSkillId()));
	if (!dst_obj_->DoOpen())
	{
		assert(false);
		return false;
	}
	return true;
}

GA_SkillToHittable::GA_SkillToHittable(const std::string& skill_name) : GA_Skill(skill_name)
{

}

CA_RunRes GA_SkillToHittable::OnRun()
{
	if (!dst_obj_)
		return kRR_Succeed;
	if (!obj_mgr_)
	{
		assert(false);
		return kRR_Failed; 
	}
	BOOST_SCOPE_EXIT_ALL(this){
		dst_obj_.reset();
		obj_mgr_.reset();
	};
	FilterGameObjAlive filter_alive(obj_mgr_->GetFilter());
	FilterGameObjHittable filter_hittable(obj_mgr_->GetFilter());
	return __super::OnRun();
}

CA_RunRes GA_SkillToHittable::SkillUsing()
{
	if (GInterface::GetSendSkillTimeDuration().TimeToDoSync([this](){
		return SkillUsingImpl();
	}))
		return kRR_Succeed;
	return kRR_Failed;
}

bool GA_SkillToHittable::DoUseSkillDirect()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!GWndExecSync([&gpm, this](){
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		auto use_skill = GetUseSkill();
		if (!use_skill)
		{
			assert(false);
			return false;
		}
		auto dst_pos = gpm.GetPos();
		if (!dst_pos)
		{
			assert(false);
			return false;
		}
		use_skill->SetDstPos(*dst_pos);
		return true;
	}))
	{
		assert(false);
		return false;
	}
	auto use_skill = GetUseSkill();
	if (!use_skill)
	{
		assert(false);
		return false;
	}
	use_skill->UseSkill(false);
	return true;
}

GA_MoveToNearly::GA_MoveToNearly(const stCD_VecInt& dst_pos) : GA_MoveTo(dst_pos)
{

}

bool GA_MoveToNearly::GenPathImpl(bool is_small_astar, const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info)
{
	out_pos_info.clear();
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	const int kDangerDist = 10;
	if (is_small_astar)
	{
		auto small_astar = GMap::GetMe().GetSmallAStar(pos_src);
		if (!small_astar)
		{
			assert(false);
			return false;
		}
		AStarDangerT<SmallAStar> danger(*small_astar, *player_pos);
		danger.SetDstDangerDistance(kDangerDist);
		if (!small_astar->GenFindPath(*player_pos, dst_pos_, out_pos_info, danger))
		{
			LOG_O(Log_debug) << "SmallAstar计算路径失败2";
			return false;
		}
		return true;
	}
	auto& astar = G_AStar::GetMe();
	AStarDangerT<G_AStar> danger(astar, *player_pos);
	danger.SetDstDangerDistance(kDangerDist);
	if (!astar.GenFindPath(*player_pos, dst_pos_, out_pos_info, danger))
	{
		LOG_O(Log_debug) << "A星计算路径失败n";
		return false;
	}
	return true;
}

GA_OpenObjs::stOpenObjInfo::stOpenObjInfo() : timer_(0)
{
	open_cnt_ = 0;
}

GA_TraGuessArea::GA_TraGuessArea(bool clear_mark_pos) : GA_TraFullMap(clear_mark_pos, -1, -1)
{

}

GA_TraGuessArea::enDoOtherRes GA_TraGuessArea::DoOther()
{
	auto& tra_guess = GMapTraMgr::GetMe().GetTraGuess();
	auto the_door = tra_guess.FindUnPassedDoor();
	if (the_door)
	{
		LOG_O(Log_trace) << "TraGuessArea之发现未打开过的传送门，去开之：" << the_door->GetCnName() << ",id:" << the_door->GetGameId();
		if (GPlayerMe::GetMe().KillToOpenObj(the_door))
			return kDOR_Continue;
	}
	return __super::DoOther();
}

GA_TesterAnyPosByName::GA_TesterAnyPosByName(const std::string& name) : find_pos_(false)
{
	room_id_ = -1;
	auto tra_policy = GMapTraMgr::GetMe().GetDirectPolicy();
	if (!tra_policy)
	{
		assert(false);
		return;
	}
	auto area = tra_policy->FindAreaByName(name);
	if (!area)
	{
		LOG_O(Log_debug) << "杀到指定位置之没有配置该地图位置:" << name;
		assert(false);
		return;
	}
	dst_pos_ = area->GetValidPos();
	room_id_ = G_AStar::GetMe().GetAreaId(dst_pos_);
}

bool GA_TesterAnyPosByName::Test()
{
	if (find_pos_)
	{
		//assert(false);
		return true;
	}
	auto gpm_pos = GPlayerMe::GetMe().GetPos();
	if (!gpm_pos)
	{
		assert(false);
		return true;
	}
	if (room_id_ == G_AStar::GetMe().GetAreaId(*gpm_pos))
	{
		find_pos_ = true;
		return true;
	}
	return false;
}

stCD_VecInt GA_TesterAnyPosByName::GetDstPos()
{
	return find_pos_ ? dst_pos_ : stCD_VecInt(-1, -1);
}

CA_RunRes GA_MoveToPosByName::OnRun()
{
	if (dst_pos_.AnyLess(0))
	{
		if (!tester_any_obj_)
		{
			tester_any_obj_ = GA_Factory::GetMe().MakeTesterAnyObj(obj_name_);
			if (!tester_any_obj_)
			{
				assert(false);
				return kRR_Failed;
			}
			tester_any_obj_->SetConsumer(Consumer());
		}
		if (!action_detect_)
		{
			auto tra_policy = GMapTraMgr::GetMe().GetCurPolicy();
			if (!tra_policy)
			{
				assert(false);
				return kRR_Failed;
			}
			action_detect_ = tra_policy->MakeTraMap(Consumer(), tester_any_obj_, clear_mark_pos_);
			if (!action_detect_)
			{
				assert(false);
				return kRR_Failed;
			}

		}
		//FilterGameObjName filter_name(obj_mgr_->GetFilter(), obj_name_);
		if (!as_timer_action_)
		{
			as_timer_action_ = GA_Factory::GetMe().MakeAsTimerAction(action_detect_, tester_any_obj_);
			if (!as_timer_action_)
			{
				assert(false);
				return kRR_Failed;
			}
		}
		auto run_res = as_timer_action_->Run();
		if (run_res)
		{
			//assert(false);
			//if (!obj_mgr_->RebuildAll())
			//{
			//	assert(false);
			//	return kRR_Failed;
			//}
			//dst_obj_ = obj_mgr_->FindByName(obj_name_);
			dst_pos_ = tester_any_obj_->GetDstPos();
		}
		else if (run_res == tester_any_obj_)
		{
			dst_pos_ = tester_any_obj_->GetDstPos();
		}
		else
		{
			LOG_O(Log_debug) << "寻找目标位置失败------------------------";
			return kRR_Failed;
		}
		if (dst_pos_.AnyLess(0))
		{
			assert(false);
			return kRR_Failed;
		}
	}
	//auto dst_pos = dst_obj_->GetPos();
	//if (!dst_pos)
	//{
	//	assert(false);
	//	return kRR_Failed;
	//}
	if (action_move_to_)
		action_move_to_->SetDstPos(dst_pos_);
	else
	{
		action_move_to_ = GA_Factory::GetMe().MakeMoveToNearly(dst_pos_);
		if (!action_move_to_)
		{
			assert(false);
			return kRR_Failed;
		}
	}
	return action_move_to_->Run();
	//return GA_MoveToObjByObj::HandleRunRes(dst_obj_, action_move_to_->Run());
}

GA_MoveToPosByName::GA_MoveToPosByName(bool clear_mark_pos, const std::string& obj_name) : obj_name_(obj_name), dst_pos_(-1, -1)
{
	clear_mark_pos_ = clear_mark_pos;
}
