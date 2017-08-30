
// CSViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSViewer.h"
#include "CSViewerDlg.h"
#include "afxdialogex.h"
#include <CmnMix/Cmn_TemplateUtility.h>
#include "CfgStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCSViewerDlg dialog

CCSViewerDlg::CCSViewerDlg(CWnd* pParent, UINT dlg_template_id )
	: CDialog(dlg_template_id, pParent)
{
	m_hIcon = GetMyApp().LoadIconW(IDR_MAINFRAME);
	is_doing_ = false;
	ban_time_ = 0;
}

void CCSViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER_AC, str_ac_);
	DDX_Text(pDX, IDC_EDIT_USER_AC_PWD, str_ac_pwd_);
	DDX_Text(pDX, IDC_EDIT_CARD_ID, str_card_id_);
	DDX_Text(pDX, IDC_EDIT_CARD_PWD, str_card_pwd_);
	DDX_Control(pDX, IDC_EDIT_USER_AC, edit_ac_);
	DDX_Control(pDX, IDC_EDIT_USER_AC_PWD, edit_ac_pwd_);
	DDX_Control(pDX, IDC_EDIT_CARD_ID, edit_card_id_);
	DDX_Control(pDX, IDC_EDIT_CARD_PWD, edit_card_pwd_);
	DDX_Control(pDX, IDC_LIST_CARD_INFO, list_ctrl_card_);
	DDX_Control(pDX, IDC_LIST_AC_INFO, list_ctrl_ac_);
	DDX_Radio(pDX, IDC_RADIO_AS_DAY, stSingletonCfg::GetMe().radio_time_unit_);
	DDX_Text(pDX, IDC_EDIT_BAN_TIME, ban_time_);
	DDX_Control(pDX, IDC_COMBO_AC_STATE, com_ac_state_);
}

BEGIN_MESSAGE_MAP(CCSViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_DO_REMOVE_BIND, &CCSViewerDlg::OnBtDoRemoveBind)
	ON_BN_CLICKED(IDC_BT_PEER_AC_INFO, &CCSViewerDlg::OnBtPeerAcInfo)
	ON_BN_CLICKED(IDC_BT_PEER_CARD_INFO, &CCSViewerDlg::OnBtPeerCardInfo)
	ON_BN_CLICKED(IDC_BT_PEER_AC_BY_CARD, &CCSViewerDlg::OnBtPeerAcByCard)
	ON_BN_CLICKED(IDC_BT_PEER_CARD_BY_AC, &CCSViewerDlg::OnBtPeerCardByAc)
	ON_BN_CLICKED(IDC_RADIO_AS_DAY, &OnBnClickedRadioAsDay)
	ON_BN_CLICKED(IDC_RADIO_AS_MINUTE, &OnBnClickedRadioAsMinute)
	ON_BN_CLICKED(IDC_BT_FN_MORE, &OnBtFnMore)
	ON_BN_CLICKED(IDC_BT_SET_AC_STATE, &OnBtSetAcState)
END_MESSAGE_MAP()


// CCSViewerDlg message handlers

BOOL CCSViewerDlg::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	auto& cfg_info = stCSV_SvrInfo::GetMe();
	if (cfg_info.singleton_event_name_.empty())
	{
		AfxMessageBox(_T("csv configure info read error1"));
		cfg_info.Save();
		return FALSE;
	}
	SetWindowText(CA2CT(cfg_info.wnd_title_.c_str()));
	// TODO: Add extra initialization here
	edit_ac_.LimitText(kUserNameMaxLen);
	edit_ac_pwd_.LimitText(kUserPwdMaxLen);
	edit_card_id_.LimitText(kCardIdLen);
	edit_card_pwd_.LimitText(kCardPwdLen);

	com_ac_state_.InsertString((int)enAcState::kAS_Normal, _T("恢复正常"));
	com_ac_state_.InsertString((int)enAcState::kAS_Freeze, _T("冻结"));
	com_ac_state_.InsertString((int)enAcState::kAS_Ban, _T("封号"));
	com_ac_state_.SetCurSel((int)enAcState::kAS_Normal);

#ifndef CSV_FULL_FUNCTIONS
	auto bt_tmp = GetDlgItem(IDC_BT_FN_MORE);
	if (bt_tmp)
		bt_tmp->ShowWindow(SW_HIDE);
