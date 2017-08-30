#include "stdafx.h"
#include <Common/CmnEvent.h>
#include <Common/UsefulFunc.h>
#include <Common/EnsureCleanup.h>
#include <Protocol/P_Defines.h>
#include <Common/LogOut.h>
#include <luabind/luabind.hpp>
#include <Script/lua_base.h>

//#define _CmnEventObjCount__
#ifdef _CmnEventObjCount__
int g_cmn_event_obj_cnt_ = 0;
#endif

//////////////////////////////////////////////////////////////////////////
const int kEventLuaTbl_VoidIdx = 1;
const int kEventLuaTbl_BoolIdx = 2;
const int kEventLuaTbl_BindDataIdx = 3;
const int kEventLuaTbl_PrivateDataIdx = 4;
//////////////////////////////////////////////////////////////////////////

ICmnEvent::~ICmnEvent()
{
#ifdef _CmnEventObjCount__
	LOG_O(Log_debug) << "event obj cnt minus to->" << --g_cmn_event_obj_cnt_ << " name->" << event_name_;
#endif
}

void ICmnEvent::AddSink( ICmnEventSink* sink )
{
	if ( !sink )
	{
		assert(false);
		return;
	}
	assert(!ExistTheSink(sink));
	sinks_.push_back(sink);
}

void ICmnEvent::RemoveSink( ICmnEventSink* sink )
{
	if ( !sink )
	{
		assert(false);
		return;
	}
	assert(!sinks_.empty());
	assert(ExistTheSink(sink));
	SinksT sinks_tmp;
	sinks_tmp.swap(sinks_);
	for ( auto v : sinks_tmp )
	{
		if ( sink != v )
			sinks_.push_back(v);
	}
}

void ICmnEvent::ExecAsyc()
{
	if (IsDiscard())
		return;
	if (IsWaiting())
		return;
	if (IsDoing())
		return;
	SetWating(true);
	auto shared_this = shared_from_this();
	assert(shared_this);
	GetIoService().post([shared_this](){
		assert(shared_this->IsWaiting());
		shared_this->SetWating(false);
		shared_this->Exec();
	});
}

void ICmnEvent::ExecAsyncNoSetWaiting()
{
	if (IsDoing())
	{
		assert(IsWaiting());
		SetWating(false);
	}
	auto shared_this = shared_from_this();
	assert(shared_this);
	assert(shared_this->IsWaiting());
	GetIoService().post([shared_this](){
		assert(shared_this->IsWaiting());
		shared_this->SetWating(false);
		shared_this->Exec();
	});
}

const std::string& ICmnEvent::GetEventName() const
{
	assert(!event_name_.empty());
	return event_name_;
}
void ICmnEvent::SetEventName( const std::string& event_name )
{
	assert(!event_name.empty());
	event_name_ = event_name;
#ifdef _CmnEventObjCount__
	LOG_O(Log_debug) << "event obj cnt plus to->" << ++g_cmn_event_obj_cnt_ << " name->" << event_name_;
#endif
}

void ICmnEvent::DiscardHelper()
{
	assert(!sinks_.empty());
	auto shared_this = shared_from_this();
	auto sinks_tmp = sinks_;
	for ( auto v : sinks_tmp )
	{
		if ( !ExistTheSink(v) )
			continue;
		assert(v);
		v->OnDiscard(shared_this);
	}
}

bool ICmnEvent::ExistTheSink( ICmnEventSink* sink ) const
{
	assert(sink);
	for ( auto v : sinks_ )
	{
		if ( v == sink )
			return true;
	}
	return false;
}

ICmnEvent::ICmnEvent()
{
	ownership_ = kCustom;
	named_ = false;
	last_left_event_ = nullptr;
}

ICmnEvent::ICmnEvent( const ICmnEvent& rhs ) : event_name_(rhs.event_name_)
{
	ownership_ = rhs.ownership_;
	named_ = rhs.named_;
	last_left_event_ = nullptr;
}

bool ICmnEvent::Named() const
{
	return named_;
}

void ICmnEvent::SetNamed()
{
	named_ = true;
}

ICmnEvent::enOwnership ICmnEvent::GetOwnership() const
{
	assert(kCustom == ownership_ || kProto == ownership_);
	return ownership_;
}

bool ICmnEvent::OwnershipLvlUp()
{
	if ( kCustom == ownership_ )
	{
		ownership_ = kProto;
		return true;
	}
	assert(kProto == ownership_);
	return true;
}

ICmnEvent* ICmnEvent::GetLastLeftEvent() const
{
	assert(!last_left_event_ || !last_left_event_->IsDiscard() && !last_left_event_->GetEventName().empty());
	return last_left_event_;
}

void ICmnEvent::SetLastLeftEvent( ICmnEvent* left_event )
{
	last_left_event_ = left_event;
	assert(!last_left_event_ || !last_left_event_->IsDiscard() && !last_left_event_->GetEventName().empty());
}

void ICmnEvent::SetUnNamed()
{
	named_ = false;
}

CmnTimerEvent::CmnTimerEvent( const EventTouchT& func_touched, std::size_t milli_seconds ) : func_touched_(func_touched)
{
	assert(milli_seconds > 0);
	auto ptr = new CmnEventSink_TimerDoOne(milli_seconds);
	timer_do_.reset(ptr);
	assert(timer_do_);
	AddSink(ptr);
}

CmnTimerEvent::CmnTimerEvent( const CmnTimerEvent& rhs ) : CmnEventBase(rhs), func_touched_(rhs.func_touched_)
{
	assert(rhs.timer_do_);
	auto ptr = new CmnEventSink_TimerDoOne(rhs.timer_do_->GetMilliSeconds());
	timer_do_.reset(ptr);
	assert(timer_do_);
	AddSink(ptr);
}

CmnTimerEvent::CmnEventPtrT CmnTimerEvent::Clone() const
{
	assert(timer_do_);
	return CmnEventPtrT(new CmnTimerEvent(*this));
}

bool CmnTimerEvent::Done()
{
	assert(timer_do_);
	if ( !timer_do_->IsDoing() )
		return false;
	return func_touched_(*this);
}

ICmnEventSink::~ICmnEventSink()
{
}

void CmnEventBase::Exec()
{
	if (is_discard_)
		return;
	if (is_waiting_)
		return;
	if (is_doing_)
		return;
	is_doing_ = true;
	{
		BOOST_SCOPE_EXIT_ALL(this){
			assert(is_doing_);
			is_doing_ = false;
		};
		if (Done())
		{
			if ( !is_discard_ && !sinks_.empty() )
			{
				auto shared_this = shared_from_this();
				bool need_discard = true;
				for ( auto v : sinks_ )
				{
					assert(!is_discard_);
					assert(v);
					auto on_done_result = v->OnDone(shared_this);
					assert(!is_discard_);
					if ( ICmnEventSink::kOD_Succeed == on_done_result )
						return;
					else if ( ICmnEventSink::kOD_Continue == on_done_result )
						need_discard = false;
					else
						assert(ICmnEventSink::kOD_None == on_done_result);
				}
				if ( !need_discard )
					return;
			}
			Discard();
			return;
		}
	}
	if (is_discard_)
		return;
	if (sinks_.empty())
	{
		Discard();
		return;
	}
	if (is_waiting_)
		return;
	auto shared_this = shared_from_this();
	for ( auto v : sinks_ )
	{
		assert(!is_discard_);
		assert(v);
		if (v->Help(shared_this))
		{
			assert(!is_discard_);
			break;
		}
		assert(!is_discard_);
	}
}

bool CmnEventBase::IsDiscard() const
{
	return is_discard_;
}

CmnEventBase::CmnEventBase()
{
	is_discard_ = false;
	is_waiting_ = false;
	is_doing_ = false;
}

CmnEventBase::CmnEventBase( const CmnEventBase& rhs ) : ICmnEvent(rhs)
{
	is_discard_ = false;
	is_waiting_ = false;
	is_doing_ = false;
}

void CmnEventBase::Discard()
{
	if (is_discard_)
		return;
	is_discard_ = true;
	if (sinks_.empty())
		return;
	DiscardHelper();
}

