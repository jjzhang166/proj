#include "stdafx.h"
#include <StringValue/CSV_Node.h>
#include <CommonX/CmnX_StaticConts.h>

CSV_Node::~CSV_Node()
{

}

PtValue CSV_Node::Parse(const std::string& str)
{
	return Parse(str.c_str(), str.size());
}

PtValue CSV_Node::Parse(pt_csz str, pt_dword str_size)
{
	return Parse(str_size, str);
}

void CSV_Node::Add(const CSV_NodePtr& sub_node)
{
	assert(false);
}

void CSV_Node::Erase(const CSV_NodePtr& sub_node)
{
	assert(false);
}

void CSV_Node::clear()
{
	assert(false);
}

CSV_NodePtr CSV_Node::Add_(const CSV_NodePtr& sub_node)
{
	Add(sub_node);
	return shared_from_this();
}

void CSV_Node::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<CSV_Node, CSV_NodePtr>("CSV_Node")
		.def("添加", &CSV_Node::Add_),

		class_<CSV_Nodes, CSV_Node>("CSV_Nodes"),

		class_<CSV_Some, CSV_Nodes>("CSV_Some"),

		class_<CSV_Any, CSV_Nodes>("CSV_Any"),

		class_<CSV_Catcher, CSV_Some>("CSV_Catcher")
		.def("添加捕获", &CSV_Catcher::AddCatch_)
		.def("匹配", &CSV_Catcher::AddMatched)
		.def("捕获", &CSV_Catcher::AddCatchValue)
		.def("捕获", &CSV_Catcher::AddCatchString),

		class_<CSV_Calculator, CSV_Catcher>("CSV_Calculator"),

		class_<CSV_String, CSV_Node>("CSV_String"),

		class_<CSV_Value, CSV_Node>("CSV_Value")
	];
}

void CSV_Nodes::Add(const CSV_NodePtr& sub_node)
{
	return AddImpl(nodes_, sub_node);
}

void CSV_Nodes::Erase(const CSV_NodePtr& sub_node)
{
	if (!sub_node)
	{
		assert(false);
		return;
	}
	auto it = std::find(nodes_.begin(), nodes_.end(), sub_node);
	if (it == nodes_.end()) return;
	nodes_.erase(it);
}

void CSV_Nodes::clear()
{
	nodes_.clear();
}

void CSV_Nodes::AddImpl(NodeCont& nodes, const CSV_NodePtr& sub_node)
{
	if (!sub_node)
	{
		assert(false);
		return;
	}
	if (sub_node.get() == this)
	{
		assert(false);
		return;
	}
	for (auto& v : nodes)
	{
		if (v == sub_node)
		{
			//assert(false);
			return;
		}
	}
	nodes.push_back(sub_node);
}

const CSV_Nodes::NodeCont& CSV_Nodes::GetNodes() const
{
	return nodes_;
}

PtValue CSV_Any::Parse(pt_dword& str_size, pt_csz& str)
{
	PtValue res;
	for (auto& node : nodes_)
	{
		res = node->Parse(str_size, str);
		if (res)
			break;
	}
	return res;
}

PtValue CSV_Some::Parse(pt_dword& str_size, pt_csz& str)
{
	PtValue tmp;
	CmnBufferVector<PtValue, 100> values;
	for (auto& node : nodes_)
	{
		tmp = node->Parse(str_size, str);
		if (tmp)
			values.push_back(tmp);
	}
	if (values.empty())
		return nullptr;
	if (last_values_)
		last_values_->clear();
	else
	{
		last_values_.reset(new pt_values);
		if (!last_values_)
		{
			assert(false);
			return nullptr;
		}
	}
	for (auto& v : values)
		last_values_->Add(v);
	return last_values_;
}

PtValue CSV_String::Parse(pt_dword& str_size, pt_csz& str)
{
	assert(str);
	if (str_size < 1)
		return nullptr;
	auto str_end = str + str_size;
	auto my_cstr = str_.c_str();
	auto my_str_size = str_.size();
	auto it = std::search(str, str_end, my_cstr, my_cstr + my_str_size);
	if (it == str_end)
		return nullptr;
	if (!last_value_)
	{
		last_value_.reset(new pt_value_t<pt_csz>);
		if (!last_value_)
		{
			assert(false);
			return nullptr;
		}
		last_value_->SetValue(my_cstr, my_str_size);
	}
	str = it + my_str_size;
	assert(str_size >= my_str_size);
	str_size -= my_str_size;
	return last_value_;
}

CSV_String::CSV_String(const std::string& str) : str_(str)
{
	assert(!str.empty());
}

CSV_Value::CSV_Value(const PtValue& value) : value_(value)
{
	assert(value);
}

