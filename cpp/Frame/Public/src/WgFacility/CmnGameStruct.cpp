#include "stdafx.h"
#include <WgFacility/CmnGameStruct.h>
#include <CmnMix/Cmn_UsefulFunc.h>

std::string GameWString::GetStdString() const
{
	return WideCharToGbk(*this);
}

std::wstring GameWString::GetStdStr() const
{
	return std::wstring(c_str(), size());
}

std::string GameString::GetStdString() const
{
	const BasicString* ptr = this;
	return RStd::to_std(*ptr);
}

std::ostream& operator<<(std::ostream& ostm, const GameWString& v)
{
	ostm << v.GetStdString();
	return ostm;
}
