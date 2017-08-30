#pragma once
/*
@author zhp
@date 2014/9/10 20:19
@purpose produce consumer,command mode
*/
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <Net/IoTimer.h>
#include <set>
#include <luabind/object.hpp>
#include <queue>
#include <list>
#include "../DependSelfLib.h"

//源事件、目标事件
//原生事件(自带的事件)、自定义事件
class ICmnEventSink;
class ICmnEvent : public boost::enable_shared_from_this<ICmnEvent>{
public:
	typedef boost::shared_ptr<ICmnEvent> CmnEventPtrT;
	typedef boost::weak_ptr<ICmnEvent> WeakCmnEventPtrT;
	typedef std::list<ICmnEventSink*> SinksT;
	enum enOwnership{
		kCustom,	//自定义事件
		kProto,		//原生事件、自带的事件
	};

public:
	ICmnEvent();
	ICmnEvent( const ICmnEvent& rhs );
	virtual ~ICmnEvent();
	virtual bool IsDiscard() const = 0;
	virtual void Discard() = 0;
	//表示等待处理或执行
	virtual bool IsWaiting() const = 0;
	virtual bool IsDoing() const = 0;
	virtual void SetWating( bool is_wait ) = 0;
	virtual void Exec() = 0;
	void ExecAsyc();
	void ExecAsyncNoSetWaiting();
	virtual CmnEventPtrT Clone() const = 0;
	const std::string& GetEventName() const;
	void SetEventName( const std::string& event_name );
	virtual void AddSink( ICmnEventSink* sink );
	void RemoveSink( ICmnEventSink* sink );
	bool Named() const;
	void SetNamed();
	void SetUnNamed();
	enOwnership GetOwnership() const;
	bool OwnershipLvlUp();
	//最新的左端事件
	ICmnEvent* GetLastLeftEvent() const;
	void SetLastLeftEvent( ICmnEvent* left_event );

protected:
	void DiscardHelper();
	bool ExistTheSink( ICmnEventSink* sink ) const;
	
protected:
	SinksT				sinks_;
	std::string			event_name_;
	enOwnership			ownership_;			//所属类型
	ICmnEvent*			last_left_event_;
	bool				named_;;
};

class ICmnEventSink{
public:
	enum enOnDone{
		kOD_None,
		kOD_Continue,
		kOD_Succeed,
	};

public:
	virtual ~ICmnEventSink();
	virtual void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event ) = 0;
	//没完成事件，寻求帮助
	virtual bool Help( ICmnEvent::CmnEventPtrT& cmn_event ) = 0;
	virtual enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event ) = 0;
};

class CmnEventBaseSink : public ICmnEventSink{
public:
	void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event );
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
	enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );
};

//转发器，装饰者
class CmnEventDecorator : public ICmnEvent, public CmnEventBaseSink{
public:
	CmnEventDecorator( const CmnEventPtrT& cmn_event );
	CmnEventDecorator( const CmnEventDecorator& rhs );
	~CmnEventDecorator();
	bool IsDiscard() const;
	void Discard();
	bool IsWaiting() const;
	bool IsDoing() const;
	void SetWating( bool is_wait );
	void Exec();
	void OnDiscard( CmnEventPtrT& cmn_event );
	CmnEventPtrT Clone() const;

protected:
	CmnEventPtrT			handler_;
};

class CmnEventBase : public ICmnEvent{
protected:
	CmnEventBase();
	CmnEventBase( const CmnEventBase& rhs );
	~CmnEventBase();

public:
	bool IsDiscard() const;
	void Discard();
	bool IsWaiting() const;
	bool IsDoing() const;
	void SetWating( bool is_wait );
	void Exec();

protected:
	virtual bool Done() = 0;

protected:
	bool		is_discard_;
	bool		is_waiting_;
	bool		is_doing_;
};

//普通事件
class CmnGeneralEvent : public CmnEventBase{
public:
	typedef std::function<void(ICmnEvent&)> EventTouchT;

public:
	CmnGeneralEvent( const EventTouchT& func_touched );
	CmnGeneralEvent( const CmnGeneralEvent& rhs );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

protected:
	EventTouchT			func_touched_;
};

class CmnWaitableEventSink : public CmnEventBaseSink{
public:
	enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );
};