bool CmnEventBase::IsWaiting() const
{
	return is_waiting_;
}

void CmnEventBase::SetWating( bool is_wait )
{
	is_waiting_ = is_wait;
}

CmnEventBase::~CmnEventBase()
{
	assert(is_discard_);
}

bool CmnEventBase::IsDoing() const
{
	return is_doing_;
}

void CmnEventBaseSink::OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	//cmn_event->RemoveSink(this);
}

bool CmnEventBaseSink::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	/*
	cmn_event->SetWating(true);
	GetIoService().post([cmn_event](){
		cmn_event->SetWating(false);
		cmn_event->Exec();
	});
	return true;*/
	return false;
}

ICmnEventSink::enOnDone CmnEventBaseSink::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	return kOD_None;
}

bool CmnEventSink_RunOne::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	cmn_event->SetWating(true);
	GetIoService().post([cmn_event](){
		if ( GetIoService().RunOne() <= 0 )
			cmn_event->Discard();
		else
		{
			cmn_event->SetWating(false);
			cmn_event->Exec();
		}
	});
	return true;
}

CmnEventLinker::CmnEventLinker( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right ) : event_left_(event_left), event_right_(event_right)
{
	assert(event_left);
	assert(event_right);
	assert(!event_left->IsDiscard());
	assert(!event_right->IsDiscard());
	assert(event_left != event_right);
	event_left->AddSink(this);
	event_right->AddSink(this);
}

CmnEventLinker::CmnEventLinker( const CmnEventLinker& rhs )
{
	assert(!rhs.event_left_.expired());
	assert(!rhs.event_right_.expired());
	auto rhs_event_left = rhs.event_left_.lock();
	auto rhs_event_right = rhs.event_right_.lock();
	assert(rhs_event_left);
	assert(rhs_event_right);
	assert(!rhs_event_left->IsDiscard());
	assert(!rhs_event_right->IsDiscard());
	auto event_left = rhs_event_left->Clone();
	auto event_right = rhs_event_right->Clone();
	assert(event_left);
	assert(event_right);
	event_left->AddSink(this);
	event_right->AddSink(this);
	event_left_ = event_left;
	event_right_ = event_right;
}

void CmnEventLinker::OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	//__super::OnDiscard(cmn_event);
	if ( !event_left_.expired() && event_left_.lock() == cmn_event )
	{
		Disconnect();
		return;
	}
	else if ( !event_right_.expired() && event_right_.lock() == cmn_event )
	{
		Disconnect();
		return;
	}
	assert(false);
}

void CmnEventLinker::Disconnect()
{
	if ( !event_left_.expired() )
	{
		const auto& event_left = event_left_.lock();
		event_left->RemoveSink(this);
		event_left->SetLastLeftEvent(nullptr);
		event_left_.reset();
	}
	if ( !event_right_.expired() )
	{
		const auto& event_right = event_right_.lock();
		event_right->RemoveSink(this);
		event_right->SetLastLeftEvent(nullptr);
		event_right_.reset();
	}
	if ( !event_owner_.expired() )
	{
		auto ptr = event_owner_.lock();
		event_owner_.reset();
		ptr->Discard();
	}
}

void CmnEventLinker::SetOwner( const ICmnEvent::CmnEventPtrT& event_owner )
{
	assert(event_owner);
	assert(!event_owner->IsDiscard());
	event_owner_ = event_owner;
}

CmnEventLinkTo::CmnEventLinkTo( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right )
	: CmnEventLinker(event_left, event_right)
{
}

CmnEventLinkTo::CmnEventLinkTo( const CmnEventLinkTo& rhs )
	: CmnEventLinker(rhs)
{
}

CmnEventLinkTo::EventLinkerPtrT CmnEventLinkTo::CloneSink() const
{
	return EventLinkerPtrT(new CmnEventLinkTo(*this));
}

ICmnEventSink::enOnDone CmnEventLinkTo::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	if ( event_left_.expired() || event_right_.expired() )
	{
		assert(false);
		Disconnect();
		return kOD_None;
	}
	auto event_left = event_left_.lock();
	assert(event_left);
	assert(!event_left->IsDiscard());
	auto event_right = event_right_.lock();
	assert(event_right);
	assert(!event_right->IsDiscard());
	if ( event_left != cmn_event )
	{
		if ( event_right == cmn_event )
			return kOD_Continue;
		assert(false);
		Disconnect();
		return kOD_None;
	}
	if ( !event_right->IsWaiting() )
	{
		event_right->SetLastLeftEvent(event_left.get());
		event_right->ExecAsyc();
	}
	return kOD_Continue;
}

CmnEventBiLinker::CmnEventBiLinker( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right )
	: CmnEventLinker(event_left, event_right)
{
}

CmnEventBiLinker::CmnEventBiLinker( const CmnEventBiLinker& rhs )
	: CmnEventLinker(rhs)
{
}

CmnEventBiLinker::EventLinkerPtrT CmnEventBiLinker::CloneSink() const
{
	return EventLinkerPtrT(new CmnEventBiLinker(*this));
}

ICmnEventSink::enOnDone CmnEventBiLinker::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	if ( event_left_.expired() || event_right_.expired() )
	{
		assert(false);
		Disconnect();
		return kOD_None;
	}
	auto event_left = event_left_.lock();
	auto event_right = event_right_.lock();
	assert(event_left);
	assert(event_right);
	if ( cmn_event == event_right )
		event_left.swap(event_right);
	else if ( cmn_event != event_left )
	{
		assert(false);
		return kOD_None;
	}
	assert(event_left == cmn_event);
	assert(!event_left->IsDiscard());
	assert(event_right);
	assert(!event_right->IsDiscard());
	if ( !event_right->IsWaiting() )
	{
		event_right->SetLastLeftEvent(event_left.get());
		event_right->ExecAsyc();
	}
	return kOD_Continue;
}

void CmnEventsMgr::OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event )
{
	using namespace luabind;
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	if (discard_all_)
		return;
	__super::OnDiscard(cmn_event);
	const auto& event_name = cmn_event->GetEventName();
	assert(!event_name.empty());
	assert(lua_state_);
	const auto& regs = registry(lua_state_);
	assert(LUA_TTABLE == type(regs));
	const auto& the_tbl = object(regs[global_tbl_name_]);
	assert(LUA_TTABLE == type(the_tbl));
	const auto& event_obj = object(the_tbl[event_name]);
	if ( LUA_TNIL != type(event_obj) )
		the_tbl[event_name] = object();
	assert(LUA_TNIL == type(the_tbl[event_name]));

	auto it = cmn_events_.find(cmn_event);
	if ( it != cmn_events_.end() )
	{
		cmn_events_.erase(it);
		return;
	}
	else
	{
		it = timer_events_.find(cmn_event);
		if ( it != timer_events_.end() )
		{
			timer_events_.erase(it);
			return;
		}
	}
	auto ptr_it_end = linker_events_.end();
	for ( auto ptr_it = linker_events_.begin(); ptr_it != ptr_it_end; ++ptr_it )
	{
		auto& v = *ptr_it;
		assert(v.second);
		if ( v.second == cmn_event )
		{
			/*auto& ptr_pair = v.first;
			assert(ptr_pair.first);
			auto& event_left = ptr_pair.first;
			assert(event_left->Named());*/
			/*if ( !event_left->IsDiscard() && !event_left->IsWaiting() && !event_left->Named() )
			{
				
				GetIoService().post([event_left](){
					assert(event_left);
					assert(!event_left->IsDiscard());
					assert(!event_left->IsWaiting());
					assert(!event_left->Named());
					event_left->Discard();
				});
			}*/
			/*auto& event_right = ptr_pair.second;
			assert(event_right->Named());*/
			/*if ( !event_right->IsDiscard() && !event_right->IsWaiting() && !event_right->Named() )
			{

				GetIoService().post([event_right](){
					assert(event_right);
					assert(!event_right->IsDiscard());
					assert(!event_right->IsWaiting());
					assert(!event_right->Named());
					event_right->Discard();
				});
			}*/
			linker_events_.erase(ptr_it);
			return;
		}
	}
	assert(false);
}

