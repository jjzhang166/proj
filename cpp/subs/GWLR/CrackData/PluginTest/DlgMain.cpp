// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "PluginTest.h"
#include "DlgMain.h"
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Lua.h"
#include <ProtocolX/P_DefinesX.h>
#include "GloFns.h"

// DlgMain dialog

IMPLEMENT_DYNAMIC(DlgMain, CDialog)

DlgMain::DlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMain::IDD, pParent)
{

}

DlgMain::~DlgMain()
{
}

void DlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMain, CDialog)
	ON_BN_CLICKED(ID_BT_TEST, &DlgMain::OnBtTest)
	ON_BN_CLICKED(IDC_BT_RUN_SCRIPT, &DlgMain::OnBtRunScript)
END_MESSAGE_MAP()

BOOL DlgMain::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg)
	{
		if ( WM_KEYDOWN == pMsg->message )
		{
			if ( VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam )
				return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}


// DlgMain message handlers
void DlgMain::OnBtTest()
{
	LOG_O(Log_trace) << "bt test";
}

void DlgMain::OnBtRunScript()
{
	const auto& the_msg = CmnExceptionBase::DefCatchCall([](){
		LoadScriptFile("Loader");
		return kEmptyStr;
	});
	if ( !the_msg.empty() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << the_msg;
	}
}