//可等待事件
class CmnWaitableEvent : public CmnEventBase{
public:
	typedef std::function<void(ICmnEvent&)> EventTouchT;

public:
	CmnWaitableEvent( const EventTouchT& func_touched );
	CmnWaitableEvent( const CmnWaitableEvent& rhs );
	CmnWaitableEvent();
	void AddSink( ICmnEventSink* sink );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

private:
	CmnWaitableEventSink			waitable_sink_;
	boost::optional<EventTouchT>	func_touched_;
};

class CmnEventSink_RunOne : public CmnEventBaseSink{
public:
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
};

class CmnEventSink_TimerDo : public CmnEventBaseSink, public boost::enable_shared_from_this<CmnEventSink_TimerDo>{
public:
	typedef boost::shared_ptr<CmnEventSink_TimerDo> SharedPtrT;
	typedef boost::weak_ptr<CmnEventSink_TimerDo> WeakPtrT;

public:
	CmnEventSink_TimerDo( std::size_t milli_seconds );
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
	std::size_t GetMilliSeconds() const;
	virtual bool IsDoing() const = 0;

protected:
	virtual bool DoOne() = 0;

private:
	class SinkTimerDoImpl_{
	public:
		SinkTimerDoImpl_( const SharedPtrT& shared_this );
		void Do( const IoTimerPtr& timer ) const;

	private:
		WeakPtrT		weak_this_;
	};

protected:
	IoTimerPtr		timer_;
	std::size_t		milli_seconds_;
};

class CmnEventSink_TimerDoOne : public CmnEventSink_TimerDo{
public:
	CmnEventSink_TimerDoOne( std::size_t milli_seconds );
	void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event );
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
	bool IsDoing() const;
	enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );

protected:
	bool DoOne();

private:
	ICmnEvent::WeakCmnEventPtrT			cmn_event_;
};

class CmnEventSink_TimerDoMulti : public CmnEventSink_TimerDo{
public:
	typedef std::queue<ICmnEvent::WeakCmnEventPtrT> EventHandleT;

public:
	CmnEventSink_TimerDoMulti( std::size_t milli_seconds );
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
	bool IsDoing() const;

protected:
	bool DoOne();

private:
	EventHandleT	unhandled_events_;
};

//计时器事件
class CmnTimerEvent : public CmnEventBase{
public:
	typedef std::function<bool(ICmnEvent&)> EventTouchT;

public:
	CmnTimerEvent( const EventTouchT& func_touched, std::size_t milli_seconds );
	CmnTimerEvent( const CmnTimerEvent& rhs );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

private:
	CmnEventSink_TimerDo::SharedPtrT timer_do_;
	EventTouchT						func_touched_;
};

//事件连接器
class CmnEventLinker : public CmnEventBaseSink{
public:
	typedef boost::shared_ptr<CmnEventLinker> EventLinkerPtrT;

protected:
	CmnEventLinker( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right );
	CmnEventLinker( const CmnEventLinker& rhs );

public:
	void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event );
	virtual EventLinkerPtrT CloneSink() const = 0;
	void Disconnect();
	void SetOwner( const ICmnEvent::CmnEventPtrT& event_owner );

protected:
	ICmnEvent::WeakCmnEventPtrT event_left_;
	ICmnEvent::WeakCmnEventPtrT	event_right_;
	ICmnEvent::WeakCmnEventPtrT	event_owner_;
};

//连接器事件
class CmnLinkerEvent : public CmnEventDecorator{
public:
	typedef CmnEventLinker::EventLinkerPtrT EventLinkerPtrT;

public:
	CmnLinkerEvent( const CmnEventPtrT& cmn_event, const EventLinkerPtrT& event_linker );
	CmnLinkerEvent( const CmnLinkerEvent& rhs );
	void Discard();
	bool IsDiscard() const;
	void OnDiscard( CmnEventPtrT& cmn_event );
	CmnEventPtrT Clone() const;	

protected:
	EventLinkerPtrT			linker_;
};

//单向连接
class CmnEventLinkTo : public CmnEventLinker{
public:
	CmnEventLinkTo( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right );
	CmnEventLinkTo( const CmnEventLinkTo& rhs );
	ICmnEventSink::enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );
	EventLinkerPtrT CloneSink() const;
};

