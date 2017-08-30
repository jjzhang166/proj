#pragma once
/*
@author zhp
@date 2014/7/16 5:35
@purpose csviewer,list controls
*/
#include "stdafx.h"
#include "Common/UiCtrls.h"
#include <Net/DbData.h>

//card info
class ListCtrl_CardInfo : public CListCtrlStyled{
	//type definitions
public:
	enum enColumns{
		kCardId = 0,
		kCardPwd,
		kCardDays,
		kUseTime,
		kAddTime,
		kIsUsed,
		kUserName,
	};
public:
	void AddCardInfo(const stData_Card& card_info);

protected:
	virtual void PreSubclassWindow();
};

//account info
class ListCtrl_AcInfo : public CListCtrlStyled{
	//type definitions
public:
	enum enColumns{
		kUserName = 0,
		kUserPwd,
		kIsBindMachine,
		kLastLoginTime,
		kLastPayTime,
		kLastLoginIp,
		kRegTime,
		kEndDayTime,
		kQQ,
		kRegIp,
		kAcState,
		kBanEndTime,
	};

public:
	void AddAcInfo(const stData_Account& ac_info);
	void AddAcStateInfo(const stData_AcState& ac_state);

protected:
	virtual void PreSubclassWindow();
};