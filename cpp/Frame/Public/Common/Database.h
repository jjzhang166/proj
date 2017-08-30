#pragma once
/*
@author zhp
@date 2014/4/19 17:04
@purpose for database support
*/
#include <boost/asio.hpp>			//这个文件必须在使用Poco之前包含进来，否则就会有编译错误
#include <string>
#include <Poco/Data/Session.h>
#include <Poco/Data/AbstractBinder.h>
#include "LogOut.h"
#include <sstream>
#include <Common/CmnException.h>
#include <Net/IoTimer.h>
#include "../DependSelfLib.h"
#include <CmnMix/Cmn_TemplateUtility.h>

class DbSessionFactory{
public:
	typedef boost::optional<Poco::Data::Session> SessionOptionalT;
	//sql server
	static SessionOptionalT CreateSqlServer( const std::string& sql_connect_str );
	static std::string GenSqlServerConnectStr( const std::string& remote_ip, WORD ip_port, const std::string& db_name, 
		const std::string& db_ac, const std::string& db_pwd );
	//access
	static Poco::Data::Session CreateAccess( const std::string& user_dsn );
	//register ODBC
	static void RegisterODBC();
	//unregister ODBC
	static void UnregisterODBC();
};
//数据重连时间间隔
const std::size_t kDbReconnectTimeSpan = 1000 * 60 * 8;
//////////////////////////////////////////////////////////////////////////
//TypeHandlerHelper
template<typename Type_>
class TypeHandlerHelper;
template<>
class TypeHandlerHelper<Poco::Data::AbstractBinder::Ptr>{
public:
	TypeHandlerHelper( std::size_t& pos, Poco::Data::AbstractBinder::Ptr& bind_ptr, Poco::Data::AbstractBinder::Direction dir );
		
	template<typename T>
	const TypeHandlerHelper& Do( const T& var ) const{
		using namespace Poco::Data;
		TypeHandler<T>::bind(pos_++, var, bind_ptr_, bind_dir_);
		return *this;
	}

private:
	Poco::Data::AbstractBinder::Ptr&		bind_ptr_;
	Poco::Data::AbstractBinder::Direction	bind_dir_;
	std::size_t&							pos_;
};
template<>
class TypeHandlerHelper<Poco::Data::AbstractExtractor::Ptr>{
public:
	TypeHandlerHelper( std::size_t& pos, Poco::Data::AbstractExtractor::Ptr& extr_ptr );
	template<typename T>
	const TypeHandlerHelper& Do( T& var, const T& def_var ) const{
		using namespace Poco::Data;
		TypeHandler<T>::extract(pos_++, var, def_var, extr_ptr_);
		return *this;
	}

private:
	Poco::Data::AbstractExtractor::Ptr&		extr_ptr_;
	std::size_t&							pos_;
};
template<>
class TypeHandlerHelper<Poco::Data::AbstractPreparator::Ptr>{
public:
	TypeHandlerHelper( std::size_t& pos, Poco::Data::AbstractPreparator::Ptr& prep_ptr );
	template<typename T>
	const TypeHandlerHelper& Do( const T& var ) const{
		using namespace Poco::Data;
		TypeHandler<T>::prepare(pos_++, var, prep_ptr_);
		return *this;
	}

private:
	Poco::Data::AbstractPreparator::Ptr&	prep_ptr_;
	std::size_t&							pos_;
};

TypeHandlerHelper<Poco::Data::AbstractBinder::Ptr> MakeTypeHandlerH( std::size_t& pos, 
	Poco::Data::AbstractBinder::Ptr& bind_ptr, Poco::Data::AbstractBinder::Direction dir );
