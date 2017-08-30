#include "stdafx.h"
#include <Net/JsonCall.h>
#include "Json/writer.h"
#include "Common/LogOut.h"
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>

//global function name key
const char* const kJsonEchoKey_Function_ = "func_name";
//getter key
const char* const kJsonEchoKey_Getter_ = "obj_getter";
//params
const char* const kJsonEchoKey_Params_ = "params";
//call mode
const char* const kJsonEchoKey_CallMode = "mode";
//is global function
const char* const kJsonEchoKey_IsGlobalFunc = "g_func";
//响应操作结果
const char* const kFuncName_RespondRes = "__OnDoRes__";

JsonEchoBase::JsonEchoBase(LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id /*= KFuncObjTemplateId_JsonEcho */) :
	FuncObj(&parent, obj_id), lua_vm_(lua_vm)
{

}

bool JsonEcho::GlobalFuncCall( const char* lua_global_func_name, const Json::Value* params )
{
	if ( !lua_global_func_name )
		return false;

	Json::Value impl;
	impl[kJsonEchoKey_Function_] = lua_global_func_name;
	impl[kJsonEchoKey_CallMode] = kGlobalCall;
	return JsonCall(impl, params);
}

bool JsonEcho::MemFuncCall( const ObjGetter& obj_getter, const char* mem_func_name, const Json::Value* params )
{
	return MemFuncCallImpl(&obj_getter, mem_func_name, params, kMemCall);
}

bool JsonEchoBase::JsonCall( Json::Value& lua_implementor, const Json::Value* params )
{
	if (params)
	{
		if ( params->isNull() || !params->isArray() || params->size() <= 0 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "Json::Value类型的params不能为null、不能不是array类型、参数个数不能小于1，否则并未真正发包";
			return false;
		}
		lua_implementor[kJsonEchoKey_Params_] = *params;
	}
	Json::FastWriter writer;
	return DoInnerImpl(writer.write(lua_implementor));
}

bool JsonEchoBase::OnInner( P_MsgNT msg_num, void* param, size_t param_size )
{
	if ( kInnerMsg_JsonCall == msg_num )
		return DoDecodeImpl(param, param_size);
	return __super::OnInner(msg_num, param, param_size);
}

bool JsonEchoBase::OnJsonCall( const Json::Value& lua_implementor, const Json::Value* params )
{
	const auto& json_call_mode = lua_implementor[kJsonEchoKey_CallMode];
	if ( json_call_mode.isNull() || !json_call_mode.isInt() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "call mode不符合判定";
		return false;
	}
	const auto& json_fn = lua_implementor[kJsonEchoKey_Function_];
	if ( json_fn.isNull() || !json_fn.isString() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "竟然不是字符串类型";
		return false;
	}
	const auto kCallMode = (enCallMode)json_call_mode.asInt();
	switch (kCallMode)
	{
	case kGlobalCall:
		{
			return OnGlobalFuncCall(json_fn.asCString(), params);
		}
	case kMemCall:
		{
			const auto& json_getter = lua_implementor[kJsonEchoKey_Getter_];
			if ( json_getter.isNull() || !json_getter.isArray() )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "kMemCall, object getter判定不通过";
				return false;
			}
			return OnMemFuncCall(&json_getter, json_fn.asCString(), params);
		}
	case kThisMemCall:
		{
			const auto& json_getter = lua_implementor[kJsonEchoKey_Getter_];
			const Json::Value* obj_getter = nullptr;
			if ( !json_getter.isNull() )
			{
				if ( !json_getter.isArray() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "kThisMemCall, object getter判定不通过";
					return false;
				}
				obj_getter = &json_getter;
			}
			stThisCallHelper helper = {
				obj_getter, json_fn.asCString(), params, false
			};
			return OnThisMemCall(helper);
		}
	case kThisMemCall_Sync:
		{
			const auto& json_getter = lua_implementor[kJsonEchoKey_Getter_];
			const Json::Value* obj_getter = nullptr;
			if ( !json_getter.isNull() )
			{
				if ( !json_getter.isArray() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "kThisMemCall, object getter判定不通过";
					return false;
				}
				obj_getter = &json_getter;
			}
			stThisCallHelper helper = {
				obj_getter, json_fn.asCString(), params, true
			};
			return OnThisMemCall(helper);
		}
	default:
		break;
	}
	assert(false);
	return false;
}


