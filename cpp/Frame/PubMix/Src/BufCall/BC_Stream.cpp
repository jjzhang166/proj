#include "stdafx.h"
#include <BufCall/BC_Stream.h>

BC_AsStreamR::BC_AsStreamR( const char* buffer, pt_dword buffer_size ) : impl_(buffer, buffer_size)
{
	VERIFY(impl_.Skip(sizeof(Detail::BC_Uuid)));
}

const Detail::BC_Uuid* BC_AsStreamR::GetFuncId() const
{
	return (const Detail::BC_Uuid*)impl_.GetBuffer();
}

pt_dword BC_AsStreamR::ReadSize() const
{
	return impl_.ReadSize();
}

pt_dword BC_AsStreamR::LeftSize() const
{
	return impl_.LeftSize();
}

bool BC_AsStreamR::Skip( pt_dword skip_size )
{
	return impl_.Skip(skip_size);
}

const char* BC_AsStreamR::CurPtr() const
{
	return impl_.CurPtr();
}

const std::string& BC_AsStreamW::GetStrBuffer() const
{
	return impl_.GetStrBuffer();
}

BC_AsStreamW::BC_AsStreamW( const Detail::BC_Uuid& func_id )
{
	impl_.Write(&func_id, sizeof(func_id));
}

void BC_AsStreamW::WriteTypeIdx( BC_TypeIndexT type_idx )
{
	impl_.Write(&type_idx, sizeof(type_idx));
}

void BC_AsStreamW::Fill(pt_dword size, unsigned char value)
{
	impl_.Fill(size, value);
}

void BC_AsStreamW::WriteValue(const void* src, pt_dword src_size)
{
	impl_.Write(src, src_size);
}

void BC_NoTypeIdxStreamW::WriteTypeIdx(BC_TypeIndexT type_idx)
{

}

void BC_NoTypeIdxStreamW::WriteValue(const void* src, pt_dword src_size)
{
	impl_.Write(src, src_size);
}

void BC_NoTypeIdxStreamW::Fill(pt_dword size, unsigned char value)
{
	impl_.Fill(size, value);
}

BC_NoTypeIdxStreamW::BC_NoTypeIdxStreamW(void* buffer_begin, unsigned int buffer_size) : impl_(buffer_begin, buffer_size)
{

}

pt_dword BC_NoTypeIdxStreamW::LeftSize() const
{
	return impl_.LeftSize();
}

pt_dword BC_NoTypeIdxStreamW::GetWriteSize() const
{
	return impl_.GetWriteSize();
}

BC_NoTypeIdxStreamR::BC_NoTypeIdxStreamR(pt_pcbyte buffer, pt_dword buffer_size) : impl_((pt_csz)buffer, buffer_size)
{

}

pt_dword BC_NoTypeIdxStreamR::LeftSize() const
{
	return impl_.LeftSize();
}

bool BC_NoTypeIdxStreamR::Skip(pt_dword skip_size)
{
	return impl_.Skip(skip_size);
}

const char* BC_NoTypeIdxStreamR::CurPtr() const
{
	return impl_.CurPtr();
}
