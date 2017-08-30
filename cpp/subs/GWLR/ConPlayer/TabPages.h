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

	static const int		kPagesMax = 1;
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
	void SetSession( FuncObj_NormalS* nors );
	FuncObj_NormalS* GetSession();

protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();

private:
	//设置劫持dll配置表
	void SetHijackDllCfg();

private:
	afx_msg void OnBtSelectGamePath();
	afx_msg void OnBtSetMultiHit();
	afx_msg void OnBtDisconnect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(PageAccount)

private:
	CEdit		edit_multi_hit_;
	CEdit		edit_run_state_;
	CStatic		static_multi_hit_;

private:
	FuncObj_NormalS*			nor_;
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