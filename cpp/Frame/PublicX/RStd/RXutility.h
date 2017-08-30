#pragma once
/*
@author zhp
@date 2017/2/27 6:37
@purpose for xutility release
*/
#include "RYvals.h"
#include <xutility>

__RSTD_BEGIN_

using _STD _Container_base0;
using _STD _Iterator_base0;
typedef _Container_base0 _Container_base;
typedef _Iterator_base0 _Iterator_base;
using _STD _Container_base12;
using _STD _Container_proxy;

#ifdef _DEBUG
using _STD _Debug_pointer;
#endif

template<typename T>
T to_std(const T&);

template<typename T>
T from_std(const T&);

_STD_END
