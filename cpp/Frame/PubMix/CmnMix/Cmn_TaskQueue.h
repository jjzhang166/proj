#pragma once
/*
@author zhp
@date 2016/2/1 5:11
@purpose 生产者与消费者模式
*/
#include <CommonX/CmnX_TaskQueue.h>
#include <CommonX/CmnX_StaticConts.h>
#include <CommonX/CmnX_UsefulClass.h>

#pragma pack(push)
#pragma pack(1)

template<unsigned short ContMaxSize_>
class CmnTaskStaticQueue : public Cmnx_TaskQueueImpl<FunctionVoid, Cmnx_TaskQueue_TasksHolder<CmnStaticQueue<FunctionVoid, ContMaxSize_> >,
	Cmnx_TaskQueue_AlgoMore_BuffRecycle<CmnStaticQueue<FunctionVoid, ContMaxSize_> >, Cmnx_TaskQueueBase_SelfProcess>{
};

template<unsigned short FuncParamMaxSize_, unsigned short ContMaxSize_>
class CmnTaskStaticQueue_Atomic_Helper{
public:
	static const unsigned short	FuncParamMaxSize = FuncParamMaxSize_;

public:
	typedef void(*TaskFuncT_)(void* func_param, unsigned short param_size);
	struct stFuncInfo{
		TaskFuncT_		func_;
		unsigned short	func_param_size_;
		unsigned char	func_param_[FuncParamMaxSize];

	protected:
		stFuncInfo(){
			Cmnx_MemSetValue(this, sizeof(*this), 0);
		}

	public:
		stFuncInfo(const stFuncInfo& rhs){
			Cmnx_CpyMem(this, &rhs, sizeof(*this));
		}
		stFuncInfo& operator = (const stFuncInfo& rhs){
			Cmnx_CpyMem(this, &rhs, sizeof(*this));
		}
	};
	typedef CmnStaticQueue<stFuncInfo, ContMaxSize_> TasksT_;
};

template<unsigned short FuncParamMaxSize_, unsigned short ContMaxSize_, typename TaskHolderT_ = Cmnx_TaskQueue_TasksHolder<typename CmnTaskStaticQueue_Atomic_Helper<FuncParamMaxSize_, ContMaxSize_>::TasksT_>,
	typename TaskQueueBaseT_ = Cmnx_TaskQueueBase_SelfProcess,
	typename AlgoMoreT_ = Cmnx_TaskQueue_AlgoMore_BuffRecycle<typename CmnTaskStaticQueue_Atomic_Helper<FuncParamMaxSize_, ContMaxSize_>::TasksT_> >
class CmnTaskStaticQueue_Atomic : public CmnTaskStaticQueue_Atomic_Helper<FuncParamMaxSize_, ContMaxSize_>,
	public Cmnx_TaskQueueImplT<typename CmnTaskStaticQueue_Atomic_Helper<FuncParamMaxSize_, ContMaxSize_>::stFuncInfo, TaskHolderT_,
	AlgoMoreT_, TaskQueueBaseT_>{
private:
	struct stFuncInfoImpl : stFuncInfo{};
	static_assert(sizeof(stFuncInfoImpl) == sizeof(stFuncInfo), "");

public:
	template<typename FuncParamT_>
	static stFuncInfo MakeFuncInfo(TaskFuncT_ func, const FuncParamT_& param){
		struct stFuncParamHelper{
			FuncParamT_		param_;

			stFuncParamHelper(const FuncParamT_& the_param){
				Cmnx_CpyMem(this, &the_param, sizeof(the_param));
			}
		};
		static_assert(sizeof(stFuncParamHelper) == sizeof(param), "");
		static_assert(sizeof(stFuncParamHelper) <= FuncParamMaxSize, "");
		stFuncInfoImpl func_info;
		if (!func)
		{
			assert(false);
			return func_info;
		}
		func_info.func_ = func;
		func_info.func_param_size_ = sizeof(stFuncParamHelper);
		new ((stFuncParamHelper*)func_info.func_param_) stFuncParamHelper(param);
		return func_info;
	}

private:
	void DoExecTask(TaskFuncT& cur_task){
		cur_task.func_(cur_task.func_param_, cur_task.func_param_size_);
	}
};

class CmnTaskStaticQueue_Msg_HelperBase{
public:
	typedef unsigned short MsgIdT;
	typedef unsigned short size_type_;
	struct stFuncInfo{
		MsgIdT			msg_id_;
		size_type_		offset_begin_;
		size_type_		offset_end_;

	protected:
		stFuncInfo();

	public:
		CmnX_CodeInfo ToCodeInfo() const;
		stFuncInfo(const stFuncInfo& rhs);
		stFuncInfo& operator = (const stFuncInfo& rhs);
	};
};
template<unsigned short ContMaxSize_>
class CmnTaskStaticQueue_Msg_Helper : public CmnTaskStaticQueue_Msg_HelperBase{
public:
	typedef CmnStaticQueue<stFuncInfo, ContMaxSize_> TasksT_;
};

template<unsigned short ContMaxSize_, typename TaskHolderT_ = Cmnx_TaskQueue_TasksNil<typename CmnTaskStaticQueue_Msg_Helper<ContMaxSize_>::TasksT_>,
	typename TaskQueueBaseT_ = Cmnx_TaskQueueBase_MultiProcess,
	typename AlgoMoreT_ = Cmnx_TaskQueue_AlgoMore_Normal<typename CmnTaskStaticQueue_Msg_Helper<ContMaxSize_>::TasksT_> >
class CmnTaskStaticQueue_Msg : public CmnTaskStaticQueue_Msg_Helper<ContMaxSize_>,
	public Cmnx_TaskQueueImplT<typename CmnTaskStaticQueue_Msg_Helper<ContMaxSize_>::stFuncInfo, TaskHolderT_,
	AlgoMoreT_, TaskQueueBaseT_>{
private:
	struct stFuncInfoImpl : stFuncInfo{};
	static_assert(sizeof(stFuncInfoImpl) == sizeof(stFuncInfo), "");

public:
	static stFuncInfo MakeFuncInfo(MsgIdT msg_id, size_type offset_begin, size_type offset_end){
		stFuncInfoImpl res;
		res.msg_id_ = msg_id;
		res.offset_begin_ = offset_begin;
		res.offset_end_ = offset_end;
		return res;
	}
};

#pragma pack(pop)
