#include "stdafx.h"
#include <Actions/CA_Action.h>
#include <CmnMix/Cmn_Exception.h>
#include <boost/scope_exit.hpp>
#include <CommonX/CmnX_StaticConts.h>
#include <AppMix/GameComponent.h>

//////////////////////////////////////////////////////////////////////////
class _CA_ActionTerminateExcept : public CmnExceptionBase{
public:
	_CA_ActionTerminateExcept(const CA_ActionPtr& dst_action, const CA_ActionPtr& error_action, CA_IAction::enTerminateFlag terminate_flag)
		: dst_action_(dst_action), error_action_(error_action){
		assert(dst_action);
		assert(error_action);
		terminate_flag_ = terminate_flag;
	}

	CA_ActionPtr				dst_action_;
	CA_ActionPtr				error_action_;
	CA_IAction::enTerminateFlag	terminate_flag_;

	CMN_EXCEPTION_CLONE
};
//////////////////////////////////////////////////////////////////////////

CA_IAction::~CA_IAction()
{
}

void CA_IAction::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<CA_IAction, CA_ActionPtr>("CA_IAction")
	];
}

CA_Action::CA_Action()
{
	is_running_ = false;
}

bool CA_Action::IsRunning() const
{
	return is_running_;
}

CA_RunRes CA_Action::Run()
{
	assert(shared_from_this());
	if (is_running_)
	{
		assert(false);
		return kRR_Failed;
	}
	is_running_ = true;
	BOOST_SCOPE_EXIT_ALL(this){
		is_running_ = false;
	};
	while (true)
	{
		try{
			return OnRun();
		} DO_CATCH_THE_EXCEPTION(const _CA_ActionTerminateExcept& e, {
			if (e.dst_action_ != shared_from_this())
				throw;
			if (e.terminate_flag_ & kTF_ReRun)
				continue;
			return CA_RunRes(e.error_action_);
		})
	}
	assert(false);
	return kRR_Failed;
}

void CA_Action::Terminate(enTerminateFlag terminate_flag)
{
	if (!is_running_)
	{
		assert(false);
		return;
	}
#ifdef _DEBUG
	int to_cnt = 0;
#endif
	auto shared_this = shared_from_this();
	assert(shared_this);
	if (terminate_flag & kTF_ToDecoration)
	{
#ifdef _DEBUG
		++to_cnt;
#endif
		auto decoration = Decoration();
		if (decoration && decoration->IsRunning())
			throw _CA_ActionTerminateExcept(decoration, shared_this, terminate_flag);
	}
	if (terminate_flag & kTF_ToParent)
	{
#ifdef _DEBUG
		++to_cnt;
#endif
		auto parent = Parent();
		if (parent && parent->IsRunning())
			throw _CA_ActionTerminateExcept(parent, shared_this, terminate_flag);
	}
	if (terminate_flag & kTF_ToConsumer)
	{
#ifdef _DEBUG
		++to_cnt;
#endif
		auto consumer = Consumer();
		if (consumer && consumer->IsRunning())
			throw _CA_ActionTerminateExcept(consumer, shared_this, terminate_flag);
	}
	if (terminate_flag & kTF_ToTester)
	{
#ifdef _DEBUG
		++to_cnt;
#endif
		auto tester = Decoration();
		if (tester && tester->IsRunning())
			throw _CA_ActionTerminateExcept(tester, shared_this, terminate_flag);
	}
	assert(0 == to_cnt);
	throw _CA_ActionTerminateExcept(shared_this, shared_this, terminate_flag);
}

void CA_Action::Terminate(enTerminateFlag terminate_flag, const CA_ActionPtr& dst_action)
{
	if (!dst_action)
	{
		assert(false);
		return;
	}
	if (!dst_action->IsRunning())
	{
		assert(false);
		return;
	}
	if (!is_running_)
	{
		assert(false);
		return;
	}
	throw _CA_ActionTerminateExcept(dst_action, shared_from_this(), terminate_flag);
}

