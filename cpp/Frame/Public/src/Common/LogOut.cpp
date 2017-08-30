#include "stdafx.h"
#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>

std::ostream& operator<<(std::ostream& ostm, const CString& str)
{
	ostm << WideCharToGbk(std::wstring(str));
	return ostm;
}
