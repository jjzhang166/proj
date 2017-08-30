#include "stdafx.h"
#include "GProtocolR.h"
#include "GClass.h"
#include "GProtocolS.h"
#include <Common/LogOut.h>
#include "GPlayer.h"
#include "../FuncObjC.h"
#include "GConst.h"
#include "../EventsName.h"
#include <luabind/object.hpp>
#include "GMap.h"

//////////////////////////////////////////////////////////////////////////
const boost::optional<WORD> g_hook_game_send_of_ret_offset = (WORD)5;
//////////////////////////////////////////////////////////////////////////

CMDR_AllowSwitchChannel::stServerInfo::stServerInfo()
{
	allm_net_address_ = 0;
	port_ = 0;
}

bool CMDR_AllowSwitchChannel::stServerInfo::Read( StrInputBStreamBase& istm )
{
	istm >> allm_net_address_ >> GameCharRuntimeA(str_ip_) >> port_;
	return true;
}

CMDR_AllowSwitchChannel::CMDR_AllowSwitchChannel()
{
	unknown_result_ = 0;
	stage_server_addresses_ = 0;
}

bool CMDR_AllowSwitchChannel::Read( StrInputBStreamBase& istm )
{
	istm >> unknown_result_ >> GameCharRuntime(channel_id_) >> GameCharRuntime(account_id_) >> GameCharRuntime(session_key_) >> stage_server_addresses_;
	istm >> server_info_ >> prev_channel_id_;

	if (channel_id_.empty())
	{
		assert(false);
		return false;
	}
	if (account_id_.empty())
	{
		assert(false);
		return false;
	}
	if (session_key_.empty())
	{
		assert(false);
		return false;
	}
	CMDS_Msg_SwitchChannel msg;
	msg.account_id_ = account_id_;
	msg.channel_name_ = account_id_;
	msg.session_key_ = session_key_;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

CMDR_MsgBase* CMDR_MsgBase::Clone() const
{
	assert(false);
	return nullptr;
}

void CMDR_MsgBase::OnReceived() const
{

}

boost::optional<WORD> CMDR_MsgBase::NewRetOffset() const
{
	return boost::optional<WORD>();
}

bool CMDR_Hook_EnteredCloneMap::Read( StrInputBStreamBase& istm )
{
	return true;
}

void CMDR_Hook_EnteredCloneMap::OnReceived() const
{
	using namespace GClass;
	GAmbient::GetMe().HandleEnteredFb();
}

CMDR_ChannelList::CMDR_ChannelList()
{
	channels_ = 0;
}

void CMDR_ChannelList::OnReceived() const
{
	using namespace GClass;
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	const auto& cont = channels_info_.GetCont();
	auto it = std::max_element(cont.begin(), cont.end(), []( const stChannelInfo& lhs, const stChannelInfo& rhs ){
		return lhs.CalcFreeWeight() < rhs.CalcFreeWeight();
	});
	if ( it == cont.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "找不到最空闲的频道";
		assert(false);
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "最空闲的频道->" << it->channel_id_;
	nor->SetChannelId(it->channel_id_);
	nor->SetSelectedRole(false);
	nor->GetLuaScript().OutputScriptInfo("等待初始化");
	GAllPlayersStaticInfo::GetMe().ClearPlayersInfo();
	GetIoService().post([](){
		GAmbient::GetMe().HandleEnteredSelectChannel();
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->GetLuaScript().OutputScriptInfo("初始化完成");
	});
}

bool CMDR_ChannelList::Read( StrInputBStreamBase& istm )
{
	istm >> channels_ >> channels_info_;
	return true;
}

CMDR_ChannelList::stChannelInfo::stChannelInfo()
{
	wApple_ChannelElementForClient = 0;
	dwChannelNumber = 0;
	enabled_ = false;
	capacity_ = 0;
	player_count_ = 0;
	dwMinPvpPoint = 0;
	dwMaxPvpPoint = 0;
	dwChannelPropertyId = 0;
}

bool CMDR_ChannelList::stChannelInfo::Read( StrInputBStreamBase& istm )
{
	istm >> wApple_ChannelElementForClient >> dwChannelNumber;
	istm >> GameCharRuntime(channel_id_) >> GameCharRuntime(title_);
	istm >> enabled_ >> capacity_ >> player_count_;
	assert(1 == (int)enabled_ || 0 == (int)enabled_);
	if ( capacity_ <= 0 )
	{
		assert(false);
		return false;
	}
	if ( player_count_ < 0 )
	{
		assert(false);
		return false;
	}
	istm >> dwMinPvpPoint >> dwMaxPvpPoint >> dwChannelPropertyId;
	return true;
}

int CMDR_ChannelList::stChannelInfo::CalcFreeWeight() const
{
	//assert(player_count_ <= capacity_);
	int free_weight = !!enabled_ * 10000 * 20;
	free_weight += capacity_ - player_count_;
	return free_weight;
}

CMDR_PartialListPlayersStaticInfo::CMDR_PartialListPlayersStaticInfo()
{
	wStaticInfo = 0;
	wApple_StaticPlayerInfo = 0;
	lPlayerSerial = 0;
	wCreatedDateTime = 0;
	wApple_DateTime = 0;
	wYear = 0;
	wMonth = 0;
	wDay = 0;
	wHour = 0;
	dwMinute = 0;
	dwSecond = 0;
	dwMilliSec = 0;
	bIsInfinity = false;
	is_deleted_ = false;
}

bool CMDR_PartialListPlayersStaticInfo::Read( StrInputBStreamBase& istm )
{
	istm >> GameCharRuntime(account_id_) >> wStaticInfo >> wApple_StaticPlayerInfo;
	istm >> GameCharRuntime(player_name_) >> lPlayerSerial >> GameCharRuntime(strPlayerGender);
	istm >> GameCharRuntime(player_job_name_) >> wCreatedDateTime >> wApple_DateTime;
	istm >> wYear >> wMonth >> wDay >> wHour >> dwMinute >> dwSecond >> dwMilliSec;
	istm >> bIsInfinity >> is_deleted_;
	if (account_id_.empty())
	{
		assert(false);
		return false;
	}
	if (player_name_.empty())
	{
		assert(false);
		return false;
	}
	if (strPlayerGender.empty())
	{
		assert(false);
		return false;
	}
	if (player_job_name_.empty())
	{
		assert(false);
		return false;
	}
	assert(1 == (int)is_deleted_ || 0 == (int)is_deleted_);
	return true;
}

void CMDR_PartialListPlayersStaticInfo::OnReceived() const
{
	using namespace GClass;
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	if ( GType::kGS_InGame == nor->GetGameState() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "角色已进入游戏，竟然还收到了选择角色时的角色信息，很有可能是手动点击退回到了选择角色界面";
		nor->SetGameState(GType::kGS_Invalid);
		nor->SetSelectedRole(false);
		GAllPlayersStaticInfo::GetMe().ClearPlayersInfo();
	}
	if (nor->IsSelectedRole())
		return;
	GAllPlayersStaticInfo::GetMe().AddPlayersInfo(*this);
}

bool CMDR_PartialListPlayersStaticInfo::ConvertToRoleInfo( CG_RoleInfo& role_info ) const
{
	using namespace GType;
	using namespace GClass;
	if (player_name_.empty())
	{
		assert(false);
		return false;
	}
	if (player_job_name_.empty())
	{
		assert(false);
		return false;
	}
	role_info.role_name_ = WideCharToGbk(player_name_);
	const auto& job_name_map = GClass::JobNameMap::GetMe();
	role_info.role_job_ = WideCharToGbk(job_name_map.En2ChName(player_job_name_));
	if (role_info.role_job_.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

CMDR_OnCreatePlayer::CMDR_OnCreatePlayer()
{
	result_ = kNameIsUsed;
}

bool CMDR_OnCreatePlayer::Read( StrInputBStreamBase& istm )
{
	using namespace GClass;
	istm >> result_;
	if ( kSucceed == result_ )
	{
		GetIoService().dispatch([](){
			auto nor = FuncObj_NormalC::GetInstance();
			if ( !nor )
			{
				assert(false);
				return;
			}

			nor->SetGameState(GType::kGS_Invalid);
			nor->SetSelectedRole(false);
			GAllPlayersStaticInfo::GetMe().ClearPlayersInfo();
		});
	}
	return true;
}

void CMDR_OnCreatePlayer::OnReceived() const
{
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	nor->OnCreatePlayerResult(result_);
}

CMDR_EndPartialListPlayers::CMDR_EndPartialListPlayers()
{
	dwConnectionSerial = 0;
	dwSendingType = 0;
}

bool CMDR_EndPartialListPlayers::Read( StrInputBStreamBase& istm )
{
	istm >> GameCharRuntime(account_id_) >> GameCharRuntime(selected_player_id_);
	istm >> dwConnectionSerial >> dwSendingType;
	if (account_id_.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

void CMDR_EndPartialListPlayers::OnReceived() const
{
	using namespace GClass;
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	if (nor->IsSelectedRole())
	{
		assert(!selected_player_id_.empty());
		return;
	}
	assert(selected_player_id_.empty());
	Json::Value json_array;
	if ( !GAllPlayersStaticInfo::GetMe().GenRolesInfoToJson(json_array) )
	{
		assert(false);
		return;
	}
	if (json_array.isNull())
		json_array.ConvertToEmptyArray();
	nor->GetSessionOper().ThisMemCall(nullptr, "SetRoleListInfo", &JsonEcho::MakeParams(json_array));
	GAmbient::GetMe().HandleEnteredSelectRole();
}

bool CMDR_Hook_SelectPlayer::Read( StrInputBStreamBase& istm )
{
#ifdef _DEBUG
	std::wstring role_name;
	istm >> GameCharRuntime(role_name);
	assert(!role_name.empty());
#endif
	return true;
}

void CMDR_Hook_SelectPlayer::OnReceived() const
{
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	nor->SetSelectedRole(true);
}

CMDR_OnMailList::stMailItem::stMailItem()
{
	mail_serial_ = 0;
	gold_ = 0;
}

bool CMDR_OnMailList::stMailItem::Read( StrInputBStreamBase& istm )
{
	using namespace GClass;
	WORD w_tmp;
	bool b_tmp;
	istm >> w_tmp >> mail_serial_ >> GameCharRuntime(title_) >> GameCharRuntime(sender_);
	istm >> b_tmp;
	//assert(0 == b_tmp || 1 == b_tmp);
	istm >> gold_ >> GameCharRuntime(sample_item_id_) >> b_tmp;
	//assert(0 == b_tmp || 1 == b_tmp);
	istm >> GameCharRuntime(mail_type_) >> w_tmp;
	istm >> MakeStrInputSkip<AppleDateTime>() >> b_tmp;
	//assert(0 == b_tmp || 1 == b_tmp);
	istm >> MakeStrInputSkipStr<wchar_t>();
	return true;
}

CMDR_OnMailList::CMDR_OnMailList()
{
	result_ = 0;
}

struct stCmdr_MailList : CMDR_HeaderBase{
	DWORD	dwResult;
	WORD	wList;
	WORD	warray;
	DWORD	dwsize;
	struct  
	{
		WORD			witem_i;
		WORD			wApple_MailHeader;
		LONGLONG		llMailSerial;			// 序号，用于接受邮件时需要
		WORD			wTitleLen;
		std::wstring	strTitle;
		WORD			wSenderIdLen;
		std::wstring	strSenderId;
		BYTE			bIsRead;
		LONGLONG		llGold;					// 金币	= 0，则表示没有金币
		WORD			wSampleItemIdLen;		// 如果 = 1，则表示没有图标没有物品，也可以表示附件没有物品
		std::wstring	strSampleItemId;		// 邮件图标物品
		BYTE			bHadAttchmentGot;
		WORD			wTypeLen;
		std::wstring	strType;				// 邮件类型 分系统(SYSTEM)和玩家(NORMAL)
		WORD			wExpireDate;
		APPLE_DATA_TIME	theDataTime;
		BYTE			bIsSampleItemSealed;
		WORD			wSampleItemGradeLen;
		std::wstring	strSampleItemGrade;
	}Item[100];
};
//static_assert(sizeof(stCmdr_MailList) == sizeof())

bool CMDR_OnMailList::Read( StrInputBStreamBase& istm )
{
	WORD list_tmp;
	istm >> result_ >> list_tmp;
	istm >> mail_items_;
	return true;
}

void CMDR_OnMailList::OnReceived() const
{
	auto self_ptr = (CMDR_MsgBase*)this;
	auto& shared_this = self_ptr->shared_from_this();
	assert(shared_this);
	auto& gps = GPlayerSelf::GetMe();
	gps.ResetMailList(std::static_pointer_cast<CMDR_OnMailList>(shared_this));
}

CMDR_OnMyProductList::CMDR_OnMyProductList()
{
	result_ = 0;
}

bool CMDR_OnMyProductList::Read( StrInputBStreamBase& istm )
{
	using namespace GClass;
	WORD w_tmp;
	istm >> result_ >> w_tmp >> MakeStrInputSkip<AppleDateTime>() >> w_tmp >> my_products_;
	return true;
}

void CMDR_OnMyProductList::OnReceived() const
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "CMDR_OnMyProductList";*/
	auto self_ptr = (CMDR_OnMyProductList*)this;
	auto& cont = self_ptr->my_products_.GetCont();
	std::sort(cont.begin(), cont.end(), []( const stMyProduct& lhs, const stMyProduct& rhs ){
		return lhs.oid_market_ < rhs.oid_market_;
	});

	auto& shared_this = self_ptr->shared_from_this();
	assert(shared_this);
	auto& gps = GPlayerSelf::GetMe();
	gps.ResetMyProductList(std::static_pointer_cast<CMDR_OnMyProductList>(shared_this));
}

CMDR_OnMyProductList::stMyProduct::stMyProduct()
{
	oid_market_ = 0;
	item_cnt_ = 0;
	price_ = 0;
	item_grade_ = -1;
}

bool CMDR_OnMyProductList::stMyProduct::Read( StrInputBStreamBase& istm )
{
	WORD w_tmp;
	DWORD dw_tmp;
	auto& skip_str = MakeStrInputSkipStr<wchar_t>();
	/*std::wstring str_tmp;
	GameCharRuntime skip_str(str_tmp);*/
	auto& skip_date_time = MakeStrInputSkip<GClass::AppleDateTime>();
	istm >> w_tmp;
	istm >> oid_market_;
	istm >> skip_str;
	istm >> skip_str;
	istm >> skip_str;
	istm >> dw_tmp;
	istm >> dw_tmp;
	istm >> GameCharRuntime(item_id_);
	istm >> item_cnt_;
	istm >> dw_tmp;
	istm >> item_grade_;
	istm >> dw_tmp;
	istm >> skip_str;
	istm >> skip_str;
	istm >> skip_str;
	istm >> skip_str;
	istm >> skip_str;
	istm >> dw_tmp;
	istm >> w_tmp;
	istm >> skip_date_time;
	istm >> price_;
	istm >> w_tmp;
	istm >> skip_date_time;
	istm >> dw_tmp;
	istm >> skip_str;
	return true;
}

CMDR_RecvMailContents::CMDR_RecvMailContents()
{
	result_ = -1;
	mail_serial_ = 0;
	mail_item_cnt_ = 0;
}

bool CMDR_RecvMailContents::Read( StrInputBStreamBase& istm )
{
	WORD w_tmp;
	istm >> result_ >> w_tmp >> w_tmp >> mail_serial_ >> MakeStrInputSkipStr<wchar_t>()
		>> w_tmp >> w_tmp >> mail_item_cnt_;
	return true;
}

void CMDR_RecvMailContents::OnReceived() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "mail content result->" << result_;
	if ( 0 != result_ )
		return;
	assert(mail_serial_);
	if ( mail_item_cnt_ > 0 )
		return;
	assert(0 == mail_item_cnt_);
}

CMDR_OnGetAttachments::CMDR_OnGetAttachments()
{
	result_ = -1;
	mail_serial_ = 0;
}

bool CMDR_OnGetAttachments::Read( StrInputBStreamBase& istm )
{
	istm >> result_ >> mail_serial_;
	return true;
}

void CMDR_OnGetAttachments::OnReceived() const
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "get attachments result->" << result_;*/
	//进入游戏后，在一定时间无法移动邮件物品
	//const int kCanNotMoveMailProduct = 6;
	if ( 0 == result_ )
		GPlayerSelf::GetMe().SucceedToGetAttachments(mail_serial_);
	else
	{
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
			return;
		events_mgr->ActiveEvent(kPEN_HandleDonAllMails);
	}
}

bool CMDR_OnRegisterProduct::Read( StrInputBStreamBase& istm )
{
	//已超出账号的每日交易次数
	const int kResult_PerAcLimit = 0x0D;
	//已超出角色交易次数
	const int kResult_PerRoleLimit = 0x0E;
	//在一定时间内无法移动物品
	const int kResult_CanNotMoveItem = 0x11;
	int result_v;
	istm >> result_v;
	if ( kResult_CanNotMoveItem == result_v )
		result_ = kRPR_CanNotMoveItem;
	else if ( kResult_PerAcLimit == result_v )
		result_ = kRPR_PerAcLimit;
	else if ( kResult_PerRoleLimit == result_v )
		result_ = kRPR_PerRoleLimit;
	else if ( 0 == result_v )
		result_ = kRPR_Succeed;
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "another result->" << result_v;
	}
	return true;
}

CMDR_OnRegisterProduct::CMDR_OnRegisterProduct()
{
	result_ = kRPR_Invalid;
}

void CMDR_OnRegisterProduct::OnReceived() const
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "CMDR_OnRegisterProduct";*/
	GPlayerSelf::GetMe().OnRegisterProductResult(*this);
}

CMDR_OnBuyProduct::CMDR_OnBuyProduct()
{
	result_ = -1;
}

bool CMDR_OnBuyProduct::Read( StrInputBStreamBase& istm )
{
	istm >> result_;
	return true;
}

void CMDR_OnBuyProduct::OnReceived() const
{
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
		return;
	const auto& cmn_event = events_mgr->FindByName(kPPEN_HandleTrade);
	if (cmn_event)
		cmn_event->ExecAsyc();
}

CMDR_OnRequestProductInfo::CMDR_OnRequestProductInfo()
{
	result_ = -1;
	average_price_ = 0;
	trade_count_ = 0;
	lower_price_ = 0;
}

bool CMDR_OnRequestProductInfo::Read( StrInputBStreamBase& istm )
{
	DWORD dw_tmp;
	istm >> result_ >> dw_tmp;
	istm >> average_price_ >> trade_count_ >> lower_price_;
	return true;
}

void CMDR_OnRequestProductInfo::OnReceived() const
{
	if ( 0 != result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "查询物品拍卖信息返回结果失败, result->" << result_;
	}
	auto self_ptr = (CMDR_OnRequestProductInfo*)this;
	auto& shared_this = self_ptr->shared_from_this();
	assert(shared_this);
	GPlayerSelf::GetMe().ResetCurRequestProductInfo(std::static_pointer_cast<CMDR_OnRequestProductInfo>(shared_this));
}

CMDR_OnRequestProductList::CMDR_OnRequestProductList()
{
	result_ = 0;
}

bool CMDR_OnRequestProductList::Read( StrInputBStreamBase& istm )
{
	DWORD dw_tmp;
	WORD w_tmp;
	istm >> result_ >> dw_tmp >> dw_tmp >> dw_tmp;
	istm >> w_tmp >> MakeStrInputSkip<GClass::AppleDateTime>() >> w_tmp;
	istm >> product_items_;
	return true;
}

void CMDR_OnRequestProductList::OnReceived() const
{
	auto self_ptr = (CMDR_OnRequestProductList*)this;
	auto& shared_this = self_ptr->shared_from_this();
	assert(shared_this);
	GPlayerSelf::GetMe().ResetCurRequestProductList(std::static_pointer_cast<CMDR_OnRequestProductList>(shared_this));
}

CMDR_OnRequestProductList::stProductItem::stProductItem()
{
	oid_market_ = 0;
	item_cnt_ = 0;
	item_grade_ = 0;
	price_ = 0;
}

bool CMDR_OnRequestProductList::stProductItem::Read( StrInputBStreamBase& istm )
{
	WORD w_tmp;
	DWORD dw_tmp;
	auto& skip_str = MakeStrInputSkipStr<wchar_t>();
	auto& skip_date_time = MakeStrInputSkip<GClass::AppleDateTime>();
	istm >> w_tmp >> oid_market_;
	istm >> skip_str >> skip_str >> GameCharRuntime(item_ch_name_);
	istm >> dw_tmp >> dw_tmp >> GameCharRuntime(item_id_) >> item_cnt_ >> dw_tmp >> item_grade_ >> dw_tmp;
	istm >> skip_str >> skip_str >> skip_str >> skip_str >> skip_str;
	istm >> dw_tmp >> w_tmp;
	istm >> w_tmp;
	istm >> skip_date_time >> price_ >> w_tmp >> skip_date_time;
	istm >> w_tmp >> skip_str;
	return true;
}

CMDR_OnLoginResult::CMDR_OnLoginResult()
{
	result_ = -1;
}

struct stCmdr_LoginResult : CMDR_HeaderBase{
	DWORD			dwResult;		//  登陆失败=0x0B  登陆成功=0
	WORD			wAccountIdLen;
	std::wstring	strAccountId;
	DWORD			dwAge;
	DWORD			dwShutdown;
	DWORD			dwKickTime;
	BYTE			bIsGm;
	BYTE			bIsSpecificAccount;
	BYTE			bPlayerNotCreated;
	WORD			wWorldNameLen;
	std::wstring	strWorldName;
	WORD			wWorldIdLen;
	std::wstring	strWorldId;
	LONGLONG		llOidAccount;
	BYTE			bIsReturnee;
	BYTE			bIsMembership;
	WORD			wReturnExpiration;
	APPLE_DATA_TIME	theReturnExpirationDataTime;
	WORD			wMembershipExpiration;
	APPLE_DATA_TIME	theMembershipExpirationDataTime;
	WORD			wCreated;
	APPLE_DATA_TIME	theCreatedDataTime;
	WORD			wBlockExpiration;
	APPLE_DATA_TIME	theBlockExpirationDataTime;
	DWORD			dwBlockType;
	WORD			wBlockReasonLen;
	std::wstring	strBlockReason;
	DWORD			dwBlockGrade;
	WORD			wBlockGradeExpiration;
	APPLE_DATA_TIME	theBlockGradeExpirationDataTime;
};
static_assert(sizeof(RECV_LOGIN) == sizeof(stCmdr_LoginResult), "");

bool CMDR_OnLoginResult::Read( StrInputBStreamBase& istm )
{
	istm >> result_;
	return true;
}

void CMDR_OnLoginResult::OnReceived() const
{
	if ( kBanAc == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "帐号被封";
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->BanAccount();
	}
	else if ( kSucceed != result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "游戏服务器判断登陆失败";
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->Disconnect();
	}
}

bool CMDR_Hook_RequestSwitchChannel::Read( StrInputBStreamBase& istm )
{
	return true;
}

boost::optional<WORD> CMDR_Hook_RequestSwitchChannel::NewRetOffset() const
{
	using namespace GClass;
	LOG_ENTER_FUNC;
	if (GAmbient::GetMe().IsInitBefore())
	{
		LOG_O(Log_trace) << "游戏前的初始化成功，允许选择频道了";
		static boost::optional<WORD> allowed = (WORD)0;
		return allowed;
	}
	else
	{
		LOG_O(Log_debug) << "游戏前的初始化尚未成功，不允许选择频道";
		return g_hook_game_send_of_ret_offset;
	}
}

bool CMDR_OnSellItemResult::Read( StrInputBStreamBase& istm )
{
	istm >> result_;
	return true;
}

void CMDR_OnSellItemResult::OnReceived() const
{
	if ( kRestrict == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "卖物品有了限制";
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->GetLuaScript().OutputScriptInfo("被限制");
		nor->GetSessionOper().ThisMemCall(nullptr, "RoleRestricted", nullptr);
	}
}

CMDR_OnSellItemResult::CMDR_OnSellItemResult()
{
	result_ = kSucceed;
}

CMDR_OnStartStageResult::CMDR_OnStartStageResult()
{
	result_ = kSucceed;
}

bool CMDR_OnStartStageResult::Read( StrInputBStreamBase& istm )
{
	WORD w_tmp;
	istm >> w_tmp;
	istm >> players_info_;
	istm >> w_tmp;
	GClass::AppleDateTime date_tmp;
	istm >> date_tmp;
	istm >> result_;
	return true;
}

void CMDR_OnStartStageResult::OnReceived() const
{
	if ( kRestrict == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "进副本有了限制";
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->GetLuaScript().OutputScriptInfo("被限制");
		nor->GetSessionOper().ThisMemCall(nullptr, "RoleRestricted", nullptr);
	}
	else if ( kLimitCnt == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "进入副本次数已达上限";
		GClass::GAmbient::GetMe().FailToEnterFb();
	}
}

bool CMDR_OnStartStageResult::stPlayersInfo::Read( StrInputBStreamBase& istm )
{
	istm >> MakeStrInputSkipStr<wchar_t>();
	return true;
}

bool CMDR_OnCreateNPC::Read( StrInputBStreamBase& istm )
{
	if (GMap::GetMe().IsTown())
		return true;
	auto& str_skip = MakeStrInputSkipStr<wchar_t>();
	istm >> GameCharRuntime(obj_id_);
	istm >> GameCharRuntime(team_id_);
	istm >> str_skip >> str_skip;
	DWORD dw_tmp;
	istm >> dw_tmp >> dw_tmp >> dw_tmp >> dw_tmp;
	istm >> dw_tmp >> dw_tmp >> dw_tmp >> dw_tmp;
	istm >> dw_tmp >> dw_tmp >> dw_tmp;
	float pos_x, pos_y, f_tmp;
	istm >> pos_x >> pos_y;
	istm >> f_tmp >> f_tmp >> f_tmp;
	istm >> f_tmp >> f_tmp >> f_tmp;
	istm >> is_dummy_;
	assert(1 == (int)is_dummy_ || 0 == (int)is_dummy_);
	/*istm >> f_tmp >> f_tmp >> f_tmp >> f_tmp >> f_tmp;
	istm >> f_tmp >> f_tmp >> f_tmp >> f_tmp >> f_tmp >> f_tmp;*/

	if (is_dummy_)
	{
		if ( team_id_ != kGameObjType_NPC )
			return true;
		has_valid_data_ = true;
		DWORD valid_x, valid_y;
		__asm
		{
			fld dword ptr[pos_x]
			mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
				call eax
				mov dword ptr[valid_x], eax
				fld dword ptr[pos_y]
			mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
				call eax
				mov dword ptr[valid_y], eax
		}
		valid_data_x_ = valid_x;
		valid_data_y_ = valid_y;
	}
	return true;
}

CMDR_OnCreateNPC::CMDR_OnCreateNPC()
{
	is_dummy_ = false;
	has_valid_data_ = false;
	valid_data_x_ = 0;
	valid_data_y_ = 0;
}

void CMDR_OnCreateNPC::OnReceived() const
{
	if ( !has_valid_data_ )
		return;
	assert(is_dummy_);
	using namespace GAddr;
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "存在ValidationData";
	//assert(false);
	g_through_room_valid_data_x = valid_data_x_;
	g_through_room_valid_data_y = valid_data_y_;
	g_through_room_dummy_exist = is_dummy_;
	if ( !is_dummy_ && kGameObjType_NPC == team_id_ )
	{
		auto ptr_this = (CMDR_OnCreateNPC*)this;
		GameObjMgr::GetMe().AddNpcBlacklist(std::move(ptr_this->obj_id_));
	}
}

CMDR_OnCreateNpcByClient::CMDR_OnCreateNpcByClient()
{
	task_npc_serial_ = 0;
}

bool CMDR_OnCreateNpcByClient::Read( StrInputBStreamBase& istm )
{
	istm >> task_npc_serial_;
	return true;
}

void CMDR_OnCreateNpcByClient::OnReceived() const
{
	if ( 0 == task_npc_serial_ )
		return;
	auto self_ptr = (CMDR_OnCreateNpcByClient*)this;
	auto& shared_this = self_ptr->shared_from_this();
	assert(shared_this);
	auto& gps = GPlayerSelf::GetMe();
	gps.ResetOnCreateNpcByClient(std::static_pointer_cast<CMDR_OnCreateNpcByClient>(shared_this));
}

bool CMDR_OnAcceptQuestResult::Read( StrInputBStreamBase& istm )
{
	istm >> result_;
	return true;
}

void CMDR_OnAcceptQuestResult::OnReceived() const
{
	if ( kRestrict == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "交任务有了限制";
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->GetLuaScript().OutputScriptInfo("被限制");
		nor->GetSessionOper().ThisMemCall(nullptr, "RoleRestricted", nullptr);
	}
}

CMDR_OnAcceptQuestResult::CMDR_OnAcceptQuestResult()
{
	result_ = kSucceed;
}

CMDR_OnPlayerDead::CMDR_OnPlayerDead()
{
	player_serial_ = 0;
}

bool CMDR_OnPlayerDead::Read( StrInputBStreamBase& istm )
{
	istm >> player_serial_;
	return true;
}

void CMDR_OnPlayerDead::OnReceived() const
{
	auto& gps = GPlayerSelf::GetMe();
	if ( gps.GetObjSerial() == player_serial_ )
	{
		gps.PlayerDied();
	}
}

bool CMDR_Hook_RequestProjectileHit::Read( StrInputBStreamBase& istm )
{
	SEND_SKILL_HIT_HEADER	theRequestProjectileHitHeader;
	istm >> theRequestProjectileHitHeader.wRequestHitInfos;
	istm >> theRequestProjectileHitHeader.warray;
	istm >> theRequestProjectileHitHeader.dwsize;

	SEND_REQUEST_PROJECTILE_HIT_BODY	theRequestProjectileHitBody;
	for (DWORD i = 0; i<theRequestProjectileHitHeader.dwsize; i++)
	{
		istm >> theRequestProjectileHitBody.wItem_i;
		istm >> theRequestProjectileHitBody.wHitInfo;
		istm >> theRequestProjectileHitBody.dwProjectileSerial;
		istm >> theRequestProjectileHitBody.dwDefenderSerial;
		istm >> theRequestProjectileHitBody.dwAttackerSerial;
		istm >> theRequestProjectileHitBody.wHitResultLen;
		istm >> theRequestProjectileHitBody.strHitResult;
		istm >> theRequestProjectileHitBody.dwHitType;
		istm >> theRequestProjectileHitBody.dwDeltaHp;
		istm >> theRequestProjectileHitBody.dwRandomSeed;
		istm >> theRequestProjectileHitBody.wHitProjectileInfo;
		istm >> theRequestProjectileHitBody.wAppleHitProjectileInfo;
		*(FLOAT*)istm.CurPtr() = -1;
		istm >> theRequestProjectileHitBody.fAttackPowerRate;

 		istm >> theRequestProjectileHitBody.fDamagePlusRateToSuperArmor;
		istm >> theRequestProjectileHitBody.fDamagePlusRateForCounter;
		istm >> theRequestProjectileHitBody.wPhysicalAttribute;
 		istm >> theRequestProjectileHitBody.wElementalAttributeLen;
 		istm >> theRequestProjectileHitBody.strElementalAttribute;
		istm >> theRequestProjectileHitBody.dwEventOptionSerial;
		istm >> theRequestProjectileHitBody.dwEventOptionDamageReduceRate;
		istm >> theRequestProjectileHitBody.bIsCounterAttack;
		istm >> theRequestProjectileHitBody.bIsHitSuperArmor;
		istm >> theRequestProjectileHitBody.dwProjectileHash;
		istm >> theRequestProjectileHitBody.bFirstHit;
		istm >> theRequestProjectileHitBody.dwActionHash;
		istm >> theRequestProjectileHitBody.dwSkillHash;
		istm >> theRequestProjectileHitBody.dwSkillLevel;
		istm >> theRequestProjectileHitBody.fProjectileScalex;
		istm >> theRequestProjectileHitBody.fProjectileScaley;
		istm >> theRequestProjectileHitBody.fProjectileScalez;
		istm >> theRequestProjectileHitBody.dwTimeStamp;
	}
	return true;
}

boost::optional<WORD> CMDR_Hook_RequestAddBuff::NewRetOffset() const
{
	return g_hook_game_send_of_ret_offset;
}

bool CMDR_Hook_RequestAddBuff::Read( StrInputBStreamBase& istm )
{
	return true;
}

bool CMDR_Hook_RequestChangeHp::Read( StrInputBStreamBase& istm )
{
	DWORD tmp;
	istm >> tmp;
	*(int*)istm.CurPtr() = -1;
	return true;
}

bool CMDR_Hook_ChangeSector::Read( StrInputBStreamBase& istm )
{
	return true;
}

boost::optional<WORD> CMDR_Hook_ChangeSector::NewRetOffset() const
{
	return g_hook_game_send_of_ret_offset;
}

bool CMDR_OnChangeVillage::Read( StrInputBStreamBase& istm )
{
	istm >> result_;
	return true;
}

void CMDR_OnChangeVillage::OnReceived() const
{
	if ( kRestrict == result_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "过城镇有了限制";
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->GetLuaScript().OutputScriptInfo("被限制");
		nor->GetSessionOper().ThisMemCall(nullptr, "RoleRestricted", nullptr);
	}
}

CMDR_OnChangeVillage::CMDR_OnChangeVillage()
{
	result_ = kSucceed;
}
