#include "stdafx.h"
#include "GMatch.h"
#include "FuncObjC.h"
#include "Protocol/P_NormalMsg.h"
#include "GPlayer.h"

//global functions
//点击右下角
bool PressRightBottomBt()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	func_obj_normal->GetGLua().LuaObjCall<void>("HandlePressRightBottomBt");
	return true;
}

GMatchMgr::GMatchMgr()
{
	match_mode_ = &match_mode_invalid_;

	match_mode_invalid_.SetParent(this);
	//注册支持的比赛模式
	//经理人模式
	RegisterRuntimeClass(GType::enMatchMode_Managers, GET_RUNTIME_CLASS(MatchMode_Managers));
	//自定义模式
	RegisterRuntimeClass(GType::enMatchMode_Custom, GET_RUNTIME_CLASS(MatchMode_Custom));
}

IGMatchMgr::~IGMatchMgr(){}

IGMatchMgr& IGMatchMgr::GetMe()
{
	static GMatchMgr match_mgr;
	return match_mgr;
}

bool GMatchMgr::ChooseMatch( GType::enMatchMode match_mode )
{
	assert(match_mode_);
	if ( match_mode_->GetMatchMode() == match_mode )
		//已经是同一比赛模式了
	{
		return true;
	}
	else
	{
		auto runtime_match = FindRuntimeClass(match_mode);
		if ( !runtime_match )
		{
			//不支持指定的比赛模式
			return false;		
		}

		match_mode_ = P_CAST<IMatchMode*>(runtime_match->CreateObject());
		if ( !match_mode_ )
		{
			//创建比赛模式对象失败
			SafeDelete();
			return false;
		}

		match_mode_->SetParent(this);
		//点击具体的比赛模式
		//match_mode_->PressSpecifyMatchMode(nullptr);
		return true;
	}
	
	assert(!"不能执行到这里");
}

bool GMatchMgr::IsSupportMatchMode( GType::enMatchMode match_mode )
{
	return !!FindRuntimeClass(match_mode);
}

GType::enMatchMode GMatchMgr::GetCurMatchMode() const
{
	assert(match_mode_);
	return match_mode_->GetMatchMode();
}

void GMatchMgr::SafeDelete()
{
	if ( match_mode_ != &match_mode_invalid_ )
	{
		SAFE_DELETE(match_mode_);
		assert(!match_mode_);
		match_mode_ = &match_mode_invalid_;
	}
}

GMatchMgr::~GMatchMgr()
{
	SafeDelete();
}

IMatchMode* GMatchMgr::MatchMode() const
{
	assert(match_mode_);
	return match_mode_;
}

void GMatchMgr::PressFriendshipMatch( IoTimer* timer )
{
	if (IsReadyAtSelectedScene())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		func_obj_normal->GetGLua().LuaObjCall<void>("HandlePressFriendshipMatch");

		if (IsReadyAtSelectedScene())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(500, boost::bind(&GMatchMgr::PressFriendshipMatch,
					this, _1));
			}
		}
	}
}

void GMatchMgr::PressFriendshipMatch()
{
	PressFriendshipMatch(nullptr);
}

bool GMatchMgr::IsReadyAtSelectedScene()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
	{
		return func_obj_normal->IsTrainerSelectedScene();
	}
	return false;
}

GType::stMatchInfo& GMatchMgr::GetMatchInfo()
{
	return match_info_;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(MatchMode_Managers, CObject)

GType::enMatchMode MatchMode_Managers::GetMatchMode() const
{
	return GType::enMatchMode_Managers;
}

MatchMode_Managers::MatchMode_Managers()
{
	//注册支持的对阵模式
	//对阵电脑
	RegisterRuntimeClass(GType::enFightMode_Computer, GET_RUNTIME_CLASS(FightMode_Managers_Computer));
	//对阵好友
	RegisterRuntimeClass(GType::enFightMode_Friend, GET_RUNTIME_CLASS(FightMode_Managers_Friend));
}

const char* MatchMode_Managers::GetLuaHandlePressMatchModeFuncName()
{
	return "HandlePressManagersMode";
}

void MatchMode_Managers::EnteredRoom()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
	{
		func_obj_normal->SetGameState(GType::enGameState_Managers);
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "进入到经理人模式的房间了";
	}
}

