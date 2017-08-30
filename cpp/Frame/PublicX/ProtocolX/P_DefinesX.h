#pragma once
/*
@author zhp
@date 2016/1/27 16:25
@purpose 一些通用定义
pc->protocol call
pd->protocol dll
*/
#include "P_BaseTypeX.h"
#include <string>
#include <functional>
#include <boost/optional/optional.hpp>
#include <boost/preprocessor/cat.hpp>

//////////////////////////////////////////////////////////////////////////
typedef std::string StdString;
typedef std::wstring StdWstring;
typedef boost::optional<bool> OptionalBool;
typedef std::function<void()> FunctionVoid;
typedef std::function<bool()> FunctionBool;
typedef void(__stdcall* SleepFunctionT)(unsigned long);
//////////////////////////////////////////////////////////////////////////
//length of one uuid string
const pt_dword kUuidStrLen = 36;
const pt_dword kUuidStrLenEx = 32;

const pt_csz kLpkFileMapNameS = "F4B5FC53-BAD3-44EA-A18C-5728C2F188FA";
//////////////////////////////////////////////////////////////////////////
const OptionalBool kOptionalBoolNil;
const OptionalBool kOptionalTrue;
const OptionalBool kOptionalFalse;
//空字符串
const char* const kEmptyCStr = "";
const StdString kEmptyStr;
const StdWstring kEmptyWStr;

//typedef FunctionVoid f;
const FunctionVoid kFunc_DoNothing = [](){};
const FunctionBool kFunc_RetTrue = [](){ return true; };
const FunctionBool kFunc_RetFalse = [](){ return false; };
//////////////////////////////////////////////////////////////////////////
#define pc_std		__stdcall
//c调用约定
#define pc_cdecl	__cdecl
#define pc_this		__thiscall
#define pc_fast		__fastcall
//从左向右传递参数，通过EAX返回，堆栈由被调用者清除
#define pc_pascal	__pascal
#define pc_naked	_declspec(naked)
/**/
#define pd_import	__declspec(dllimport)
#define pd_export	__declspec(dllexport)
#define pd_export_c extern "C" __declspec(dllexport)
/**/
#define p_line		__LINE__
#define p_supper	__super
#define p_funcsig	__FUNCSIG__
/**/
#define p_selectany __declspec(selectany)
/**/
#define p_ostm_to_num(type)	\
inline std::ostream& operator << (std::ostream& ostm, const type& v){	\
	return ostm << (int)v;	\
}
/**/
#define P_CAST dynamic_cast
/**/
//得到名字
#define P_RUNTIME_NAME(v) (#v)
/**/
//成员变量点位符
#define p_mem_placeholder(size)	pt_byte	BOOST_PP_CAT(_ukn_, p_line)[size]
/**/
#define CMN_CLONE_IMPL(DerivedT) \
	DerivedT* Clone() const override{	\
	return new DerivedT(*this);	\
}	\
/**/
#define CMN_CAST_TO	\
	template<typename T>	\
	const T* CastTo() const{	\
		return static_cast<const T*>(this);	\
	}	\
	template<typename T>	\
	T* CastTo(){	\
		return static_cast<T*>(this);	\
	}	\
	template<typename T>	\
	const T& CastAs() const{	\
		return *static_cast<const T*>(this);	\
	}	\
	template<typename T>	\
	T& CastAs(){	\
		return *static_cast<T*>(this);	\
	}
/**/
//////////////////////////////////////////////////////////////////////////
/*
* VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
* 0x40 : unassigned
* VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
*/
/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
//////////////////////////////////////////////////////////////////////////