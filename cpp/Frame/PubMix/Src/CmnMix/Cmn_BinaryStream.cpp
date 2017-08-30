#include "stdafx.h"
#include <CmnMix/Cmn_BinaryStream.h>
#include <CommonX/CmnX_UsefulFunc.h>

StrOutputBStream::StrOutputBStream()
{

}

const std::string& StrOutputBStream::GetStrBuffer() const
{
	return str_buffer_;
}

void StrOutputBStream::Write( const void* src, pt_dword src_size )
{
	if ( !src || src_size <= 0 )
	{
		assert(false);
		return;
	}
	str_buffer_.append((const char*)src, src_size);
}

void StrOutputBStream::Fill( pt_dword size, char value )
{
	str_buffer_.append(size, value);
}

StrInputBStream::StrInputBStream( const char* buffer, pt_dword buffer_size )
{
	cur_idx_ = 0;
	Append(buffer, buffer_size);
}

StrInputBStream::StrInputBStream( const std::string& str_buffer )
{
	cur_idx_ = 0;
	Append(str_buffer.c_str(), str_buffer.size());
}

void StrInputBStream::Append( const char* buffer, pt_dword buffer_size )
{
	if ( !buffer || buffer_size <= 0 )
	{
		assert(false);
		return;
	}
	str_buffer_.append(buffer, buffer_size);
}

void StrInputBStream::ReRead()
{
	cur_idx_ = 0;
}

bool StrInputBStream::Read( void* dst, pt_dword dst_size )
{
	if ( !dst || dst_size <= 0 )
	{
		assert(false);
		return false;
	}
	auto buffer_size = str_buffer_.size();
	if ( cur_idx_ >= buffer_size )
	{
		assert(false);
		return false;
	}
	auto left_size = buffer_size - cur_idx_;
	if ( dst_size > left_size )
	{
		assert(false);
		return false;
	}
	CopyMemory(dst, &str_buffer_[cur_idx_], dst_size);
	cur_idx_ += dst_size;
	return true;
}

const char* StrInputBStream::CurPtr() const
{
	return &str_buffer_[cur_idx_];
}

pt_dword StrInputBStream::LeftSize() const
{
	auto buffer_size = str_buffer_.size();
	if ( cur_idx_ >= buffer_size )
		return 0;
	return buffer_size - cur_idx_;
}

bool StrInputBStream::Skip( pt_dword skip_size )
{
	auto left_size = str_buffer_.size() - cur_idx_;
	if ( skip_size > left_size )
	{
		assert(false);
		return false;
	}
	cur_idx_ += skip_size;
	return true;
}

pt_dword StrInputBStream::ReadSize() const
{
	return cur_idx_;
}

StrInputBStreamBase::~StrInputBStreamBase()
{

}

StableInputBStream::StableInputBStream( const char* buffer, pt_dword buffer_size )
{
	buffer_begin_ = nullptr;
	cur_ptr_ = nullptr;
	buffer_end_ = nullptr;
	Append(buffer, buffer_size);
}

StableInputBStream::StableInputBStream( const char* buffer_begin, const char* buffer_end )
{
	buffer_begin_ = nullptr;
	cur_ptr_ = nullptr;
	buffer_end_ = nullptr;
	Append(buffer_begin, std::distance(buffer_begin, buffer_end));
}

StableInputBStream::StableInputBStream( const std::string& str_input )
{
	buffer_begin_ = nullptr;
	cur_ptr_ = nullptr;
	buffer_end_ = nullptr;
	Append(str_input.c_str(), str_input.size());
}

void StableInputBStream::Append( const char* buffer, pt_dword buffer_size )
{
	if ( buffer_begin_ || cur_ptr_ || buffer_end_ )
	{
		assert(false);
		return;
	}
	if ( !buffer || buffer_size < 0 )
	{
		assert(false);
		return;
	}
	buffer_begin_ = buffer;
	cur_ptr_ = buffer;
	buffer_end_ = buffer + buffer_size;
}

bool StableInputBStream::Read( void* dst, pt_dword dst_size )
{
	if ( !dst || dst_size <= 0 )
	{
		assert(false);
		return false;
	}
	auto left_size = LeftSize();
	if ( dst_size > left_size )
	{
		assert(false);
		return false;
	}
	CopyMemory(dst, cur_ptr_, dst_size);
	cur_ptr_ += dst_size;
	return true;
}

void StableInputBStream::ReRead()
{
	cur_ptr_ = buffer_begin_;
}

const char* StableInputBStream::CurPtr() const
{
	return cur_ptr_;
}

pt_dword StableInputBStream::LeftSize() const
{
	if ( !cur_ptr_ )
		return 0;
	if ( !buffer_end_ )
		return 0;
	return std::distance(cur_ptr_, buffer_end_);
}

bool StableInputBStream::Skip( pt_dword skip_size )
{
	auto left_size = LeftSize();
	if ( skip_size > left_size )
	{
		assert(false);
		return false;
	}
	cur_ptr_ += skip_size;
	return true;
}

const char* StableInputBStream::GetBuffer() const
{
	return buffer_begin_;
}

pt_dword StableInputBStream::ReadSize() const
{
	if ( !cur_ptr_ )
	{
		assert(false);
		return 0;
	}
	if ( cur_ptr_ < buffer_begin_ )
	{
		assert(false);
		return 0;
	}
	return (pt_dword)(cur_ptr_ - buffer_begin_);
}

stBStreamOutput::~stBStreamOutput()
{

}

void stBStreamOutput::Write( StrOutputBStream& ostm ) const
{

}

StrOutputBStream& operator << ( StrOutputBStream& ostm, const stBStreamOutput& msg )
{
	msg.Write(ostm);
	return ostm;
}

stBStreamInput::~stBStreamInput()
{

}

bool stBStreamInput::Read( StrInputBStreamBase& istm )
{
	assert(false);
	return false;
}

StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, stBStreamInput& recv_value )
{
	recv_value.Read(istm);
	return istm;
}

StableOutputBStream::StableOutputBStream(void* buffer_begin, unsigned int buffer_size) : buffer_(buffer_begin, buffer_size)
{
	write_size_ = 0;
}

void StableOutputBStream::Write(const void* src, pt_dword src_size)
{
	if (!src || src_size <= 0 )
	{
		assert(false);
		return;
	}
	auto the_size = LeftSize();
	if (the_size < 0)
	{
		assert(false);
		return;
	}
	if (the_size < src_size)
	{
		assert(false);
		return;
	}
	Cmnx_CpyMem((char*)buffer_.begin_addr_ + write_size_, src, src_size);
	write_size_ += src_size;
}

void StableOutputBStream::Fill(pt_dword size, unsigned char value)
{
	if (size <= 0)
	{
		assert(false);
		return;
	}
	auto the_size = LeftSize();
	if (the_size < 0)
	{
		assert(false);
		return;
	}
	if (the_size < size)
	{
		assert(false);
		return;
	}
	Cmnx_MemSetValue((char*)buffer_.begin_addr_ + write_size_, size, value);
	write_size_ += size;
}

pt_dword StableOutputBStream::LeftSize() const
{
	return (pt_dword)buffer_.GetCodeSize() - write_size_;
}

pt_dword StableOutputBStream::GetWriteSize() const
{
	return write_size_;
}
