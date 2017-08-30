// Dlgs.cpp : implementation file
//

#include "stdafx.h"
#include "Dlgs.h"
#include "CS_CfgInfo.h"
#include <Protocol/P_PriDefines.h>
#include "DataHandlerS.h"
#include "CS_CfgInfo.h"

// DlgMgrTbl dialog

IMPLEMENT_DYNAMIC(DlgMgrTbl, CDialog)

DlgMgrTbl::DlgMgrTbl(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMgrTbl::IDD, pParent)
{

}

DlgMgrTbl::~DlgMgrTbl()
{
}

void DlgMgrTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DB_OLD, str_db_old_);
	DDX_Text(pDX, IDC_EDIT_DB_NEW, str_db_new_);
}


BEGIN_MESSAGE_MAP(DlgMgrTbl, CDialog)
	ON_BN_CLICKED(IDC_BT_CREATE_DB_TBL, &DlgMgrTbl::OnBtCreateDbTbl)
	ON_BN_CLICKED(IDC_BT_IMPORT_DB_DATA, &DlgMgrTbl::OnBtImportDbData)
	ON_BN_CLICKED(IDC_BT_COMBINE_DB, &DlgMgrTbl::OnBtCombineDb)
END_MESSAGE_MAP()

void DlgMgrTbl::SetWndText(const char* text)
{
	if (!text)
	{
		assert(false);
		return;
	}
	SetWindowText(CA2CT(text));
}

void DlgMgrTbl::SetWndText(const CString& text)
{
	SetWindowText(text);
}

