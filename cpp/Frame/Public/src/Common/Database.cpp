#include "stdafx.h"
#include <Common/Database.h>
#include "Poco/Data/ODBC/Connector.h" 
#include <atlstr.h>
#include "Common/LogOut.h"
#include <cassert>
#include <Common/CmnException.h>

#pragma comment(lib, "odbc32.lib")

DbSessionFactory::SessionOptionalT DbSessionFactory::CreateSqlServer( const std::string& sql_connect_str )
{
	assert(!sql_connect_str.empty());
	RegisterODBC();
	try{
		Poco::Data::Session session("ODBC", sql_connect_str.c_str()/*, 3*/);
		return SessionOptionalT(session);
	}
	DO_CATCH_THE_EXCEPTION(const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		const std::string& str_err = e.displayText();
		LOG_O(Log_error) << str_err;
		return SessionOptionalT();
	})
}

Poco::Data::Session DbSessionFactory::CreateAccess( const std::string& user_dsn )
{
	RegisterODBC();
	try{
		Poco::Data::Session session("ODBC", "DSN=" + user_dsn);
		return session;
	}
	DO_CATCH_THE_EXCEPTION( const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << e.displayText();
		return Poco::Data::Session("error", "error");
	})
}

void DbSessionFactory::RegisterODBC()
{
	static bool init_once = false;
	if ( !init_once )
	{
		init_once = true;
		Poco::Data::ODBC::Connector::registerConnector();
	}
}

void DbSessionFactory::UnregisterODBC()
{
	static bool init_once = false;
	if ( !init_once )
	{
		init_once = true;
		Poco::Data::ODBC::Connector::unregisterConnector();
	}
}

std::string DbSessionFactory::GenSqlServerConnectStr( const std::string& remote_ip, WORD ip_port, const std::string& db_name, const std::string& db_ac, const std::string& db_pwd )
{
	/*if ( remote_ip.empty() || db_name.empty() || db_ac.empty() || db_pwd.empty() )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "创建sql server会话失败";
		return Poco::Data::Session("error", "error");
	}*/
	CStringA connection_str;
	/*connection_str.Format("DRIVER=SQL Server;UID=%s;PWD=%s;DATABASE=%s;SERVER=%s;",
		uid.c_str(), pwd.c_str(), database.c_str(), remote_ip.c_str());*/
	connection_str.Format("DRIVER=SQL Server;UID=%s;PWD=%s;DATABASE=%s;Address=%s,%u;Network=DBMSSOCN;SERVER=%s;auto-reconnect=true",
		db_ac.c_str(), db_pwd.c_str(), db_name.c_str(), remote_ip.c_str(), ip_port, remote_ip.c_str());
	return std::string(connection_str);
}

TypeHandlerHelper<Poco::Data::AbstractBinder::Ptr> MakeTypeHandlerH( std::size_t& pos,
			Poco::Data::AbstractBinder::Ptr& bind_ptr, Poco::Data::AbstractBinder::Direction dir )
{
	return TypeHandlerHelper<Poco::Data::AbstractBinder::Ptr>(pos, bind_ptr, dir);
}

TypeHandlerHelper<Poco::Data::AbstractExtractor::Ptr> MakeTypeHandlerH( std::size_t& pos, Poco::Data::AbstractExtractor::Ptr& extr_ptr )
{
	return TypeHandlerHelper<Poco::Data::AbstractExtractor::Ptr>(pos, extr_ptr);
}

TypeHandlerHelper<Poco::Data::AbstractPreparator::Ptr> MakeTypeHandlerH( std::size_t& pos, Poco::Data::AbstractPreparator::Ptr& prep_ptr )
{
	return TypeHandlerHelper<Poco::Data::AbstractPreparator::Ptr>(pos, prep_ptr);
}

DataWhere_BindValue_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper, const char* column_value )
{
	return DataWhere_BindValue_(column_name, oper, column_value);
}

DataWhere_BindValue_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper, const std::string& column_value )
{
	return DataWhere_BindValue_(column_name, oper, column_value);
}

TypeHandlerHelper<Poco::Data::AbstractBinder::Ptr>::TypeHandlerHelper( std::size_t& pos, 
			Poco::Data::AbstractBinder::Ptr& bind_ptr, Poco::Data::AbstractBinder::Direction dir )
		: pos_(pos), bind_ptr_(bind_ptr), bind_dir_(dir){}

TypeHandlerHelper<Poco::Data::AbstractExtractor::Ptr>::TypeHandlerHelper( std::size_t& pos, Poco::Data::AbstractExtractor::Ptr& extr_ptr )
	: extr_ptr_(extr_ptr), pos_(pos){}

TypeHandlerHelper<Poco::Data::AbstractPreparator::Ptr>::TypeHandlerHelper( std::size_t& pos, Poco::Data::AbstractPreparator::Ptr& prep_ptr ) 
	: pos_(pos), prep_ptr_(prep_ptr){}

