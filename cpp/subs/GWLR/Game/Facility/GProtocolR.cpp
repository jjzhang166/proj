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
#include "GameObjMgr.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_ProtocolR.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>

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

bool CMDR_MsgBase::DoDirect( const CMDR_HeaderBase* msg )
{
	assert(false);
	return false;
}

const char* CMDR_MsgBase::NewRetAddr() const
{
	return nullptr;
}

bool CMDR_Hook_Hit::DoDirect( const CMDR_HeaderBase* msg )
{
	return false;
	/*auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return false;
	LOG_O(Log_trace) << "DoDirect";
	auto multi_hit_rate = norc->GetMultiHitRate();

	for ( int i = 0; i < multi_hit_rate; ++i )
	{
		MMO_gamehooksend(msg);
	}
	
	return false;*/
}

bool CMDR_MonsterDead::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_MonsterDead>();
	if (!real_msg)
		return false;
	monster_id_ = real_msg->monster_id_;
	assert(0 != monster_id_);
	return 0 != monster_id_;
}

CMDR_MonsterDead::CMDR_MonsterDead()
{
	monster_id_ = 0;
}

void CMDR_MonsterDead::OnReceived() const
{
	assert(0 != monster_id_);
	//怪物的死亡还是先注释掉吧，所出问题
	GameObjMgr::GetMe().AddDeadMonster(monster_id_);
}

bool CMDR_PlayerList::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_RoleList>();
	if (!real_msg)
		return false;
	players_info_.clear();
	stPlayerInfo player_info;
	for (auto& v : real_msg->role_list_)
	{
		player_info.player_id_ = v.dwPlayerId;
		player_info.player_lvl_ = v.nPlayerLevel;
		player_info.player_name_ = CDF_EmptyStr(v.szPlayerName);
		if (player_info.player_lvl_ > 0 && !player_info.player_name_.empty())
			players_info_.push_back(player_info);
	}
	assert(players_info_.size() == real_msg->role_list_.cnt_);
	return true;
}

void CMDR_PlayerList::OnReceived() const
{
	using namespace GType;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return;
	}
	auto pthis = (CMDR_PlayerList*)this;
	auto shared_this = PointerCast<CMDR_PlayerList>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	norc->SetRolesList(shared_this);

	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	Json::Value json_array;
	if ( !GenRolesInfoToJson(json_array) )
	{
		assert(false);
		return;
	}
	if (json_array.isNull())
		json_array.ConvertToEmptyArray();
	nor->GetSessionOper().ToCall(BC_MakeP("SetRoleListInfo")<<json_array);

	static bool exec_once = false;
	if ( !exec_once )
	{
		//LOG_O(Log_trace) << "角色列表2";
		exec_once = true;
		norc->SetGameState(kGS_SelectRole);
	}
}

bool CMDR_PlayerList::GenRolesInfoToJson( Json::Value& json_array ) const
{
	if (players_info_.empty())
	{
		json_array.clear();
		return true;
	}
	CG_RoleInfo role_info;
	Json::Value json_v;
	for ( const auto& role : players_info_ )
	{
		if ( !role.ConvertToRoleInfo(role_info) )
		{
			assert(false);
			return false;
		}
		if ( !role_info.ConvertToJson(json_v) )
		{
			assert(false);
			return false;
		}
		json_array.append(json_v);
	}
	return true;
}

bool CMDR_PlayerLine::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_SerLine>();
	if (!real_msg)
		return false;
	lines_info_.clear();
	stLineInfo line_info;
	for (auto& v : real_msg->ser_lines_)
	{
		line_info.line_idx_ = v.wLineIndex;
		line_info.line_lvl_ = (GType::enPlayerLineType)v.bLineLevel;
		lines_info_.push_back(line_info);
	}
	assert(lines_info_.size() >= 0 && lines_info_.size() <= 500);
	return true;
}

void CMDR_PlayerLine::OnReceived() const
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;

	auto pthis = (CMDR_PlayerLine*)this;
	auto shared_this = PointerCast<CMDR_PlayerLine>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	norc->on_ser_line_ = shared_this;
}

CMDR_OnCreatePlayer::CMDR_OnCreatePlayer()
{
	res_ = -1;
}

bool CMDR_OnCreatePlayer::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnCreateRole>();
	if (!real_msg)
		return false;
	res_ = real_msg->res_;
	return true;
}

void CMDR_OnCreatePlayer::OnReceived() const
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;

	auto pthis = (CMDR_OnCreatePlayer*)this;
	auto shared_this = PointerCast<CMDR_OnCreatePlayer>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	//创建成功后，需要清空下角色列表
	if ( 0 == res_ )
		norc->ClrRolesList();
	norc->SetCreateRoleRes(shared_this);
}