const char* MatchMode_Managers::GetLuaHandlePressBeginGameFuncName() const
{
	return "HandlePressRightBottomBt";
}

//////////////////////////////////////////////////////////////////////////

IFightMode::~IFightMode()
{

}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(FightMode_Invalid, CObject)
GType::enFightMode FightMode_Invalid::GetFightMode() const
{
	return GType::enFightMode_Invalid;
}

void FightMode_Invalid::EnterWaiting()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::EnterChoosePlayer()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::EnterMatchReady()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::PressSpecifyFightMode( IoTimer* timer )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::BeginMatch()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

bool FightMode_Invalid::CanPressChoosePlayer_Begin() const
{
	return false;
}

bool FightMode_Invalid::CanPressReady_Begin() const
{
	return false;
}

void FightMode_Invalid::MatchEnd()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::MatchEnd_Prize()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

void FightMode_Invalid::MatchEnd_Back()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "对阵模式无效";
}

//////////////////////////////////////////////////////////////////////////
bool MatchModeImpl::IsSupportFightMode( GType::enFightMode fight_mode ) const
{
	return !!FindRuntimeClass(fight_mode);
}

bool MatchModeImpl::ChooseFightMode( GType::enFightMode fight_mode )
{
	assert(fight_mode_);
	if ( fight_mode_->GetFightMode() == fight_mode )
	{
		return true;
	}
	else
	{
		auto runtime_fight = FindRuntimeClass(fight_mode);
		if ( !runtime_fight )
		{
			//不支持指定的对阵模式
			return false;		
		}

		fight_mode_ = P_CAST<FightModeImpl*>(runtime_fight->CreateObject());
		if ( !fight_mode_ )
		{
			//创建对阵模式对象失败
			SafeDelte();
			return false;
		}

		fight_mode_->SetParent(this);
		//点击具体的对阵模式
		//fight_mode_->PressSpecifyFightMode(nullptr);
		return true;
	}

	assert(!"不能执行到这里");
}

IFightMode* MatchModeImpl::FightMode() const
{
	assert(fight_mode_);
	return fight_mode_;
}

MatchModeImpl::MatchModeImpl()
{
	fight_mode_ = &fight_mode_invalid_;
	parent_ = nullptr;
	fight_mode_invalid_.SetParent(this);
}

MatchModeImpl::~MatchModeImpl()
{
	SafeDelte();
}

void MatchModeImpl::SafeDelte()
{
	if ( fight_mode_ != &fight_mode_invalid_ )
	{
		SAFE_DELETE(fight_mode_);
		assert(!fight_mode_);
		fight_mode_ = &fight_mode_invalid_;
	}
}

void MatchModeImpl::PressSpecifyMatchMode( IoTimer* timer )
{
	if (IsCanPressMatchMode())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		func_obj_normal->GetGLua().LuaObjCall<void>(GetLuaHandlePressMatchModeFuncName());

		if (IsCanPressMatchMode())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(500, boost::bind(&MatchModeImpl::PressSpecifyMatchMode,
					this, _1));
			}
		}
	}
}

bool MatchModeImpl::IsCanPressMatchMode() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
	{
		return func_obj_normal->IsTrainerSelectedScene();
	}
	return false;
}

const char* MatchModeImpl::GetLuaHandlePressMatchModeFuncName()
{
	return "LuaDoNothing";
}

void MatchModeImpl::EnteredRoom()
{

}

IGMatchMgr* MatchModeImpl::GetParent() const
{
	assert(parent_);
	return parent_;
}

void MatchModeImpl::SetParent( IGMatchMgr* parent )
{
	assert(parent);
	assert(!parent_);
	parent_ = P_CAST<GMatchMgr*>(parent);
	assert(parent_);
}

const char* MatchModeImpl::GetLuaHandlePressBeginGameFuncName() const
{
	return "LuaDoNothing";
}

