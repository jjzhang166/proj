#pragma once
/*
@author zhp
@date 2017/4/26 21:11
@purpose for actions
CA->Common Action.
*/
#include <boost/enable_shared_from_this.hpp>
#include <deque>
#include <type_traits>
#include <ProtocolX/P_BaseTypeX.h>
#include <Net/IoTimer.h>
#include <CmnMix/Cmn_PocoMore.h>
#include "CA_Types.h"
#include <luabind/LuaBase.hpp>
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
/*
行为可以执行，也可以被中止，中止是用的异常。
tester：行为检验者、测试者，多用于判断某个条件或某个行为。
ProductTester：产品检验者，如果产品满足检验了，那么便会产生该Product给Consumer。
Producer：生产者，里面的元素都是ProductTester，Producer依靠ProductTester来决定产品是否合格，并决定是何种Product。
Consumer：消费者，用来处理Product的。
Decoration：装饰品，装饰者用来装饰的对象。对于FastTester，Decoration便是tester满足条件后运行的行为。
Parent：父亲，若是Decoration为空，Parent便是tester满足条件后运行的行为。
******************
CA_IAction：表示一个行为，理论上可以是任何行为，任何函数实现的行为，都可以是一个Action。
CA_Action：行为的通用的默认实现，可以是单个行为，也可以是多个行为。
CA_Actions：行为容器，表示多个行为，每次run的时候，依次遍历各个子行为。
CA_Consumer：消费者模式，表示多个行为，按push顺序执行一次子行为，相当于队列。
CA_EnsureConsumer：确保消费者，AddEnsure接口可以确保该子行为能够执行正常执行完毕，无论是否有其他行为抛出异常。
CA_SmartConsumer：类似CA_EnsureConsumer，既可以确保行为，又可以动态添加tester、ProductTester、Producer，以便可以产生新的行为。
CA_Producer：生产者。
CA_FastProducer：快速生产者，如果有合格的Product，则会迅速转到Consumer，并让Consumer使用该Product。
CA_Product：产品，它可以创建一个ProductTester，以便判断自身是否是合格的或满足条件的；也可以创建一个Producer。
CA_Decorator：装饰者。装饰者与装饰品的关系是装饰者来装饰装饰品。
CA_AsTimerAction：包装一个Action，使其可以每隔段时间使可运行一次。
CA_Tester：检验者、测试者，通常用来测试一个条件。
CA_FastTester：快带检验者。
CA_Not：包装一个Action，让其由kRR_Succeed->kRR_Failed，或kRR_Failed->kRR_Succeed。
CA_Lambda：简单的Action。
*/
//////////////////////////////////////////////////////////////////////////
class CA_IAction : public CA_Define{
public:
	static void RegForLua(lua_State* l);
	virtual ~CA_IAction();
	//装饰品
	virtual CA_ActionPtr Decoration() const = 0;
	virtual CA_ActionPtr Parent() const = 0;
	virtual CA_ActionPtr Consumer() const = 0;
	//此Action的检验者、测试者。
	virtual CA_ActionPtr Tester() = 0;
	virtual void SetParent(const CA_ActionPtr& parent) = 0;
	virtual void SetConsumer(const CA_ActionPtr& consumer) = 0;
	virtual bool IsRunning() const = 0;
	virtual CA_RunRes Run() = 0;
	virtual void Terminate(enTerminateFlag terminate_flag) = 0;
	virtual void Terminate(enTerminateFlag terminate_flag, const CA_ActionPtr& dst_action) = 0;
	void Terminate(int terminate_flag){ return Terminate(enTerminateFlag(terminate_flag)); }
	void Terminate(int terminate_flag, const CA_ActionPtr& dst_action){ return Terminate(enTerminateFlag(terminate_flag), dst_action); }

public:
	virtual int Weight() const = 0;
	virtual void Add(const CA_ActionPtr& action) = 0;
	virtual void Erase(const CA_ActionPtr& action) = 0;
	virtual void AddEnsure(const CA_ActionPtr& action) = 0;
	virtual void EraseEnsure(const CA_ActionPtr& action) = 0;
	virtual void clear() = 0;
};
//////////////////////////////////////////////////////////////////////////
class CA_Action : public CA_IAction, public boost::enable_shared_from_this<CA_Action>{
public:
	CA_Action();
	CA_ActionPtr Decoration() const override;
	CA_ActionPtr Parent() const override final;
	CA_ActionPtr Consumer() const override;
	CA_ActionPtr Tester() override;
	void SetParent(const CA_ActionPtr& parent) override;
	void SetConsumer(const CA_ActionPtr& consumer) override;
	bool IsRunning() const override final;
	CA_RunRes Run() override final;
	void Terminate(enTerminateFlag terminate_flag) override final;
	void Terminate(enTerminateFlag terminate_flag, const CA_ActionPtr& dst_action) override final;
	using CA_IAction::Terminate;

public:
	int Weight() const override;
	void Add(const CA_ActionPtr& action) override;
	void Erase(const CA_ActionPtr& action) override;
	void AddEnsure(const CA_ActionPtr& action) override;
	void EraseEnsure(const CA_ActionPtr& action) override;
	void clear() override;

protected:
	virtual CA_RunRes OnRun() = 0;

private:
	CA_ActionWeakPtr	parent_;
	CA_ActionWeakPtr	consumer_;
	bool				is_running_;
};
//////////////////////////////////////////////////////////////////////////
class CA_Actions : public CA_Action{
public:
	typedef std::deque<CA_ActionPtr> ActionsType;

public:
	const ActionsType& GetActions() const;
	void Add(const CA_ActionPtr& action) override;
	void Erase(const CA_ActionPtr& action) override;
	void clear() override;

protected:
	CA_RunRes OnRun() override;
	void AddImpl(ActionsType& actions, const CA_ActionPtr& action);
	void EraseImpl(ActionsType& actions, const CA_ActionPtr& action);

protected:
	ActionsType		actions_;
};