CMDR_OnEnterFb::CMDR_OnEnterFb()
{
	fb_type_ = kFT_Normal;
	error_res_ = -1;
}

bool CMDR_OnEnterFb::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnEnterFb>();
	if (!real_msg)
		return false;
	fb_type_ = (enFbType)kFT_Normal;
	error_res_ = real_msg->nError;
	return true;
}

void CMDR_OnEnterFb::OnReceived() const
{
	auto pthis = (CMDR_OnEnterFb*)this;
	auto shared_this = PointerCast<CMDR_OnEnterFb>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	GPlayerMe::GetMe().SetOnEnterFb(shared_this);
}

CMDR_OnFbInfo::CMDR_OnFbInfo()
{
	res_ = -1;
	fb_type_ = CD_En_FbType::kFT_Invalid;
}

bool CMDR_OnFbInfo::DoDirect( const CMDR_HeaderBase* msg )
{
	using namespace GType;
	auto real_msg = msg->MsgAs<stCD_MsgR_OnFbInfo>();
	if (!real_msg)
		return false;
	res_ = real_msg->dwRetValue;
	if ( 0 != res_ )
		return false;

	/*fb_type_ = (CD_En_FbType)task_info_ptr->dwTaskFbType;
	if ( CD_En_FbType::kFT_Sociaty != fb_type_ && CD_En_FbType::kFT_MiaoMiao != fb_type_ && CD_En_FbType::kFT_QianShou != fb_type_ )
		return false;*/
	fb_info_.clear();
	for (auto& fbs : real_msg->fb_info_)
	{
		stFbInfo fb_info;
		fb_info.fb_type_ = fbs.dwFbType;
		for (auto fb_id : fbs.fb_ids_)
		{
			fb_info.fb_ids_.push_back(fb_id);
		}
		fb_info_.push_back(fb_info);
	}
	return true;
}

void CMDR_OnFbInfo::OnReceived() const
{
	auto pthis = (CMDR_OnFbInfo*)this;
	auto shared_this = PointerCast<CMDR_OnFbInfo>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	if ( 0 == res_ )
		gpm.SetOnFbInfo(shared_this);
	else
		gpm.SetOnFbInfo(GPlayerMe::MsgOnFbInfoOptionalT(GPlayerMe::MsgOnFbInfoT()));
}

CMDR_OnFbInfo::FbIdsT CMDR_OnFbInfo::GetFbInfoBy( int fb_type ) const
{
	FbIdsT res;
	for ( auto& v : fb_info_ )
	{
		if ( fb_type == v.fb_type_ )
			std::copy(v.fb_ids_.begin(), v.fb_ids_.end(), std::back_inserter(res));
	}
	return res;
}

CMDR_OnFbInfo::FbIdsT CMDR_OnFbInfo::GetFbInfo_Cat() const
{
	return GetFbInfoBy(0x00000065);
}

bool CMDR_OnRefreshMySellList::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRefreshMySellList>();
	if (!real_msg)
		return false;
	return DoDirectImpl(real_msg->req_items_.begin(), real_msg->req_items_.cnt_);
}

void CMDR_OnRefreshMySellList::OnReceived() const
{
	auto pthis = (CMDR_OnRefreshMySellList*)this;
	auto shared_this = PointerCast<CMDR_OnRefreshMySellList>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnRefreshMySellList(shared_this);
}

bool CMDR_OnGainAuctionPageInfo::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnGainAuctionPageInfo>();
	if (!real_msg)
		return false;
	res_ = real_msg->dwRet;
	total_item_cnt_ = real_msg->nReqItemCount;
	cur_page_item_cnt_ = real_msg->req_items_.cnt_;
	assert(cur_page_item_cnt_ <= kAuctionPageCnt);
	return DoDirectImpl(real_msg->req_items_.begin(), cur_page_item_cnt_);
}

void CMDR_OnGainAuctionPageInfo::OnReceived() const
{
	auto pthis = (CMDR_OnGainAuctionPageInfo*)this;
	auto shared_this = PointerCast<CMDR_OnGainAuctionPageInfo>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnGainAuctionPageInfo(shared_this);
}

CMDR_OnGainAuctionPageInfo::CMDR_OnGainAuctionPageInfo()
{
	res_ = -1;
	total_item_cnt_ = 0;
	cur_page_item_cnt_ = 0;
}

