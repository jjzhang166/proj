#include "stdafx.h"
#include <Net/DbData.h>

void stData_UserGrant::SetUserGrant( enUserGrant user_grant )
{
	user_grant_ = user_grant;
}

enUserGrant stData_UserGrant::GetUserGrant() const
{
	return enUserGrant(user_grant_);
}

bool stData_UserGrant::operator<( const stData_UserGrant& rhs )
{
	return user_grant_ < rhs.user_grant_;
}
