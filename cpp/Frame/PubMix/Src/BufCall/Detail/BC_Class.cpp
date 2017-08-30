#include "stdafx.h"
#include <BufCall/Detail/BC_Class.h>
#include <StdEx/TString.h>
#include <algorithm>

const bufcall::ClassProxy::stFuncInfo* bufcall::ClassProxy::FindMemFn(const ::Detail::BC_Uuid& fn_id) const
{
	auto it = mem_fns_.find(fn_id);
	if (it != mem_fns_.end())
		return &it->second;
	for ( auto& v : base_ )
	{
		if (!v.second)
		{
			v.second = ClassProxyMgr::GetMe().FindClass(v.first);
			if (!v.second)
			{
				assert(false);
				return nullptr;
			}
		}
		auto fn_info = v.second->FindMemFn(fn_id);
		if (fn_info)
			return fn_info;
	}
	return nullptr;
}

void bufcall::ClassProxy::AddMemFn(pt_csz mem_fn_name, BC_Function* fn)
{
	assert(mem_fn_name);
	assert(fn);
	const ::Detail::BC_Uuid& fn_id = GenUuidFromBuff(mem_fn_name, StdEx::TStrLen(mem_fn_name));

	auto it = mem_fns_.find(fn_id);
	if (it != mem_fns_.end())
	{
		assert(false);
		return;
	}
	stFuncInfo tmp;
	tmp.func_impl_ = FunctionT(fn);
#ifdef _DEBUG
	tmp.func_name_ = mem_fn_name;
#endif
	mem_fns_[fn_id] = tmp;
}

void bufcall::ClassProxy::AddBaseClass(ClassId cls_id, const ClassProxy* base)
{
	auto it = std::find_if(base_.begin(), base_.end(), [cls_id](const BaseClsInfo& v){
		return v.first == cls_id;
	});
	if (it != base_.end())
	{
		assert(false);
		return;
	}
	base_.push_back(std::make_pair(cls_id, base));
}

bufcall::ClassProxy::ClassProxy(pt_csz cls_name)
{
#ifdef _DEBUG
	cls_name_ = cls_name;
#endif
}

pt_csz bufcall::ClassProxy::BC_GetClassName() const
{
#ifdef _DEBUG
	return cls_name_;
#else
	return nullptr;
#endif
}

void bufcall::ClassProxyMgr::AddClass(ClassId class_id, ClassProxy* class_proxy)
{
	if (!class_proxy)
	{
		assert(false);
		return;
	}
	ClassProxyPtr ptr(class_proxy);
	auto it = class_proxy_.find(class_id);
	if (it != class_proxy_.end())
	{
		assert(false);
		return;
	}
	class_proxy_[class_id] = ptr;
}

bufcall::ClassProxy* bufcall::ClassProxyMgr::FindClass(ClassId class_id) const
{
	auto it = class_proxy_.find(class_id);
	if (it != class_proxy_.end())
	{
		return it->second.get();
	}
	return nullptr;
}

bufcall::ClassProxyMgr& bufcall::ClassProxyMgr::GetMe()
{
	static ClassProxyMgr me;
	return me;
}

bufcall::BC_RegClassImpl::BC_RegClassImpl(RegClassFnT f)
{
	assert(f);
	f();
}
