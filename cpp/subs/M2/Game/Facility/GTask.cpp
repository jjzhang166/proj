#include "stdafx.h"
#include "GTask.h"
#include "GMap.h"
#include "GPlayer.h"
#include <Script/LuaScript.h>

GDbQuestMgr::GDbQuestMgr()
{

}

bool GDbQuestMgr::RebuildAllImpl()
{
	auto taskDb = CD_Interface::GetDbMgr().GetDbQuest();
	if (IsUnValidPtr(taskDb))
	{
		assert(false);
		return false;
	}

	auto tab = taskDb->GetDbTbl();
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

	for (auto& i : tab->tbl_items_)
	{
		GDbQuest tmpDb;
		tmpDb.SetQuestDb(&i);
		if (!tmpDb.UpdateImpl())
		{
			assert(false);
			return false;
		}

		task_db_.push_back(tmpDb);
	}

	return true;
}

bool GDbQuestMgr::RebuildAll()
{
	if (!task_db_.empty())
		return true;

	return __super::RebuildAll();
}

void GDbQuestMgr::DebugInfo() const
{
	if (task_db_.empty())
		return;

	for (auto& i : task_db_)
		i.DebugInfo();
}

bool GDbQuest::UpdateImpl()
{
	if (!item_quest_db_)
	{
		assert(false);
		return false;
	}

	if (item_quest_db_->sub_id_ < 0)
	{
		assert(false);
		return false;
	}
	if (item_quest_db_->chapter_id_ < 0)
	{
		assert(false);
		return false;
	}

	chapter_id_ = item_quest_db_->chapter_id_;
	sub_id_ = item_quest_db_->sub_id_;
	ch_name_ = WideCharToGbk(CD_SafeName(item_quest_db_->ch_name_));
	is_branch_line_ = item_quest_db_->is_branch_line_;
	return true;
}

GDbQuest::GDbQuest()
{
	chapter_id_		= 0;
	sub_id_			= 0;
	item_quest_db_	= nullptr;
	is_branch_line_ = false;
}

void GDbQuest::DebugInfo() const
{
	LOG_O(Log_debug) << "GTaskDb_Info:" << "Task_Name-" << ch_name_ << ",Task_Id-" << sub_id_ << ",Chapter_Id" << chapter_id_;
}





GDbQuestStateMgr::GDbQuestStateMgr()
{

}

bool GDbQuestStateMgr::RebuildAllImpl()
{
	auto quest = CD_Interface::GetDbMgr().GetDbQuestStates();
	if (IsUnValidPtr(quest))
	{
		assert(false);
		return false;
	}
	auto tal = quest->GetDbTbl();
	if (IsUnValidPtr(tal))
	{
		assert(false);
		return false;
	}

	if (!tal->tbl_items_.IsValid())
	{
		assert(false);
		return false;
	}

	for (auto& v : tal->tbl_items_)
	{
		GDbQuestState _quest;
		_quest.SetQuestStateAddr(&v);
		if (!_quest.UpdateImpl())
		{
			assert(false);
			return false;
		}

		quest_.push_back(_quest);
	}

	return true;
}

void GDbQuestStateMgr::DebugInfo() const
{
	if (quest_.empty())
		return;
	for (auto& i : quest_)
		i.DebugInfo();
}

GDbQuestState::GDbQuestState()
{
	quest_state_ptr_ = nullptr;
	quest_state_ = 0;
	is_complete_state_ = false;
}

bool GDbQuestState::UpdateImpl()
{
	if (IsUnValidPtr(quest_state_ptr_))
	{
		assert(false);
		return false;
	}

	quest_state_ = quest_state_ptr_->quest_state_;

	if (!quest_state_ptr_->branch_ids_.IsValid())
	{
		assert(false);
		return true;
	}
	is_complete_state_ = quest_state_ptr_->is_complete_state_;
	if (!quest_state_ptr_->map_pins_.IsValid())
	{
		assert(false);
		return false;
	}
	if (CD_SafeName(quest_state_ptr_->simple_prompt_))
		simple_prompt_.assign(WideCharToGbk(quest_state_ptr_->simple_prompt_));
	for (auto& v : quest_state_ptr_->map_pins_)
	{
		GDbMapPins map_pin;
		map_pin.SetMapPinAddr(v.tbl_item_);
		if (!map_pin.UpdateImpl())
		{
			assert(false);
			return false;
		}
		map_pins_.push_back(map_pin);
	}
	if (!quest_state_ptr_->map_pins1_.IsValid())
	{
		assert(false);
		return false;
	}
	for (auto& v : quest_state_ptr_->map_pins1_)
	{
		GDbMapPins map_pin;
		map_pin.SetMapPinAddr(v.tbl_item_);
		if (!map_pin.UpdateImpl())
		{
			assert(false);
			return false;
		}
		map_pins1_.push_back(map_pin);
	}
	return true;
}

