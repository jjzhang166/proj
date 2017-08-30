#pragma once
/*
@author zhp
@date 2016/2/9 21:19
@purpose for lock
*/
#include <Windows.h>
#include <string>
#include <memory>

class Cmnx_MutexBase{
private:
	Cmnx_MutexBase( const Cmnx_MutexBase& );
	void operator=( const Cmnx_MutexBase& );

public:
	Cmnx_MutexBase();
	virtual ~Cmnx_MutexBase();
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class Cmnx_Lockable{
public:
	explicit Cmnx_Lockable( Cmnx_MutexBase& the_mutex, bool is_auto_lock = true );
	~Cmnx_Lockable();

private:
	Cmnx_MutexBase&		mutex_;
};

class Cmnx_MutexProxy : public Cmnx_MutexBase{
public:
	explicit Cmnx_MutexProxy( Cmnx_MutexBase& impl );
	void lock();
	void unlock();

private:
	Cmnx_MutexBase&		impl_;
};

class Cmnx_MutexRefPtr : public Cmnx_MutexBase{
public:
	typedef std::shared_ptr<Cmnx_MutexBase> LockImplPtrT;

public:
	Cmnx_MutexRefPtr();
	explicit Cmnx_MutexRefPtr( const LockImplPtrT& impl );
	void SetLockImpl( const LockImplPtrT& impl );
	void lock();
	void unlock();

private:
	LockImplPtrT		impl_;
};

class Cmnx_NullMutex : public Cmnx_MutexBase{
public:
	void lock();
	void unlock();
};

class Cmnx_Mutex_CriticalSection : public Cmnx_MutexBase{
public:
	Cmnx_Mutex_CriticalSection();
	void lock();
	void unlock();

private:
	CRITICAL_SECTION	lock_impl_;
};

class Cmnx_Mutex_MutexA : public Cmnx_MutexBase{
public:
	explicit Cmnx_Mutex_MutexA( const char* lock_name );
	~Cmnx_Mutex_MutexA();
	void lock();
	void unlock();

private:
	HANDLE				the_lock_;
};