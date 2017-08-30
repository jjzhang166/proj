#include "stdafx.h"
#include <Common/JsonMore.h>
#include "Script/lua_base.h"
#include <fstream>
#include <Net/JsonCall.h>
#include <Net/JsonEchoCoorTrans.h>

JsonReader& JsonReader::GetMe()
{
	static JsonReader  json_reader;
	return json_reader;
}

bool JsonReader::ParseCStr( const char* document, Json::Value &root )
{
	return parse(document, root, false);
}

JsonReader::JsonReader()
{

}

bool JsonReader::ParseFromFile( const char* full_file_name, Json::Value& root )
{
	std::ifstream ifs(full_file_name);
	return parse(ifs, root, false);
}

void RegJsonForLua::RegValue( LuaVM* lua_vm )
{
	assert(lua_vm);
	assert(lua_vm->lstate_);
	using namespace luabind;
	using namespace Json;
	//value
	//数组的索引方式
	typedef const Value& (Value::*IndexFnType_c)( UInt ) const;
	typedef Value& (Value::*IndexFnType)( UInt );
	//key的索引方式
	typedef const Value& (Value::*KeyFnType_c)( const char* ) const;
	typedef Value& (Value::*KeyFnType)( const char* );
	//value iterator
	typedef Value::iterator (Value::*ValueIter)();
	typedef Value::const_iterator (Value::*ValueIter_C)() const;
	//is member
	typedef bool (Value::*IsMemberT)( const char* ) const;
	//value iterator
	//自增
	typedef Value::iterator& (Value::iterator::*Increment)();
	typedef Value::const_iterator& (Value::const_iterator::*ConstIncrement)();

	module(lua_vm->lstate_)
		[
			class_<Value>("JsonValue")
			.def(constructor<>())
			.def(constructor<Int>())
			//.def(constructor<double>())		//LUA里面这个double类型的构造会与int类型的构造函数有冲突
			.def(constructor<const char*, const char*>())
			.def(constructor<const std::string &>())
			.def(constructor<bool>())
			.def(constructor<const Value &>())
			//数组的索引方式
			.def("Index", (IndexFnType_c)&Value::operator[])
			.def("Index", (IndexFnType)&Value::operator[])
			.def("Key", (KeyFnType_c)&Value::operator[])
			.def("Key", (KeyFnType)&Value::operator[])
			.def("Begin", (ValueIter)&Value::begin)
			.def("Begin", (ValueIter_C)&Value::begin)
			.def("End", (ValueIter)&Value::end)
			.def("End", (ValueIter_C)&Value::end)
			.def("IsMember", (IsMemberT)&Value::isMember)
			.def("IsValidIndex", &Value::isValidIndex)
			.def("Empty", &Value::empty)
			.def("Size", &Value::size)
			.def("IsInt", &Value::isInt)
			.def("IsUInt", &Value::isUInt)
			.def("IsNumeric", &Value::isNumeric)
			.def("IsNull", &Value::isNull)
			.def("IsBool", &Value::isBool)
			.def("IsString", &Value::isString)
			.def("IsArray", &Value::isArray)
			.def("IsObject", &Value::isObject)
			.def("IsEqual", &Value::operator==)
			.def("IsAddrEqual", &Value::IsAddrEqual)
			.def("AsInt", &Value::asInt)
			.def("AsUInt", &Value::asUInt)
			.def("AsDouble", &Value::asDouble)
			.def("AsBool", &Value::asBool)
			.def("AsString", &Value::asCString)
			.def("assign", &Value::operator=),

			class_<ValueIteratorBase>("JsonValueIterBase"),

			class_<Value::iterator, ValueIteratorBase>("JsonValueIter")
			.def(constructor<const Value::iterator&>())
			.def("Increment", (Increment)&Value::iterator::operator++)
			.def("Deref", &Value::iterator::operator*)
			.def("IsEqual", &Value::iterator::operator==),

			class_<Value::const_iterator, ValueIteratorBase>("JsonValueConstIter")
			.def(constructor<const Value::const_iterator&>())
			.def("Increment", (ConstIncrement)&Value::const_iterator::operator++)
			.def("Deref", &Value::const_iterator::operator*)
			.def("IsEqual", &Value::const_iterator::operator==)
		];

	//JsonValue的闭包(closure)，用于for循环
	lua_vm->DoString("\
		function Closure_JsonValue( json_value )	\
			local it = json_value:Begin();	\
			return function()	\
				if not it:IsEqual(json_value:End()) then	\
					local item = it:Deref();	\
					it:Increment();	\
					return item;	\
				end	\
			end	\
		end");
	
	//get atomic
	lua_vm->DoString("	\
		function JsonValueGet( json_value_atom )	\
			if json_value_atom == nil then	\
				return;	\
			end	\
			\
			if json_value_atom:IsString() then	\
				return json_value_atom:AsString();	\
			elseif json_value_atom:IsInt() then	\
				return json_value_atom:AsInt();	\
			elseif json_value_atom:IsUInt() then	\
				return json_value_atom:AsUInt();	\
			elseif json_value_atom:IsBool() then	\
				return json_value_atom:AsBool();	\
			elseif json_value_atom:IsNumeric() then	\
				return json_value_atom:AsDouble();	\
			elseif json_value_atom:IsNull() then	\
				return;	\
			else	\
				return json_value_atom;	\
			end	\
		end");
	
	//json value unpack
	lua_vm->DoString("	\
		function JsonValueUnpack( json_value )	\
			if json_value == nil or json_value:IsNull() then	\
				return;	\
			end	\
			local value_atomic = {};	\
			local idx = 1;	\
			for v in Closure_JsonValue(json_value) do	\
				local v_atom = JsonValueGet(v);	\
				if v_atom == nil then	\
					LuaLogE(\"JsonValueUnpack索引到为空的值了\");	\
					return;	\
				end	\
				value_atomic[idx] = v_atom;	\
				idx = idx + 1;	\
			end	\
			return unpack(value_atomic);	\
		end	\
		");
		
	//register json echo
	//JsonEcho::RegForLua(*lua_vm);
	//register json echo transmit
	JsonEchoTrans::RegForLua(*lua_vm);
	//JECoorTrans::RegForLua(*lua_vm);
}