bool CMDR_AuctionItemsInfo::DoDirectImpl(const stCD_MsgR_ReqItemByType* req_item_ptr, int product_item_cnt)
{
	items_info_.clear();
	if ( product_item_cnt < 0 )
	{
		assert(false);
		return false;
	}
	for ( auto i = 0; i < product_item_cnt; ++i, ++req_item_ptr )
	{
		auto the_item = CD_FuncCall::FindReqItem(req_item_ptr->ukn_, &req_item_ptr->ukn_id_);
		if (IsUnValidPtr(the_item))
		{
			assert(false);
			continue;
		}
		if (IsUnValidPtr(the_item->item_db_))
		{
			assert(false);
			continue;
		}
		stAuctionItemInfo item_info;
		item_info.req_item_id_ = req_item_ptr->dwReqItemById;
		item_info.req_item_key_ = req_item_ptr->dwReqItemByKey;
		item_info.req_item_by_player_name_.assign(req_item_ptr->szReqItemByPlayerName);
		item_info.item_id_ = the_item->item_db_->item_id_;
		item_info.item_cnt_ = the_item->item_cnt_;
		item_info.begin_price_ = req_item_ptr->nReqItemByPrice1;
		item_info.smart_price_ = req_item_ptr->nReqItemByPrice2;
		item_info.jing_pai_price_ = req_item_ptr->nReqItemByPrice3;
		items_info_.push_back(item_info);
	}
	return true;
}

void CMDR_AuctionItemsInfo::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "auction items info,total cnt:" << items_info_.size();
	for ( auto& v : items_info_ )
	{
		v.DebugInfo();
	}
}

const stAuctionItemInfo* CMDR_AuctionItemsInfo::FindByName( const std::string& role_name )
{
	if (role_name.empty())
		return nullptr;
	for ( auto& v : items_info_ )
	{
		if ( role_name == v.req_item_by_player_name_ )
			return &v;
	}
	return nullptr;
}

const stAuctionItemInfo* CMDR_AuctionItemsInfo::FindByLimitPrice( int price_limit )
{
	assert(price_limit > 0);
	for ( auto& v : items_info_ )
	{
		if ( price_limit >= v.smart_price_ )
			return &v;
	}
	return nullptr;
}

int CMDR_AuctionItemsInfo::QuerySoldCnt( int item_id )
{
	int cnt = 0;
	for ( auto& v : items_info_ )
	{
		if ( v.item_id_ == item_id )
			++cnt;
	}
	return cnt;
}

void CMDR_AuctionItemsInfo::Sort()
{
	std::sort(items_info_.begin(), items_info_.end());
}

CMDR_OnSafeTime::CMDR_OnSafeTime()
{
	left_time_ = -1;
}

bool CMDR_OnSafeTime::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSafeTime>();
	if (!real_msg)
		return false;
	left_time_ = real_msg->left_time_;
	return true;
}

void CMDR_OnSafeTime::OnReceived() const
{
	auto pthis = (CMDR_OnSafeTime*)this;
	auto shared_this = PointerCast<CMDR_OnSafeTime>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnLeftTime(shared_this);
}

CMDR_OnBuyAuction::CMDR_OnBuyAuction()
{
	res_ = -1;
}

bool CMDR_OnBuyAuction::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnBuyAuction>();
	if (!real_msg)
		return false;
	res_ = real_msg->dwBuyItemRet;
	return true;
}

void CMDR_OnBuyAuction::OnReceived() const
{
	auto pthis = (CMDR_OnSafeTime*)this;
	auto shared_this = PointerCast<CMDR_OnBuyAuction>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnBuyAuction(shared_this);
}

CMDR_OnAcceptMail::CMDR_OnAcceptMail()
{
	res_ = -1;
	error_ = -1;
}

bool CMDR_OnAcceptMail::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnAcceptMail>();
	if (!real_msg)
		return false;
	res_ = real_msg->nRetValue;
	error_ = real_msg->nRetError;
	return true;
}

void CMDR_OnAcceptMail::OnReceived() const
{
	auto pthis = (CMDR_OnAcceptMail*)this;
	auto shared_this = PointerCast<CMDR_OnAcceptMail>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnAcceptMail(shared_this);
}

CMDR_OnOpenNpc::CMDR_OnOpenNpc()
{
	res_ = -1;
}

bool CMDR_OnOpenNpc::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnOpenNpc>();
	if (!real_msg)
		return false;
	res_ = real_msg->res_;
	return true;
}

void CMDR_OnOpenNpc::OnReceived() const
{
	auto pthis = (CMDR_OnOpenNpc*)this;
	auto shared_this = PointerCast<CMDR_OnOpenNpc>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnOpenNpc(shared_this);
}

CMDR_OnSoldAuction::CMDR_OnSoldAuction()
{
	res_ = -1;
}

bool CMDR_OnSoldAuction::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSoldAuction>();
	if (!real_msg)
		return false;
	res_ = (int)real_msg->dwRetValue;
	return true;
}

void CMDR_OnSoldAuction::OnReceived() const
{
	auto pthis = (CMDR_OnSoldAuction*)this;
	auto shared_this = PointerCast<CMDR_OnSoldAuction>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnSoldAuction(shared_this);
}

