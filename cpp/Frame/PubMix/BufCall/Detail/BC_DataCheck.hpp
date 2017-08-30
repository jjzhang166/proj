#pragma once
/*
@author zhp
@date 2016/5/6 10:07
@purpose check data
*/
#include "../BC_DataConver.h"

//////////////////////////////////////////////////////////////////////////
template<typename CheckT>
struct BC_Check;

template<typename T, int MaxSize, int MinSize = 0>
struct BC_Check_Size;
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Policy>
struct BC_DataWrapper_CheckBase{
private:
	typedef T check_type;
	typedef typename check_type::type T_type_;
	typedef BC_DataWrapper<T_type_, Policy> WrapperImpl_;

public:
	typedef typename WrapperImpl_::type type;
	typedef typename WrapperImpl_::t_info t_info;
	typedef typename WrapperImpl_::type_tag type_tag;

	template<typename Stream>
	static check_type AsT(Stream& stm){
		check_type res;
		const auto& holder = WrapperImpl_::AsT(stm);
		if (!holder.Valid())
			return res;
		SetValueAux<T_type_>(res.value_, static_cast<typename BC_DataConverter<typename BC_RemoveTypeCVR<decltype(holder)>::type>::type>(holder));
		res.valid_ = true;
		return res;
	}
	template<typename Stream>
	static void Write(Stream& stm, const T_type_& value){
		WrapperImpl_::Write(stm, value);
	}
	template<typename Stream>
	static void WriteValue(Stream& stm, const T_type_& value){
		WrapperImpl_::WriteValue(stm, value);
	}
	template<typename Stream>
	static void Write(Stream& stm, const check_type& value){
		WrapperImpl_::Write(stm, value.value_);
	}
	template<typename Stream>
	static void WriteValue(Stream& stm, const check_type& value){
		WrapperImpl_::WriteValue(stm, value.value_);
	}
};
template<typename CheckT, typename Policy>
struct BC_DataWrapper<BC_Check<CheckT>, Policy> : BC_DataWrapper_CheckBase<BC_Check<CheckT>, Policy>{
};
template<typename CheckT, typename Policy>
struct BC_DataWrapper<const BC_Check<CheckT>&, Policy> : BC_DataWrapper_CheckBase<BC_Check<CheckT>, Policy>{
};
template<typename CheckT, typename Policy>
struct BC_DataWrapper<const BC_Check<CheckT>, Policy> : BC_DataWrapper_CheckBase<BC_Check<CheckT>, Policy>{
};

template<typename CheckT>
struct BC_Check{
private:
	template<typename U, typename Policy>
	friend struct BC_DataWrapper_CheckBase;

public:
	typedef typename CheckT::type type;
	typedef typename CheckT::other ImplT;

public:
	BC_Check(){
		valid_ = false;
	}
	BC_Check(const type& v) : value_(v){
		valid_ = true;
		Valid();
	}
	template<typename U>
	BC_Check(const U& v){
		SetValueAux<type>(value_, static_cast<typename BC_DataConverter<U>::type>(v));
		valid_ = true;
		Valid();
	}
	BC_Check& operator = (const type& v){
		value_ = v;
		valid_ = true;
		Valid();
		return *this;
	}
	bool Valid() const{
		if (!valid_)
			return false;
		if (!ImplT().Check(value_))
		{
			assert(false);
			throw Detail::BC_Except_CheckErr("check error:");
			valid_ = false;
			return false;
		}
		return true;
	}

public:
	type			value_;

private:
	mutable bool	valid_;
};
//////////////////////////////////////////////////////////////////////////
template<int MaxSize, int MinSize>
struct BC_Check_SizeBase{
	template<typename T>
	using other_ = BC_Check_Size<typename BC_RemoveTypeCVR<T>::type, MaxSize, MinSize>;
};

template<typename T, int MaxSize, int MinSize>
struct BC_Check_Size : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef T type;
	typedef other_<type> other;
	static bool Check(const type&){
		return true;
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename CharT, typename _Traits, typename _Alloc, int MaxSize, int MinSize>
struct BC_Check_Size<std::basic_string<CharT, _Traits, _Alloc>, MaxSize, MinSize> : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef std::basic_string<CharT, _Traits, _Alloc> type;
	typedef other_<type> other;
	static bool Check(const type& v){
		auto size = v.size();
		return size >= MinSize && size <= MaxSize;
	}
};

template<typename CharT, typename StringTraits, int MaxSize, int MinSize>
struct BC_Check_Size<ATL::CStringT<CharT, StringTraits>, MaxSize, MinSize> : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef ATL::CStringT<CharT, StringTraits> type;
	typedef other_<type> other;
	static bool Check(const type& v){
		auto size = v.GetLength();
		return size >= MinSize && size <= MaxSize;
	}
};

template<typename CharT, int MaxSize, int MinSize, int MaxSize_ = MaxSize + 1>
struct BC_Check_Size_Char : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef other_<const CharT*> other;
	static bool Check(const CharT* v){
		auto size = StdEx::TStrMaxLen(v, MaxSize_);
		return size >= MinSize && size <= MaxSize;
	}
};

template<int MaxSize, int MinSize>
struct BC_Check_Size<const char*, MaxSize, MinSize> : BC_Check_Size_Char<char, MaxSize, MinSize>{
	typedef const char* type;
};

template<int MaxSize, int MinSize>
struct BC_Check_Size<const wchar_t*, MaxSize, MinSize> : BC_Check_Size_Char<wchar_t, MaxSize, MinSize>{
	typedef const wchar_t* type;
};

template<int size, int MaxSize, int MinSize>
struct BC_Check_Size<char[size], MaxSize, MinSize> : BC_Check_Size_Char<char, MaxSize, MinSize, size>{
	using type = char[size];
};

template<int size, int MaxSize, int MinSize>
struct BC_Check_Size<wchar_t[size], MaxSize, MinSize> : BC_Check_Size_Char<wchar_t, MaxSize, MinSize, size>{
	using type = wchar_t[size];
};

template<typename T, typename AllocT, int MaxSize, int MinSize>
struct BC_Check_Size<std::vector<T, AllocT>, MaxSize, MinSize> : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef std::vector<T, AllocT> type;
	typedef other_<type> other;
	static bool Check(const type& v){
		auto size = v.size();
		return size >= MinSize && size <= MaxSize;
	}
};

template<typename ContT, int MaxSize, int MinSize>
struct BC_Check_Size<BC_Cont<ContT>, MaxSize, MinSize> : BC_Check_SizeBase<MaxSize, MinSize>{
	typedef BC_Cont<ContT> type;
	typedef other_<type> other;
	static bool Check(const type& v){
		auto size = v.cont_.size();
		return size >= MinSize && size <= MaxSize;
	}
};
//////////////////////////////////////////////////////////////////////////