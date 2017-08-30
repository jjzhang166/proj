#include "stdafx.h"
#include <CommonX/CmnX_Lock.h>
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>

Cmnx_Mutex_CriticalSection::Cmnx_Mutex_CriticalSection()
{
	ZeroMemory(&lock_impl_, sizeof(lock_impl_));
	if ( !InitializeCriticalSectionAndSpinCount(&lock_impl_, 0) )
	{
		assert(false);
		return;
	}
}

void Cmnx_Mutex_CriticalSection::lock()
{
	::EnterCriticalSection(&lock_impl_);
}

void Cmnx_Mutex_CriticalSection::unlock()
{
	::LeaveCriticalSection(&lock_impl_);
}

Cmnx_MutexBase::~Cmnx_MutexBase()
{

}

Cmnx_MutexBase::Cmnx_MutexBase()
{

}

void Cmnx_NullMutex::lock()
{

}

void Cmnx_NullMutex::unlock()
{

}

Cmnx_Lockable::Cmnx_Lockable( Cmnx_MutexBase& the_mutex, bool is_auto_lock ) : mutex_(the_mutex)
{
	if (is_auto_lock)
		the_mutex.lock();
}

Cmnx_Lockable::~Cmnx_Lockable()
{
	mutex_.unlock();
}

Cmnx_Mutex_MutexA::Cmnx_Mutex_MutexA( const char* lock_name )
{
	if (lock_name)
	{
		std::string str_name = "Global\\";
		str_name += lock_name;
		the_lock_ = ::CreateMutexA(GetNullDACL(), FALSE, str_name.c_str());
	}
	else
		the_lock_ = ::CreateMutexA(GetNullDACL(), FALSE, nullptr);
	if ( !the_lock_ )
	{
		assert(false);
		return;
	}
}

void Cmnx_Mutex_MutexA::lock()
{
	DWORD dwRet = ::WaitForSingleObject(the_lock_, INFINITE);
	if ( dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED )
		return;
	assert(false);
}

void Cmnx_Mutex_MutexA::unlock()
{
	if (::ReleaseMutex(the_lock_))
		return;
	assert(false);
}

Cmnx_Mutex_MutexA::~Cmnx_Mutex_MutexA()
{
	if (the_lock_)
	{
		::CloseHandle(the_lock_);
		the_lock_ = nullptr;
	}
}

Cmnx_MutexProxy::Cmnx_MutexProxy( Cmnx_MutexBase& impl ) : impl_(impl)
{

}

void Cmnx_MutexProxy::lock()
{
	impl_.lock();
}

void Cmnx_MutexProxy::unlock()
{
	impl_.unlock();
}

Cmnx_MutexRefPtr::Cmnx_MutexRefPtr()
{

}

Cmnx_MutexRefPtr::Cmnx_MutexRefPtr( const LockImplPtrT& impl ) : impl_(impl)
{

}

void Cmnx_MutexRefPtr::SetLockImpl( const LockImplPtrT& impl )
{
	impl_ = impl;
}

void Cmnx_MutexRefPtr::lock()
{
	if ( !impl_ )
	{
		assert(false);
		return;
	}
	impl_->lock();
}

void Cmnx_MutexRefPtr::unlock()
{
	if ( !impl_ )
	{
		assert(false);
		return;
	}
	impl_->unlock();
}