TypeHandlerHelper<Poco::Data::AbstractExtractor::Ptr> MakeTypeHandlerH( std::size_t& pos, Poco::Data::AbstractExtractor::Ptr& extr_ptr );
TypeHandlerHelper<Poco::Data::AbstractPreparator::Ptr> MakeTypeHandlerH( std::size_t& pos, Poco::Data::AbstractPreparator::Ptr& prep_ptr );
//////////////////////////////////////////////////////////////////////////
//data execute helper
class DataExecH{
public:
	DataExecH() : is_succeed_(true){}
	void operator () (){
		is_succeed_ = false;
	}
	bool Succeed() const{
		return is_succeed_;
	}
	operator bool () const{
		return Succeed();
	}

private:
	bool			is_succeed_;
};
#define DATA_EXCEPTION_BEGIN	try{ (void*)0
#define DATA_EXCEPTION_END(failed_func)		} DO_CATCH_THE_EXCEPTION( const Poco::Exception& e, {		\
		failed_func();	\
		LOG_ENTER_FUNC;	\
		LOG_O(Log_fatal) << e.displayText();	\
		db_mgr_.BeginConnectDbSession(kDbReconnectTimeSpan);	\
	}) DO_CATCH_THE_EXCEPTION( const CmnPauseLogicException& e, {	\
		if ( !e.IsIdentityEqual(db_mgr_.GetPauseId()) )	\
			throw;	\
		failed_func();	\
		LOG_ENTER_FUNC;	\
		LOG_O(Log_fatal) << e.what();	\
		db_mgr_.BeginConnectDbSession(kDbReconnectTimeSpan);	\
	})DO_CATCH_THE_EXCEPTION( const std::exception& e, {	\
		failed_func();	\
		LOG_ENTER_FUNC;	\
		LOG_O(Log_fatal) << e.what();	\
		assert(false);	\
	}) (void*)0	\
/**/

template<typename DataT>
struct DataBinder : public DataT{
	DataBinder(){}
	DataBinder( const DataT& other ) : DataT(other){}
	DataBinder( const DataBinder& other ) : DataT(other){}
	DataBinder& operator = ( const DataT& other ){
		__super::operator = (other);
		return *this;
	}
	DataBinder& operator = ( const DataBinder& other ){
		__super::operator = (other);
		return *this;
	}
};

namespace Detail{
	struct Stream_CalcCnt{
		Stream_CalcCnt(){
			cnt_ = 0;
		}
		template<typename T>
		Stream_CalcCnt& operator&(const T&){
			++cnt_;
			return *this;
		}

		std::size_t cnt_;
	};
	template<typename T>
	struct Stream_Call{
	public:
		Stream_Call(const T& ptr) : ptr_(ptr){}

		template<typename U>
		const Stream_Call& operator& (const U& var) const{
			ptr_.Do(var);
			return *this;
		}
	private:
		const T&		ptr_;
	};
	template<typename T, typename U>
	struct Stream_Call_Dlf{
	public:
		Stream_Call_Dlf(const T& ptr, U* data, const U* dlf_data) : ptr_(ptr), data_(data), dlf_data_(dlf_data){}

		template<typename T_>
		const Stream_Call_Dlf& operator& (T_& var) const{
			auto offset = (int)(const void*)&var - (int)(const void*)data_;
			assert(offset >= 0);
			auto def_v = (int)(const void*)dlf_data_ + offset;
			ptr_.Do<T_>(var, PointerToRef((const T_*)(void*)def_v));
			return *this;
		}
	private:
		const T&	ptr_;
		U*			data_;
		const U*	dlf_data_;
	};

	template<typename T>
	struct StrHolderHelper{
		static const char* get(){
			static std::string str;
			if (!str.empty())
				return str.c_str();
			T tmp;
			Stream_CalcCnt calc_cnt;
			tmp.serialize(calc_cnt);
			calc_cnt.cnt_;
			for (std::size_t i = 0; i != calc_cnt.cnt_;++i)
				str.append("?,");
			if (str.empty())
			{
				assert(false);
				return "";
			}
			str.pop_back();
			return str.c_str();
		}
	};
}

namespace Poco {
	namespace Data {
template <typename T >
class TypeHandler<DataBinder<T> >
{
public:
	typedef DataBinder<T> BindDataT;

	static std::size_t size(){
		static std::size_t cnt = 0;
		if (!cnt)
			return cnt;
		T tmp;
		Detail::Stream_CalcCnt calc_cnt;
		tmp.serialize(calc_cnt);
		cnt = calc_cnt.cnt_;
		assert(cnt);
		return cnt;
	}

	static void bind(std::size_t pos, const BindDataT& data_value, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir){
		auto type_handler = MakeTypeHandlerH(pos, pBinder, dir);
		::Detail::Stream_Call<decltype(type_handler)> stm(type_handler);
		const_cast<BindDataT&>(data_value).serialize(stm);
	}

	static void extract(std::size_t pos, BindDataT& data_value, const BindDataT& deflt, AbstractExtractor::Ptr pExtr){
		auto type_handler = MakeTypeHandlerH(pos, pExtr);
		::Detail::Stream_Call_Dlf<decltype(type_handler), BindDataT> stm(type_handler, &data_value, &deflt);
		data_value.serialize(stm);
	}

