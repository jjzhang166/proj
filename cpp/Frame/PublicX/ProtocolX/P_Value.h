#pragma once
/*
@author zhp
@date 2017/5/13 6:26
@purpose for any value
*/
#include "ProtocolX/P_BaseTypeX.h"
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>
#include <luabind/luabind.hpp>
#include <string>
#include <vector>
#include <StdEx/TString.h>
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
class pt_value;
typedef boost::shared_ptr<pt_value> PtValue;
//////////////////////////////////////////////////////////////////////////
class pt_value : public boost::enable_shared_from_this<pt_value>{
public:
	typedef std::vector<PtValue> ValueCont;

public:
	static void RegForLua(lua_State* l);
	virtual ~pt_value();
	virtual PtValue Clone() const = 0;
	virtual void SetValue(pt_char value);
	virtual void SetValue(pt_wchar value);
	virtual void SetValue(pt_csz value);
	virtual void SetValue(pt_csz value, pt_dword size);
	virtual void SetValue(pt_cwsz value);
	virtual void SetValue(pt_cwsz value, pt_dword size);
	virtual void SetValue(pt_bool value);
	virtual void SetValue(pt_short value);
	virtual void SetValue(pt_int value);
	virtual void SetValue(pt_long value);
	virtual void SetValue(pt_float value);
	virtual void SetValue(pt_dint value);
	virtual void SetValue(pt_double value);
	virtual void SetValue(pt_ldouble value);
	template<typename EnumT>
	void SetValue(typename std::conditional<std::is_enum<EnumT>::value, EnumT, void>::type enum_value){
		SetValue(pt_int(enum_value));
	}

public:
	virtual pt_char GetValue(pt_char) const;
	virtual pt_wchar GetValue(pt_wchar) const;
	virtual pt_csz GetValue(pt_csz) const;
	virtual pt_cwsz GetValue(pt_cwsz) const;
	virtual pt_bool GetValue(pt_bool) const;
	virtual pt_short GetValue(pt_short) const;
	virtual pt_int GetValue(pt_int) const;
	virtual pt_long GetValue(pt_long) const;
	virtual pt_float GetValue(pt_float) const;
	virtual pt_dint GetValue(pt_dint) const;
	virtual pt_double GetValue(pt_double) const;
	virtual pt_ldouble GetValue(pt_ldouble) const;
	virtual luabind::object GetValue(lua_State* l) const;
	template<typename EnumT>
	typename std::conditional<std::is_enum<EnumT>::value, EnumT, void>::type
		GetValue(typename std::conditional<std::is_enum<EnumT>::value, EnumT, void>::type) const{
		return (EnumT)GetValue(pt_int());
	}
	virtual const ValueCont& GetValues() const;

public:
	virtual void Add(const PtValue& value);
	virtual void Erase(const PtValue& value);
	virtual void clear();
};

class pt_values : public pt_value{
public:
	PtValue Clone() const override;
	luabind::object GetValue(lua_State* l) const override;
	void Add(const PtValue& value) override;
	void Erase(const PtValue& value) override;
	void clear() override;
	const ValueCont& GetValues() const override;

private:
	ValueCont		values_;
};

template<typename ValueType>
class pt_string_value : public pt_value{
	static_assert(!std::is_same<ValueType, pt_csz>::value, "");
	static_assert(!std::is_same<ValueType, pt_cwsz>::value, "");

public:
	void SetValue(pt_csz value) override{
		if (!value)
		{
			assert(false);
			return;
		}
		SetValueImpl(value, StdEx::TStrLen(value));
	}
	void SetValue(pt_csz value, pt_dword size) override{
		if (!value)
		{
			assert(false);
			return;
		}
		SetValueImpl(value, size);
	}
	void SetValue(pt_cwsz value) override{
		if (!value)
		{
			assert(false);
			return;
		}
		SetValueImpl(value, StdEx::TStrLen(value));
	}
	void SetValue(pt_cwsz value, pt_dword size) override{
		if (!value)
		{
			assert(false);
			return;
		}
		SetValueImpl(value, size);
	}

private:
	template<typename T>
	void SetValueImpl(T value, pt_dword size){
		assert(value);
		auto res = ValueType();
		if (!boost::conversion::detail::try_lexical_convert(value, size, res))
		{
			assert(false);
			return;
		}
		((pt_value*)this)->SetValue(res);
	}
};
//////////////////////////////////////////////////////////////////////////
namespace Details{
	template<typename ValueType>
	struct stPtValueBase{
		typedef typename std::conditional<std::is_enum<ValueType>::value, pt_int, ValueType>::type value_type;
		typedef pt_string_value<value_type> type;
	};
	template<>
	struct stPtValueBase<pt_csz>{
		typedef pt_value type;
		typedef pt_csz value_type;
	};
	template<>
	struct stPtValueBase<pt_cwsz>{
		typedef pt_value type;
		typedef pt_cwsz value_type;
	};
	template<>
	struct stPtValueBase<pt_char>{
		typedef pt_value type;
		typedef pt_char value_type;
	};
	template<>
	struct stPtValueBase<pt_wchar>{
		typedef pt_value type;
		typedef pt_wchar value_type;
	};