#endif

	GainLocalInfo::TimerCheck();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCSViewerDlg::OnPaint()
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
		__super::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCSViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCSViewerDlg::SetWndText( pt_csz wnd_text )
{
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
	cstr_wnd_text += CA2CT(wnd_text);
	cstr_wnd_text += _T("*|");
	SetWindowText(cstr_wnd_text);
}

void CCSViewerDlg::OnBtDoRemoveBind()
{
	if (is_doing_)
		return;
	UpdateData();
	if ( str_ac_.GetLength() < kUserNameMinLen )
	{
		return;
	}
	SetDoing(true);
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([]( Connection_CS::SelfPtr& conne ){
			auto dlg = CCSViewerDlg::GetInstance();
			if ( !dlg )
			{
				assert(false);
				return;
			}
			std::string user_name_dst = CT2CA(dlg->str_ac_);
			conne->GetUserOper().Grant_RemoveBind(user_name_dst.c_str());
		});
		SetDoing(false);
	});
}

void CCSViewerDlg::SetDoing( bool is_doing )
{
	is_doing_ = is_doing;
	if ( !IsWindow(m_hWnd) )
		return;
	auto bt_tmp = GetDlgItem(IDC_BT_DO_REMOVE_BIND);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PEER_AC_INFO);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PEER_CARD_INFO);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PEER_AC_BY_CARD);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PEER_CARD_BY_AC);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_FN_MORE);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_SET_AC_STATE);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);
}

void DlgMore::OnBtOpenCards()
{
	auto dlg = CCSViewerDlg::GetInstance();
	if (dlg)
		dlg->UpdateData();
	UpdateData();
	SetDoing(true);
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([]( Connection_CS::SelfPtr& conne ){
			auto dlg = DlgMore::GetInstance();
			if ( !dlg )
			{
				assert(false);
				return;
			}
			int minutes = 0;
			auto& cfg = stSingletonCfg::GetMe();
			if ( 0 == cfg.radio_time_unit_ )
				minutes = dlg->cards_unit_ * 24 * 60;
			else
				minutes = dlg->cards_unit_;
			conne->GetUserOper().Grant_OpenCard(dlg->cards_cnt_, minutes, dlg->GetCardGrant(), dlg->ac_type_);
		});
		SetDoing(false);
	});
}

void DlgMore::OnBtGenRegKey()
{
	auto dlg = CCSViewerDlg::GetInstance();
	if (dlg)
		dlg->UpdateData();
	UpdateData();
	SetDoing(true);
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([]( Connection_CS::SelfPtr& conne ){
			auto dlg = DlgMore::GetInstance();
			if ( !dlg )
			{
				assert(false);
				return;
			}
			conne->GetUserOper().Grant_GenRegKey(dlg->reg_key_cnt_, dlg->ac_type_);
		});
		SetDoing(false);
	});
}

void CCSViewerDlg::OnBnClickedRadioAsDay()
{
	UpdateData();
}

void CCSViewerDlg::OnBnClickedRadioAsMinute()
{
	UpdateData();
}

Poco::UInt64 DlgMore::GetCardGrant() const
{
	return card_grant_;
}