bool CmnEventsMgr::HandleOneEvent() const
{
	auto& io_s = GetIoService();
	if ( io_s.PollOne() >= 1 )
		return true;
	if ( wait_monitor_ && wait_monitor_->IsDoing() )
		goto LABLE_RUN_ONE;
	for ( const auto& v : timer_events_ )
	{
		assert(v);
		assert(!v->IsDiscard());
		if (v->IsWaiting())
			goto LABLE_RUN_ONE;
	}
	return false;
LABLE_RUN_ONE:
	if ( io_s.RunOne() <= 0 )
	{
		assert(false);
		return false;
	}
	return true;
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::FindByName( const std::string& event_name ) const
{
	if (event_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "事件对象名不能为空";
		return CmnEventPtrT();
	}
	if ( &event_name == &kStrPlaceholder )
		return CmnEventPtrT();
	for ( const auto& v : cmn_events_ )
	{
		assert(v);
		const auto& str_event_name = v->GetEventName();
		if ( &str_event_name == &event_name || str_event_name == event_name )
			return v;
	}
	for ( const auto& v : timer_events_ )
	{
		assert(v);
		const auto& str_event_name = v->GetEventName();
		if ( &str_event_name == &event_name || str_event_name == event_name )
			return v;
	}
	for ( const auto& v : linker_events_ )
	{
		assert(v.second);
		const auto& str_event_name = v.second->GetEventName();
		if ( &str_event_name == &event_name || str_event_name == event_name )
			return v.second;
	}
	return CmnEventPtrT();
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateGeneralEvent( const CmnGeneralEvent::EventTouchT& func, const std::string& event_name )
{
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnGeneralEvent(func));
	assert(cmn_event);
	cmn_event->AddSink(this);
	BindEventName(cmn_event, event_name);
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateConditionEvent( const CmnCondtionEvent::EventCondtionT& cond_func, const CmnGeneralEvent::EventTouchT& func, const std::string& event_name )
{
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnCondtionEvent(cond_func, func));
	assert(cmn_event);
	cmn_event->AddSink(GetWaitMonitorSink());
	cmn_event->AddSink(this);
	BindEventName(cmn_event, event_name);
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateTimerEvent( std::size_t milli_seconds, const CmnTimerEvent::EventTouchT& func, const std::string& event_name )
{
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	if ( milli_seconds <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "计时器的间隔时间不能小于等于0";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnTimerEvent(func, milli_seconds));
	assert(cmn_event);
	cmn_event->AddSink(this);
	BindEventName(cmn_event, event_name);
	timer_events_.insert(cmn_event);
	return cmn_event;
}

ICmnEventSink* CmnEventsMgr::GetWaitMonitorSink()
{
	if (wait_monitor_)
		return wait_monitor_.get();
	wait_monitor_.reset(new CmnEventSink_TimerDoMulti(500));
	assert(wait_monitor_);
	return wait_monitor_.get();
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateLinkToEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, const std::string& event_name )
{
	return CreateLinkerEventImpl(event_left, event_right, true, event_name);
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateBiLinkerEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, const std::string& event_name )
{
	return CreateLinkerEventImpl(event_left, event_right, false, event_name);
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateLinkerEventImpl( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, bool is_link_to, const std::string& event_name )
{
	if ( !event_left || !event_right )
		return CmnEventPtrT();
	if ( event_left->IsDiscard() || event_right->IsDiscard() )
	{
		assert(false);
		return CmnEventPtrT();
	}
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "绑定关系事件名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	if ( event_left->GetEventName() == event_right->GetEventName() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "相同的事件名不能绑定->" << event_right->GetEventName();
		return CmnEventPtrT();
	}
	if ( event_left == event_right )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "相同的事件不能绑定";
		return CmnEventPtrT();
	}
	CmnPtrPairT two_ptr_pair(event_left, event_right);
	auto it = linker_events_.find(two_ptr_pair);
	if ( it != linker_events_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "事件已绑定过了，源事件->" << event_left->GetEventName() << " 目标事件->" << event_right->GetEventName();
		return CmnEventPtrT();
	}
	/*CmnEventPtrT left_ptr;
	if (event_left->IsDiscard())
	{
		left_ptr = event_left->Clone();
		if ( !left_ptr )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "事件连接之左边事件对象正在运行中，无法连接, 事件名->" << event_left->GetEventName();
			return CmnEventPtrT();
		}
		event_left->Discard();
		left_ptr->AddSink(GetWaitMonitorSink());
		left_ptr->AddSink(this);
	}
	else
		left_ptr = event_left;
	CmnEventPtrT right_ptr;
	if (event_right->IsDiscard())
	{
		right_ptr = event_right->Clone();
		if ( !right_ptr )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "事件连接之右边事件对象正在运行中，无法连接，事件名->" << event_right->GetEventName();
			return CmnEventPtrT();
		}
		event_right->Discard();
		right_ptr->AddSink(GetWaitMonitorSink());
		right_ptr->AddSink(this);
		cmn_events_.insert(right_ptr);
	}
	else
		right_ptr = event_right;
	if ( left_ptr != event_left )
		cmn_events_.insert(left_ptr);
	if ( right_ptr != event_right )
		cmn_events_.insert(right_ptr);*/
	CmnEventLinker* linker_ptr;
	if (is_link_to)
		linker_ptr = new CmnEventLinkTo(event_left, event_right);
	else
		linker_ptr = new CmnEventBiLinker(event_left, event_right);
	assert(linker_ptr);
	CmnLinkerEvent::EventLinkerPtrT event_linker(linker_ptr);
	assert(event_linker);
	CmnEventPtrT linker_event(new CmnLinkerEvent(event_left, event_linker));
	event_linker->SetOwner(linker_event);
	assert(linker_event);
	linker_event->AddSink(this);
	BindEventName(linker_event, event_name);
	linker_events_[two_ptr_pair] = linker_event;
	return linker_event;
}

bool CmnEventsMgr::ActiveEvent( const std::string& event_name )
{
	auto cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件名->[" << event_name << "]不存在，无法激活";
		return false;
	}
	return AsyncExecEvent(cmn_event);
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateWaitableEvent( const std::string& event_name, const CmnWaitableEvent::EventTouchT& func )
{
	if (event_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "可等待事件名不能为空";
		return CmnEventPtrT();
	}
	if (FindByName(event_name))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnWaitableEvent(func));
	assert(cmn_event);
	cmn_event->AddSink(this);
	cmn_event->SetEventName(event_name);
	cmn_event->SetNamed();
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateWaitableEvent( const std::string& event_name )
{
	if (event_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "可等待事件名不能为空";
		return CmnEventPtrT();
	}
	if (FindByName(event_name))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnWaitableEvent);
	assert(cmn_event);
	cmn_event->AddSink(this);
	cmn_event->SetEventName(event_name);
	cmn_event->SetNamed();
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

bool CmnEventsMgr::TerminateEvent( const std::string& event_name )
{
	auto cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件名->[" << event_name << "]不存在，无法中止";*/
		return false;
	}
	cmn_event->Discard();
	return true;
}

CmnEventsMgr::CmnEventsMgr( lua_State* lua_state ) : global_tbl_name_("_EA0B4008A8E7_")
{
	assert(!single_this_);
	single_this_ = this;
	discard_all_ = false;
	assert(lua_state);
	lua_state_ = lua_state;
	RegForLua();
	ResetPrivateTbl();
}

void CmnEventsMgr::TerminateAllEvents()
{
	discard_all_ = true;
	for ( auto& v : linker_events_ )
	{
		assert(v.second);
		v.second->Discard();
	}
	linker_events_.clear();

	for ( auto& v : cmn_events_ )
	{
		assert(v);
		v->Discard();
	}
	cmn_events_.clear();

	for ( auto& v : timer_events_ )
	{
		assert(v);
		v->Discard();
	}
	timer_events_.clear();

	if (wait_monitor_)
		wait_monitor_.reset();
	ResetPrivateTbl();

	discard_all_ = false;
}

