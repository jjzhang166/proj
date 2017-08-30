#pragma once
/*
@author zhp
@date 2014/8/2 4:12
@purpose for game object base,UpdateAll, RebuildAll
*/
#include "Iterator.h"
#include <boost/shared_ptr.hpp>
#include <functional>
#include <set>
#include <Script/lua_base.h>
#include "CmnPriLuaData.h"
#include <boost/optional.hpp>
#include <vector>
#include "../DependSelfLib.h"
#include <CmnMix/Cmn_GameObjBase.h>

//过滤
template<typename AttachDataT_>
class GObjFilterAttachProxy;
template<>
class GObjFilterAttachProxy<void>{};
template<typename AttachDataT_>
class GObjFilterAttachProxy{
public:
	typedef AttachDataT_ AttachDataT;

public:
	void SetAttachData( const AttachDataT& attach_data ){
		attach_data_ = attach_data;
	}
	const AttachDataT& GetAttachData() const{
		return attach_data_;
	}

protected:
	AttachDataT				attach_data_;
};
extern const CmnPriLuaData* g_obj_filter_lua_env_;
void SetGObjFilterLuaEnv( const CmnPriLuaData* pri_lua_data );
class GObjFilterBase{
public:
	typedef boost::shared_ptr<GObjFilterBase> ObjFilterBasePtrT;

public:
	virtual ~GObjFilterBase();
	virtual bool AddLuaFilterHandler( const luabind::object& func_filter, const luabind::object& handler_func ) = 0;
	virtual bool AddFilter( const ObjFilterBasePtrT& sub_filter ) = 0;
};
template<typename FilterObjT_, typename AttachDataT_ = void>
class GObjFilter : public GObjFilterBase, public GObjFilterAttachProxy<AttachDataT_>{
public:
	typedef FilterObjT_ FilterObjT;	
	typedef boost::shared_ptr<GObjFilter> SelfPtrT;
	typedef std::set<SelfPtrT> ObjFilterContT;
	typedef std::function<bool( FilterObjT& filter_obj )> FilterFuncT;
	typedef FilterFuncT HandleFuncT;
	typedef std::pair<FilterFuncT, HandleFuncT> ObjHandlerT;
	typedef std::vector<ObjHandlerT> ObjHandlerContT;

public:
	void ResetGeneralHandler( const HandleFuncT& general_handler ){
		general_handler_ = general_handler;
	}
	bool AddFilterHandler( const FilterFuncT& func_filter, const HandleFuncT& handler_func ){
		filter_handler_.push_back(ObjHandlerT(func_filter, handler_func));
		return true;
	}
	bool AddLuaFilterHandler( const luabind::object& func_filter, const luabind::object& handler_func ){
		using namespace luabind;
		if ( !g_obj_filter_lua_env_ )
		{
			assert(false);
			return false;
		}
		if ( LUA_TFUNCTION != type(func_filter) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "过滤的参数必须是函数类型";
			return false;
		}
		if ( LUA_TFUNCTION != type(handler_func) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "过滤处理的参数必须是函数类型";
			return false;
		}
		return AddFilterHandler(MakeFilterFunc(func_filter), MakeHandlerFunc(handler_func));
	}
	bool AddFilter( const ObjFilterBasePtrT& sub_filter ){
		return AddSubObjFilter(SmartCast<SelfPtrT>(sub_filter));
	}
	bool AddSubObjFilter( const SelfPtrT& sub_filter ){
		if ( !sub_filter )
		{
			assert(false);
			return false;
		}
		obj_filter_cont_.insert(sub_filter);
		return true;
	}
	void RemoveSubObjFilter( const SelfPtrT& sub_filter ){
		obj_filter_cont_.erase(sub_filter);
	}
	bool TryFilter( FilterObjT& filter_obj ) const{
		for ( const auto& v : filter_handler_ )
		{
			assert(v.first);
			if (v.first(filter_obj))
				return true;
		}
		for ( const auto& v : obj_filter_cont_ )
		{
			assert(v);
			if (v->TryFilter(filter_obj))
				return true;
		}
		return false;
	}
	bool FilterDoObj( FilterObjT& filter_obj ) const{
		for ( const auto& v : filter_handler_ )
		{
			assert(v.first);
			if (v.first(filter_obj))
			{
				assert(v.second);
				if (v.second(filter_obj))
					return true;
			}
		}
		for ( const auto& v : obj_filter_cont_ )
		{
			assert(v);
			if (v->FilterDoObj(filter_obj))
				return true;
		}
		if (general_handler_)
			return general_handler_(filter_obj);
		return false;
	}

	static FilterFuncT MakeFilterFunc( const luabind::object& func_filter ){
		assert(g_obj_filter_lua_env_);
		assert(LUA_TFUNCTION == luabind::type(func_filter));
		const auto& func_filter_name = RandomUniqueStr();
		g_obj_filter_lua_env_->SetLuaObj(func_filter_name, func_filter);
		return [func_filter_name]( FilterObjT& filter_obj )->bool{
			if ( !g_obj_filter_lua_env_ )
			{
				assert(false);
				return false;
			}
			assert(36 == func_filter_name.size());
			return CallFunction(g_obj_filter_lua_env_->GetLuaObj(func_filter_name), false, filter_obj);
		};
	}
	static HandleFuncT MakeHandlerFunc( const luabind::object& handler_func ){
		assert(g_obj_filter_lua_env_);
		assert(LUA_TFUNCTION == luabind::type(handler_func));
		const auto& handler_func_name = RandomUniqueStr();
		g_obj_filter_lua_env_->SetLuaObj(handler_func_name, handler_func);
		return [handler_func_name]( FilterObjT& filter_obj )->bool{
			if ( !g_obj_filter_lua_env_ )
			{
				assert(false);
				return false;
			}
			assert(36 == handler_func_name.size());
			return CallFunction(g_obj_filter_lua_env_->GetLuaObj(handler_func_name), true, filter_obj);
		};
	}

protected:
	ObjHandlerContT		filter_handler_;
	HandleFuncT			general_handler_;
	ObjFilterContT		obj_filter_cont_;
};

//生产-达成
template<typename T>
class ProduceToDone{
public:
	typedef T ProductT;
	typedef boost::optional<ProductT> OptionalProductT;
	typedef std::function<OptionalProductT()> ProduceFuncT;
	typedef std::function<void( ProductT& product )> DoneFuncT;
	typedef std::pair<ProduceFuncT, DoneFuncT> ProduceDonePairT;
	typedef std::vector<ProduceDonePairT> ProduceDoneContT;

public:
	void Clear(){
		produce_done_.clear();
	}
	void AddItem( const ProduceFuncT& produce_func, const DoneFuncT& done_func ){
		produce_done_.push_back(ProduceDonePairT(produce_func, done_func));
	}
	void DoProduce(){
		for ( const auto& p_pair : produce_done_ )
		{
			auto& res_product = p_pair.first();
			if (res_product)
				p_pair.second(*res_product);
		}
	}

private:
	ProduceDoneContT	produce_done_;
};