void DlgMore::OnBnClickedPaySome()
{
	CFileDialog file_dlg(TRUE, nullptr, _T("*.lua"), OFN_HIDEREADONLY, 
		_T("*.lua") _T("|")_T("*.lua") _T("||"));
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		if (PathFileExists(file_path))
		{
			auto dlg = CCSViewerDlg::GetInstance();
			if (dlg)
				dlg->UpdateData();
			UpdateData();
			if (dlg->str_ac_.IsEmpty())
			{
				LOG_O(Log_debug) << "被充值者名字为空";
				return;
			}

			SetDoing(true);

			std::string full_file_name = CT2CA(file_path);

			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "批量使用卡，文件：" << full_file_name << " 被充值者：" << dlg->str_ac_;
			GetIoService().post([=](){
				Connection_CS::AfterLoginSucced([full_file_name, this]( Connection_CS::SelfPtr& conne ){
					std::ifstream ifs(full_file_name);
					std::string card_id, card_pwd;
					std::vector<SomeCardsInfo> cards_info;
					int card_idx = 1;
					while (!!ifs)
					{
						card_id.clear();
						card_pwd.clear();
						std::getline(ifs, card_id, ' ');
						std::getline(ifs, card_pwd, '\n');
						if ( !card_id.empty() && !card_pwd.empty() )
						{
							SomeCardsInfo tmp;
							tmp.card_id_ = card_id;
							tmp.card_pwd_ = card_pwd;
							tmp.card_idx_ = card_idx++;
							cards_info.push_back(tmp);
						}
					}
					std::sort(cards_info.begin(), cards_info.end());
					auto it = Unique(cards_info.begin(), cards_info.end());
					if ( it != cards_info.end() )
					{
						for ( auto it_new = it; it_new != cards_info.end(); ++it_new )
						{
							LOG_O(Log_debug) << "存在重复的卡，卡号：" << it_new->card_id_;
						}
						//cards_info.erase(it, cards_info.end());
						return;
					}
					std::sort(cards_info.begin(), cards_info.end(), []( const SomeCardsInfo& lhs, const SomeCardsInfo& rhs ){
						return lhs.card_idx_ < rhs.card_idx_;
					});
					int err_cnt = 0, valid_cnt = 0;
					auto dlg = CCSViewerDlg::GetInstance();
					if (!dlg)
					{
						assert(false);
						return;
					}
					std::string user_name = CT2CA(dlg->str_ac_);
					for ( auto& v : cards_info )
					{
						bool res = false;
						if ( !conne || !conne->socket().is_open() )
							break;
						res = conne->GetUserOper().UserPay(user_name.c_str(), v.card_id_.c_str(), v.card_pwd_.c_str());
						if ( !res )
						{
							++err_cnt;
							LOG_O(Log_debug) << "第[" << v.card_idx_ << "]个卡充值失败，卡号：" << v.card_id_ << " 卡密：" << v.card_pwd_;
						}
						else
						{
							++valid_cnt;
						}
					}
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "充值成功次数：" << valid_cnt << " 失败次数：" << err_cnt << " 总次数：" << valid_cnt + err_cnt;
				});

				SetDoing(false);
				auto dlg_more = DlgMore::GetInstance();
				if (dlg_more)
				{
					dlg_more->SetWndText("批量使用卡结束，请看LOG输出");
				}
			});
		}
	}
}

void DlgMore::OnBtRollbackSomePay()
{
	CFileDialog file_dlg(TRUE, nullptr, _T("*.lua"), OFN_HIDEREADONLY, 
		_T("*.lua") _T("|")_T("*.lua") _T("||"));
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		if (PathFileExists(file_path))
		{
			SetDoing(true);

			std::string full_file_name = CT2CA(file_path);

			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "批量充值回滚，文件：" << full_file_name;
			GetIoService().post([=](){
				Connection_CS::AfterLoginSucced([full_file_name]( Connection_CS::SelfPtr& conne ){
					std::ifstream ifs(full_file_name);
					std::string card_id, card_pwd;
					std::vector<SomeCardsInfo> cards_info;
					int card_idx = 1;
					while (!!ifs)
					{
						card_id.clear();
						card_pwd.clear();
						std::getline(ifs, card_id, ' ');
						std::getline(ifs, card_pwd, '\n');
						if ( !card_id.empty() && !card_pwd.empty() )
						{
							SomeCardsInfo tmp;
							tmp.card_id_ = card_id;
							tmp.card_pwd_ = card_pwd;
							tmp.card_idx_ = card_idx++;
							cards_info.push_back(tmp);
						}
					}
					std::sort(cards_info.begin(), cards_info.end());
					auto it = Unique(cards_info.begin(), cards_info.end());
					if ( it != cards_info.end() )
					{
						for ( auto it_new = it; it_new != cards_info.end(); ++it_new )
						{
							LOG_O(Log_debug) << "存在重复的卡，卡号：" << it_new->card_id_;
						}
						//cards_info.erase(it, cards_info.end());
						return;
					}
					std::sort(cards_info.begin(), cards_info.end(), []( const SomeCardsInfo& lhs, const SomeCardsInfo& rhs ){
						return lhs.card_idx_ < rhs.card_idx_;
					});
					int err_cnt = 0, valid_cnt = 0;
					auto& cfg = stSingletonCfg::GetMe();
					for ( auto& v : cards_info )
					{
						bool res = false;
						if ( !conne || !conne->socket().is_open() )
							break;
						res = conne->GetUserOper().Grant_RollbackPay(v.card_id_.c_str(), v.card_pwd_.c_str(), cfg.rollback_direction_);
						if ( !res )
						{
							++err_cnt;
							LOG_O(Log_debug) << "第[" << v.card_idx_ << "]个卡充值回滚失败，卡号：" << v.card_id_ << " 卡密：" << v.card_pwd_;
						}
						else
						{
							++valid_cnt;
						}
					}
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "充值回滚失成功次数：" << valid_cnt << " 失败次数：" << err_cnt << " 总次数：" << valid_cnt + err_cnt;
				});

				SetDoing(false);
				auto dlg = CCSViewerDlg::GetInstance();
				if (dlg)
				{
					dlg->SetWndText("批量充值回滚结束，请看LOG输出");
				}
			});
		}
	}
}