	static void prepare(std::size_t pos, const BindDataT& data_value, AbstractPreparator::Ptr pPrep){
		auto type_hander = MakeTypeHandlerH(pos, pPrep);
		::Detail::Stream_Call<decltype(type_hander)> stm(type_hander);
		const_cast<BindDataT&>(data_value).serialize(stm);
	}
};
}	//Data
}//Poco
//////////////////////////////////////////////////////////////////////////
class DataColums_ : public std::ostringstream{
public:
	DataColums_( const char* column_name );
	DataColums_( const std::string& column_name );

	DataColums_& Add( const char* column_name );
	DataColums_& Add( const std::string& column_name );

	DataColums_& operator , ( const char* column_name );
	DataColums_& operator , ( const std::string& column_name );
};
Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataColums_& column_data );

DataColums_ DataColumns( const char* column_name );
DataColums_ DataColumns( const std::string& column_name );

class DataColumn_Placeholder_ : public std::ostringstream{
public:
	DataColumn_Placeholder_( const char* column_name );
	DataColumn_Placeholder_( const std::string& column_name );

	DataColumn_Placeholder_& Add( const char* column_name );
	DataColumn_Placeholder_& Add( const std::string& column_name );

	DataColumn_Placeholder_& operator , ( const char* column_name );
	DataColumn_Placeholder_& operator , ( const std::string& column_name );
};
Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataColumn_Placeholder_& column_data );

DataColumn_Placeholder_ DataColumn( const char* column_name );
DataColumn_Placeholder_ DataColumn( const std::string& column_name );
//////////////////////////////////////////////////////////////////////////
class DataCondition : public std::ostringstream{
public:
	//operate
	class OperBase_{
	public:
		virtual const char* Operate() const = 0;
	};

private:
	//equal
	class OperEq : public OperBase_{
		virtual const char* Operate() const;
	};
	//not euqal
	class OperNeq : public OperBase_{
		virtual const char* Operate() const;
	};

public:
	static const OperEq				eq;
	static const OperNeq			neq;
};
Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataCondition& data_condition );

class DataWhere_Placeholder_ : public DataCondition{
public:
	DataWhere_Placeholder_( const char* column_name, const OperBase_& oper );
	DataWhere_Placeholder_& And( const char* column_name, const OperBase_& oper );
	DataWhere_Placeholder_& Or( const char* column_name, const OperBase_& oper );
};
Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataWhere_Placeholder_& data_condition );

class DataWhere_BindValue_ : public DataCondition{
public:
	DataWhere_BindValue_( const char* column_name, const OperBase_& oper, const char* colume_value );
	DataWhere_BindValue_( const char* column_name, const OperBase_& oper, const std::string& colume_value );
	template<typename T>
	DataWhere_BindValue_( const char* column_name, const OperBase_& oper, const T& colume_value ){
		*this << ' ' << column_name << oper.Operate() << colume_value;
	}

	DataWhere_BindValue_& And( const char* column_name, const OperBase_& oper, const char* colume_value );
	DataWhere_BindValue_& And( const char* column_name, const OperBase_& oper, const std::string& colume_value );
	template<typename T>
	DataWhere_BindValue_& And( const char* column_name, const OperBase_& oper, const T& colume_value ){
		*this << " and " << column_name << oper.Operate() << colume_value;
		return *this;
	}

	DataWhere_BindValue_& Or( const char* column_name, const OperBase_& oper, const char* colume_value );
	DataWhere_BindValue_& Or( const char* column_name, const OperBase_& oper, const std::string& colume_value );
	template<typename T>
	DataWhere_BindValue_& Or( const char* column_name, const OperBase_& oper, const T& colume_value ){
		*this << " or " << column_name << oper.Operate() << colume_value;
		return *this;
	}
};
Poco::Data::Statement& operator << ( Poco::Data::Statement& statement, const DataWhere_BindValue_& data_condition );