int CA_Action::Weight() const
{
	return 0;
}

void CA_Action::SetParent(const CA_ActionPtr& parent)
{
	parent_ = parent;
	if (parent)
	{
		if (!Consumer())
			SetConsumer(parent->Consumer());
	}
}

CA_ActionPtr CA_Action::Parent() const
{
	if (parent_.expired())
		return nullptr;
	return parent_.lock();
}

void CA_Action::Add(const CA_ActionPtr& action)
{
	assert(false);
}

void CA_Action::Erase(const CA_ActionPtr& action)
{
	assert(false);
}

CA_ActionPtr CA_Action::Decoration() const
{
	return nullptr;
}

CA_ActionPtr CA_Action::Tester()
{
	return nullptr;
}

void CA_Action::clear()
{
	assert(false);
}

void CA_Action::SetConsumer(const CA_ActionPtr& consumer)
{
	consumer_ = consumer;
}

void CA_Action::AddEnsure(const CA_ActionPtr& action)
{
	Add(action);
}

void CA_Action::EraseEnsure(const CA_ActionPtr& action)
{
	Erase(action);
}

CA_ActionPtr CA_Action::Consumer() const
{
	if (consumer_.expired())
	{
		CA_IAction* p;
		{
			auto parent = Parent();
			p = parent.get();
		}
		if (p)
			return p->Consumer();
		return nullptr;
	}
	return consumer_.lock();
}

CA_RunRes::CA_RunRes()
{
	run_res_ = CA_Define::kRR_Failed;
}

CA_RunRes::CA_RunRes(CA_Define::enRunRes run_res)
{
	assert(run_res != CA_Define::kRR_Terminated);
	run_res_ = run_res;
}

CA_RunRes::CA_RunRes(const CA_ActionPtr& error_action)
{
	assert(error_action);
	run_res_ = CA_Define::kRR_Terminated;
	error_action_ = error_action;
}

CA_RunRes CA_Actions::OnRun()
{
	for (auto& action : actions_)
	{
		assert(action);
		const auto& res = action->Run();
		if (res.run_res_ < kRR_Succeed)
			return res;
	}
	return kRR_Succeed;
}

void CA_Actions::Add(const CA_ActionPtr& action)
{
	return AddImpl(actions_, action);
}

void CA_Actions::Erase(const CA_ActionPtr& action)
{
	return EraseImpl(actions_, action);
}

void CA_Actions::AddImpl(ActionsType& actions, const CA_ActionPtr& action)
{
	if (!action)
	{
		assert(false);
		return;
	}
	if (action.get() == this)
	{
		assert(false);
		return;
	}
#ifdef _DEBUG
	auto parent = Parent();
	if (parent && parent == action)
	{
		assert(false);
		return;
	}
#endif
	auto it = std::find(actions.begin(), actions.end(), action);
	if (it != actions.end())
	{
		//assert(false);
		return;
	}
	if (!action->Parent())
		action->SetParent(shared_from_this());
	struct stHelp{
		CA_ActionPtr	action_;
		int				weight_;
	};
	CmnBufferVector<stHelp, 100> cont_tmp;
	for (auto& v : actions)
		cont_tmp.push_back({ v, v->Weight() });
	cont_tmp.push_back({ action, action->Weight() });

	std::sort(cont_tmp.begin(), cont_tmp.end(), [](const stHelp& lhs, const stHelp& rhs){
		return lhs.weight_ < rhs.weight_;
	});
	actions.clear();
	for (auto& v : cont_tmp)
		actions.push_back(v.action_);
}

void CA_Actions::EraseImpl(ActionsType& actions, const CA_ActionPtr& action)
{
	auto it = std::find(actions.begin(), actions.end(), action);
	if (it == actions.end())
		return;
	assert(action);
	auto the_parent = action->Parent();
	if (the_parent && the_parent.get() == this)
		action->SetParent(nullptr);
	actions.erase(it);
}

