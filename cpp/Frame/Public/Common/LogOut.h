#pragma once
/*
@author zhp
@date 2014-1-09
@purpose log support
*/
#include <CmnMix/Cmn_LogOut.h>
#include "../DependSelfLib.h"
#include <atlstr.h>

std::ostream& operator << (std::ostream& ostm, const CString& str);