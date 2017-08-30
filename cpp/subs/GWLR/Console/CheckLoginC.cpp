#include "stdafx.h"
#include "CheckLoginC.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include "DlgUnion.h"
#include "Console.h"
#include <protect/Protect.h>
#include "TabOption.h"
#include "MainFrm.h"
#include "AntiCrack.h"
#include "ServerIpInfo.h"

CheckLoginC::CheckLoginC( const std::string& check_server_ip ) : 
	CmnCheckLoginC<CmnCheckLoginOperC, CheckLoginC>(check_server_ip, P_Port_CheckServer, theApp.public_key_, theApp.private_key_)
{
}

bool CheckLoginC::AfterLoginSucced( const std::function<void( SelfPtr& conne_self )>& func )
{
	auto dlg_login = DlgLogin::GetInstance();
	if ( !dlg_login )
	{
		assert(false);
		return false;
	}
	auto conne = CheckLoginC::GetSelfPtr();
	if ( !conne )
		return false;
	auto& user_oper = conne->GetUserOper();

	if (user_oper.GetLoginRes())
	{
		func(conne);
		return true;
	}
	
	std::string user_ac = CT2CA(dlg_login->GetAc());
	std::string user_pwd = CT2CA(dlg_login->GetPwd());
	user_oper.Login(false, user_ac.c_str(), user_pwd.c_str(), kConsoleMinVer);

	if (user_oper.GetLoginRes())
	{
		func(conne);
		return true;
	}
	return false;
}

bool CheckLoginC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	auto dlg_check_server = DlgCheckServerInfo::GetInstance();
	if (dlg_check_server)
	{
		dlg_check_server->ConnectSucceed();
	}
	return true;
}

void CheckLoginC::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);

	auto dlg_check_server = DlgCheckServerInfo::GetInstance();
	if (dlg_check_server)
	{
		dlg_check_server->SetConnecting(false);
	}
}

bool CheckLoginC::OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, const TimeT& end_day_time,
							 const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas )
{
	VMP_BEGIN_EX;
	p_supper::OnLoginRes(res, info, ac_pwd_machine_mix, new_cookie, end_day_time, ser_cur_time, addr_datas, str_addr_datas);

	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
	{
		assert(false);
		return true;
	}
	CheckLocalMachinePort();
	auto dlg_login = DlgLogin::GetInstance();
	if (dlg_login)
	{
		dlg_login->SetWndText(info);
	}
	auto dlg = DlgMultiLogin::GetInstance();
	if (dlg)
		dlg->OnLoginInfo(info);

	if ( !res )
		return true;
	auto main_frm = CMainFrame::GetInstance();
	if ( !main_frm )
		return true;

	tab_option->SetUserEndTime(Poco::Timestamp::fromEpochTime(end_day_time));
	main_frm->SetValidAcInfo(ac_pwd_machine_mix);

	GetIoService().post([=](){
		VMP_BEGIN_EX;
		tab_option->SetLoginSucceed(res);
		auto dlg_login = DlgLogin::GetInstance();
		if (dlg_login)
		{
			dlg_login->OnLoginSucceed();
		}
		auto main_wnd = CMainFrame::GetInstance();
		if (main_wnd)
		{
			main_wnd->BeginCheckAcValid();
		}
		VMP_END;
	});

	if (theApp.is_auto_run_)
	{
		theApp.is_auto_run_ = false;
		auto list_ctrl = PageAccount_List::GetInstance();
		if (list_ctrl)
			list_ctrl->OnMenuLoginSomeGame();
	}
	return res;
	VMP_END;
}

bool CheckLoginC::OnRemoveBind( bool res, pt_csz info )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "解绑返回结果->" << res;

	if (res)
	{
		//2秒后结束
		SetIoTimer(2 * 1000, []( IoTimer* ){
			auto tab_option = CTabOption::GetInstance();
			if (tab_option)
				tab_option->ClearLoginInfo();
		});
	}
	auto dlg = DlgRemoveBind::GetInstance();
	if (dlg)
	{
		dlg->SetWndText(info);
	}
	return true;
}

bool CheckLoginC::OnRegUser( bool res, pt_csz info )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "注册返回结果->" << res;
	auto dlg_reg = DlgReg::GetInstance();
	if (dlg_reg)
	{
		dlg_reg->SetWndText(info);
	}
	return true;
}

bool CheckLoginC::OnUserPay( bool res, pt_csz info )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "充值返回结果->" << res;
	auto dlg_pay = DlgPay::GetInstance();
	if (dlg_pay)
	{
		dlg_pay->SetWndText(info);
	}
	return true;
}

bool CheckLoginC::OnLeaveMsg( bool res, pt_csz info )
{
	auto dlg = DlgMsgBoard::GetInstance();
	if (dlg)
		dlg->SetWndText(info);
	return true;
}
