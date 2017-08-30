#include "stdafx.h"
#include <Protocol/P_Defines.h>
#include <Common/UsefulFunc.h>

const std::string kStrPlaceholder = RandomUniqueStr();
//const std::wstring kWStrPlaceholder = RandomUniqueWStr();

bool P_StringCheck::ValidUserName( pt_csz user_name )
{
	auto len = StdEx::TStrLen(user_name);
	if ( len > kUserNameMaxLen || len < kUserNameMinLen )
		return false;
	return true;
}

bool P_StringCheck::ValidUserPwd( pt_csz user_pwd )
{
	auto len = StdEx::TStrLen(user_pwd);
	if ( len > kUserPwdMaxLen || len < kUserPwdMinLen )
		return false;
	return true;
}

bool P_StringCheck::ValidMachineCode( pt_csz machine_code )
{
	auto len = StdEx::TStrLen(machine_code);
	if (len != kMachineCodeLen)
		return false;
	return true;
}

bool P_StringCheck::ValidMsgBoard( pt_csz msg )
{
	auto len = StdEx::TStrLen(msg);
	if ( len <= 0 || len > kMsgBoardMsgMaxLen )
		return false;
	return true;
}

bool P_StringCheck::ValidQQ( pt_csz qq )
{
	auto len = StdEx::TStrLen(qq);
	if ( len > kQQMaxLen || len < kQQMinLen )
		return false;
	return true;
}

bool P_StringCheck::ValidCard( pt_csz str )
{
	auto len = StdEx::TStrLen(str);
	if ( len != kCardIdLen && len != kCardIdLenEx )
		return false;
	return true;
}

bool P_StringCheck::ValidStrData( pt_csz str )
{
	if ( StdEx::TStrLen(str) > kTblStrDataMaxLen )
		return false;
	return true;
}

bool P_StringCheck::ValidCookie( pt_csz cookie )
{
	auto len = StdEx::TStrLen(cookie);
	if ( 0 != len && kCookieStrLen != len )
		return false;
	return true;
}