class CA_EveryActions : public CA_Actions{
protected:
	CA_RunRes OnRun() override;
};

class CA_Consumer : public CA_Actions{
public:
	CA_ActionPtr Consumer() const override;
	void SetConsumer(const CA_ActionPtr& consumer) override;

protected:
	CA_RunRes OnRun() override;
	//成功消费
	virtual void OnConsumedProduct(const CA_ActionPtr& product);
};

//确保消费者
class CA_EnsureConsumer : public CA_Consumer{
public:
	CA_ActionPtr Tester() override;
	void AddEnsure(const CA_ActionPtr& action) override;
	void EraseEnsure(const CA_ActionPtr& action) override;
	void clear() override;

protected:
	CA_RunRes OnRun() override;
	void OnConsumedProduct(const CA_ActionPtr& product) override;

protected:
	CA_ActionPtr	tester_;

private:
	ActionsType		ensure_actions_;
};

class CA_SmartConsumer : public CA_Action{
	struct stEnsureTesterInfo{
		CA_ActionWeakPtr	action_;
		CA_ActionWeakPtr	action_tester_;
		CA_ActionWeakPtr	as_timer_action_;
	};
	typedef std::vector<stEnsureTesterInfo> EnsureTesterInfoCont;

public:
	CA_SmartConsumer();
	CA_ActionPtr Consumer() const override;
	void SetConsumer(const CA_ActionPtr& consumer) override;
	CA_ActionPtr Tester() override;
	void Add(const CA_ActionPtr& action) override;
	void Erase(const CA_ActionPtr& action) override;
	void clear() override;
	void AddEnsure(const CA_ActionPtr& action) override;
	//action_tester可以是普通的tester，也可以是ProductTester，还可以是Producer。
	void AddEnsure(const CA_ActionPtr& action, const CA_ActionPtr& action_tester);
	void EraseEnsure(const CA_ActionPtr& action) override;
	//全局检测
	void AddTester(const CA_ActionPtr& tester);
	void EraseTester(const CA_ActionPtr& tester);

protected:
	CA_RunRes OnRun() override;

protected:
	CA_ActionPtr				tester_;

private:
	CA_T<CA_EnsureConsumer>		consumer_impl_;
	CA_ActionPtr				timer_actions_;
	CA_ActionPtr				impl_;
	EnsureTesterInfoCont		ensure_testers_;
};

//生产者的子元素是ProductTester，而ProductTester的子元素只有一个，是Product。
class CA_Producer : public CA_Actions{
protected:
	CA_RunRes OnRun() override final;
	virtual void OnProduce(const CA_ActionPtr& product, const CA_ActionPtr& consumer);
	virtual enRunRes OnPoor();
};

template<CA_Define::enRunRes OnPoorResult>
class CA_ProducerT : public CA_Producer{
protected:
	enRunRes OnPoor() override final{
		return OnPoorResult;
	}
};

template<CA_Define::enRunRes OnPoorResult>
class CA_FastProducer : public CA_ProducerT<OnPoorResult>{
protected:
	void OnProduce(const CA_ActionPtr& product, const CA_ActionPtr& consumer) override{
		__super::OnProduce(product, consumer);
		assert(consumer);
		Terminate(kTF_ReRun, consumer);
	}
};
//////////////////////////////////////////////////////////////////////////
class CA_Decorator : public CA_Action{
public:
	CA_ActionPtr Decoration() const override final;
	CA_ActionPtr Tester() override;
	void SetParent(const CA_ActionPtr& parent) override final;
	void SetConsumer(const CA_ActionPtr& consumer) override final;
	void Add(const CA_ActionPtr& decoration) override;
	void Erase(const CA_ActionPtr& decoration) override;
	void clear() override;
	int Weight() const override final;

protected:
	CA_RunRes OnRun() override;

private:
	CA_ActionPtr	decoration_;
};

