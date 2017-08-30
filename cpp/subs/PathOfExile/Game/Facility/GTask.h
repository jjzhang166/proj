#pragma once
/*
@author zhp
@date 2017/2/5 22:39
@purpose for task
*/
#include "GClass.h"
#include <boost/optional.hpp>
#include <vector>
#include "GameObj.h"
#include <WgFacility/CmnQuestHandler.h>
#include "GMap.h"

class GDbQuest : public GGameObjBase{
public:
	GDbQuest();
	inline void SetQuestDb(stCD_TblItemQuest* quest_Db_){ item_quest_db_ = quest_Db_; }
	void DebugInfo() const override;
	bool UpdateImpl() override;
	inline const std::string& GetName() const{ return ch_name_; }
	inline bool IsBranchLine() const{ return is_branch_line_; }

private:
	stCD_TblItemQuest*	item_quest_db_;

private:
	pt_dword			chapter_id_;		//章节		(有几个为0 id 有待确定)
	std::string			ch_name_;			//任务名
	pt_dword			sub_id_;			//任务id		(有几个为0 id 有待确定)
	bool				is_branch_line_;
};

class GDbQuestMgr : public GGameObjMgrBase, public Singleton<GDbQuestMgr, Singleton_MakeMe>{
	typedef std::vector<GDbQuest>    Cont;
public:
	GDbQuestMgr();
	bool RebuildAll() override;
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

private:
	Cont		task_db_;
};

class GDbQuestState : public GGameObjBase{
	typedef std::vector<int> Cont;

public:
	typedef std::vector<GDbMapPins> MapPinsCont;

public:
	GDbQuestState();
	void SetQuestStateAddr(stCD_TblItemQuestStates* quest_state);
	void DebugInfo() const override;
	bool UpdateImpl() override;
	//inline bool IsCompleteState() const{ return is_complete_state_; }
	inline bool IsCompleteState() const{ return quest_state_ == 0 && is_complete_state_; }
	inline const MapPinsCont& GetMapPins() const{ return map_pins_; }
	inline const MapPinsCont& GetMapPins1() const{ return map_pins1_; }
	inline pt_dword GetQuestState() const{ return quest_state_; }
	inline const std::string& GetSimplePrompt() const{ return simple_prompt_; }

private:
	stCD_TblItemQuestStates*	quest_state_ptr_;

private:
	std::string					simple_prompt_;
	pt_dword					quest_state_;		//任务状态（0：完成，从开始做某个任务，到完成状态，数值是依次减1的）
	MapPinsCont					map_pins_;
	MapPinsCont					map_pins1_;
	bool						is_complete_state_;
};

class GDbQuestStateMgr : public GGameObjMgrBase, Singleton<GDbQuestStateMgr, Singleton_MakeMe>{
	typedef std::vector<GDbQuestState>  Cont;
public:
	GDbQuestStateMgr();
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

private:
	Cont	quest_;
};

class GCurQuestInfo : public GGameObjBase{
public:
	GCurQuestInfo();
	void SetQuestInfoAddr(stCD_DlgWaypointQuestItem* quest_item, stCD_PlayerMe* attr_player, int chapter);
	bool UpdateImpl() override;
	bool IsCanDoneState() const;
	const std::string& GetMapName() const;
	inline const GDbQuest& GetDbQuest() const{ return db_quest_; }
	inline const GDbQuestState& GetDbQuestState() const{ return db_quest_state_; }
	inline bool Exist(const std::string& quest_state) const{ return db_quest_state_.GetSimplePrompt() == quest_state; }
	inline bool IsCompleteState() const{ return db_quest_state_.IsCompleteState() && GetMapName().empty(); } //下次任务验证是否可以
	inline pt_dword GetQuestState() const{ return db_quest_state_.GetQuestState(); }
	inline const std::string& GetQuestName() const{ return db_quest_.GetName(); }

private:
	stCD_DlgWaypointQuestItem*		ui_quest_item_;
	stCD_PlayerMe*					player_me_;

private:
	GDbQuest						db_quest_;
	GDbQuestState					db_quest_state_;
	int								chapter_;
};

class GCurQuestInfoMgr : public GGameObjMgrBase{
public:
	typedef std::vector<GCurQuestInfo> QuestsCont;

public:
	GCurQuestInfoMgr();
	inline const QuestsCont& GetQuests() const{ return quests_; }
	inline const enCD_kMapDifficulty Getdifficulty() const{ return difficulty_; }
	const GCurQuestInfo* FindByName(const std::string& quest_name) const;
	void SetDifficultChapterInfo(enCD_kMapDifficulty difficulty, int chapter);

private:
	bool RebuildAllImpl() override;

private:
	QuestsCont				quests_;
	enCD_kMapDifficulty		difficulty_;
	int						chapter_;
};

class GUiQuestStateInfo : public GGameObjBase, public Singleton<GUiQuestStateInfo, Singleton_MakeMe>{
public:
	struct stQuestState{
		std::string					quest_name_;
		//std::string				state_;
		std::vector<std::string>	states_;
	};

public:
	bool Exist(const GDbQuest& db_quest, const std::string& quest_state_name) const;
	bool HasQuest() const;
	bool HasTheQuestByName(const std::string& quest_name) const;

private:
	bool UpdateImpl() override;

private:
	std::vector<stQuestState>		quest_state_cont_;
};
//////////////////////////////////////////////////////////////////////////
class GQuestHandlerNode : public CQH_Node{
	friend class GQuestResolver;
public:
	const std::string& GetQuestName(int lvl) const;
	const std::string& GetQuestStateName(int lvl) const;
	std::string GetQuestMapName(int lvl) const;

private:
	const char* __GetQuestName__();
	const char* __GetQuestStateName__();
	std::string __GetQuestMapName__();
};

