#include "stdafx.h"
#include "CheckServerC.h"
#include "LoginDlg.h"
#include <protect/Protect.h>
#include "CSViewer.h"
#include <json/writer.h>
#include <fstream>
#include "CSViewerDlg.h"
#include "CfgStruct.h"

Connection_CS::Connection_CS( const std::string& svr_ip ) 
	: CmnCheckLoginC<CmnCheckLoginGrantOperC, Connection_CS>(svr_ip)
{
}


void Connection_CS::OnPromptInfo( pt_csz prompt_info )
{
	auto dlg = CCSViewerDlg::GetInstance();
	if (dlg)
	{
		dlg->SetWndText(prompt_info);
	}
	
	auto dlg_login = LoginDlg::GetInstance();
	if (dlg_login)
	{
		dlg_login->SetWndText(prompt_info);
	}
}

bool Connection_CS::AfterLoginSucced( const std::function<void( SelfPtr& conne_self )>& func )
{
	auto conne = Connection_CS::GetSelfPtr();
	if ( !conne )
		return false;
	auto& user_oper = conne->GetUserOper();
	if ( !user_oper.GetLoginRes() )
	{
		user_oper.Login(false, GetMyApp().GetUserAc().c_str(), GetMyApp().GetUserPwd().c_str(), 999999);
	}
	if (user_oper.GetLoginRes())
	{
		func(conne);
		return true;
	}
	return false;
}

bool Connection_CS::OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, 
							   const TimeT& end_day_time, const TimeT& ser_cur_time, 
							   const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	p_supper::OnLoginRes(res, info, ac_pwd_machine_mix, new_cookie, end_day_time, ser_cur_time, addr_datas, str_addr_datas, ac_type);

	LOG_O(Log_trace) << "登陆结果：" << info;
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnUserPay( bool res, pt_csz info )
{
	LOG_O(Log_trace) << "支付结果：" << info;
	OnPromptInfo(info);
	return true;
}

void Connection_CS::OnRecvCardInfo( const CardInfoT& cards_info )
{
	auto cstr_file_name = GetModulePath(nullptr);
	cstr_file_name += _T("cards_info.lua");
	std::string out_file_name = CT2CA(cstr_file_name);
	std::ofstream ofs(out_file_name, std::ios_base::out | std::ios_base::trunc);

	for ( const auto& a_card : cards_info )
	{
		ofs << a_card.first << ' ' << a_card.second << '\n';
	}
	LOG_O(Log_trace) << "生成成功";
}

void Connection_CS::OnRecvRegKey( const RegKeyInfoT& reg_keys )
{
	auto cstr_file_name = GetModulePath(nullptr);
	cstr_file_name += _T("reg_key_info.lua");
	std::string out_file_name = CT2CA(cstr_file_name);
	std::ofstream ofs(out_file_name, std::ios_base::out | std::ios_base::trunc);

	for ( const auto& akey : reg_keys )
	{
		ofs << akey << '\n';
	}
	LOG_O(Log_trace) << "生成成功";
}

void Connection_CS::OnRecvAcInfo(const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "接收到帐号信息";
	auto dlg = CCSViewerDlg::GetInstance();
	if (!dlg)
	{
		assert(false);
		return;
	}
	auto& list_ac = dlg->GetList_Ac();
	list_ac.DeleteAllItems();
	for (const auto& v : ac_info)
	{
		list_ac.AddAcInfo(v);
	}
	for (auto& v : ac_state)
	{
		list_ac.AddAcStateInfo(v);
	}
}

void Connection_CS::OnRecvCardDataInfo(const stData_Card::CardContT& card_info)
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "接收到卡信息";

	auto dlg = CCSViewerDlg::GetInstance();
	if ( !dlg )
	{
		assert(false);
		return;
	}

	auto& list_card = dlg->GetList_Card();
	list_card.DeleteAllItems();
	for (const auto& card_info : card_info)
	{
		list_card.AddCardInfo(card_info);
	}
}

bool Connection_CS::OnGrant_RemoveBind( bool res, pt_csz info )
{
	LOG_O(Log_trace) << "解绑结果：" << info;
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info )
{
	LOG_O(Log_trace) << "开卡结果：" << info;
	OnRecvCardInfo(card_info);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key )
{
	LOG_O(Log_trace) << "生成注册码结果：" << info;
	OnRecvRegKey(reg_key);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	OnRecvAcInfo(ac_info, ac_state);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	OnRecvCardDataInfo(card_info);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	OnRecvAcInfo(ac_info, ac_state);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	OnRecvCardDataInfo(card_info);
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_RollbackPay( bool res, pt_csz info )
{
	LOG_O(Log_trace) << "支付回滚结果：" << info;
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_SetAddrData( bool res, pt_csz info )
{
	LOG_O(Log_trace) << "设置数据结果：" << info;
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_SetStrData( bool res, pt_csz info )
{
	LOG_O(Log_trace) << "设置字符串数据结果：" << info;
	OnPromptInfo(info);
	return true;
}

bool Connection_CS::OnGrant_SetAcState(bool res, pt_csz info)
{
	LOG_O(Log_trace) << "设置帐号状态结果：" << info;
	OnPromptInfo(info);
	return true;
}
