#define LUABIND_BUILDING
#include <luabind/object.hpp>
#include <luabind/detail/FromLuaHandle.hpp>

namespace luabind {
	namespace detail
	{
		void IFromLuaHandle::AddLuaBase(const adl::object& base)
		{
			assert(false);
		}

		int IFromLuaHandle::DoIndex(lua_State* l, int self, int k) const
		{
			return 0;
		}

		int FromLuaHandleImpl::GetLuaBase(lua_State* L){
			auto n = lua_gettop(L);
			if (n < 1 || n > 2)
			{
				assert(false);
				return 0;
			}
			if (LUA_TTABLE != lua_type(L, 1))
			{
				assert(false);
				return 0;
			}
			int idx = 1;
			if (LUA_TNUMBER == lua_type(L, 2))
				idx = (int)lua_tonumber(L, 2);
			lua_rawgeti(L, 1, idx);
			return 1;
		}

		void FromLuaHandleImpl::AddLuaBaseImpl(const adl::object& base)
		{
			auto L = base.interpreter();
			if (!L)
			{
				assert(false);
				return;
			}
#ifdef _DEBUG
			base.push(L);
			auto t = lua_type(L, -1);
			lua_pop(L, 1);
			if (LUA_TTABLE != t && LUA_TUSERDATA != t)
			{
				assert(false);
				return;
			}
#endif
			if (!lua_bases_.interpreter())
			{
				lua_newtable(L);
				handle(L, -1).swap(lua_bases_);
				lua_newtable(L);
				lua_pushcfunction(L, GetLuaBase);
				lua_setfield(L, -2, "GetLuaBase");
				lua_setmetatable(L, -2);
			}
			else
			{
				lua_bases_.push(L);
			}
			base.push(L);
			lua_rawseti(L, -2, luaL_getn(L, -2) + 1);

			lua_pop(L, 1);
		}

		int FromLuaHandleImpl::DoIndexImpl_(lua_State* L, int self, int k) const{
			if (!lua_bases_.interpreter())
				return 0;
#ifdef _DEBUG
			auto top = lua_gettop(L);
#endif
			lua_bases_.push(L);
			lua_getmetatable(L, -1);
			lua_pushvalue(L, k);
			lua_rawget(L, -2);
			if (LUA_TFUNCTION == lua_type(L, -1))
			{
				lua_pushvalue(L, self);
				lua_pushvalue(L, -4);
				lua_pushcclosure(L, ChooseCall, 3);
				lua_replace(L, -3);
				lua_pop(L, 1);
				assert(top == lua_gettop(L) - 1);
				return 1;
			}
			lua_pop(L, 2);

			lua_pushnil(L);
			while (lua_next(L, -2)){
				lua_pushvalue(L, k);
				lua_gettable(L, -2);
				auto t = lua_type(L, -1);
				if (LUA_TNIL != t){
					if (LUA_TFUNCTION == t && LUA_TUSERDATA == lua_type(L, -2)){						lua_pushvalue(L, self);						lua_pushvalue(L, -3);						lua_pushcclosure(L, ChooseCall, 3);					}
					lua_insert(L, -4);
					lua_pop(L, 3);
					assert(top == lua_gettop(L) - 1);
					return 1;				}
				lua_pop(L, 2);
			}
			lua_pop(L, 1);
			assert(top == lua_gettop(L));
			return 0;
		}

		int FromLuaHandleImpl::ChooseCall(lua_State* L)
		{
			auto n = lua_gettop(L);
			if (n > 0)
			{
				if (lua_equal(L, 1, lua_upvalueindex(2)))
				{
					lua_pushvalue(L, lua_upvalueindex(3));
					lua_replace(L, 1);
				}
			}
			lua_pushvalue(L, lua_upvalueindex(1));
			lua_insert(L, 1);
			lua_call(L, n, LUA_MULTRET);
			return lua_gettop(L);
		}

		void FromLuaHandleImpl::ClearAllLuaBase()
		{
			handle().swap(lua_bases_);
		}

		void lua_baseImpl::OnInitLuaBase(lua_State* L){}

		lua_baseImpl::lua_baseImpl()
		{
			is_init_lua_base_impl_ = false;
		}

}}
