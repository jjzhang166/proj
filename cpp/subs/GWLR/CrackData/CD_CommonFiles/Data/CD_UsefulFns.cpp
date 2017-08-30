#include "stdafx.h"
#include "CD_UsefulFns.h"
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_Const.h"

pt_dword CDF_GetCryGameBaseAddr()
{
	static auto dwCryGameBaseAddr = (pt_dword)GetModuleHandleA("CryGame.dll");
	assert(dwCryGameBaseAddr);
	return dwCryGameBaseAddr;
}

pt_dword CDF_Offset2CryGameAddr(pt_dword offset)
{
	auto addr = CDF_GetCryGameBaseAddr();
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return 0;
	}
	return offset + addr;
}

pt_dword CDF_GetCryActionBaseAddr()
{
	static auto dwCryActionBaseAddr = (pt_dword)GetModuleHandleA("CryAction.dll");
	assert(dwCryActionBaseAddr);
	return dwCryActionBaseAddr;
}

pt_dword CDF_Offset2CryActionAddr(pt_dword offset)
{
	auto addr = CDF_GetCryActionBaseAddr();
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return 0;
	}
	return offset + addr;
}

bool CDF_ValidStr(pt_csz str)
{
	if (IsUnValidPtr(str))
		return false;
	auto cnt = Cmnx_StrMaxLen(str, kMaxNameStrLen);
	if (cnt <= 0 || cnt >= kMaxNameStrLen)
		return false;
	return true;
}

pt_csz CDF_EmptyStr(pt_csz str)
{
	return Cmnx_EmptyStrAsMax(str, kMaxNameStrLen);
}

pt_csz CDF_NilStr(pt_csz str)
{
	return Cmnx_NilStrAsMax(str, kMaxNameStrLen);
}

pt_dword CDF_GetMhoClientBaseAddr()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("MHOClientBase.dll");
	assert(dwCryGameBaseAddr);
	return dwCryGameBaseAddr;
}

pt_dword CDF_Offset2MhoClientBaseAdd(pt_dword offset)
{
	auto addr = CDF_GetMhoClientBaseAddr();
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return 0;
	}
	return offset + addr;
}

pt_dword CDF_GetCryRenderD3d9()
{
	static auto the_addr = (pt_dword)GetModuleHandleA(kD3d9DllName);
	assert(the_addr);
	return the_addr;
}

pt_dword CDF_Offset2CryRenderD3d9(pt_dword offset)
{
	auto addr = CDF_GetCryRenderD3d9();
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return 0;
	}
	return offset + addr;
}
