#pragma once
#include "stdafx.h"
#include <stack>
#include "FuncObjS.h"
#include "CfgStruct.h"
#include <list>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <stddef.h>
#include <functional>
#include "Common/UsefulClass.h"
#include <Net/JsonCall.h>
#include <Poco/DateTime.h>
#include <Net/IoTimer.h>
#include <Net/PocoMore.h>

//subitem index
enum enListSubItems{
	kSubitemAccount = 0,
	kSubitemPassword,
	kSubitemService,			//服务器
	kSubitemRunState,			//运行状态
	kSubitemGameAcType,			//号的种类
	kSubitemAutoRunScript,		//自动运行脚本
	kSubitemDiscoverGoodPlayer,	//高价值球员
	kSubitemTrainerName,		//教练名
	kSubitemTrainerLevel,		//教练等级
	kSubitemMatchCnt,			//比赛局数
	kSubitemCurEP,				//当前EP
	kSubitemCurScript,			//当前脚本
	kSubitemGainedEP,			//获得的EP
	kSubitemSpid,				//球员ID
	kSubitemOutRes,				//输出结果
	kSubitemScriptOutputInfo,	//脚本输出信息
	kSubitemLastNotifyTime,		//最后通信时间
};

//for login
struct LoginHelper{
	int							item_;
	DWORD						login_process_id_;				//登陆器进程id
	bool						is_logining_;					//是否正在登陆
	boost::weak_ptr<Session>	session_login_;					//登陆会话
	IoTimerPtr					timer_login_;					//开始登陆的时间

public:
	//释放登陆计时
	void FreeLoginTimer();

public:
	LoginHelper();
	struct tagItem{};
	struct tagLoginProcessId{};
	//struct tagSessionLogin{};
};
typedef boost::multi_index_container<
	LoginHelper, boost::multi_index::indexed_by<
	boost::multi_index::ordered_unique<boost::multi_index::tag<LoginHelper::tagItem>, 
	BOOST_MULTI_INDEX_MEMBER(LoginHelper, decltype(LoginHelper::item_), item_) >,
	boost::multi_index::ordered_non_unique<boost::multi_index::tag<LoginHelper::tagLoginProcessId>, 
	BOOST_MULTI_INDEX_MEMBER(LoginHelper, decltype(LoginHelper::login_process_id_), login_process_id_) >/*,
	boost::multi_index::ordered_non_unique<boost::multi_index::tag<LoginHelper::tagSessionLogin>, 
	BOOST_MULTI_INDEX_MEMBER(LoginHelper, decltype(LoginHelper::session_login_), session_login_) >*/
	> > LoginHelperContT;

//游戏服务区
class GameServiceMgr : public Singleton<GameServiceMgr, Singleton_Instance>{
	//type definition
public:
	typedef std::pair<std::string, DWORD> ServiceInfoT;
	typedef std::vector<ServiceInfoT> ContT;

public:
	GameServiceMgr();
	//得到服务器ID
	DWORD GetServiceId( const std::string& service_name ) const;
	//得到游戏服务区信息
	const ContT& GetServicesInfo() const;
	//得到服务器索引
	int GetServiceIdx( const std::string& service_name ) const;

private:
	ContT				services_info_;
};