bool CmnEventsMgr::BindEvent( const std::string& event_left, const std::string& event_right, const std::string& event_name )
{
	auto left_ptr = FindByName(event_left);
	if ( !left_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "绑定失败，不存在源事件名->" << event_left;
		return false;
	}
	auto right_ptr = FindByName(event_right);
	if ( !right_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "绑定失败，不存在目标事件名->" << event_right;
		return false;
	}
	if ( !CreateLinkToEvent(left_ptr, right_ptr, event_name) )
		return false;
	return true;
}

bool CmnEventsMgr::RegGeneralEvent_2_( const luabind::object& func, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegGeneralEvent(func, event_name);
}

void CmnEventsMgr::EventHandler_tbl_1_( ICmnEvent& cmn_event )
{
	using namespace luabind;
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
	{
		assert(false);
		return;
	}
	assert(mgr->lua_state_);
	const auto& the_name = cmn_event.GetEventName();
	assert(!the_name.empty());
#ifdef _DEBUG
	auto top_old = lua_gettop(mgr->lua_state_);
#endif
	const auto& lua_func = object(mgr->GetEventLuaTbl(the_name)[kEventLuaTbl_VoidIdx]);
	assert(LUA_TFUNCTION == type(lua_func));
	LuaObjCallByDlt(lua_func, true, the_name);
	assert(top_old == lua_gettop(mgr->lua_state_));
}

bool CmnEventsMgr::RegGeneralEvent( const luabind::object& func, const std::string& event_name )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册普通事件之回调函数的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateGeneralEvent(EventHandler_tbl_1_, event_name);
	if ( !cmn_event )
		return false;
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_VoidIdx] = func;
	if (event_name.empty())
		VERIFY(AsyncExecEvent(cmn_event));
	return true;
}

void CmnEventsMgr::RegForLua()
{
	static bool init_once = false;
	if (init_once)
		return;
	init_once = true;
	using namespace luabind;
	module(lua_state_)
		[
			def("处理事件", &CmnEventsMgr::HandleOneEvent_),
			def("激活事件", &CmnEventsMgr::ActiveEvent_),
			def("中止事件", &CmnEventsMgr::TerminateEvent_),
			def("中止所有事件", &CmnEventsMgr::TerminateAllEvents_),
			def("中止所有自定义事件", &CmnEventsMgr::TerminateCustomEvents_),
			def("中止所有未激活的自定义事件", &CmnEventsMgr::TerminateUnActiveCustomEvents_),
			def("存在事件", &CmnEventsMgr::ExistTheEvent_),
			def("已激活事件", &CmnEventsMgr::ActivedTheEvent_),
			def("绑定事件", &CmnEventsMgr::BindEvent_1_),
			def("绑定事件", &CmnEventsMgr::BindEvent_2_),
			def("互相绑定事件", &CmnEventsMgr::BiBindEvent_1_),
			def("互相绑定事件", &CmnEventsMgr::BiBindEvent_2_),
			def("相互绑定事件", &CmnEventsMgr::BiBindEvent_1_),
			def("相互绑定事件", &CmnEventsMgr::BiBindEvent_2_),
			def("获取当前源事件", &CmnEventsMgr::GetCurSourceEvent_),
			def("解绑事件", &CmnEventsMgr::RemoveBindEvent_),
			def("解除所有绑定关系", &CmnEventsMgr::RemoveBindRelations_1_),
			def("解除所有绑定关系", &CmnEventsMgr::RemoveBindRelations_2_),
			def("解除所有自定义的绑定关系", &CmnEventsMgr::RemoveCustomBindRelations_1_),
			def("解除所有自定义的绑定关系", &CmnEventsMgr::RemoveCustomBindRelations_2_),
			def("解除源事件所有绑定关系", &CmnEventsMgr::RemoveSourceBindRelations_),
			def("解除源事件所有自定义的绑定关系", &CmnEventsMgr::RemoveSourceCustomBindRelations_),
			def("等待触发事件", &CmnEventsMgr::WaitTouchEvent_1_),
			def("等待触发事件", &CmnEventsMgr::WaitTouchEvent_2_),
			def("遍历所有命名事件", &CmnEventsMgr::TraverseAllNamedEvent_),
			def("属于自带的事件", &CmnEventsMgr::BelongProtoEvent_),
			def("属于自定义事件", &CmnEventsMgr::BelongCustomEvent_),
			def("升级成自带的事件", &CmnEventsMgr::LvlToProtoEvent_),
			def("加载自带的事件", &CmnEventsMgr::LoadProtoEvents_),
			def("给事件绑定数据", &CmnEventsMgr::BindEventData_),
			def("获取事件绑定的数据", &CmnEventsMgr::GetEventBindData_),
			def("注册普通事件", &CmnEventsMgr::RegGeneralEvent_1_),
			def("注册普通事件", &CmnEventsMgr::RegGeneralEvent_2_),
			def("注册即时事件", &CmnEventsMgr::RegInstantEvent_1_),
			def("注册即时事件", &CmnEventsMgr::RegInstantEvent_2_),
			def("注册循环事件", &CmnEventsMgr::RegCircleEvent_1_),
			def("注册循环事件", &CmnEventsMgr::RegCircleEvent_2_),
			def("注册可等待事件", &CmnEventsMgr::RegWaitableEvent_1_),
			def("注册可等待事件", &CmnEventsMgr::RegWaitableEvent_2_),
			def("注册条件事件", &CmnEventsMgr::RegConditionEvent_1_),
			def("注册条件事件", &CmnEventsMgr::RegConditionEvent_2_),
			def("注册计时器事件", &CmnEventsMgr::RegTimerEvent_1_),
			def("注册计时器事件", &CmnEventsMgr::RegTimerEvent_2_)
		];
}

bool CmnEventsMgr::ActiveEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->ActiveEvent(event_name);
}

bool CmnEventsMgr::TerminateEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->TerminateEvent(event_name);
}

void CmnEventsMgr::TerminateAllEvents_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->TerminateAllEvents();
}

bool CmnEventsMgr::BindEvent_1_( const std::string& event_left, const std::string& event_right )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BindEvent(event_left, event_right, kEmptyStr);
}

CmnEventsMgr::~CmnEventsMgr()
{
	assert(single_this_ == this);
	single_this_ = nullptr;
}

bool CmnEventsMgr::HandleOneEvent_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->HandleOneEvent();
}

CmnEventsMgr* CmnEventsMgr::GetInstance()
{
	return single_this_;
}

luabind::object CmnEventsMgr::GetEventLuaTbl( const std::string& event_name ) const
{
	using namespace luabind;
	assert(!event_name.empty());
	assert(lua_state_);
	const auto& regs = registry(lua_state_);
	assert(LUA_TTABLE == type(regs));
	const auto& the_tbl = object(regs[global_tbl_name_]);
	assert(LUA_TTABLE == type(the_tbl));
	auto event_obj = object(the_tbl[event_name]);
	if ( LUA_TTABLE != type(event_obj) )
	{
		event_obj = newtable(lua_state_);
		the_tbl[event_name] = event_obj;
	}
	return event_obj;
}

bool CmnEventsMgr::RegWaitableEvent( const std::string& event_name )
{
	using namespace luabind;
	if ( !CreateWaitableEvent(event_name) )
		return false;
	return true;
}

bool CmnEventsMgr::RegWaitableEvent( const std::string& event_name, const luabind::object& func )
{
	using namespace luabind;
	assert(lua_state_);
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册可等待事件之回调函数的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateWaitableEvent(event_name, EventHandler_tbl_1_);
	if ( !cmn_event )
		return false;
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_VoidIdx] = func;
	return true;
}

bool CmnEventsMgr::RegConditionEvent( const luabind::object& cond_func, const luabind::object& func, const std::string& event_name )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(cond_func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册条件事件之条件回调的参数必须是函数类型->" << event_name;
		return false;
	}
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册条件事件之触发回调的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateConditionEvent(EventHandler_tbl_2_, EventHandler_tbl_1_, event_name);
	if ( !cmn_event )
		return false;
	auto tbl_obj = GetEventLuaTbl(cmn_event->GetEventName());
	assert(LUA_TTABLE == type(tbl_obj));
	tbl_obj[kEventLuaTbl_VoidIdx] = func;
	tbl_obj[kEventLuaTbl_BoolIdx] = cond_func;
	if (event_name.empty())
		VERIFY(AsyncExecEvent(cmn_event));
	return true;
}

