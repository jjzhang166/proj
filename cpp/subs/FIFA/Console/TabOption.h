#pragma once
#include "TabPages.h"
#include "LoginS.h"
#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Net/IoTimer.h>
#include "CoordinateS.h"

class CTabOption : public CTabCtrl, public Server, public Singleton<CTabOption, Singleton_Instance>{
public:
	CTabOption();
	~CTabOption();

	//type definition
public:
	//关键数据
	typedef std::vector<DWORD> AddrDatasT;

public:
	PageAccount& GetPageAccount();
	//get succeed
	bool IsLoginSucceed() const;
	//set login succeed
	int SetLoginSucceed( bool succeed );
	//设置用户使用期限
	void SetUserEndTime( const Poco::Timestamp& timestamp );
	void SetUserEndTime( const DateTimeLocal& end_date_time );
	//清空登陆信息
	void ClearLoginInfo();
	//得到使用期限
	const DateTimeLocal& GetEndDateTime() const;
	//得到关键数据
	const AddrDatasT& GetAddrDatas() const;
	//添加关键数据
	void AddAddrData( DWORD addr_data );
	//连接协调服务
	void ConnectCoordinate();
	//检测调试状态
	void CheckDebugger();
	//CRC检测
	void CheckCrcValid();
	CoorTrans::TrackIterPtrT MakeTransTrackIterPtr();

protected:
	afx_msg void OnSelChange( NMHDR* notify, LRESULT* result );
	afx_msg void OnSelChanging( NMHDR* notify, LRESULT* result );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	virtual Session* CreateSession();
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;

private:
	CoordinateServer					coor_server_;				//协调服务
	DateTimeLocal						user_end_time_;				//用户使用期限
	LoginServer							login_s_;

private:
	typedef TablePage* TablePagePtr;

	TablePagePtr						table_pages_[stMakeTabPages::kPagesMax];
	bool								login_succeed_;				//登陆是否成功
	AddrDatasT							addr_datas_;

	IoConnectIterPtr					io_connects_iter_;
};