// DlgMgrTbl message handlers
void DlgMgrTbl::OnBtCreateDbTbl()
{
	UpdateData();
	if (str_db_new_.IsEmpty())
	{
		SetWndText(_T("新数据库名不能为空"));
		return;
	}
	try{
		auto& cfg_info = stCS_CfgInfo::GetMe();
		std::string db_name = CT2CA(str_db_new_);
		auto db_session = DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(cfg_info.sql_svr_ip_, kSqlPort, db_name, kSqlSvrAc, kSqlSvrPwd));
		if (!db_session)
		{
			LOG_O(Log_debug) << "连接失败";
			SetWndText("连接失败");
			assert(false);
			return;
		}
		using namespace Poco::Data::Keywords;

		Poco::Data::Statement select(*db_session);
		select << R"##(
CREATE TABLE tbl_Account ( 
	UserName varchar(20) NOT NULL,
	Pwd varchar(20) NOT NULL,
	MachineCode varchar(36) NOT NULL,
	LastLoginTime datetime,
	LastPayTime datetime,
	LastLoginIP varchar(15),
	RegTime datetime NOT NULL,
	EndDayTime datetime NOT NULL,
	QQ varchar(12),
	RegIp varchar(15) DEFAULT 'nil' NOT NULL,
	Cookie varchar(36),
	CookieEndTime datetime,
	AcType int DEFAULT 0 NOT NULL
);
ALTER TABLE tbl_Account ADD CONSTRAINT PK_tbl_Account PRIMARY KEY CLUSTERED (UserName);
ALTER TABLE tbl_Account ADD CONSTRAINT UQ_tbl_Account_UserName UNIQUE (UserName);


CREATE TABLE tbl_Card ( 
	CardId varchar(36) NOT NULL,
	CardPwd varchar(36) NOT NULL,
	UseTime datetime,
	AddTime datetime NOT NULL,
	AddUser varchar(20) NOT NULL,
	IsUsed bit DEFAULT 0 NOT NULL,
	UserName varchar(20),
	CardMinutes int DEFAULT 0 NOT NULL,
	CardGrant bigint DEFAULT 0 NOT NULL,
	AcType int DEFAULT 0 NOT NULL
);
ALTER TABLE tbl_Card ADD CONSTRAINT PK_tbl_Card PRIMARY KEY CLUSTERED (CardId);
ALTER TABLE tbl_Card ADD CONSTRAINT UQ_tbl_Card_CardId UNIQUE (CardId);
--ALTER TABLE tbl_Card ADD CONSTRAINT FK_tbl_Card_tbl_Account FOREIGN KEY (UserName) REFERENCES tbl_Account (UserName) ON DELETE CASCADE;


CREATE TABLE tbl_AcState( 
	UserName varchar(20) NOT NULL,
	State int NOT NULL,
	LastState int,
	BeginTime datetime NOT NULL,
	EndTime datetime,
	Period int NOT NULL
);
ALTER TABLE tbl_AcState ADD CONSTRAINT PK_tbl_AcState PRIMARY KEY CLUSTERED (UserName);
ALTER TABLE tbl_AcState ADD CONSTRAINT UQ_tbl_AcState_UserName UNIQUE (UserName);
ALTER TABLE tbl_AcState ADD CONSTRAINT FK_tbl_AcState_tbl_Account FOREIGN KEY (UserName) REFERENCES tbl_Account (UserName) ON DELETE CASCADE;


CREATE TABLE tbl_MsgBoard ( 
	id int NOT NULL,
	msg varchar(150) NOT NULL,
	UserName varchar(20) NOT NULL,
	AddTime datetime NOT NULL
);
ALTER TABLE tbl_MsgBoard ADD CONSTRAINT PK_tbl_MsgBoard PRIMARY KEY CLUSTERED (id);
ALTER TABLE tbl_MsgBoard ADD CONSTRAINT UQ_tbl_MsgBoard_id UNIQUE (id);
ALTER TABLE tbl_MsgBoard ADD CONSTRAINT FK_tbl_MsgBoard_tbl_Account FOREIGN KEY (UserName) REFERENCES tbl_Account (UserName) ON DELETE CASCADE;


CREATE TABLE tbl_UserGrant ( 
	UserName varchar(20) NOT NULL,
	user_grant bigint DEFAULT 0 NOT NULL
);
ALTER TABLE tbl_UserGrant ADD CONSTRAINT PK_tbl_UserGrant PRIMARY KEY CLUSTERED (UserName);
ALTER TABLE tbl_UserGrant ADD CONSTRAINT UQ_tbl_UserGrant_UserName UNIQUE (UserName);
ALTER TABLE tbl_UserGrant ADD CONSTRAINT FK_tbl_UserGrant_tbl_Account FOREIGN KEY (UserName) REFERENCES tbl_Account (UserName) ON DELETE CASCADE;


CREATE TABLE tbl_RegKey ( 
	RegKeyId varchar(36) NOT NULL,
	AddTime datetime NOT NULL,
	AddUser varchar(20) NOT NULL,
	IsUsed bit DEFAULT 0 NOT NULL,
	UserName varchar(20),
	UseTime datetime,
	AcType int DEFAULT 0 NOT NULL
);
ALTER TABLE tbl_RegKey ADD CONSTRAINT PK_tbl_RegKey PRIMARY KEY CLUSTERED (RegKeyId);
ALTER TABLE tbl_RegKey ADD CONSTRAINT UQ_tbl_RegKey_RegKeyId UNIQUE (RegKeyId);
--ALTER TABLE tbl_RegKey ADD CONSTRAINT FK_tbl_RegKey_tbl_Account FOREIGN KEY (UserName) REFERENCES tbl_Account (UserName) ON DELETE CASCADE;


CREATE TABLE tbl_AddrData ( 
	id int NOT NULL,
	AddrData int NOT NULL
);
ALTER TABLE tbl_AddrData ADD CONSTRAINT PK_tbl_AddrData PRIMARY KEY CLUSTERED (id);
ALTER TABLE tbl_AddrData ADD CONSTRAINT UQ_tbl_AddrData_id UNIQUE (id);


CREATE TABLE tbl_StrData ( 
	id int NOT NULL,
	StrData varchar(200)
	);
ALTER TABLE tbl_StrData ADD CONSTRAINT PK_tbl_StrData PRIMARY KEY CLUSTERED (id);
ALTER TABLE tbl_StrData	ADD CONSTRAINT UQ_tbl_StrData_id UNIQUE (id);
		)##", now;
		LOG_O(Log_trace) << "创建表成功";
		SetWndText("创建表成功");
	}
	catch (const Poco::Exception& e){
		std::string str = e.displayText();
		SetWndText(str.c_str());
		LOG_O(Log_trace) << str;
	}
}