void CMDR_OnSoldItemToShop::OnReceived() const
{
	auto pthis = (CMDR_OnSoldItemToShop*)this;
	auto shared_this = PointerCast<CMDR_OnSoldItemToShop>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnSoldToShop(shared_this);
}

CMDR_OnSoldItemToShop::CMDR_OnSoldItemToShop()
{
	res_ = -1;
}

bool CMDR_OnSoldItemToShop::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSoldItemToShop>();
	if (!real_msg)
		return false;
	res_ = real_msg->nRet;
	return true;
}

CMDR_OnCommitTaskFailed::CMDR_OnCommitTaskFailed()
{
	res_ = -1;
	task_id_ = 0;
}

bool CMDR_OnCommitTaskFailed::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnCommitTaskFailed>();
	if (!real_msg)
		return false;
	res_ = real_msg->nRet;
	task_id_ = real_msg->dwTaskId;
	return true;
}

void CMDR_OnCommitTaskFailed::OnReceived() const
{
	auto pthis = (CMDR_OnCommitTaskFailed*)this;
	auto shared_this = PointerCast<CMDR_OnCommitTaskFailed>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetCommitTaskFailed(shared_this);
}

bool CMDR_PlayerList::stPlayerInfo::ConvertToRoleInfo( CG_RoleInfo& role_info ) const
{
	using namespace GType;
	using namespace GClass;
	if (player_name_.empty())
	{
		assert(false);
		return false;
	}
	role_info.role_name_ = player_name_;
	return true;
}

CMDR_OnDoNpcChat::CMDR_OnDoNpcChat()
{
	cur_task_id_ = -1;
}

bool CMDR_OnDoNpcChat::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnDoNpcChat>();
	if (!real_msg)
		return false;
	cur_task_id_ = real_msg->task_id_;
	return true;
}

void CMDR_OnDoNpcChat::OnReceived() const
{
	auto pthis = (CMDR_OnDoNpcChat*)this;
	auto shared_this = PointerCast<CMDR_OnDoNpcChat>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnDoNpcChat(shared_this);
}

bool CMDR_Hook_SendMoveRoom::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_size = msg->GetMsgLength();
	auto real_msg = Detail::CDF_CheckMsg((stCD_Msg_MoveRoom*)msg->MsgAs<void>(), real_size);
	if (!real_msg)
		return false;
	auto& gpm = GPlayerMe::GetMe();
	//WriteLock wl(gpm.mutex_msg_on_hook_move_room_);
	DateTimeLocal time_now;
	auto msg_new = *real_msg;
	/*if ( time_span.totalMilliseconds() < kSendMoveRoomSpan && IsSendMoveRoolMsgSame(msg_, gpm.msg_on_hook_move_room_.msg_) )
	{
		LOG_O(Log_debug) << "Game已经发过过此房间封包了";
		offset_valid_ = true;
		return false;
	}*/	
	if ( _strnicmp(msg_new.szDoorCmd, gpm.msg_on_hook_move_room_.msg_.szDoorCmd, _countof(msg_.szDoorCmd)) == 0 )
	{
		//auto time_span = time_now - gpm.msg_on_hook_move_room_.msg_time_;
		//if ( /*time_span.totalMilliseconds() < kSendMoveRoomSpan ||*/ !gpm.msg_on_send_move_ret_  )
		{
			LOG_O(Log_debug) << "游戏又要主动发这个过房间命令：" << msg_new.szDoorCmd;
			offset_valid_ = true;
			//assert(false);
			GetIoService().post([](){
				assert(false);
			});
			return false;
		}
	}

	msg_ = msg_new;
	msg_time_ = time_now;

	gpm.msg_on_hook_move_room_ = *this;
	offset_valid_ = false;

	gpm.OnGameSendMoveRoom(*this);
	return true;
}

CMDR_Hook_SendMoveRoom::CMDR_Hook_SendMoveRoom()
{
	ZeroMemory(&msg_, sizeof(msg_));
	offset_valid_ = false;
}

//不让客户端发包，则返回这个函数
pc_naked void IgnoreThePacket()
{
	_asm mov al, 1
	_asm retn 4
}

const char* CMDR_Hook_SendMoveRoom::NewRetAddr() const
{
	//这里已无效，去参考Hooker_GamePassRoom
	if ( !LuasScriptEchoC::IsRunning() )
		return nullptr;
	if ( !offset_valid_ )
		return nullptr;
	return (pt_csz)(pt_pcvoid)IgnoreThePacket;
}