void JsonEchoBase::RegForLua( LuaVM& lua_vm )
{
	using namespace luabind;
	module(lua_vm.lstate_)
		[
			def("JsonEcho_GetFuncNameStr", &JsonEchoBase::GetFuncNameStr),
			def("JsonEcho_GetParamsStr", &JsonEchoBase::GetParamsStr),
			def("JsonEcho_GetResKey", &JsonEchoBase::GetResKey),
			def("ObjGetter_IsGlobalFunc", &JsonEchoBase::ObjGetter::IsGlobalFunc),

			class_<JsonEchoBase>("JsonEchoBase")
			.def(kFuncName_RespondRes, &JsonEchoBase::OnDoResult_)
			.def("__DoResult__", &JsonEchoBase::DoResult_)
		];
	
	lua_vm.DoString("	\
		function _JsonEcho_CallGlobalFunc_( fn_name, fn_params )	\
			local fn = _G[fn_name];	\
			if fn == nil then	\
				LuaLogD(\"不存在函数：\" .. fn_name);	\
				return;	\
			end	\
				return fn(JsonValueUnpack(fn_params));	\
		end	\
			\
		function _JsonEcho_CallOneMemFunc_( obj, mem_fn_name, mem_fn_params )	\
			local mem_fn = obj[mem_fn_name];	\
			if mem_fn == nil then	\
				LuaLogD(\"成员函数不存在->\" .. mem_fn_name);	\
				assert(false);	\
				return;	\
			end	\
			return mem_fn(obj, JsonValueUnpack(mem_fn_params));		\
		end	\
		");
	//call member function
	lua_vm.DoString("	\
		function __JsonEcho_CallMemFunc__( real_obj, obj_getter, mem_fn_name, mem_fn_params, is_sync )	\
			if obj_getter == nil then	\
				if real_obj == nil then		\
					LuaLogD(\"obj getter为空时，real obj不能为空\");	\
					return false;		\
				end		\
				if is_sync == true then		\
					local res = _JsonEcho_CallOneMemFunc_(real_obj, mem_fn_name, mem_fn_params);	\
					if res == nil then	\
						LuaLogD(mem_fn_name .. \"返回空值\");	\
						return true;	\
					end	\
					local json_res = JsonValue();	\
					local json_key = json_res:Key(JsonEcho_GetResKey());	\
					json_key:assign(JsonValue(res));	\
					real_obj:__DoResult__(mem_fn_name, json_res);	\
				else	\
					_JsonEcho_CallOneMemFunc_(real_obj, mem_fn_name, mem_fn_params);	\
				end	\
				return true;	\
			end	\
			local str_func_name = JsonEcho_GetFuncNameStr();	\
			local str_params_name = JsonEcho_GetParamsStr();	\
			for v in Closure_JsonValue(obj_getter) do	\
				local func_name = v:Key(str_func_name):AsString();	\
				local json_params = v:Key(str_params_name);	\
				if ObjGetter_IsGlobalFunc(v) then	\
					real_obj = _JsonEcho_CallGlobalFunc_(func_name, json_params);	\
				else	\
					real_obj = _JsonEcho_CallOneMemFunc_(real_obj, func_name, json_params);	\
				end	\
			end	\
			if real_obj == nil then		\
				LuaLogD(\"计算出real obj为空\");	\
				return false;	\
			end	\
			\
			if is_sync == true then		\
				local res = _JsonEcho_CallOneMemFunc_(real_obj, mem_fn_name, mem_fn_params);	\
				if res == nil then	\
					LuaLogD(mem_fn_name .. \"返回空值\");	\
					return true;	\
				end	\
				local json_res = JsonValue();	\
				local json_key = json_res:Key(JsonEcho_GetResKey());	\
				json_key:assign(JsonValue(res));	\
				real_obj:__DoResult__(mem_fn_name, json_res);	\
			else	\
				_JsonEcho_CallOneMemFunc_(real_obj, mem_fn_name, mem_fn_params);	\
			end		\
			return true;	\
		end	\
		");

	//call global function
	lua_vm.DoString("	\
		function __JsonEcho_CallGlobalFunc__( fn_name, fn_params )	\
			local fn = _G[fn_name];	\
			if fn == nil then	\
				LuaLogD(\"不存在函数：\" .. fn_name);	\
				return false;	\
			end	\
			fn(JsonValueUnpack(fn_params));	\
			return true;	\
		end	\
		");
}

bool JsonEcho::OnGlobalFuncCall( const char* lua_global_func_name, const Json::Value* params )
{
	if ( !lua_global_func_name )
	{
		assert(false);
		return false;
	}
	if (!lua_vm_.LuaObjCallByDlt("__JsonEcho_CallGlobalFunc__", false, lua_global_func_name, params))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "执行__JsonEcho_CallGlobalFunc__出错，func:" << lua_global_func_name;
		return false;
	}
	return true;
}