const CA_Actions::ActionsType& CA_Actions::GetActions() const
{
	return actions_;
}

void CA_Actions::clear()
{
	actions_.clear();
}

void CA_Decorator::SetParent(const CA_ActionPtr& parent)
{
	__super::SetParent(parent);
	if (decoration_ && !decoration_->Parent())
		decoration_->SetParent(parent);
}

void CA_Decorator::Erase(const CA_ActionPtr& decoration)
{
	if (decoration_ == decoration)
		decoration_.reset();
}

void CA_Decorator::Add(const CA_ActionPtr& decoration)
{
	if (decoration && decoration.get() == this)
	{
		assert(false);
		return;
	}
	decoration_ = decoration;
}

CA_RunRes CA_Decorator::OnRun()
{
	if (decoration_)
		return decoration_->Run();
	assert(false);
	return kRR_Failed;
}

CA_ActionPtr CA_Decorator::Decoration() const
{
	return decoration_;
}

int CA_Decorator::Weight() const
{
	if (decoration_)
		return decoration_->Weight();
	return __super::Weight();
}

CA_ActionPtr CA_Decorator::Tester()
{
	if (decoration_)
		return decoration_->Tester();
	return nullptr;
}

void CA_Decorator::clear()
{
	decoration_.reset();
}

void CA_Decorator::SetConsumer(const CA_ActionPtr& consumer)
{
	__super::SetConsumer(consumer);
	if (decoration_)
		decoration_->SetConsumer(consumer);
}

CA_AsTimerAction::CA_AsTimerAction(const CA_ActionPtr& timer_action, pt_dword milli_seconds) : timer_action_(timer_action)
{
	assert(timer_action);
	assert(milli_seconds);
	milli_seconds_ = milli_seconds;
}

CA_RunRes CA_AsTimerAction::OnRun()
{
	if (!timer_action_)
	{
		assert(false);
		return kRR_Failed;
	}
	if (io_timer_)
	{
		DateTimeLocal time_now;
		auto time_span = time_now - last_time_;
		auto left_time = milli_seconds_;
		auto total_time = time_span.totalMilliseconds();
		if (total_time >= milli_seconds_)
		{
			if (!timer_action_->IsRunning())
			{
				last_time_ = time_now;
				timer_action_->Run();
			}
		}
		else
			left_time = milli_seconds_ - total_time;
		io_timer_->SetDuration(left_time);
		io_timer_->Continue();
	}
	else
	{
		CA_ActionWeakPtr weak_this = shared_from_this();
		io_timer_ = LockableIoTimer(milli_seconds_, true, [weak_this, this](const IoTimerPtr& timer){
			if (weak_this.expired())
			{
				assert(false);
				return;
			}
			auto shared_this = weak_this.lock();
			if (!shared_this)
			{
				assert(false);
				return;
			}
			if (!shared_this->IsRunning())
			{
				assert(false);
				return;
			}
			if (!timer_action_)
			{
				assert(false);
				return;
			}
			assert(timer);
			timer->Continue();
			if (timer_action_->IsRunning())
				return;
			last_time_ = DateTimeLocal();
			timer_action_->Run();
		});
		if (!io_timer_)
		{
			assert(false);
			return kRR_Failed;
		}
	}	
	BOOST_SCOPE_EXIT_ALL(this){
		io_timer_->Cancel();
	};
	return __super::OnRun();
}

CA_ActionPtr CA_AsTimerAction::Tester()
{
	if (timer_action_)
		return timer_action_;
	return __super::Tester();
}

void CA_AsTimerAction::SetTimerAction(const CA_ActionPtr& timer_action)
{
	assert(timer_action);
	timer_action_ = timer_action;
}

CA_RunRes CA_Consumer::OnRun()
{
	while (!actions_.empty())
	{
		auto product = actions_.front();
		actions_.pop_front();
		assert(product);
		const auto& res = product->Run();
		if (res.run_res_ < kRR_Succeed)
			return res;
		OnConsumedProduct(product);
	}
	return kRR_Succeed;
}