bool CMDR_Hook_SendMoveRoom::IsSendMoveRoolMsgSame( const stCD_Msg_MoveRoom& lhs, const stCD_Msg_MoveRoom& rhs )
{
	if ( _strnicmp(lhs.szDoorCmd, rhs.szDoorCmd, _countof(lhs.szDoorCmd) - 1) != 0 )
		return false;
	if ( !FloatEqual(lhs.theDoorCoord.fx, rhs.theDoorCoord.fx) )
		return false;
	if ( !FloatEqual(lhs.theDoorCoord.fy, rhs.theDoorCoord.fy) )
		return false;
	/*if ( !FloatEqual(lhs.theDoorCoord.fz, rhs.theDoorCoord.fz) )
		return false;*/
	return true;
}

CMDR_OnBuffStatus::CMDR_OnBuffStatus()
{
	iPoisoningFeebHpFlag = 0;
}

bool CMDR_OnBuffStatus::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnBuffStatus>();
	if (!real_msg)
		return false;
	iPoisoningFeebHpFlag = real_msg->dwPoisoningFeebHpFlag;
	return true;
}

CMDR_OnRoleHpState::CMDR_OnRoleHpState()
{
	res_ = 0;
	role_state_ = 0;
}

bool CMDR_OnRoleHpState::DoDirect( const CMDR_HeaderBase* msg )
{
	assert(false);
	return false;
	/*res_ = msg->CastTo<WORD>(MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___);
	role_state_ = msg->CastTo<WORD>(MMOCLIENT_REVERSAL_PACKET_HEADER_VALUE_OFFSET___ + sizeof(int));
	if ( !IsPoisoning() )
		return false;
	return true;*/
}

void CMDR_OnRoleHpState::OnReceived() const
{
	/*auto pthis = (CMDR_OnRoleHpState*)this;
	auto shared_this = PointerCast<CMDR_OnRoleHpState>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnRoleHpState(shared_this);
	LOG_O(Log_debug) << "中毒, res:" << res_ << " state:" << role_state_;*/
}

bool CMDR_OnRoleHpState::IsPoisoning() const
{
	if (res_ == 0x2B/*MMOCLIENT_REVERSAL_ROLE_HP_STATE_RET_PACKET_FALG___*/ && role_state_ == 0x7049/*MMOCLIENT_REVERSAL_ROLE_HP_STATE_ZHONG_DU_STATE___*/)
		return true;
	return false;
}

CMDR_OnSendMail::CMDR_OnSendMail()
{
	res_ = -1;
}

bool CMDR_OnSendMail::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSendMail>();
	if (!real_msg)
		return false;
	res_ = real_msg->res_;
	return true;
}

void CMDR_OnSendMail::OnReceived() const
{
	auto pthis = (CMDR_OnSendMail*)this;
	auto shared_this = PointerCast<CMDR_OnSendMail>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnSendMail(shared_this);
}

CMDR_OnUseItemRet1::CMDR_OnUseItemRet1()
{
	res_ = 0;
}

void CMDR_OnUseItemRet1::OnReceived() const
{
	auto pthis = (CMDR_OnUseItemRet1*)this;
	auto shared_this = PointerCast<CMDR_OnUseItemRet1>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnUseItemRet1(shared_this);
}

bool CMDR_OnUseItemRet1::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnUseItemRet1>();
	if (!real_msg)
		return false;
	res_ = real_msg->bRet;
	return true;
}

void CMDR_OnUseItemRet2::OnReceived() const
{
	auto pthis = (CMDR_OnUseItemRet2*)this;
	auto shared_this = PointerCast<CMDR_OnUseItemRet2>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnUseItemRet2(shared_this);
}

bool CMDR_OnUseItemRet2::DoDirect( const CMDR_HeaderBase* msg )
{
	return true;
}

CMDR_OnBuyItem::CMDR_OnBuyItem()
{
	res_ = -1;
}

bool CMDR_OnBuyItem::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnBuyItem>();
	if (!real_msg)
		return false;
	res_ = real_msg->nRet;
	return true;
}

void CMDR_OnBuyItem::OnReceived() const
{
	auto pthis = (CMDR_OnBuyItem*)this;
	auto shared_this = PointerCast<CMDR_OnBuyItem>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.OnBuyItem(shared_this);
}

CMDR_OnCollectRet::CMDR_OnCollectRet()
{
	res_ = -1;
	gain_item_id_ = -1;
}

bool CMDR_OnCollectRet::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnCollectRet>();
	if (!real_msg)
		return false;
	res_ = real_msg->nRet;
	if (real_msg->gained_items_.cnt_ > 0)
		gain_item_id_ = real_msg->gained_items_.begin()->dwCollectItemId;
	else
		gain_item_id_ = -1;
	return true;
}

