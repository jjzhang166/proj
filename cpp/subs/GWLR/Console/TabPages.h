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
#include "DlgUnion.h"
//////////////////////////////////////////////////////////////////////////
struct stMakeTabPages{
	LPCTSTR						item_name;
	const CRuntimeClass*		obj_class;

	static const int		kPagesMax = 
#ifdef _WG_TEST
		9
#else
		3
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

private:
	virtual void OnCancel();
	DECLARE_DYNCREATE(TablePage)
	//DECLARE_MESSAGE_MAP()
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
	//刷新当前帐号通知项
	void RefreshCurNotifyItem();
	//得到当前帐号通知项
	int GetCurNotifyItem() const;
	//设置当前帐号通知项
	void SetCurNotifyItem( int cur_item );
	FuncObj_NormalS* GetCurNotifyFuncObj();
	//通知右键攻击
	void NotifyRightBtHit();
	//12点自动重启
	void SetRunConsoleAt3Time();
	//通知开启城镇寻路
	void NotifyOpenVillagePath();

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();

private:
	//设置劫持dll配置表
	void SetHijackDllCfg();
	//覆盖动画
	void CopyMovies( const CPath& movie_path );
	//优化事件
	void YouHuaSetting();

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
	afx_msg void OnBtRunFirstScript();
	afx_msg void OnSelchangeRoleMaxCnt();
	afx_msg void OnChangeLoginDelay();
	//无限右键攻击
	afx_msg void OnBnRightBtHit();
	//3点定时重启
	afx_msg void OnBnAutoRunConsoleAt3Time();
	//封号就停止登陆
	afx_msg void OnBtStopLoginAtBanAc();
	//启用迅雷加速
	afx_msg void OnBtUseXunLei();
	//启用优化
	afx_msg void OnBtUseYouHua();
	//掉线立即重登
	afx_msg void OnBtLoginAtDisconnectRightNow();
	//开启城镇寻路
	afx_msg void OnBtOpenVillagePath();
	//忽略密码错误
	afx_msg void OnBtIgnorePwdErr();
	//自动拨号
	afx_msg void OnBtAutoDial();
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(PageAccount)

private:
	PageAccount_List				list_ctrl_;
	CComboBox						combo_crack_catpcha_;
	CComboBox						combo_role_max_cnt_;
	CEdit							edit_cur_notify_item_;
	CEdit							edit_login_delay_;
	DlgAutoDial						dlg_auto_dial_;

private:
	DWORD							stored_ip_;				//仓库号所在IP
	int								cur_notify_item_;		//当前帐号通知项
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

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange( CDataExchange* pDX );

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
	BOOL OnInitDialog();

	afx_msg void OnBtGetMyOfferList();
	afx_msg void OnBtGetOtherOfferItems();
	DECLARE_DYNCREATE(PageTrade)
	DECLARE_MESSAGE_MAP()

private:
	CList_MyOfferList				my_offer_list_;			//我的出售列表
	CList_OtherOffers				other_offer_items_;		//其他号的出售项
	CEdit							my_offer_total_;
	CEdit							other_offer_total_;
	CButton							bt_refresh_my_list_;
	CButton							bt_refresh_other_list_;
	int								cur_notify_item_;		//当前通信项
};

//技能信息
class PageSkill : public TablePage, public Singleton<PageSkill, Singleton_Instance>{
public:
	PageSkill();
	//设置技能总数
	void SetSkillTotalCnt( int cnt );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageSkill)
	//刷新技能
	afx_msg void OnBtRefreshSkillInfo();
	DECLARE_MESSAGE_MAP()

private:
	CEdit							edit_skill_total_cnt_;
};

//游戏对象信息
class PageGameObjs : public TablePage, public Singleton<PageGameObjs, Singleton_Instance>{
public:
	PageGameObjs();
	//设置游戏对象总数
	void SetGameObjTotalCnt( int cnt );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageGameObjs)
	//刷新游戏对象
	afx_msg void OnBtRefreshGameObjs();
	DECLARE_MESSAGE_MAP()

private:
	CEdit							edit_gameobj_total_cnt_;
};

//物品信息
class PageItems : public TablePage, public Singleton<PageItems, Singleton_Instance>{
public:
	PageItems();
	void SetItemsTotalCnt( int cnt );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageItems)
	afx_msg void OnBtRefreshPageItems();
	DECLARE_MESSAGE_MAP()

private:
	CEdit						edit_item_total_cnt_;
};

//房间信息
class PageRoom : public TablePage, public Singleton<PageRoom, Singleton_Instance>{
public:
	PageRoom();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageRoom)
	afx_msg void OnBtRefreshRoomInfo();
	DECLARE_MESSAGE_MAP()

private:
};

//任务
class PageTask : public TablePage, public Singleton<PageTask, Singleton_Instance>{
public:
	PageTask();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageTask)
	DECLARE_MESSAGE_MAP()

private:
};

//注意事项
class PageRemark : public TablePage, public Singleton<PageRemark, Singleton_Instance>{
public:
	PageRemark();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_DYNCREATE(PageRemark)
	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();

private:
	CEdit			static_remark_;
};