bool JsonEchoBase::OnMemFuncCall( const Json::Value* obj_getter, const char* mem_func_name, const Json::Value* params )
{
	stThisCallHelper helper = {
		obj_getter, mem_func_name, params, false
	};
	return OnThisFuncCallImpl(this, helper);
}

const char* JsonEchoBase::GetFuncNameStr()
{
	return kJsonEchoKey_Function_;
}

const char* JsonEchoBase::GetParamsStr()
{
	return kJsonEchoKey_Params_;
}

JsonEchoBase::ObjGetter_Multi JsonEchoBase::MakeGetter_Multi( const char* getter_func_name, const Json::Value* params )
{
	return std::move(ObjGetter_Multi(getter_func_name, params));
}

JsonEchoBase::ObjGetter::ObjGetter( const char* getter_func_name, const Json::Value* params, bool is_global_func ) 
	: getter_func_name_(getter_func_name)
{
	assert(getter_func_name);
	if (params)
		params_ = *params;
	is_global_func_ = is_global_func;
}

JsonEchoBase::ObjGetter::ObjGetter( const char* getter_func_name, const Json::Value* params )
	: getter_func_name_(getter_func_name)
{
	assert(getter_func_name);
	if (params)
		params_ = *params;
	is_global_func_ = true;
}

Json::Value JsonEchoBase::ObjGetter::get() const
{
	Json::Value getter;
	if ( !get(getter) )
		return Json::Value::null;
	return getter;
}

bool JsonEchoBase::ObjGetter::get( Json::Value& getter ) const
{
	Json::Value one_obj;
	AsJsonValue(one_obj);
	getter.append(one_obj);
	return true;
}

void JsonEchoBase::ObjGetter::AsJsonValue( Json::Value& json_obj ) const
{
	if ( !params_.isNull() && params_.isArray() )
		json_obj[kJsonEchoKey_Params_] = params_;
	json_obj[kJsonEchoKey_Function_] = getter_func_name_;
	json_obj[kJsonEchoKey_IsGlobalFunc] = is_global_func_;
}

bool JsonEchoBase::ObjGetter::IsGlobalFunc( const Json::Value& obj_getter )
{
	const auto& json_global_func = obj_getter[kJsonEchoKey_IsGlobalFunc];
	if ( json_global_func.isNull() || !json_global_func.isBool() )
	{
		assert(false);
		return false;
	}
	return json_global_func.asBool();
}

JsonEchoBase::ObjGetter_Multi::ObjGetter_Multi( const char* getter_func_name, const Json::Value* params )
	: ObjGetter(getter_func_name, params, true)
{

}

JsonEchoBase::ObjGetter_Multi::ObjGetter_Multi( const char* getter_func_name, const Json::Value* params, bool is_global_func )
	: ObjGetter(getter_func_name, params, is_global_func)
{

}

JsonEchoBase::ObjGetter_Multi& JsonEchoBase::ObjGetter_Multi::AddTail( const char* getter_func_name, const Json::Value* params )
{
	if ( !getter_func_name )
	{
		assert(false);
		return *this;
	}
	tail_getter_.push_back(std::move(ObjGetter(getter_func_name, params, false)));
	return *this;
}

bool JsonEchoBase::ObjGetter_Multi::get( Json::Value& getter ) const
{
	if ( !__super::get(getter) )
		return false;
	for ( const auto& obj_getter : tail_getter_ )
	{
		if ( !obj_getter.get(getter) )
			return false;
	}
	return true;
}

JsonEcho::JsonEcho( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id ) : JsonEchoBase(lua_vm, parent, obj_id)
{

}

void JsonEcho::RegForLua( LuaVM& lua_vm )
{
	__super::RegForLua(lua_vm);

	using namespace luabind;
	module(lua_vm.lstate_)
		[
			class_<JsonEcho, JsonEchoBase>("JsonEcho")
		];
}

bool JsonEchoBase::OnGlobalFuncCall( const char* lua_global_func_name, const Json::Value* params )
{
	assert(false);
	return false;
}

JsonEchoBase::ObjGetter_This JsonEchoBase::MakeGetter_This( const char* getter_func_name, const Json::Value* params )
{
	return std::move(ObjGetter_This(getter_func_name, params));
}

