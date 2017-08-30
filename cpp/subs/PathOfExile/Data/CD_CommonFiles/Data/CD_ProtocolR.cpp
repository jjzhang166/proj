#include "stdafx.h"
#include "CD_ProtocolR.h"
#include <CmnMix/Cmn_LogOut.h>

stCDR_PromptInfo::stCDR_PromptInfo()
{
	CD_ZERO_MSGR();
}

stCDR_OnHpMp::stCDR_OnHpMp()
{
	CD_ZERO_MSGR();
}

bool stCDR_OnHpMp::IsDied() const
{
	if (kVT_Hp == value_type_ && 0 == ukn2_ && 0 == ukn_b1_)
	{
		if (0 == cur_value_)
			return true;
	}
	return false;
}

stCDGR_OnMove::stCDGR_OnMove()
{
	ZeroMemory((char*)this + sizeof(stCD_GameEntityMsg), sizeof(*this) - sizeof(stCD_GameEntityMsg));
	flag_ = kTTSF_Normal;
}

stCDR_PreGamePromptInfo::stCDR_PreGamePromptInfo()
{
	CD_ZERO_MSGR();
}

int stCDGR_PromptInfo::GetRemaining() const
{
	if (!values_.IsValid())
	{
		assert(false);
		return -1;
	}
	if (values_.size() < 2)
	{
		assert(false);
		return -1;
	}
	return values_[1];
}
