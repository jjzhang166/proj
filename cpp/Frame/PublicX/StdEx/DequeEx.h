#pragma once
/*
@author zhp
@date 2017/4/12 22:33
@purpose deque extention
*/
#include <deque>

namespace StdEx{
	template<typename T>
	using deque = std::deque<T, std::allocator<T> >;
}