void CMDR_OnCollectRet::OnReceived() const
{
	auto pthis = (CMDR_OnCollectRet*)this;
	auto shared_this = PointerCast<CMDR_OnCollectRet>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.OnCollectRet(shared_this);
	if ( 0 == res_ )
	{
		auto& item_mgr = ItemDbMgr::GetMe();
		auto item = item_mgr.FindById(gain_item_id_);
		if ( !item )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "CMDR_OnCollectRet之采集或剥皮得到的物品查找不到，物品ID:" << gain_item_id_;
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "采集或剥皮得到的物品，得到物品：" << item->item_name_;
		}
	}
	//LOG_O(Log_trace) << "采集或剥皮返回结果：" << res_;
}

bool CMDR_OnCollectRet::CanNotGainTheCollectItem() const
{
	return kEC_PacketIsFull == res_ || kEC_CntLimit == res_;
}

bool CMDR_OnCollectRet::IsSucceed() const
{
	return 0 == res_ && 0 != gain_item_id_;
}

bool CMDR_OnArrangeItemsRet::DoDirect( const CMDR_HeaderBase* msg )
{
	return true;
}

void CMDR_OnArrangeItemsRet::OnReceived() const
{
	auto pthis = (CMDR_OnArrangeItemsRet*)this;
	auto shared_this = PointerCast<CMDR_OnArrangeItemsRet>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.OnArrangeItemsRet(shared_this);
}

CMDR_OnSendMail2::CMDR_OnSendMail2()
{
	ZeroMemory(&res_, sizeof(res_));
}

bool CMDR_OnSendMail2::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSendMail2>();
	if (!real_msg)
		return false;

	//这个结果值总是为1，因为邮寄成功了，这个包不会到的
	/*if ( 1 != res_.nRet )
	{
		LOG_O(Log_debug) << "邮件错误返回结果值不为1，为：" << res_.nRet;
	}*/
	res_ = *real_msg;
	return true;
}

void CMDR_OnSendMail2::OnReceived() const
{
	auto pthis = (CMDR_OnSendMail2*)this;
	auto shared_this = PointerCast<CMDR_OnSendMail2>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.SetOnSendMail2(shared_this);
}

bool CMDR_OnSendMail2::IsInSafeTime() const
{
	return 0x00046CE4 == res_.dwErrorValue;
}

bool CMDR_OnSendMail2::NoExistRole() const
{
	return 0x00046CD4 == res_.dwErrorValue;
}

bool CMDR_OnSendMail2::MailIsQuick() const
{
	return 0x00046CE2 == res_.dwErrorValue;
}

bool CMDR_OnSendMail2::MailItemWrong() const
{
	return 290023 == res_.dwErrorValue;
}

bool CMDR_OnSendMail2::MailLimit() const
{
	return 290019 == res_.dwErrorValue;
}

bool CMDR_OnSendMoveRoomRet::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnSendMoveRoomRet>();
	if (!real_msg)
		return false;
	msg_ = *real_msg;
	if ( msg_.dwRolePacketId != GPlayerMe::GetMe().GetPacketId() )
		return false;
	return true;
}

void CMDR_OnSendMoveRoomRet::OnReceived() const
{
	auto pthis = (CMDR_OnSendMoveRoomRet*)this;
	auto shared_this = PointerCast<CMDR_OnSendMoveRoomRet>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_send_move_ret_ = shared_this;
	//LOG_O(Log_trace) << "CMDR_OnSendMoveRoomRet";
}

CMDR_OnAcceptTaskRet::CMDR_OnAcceptTaskRet()
{
	task_id_ = 0;
}

bool CMDR_OnAcceptTaskRet::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnAcceptTaskRet>();
	if (!real_msg)
		return false;
	task_id_ = (int)real_msg->dwTaskId;
	return true;
}

void CMDR_OnAcceptTaskRet::OnReceived() const
{
	auto pthis = (CMDR_OnAcceptTaskRet*)this;
	auto shared_this = PointerCast<CMDR_OnAcceptTaskRet>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_accept_task_ = shared_this;
}

CMDR_OnGiveupTaskRet::CMDR_OnGiveupTaskRet()
{
	task_id_ = 0;
}

bool CMDR_OnGiveupTaskRet::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnGiveupTaskRet>();
	if (!real_msg)
		return false;
	task_id_ = (int)real_msg->dwTaskId;
	return true;
}

void CMDR_OnGiveupTaskRet::OnReceived() const
{
	auto pthis = (CMDR_OnGiveupTaskRet*)this;
	auto shared_this = PointerCast<CMDR_OnGiveupTaskRet>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_giveup_task_ = shared_this;
}

CMDR_OnEnterFbError::CMDR_OnEnterFbError()
{
	error_res_ = 0;
}

bool CMDR_OnEnterFbError::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnEnterFbError>();
	if (!real_msg)
		return false;
	error_res_ = real_msg->dwRetValue;
	return true;
}