class ListNormal : public CListCtrl{
//type definitions
public:
	//禁止项,游戏帐号，游戏区，原因
	struct stProhibitAc : public std::pair<std::string, std::string>{
		stSpecificCfg::enProhibitReason				prohibit_reason_;				//帐号被禁止的原因
	};
	typedef std::set<stProhibitAc> ProhibitT;

public:
	ListNormal();

public:
	void StartItemScript( int item );
	void StopItemScript( int item );
	void Delete_Item( int item );
	//删除所有帐号
	void DeleteAllItems();
	void Insert_Item( LPCTSTR account, LPCTSTR password, LPCTSTR service_name );
	//关闭游戏进程
	bool CloseGameProcess( int item );
	//得到一个空闲的有效项
	int GetFreeItem();
	//设置服务器
	bool SetItemService( int item, LPCTSTR service_name );
	//do item
	bool DoItem( const std::function<bool(int, const ListNormal&)>& func );
	//do item func obj
	void DoItemFuncObj( const std::function<void(FuncObj_NormalS&)>& func );

public:
	int FindItemByAccount( LPCTSTR account, LPCTSTR service_name ) const;
	int FindItemByAccount( const std::string& game_ac, const std::string& game_ser_name ) const;
	//设置item func obj
	void SetItemFuncObj( int item, FuncObj_NormalS* func_obj );
	//得到item func obj
	FuncObj* GetItemFuncObj( int item );
	FuncObj_NormalS* GetNormalS( LPCTSTR account );
	FuncObj_NormalS* GetNormalS( LPCTSTR ac, LPCTSTR service_name );
	FuncObj_NormalS* GetNormalS( int item );
	FuncObj_NormalS* GetNormalS( const std::string& game_ac, int ser_idx );
	void FreeItemByFuncObj( FuncObj& func_obj, const CString& str_run_state );
	int FindItemByFuncObj( FuncObj& func_obj );
	CString GetItemAccount( int item ) const;
	//得到服务器名
	CString GetItemServiceName( int item ) const;
	//得到服务索引
	int GetItemServiceIdx( int item ) const;
	int GetItemServiceIdx( const std::string& ac, const std::string& service_name ) const;
	//game ac type
	enGameAcType GetItemGameAcType( int item );
	//接受连接
	int AcceptSession( DWORD client_proccess_id );
	//设置是否自动运行脚本
	void SetAutoRunScript( int item, bool auto_run_script );
	//查找指定的登陆连接
	LoginHelper* FindLoginHelperBySession( Session* login_session );
	LoginHelper* FindLoginHelperByPid( DWORD pid );
	//删除指定的登陆连接
	void EraseLoginHelperBySession( Session* login_session );
	void EraseLoginHelper( LoginHelper* login_hlp );
	//得到未被禁止的item项
	int GetNoProhibitItem( const std::string& user_ac, const std::string& service_name );
	//增加禁止登陆项
	void AddProhibitItem( const std::string& user_ac, const std::string& service_name, stSpecificCfg::enProhibitReason reason );
	//删除禁止登陆项
	void RemoveProhibitItem( const std::string& user_ac, const std::string& game_service );
	void RemoveProhibitItem( int item );
	//查找一个禁止项
	ProhibitT::iterator FindProhibitIter( const std::string& user_ac, const std::string& game_service );
	//清除51场禁止项
	void ClearProhibit_51();
	//效验证密码
	bool CheckSamePwd( LPCTSTR ac, LPCTSTR pwd );

public:
	const stSpecificCfg& GetAccountCfg( const std::string& ac, const std::string& service_name );
	stSpecificCfg* FindSpecificCfg( const std::string& ac, const std::string& service_name );
	void SaveCfgByAccount( const std::string& ac, const std::string& service_name );
	void SaveAllSpecificCfg();
	//若不存在某帐号的特殊配置，则会生成该项
	stSpecificCfg* GenSpecificCfg( const std::string& ac, const std::string& service_name );

	//登陆相关
public:
	//开始登陆游戏
	bool BeginLogin( int item );
	//执行登陆一个帐号
	bool DoLoginOne();
	//得到正在登陆和在线的数量
	DWORD GetLoginingCnt() const;