void GDbQuestState::DebugInfo() const
{
	LOG_O(Log_debug) << "QuestState:" << quest_state_;
}

void GDbQuestState::SetQuestStateAddr(stCD_TblItemQuestStates* quest_state)
{
	quest_state_ptr_ = quest_state;
}

GQuestResolver& GQuestResolver::GetMe()
{
	static GQuestResolver me;
	return me;
}

void GQuestResolver::RegForLua(lua_State* l)
{
	__super::RegForLua(l);
	using namespace luabind;
	module(l)[
		class_<enStopQuestReason>("停止任务原因")
		.enum_("")
		[
			value("接到新任务", kSQR_AcceptNewQuest)
		],

		class_<GQuestHandlerNode, CQH_Node>("GQuestHandlerNode")
		.def_readonly("任务名", &GQuestHandlerNode::__GetQuestName__)
		.def_readonly("状态名", &GQuestHandlerNode::__GetQuestStateName__)
		.def_readonly("地图名", &GQuestHandlerNode::__GetQuestMapName__),

		def("停止任务", &GQuestResolver::QuestStop)
	];
	auto lua_glo = globals(l);
	lua_glo["处理任务状态"] = lua_glo["MakeHandlePerQuest"];
}

CQH_AcceptHandler* GQuestResolver::CreateAcceptHandler(const luabind::object& fn)
{
	assert(false);
	return nullptr;
}

CQH_DoHandler* GQuestResolver::CreateDoHandler(const luabind::object& fn)
{
	return new GQuestDoHandler(fn);
}

CQH_DoneHandler* GQuestResolver::CreateDoneHandler(const luabind::object& fn)
{
	return new GQuestDoneHandler(fn);
}

bool GQuestResolver::OnUpdateAccept()
{
	return true;
}

bool GQuestResolver::OnUpdateDo()
{
	auto& mgr = GUiQuestStateInfo::GetMe();
	if (!mgr.Update())
	{
		assert(false);
		return false;
	}
	if (!DoUpdateQuestMgrInfo())
	{
		assert(false);
		return false;
	}
	return true;
}

bool GQuestResolver::OnUpdateDone()
{
	if (!DoUpdateQuestMgrInfo())
	{
		assert(false);
		return false;
	}
	return true;
}

bool GQuestResolver::OnNoneQuest()
{
	if (HandleStartQuest())
		return true;
	if (HandleCanDone())
		return true;
	if (HandleAcceptQuests())
		return true;
	//这个要放到最后面
	return HandleNoneQuest();
}

CQH_Quests* GQuestResolver::CreateHandlerQuests(const std::string& quest_name)
{
	return new GQuestsHandler(*this, quest_name);
}

CQH_PerQuest* GQuestResolver::CreateHandlerPerQuest(const std::string& state_name)
{
	return new GQuestStateHandler(state_name);
}

bool GQuestResolver::HasTheQuest(const std::string& quest_name) const
{
	if (handle_can_done_)
	{
		auto quest = total_quest_mgr_.FindByName(quest_name);
		if (quest && quest->IsCanDoneState())
			return true;
	}
	if (handle_all_quest_)
	{
		auto quest = total_quest_mgr_.FindByName(quest_name);
		if (quest && !quest->IsCompleteState())
			return true;
	}
	auto cur_quest_info = cur_difficulty_quest_mgr_.FindByName(quest_name);
	if (cur_quest_info)
	{
		if (cur_quest_info->IsCompleteState())
			return false;
		return true;
	}
	//if (handle_can_done_)
	//{
	//	auto quest = total_quest_mgr_.FindByName(quest_name);
	//	if (quest && quest->IsCanDoneState())
	//		return true;
	//}
	//if (handle_all_quest_)
	//{
	//	auto quest = total_quest_mgr_.FindByName(quest_name);
	//	if (quest && !quest->IsCompleteState())
	//		return true;
	//}
	return false;
}

