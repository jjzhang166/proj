#pragma once
/*
@author zhp
@date 2014/9/16 21:34
@purpose for some template utilities
*/
#include <ProtocolX/P_DefinesX.h>
#include "CmnMix/Cmn_Exception.h"
#include <memory>
#include <boost/shared_ptr.hpp>
#include "../DependSelfLib.h"


template<typename Cont>
struct DeduceContValueType;

template<class T, class U>
std::shared_ptr<typename T::element_type>
	SmartCast( const std::shared_ptr<U>& r ){
		typedef typename T::element_type EleT;
		(void)static_cast<EleT*>(static_cast<U*>(0));
		if ( !r )
		{
			assert(false);
			throw CmnCastException(typeid(U), "is zero,", typeid(EleT));
		}
		auto res = P_CAST<EleT*>(r.get());
		if ( !res )
			throw CmnCastException(typeid(U), typeid(EleT));
		return std::shared_ptr<EleT>(r, res);
}
template<class T, class U>
boost::shared_ptr<typename T::element_type>
	SmartCast( const boost::shared_ptr<U>& r ){
		typedef typename T::element_type EleT;
		(void)static_cast<EleT*>(static_cast<U*>(0));
		if ( !r )
		{
			assert(false);
			throw CmnCastException(typeid(U), "is zero,", typeid(EleT));
		}
		auto res = P_CAST<EleT*>(r.get());
		if ( !res )
			throw CmnCastException(typeid(U), typeid(EleT));
		return boost::shared_ptr<EleT>(r, res);
}
template<class T, class U>
std::shared_ptr<T> PointerCast( const std::shared_ptr<U>& r ){
	typedef typename std::shared_ptr<T>::element_type EleT;
	(void)static_cast<const EleT*>(static_cast<const U*>(0));
	if ( !r )
	{
		assert(false);
		throw CmnCastException(typeid(U), "is zero,", typeid(EleT));
	}
	auto res = P_CAST<EleT*>(r.get());
	if ( !res )
		throw CmnCastException(typeid(U), typeid(EleT));
	return std::shared_ptr<EleT>(r, res);
}
template<class T, class U>
boost::shared_ptr<T> PointerCast( const boost::shared_ptr<U>& r ){
	typedef typename boost::shared_ptr<T>::element_type EleT;
	(void)static_cast<EleT*>(static_cast<U*>(0));
	if ( !r )
	{
		assert(false);
		throw CmnCastException(typeid(U), "is zero,", typeid(EleT));
	}
	auto res = P_CAST<EleT*>(r.get());
	if ( !res )
		throw CmnCastException(typeid(U), typeid(EleT));
	return boost::shared_ptr<EleT>(r, res);
}
template<class T, class U>
T* PointerCast( U r ){
	(void)static_cast<T*>(static_cast<U>(0));
	if ( !r )
	{
		assert(false);
		throw CmnCastException(typeid(U), "is zero,", typeid(T));
	}
	auto res = P_CAST<T*>(r);
	if ( !res )
		throw CmnCastException(typeid(U), typeid(T));
	return res;
}

template<typename Cont>
struct DeduceContValueType{
	typedef typename Cont::value_type type;
};
template<typename ValueT, int size>
struct DeduceContValueType<ValueT[size]>{
	typedef ValueT type;
};

/*
为了解决operator返回类型无法成功赋值的问题
*/
template<typename T>
void SetValueAux( T& dst, const T& src ){
	dst = src;
}

template<typename T>
T& PointerToRef( T* ptr ){
	auto ptr_ptr = &ptr;
	return **ptr_ptr;
}
template<typename T>
const T& PointerToRef( const T* ptr ){
	auto ptr_ptr = &ptr;
	return **ptr_ptr;
}
//////////////////////////////////////////////////////////////////////////
template<typename SmartPtrT, typename DerivedT>
struct SmartT : std::pair<SmartPtrT, DerivedT*>{
	typedef SmartPtrT smart_type;
	typedef DerivedT derived_type;
	typedef std::pair<smart_type, derived_type*> MyBase;

	SmartT() = default;
	SmartT(const void*) : MyBase(){}
	SmartT(const smart_type& the_first) : MyBase(the_first, nullptr){
#ifdef _DEBUG
		if (the_first)
		{
			if (!dynamic_cast<derived_type*>(the_first.get()))
			{
				assert(false);
				return;
			}
		}
#endif
		second = (derived_type*)the_first.get();
	}
	explicit SmartT(derived_type* the_second) : MyBase(nullptr, the_second){
		first.reset(the_second);
	}
	SmartT(const smart_type& the_first, derived_type* the_second) : MyBase(the_first, the_second){}
	friend bool operator == (const SmartT& lhs, const smart_type& rhs){
		return lhs.first == rhs;
	}
	friend bool operator == (const SmartT& lhs, const derived_type* rhs){
		return lhs.second == rhs;
	}
	friend bool operator == (const smart_type& lhs, const SmartT& rhs){
		return lhs == rhs.first;
	}
	friend bool operator == (const derived_type* lhs, const SmartT& rhs){
		return lhs == rhs.second;
	}
	friend bool operator == (const SmartT& lhs, const SmartT& rhs){
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
	friend bool operator != (const SmartT& lhs, const smart_type& rhs){
		return lhs.first != rhs;
	}
	friend bool operator != (const SmartT& lhs, const derived_type* rhs){
		return lhs.second != rhs;
	}
	friend bool operator != (const smart_type& lhs, const SmartT& rhs){
		return lhs != rhs.first;
	}
	friend bool operator != (const derived_type* lhs, const SmartT& rhs){
		return lhs != rhs.second;
	}
	friend bool operator != (const SmartT& lhs, const SmartT& rhs){
		return lhs.first != rhs.first || lhs.second != rhs.second;
	}
	operator bool() const{
		return first && second;
	}
	derived_type* operator -> () const{
		assert(first && second);
		return second;
	}
	operator smart_type () const{
		return first;
	}
	void reset(){
		first.reset();
		second = nullptr;
	}
};

template<typename WeakPtrT, typename SmartPtrT, typename DerivedT>
struct SmartWeakT : public std::pair<WeakPtrT, DerivedT*>{
	typedef WeakPtrT weak_type;
	typedef SmartPtrT smart_type;
	typedef DerivedT derived_type;
	typedef std::pair<weak_type, derived_type*> MyBase;

	SmartWeakT() = default;
	SmartWeakT(const void*){}
	SmartWeakT(const smart_type& the_first) : MyBase(the_first, nullptr){
#ifdef _DEBUG
		if (the_first)
		{
			if (!dynamic_cast<derived_type*>(the_first.get()))
			{
				assert(false);
				return;
			}
		}
#endif
		second = (derived_type*)the_first.get();
	}
	SmartWeakT(const smart_type& the_first, derived_type* the_second) : MyBase(the_first, the_second){}
	SmartWeakT(const SmartT<SmartPtrT, derived_type>& rhs) : MyBase(rhs.first, rhs.second){}
	bool expired() const{
		return first.expired();
	}
	SmartT<SmartPtrT, derived_type> lock() const{
		return SmartT<SmartPtrT, derived_type>(first.lock(), second);
	}
};
//////////////////////////////////////////////////////////////////////////