void MatchModeImpl::EnterWaitingImpl( IoTimer* timer )
{
	//它会自动等待服务端的响应，并进入到choose player状态
	//即，do nothing
}

void MatchModeImpl::EnterChoosePlayerImpl( IoTimer* timer )
{
	if (fight_mode_->CanPressChoosePlayer_Begin())
	{
		if ( !PressRightBottomBt() )
			return;

		if (fight_mode_->CanPressChoosePlayer_Begin())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&MatchModeImpl::EnterChoosePlayerImpl,
					this, _1));
			}
		}
	}
}

void MatchModeImpl::EnterMatchReadyImpl( IoTimer* timer )
{
	if (fight_mode_->CanPressReady_Begin())
	{
		if ( !PressRightBottomBt() )
			return;

		if (fight_mode_->CanPressReady_Begin())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&MatchModeImpl::EnterMatchReadyImpl,
					this, _1));
			}
		}
	}
}

GType::stMatchInfo& MatchModeImpl::GetMatchInfo()
{
	return parent_->GetMatchInfo();
}

void MatchModeImpl::MatchRoom_CloseContractDlg()
{
	ImplMatchRoomCloseContractDlg(nullptr);
}

bool MatchModeImpl::IsMatchRoomContractDlgClosed() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	const auto game_state = func_obj_normal->GetGameState();
	return game_state < GType::enGameState_Managers || game_state >= GType::enGameState_Match_Wait;
}

void MatchModeImpl::ImplMatchRoomCloseContractDlg( IoTimer* timer )
{
	if ( !IsMatchRoomContractDlgClosed() )
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;

		func_obj_normal->GetGLua().LuaObjCall<void>("HandlePressDlg_Contract");
		func_obj_normal->GetGLua().LuaObjCall<void>("HandlePressDlg_DoneContract");

		if ( !IsMatchRoomContractDlgClosed() )
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(500, boost::bind(&MatchModeImpl::ImplMatchRoomCloseContractDlg,
					this, _1));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(MatchModel_Invalid, CObject)
GType::enMatchMode MatchModel_Invalid::GetMatchMode() const
{
	return GType::enMatchMode_Invalid;
}

//////////////////////////////////////////////////////////////////////////
IMatchMode::~IMatchMode()
{

}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(MatchMode_Custom, CObject)
GType::enMatchMode MatchMode_Custom::GetMatchMode() const
{
	return GType::enMatchMode_Custom;
}

MatchMode_Custom::MatchMode_Custom()
{
	//注册支持的对阵模式
	//对阵电脑
	RegisterRuntimeClass(GType::enFightMode_Computer, GET_RUNTIME_CLASS(FightMode_Computer));
	//对阵好友
	RegisterRuntimeClass(GType::enFightMode_Friend, GET_RUNTIME_CLASS(FightMode_Friend));
}

void MatchMode_Custom::EnteredRoom()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
	{
		func_obj_normal->SetGameState(GType::enGameState_Custom);
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "进入到自定义模式的房间了";
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(FightMode_Computer, CObject)

GType::enFightMode FightMode_Computer::GetFightMode() const
{
	return GType::enFightMode_Computer;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(FightMode_Friend, CObject)

GType::enFightMode FightMode_Friend::GetFightMode() const
{
	return GType::enFightMode_Friend;
}

//////////////////////////////////////////////////////////////////////////

bool FightModeImpl::IsCanPressFightMode() const
{
	return false;
}

const char* FightModeImpl::GetLuaHandlePressFightModeFuncName() const
{
	return "LuaDoNothing";
}

void FightModeImpl::PressSpecifyFightMode( IoTimer* timer )
{
	if (IsCanPressFightMode())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		func_obj_normal->GetGLua().LuaObjCall<void>(GetLuaHandlePressFightModeFuncName());

		if (IsCanPressFightMode())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&FightModeImpl::PressSpecifyFightMode,
					this, _1));
			}
		}
	}
}

FightModeImpl::FightModeImpl()
{
	parent_ = nullptr;
	match_cnt_ = 0;
	time_match_begin_ = 0;
}