//DataWhere_Placeholder_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper );
DataWhere_BindValue_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper, const char* column_value );
DataWhere_BindValue_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper, const std::string& column_value );
template<typename T>
DataWhere_BindValue_ DataWhere( const char* column_name, const DataCondition::OperBase_& oper, const T& column_value ){
	return DataWhere_BindValue_(column_name, oper, column_value);
}
//////////////////////////////////////////////////////////////////////////
class DatabaseMgrBase{
protected:
	DatabaseMgrBase( const std::string& remote_ip, WORD ip_port, const std::string& db_name, 
		const std::string& db_ac, const std::string& db_pwd );

public:
	Poco::Data::Session& GetDbSession();
	//每隔一段时间链接一次
	void BeginConnectDbSession( std::size_t timer_time );
	bool IsConnected();
	CmnPauseLogicException::PauseIdentity GetPauseId() const;

protected:
	virtual void OnReconnected() = 0;

private:
	bool DoReconnectImpl( std::size_t timer_time );

protected:
	DbSessionFactory::SessionOptionalT		db_session_;

private:
	IoTimerPtr		connect_timer;
	int				connect_cnt_;
	std::string		db_connect_str_;
	CmnPauseLogicException::PauseIdentity	pause_id_;
};
//table base
class DataTblBase{
public:
	DataTblBase( DatabaseMgrBase& db_mgr, const std::string& tbl_name );
	virtual ~DataTblBase();

	//interface
public:
	const std::string& GetTblName() const;

	//data operate
public:
	//插入数据
	Poco::Data::Statement InsertInto();
	//删除数据
	Poco::Data::Statement DeleteIf( const DataWhere_BindValue_& where_condition );
	//更新数据
	Poco::Data::Statement Update( const DataColumn_Placeholder_& columns, const DataWhere_BindValue_& where_condition );
	//恢复数据
	bool SelectData();
	//选择数据
	Poco::Data::Statement& SelectInto( Poco::Data::Statement& select );
	Poco::Data::Statement& SelectInto( Poco::Data::Statement& select, const DataColums_& columns );
	Poco::Data::Statement& SelectInto( Poco::Data::Statement& select, const DataWhere_BindValue_& where_condition );
	Poco::Data::Statement& SelectInto( Poco::Data::Statement& select, const DataColums_& columns, const DataWhere_BindValue_& where_condition );

protected:
	//restore data
	virtual bool SelectDataInto( Poco::Data::Statement& select ) = 0;
	//string place holer
	virtual const char* StrPlaceholder() = 0;
	virtual void ClearData() = 0;

protected:
	DatabaseMgrBase&					db_mgr_;
	const std::string					tbl_name_;

	//session operator
public:
	//Poco::Data::Statement				insert_;
	//Poco::Data::Statement				delete_;
	//Poco::Data::Statement				update_;
	//Poco::Data::Statement				select_;
};
#define IMPL_DATA_OPER_(mem_name)		\
	protected:	\
		bool SelectDataInto( Poco::Data::Statement& select ){	\
			using namespace Poco::Data::Keywords;	\
			DataExecH exec_helper;		\
			DATA_EXCEPTION_BEGIN;		\
			mem_name.clear();		\
			select , into(mem_name), now;	\
			return true;	\
			DATA_EXCEPTION_END(exec_helper);	\
			return exec_helper;	\
		}	\
/**/
#define IMPL_DATA_OPER(DataT) IMPL_DATA_OPER_(datas_)	\
		public:	\
			typedef DataBinder<DataT> DataType;	\
			typedef std::vector<DataType> ContType;	\
			const ContType& GetDatas() const{	\
				return datas_;	\
			}	\
			void ClearData() { datas_.clear(); }	\
		protected:	\
			const char* StrPlaceholder(){	\
				return Detail::StrHolderHelper<DataT>::get();	\
			}	\
		ContType				datas_		\
/**/
/*
namespace Poco {
	namespace Data {
template <typename CharT, std::size_t CharCnt>
class TypeHandler<CharArray<CharT, CharCnt> >
{
public:
	typedef CharArray<CharT, CharCnt> CharArrayT;

public:
	static std::size_t size(){
		return 1;
	}

	static void bind( std::size_t pos, const CharArrayT& char_array, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir ){
		TypeHandler<std::string>::bind(pos, char_array, pBinder, dir);
	}

	static void extract( std::size_t pos, CharArrayT& char_array, const CharArrayT& deflt, AbstractExtractor::Ptr pExtr ){
		std::string str_tmp;
		TypeHandler<std::string>::extract(pos, str_tmp, deflt, pExtr);
		char_array = str_tmp;
	}

	static void prepare( std::size_t pos, const CharArrayT& char_array, AbstractPreparator::Ptr pPrep ){
		TypeHandler<std::string>::prepare(pos, char_array, pPrep);
	}
};
}}*/