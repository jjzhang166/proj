#pragma once
/*
@author zhp
@date 2016/3/24 18:29
@purpose for feature code,auto update.
*/
#include "../DependSelfLib.h"
#include <CommonX/CmnX_UsefulClass.h>
#include <string>
#include <boost/optional.hpp>
#include <fstream>


class Cmn_FeatureCodeScan{
public:
	Cmn_FeatureCodeScan();
	virtual ~Cmn_FeatureCodeScan();

public:
	void SetCodeRange( const void* begin_addr, const void* end_addr );
	void SetCodeRange( const void* begin_addr, size_t code_size );
	void SetModuleBegin( const void* module_begin_addr );
	void SetModuleName(pt_csz module_name);

public:
	int OffsetToBeginAddr( const void* addr ) const;
	int OffsetToModuleBegin( const void* addr ) const;
	const void* FindFirst( const char* mark_code ) const;
	//occur_cnt：第几个出现这个特征码的
	const void* Find( const char* mark_code, unsigned char occur_cnt ) const;
	//获取值
	const void* ScanValue( const char* mark_code, unsigned char occur_cnt, short relative_offset, unsigned short value_byte_cnt, void* out_value_buffer ) const;
	//查找立即数
	template<typename T>
	boost::optional<T> ScanImme( const char* mark_code, short relative_offset, unsigned char occur_cnt = 1 ) const{
		boost::optional<T> res;
		char buffer[sizeof(T)];
		if ( !ScanValue(mark_code, occur_cnt, relative_offset, sizeof(T), buffer) )
		{
			//assert(false);
			return res;
		}
		res = *(T*)buffer;
		return res;
	}
	//查找内存地址
	const void* ScanAddr( const char* mark_code, short relative_offset, unsigned char occur_cnt = 1 ) const;
	//查找call的地址
	const void* ScanCall( const char* mark_code, short relative_offset, unsigned char occur_cnt = 1 ) const;
	//根据调用地址，得到函数的绝对地址，调用地址是以call为起始的地址
	static const void* CallAddr2AbsoluteAddr(const void* call_addr);


protected:
	virtual void CodeRangeChanged() = 0;
	virtual CmnX_CodeInfo ReserveMemory() const = 0;

private:
	const void* FindImpl( const char* mark_code, unsigned char occur_cnt ) const;
	const void* DoFindImpl( const CmnX_CodeInfo& real_code_range, unsigned char occur_cnt, const std::string& mark_code, const std::string& ignore_vec ) const;
	const void* DoScanValue( const char* mark_code, unsigned char occur_cnt, short relative_offset ) const;

protected:
	CmnX_CodeInfo		code_range_;
	const void*			module_addr_;
};

//直接用指针读内存
class Cmn_FeatureCodeScan_Direct : public Cmn_FeatureCodeScan{
private:
	void CodeRangeChanged();
	CmnX_CodeInfo ReserveMemory() const;
};

//用ReadProcessMemory读内存
class Cmn_FeatureCodeScan_ByRPM : public Cmn_FeatureCodeScan{
private:
	void CodeRangeChanged();
	CmnX_CodeInfo ReserveMemory() const;

private:
	mutable std::string			code_buffer_;
};

//feature code out fstream
class Cmn_FC_OFstream{
public:
	typedef Cmn_FC_OFstream& (*ManipulatorT)(Cmn_FC_OFstream&);

private:
	struct stManipEndlLog{
		explicit stManipEndlLog(const char* file_name, int line);

		const char*	file_name_;
		int			line_;
	};

public:
	explicit Cmn_FC_OFstream(const char *_Filename,
		std::ios_base::openmode _Mode = std::ios_base::out,
		int _Prot = (int)std::ios_base::_Openprot);
	explicit Cmn_FC_OFstream(const std::string& _Filename,
		std::ios_base::openmode _Mode = std::ios_base::out,
		int _Prot = (int)std::ios_base::_Openprot);

public:
	Cmn_FC_OFstream& operator << (ManipulatorT func);
	Cmn_FC_OFstream& operator << (const stManipEndlLog& endl_log);
	Cmn_FC_OFstream& operator << (const char* str);
	Cmn_FC_OFstream& operator << (const void* addr);
	Cmn_FC_OFstream& operator << (unsigned int v);
	Cmn_FC_OFstream& operator << (int v);

public:
	static stManipEndlLog MakeEndlLog(const char* file_name, int line);
	void close();

private:
	void SetDefWide();

private:
	std::ofstream			impl_;
};

#define FC_OFstream_EndlLog	Cmn_FC_OFstream::MakeEndlLog(__FILE__, __LINE__)