	template<typename CharT>
	class pt_value_Char : public pt_value{
		typedef std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > StringType;

	public:
		typedef CharT value_type;
		typedef const value_type* c_str_type;

	public:
		pt_value_Char() : value_(){}
		pt_value_Char(c_str_type value) : value_(value){}
		operator c_str_type () const{
			return GetValue(c_str_type());
		}
		PtValue Clone() const override{
			return PtValue(new pt_value_Char(*this));
		}
		void SetValue(c_str_type value) override{
			value_ = value;
		}
		void SetValue(c_str_type value, pt_dword size) override{
			value_.assign(value, size);
		}
		c_str_type GetValue(c_str_type) const override{
			return value_.c_str();
		}
		luabind::object GetValue(lua_State* l) const override{
			if (!l)
			{
				assert(false);
				return luabind::object();
			}
			return luabind::object(l, value_);
		}

	private:
		StringType		value_;
	};
}
//////////////////////////////////////////////////////////////////////////
template<typename ValueType>
class pt_value_t : public Details::stPtValueBase<ValueType>::type{
public:
	typedef typename Details::stPtValueBase<ValueType>::value_type value_type;

public:
	pt_value_t() : value_(){}
	pt_value_t(value_type value) : value_(value){}
	operator value_type () const{
		return GetValue(value_type());
	}
	PtValue Clone() const override{
		return PtValue(new pt_value_t(*this));
	}
	void SetValue(value_type value) override{
		value_ = value;
	}
	value_type GetValue(value_type) const override{
		return value_;
	}
	luabind::object GetValue(lua_State* l) const override{
		if (!l)
		{
			assert(false);
			return luabind::object();
		}
		return luabind::object(l, value_);
	}

private:
	value_type		value_;
};

template<>
class pt_value_t<pt_csz> : public Details::pt_value_Char<char>{
public:
	pt_value_t() : Details::pt_value_Char<char>(){}
	pt_value_t(pt_csz value) : Details::pt_value_Char<char>(value){}
};

template<>
class pt_value_t<pt_cwsz> : public Details::pt_value_Char<wchar_t>{
public:
	pt_value_t() : Details::pt_value_Char<wchar_t>(){}
	pt_value_t(pt_cwsz value) : Details::pt_value_Char<wchar_t>(value){}
};

//////////////////////////////////////////////////////////////////////////
//字符类型
typedef pt_value_t<pt_char> pt_value_char;
typedef pt_value_t<pt_wchar> pt_value_wchar;
//字符串类型
typedef pt_value_t<pt_csz> pt_value_csz;
typedef pt_value_t<pt_cwsz> pt_value_cwsz;
//布尔类型
typedef pt_value_t<pt_bool> pt_value_bool;
//短整数类型
typedef pt_value_t<pt_short> pt_value_short;
//整数类型
typedef pt_value_t<pt_int> pt_value_int;
typedef pt_value_t<pt_long> pt_value_long;
typedef pt_value_t<pt_float> pt_value_float;
typedef pt_value_t<pt_dint> pt_value_dint;
//浮点类型
typedef pt_value_t<pt_double> pt_value_double;
typedef pt_value_t<pt_ldouble> pt_value_ldouble;

class PtValueFactory{
public:
	template<typename ValueType>
	static PtValue MakeValue(){
		return PtValue(new pt_value_t<ValueType>);
	}
	template<typename ValueType>
	static PtValue MakeValue(ValueType value){
		return PtValue(new pt_value_t<ValueType>(value));
	}
};
//////////////////////////////////////////////////////////////////////////
