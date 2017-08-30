#pragma once
/*
@author zhp
@date 2017/3/29 10:54
@purpose for filter
CF->Common Filter
*/
#include <Common/UsefulClass.h>
#include <vector>
#include <luabind/luabind.hpp>
#include <boost/smart_ptr.hpp>

class CF_BasedNone{};
class CF_BasedLua{
public:
	virtual ~CF_BasedLua(){}
	virtual bool Parse(const luabind::object& lua_obj){
		assert(false);
		return false;
	}
};

template<typename FittedType_, typename BasedType_ = CF_BasedNone>
struct CF_Traits{
	typedef FittedType_ FittedType;
	typedef BasedType_ BasedType;
};

struct CF_Getter{
	template<typename T>
	static const T* GetPointer(const T& v){
		return &v;
	}
	template<typename T>
	static const T* GetPointer(const T* ptr){
		return ptr;
	}
};

struct CF_SmartGetter{
	template<typename T>
	static T& GetPointer(T& v){
		return v;
	}
};

template<typename Traits_>
class CF_IFilter : public Traits_::BasedType{
public:
	typedef Traits_ TraitsType;
	typedef typename TraitsType::FittedType FittedType;
	typedef boost::shared_ptr<CF_IFilter<Traits_> > FilterPtr;

protected:
	CF_IFilter() = default;
	virtual ~CF_IFilter(){}

public:
	virtual bool DoFilt(const FittedType& obj) const = 0;
};

template<typename Traits_, bool DefRet>
class CF_Filter : public CF_IFilter<Traits_>{
protected:
	CF_Filter() = default;

public:
	bool DoFilt(const FittedType& obj) const override{
		return DefRet;
	}
};

template<typename Traits_, bool DefRet_, typename ValueType_,
	typename ContT = std::vector<value_type>,
	typename Getter_ = CF_Getter>
class CF_Cont : public CF_Filter<Traits_, DefRet_>{
	typedef ValueType_ value_type;
	typedef Getter_ getter_type;
	typedef NameList<value_type, ContT> FiltersContT;

public:
	template<typename... Args>
	CF_Cont(const Args&... args) : filters_(args...){}
	template<typename T>
	CF_Cont& operator & (const T& v){
		auto p = getter_type::GetPointer(v);
		if (!p)
		{
			assert(false);
			return *this;
		}
		filters_.Add(p);
		return *this;
	}
	template<typename T>
	void Erase(const T& v){
		filters_.Delete(getter_type::GetPointer(v));
	}
	void clear(){
		filters_.clear();
	}
	bool DoFilt(const FittedType& obj) const override{
		for (const auto& filter : filters_)
		{
			const auto& f = getter_type::GetPointer(filter);
			if (!f)
				continue;
			if (f->DoFilt(obj) == DefRet)
				return DefRet;
		}
		return !DefRet;
	}
	const FiltersContT& GetFilters() const{
		return filters_;
	}
	FiltersContT& GetFilters(){
		return filters_;
	}

public:
	static const bool DefRet = DefRet_;

protected:
	FiltersContT	filters_;
};

template<typename FilterContT_>
class CF_Ensure : public CF_IFilter<typename FilterContT_::TraitsType>{
	typedef FilterContT_ FilterContType;

public:
	void clear(){
		filter_cont_.Erase(*this);
	}

protected:
	CF_Ensure(FilterContType& filter_cont) : filter_cont_(filter_cont){
		filter_cont & *this;
	}
	~CF_Ensure(){
		filter_cont_.Erase(*this);
	}

protected:
	FilterContType&		filter_cont_;
};

template<bool DefRet, typename ValueType_, ValueType_ MaxValueCnt, ValueType_ InvalidValue>
class CF_Value : public CF_Filter<CF_Traits<ValueType_>, DefRet>{
	typedef ValueType_ value_type;

public:
	CF_Value(){
		clear();
	}
	template<typename... Args>
	CF_Value(value_type value, Args... values) : CF_Value(){
		Add(value, values...);
	}
	CF_Value& operator & (value_type value){
		Add(value);
		return *this;
	}
	void Add(value_type value){
		for (auto& v : values_)
		{
			if (v == value)
				return;
			else if (v == InvalidValue)
			{
				v = value;
				return;
			}
		}
		assert(false);
	}
	template<typename... Args>
	void Add(value_type value, Args... values){
		Add(value);
		Add(values...);
	}
	void Erase(value_type value){
		value_type* it;
		for (auto& v : values_)
		{
			if (v == value)
			{
				it = &v;
				goto LABLE_CONTINUE;
			}
		}
		return;
	LABLE_CONTINUE:
		assert(it);
		auto it_end = (value_type*)values_ + MaxValueCnt;
		std::move(it + 1, it_end, it);
		--it_end;
		*it_end = InvalidValue;
	}
	void clear(){
		for (auto& v : values_)
			v = InvalidValue;
	}
	bool DoFilt(const value_type& value) const override{
		for (auto v : values_)
		{
			if (v == InvalidValue)
				break;
			else if ((v == value) == DefRet)
				return DefRet;
		}
		return !DefRet;
	}

private:
	value_type	values_[MaxValueCnt];
};

template<typename BaseT, typename FnT>
class CF_Lambda : public BaseT{
public:
	typedef typename BaseT::FittedType FittedType;

public:
	template<typename... Args>
	CF_Lambda(const FnT& fn, Args&... args) : fn_(fn), BaseT(args...){}
	bool DoFilt(const FittedType& obj) const override{
		return fn_(obj);
	}

private:
	FnT		fn_;
};

template<typename BaseT, typename FnT, typename... Args>
CF_Lambda<BaseT, FnT> CF_MakeLambda(const FnT& fn, Args&... args){
	return CF_Lambda<BaseT, FnT>(fn, args...);
}
template<typename BaseT, typename FnT, typename... Args>
CF_Lambda<BaseT, FnT>* CF_MakeLambdaPtr(const FnT& fn, Args&... args){
	return new CF_Lambda<BaseT, FnT>(fn, args...);
}
