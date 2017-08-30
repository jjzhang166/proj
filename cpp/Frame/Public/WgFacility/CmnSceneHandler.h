#pragma once
/*
@author zhp
@date 2017/4/25 0:43
@purpose for common game scene
*/
#include <boost/smart_ptr.hpp>
#include <luabind/luabind.hpp>
#include <vector>
#include "../DependSelfLib.h"
//////////////////////////////////////////////////////////////////////////
class CSH_Handler;
typedef boost::shared_ptr<CSH_Handler> CSH_HandlerPtr;
class CSH_PerHandler;
class CSH_HandlerCont;
class CSH_RecycleHandler;
//policy
struct CSH_tagPer{};
struct CSH_tagCont{};
//////////////////////////////////////////////////////////////////////////
class CSH_Handler{
public:
	static void RegForLua(lua_State* l);
	virtual ~CSH_Handler();
	virtual bool InScene() const = 0;
	virtual bool Handle() = 0;

public:
	virtual void Add(const CSH_HandlerPtr& sub_handler) = 0;
	virtual bool Parse(const luabind::object& tbl) = 0;
};

class CSH_PerHandler : public CSH_Handler{
public:
	CSH_PerHandler(const luabind::object& fn);

public:
	bool Handle() override;

public:
	void Add(const CSH_HandlerPtr& sub_handler) override;
	bool Parse(const luabind::object& params) override;

private:
	luabind::object		fn_impl_;
	luabind::object		fn_params_;
	bool				param_direct_;
};

//场景容器
class CSH_HandlerCont : public CSH_Handler{
public:
	typedef std::vector<CSH_HandlerPtr> HandlerContT;

public:
	bool InScene() const override;
	bool Handle() override;

public:
	void Add(const CSH_HandlerPtr& sub_handler) override;
	bool Parse(const luabind::object& tbl) override;

private:
	HandlerContT			handlers_;
	mutable CSH_HandlerPtr	last_valid_handler_;
};

//场景循环处理
class CSH_RecycleHandler : public CSH_HandlerCont{
public:
	bool Handle() override;
};

//场景连接器
class CSH_Connector : public CSH_HandlerCont{
public:
	//添加目标场景
	void AddDstScene(const CSH_HandlerPtr& dst_handler);
	bool Handle() override;

private:
	CSH_HandlerPtr		dst_scenes_;
};
//////////////////////////////////////////////////////////////////////////
namespace Details{
	template<typename Policy>
	struct CSH_MakerImpl_;
	template<>
	struct CSH_MakerImpl_<CSH_tagPer>{
		template<typename HandlerT>
		static CSH_HandlerPtr Make(const luabind::object& fn){
			if (!fn.is_valid() || luabind::type(fn) != LUA_TFUNCTION)
			{
				assert(false);
				return nullptr;
			}
			return CSH_HandlerPtr(new HandlerT(fn));
		}
	};
	template<>
	struct CSH_MakerImpl_<CSH_tagCont>{
		template<typename HandlerT>
		static CSH_HandlerPtr Make(const luabind::object& param){
			if (!param.is_valid() || luabind::type(param) != LUA_TTABLE)
			{
				assert(false);
				return nullptr;
			}
			CSH_HandlerPtr handler(new HandlerT);
			if (!handler)
			{
				assert(false);
				return nullptr;
			}
			if (!handler->Parse(param))
			{
				assert(false);
				return nullptr;
			}
			return handler;
		}
	};
}
//////////////////////////////////////////////////////////////////////////
template<typename HandlerT>
CSH_HandlerPtr CSH_MakeHandler(){
	return CSH_HandlerPtr(new HandlerT);
}

template<typename HandlerT, typename Policy = CSH_tagPer>
CSH_HandlerPtr CSH_MakeHandler(const luabind::object& lua_value){
	return Details::CSH_MakerImpl_<Policy>::Make<HandlerT>(lua_value);
}

template<typename HandlerT>
CSH_HandlerPtr CSH_MakeHandler(const luabind::object& fn, const luabind::object& params){
	if (!fn.is_valid() || luabind::type(fn) != LUA_TFUNCTION)
	{
		assert(false);
		return nullptr;
	}
	CSH_HandlerPtr handler(new HandlerT(fn));
	if (!handler)
	{
		assert(false);
		return nullptr;
	}
	if (!handler->Parse(params))
	{
		assert(false);
		return nullptr;
	}
	return handler;
}
