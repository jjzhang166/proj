#define LUABIND_BUILDING
#include <luabind/object.hpp>
#include <luabind/LuaBase.hpp>

namespace luabind{
	lua_base::lua_base(){}

	void lua_base::AddLuaBase(const adl::object& base)
	{
		return AddLuaBaseImpl(base);
	}

	int lua_base::DoIndex(lua_State* l, int self, int k) const
	{
		return DoIndexImpl_(l, self, k);
	}

}
