#pragma once
/*
@author zhp
@date 2016/11/4 3:06
@purpose for Cmn_LuaOper
*/
#include <luabind/luabind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/insert.hpp>
#include "Cmn_Exception.h"
#include <xtr1common>


template<typename... Args>
struct tagLCall;
struct tagLC_NoThrow;

namespace Detail{
	template<typename R>
	struct CallImpl{
		template<typename... Args>
		static R Call(const luabind::object& lua_func, const Args&... args){
			return luabind::call_function<R>(lua_func, args...);
		}
	};
	template<>
	struct CallImpl<void>{
		template<typename... Args>
		static void Call(const luabind::object& lua_func, const Args&... args){
			luabind::call_function<void>(lua_func, args...);
		}
	};

	template<bool, typename SetT>
	struct tagLC_NoThrowImpl{
		typedef SetT ThinSetT;
		//会抛出异常
		template<typename R, typename... Args>
		static R Call(const luabind::object& lua_func, const Args&... args){
			try{
				return CallImpl<R>::Call(lua_func, args...);
			} DO_CATCH_ALL_EXCEPTION({
				//这里还必须得用HandleDelayException处理一下。
				CmnExceptionBase::HandleDelayException();
			})
			assert(false);
			return R();
		}
	};
	template<typename SetT>
	struct tagLC_NoThrowImpl<true, SetT>{
		typedef typename boost::mpl::erase_key<SetT, tagLC_NoThrow>::type ThinSetT;
		//不会抛出异常
		template<typename R, typename... Args>
		static R Call(const luabind::object& lua_func, const Args&... args){
			try{
				auto lua_state = lua_func.interpreter();
				auto top_index_old = ::lua_gettop(lua_state);
				BOOST_SCOPE_EXIT_ALL(top_index_old, lua_state){
					::lua_settop(lua_state, top_index_old);
				};
				return tagLC_NoThrowImpl<false, SetT>::Call<R>(lua_func, args...);
			} DO_CATCH_THE_EXCEPTION(const CmnExceptionBase& e, {
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "调用Lua函数时出现自定义错误：" << e.what();
			}) DO_CATCH_THE_EXCEPTION(const std::exception& e, {
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "调用Lua函数时出现标准错误：" << e.what();
			}) DO_CATCH_ALL_EXCEPTION({
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "调用Lua函数时出现未知错误。";
			})
			return R();
		}
	};

	template<typename T>
	struct tagLC_SetHolder{
		typedef T SetT;
	};

	template<typename SetHolder, int SetSize>
	struct tagLC_TagHolder{
		typedef typename SetHolder::SetT SetT;
		typedef typename boost::mpl::has_key<SetT, tagLC_NoThrow>::type HasNoThrowT;

		typedef tagLC_NoThrowImpl<HasNoThrowT::value, SetT> NoThrow;
		typedef typename NoThrow::ThinSetT ThinSet1;
		static_assert(boost::mpl::size<ThinSet1>::value == 1, "must has only one return type");
		typedef typename boost::mpl::begin<ThinSet1>::type IterType;
		typedef typename boost::mpl::deref<IterType>::type ResType;
	};
	template<typename SetHolder>
	struct tagLC_TagHolder<SetHolder, 0>{
		typedef typename SetHolder::SetT SetT;
	};

	template<typename T>
	struct stLuaTagHlp{
		typedef typename boost::mpl::set<T> SetT;
		typedef tagLC_NoThrowImpl<false, void> NoThrow;
		typedef T ResType;
	};
	template<>
	struct stLuaTagHlp<tagLC_NoThrow>{
		typedef boost::mpl::set<tagLC_NoThrow> SetT;
		typedef tagLC_NoThrowImpl<true, SetT> NoThrow;
		typedef void ResType;
	};
	template<typename T>
	struct stLuaTagHlp<tagLC_SetHolder<T> > : tagLC_TagHolder<tagLC_SetHolder<T>, 
		boost::mpl::size<typename tagLC_SetHolder<T>::SetT>::value>{
	};
	template<typename... Args>
	struct stLuaTagHlp<tagLCall<Args...> > : stLuaTagHlp<typename tagLCall<Args...>::SetHolder>{};


	template<typename T>
	struct EnsureStringImpl{
		typedef T type;
	};
	template<int Idx>
	struct EnsureStringImpl<char[Idx]>{
		typedef const char* type;
	};
	template<typename T>
	struct EnsureString{
		typedef typename EnsureStringImpl<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::type type;
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	struct ToLuaStringTraits{
		typedef const T& type;
	};
	template<int SizeN>
	struct ToLuaStringTraits<char[SizeN]>{
		typedef const char* type;
	};
	template<typename T>
	void ToLuaStringImpl(std::string& out, const T& v){
		out.append(std::to_string(v));
	}
	void ToLuaStringImpl(std::string& out, const std::string& v);
	void ToLuaStringImpl(std::string& out, const char* v);
	void ToLuaStringImpl(std::string& out, const bool& v);
	template<int ArgN>
	struct ToLuaStringIdx{
		template<typename FirstT, typename... TailsT>
		static void get(std::string& out, const char* sep, const FirstT& arg1, const TailsT&... tails){
			typedef typename std::remove_cv<typename std::remove_reference<FirstT>::type>::type type;
			typedef typename ToLuaStringTraits<type>::type Type;
			ToLuaStringImpl(out, static_cast<Type>(arg1));
			out.append(sep);
			ToLuaStringIdx<sizeof...(TailsT)>::get(out, sep, tails...);
		}
	};
	template<>
	struct ToLuaStringIdx<1>{
		template<typename FirstT>
		static void get(std::string& out, const char*, const FirstT& arg){
			typedef typename std::remove_cv<typename std::remove_reference<FirstT>::type>::type type;
			typedef typename ToLuaStringTraits<type>::type Type;
			ToLuaStringImpl(out, static_cast<Type>(arg));
		}
	};
	template<>
	struct ToLuaStringIdx<0>{
		static void get(std::string&, const char*){}
	};
	template<typename... Args>
	void ToLuaString(std::string& out, const char* sep, const Args&... args){
		assert(sep);		
		return ToLuaStringIdx<sizeof...(Args)>::get(out, sep, args...);;
	}
	//////////////////////////////////////////////////////////////////////////
}
