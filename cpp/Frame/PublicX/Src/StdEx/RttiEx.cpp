#include "stdafx.h"
#include <StdEx/RttiEx.h>
#include <Windows.h>
#include <cassert>
#include <DbgHelp.h>

#pragma comment(lib, "Dbghelp.lib")

using namespace StdEx;
extern "C" PVOID __CLRCALL_OR_CDECL __RTCastToVoid(PVOID inptr) throw(...);
extern "C" PVOID __CLRCALL_OR_CDECL __RTtypeid(PVOID inptr)	throw(...);
extern "C" PVOID __CLRCALL_OR_CDECL __RTDynamicCast(PVOID inptr, LONG VfDelta, PVOID SrcType, PVOID TargetType, BOOL isReference) throw(...);

void* StdEx::RttiEx::RTCastToVoid(void* inptr)
{
	return __RTCastToVoid(inptr);
}

void* StdEx::RttiEx::RTtypeid(void* inptr)
{
	return __RTtypeid(inptr);
}

void* StdEx::RttiEx::RTDynamicCast(void* inptr, long VfDelta, void* SrcType, void* TargetType, bool isReference)
{
	return __RTDynamicCast(inptr, VfDelta, SrcType, TargetType, isReference);
}

const std::type_info* StdEx::RttiEx::TypeId(pt_pcvoid obj_addr)
{
	return (const std::type_info*)RTtypeid((pt_pvoid)obj_addr);
}

const RttiEx::RTTICompleteObjectLocator* StdEx::RttiEx::GetCompleteObjectLocator(pt_pcvoid vtbl)
{
	if (!vtbl)
	{
		assert(false);
		return nullptr;
	}
	return (RTTICompleteObjectLocator*)(((void**)vtbl)[-1]);
}

const std::type_info* StdEx::RttiEx::VtblTypeId(pt_pcvoid vtbl)
{
	auto col = GetCompleteObjectLocator(vtbl);
	if (!col)
		return nullptr;
	if (!col->pTypeDescriptor)
	{
		assert(false);
		return nullptr;
	}
	return &col->pTypeDescriptor->AsTypeInfo();
}

int StdEx::RttiEx::TraBaseClass(pt_pcvoid obj_addr, const Fn_TraClassHierarchy& fn)
{
	if (!obj_addr)
	{
		assert(false);
		return 0;
	}
	return VtblTraBaseClass(*(pt_pcvoid*)obj_addr, fn);
}

int StdEx::RttiEx::VtblTraBaseClass(pt_pcvoid vtbl, const Fn_TraClassHierarchy& fn)
{
	if (!vtbl)
	{
		assert(false);
		return 0;
	}
	auto col = GetCompleteObjectLocator(vtbl);
	if (!col)
	{
		assert(false);
		return 0;
	}
	if (!col->pTypeDescriptor)
	{
		assert(false);
		return 0;
	}
	auto chd = col->pClassDescriptor;
	if (!chd)
		return 0;
	if (0 == chd->numBaseClasses)
		return 0;
	if (!chd->pBaseClassArray)
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (pt_dword idx = 0; idx < chd->numBaseClasses; ++idx)
	{
		auto bcd = chd->pBaseClassArray->arrayOfBaseClassDescriptors[idx];
		if (!bcd)
		{
			assert(false);
			break;
		}
		if (!bcd->pTypeDescriptor)
		{
			assert(false);
			break;
		}
		++cnt;
		if (!fn(bcd->pTypeDescriptor->AsTypeInfo()))
			break;
	}
	return cnt;
}

pt_csz StdEx::RttiEx::Name2ShortName(pt_csz full_name){
	if (!full_name)
	{
		assert(false);
		return nullptr;
	}
	const pt_csz pre_str[] = { "class ", "struct " };
	for (auto str : pre_str)
	{
		auto the_str = full_name;
		while (*str && *the_str)
		{
			if (*str != *the_str)
				goto LABLE_CONTINUE;
			++str;
			++the_str;
		}
		if (*str == 0)
			return the_str;
	LABLE_CONTINUE:
		continue;
	}
	return full_name;
}

pt_csz StdEx::RttiEx::TypeName(pt_pcvoid obj_addr)
{
	auto t = TypeId(obj_addr);
	if (!t)
		return "";
	return Name2ShortName(t->name());
}

pt_csz StdEx::RttiEx::VtblTypeName(pt_pcvoid vtbl)
{
	auto t = VtblTypeId(vtbl);
	if (!t)
		return "";
	return Name2ShortName(t->name());
}

pt_csz StdEx::RttiEx::TypeFullName(pt_pcvoid obj_addr)
{
	auto t = TypeId(obj_addr);
	if (!t)
		return "";
	return t->name();
}

pt_csz StdEx::RttiEx::VtblTypeFullName(pt_pcvoid vtbl)
{
	auto t = VtblTypeId(vtbl);
	if (!t)
		return "";
	return t->name();
}

pt_csz StdEx::RttiEx::TypeRawName(pt_pcvoid obj_addr)
{
	auto t = TypeId(obj_addr);
	if (!t)
		return "";
	return t->raw_name();
}

pt_csz StdEx::RttiEx::VtblTypeRawName(pt_pcvoid vtbl)
{
	auto t = VtblTypeId(vtbl);
	if (!t)
		return "";
	return t->raw_name();
}

std::string StdEx::RttiEx::UnDName(pt_csz symbol_name)
{
	assert(symbol_name);
	char buffer[0x3000];
	const int UNDNAME_TYPE_ONLY = 0x2800;
	if (*symbol_name == '.')
		++symbol_name;
	auto size = UnDecorateSymbolName(symbol_name, buffer, 100, UNDNAME_32_BIT_DECODE | UNDNAME_TYPE_ONLY
		| UNDNAME_NAME_ONLY
		);
	if (size <= 0)
	{
		assert(false);
		return std::string();
	}
	return std::string(buffer, size);
}

const std::type_info& StdEx::RttiEx::TypeDescriptor::AsTypeInfo() const
{
	return *(const std::type_info*)(const void*)this;
}
