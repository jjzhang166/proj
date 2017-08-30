#pragma once
/*
@author zhp
@date 2013-12-30
@purpose memory operate
*/
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <string>
#include <atlstr.h>
#include "Cmn_LogOut.h"
#include "Cmn_Exception.h"
#include "Cmn_UsefulFunc.h"
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>
#include <CommonX/CmnX_UsefulFunc.h>

//数据索引层次
#define MemOper_DataIndex_Max 5
//函数调用
//stdcall
class Oper_StdCall{};
//cdecl
class Oper_Cdecl{};
//thiscall
class Oper_ThisCall{};
//虚函数调用
template<unsigned int vtbl_offset>
struct MemOper_DoVtbl;

class MemOperBase{
public:
	void swap( MemOperBase& rhs ){
		std::swap(addr_, rhs.addr_);
	}
	void PlusValue( int v ){
		addr_ = (void*)((int)addr_ + v);
	}

protected:
	MemOperBase( void* addr );
	MemOperBase();

private:
	void Call();

public:
	void*		addr_;
};

//call function do something else
//like a sentry
struct MemOper_DoElse{
	MemOper_DoElse();
	~MemOper_DoElse();
	//地址转换，调用地址的索引
	void* RetrieveAddr( const MemOperBase* mem_oper_base );
};
//call function lock
template<int CallAddr>
struct MemOper_Lock : MemOper_DoElse{
	MemOper_Lock(){
		if ( !src_thread_handle_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << "src_thread_handle_为空，小心多线程问题哦";
			return;
		}
		assert(GetCurrentThreadId() != src_thread_id_);
		::SuspendThread(src_thread_handle_);
	}
	~MemOper_Lock(){
		if (src_thread_handle_)
			::ResumeThread(src_thread_handle_);
	}
	//调用某个call的源线程
	static void SetSrcThread( DWORD src_thread_id ){
		src_thread_handle_ = ::OpenThread(THREAD_SUSPEND_RESUME, FALSE, src_thread_id);
		assert(src_thread_handle_);
		if ( !src_thread_handle_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << "打开线程失败";
		}
#ifdef _DEBUG
		else
			src_thread_id_ = src_thread_id;
#endif
	}
	static void SetCurThreadAsSrc(){
		if (src_thread_handle_)
			return;
		SetSrcThread(::GetCurrentThreadId());
	}
	static HANDLE		src_thread_handle_;
#ifdef _DEBUG
	static DWORD		src_thread_id_;
#endif
};

#ifdef _DEBUG
template<int CallAddr>
p_selectany DWORD MemOper_Lock<CallAddr>::src_thread_id_ = 0;
#endif

template<int CallAddr>
p_selectany HANDLE MemOper_Lock<CallAddr>::src_thread_handle_ = nullptr;

//地址转换，支持对全局变量里保存的地址的调用
struct MemOper_RetrieveAddr : MemOper_DoElse{
	void* RetrieveAddr( const MemOperBase* mem_oper_base );
};
template<typename T, typename DoElse = MemOper_DoElse>
class MemOper;
//捕获异常
#define MemOper_TRY_BEGIN_	try{
#define MemOper_TRY_END_	}	\
	DO_CATCH_THE_EXCEPTION( const std::exception& e, {	\
		assert(false);	\
		LOG_ENTER_FUNC;	\
		LOG_O(Log_fatal) << e.what();	\
	})	\
	DO_CATCH_ALL_EXCEPTION({	\
		assert(false);	\
		LOG_ENTER_FUNC;	\
		LOG_O(Log_fatal) << "调用函数出现了未知错误";	\
	})	\
/**/
template<typename DoElse>
class MemOper<Oper_StdCall, DoElse> : public MemOperBase{
public:
	template<typename T>
	MemOper(T p) : MemOperBase((void*)p){}
	
	template<typename R, typename... Args>
	R Call(const Args... args) const
	{
		MemOper_TRY_BEGIN_
		/*stdcall*/
		DoElse do_else;
		typedef R (_stdcall *FnType)(const Args...);
		FnType fn = reinterpret_cast<FnType>(do_else.RetrieveAddr(this));
		return fn(args...);
		MemOper_TRY_END_
		return R();
	}
	/**/
};

template<typename DoElse>
class MemOper<Oper_Cdecl, DoElse> : public MemOperBase{
public:
	template<typename T>
	MemOper(T p) : MemOperBase((void*)p){}

	template<typename R, typename... Args>
	R Call(const Args... args) const
	{
		MemOper_TRY_BEGIN_
		/*cdecl*/
		DoElse do_else;
		typedef R (_cdecl *FnType)(const Args...);
		FnType fn = reinterpret_cast<FnType>(do_else.RetrieveAddr(this));
		return fn(args...);
		MemOper_TRY_END_
		return R();
	}
	/**/
};