void CA_Consumer::OnConsumedProduct(const CA_ActionPtr& product)
{
}

void CA_Consumer::SetConsumer(const CA_ActionPtr& consumer)
{
}

CA_ActionPtr CA_Consumer::Consumer() const
{
	return ((CA_Consumer*)this)->shared_from_this();
}

CA_RunRes CA_Producer::OnRun()
{
	for (auto& product_tester : actions_)
	{
		assert(product_tester);
		const auto& res = product_tester->Run();
		if (res.run_res_ < kRR_Succeed)
			continue;
		auto product = product_tester->Decoration();
		if (!product)
		{
			assert(false);
			continue;
		}
		auto consumer = Consumer();
		if (!consumer)
		{
			assert(false);
			return kRR_Failed;
		}
		OnProduce(product, consumer);
		return kRR_Succeed;
	}
	return OnPoor();
}

void CA_Producer::OnProduce(const CA_ActionPtr& product, const CA_ActionPtr& consumer)
{
	assert(product);
	assert(consumer);
	consumer->Add(product);
}

CA_Producer::enRunRes CA_Producer::OnPoor()
{
	return kRR_Failed;
}

CA_RunRes CA_Tester::OnRun()
{
	if (Test())
		return kRR_Succeed;
	return kRR_Failed;
}

CA_ActionPtr CA_Tester::Tester()
{
	return shared_from_this();
}

CA_ActionPtr CA_ProductTester::Decoration() const
{
	if (product_.expired()) return nullptr;
	return product_.lock();
}

void CA_ProductTester::Add(const CA_ActionPtr& product)
{
	if (product && product.get() == this)
	{
		assert(false);
		return;
	}
	product_ = product;
}

void CA_ProductTester::Erase(const CA_ActionPtr& product)
{
	if (product_.expired()) return;
	if (product_.lock() == product)
		product_.reset();
}

void CA_ProductTester::clear()
{
	product_.reset();
}

CA_RunRes CA_FastTester::OnRun()
{
	if (Test())
	{
		Terminate(terminate_flag_);
		return kRR_Succeed;
	}
	return kRR_Failed;
}

CA_FastTester::CA_FastTester()
{
	terminate_flag_ = enTerminateFlag(kTF_ToDecoration | kTF_ToParent | kTF_ToConsumer);
}

CA_Factory::~CA_Factory()
{

}

CA_ActionPtr CA_Factory::MakeNot(const CA_ActionPtr& decoration)
{
	if (!decoration)
	{
		assert(false);
		return nullptr;
	}
	return MakeAction<CA_Not>(decoration);
}

CA_T<CA_AsTimerAction> CA_Factory::MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action, pt_dword milli_seconds)
{
	if (!decoration)
	{
		assert(false);
		return nullptr;
	}
	if (!timer_action)
	{
		assert(false);
		return nullptr;
	}
	CA_T<CA_AsTimerAction> res(new CA_AsTimerAction(timer_action, milli_seconds));
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!timer_action->Parent())
		timer_action->SetParent(res);
	res->Add(decoration);
	return res;
}

CA_RunRes CA_Not::OnRun()
{
	const auto& run_res = __super::OnRun();
	if (run_res.run_res_ == kRR_Succeed)
		return kRR_Failed;
	else if (run_res.run_res_ == kRR_Failed)
		return kRR_Succeed;
	return run_res;
}

void CA_EnsureConsumer::AddEnsure(const CA_ActionPtr& action)
{
	return AddImpl(ensure_actions_, action);
}

void CA_EnsureConsumer::EraseEnsure(const CA_ActionPtr& action)
{
	return EraseImpl(ensure_actions_, action);
}

CA_RunRes CA_EnsureConsumer::OnRun()
{
	for (auto& v : ensure_actions_)
		Add(v);
	return __super::OnRun();
}

