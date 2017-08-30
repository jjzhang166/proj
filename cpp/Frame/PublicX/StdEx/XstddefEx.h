#pragma once
/*
@author zhp
@date 2017/3/15 6:09
@purpose for xstddef
*/
#include <xstddef>
#include "TString.h"

namespace StdEx{
	template<typename CharT>
	struct LessChar : public std::binary_function<const CharT*, const CharT*, bool>{
		inline bool operator()(const CharT* left, const CharT* right) const{
			assert(left);
			assert(right);
			return TStrCmp(left, right) < 0;
		}
	};
}
