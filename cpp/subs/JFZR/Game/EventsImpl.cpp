#include "stdafx.h"
#include "EventsImpl.h"
#include "EventsName.h"
#include <luabind/function.hpp>
#include <Script/lua_base.h>
#include <Protocol/P_Defines.h>
#include "Facility/GPlayer.h"
#include "Facility/GMap.h"
#include "FuncObjC.h"

CmnEventsMgr* CmnEventsFatory_Impl::CreateEventsMgr()
{
	return new CmnEventsMgr_Impl(lua_state_);
}

CmnEventsFatory_Impl::CmnEventsFatory_Impl( lua_State* lua_state )
{
	lua_state_ = lua_state;
	assert(lua_state);
}

CmnEventsMgr_Impl::CmnEventsMgr_Impl( lua_State* lua_state ) : CmnEventsMgr(lua_state)
{
	failed_do_task_cnt_ = 0;
}

void TerminteRunTaskHandlers()
{
	using namespace luabind;
	auto event_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !event_mgr )
	{
		assert(false);
		return;
	}
	VERIFY(event_mgr->BindEventPrivateData(kPEN_AutoRunOneTaskHandler, object()));
	event_mgr->TerminateEvent(kPEN_AfterDoneOneTask);
	const auto& event_tmp = event_mgr->CreateWaitableEvent(kPEN_AfterDoneOneTask);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
}

bool DoTaskContinue()
{
	auto& gm = GMap::GetMe();
	auto& gps = GPlayerSelf::GetMe();
	gm.Update();
	if ( !gm.IsTown() )
	{
		if ( !gm.Update() )
		{
			assert(false);
			return false;
		}
	}
	if ( !gps.UpdateFatigueData() )
	{
		assert(false);
		return false;
	}
	auto event_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !event_mgr )
	{
		assert(false);
		return false;
	}
	auto fatigue_data = gps.GetFatigueData();
	if ( fatigue_data <= 0 )
	{
		assert(0 == fatigue_data);
		if ( !gm.IsTown() )
		{
			gps.ReturnCity();
		}
		event_mgr->ActiveEvent(kPEN_FatigueZero);
		return false;
	}	
	return true;
}

void AfterHandleOneTaskHandler( ICmnEvent& cmn_event )
{
	if (DoTaskContinue())
	{
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		event_mgr->ActiveEvent(kPEN_AfterDoneOneTask);
		return;
	}
}

void AutoTouchGameStateEvent()
{
	auto event_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !event_mgr )
	{
		assert(false);
		return;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	switch (nor->GetGameState())
	{
	case GType::kGS_Invalid:
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "游戏尚未初始化好，得等会";
		}
		break;
	case GType::kGS_SelectChannel:
		event_mgr->ActiveEvent(kPEN_SelectChannel);
		break;
	case GType::kGS_SelectRole:
		{
			auto cmn_timer = event_mgr->FindByName(kPPEN_WaitRetSelectRole);
			if (cmn_timer)
				cmn_timer->Discard();
			event_mgr->ActiveEvent(kPEN_SelectRole);
		}
		break;
	case GType::kGS_InGame:
		{
			GMap::GetMe().Update();
			GPlayerSelf::GetMe().Update();
			auto cmn_timer = event_mgr->FindByName(kPPEN_WaitRetSelectRole);
			if (cmn_timer)
				cmn_timer->Discard();
			event_mgr->ActiveEvent(kPEN_RoleInGame);
		}		
		break;
	default:
		assert(false);
		break;
	}
}

void AutoTouchGameEvent_( ICmnEvent& )
{
	AutoTouchGameStateEvent();
}

