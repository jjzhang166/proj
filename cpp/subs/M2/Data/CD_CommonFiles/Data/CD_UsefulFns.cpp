#include "stdafx.h"
#include "CD_UsefulFns.h"
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_Const.h"
#include <StdEx/RttiEx.h>

pt_csz CD_TypeName(pt_pcvoid obj_addr)
{
	return CD_SafeName(StdEx::RttiEx::TypeName(obj_addr));
}

pt_csz CD_VtblTypeName(pt_pcvoid vtbl)
{
	return CD_SafeName(StdEx::RttiEx::VtblTypeName(vtbl));
}