bool CmnEventsMgr::EventHandler_tbl_2_( ICmnEvent& cmn_event )
{
	using namespace luabind;
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
	{
		assert(false);
		return true;
	}
	const auto& the_name = cmn_event.GetEventName();
	assert(!the_name.empty());
	return LuaObjCallByDlt(mgr->GetEventLuaTbl(the_name)[kEventLuaTbl_BoolIdx], true, the_name);
}

bool CmnEventsMgr::RegTimerEvent( std::size_t milli_seconds, const luabind::object& func, const std::string& event_name )
{
	using namespace luabind;
	if ( milli_seconds <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册计时器事件之计时间隔必须大于0";
		return false;
	}
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册计时器事件之回调函数的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateTimerEvent(milli_seconds, EventHandler_tbl_2_, event_name);
	if ( !cmn_event )
		return false;
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BoolIdx] = func;
	if (event_name.empty())
		VERIFY(AsyncExecEvent(cmn_event));
	return true;
}

bool CmnEventsMgr::RegWaitableEvent_1_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegWaitableEvent(event_name);
}

bool CmnEventsMgr::RegWaitableEvent_2_( const std::string& event_name, const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegWaitableEvent(event_name, func);
}

bool CmnEventsMgr::RegConditionEvent_2_( const luabind::object& cond_func, const luabind::object& func, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegConditionEvent(cond_func, func, event_name);
}

bool CmnEventsMgr::RegTimerEvent_2_( std::size_t milli_seconds, const luabind::object& func, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegTimerEvent(milli_seconds, func, event_name);
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateCircleEvent( const CmnCircleEvent::EventTouchT& func, const std::string& event_name )
{
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnCircleEvent(func));
	assert(cmn_event);
	cmn_event->AddSink(GetWaitMonitorSink());
	cmn_event->AddSink(this);
	BindEventName(cmn_event, event_name);
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

bool CmnEventsMgr::RegCircleEvent( const luabind::object& func, const std::string& event_name )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册循环事件之回调函数的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateCircleEvent(EventHandler_tbl_2_, event_name);
	if ( !cmn_event )
		return false;
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BoolIdx] = func;
	if (event_name.empty())
		VERIFY(AsyncExecEvent(cmn_event));
	return true;
}

bool CmnEventsMgr::RegCircleEvent_2_( const luabind::object& func, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegCircleEvent(func, event_name);
}

bool CmnEventsMgr::RegCircleEvent_1_( const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegCircleEvent(func, kEmptyStr);
}

bool CmnEventsMgr::RegGeneralEvent_1_( const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegGeneralEvent(func, kEmptyStr);
}

bool CmnEventsMgr::RegConditionEvent_1_( const luabind::object& cond_func, const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegConditionEvent(cond_func, func, kEmptyStr);
}

bool CmnEventsMgr::RegTimerEvent_1_( std::size_t milli_seconds, const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegTimerEvent(milli_seconds, func, kEmptyStr);
}

bool CmnEventsMgr::AsyncExecEvent( const CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	if ( cmn_event->IsDiscard() )
		return false;
	cmn_event->ExecAsyc();
	return true;
}

bool CmnEventsMgr::ExistTheEvent( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	assert(!cmn_event->IsDiscard());
	return !cmn_event->IsDiscard();
}

bool CmnEventsMgr::ActivedTheEvent( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	if (cmn_event->IsDiscard())
		return false;
	return cmn_event->IsWaiting() || cmn_event->IsDoing();
}

void CmnEventsMgr::ResetPrivateTbl()
{
	using namespace luabind;
	assert(lua_state_);
	const auto& regs = registry(lua_state_);
	assert(LUA_TTABLE == type(regs));
	const auto& new_tbl = newtable(lua_state_);
	assert(LUA_TTABLE == type(new_tbl));
	regs[global_tbl_name_] = new_tbl;
}

bool CmnEventsMgr::RemoveBindEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right )
{
	assert(event_left);
	assert(event_right);
	CmnPtrPairT ptr_pair(event_left, event_right);
	auto it = linker_events_.find(ptr_pair);
	if ( it == linker_events_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "解绑失败，源事件->" << event_left->GetEventName() << " 与目标事件->" << event_right->GetEventName() << " 不存在关系";
		return false;
	}
	assert(it->second);
	it->second->Discard();
	return true;
}

bool CmnEventsMgr::RemoveBindEvent( const std::string& event_left, const std::string& event_right )
{
	auto left_ptr = FindByName(event_left);
	if ( !left_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法解绑，不存在源事件名->" << event_left;
		return false;
	}
	auto right_ptr = FindByName(event_right);
	if ( !right_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法解绑，不存在目标事件名->" << event_right;
		return false;
	}
	return RemoveBindEvent(event_left, event_right);
}

void CmnEventsMgr::BindEventName( CmnEventPtrT& cmn_event, const std::string& event_name )
{
	assert(cmn_event);
	if (event_name.empty())
	{
		const auto& kUniqueStr = RandomUniqueStr();
		assert(!kUniqueStr.empty());
		assert(36 == kUniqueStr.size());
		assert(!FindByName(kUniqueStr));
		cmn_event->SetEventName(kUniqueStr);
	}
	else
	{
		cmn_event->SetEventName(event_name);
		cmn_event->SetNamed();
	}
}

CmnEventsMgr::TwoPtr2PtrMapT::const_iterator CmnEventsMgr::FindTwoPtrMap( const TwoPtr2PtrMapT& two_ptr_map, const CmnPtrPairT& two_ptr_pair )
{
	auto it_end = two_ptr_map.end();
	for ( auto it = two_ptr_map.begin(); it != it_end; ++it )
	{
		auto& v = it->first;
		if ( v.first == two_ptr_pair.first )
		{
			if ( v.second == two_ptr_pair.second )
				return it;
		}
		else if ( v.second == two_ptr_pair.first )
		{
			if ( v.first == two_ptr_pair.second )
				return it;
		}
	}
	return it_end;
}

bool CmnEventsMgr::ExistTheEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->ExistTheEvent(event_name);
}

bool CmnEventsMgr::ActivedTheEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->ActivedTheEvent(event_name);
}

bool CmnEventsMgr::RemoveBindEvent_( const std::string& event_left, const std::string& event_right )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RemoveBindEvent(event_left, event_right);
}

bool CmnEventsMgr::BiBindEvent( const std::string& event_left, const std::string& event_right, const std::string& event_name )
{
	auto left_ptr = FindByName(event_left);
	if ( !left_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "互相绑定失败，不存在源事件名->" << event_left;
		return false;
	}
	auto right_ptr = FindByName(event_right);
	if ( !right_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "互相绑定失败，不存在目标事件名->" << event_right;
		return false;
	}
	if ( !CreateBiLinkerEvent(left_ptr, right_ptr, event_name) )
		return false;
	return true;
}

bool CmnEventsMgr::BindEvent_2_( const std::string& event_left, const std::string& event_right, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BindEvent(event_left, event_right, event_name);
}

bool CmnEventsMgr::BiBindEvent_1_( const std::string& event_left, const std::string& event_right )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BiBindEvent(event_left, event_right, kEmptyStr);
}

bool CmnEventsMgr::BiBindEvent_2_( const std::string& event_left, const std::string& event_right, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BiBindEvent(event_left, event_right, event_name);
}

bool CmnEventsMgr::WaitTouchEvent( const CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	if (cmn_event->IsDiscard())
		return false;
	bool event_touch = false;
	CmnWaitTouchSink wait_sink(cmn_event, event_touch);
	auto& io_s = GetIoService();
	while ( io_s.RunOne() >= 1 )
	{
		if (event_touch)
			break;
	}
	assert(event_touch);
	return event_touch;
}

bool CmnEventsMgr::WaitTouchEvent( const CmnEventPtrT& cmn_event, std::size_t timeout )
{
	assert(cmn_event);
	if ( timeout <= 0 )
		return WaitTouchEvent(cmn_event);
	if (cmn_event->IsDiscard())
		return false;
	bool event_touch = false;
	CmnWaitTouchSink wait_sink(cmn_event, event_touch);
	WaitUntil([&event_touch](){
		return event_touch;
	}, timeout);
	return event_touch;
}