struct stData_Account_Old{
	std::string				user_name_;					//用户名
	std::string				user_pwd_;					//用户密码
	std::string				machine_code_;				//机器码
	DateTimeLocal			last_login_time_;			//最后登陆时间
	DateTimeLocal			last_pay_time_;				//最后支付时间
	std::string				last_login_ip_;				//最后登陆时间
	DateTimeLocal			reg_time_;					//注册时间
	DateTimeLocal			end_day_time_;				//用户截止时间
	std::string				qq_;						//qq号码
	std::string				reg_ip_;					//注册时的IP

	operator DataBinder<stData_Account>() const{
		DataBinder<stData_Account> tmp;
		tmp.user_name_ = user_name_;
		tmp.user_pwd_ = user_pwd_;
		tmp.last_login_time_ = last_login_time_;
		tmp.last_pay_time_ = last_pay_time_;
		tmp.last_login_ip_ = last_login_ip_;
		tmp.reg_time_ = reg_time_;
		tmp.end_day_time_ = end_day_time_;
		tmp.qq_ = qq_;
		tmp.reg_ip_ = reg_ip_;
		return tmp;
	}

	template<class Archive>
	void serialize(Archive & ar){
		ar & user_name_
			& user_pwd_
			& machine_code_
			& last_login_time_
			& last_pay_time_
			& last_login_ip_
			& reg_time_
			& end_day_time_
			& qq_
			& reg_ip_;
	}
};
struct stData_MsgBoard_Old{
	stData_MsgBoard_Old(){
		id_ = 0;
		pid_ = 0;
	}
	operator DataBinder<stData_MsgBoard>() const{
		DataBinder<stData_MsgBoard> tmp;
		tmp.id_ = id_;
		tmp.msg_ = msg_;
		tmp.user_name_ = user_name_;
		tmp.add_time_ = add_time_;
		return tmp;
	}

	Poco::UInt32			id_;						//留言版消息号
	std::string				msg_;						//留言信息
	int						pid_;
	std::string				user_name_;					//留言者
	DateTimeLocal			add_time_;					//留言时间

	template<class Archive>
	void serialize(Archive & ar){
		ar & id_
			& msg_
			& pid_
			& user_name_
			& add_time_;
	}
};
template<typename T, typename U>
void SelectDataTo(Poco::Data::Session& session_old, Poco::Data::Session& session_new, pt_csz tbl_name){
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement select(session_old);
	Poco::Data::Statement insert(session_new);

	std::vector<DataBinder<T> > vec_old;
	select << "select * from " << tbl_name, into(vec_old), now;
	if (vec_old.empty())
		return;

	std::vector<DataBinder<U> > vec_new;
	for (auto& v : vec_old)
		vec_new.push_back(v);
	insert << "insert into " << tbl_name << " values(" << Detail::StrHolderHelper<U>::get() << ")", use(vec_new), now;
}

