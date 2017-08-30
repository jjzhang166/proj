#pragma once
/*
@author zhp
@date 2017/7/12 11:07
@purpose for ac operate
*/
#include <mfc/MfcAcOper.h>
#include "resource.h"
#include "MapOper.h"

class DlgMapOper;
class MyAcOper : public MfcAcOperBase{
public:
	MyAcOper();
	DlgMapOper* GetDlgMapOper();
	
protected:
	BOOL OnInitDialog() override;

private:
	afx_msg void OnBtMapOper();
	afx_msg void OnDestroy();

private:
	DlgMapOper		dlg_map_oper_;

	DECLARE_MESSAGE_MAP()
};

class MyAcOperMgr : public MfcAcOperMgr{
public:
	MyAcOper* Find(const std::string& ac_item_key) const;

protected:
	UINT GetAcOperDlgTemplateId() const override;
	MyAcOper* MakeAcOper() override;
};