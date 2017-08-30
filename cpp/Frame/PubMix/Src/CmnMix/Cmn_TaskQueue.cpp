#include "stdafx.h"
#include <CmnMix/Cmn_TaskQueue.h>
#include <CommonX/CmnX_UsefulFunc.h>

namespace{
	int diable_waring = 0;
}

CmnTaskStaticQueue_Msg_HelperBase::stFuncInfo::stFuncInfo()
{
	Cmnx_MemSetValue(this, sizeof(*this), 0);
}

CmnX_CodeInfo CmnTaskStaticQueue_Msg_HelperBase::stFuncInfo::ToCodeInfo() const
{
	CmnX_CodeInfo res;
	res.begin_addr_ = (void*)(int)(short)offset_begin_;
	res.end_addr_ = (void*)(int)(short)offset_end_;
	return res;
}

CmnTaskStaticQueue_Msg_HelperBase::stFuncInfo::stFuncInfo(const stFuncInfo& rhs)
{
	Cmnx_CpyMem(this, &rhs, sizeof(*this));
}

CmnTaskStaticQueue_Msg_HelperBase::stFuncInfo& CmnTaskStaticQueue_Msg_HelperBase::stFuncInfo::operator = (const stFuncInfo& rhs)
{
	Cmnx_CpyMem(this, &rhs, sizeof(*this));
	return *this;
}