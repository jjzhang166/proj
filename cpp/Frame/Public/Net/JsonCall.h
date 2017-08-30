#pragma once
/*
@author zhp
@date 2014-3-11 14:40
@purpose for json call, useful of remote call lua
现如今已被废弃，今后最好用BufCall，时间是：2016/4/22 2:48。
*/
#include "FuncObjs/FuncObj.h"
#include "Common/JsonMore.h"
#include "Script/lua_base.h"
#include "Protocol/P_Defines.h"
#include "Common/TupleMore.h"
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include "Common/Iterator.h"
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include "../DependSelfLib.h"
#include <CmnMix/Cmn_LogOut.h>

//json remote call
class JsonEchoBase : public FuncObj, public boost::noncopyable{
private:
	struct MakeParamsHelper_{
		MakeParamsHelper_( Json::Value& params ) : params_(params){}
		template<typename T>
		void operator () ( const T& param ){
			params_.append(param);
		}
		void operator () ( const Json::Value& param );

	private:
		Json::Value&			params_;
	};
	static void MakeParams();
	static void MakeParams_();

protected:
	//call mode
	enum enCallMode{
		kGlobalCall,			//全局调用
		kMemCall,				//调用成员
		kThisMemCall,			//this调用，异步调用
		kThisMemCall_Sync,		//同步调用
	};
	JsonEchoBase(LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id = kFuncObjTemplateId_JsonEchoBase);

public:
	LuaVM& GetLuaVM();

	//register for lua
public:
	//register for lua
	static void RegForLua( LuaVM& lua_vm );
	//make params
#define JsonEcho_MakeParams_HELPER_(z, n, _)	\
	template<BOOST_PP_ENUM_PARAMS(n, typename T_)>	\
	static Json::Value MakeParams( BOOST_PP_ENUM_BINARY_PARAMS(n, const T_, &P_) ){	\
		Json::Value params;	\
		boost::tuples::for_each_element(boost::make_tuple(BOOST_PP_ENUM_PARAMS(n, P_)), MakeParamsHelper_(params));	\
		return std::move(params);	\
	}	\
	template<BOOST_PP_ENUM_PARAMS(n, typename T_)>	\
	static void MakeParams_( Json::Value& params BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_BINARY_PARAMS(n, const T_, &P_) ){	\
		boost::tuples::for_each_element(boost::make_tuple(BOOST_PP_ENUM_PARAMS(n, P_)), MakeParamsHelper_(params));	\
	}	\
	/**/
	BOOST_PP_REPEAT_FROM_TO(1, 10, JsonEcho_MakeParams_HELPER_, _)
#undef JsonEcho_MakeParams_HELPER_

	//type definition
public:
	//this call helper
	struct stThisCallHelper{
		const Json::Value*		obj_getter_;
		const char*				mem_func_name_;
		const Json::Value*		params_;
		bool					is_sync_;
	};
	//obj getter, for send
	class ObjGetter{
	public:
		ObjGetter( const char* getter_func_name, const Json::Value* params );
		//get json::value
		Json::Value get() const;
		virtual bool get( Json::Value& getter ) const;
		//输出到json::value
		void AsJsonValue( Json::Value& json_obj ) const;

	protected:
		ObjGetter( const char* getter_func_name, const Json::Value* params, bool is_global_func );

	public:
		static bool IsGlobalFunc( const Json::Value& obj_getter );
	
	private:
		std::string				getter_func_name_;
		Json::Value				params_;
		bool					is_global_func_;
	};
	//multi getter
	class ObjGetter_Multi : public ObjGetter{
	public:
		ObjGetter_Multi( const char* getter_func_name, const Json::Value* params );
		//后续object getter
		JsonEchoBase::ObjGetter_Multi& AddTail( const char* getter_func_name, const Json::Value* params );
		virtual bool get( Json::Value& getter ) const;

	protected:
		ObjGetter_Multi( const char* getter_func_name, const Json::Value* params, bool is_global_func );

	private:
		std::vector<ObjGetter>			tail_getter_;
	};
	//this getter
	class ObjGetter_This : public ObjGetter_Multi{
	public:
		ObjGetter_This( const char* getter_func_name, const Json::Value* params );
	};
	//make muliple object getter,
	//usage: MakeGetter_Multi("GlobalGetterFuncName", params).AddTail("MemGetterFuncName", params)
	static ObjGetter_Multi MakeGetter_Multi( const char* getter_func_name, const Json::Value* params );
	//this call
	//usage: MakeGetter_This("ThisMemberFunctionName", params)
	static ObjGetter_This MakeGetter_This( const char* getter_func_name, const Json::Value* params );

	//结果
	template<typename RetT>
	class stDoResult{
	public:
		stDoResult( RetT& out_ret, const RetT& def_ret ) : out_ret_(out_ret), def_ret_(def_ret){}
		void SetResult( const RetT& res ){
			out_ret_ = res;
		}
		const RetT& GetDefResult() const{
			return def_ret_;
		}

	private:
		RetT&			out_ret_;
		const RetT&		def_ret_;
	};
	template<typename RetT>
	static stDoResult<RetT> MakeResult( RetT& out_ret, const RetT& def_ret ){
		return stDoResult<RetT>(out_ret, def_ret);
	}

