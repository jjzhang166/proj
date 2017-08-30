#pragma once
/*
@author zhp
@date 2017/5/13 4:34
@purpose for parse value of string
CSV->Common String Value.
*/
#include <ProtocolX/P_Value.h>
#include <CmnMix/Cmn_LuaOper.h>
#include <CmnMix/Cmn_TemplateUtility.h>
#include <luabind/unpack_call.hpp>
#include "../DependSelfLib.h"

//////////////////////////////////////////////////////////////////////////
class CSV_Node;
typedef boost::shared_ptr<CSV_Node> CSV_NodePtr;

template<typename DerivedT>
using CSV_T = SmartT<CSV_NodePtr, DerivedT>;
//////////////////////////////////////////////////////////////////////////
class CSV_Node : public boost::enable_shared_from_this<CSV_Node>{
public:
	static void RegForLua(lua_State* l);
	template<typename ValueType>
	static ValueType Eval(const luabind::object& fn, const PtValue& value, ValueType def_value){
		using namespace luabind;
		if (!fn.is_valid())
		{
			assert(false);
			return def_value;
		}
		if (!value)
		{
			assert(false);
			return def_value;
		}
		auto param = value->GetValue(fn.interpreter());
		if (!param.is_valid())
			return def_value;
		else if (LUA_TTABLE != type(param))
			return LuaObjCallByDlt(fn, def_value, param);
		return LuaObjCallByDlt(fn, def_value, luabind::unpack_call(param));
	}

public:
	virtual ~CSV_Node();
	PtValue Parse(const std::string& str);
	PtValue Parse(pt_csz str, pt_dword str_size);
	virtual PtValue Parse(pt_dword& str_size, pt_csz& str) = 0;

public:
	virtual void Add(const CSV_NodePtr& sub_node);
	CSV_NodePtr Add_(const CSV_NodePtr& sub_node);
	virtual void Erase(const CSV_NodePtr& sub_node);
	virtual void clear();
};

class CSV_Nodes : public CSV_Node{
public:
	typedef std::vector<CSV_NodePtr> NodeCont;

public:
	using CSV_Node::Parse;
	void Add(const CSV_NodePtr& sub_node) override;
	void Erase(const CSV_NodePtr& sub_node) override;
	void clear() override;
	const NodeCont& GetNodes() const;

protected:
	void AddImpl(NodeCont& nodes, const CSV_NodePtr& sub_node);

protected:
	NodeCont		nodes_;
};

class CSV_Some : public CSV_Nodes{
public:
	PtValue Parse(pt_dword& str_size, pt_csz& str) override;

protected:
	PtValue			last_values_;
};

class CSV_Any : public CSV_Nodes{
public:
	PtValue Parse(pt_dword& str_size, pt_csz& str) override;
};

//捕获者
class CSV_Catcher : public CSV_Some{
public:
	CSV_Catcher(const std::string& name);
	using CSV_Node::Parse;
	PtValue Parse(pt_dword& str_size, pt_csz& str) override;
	//添加捕获
	void AddCatch(const CSV_NodePtr& sub_value_node);

public:
	CSV_NodePtr AddCatch_(const CSV_NodePtr& sub_node);
	//匹配
	CSV_NodePtr AddMatched(const std::string& str);
	//捕获
	CSV_NodePtr AddCatchValue(const PtValue& value);
	CSV_NodePtr AddCatchString(const std::string& str);

public:
	inline const std::string& GetName() const{ return name_; }

private:
	NodeCont			catch_nodes_;
	std::string			name_;
};

class CSV_Calculator : public CSV_Catcher{
public:
	CSV_Calculator(const std::string& name, const luabind::object& fn);
	template<typename ValueType>
	ValueType Eval(const PtValue& value, ValueType def_value){
		return __super::Eval(fn_, value, def_value);
	}
	template<typename ValueType>
	ValueType Eval(const std::string& str, ValueType def_value){
		auto value = Parse(str);
		if (!value)
			return def_value;
		return Eval(value, def_value);
	}
	template<typename ValueType>
	ValueType Eval(pt_csz str, pt_dword str_size, ValueType def_value){
		auto value = Parse(str, str_size);
		if (!value)
			return def_value;
		return Eval(value, def_value);
	}

private:
	luabind::object		fn_;
};

class CSV_String : public CSV_Node{
public:
	CSV_String(const std::string& str);

public:
	PtValue Parse(pt_dword& str_size, pt_csz& str) override;

private:
	std::string		str_;
	PtValue			last_value_;
};

class CSV_Value : public CSV_Node{
public:
	CSV_Value(const PtValue& value);

public:
	PtValue Parse(pt_dword& str_size, pt_csz& str) override;

private:
	PtValue		value_;
};

class CSV_Factory{
public:
	static CSV_T<CSV_Catcher> MakeCatcher(const std::string& name);
	static CSV_T<CSV_Calculator> MakeCalclator(const std::string& name, const luabind::object& fn);
	static CSV_NodePtr MakeString(const std::string& str);
	static CSV_NodePtr MakeValue(const PtValue& value);
};