#pragma once
/*
@author zhp
@date 2016/4/14 18:31
@purpose some base type define
pt->protocol type
*/
#include <cassert>

typedef char						pt_char;
typedef const char					pt_cchar;
typedef wchar_t						pt_wchar;
typedef const wchar_t				pt_cwchar;
typedef unsigned char				pt_byte;
typedef const unsigned char			pt_cbyte;
typedef char*						pt_sz;
typedef wchar_t*					pt_wsz;
typedef unsigned char*				pt_pbyte;
typedef const char*					pt_csz;
typedef const wchar_t*				pt_cwsz;
typedef const unsigned char*		pt_pcbyte;
typedef bool						pt_bool;
typedef const bool					pt_cbool;
typedef bool*						pt_pbool;
typedef const bool*					pt_pcbool;
static_assert(sizeof(pt_bool) == 1, "");
static_assert(sizeof(pt_char) == 1, "");

typedef short						pt_short;
typedef const short					pt_cshort;
typedef unsigned short				pt_word;
typedef const unsigned short		pt_cword;
typedef short*						pt_pshort;
typedef unsigned short*				pt_pword;
typedef const short*				pt_pcshort;
typedef const unsigned short*		pt_pcword;
static_assert(sizeof(pt_short) == 2, "");

typedef int							pt_int;
typedef const int					pt_cint;
typedef unsigned int				pt_dword;
typedef const unsigned int			pt_cdword;
typedef int*						pt_pint;
typedef unsigned int*				pt_pdword;
typedef const int*					pt_pcint;
typedef const unsigned int*			pt_pcdword;
static_assert(sizeof(pt_int) == 4, "");

typedef long						pt_long;
typedef const long					pt_clong;
typedef unsigned long				pt_ulong;
typedef const unsigned long			pt_culong;
typedef long*						pt_plong;
typedef unsigned long*				pt_pulong;
typedef const long*					pt_pclong;
typedef const unsigned long*		pt_pculong;
static_assert(sizeof(pt_long) == 4, "");

typedef float						pt_float;
typedef const float					pt_cfloat;
typedef float*						pt_pfloat;
typedef const float*				pt_pcfloat;
static_assert(sizeof(pt_float) == 4, "");

typedef void						pt_void;
typedef void*						pt_pvoid;
typedef const void*					pt_pcvoid;
static_assert(sizeof(pt_pvoid) == 4, "");

typedef long long					pt_dint;
typedef const long long				pt_cdint;
typedef unsigned long long			pt_qword;
typedef const unsigned long long	pt_cqword;
typedef long long*					pt_pdint;
typedef unsigned long long*			pt_pqword;
typedef const long long*			pt_pcdint;
typedef const unsigned long long*	pt_pcqword;
static_assert(sizeof(pt_dint) == 8, "");

typedef double						pt_double;
typedef const double				pt_cdouble;
typedef double*						pt_pdouble;
typedef const double*				pt_pcdouble;
static_assert(sizeof(double) == 8, "");

typedef long double					pt_ldouble;
typedef const long double			pt_cldouble;
typedef long double*				pt_pldouble;
typedef const long double*			pt_pcldouble;
static_assert(sizeof(pt_ldouble) == 8, "");

//////////////////////////////////////////////////////////////////////////
template<int TypeSize>
struct pt_as_size{};

template<>
struct pt_as_size<1>{
	typedef pt_char type;
};

template<>
struct pt_as_size<2>{
	typedef pt_short type;
};

template<>
struct pt_as_size<4>{
	typedef pt_int type;
};

template<>
struct pt_as_size<8>{
	typedef pt_dint type;
};
//////////////////////////////////////////////////////////////////////////
namespace Detail{
	template<pt_dint Value_, bool IsNegative>
	struct _GetAbsConstValue_{
		static const auto value = Value_;
	};
	template<pt_dint Value_>
	struct _GetAbsConstValue_<Value_, true>{
		static const auto value = -Value_;
	};
	