	//for send
public:
	//this member call
	bool ThisMemCall( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params );
	//同步版本，会返回结果
	template<typename RetT>
	bool ThisMemCallSync( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params, stDoResult<RetT>& do_result ){
		if ( !ThisMemCallSyncImpl(obj_getter, mem_func_name, params) )
			return false;
		assert(doing_result_);
		const auto& json_res = (*doing_result_)[GetResKey()];
		const auto& res = lua_vm_.LuaObjCallByDlt("JsonValueGet", do_result.GetDefResult(), json_res);
		doing_result_.reset();
		do_result.SetResult(res);
		return true;
	}
	virtual bool DecodeJson( void* param, size_t param_size );

	//for recv
protected:
	//impl OnInner
	virtual bool OnInner( P_MsgNT msg_num, void* param, size_t param_size );
	//on receive lua function call
	bool OnJsonCall( const Json::Value& lua_implementor, const Json::Value* params );
	//on global function call
	virtual bool OnGlobalFuncCall( const char* lua_global_func_name, const Json::Value* params );
	//on member function call
	bool OnMemFuncCall( const Json::Value* obj_getter, const char* mem_func_name, const Json::Value* params );
	//on this member function call
	virtual bool OnThisMemCall( const stThisCallHelper& helper ) = 0;
	template<typename DerivedT>
	bool OnThisFuncCallImpl( DerivedT* derived_ptr, const stThisCallHelper& helper ){
		if (helper.obj_getter_)
		{
			assert(!helper.obj_getter_->isNull() && helper.obj_getter_->isArray());
			(void*)0;
		}
		
		if ( !helper.mem_func_name_ )
		{
			assert(false);
			return false;
		}
		if (!lua_vm_.LuaObjCallByDlt("__JsonEcho_CallMemFunc__", false, derived_ptr, helper.obj_getter_,
			helper.mem_func_name_, helper.params_, helper.is_sync_) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "执行__JsonEcho_CallMemFunc__出错" << "\tmem func:" << helper.mem_func_name_;
			return false;
		}
		return true;
	}
	//json call
	//lua_implementor->执行体。params->执行体所需的参数，是可选的
	bool JsonCall( Json::Value& lua_implementor, const Json::Value* params );
	virtual bool DoInnerImpl( const std::string& str_buffer );
	virtual bool DoDecodeImpl( void* param, size_t param_size );
	bool MemFuncCallImpl( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params, enCallMode call_mode );

private:
	static const char* GetFuncNameStr();
	static const char* GetParamsStr();
	//result key
	static const char* GetResKey();
	//同步发送
	bool ThisMemCallSyncImpl( const ObjGetter* obj_getter, const char* mem_func_name, const Json::Value* params );
	//发送结果
	void DoResult_( const std::string& func_name, const Json::Value& res );
	//响应操作结果
	void OnDoResult_( const std::string& func_name, const Json::Value& res );
	bool ObjValid() const override;

protected:
	LuaVM&							lua_vm_;
	std::string						doing_func_name_;			//正在调用的函数名
	boost::optional<Json::Value>	doing_result_;				//调用返回结果
};

class JsonEchoVmp : public JsonEchoBase{
protected:
	JsonEchoVmp( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id = kFuncObjTemplateId_JsonEchoBase );

private:
	bool DoInnerImpl( const std::string& str_buffer );
	bool DoDecodeImpl( void* param, size_t param_size );
};

class JsonEcho : public JsonEchoBase{
public:
	 JsonEcho( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id = KFuncObjTemplateId_JsonEcho );
	 //register for lua
	 static void RegForLua( LuaVM& lua_vm );

public:
	//json call like obj_name:member_function_name(...)
	bool MemFuncCall( const ObjGetter& obj_getter, const char* mem_func_name, const Json::Value* params );
	//json call global function
	bool GlobalFuncCall( const char* lua_global_func_name, const Json::Value* params );

private:
	virtual bool OnGlobalFuncCall( const char* lua_global_func_name, const Json::Value* params );
	virtual bool OnThisMemCall( const stThisCallHelper& helper );
};

//转发器,json echo transmit
class JsonEchoTrans : public JsonEcho{
	//type definitions
public:
	//receiver iterator
	typedef Iterator<JsonEchoBase*> ReceiIterator;
	typedef std::shared_ptr<ReceiIterator> ReceiIteratorPtr;
	//one receiver
	class IterOneReceiver : public ReceiIterator{
	public:
		IterOneReceiver();
		void First() override;
		void Next() override;
		bool IsDone() const override;
		value_type CurItem() const override;

	public:
		void ResetReceiver(value_type receiver);

	private:
		bool				is_done_;
		value_type			receiver_;
	};
	//multiple receiver
	class IterMultiReceiver : public ReceiIterator{
	public:
		IterMultiReceiver();
		void First() override;
		void Next() override;
		bool IsDone() const override;
		value_type CurItem() const override;

	public:
		void AddReceiver( value_type receiver );

	private:
		typedef std::list<value_type> ReceiversT;

	private:
		ReceiversT				receivers_;
		ReceiversT::iterator	it_recei_;
	};

public:
	JsonEchoTrans( LuaVM& lua_vm, FuncObj& parent, P_ObjId obj_id = kFuncObjTemplateId_JsonEchoTrans );
	//register for lua
	static void RegForLua( LuaVM& lua_vm );
	//设置接收器，必须保证接收器在发包时的有效性，否则就宕机
	//如果接收器不存在的话，则默认忽略该事件
	void SetReceiver( const ReceiIteratorPtr& receiver );

protected:
	virtual bool OnInner( P_MsgNT msg_num, void* param, size_t param_size );

protected:
	ReceiIteratorPtr				receiver_;			//接收器
};