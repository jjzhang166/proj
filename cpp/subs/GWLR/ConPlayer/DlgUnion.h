#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <string>
#include "Common/UsefulClass.h"
#include "CfgStruct.h"

class DlgUseful : public CDialog{
public:
	explicit DlgUseful( LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr );
	explicit DlgUseful( UINT nIDTemplate, CWnd* pParentWnd = nullptr );

public:
	//get string from edit
	static bool GetStringFromEdit( CEdit& edit_obj, std::string& str_out );
	//set wnd text
	void SetWndText( const char* wnd_text );
	void SetWndTxt( const std::string& wnd_text );

protected:
	static const UINT_PTR kEnableTimer = 1;

private:
	CString					wnd_text_old_;
};

//dlg login
class DlgLogin : public DlgUseful, public Singleton<DlgLogin, Singleton_Instance>{
public:
	DlgLogin();
	//执行
	void SetDoing( bool is_doing );
	const CString& GetAc() const;
	const CString& GetPwd() const;
	void OnLoginSucceed();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
	afx_msg void OnBnClickedBtRemoveBind();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private:
	CEdit				edit_account_;
	CEdit				edit_password_;

private:
	CString				str_password_;
	bool				is_doing_;
	CString				str_account_;
};

//for register account
class DlgReg : public DlgUseful, public Singleton<DlgReg, Singleton_Instance>{
public:
	DlgReg();
	//执行
	void SetDoing( bool is_doing );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

private:
	virtual void OnOK();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	DECLARE_MESSAGE_MAP()

private:
	CEdit				edit_account_;
	CEdit				edit_password_;
	CEdit				edit_confirm_password_;
	CEdit				edit_qq_;

private:
	CString				str_account_;
	CString				str_password_;
	CString				str_confirm_password_;
	CString				str_qq_;
	CString				str_reg_key_;
	bool				is_doing_;
};

//for pay
class DlgPay : public DlgUseful, public Singleton<DlgPay, Singleton_Instance>{
public:
	DlgPay();
	//执行
	void SetDoing( bool is_doing );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

private:
	virtual void OnOK();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	DECLARE_MESSAGE_MAP()

private:
	CEdit				edit_card_id_;
	CEdit				edit_card_password_;
	CEdit				edit_account_;
	CEdit				edit_confirm_account_;

private:
	CString				str_account_;
	CString				str_confirm_account_;
	CString				str_card_serial_;
	CString				str_card_password_;
	bool				is_doing_;
};
// DlgAddAccountHelper dialog

class DlgAddAccountHelper : public DlgUseful
{
public:
	DlgAddAccountHelper(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgAddAccountHelper();

public:
	//get the game account
	const CString& GetAccount() const;
	//get the game password
	const CString& GetPassword() const;

// Dialog Data
	enum { IDD = IDD_DLG_ADD_ACCOUNT_HELPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CEdit						edit_account_;
	CEdit						edit_password_;
	CComboBox					combo_select_service_;

private:
	CString						str_account_;
	CString						str_password_;
};

class DlgSelectScript : public CDialog
{
public:
	DlgSelectScript( CWnd* pParent );   // standard constructor
	virtual ~DlgSelectScript();

public:
	const CString& GetSelectedScriptName() const;

	// Dialog Data
	enum { IDD = IDD_DLG_SET_SCRIPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CComboBox					combo_scripts_;

private:
	CString						script_name_;
};

//选择游戏服务器
class SelectGameService : public CDialog{
public:
	SelectGameService( CWnd* parent );
	const CString& GetSelectedService() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CComboBox					combo_select_service_;

private:
	CString						selected_service_;
};

//登陆完成后自动运行脚本
class AutoRunScript_LoginSucceed : public CDialog{
public:
	AutoRunScript_LoginSucceed( CWnd* parent );
	bool IsAutoRunScript() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CComboBox			combo_auto_run_;

private:
	bool				is_auto_run_;		
};

//答码之设置帐号和密码
class CrackCaptcha_SetAccountDlg : public CDialog{
public:
	CrackCaptcha_SetAccountDlg( CWnd* parent );
	const CString& GetAccount() const;
	const CString& GetPassword() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CString				str_account_;
	CString				str_password_;
};

//答码之设置服务信息
class CrackCapthcha_SetServiceInfo : public CDialog{
public:
	CrackCapthcha_SetServiceInfo( CWnd* parent );
	DWORD GetIpAddr() const;
	std::string GetIpString() const;
	WORD GetPort() const;
	void SetIpInfo( const char* ip_str, WORD port );
	void SetIpInfo( DWORD ip, WORD port );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

private:
	DWORD							ip_address_;
	WORD							port_;
};

//设置验证服务信息
class DlgCheckServerInfo : public CDialog, public Singleton<DlgCheckServerInfo, Singleton_Instance>{
public:
	DlgCheckServerInfo();

public:
	std::string GetIpString() const;
	void SetIpInfo( const char* ip_str );
	void SetIpInfo( DWORD ip );
	void ConnectSucceed();
	void SetConnecting( bool connecting );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

private:
	DWORD				ip_address_;
	bool				is_connecting_;
};

//设置最大登陆数量
class DlgSetMaxLogin : public CDialog{
public:
	DlgSetMaxLogin( CWnd* parent );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();

private:
	short				max_login_;
};

// DlgMgrAc dialog
class DlgMgrAc : public CDialog
{
public:
	DlgMgrAc(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgMgrAc();

// Dialog Data
	enum { IDD = IDD_DLG_MGR_AC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	afx_msg void OnBnClickedBtImportAc();
	afx_msg void OnBnClickedBtExportAc();
	DECLARE_MESSAGE_MAP()

private:
	int				use_old_format_;
};

//设置号的种类
class DlgSelectGameAcType : public CDialog{
public:
	DlgSelectGameAcType( CWnd* parent );
	enGameAcType GetGameAcType() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

private:
	CComboBox						combo_ac_type_;
	enGameAcType		game_ac_type_;
};

//留言版
class DlgMsgBoard : public DlgUseful{
public:
	DlgMsgBoard();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	void SetDoing( bool is_doing );
	DECLARE_MESSAGE_MAP()

private:
	CEdit					msg_board_;
	CString					str_msg_board_;
	bool					is_doing_;
};