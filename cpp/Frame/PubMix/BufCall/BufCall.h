#pragma once
/*
@author zhp
@date 2016/4/17 18:38
@purpose 可以从一个缓冲区来调用函数
*/
#include "BC_Stream.h"
#include "BC_Function.h"
#include "BC_Size.h"
#include <vector>
#include "Detail/BC_Class.h"
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
//发包缓冲区对象
BC_AsStreamW BC_MakeP( pt_csz function_name );
//供发包用，容器参数
template<typename ContT>
BC_Cont<const ContT&> BC_MakeCont( const ContT& cont );
//供发包用，解决数组和字符串的二义性
template<typename T>
inline auto BC_MakeArray(const T& v) -> decltype(Detail::BC_MakeArrayImpl<T>::get(v)){
	return Detail::BC_MakeArrayImpl<T>::get(v);
}
//供收包中的参数用
template<typename ContT>
struct BC_Cont;
/*
@remark：std::array<T, size>只可当作数组使用，即它的元素不能是字符串、tuple等可变长度类型。编译期所占内存与运行时所占内存一致。
std::array<T, size>也是一种容器，类似BC_Cont。
BC_Cont中的元素可以自由组合。
tuple最多有CMN_ARITY_MAX+1个参数，通过BC_MakeFunction添加的函数最多有CMN_ARITY_MAX+1个参数。
默认支持std::vector<T, AllocT>，即使用vector，不必用BC_Cont<std::vector<T, AllocT> >的形式。
支持所有标准库容器，但非vector容器，必须得用BC_Cont<ContainerType>的形式包装一下。也支持自定义容器，要求容器必须有value_type的类型声明，也必须有push_back操作。
默认直接支持的复杂类型有std::bitset、std::array、std::vector、std::tuple、std::pair、std::basic_string、ATL::CStringT、Json::Value、DateTimeLocal、数组、宽/窄字符串常量。
默认支持的简单类型为C++语言内建类型，并分为有符号与无符号；支持任意枚举类型。
对于参数，尽量用常量引用的形式去声明，这样可以减少不必要的复制操作，尤其是vector。
均可用const char*或const wchar_t*的形式去接收std::basic_string、ATL::CStringT对象。
不管是字符串常量，还是const char[]/const wchar_t[]的字符数组形式，均被解释成字符串，所以若想被解释成数组，最好用std::array或用BC_MakeArray包装一下。
注册类和成员函数需要用到这两个宏：BC_REG_CLS_DECLARE、BC_REG_CLS_DEFINE。
支持POD数据类型，需要通过BC_DEFINE_POD来声明POD结构体。
支持自定义类型，该类型需要有这样的成员函数：/ *
template<class Archive>
void serialize(Archive & ar){
	ar
	& member1
	& member2
	& member3;
}
* /
若没有serialize成员函数，则需要operator <<和operator >>操作符。
支持验证长度，用这个宏：BC_CHECK_SIZE。也支持自定义验证，去参考BC_Check_Size的实现。
*/
//////////////////////////////////////////////////////////////////////////
#define BC_DEFINE_POD(struct_name)	\
	struct struct_name;	\
	BUFCALL_TYPE_TO_STRING(struct_name);	\
	template<typename Policy>	\
	struct BC_TypeConverter_0<struct_name, Policy> : BC_TypeConverter_POD_Base<struct_name, Policy>{};	\
	struct struct_name
/**/
#define BC_REG_CLS_DECLARE(cls) private:	\
static void _Fn_Reg_##cls##_();	\
static const bufcall::BC_RegClassImpl g_reg_##cls##_
/**/
//注册成员函数
#define BC_REG_CLS_DEFINE(cls) const bufcall::BC_RegClassImpl cls::g_reg_##cls##_ = &cls::_Fn_Reg_##cls##_;	\
void cls::_Fn_Reg_##cls##_()
/**/
//验证长度，可验证的类型：字符串、std::basic_string、ATL::CStringT、std::vector、BC_Cont<ContT>
#define BC_CHECK_SIZE(cls, max_size, min_size)	BC_Check<BC_Check_Size<cls, max_size, min_size> >
/**/

/*此类单独用，或可当作加密/解密使用*/
class BC_BufDecode : public BC_BufClassRuntimeInfo{
public:
	BC_BufDecode();

public:
	bool Decode( BC_AsStreamR& call_info ) const;
	bool Decode( pt_pcvoid buffer, pt_dword buffer_size ) const;

	//扩展者去重载
protected:
	//可能会抛出异常
	virtual bool OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const;

private:
	mutable boost::optional<const bufcall::ClassProxy*>	cls_proxy_;
};

//不会抛出异常
class BC_BufCallBase : public BC_BufDecode{
public:
	//远程调用
	virtual bool ToCall( const BC_AsStreamW& stm ) = 0;
	//同步远程调用
	virtual bool ToCallSync(const BC_AsStreamW& stm, boost::optional<bool>& oper_res);

protected:
	//不会抛出异常
	virtual bool OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const override;
};

//会抛出异常
class BC_BufCallBase_Except : public BC_BufCallBase{
private:
	//会抛出异常
	bool OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const override;
};