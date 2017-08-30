#pragma once
/*
@author zhp
@date 2014/7/16 5:39
@purpose for ui
*/
#include "stdafx.h"
#include <afxcmn.h>
#include "../DependSelfLib.h"

class CListCtrlStyled : public CListCtrl{
protected:
	virtual void PreSubclassWindow();
};