DataTblBase::DataTblBase( DatabaseMgrBase& db_mgr, const std::string& tbl_name )
	: db_mgr_(db_mgr), tbl_name_(tbl_name)/*, select_(db_mgr.GetDbSession()), insert_(db_mgr.GetDbSession()), update_(db_mgr.GetDbSession()), delete_(db_mgr.GetDbSession())*/
{

}

DataTblBase::~DataTblBase()
{

}

bool DataTblBase::SelectData()
{
	if ( !db_mgr_.IsConnected() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "连接已断开，不能从数据库中恢复数据，表名：" << tbl_name_;
		return false;
	}
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;

	Poco::Data::Statement select(db_mgr_.GetDbSession());
	select << "select * from " << tbl_name_;
	return SelectDataInto(select);

	DATA_EXCEPTION_END(exec_helper);
	return exec_helper;
}

Poco::Data::Statement DataTblBase::InsertInto()
{
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement insert(db_mgr_.GetDbSession());
	insert << "insert into " << tbl_name_ << " values(" << StrPlaceholder() << ")";
	return insert;
}

Poco::Data::Statement DataTblBase::DeleteIf( const DataWhere_BindValue_& where_condition )
{
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement sta_delete(db_mgr_.GetDbSession());
	sta_delete << "delete from " << tbl_name_ << " where" << where_condition;
	return sta_delete;
}

Poco::Data::Statement DataTblBase::Update( const DataColumn_Placeholder_& columns, const DataWhere_BindValue_& where_condition )
{
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement update(db_mgr_.GetDbSession());
	update << "update " << tbl_name_ << " set" << columns << "where" << where_condition;
	return update;
}

const std::string& DataTblBase::GetTblName() const
{
	return tbl_name_;
}

Poco::Data::Statement& DataTblBase::SelectInto( Poco::Data::Statement& select )
{
	select << "select * from " << tbl_name_;
	return select;
}

Poco::Data::Statement& DataTblBase::SelectInto( Poco::Data::Statement& select, const DataColums_& columns )
{
	using namespace Poco::Data::Keywords;
	select << "select " << columns << " from " << tbl_name_;
	return select;
}

Poco::Data::Statement& DataTblBase::SelectInto( Poco::Data::Statement& select, const DataWhere_BindValue_& where_condition )
{
	using namespace Poco::Data::Keywords;
	select << "select * from " << tbl_name_ << " where" << where_condition;
	return select;
}

Poco::Data::Statement& DataTblBase::SelectInto( Poco::Data::Statement& select, const DataColums_& columns, const DataWhere_BindValue_& where_condition )
{
	using namespace Poco::Data::Keywords;
	select << "select " << columns << " from " << tbl_name_ << " where" << where_condition;
	return select;
}

const char* DataCondition::OperEq::Operate() const
{
	return "=";
}

const char* DataCondition::OperNeq::Operate() const
{
	return "!=";
}

DataWhere_Placeholder_::DataWhere_Placeholder_( const char* column_name, const OperBase_& oper )
{
	*this << ' ' << column_name << oper.Operate() << '?';
}

DataWhere_Placeholder_& DataWhere_Placeholder_::And( const char* column_name, const OperBase_& oper )
{
	*this << " and " << column_name << oper.Operate() << '?';
	return *this;
}

DataWhere_Placeholder_& DataWhere_Placeholder_::Or( const char* column_name, const OperBase_& oper )
{
	*this << " or " << column_name << oper.Operate() << '?';
	return *this;
}

DataWhere_BindValue_::DataWhere_BindValue_( const char* column_name, const OperBase_& oper, const char* colume_value )
{
	*this << ' ' << column_name << oper.Operate() << '\'' << colume_value << '\'';
}

DataWhere_BindValue_::DataWhere_BindValue_( const char* column_name, const OperBase_& oper, const std::string& colume_value )
{
	*this << ' ' << column_name << oper.Operate() << '\'' << colume_value << '\'';
}

DataWhere_BindValue_& DataWhere_BindValue_::And( const char* column_name, const OperBase_& oper, const char* colume_value )
{
	*this << " and " << column_name << oper.Operate() << '\'' << colume_value << '\'';
	return *this;
}

DataWhere_BindValue_& DataWhere_BindValue_::And( const char* column_name, const OperBase_& oper, const std::string& colume_value )
{
	*this << " and " << column_name << oper.Operate() << '\'' << colume_value << '\'';
	return *this;
}

DataWhere_BindValue_& DataWhere_BindValue_::Or( const char* column_name, const OperBase_& oper, const char* colume_value )
{
	*this << " or " << column_name << oper.Operate() << '\'' << colume_value << '\'';
	return *this;
}

DataWhere_BindValue_& DataWhere_BindValue_::Or( const char* column_name, const OperBase_& oper, const std::string& colume_value )
{
	*this << " or " << column_name << oper.Operate() << '\'' << colume_value << '\'';
	return *this;
}

Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataCondition& data_condition )
{
	return statement << data_condition.str();
}

Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataWhere_Placeholder_& data_condition )
{
	return statement << data_condition.str();
}

Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataWhere_BindValue_& data_condition )
{
	return statement << data_condition.str();
}

