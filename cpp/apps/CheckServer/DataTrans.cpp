#include "stdafx.h"
#include "DataTrans.h"

void DataH_AllUseData::SelectAllDatas()
{
	if ( !db_mgr_.IsConnected() )
		return;
	DataType data_tmp;
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement select(db_mgr_.GetDbSession());
	SelectInto(select), into(data_tmp.id_), into(data_tmp.user_name_), into(data_tmp.user_pwd_),
		into(data_tmp.machine_code_), into(data_tmp.ok_ip_),
		into(data_tmp.date_line_), into(data_tmp.last_log_time_),
		into(data_tmp.last_log_ip_), into(data_tmp.last_pay_time_),
		into(data_tmp.add_time_), into(data_tmp.add_ip_),
		into(data_tmp.is_can_log_), into(data_tmp.remarks_), into(data_tmp.qq_),
		range(0, 1);
	
	while (!select.done())
	{
		select.execute();
		datas_.push_back(data_tmp);
	}
	DATA_EXCEPTION_END(exec_helper);
}

DataH_AllUseData::DataH_AllUseData( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "AllUseData")
{

}

void DataH_CardInfo::SelectAllDatas()
{
	if ( !db_mgr_.IsConnected() )
		return;
	DataType data_tmp;
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	Poco::Data::Statement select(db_mgr_.GetDbSession());
	SelectInto(select), into(data_tmp.id_), into(data_tmp.Card_id_), into(data_tmp.Card_pwd_),
		into(data_tmp.card_days_), into(data_tmp.user_name_),
		into(data_tmp.use_time_), into(data_tmp.add_time_),
		into(data_tmp.add_user_), into(data_tmp.add_ip_),
		into(data_tmp.is_used_), range(0, 1);

	while (!select.done())
	{
		select.execute();
		datas_.push_back(data_tmp);
	}
	DATA_EXCEPTION_END(exec_helper);
}

DataH_CardInfo::DataH_CardInfo( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "CardInfo")
{

}
