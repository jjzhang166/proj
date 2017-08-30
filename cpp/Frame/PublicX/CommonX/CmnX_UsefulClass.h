#pragma once
/*
@author zhp
@date 2016/1/27 16:59
@purpose 一些通用小类
*/
#include "../DependSelfLib.h"
#include <cassert>
#include <ProtocolX/P_DefinesX.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_types.hpp>

class CmnX_CodeInfo{
public:
	CmnX_CodeInfo();
	CmnX_CodeInfo( void* begin_addr, void* end_addr );
	CmnX_CodeInfo( void* begin_addr, unsigned int addr_size );
	CmnX_CodeInfo( const CmnX_CodeInfo& rhs );
	CmnX_CodeInfo& operator = ( const CmnX_CodeInfo& rhs );
	bool IsInRange( const void* addr ) const;
	bool IsValid() const;
	int OffsetToBeginAddr( const void* addr ) const;
	//是否存在交集
	bool ExistIntersection( const CmnX_CodeInfo& rhs ) const;
	CmnX_CodeInfo CalcIntersection( const CmnX_CodeInfo& rhs ) const;
	int GetCodeSize() const;
	CmnX_CodeInfo& operator -= ( const CmnX_CodeInfo& rhs );
	CmnX_CodeInfo& operator - ( const CmnX_CodeInfo& rhs ) const;
	CmnX_CodeInfo& operator += ( const CmnX_CodeInfo& rhs );
	CmnX_CodeInfo& operator + ( const CmnX_CodeInfo& rhs ) const;

public:
	void*		begin_addr_;
	void*		end_addr_;
};
static_assert(sizeof(CmnX_CodeInfo) == sizeof(void*) * 2, "");

//////////////////////////////////////////////////////////////////////////
//GetInstance
class Singleton_Instance;
//GetMe
class Singleton_Me;
//make me
class Singleton_MakeMe;

//sigleton
template<typename Derived, typename Tag>
class Singleton;
template<typename Derived>
class Singleton<Derived, Singleton_Instance>{
private:
	Singleton( const Singleton& );
	Singleton& operator = ( const Singleton& );

public:
	Singleton(){
		assert(!this_ptr_);
		this_ptr_ = static_cast<Derived*>(this);
	}
	~Singleton(){
		assert(this_ptr_);
		this_ptr_ = nullptr;
	}
	static Derived* GetInstance(){
		return this_ptr_;
	}

private:
	static Derived*					this_ptr_;
};
template<typename Derived>
class Singleton<Derived, Singleton_Me> : public Singleton<Derived, Singleton_Instance>{
public:
	static Derived& GetMe(){
		auto ptr_self = GetInstance();
		assert(ptr_self);
		return *ptr_self;
	}
};

template<typename Derived>
class Singleton<Derived, Singleton_MakeMe> : public Singleton<Derived, Singleton_Instance>{
public:
	static Derived& GetMe(){
		auto ptr_self = GetInstance();
		if (ptr_self)
			return *ptr_self;
		ptr_self = new Derived;
		assert(ptr_self);
		return *ptr_self;
	}
	static void ReleaseMe(){
		assert(GetInstance());
		delete GetInstance();
	}
};
template<typename Derived>
p_selectany Derived* Singleton<Derived, Singleton_Instance>::this_ptr_ = nullptr;
//////////////////////////////////////////////////////////////////////////
template<typename MutextT, typename LockerT>
class StaticLocker{
public:
	typedef MutextT MutextType;
	typedef LockerT LockerType;

public:
	StaticLocker() : locker_(singleton_mutex_){}
	StaticLocker(const StaticLocker&) = delete;
	StaticLocker& operator = (const StaticLocker&) = delete;

private:
	LockerType			locker_;

private:
	static MutextType	singleton_mutex_;
};
template<typename MutextT, typename LockerT>
p_selectany MutextT StaticLocker<MutextT, LockerT>::singleton_mutex_;

typedef StaticLocker<boost::mutex, boost::unique_lock<boost::mutex> > StaticMutexLocker;
//////////////////////////////////////////////////////////////////////////
