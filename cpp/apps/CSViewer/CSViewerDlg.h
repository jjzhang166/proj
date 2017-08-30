
// CSViewerDlg.h : header file
//

#pragma once
#include "CheckServerC.h"
#include <Common/UsefulClass.h>
#include "ListCtrls.h"

// CCSViewerDlg dialog
class CCSViewerDlg : public CDialog, public Singleton<CCSViewerDlg, Singleton_Instance>{
	friend class DlgMore;

// Construction
public:
	CCSViewerDlg(CWnd* pParent = NULL, UINT dlg_template_id = IDD_CSVIEWER_DIALOG_FULL);	// standard constructor

public:
	//set wnd text
	void SetWndText( pt_csz wnd_text );
	void SetDoing( bool is_doing );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	//interface
public:
	ListCtrl_CardInfo& GetList_Card();
	ListCtrl_AcInfo& GetList_Ac();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtDoRemoveBind();
	afx_msg void OnBtPeerAcInfo();
	afx_msg void OnBtPeerCardInfo();
	afx_msg void OnBtPeerAcByCard();
	afx_msg void OnBtPeerCardByAc();
	afx_msg void OnBnClickedRadioAsDay();
	afx_msg void OnBnClickedRadioAsMinute();
	afx_msg void OnBtFnMore();
	afx_msg void OnBtSetAcState();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CString					wnd_text_old_;
	bool					is_doing_;

protected:
	CString					str_ac_;
	CString					str_ac_pwd_;
	CString					str_card_id_;
	CString					str_card_pwd_;
	int						ban_time_;

private:
	CEdit					edit_ac_;
	CEdit					edit_ac_pwd_;
	CEdit					edit_card_id_;
	CEdit					edit_card_pwd_;
	ListCtrl_CardInfo		list_ctrl_card_;
	ListCtrl_AcInfo			list_ctrl_ac_;
	CComboBox				com_ac_state_;
};

class DlgSetProjDatas : public CDialog{
	// Construction
public:
	DlgSetProjDatas(CWnd* pParent = NULL, UINT dlg_template_id = IDD_DLG_SET_PROJ_DATAS );	// standard constructor

public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	afx_msg void OnBtAddProjDataItem();
	DECLARE_MESSAGE_MAP()
	void OnOK();
	void SetDoing( bool is_doing );

private:
	CComboBox			data_type_;
	CListCtrl			data_info_;
	CString				edit_id_;
	CString				edit_data_;
};

// DlgMore dialog

class DlgMore : public CDialog, public Singleton<DlgMore, Singleton_Instance>
{
public:
	struct SomeCardsInfo{
		std::string		card_id_;
		std::string		card_pwd_;
		int				card_idx_;
		friend bool operator == (const SomeCardsInfo& lhs, const SomeCardsInfo& rhs){
			return lhs.card_id_ == rhs.card_id_;
		}
		friend bool operator < (const SomeCardsInfo& lhs, const SomeCardsInfo& rhs){
			return lhs.card_id_ < rhs.card_id_;
		}

		SomeCardsInfo();
	};

	DECLARE_DYNAMIC(DlgMore)

public:
	DlgMore(CWnd* pParent);   // standard constructor
	virtual ~DlgMore();

// Dialog Data
	enum { IDD = IDD_DLG_MORE };

public:
	void SetDoing(bool is_doing);
	unsigned __int64 GetCardGrant() const;
	void SetWndText(pt_csz wnd_text);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBtOpenCards();
	//生成注册码
	afx_msg void OnBtGenRegKey();
	afx_msg void OnBnClickedPaySome();
	afx_msg void OnBtRollbackSomePay();
	afx_msg void OnBtProjDatas();
	DECLARE_MESSAGE_MAP()

private:
	short					reg_key_cnt_;			//注册码数量
	short					cards_cnt_;				//开卡数量
	int						cards_unit_;			//卡的时间
	unsigned __int64		card_grant_;
	int						ac_type_;
};