void DlgMore::OnBtProjDatas()
{
	DlgSetProjDatas dlg(this);
	dlg.DoModal();
}


void CCSViewerDlg::OnBtPeerAcInfo()
{
	if (is_doing_)
		return;
	UpdateData();
	auto size_tmp = str_ac_.GetLength();
	if ( size_tmp > kUserNameMaxLen || size_tmp < kUserNameMinLen )
		return;
	size_tmp = str_ac_pwd_.GetLength();
	if ( size_tmp > kUserPwdMaxLen || size_tmp < kUserPwdMinLen )
		return;
	SetDoing(true);

	std::string ac = CT2CA(str_ac_);
	std::string ac_pwd = CT2CA(str_ac_pwd_);
	list_ctrl_ac_.DeleteAllItems();
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([&ac, &ac_pwd]( Connection_CS::SelfPtr& conne ){
			auto dlg = CCSViewerDlg::GetInstance();
			if ( !dlg )
			{
				assert(false);
				return;
			}
			conne->GetUserOper().Grant_PeerAcInfo(ac.c_str(), ac_pwd.c_str());
		});
		SetDoing(false);
	});
}


void CCSViewerDlg::OnBtPeerCardInfo()
{
	if (is_doing_)
		return;
	UpdateData();
	if ( str_card_id_.GetLength() < 1 )
		return;
	if ( str_card_pwd_.GetLength() < 1 )
		return;

	SetDoing(true);

	std::string card_id = CT2CA(str_card_id_);
	std::string card_pwd = CT2CA(str_card_pwd_);
	list_ctrl_card_.DeleteAllItems();
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([&card_id, &card_pwd, this]( Connection_CS::SelfPtr& conne ){
			auto dlg = CCSViewerDlg::GetInstance();
			if ( !dlg )
			{
				assert(false);
				return;
			}
			conne->GetUserOper().Grant_PeerCardInfo(card_id.c_str(), card_pwd.c_str());
		});
		SetDoing(false);
	});
}


void CCSViewerDlg::OnBtPeerAcByCard()
{
	if (is_doing_)
		return;
	UpdateData();
	if ( str_card_id_.GetLength() < 1 )
		return;
	if ( str_card_pwd_.GetLength() < 1 )
		return;
	SetDoing(true);

	std::string card_id = CT2CA(str_card_id_);
	std::string card_pwd = CT2CA(str_card_pwd_);
	list_ctrl_ac_.DeleteAllItems();
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([&card_id, &card_pwd]( Connection_CS::SelfPtr& conne ){
			conne->GetUserOper().Grant_PeerAcByCard(card_id.c_str(), card_pwd.c_str());
		});
		SetDoing(false);
	});
}


void CCSViewerDlg::OnBtPeerCardByAc()
{
	if (is_doing_)
		return;
	UpdateData();
	auto size_tmp = str_ac_.GetLength();
	if ( size_tmp > kUserNameMaxLen || size_tmp < kUserNameMinLen )
		return;
	size_tmp = str_ac_pwd_.GetLength();
	if ( size_tmp > kUserPwdMaxLen || size_tmp < kUserPwdMinLen )
		return;
	SetDoing(true);

	std::string ac = CT2CA(str_ac_);
	std::string ac_pwd = CT2CA(str_ac_pwd_);
	list_ctrl_card_.DeleteAllItems();
	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([&ac, &ac_pwd]( Connection_CS::SelfPtr& conne ){
			conne->GetUserOper().Grant_PeerCardByAc(ac.c_str(), ac_pwd.c_str());
		});
		SetDoing(false);
	});
}

