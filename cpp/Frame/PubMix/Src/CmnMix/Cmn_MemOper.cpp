#include "stdafx.h"
#include <CmnMix/Cmn_MemOper.h>
#include <CommonX/CmnX_UsefulFunc.h>

MemOperBase::MemOperBase( void* addr ) : addr_(addr)
{
}

MemOperBase::MemOperBase()
{

}

void* MemOperApplyHelper__( unsigned int addr_base, const int* data_offset, int N_)
{
	if (IsUnValidPtr(addr_base))
	{
		assert(false);
		return nullptr;
	}
	char* mem_helper = (char*)(void*)addr_base;
	for ( pt_dword i = 0; i != N_; )
	{
		mem_helper = mem_helper + data_offset[i];
		if ( ++i == N_ )
			break;
		addr_base = *(unsigned int*)(void*)mem_helper;
		if (IsUnValidPtr(addr_base))
		{
			//assert(false);
			return nullptr;
		}
		mem_helper = (char*)(void*)addr_base;
	}
	return mem_helper;
}

MemOper_DoElse::MemOper_DoElse()
{

}

MemOper_DoElse::~MemOper_DoElse()
{

}

void* MemOper_DoElse::RetrieveAddr( const MemOperBase* mem_oper_base )
{
	return mem_oper_base->addr_;
}

void* MemOper_RetrieveAddr::RetrieveAddr( const MemOperBase* mem_oper_base )
{
	return *(void**)mem_oper_base->addr_;
}
