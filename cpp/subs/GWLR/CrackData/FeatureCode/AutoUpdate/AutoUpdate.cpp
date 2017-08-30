// AutoUpdate.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AutoUpdate.h"
#include <assert.h>
#include <CmnMix/Cmn_FeatureCode.h>
#include "FeatureCodes.h"
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_UsefulClass.h>
#include "GloFns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CAutoUpdateApp

BEGIN_MESSAGE_MAP(CAutoUpdateApp, CWinApp)
END_MESSAGE_MAP()


// CAutoUpdateApp construction

CAutoUpdateApp::CAutoUpdateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAutoUpdateApp object

CAutoUpdateApp theApp;

BOOL CAutoUpdateApp::InitInstance()
{
	CWinApp::InitInstance();
	
	try{
		auto& gs = GlobalSetter::GetMe();
		gs.SetSelfModuleHandle(AfxGetModuleState()->m_hCurrentInstanceHandle);
		gs.SetEnableDump(true);

		LogDbgviewInstanceHelper log_dbgview_creator;
		log_dbgview_creator.ClearHolder();
		//////////////////////////////////////////////////////////////////////////
		LOG_O(Log_trace) << "开始更新";

		ScanCodeHelper scan_code;
		const auto def_dir = GetDefFileDir();

		/*{
			auto cd_updates = def_dir + "CD_Updates.h";
			Cmn_FC_OFstream ofs_updates(cd_updates.c_str(), std::ios_base::out | std::ios_base::trunc);
			scan_code.ScanCode_Updates_Old(ofs_updates);
		}*/

		if (!scan_code.ScanCode_Test())
			return FALSE;

		{
			auto cd_pt_update = def_dir + "CD_PT_Updates.h";
			Cmn_FC_OFstream ofs_pt_update(cd_pt_update.c_str(), std::ios_base::out | std::ios_base::trunc);
			scan_code.ScanCode_PT_Update(ofs_pt_update);
		}

		{
			auto pt_update_assert = def_dir + "CD_PT_UpdatesAssert.h";
			Cmn_FC_OFstream ofs_pt_update_assert(pt_update_assert.c_str(), std::ios_base::out | std::ios_base::trunc);
			scan_code.ScanCode_PT_UpdateAssert(ofs_pt_update_assert);
		}

		{
			auto data_analyse_update = def_dir + "DataAnalyseUpdate.h";
			Cmn_FC_OFstream ofs_data_analyse_update(data_analyse_update.c_str(), std::ios_base::out | std::ios_base::trunc);
			scan_code.ScanCode_DataAnalyseUpdate(ofs_data_analyse_update);
		}
	}
	catch( ... )
	{
		LOG_O(Log_error) << "更新出错了";
		assert(false);
	}
	return FALSE;
}