bool GQuestResolver::HasTheQuestState(const std::string& quest_name, const std::string& quest_state) const
{
	//auto cur_quest_info = cur_difficulty_quest_mgr_.FindByName(quest_name);
	auto cur_quest_info = total_quest_mgr_.FindByName(quest_name);
	if (!cur_quest_info)
	{
		//assert(false);
		return false;
	}
	if (cur_quest_info->IsCompleteState())
		return false;
	auto& mgr = GUiQuestStateInfo::GetMe();
	if (!mgr.Exist(cur_quest_info->GetDbQuest(), quest_state))
	{
		if (handle_can_done_ || handle_all_quest_)
			return cur_quest_info->Exist(quest_state);
		return false;
	}	
	return true;
}

bool GQuestResolver::DoUpdateQuestMgrInfoImpl()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto& gmap = GMap::GetMe();
	if (!gmap.Update())
	{
		assert(false);
		return false;
	}
	cur_difficulty_quest_mgr_.SetDifficultChapterInfo(gpm.GetDiffLvl(), gmap.GetChapter());
	if (!cur_difficulty_quest_mgr_.RebuildAll())
	{
		assert(false);
		return false;
	}
	total_quest_mgr_.SetDifficultChapterInfo(gpm.GetDiffLvl(), -1);
	//total_quest_mgr_.SetDifficultChapterInfo(kMD_Invalid, -1);
	if (!total_quest_mgr_.RebuildAll())
	{
		assert(false);
		return false;
	}
	return true;
}

void GQuestResolver::OnHandleOne(enHandleFlag flag, const CQH_Handler& handler)
{
	ClearAtNoneVars();
}

bool GQuestResolver::HandleStartQuest()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	//if (1 != gpm.GetRoleLvl())
	//	return false;
	auto& map = GMap::GetMe();
	if (!map.Update())
	{
		assert(false);
		return false;
	}
	auto& mgr = GUiQuestStateInfo::GetMe();
	if (!mgr.Update())
	{
		assert(false);
		return false;
	}
	if (map.GetMapName() != kMapName_JueWangYanTan || mgr.HasQuest())
		return false;
	LOG_O(Log_info) << "开始做：" << kTaskName_StartQuest;
	auto res = ToHandle(kTaskName_StartQuest, CQH_Resolver::kHF_Do);
	if (res)
		LOG_O(Log_info) << kTaskName_StartQuest << "  做成功";
	else
		LOG_O(Log_info) << kTaskName_StartQuest << "  做失败";
	return res;
}

bool GQuestResolver::HandleNoneQuest()
{
	if (handle_all_quest_) return false;
	for (auto& v : total_quest_mgr_.GetQuests())
	{
		if (!v.IsCompleteState())
		{
			LOG_O(Log_info) << "任务处理之有未完成的任务，准备去做：" << v.GetQuestName();
			handle_all_quest_ = true;
			return true;
		}
	}
	return false;
}

bool GCurQuestInfoMgr::RebuildAllImpl()
{
	quests_.clear();
	auto player_me = CD_Interface::GetPlayer();
	if (!player_me)
	{
		assert(false);
		return false;
	}
	CD_Interface::TraUiQuestItem(kTUSF_ShowHide, difficulty_, chapter_, 
		[this, player_me](enCD_kMapDifficulty difficulty, int chapter, stCD_DlgWaypointQuestItem* quest_item){
		GCurQuestInfo tmp;
		tmp.SetQuestInfoAddr(quest_item, player_me, chapter);
		if (!tmp.UpdateImpl())
		{
			assert(false);
			return false;
		}
		quests_.push_back(tmp);
		return true;
	});
	return true;
}

const GCurQuestInfo* GCurQuestInfoMgr::FindByName(const std::string& quest_name) const
{
	for (auto& v : quests_)
	{
		if (v.GetDbQuest().GetName() != quest_name)
			continue;
		return &v;
	}
	return nullptr;
}

GCurQuestInfoMgr::GCurQuestInfoMgr()
{
	difficulty_ = kMD_Invalid;
	chapter_ = -1;
}

void GCurQuestInfoMgr::SetDifficultChapterInfo(enCD_kMapDifficulty difficulty, int chapter)
{
	difficulty_ = difficulty;
	chapter_ = chapter;
}

GCurQuestInfo::GCurQuestInfo()
{
	ui_quest_item_ = nullptr;
	player_me_ = nullptr;
	chapter_ = -1;
}

void GCurQuestInfo::SetQuestInfoAddr(stCD_DlgWaypointQuestItem* quest_item, stCD_PlayerMe* attr_player, int chapter)
{
	ui_quest_item_ = quest_item;
	player_me_ = attr_player;
	chapter_ = chapter;
}