class CA_AsTimerAction : public CA_Decorator{
public:
	CA_AsTimerAction(const CA_ActionPtr& timer_action, pt_dword milli_seconds);
	void SetTimerAction(const CA_ActionPtr& timer_action);
	CA_ActionPtr Tester() override;

protected:
	CA_RunRes OnRun() override;

private:
	DateTimeLocal		last_time_;
	IoTimerPtr			io_timer_;
	CA_ActionPtr		timer_action_;
	pt_dword			milli_seconds_;
};

//检验者，不应该继承自CA_Decorator，因为tester对产品的保存应该是弱引用。
class CA_Tester : public CA_Action{
public:
	CA_ActionPtr Tester() override;

protected:
	CA_RunRes OnRun() override;
	virtual bool Test() = 0;
};

class CA_FastTester : public CA_Tester{
public:
	CA_FastTester();

protected:
	CA_RunRes OnRun() override final;

protected:
	enTerminateFlag		terminate_flag_;
};

class CA_EnsureTester : public CA_Decorator{
public:
	void AddEnsure(const CA_ActionPtr& action) override;
	void EraseEnsure(const CA_ActionPtr& action) override;
	void clear() override;
	bool IsTesterError(const CA_RunRes& run_res) const;

protected:
	CA_RunRes OnRun() override;

private:
	CA_RunRes RunOneTester(const CA_ActionPtr& tester);

private:
	CA_T<CA_Actions>		action_testers_;
	CA_Actions::ActionsType	action_testers_old_;
	CA_ActionPtr			as_timer_action_;
	CA_ActionPtr			action_do_impl_;
};

class CA_ProductTester : public CA_Tester{
public:
	CA_ActionPtr Decoration() const override final;
	void Add(const CA_ActionPtr& product) override;
	void Erase(const CA_ActionPtr& product) override;
	void clear() override;

protected:
	CA_ActionWeakPtr	product_;
};

class CA_Product : public CA_Tester{
public:
	CA_ActionPtr Tester() override;
	virtual CA_ActionPtr Producer();

protected:
	CA_RunRes OnRun() override;
	virtual CA_ActionPtr MakeProductTester(const CA_ActionPtr& shared_this);

private:
	CA_ActionWeakPtr		product_tester_;
	CA_ActionWeakPtr		producer_;
};

//对于CA_FastProduct来说，可以不用Producer接口了。
class CA_FastProduct : public CA_Product{
protected:
	CA_ActionPtr MakeProductTester(const CA_ActionPtr& shared_this) override;

private:
	CA_ActionPtr Producer() override final;
};

template<typename ProductImplT_, typename ProductBase>
class CA_ProductT : public ProductBase{
public:
	typedef typename std::conditional<std::is_same<ProductImplT_, CA_IAction>::value,
		CA_ActionPtr, CA_T<ProductImplT_> >::type ProductImplType;
	typedef CA_ProductT ProductType;

public:
	void Add(const CA_ActionPtr& product_impl) override{
		if (product_impl.get() == this)
		{
			assert(false);
			return;
		}
#ifdef _DEBUG
		if (product_impl)
		{
			if (!dynamic_cast<ProductImplT_*>(product_impl.get()))
			{
				assert(false);
				return;
			}
		}
#endif
		//__super::Add(product_impl);
		product_impl_ = product_impl;
	}
	void Erase(const CA_ActionPtr& product_impl) override{
		//__super::Erase(product_impl);
		if (product_impl_ == product_impl)
			product_impl_.reset();
	}
	void clear() override{
		//__super::clear();
		product_impl_.reset();
	}

protected:
	CA_RunRes OnRun() override final{
		if (!product_impl_)
			return __super::OnRun();
		return product_impl_->Run();
	}

protected:
	ProductImplType		product_impl_;
};

class CA_Not : public CA_Decorator{
protected:
	CA_RunRes OnRun() override final;
};