bool JsonEchoBase::MemFuncCallImpl( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params, enCallMode call_mode )
{
	if ( !mem_func_name )
		return false;
	Json::Value impl;
	if (obj_getter)
	{
		Json::Value getter;
		if ( !obj_getter->get(getter) )
			return false;
		impl[kJsonEchoKey_Getter_] = getter;
	}
	impl[kJsonEchoKey_Function_] = mem_func_name;
	impl[kJsonEchoKey_CallMode] = call_mode;
	return JsonCall(impl, params);
}

bool JsonEchoBase::ThisMemCall( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params )
{
	return MemFuncCallImpl(obj_getter, mem_func_name, params, kThisMemCall);
}


bool JsonEcho::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

LuaVM& JsonEchoBase::GetLuaVM()
{
	return lua_vm_;
}

void JsonEchoBase::OnDoResult_( const std::string& func_name, const Json::Value& res )
{
	if ( doing_func_name_ != func_name )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "函数->" << func_name << "  并未调用，调用着的函数->" << doing_func_name_;
		assert(false);
		return;
	}
	assert(!doing_result_);
	doing_result_ = res;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "函数->" << func_name << "  返回结果了";
}

void JsonEchoBase::DoResult_( const std::string& func_name, const Json::Value& res )
{
	ThisMemCall(nullptr, kFuncName_RespondRes, &JsonEchoBase::MakeParams(func_name, res));
}

bool JsonEchoBase::ThisMemCallSyncImpl( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params )
{
	if ( !MemFuncCallImpl(obj_getter, mem_func_name, params, kThisMemCall_Sync) )
		return false;
	assert(!doing_result_);
	doing_result_.reset();
	doing_func_name_ = mem_func_name;
	WaitUntil(2 * 60 * 1000, [this]()->bool{
		if ( !ObjValid() )
			return true;
		if (doing_result_)
			return true;
		return false;
	});
	if ( !doing_result_ )
		return false;
	if ( doing_result_->isNull() || !doing_result_->isObject() )
		return false;
	return true;
}

const char* JsonEchoBase::GetResKey()
{
	return "__res__";
}

bool JsonEchoBase::DecodeJson( void* param, size_t param_size )
{
	Json::Value impl;
	auto c_begin = (char*)param;
	if ( !JsonReader::GetMe().parse(c_begin, c_begin + param_size, impl, false) )
		return false;
	if (impl.isNull())
		return false;
	if ( !impl.isObject() )
		return false;

	const Json::Value* params = &(*const_cast<const Json::Value*>(&impl))[kJsonEchoKey_Params_];
	if (params->isNull())
	{
		params = nullptr;
	}
	else
	{
		if ( !params->isArray() )
			return false;
	}
	return OnJsonCall(impl, params);
}

bool JsonEchoBase::DoInnerImpl( const std::string& str_buffer )
{
	auto func_obj = P_CAST<FuncObjTop_Secret*>(GetMosttop());
	if ( !func_obj )
	{
		assert(false);
		return false;
	}
	const std::string& str_encode = func_obj->GetCompoSend().Encrypt(str_buffer);
	return Inner(kInnerMsg_JsonCall, str_encode.c_str(), str_encode.size());
}

bool JsonEchoBase::DoDecodeImpl( void* param, size_t param_size )
{
	assert(param);
	auto func_obj = P_CAST<FuncObjTop_Secret*>(GetMosttop());
	if ( !func_obj )
	{
		assert(false);
		return false;
	}
	const std::string& str_original = func_obj->GetCompoRecv().Decrypt(std::string((char*)param, param_size));
	param = const_cast<char*>(str_original.c_str());
	param_size = str_original.size();
	return DecodeJson(param, param_size);
}

bool JsonEchoBase::ObjValid() const
{
	if ( !parent_ )
	{
		assert(false);
		return false;
	}
	return parent_->ObjValid();
}

JsonEchoBase::ObjGetter_This::ObjGetter_This( const char* getter_func_name, const Json::Value* params )
	: ObjGetter_Multi(getter_func_name, params, false)
{

}

void JsonEchoBase::MakeParamsHelper_::operator () ( const Json::Value& param ){
	if (param.isNull())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "MakeParams，Json::Value类型的参数为空，忽略之";
		return;
	}
	params_.append(param);
}

JsonEchoTrans::JsonEchoTrans( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id /*= kFuncObjTemplateId_JsonEchoTrans */ )
	: JsonEcho(lua_vm, parent, obj_id)
{
	receiver_ = nullptr;
}