IMatchMode* FightModeImpl::GetParent() const
{
	assert(parent_);
	return parent_;
}

void FightModeImpl::SetParent( IMatchMode* parent )
{
	assert(!parent_);
	parent_ = P_CAST<MatchModeImpl*>(parent);
	assert(parent_);
}

void FightModeImpl::BeginMatch()
{
	//BeginGame(nullptr);
}

void FightModeImpl::BeginMatch( IoTimer* timer )
{
	if (IsCanBegin())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		func_obj_normal->GetGLua().LuaObjCall<void>(GetParent()->GetLuaHandlePressBeginGameFuncName());

		if (IsCanBegin())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&FightModeImpl::BeginMatch,
					this, _1));
			}
		}
	}
}

bool FightModeImpl::IsCanBegin() const
{
	return false;
}

void FightModeImpl::EnterWaiting()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入到等待房间的状态";
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	func_obj_normal->SetGameState(GType::enGameState_Match_Wait);
	assert(parent_);
	parent_->EnterWaitingImpl();

	time_match_begin_ = time(nullptr);
	auto& the_timer = func_obj_normal->GetSetMatchTimer();
	the_timer.SetTimerStart(true);
	the_timer.RefreshCurTime();
	the_timer.IsTimeout();
	++match_cnt_;
}

void FightModeImpl::EnterChoosePlayer()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入到选择球员的状态";
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	func_obj_normal->SetGameState(GType::enGameState_Match_Choose_Player);
	assert(parent_);
	//parent_->EnterChoosePlayerImpl();
}

void FightModeImpl::EnterMatchReady()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入到选择比赛就绪状态";
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	func_obj_normal->SetGameState(GType::enGameState_Match_Ready);
	assert(parent_);
	//parent_->EnterMatchReadyImpl();
}

bool FightModeImpl::CanPressChoosePlayer_Begin() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	return GType::enGameState_Match_Choose_Player == func_obj_normal->GetGameState();
}

bool FightModeImpl::CanPressReady_Begin() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	return GType::enGameState_Match_Ready == func_obj_normal->GetGameState();
}

void FightModeImpl::EnterMatchAllReady()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	if ( GType::enGameState_Match_Ready != func_obj_normal->GetGameState() )
		return;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入到所有人都已就绪的状态";
	func_obj_normal->SetGameState(GType::enGameState_Match_BeginFirtAnimate);
}

void FightModeImpl::MatchEnd()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}

	//比赛场次减一
	/*GType::enPlayerPosition positions[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	auto players_iter = GPlayer::GetMe().CreatePlayerIterByPosition(positions, _countof(positions));
	if ( !players_iter )
	{
		assert(false);
		LOG_O(Log_error) << "比赛结束后竟然获得不到首发和替补的迭代器了";
		return;
	}
	for ( players_iter->First(); !players_iter->IsDone(); players_iter->Next() )
	{
		auto player = players_iter->CurItem();
		assert(player);
		if (player)
		{
			const auto player_life = player->GetLife();
			if ( player_life > 0 )
			{
				player->SetLife(player_life - 1);
			}
			else
			{
				assert(false);
				(void*)0;
			}
		}
	}*/

	if ( GType::enGameState_Match_BeginFirtAnimate > func_obj_normal->GetGameState() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "游戏状态判断不通过：" << func_obj_normal->GetGameState();
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "比赛结束矣，马上就要出比赛结果了";
	func_obj_normal->SetGameState(GType::enGameState_MatchEnd);

	//PressMatchEnd_Result(nullptr);
	auto& the_timer = func_obj_normal->GetSetMatchTimer();
	the_timer.SetTimerStart(false);

	P_Exec_MatchInfo match_info;
	match_info.time_match_end = time(nullptr);
	match_info.time_match_begin = time_match_begin_;
	match_info.match_cnt = match_cnt_;
	func_obj_normal->UserMsg(P_Exec_MatchInfo::value, &match_info, sizeof(match_info));

	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ToCall(BC_MakeP("SetMatchCnt")<<match_cnt_);
		if ( match_cnt_ >= GPlayer::GetMe().GetMatchLimitCnt() )
		{
			LOG_O(Log_warning) << "比赛次数已达上限->" << match_cnt_;

			func_obj_normal->GetSessionOper().ToCall(BC_MakeP("MatchLimitCnt"));
		}
	}
}