void CA_EnsureConsumer::OnConsumedProduct(const CA_ActionPtr& product)
{
	assert(product);
	EraseEnsure(product);
}

CA_ActionPtr CA_EnsureConsumer::Tester()
{
	if (tester_)
		return tester_;
	CA_ActionWeakPtr weak_this = shared_from_this();
	tester_ = CA_Factory::MakeLambda([weak_this, this](){
		if (weak_this.expired())
			return kRR_Failed;
		auto lock_this = weak_this.lock();
		for (auto& v : ensure_actions_)
		{
			auto the_tester = v->Tester();
			if (the_tester)
				the_tester->Run();
		}
		return kRR_Succeed;
	});
	if (!tester_)
	{
		assert(false);
		return nullptr;
	}
	tester_->SetConsumer(Consumer());
	tester_->SetParent(shared_from_this());
	return tester_;
}

void CA_EnsureConsumer::clear()
{
	__super::clear();
	ensure_actions_.clear();
}

CA_ActionPtr CA_Product::Tester()
{
	if (!product_tester_.expired())
		return product_tester_.lock();
	auto shared_this = shared_from_this();
	if (!shared_this)
	{
		assert(false);
		return nullptr;
	}
	auto product_tester = MakeProductTester(shared_this);
	if (!product_tester)
	{
		assert(false);
		return nullptr;
	}
	product_tester->SetParent(shared_this);
	product_tester->Add(shared_this);
	product_tester_ = product_tester;
	return product_tester;
}

CA_ActionPtr CA_Product::Producer()
{
	if (!producer_.expired())
		return producer_.lock();
	auto consumer = Consumer();
	if (!consumer)
	{
		assert(false);
		return nullptr;
	}
	auto product_tester = Tester();
	if (!product_tester)
	{
		assert(false);
		return nullptr;
	}
	auto shared_this = shared_from_this();
	if (!shared_this)
	{
		assert(false);
		return nullptr;
	}
	auto producer = CA_Factory::MakeProducer(consumer, product_tester, shared_this);
	if (!producer)
	{
		assert(false);
		return nullptr;
	}
	producer->SetParent(shared_this);
	producer_ = producer;
	return producer;
}

CA_RunRes CA_Product::OnRun()
{
	auto product_impl = Decoration();
	if (!product_impl)
	{
		assert(false);
		return kRR_Failed;
	}
	return product_impl->Run();
}

CA_ActionPtr CA_Product::MakeProductTester(const CA_ActionPtr& shared_this)
{
	CA_ActionWeakPtr weak_this = shared_this;
	return CA_Factory::MakeLambda<CA_ProductTester>([weak_this, this](){
		if (weak_this.expired())
			return false;
		auto lock_this = weak_this.lock();
		return Test();
	});
}

CA_SmartConsumer::CA_SmartConsumer()
{
	consumer_impl_ = CA_Factory::MakeEnsureConsumer();
	assert(consumer_impl_);
	impl_ = consumer_impl_;
}

void CA_SmartConsumer::AddEnsure(const CA_ActionPtr& action)
{
	if (!action)
	{
		assert(false);
		return;
	}
	if (!consumer_impl_)
	{
		assert(false);
		return;
	}
	consumer_impl_->AddEnsure(action);
}

void CA_SmartConsumer::AddEnsure(const CA_ActionPtr& action, const CA_ActionPtr& action_tester)
{
	if (!action_tester)
		return AddEnsure(action);
	if (!action)
	{
		assert(false);
		return;
	}
	if (!consumer_impl_)
	{
		assert(false);
		return;
	}
	if (!action_tester->Parent())
		action_tester->SetParent(consumer_impl_);
	CA_ActionPtr as_timer_action = Cmn_GameDllApp::GetApp().GetAppFactory().MakeAsTimerAction(action, action_tester);
	if (!as_timer_action)
	{
		assert(false);
		return;
	}
	ensure_testers_.push_back({ action, action_tester, as_timer_action });
	consumer_impl_->AddEnsure(as_timer_action);
}