bool JsonEchoTrans::OnInner( P_MsgNT msg_num, void* param, size_t param_size )
{
	if ( kInnerMsg_JsonCall == msg_num )
	{
		if (receiver_)
		{
			assert(param);
			auto func_obj_self = P_CAST<FuncObjTop_Secret*>(GetMosttop());
			if ( !func_obj_self )
			{
				assert(false);
				return false;
			}
			const std::string& str_original = func_obj_self->GetCompoRecv().Decrypt(std::string((char*)param, param_size));

			for ( receiver_->First(); !receiver_->IsDone(); receiver_->Next() )
			{
				auto receiver_item = receiver_->CurItem();
				assert(receiver_item);
				if ( !receiver_item )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "接收器的某项怎么为空?";
					return false;
				}
				auto func_obj = P_CAST<FuncObjTop_Secret*>(receiver_item->GetMosttop());
				if ( !func_obj )
				{
					assert(false);
					return false;
				}
				const std::string& str_encode = func_obj->GetCompoSend().Encrypt(str_original);
				if ( !receiver_item->Inner(msg_num, (char*)str_encode.c_str(), str_encode.size()) )
					return false;
			}
			return true;
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "接收器不存在，忽略之";
			return true;
		}
	}
	return __super::OnInner(msg_num, param, param_size);
}

void JsonEchoTrans::RegForLua( LuaVM& lua_vm )
{
	__super::RegForLua(lua_vm);

	using namespace luabind;
	module(lua_vm.lstate_)
		[
			class_<JsonEchoTrans, JsonEchoBase>("JsonEchoTrans")
		];
}

void JsonEchoTrans::SetReceiver( const ReceiIteratorPtr& receiver )
{
	receiver_ = receiver;
}

JsonEchoTrans::IterOneReceiver::IterOneReceiver()
{
	ResetReceiver(nullptr);
}

void JsonEchoTrans::IterOneReceiver::First()
{
	if (receiver_)
		is_done_ = false;
}

void JsonEchoTrans::IterOneReceiver::Next()
{
	is_done_ = true;
}

bool JsonEchoTrans::IterOneReceiver::IsDone() const
{
	if (is_done_)
		return is_done_;
	return !receiver_;
}

JsonEchoTrans::IterOneReceiver::value_type 
JsonEchoTrans::IterOneReceiver::CurItem() const
{
	return receiver_;
}

void JsonEchoTrans::IterOneReceiver::ResetReceiver(value_type receiver)
{
	receiver_ = receiver;
	is_done_ = !receiver;
}

void JsonEchoTrans::IterMultiReceiver::First()
{
	it_recei_ = receivers_.begin();
}

JsonEchoTrans::IterMultiReceiver::IterMultiReceiver()
{
	it_recei_ = receivers_.begin();
}

void JsonEchoTrans::IterMultiReceiver::Next()
{
	++it_recei_;
}

bool JsonEchoTrans::IterMultiReceiver::IsDone() const
{
	return it_recei_ == receivers_.end();
}

JsonEchoTrans::IterMultiReceiver::value_type 
JsonEchoTrans::IterMultiReceiver::CurItem() const
{
	return *it_recei_;
}

void JsonEchoTrans::IterMultiReceiver::AddReceiver( value_type receiver )
{
	receivers_.push_back(receiver);
}

JsonEchoVmp::JsonEchoVmp( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id /*= kFuncObjTemplateId_JsonEchoBase */ )
	: JsonEchoBase(lua_vm, parent, obj_id)
{

}

bool JsonEchoVmp::DoInnerImpl( const std::string& str_buffer )
{
	VMP_BEGIN_EX;
	auto func_obj = P_CAST<FuncObjTop_Secret*>(GetMosttop());
	if ( !func_obj )
	{
		assert(false);
		return false;
	}
	const std::string& str_encode = func_obj->GetCompoSend().Encrypt(str_buffer);
	bool res = Inner(kInnerMsg_JsonCall, str_encode.c_str(), str_encode.size());
	VMP_END;
	return res;
}

bool JsonEchoVmp::DoDecodeImpl( void* param, size_t param_size )
{
	VMP_BEGIN_EX;
	assert(param);
	auto func_obj = P_CAST<FuncObjTop_Secret*>(GetMosttop());
	if ( !func_obj )
	{
		assert(false);
		return false;
	}
	const std::string& str_original = func_obj->GetCompoRecv().Decrypt(std::string((char*)param, param_size));
	param = const_cast<char*>(str_original.c_str());
	param_size = str_original.size();
	auto res = DecodeJson(param, param_size);
	VMP_END;
	return res;
}