void DlgMgrTbl::OnBtImportDbData()
{
	UpdateData();
	if (str_db_new_.IsEmpty())
	{
		SetWndText(_T("新数据库名不能为空"));
		return;
	}
	if (str_db_old_.IsEmpty())
	{
		SetWndText(_T("旧数据库名不能为空"));
		return;
	}
	try{
		auto& cfg_info = stCS_CfgInfo::GetMe();
		std::string db_name_new = CT2CA(str_db_new_);
		std::string db_name_old = CT2CA(str_db_old_);
		auto db_session = DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(cfg_info.sql_svr_ip_, kSqlPort, db_name_new, kSqlSvrAc, kSqlSvrPwd));
		if (!db_session)
		{
			LOG_O(Log_debug) << "连接失败";
			SetWndText("连接失败");
			assert(false);
			return;
		}
		auto db_session_old = DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(cfg_info.sql_svr_ip_, kSqlPort, db_name_old, kSqlSvrAc, kSqlSvrPwd));
		if (!db_session_old)
		{
			LOG_O(Log_debug) << "连接失败1";
			SetWndText("连接失败1");
			assert(false);
			return;
		}

		//SelectDataTo<stData_Account_Old, stData_Account>(*db_session_old, *db_session, "tbl_account");
		SelectDataTo<stData_Account, stData_Account>(*db_session_old, *db_session, "tbl_account");
		//SelectDataTo<stData_Card, stData_Card>(*db_session_old, *db_session, "tbl_card");
		//SelectDataTo<stData_MsgBoard_Old, stData_MsgBoard>(*db_session_old, *db_session, "tbl_MsgBoard");
		SelectDataTo<stData_MsgBoard, stData_MsgBoard>(*db_session_old, *db_session, "tbl_MsgBoard");
		SelectDataTo<stData_UserGrant, stData_UserGrant>(*db_session_old, *db_session, "tbl_UserGrant");
		//SelectDataTo<stData_RegKey, stData_RegKey>(*db_session_old, *db_session, "tbl_RegKey");

		LOG_O(Log_trace) << "导入表成功";
		SetWndText("导入表成功");
	}
	catch (const Poco::Exception& e){
		std::string str = e.displayText();
		SetWndText(str.c_str());
		LOG_O(Log_trace) << str;
	}
}

template<typename T, typename U>
void CombineDataTo(Poco::Data::Session& session_old, Poco::Data::Session& session_new, pt_csz tbl_name){
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement select(session_old);
	Poco::Data::Statement select_new(session_new);
	Poco::Data::Statement insert(session_new);

	std::vector<DataBinder<T> > vec_old;
	select << "select * from " << tbl_name, into(vec_old), now;
	if (vec_old.empty())
		return;

	std::vector<DataBinder<U> > exists_data;
	select_new << "select * from " << tbl_name, into(exists_data), now;

	std::vector<DataBinder<U> > new_data;
	for (auto& v : vec_old)
	{
		auto it = std::find_if(exists_data.begin(), exists_data.end(), [&v](const DataBinder<U>& data){
			if (v.user_name_ == data.user_name_)
				return true;
			return false;
		});
		if (it != exists_data.end())
		{
			LOG_O(Log_warning) << "用户名：" << v.user_name_ << " 已经存在了";
		}
		else
			new_data.push_back(v);
	}
	insert << "insert into " << tbl_name << " values(" << Detail::StrHolderHelper<U>::get() << ")", use(new_data), now;
}

void DlgMgrTbl::OnBtCombineDb()
{
	UpdateData();
	if (str_db_new_.IsEmpty())
	{
		SetWndText(_T("新数据库名不能为空"));
		return;
	}
	if (str_db_old_.IsEmpty())
	{
		SetWndText(_T("旧数据库名不能为空"));
		return;
	}
	try{
		auto& cfg_info = stCS_CfgInfo::GetMe();
		std::string db_name_new = CT2CA(str_db_new_);
		std::string db_name_old = CT2CA(str_db_old_);
		auto db_session = DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(cfg_info.sql_svr_ip_, kSqlPort, db_name_new, kSqlSvrAc, kSqlSvrPwd));
		if (!db_session)
		{
			LOG_O(Log_debug) << "连接失败";
			SetWndText("连接失败");
			assert(false);
			return;
		}
		auto db_session_old = DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(cfg_info.sql_svr_ip_, kSqlPort, db_name_old, kSqlSvrAc, kSqlSvrPwd));
		if (!db_session_old)
		{
			LOG_O(Log_debug) << "连接失败1";
			SetWndText("连接失败1");
			assert(false);
			return;
		}

		CombineDataTo<stData_Account, stData_Account>(*db_session_old, *db_session, "tbl_account");

		LOG_O(Log_trace) << "合并表成功";
		SetWndText("合并表成功");
	}
	catch (const Poco::Exception& e){
		std::string str = e.displayText();
		SetWndText(str.c_str());
		LOG_O(Log_trace) << str;
	}
}
