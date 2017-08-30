#include "stdafx.h"
#include "LoginDlg.h"
#include "Protocol/P_Defines.h"
#include "resource.h"
#include "CSViewerDlg.h"
#include "Common/UsefulFunc.h"
#include "CSViewer.h"
#include "CfgStruct.h"

BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	
END_MESSAGE_MAP()

LoginDlg::LoginDlg() : CDialog(IDD_CSV_DLG_LOGIN, nullptr)
{
	is_logining_ = false;
	const auto& cfg = stSingletonCfg::GetMe();
	str_ac_ = cfg.authen_account_;
	str_pwd_ = cfg.authen_password_;
}

LoginDlg::~LoginDlg()
{
}

BOOL LoginDlg::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;

	auto& cfg_info = stCSV_SvrInfo::GetMe();
	if (cfg_info.singleton_event_name_.empty())
	{
		AfxMessageBox(_T("csv configure info read error2"));
		cfg_info.Save();
		return FALSE;
	}

	edit_ac_.SetLimitText(kUserNameMaxLen);
	edit_pwd_.SetLimitText(kUserPwdMaxLen);
	return TRUE;
}

void LoginDlg::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CSV_AC, str_ac_);
	DDX_Text(pDX, IDC_EDIT_CSV_PWD, str_pwd_);

	DDX_Control(pDX, IDC_EDIT_CSV_AC, edit_ac_);
	DDX_Control(pDX, IDC_EDIT_CSV_PWD, edit_pwd_);
}

void LoginDlg::OnOK()
{
	if (is_logining_)
		return;
	SetLogining(true);

	UpdateData();

	if ( str_ac_.GetLength() < kUserNameMinLen )
		return;
	if ( str_pwd_.GetLength() < kUserPwdMinLen )
		return;

	std::string ac = CT2CA(str_ac_);
	std::string pwd = CT2CA(str_pwd_);
	GetMyApp().SetUserInfo(ac, pwd);
	GetIoService().post([=](){
		auto connection = Connection_CS::GetSelfPtr();
		if (connection)
		{
			auto& user_oper = connection->GetUserOper();
			auto res = user_oper.Login(false, ac.c_str(), pwd.c_str(), 99999);
			auto dlg_login = LoginDlg::GetInstance();
			if (dlg_login)
				dlg_login->SetLogining(false);

			if (res)
			{
				if (dlg_login)
					dlg_login->LoginSucceed();
			}
		}
	});
}

void LoginDlg::SetLogining( bool logining )
{
	is_logining_ = logining;
	if (IsWindow(m_hWnd))
		return;
	auto bt_tmp = GetDlgItem(IDOK);
	if (bt_tmp)
		bt_tmp->EnableWindow(!logining);
}

void LoginDlg::LoginSucceed()
{
	EndDialog(IDOK);
}

const CString& LoginDlg::GetAc() const
{
	return str_ac_;
}

const CString& LoginDlg::GetAcPwd() const
{
	return str_pwd_;
}

void LoginDlg::SetWndText( const std::string& wnd_text )
{
	if (wnd_text.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "窗口的文字为空";
		return;
	}
	if ( !m_hWnd || !IsWindow(m_hWnd) )
	{
		return;
	}

	if (wnd_text_old_.IsEmpty())
	{
		GetWindowText(wnd_text_old_);
	}
	CString cstr_wnd_text = wnd_text_old_;
	cstr_wnd_text += _T("--->|*");
	cstr_wnd_text += CA2CT(wnd_text.c_str());
	cstr_wnd_text += _T("*|");
	SetWindowText(cstr_wnd_text);
}