void CMDR_OnEnterFbError::OnReceived() const
{
	auto pthis = (CMDR_OnEnterFbError*)this;
	auto shared_this = PointerCast<CMDR_OnEnterFbError>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_enter_fb_err_ = shared_this;
}

CMDR_RoleDead::CMDR_RoleDead()
{
	packet_id_ = 0;
}

bool CMDR_RoleDead::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_RoleDead>();
	if (!real_msg)
		return false;
	packet_id_ = real_msg->packet_id_;
	return true;
}

void CMDR_RoleDead::OnReceived() const
{
	auto& gpm = GPlayerMe::GetMe();
	if ( packet_id_ == gpm.GetPacketId() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "收到包，角色死亡了";
		//角色死亡了
		if ( !gpm.is_hitting_monster_ )
			return;
		throw CmnPauseLogicException(kPauseId_RoleDead, "角色死亡了");
	}
}

CMDR_OnDeleteRole::CMDR_OnDeleteRole()
{
	res_ = enErrorCode(-1);
	protect_time_ = -1;
}

bool CMDR_OnDeleteRole::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnDeleteRole>();
	if (!real_msg)
		return false;
	res_ = enErrorCode(real_msg->nRet);
	protect_time_ = real_msg->nRetParamer;
	return true;
}

void CMDR_OnDeleteRole::OnReceived() const
{
	LOG_O(Log_debug) << "删除角色结果：" << res_ << " 保护时间：" << protect_time_;
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
		return;
	auto pthis = (CMDR_OnDeleteRole*)this;
	auto shared_this = PointerCast<CMDR_OnDeleteRole>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	nor->on_delete_role_ = shared_this;
}

bool CMDR_OnRequestDeal::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRequestDeal>();
	if (!real_msg)
		return false;
	deal_info_ = *real_msg;
	return true;
}

CMDR_OnRequestDeal::CMDR_OnRequestDeal()
{
	ZeroMemory(&deal_info_, sizeof(deal_info_));
}

void CMDR_OnRequestDeal::OnReceived() const
{
	auto pthis = (CMDR_OnRequestDeal*)this;
	auto shared_this = PointerCast<CMDR_OnRequestDeal>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_request_deal_ = shared_this;
}

bool CMDR_OnRequestDealFail::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRequestDealFail>();
	if (!real_msg)
		return false;
	if (real_msg->sub_type_ != stCD_MsgR_OnRequestDealFail::kOnDealFail)
		return false;
	res_ = enErrorCode(real_msg->dwRequestDealFailValue);
	the_role_name_.assign(real_msg->szRoleName);
	return true;
}

CMDR_OnRequestDealFail::CMDR_OnRequestDealFail()
{
	res_ = enErrorCode(-1);
}

void CMDR_OnRequestDealFail::OnReceived() const
{
	auto pthis = (CMDR_OnRequestDealFail*)this;
	auto shared_this = PointerCast<CMDR_OnRequestDealFail>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_request_deal_fail_ = shared_this;
}

bool CMDR_OnRecvQuestDeal::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRecvQuestDeal>();
	if (!real_msg)
		return false;
	deal_key_ = real_msg->deal_key_;
	recv_time_ = DateTimeLocal();
	return true;
}

CMDR_OnRecvQuestDeal::CMDR_OnRecvQuestDeal()
{
	deal_key_ = 0;
}

void CMDR_OnRecvQuestDeal::OnReceived() const
{
	auto pthis = (CMDR_OnRecvQuestDeal*)this;
	auto shared_this = PointerCast<CMDR_OnRecvQuestDeal>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_recv_quest_deal_ = shared_this;
}

CMDR_OnCancelDeal::CMDR_OnCancelDeal()
{
	role_id_of_cancel_ = 0;
}

bool CMDR_OnCancelDeal::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnCancelDeal>();
	if (!real_msg)
		return false;

	role_id_of_cancel_ = real_msg->role_id_of_cancel_;
	return true;
}

void CMDR_OnCancelDeal::OnReceived() const
{
	auto pthis = (CMDR_OnCancelDeal*)this;
	auto shared_this = PointerCast<CMDR_OnCancelDeal>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_cancel_deal_ = shared_this;

	if (gpm.is_dealing_handle_)
	{
		throw CmnPauseLogicException(kPauseId_CancelDeal, "取消了交易");
	}
}

bool CMDR_OnDealFail::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnDealFail>();
	if (!real_msg)
		return false;

	res_ = (enErrorCode)real_msg->dwDealFailRet;
	dst_role_id_ = real_msg->deal_role_id_;
	return true;
}

CMDR_OnDealFail::CMDR_OnDealFail()
{
	res_ = enErrorCode(-1);
	dst_role_id_ = 0;
}