PtValue CSV_Value::Parse(pt_dword& str_size, pt_csz& str)
{
	assert(str);
	if (str_size < 1)
		return nullptr;
	if (!value_)
	{
		assert(false);
		return nullptr;
	}
	auto digit_begin = str;
	auto left_size = str_size;
	for (pt_dword i = 0; i < left_size; ++i)
	{
		auto c = *(digit_begin + i);
		if (std::isdigit(c) || c == '+' || c == '-' || c == '.')
		{
			digit_begin += i;
			break;
		}
	}
	auto digit_end = digit_begin;
	left_size -= digit_begin - str;
	for (pt_dword i = 0; i < left_size; ++i)
	{
		auto c = *(digit_begin + i);
		if (std::isdigit(c))
			continue;
		if (c == '.')
			continue;
		digit_end = digit_begin + i;
		break;
	}
	if (digit_begin == digit_end)
	{
		//assert(false);
		return nullptr;
	}
	assert(digit_end > digit_begin);
	assert(digit_begin >= str);
	value_->SetValue(digit_begin, digit_end - digit_begin);
	str = digit_end;
	str_size -= digit_end - str;
	return value_;
}

void CSV_Catcher::AddCatch(const CSV_NodePtr& sub_value_node)
{
	Add(sub_value_node);
	AddImpl(catch_nodes_, sub_value_node);
}

PtValue CSV_Catcher::Parse(pt_dword& str_size, pt_csz& str)
{
	PtValue tmp;
	struct stHelp{
		CSV_NodePtr		node_;
		PtValue			value_;
	};
	CmnBufferVector<stHelp, 100> values_tmp;
	for (auto& node : nodes_)
	{
		tmp = node->Parse(str_size, str);
		if (!tmp)
			return nullptr;
		values_tmp.push_back({ node, tmp });
	}
	if (values_tmp.empty())
		return nullptr;
	CmnBufferVector<PtValue, 100> res_values_tmp;
	auto it_begin = catch_nodes_.begin(), it_end = catch_nodes_.end();
	for (auto& v : values_tmp)
	{
		auto it = std::find(it_begin, it_end, v.node_);
		if (it == it_end)
			continue;
		res_values_tmp.push_back(v.value_);
	}
	if (res_values_tmp.empty())
	{
		assert(false);
		return nullptr;
	}
	if (last_values_)
		last_values_->clear();
	else
	{
		last_values_.reset(new pt_values);
		if (!last_values_)
		{
			assert(false);
			return nullptr;
		}
	}
	for (auto& v : res_values_tmp)
		last_values_->Add(v);
	return last_values_;
}

CSV_NodePtr CSV_Catcher::AddCatch_(const CSV_NodePtr& sub_node)
{
	AddCatch(sub_node);
	return shared_from_this();
}

CSV_NodePtr CSV_Catcher::AddMatched(const std::string& str)
{
	auto ptr = CSV_Factory::MakeString(str);
	if (ptr)
		return Add_(ptr);
	assert(false);
	return shared_from_this();
}

CSV_NodePtr CSV_Catcher::AddCatchValue(const PtValue& value)
{
	auto ptr = CSV_Factory::MakeValue(value);
	if (ptr)
		return AddCatch_(ptr);
	assert(false);
	return shared_from_this();
}

CSV_Catcher::CSV_Catcher(const std::string& name) : name_(name)
{
}

CSV_NodePtr CSV_Catcher::AddCatchString(const std::string& str)
{
	auto ptr = CSV_Factory::MakeString(str);
	if (ptr)
		return AddCatch_(ptr);
	assert(false);
	return shared_from_this();
}

CSV_Calculator::CSV_Calculator(const std::string& name, const luabind::object& fn) : CSV_Catcher(name), fn_(fn)
{
	assert(LUA_TFUNCTION == luabind::type(fn));
}

CSV_T<CSV_Calculator> CSV_Factory::MakeCalclator(const std::string& name, const luabind::object& fn)
{
	using namespace luabind;
	if (!fn.is_valid())
	{
		assert(false);
		return nullptr;
	}
	if (LUA_TFUNCTION != type(fn))
	{
		assert(false);
		return nullptr;
	}
	return CSV_T<CSV_Calculator>(new CSV_Calculator(name, fn));
}

CSV_NodePtr CSV_Factory::MakeString(const std::string& str)
{
	if (str.empty())
	{
		assert(false);
		return nullptr;
	}
	return CSV_NodePtr(new CSV_String(str));
}

CSV_NodePtr CSV_Factory::MakeValue(const PtValue& value)
{
	if (!value)
	{
		assert(false);
		return nullptr;
	}
	return CSV_NodePtr(new CSV_Value(value));
}

CSV_T<CSV_Catcher> CSV_Factory::MakeCatcher(const std::string& name)
{
	return CSV_T<CSV_Catcher>(new CSV_Catcher(name));
}