template<typename DoElse>
class MemOper<Oper_ThisCall, DoElse> : public MemOperBase{
public:
	template<typename T>
	MemOper(T p) : MemOperBase((void*)p){}

	template<typename R, typename This, typename... Args>
	R Call(This p_this, const Args... args) const
	{
		static_assert(std::is_pointer<This>::value == true, "");
		if (IsUnValidPtr(p_this))
		{
			assert(false);
			return R();
		}
		MemOper_TRY_BEGIN_
		/*thiscall*/
		DoElse do_else;
		typedef R (__thiscall *FnType)(This, const Args...);
		FnType fn = reinterpret_cast<FnType>(do_else.RetrieveAddr(this));
		return fn(p_this, args...);
		MemOper_TRY_END_
		return R();
	}
	/**/
};

template<unsigned int vtbl_offset>
struct MemOper_DoVtbl{
	static_assert(vtbl_offset % sizeof(void*) == 0, "");
	static_assert(vtbl_offset < 0x1000 * sizeof(void*), "");

	static const unsigned int	vtbl_idx = vtbl_offset / sizeof(void*);

	static void* GetFn(const void* obj){
		if (IsUnValidPtr(obj))
		{
			assert(false);
			return nullptr;
		}
		obj = (*(void**)obj);
		if (IsUnValidPtr(obj))
		{
			assert(false);
			return nullptr;
		}
		return *((void**)obj + vtbl_idx);
	}
};
template<unsigned int vtbl_offset>
class MemOper<Oper_ThisCall, MemOper_DoVtbl<vtbl_offset> >{
public:
	template<typename R, typename This, typename... Args>
	static R Call(This p_this, const Args... args)
	{
		static_assert(std::is_pointer<This>::value == true, "");
		MemOper_TRY_BEGIN_
		/*thiscall*/
		typedef R(__thiscall *FnType)(This, const Args...);
		FnType fn = reinterpret_cast<FnType>(MemOper_DoVtbl<vtbl_offset>::GetFn(p_this));
		return fn(p_this, args...);
		MemOper_TRY_END_
		return R();
	}
	/**/
};

template<typename T, typename DoElse>
class MemOper : public MemOperBase{
public:
	template<typename U>
	explicit MemOper(U p) : MemOperBase((void*)p){}
	MemOper() : MemOperBase(){}
	template<typename U>
	MemOper& operator = ( U p ){
		MemOper tmp(p);
		swap(tmp);
		return *this;
	}
	operator const T&() const{
		return *reinterpret_cast<T*>(addr_);
	}
	operator T&(){
		return *reinterpret_cast<T*>(addr_);
	}
	//get specify type addrress
	T* get(){
		return reinterpret_cast<T*>(addr_);
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename T, typename DoElse, typename U>
MemOper<T, DoElse> MakeMemOper(T, DoElse, U addr){
	return MemOper<T, DoElse>(addr);
}

template<typename T, typename U>
MemOper<T, MemOper_DoElse> MakeMemOper(T, U addr){
	return MemOper<T, MemOper_DoElse>(addr);
}

template<unsigned int vtbl_offset>
MemOper<Oper_ThisCall, MemOper_DoVtbl<vtbl_offset> > MakeMemOper(){
	return MemOper<Oper_ThisCall, MemOper_DoVtbl<vtbl_offset> >();
}
//////////////////////////////////////////////////////////////////////////

void* MemOperApplyHelper__( unsigned int, const int*, int );
template<typename T, pt_dword N_>
bool MemOperApply( MemOper<T>& mem_output, unsigned int addr_base, const int (&data_offset)[N_] ){
	auto res = MemOperApplyHelper__(addr_base, data_offset, N_);
	if ( !res )
		return false;
	mem_output.addr_ = res;
	return true;
}
template<typename T, pt_dword N_>
bool MemOperApplyR( std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str_,
	unsigned int addr_base, const int (&data_offset)[N_] ){
		MemOper<T*> mem_data(0);
		if ( !MemOperApply(mem_data, addr_base, data_offset) )
			return false;
		str_ = mem_data;
		return true;
}
template<typename T, pt_dword N_>
bool MemOperApplyR( CStringT<T, StrTraitATL<T, ChTraitsCRT<T> > >& str_,
	unsigned int addr_base, const int (&data_offset)[N_] ){
		MemOper<T*> mem_data(0);
		if ( !MemOperApply(mem_data, addr_base, data_offset) )
			return false;
		str_ = mem_data;
		return true;
}
template<typename T, pt_dword N_>
bool MemOperApplyR( T& data, const unsigned int addr_base, const int (&data_offset)[N_] ){
	MemOper<T> mem_data(0);
	if ( !MemOperApply(mem_data, addr_base, data_offset) )
		return false;
	data = mem_data;
	return true;
}
template<typename T, int N_>
bool MemOperApplyW( T data, unsigned int addr_base, const int (&data_offset)[N_] ){
	MemOper<T> mem_data(0);
	if ( !MemOperApply(mem_data, addr_base, data_offset) )
		return false;
	static_cast<T&>(mem_data) = data;
	return true;
}
template<typename T, pt_dword N_>
bool MemOperGetAddr( T& data, const unsigned int addr_base, const int (&data_offset)[N_] ){
	MemOper<T> mem_data(0);
	if ( !MemOperApply(mem_data, addr_base, data_offset) )
		return false;
	data = (T)(void*)mem_data.get();
	return true;
}

#define MemOperApply_Equal
#define MemOperApply_NotEqual	\
	addr_base = *(unsigned int*)(void*)mem_helper;	\
	if (IsUnValidPtr(addr_base))	\
		return false;	\
	mem_helper = (char*)(void*)addr_base;	\
	/**/
#define MemOperApply_Internal_RepeatM(z, n, max_n)	\
	mem_helper = mem_helper + data_offset_##n;	\
	BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_INC(n), max_n), MemOperApply_Equal, MemOperApply_NotEqual)	\
	/**/