void CMDR_OnDealFail::OnReceived() const
{
	auto pthis = (CMDR_OnDealFail*)this;
	auto shared_this = PointerCast<CMDR_OnDealFail>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_deal_fail_ = shared_this;
}

bool CMDR_OnMoveItemToDealTab::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnMoveItemToDealTab>();
	if (!real_msg)
		return false;
	the_move_item_ = *real_msg;
	return true;
}

CMDR_OnMoveItemToDealTab::CMDR_OnMoveItemToDealTab()
{
	ZeroMemory(&the_move_item_, sizeof(the_move_item_));
}

void CMDR_OnMoveItemToDealTab::OnReceived() const
{
	auto pthis = (CMDR_OnMoveItemToDealTab*)this;
	auto shared_this = PointerCast<CMDR_OnMoveItemToDealTab>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_move_item_deal_tab_ = shared_this;
}

CMDR_OnMoveDealGold::CMDR_OnMoveDealGold()
{
	ZeroMemory(&deal_info_, sizeof(deal_info_));
}

bool CMDR_OnMoveDealGold::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnMoveDealGold>();
	if (!real_msg)
		return false;
	deal_info_ = *real_msg;
	return true;
}

void CMDR_OnMoveDealGold::OnReceived() const
{
	auto pthis = (CMDR_OnMoveDealGold*)this;
	auto shared_this = PointerCast<CMDR_OnMoveDealGold>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_move_deal_gold_ = shared_this;
}

bool CMDR_OnRecvDealStatus::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRecvDealStatus>();
	if (!real_msg)
		return false;
	the_deal_role_id_ = real_msg->dwDoDealRoleId;
	deal_oper_ = (enOperator)real_msg->bDoDealIndex;
	return true;
}

CMDR_OnRecvDealStatus::CMDR_OnRecvDealStatus()
{
	the_deal_role_id_ = 0;
	deal_oper_ = enOperator(-1);
}

void CMDR_OnRecvDealStatus::OnReceived() const
{
	auto pthis = (CMDR_OnRecvDealStatus*)this;
	auto shared_this = PointerCast<CMDR_OnRecvDealStatus>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_recv_deal_status_ = shared_this;
	auto self_packet_id = gpm.GetPacketId();
	if ( self_packet_id == the_deal_role_id_ )
		gpm.msg_on_deal_status_self_ = shared_this;
	else
		gpm.msg_on_deal_status_other_ = shared_this;
}

CMDR_OnRecvDealEnd::CMDR_OnRecvDealEnd()
{
	res_ = enErrorCode(-1);
}

bool CMDR_OnRecvDealEnd::DoDirect( const CMDR_HeaderBase* msg )
{
	auto real_msg = msg->MsgAs<stCD_MsgR_OnRecvDealEnd>();
	if (!real_msg)
		return false;

	res_ = (enErrorCode)real_msg->res_;
	return true;
}

void CMDR_OnRecvDealEnd::OnReceived() const
{
	auto pthis = (CMDR_OnRecvDealEnd*)this;
	auto shared_this = PointerCast<CMDR_OnRecvDealEnd>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_recv_deal_end_ = shared_this;
}

bool CMDR_Hook_CreateRole::DoDirect( const CMDR_HeaderBase* msg )
{
	return true;
}

void CMDR_Hook_CreateRole::OnReceived() const
{
	auto pthis = (CMDR_Hook_CreateRole*)this;
	auto shared_this = PointerCast<CMDR_Hook_CreateRole>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	norc->on_hook_create_role_ = shared_this;
}

bool CMDR_OnGetMailItemsInfo::DoDirect( const CMDR_HeaderBase* msg )
{
	return true;
}

void CMDR_OnGetMailItemsInfo::OnReceived() const
{
	auto pthis = (CMDR_OnGetMailItemsInfo*)this;
	auto shared_this = PointerCast<CMDR_OnGetMailItemsInfo>(pthis->shared_from_this());
	if ( !shared_this )
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.msg_on_get_mail_items_info_ = shared_this;
}

const char* CMDR_Hook_CancelDeal::NewRetAddr() const
{
	if (!LuasScriptEchoC::IsRunning())
		return nullptr;
	if (!is_ignore_)
		return nullptr;
	return (pt_csz)(pt_pcvoid)IgnoreThePacket;
}

bool CMDR_Hook_CancelDeal::DoDirect(const CMDR_HeaderBase* msg)
{
	is_ignore_ = false;
	auto real_size = msg->GetMsgLength();
	auto real_msg = Detail::CDF_CheckMsg((stCD_Msg_AcceptDeal*)msg->MsgAs<void>(), real_size);
	if (!real_msg)
		return false;
	if ((pt_byte)real_msg->no_accept_ == 1)
		is_ignore_ = true;
	return false;
}