void RegJsonForLua::RegReader( LuaVM* lua_vm )
{
	assert(lua_vm);
	assert(lua_vm->lstate_);
	using namespace luabind;
	using namespace Json;
	//JsonReader

	module(lua_vm->lstate_)
		[
			class_<JsonReader>("JsonReader")
			.def("ParseStr", &JsonReader::ParseCStr)
			.def("ParseFromFile", &JsonReader::ParseFromFile),
			def("JsonReader_Instance", &JsonReader::GetMe)
		];
}


void RegJsonForLua::RegPath( LuaVM* lua_vm )
{
	assert(lua_vm);
	assert(lua_vm->lstate_);
	using namespace luabind;
	using namespace Json;
	
	typedef const Value& (Path::*ResolveT)( const Value& ) const;
	module(lua_vm->lstate_)
		[
			class_<PathArgument>("JsonPathArgument")
			.def(constructor<UInt>())
			.def(constructor<const char*>()),

			class_<Path::InArgs>("JsonPathInArgs")
			.def(constructor<>())
			.def("PushBack", &Path::InArgs::PushBack),

			class_<Path>("JsonPath")
			.def(constructor<const char*, const Path::InArgs&>())
			.def(constructor<const char*>())
			.def(constructor<const char*, const PathArgument&>())
			.def(constructor<const char*, const PathArgument&, const PathArgument&>())
			.def(constructor<const char*, const PathArgument&, const PathArgument&, const PathArgument&>())
			.def(constructor<const char*, const PathArgument&, const PathArgument&, const PathArgument&, const PathArgument&>())
			.def(constructor<const char*, const PathArgument&, const PathArgument&, const PathArgument&, const PathArgument&, const PathArgument&>())
			/*
			用Path::make而不用Path::resolve有三个原因：
			1.没有重载带来的编译期错误
			2.Path::make返回的是Value&而不是const Value&
			3.resolve的概念比较通用，一般是指针对某种类型的对象的路径解析，boost::property_tree里就有这个概念。
			*/
			.def("Resolve", (ResolveT)&Path::resolve)
		];
}
