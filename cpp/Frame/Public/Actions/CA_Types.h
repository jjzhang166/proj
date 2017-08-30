#pragma once
/*
@author zhp
@date 2017/5/3 19:13
@purpose for action types
*/
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <CmnMix/Cmn_TemplateUtility.h>

//////////////////////////////////////////////////////////////////////////
class CA_Define{
public:
	//枚举顺序不要随意改变。
	enum enTerminateFlag : int{
		kTF_Normal = 0,				//正常中止自己
		kTF_ToDecoration = 1 << 1,	//中止到Decoration
		kTF_ToParent = 1 << 2,		//中止到父亲
		kTF_ToConsumer = 1 << 3,	//中止到消费者
		kTF_ToTester = 1 << 4,		//中止到行为检测者
		kTF_ReRun = 1 << 5,			//重新运行
	};
	enum enRunRes{
		kRR_Terminated,			//被中止
		kRR_Failed,
		kRR_Succeed,
	};
};
//////////////////////////////////////////////////////////////////////////
class CA_Define;
class CA_IAction;
typedef boost::shared_ptr<CA_IAction> CA_ActionPtr;
typedef boost::weak_ptr<CA_IAction> CA_ActionWeakPtr;
class CA_Action;
class CA_Actions;
class CA_EveryActions;
class CA_Consumer;
class CA_EnsureConsumer;
class CA_SmartConsumer;
class CA_Producer;
template<CA_Define::enRunRes OnPoorResult = CA_Define::kRR_Succeed>
class CA_ProducerT;
template<CA_Define::enRunRes OnPoorResult = CA_Define::kRR_Succeed>
class CA_FastProducer;
class CA_Decorator;
class CA_AsTimerAction;
class CA_Tester;
class CA_FastTester;
class CA_EnsureTester;
class CA_ProductTester;
class CA_Product;
class CA_FastProduct;
template<typename ProductImplT_ = CA_IAction, typename ProductBase = CA_FastProduct>
class CA_ProductT;
class CA_Not;
template<typename ActionBase, typename FnT>
class CA_Lambda;
class CA_Factory;
//////////////////////////////////////////////////////////////////////////
//Add Policy
struct CA_tagAdd{};
struct CA_tagAddEnsure{};
//////////////////////////////////////////////////////////////////////////
template<typename ActionT>
using CA_T = SmartT<CA_ActionPtr, ActionT>;

template<typename ActionT>
using CA_WeakT = SmartWeakT<CA_ActionWeakPtr, CA_ActionPtr, ActionT>;
//////////////////////////////////////////////////////////////////////////
struct CA_RunRes{
	CA_RunRes();
	CA_RunRes(CA_Define::enRunRes run_res);
	CA_RunRes(const CA_ActionPtr& error_action);
	operator CA_Define::enRunRes() const{ return run_res_; }
	operator CA_ActionPtr () const{ return error_action_; }
	operator bool() const{ return run_res_ >= CA_Define::kRR_Succeed; }
	friend bool operator == (const CA_RunRes& lhs, const CA_ActionPtr& rhs){
		return lhs.error_action_ == rhs;
	}
	friend bool operator == (const CA_RunRes& lhs, CA_Define::enRunRes rhs){
		return lhs.run_res_ == rhs;
	}
	friend bool operator == (const CA_ActionPtr& lhs, const CA_RunRes& rhs){
		return lhs == rhs.error_action_;
	}
	friend bool operator == (CA_Define::enRunRes lhs, const CA_RunRes& rhs){
		return lhs == rhs.run_res_;
	}
	template<typename ActionT>
	friend bool operator == (const CA_RunRes& lhs, const CA_T<ActionT>& rhs){
		return lhs.error_action_ == rhs.first;
	}
	template<typename ActionT>
	friend bool operator == (const CA_T<ActionT>& lhs, const CA_RunRes& rhs){
		return lhs.first == rhs.error_action_;
	}
	friend bool operator != (const CA_RunRes& lhs, const CA_ActionPtr& rhs){
		return lhs.error_action_ != rhs;
	}
	friend bool operator != (const CA_RunRes& lhs, CA_Define::enRunRes rhs){
		return lhs.run_res_ != rhs;
	}
	friend bool operator != (const CA_ActionPtr& lhs, const CA_RunRes& rhs){
		return lhs != rhs.error_action_;
	}
	friend bool operator != (CA_Define::enRunRes lhs, const CA_RunRes& rhs){
		return lhs != rhs.run_res_;
	}
	template<typename ActionT>
	friend bool operator != (const CA_RunRes& lhs, const CA_T<ActionT>& rhs){
		return lhs.error_action_ != rhs.first;
	}
	template<typename ActionT>
	friend bool operator != (const CA_T<ActionT>& lhs, const CA_RunRes& rhs){
		return lhs.first != rhs.error_action_;
	}
	bool operator < (CA_Define::enRunRes rhs) const{
		return run_res_ < rhs;
	}
	bool operator <= (CA_Define::enRunRes rhs) const{
		return run_res_ <= rhs;
	}
	bool operator >(CA_Define::enRunRes rhs) const{
		return run_res_ > rhs;
	}
	bool operator >= (CA_Define::enRunRes rhs) const{
		return run_res_ >= rhs;
	}

	CA_Define::enRunRes		run_res_;
	CA_ActionPtr			error_action_;
};
//////////////////////////////////////////////////////////////////////////
namespace Details{
	template<bool IsDerivedTester, typename ActionBase, typename FnT>
	class CA_LambdaImpl : public ActionBase{
	protected:
		typedef CA_LambdaImpl MyLambdaType;

	public:
		CA_LambdaImpl(const FnT& fn) : fn_(fn){}

	protected:
		CA_RunRes OnRun() override final{
			return fn_();
		}

	private:
		FnT		fn_;
	};

	template<typename ActionBase, typename FnT>
	class CA_LambdaImpl<true, ActionBase, FnT> : public ActionBase{
	protected:
		typedef CA_LambdaImpl MyLambdaType;

	public:
		CA_LambdaImpl(const FnT& fn) : fn_(fn){}

	protected:
		bool Test() override final{
			return fn_();
		}

	private:
		FnT		fn_;
	};
}
//////////////////////////////////////////////////////////////////////////