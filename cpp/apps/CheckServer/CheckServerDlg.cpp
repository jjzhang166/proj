
// CheckServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CheckServer.h"
#include "CheckServerDlg.h"
#include <boost/thread.hpp>
#include "DataTrans.h"
#include <Protocol/P_PriDefines.h>
#include "CS_CfgInfo.h"
#include "Dlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckServerDlg dialog



CCheckServerDlg::CCheckServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
}

void CCheckServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	auto& cfg = stCS_CfgInfo::GetMe();

	DDX_Text(pDX, IDC_EDIT_MINUS_CARD_TIME, cfg.decrement_time_);
	DDX_Radio(pDX, IDC_RADIO_AS_DAY, cfg.radio_time_unit_);
	DDX_Text(pDX, IDC_EDIT_VERSION, cfg.min_version_);
}

BEGIN_MESSAGE_MAP(CCheckServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_CREATE_IMPORT, &CCheckServerDlg::OnBtCreate_Import)
	ON_BN_CLICKED(IDC_BT_MINUS_CARD_DAYS, &CCheckServerDlg::OnBtMinusMinutes)
	ON_BN_CLICKED(IDC_RADIO_AS_DAY, &CCheckServerDlg::OnBnClickedRadioAsDay)
	ON_BN_CLICKED(IDC_RADIO_AS_MINUTE, &CCheckServerDlg::OnBnClickedRadioAsMinute)
	ON_BN_CLICKED(IDC_BT_SET_VERSION, &CCheckServerDlg::OnBtSetVersion)
END_MESSAGE_MAP()


// CCheckServerDlg message handlers

BOOL CCheckServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	auto cfg_info = &stCS_CfgInfo::GetMe();
	/*cfg_info.Save();
	return FALSE;*/
	if (cfg_info->singleton_event_name_.empty())
	{
		AfxMessageBox(_T("配置信息读取失败"));
		cfg_info->Save();
		return FALSE;
	}
	//验证唯一实例
	if ( !CheckSingletonHandle(cfg_info->singleton_event_name_) )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	AutoRunAtSystemBegin(cfg_info->singleton_event_name_, kEmptyStr);
	SetWindowText(CA2CT(cfg_info->wnd_title_.c_str()));

	ThreadablePtr io_thrd(new MyThrdIo);
	assert(io_thrd);
	GetMyApp().AddThrd(io_thrd, true);

	// TODO: Add extra initialization here
	GetIoService().post([this, cfg_info](){
		//DataExecH exec_helper;
		//DATA_EXCEPTION_BEGIN;
		const char* const kSqlSvrIp =
			//kSelfIpStr
			cfg_info->sql_svr_ip_.c_str()
			//"192.168.1.10"
			;

		LOG_O(Log_trace) << "connet to the db ip:" << kSqlSvrIp << "\tsql port:" << kSqlPort << "\tdb name:" << cfg_info->proj_db_name_
			<< "\tself svr port:" << cfg_info->self_svr_port_;
		db_singleton_.reset(new DbSingleton(kSqlSvrIp, kSqlPort, cfg_info->proj_db_name_));
	
		db_singleton_->OnReconnected();
		/*db_singleton_->SelectAllTblData();
		db_fifa_->SelectAllTblData();*/
		
		/*}DO_CATCH_THE_EXCEPTION( const Poco::Exception& e, {
			exec_helper();
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << e.displayText();
		})*/

		check_server_.reset(new CheckServer(db_singleton_, cfg_info->self_svr_port_));
	});

	//////////////////////////////////////////////////////////////////////////
	/*auto bt = GetDlgItem(IDC_BT_IMPORT_DB_DATA);
	if (bt)
		bt->EnableWindow(FALSE);
	bt = GetDlgItem(IDC_BT_CREATE_DB_TBL);
	if (bt)
		bt->EnableWindow(FALSE);*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheckServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCheckServerDlg::OnBtMinusMinutes()
{
	UpdateData();
	stCS_CfgInfo::GetMe().Save();
}

int CCheckServerDlg::CalcDecrementMinutes()
{
	const auto& cfg = stCS_CfgInfo::GetMe();
	if ( 0 == cfg.radio_time_unit_ )
		return cfg.decrement_time_ * 24 * 60;
	else
		return cfg.decrement_time_;
}


void CCheckServerDlg::OnBnClickedRadioAsDay()
{
	UpdateData();
	stCS_CfgInfo::GetMe().Save();
}


void CCheckServerDlg::OnBnClickedRadioAsMinute()
{
	UpdateData();
	stCS_CfgInfo::GetMe().Save();
}

void CCheckServerDlg::OnBtSetVersion()
{
	UpdateData();
	stCS_CfgInfo::GetMe().Save();
}

void CCheckServerDlg::OnBtCreate_Import()
{
	DlgMgrTbl dlg;
	dlg.DoModal();
}