bool CmnEventsMgr::WaitTouchEvent( const std::string& event_name )
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	return WaitTouchEvent(cmn_event);
}

bool CmnEventsMgr::WaitTouchEvent( const std::string& event_name, std::size_t timeout )
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	return WaitTouchEvent(cmn_event, timeout);
}

bool CmnEventsMgr::WaitTouchEvent_1_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->WaitTouchEvent(event_name);
}

bool CmnEventsMgr::WaitTouchEvent_2_( const std::string& event_name, std::size_t timeout )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->WaitTouchEvent(event_name, timeout);
}

void CmnEventsMgr::TraverseAllNamedEvent( const luabind::object& func ) const
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "遍历所有命名事件之回调函数必须是函数类型";
		return;
	}
	auto cmn_events_tmp = cmn_events_;
	for ( auto& v : cmn_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( !cmn_event.IsDiscard() && cmn_event.Named() )
		{
			if (LuaObjCallByDlt(func, false, cmn_event.GetEventName()))
				return;
		}
	}
	auto timer_events_tmp = timer_events_;
	for ( auto& v : timer_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( !cmn_event.IsDiscard() && cmn_event.Named() )
		{
			if (LuaObjCallByDlt(func, false, cmn_event.GetEventName()))
				return;
		}
	}
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		if ( !cmn_event.IsDiscard() && cmn_event.Named() )
		{
			if (LuaObjCallByDlt(func, false, cmn_event.GetEventName()))
				return;
		}
	}
}

void CmnEventsMgr::TraverseAllNamedEvent_( const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->TraverseAllNamedEvent(func);
}

void CmnEventsMgr::TerminateCustomEvents()
{
	auto cmn_events_tmp = cmn_events_;
	for ( auto& v : cmn_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() )
			cmn_event.Discard();
	}
	auto timer_events_tmp = timer_events_;
	for ( auto& v : timer_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() )
			cmn_event.Discard();
	}
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() )
			cmn_event.Discard();
	}
}

void CmnEventsMgr::TerminateUnActiveCustomEvents()
{
	auto cmn_events_tmp = cmn_events_;
	for ( auto& v : cmn_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && !cmn_event.IsWaiting() )
			cmn_event.Discard();
	}
	auto timer_events_tmp = timer_events_;
	for ( auto& v : timer_events_tmp )
	{
		assert(v);
		auto& cmn_event = *v;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && !cmn_event.IsWaiting() )
			cmn_event.Discard();
	}
	/*auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && !cmn_event.IsWaiting() )
			cmn_event.Discard();
	}*/
}

bool CmnEventsMgr::BelongProtoEvent( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	assert(!cmn_event->IsDiscard());
	return ICmnEvent::kProto == cmn_event->GetOwnership();
}

bool CmnEventsMgr::BelongCustomEvent( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return false;
	assert(!cmn_event->IsDiscard());
	return ICmnEvent::kCustom == cmn_event->GetOwnership();
}

bool CmnEventsMgr::LvlToProtoEvent( const std::string& event_name )
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "升级成自带的事件 失败";
		return false;
	}
	assert(!cmn_event->IsDiscard());
	return cmn_event->OwnershipLvlUp();
}

bool CmnEventsMgr::BindEventData( const CmnEventPtrT& cmn_event, const luabind::object& event_data )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BindDataIdx] = event_data;
	return true;
}

bool CmnEventsMgr::BindEventData( const std::string& event_name, const luabind::object& event_data )
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "给事件绑定数据 失败";
		return false;
	}
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BindDataIdx] = event_data;
	return true;
}

luabind::object CmnEventsMgr::GetEventBindData( const CmnEventPtrT& cmn_event ) const
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	return GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BindDataIdx];
}

luabind::object CmnEventsMgr::GetEventBindData( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获取事件绑定的数据 失败";
		return luabind::object();
	}
	return GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BindDataIdx];
}

void CmnEventsMgr::TerminateCustomEvents_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->TerminateCustomEvents();
}

void CmnEventsMgr::TerminateUnActiveCustomEvents_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->TerminateUnActiveCustomEvents();
}

bool CmnEventsMgr::BelongProtoEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BelongProtoEvent(event_name);
}

bool CmnEventsMgr::BelongCustomEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BelongCustomEvent(event_name);
}

bool CmnEventsMgr::LvlToProtoEvent_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->LvlToProtoEvent(event_name);
}

void CmnEventsMgr::LoadProtoEvents_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->LoadProtoEvents();
}

bool CmnEventsMgr::BindEventData_( const std::string& event_name, const luabind::object& event_data )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->BindEventData(event_name, event_data);
}

luabind::object CmnEventsMgr::GetEventBindData_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return luabind::object();
	return mgr->GetEventBindData(event_name);
}

CmnEventsMgr::CmnEventPtrT CmnEventsMgr::CreateInstantEvent( const CmnInstantEvent::EventTouchT& func, const std::string& event_name )
{
	if ( !event_name.empty() && FindByName(event_name) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "事件对象名->[" << event_name << "]已存在";
		return CmnEventPtrT();
	}
	CmnEventPtrT cmn_event(new CmnInstantEvent(func));
	assert(cmn_event);
	cmn_event->AddSink(this);
	BindEventName(cmn_event, event_name);
	cmn_events_.insert(cmn_event);
	return cmn_event;
}

bool CmnEventsMgr::RegInstantEvent( const luabind::object& func, const std::string& event_name )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "注册即时事件之回调函数的参数必须是函数类型->" << event_name;
		return false;
	}
	const auto& cmn_event = CreateInstantEvent(EventHandler_tbl_2_, event_name);
	if ( !cmn_event )
		return false;
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_BoolIdx] = func;
	if (event_name.empty())
		VERIFY(AsyncExecEvent(cmn_event));
	return true;
}

bool CmnEventsMgr::RegInstantEvent_1_( const luabind::object& func )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegInstantEvent(func, kEmptyStr);
}

bool CmnEventsMgr::RegInstantEvent_2_( const luabind::object& func, const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return false;
	return mgr->RegInstantEvent(func, event_name);
}

const std::string& CmnEventsMgr::GetCurSourceEvent( const std::string& rhs_event ) const
{
	const auto& cmn_event = FindByName(rhs_event);
	if ( !cmn_event )
	 return kStrPlaceholder;
	assert(!cmn_event->IsDiscard());
	auto cmn_lhs = cmn_event->GetLastLeftEvent();
	if ( !cmn_lhs )
		return kStrPlaceholder;
	assert(!cmn_lhs->IsDiscard());
	return cmn_lhs->GetEventName();
}

void CmnEventsMgr::RemoveBindRelations()
{
	if (linker_events_.empty())
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		cmn_event.Discard();
	}
}

void CmnEventsMgr::RemoveBindRelations( const CmnEventPtrT& the_cmn_event )
{
	assert(!linker_events_.empty());
	assert(the_cmn_event);
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		if ( v.first.first == the_cmn_event || v.first.second == the_cmn_event )
		{
			assert(v.second);
			assert(!v.second->IsDiscard());
			v.second->Discard();
		}
	}
}

void CmnEventsMgr::RemoveBindRelations( const std::string& event_name )
{
	if (linker_events_.empty())
		return;
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		if ( v.first.first == cmn_event || v.first.second == cmn_event )
		{
			assert(v.second);
			assert(!v.second->IsDiscard());
			v.second->Discard();
		}
	}
}

void CmnEventsMgr::RemoveCustomBindRelations()
{
	if (linker_events_.empty())
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() )
			cmn_event.Discard();
	}
}

void CmnEventsMgr::RemoveCustomBindRelations( const CmnEventPtrT& the_cmn_event )
{
	assert(!linker_events_.empty());
	assert(the_cmn_event);
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && (v.first.first == the_cmn_event || v.first.second == the_cmn_event) )
			cmn_event.Discard();
	}
}