template<typename HandlerT>
class GQuestHandlerNodeT : public CQH_HandlerImplNodeT<GQuestHandlerNode, HandlerT>{
protected:
	typedef GQuestHandlerNodeT<HandlerT> SelfType;

protected:
	template<typename... Args>
	GQuestHandlerNodeT(const Args&... args) : CQH_HandlerImplNodeT(args...){}
};

template<typename HandlerT>
class GQuestTryHandlerNodeT : public CQH_HandlerNodeT<GQuestHandlerNode, HandlerT>{
public:
	template<typename... Args>
	GQuestTryHandlerNodeT(Args&... args) : CQH_HandlerNodeT(args...){}

private:
	bool CanAccept(const GQuestHandlerNode* parent_node) const override{
		return false;
	}
};

template<typename HandlerT>
class GQuestsHandlerT : public GQuestTryHandlerNodeT<HandlerT>{
public:
	template<typename... Args>
	GQuestsHandlerT(Args&... args) : GQuestTryHandlerNodeT(args...){}

protected:
	bool CanDo(const GQuestHandlerNode* parent_node) const override{
		if (!GQuestResolver::GetMe().HasTheQuest(GetName()))
			return false;
		return __super::CanDo(parent_node);
	}
	bool CanDone(const GQuestHandlerNode* parent_node) const override{
		auto& quest_mgr = GQuestResolver::GetMe().GetCurDifficultyQuestMgr();
		auto cur_quest = quest_mgr.FindByName(GetName());
		if (!cur_quest)
			return false;
		if (!cur_quest->IsCanDoneState())
			return false;
		return __super::CanDone(parent_node);
	}
};

class GSingleQuestsHandler : public GQuestsHandlerT<CQH_SingleQuests>{
public:
	GSingleQuestsHandler(const std::string& quest_name);
};

class GQuestsHandler : public GQuestsHandlerT<CQH_Quests>{
public:
	GQuestsHandler(CQH_Resolver& resolver, const std::string& quest_name);
};

class GQuestStateHandler : public GQuestTryHandlerNodeT<CQH_PerQuest>{
public:
	GQuestStateHandler(const std::string& state_name);

protected:
	bool CanDo(const GQuestHandlerNode* parent_node) const override;
};

class GQuestDoHandler : public GQuestHandlerNodeT<CQH_DoHandler>{
public:
	GQuestDoHandler(const luabind::object& fn);
	bool CanDo(const GQuestHandlerNode* parent_node) const override;
};

class GQuestDoneHandler : public GQuestHandlerNodeT<CQH_DoneHandler>{
public:
	GQuestDoneHandler(const luabind::object& fn);
	bool CanDone(const GQuestHandlerNode* parent_node) const override;
};

class GQuestResolver : public CQH_Resolver{
public:
	GQuestResolver();
	static void RegForLua(lua_State* l);
	static GQuestResolver& GetMe();
	inline GCurQuestInfoMgr& GetCurDifficultyQuestMgr(){ return cur_difficulty_quest_mgr_; }
	inline GCurQuestInfoMgr& GetTotalQuestMgr(){ return total_quest_mgr_; }
	bool HasTheQuest(const std::string& quest_name) const;
	bool HasTheQuestState(const std::string& quest_name, const std::string& quest_state) const;
	int HandleAll() override;
	void Stop(enStopQuestReason reason);

private:
	bool OnIdle(enHandleFlag flag) override;
	bool OnUpdateAccept() override;
	bool OnUpdateDo() override;
	bool OnUpdateDone() override;
	void OnHandleOne(enHandleFlag flag, const CQH_Handler& handler) override;
	void OnHandleAll() override;
	bool OnNoneQuest() override;
	CQH_SingleQuests* CreateSingleHandlerQuests(const std::string& quest_name) override;
	CQH_Quests* CreateHandlerQuests(const std::string& quest_name) override;
	CQH_PerQuest* CreateHandlerPerQuest(const std::string& state_name) override;
	CQH_AcceptHandler* CreateAcceptHandler(const luabind::object& fn) override;
	CQH_DoHandler* CreateDoHandler(const luabind::object& fn) override;
	CQH_DoneHandler* CreateDoneHandler(const luabind::object& fn) override;
	static void QuestStop(enStopQuestReason reason);

private:
	bool DoUpdateQuestMgrInfo();
	bool DoUpdateQuestMgrInfoImpl();

private:
	bool HandleStartQuest();
	bool HandleCanDone();
	bool HandleAcceptQuests();
	bool HandleNoneQuest();
	void ClearAtNoneVars();

private:
	GCurQuestInfoMgr	cur_difficulty_quest_mgr_;
	GCurQuestInfoMgr	total_quest_mgr_;
	enStopQuestReason	stop_reason_;
	int					handle_accept_quest_cnt_;
	bool				handle_can_done_;
	bool				handle_all_quest_;
};
//////////////////////////////////////////////////////////////////////////
