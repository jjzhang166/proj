#include "stdafx.h"
#include <BufCall/BufCall.h>
#include <protect/Protect.h>
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_Exception.h>

BC_AsStreamW BC_MakeP( pt_csz function_name )
{
	return BC_AsStreamW(GenUuidFromBuff(function_name, strlen(function_name)));
}

BC_BufDecode::BC_BufDecode()
{
}

bool BC_BufDecode::Decode( BC_AsStreamR& stm ) const
{
	VMP_BEGIN_EX;
	auto func_id = stm.GetFuncId();
	if ( !func_id )
	{
		assert(false);
		return false;
	}
	if ( stm.ReadSize() != sizeof(*func_id) )
	{
		assert(false);
		return false;
	}

	if (!cls_proxy_)
	{
		bufcall::ClassId cls_id = typeid(*this);
		auto& mgr = bufcall::ClassProxyMgr::GetMe();
		auto ptr = mgr.FindClass(cls_id);
		if (!ptr)
		{
			assert(false);
			return false;
		}
		cls_proxy_ = ptr;
		if (!cls_proxy_)
		{
			assert(false);
			return false;
		}
	}
	
	auto function = (*cls_proxy_)->FindMemFn(*func_id);
	if ( !function )
	{
		assert(false);
		return false;
	}
	std::string err_info;
	return OnCallImpl(
#ifdef _DEBUG
		function->func_name_
#else
		nullptr
#endif
		, *function->func_impl_, stm, &err_info);
	VMP_END;
}

bool BC_BufDecode::Decode( pt_pcvoid buffer, pt_dword buffer_size ) const
{
	if ( !buffer || buffer_size <= 0 )
	{
		assert(false);
		return false;
	}
	BC_AsStreamR stm((pt_csz)buffer, buffer_size);
	return Decode(stm);
}

bool BC_BufDecode::OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const
{
	if ( !function.DoCall(const_cast<BC_BufDecode*>(this), function_name, params_buf, err_info) )
	{
		LOG_O(Log_debug) << "BuffCallÊ§°Ü1£º" << *err_info;
		assert(false);
		return false;
	}
	return true;
}

bool BC_BufCallBase::OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const
{
	try{
		return p_supper::OnCallImpl(function_name, function, params_buf, err_info);
	}
	DO_CATCH_ALL_EXCEPTION({
		return false;
	})
}

bool BC_BufCallBase::ToCallSync(const BC_AsStreamW& stm, boost::optional<bool>& oper_res)
{
	assert(false);
	return false;
}

bool BC_BufCallBase_Except::OnCallImpl( pt_csz function_name, const BC_Function& function, BC_AsStreamR& params_buf, std::string* err_info ) const
{
	if (BC_BufDecode::OnCallImpl(function_name, function, params_buf, err_info))
		return true;
	if (err_info)
	{
		LOG_O(Log_debug) << "BuffCallÊ§°Ü2£º" << *err_info;
		assert(false);
		throw CmnExitIoThreadException(*err_info);
	}
	else
	{
		LOG_O(Log_debug) << "BuffCallÊ§°Ü3";
		assert(false);
		throw CmnExitIoThreadException("BuffCallÊ§°Ü4");
	}
	return false;
}
