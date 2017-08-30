// AutoUpdate.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include <assert.h>
#include <CmnMix/Cmn_FeatureCode.h>
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_UsefulClass.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMN_MY_DLLAPP_DEFINE(CAutoUpdateApp);

CAutoUpdateApp::CAutoUpdateApp()
{
#ifdef VER_GJ3_
	update_out_file_name_ = "FeatureCode_path/CD_Updates_gj3.h";
#elif VER_GJ_
	update_out_file_name_ = "FeatureCode_path/CD_Updates_gj.h";
#else
	update_out_file_name_ = "FeatureCode_path/CD_Updates.h";
#endif
	auto exe_module = GetModuleHandleA("PathOfExile.exe");
	//SetExeModule(exe_module, 0x115F000);
	SetExeModule(exe_module, 0x0150d000);

#ifdef VER_GF_
	auto addr = (void*)GetModuleHandleA("PathOfExileBase.dll");
	scan_base_.SetCodeRange(addr, (size_t)0x01568000);
	scan_base_.SetModuleBegin(addr);
#endif
}
