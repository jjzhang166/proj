#include "stdafx.h"
#include <ProtocolX/P_Value.h>
#include <cassert>

pt_value::~pt_value()
{

}

void pt_value::SetValue(pt_char value)
{
	assert(false);
}

void pt_value::SetValue(pt_wchar value)
{
	assert(false);
}

void pt_value::SetValue(pt_csz value)
{
	assert(false);
}

void pt_value::SetValue(pt_csz value, pt_dword size)
{
	assert(false);
}

void pt_value::SetValue(pt_cwsz value)
{
	assert(false);
}

void pt_value::SetValue(pt_cwsz value, pt_dword size)
{
	assert(false);
}

void pt_value::SetValue(pt_bool value)
{
	assert(false);
}

void pt_value::SetValue(pt_short value)
{
	assert(false);
}

void pt_value::SetValue(pt_int value)
{
	assert(false);
}

void pt_value::SetValue(pt_long value)
{
	assert(false);
}

void pt_value::SetValue(pt_float value)
{
	assert(false);
}

void pt_value::SetValue(pt_dint value)
{
	assert(false);
}

void pt_value::SetValue(pt_double value)
{
	assert(false);
}

void pt_value::SetValue(pt_ldouble value)
{
	assert(false);
}

pt_char pt_value::GetValue(pt_char) const
{
	assert(false);
	return 0;
}

pt_wchar pt_value::GetValue(pt_wchar) const
{
	assert(false);
	return 0;
}

pt_csz pt_value::GetValue(pt_csz) const
{
	assert(false);
	return 0;
}

pt_cwsz pt_value::GetValue(pt_cwsz) const
{
	assert(false);
	return 0;
}

pt_bool pt_value::GetValue(pt_bool) const
{
	assert(false);
	return 0;
}

pt_short pt_value::GetValue(pt_short) const
{
	assert(false);
	return 0;
}

pt_int pt_value::GetValue(pt_int) const
{
	assert(false);
	return 0;
}

pt_long pt_value::GetValue(pt_long) const
{
	assert(false);
	return 0;
}

pt_float pt_value::GetValue(pt_float) const
{
	assert(false);
	return 0;
}

pt_dint pt_value::GetValue(pt_dint) const
{
	assert(false);
	return 0;
}

pt_double pt_value::GetValue(pt_double) const
{
	assert(false);
	return 0;
}

pt_ldouble pt_value::GetValue(pt_ldouble) const
{
	assert(false);
	return 0;
}

luabind::object pt_value::GetValue(lua_State* l) const
{
	assert(false);
	return luabind::object();
}

void pt_value::Add(const PtValue& value)
{
	assert(false);
}

void pt_value::Erase(const PtValue& value)
{
	assert(false);
}

void pt_value::clear()
{
	assert(false);
}

template<typename ValueType>
static PtValue __MakePtValue__1(){
	return PtValueFactory::MakeValue<ValueType>();
}
template<typename ValueType>
static PtValue __MakePtValue__2(ValueType value){
	return PtValueFactory::MakeValue(value);
}

void pt_value::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<pt_value, PtValue>("pt_value"),

		class_<pt_value_char, pt_value>("pt_value_char"),
		def("字符类型", &__MakePtValue__1<pt_char>),

		class_<pt_value_csz, pt_value>("pt_value_csz"),
		def("字符串类型", &__MakePtValue__1<pt_csz>),

		class_<pt_value_bool, pt_value>("pt_value_bool"),
		def("布尔类型", &__MakePtValue__1<pt_bool>),

		class_<pt_value_short, pt_value>("pt_value_short"),
		def("短整数类型", &__MakePtValue__1<pt_short>),

		class_<pt_value_int, pt_value>("pt_value_int"),
		def("整数类型", &__MakePtValue__1<pt_int>),

		class_<pt_value_double, pt_value>("pt_value_double"),
		def("浮点类型", &__MakePtValue__1<pt_double>)
	];
}

const pt_value::ValueCont& pt_value::GetValues() const
{
	static ValueCont empty;
	return empty;
}

void pt_values::Add(const PtValue& value)
{
	if (!value)
	{
		assert(false);
		return;
	}
	if (value.get() == this)
	{
		assert(false);
		return;
	}
	for (auto& v : values_)
	{
		if (v == value)
		{
			assert(false);
			return;
		}
	}
	values_.push_back(value);
}

void pt_values::Erase(const PtValue& value)
{
	if (!value)
	{
		assert(false);
		return;
	}
	auto it = std::find(values_.begin(), values_.end(), value);
	if (it == values_.end()) return;
	values_.erase(it);
}

void pt_values::clear()
{
	values_.clear();
}

luabind::object pt_values::GetValue(lua_State* l) const
{
	using namespace luabind;
	if (values_.empty())
		return object();
	if (!l)
	{
		assert(false);
		return object();
	}
	object tbl = newtable(l);
	int idx = 1;
	for (auto& v : values_)
	{
		tbl[idx] = v->GetValue(l);
		++idx;
	}
	return tbl;
}

PtValue pt_values::Clone() const
{
	 auto ptr = PtValue(new pt_values(*this));
	 ptr->clear();
	 for (auto& i : values_)
		 ptr->Add(i->Clone());
	 return ptr;
}

const pt_values::ValueCont& pt_values::GetValues() const
{
	return values_;
}
