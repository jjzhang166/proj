#pragma once
/*
@author zhp
@date 2014/5/9 6:32
@purpose for transform db data form one to another one
*/
#include <string>
#include "Common/Database.h"
#include "Poco/DateTime.h"
#include <Net/PocoMore.h>

struct stAllUseData{
	int					id_;
	std::string			user_name_;
	std::string			user_pwd_;
	std::string			machine_code_;
	std::string			ok_ip_;
	DateTimeLocal		date_line_;
	DateTimeLocal		last_log_time_;
	std::string			last_log_ip_;
	DateTimeLocal		last_pay_time_;
	DateTimeLocal		add_time_;
	std::string			add_ip_;
	int					is_can_log_;
	std::string			remarks_;
	std::string			qq_;
};

struct stCardInfo{
	int					id_;
	std::string			Card_id_;
	std::string			Card_pwd_;
	int					card_days_;
	std::string			user_name_;
	DateTimeLocal		use_time_;
	DateTimeLocal		add_time_;
	std::string			add_user_;
	std::string			add_ip_;
	int					is_used_;
};

class DataH_AllUseData : public DataTblBase{
public:
	DataH_AllUseData( DatabaseMgrBase& db_mgr );

	//interface
public:
	void SelectAllDatas();

public:
	typedef stAllUseData DataType;
	typedef std::vector<DataType> ContType;
	const ContType& GetDatas(){
		return datas_;
	}
	
protected:
	//restore data
	virtual bool SelectDataInto( Poco::Data::Statement& select ){ return false; }
	//string place holer
	virtual const char* StrPlaceholder(){ return ""; }

protected:
	ContType				datas_;
};

class DataH_CardInfo: public DataTblBase{
public:
	DataH_CardInfo( DatabaseMgrBase& db_mgr );

	//interface
public:
	void SelectAllDatas();

public:
	typedef stCardInfo DataType;
	typedef std::vector<DataType> ContType;
	const ContType& GetDatas(){
		return datas_;
	}

protected:
	//restore data
	virtual bool SelectDataInto( Poco::Data::Statement& select ){ return false; }
	//string place holer
	virtual const char* StrPlaceholder(){ return ""; }

protected:
	ContType				datas_;
};