void CA_SmartConsumer::AddTester(const CA_ActionPtr& tester)
{
	if (!tester)
	{
		assert(false);
		return;
	}
	if (!timer_actions_)
	{
		timer_actions_ = CA_Factory::MakeAction<CA_EveryActions>();
		if (!timer_actions_)
		{
			assert(false);
			return;
		}
		assert(impl_ == consumer_impl_);
		impl_ = Cmn_GameDllApp::GetApp().GetAppFactory().MakeAsTimerAction(consumer_impl_, timer_actions_);
	}
	if (!tester->Parent())
		tester->SetParent(consumer_impl_);
	timer_actions_->Add(tester);
}

CA_RunRes CA_SmartConsumer::OnRun()
{
	if (!impl_)
	{
		assert(false);
		return kRR_Failed;
	}
	return impl_->Run();
}

void CA_SmartConsumer::EraseEnsure(const CA_ActionPtr& action)
{
	if (!action)
	{
		assert(false);
		return;
	}
	CmnBufferVector<stEnsureTesterInfo, 100> cont_tmp;
	for (auto& v : ensure_testers_)
	{
		if (v.action_.expired())
			continue;
		if (v.action_tester_.expired())
			continue;
		if (v.as_timer_action_.expired())
			continue;
		cont_tmp.push_back(v);
	}
	ensure_testers_.clear();
	for (auto& v : cont_tmp)
	{
		if (v.action_.lock() == action)
			consumer_impl_->EraseEnsure(v.as_timer_action_.lock());
		else
			ensure_testers_.push_back(v);
	}
	if (!consumer_impl_)
	{
		assert(false);
		return;
	}
	consumer_impl_->EraseEnsure(action);
}

void CA_SmartConsumer::EraseTester(const CA_ActionPtr& tester)
{
	if (!timer_actions_)
	{
		assert(false);
		return;
	}
	timer_actions_->Erase(tester);
}

void CA_SmartConsumer::Add(const CA_ActionPtr& action)
{
	if (!consumer_impl_)
	{
		assert(false);
		return;
	}
	consumer_impl_->Add(action);
}

void CA_SmartConsumer::Erase(const CA_ActionPtr& action)
{
	if (!consumer_impl_)
	{
		assert(false);
		return;
	}
	consumer_impl_->Erase(action);
}

CA_ActionPtr CA_SmartConsumer::Tester()
{
	if (tester_)
		return tester_;
	if (!consumer_impl_)
	{
		assert(false);
		return nullptr;
	}
	CA_ActionWeakPtr weak_this = shared_from_this();
	tester_ = CA_Factory::MakeLambda([weak_this, this](){
		if (weak_this.expired())
			return kRR_Failed;
		auto lock_this = weak_this.lock();
		CmnBufferVector<stEnsureTesterInfo, 100> cont_tmp;
		for (auto& v : ensure_testers_)
		{
			if (v.action_.expired())
				continue;
			if (v.action_tester_.expired())
				continue;
			if (v.as_timer_action_.expired())
				continue;
			cont_tmp.push_back(v);
		}
		ensure_testers_.clear();
		for (auto& v : cont_tmp)
			ensure_testers_.push_back(v);
		cont_tmp.clear();

		for (auto& v : ensure_testers_)
		{
			auto the_tester = v.action_tester_.lock();
			if (the_tester)
				the_tester->Run();
		}
		return kRR_Succeed;
	});
	if (!tester_)
	{
		assert(false);
		return nullptr;
	}
	tester_->SetConsumer(consumer_impl_);
	tester_->SetParent(consumer_impl_);
	return tester_;
}

void CA_SmartConsumer::clear()
{
	if (consumer_impl_)
		consumer_impl_->clear();
	if (timer_actions_)
		timer_actions_->clear();
	ensure_testers_.clear();
}

CA_ActionPtr CA_SmartConsumer::Consumer() const
{
	return consumer_impl_;
}