const DataCondition::OperEq DataCondition::eq;
const DataCondition::OperNeq DataCondition::neq;

DataColumn_Placeholder_::DataColumn_Placeholder_( const char* column_name )
{
	*this << ' ' << column_name << '=' << "? ";
}

DataColumn_Placeholder_::DataColumn_Placeholder_( const std::string& column_name )
{
	*this << ' ' << column_name << '=' << "? ";
}

DataColumn_Placeholder_& DataColumn_Placeholder_::Add( const char* column_name )
{
	*this << ',' << column_name << '=' << "? ";
	return *this;
}

DataColumn_Placeholder_& DataColumn_Placeholder_::Add( const std::string& column_name )
{
	*this << ',' << column_name << '=' << "? ";
	return *this;
}

DataColumn_Placeholder_& DataColumn_Placeholder_::operator , ( const char* column_name )
{
	return Add(column_name);
}

DataColumn_Placeholder_& DataColumn_Placeholder_::operator , ( const std::string& column_name )
{
	return Add(column_name);
}

DataColumn_Placeholder_ DataColumn( const char* column_name )
{
	return DataColumn_Placeholder_(column_name);
}

DataColumn_Placeholder_ DataColumn( const std::string& column_name )
{
	return DataColumn_Placeholder_(column_name);
}

Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataColumn_Placeholder_& column_data )
{
	statement << column_data.str();
	return statement;
}

DataColums_::DataColums_( const char* column_name )
{
	*this << column_name;
}

DataColums_::DataColums_( const std::string& column_name )
{
	*this << column_name;
}

DataColums_& DataColums_::Add( const char* column_name )
{
	*this << ',' << column_name;
	return *this;
}

DataColums_& DataColums_::Add( const std::string& column_name )
{
	*this << ',' << column_name;
	return *this;
}

DataColums_& DataColums_::operator , ( const char* column_name )
{
	return Add(column_name);
}

DataColums_& DataColums_::operator , ( const std::string& column_name )
{
	return Add(column_name);
}

Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataColums_& column_data )
{
	return statement << column_data.str();
}

DataColums_ DataColumns( const char* column_name )
{
	return DataColums_(column_name);
}

DataColums_ DataColumns( const std::string& column_name )
{
	return DataColums_(column_name);
}

DatabaseMgrBase::DatabaseMgrBase( const std::string& remote_ip, WORD ip_port, const std::string& db_name, 
								 const std::string& db_ac, const std::string& db_pwd ) :
	db_session_(DbSessionFactory::CreateSqlServer(DbSessionFactory::GenSqlServerConnectStr(remote_ip, ip_port, db_name, db_ac, db_pwd))), 
		db_connect_str_(DbSessionFactory::GenSqlServerConnectStr(remote_ip, ip_port, db_name, db_ac, db_pwd))
{
	connect_cnt_ = 0;
	pause_id_.id_ = (int)this;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "数据库[" << db_name << "]加载结果：" << IsConnected();
	BeginConnectDbSession(kDbReconnectTimeSpan);
}

void DatabaseMgrBase::BeginConnectDbSession( std::size_t timer_time )
{
	if ( connect_timer && connect_timer->IsClocking() )
		return;
	if (IsConnected())
		return;

	if ( timer_time <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "立即重连一次数据库";
		if (db_session_)
			db_session_->close();
		db_session_ = DbSessionFactory::CreateSqlServer(db_connect_str_);
		if (IsConnected())
		{
			LOG_O(Log_trace) << "重连数据库成功1";
			OnReconnected();
			connect_cnt_ = 0;
		}
		return;
	}
	if ( connect_cnt_ > 0 )
		return;
	++connect_cnt_;
	if (DoReconnectImpl(timer_time))
		return;
	connect_timer = CreateIoTimer(timer_time, true, [this, timer_time](const IoTimerPtr& timer){
		if (DoReconnectImpl(timer_time))
			return;
		timer->Continue();
	});
}

Poco::Data::Session& DatabaseMgrBase::GetDbSession()
{
	if ( !db_session_ )
	{
		assert(false);
		throw CmnPauseLogicException(pause_id_, "db_session不能为空");
	}
	return *db_session_;
}

bool DatabaseMgrBase::IsConnected()
{
	if ( !db_session_ )
		return false;
	return db_session_->isConnected();
}

CmnPauseLogicException::PauseIdentity DatabaseMgrBase::GetPauseId() const
{
	return pause_id_;
}

bool DatabaseMgrBase::DoReconnectImpl( std::size_t timer_time )
{
	if (IsConnected())
		return true;

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "开始重连数据库，次数：" << connect_cnt_++ << " 时间间隔：" << timer_time << " 链接字符串：" << db_connect_str_;
	if (db_session_)
		db_session_->close();
	db_session_ = DbSessionFactory::CreateSqlServer(db_connect_str_);
	if (IsConnected())
	{
		LOG_O(Log_trace) << "重连数据库成功";
		OnReconnected();
		connect_cnt_ = 0;
		return true;
	}
	return false;
}
