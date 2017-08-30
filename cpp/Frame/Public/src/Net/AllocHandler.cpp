#include "stdafx.h"
#include "Net/AllocHandler.h"

handler_allocator::handler_allocator() : in_use_(false)
{
}
void* handler_allocator::allocate(pt_dword size)
{
	if (!in_use_ && size < storage_.size)
	{
		in_use_ = true;
		return storage_.address();
	}
	else
	{
		return ::operator new(size);
	}
}

void handler_allocator::deallocate( void* pointer )
{
	if (pointer == storage_.address())
	{
		in_use_ = false;
	}
	else
	{
		::operator delete(pointer);
	}
}