	//会被远程调用
public:
	//设置EP
	void SetEP( FuncObj_NormalS& func_obj_normal );
	//比赛局数
	void SetMatchCnt( FuncObj_NormalS& func_obj_normal );
	//设置运行状态
	void SetRunState( FuncObj_NormalS& func_obj_normal );
	//设置教练名
	void SetTrainerName( FuncObj_NormalS& func_obj_normal );
	//设置教练等级
	void SetTrainerLevel( FuncObj_NormalS& func_obj_normal );
	//设置脚本输出信息
	void SetScriptOutputInfo( FuncObj_NormalS& func_obj_normal, const char* output_info );
	//开始脚本
	void StartRunScript( FuncObj_NormalS& func_obj_normal );
	//到了某个游戏场景
	void OnGameState( FuncObj_NormalS& func_obj_normal );
	//发现好的球员
	void DiscoverGoodPlayer( FuncObj_NormalS& func_obj_normal );
	
protected:
	virtual void PreSubclassWindow();

public:
	//响应右键down
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//开始脚本
	afx_msg void OnMenuItemStartItemScript();
	//停止脚本
	afx_msg void OnMenuItemStopScript();
	//添加帐号
	afx_msg void OnMenuAddAccount();
	//删除帐号
	afx_msg void OnMenuDeleteAccount();
	//显示游戏窗口
	afx_msg void OnMenuListShowGameWindow();
	//设置脚本
	afx_msg void OnMenuListSetScript();
	//显示统计信息
	afx_msg void OnMenuShowAccountInfo();
	//开始所有脚本
	afx_msg void OnMenuStartAllScript();
	//停止所有脚本
	afx_msg void OnMenuStopAllScript();
	//显示所有统计信息
	afx_msg void OnMenuShowAllAccountInfo();
	//关闭游戏进程
	afx_msg void OnMenuCloseProcess();
	//关闭所有游戏进程
	afx_msg void OnCloseAllProcess();
	//开始登陆游戏
	afx_msg void OnMenuBeginLogin();
	//选择服务器
	afx_msg void OnMenuSelectService();
	//批量上号
	afx_msg void OnMenuLoginSomeGame();
	//自动运行脚本
	afx_msg void OnMenuAutoRunScript();
	//批量自动运行脚本
	afx_msg void OnMenuAllAutoRunScript();
	//停止批量上号
	afx_msg void OnMenuStopLoginAll();
	//设置号种类
	afx_msg void OnMenuSelectGameAcType();
	//查看此号的交易信息
	afx_msg void OnMenuAsNotifyObj();
	//取消禁止登陆项
	afx_msg void OnMenuCancelprohibit();
	//查看此号物品信息
	afx_msg void OnMenuLookItemInfo();
	//查看此号球员信息
	afx_msg void OnMenuLookPlayerInfo();
	DECLARE_MESSAGE_MAP()

private:
	//更新最后通信的时间
	void UpdateLastNotifyTime( int item );

protected:
	//first == 帐号，second == 服务器名
	typedef std::pair<std::string, std::string> SpecificCfgKeyT;
	typedef std::map<SpecificCfgKeyT, stSpecificCfg> SpecificCfgT;

	struct stGameInfo{
		int						cur_ep_;
		int						gain_total_ep_;				//获得的总EP
		int						match_cnt_;					//比赛局数
		int						trainer_level_;				//教练等级
		CString					running_state_;				//运行状态
		CString					trainer_name_;				//教练名
		GType::enGameState		game_state_;				//游戏状态

		stGameInfo();
	};
	
protected:
	//std::stack<int>							start_items_;	
	SpecificCfgT							specific_cfgs_;
	LoginHelperContT						login_helper_;
	ProhibitT								prohibit_login_;			//禁止启动项
	GameServiceMgr							game_service_mgr_;
};

class PageAccount_List : public ListNormal{
private:
	PageAccount_List();
	~PageAccount_List();
	friend class PageAccount;

public:
	void SaveAllAccountCfg();
	//保存EP到指定的文件
	void SaveEpToTheFile();
	int CalcConnectionsCnt();

	//协调通信相关
public:
	//游戏号的迭代
	FuncObj_NormalS::IteratorPtr CreateIterByGameAcType( enGameAcType game_ac_type, int service_idx );

public:
	static PageAccount_List* GetInstance();

protected:
	virtual void PreSubclassWindow();

private:
	//每到第二天的0点就清空禁止项
	void Timer_ClrProhibitAtMidnight( const boost::system::error_code& error );

private:
	static PageAccount_List*		this_;
	//boost::asio::deadline_timer		timer_clr_prohibit_;		//每天0点清空禁止项
	DateTimeLocal					time_start_;				//启动时间
};