void CA_SmartConsumer::SetConsumer(const CA_ActionPtr& consumer)
{
}

void CA_EnsureTester::AddEnsure(const CA_ActionPtr& action)
{
	if (!action)
	{
		assert(false);
		return;
	}
	auto the_tester = action->Tester();
	if (!the_tester)
		return;
	if (!action_testers_)
	{
		action_testers_ = CA_Factory::MakeActions();
		if (!action_testers_)
		{
			assert(false);
			return;
		}
	}
	action_testers_->Add(the_tester);
	if (action_do_impl_ && action_do_impl_ == as_timer_action_)
		return;
	action_do_impl_ = Decoration();
	if (!action_do_impl_)
	{
		assert(false);
		return;
	}
	auto tester_wrapper = CA_Factory::MakeLambda([this]() -> CA_RunRes{
		if (!action_testers_)
			return kRR_Failed;
		for (auto& v : action_testers_->GetActions())
			RunOneTester(v);
		return kRR_Succeed;
	});
	if (!tester_wrapper)
	{
		assert(false);
		return;
	}
	as_timer_action_ = Cmn_GameDllApp::GetApp().GetAppFactory().MakeAsTimerAction(action_do_impl_, tester_wrapper);
	if (!as_timer_action_)
	{
		assert(false);
		return;
	}
	action_do_impl_ = as_timer_action_;
}

CA_RunRes CA_EnsureTester::OnRun()
{
	if (action_do_impl_)
	{
		action_testers_old_.clear();
		for (auto& v : action_testers_->GetActions())
			action_testers_old_.push_back(v);
		return action_do_impl_->Run();
	}
	return __super::OnRun();
}

bool CA_EnsureTester::IsTesterError(const CA_RunRes& run_res) const
{
	if (!run_res.error_action_)
		return false;
	for (auto& v : action_testers_old_)
	{
		if (run_res.error_action_ == v)
			return true;
	}
	return false;
}

void CA_EnsureTester::clear()
{
	if (action_testers_)
		action_testers_->clear();
	action_testers_old_.clear();
}

CA_RunRes CA_EnsureTester::RunOneTester(const CA_ActionPtr& tester)
{
	assert(tester);
	auto the_top_action = tester->Decoration();
	if (the_top_action && the_top_action->IsRunning())
		return tester->Run();
	the_top_action = tester->Parent();
	if (the_top_action && the_top_action->IsRunning())
		return tester->Run();
	assert(action_do_impl_);
	tester->SetParent(action_do_impl_);
	BOOST_SCOPE_EXIT_ALL(&the_top_action, &tester){
		tester->SetParent(the_top_action);
	};
	return tester->Run();
}

void CA_EnsureTester::EraseEnsure(const CA_ActionPtr& action)
{
	if (action_testers_)
		action_testers_->Erase(action);
}

CA_RunRes CA_EveryActions::OnRun()
{
	for (auto& action : actions_)
	{
		assert(action);
		action->Run();
	}
	return kRR_Succeed;
}

CA_ActionPtr CA_FastProduct::MakeProductTester(const CA_ActionPtr& shared_this)
{
	struct ProductTester : CA_ProductTester{
		CA_RunRes ProductTester::OnRun() override{
			if (Test())
			{
				auto product = Decoration();
				if (product)
				{
					auto consumer = Consumer();
					if (consumer)
						consumer->Add(product);
					else
						assert(false);
				}
				else
					assert(false);
				Terminate(kTF_ToConsumer | kTF_ReRun);
				return kRR_Succeed;
			}
			return kRR_Failed;
		}
	};
	CA_ActionWeakPtr weak_this = shared_this;
	return CA_Factory::MakeLambda<ProductTester>([weak_this, this](){
		if (weak_this.expired()) return false;
		auto lock_this = weak_this.lock();
		return Test();
	});
}

CA_ActionPtr CA_FastProduct::Producer()
{
	assert(!"此类不需要该接口了");
	return nullptr;
}