void CmnEventsMgr::RemoveCustomBindRelations( const std::string& event_name )
{
	if (linker_events_.empty())
		return;
	const auto& the_event = FindByName(event_name);
	if ( !the_event )
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && (v.first.first == the_event || v.first.second == the_event) )
			cmn_event.Discard();
	}
}

void CmnEventsMgr::RemoveSourceBindRelations( const CmnEventPtrT& event_left )
{
	assert(!linker_events_.empty());
	assert(event_left);
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		if ( v.first.first == event_left )
		{
			assert(v.second);
			auto& cmn_event = *v.second;
			assert(!cmn_event.IsDiscard());
			cmn_event.Discard();
		}
	}
}

void CmnEventsMgr::RemoveSourceBindRelations( const std::string& event_left )
{
	if (linker_events_.empty())
		return;
	const auto& the_event = FindByName(event_left);
	if ( !the_event )
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		if ( v.first.first == the_event )
		{
			assert(v.second);
			auto& cmn_event = *v.second;
			assert(!cmn_event.IsDiscard());
			cmn_event.Discard();
		}
	}
}

void CmnEventsMgr::RemoveSourceCustomBindRelations( const CmnEventPtrT& event_left )
{
	assert(!linker_events_.empty());
	assert(event_left);
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && v.first.first == event_left )
			cmn_event.Discard();
	}
}

void CmnEventsMgr::RemoveSourceCustomBindRelations( const std::string& event_left )
{
	if (linker_events_.empty())
		return;
	const auto& the_event = FindByName(event_left);
	if ( !the_event )
		return;
	auto linker_events_tmp = linker_events_;
	for ( auto& v : linker_events_tmp )
	{
		assert(v.second);
		auto& cmn_event = *v.second;
		assert(!cmn_event.IsDiscard());
		if ( ICmnEvent::kCustom == cmn_event.GetOwnership() && v.first.first == the_event )
			cmn_event.Discard();
	}
}

const std::string& CmnEventsMgr::GetCurSourceEvent_( const std::string& rhs_event )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return kStrPlaceholder;
	return mgr->GetCurSourceEvent(rhs_event);
}

void CmnEventsMgr::RemoveBindRelations_1_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveBindRelations();
}

void CmnEventsMgr::RemoveBindRelations_2_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveBindRelations(event_name);
}

void CmnEventsMgr::RemoveCustomBindRelations_1_()
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveCustomBindRelations();
}

void CmnEventsMgr::RemoveCustomBindRelations_2_( const std::string& event_name )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveCustomBindRelations(event_name);
}

void CmnEventsMgr::RemoveSourceBindRelations_( const std::string& event_left )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveSourceBindRelations(event_left);
}

void CmnEventsMgr::RemoveSourceCustomBindRelations_( const std::string& event_left )
{
	auto mgr = CmnEventsMgr::GetInstance();
	if ( !mgr )
		return;
	mgr->RemoveSourceCustomBindRelations(event_left);
}

bool CmnEventsMgr::BindEventPrivateData( const std::string& event_name, const luabind::object& event_private_data )
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "给事件绑定私有数据 失败->" << event_name;
		return false;
	}
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_PrivateDataIdx] = event_private_data;
	return true;
}

bool CmnEventsMgr::BindEventPrivateData( const CmnEventPtrT& cmn_event, const luabind::object& event_private_data )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_PrivateDataIdx] = event_private_data;
	return true;
}

luabind::object CmnEventsMgr::GetEventPrivateData( const std::string& event_name ) const
{
	const auto& cmn_event = FindByName(event_name);
	if ( !cmn_event )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获取事件绑定的私有数据 失败->" << event_name;
		return luabind::object();
	}
	return GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_PrivateDataIdx];
}

luabind::object CmnEventsMgr::GetEventPrivateData( const CmnEventPtrT& cmn_event ) const
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	return GetEventLuaTbl(cmn_event->GetEventName())[kEventLuaTbl_PrivateDataIdx];
}

lua_State* CmnEventsMgr::GetLuaState() const
{
	assert(lua_state_);
	return lua_state_;
}

CmnEventsMgr* CmnEventsMgr::single_this_ = nullptr;

CmnEventDecorator::CmnEventDecorator( const CmnEventPtrT& cmn_event ) : handler_(cmn_event)
{
	assert(cmn_event);
	assert(this != cmn_event.get());
	cmn_event->AddSink(this);
}

CmnEventDecorator::CmnEventDecorator( const CmnEventDecorator& rhs ) : ICmnEvent(rhs)
{
	assert(rhs.handler_);
	handler_ = rhs.handler_->Clone();
	assert(handler_);
	handler_->AddSink(this);
}

bool CmnEventDecorator::IsDiscard() const
{
	if ( !handler_ )
		return true;
	return handler_->IsDiscard();
}

void CmnEventDecorator::Discard()
{
	if ( !handler_ )
		return;
	handler_->RemoveSink(this);
	handler_->Discard();
	handler_.reset();
	if (sinks_.empty())
		return;
	DiscardHelper();
}

void CmnEventDecorator::Exec()
{
	assert(handler_);
	handler_->Exec();
}

void CmnEventDecorator::OnDiscard( CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	assert(handler_ == cmn_event);
	handler_.reset();
	cmn_event->RemoveSink(this);
	if (sinks_.empty())
		return;
	DiscardHelper();
	__super::OnDiscard(cmn_event);
}

bool CmnEventDecorator::IsWaiting() const
{
	assert(handler_);
	return handler_->IsWaiting();
}

void CmnEventDecorator::SetWating( bool is_wait )
{
	assert(handler_);
	handler_->SetWating(is_wait);
}

CmnEventDecorator::CmnEventPtrT CmnEventDecorator::Clone() const
{
	return CmnEventPtrT(new CmnEventDecorator(*this));
}

CmnEventDecorator::~CmnEventDecorator()
{
	assert(!handler_);
}

bool CmnEventDecorator::IsDoing() const
{
	assert(handler_);
	return handler_->IsDoing();
}

CmnLinkerEvent::CmnLinkerEvent( const CmnEventPtrT& cmn_event, const EventLinkerPtrT& event_linker ) : CmnEventDecorator(cmn_event), linker_(event_linker)
{
	assert(event_linker);
}

CmnLinkerEvent::CmnLinkerEvent( const CmnLinkerEvent& rhs ) : CmnEventDecorator(rhs)
{
	assert(!rhs.IsDiscard());
	assert(rhs.linker_);
	linker_ = rhs.linker_->CloneSink();
	assert(linker_);
}

CmnLinkerEvent::CmnEventPtrT CmnLinkerEvent::Clone() const
{
	return CmnEventPtrT(new CmnLinkerEvent(*this));
}

void CmnLinkerEvent::Discard()
{
	if ( !linker_ || !handler_ )
		return;
	if (sinks_.empty())
	{
		__super::Discard();
		assert(!handler_);
		linker_->Disconnect();
		linker_.reset();
		return;
	}
	assert(handler_);
	handler_->RemoveSink(this);
	handler_.reset();
	assert(!handler_);
	auto linker_tmp = linker_;
	linker_.reset();
	linker_tmp->Disconnect();
	DiscardHelper();
}

bool CmnLinkerEvent::IsDiscard() const
{
	if ( !linker_ )
		return true;
	return __super::IsDiscard();
}

void CmnLinkerEvent::OnDiscard( CmnEventPtrT& cmn_event )
{
	assert(linker_);
	linker_->Disconnect();
	linker_.reset();
	__super::OnDiscard(cmn_event);
}

CmnGeneralEvent::CmnGeneralEvent( const EventTouchT& func_touched ) : func_touched_(func_touched)
{
}

CmnGeneralEvent::CmnGeneralEvent( const CmnGeneralEvent& rhs ) : CmnEventBase(rhs), func_touched_(rhs.func_touched_)
{

}

bool CmnGeneralEvent::Done()
{
	if (is_discard_)
		return false;
	func_touched_(*this);
	return true;
}

CmnGeneralEvent::CmnEventPtrT CmnGeneralEvent::Clone() const
{
	return CmnEventPtrT(new CmnGeneralEvent(*this));
}

CmnCondtionEvent::CmnCondtionEvent( const EventCondtionT& event_cond, const EventTouchT& func_touched )
	: CmnGeneralEvent(func_touched), event_cond_(event_cond)
{

}

