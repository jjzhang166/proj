#include "stdafx.h"
#include <CmnMix/Cmn_IoTimer.h>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/placeholders.hpp>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/scope_exit.hpp>
#include <boost/thread/tss.hpp>
#include <CmnMix/Cmn_UsefulFunc.h>

//////////////////////////////////////////////////////////////////////////
static boost::thread_specific_ptr<bool>	g_io_timer_locked_;
//////////////////////////////////////////////////////////////////////////
class IoTimerImpl : public IoTimer, public boost::enable_shared_from_this<IoTimerImpl>{
	struct TimerImpl{
		TimerImpl(pt_dword milli_seconds) : timer_(GetIoService().GetImpl(),
			boost::posix_time::milliseconds(milli_seconds)){
			running_ = false;
		}

		boost::asio::deadline_timer		timer_;
		bool							running_;
	};
	typedef boost::shared_ptr<TimerImpl> TimerImplPtr;

protected:
	IoTimerImpl(pt_dword milli_seconds){
		milli_seconds_ = milli_seconds;
		assert(milli_seconds_ > 0);
	}
	~IoTimerImpl(){
		Cancel();
	}

public:
	void Cancel() override final{
		if (!timer_impl_ || !timer_impl_->running_)
			return;
		boost::system::error_code ec;
		timer_impl_->timer_.cancel(ec);
		//timer_impl_->timer_.cancel_one();
		timer_impl_.reset();
	}
	void SetDuration(pt_dword milli_seconds) override final{
		assert(milli_seconds);
		milli_seconds_ = milli_seconds;
	}
	pt_dword GetDuration() const{
		return milli_seconds_;
	}
	void Continue() override final{
		if (timer_impl_)
		{
			if (timer_impl_->running_)
				return;
			timer_impl_->timer_.expires_from_now(boost::posix_time::milliseconds(milli_seconds_));
		}
		else
		{
			timer_impl_.reset(new TimerImpl(milli_seconds_));
			if (!timer_impl_)
			{
				assert(false);
				return;
			}
		}
		timer_impl_->running_ = true;
		auto shared_this = shared_from_this();
		assert(shared_this);
		auto timer_impl = timer_impl_;
		timer_impl_->timer_.async_wait([shared_this, timer_impl, this](const boost::system::error_code& error){
			if (timer_impl_ != timer_impl)
				return;
			timer_impl->running_ = false;
			if (error)
			{
				LOG_O(Log_debug) << "¼ÆÊ±³ö´í,error msg-->" << error.message();
				return;
			}
			TimerCallBack();
		});
	}
	bool IsClocking() const override final{
		return timer_impl_ && timer_impl_->running_;
	}

protected:
	virtual void TimerCallBack() = 0;

private:
	pt_dword		milli_seconds_;
	TimerImplPtr	timer_impl_;
};

class IoTimer_SetTimer : public IoTimerImpl{
public:
	IoTimer_SetTimer(const std::function<void(IoTimer*)>& call_back, pt_dword milli_seconds)
		: IoTimerImpl(milli_seconds), call_back_(call_back){}

private:
	void TimerCallBack() override{
		call_back_(this);
	}

private:
	std::function<void(IoTimer*)>	call_back_;
};

class IoTimer_SetTimer1 : public IoTimerImpl{
public:
	IoTimer_SetTimer1(const std::function<void(IoTimer&)>& call_back, pt_dword milli_seconds)
		: IoTimerImpl(milli_seconds), call_back_(call_back){}

private:
	void TimerCallBack() override{
		call_back_(*this);
	}

private:
	std::function<void(IoTimer&)>	call_back_;
};

class IoTimer_SetTimerTimes : public IoTimerImpl{
public:
	IoTimer_SetTimerTimes(const std::function<void(pt_dword)>& call_back, pt_dword milli_seconds,
		pt_dword max_times)
		: IoTimerImpl(milli_seconds), call_back_(call_back), max_times_(max_times){
		assert(max_times_ > 0);
		cur_times_ = 0;
	}

private:
	void TimerCallBack() override{
		call_back_(cur_times_++);
		if (max_times_ != cur_times_)
			Continue();
	}

private:
	std::function<void(pt_dword)>	call_back_;
	pt_dword						cur_times_;
	const pt_dword					max_times_;
};

IoTimer::~IoTimer()
{
}