bool GCurQuestInfo::UpdateImpl()
{
	if (!ui_quest_item_)
	{
		assert(false);
		return false;
	}
	if (!ui_quest_item_->db_quest_.IsValid())
	{
		assert(false);
		return false;
	}
	if (!player_me_)
	{
		assert(false);
		return false;
	}
	auto ui_map = ui_quest_item_->GetMap();
	if (!ui_map)
	{
		assert(false);
		return false;
	}
	stCD_QuestStatesPair quest_state;
	if (player_me_->FindQuestState(quest_state, ui_quest_item_->db_quest_, ui_map->difficulty_) != &quest_state)
	{
		assert(false);
		return false;
	}
	if (!quest_state.IsValid())
	{
		assert(false);
		return false;
	}
	db_quest_.SetQuestDb(ui_quest_item_->db_quest_.tbl_item_);
	if (!db_quest_.UpdateImpl())
	{
		assert(false);
		return false;
	}
	db_quest_state_.SetQuestStateAddr(quest_state.tbl_item_);
	if (!db_quest_state_.UpdateImpl())
	{
		assert(false);
		return false;
	}
	return true;
}

const std::string& GCurQuestInfo::GetMapName() const
{
	auto& map_pin = db_quest_state_.GetMapPins();
	if (!map_pin.empty())
		return map_pin.front().GetMapName();
	auto& map_pin1 = db_quest_state_.GetMapPins1();
	if (!map_pin1.empty())
		return map_pin1.front().GetMapName();
	return kEmptyStr;
}

bool GCurQuestInfo::IsCanDoneState() const
{
	auto quest_state = db_quest_state_.GetQuestState();
	return quest_state == 1 /*|| quest_state == 2*/;
}

bool GQuestDoHandler::CanDo(const GQuestHandlerNode* parent_node) const
{
	return true;
	//下面的判断已经没有必要了，因为上一层已经判断了
	/*if (!parent_node) return false;
	auto& mgr = GUiQuestStateInfo::GetMe();
	return mgr.Exist(parent_node->GetQuestName(1), parent_node->GetQuestStateName(0));*/
}

GQuestDoHandler::GQuestDoHandler(const luabind::object& fn) : SelfType(fn)
{

}

bool GQuestDoneHandler::CanDone(const GQuestHandlerNode* parent_node) const
{
	return true;
}

GQuestDoneHandler::GQuestDoneHandler(const luabind::object& fn) : SelfType(fn)
{

}

const std::string& GQuestHandlerNode::GetQuestName(int lvl) const
{
	auto handler = GetHandler(lvl);
	assert(handler);
	return handler->GetName();
}

const std::string& GQuestHandlerNode::GetQuestStateName(int lvl) const
{
	auto handler = GetHandler(lvl);
	assert(handler);
	return handler->GetName();
}

std::string GQuestHandlerNode::GetQuestMapName(int lvl) const
{
	//auto& mgr = GQuestResolver::GetMe().GetCurDifficultyQuestMgr();
	auto& mgr = GQuestResolver::GetMe().GetTotalQuestMgr();
	auto quest = mgr.FindByName(GetQuestName(lvl + 1));
	if (!quest)
	{
		assert(false);
		return kEmptyStr;
	}
	return quest->GetMapName();
}

const char* GQuestHandlerNode::__GetQuestName__()
{
	return GetQuestName(2).c_str();
}

const char* GQuestHandlerNode::__GetQuestStateName__()
{
	return GetQuestStateName(1).c_str();
}

std::string GQuestHandlerNode::__GetQuestMapName__()
{
	return GetQuestMapName(1);
}

bool GUiQuestStateInfo::UpdateImpl()
{
	quest_state_cont_.clear();
	CD_Interface::TraUiQuestStateItem([this](stCD_UiQuestStateQuestItem* item){
		wchar_t buffer_quest[100], buffer_state[100];
		if (!item->GetQuestName(buffer_quest, sizeof(buffer_quest)))
		{
			assert(false);
			return false;
		}
		if (!item->GetQuestState(buffer_state, sizeof(buffer_state)))
		{
			assert(false);
			return false;
		}
		quest_state_cont_.push_back(stQuestState());
		auto& back = quest_state_cont_.back();
		back.quest_name_ = WideCharToGbk(buffer_quest);
		back.state_ = WideCharToGbk(buffer_state);
		return true;
	});
	return true;
}

