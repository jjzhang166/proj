#include "stdafx.h"
#include "MyAcOper.h"


BEGIN_MESSAGE_MAP(MyAcOper, MfcAcOperBase)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_MAP_OPER, &MyAcOper::OnBtMapOper)
END_MESSAGE_MAP()

void MyAcOper::OnBtMapOper()
{
	if (dlg_map_oper_.IsWindowVisible())
		return;
	if (!dlg_map_oper_.m_hWnd)
	{
		if (!dlg_map_oper_.Create(IDD_DLG_MAP_OPER, this))
		{
			assert(false);
			return;
		}
	}
	dlg_map_oper_.ShowWindow(SW_SHOW);
}

BOOL MyAcOper::OnInitDialog()
{
	if (!__super::OnInitDialog())
	{
		assert(false);
		return FALSE;
	}
	if (!dlg_map_oper_.Create(IDD_DLG_MAP_OPER, this))
	{
		assert(false);
		return FALSE;
	}
	return TRUE;
}

MyAcOper::MyAcOper() : dlg_map_oper_(this)
{
}

DlgMapOper* MyAcOper::GetDlgMapOper()
{
	if (dlg_map_oper_.IsWindowVisible())
		return &dlg_map_oper_;
	return nullptr;
}

void MyAcOper::OnDestroy()
{
	__super::OnDestroy();
	dlg_map_oper_.CloseWindow();
}

UINT MyAcOperMgr::GetAcOperDlgTemplateId() const
{
	return IDD_DLG_AC_OPER;
}

MyAcOper* MyAcOperMgr::MakeAcOper()
{
	return new MyAcOper;
}

MyAcOper* MyAcOperMgr::Find(const std::string& ac_item_key) const
{
	auto res = __super::Find(ac_item_key);
	if (!res) return nullptr;
	return (MyAcOper*)res;
}
