#pragma once
/*
@author zhp
@date 2016/4/11 21:41

CDF->common data function
*/
#include <ProtocolX/P_BaseTypeX.h>

//////////////////////////////////////////////////////////////////////////
pt_dword CDF_GetCryGameBaseAddr();
pt_dword CDF_Offset2CryGameAddr(pt_dword offset);
pt_dword CDF_GetCryActionBaseAddr();
pt_dword CDF_Offset2CryActionAddr(pt_dword offset);
pt_dword CDF_GetMhoClientBaseAddr();
pt_dword CDF_Offset2MhoClientBaseAdd(pt_dword offset);
pt_dword CDF_GetCryRenderD3d9();
pt_dword CDF_Offset2CryRenderD3d9(pt_dword offset);
//////////////////////////////////////////////////////////////////////////
bool CDF_ValidStr(pt_csz str);
pt_csz CDF_EmptyStr(pt_csz str);
pt_csz CDF_NilStr(pt_csz str);