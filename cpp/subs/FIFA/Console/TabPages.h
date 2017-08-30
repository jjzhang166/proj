#pragma once
/*
@author zhp
@date 2013-12-23
@purpose table pages
*/
#include <string>
#include <FuncObjs/FuncObj.h>
#include <stack>
#include "FuncObjS.h"
#include "ListCtrls.h"
#include "ListCtrl_Players.h"
//////////////////////////////////////////////////////////////////////////
struct stMakeTabPages{
	LPCTSTR						item_name;
	const CRuntimeClass*		obj_class;

	static const int		kPagesMax = 
#ifdef _WG_TEST
		5
#else
		4
#endif
		;
};

class CTabOption;
class TablePage : public CDialog{
protected:
	TablePage( UINT template_id = -1 );

public:
	bool Create( CWnd* parent_wnd );
	//得到CTabOption
	CTabOption& Parent2TabOption();
	//get normal_s
	FuncObj_NormalS* GetNormalS_ByAccount( const CString& str_account );
	FuncObj_NormalS* GetNormalS_ByAccount( const char* str_account );
	void SetTabIdx( int tab_idx );
	int GetTabIdx() const;

private:
	virtual void OnCancel();
	DECLARE_DYNCREATE(TablePage)
	//DECLARE_MESSAGE_MAP()

protected:
	int		tab_idx_;
};

//////////////////////////////////////////////////////////////////////////

class PageAccount : public TablePage, public Singleton<PageAccount, Singleton_Instance>{
private:
	PageAccount();

public:
	PageAccount_List& GetAccountList();
	//save config
	void SaveConfig();
	//得到仓库号服务所在IP
	std::string GetStoredIp() const;
	//12点自动重启
	void SetRunConsoleAt12Time();

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();

private:
	//设置劫持dll配置表
	void SetHijackDllCfg();

private:
	afx_msg void OnBtSelectGamePath();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCrackCaptcha();
	//设置最大更新数量
	afx_msg void OnBnClickedBtSetMaxLogin();
	//管理帐号
	afx_msg void OnBnClickedBtAcAcMgr();
	//弹出批量操作菜单
	afx_msg void OnBnShowAllOperMenu();
	//确认仓库号IP
	afx_msg void OnBtOkStoretdIp();
	//保存EP
	afx_msg void OnBnSaveEp();
	//设置ep文件目录
	afx_msg void OnBnSetEpFilePath();
	//12点定时重启
	afx_msg void OnBnAutoRunConsoleAt12Time();
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(PageAccount)

private:
	PageAccount_List				list_ctrl_;
	CComboBox						combo_crack_catpcha_;
	DWORD							stored_ip_;				//仓库号所在IP
};

class PageSetting : public TablePage{
private:
	PageSetting();

protected:
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnBtGamePath();
	DECLARE_DYNCREATE(PageSetting)
	DECLARE_MESSAGE_MAP()

protected:
	//设置劫持dll配置表
	void SetHijackDllCfg();
};

class PageTest : public TablePage, public Singleton<PageTest, Singleton_Instance>{
private:
	PageTest();

public:
	//设置球员信息总数
	void SetPlayersTotalCnt( int cnt );
	//设置物品信息总数
	void SetItemsTotalCnt( int cnt );
	FuncObj_NormalS* GetCurNormalS();
	CList_ItemsSingle& GetCtrlItem();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange( CDataExchange* pDX );

protected:
	FuncObj_NormalS* GetItemNormal( int item );

private:
	//打开一个要加载的DLL文件
	afx_msg void OnBtOpenLib();
	//让注入dll加载这个dll
	afx_msg void OnBtLoadLib();
	//卸载这个dll
	afx_msg void OnBtUnloadLib();
	//调用LuaTest函数
	afx_msg void OnBtCallLuaLuaTest();
	//重新加载lua文件
	afx_msg void OnBtReloadLua();
	//更新log到临时文件
	afx_msg void OnFlushLogToTempFile();
	//执行脚本
	afx_msg void OnBtRunScript();
	//中止脚本
	afx_msg void OnBtTerminateScript();
	//响应点击选择帐号
	afx_msg void OnDropdownSelectAccount();
	//刷新球员信息
	afx_msg void OnBtGetPlayersInfo();
	//开启成功登陆
	afx_msg void OnBtLoginSucceed();
	//刷新物品信息
	afx_msg void OnBtGetItemsInfo();
	DECLARE_DYNCREATE(PageTest)
	DECLARE_MESSAGE_MAP()

private:
	CComboBox					combo_select_account_;
	CList_PlayersSingle			list_players_;
	CList_ItemsSingle			list_items_;
	CEdit						players_total_;
	CEdit						items_total_;
};

// PageTrade dialog
class PageTrade : public TablePage, public Singleton<PageTrade, Singleton_Instance>{
public:
	PageTrade();
	virtual ~PageTrade();

	//interfaces
public:
	FuncObj_NormalS* GetCurNormalS();
	//设置当前通信项
	void SetCurNotifyItem( int item );
	//设置我的出售列表项数
	void SetMyOfferListCnt( int cnt );
	//设置其他号的项数
	void SetOtherOfferItemsCnt( int cnt );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnBtGetMyOfferList();
	afx_msg void OnBtGetOtherOfferItems();
	DECLARE_DYNCREATE(PageTrade)
	DECLARE_MESSAGE_MAP()

private:
	CList_MyOfferList				my_offer_list_;			//我的出售列表
	CList_OtherOffers				other_offer_items_;		//其他号的出售项
	CEdit							my_offer_total_;
	CEdit							other_offer_total_;
	int								cur_notify_item_;		//当前通信项
};

class PageItemInfo : public TablePage, public Singleton<PageItemInfo, Singleton_Instance>{
public:
	PageItemInfo();
	CList_ItemsSingle& GetCtrlItem();
	void SetItemsTotalCnt( int cnt );
	void SetNofityItem( int notify_item );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_DYNCREATE(PageItemInfo)
	//刷新物品信息
	afx_msg void OnBtGetItemsInfo();
	DECLARE_MESSAGE_MAP()

private:
	CList_ItemsSingle			list_items_;
	CEdit						items_total_;
	CEdit						edit_notify_item_;

private:
	int							notify_item_;
};

class PagePlayerInfo : public TablePage, public Singleton<PagePlayerInfo, Singleton_Instance>{
public:
	PagePlayerInfo();
	CList_PlayersSingle& GetCtrlPlayer();
	void SetItemsTotalCnt( int cnt );
	void SetNofityItem( int notify_item );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_DYNCREATE(PagePlayerInfo)
	//刷新球员信息
	afx_msg void OnBtGetPlayersInfo();
	DECLARE_MESSAGE_MAP()

private:
	CList_PlayersSingle			list_players_;
	CEdit						players_total_;
	CEdit						edit_notify_item_;

private:
	int							notify_item_;
};