ListCtrl_CardInfo& CCSViewerDlg::GetList_Card()
{
	return list_ctrl_card_;
}

ListCtrl_AcInfo& CCSViewerDlg::GetList_Ac()
{
	return list_ctrl_ac_;
}

void CCSViewerDlg::OnBtFnMore()
{
	DlgMore dlg(nullptr);
	dlg.DoModal();
}

void CCSViewerDlg::OnBtSetAcState()
{
	UpdateData();
	if (str_ac_.IsEmpty())
	{
		SetWndText("帐号不能为空");
		return;
	}
	std::string ac = CT2CA(str_ac_);
	auto ac_state = (enAcState)com_ac_state_.GetCurSel();

	pt_dword minutes = 0;
	auto& cfg = stSingletonCfg::GetMe();
	if (0 == cfg.radio_time_unit_)
		minutes = ban_time_ * 24 * 60;
	else
		minutes = ban_time_;

	SetDoing(true);

	GetIoService().post([=](){
		Connection_CS::AfterLoginSucced([=](Connection_CS::SelfPtr& conne){
			conne->GetUserOper().Grant_SetAcState(ac.c_str(), ac_state, minutes);
		});
		SetDoing(false);
	});

}

void CCSViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (list_ctrl_card_.m_hWnd && list_ctrl_ac_.m_hWnd && nType != SIZE_MINIMIZED && cx != 0 && cy != 0)
	{
		CRect rc;
		list_ctrl_card_.GetWindowRect(rc);
		if (!rc.IsRectEmpty())
		{
			list_ctrl_card_.SetWindowPos(NULL, rc.left, rc.top, cx - 10, rc.Height(), SWP_NOMOVE);
		}

		list_ctrl_ac_.GetWindowRect(rc);
		if (!rc.IsRectEmpty())
		{
			list_ctrl_ac_.SetWindowPos(NULL, rc.left, rc.top, cx - 10, rc.Height(), SWP_NOMOVE);
		}
	}
}

DlgMore::SomeCardsInfo::SomeCardsInfo()
{
	card_idx_ = -1;
}

BEGIN_MESSAGE_MAP(DlgSetProjDatas, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_PROJ_DATAS_ADD, &DlgSetProjDatas::OnBtAddProjDataItem)
END_MESSAGE_MAP()

void DlgSetProjDatas::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATA_TYPE, data_type_);
	DDX_Control(pDX, IDC_LIST_PROJ_DATAS, data_info_);
	DDX_Text(pDX, IDC_EDIT_PROJ_DATAS_ID, edit_id_);
	DDX_Text(pDX, IDC_EDIT_PROJ_DATAS_DATA, edit_data_);
}

BOOL DlgSetProjDatas::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	data_type_.InsertString(0, _T("AddrData"));
	data_type_.InsertString(1, _T("StrData"));
	data_type_.SetCurSel(0);

	data_info_.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 100);
	data_info_.InsertColumn(1, _T("数据"), LVCFMT_LEFT, 520);
	return TRUE;
}

void DlgSetProjDatas::OnBtAddProjDataItem()
{
	UpdateData();
	const auto kItemMax = data_info_.GetItemCount();
	data_info_.InsertItem(kItemMax, edit_id_);
	data_info_.SetItemText(kItemMax, 1, edit_data_);
}

