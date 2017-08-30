#include "stdafx.h"
#include <WgFacility/CmnSceneHandler.h>
#include <boost/scope_exit.hpp>
#include <Common/LogOut.h>
#include <CmnMix/Cmn_LuaOper.h>
#include <luabind/unpack_call.hpp>
#include <AppMix/OnlyGameComponent.h>

CSH_Handler::~CSH_Handler()
{

}

static CSH_HandlerPtr CSH_MakeRecycleHandler_(const luabind::object& tbl){
	return Cmn_OnlyGameDllApp::GetApp().GetAppFactory().CreateSceneRecycleHandler(tbl);
}

void CSH_Handler::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<CSH_Handler, CSH_HandlerPtr>("CSH_Handler")
		.def("在场景中", &CSH_Handler::InScene)
		.def("处理", &CSH_Handler::Handle)
		.def("添加", &CSH_Handler::Add),

		class_<CSH_PerHandler, CSH_Handler>("CSH_PerHandler"),

		class_<CSH_HandlerCont, CSH_Handler>("CSH_HandlerCont"),

		class_<CSH_RecycleHandler, CSH_HandlerCont>("CSH_RecycleHandler"),

		class_<CSH_Connector, CSH_HandlerCont>("CSH_Connector")
		.def("添加目标场景", &CSH_Connector::AddDstScene),

		def("场景容器", &CSH_MakeHandler<CSH_HandlerCont, CSH_tagCont>),
		def("场景循环处理", &CSH_MakeRecycleHandler_),
		def("场景连接器", &CSH_MakeHandler<CSH_Connector, CSH_tagCont>)
	];
}

bool CSH_HandlerCont::InScene() const
{
	last_valid_handler_.reset();
	for (auto& v : handlers_)
	{
		if (v->InScene())
		{
			last_valid_handler_ = v;
			return true;
		}
	}
	return false;
}

bool CSH_HandlerCont::Handle()
{
	BOOST_SCOPE_EXIT_ALL(this){
		last_valid_handler_.reset();
	};
	if (!last_valid_handler_)
	{
		if (!InScene())
			return false;
		if (!last_valid_handler_)
		{
			assert(false);
			return false;
		}
	}
	return last_valid_handler_->Handle();
}

void CSH_HandlerCont::Add(const CSH_HandlerPtr& sub_handler)
{
	if (!sub_handler)
	{
		assert(false);
		return;
	}
	if (sub_handler.get() == this)
	{
		assert(false);
		return;
	}
	for (auto& v : handlers_)
	{
		if (v == sub_handler)
		{
			assert(false);
			return;
		}
	}
	handlers_.push_back(sub_handler);
}

bool CSH_HandlerCont::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (!tbl.is_valid())
		return true;
	if (LUA_TTABLE != type(tbl))
	{
		LOG_O(Log_error) << "必须是表类型";
		assert(false);
		return false;
	}
	CSH_HandlerPtr handler_tmp;
	object empty_obj;
	for (iterator it(tbl), it_end; it != it_end; ++it)
	{
		object v = *it;
		object key = it.key();
		auto t = type(key);
		if (LUA_TNUMBER == t)
		{
			const auto& node = LuaObjCast(v, handler_tmp);
			if (!node)
			{
				LOG_O(Log_error) << "必须是场景类型";
				assert(false);
				continue;
			}
			if (!node->Parse(empty_obj))
				continue;
			Add(node);
		}
		else if (LUA_TUSERDATA == t)
		{
			const auto& node = LuaObjCast(key, handler_tmp);
			if (!node)
			{
				LOG_O(Log_error) << "表的键必须是场景类型";
				assert(false);
				continue;
			}
			if (!node->Parse(v))
				continue;
			Add(node);
		}
		else
		{
			assert(false);
		}
	}
	return true;
}

CSH_PerHandler::CSH_PerHandler(const luabind::object& fn) : fn_impl_(fn)
{
	using namespace luabind;
	assert(LUA_TFUNCTION == type(fn));
	param_direct_ = true;
}

void CSH_PerHandler::Add(const CSH_HandlerPtr& sub_handler)
{
	assert(false);
}

bool CSH_PerHandler::Parse(const luabind::object& params)
{
	using namespace luabind;
	if (!params.is_valid() || LUA_TTABLE != type(params))
		param_direct_ = true;
	else
		param_direct_ = false;
	fn_params_ = params;
	return true;
}

bool CSH_PerHandler::Handle()
{
	if (param_direct_)
		return LuaObjCallByDlt(fn_impl_, true, fn_params_);
	return LuaObjCallByDlt(fn_impl_, true, luabind::unpack_call(fn_params_));
}

bool CSH_RecycleHandler::Handle()
{
	int cnt = 0;
	while (true)
	{
		if (InScene())
		{
			if (!__super::Handle())
				break;
			++cnt;
		}
		DummySleep(2000);
	}
	return cnt > 0;
}

void CSH_Connector::AddDstScene(const CSH_HandlerPtr& dst_handler)
{
	if (!dst_handler)
	{
		assert(false);
		return;
	}
	if (!dst_scenes_)
	{
		dst_scenes_ = CSH_MakeHandler<CSH_HandlerCont>();
		if (!dst_scenes_)
		{
			assert(false);
			return;
		}
	}
	dst_scenes_->Add(dst_handler);
}

bool CSH_Connector::Handle()
{
	if (!dst_scenes_)
	{
		assert(false);
		return false;
	}
	while (true)
	{
		if (dst_scenes_->InScene())
			return true;
		DummySleep(1000);
		if (!InScene())
		{
			assert(false);
			return false;
		}
		if (!__super::Handle())
		{
			assert(false);
			return false;
		}
		DummySleep(1000);
	}
	assert(false);
	return false;
}
