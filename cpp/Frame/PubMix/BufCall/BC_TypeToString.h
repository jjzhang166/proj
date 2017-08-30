#pragma once
/*
@author zhp
@date 2016/4/17 19:09
@purpose 通过类型得到字符串
*/
#include <string>
#include <cassert>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/size.hpp>
#include "BC_Base.h"

template <typename T>
struct BC_Type2String{
	static std::string get(){
		auto cls_proxy = bufcall::ClassProxyMgr::GetMe().FindClass(bufcall::Detail::BC_GetTypeId<T>());
		if (cls_proxy && cls_proxy->BC_GetClassName())
			return cls_proxy->BC_GetClassName();
		return "unknown type";
	}
};

template <typename T>
struct BC_Type2String<T*>{
	static const std::string& get(){
		static std::string str = BC_Type2String<T>::get() + "*";
		return str;
	}
};

template <typename T>
struct BC_Type2String<T&>{
	static const std::string& get(){
		static std::string str = BC_Type2String<T>::get() + "&";
		return str;
	}
};

template <typename T>
struct BC_Type2String<const T>{
	static const std::string& get(){
		static std::string str = "const " + BC_Type2String<T>::get();
		return str;
	}
};

#define BUFCALL_TYPE_TO_STRING(x) \
	template <> \
	struct BC_Type2String<x>{ \
		static const std::string& get(){ \
			static std::string str = #x;	\
			return str; \
		} \
	}
/**/
BUFCALL_TYPE_TO_STRING(void);

template <typename End>
std::string BC_FormatSignatureAux( bool, End, End ){
	return std::string();
}

template <typename Iter, class End>
std::string BC_FormatSignatureAux( bool first, Iter, End end ){
	using namespace boost;
	std::string res;
	if (!first)
		res.push_back(',');
	res += BC_Type2String<typename Iter::type>::get();
	res += BC_FormatSignatureAux(false, typename mpl::next<Iter>::type(), end);
	return res;
}

template <typename Signature>
std::string BC_FormatSignature( char const* function, Signature ){
	using namespace boost;
	typedef typename mpl::begin<Signature>::type first;

	std::string res = BC_Type2String<typename first::type>::get();
	if (function)
	{
		res.push_back(' ');
		res += function;
	}
	res.push_back('(');
	res += BC_FormatSignatureAux(true, typename mpl::next<first>::type(), typename mpl::end<Signature>::type());
	res.push_back(')');
	return res;
}