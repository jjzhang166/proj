#pragma once
/*
@author zhp
@date 2017/1/15 16:43
@purpose for lua raw call
*/
#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>

namespace luabind {
	namespace detail  {

		struct raw_call_converter
		{
			int const consumed_args(...)
			{
				return 0;
			}

			lua_State* apply(lua_State* L, by_pointer<lua_State>, int)
			{
				return L;
			}

			static int match(...)
			{
				return 0;
			}

			void converter_postcall(lua_State*, by_pointer<lua_State>, int) {}
		};

		template<int N>
		struct raw_call_policy : conversion_policy<N, false>
		{
			static void precall(lua_State*, const index_map&) {}
			static void postcall(lua_State*, const index_map&) {}

			template<class T, class Direction>
			struct apply
			{
				typedef raw_call_converter type;
			};
		};

		template<int N>
		inline bool IsRawCall_(bool arity_greater, policy_cons<raw_call_policy<N>, null_type>*){
			return arity_greater;
		}
	}
} // namespace luabind::detail

namespace luabind {

	template<int N>
	detail::policy_cons<
		detail::raw_call_policy<N>
		, detail::null_type
	>
	inline raw_call(LUABIND_PLACEHOLDER_ARG(N))
	{
		return detail::policy_cons<
			detail::raw_call_policy<N>
			, detail::null_type
		>();
	}

} // namespace luabind