//双向连接
class CmnEventBiLinker : public CmnEventLinker{
public:
	CmnEventBiLinker( const ICmnEvent::CmnEventPtrT& event_left, const ICmnEvent::CmnEventPtrT& event_right );
	CmnEventBiLinker( const CmnEventBiLinker& rhs );
	ICmnEventSink::enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );
	EventLinkerPtrT CloneSink() const;
};

//条件事件
class CmnCondtionEvent : public CmnGeneralEvent{
public:
	typedef std::function<bool(ICmnEvent&)> EventCondtionT;

public:
	CmnCondtionEvent( const EventCondtionT& event_cond, const EventTouchT& func_touched );
	CmnCondtionEvent( const CmnCondtionEvent& rhs );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

protected:
	EventCondtionT			event_cond_;
};

//循环事件
class CmnCircleEvent : public CmnEventBase{
public:
	typedef std::function<bool(ICmnEvent&)> EventTouchT;

public:
	CmnCircleEvent( const EventTouchT& func_touched );
	CmnCircleEvent( const CmnCircleEvent& rhs );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

protected:
	EventTouchT			func_touched_;
};

//等待触发
class CmnWaitTouchSink : public CmnEventBaseSink{
public:
	CmnWaitTouchSink( const ICmnEvent::CmnEventPtrT& cmn_event, bool& is_touched );
	~CmnWaitTouchSink();
	void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event );
	enOnDone OnDone( ICmnEvent::CmnEventPtrT& cmn_event );

private:
	ICmnEvent::CmnEventPtrT		cmn_event_;
	bool&						is_touched_;
};

class CmnEventAsyncExecSink : public CmnEventBaseSink{
public:
	bool Help( ICmnEvent::CmnEventPtrT& cmn_event );
};

//即时事件
class CmnInstantEvent : public CmnEventBase{
public:
	typedef std::function<bool(ICmnEvent&)> EventTouchT;

public:
	CmnInstantEvent( const EventTouchT& func_touched );
	CmnInstantEvent( const CmnInstantEvent& rhs );
	CmnEventPtrT Clone() const;

protected:
	bool Done();

private:
	EventTouchT				func_touched_;
	CmnEventAsyncExecSink	async_exec_sink_;
};

//事件工厂、管理者、派发器
class CmnEventsMgr : public CmnEventBaseSink{
public:
	typedef ICmnEvent::CmnEventPtrT CmnEventPtrT;
	typedef std::set<CmnEventPtrT> CmnEventsT;
	typedef std::pair<CmnEventPtrT, CmnEventPtrT> CmnPtrPairT;
	typedef std::map<CmnPtrPairT, CmnEventPtrT> TwoPtr2PtrMapT;

public:
	CmnEventsMgr( lua_State* lua_state );
	virtual ~CmnEventsMgr();
	CmnEventPtrT FindByName( const std::string& event_name ) const;
	lua_State* GetLuaState() const;
	
public:
	//处理事件
	bool HandleOneEvent() const;
	//激活事件
	bool ActiveEvent( const std::string& event_name );
	//中止事件
	bool TerminateEvent( const std::string& event_name );
	//中止所有事件
	void TerminateAllEvents();
	//中止所有自定义事件
	void TerminateCustomEvents();
	//中止所有未激活的自定义事件
	void TerminateUnActiveCustomEvents();
	//存在事件
	bool ExistTheEvent( const std::string& event_name ) const;
	//已激活事件
	bool ActivedTheEvent( const std::string& event_name ) const;
	//绑定事件
	bool BindEvent( const std::string& event_left, const std::string& event_right, const std::string& event_name );
	//互相绑定事件，二元绑定
	bool BiBindEvent( const std::string& event_left, const std::string& event_right, const std::string& event_name );
	//获取当前源事件
	const std::string& GetCurSourceEvent( const std::string& rhs_event ) const;
	//解绑事件
	bool RemoveBindEvent( const std::string& event_left, const std::string& event_right );
	bool RemoveBindEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right );
	//解除所有绑定关系
	void RemoveBindRelations();
	void RemoveBindRelations( const std::string& event_name );
	void RemoveBindRelations( const CmnEventPtrT& the_cmn_event );
	//解除所有自定义的绑定关系
	void RemoveCustomBindRelations();
	void RemoveCustomBindRelations( const std::string& event_name );
	void RemoveCustomBindRelations( const CmnEventPtrT& the_cmn_event );
	//解除源事件所有绑定关系
	void RemoveSourceBindRelations( const std::string& event_left );
	void RemoveSourceBindRelations( const CmnEventPtrT& event_left );
	//解除源事件所有自定义的绑定关系
	void RemoveSourceCustomBindRelations( const std::string& event_left );
	void RemoveSourceCustomBindRelations( const CmnEventPtrT& event_left );
	//等待触发事件
	bool WaitTouchEvent( const std::string& event_name );
	bool WaitTouchEvent( const std::string& event_name, std::size_t timeout );
	bool WaitTouchEvent( const CmnEventPtrT& cmn_event );
	bool WaitTouchEvent( const CmnEventPtrT& cmn_event, std::size_t timeout );
	//遍历所有命名事件
	void TraverseAllNamedEvent( const luabind::object& func ) const;
	//属于自带的事件
	bool BelongProtoEvent( const std::string& event_name ) const;
	//属于自定义事件
	bool BelongCustomEvent( const std::string& event_name ) const;
	//升级成自带的事件
	bool LvlToProtoEvent( const std::string& event_name );
	//加载自带的事件
	virtual void LoadProtoEvents() = 0;
	//给事件绑定数据
	bool BindEventData( const std::string& event_name, const luabind::object& event_data );
	bool BindEventData( const CmnEventPtrT& cmn_event, const luabind::object& event_data );
	//获取事件绑定的数据
	luabind::object GetEventBindData( const std::string& event_name ) const;
	luabind::object GetEventBindData( const CmnEventPtrT& cmn_event ) const;
	//给事件绑定私有数据，未开放给LUA
	bool BindEventPrivateData( const std::string& event_name, const luabind::object& event_private_data );
	bool BindEventPrivateData( const CmnEventPtrT& cmn_event, const luabind::object& event_private_data );
	//获取事件绑定的私有数据，未开放给LUA
	luabind::object GetEventPrivateData( const std::string& event_name ) const;
	luabind::object GetEventPrivateData( const CmnEventPtrT& cmn_event ) const;

	//register
