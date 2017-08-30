#pragma once
/*
@author zhp
@date 2016/12/5 15:04
@purpose for base
*/
#include "self_dependence.h"
#include <boost/type_traits/is_pointer.hpp>


class wxHeaderCtrlEvent;
struct LuaBindWxPolicy{
	struct tagTrackWindow{};
	struct tagVoid{};
	//////////////////////////////////////////////////////////////////////////
	struct Functions{
		static void WxLuaAddTrackedWindow(lua_State* L, wxObject* obj);
		static luabind::object WxLuaInitUserData(lua_State* L, const void* obj_ptr, int wxl_type);
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename Policy, typename... Args>
	static void InitPreExec(Policy, lua_State* L, const Args&... args){}
	static void InitPreExec(tagTrackWindow, lua_State* L, wxObject* obj);
	//////////////////////////////////////////////////////////////////////////
	//static int GetWxType(void*, void*);
	static int GetWxType(wxControl*, wxControl*);
	static int GetWxType(wxApp*, wxApp*);
	static int GetWxType(wxPoint*, wxPoint*);
	static int GetWxType(wxMenu*, wxMenu*);
	static int GetWxType(wxWindow*, wxWindow*);
	static int GetWxType(wxSize*, wxSize*);
	static int GetWxType(wxNotifyEvent*, wxNotifyEvent*);
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static T* CastToImpl_(const luabind::object& obj, T* ptr){
		auto L = obj.interpreter();
		obj.push(L);
		luabind::detail::stack_pop pop(L, 1);
		auto type = GetWxType(ptr, ptr);
		assert(type >= 0);
		auto res = (T*)wxluaT_getuserdatatype(L, -1, type);
		assert(res);
		return res;
	}
	template<typename T>
	static T* CastToImpl(const luabind::object& obj, const T* ptr){
		return CastToImpl_(obj, (T*)ptr);
	}
	template<typename T>
	static typename boost::enable_if_c<!boost::is_pointer<T>::value, T>::type 
		CastToImpl(const luabind::object& obj, const T v){
		auto res = CastToImpl_(obj, (T*)0);
		if (!res) return (T)v;
		return *res;
	}	
	template<typename T>
	static T CastTo(const luabind::object& obj, T def_value){
		if (!obj.is_valid()) return def_value;
		if (LUA_TNIL == luabind::type(obj)) return def_value;
		return CastToImpl(obj, def_value);
	}
	template<typename T, typename Base>
	static T* DynamicCast(const luabind::object& obj){
		auto ptr = CastTo(obj, (Base*)nullptr);
		if (!ptr)
		{
			assert(false);
			return nullptr;
		}
		auto res = dynamic_cast<T*>(ptr);
		assert(res);
		return res;
	}
	//////////////////////////////////////////////////////////////////////////

	template<typename Policy, typename Derived>
	static luabind::object InitWxLua(lua_State* L, Derived* obj_ptr){
		InitPreExec(Policy(), L, obj_ptr);
		return Functions::WxLuaInitUserData(L, obj_ptr, GetWxType(obj_ptr, obj_ptr));
	}
};

template<typename Derived, typename Base>
class LuaBindWxProxy : public luabind::lua_base{
protected:
	explicit LuaBindWxProxy(const luabind::object& obj){
		impl_ = LuaBindWxPolicy::DynamicCast<Derived, Base>(obj);
		assert(impl_);
		lua_obj_ = obj;
	}

private:
	void OnInitLuaBase(lua_State* L) override final{
		luabind::object tmp;
		tmp.swap(lua_obj_);
		AddLuaBase(tmp);
	}

protected:
	Derived*		impl_;
	
private:
	luabind::object	lua_obj_;
};

template<typename Derived, typename RegLuaPolicy>
class LuaBindWxBase : public luabind::lua_base{
private:
	void OnInitLuaBase(lua_State* L) override final{
		AddLuaBase(LuaBindWxPolicy::InitWxLua<RegLuaPolicy>(L, (Derived*)this));
	}
};

#define LUABIND_WX_DEFINE_CLS(wxClsName, Policy) class B_ ## wxClsName : public wxClsName, \
		public LuaBindWxBase<B_ ## wxClsName, LuaBindWxPolicy::Policy>