void DlgSetProjDatas::OnOK()
{
	UpdateData();

	auto cur_sel = data_type_.GetCurSel();
	if ( 0 == cur_sel )
	{
		std::vector<std::pair<int, int> > addr_datas;
		const auto kItemMax = data_info_.GetItemCount();
		for ( int i = 0; i < kItemMax; ++i )
		{
			std::pair<int, int> data;
			data.first = boost::lexical_cast<int>(CT2CA(data_info_.GetItemText(i, 0)));
			data.second = boost::lexical_cast<DWORD>(CT2CA(data_info_.GetItemText(i, 1)));
			addr_datas.push_back(data);
		}
		if (addr_datas.empty())
			return;
		SetDoing(true);
		GetIoService().post([=](){
			Connection_CS::AfterLoginSucced([addr_datas]( Connection_CS::SelfPtr& conne ){
				if ( !conne )
				{
					assert(false);
					return;
				}
				int succeed_cnt = 0, err_cnt = 0;
				for ( auto& v : addr_datas )
				{
					if (conne->GetUserOper().Grant_SetAddrData(v.first, v.second))
						succeed_cnt++;
					else
						err_cnt++;
				}
				LOG_O(Log_trace) << "设置addr data，成功次数：" << succeed_cnt << " 失败次数：" << err_cnt;
			});

			SetDoing(false);
		});
	}
	else if ( 1 == cur_sel )
	{
		std::vector<std::pair<int, std::string> > str_datas;
		const auto kItemMax = data_info_.GetItemCount();
		for ( int i = 0; i < kItemMax; ++i )
		{
			std::pair<int, std::string> data;
			data.first = boost::lexical_cast<int>(CT2CA(data_info_.GetItemText(i, 0)));
			data.second = CT2CA(data_info_.GetItemText(i, 1));
			str_datas.push_back(data);
		}
		if (str_datas.empty())
			return;
		SetDoing(true);
		GetIoService().post([=](){
			Connection_CS::AfterLoginSucced([str_datas]( Connection_CS::SelfPtr& conne ){
				if ( !conne )
				{
					assert(false);
					return;
				}
				int succeed_cnt = 0, err_cnt = 0;
				for ( auto& v : str_datas )
				{
					if (conne->GetUserOper().Grant_SetStrData(v.first, v.second.c_str()))
						succeed_cnt++;
					else
						err_cnt++;
				}
				LOG_O(Log_trace) << "设置str data，成功次数：" << succeed_cnt << " 失败次数：" << err_cnt;
			});

			SetDoing(false);
		});
	}
}

void DlgSetProjDatas::SetDoing( bool is_doing )
{
	if ( !IsWindow(m_hWnd) )
		return;
	auto bt_tmp = GetDlgItem(IDOK);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PROJ_DATAS_ADD);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);
}

DlgSetProjDatas::DlgSetProjDatas( CWnd* pParent /*= NULL*/, UINT dlg_template_id /*= IDD_DLG_SET_PROJ_DATAS */ ) : CDialog(dlg_template_id, pParent)
{

}

// DlgMore dialog

IMPLEMENT_DYNAMIC(DlgMore, CDialog)

DlgMore::DlgMore(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMore::IDD, pParent)
{
	cards_cnt_ = 0;
	cards_unit_ = 0;
	reg_key_cnt_ = 0;
	card_grant_ = 0;
	ac_type_ = 0;
}

DlgMore::~DlgMore()
{
}

void DlgMore::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPEN_CARD_CNT, cards_cnt_);
	DDX_Text(pDX, IDC_EDIT_CARD_DAYS, cards_unit_);
	DDX_Text(pDX, IDC_EDIT_REG_KEY_CNT, reg_key_cnt_);
	DDX_Text(pDX, IDC_EDIT_CARD_GRANT, card_grant_);
	DDX_Text(pDX, IDC_EDIT_AC_TYPE, ac_type_);
}


BEGIN_MESSAGE_MAP(DlgMore, CDialog)
	ON_BN_CLICKED(IDC_BT_OPEN_CARDS, &OnBtOpenCards)
	ON_BN_CLICKED(IDC_BT_GEN_REG_KEY, &OnBtGenRegKey)
	ON_BN_CLICKED(IDC_BT_PAY_SOME, &OnBnClickedPaySome)
	ON_BN_CLICKED(IDC_BT_ROLLBACK_SOME_PAY, &OnBtRollbackSomePay)
	ON_BN_CLICKED(IDC_BT_PROJ_DATAS, &OnBtProjDatas)
END_MESSAGE_MAP()

void DlgMore::SetDoing(bool is_doing)
{
	auto bt_tmp = GetDlgItem(IDC_BT_OPEN_CARDS);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_GEN_REG_KEY);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_PAY_SOME);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);

	bt_tmp = GetDlgItem(IDC_BT_ROLLBACK_SOME_PAY);
	if (bt_tmp)
		bt_tmp->EnableWindow(!is_doing);
}

void DlgMore::SetWndText(pt_csz wnd_text)
{
	if (!m_hWnd || !IsWindow(m_hWnd))
	{
		return;
	}
	SetWindowText(CA2CT(wnd_text));
}


// DlgMore message handlers
