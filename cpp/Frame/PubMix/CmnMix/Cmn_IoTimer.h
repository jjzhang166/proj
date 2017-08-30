#pragma once
/*
@author zhp
@date 2017/5/15 18:46
@purpose for timer
*/
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>

class IoTimer{
public:
	struct lock_guard{
		lock_guard();
		~lock_guard();

	private:
		bool		old_locked_;
	};

public:
	virtual ~IoTimer();
	//继续计时
	virtual void Continue() = 0;
	//取消计时，可以取消计时后再重新计时。
	virtual void Cancel() = 0;
	//设置计时间隔
	virtual void SetDuration(pt_dword milli_seconds) = 0;
	//是否正在计时
	virtual bool IsClocking() const = 0;
};
typedef boost::shared_ptr<IoTimer> IoTimerPtr;

//计时器回调函数
//参数为IoTimerPtr
//set timer, no call times
void SetIoTimer(pt_dword milli_seconds, const std::function<void(IoTimer*)>& call_back);
void SetIoTimer1(pt_dword milli_seconds, const std::function<void(IoTimer&)>& call_back);
//参数为回调的次数
//set timer, with call times,从0开始计数
void SetIoTimer(pt_dword milli_seconds, pt_dword times, const std::function<void(pt_dword)>& call_back);
//创建计时器
IoTimerPtr CreateIoTimer(pt_dword milli_seconds, bool start_timer, const std::function<void(IoTimerPtr&)>& call_back);
//可锁定计时器
IoTimerPtr LockableIoTimer(pt_dword milli_seconds, bool start_timer, const std::function<void(IoTimerPtr&)>& call_back);