class IoTimer_CreateIoTimer : public IoTimerImpl{
public:
	IoTimer_CreateIoTimer(pt_dword milli_seconds, const std::function<void(IoTimerPtr&)>& call_back)
		: IoTimerImpl(milli_seconds), call_back_(call_back){}

protected:
	void TimerCallBack() override{
		auto my_ptr = boost::static_pointer_cast<IoTimer>(shared_from_this());
		if (!my_ptr)
		{
			assert(false);
			return;
		}
		call_back_(my_ptr);
	}

private:
	std::function<void(IoTimerPtr&)>	call_back_;
};

class IoTimer_LockableIoTimer : public IoTimerImpl{
public:
	IoTimer_LockableIoTimer(pt_dword milli_seconds, const std::function<void(IoTimerPtr&)>& call_back)
		: IoTimerImpl(milli_seconds), call_back_(call_back){}

protected:
	void TimerCallBack() override{
		auto is_locked_ptr = g_io_timer_locked_.get();
		if (is_locked_ptr && *is_locked_ptr)
		{
			auto old_duration = GetDuration();
			SetDuration(100);
			Continue();
			SetDuration(old_duration);
			return;
		}
		auto my_ptr = boost::static_pointer_cast<IoTimer>(shared_from_this());
		if (!my_ptr)
		{
			assert(false);
			return;
		}
		call_back_(my_ptr);
	}

private:
	std::function<void(IoTimerPtr&)>	call_back_;
};
//////////////////////////////////////////////////////////////////////////

void SetIoTimer(pt_dword milli_seconds, const std::function<void(IoTimer*)>& call_back)
{
	if (milli_seconds < 1)
	{
		assert(false);
		return;
	}
	IoTimerPtr timer_ptr(new IoTimer_SetTimer(call_back, milli_seconds));
	if (!timer_ptr)
	{
		assert(false);
		return;
	}
	timer_ptr->Continue();
}

void SetIoTimer1(pt_dword milli_seconds, const std::function<void(IoTimer&)>& call_back)
{
	if (milli_seconds < 1)
	{
		assert(false);
		return;
	}
	IoTimerPtr timer_ptr(new IoTimer_SetTimer1(call_back, milli_seconds));
	if (!timer_ptr)
	{
		assert(false);
		return;
	}
	timer_ptr->Continue();
}

void SetIoTimer(pt_dword milli_seconds, pt_dword times, const std::function<void(pt_dword)>& call_back)
{
	if (milli_seconds < 1)
	{
		assert(false);
		return;
	}
	if (times < 1)
	{
		assert(false);
		return;
	}
	IoTimerPtr timer_ptr(new IoTimer_SetTimerTimes(call_back, milli_seconds, times));
	if (!timer_ptr)
	{
		assert(false);
		return;
	}
	timer_ptr->Continue();
}

IoTimerPtr CreateIoTimer(pt_dword milli_seconds, bool start_timer, const std::function<void(IoTimerPtr&)>& call_back)
{
	if (milli_seconds < 1)
	{
		assert(false);
		return nullptr;
	}
	IoTimerPtr timer_ptr(new IoTimer_CreateIoTimer(milli_seconds, call_back));
	if (!timer_ptr)
	{
		assert(false);
		return nullptr;
	}
	if (start_timer)
		timer_ptr->Continue();
	return timer_ptr;
}

IoTimerPtr LockableIoTimer(pt_dword milli_seconds, bool start_timer, const std::function<void(IoTimerPtr&)>& call_back)
{
	if (milli_seconds < 1)
	{
		assert(false);
		return nullptr;
	}
	IoTimerPtr timer_ptr(new IoTimer_LockableIoTimer(milli_seconds, call_back));
	if (!timer_ptr)
	{
		assert(false);
		return nullptr;
	}
	if (start_timer)
		timer_ptr->Continue();
	return timer_ptr;
}

IoTimer::lock_guard::lock_guard()
{
	auto ptr_locked = g_io_timer_locked_.get();
	if (ptr_locked)
	{
		old_locked_ = *ptr_locked;
		*ptr_locked = true;
	}
	else
	{
		old_locked_ = false;
		g_io_timer_locked_.reset(new bool(true));
	}
}

IoTimer::lock_guard::~lock_guard()
{
	auto ptr_locked = g_io_timer_locked_.get();
	assert(ptr_locked);
	*ptr_locked = old_locked_;
}