void FightModeImpl::MatchEnd_Prize()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	if ( GType::enGameState_MatchEnd > func_obj_normal->GetGameState() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "游戏状态判断不通过：" << func_obj_normal->GetGameState();
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "比赛结束之比赛奖励";
	func_obj_normal->SetGameState(GType::enGameState_MatchEnd_Prize);

	//PressMatchEnd_Prize(nullptr);
}

void FightModeImpl::MatchEnd_Back()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "func_obj_normal尚未创建";
		return;
	}
	if ( func_obj_normal->GetGameState() < GType::enGameState_MatchEnd )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "游戏状态判断不通过：" << func_obj_normal->GetGameState();
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "比赛结束之返回比赛房间";
	func_obj_normal->SetGameState(GType::enGameState_MatchEnd_Back);
}

bool FightModeImpl::CanPressMatchEnd_Prize() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	return GType::enGameState_MatchEnd_Prize == func_obj_normal->GetGameState();
}

void FightModeImpl::PressMatchEnd_Prize( IoTimer* timer )
{
	if (CanPressMatchEnd_Prize())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		if ( !PressRightBottomBt() )
			return;

		if (CanPressMatchEnd_Prize())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&FightModeImpl::PressMatchEnd_Prize,
					this, _1));
			}
		}
	}
}

bool FightModeImpl::CanPressMatchEnd_Result() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	return GType::enGameState_MatchEnd == func_obj_normal->GetGameState();
}

void FightModeImpl::PressMatchEnd_Result( IoTimer* timer )
{
	if (CanPressMatchEnd_Result())
	{
		auto func_obj_normal = FuncObj_NormalC::GetInstance();
		if ( !func_obj_normal )
			return;
		if ( !PressRightBottomBt() )
			return;

		if (CanPressMatchEnd_Result())
		{
			if (timer)
				timer->Continue();
			else
			{
				SetIoTimer(1000, boost::bind(&FightModeImpl::PressMatchEnd_Result,
					this, _1));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(FightMode_Managers_Computer, CObject)

bool FightMode_Managers_Computer::IsCanPressFightMode() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;
	
	const auto kGameState = func_obj_normal->GetGameState();
	if ( kGameState < GType::enGameState_Managers )
		return false;
	return kGameState != GType::enGameState_Managers_Computer;
}

const char* FightMode_Managers_Computer::GetLuaHandlePressFightModeFuncName() const
{
	assert(false);
	return "HandlePressMangers_Computer";
}

bool FightMode_Managers_Computer::IsCanBegin() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;

	return GType::enGameState_Managers_Computer == func_obj_normal->GetGameState();
}

void FightMode_Managers_Computer::BeginMatch()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
		func_obj_normal->SetGameState(GType::enGameState_Managers_Computer);
	__super::BeginMatch();
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(FightMode_Managers_Friend, CObject)

bool FightMode_Managers_Friend::IsCanPressFightMode() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;

	const auto kGameState = func_obj_normal->GetGameState();
	if ( kGameState < GType::enGameState_Managers )
		return false;
	return kGameState != GType::enGameState_Managers_Friend;
}

const char* FightMode_Managers_Friend::GetLuaHandlePressFightModeFuncName() const
{
	return "HandlePressMangers_Friend";
}

bool FightMode_Managers_Friend::IsCanBegin() const
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
		return false;

	return GType::enGameState_Managers_Friend == func_obj_normal->GetGameState();
}

void FightMode_Managers_Friend::BeginMatch()
{
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if (func_obj_normal)
		func_obj_normal->SetGameState(GType::enGameState_Managers_Friend);
	__super::BeginMatch();
}

//////////////////////////////////////////////////////////////////////////
