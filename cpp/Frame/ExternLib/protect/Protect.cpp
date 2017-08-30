#include "stdafx.h"
#include "Protect.h"

__VMProtectBegin__::__VMProtectBegin__( const char* str_id )
{
	VMP_BEGIN_P(str_id);
}

__VMProtectBegin__::~__VMProtectBegin__()
{
	VMP_END;
}

__VMProtectBeginUltra__::__VMProtectBeginUltra__( const char* str_id )
{
	VMP_BEGIN_EX_P(str_id);
}

__VMProtectBeginUltra__::~__VMProtectBeginUltra__()
{
	VMP_END;
}