template<typename ActionBase, typename FnT>
class CA_Lambda : public Details::CA_LambdaImpl<std::is_base_of<CA_Tester, ActionBase>::value,
	ActionBase, FnT>{
public:
	CA_Lambda(const FnT& fn) : MyLambdaType(fn){}
};
//////////////////////////////////////////////////////////////////////////
class CA_Factory{
	template<typename AddPolicy>
	struct stAddHelper{
		template<typename... SubActions>
		static void Add(const CA_ActionPtr& parent, const SubActions&... sub_actions){
			return AddSubActionImpl_(parent, sub_actions...);
		}
	};
	template<>
	struct stAddHelper<CA_tagAddEnsure>{
		template<typename... SubActions>
		static void Add(const CA_ActionPtr& parent, const SubActions&... sub_actions){
			return AddEnsureSubActionImpl_(parent, sub_actions...);
		}
	};

public:
	virtual ~CA_Factory();
	static CA_T<CA_AsTimerAction> MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action, pt_dword milli_seconds);
	static CA_ActionPtr MakeNot(const CA_ActionPtr& decoration);
	template<typename ActionT, typename AddPolicy = CA_tagAdd, typename... SubActions>
	static CA_T<ActionT> MakeAction(const SubActions&... sub_actions){
		CA_T<ActionT> res(new ActionT);
		if (!res)
		{
			assert(false);
			return nullptr;
		}
		stAddHelper<AddPolicy>::Add(res, sub_actions...);
		return res;
	}
	template<typename ActionBase, typename FnT, typename... SubActions>
	static CA_ActionPtr MakeLambda(const FnT& fn, const SubActions&... sub_actions){
		CA_ActionPtr res(new CA_Lambda<ActionBase, FnT>(fn));
		AddSubActionImpl_(res, sub_actions...);
		return res;
	}
	template<typename FnT>
	static CA_ActionPtr MakeLambda(const FnT& fn){
		return CA_ActionPtr(new CA_Lambda<CA_Action, FnT>(fn));
	}
	//product_tester最好继承自CA_Decorator。
	template<typename... ProductsInfo>
	static CA_ActionPtr MakeProducer(const CA_ActionPtr& consumer, const ProductsInfo&... products_info){
		return MakeProducer<CA_FastProducer<CA_IAction::kRR_Succeed> >(consumer, products_info...);
	}
	template<typename ProducerT, typename... ProductsInfo>
	static CA_ActionPtr MakeProducer(const CA_ActionPtr& consumer, const ProductsInfo&... products_info){
		if (!consumer)
		{
			assert(false);
			return nullptr;
		}
		CA_ActionPtr producer = MakeAction<ProducerT>();
		if (!producer)
		{
			assert(false);
			return nullptr;
		}
		producer->SetConsumer(consumer);
		FillProducts(producer, products_info...);
		return producer;
	}
	template<typename... ProductsInfo>
	static void FillProducts(const CA_ActionPtr& producer, const ProductsInfo&... products_info){
		AddProductImpl_(producer, products_info...);
	}
	template<typename... SubActions>
	static CA_T<CA_Actions> MakeActions(const SubActions&... sub_actions){
		return MakeAction<CA_Actions>(sub_actions...);
	}
	template<typename... SubActions>
	static CA_ActionPtr MakeConsumer(const SubActions&... sub_actions){
		return MakeAction<CA_Consumer>(sub_actions...);
	}
	template<typename... SubActions>
	static CA_T<CA_EnsureConsumer> MakeEnsureConsumer(const SubActions&... sub_actions){
		return MakeAction<CA_EnsureConsumer, CA_tagAddEnsure>();
	}

private:
	static void AddSubActionImpl_(const CA_ActionPtr& parent){}
	static void AddSubActionImpl_(const CA_ActionPtr& parent, const CA_ActionPtr& sub_action){
		assert(sub_action);
		parent->Add(sub_action);
	}
	template<typename... SubActions>
	static void AddSubActionImpl_(const CA_ActionPtr& parent, const CA_ActionPtr& sub_action, const SubActions&... sub_actions){
		AddSubActionImpl_(parent, sub_action);
		AddSubActionImpl_(parent, sub_actions...);
	}
	static void AddProductImpl_(const CA_ActionPtr& producer){}
	static void AddProductImpl_(const CA_ActionPtr& producer, const CA_ActionPtr& product_tester, const CA_ActionPtr& product){
		if (!product_tester || !product)
		{
			assert(false);
			return;
		}
		product_tester->Add(product);
		producer->Add(product_tester);
	}
	template<typename... ProductsInfo>
	static void AddProductImpl_(const CA_ActionPtr& producer, const CA_ActionPtr& product_tester, const CA_ActionPtr& product, const ProductsInfo&... products_info){
		AddProductImpl_(producer, product_tester, product);
		AddProductImpl_(producer, products_info...);
	}
	static void AddEnsureSubActionImpl_(const CA_T<CA_EnsureConsumer>& parent){}
	static void AddEnsureSubActionImpl_(const CA_T<CA_EnsureConsumer>& parent, const CA_ActionPtr& sub_action){
		assert(sub_action);
		parent->AddEnsure(sub_action);
	}
	template<typename... SubActions>
	static void AddEnsureSubActionImpl_(const CA_T<CA_EnsureConsumer>& parent, const CA_ActionPtr& sub_action, const SubActions&... sub_actions){
		AddEnsureSubActionImpl_(parent, sub_action);
		AddEnsureSubActionImpl_(parent, sub_actions...);
	}
};
//////////////////////////////////////////////////////////////////////////