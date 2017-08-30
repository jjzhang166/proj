#include "stdafx.h"
#include "resource.h"
#include "MultiLogin.h"
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>
#include "ServerIpInfo.h"

BEGIN_MESSAGE_MAP(DlgMultiLogin, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(ListMultiLogin, CListCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_ADD_ACCOUNT, &ListMultiLogin::OnMenuAddAccount)
	ON_COMMAND(ID_MENU_DELETE_ACCOUNT, &ListMultiLogin::OnMenuDeleteAccount)
END_MESSAGE_MAP()

DlgMultiLogin::DlgMultiLogin() : CDialog(IDD_DLG_MULTI_LOGIN, AfxGetMainWnd())
{
}

void DlgMultiLogin::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MULTI_LOGIN, list_multi_login_);
}

BOOL DlgMultiLogin::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	return TRUE;
}

void DlgMultiLogin::OnOK()
{
	auto bt_ok = GetDlgItem(IDOK);
	if (bt_ok)
		bt_ok->EnableWindow(FALSE);
	GetIoService().post([this](){
		VMP_BEGIN_EX;
		auto& cfg = stSingletonCfg::GetMe();
		const auto kItemMax = list_multi_login_.GetItemCount();
		for ( int item = 0; item < kItemMax; ++item )
		{
			if ( !IsWindowVisible() || !IsWindow(m_hWnd) )
				break;
			if ( !list_multi_login_.GetCheck(item) )
				continue;
			ac_tmp_ = list_multi_login_.GetItemAccount(item);
			std::string ac = CT2CA(ac_tmp_);
			auto ac_pwd = cfg.FindByAc(ac);
			if ( !ac_pwd )
				continue;
			const DateTimeLocal& time_now = G_GetSerCurTime();
			if ( !ac_pwd->IsTheLoginSucceed() || ac_pwd->TestTimeEnd(time_now) )
			{
				auto check_login = CheckLoginC::GetSelfPtr();
				if ( !check_login )
					return;
				std::string ac = CT2CA(ac_tmp_);
				pwd_tmp_ = list_multi_login_.GetItemText(item, kLML_Pwd);
				std::string pwd = CT2CA(pwd_tmp_);
				auto& user_oper = check_login->GetUserOper();
				user_oper.Login(true, ac.c_str(), pwd.c_str(), kConsoleMinVer);
			}
		}
		if ( !IsWindow(m_hWnd) )
			return;
		auto bt_ok = GetDlgItem(IDOK);
		if (bt_ok)
			bt_ok->EnableWindow(TRUE);
		VMP_END;
	});	
}

void DlgMultiLogin::OnClose()
{
	__super::OnClose();
}

void DlgMultiLogin::OnLoginInfo( const std::string& res )
{
	VMP_BEGIN_EX;
	auto item = list_multi_login_.FindByAc(ac_tmp_);
	if ( item >= 0 )
		list_multi_login_.SetItemText(item, kLML_State, CA2CT(res.c_str()));
	VMP_END;
}

void ListMultiLogin::OnAdd( DlgAddLoginAc& dlg )
{
	auto& cfg = stSingletonCfg::GetMe();
	std::string ac = CT2CA(dlg.ac_);
	if (cfg.FindByAc(ac))
		return;
	if ( FindByAc(dlg.ac_) >= 0 )
		return;
	const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, dlg.ac_);
	SetItemText(kItemMax, kLML_Pwd, dlg.pwd_);
	SetCheck(kItemMax, TRUE);

	std::string pwd = CT2CA(dlg.pwd_);
	cfg.AddLoginInfo(ac, pwd);
}

void ListMultiLogin::OnMenuAddAccount()
{
	DlgAddLoginAc dlg(this);
	dlg.DoModal();

}

void ListMultiLogin::OnMenuDeleteAccount()
{
	auto pos = GetFirstSelectedItemPosition();
	std::vector<int> items;
	while (pos)
	{
		items.push_back(GetNextSelectedItem(pos));
	}
	std::reverse(items.begin(), items.end());
	auto& cfg = stSingletonCfg::GetMe();
	for ( const auto item : items )
	{
		std::string ac = CT2CA(GetItemAccount(item));
		cfg.DelAllByAc(ac);
		DeleteItem(item);
	}
}

void ListMultiLogin::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	//style
	{
		DWORD dwStyle = GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;		//选中某行使整行高亮（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_GRIDLINES;			//网格线（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_CHECKBOXES;			//item前生成checkbox控件
		SetExtendedStyle(dwStyle);	//设置扩展风格
	}

	const int kColumnWidth = 150;
	InsertColumn(kLML_Ac, _T("帐号"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLML_Pwd, _T("密码"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLML_State, _T("状态"), LVCFMT_LEFT, kColumnWidth * 2);

	VMP_BEGIN_EX;
	auto& cfg = stSingletonCfg::GetMe();
	for (auto& v : cfg.GetLoginInfos())
	{
		CString ac = CA2CT(v.ac_.c_str());
		if ( FindByAc(ac) >= 0 )
			return;
		const auto kItemMax = GetItemCount();
		InsertItem(kItemMax, ac);
		CString pwd = CA2CT(v.pwd_.c_str());
		SetItemText(kItemMax, kLML_Pwd, pwd);
		SetCheck(kItemMax, TRUE);
		//const DateTimeLocal& time_now = G_GetSerCurTime();
		//v.TestTimeEnd(time_now);
		if (v.IsTheLoginSucceed())
		{
			auto& login_info = v.GetLoginInfo();
			if ( !login_info.empty() )
			{
				SetItemText(kItemMax, kLML_State, CA2CT(login_info.c_str()));
			}
		}
	}
	VMP_END;
}

void ListMultiLogin::OnRButtonDown( UINT nFlags, CPoint point )
{
	__super::OnRButtonDown(nFlags, point);
	ShowPopupMenu(IDR_MENU_MULTI_LOGIN, this);
}

int ListMultiLogin::FindByAc( const CString& ac )
{
	const auto kItemMax = GetItemCount();
	for ( auto i = 0; i < kItemMax; ++i )
	{
		if ( GetItemAccount(i) == ac )
			return i;
	}
	return -1;
}

CString ListMultiLogin::GetItemAccount( int item ) const
{
	return GetItemText(item, kLML_Ac);
}