	template<pt_dint Value_, bool IsBigger>
	struct _TryAsInt_{
		typedef pt_int type;
	};
	template<pt_dint Value_>
	struct _TryAsInt_<Value_, true>{
		typedef pt_dint type;
	};

	template<pt_dint Value_, bool IsBigger>
	struct _TryAsShort_{
		typedef pt_short type;
	};
	template<pt_dint Value_>
	struct _TryAsShort_<Value_, true>{
		typedef typename _TryAsInt_<Value_, (Value_ > (pt_dword)-1)>::type type;
	};
	
	template<pt_dint Value_, bool IsBigger>
	struct _TryAsChar_{
		typedef pt_char type;
	};
	template<pt_dint Value_>
	struct _TryAsChar_<Value_, true>{
		typedef typename _TryAsShort_<Value_, (Value_ > (pt_word)-1)>::type type;
	};
}

template<pt_dint Value_>
struct pt_as_value{
	static const auto abs_value = Detail::_GetAbsConstValue_<Value_, (Value_ < 0)>::value;
	typedef typename Detail::_TryAsChar_<abs_value, (abs_value > (pt_byte)-1)>::type type;
};
//////////////////////////////////////////////////////////////////////////
template<typename DstT, typename SrcT>
inline DstT OffsetAs(SrcT addr, pt_int offset){
	assert(addr);
	return *(const DstT*)(reinterpret_cast<const pt_byte*>(addr) + offset);
}
template<typename SrcT>
inline pt_char AsChar(SrcT addr, pt_int offset){
	return OffsetAs<pt_char>(addr, offset);
}
template<typename SrcT>
inline pt_byte AsByte(SrcT addr, pt_int offset){
	return OffsetAs<pt_byte>(addr, offset);
}
template<typename SrcT>
inline pt_wchar AsWchar(SrcT addr, pt_int offset){
	return OffsetAs<pt_wchar>(addr, offset);
}
template<typename SrcT>
inline pt_csz AsCsz(SrcT addr, pt_int offset){
	return OffsetAs<pt_csz>(addr, offset);
}
template<typename SrcT>
inline pt_cwsz AsCwsz(SrcT addr, pt_int offset){
	return OffsetAs<pt_cwsz>(addr, offset);
}
template<typename SrcT>
inline pt_bool AsBool(SrcT addr, pt_int offset){
	return OffsetAs<pt_bool>(addr, offset);
}
template<typename SrcT>
inline pt_short AsShort(SrcT addr, pt_int offset){
	return OffsetAs<pt_short>(addr, offset);
}
template<typename SrcT>
inline pt_word AsWord(SrcT addr, pt_int offset){
	return OffsetAs<pt_word>(addr, offset);
}
template<typename SrcT>
inline pt_int AsInt(SrcT addr, pt_int offset){
	return OffsetAs<pt_int>(addr, offset);
}
template<typename SrcT>
inline pt_dword AsDword(SrcT addr, pt_int offset){
	return OffsetAs<pt_dword>(addr, offset);
}
template<typename SrcT>
inline pt_long AsLong(SrcT addr, pt_int offset){
	return OffsetAs<pt_long>(addr, offset);
}
template<typename SrcT>
inline pt_ulong AsUlong(SrcT addr, pt_int offset){
	return OffsetAs<pt_ulong>(addr, offset);
}
template<typename SrcT>
inline pt_float AsFloat(SrcT addr, pt_int offset){
	return OffsetAs<pt_float>(addr, offset);
}
template<typename SrcT>
inline pt_dint AsDint(SrcT addr, pt_int offset){
	return OffsetAs<pt_dint>(addr, offset);
}
template<typename SrcT>
inline pt_qword AsQword(SrcT addr, pt_int offset){
	return OffsetAs<pt_qword>(addr, offset);
}
template<typename SrcT>
inline pt_double AsDouble(SrcT addr, pt_int offset){
	return OffsetAs<pt_double>(addr, offset);
}
template<typename SrcT>
inline pt_ldouble AsLdouble(SrcT addr, pt_int offset){
	return OffsetAs<pt_ldouble>(addr, offset);
}
//////////////////////////////////////////////////////////////////////////