CmnCondtionEvent::CmnCondtionEvent( const CmnCondtionEvent& rhs ) : CmnGeneralEvent(rhs), event_cond_(rhs.event_cond_)
{

}

bool CmnCondtionEvent::Done()
{
	if (event_cond_(*this))
		return __super::Done();
	return false;
}

CmnCondtionEvent::CmnEventPtrT CmnCondtionEvent::Clone() const
{
	return CmnEventPtrT(new CmnCondtionEvent(*this));
}

bool CmnEventSink_TimerDo::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	cmn_event->SetWating(true);
	if ( !timer_ )
	{
		SinkTimerDoImpl_ do_tmp(shared_from_this());
		assert(milli_seconds_ > 0);
		timer_ = CreateIoTimer(milli_seconds_, [do_tmp]( const IoTimerPtr& timer ){
			do_tmp.Do(timer);
		}, false);
		assert(timer_);
	}
	timer_->Continue();
	return true;
}

CmnEventSink_TimerDo::CmnEventSink_TimerDo( std::size_t milli_seconds )
{
	assert(milli_seconds > 0);
	milli_seconds_ = milli_seconds;
}

std::size_t CmnEventSink_TimerDo::GetMilliSeconds() const
{
	return milli_seconds_;
}

CmnEventSink_TimerDo::SinkTimerDoImpl_::SinkTimerDoImpl_( const SharedPtrT& shared_this )
	: weak_this_(shared_this)
{
}

void CmnEventSink_TimerDo::SinkTimerDoImpl_::Do( const IoTimerPtr& timer ) const
{
	WeakPtrT& weak_this = const_cast<WeakPtrT&>(weak_this_);
	if (weak_this.expired())
	{
		weak_this.reset();
		return;
	}
	assert(timer);
	auto shared_this = weak_this_.lock();
	assert(shared_this);
	assert(timer == shared_this->timer_);
	if (shared_this->DoOne())
		timer->Continue();
}

bool CmnEventSink_TimerDoMulti::DoOne()
{
	while ( !unhandled_events_.empty() )
	{
		auto& weakptr = unhandled_events_.front();
		if (weakptr.expired())
		{
			unhandled_events_.pop();
			continue;
		}
		auto event_ptr = weakptr.lock();
		assert(event_ptr);
		if (event_ptr->IsDiscard())
		{
			unhandled_events_.pop();
			continue;
		}
		unhandled_events_.pop();
		event_ptr->ExecAsyncNoSetWaiting();
		return !unhandled_events_.empty();
	}
	return false;
}

bool CmnEventSink_TimerDoMulti::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	if ( !__super::Help(cmn_event) )
		return false;
	unhandled_events_.push(cmn_event);
	return true;
}

CmnEventSink_TimerDoMulti::CmnEventSink_TimerDoMulti( std::size_t milli_seconds ) : CmnEventSink_TimerDo(milli_seconds)
{

}

bool CmnEventSink_TimerDoMulti::IsDoing() const
{
	if (unhandled_events_.empty())
		return false;
	assert(timer_);
	assert(timer_->IsClocking());
	return true;
}

CmnEventSink_TimerDoOne::CmnEventSink_TimerDoOne( std::size_t milli_seconds ) : CmnEventSink_TimerDo(milli_seconds)
{

}

void CmnEventSink_TimerDoOne::OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	if (timer_)
	{
		timer_->Cancel();
		timer_.reset();
	}
	if (cmn_event_.expired())
		return;
	assert(cmn_event == cmn_event_.lock());
	cmn_event_.reset();
}

bool CmnEventSink_TimerDoOne::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	if ( !__super::Help(cmn_event) )
		return false;
	cmn_event_ = cmn_event;
	return true;
}

bool CmnEventSink_TimerDoOne::DoOne()
{
	if (cmn_event_.expired())
		return false;
	auto sharedptr = cmn_event_.lock();
	assert(sharedptr);
	if (sharedptr->IsDiscard())
		return false;
	assert(sharedptr->IsWaiting());
	sharedptr->ExecAsyncNoSetWaiting();
	return false;
}

bool CmnEventSink_TimerDoOne::IsDoing() const
{
	if ( !timer_ )
		return false;
	return !cmn_event_.expired();
}

CmnEventSink_TimerDoOne::enOnDone CmnEventSink_TimerDoOne::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	assert(!cmn_event_.expired());
	assert(cmn_event_.lock() == cmn_event);
	cmn_event_.reset();
	return kOD_None;
}

ICmnEventSink::enOnDone CmnWaitableEventSink::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(!cmn_event->IsWaiting());
	//cmn_event->SetWating(true);
	return kOD_Succeed;
}

CmnWaitableEvent::CmnWaitableEvent( const EventTouchT& func_touched ) : func_touched_(func_touched)
{
	sinks_.push_back(&waitable_sink_);
}

CmnWaitableEvent::CmnWaitableEvent( const CmnWaitableEvent& rhs ) : CmnEventBase(rhs), func_touched_(rhs.func_touched_)
{
	sinks_.push_back(&waitable_sink_);
}

CmnWaitableEvent::CmnWaitableEvent()
{
	sinks_.push_back(&waitable_sink_);
}

CmnWaitableEvent::CmnEventPtrT CmnWaitableEvent::Clone() const
{
	return CmnEventPtrT(new CmnWaitableEvent(*this));
}

void CmnWaitableEvent::AddSink( ICmnEventSink* sink )
{
	assert(sink);
	assert(!ExistTheSink(sink));
	sinks_.pop_back();
	sinks_.push_back(sink);
	sinks_.push_back(&waitable_sink_);
}

bool CmnWaitableEvent::Done()
{
	if (func_touched_)
		(*func_touched_)(*this);
	return true;
}

CmnCircleEvent::CmnCircleEvent( const EventTouchT& func_touched ) : func_touched_(func_touched)
{

}

CmnCircleEvent::CmnCircleEvent( const CmnCircleEvent& rhs ) : CmnEventBase(rhs), func_touched_(rhs.func_touched_)
{

}

CmnCircleEvent::CmnEventPtrT CmnCircleEvent::Clone() const
{
	return CmnEventPtrT(new CmnCircleEvent(*this));
}

bool CmnCircleEvent::Done()
{
	return func_touched_(*this);
}

CmnWaitTouchSink::CmnWaitTouchSink( const ICmnEvent::CmnEventPtrT& cmn_event, bool& is_touched ) : is_touched_(is_touched), cmn_event_(cmn_event)
{
	assert(0 == is_touched);
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	cmn_event->AddSink(this);
}

CmnWaitTouchSink::~CmnWaitTouchSink()
{
	if (cmn_event_)
	{
		assert(!cmn_event_->IsDiscard());
		cmn_event_->RemoveSink(this);
	}
	else
		assert(is_touched_);
}

void CmnWaitTouchSink::OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(cmn_event->IsDiscard());
	assert(cmn_event == cmn_event_);
	cmn_event_->RemoveSink(this);
	is_touched_ = true;
	cmn_event_.reset();
}

CmnWaitTouchSink::enOnDone CmnWaitTouchSink::OnDone( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	assert(cmn_event == cmn_event_);
	is_touched_ = true;
	return kOD_None;
}

CmnInstantEvent::CmnInstantEvent( const EventTouchT& func_touched ) : func_touched_(func_touched)
{
	sinks_.push_back(&async_exec_sink_);
}

CmnInstantEvent::CmnInstantEvent( const CmnInstantEvent& rhs ) : CmnEventBase(rhs), func_touched_(rhs.func_touched_)
{
	sinks_.push_back(&async_exec_sink_);
}

CmnInstantEvent::CmnEventPtrT CmnInstantEvent::Clone() const
{
	return CmnEventPtrT(new CmnInstantEvent(*this));
}

bool CmnInstantEvent::Done()
{
	return func_touched_(*this);
}

bool CmnEventAsyncExecSink::Help( ICmnEvent::CmnEventPtrT& cmn_event )
{
	assert(cmn_event);
	assert(!cmn_event->IsDiscard());
	cmn_event->ExecAsyc();
	return true;
}