public:
	//注册普通事件
	bool RegGeneralEvent( const luabind::object& func, const std::string& event_name );
	//注册即时事件
	bool RegInstantEvent( const luabind::object& func, const std::string& event_name );
	//注册循环事件
	bool RegCircleEvent( const luabind::object& func, const std::string& event_name );
	//注册可等待事件
	bool RegWaitableEvent( const std::string& event_name );
	bool RegWaitableEvent( const std::string& event_name, const luabind::object& func );
	//注册条件事件
	bool RegConditionEvent( const luabind::object& cond_func, const luabind::object& func, const std::string& event_name );
	//注册计时器事件
	bool RegTimerEvent( std::size_t milli_seconds, const luabind::object& func, const std::string& event_name );

	//create
public:
	//普通事件
	CmnEventPtrT CreateGeneralEvent( const CmnGeneralEvent::EventTouchT& func, const std::string& event_name );
	//即时事件
	CmnEventPtrT CreateInstantEvent( const CmnInstantEvent::EventTouchT& func, const std::string& event_name );
	//循环事件
	CmnEventPtrT CreateCircleEvent( const CmnCircleEvent::EventTouchT& func, const std::string& event_name );
	//可等待事件
	CmnEventPtrT CreateWaitableEvent( const std::string& event_name );
	CmnEventPtrT CreateWaitableEvent( const std::string& event_name, const CmnWaitableEvent::EventTouchT& func );
	//条件事件
	CmnEventPtrT CreateConditionEvent( const CmnCondtionEvent::EventCondtionT& cond_func, const CmnGeneralEvent::EventTouchT& func, const std::string& event_name );
	//计时器事件
	CmnEventPtrT CreateTimerEvent( std::size_t milli_seconds, const CmnTimerEvent::EventTouchT& func, const std::string& event_name );
	//单向连接事件
	CmnEventPtrT CreateLinkToEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, const std::string& event_name );
	//双向连接事件
	CmnEventPtrT CreateBiLinkerEvent( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, const std::string& event_name );

	//////////////////////////////////////////////////////////////////////////
