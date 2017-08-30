#ifndef LUABIND_LUABASE_HPP
#define LUABIND_LUABASE_HPP
/*
@author zhp
@date 2016/12/4 18:16
@purpose Ê¹C++Àà¼Ì³ÐÓÚlua
*/
#include "detail/FromLuaHandle.hpp"

namespace luabind{
	class lua_base : public detail::lua_baseImpl, public detail::IFromLuaHandle{
	protected:
		lua_base();

	public:
		void AddLuaBase(const adl::object& base) override final;

	protected:
		using lua_baseImpl::OnInitLuaBase;

	private:
		int DoIndex(lua_State* l, int self, int k) const override final;
	};
}
#endif	//LUABIND_LUABASE_HPP
