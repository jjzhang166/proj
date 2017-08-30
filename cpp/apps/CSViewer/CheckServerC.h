#pragma once
/*
@author zhp
@date 2014/4/21 19:39
@purpose for check server connection
*/
#include "Net/client.h"
#include "Common/UsefulClass.h"
#include <Net/CmnLogin.h>
#include "CfgStruct.h"


class Connection_CS : public CmnCheckLoginC<CmnCheckLoginGrantOperC, Connection_CS>{
public:
	Connection_CS( const std::string& svr_ip );

public:
	//响应提示信息
	void OnPromptInfo( pt_csz prompt_info );
	//登陆成功后做的事
	static bool AfterLoginSucced( const std::function<void( SelfPtr& conne_self )>& func );

private:
	bool OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, 
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, 
		const StrDatasT& str_addr_datas, pt_dword ac_type) override;
	bool OnUserPay( bool res, pt_csz info ) override;
	bool OnGrant_RemoveBind( bool res, pt_csz info ) override;
	bool OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info ) override;
	bool OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key ) override;
	bool OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_RollbackPay( bool res, pt_csz info ) override;
	bool OnGrant_SetAddrData( bool res, pt_csz info ) override;
	bool OnGrant_SetStrData( bool res, pt_csz info ) override;
	bool OnGrant_SetAcState(bool res, pt_csz info) override;

private:
	void OnRecvCardInfo( const CardInfoT& cards_info );
	//得到注册码
	void OnRecvRegKey( const RegKeyInfoT& reg_keys );
	//得到帐号信息
	void OnRecvAcInfo(const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state);
	//得到卡号信息
	void OnRecvCardDataInfo(const stData_Card::CardContT& card_info);
};
