#pragma once
/*
@author zhp
@date 2017/3/7 20:58
@purpose for unpack table and call
*/
#include "object.hpp"
#include "detail/convert_to_lua.hpp"

namespace luabind{
	namespace detail{
		template<typename ContT>
		struct UnpackCallImpl{
			typedef typename ContT::value_type value_type;
			UnpackCallImpl(const ContT& cont) : cont_(cont){}

			const ContT&	cont_;
		};

		//2017/3/7 20:55,for unpack table
		template<class T>
		void convert_to_lua(lua_State* L, const UnpackCallImpl<T>& v)
		{
			for (auto it = v.cont_.begin(), it_end = v.cont_.end(); it != it_end; ++it)
			{
				const typename UnpackCallImpl<T>::value_type& the_value = *it;
				convert_to_lua(L, the_value);
			}
		}
	}

	template<typename T>
	detail::UnpackCallImpl<T> unpack_call(const T& v){
		return detail::UnpackCallImpl<T>(v);
	}
}