//多级数据索引
#define MemOperApply_RepeatM(z, n, _)		\
template<typename T>	\
bool MemOperApply( MemOper<T>& mem_output, unsigned int addr_base BOOST_PP_COMMA_IF(n)	\
				  BOOST_PP_ENUM_PARAMS(n, int data_offset_) )	\
{	\
	if (IsUnValidPtr(addr_base))	\
		return false;	\
	char* mem_helper = (char*)(void*)addr_base;	\
	BOOST_PP_REPEAT(n, MemOperApply_Internal_RepeatM, n)	\
	mem_output.addr_ = (void*)mem_helper;	\
	return true;	\
}	\
template<typename T>	\
bool MemOperApplyR( std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str_,		\
		unsigned int addr_base BOOST_PP_COMMA_IF(n)	\
	BOOST_PP_ENUM_PARAMS(n, int data_offset_) )	\
{	\
	MemOper<T*> mem_data(0);	\
	if ( !MemOperApply(mem_data, addr_base BOOST_PP_COMMA_IF(n)	\
	BOOST_PP_ENUM_PARAMS(n, data_offset_)) )	\
	return false;	\
	str_ = mem_data;	\
	return true;	\
}	\
template<typename T>	\
bool MemOperApplyR( CStringT<T, StrTraitATL<T, ChTraitsCRT<T> > >& str_,		\
		unsigned int addr_base BOOST_PP_COMMA_IF(n)	\
	BOOST_PP_ENUM_PARAMS(n, int data_offset_) )	\
{	\
	MemOper<T*> mem_data(0);	\
	if ( !MemOperApply(mem_data, addr_base BOOST_PP_COMMA_IF(n)	\
	BOOST_PP_ENUM_PARAMS(n, data_offset_)) )	\
	return false;	\
	str_ = mem_data;	\
	return true;	\
}	\
template<typename T>	\
bool MemOperApplyR( T& data, unsigned int addr_base BOOST_PP_COMMA_IF(n)	\
				   BOOST_PP_ENUM_PARAMS(n, int data_offset_) )	\
{	\
	MemOper<T> mem_data(0);	\
	if ( !MemOperApply(mem_data, addr_base BOOST_PP_COMMA_IF(n)	\
		BOOST_PP_ENUM_PARAMS(n, data_offset_)) )	\
		return false;	\
	data = mem_data;	\
	return true;	\
}	\
template<typename T>	\
bool MemOperApplyW( T data, unsigned int addr_base BOOST_PP_COMMA_IF(n)	\
				BOOST_PP_ENUM_PARAMS(n, int data_offset_) )	\
{	\
	MemOper<T> mem_data(0);	\
	if ( !MemOperApply(mem_data, addr_base BOOST_PP_COMMA_IF(n)	\
	BOOST_PP_ENUM_PARAMS(n, data_offset_)) )	\
	return false;	\
	static_cast<T&>(mem_data) = data;	\
	return true;	\
}	\
/**/
BOOST_PP_REPEAT(MemOper_DataIndex_Max, MemOperApply_RepeatM, _)

#undef MemOper_TRY_BEGIN_
#undef MemOper_TRY_END_
#undef MemOperApply_RepeatM
#undef MemOperApply_Internal_RepeatM
#undef MemOperApply_NotEqual
#undef MemOperApply_Equal
#undef MemOper_DataIndex_Max