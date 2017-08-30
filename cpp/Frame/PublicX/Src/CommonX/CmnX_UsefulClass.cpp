#include <stdafx.h>
#include <CommonX/CmnX_UsefulClass.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <cassert>

CmnX_CodeInfo::CmnX_CodeInfo()
{
	begin_addr_ = nullptr;
	end_addr_ = nullptr;
}

CmnX_CodeInfo::CmnX_CodeInfo( void* begin_addr, void* end_addr )
{
	assert(!IsUnValidPtr(begin_addr, -1));
	assert(!IsUnValidPtr(end_addr, -1));
	begin_addr_ = begin_addr;
	end_addr_ = end_addr;
}

CmnX_CodeInfo::CmnX_CodeInfo( const CmnX_CodeInfo& rhs )
{
	begin_addr_ = rhs.begin_addr_;
	end_addr_ = rhs.end_addr_;
}

CmnX_CodeInfo::CmnX_CodeInfo( void* begin_addr, unsigned int addr_size )
{
	assert(!IsUnValidPtr(begin_addr, -1));
	assert(addr_size > 0);
	begin_addr_ = begin_addr;
	end_addr_ = (char*)begin_addr + addr_size;
}

bool CmnX_CodeInfo::IsInRange( const void* addr ) const
{
	return addr >= begin_addr_ && addr < end_addr_;
}

int CmnX_CodeInfo::OffsetToBeginAddr( const void* addr ) const
{
	assert(IsInRange(addr) || end_addr_ == addr);
	return (int)addr - (int)begin_addr_;
}

bool CmnX_CodeInfo::ExistIntersection( const CmnX_CodeInfo& rhs ) const
{
	if ( begin_addr_ < rhs.begin_addr_ )
	{
		if ( end_addr_ > rhs.begin_addr_ )
			return true;
	}
	else
	{
		if ( rhs.end_addr_ > begin_addr_ )
			return true;
	}
	return false;
}

CmnX_CodeInfo CmnX_CodeInfo::CalcIntersection( const CmnX_CodeInfo& rhs ) const
{
	CmnX_CodeInfo res;
	if ( begin_addr_ < rhs.begin_addr_ )
	{
		if ( end_addr_ > rhs.begin_addr_ )
		{
			res.begin_addr_ = rhs.begin_addr_;
			if ( end_addr_ > rhs.end_addr_ )
				res.end_addr_ = rhs.end_addr_;
			else
				res.end_addr_ = end_addr_;
		}
	}
	else
	{
		if ( rhs.end_addr_ > begin_addr_ )
		{
			res.begin_addr_ = begin_addr_;
			if ( rhs.end_addr_ > end_addr_ )
				res.end_addr_ = end_addr_;
			else
				res.end_addr_ = rhs.end_addr_;
		}
	}
	return res;
}

CmnX_CodeInfo& CmnX_CodeInfo::operator=( const CmnX_CodeInfo& rhs )
{
	begin_addr_ = rhs.begin_addr_;
	end_addr_ = rhs.end_addr_;
	return *this;
}

int CmnX_CodeInfo::GetCodeSize() const
{
	return (int)end_addr_ - (int)begin_addr_;
}

CmnX_CodeInfo& CmnX_CodeInfo::operator -= ( const CmnX_CodeInfo& rhs )
{
	*(unsigned int*)&begin_addr_ -= (unsigned int)rhs.begin_addr_;
	*(unsigned int*)&end_addr_ -= (unsigned int)rhs.end_addr_;
	return *this;
}

CmnX_CodeInfo& CmnX_CodeInfo::operator += ( const CmnX_CodeInfo& rhs )
{
	*(unsigned int*)&begin_addr_ += (unsigned int)rhs.begin_addr_;
	*(unsigned int*)&end_addr_ += (unsigned int)rhs.end_addr_;
	return *this;
}

CmnX_CodeInfo& CmnX_CodeInfo::operator - ( const CmnX_CodeInfo& rhs ) const
{
	auto _Tmp = *this;
	return (_Tmp -= rhs);
}

CmnX_CodeInfo& CmnX_CodeInfo::operator + ( const CmnX_CodeInfo& rhs ) const
{
	auto _Tmp = *this;
	return (_Tmp += rhs);
}

bool CmnX_CodeInfo::IsValid() const
{
	if (IsUnValidPtr(begin_addr_, -1) || IsUnValidPtr(end_addr_, -1))
		return false;
	if ( end_addr_ < begin_addr_ )
		return false;
	return true;
}
