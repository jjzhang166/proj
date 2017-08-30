#include "stdafx.h"
#include "Protect.h"

template<>
const char* VMProtectDecryptString_Helper_( const char* str )
{
	return VMProtectDecryptStringA(str);
}

template<>
const wchar_t* VMProtectDecryptString_Helper_( const wchar_t* str )
{
	return VMProtectDecryptStringW(str);
}

#ifdef __VMP__
void __VMProtectEnd_HELPER__( int* a )
{
	int tmp = 0;
	a = (int*)((int)&tmp + (int)(void*)a);
	*a += 2;
}
#endif

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