public:
	static CmnEventsMgr* GetInstance();
	static bool HandleOneEvent_();
	static bool ActiveEvent_( const std::string& event_name );
	static bool TerminateEvent_( const std::string& event_name );
	static void TerminateAllEvents_();
	static void TerminateCustomEvents_();
	static void TerminateUnActiveCustomEvents_();
	static bool ExistTheEvent_( const std::string& event_name );
	static bool ActivedTheEvent_( const std::string& event_name );
	static bool BindEvent_1_( const std::string& event_left, const std::string& event_right );
	static bool BindEvent_2_( const std::string& event_left, const std::string& event_right, const std::string& event_name );
	static bool BiBindEvent_1_( const std::string& event_left, const std::string& event_right );
	static bool BiBindEvent_2_( const std::string& event_left, const std::string& event_right, const std::string& event_name );
	static const std::string& GetCurSourceEvent_( const std::string& rhs_event );
	static bool RemoveBindEvent_( const std::string& event_left, const std::string& event_right );
	static void RemoveBindRelations_1_();
	static void RemoveBindRelations_2_( const std::string& event_name );
	static void RemoveCustomBindRelations_1_();
	static void RemoveCustomBindRelations_2_( const std::string& event_name );
	static void RemoveSourceBindRelations_( const std::string& event_left );
	static void RemoveSourceCustomBindRelations_( const std::string& event_left );
	static bool WaitTouchEvent_1_( const std::string& event_name );
	static bool WaitTouchEvent_2_( const std::string& event_name, std::size_t timeout );
	static void TraverseAllNamedEvent_( const luabind::object& func );
	static bool BelongProtoEvent_( const std::string& event_name );
	static bool BelongCustomEvent_( const std::string& event_name );
	static bool LvlToProtoEvent_( const std::string& event_name );
	static void LoadProtoEvents_();
	static bool BindEventData_( const std::string& event_name, const luabind::object& event_data );
	static luabind::object GetEventBindData_( const std::string& event_name );
	static bool RegGeneralEvent_1_( const luabind::object& func );
	static bool RegGeneralEvent_2_( const luabind::object& func, const std::string& event_name );
	static bool RegInstantEvent_1_( const luabind::object& func );
	static bool RegInstantEvent_2_( const luabind::object& func, const std::string& event_name );
	static bool RegCircleEvent_1_( const luabind::object& func );
	static bool RegCircleEvent_2_( const luabind::object& func, const std::string& event_name );
	static bool RegWaitableEvent_1_( const std::string& event_name );
	static bool RegWaitableEvent_2_( const std::string& event_name, const luabind::object& func );
	static bool RegConditionEvent_1_( const luabind::object& cond_func, const luabind::object& func );
	static bool RegConditionEvent_2_( const luabind::object& cond_func, const luabind::object& func, const std::string& event_name );
	static bool RegTimerEvent_1_( std::size_t milli_seconds, const luabind::object& func );
	static bool RegTimerEvent_2_( std::size_t milli_seconds, const luabind::object& func, const std::string& event_name );
	//////////////////////////////////////////////////////////////////////////
	static bool AsyncExecEvent( const CmnEventPtrT& cmn_event );
	static TwoPtr2PtrMapT::const_iterator FindTwoPtrMap( const TwoPtr2PtrMapT& two_ptr_map, const CmnPtrPairT& two_ptr_pair );
	//////////////////////////////////////////////////////////////////////////

public:
	void OnDiscard( ICmnEvent::CmnEventPtrT& cmn_event );

protected:
	void RegForLua();

private:
	ICmnEventSink* GetWaitMonitorSink();
	CmnEventPtrT CreateLinkerEventImpl( const CmnEventPtrT& event_left, const CmnEventPtrT& event_right, bool is_link_to, const std::string& event_name );
	luabind::object GetEventLuaTbl( const std::string& event_name ) const;
	void ResetPrivateTbl();
#ifndef _DEBUG
	static
#endif
	void BindEventName( CmnEventPtrT& cmn_event, const std::string& event_name );

private:
	static void EventHandler_tbl_1_( ICmnEvent& cmn_event );
	static bool EventHandler_tbl_2_( ICmnEvent& cmn_event );

protected:
	lua_State*							lua_state_;

private:
	CmnEventSink_TimerDo::SharedPtrT	wait_monitor_;
	CmnEventsT							cmn_events_;
	CmnEventsT							timer_events_;
	TwoPtr2PtrMapT						linker_events_;
	bool								discard_all_;
	const char*const					global_tbl_name_;

private:
	static CmnEventsMgr*				single_this_;
};

class CmnEventsFatory{
public:
	virtual CmnEventsMgr* CreateEventsMgr() = 0;
};