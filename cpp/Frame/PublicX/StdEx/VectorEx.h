#pragma once
/*
@author zhp
@date 2017/4/12 22:33
@purpose vector extention
*/
#include <vector>

namespace StdEx{
	template<typename T>
	using vector = std::vector<T, std::allocator<T> >;
}