bool GUiQuestStateInfo::Exist(const GDbQuest& db_quest, const std::string& quest_state_name) const
{
	auto& quest_name = db_quest.GetName();
	if (db_quest.IsBranchLine())
	{
		for (auto& v : quest_state_cont_)
		{
			if (v.quest_name_.find(quest_name) != 0)
				continue;
			if (v.state_ != quest_state_name)
			//if (v.state_.find(quest_state_name) == std::string::npos)
				continue;
			return true;
		}
	}
	else
	{
		for (auto& v : quest_state_cont_)
		{
			if (v.quest_name_.find(quest_name) == std::string::npos)
				continue;
			if (v.state_ != quest_state_name)
			//if (v.state_.find(quest_state_name) == std::string::npos)
				continue;
			return true;
		}
	}	
	return false;
}

bool GUiQuestStateInfo::HasQuest() const
{
	return !quest_state_cont_.empty();
}

bool GUiQuestStateInfo::HasTheQuestByName(const std::string& quest_name) const
{
	for (auto& v : quest_state_cont_)
	{
		if (v.quest_name_.find(quest_name) != std::string::npos)
			return true;
	}
	return false;
}

GQuestsHandler::GQuestsHandler(CQH_Resolver& resolver, const std::string& quest_name) 
	: GQuestsHandlerT(resolver, quest_name)
{
}

GQuestStateHandler::GQuestStateHandler(const std::string& state_name) : GQuestTryHandlerNodeT(state_name)
{

}

bool GQuestStateHandler::CanDo(const GQuestHandlerNode* parent_node) const
{
	if (!parent_node)
		return false;
	if (!GQuestResolver::GetMe().HasTheQuestState(parent_node->GetQuestName(0), GetName()))
		return false;
	return __super::CanDo(parent_node);
}

GQuestResolver::GQuestResolver()
{
	handle_can_done_ = false;
	handle_all_quest_ = false;
	handle_accept_quest_cnt_ = 0;
	stop_reason_ = kSQR_Invalid;
}

bool GQuestResolver::HandleCanDone()
{
	if (handle_can_done_) return false;
	for (auto& v : total_quest_mgr_.GetQuests())
	{
		if (v.IsCanDoneState())
		{
			LOG_O(Log_info) << "任务处理之有未提交的任务，准备去提交：" << v.GetQuestName();
			handle_can_done_ = true;
			return true;
		}
	}
	return false;
}

bool GQuestResolver::HandleAcceptQuests()
{
	if (handle_accept_quest_cnt_ > 7) return false;
	++handle_accept_quest_cnt_;
	if (GPlayerMe::GetMe().AcceptAnyQuest())
		return true;
	handle_accept_quest_cnt_ += 7;
	return false;
}

bool GQuestResolver::DoUpdateQuestMgrInfo()
{
	return GWndExecSync([this](){
		return DoUpdateQuestMgrInfoImpl();
	});
}

bool GQuestResolver::OnIdle(enHandleFlag flag)
{
	LuasScriptEchoC::ChangeRunningScript(-1);
	if (flag == kHF_Accept)
		return true;
	DummySleep(1000);
	return true;
}

CQH_SingleQuests* GQuestResolver::CreateSingleHandlerQuests(const std::string& quest_name)
{
	return new GSingleQuestsHandler(quest_name);
}

void GQuestResolver::ClearAtNoneVars()
{
	handle_can_done_ = false;
	handle_all_quest_ = false;
	handle_accept_quest_cnt_ = 0;
}

void GQuestResolver::OnHandleAll()
{
	stop_reason_ = kSQR_Invalid;
	ClearAtNoneVars();
}

int GQuestResolver::HandleAll()
{
	int cnt = 0;
	while (true)
	{
		cnt += __super::HandleAll();
		if (stop_reason_ == kSQR_AcceptNewQuest)
			continue;
		break;
	}
	return cnt;
}

void GQuestResolver::Stop(enStopQuestReason reason)
{
	stop_reason_ = reason;
	return __super::Stop();
}

void GQuestResolver::QuestStop(enStopQuestReason reason)
{
	auto resolver = GetInstance();
	if (!resolver)
	{
		assert(false);
		return;
	}
	return resolver->CastTo<GQuestResolver>()->Stop(reason);
}

GSingleQuestsHandler::GSingleQuestsHandler(const std::string& quest_name)
	: GQuestsHandlerT(quest_name)
{
}