void CmnEventsMgr_Impl::LoadProtoEvents()
{
	auto event_tmp = CreateWaitableEvent(kPEN_AutoTouchGameEvent, AutoTouchGameEvent_);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_SelectChannel);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_SelectRole);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	/*event_tmp = CreateWaitableEvent(kPEN_CreateRole);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();*/
	event_tmp = CreateWaitableEvent(kPEN_RoleInGame, []( ICmnEvent& ){
		auto cmn_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !cmn_mgr )
		{
			assert(false);
			return;
		}
		((CmnEventsMgr_Impl*)cmn_mgr)->ResetFailedDoTaskCnt();
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_RetCity);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_EnterToFb);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_FatigueZero);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_AfterDoneOneTask);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_ChangeJob);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	//////////////////////////////////////////////////////////////////////////
	//任务处理
	event_tmp = CreateWaitableEvent(kPEN_AutoRunOneTaskHandler, []( ICmnEvent& cmn_event ){
		using namespace luabind;
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		auto lua_state = event_mgr->GetLuaState();
		assert(lua_state);
		event_mgr->BindEventData(kPEN_AfterDoneOneTask, object(lua_state, boost::ref(kPEN_AutoRunOneTaskHandler)));
		auto shared_this = cmn_event.shared_from_this();
		auto event_data = event_mgr->GetEventPrivateData(shared_this);
		if ( LUA_TNIL == type(event_data) )
		{
			event_mgr->BindEventPrivateData(shared_this, object(lua_state, boost::ref(kPPEN_AcceptTaskHandlerTasks)));
			event_mgr->ActiveEvent(kPPEN_AcceptTaskHandlerTasks);
			return;
		}
		assert(LUA_TSTRING == type(event_data));
		event_mgr->ActiveEvent(object_cast<std::string>(event_data));
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	auto inner_after_do = FindByName(kPPEN_AfterDoneRunOneTaskHandler);
	if ( !inner_after_do )
		inner_after_do = CreateWaitableEvent(kPPEN_AfterDoneRunOneTaskHandler, AfterHandleOneTaskHandler);
	if (inner_after_do)
	{
		inner_after_do->OwnershipLvlUp();
		inner_after_do->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPPEN_AcceptTaskHandlerTasks, []( ICmnEvent& cmn_event ){
		using namespace luabind;
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		const auto& event_data = event_mgr->GetEventPrivateData(kPEN_AutoRunOneTaskHandler);
		if ( LUA_TNIL == type(event_data) )
			return;
		const auto& shared_this = cmn_event.shared_from_this();
		const auto& self_data = event_mgr->GetEventPrivateData(shared_this);
		auto lua_state = event_mgr->GetLuaState();
		assert(lua_state);
		VERIFY(event_mgr->BindEventPrivateData(kPEN_AutoRunOneTaskHandler, object(lua_state, boost::ref(kPPEN_RunOneAcceptedTaskHandler))));
		if ( LUA_TNIL == type(self_data) )
		{
			const auto& tbl_globals = globals(lua_state);
			const object& func = tbl_globals["AcceptAllTaskHandlerTasks"];
			assert(LUA_TFUNCTION == type(func));
			VERIFY(event_mgr->BindEventPrivateData(kPPEN_AcceptTaskHandlerTasks, func));
			call_function<void>(func);
		}
		else
			call_function<void>(self_data);
		event_mgr->ActiveEvent(kPPEN_AfterDoneRunOneTaskHandler);
	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPPEN_RunOneAcceptedTaskHandler, []( ICmnEvent& cmn_event ){
		using namespace luabind;
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		const auto& event_data = event_mgr->GetEventPrivateData(kPEN_AutoRunOneTaskHandler);
		if ( LUA_TNIL == type(event_data) )
			return;
		const auto& shared_this = cmn_event.shared_from_this();
		auto self_data = event_mgr->GetEventPrivateData(shared_this);
		auto lua_state = event_mgr->GetLuaState();
		assert(lua_state);
		if ( LUA_TNIL == type(self_data) )
		{
			const auto& tbl_globals = globals(lua_state);
			self_data = tbl_globals["RunOneAcceptedTaskHandler"];
			assert(LUA_TFUNCTION == type(self_data));
			VERIFY(event_mgr->BindEventPrivateData(kPPEN_RunOneAcceptedTaskHandler, self_data));
		}
		if (CallFunction(self_data, false))
			VERIFY(event_mgr->BindEventPrivateData(kPEN_AutoRunOneTaskHandler, object(lua_state, boost::ref(kPPEN_AcceptTaskHandlerTasks))));
		else
			VERIFY(event_mgr->BindEventPrivateData(kPEN_AutoRunOneTaskHandler, object(lua_state, boost::ref(kPPEN_RuneOneCompletableTaskHandler))));
		event_mgr->ActiveEvent(kPPEN_AfterDoneRunOneTaskHandler);
	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPPEN_RuneOneCompletableTaskHandler, []( ICmnEvent& cmn_event ){
		using namespace luabind;
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		const auto& event_data = event_mgr->GetEventPrivateData(kPEN_AutoRunOneTaskHandler);
		if ( LUA_TNIL == type(event_data) )
			return;
		const auto& shared_this = cmn_event.shared_from_this();
		auto self_data = event_mgr->GetEventPrivateData(shared_this);
		auto lua_state = event_mgr->GetLuaState();
		assert(lua_state);
		if ( LUA_TNIL == type(self_data) )
		{
			const auto& tbl_globals = globals(lua_state);
			self_data = tbl_globals["RunOneCompletableTaskHandler"];
			assert(LUA_TFUNCTION == type(self_data));
			VERIFY(event_mgr->BindEventPrivateData(kPPEN_RuneOneCompletableTaskHandler, self_data));
		}
		if (CallFunction(self_data, false))
		{
			VERIFY(event_mgr->BindEventPrivateData(kPEN_AutoRunOneTaskHandler, object(lua_state, boost::ref(kPPEN_AcceptTaskHandlerTasks))));
			event_mgr->ActiveEvent(kPPEN_AfterDoneRunOneTaskHandler);
		}
	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	//////////////////////////////////////////////////////////////////////////
	//自动做任务
	event_tmp = CreateWaitableEvent(kPEN_AutoDoOneTask, []( ICmnEvent& ){
		using namespace luabind;
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		event_mgr->BindEventData(kPEN_AfterDoneOneTask, object(event_mgr->GetLuaState(), boost::ref(kPEN_AutoDoOneTask)));
		auto& gps = GPlayerSelf::GetMe();
		gps.CommitCompleteTasks();
		gps.AcceptLocalMapAnyAllTasks();
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	auto event_tmp2 = CreateGeneralEvent([]( ICmnEvent& ){
		using namespace luabind;
		auto& gps = GPlayerSelf::GetMe();
		if ( !gps.PreDoTask() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "无法继续全自动做任务了";
			return;
		}
		bool need_reset = true;
		if ( !gps.AutoDoneOneAcceptedTask() )
		{
			if ( !gps.AutoCompleteOneCompletedTask() )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "全自动任务之无法做一个任务，也无法完成一个任务了，意思是没有任务可做了，即将结束循环处理任务(PS.建议添加相应的任务处理)";
				//DoTaskContinue();
				//return;
				auto events_mgr = CmnEventsMgr_Impl::GetInstance();
				if ( !events_mgr )
					return;
				auto& cnt_ptr = ((CmnEventsMgr_Impl*)events_mgr)->failed_do_task_cnt_;
				++cnt_ptr;
				auto& gps = GPlayerSelf::GetMe();
				if ( 2 == cnt_ptr )
				{
					LOG_O(Log_debug) << "清空、重新生成建议城镇，并执行一次建议城镇";
					gps.ClearRecommandVillages();
					gps.GenRecommandVillages();
					if ( !gps.DoOneRecommandVillage() )
						cnt_ptr = 4;
				}
				if ( 3 == cnt_ptr )
				{
					LOG_O(Log_debug) << "再次执行一次建议城镇";
					--cnt_ptr;
					if ( !gps.DoOneRecommandVillage() )
						cnt_ptr = 4;
				}
				if ( 4 == cnt_ptr )
				{
					gps.AutoGotoCommitCompletedTask(true);
				}
				if ( 5 == cnt_ptr )
					gps.AutoGotoCommitCompletedTask(false);
				if ( cnt_ptr >= 6 )
				{
					const auto& tbl_globals = globals(events_mgr->GetLuaState());
					const object& the_func = tbl_globals["处理无法继续任务"];
					assert(LUA_TFUNCTION == type(the_func));
					if (CallFunction(the_func, false))
						cnt_ptr = 0;
				}
				if ( cnt_ptr >= 7 )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "老是无法继续任务，只能结束脚本了";
					cnt_ptr = 0;
					const auto& the_cmn_event = events_mgr->FindByName(kPPEN_KeepScriptRunningTimer);
					if (the_cmn_event)
						the_cmn_event->Discard();
					return;
				}
				need_reset = false;
			}
		}
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
			return;
		if (need_reset)
			((CmnEventsMgr_Impl*)events_mgr)->failed_do_task_cnt_ = 0;
		events_mgr->ActiveEvent(kPPEN_AfterDoneRunOneTaskHandler);
	}, kEmptyStr);
	if (event_tmp2)
		event_tmp2->OwnershipLvlUp();
	event_tmp = CreateLinkToEvent(event_tmp, event_tmp2, kEmptyStr);
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	//////////////////////////////////////////////////////////////////////////
	//mail
	event_tmp = CreateWaitableEvent(kPPEN_HandleMails);
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPEN_HandleMails, []( ICmnEvent& ){
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
		{
			assert(false);
			return;
		}
		if ( !events_mgr->ExistTheEvent(kPPEN_HandleMailRelation) )
		{
			auto inner_handle = events_mgr->FindByName(kPPEN_HandleMails);
			if ( !inner_handle )
			{
				assert(false);
				return;
			}
			auto inner_handle_impl = events_mgr->FindByName(kPPEN_HandleMailsImpl);
			if ( !inner_handle_impl )
			{
				assert(false);
				return;
			}
			inner_handle = events_mgr->CreateLinkToEvent(inner_handle, inner_handle_impl, kPPEN_HandleMailRelation);
			if ( !inner_handle )
			{
				assert(false);
				return;
			}
			inner_handle->OwnershipLvlUp();
			inner_handle->SetUnNamed();
			inner_handle_impl->ExecAsyc();
			return;
		}
		events_mgr->ActiveEvent(kPPEN_HandleMailsImpl);
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_HandleDonAllMails, []( ICmnEvent& ){
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
		{
			assert(false);
			return;
		}
		events_mgr->TerminateEvent(kPPEN_HandleMailRelation);
		GPlayerSelf::GetMe().Update();
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPPEN_HandleMailsImpl, []( ICmnEvent& ){
		auto& gps = GPlayerSelf::GetMe();
		if ( !gps.ReceiveOneMailAttachments() )
		{
			if ( !gps.RemoveOneRubbishMail() )
			{
				auto event_mgr = CmnEventsMgr_Impl::GetInstance();
				if ( !event_mgr )
				{
					assert(false);
					return;
				}
				event_mgr->ActiveEvent(kPEN_HandleDonAllMails);
			}
		}

	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	//////////////////////////////////////////////////////////////////////////
	//交易事件
	event_tmp = CreateWaitableEvent(kPEN_HandleTrade, []( ICmnEvent& ){
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
		{
			assert(false);
			return;
		}
		auto& gps = GPlayerSelf::GetMe();
		gps.Update();
		gps.ClearTradeEventQueue();
		gps.ClearSendMailList();
		auto& packet = gps.GetPacket();
		//////////////////////////////////////////////////////////////////////////
		//生成或添加交易处理事项
		packet.GetTabNormal().DoTradeItemFilter();
		packet.GetTabMaterial().DoTradeItemFilter();
		packet.GetTabConsume().DoTradeItemFilter();
		gps.PushTradeEvent(kPPEN_HandleBuyOtherProducts);
		//////////////////////////////////////////////////////////////////////////

		auto& events_queue = gps.GetTradeEventQueue();
		if (events_queue.empty())
		{
			events_mgr->ActiveEvent(kPEN_DoneTrade);
			return;
		}
		if ( !events_mgr->ExistTheEvent(kPPEN_HandleTradeRelation) )
		{
			auto inner_handle = events_mgr->FindByName(kPPEN_HandleTrade);
			if ( !inner_handle )
			{
				assert(false);
				return;
			}
			auto inner_handle_impl = events_mgr->FindByName(kPPEN_HandleTradeImpl);
			if ( !inner_handle_impl )
			{
				assert(false);
				return;
			}
			inner_handle = events_mgr->CreateLinkToEvent(inner_handle, inner_handle_impl, kPPEN_HandleTradeRelation);
			if ( !inner_handle )
			{
				assert(false);
				return;
			}
			inner_handle->OwnershipLvlUp();
			inner_handle->SetUnNamed();
			inner_handle_impl->ExecAsyc();
			return;
		}
		events_mgr->ActiveEvent(kPPEN_HandleTradeImpl);
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPEN_DoneTrade, []( ICmnEvent& ){
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
		{
			assert(false);
			return;
		}
		events_mgr->TerminateEvent(kPPEN_HandleTradeRelation);
		auto& gps = GPlayerSelf::GetMe();
		gps.ClearTradeEventQueue();
		gps.Update();
		gps.DoSendMailList();
	});
	if (event_tmp)
		event_tmp->OwnershipLvlUp();
	event_tmp = CreateWaitableEvent(kPPEN_HandleTrade);
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPPEN_HandleTradeImpl, []( ICmnEvent& ){
		auto event_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !event_mgr )
		{
			assert(false);
			return;
		}
		auto& events_queue = GPlayerSelf::GetMe().GetTradeEventQueue();
		if (events_queue.empty())
		{
			event_mgr->ActiveEvent(kPEN_DoneTrade);
			return;
		}
		event_mgr->ActiveEvent(events_queue.front());
		events_queue.pop();
	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	event_tmp = CreateWaitableEvent(kPPEN_HandleBuyOtherProducts, []( ICmnEvent& ){
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
		{
			assert(false);
			return;
		}
		if ( !GPlayerSelf::GetMe().BuyOneOtherProduct() )
			events_mgr->ActiveEvent(kPPEN_HandleTradeImpl);
	});
	if (event_tmp)
	{
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
	}
	//////////////////////////////////////////////////////////////////////////
	event_tmp = FindByName(kPPEN_KeepScriptRunningTimer);
	if ( !event_tmp )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "创建一个计时器事件，用来防止中途退出脚本";
		event_tmp = CreateTimerEvent(1000 * 60 * 5, []( ICmnEvent& the_event ){
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "防止中途退出脚本的计时器事件，正在计时中";
			return false;
		}, kPPEN_KeepScriptRunningTimer);
		if ( !event_tmp )
		{
			assert(false);
			return;
		}
		event_tmp->OwnershipLvlUp();
		event_tmp->SetUnNamed();
		event_tmp->ExecAsyc();
	}
	//////////////////////////////////////////////////////////////////////////
}

void CmnEventsMgr_Impl::ResetFailedDoTaskCnt()
{
	failed_do_task_cnt_ = 0;
}
