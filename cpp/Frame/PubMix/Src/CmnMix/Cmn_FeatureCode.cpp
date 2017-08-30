#include "stdafx.h"
#include <CmnMix/Cmn_FeatureCode.h>
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include <CommonX/CmnX_DllApi.h>
#include <iomanip>
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_Toolhelp.h>

void Cmn_FeatureCodeScan::SetCodeRange( const void* begin_addr, const void* end_addr )
{
	bool is_code_range_changed = code_range_.begin_addr_ != begin_addr || code_range_.end_addr_ != end_addr;
	code_range_.begin_addr_ = (void*)begin_addr;
	code_range_.end_addr_ = (void*)end_addr;
	if (is_code_range_changed)
		CodeRangeChanged();
}

void Cmn_FeatureCodeScan::SetCodeRange( const void* begin_addr, size_t code_size )
{
	return SetCodeRange(begin_addr, (const char*)begin_addr + code_size);
}

Cmn_FeatureCodeScan::~Cmn_FeatureCodeScan()
{

}

const void* Cmn_FeatureCodeScan::FindFirst( const char* mark_code ) const
{
	return FindImpl(mark_code, 1);
}

const void* Cmn_FeatureCodeScan::FindImpl( const char* mark_code, unsigned char occur_cnt ) const
{
	if (IsUnValidPtr(mark_code))
	{
		assert(false);
		return nullptr;
	}
	if ( !code_range_.IsValid() )
	{
		assert(false);
		return nullptr;
	}
	if ( occur_cnt <= 0 )
	{
		assert(false);
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	auto mark_code_len = strlen(mark_code);
	if ( mark_code_len % 2 )
	{
		assert(false);
		return nullptr;
	}
	mark_code_len /= 2;
	if ( mark_code_len <= 0 )
	{
		assert(false);
		return nullptr;
	}
	std::string byte_mark_code, ignore_vec;
	char c[3];
	c[2] = 0;
	for ( auto i = 0; i != mark_code_len; ++i )
	{
		c[0] = mark_code[i*2];
		c[1] = mark_code[i*2+1];
		if ( '?' == c[0] || '?' == c[1] )
		{
			if ( c[0] != c[1] )
			{
				assert(false);
				return nullptr;
			}
			ignore_vec.push_back(1);
		}
		else
			ignore_vec.push_back(0);
		byte_mark_code.push_back((char)::strtol(c, nullptr, 16));
	}
	//////////////////////////////////////////////////////////////////////////
	const auto& real_code_range = ReserveMemory();
	if ( real_code_range.begin_addr_ == real_code_range.end_addr_ )
	{
		assert(false);
		return nullptr;
	}
	return DoFindImpl(real_code_range, occur_cnt, byte_mark_code, ignore_vec);
}

const void* Cmn_FeatureCodeScan::DoFindImpl( const CmnX_CodeInfo& real_code_range, unsigned char occur_cnt, const std::string& mark_code, const std::string& ignore_vec ) const
{
	if ( !real_code_range.IsValid() )
	{
		assert(false);
		return nullptr;
	}
	assert(occur_cnt > 0);
	auto mark_code_size = mark_code.size();
	if ( mark_code_size <= 0 || mark_code_size != ignore_vec.size() )
	{
		assert(false);
		return nullptr;
	}

	int the_find_cnt = 0;
	auto the_addr = (const char*)real_code_range.begin_addr_;
	auto the_end_addr = (const char*)real_code_range.end_addr_ - mark_code_size;

	const char* ptr_ignore = ignore_vec.c_str();
	const char* ptr_mark_code = mark_code.c_str();

	for ( ; the_addr <= the_end_addr; ++the_addr )
	{		
		int idx = 0;
		for ( ; idx != mark_code_size; ++idx )
		{
			if (ptr_ignore[idx])
				continue;
			if ( ptr_mark_code[idx] != *(the_addr + idx) )
				goto LABLE_CONTINUE;
		}
		if ( idx >= (int)mark_code_size )
		{
			++the_find_cnt;
			if ( the_find_cnt < occur_cnt )
				goto LABLE_CONTINUE;
			the_addr = (const char*)code_range_.begin_addr_ + real_code_range.OffsetToBeginAddr(the_addr);
			return the_addr;
		}

LABLE_CONTINUE:
		continue;
	}

	return nullptr;
}

const void* Cmn_FeatureCodeScan::Find( const char* mark_code, unsigned char occur_cnt ) const
{
	return FindImpl(mark_code, occur_cnt);
}

const void* Cmn_FeatureCodeScan::ScanValue( const char* mark_code, unsigned char occur_cnt, short relative_offset, unsigned short value_byte_cnt, void* out_value_buffer ) const
{
	if ( value_byte_cnt < 1 )
	{
		assert(false);
		return nullptr;
	}
	auto the_addr = (const char*)DoScanValue(mark_code, occur_cnt, relative_offset);
	if (IsUnValidPtr(the_addr))
	{
		//assert(false);
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	const auto& real_code_range = ReserveMemory();
	if ( real_code_range.begin_addr_ == real_code_range.end_addr_ )
	{
		assert(false);
		return nullptr;
	}
	auto the_dst_addr = the_addr;
	the_addr = (const char*)real_code_range.begin_addr_ + code_range_.OffsetToBeginAddr(the_addr);
	if ( !real_code_range.IsInRange(the_addr) )
	{
		assert(false);
		return nullptr;
	}
	if ( !real_code_range.IsInRange(the_addr + value_byte_cnt - 1) )
	{
		assert(false);
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	if (out_value_buffer)
	{
		if ( !Cmnx_CpyMem(out_value_buffer, the_addr, value_byte_cnt) )
		{
			assert(false);
			return nullptr;
		}
	}
	return the_dst_addr;
}

const void* Cmn_FeatureCodeScan::DoScanValue( const char* mark_code, unsigned char occur_cnt, short relative_offset ) const
{
	auto the_addr = (const char*)Find(mark_code, occur_cnt);
	if (IsUnValidPtr(the_addr))
	{
		//assert(false);
		return nullptr;
	}
	the_addr += (int)relative_offset;
	if (IsUnValidPtr(the_addr))
	{
		assert(false);
		return nullptr;
	}
	return the_addr;
}

int Cmn_FeatureCodeScan::OffsetToBeginAddr( const void* addr ) const
{
	return code_range_.OffsetToBeginAddr(addr);
}

Cmn_FeatureCodeScan::Cmn_FeatureCodeScan()
{
	module_addr_ = nullptr;
}

int Cmn_FeatureCodeScan::OffsetToModuleBegin( const void* addr ) const
{
	if (IsUnValidPtr(module_addr_))
	{
		assert(false);
		return 0;
	}
	if ( !code_range_.IsInRange(addr) )
	{
		assert(false);
		return 0;
	}
	return (int)addr - (int)module_addr_;
}

const void* Cmn_FeatureCodeScan::ScanAddr( const char* mark_code, short relative_offset, unsigned char occur_cnt /*= 1 */ ) const
{
	return ScanValue(mark_code, occur_cnt, relative_offset, 1, nullptr);
}

const void* Cmn_FeatureCodeScan::ScanCall( const char* mark_code, short relative_offset, unsigned char occur_cnt /*= 1 */ ) const
{
	unsigned int call_addr = 0;
	auto the_call_addr = (unsigned int)ScanValue(mark_code, occur_cnt, relative_offset, sizeof(call_addr), &call_addr);
	if (IsUnValidPtr(the_call_addr))
		return nullptr;
	auto res = the_call_addr - 1 + 5 + call_addr;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return (const void*)res;
}

void Cmn_FeatureCodeScan::SetModuleBegin( const void* module_begin_addr )
{
	module_addr_ = module_begin_addr;
}

void Cmn_FeatureCodeScan::SetModuleName(pt_csz module_name)
{
	if (!module_name)
	{
		LOG_O(Log_debug) << "模块名不能为空";
		assert(false);
		return;
	}
	auto h_module = GetModuleHandleA(module_name);
	if (!h_module)
	{
		LOG_O(Log_debug) << "模块名不存在：" << module_name;
		assert(false);
		return;
	}
	CToolhelp tool_hlp(TH32CS_SNAPMODULE, GetCurrentProcessId());

	MODULEENTRY32 m_entry;
	m_entry.dwSize = sizeof(m_entry);
	if (!tool_hlp.ModuleFind(h_module, &m_entry))
	{
		LOG_O(Log_debug) << "没有找到模块：" << module_name;
		assert(false);
		return;
	}

	SetModuleBegin((const void*)h_module);
	SetCodeRange((const char*)h_module + 0x1000, (const char*)h_module + m_entry.modBaseSize - 0x1000);
}

const void* Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr(const void* call_addr)
{
	if (IsUnValidPtr(call_addr))
	{
		assert(false);
		return nullptr;
	}
	auto the_ptr = (pt_cbyte*)call_addr;
	if (*the_ptr != 0xe8)
	{
		assert(false);
		return nullptr;
	}
	++the_ptr;
	auto res = (pt_cbyte*)call_addr + 5 + *(pt_cdword*)the_ptr;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return (const void*)res;
}

void Cmn_FeatureCodeScan_Direct::CodeRangeChanged()
{

}

CmnX_CodeInfo Cmn_FeatureCodeScan_Direct::ReserveMemory() const
{
	return code_range_;
}

void Cmn_FeatureCodeScan_ByRPM::CodeRangeChanged()
{
	code_buffer_.clear();
}

CmnX_CodeInfo Cmn_FeatureCodeScan_ByRPM::ReserveMemory() const
{
	CmnX_CodeInfo res;
	if ( !code_range_.IsValid() )
	{
		assert(false);
		return res;
	}
	auto the_size = code_range_.GetCodeSize();
	if (code_buffer_.empty())
	{
		code_buffer_.resize(the_size, '\0');
		assert(code_buffer_.size() == the_size);
		if ( !ReadMemory(code_range_.begin_addr_, (void*)code_buffer_.c_str(), the_size) )
		{
			assert(false);
			return res;
		}
	}
	
	res.begin_addr_ = (void*)code_buffer_.c_str();
	res.end_addr_ = (void*)(code_buffer_.c_str() + the_size);
	return res;
}

Cmn_FC_OFstream::Cmn_FC_OFstream(const char *_Filename, std::ios_base::openmode _Mode, int _Prot) :
impl_(_Filename, _Mode, _Prot)
{
	using namespace std;
	impl_ << hex << showbase << uppercase;
	impl_ << internal << setfill('0');
}

Cmn_FC_OFstream::Cmn_FC_OFstream(const std::string& _Filename, std::ios_base::openmode _Mode, int _Prot)
	: Cmn_FC_OFstream(_Filename.c_str(), _Mode, _Prot)
{

}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(ManipulatorT func)
{
	if (!func)
	{
		assert(false);
		return *this;
	}
	return func(*this);
}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(const stManipEndlLog& endl_log)
{
	LOG_O(Log_trace) << "文件：" << endl_log.file_name_ << "，已更新到的行数：" << endl_log.line_;
	impl_ << std::endl;
	return *this;
}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(const char* str)
{
	impl_ << str;
	return *this;
}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(const void* addr)
{
	SetDefWide();
	impl_ << (unsigned int)addr;
	return *this;
}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(unsigned int v)
{
	SetDefWide();
	impl_ << (unsigned int)v;
	return *this;
}

Cmn_FC_OFstream& Cmn_FC_OFstream::operator<<(int v)
{
	if (v < 0)
	{
		v = -v;
		impl_ << "-";
	}
	SetDefWide();
	impl_ << v;
	return *this;
}

Cmn_FC_OFstream::stManipEndlLog Cmn_FC_OFstream::MakeEndlLog(const char* file_name, int line)
{
	return stManipEndlLog(file_name, line);
}

void Cmn_FC_OFstream::SetDefWide()
{
	impl_ << std::setw(sizeof(int) * 2 + 2);
}

void Cmn_FC_OFstream::close()
{
	impl_.close();
}

Cmn_FC_OFstream::stManipEndlLog::stManipEndlLog(const char* file_name, int line)
{
	assert(line >= 0);
	assert(file_name);
	file_name_ = file_name;
	line_ = line;
}
