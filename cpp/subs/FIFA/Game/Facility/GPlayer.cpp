#include "stdafx.h"
#include "GPlayer.h"
#include "Common/LogOut.h"
#include "GClass.h"
#include "FuncObjC.h"
#include "MatchInterface.h"
#include "CfgStructC.h"
#include "MatchInterface.h"
#include <Communicate/CGL_Defines.h>

GPlayer& GPlayer::GetMe()
{
	static GPlayer player;
	return player;
}

GPlayer::GPlayer()
{
	ep_ = 0;
	is_timeout_at_match_room_ = false;
	cc_state_ = GType::enCC_SubState_Invalid;
	create_coach_succeed_ = false;
	ability_filter_ = 70;	//默认是70
	gp_ = 0;
	request_gp_data_ = false;
	exchange_price_filter_ = 500000;
	high_price_filter_ = 1000000;
	against_offer_price_ = 500000;
	error_sick_enemy_cnt_ = 0;
	hour_wait_sale_player_ = 4 * 1;
	max_players_cnt_ = 100;
	match_cnt_limit_ = 100;
	select_enemy_cnt_ = 0;
}

void GPlayer::SetEp( double ep )
{
	LOG_ENTER_FUNC;
	if ( ep_ > ep )
	{
		LOG_O(Log_warning) << "ep竟然少于之前的";
	}
	LOG_O(Log_trace) << "设置EP，之前：" << ep_ << "\t之后" << ep;
	ep_ = ep;

	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP(g_call_fn_name_set_ep)<<ep_);
	}
}

double GPlayer::GetEp() const
{
	return ep_;
}

bool GPlayer::SetBagItems( const Json::Value& bag_items_array )
{
	LOG_ENTER_FUNC;

	if ( bag_items_array.isNull() || !bag_items_array.isArray() )
	{
		LOG_O(Log_error) << "bag_items_array竟然不是数组";
		return false;
	}
	LOG_O(Log_trace) << "设置背包物品";
	
	stItem item_tmp;
	ItemContT items_tmp;
	for ( const auto& item : bag_items_array )
	{
		if ( !item_tmp.ParseFromJsonObj(item) )
		{
			LOG_O(Log_error) << "stItem::ParseFromJsonObj出错";
			return false;
		}

		items_tmp.push_back(item_tmp);
	}

	items_.swap(items_tmp);
	return true;
}

const GPlayer::stLevelInfo& GPlayer::GetLevelInfo() const
{
	return level_info_;
}

void GPlayer::SetLevelInfo( const stLevelInfo& level_info )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置等级信息，之前{ level:" << level_info_.GetLevel() << "\texp:" << level_info_.GetExp()
		<< "\tto:" << level_info_.GetTo() << "}之后{ level:" << level_info.GetLevel() << "\texp:" << level_info.GetExp()
		<< "\tto:" << level_info.GetTo() << "}";
	level_info_ = level_info;


	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP("SetTrainerLevel")<<level_info_.GetLevel());
	}
}

const char* GPlayer::GetName() const
{
	return name_.c_str();
}

void GPlayer::SetName( const char* name )
{
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
		return;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置名字，之前：" << name_ << "之后：" << name;
	/*if ( name_ != name )
	{
		SetIoTimer(5 * 1000, []( IoTimer* ){
			auto& gp = GPlayer::GetMe();
			gp.RefreshSelfOfferList();
			gp.SendSelfSoldInfo();
		});
	}*/
	name_ = name;

	normal_c->GetSessionOper().ToCall(BC_MakeP(g_call_fn_name_set_trainer_name)<<name);
}

GPlayer::stPlayer* GPlayer::FindPlayerById( const char* id ) const
{
	if ( !id )
		return nullptr;
	for ( const auto& it : players_ )
	{
		if ( it.id_ == id )
			return const_cast<stPlayer*>(&it);
	}
	return nullptr;
}

bool GPlayer::Init_SetAllPlayers( const Json::Value& players_array )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "初始化之设置所有的球员";
	if (players_array.empty())
	{
		LOG_O(Log_error) << "初始化之设置所有球员之players_array竟然是空的";
		return false;
	}

	if ( !players_array.isArray() && !players_array.isObject() )
	{
		LOG_O(Log_error) << "初始化之设置所有球员之players_array竟然不是数组和json obj";
		return false;
	}
	
	PlayerContT players_tmp;

	for ( const auto& json_player : players_array )
	{
		if ( !AddPlayerFromJsonObj(json_player, players_tmp) )
			goto LABEL_RET_ERROR;
	}

	players_tmp.swap(players_);
	return true;
LABEL_RET_ERROR:
	LOG_O(Log_error) << "设置所有的球员失败";
	return false;
}

GPlayer::SharedPlayerIter
	GPlayer::CreateExchangePlayerIter() const
{
	return SharedPlayerIter(new stPlayersNormalIter([]( const stPlayer& player ){
		return player.CanExchangePlayer();
	}));
}

bool GPlayer::IsCanExchangePlayer() const
{
	auto player_cnt = 0;
	for ( const auto& player : players_ )
	{
		if (player.CanExchangePlayer())
		{
			++player_cnt;
			if ( player_cnt >= 2 )
				return true;
		}
	}
	return false;
}

void GPlayer::ErasePlayerById( const char* id )
{
	auto it_end = players_.end();
	for ( auto it = players_.begin(); it != it_end; ++it )
	{
		if ( it->id_ == id )
		{
			players_.erase(it);
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "删除成功，球员ID：" << id;
			return;
		}
	}
}

void GPlayer::ErasePlayerById( const char* id_1, const char* id_2 )
{
	auto it_end = players_.end();
	int erase_cnt = 0;
	for ( auto it = players_.begin(); it != it_end; )
	{
		if ( it->id_ == id_1 || it->id_ == id_2 )
		{
			players_.erase(it++);
			if ( ++erase_cnt == 2 )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_trace) << "删除成功，球员ID1：" << id_1 << "  球员ID1：" << id_2;
				return;
			}
			continue;
		}
		it++;
	}
}

bool GPlayer::IsNeedAutoSelectPlayer() const
{
	if (players_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "球员还为空呢，不能判断出到底需不需要自动选拔球员";
		return false;
	}

	GType::enPlayerPosition postions[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	auto player_it = CreatePlayerIterByPosition(postions, _countof(postions));
	if ( !player_it )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "创建有职位的球员的迭代器失败";
		return false;
	}

	for ( player_it->First(); !player_it->IsDone(); player_it->Next() )
	{
		if ( player_it->CurItem()->life_ == 0 )
		{
			return true;
		}
	}

	//候补中不能全为0
	GType::enPlayerPosition third_postions[] = {
		GType::enPlayerPosition_Third
	};
	auto player_third = CreatePlayerIterByPosition(third_postions, _countof(third_postions));
	if ( !player_third )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "player_third 创建有职位的球员的迭代器失败";
		return false;
	}
	auto third_total_cnt = 0, third_rubbish_cnt = 0;
	for ( player_third->First(); !player_third->IsDone(); player_third->Next() )
	{
		if ( 0 == player_third->CurItem()->GetLife() )
			++third_rubbish_cnt;
		++third_total_cnt;
	}
	//候补全为0
	if ( third_rubbish_cnt == third_total_cnt )
		return true;
	return false;
}

bool GPlayer::SetFirstSecondPlayers( const Json::Value& players_array )
{
	if ( players_array.isNull() || !players_array.isArray() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "players_array类型不是数组";
		return false;
	}
	const auto kSize = players_array.size();
	if ( kSize == 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "比赛结束后发送的奖励的球员数量为0，可能是放弃比赛的原因吧";
		return false;
	}
	if ( kSize != 18 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "首发球员和替补球员的人数和不等于18？,球员数是：" << kSize;
	}

	//ResetFirstTeamPlayersToThird();

	for ( const auto& json_value : players_array )
	{
		const auto& json_player_id = json_value["id"];
		if ( json_player_id.isNull() || !json_player_id.isString() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "json_player的ID竟然不为字符串类型";
			return false;
		}
		auto player = FindPlayerById(json_player_id.asCString());
		if ( !player )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "在所有的队员中竟然没有找到指定的球员，ID:" << json_player_id.asCString();
			continue;
		}
		if ( !player->position_info_.ParseFromJsonObj(json_value) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "职位信息解析失败";
			continue;
		}
		if ( player->position_info_.pos_ != stPlayer::PositionInfo::kSecondPos )
			player->player_pos_ = GType::enPlayerPosition_First;
		else
			player->player_pos_ = GType::enPlayerPosition_Second;
	}
	assert(11 == GetThePosPlayersCnt(GType::enPlayerPosition_First));
	assert(7 == GetThePosPlayersCnt(GType::enPlayerPosition_Second));
	return true;
}

GPlayer::SharedPlayerIter GPlayer::CreatePlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const
{
	if ( !player_position || position_cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "CreatePlayerIterByPosition之创建迭代器失败";
		return SharedPlayerIter();
	}

	std::vector<GType::enPlayerPosition> positions;
	for ( auto i = 0; i != position_cnt; ++i )
	{
		positions.push_back(player_position[i]);
	}
	
	return SharedPlayerIter(new stPlayersNormalIter([positions]( const stPlayer& player ){
		auto player_position = player.GetPosition();
		for ( const auto pos : positions )
		{
			if ( pos == player_position )
				return true;
		}
		return false;
	}));
}

bool GPlayer::MatchEnd_UpdatePlayers( const Json::Value& players_obj )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "比赛结束之设置所有的球员";
	if (players_obj.empty())
	{
		LOG_O(Log_error) << "比赛结束之设置球员之players_obj竟然是空的";
		return false;
	}

	if ( players_obj.isNull() || !players_obj.isObject() )
	{
		LOG_O(Log_error) << "比赛结束之设置球员之players_obj竟然不是数组";
		return false;
	}

	stPlayer player_tmp;
	for ( const auto& json_player : players_obj )
	{
		if ( !player_tmp.ParseFromJsonObj(json_player) )
		{
			LOG_O(Log_error) << "更新球员数据失败";
			return false;
		}
		auto player_exist = FindPlayerById(player_tmp.id_.c_str());
		if (player_exist)
		{
			player_exist->MatchEndUpdate(player_tmp);
		}
		else
		{
			players_.push_back(player_tmp);
		}
	}

	ResetFirstTeamPlayersToThird();
	return true;
	//return Init_SetAllPlayers(players_obj);
}

bool GPlayer::AddPlayerFromJsonObj( const Json::Value& player_obj, PlayerContT& out_players_cont )
{
	stPlayer player;
	if ( !player.ParseFromJsonObj(player_obj) )
	{
		LOG_O(Log_error) << "增加单个球员失败";
		return false;
	}
	out_players_cont.push_back(player);
	return true;	
}

GPlayer::stItem* GPlayer::FindItemById( const char* id )
{
	for ( auto& item : items_ )
	{
		if ( item.id_ == id )
			return &item;
	}

	return nullptr;
}

GPlayer::stItem* GPlayer::FindItemByItemId( int item_id )
{
	for ( auto& item : items_ )
	{
		if ( item.itemid_ == item_id )
			return &item;
	}

	return nullptr;
}

bool GPlayer::ContractFirstAndSecondPlayers()
{
	GType::enPlayerPosition positions[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	return ContractPlayersByPosition(positions, _countof(positions));
}

bool GPlayer::ContractPlayersByPosition( GType::enPlayerPosition player_position[], int position_cnt )
{
	auto player_iter = CreatePlayerIterByPosition(player_position, position_cnt);
	if ( !player_iter )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样职位的球员，职位是：" << player_position;
		return false;
	}

	std::vector<stPlayer*> players_tmp;
	for ( player_iter->First(); !player_iter->IsDone(); player_iter->Next() )
	{
		auto player = player_iter->CurItem();
		if ( player->life_ == 0 )
		{
			players_tmp.push_back(player);
		}
	}

	LOG_ENTER_FUNC;
	if (players_tmp.empty())
	{
		LOG_O(Log_warning) << "该职位的球员并不需要续约，职位是：" << player_position;
		return false;
	}
	Json::Value msg;
	msg["_method"] = "PUT";
	auto& json_array = msg["players"];

	for ( const auto player : players_tmp )
	{
		json_array.append(Json::Value(player->id_));
	}
	assert(players_tmp.size() == json_array.size());
	LOG_O(Log_trace) << "续约球员，个数：" << players_tmp.size();
	Json::Value respond;
	if ( !GClass::HttpPackageSend::Post_EncryptJsonMsgSync("/players/recontract/preview", msg, respond) )
		return false;
	if ( !GClass::HttpPackageSend::Post_EncryptJsonMsgSync("/players/recontract", msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;

	for ( const auto& player : players_tmp )
	{
		assert(player);
		//默认续约到满40场
		player->SetGrade(40);
	}
	LOG_O(Log_trace) << "续约成功";
	return true;
}

void GPlayer::SendAutoMatch()
{
	GClass::HttpPackageSend::Post_JsonMsgAsync("/calendar/today/sim", nullptr);
}

bool GPlayer::HasAutoMatchTicket()
{
	auto item = FindItemByItemId(stItem::ItemId_AutoMatchTicket());
	if ( !item )
		return false;
	return item->GetRemains() > 0;
}

int GPlayer::GetFirstTeamCnt() const
{
	int cnt = 0;
	for ( const auto& player : players_ )
	{
		if (player.IsFirstTeamPlayer())
		{
			++cnt;
		}
	}
	return cnt;
}

int GPlayer::GetSecondTeamCnt() const
{
	int cnt = 0;
	for ( const auto& player : players_ )
	{
		if (player.IsSecondTeamPlayer())
		{
			++cnt;
		}
	}
	return cnt;
}

bool GPlayer::SendFirstAndSecondPlayersInfo() const
{
	Json::Value msg;
	if ( !CombineSquadInfo(msg) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "组合球队信息失败";
		return false;
	}
	return GType::enHttpSendCall_Result_Fail != GClass::HttpPackageSend::SendEncryptedJsonMsgAsync("/coach/me/squad", msg);
}

GPlayer::SharedPlayerIter GPlayer::CreateFirstTeamIter() const
{
	return SharedPlayerIter(new stPlayersNormalIter([]( const stPlayer& player ){
		return player.IsFirstTeamPlayer();
	}));
}

GPlayer::PlayerContT& GPlayer::GetAllPlayers()
{
	return players_;
}

void GPlayer::ResetFirstTeamPlayersToThird()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置所有的一线球员为侯补";

	auto first_team_iter = CreateFirstTeamIter();
	if (first_team_iter)
	{
		stPlayer::PositionInfo empty_pos_info;
		for ( first_team_iter->First(); !first_team_iter->IsDone(); first_team_iter->Next() )
		{
			auto player = first_team_iter->CurItem();
			assert(player);
			if (player)
			{
				player->player_pos_ = GType::enPlayerPosition_Third;
				player->position_info_ = empty_pos_info;
			}
		}
	}
}

bool GPlayer::AutoSelectPlayerBySend()
{
	LOG_ENTER_FUNC;
	if ( !IsNeedAutoSelectPlayer() )
	{
		LOG_O(Log_error) << "自动选拔球员的条件不满足";
		return false;
	}

	auto degrade_first_and_second = DegradeFirstAndSecondSync();
	if ( !degrade_first_and_second.first )
	{
		LOG_O(Log_error) << "降级首发和替补球员失败";
		return false;
	}

	if (degrade_first_and_second.second)
	{
		DegradeAllThirdRubbishPlayers();
		DegradeAllThirdSamePlayers();
		FillFullThirdPlayers();
		return true;
	}

	if ( !DegradeAllThirdRubbishPlayers() )
	{
		LOG_O(Log_error) << "第一次DegradeAllThirdRubbishPlayers失败";
		return false;
	}

	DegradeAllThirdSamePlayers();

	if ( !FillFullThirdPlayers() )
	{
		LOG_O(Log_error) << "第一次FillFullThirdPlayers失败";
		return false;
	}

	degrade_first_and_second = DegradeFirstAndSecondSync();
	if ( !degrade_first_and_second.first )
	{
		LOG_O(Log_error) << "降级首发和替补球员失败";
		return false;
	}

	if (degrade_first_and_second.second)
	{
		DegradeAllThirdRubbishPlayers();
		DegradeAllThirdSamePlayers();
		FillFullThirdPlayers();
		return true;
	}

	if ( !DegradeAllThirdRubbishPlayers() )
	{
		LOG_O(Log_error) << "第二次DegradeAllThirdRubbishPlayers失败";
		return false;
	}
	//降级同名球员
	DegradeAllThirdSamePlayers();

	if ( !FillFullThirdPlayers() )
	{
		LOG_O(Log_error) << "第二次FillFullThirdPlayers失败";
		return false;
	}

	auto degrade_first_and_second_again = DegradeFirstAndSecondSync();
	DegradeAllThirdRubbishPlayers();
	FillFullThirdPlayers();
	return degrade_first_and_second_again.first && degrade_first_and_second_again.second;
}

std::pair<bool, bool> GPlayer::DegradeFirstAndSecond()
{
	std::pair<bool, bool> res(false, false);
	assert(false);
	return res;
	GType::enPlayerPosition positions_upper[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	//职位高的
	auto player_upper_iter = CreateRubbishPlayerIterByPosition(positions_upper, _countof(positions_upper));
	if ( !player_upper_iter )
		return res;

	GType::enPlayerPosition positions_lower[] = {
		GType::enPlayerPosition_Third
	};
	//职位低的
	auto player_lower_iter = CreateNoRubbishPlayerIterByPosition(positions_lower, _countof(positions_lower));
	if ( !player_lower_iter )
		return res;

	player_upper_iter->First();
	player_lower_iter->First();
	if ( !player_upper_iter->IsDone() && !player_lower_iter->IsDone() )
	{
		auto player_upper = player_upper_iter->CurItem(), player_lower = player_lower_iter->CurItem();
		assert(player_upper->CanExchangePosition(*player_lower));
		if (player_upper->ExchangePostion(*player_lower, true))
		{
			player_upper_iter->Next();
			player_lower_iter->Next();
		}
		else
		{
			DummySleep(500);
		}
	}

	auto players_old = players_;
	bool is_need_send = false;
	for ( ;
		!player_upper_iter->IsDone() && !player_lower_iter->IsDone(); 
		player_upper_iter->Next(), player_lower_iter->Next() )
	{
		auto player_upper = player_upper_iter->CurItem(), player_lower = player_lower_iter->CurItem();
		assert(player_upper);
		assert(player_lower);
		if ( player_upper && player_lower )
		{
			assert(player_upper->CanExchangePosition(*player_lower));
			VERIFY(player_upper->ExchangePostion(*player_lower, false));
			is_need_send = true;
		}
	}

	if ( is_need_send && !SendFirstAndSecondPlayersInfo() )
	{
		players_ = players_old;
		res.first = false;
		res.second = false;
	}
	else
	{
		res.first = true;
		res.second = player_upper_iter->IsDone();
	}
	return res;
}

GPlayer::SharedPlayerIter GPlayer::CreateRubbishPlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const
{
	if ( !player_position || position_cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "CreateRubbishPlayerIterByPosition之创建迭代器失败";
		return SharedPlayerIter();
	}

	std::vector<GType::enPlayerPosition> positions;
	for ( auto i = 0; i != position_cnt; ++i )
	{
		positions.push_back(player_position[i]);
	}

	return SharedPlayerIter(new stPlayersNormalIter([positions]( const stPlayer& player ){
		if ( player.GetLife() > 0 )
			return false;
		auto player_position = player.GetPosition();
		for ( const auto pos : positions )
		{
			if ( pos == player_position )
				return true;
		}
		return false;
	}));
}

GPlayer::SharedPlayerIter GPlayer::CreateNoRubbishPlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const
{
	if ( !player_position || position_cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "CreateRubbishPlayerIterByPosition之创建迭代器失败";
		return SharedPlayerIter();
	}

	std::vector<GType::enPlayerPosition> positions;
	for ( auto i = 0; i != position_cnt; ++i )
	{
		positions.push_back(player_position[i]);
	}

	return SharedPlayerIter(new stPlayersNormalIter([positions]( const stPlayer& player ){
		if ( player.GetLife() < 1 )
			return false;
		auto player_position = player.GetPosition();
		for ( const auto pos : positions )
		{
			if ( pos == player_position )
				return true;
		}
		return false;
	}));
}

bool GPlayer::DegradeAllThirdRubbishPlayers()
{
	GType::enPlayerPosition positions_upper[] = {
		GType::enPlayerPosition_Third
	};
	//职位高的
	auto player_iter = CreateRubbishPlayerIterByPosition(positions_upper, _countof(positions_upper));
	LOG_ENTER_FUNC;
	if ( !player_iter )
	{
		LOG_O(Log_error) << "降级侯补球员中，创建迭代器失败";
		return false;
	}

	for ( player_iter->First(); !player_iter->IsDone(); player_iter->Next() )
	{
		auto player = player_iter->CurItem();
		assert(player);
		if (player)
		{
			//降级
			assert(player->CanDegradeFromThirdToNo());
			if ( !player->DegradeFromThirdToNo() )
				return false;
		}
	}
	return true;
}

bool GPlayer::FillFullThirdPlayers()
{
	const auto kThridPlayerLeft = 18 - GetThirdPlayerCnt();
	if ( kThridPlayerLeft > 0 )
	{
		static GType::enPlayerPosition positions_lower[] = {
			GType::enPlayerPosition_No
		};
		//职位低的
		auto player_iter = CreateNoRubbishPlayerIterByPosition(positions_lower, _countof(positions_lower));
		LOG_ENTER_FUNC;
		if ( !player_iter )
		{
			LOG_O(Log_error) << "填满侯补球员中，创建迭代器失败";
			return false;
		}

		static GType::enPlayerPosition pos_first_team[] = {
			GType::enPlayerPosition_First, GType::enPlayerPosition_Second, GType::enPlayerPosition_Third
		};
		player_iter->First();
		for ( auto i = 0; i != kThridPlayerLeft && !player_iter->IsDone(); player_iter->Next() )
		{
			auto player = player_iter->CurItem();
			assert(player);
			if ( !player )
				continue;
			//如果有相同的球员则忽略之
			if (HasSamePlayerAtPos(pos_first_team, _countof(pos_first_team), *player, players_))
				continue;
			//升级
			assert(player->CanUpgradeFromNoToThird());
			if ( !player->UpgradeFromNoToThird() )
				return false;
			++i;
		}
	}
	else if ( kThridPlayerLeft != 0 )
	{
		assert(false);
		(void*)0;
		return false;
	}
	return true;
}

int GPlayer::GetThirdPlayerCnt() const
{
	int cnt = 0;
	for ( const auto& player : players_ )
	{
		if ( GType::enPlayerPosition_Third == player.GetPosition() )
		{
			++cnt;
		}
	}
	return cnt;
}

int GPlayer::GetAllPlayersCnt() const
{
	return players_.size();
}

bool GPlayer::IsNeedFirePlayer() const
{
	for ( const auto& player : players_ )
	{
		if (player.CanBeFired())
		{
			return true;
		}
	}
	return false;
}

bool GPlayer::AutoFirePlayers()
{
	if ( !IsNeedFirePlayer() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不能解雇球员";
		return false;
	}

	auto removed_it = std::remove_if(players_.begin(), players_.end(), []( const stPlayer& player ){
		if (player.CanBeFired())
		{
			return player.SendFireMsg();
		}
		return false;
	});
	if ( removed_it != players_.end() )
	{
		players_.erase(removed_it, players_.end());
		return true;
	}
	return false;
}

void GPlayer::AutoSelectSickEnemy()
{
	++select_enemy_cnt_;
	auto& match_mgr = IGMatchMgr::GetMe();
	const auto& match_info = match_mgr.MatchMode()->GetMatchInfo();
	const auto kMatchRoomId = match_info.GetRoomId();
	if ( 0 == kMatchRoomId )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "比赛房间ID为0，不能选择弱队";
		return;
	}

	CStringA str_uri;
	str_uri.Format("/room/%u/AI", kMatchRoomId);

	Json::Value msg, respond;
	auto& msg_index = msg["index"];
	msg_index["team"] = 12;
	msg_index["league"] = 0;
	msg_index["nation"] = 23;
	msg_index["difficulty"] = 4;

	msg["_method"] = "PUT";
	/*if ( error_sick_enemy_cnt_ >= 5 )
	{
		auto& http_send = GClass::HttpPackageSend::GetMe();
		http_send.ResetSendCount();
		error_sick_enemy_cnt_ = 0;
	}*/
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(str_uri, &msg, respond) )
	{
		/*++error_sick_enemy_cnt_;
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "随机电脑与随机对手循环点击失败次数->" << error_sick_enemy_cnt_;*/
		return;
	}
}

bool GPlayer::IsSelectedSickEnemy()
{
	if ( select_enemy_cnt_ >= 3 )
	{
		select_enemy_cnt_ = 0;
		return true;
	}
	if ( enemy_info_.isNull() || !enemy_info_.isObject() )
		return false;
	const auto& team = enemy_info_["team"];
	if ( team.isNull() || !team.isInt() )
		return false;
	if ( 12 != team.asInt() )
		return false;
	const auto& nation = enemy_info_["nation"];
	if ( nation.isNull() || !nation.isInt() )
		return false;
	if ( 23 != nation.asInt() )
		return false;
	select_enemy_cnt_ = 0;
	return true;
}

void GPlayer::SetEnemyInfo( const Json::Value& json_enemy_info )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置对手的信息";
	if ( json_enemy_info.isNull() || !json_enemy_info.isObject() )
	{
		LOG_O(Log_error) << "json_enemy_info为空";
		return;
	}
	const auto& team = json_enemy_info["team"];
	if ( team.isNull() || !team.isInt() )
	{
		LOG_O(Log_error) << "不存在所属team信息";
		return;
	}
	const auto& nation = json_enemy_info["nation"];
	if ( nation.isNull() || !nation.isInt() )
	{
		LOG_O(Log_error) << "不存在所属nation信息";
		return;
	}
	enemy_info_["team"] = team.asInt();
	enemy_info_["nation"] = nation.asInt();
}

int GPlayer::AutoMatchTicketCnt()
{
	auto item = FindItemByItemId(stItem::ItemId_AutoMatchTicket());
	if ( !item )
		return 0;
	return item->GetRemains();
}

void GPlayer::UseSomeAutoMatchTicket( int cnt )
{
	cnt = std::min(cnt, AutoMatchTicketCnt());
	cnt = std::max(cnt, 0);
	for ( int i = 0;i != cnt; ++i )
	{
		SendAutoMatch();
		DummySleep(300);
	}
}

void GPlayer::SetFirstPlayers( const Json::Value& players_array )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "开始设置首发球员";
	if ( players_array.isNull() || !players_array.isArray() )
		goto LABLE_ERR;
	for ( const auto& json_player : players_array )
	{
		const auto& id = json_player["id"];
		if ( id.isNull() || !id.isString() )
			goto LABLE_ERR;

		const auto& role = json_player["role"];
		if ( role.isNull() || !role.isInt() )
			goto LABLE_ERR;

		auto player = FindPlayerById(id.asCString());
		if ( !player )
		{
			LOG_O(Log_error) << "没有找到指定ID的球员，id是：" << id.asCString();
			goto LABLE_ERR;
		}
		assert(player->player_pos_ >= GType::enPlayerPosition_Third);
		player->player_pos_ = GType::enPlayerPosition_First;
		player->position_info_.pos_ = role.asInt();
	}
	assert(11 == GetThePosPlayersCnt(GType::enPlayerPosition_First));
	return;
LABLE_ERR:
	LOG_O(Log_error) << "比赛结束后，设置首发球员出错";
}

int GPlayer::GetThePosPlayersCnt( GType::enPlayerPosition pos ) const
{
	int cnt = 0;
	for ( const auto& player : players_ )
	{
		if ( pos == player.GetPosition() )
			++cnt;
	}
	return cnt;
}

void GPlayer::SetSecondPlayers( const Json::Value& players_array )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "开始设置替补球员";
	if ( players_array.isNull() || !players_array.isArray() )
		goto LABLE_ERR;
	for ( const auto& json_player : players_array )
	{
		const auto& id = json_player["id"];
		if ( id.isNull() || !id.isString() )
			goto LABLE_ERR;

		auto player = FindPlayerById(id.asCString());
		if ( !player )
		{
			LOG_O(Log_error) << "没有找到指定ID的球员, id:" << id.asCString();
			goto LABLE_ERR;
		}
		assert(player->player_pos_ >= GType::enPlayerPosition_Third);
		player->player_pos_ = GType::enPlayerPosition_Second;
		player->position_info_.pos_ = stPlayer::PositionInfo::kSecondPos;
	}
	assert(7 == GetThePosPlayersCnt(GType::enPlayerPosition_Second));
	return;
LABLE_ERR:
	LOG_O(Log_error) << "比赛结束后，设置替补球员出错";
}

int GPlayer::CanJoinedPlayersCnt() const
{
	int cnt = 0;
	for ( const auto& player : players_ )
	{
		if ( player.GetLife() > 0 )
			++cnt;
	}
	return cnt;
}

bool GPlayer::NeedSupplyThirdPlayers() const
{
	auto third_cnt = 0, second_team_cnt = 0;
	for ( const auto& player : players_ )
	{
		if ( GType::enPlayerPosition_Third == player.GetPosition() )
		{
			++third_cnt;
		}
		else if ( GType::enPlayerPosition_No == player.GetPosition() && player.GetLife() > 0 )
		{
			++second_team_cnt;
		}
	}
	return third_cnt < 18 && second_team_cnt;
}

bool GPlayer::IsTimeoutAtMatchRoom() const
{
	return is_timeout_at_match_room_;
}

IoTimerPtr GPlayer::GetTimer_AtMatchRoom( std::size_t milli_seconds )
{
	if ( timer_at_match_room_ || milli_seconds <= 0 )
	{
		return timer_at_match_room_;
	}
	else
	{
		timer_at_match_room_ = CreateIoTimer(milli_seconds, false, [=](IoTimerPtr){
			this->is_timeout_at_match_room_ = true;
		});
	}
	return timer_at_match_room_;
}

void GPlayer::ResetTimeoutAtMatchRoom()
{
	is_timeout_at_match_room_ = false;
}

GType::enCreateCoach_SubState GPlayer::GetCC_SubState() const
{
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
	{
		return GType::enCC_SubState_Invalid;
	}
	if ( GType::enGameState_CreateCoach != normal_c->GetGameState() )
	{
		return GType::enCC_SubState_Invalid;
	}
	return cc_state_;
}

void GPlayer::SetCC_SubState( GType::enCreateCoach_SubState cc_state )
{
	if ( cc_state_ == cc_state )
		return;
	LOG_ENTER_FUNC;
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
	{
		LOG_O(Log_warning) << "normal_c不存在了";
		return;
	}
	if ( GType::enGameState_CreateCoach != normal_c->GetGameState() )
	{
		LOG_O(Log_warning) << "游戏状态不是创建教练的状态，设置创建教练子状态失败";
		return;
	}

	LOG_O(Log_trace) << "create coach,SubState,之前：" << cc_state_ << "  之后：" << cc_state;
	cc_state_ = cc_state;
}

bool GPlayer::IsCreateCoachProcedure() const
{
	/*if ( GType::enCC_SubState_Invalid == cc_state_ )
		return false;*/
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
		return false;
	return GType::enGameState_CreateCoach == normal_c->GetGameState();
}

std::string GPlayer::GenCoachName()
{
	//return "222";
	return std::move(Random_Str(5, 16));
}

bool GPlayer::IsCreateCoachSucceed() const
{
	return create_coach_succeed_;
}

void GPlayer::SetCreateCoachSucceed( bool is_succeed )
{
	if (IsCreateCoachProcedure())
		create_coach_succeed_ = is_succeed;
}

void GPlayer::SaleSpecifyPlayers( int ability, double price )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "拍卖一些指定的球员，能力值是：" << ability << "\t价格是：" << price;
	for ( auto& player : players_ )
	{
		if ( player.GetAbility() == ability )
		{
			if (player.CanBeSale())
				player.SaleSelfAs(price);
		}
	}

	/*auto it = std::remove_if(players_.begin(), players_.end(), [=]( stPlayer& player ){
		if ( player.GetAbility() == ability )
		{
			return player.CanBeSale() && player.SaleSelfAs(price);
		}
		return false;
	});
	if ( it != players_.end() )
		players_.erase(it, players_.end());*/
}

GPlayer::stPlayer* GPlayer::FindPlayerBySpid( int spid )
{
	auto it = std::find_if(players_.begin(), players_.end(),
		[=]( const stPlayer& player ){
			return player.GetSpid() == spid;
	});
	if ( it != players_.end() )
		return &*it;
	return nullptr;
}

int GPlayer::GetAbilityFilter() const
{
	return ability_filter_;
}

void GPlayer::SetAbilityFilter( int ability_filter )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置过滤的能力值，之前：" << ability_filter_ << "  之后：" << ability_filter;
	ability_filter_ = ability_filter;
}

void GPlayer::SetAchievementInfo( const Json::Value& achievements )
{
	if ( achievements.isNull() || !achievements.isArray() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "设置成就信息，参数不正确";
		return;
	}
	Achievement achieve_tmp;
	for ( const auto& json_achieve : achievements )
	{
		if (achieve_tmp.ParseFromJsonObj(json_achieve))
		{
			if (achieve_tmp.CanBeCollect())
				achieve_tmp.Collect();
		}
	}
}

int GPlayer::GetGp() const
{
	return gp_;
}

void GPlayer::SetGp( int gp )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "set gp, 之前：" << gp_ << "\t之后:" << gp;
	gp_ = gp;
}

bool GPlayer::BuyGiftPackageByIdx( int idx )
{
	if ( !CanBuyGiftPackage(idx) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "买第" << idx << "个礼包失败，因为只有" << gift_packages_.size() << "个礼包";
		return false;
	}

	assert(idx >= 0 && idx < (int)gift_packages_.size());
	return gift_packages_[idx].Buy();
}

bool GPlayer::CanBuyGiftPackage( int idx ) const
{
	return idx >=0 && idx < (int)gift_packages_.size() && gift_packages_[idx].CanBuy();
}

bool GPlayer::SetGiftPackageInfo( const Json::Value& json_gp )
{
	LOG_ENTER_FUNC;
	if ( json_gp.isNull() || !json_gp.isArray() )
	{
		LOG_O(Log_error) << "设置礼包信息之，参数判定不通过";
		return false;
	}

	decltype(gift_packages_) gift_p_tmp;
	GiftPackage gp_tmp;
	for ( const auto& gp_item : json_gp )
	{
		if (gp_tmp.ParseFromJsonObj(gp_item))
		{
			gift_p_tmp.push_back(gp_tmp);
		}
		else
		{
			LOG_O(Log_error) << "json_gp的某一项判定不通过";
			return false;
		}
	}

	std::swap(gift_packages_, gift_p_tmp);
	LOG_O(Log_trace) << "成功设置礼品信息, 当前礼品个数：" << gift_packages_.size();
	return true;
}

bool GPlayer::IsGettingGpData() const
{
	return request_gp_data_;
}

void GPlayer::SetGettingGpDataTag( bool is_getting )
{
	if ( request_gp_data_ == is_getting )
		return;

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置请求GP礼包的标志，之前：" << request_gp_data_ << "\t之后：" << is_getting;
	request_gp_data_ = is_getting;
}

void GPlayer::SetMatchAddict( bool is_addict )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "set match addict->" << is_addict;
	if (is_addict)
	{
		auto func_obj = FuncObj_NormalC::GetInstance();
		if (func_obj)
		{
			func_obj->GetLuaScript().TerminateScript();
			func_obj->GetSessionOper().ToCall(BC_MakeP("SetMatchAddict"));
		}
	}
}

GPlayer::ItemContT& GPlayer::GetAllItems()
{
	return items_;
}

bool GPlayer::CombineSquadInfo( Json::Value& squad_info ) const
{
	if (players_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "球员为空";
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	auto players_iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !players_iter )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "创建首发和替补球员的迭代器失败";
		return false;
	}

	squad_info["_method"] = "PUT";
	Json::Value squard;
	squard["formation"] = 1;
	squard["tactic"] = 0;

	//positions
	Json::Value positions;
	//kicker
	Json::Value kickers;
	auto kicker_cnt = 5;

	for ( players_iter->First(); !players_iter->IsDone(); players_iter->Next() )
	{
		auto player = players_iter->CurItem();
		assert(player);
		if (player)
		{
			Json::Value pos_info;
			pos_info["id"] = player->GetId();
			pos_info["pos"] = player->position_info_.pos_;
			if ( !player->position_info_.starting_.isNull() && !player->position_info_.starting_.empty() )
				pos_info["starting"] = player->position_info_.starting_;

			positions.append(pos_info);
			if ( kicker_cnt > 0 )
			{
				if ( GType::enPlayerPosition_First == player->player_pos_ )
				{
					kickers.append(player->id_);
					--kicker_cnt;
				}
			}
		}
	}
	assert(positions.size() == 18);
	assert(5 == kickers.size());

	squard["positions"] = positions;
	squad_info["captain"] = kickers[1];
	squad_info["kickers"] = kickers;
	squad_info["squad"] = squard;
	return true;
}

void GPlayer::SendSquadInfoAtMatch()
{
	auto& match_mgr = IGMatchMgr::GetMe();
	const auto& match_info = match_mgr.GetMatchInfo();
	if ( 0 == match_info.GetMatchId() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "比赛ID为0，不能发送球员信息";
		return;
	}
	
	Json::Value squad_info;
	if ( !CombineSquadInfo(squad_info) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "组合球队信息失败";
		return;
	}

	CStringA str_uri;
	str_uri.Format("/match/%u/side/left/squad real", match_info.GetMatchId());
	if ( GType::enHttpSendCall_Result_Succeed != GClass::HttpPackageSend::SendEncryptedJsonMsgAsync(str_uri, squad_info) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "球员信息，发包失败";
		return;
	}
}

GPlayer::stPlayer* GPlayer::GetFirstPlayerByPosition( GType::enPlayerPosition pos )
{
	for ( auto& player : players_ )
	{
		if ( pos == player.GetPosition() )
			return &player;
	}
	return nullptr;
}

void GPlayer::OnSellPlayerSucceed( const std::string& player_id )
{
	ErasePlayerById(player_id.c_str());
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "拍卖球员成功";
}

GPlayer::stPlayer* GPlayer::GetCanBeSoldPlayer()
{
	for ( auto& player : players_ )
	{
		if (player.CanBeSale())
			return &player;
	}
	return nullptr;
}

GPlayer::stSelfOfferItem::ContT& GPlayer::GetSelfOfferList()
{
	return self_offer_list_;
}

void GPlayer::RefreshSelfOfferList()
{
	LOG_ENTER_FUNC;
	Json::Value respond;
	if ( !GClass::HttpPackageSend::Get_JsonMsgSync("/transfer/my/offer/list", respond) )
	{
		LOG_O(Log_warning) << "刷新我的出售列表出错";
		return;
	}

	const auto& offers = respond["offers"];
	if ( offers.isNull() || !offers.isArray() )
	{
		LOG_O(Log_trace) << "offers字段不对";
		return;
	}
	stSelfOfferItem::ContT offer_list;
	stSelfOfferItem self_offer;
	for ( const auto& offer : offers )
	{
		if ( !self_offer.ParseFromJsonObj(offer) )
		{
			LOG_O(Log_trace) << "parse我的出售项失败";
			continue;
		}
		if ( stSelfOfferItem::kSS_Sold == self_offer.sold_state_ )
			//卖出去了
		{
			if (self_offer.SoldDone(false))
			{
				continue;
			}
			LOG_O(Log_error) << "不能取消拍卖过期球员";
		}
		if ( stSelfOfferItem::kSS_Expired == self_offer.sold_state_ )
			//过期了
		{
			if (self_offer.CancelExpired(false))
			{
				continue;
			}
			LOG_O(Log_error) << "不能取消拍卖过期球员";
		}
		offer_list.push_back(self_offer);
	}
	offer_list.swap(self_offer_list_);
	LOG_O(Log_trace) << "刷新我的出售列表成功，size->" << self_offer_list_.size();
	time_refresh_offer_list_ = Poco::DateTime();

	SendSelfSoldInfo();
}

void GPlayer::CancelSale( const std::string& id )
{
	auto it = std::find_if(self_offer_list_.begin(), self_offer_list_.end(), [&]( const stSelfOfferItem& offer_item ){
		return offer_item.id_ == id;
	});
	if ( it != self_offer_list_.end() )
	{
		self_offer_list_.erase(it);
	}
}

GPlayer::stSelfOfferItem* GPlayer::FindSoldPlayer( const std::string& player_id )
{
	for ( auto& sold_player : self_offer_list_ )
	{
		if ( sold_player.player_id_ == player_id )
			return &sold_player;
	}
	return nullptr;
}

void GPlayer::SendSelfSoldInfo() const
{
	auto func_obj = FuncObj_NormalC::GetInstance();
	if ( !func_obj )
		return;
	Json::Value sold_players;
	if ( !GenSoldPlayersInfo(sold_players, true) )
		return;
	if ( sold_players.empty() )
		return;
	func_obj->GetCoordinate().SellPlayersSwitch(sold_players);
}

bool GPlayer::GenSoldPlayersInfo( Json::Value& sold_players_info, bool against_offer_price ) const
{
	if (self_offer_list_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "我的出售列表为空";
		return false;
	}
	Json::Value sold_player;
	for ( const auto& offer_item : self_offer_list_ )
	{
		if ( stSelfOfferItem::kSS_Waiting == offer_item.sold_state_ )
		{
			if (against_offer_price)
			{
				//拍卖价格大于，则不转发给对方阵营
				if ( against_offer_price_ >= 0 && offer_item.price_ >= against_offer_price_ )
					continue;
			}
			if ( !offer_item.ConvertToJson(sold_player) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "can not convert";
				return false;
			}
			sold_players_info.append(sold_player);
		}
	}
	return true;
}

GPlayer::stSellingPlayerInfo::ContT& GPlayer::GetSellingPlayers()
{
	return selling_players_;
}

GPlayer::stSellingPlayerInfo* GPlayer::FindSellingPlayer( const stOfferItem& offer_item )
{
	auto pair_range = selling_players_.equal_range(offer_item.spid_);
	for ( ; pair_range.first != pair_range.second; ++pair_range.first )
	{
		auto& selling_item = pair_range.first->second;
		if ( offer_item.price_ == selling_item.price_ && offer_item.issued_at_ == selling_item.oldest_date_ )
		{
			return &selling_item;
		}
	}
	return nullptr;
}

GPlayer::stSellingPriceRange::ContT& GPlayer::GetSellingPrice()
{
	return selling_price_;
}

GPlayer::stSellingPriceRange* GPlayer::FindSellingPrice( int spid, int grade )
{
	auto it = selling_price_.find(stSellingPriceRange::KeyT(spid, grade));
	if ( it != selling_price_.end() )
		return &it->second;
	return nullptr;
}

bool GPlayer::UpdateSpecifySellingPlayer( int spid, stSellingPlayerInfo::ContT& out_selling_players )
{
	Json::Value respond;
	CStringA uri;
	uri.Format("/transfer/search/%u", spid);
	if ( !GClass::HttpPackageSend::Get_JsonMsgSync(uri, respond) )
		return false;
	stSellingPlayerInfo selling_player;
	LOG_ENTER_FUNC;

	for ( const auto& sold_player : respond )
	{
		if ( !selling_player.ParseFromJsonObj(sold_player) )
		{
			LOG_O(Log_error) << "解析拍卖球员数据出错, spid->" << spid;
			return false;
		}
		out_selling_players.insert(stSellingPlayerInfo::ContT::value_type(spid, selling_player));
	}
	LOG_O(Log_trace) << "更新拍卖球员数据成功，spid->" << spid;
	return true;
}

bool GPlayer::UpdateSellingPrice( int spid, int grade )
{
	const auto kKey = stSellingPriceRange::KeyT(spid, grade);

	auto it = selling_price_.find(kKey);
	if ( it == selling_price_.end() )
	{
		stSellingPriceRange selling_range;
		if ( !selling_range.Update(spid, grade) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新价格信息失败， spid->" << spid << "\tgrade->" << grade;
			return false;
		}
		selling_price_.insert(stSellingPriceRange::ContT::value_type(
			kKey, selling_range
			));
	}
	else
	{
		if ( !it->second.NeedUpdate() )
			return true;
		if ( !it->second.Update(spid, grade) )
			return false;
	}
	return true;
}

bool GPlayer::AddOtherOffers( const Json::Value& sold_players )
{
	const auto kSize = others_offers_.size();
	if ( kSize > 2000 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "其他人的球员信息太多了，忽略之, size->" << kSize;
		return false;
	}
	stOfferItem offer_item;
	for ( const auto& json_player : sold_players )
	{
		if ( !offer_item.ParseFromJsonObj(json_player) )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "解析失败，无法添加其他人的拍卖项";
			continue;
		}
		auto it = std::find(others_offers_.begin(), others_offers_.end(), offer_item);
		if ( it == others_offers_.end() )
			others_offers_.push_back(offer_item);
	}
	//排序对方阵营的拍卖信息，价格从小到大排序
	others_offers_.sort([]( const stOfferItem& lhs, const stOfferItem& rhs ){
		return lhs.price_ < rhs.price_;
	});
	return true;
}

bool GPlayer::DeleteOtherOffers( const Json::Value& sold_players )
{
	stOfferItem offer_item;
	for ( const auto& json_player : sold_players )
	{
		if ( !offer_item.ParseFromJsonObj(json_player) )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "解析失败，无法删除其他人的拍卖项";
			continue;
		}
		auto it = std::find(others_offers_.begin(), others_offers_.end(), offer_item);
		if ( it != others_offers_.end() )
			others_offers_.erase(it);
	}
	return true;
}

bool GPlayer::KeepSellingPlayerAs( double price, int cnt )
{
	if ( cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "个数太少了，cnt->" << cnt;
		return false;
	}
	if ( price < 100 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "拍卖的价格不太小了,price->" << price;
		return false;
	}

	price = stPlayer::AdjustPrice(price);

	for ( const auto& offer_item : self_offer_list_ )
	{
		if ( offer_item.price_ == price )
		{
			if ( --cnt < 1 )
				return true;
		}		
	}

	//补足拍卖球员
	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}
	//拍卖成功的球员
	std::vector<std::string> sale_players;
	LOG_ENTER_FUNC;

	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		if ( !player->CanBeSale_NoStrict() )
			continue;

		//判断这个球员是否在有效的价格范围内
		if ( !UpdateSellingPrice(player->spid_, player->grade_) )
		{
			LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}
		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}
		if ( !price_range->InRange(price) )
			continue;

		if ( !player->SaleSelfAs_NoStrict(price, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( --cnt < 1 )
			break;
	}
	if ( !sale_players.empty() )
	{
		auto func_obj = FuncObj_NormalC::GetInstance();
		if ( !func_obj )
			return false;
		RefreshSelfOfferList();
		Json::Value json_sold_player, json_array;
		for ( const auto& player_id : sale_players )
		{
			ErasePlayerById(player_id.c_str());

			auto sold_player = FindSoldPlayer(player_id);
			if ( !sold_player )
			{
				LOG_O(Log_error) << "拍卖了一个球员，竟然从我的拍卖列表中找不到,player id->" << player_id;
				continue;
			}
			
			if ( !sold_player->ConvertToJson(json_sold_player) )
			{
				assert(false);
				continue;
			}
			json_array.append(json_sold_player);
		}
		if (json_array.empty() )
		{
			LOG_O(Log_error) << "json array竟然为空!";
		}
		else
		{
			func_obj->GetCoordinate().SellPlayersSwitch(json_array);
		}
	}
	return cnt < 1;
}

bool GPlayer::KeepPlayersCntMoreThan( int cnt, double price_limit )
{
	if ( cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "数量太少,cnt->" << cnt;
		return false;
	}
	if ( price_limit < 100 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "价格上限不对，price limit->" << price_limit;
		return false;
	}
	const auto kMaxPlayerCnt = max_players_cnt_;
	const auto kCurPlayerCnt = GetAllPlayersCnt();
	if ( kCurPlayerCnt >= kMaxPlayerCnt )
		return true;
	cnt = std::min(kMaxPlayerCnt, cnt);
	auto need_cnt = cnt - kCurPlayerCnt;
	if ( need_cnt < 1 )
		return true;

	//开卡补足
	need_cnt -= OpenSomePlayersAs(need_cnt);
	assert(need_cnt >= 0);
	if ( 0 == need_cnt )
		return true;

	//购买球员
	need_cnt -= BuySomeOtherPlayers(need_cnt, price_limit);
	assert(need_cnt >= 0);
	return 0 == need_cnt;
}

bool GPlayer::CanBuy( double price )
{
	const auto& gp = GPlayer::GetMe();
	return gp.GetAllPlayersCnt() < gp.max_players_cnt_ && gp.GetEp() >= price;
}

bool GPlayer::Buy( int spid, int grade, double price )
{
	if ( !CanBuy(price) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "条件不足，不能购买";
		return false;
	}
	Json::Value respond;
	CStringA uri;
	auto real_price = (unsigned __int64)price;
	uri.Format("/transfer/purchase/%u/%u/%I64u/purchase", spid, grade, real_price);

	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(uri, nullptr, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;

	static Json::Path path_ep(".resources.[0].body");
	const auto& body_ep = path_ep.resolve(respond);
	if ( body_ep.isNull() || !body_ep.isNumeric() )
		return false;
	static Json::Path path_new_player(".resources.[1].body");
	const auto& body_new_player = path_new_player.resolve(respond);
	if ( body_new_player.isNull() || !body_new_player.isObject() )
		return false;

	auto& gp = GPlayer::GetMe();
	gp.SetEp(body_ep.asDouble());
	stPlayer new_player;
	if ( !new_player.ParseFromJsonObj(body_new_player) )
		return false;
	gp.GetAllPlayers().push_back(new_player);

	LOG_O(Log_trace) << "购买球员成功";
	return true;
}

bool GPlayer::BuyOtherPlayers()
{
	const auto kMaxPlayerCnt = max_players_cnt_;
	auto cur_player_cnt = GetAllPlayersCnt();
	if ( cur_player_cnt >= kMaxPlayerCnt )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "当然球员数为" << cur_player_cnt << "，不能再购买其他球员了";
		return false;
	}
	auto need_cnt = kMaxPlayerCnt - cur_player_cnt;
	assert(need_cnt > 0);
	need_cnt -= BuySomeOtherPlayers(need_cnt, std::numeric_limits<int>::max());
	assert(need_cnt >= 0);
	return 0 == need_cnt;
}

int GPlayer::BuySomeOtherPlayers( int cnt, double price_limit )
{
	assert(price_limit >= 100);
	if ( cnt < 1 )
		return 0;
	const auto kMaxPlayerCnt = max_players_cnt_;
	const auto kCurPlayerCnt = GetAllPlayersCnt();
	if ( kCurPlayerCnt >= kMaxPlayerCnt )
		return 0;
	auto need_cnt = std::min(cnt, kMaxPlayerCnt - kCurPlayerCnt);
	auto cnt_tmp = need_cnt;
	std::vector<stOfferItem::ContT::iterator> erase_iters;
	for ( auto it = others_offers_.begin(); it != others_offers_.end(); ++it )
	{
		if ( it->price_ >= price_limit )
			continue;
		if ( !it->CanBuy() )
			continue;
		if ( !it->Buy(false) )
			continue;
		erase_iters.push_back(it);
		if ( --cnt_tmp < 1 )
			break;
	}

	for ( const auto& iter : erase_iters )
	{
		others_offers_.erase(iter);
	}
	/*//购买的球员并不一定是本局域网出售的，所以先去掉这些，防错而已
	auto func_obj = FuncObj_NormalC::GetInstance();
	if ( !func_obj )
		return need_cnt - cnt_tmp;
	
	Json::Value sold_player, json_array;
	for ( const auto& iter : erase_iters )
	{
		if (iter->ConvertToJson(sold_player))
		{
			json_array.append(sold_player);
		}
		others_offers_.erase(iter);
	}
	if ( !json_array.empty() )
		func_obj->GetCoordinate().CancelSale_SelfSide(json_array);*/
	return need_cnt - cnt_tmp;
}

int GPlayer::OpenSomePlayersAs( int cnt )
{
	if ( cnt < 1 )
		return 0;
	const auto kMaxPlayerCnt = max_players_cnt_;
	const auto kCurPlayerCnt = GetAllPlayersCnt();
	if ( kCurPlayerCnt >= kMaxPlayerCnt )
		return 0;
	auto need_cnt = std::min(cnt, kMaxPlayerCnt - kCurPlayerCnt);
	auto cnt_tmp = need_cnt;

	static const int kItemIds[] = {
		stItem::ItemId_BlueCard(), stItem::ItemId_GreenCard(),
		stItem::ItemId_RedCard(), stItem::ItemId_PurpleCard(),
		stItem::ItemId_BlueBallCard(), stItem::ItemId_GreenBallCard(),
		stItem::ItemId_RedBallCard(), stItem::ItemId_13ForwardCard(),
		stItem::ItemId_13MidCard(), stItem::ItemId_13BackCard(),
		stItem::ItemId_13GateCard(), stItem::ItemId_32CountryCard(),
		stItem::ItemId_OrangeCard(), stItem::ItemId_WCard(),
		20001, 20002, 20004, 20027, /*30002,*/
		502/*支付型EP卡*/, 20003/*13西班牙足球甲级联赛*/, 28002/*13西班牙足球甲级联赛球员报(能力值65)*/,
		28001, 28003, 28004, //推测卡
		82214174/*世界杯32强国家队球员卡*/, 82214173/*世界杯16强国家队球员卡*/,
		82214172/*世界杯8强国家队球员卡*/, 82214171/*世界杯4强国家队球员卡*/,
		82214191/*世界杯失意球员卡*/,
		556/*支付EP卡（20000EP）*/, 565/*支付EP卡（200000EP)*/,
		82214192/*10 赛季球员卡*/, 150008/*橙色球探卡*/,
		82214195, 82214196, 82214197, 82214407,
		28006, 28007, 28008, 28009, 150003,82214102,82214101,82214189,82214406,82231066,570,572,
		1000207,82214188,82214405,1000211,82214194,82214103
	};
	auto specific_use = stItem::GetSpecificUse();

	for ( auto i = 0; i != _countof(kItemIds); ++i )
	{
		auto item = FindItemByItemId(kItemIds[i]);
		if ( !item )
			continue;
		while ( item->remains_ > 0 )
		{
			if ( !item->CanUse() )
				break;
			if ( !item->Use() )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "使用物品失败0,item id->" << item->itemid_;
				break;
			}
			if ( --cnt_tmp < 1 )
				goto LABEL_RET;
			item = FindItemByItemId(kItemIds[i]);
			if ( !item )
				break;
		}
	}
	
	if ( specific_use.first && specific_use.second > 0 )
	{
		for ( auto i = 0; i != specific_use.second; ++i )
		{
			auto item = FindItemByItemId(specific_use.first[i].item_id_);
			if ( !item )
				continue;
			while ( item->remains_ > 0 )
			{
				if ( !item->CanUse() )
					break;
				if ( !item->UseEx(specific_use.first[i]) )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "使用物品失败1,item id->" << item->itemid_;
					break;
				}
				if ( --cnt_tmp < 1 )
					goto LABEL_RET;
				item = FindItemByItemId(specific_use.first[i].item_id_);
				if ( !item )
					break;
			}
		}
	}
	
LABEL_RET:
	return need_cnt - cnt_tmp;
}

bool GPlayer::NeedRefreshSelfOffserList() const
{
	static bool exec_once = false;
	if ( !exec_once )
	{
		exec_once = true;
		return true;
	}
	Poco::DateTime time_now;
	auto time_span = time_now - time_refresh_offer_list_;
	//1分钟以上才允许刷新我的拍卖列表
	return time_span.totalMinutes() >= 1;
}

void GPlayer::ClearOtherSoldInfo()
{
	others_offers_.clear();
}

bool GPlayer::GenOtherOfferItemsInfo( Json::Value& sold_players_info ) const
{
	if (others_offers_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "其他号的出售项为空";
		return false;
	}
	Json::Value sold_player;
	for ( const auto& offer_item : others_offers_ )
	{
		if ( !offer_item.ConvertToJson(sold_player) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "can not convert";
			return false;
		}
		sold_players_info.append(sold_player);
	}
	return true;
}

double GPlayer::GetExchPriceFilter() const
{
	return exchange_price_filter_;
}

void GPlayer::SetExchPriceFilter( double price_filter )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置价值过滤，之前：" << exchange_price_filter_ << "\t之后：" << price_filter;
	exchange_price_filter_ = price_filter;
}

bool GPlayer::KeepSellingRubbishPlayer( double price_min, double price_max )
{
	for ( const auto& sell_item : self_offer_list_ )
	{
		if ( stSelfOfferItem::kSS_Waiting == sell_item.sold_state_ && sell_item.price_ <= price_max && sell_item.price_ >= price_min )
			return true;
	}

	auto func_obj = FuncObj_NormalC::GetInstance();
	if ( !func_obj )
		return false;

	//寻找一个最高价值的球员，卖掉
	//补足拍卖球员
	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<stPlayer*> players_tmp;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		if ( !player->CanBeSale_NoStrict() )
			continue;
		players_tmp.push_back(player);
	}
	if (players_tmp.empty())
		return false;
	//排序球员，从大到小
	std::sort(players_tmp.begin(), players_tmp.end(), []( const stPlayer* lhs, const stPlayer* rhs ){
		assert(lhs && rhs);
		return lhs->GetPrice() > rhs->GetPrice();
	});
	
	for ( stPlayer* player : players_tmp )
	{
		if ( player->GetPrice() > price_max )
			continue;
		if ( player->GetPrice() < price_min )
			continue;
		if ( !UpdateSellingPrice(player->spid_, player->grade_) )
		{
			LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}
		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}

		if (player->SaleSelfAs_NoStrict(price_range->price_lower_, true, true))
			return true;
	}
	return false;
}

bool GPlayer::KeepSellingGoodPlayers( int cnt, double price_limit )
{
	if ( cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "个数太少了，cnt->" << cnt;
		return false;
	}
	if ( price_limit < 100 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "价格上限不对，price limit->" << price_limit;
		return false;
	}
	for ( const auto& offer_item : self_offer_list_ )
	{
		if ( stSelfOfferItem::kSS_Waiting == offer_item.sold_state_ )
		{
			if ( --cnt < 1 )
				return true;
		}		
	}

	//补足拍卖球员
	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}
	
	LOG_ENTER_FUNC;

	std::vector<stPlayer*> players_tmp;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		if ( !player->CanBeSale_NoStrict() )
			continue;
		players_tmp.push_back(player);
	}

	if (players_tmp.empty())
		return false;
	//排序球员，从大到小
	std::sort(players_tmp.begin(), players_tmp.end(), []( const stPlayer* lhs, const stPlayer* rhs ){
		assert(lhs && rhs);
		return lhs->GetPrice() > rhs->GetPrice();
	});

	//拍卖成功的球员
	std::vector<std::string> sale_players;
	for ( stPlayer* player : players_tmp )
	{
		//判断这个球员是否在有效的价格范围内
		if ( !UpdateSellingPrice(player->spid_, player->grade_) )
		{
			LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}
		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
			continue;
		}
		if ( price_range->price_upper_ >= price_limit )
			continue;

		if ( !player->SaleSelfAs_NoStrict(price_range->price_upper_, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( --cnt < 1 )
			break;
	}

	if ( !sale_players.empty() )
	{
		auto func_obj = FuncObj_NormalC::GetInstance();
		if ( !func_obj )
			return false;
		RefreshSelfOfferList();
		Json::Value json_sold_player, json_array;
		for ( const auto& player_id : sale_players )
		{
			ErasePlayerById(player_id.c_str());
			auto sold_player = FindSoldPlayer(player_id);
			if ( !sold_player )
			{
				LOG_O(Log_error) << "拍卖了一个球员，竟然从我的拍卖列表中找不到,player id->" << player_id;
				continue;
			}

			if ( !sold_player->ConvertToJson(json_sold_player) )
			{
				assert(false);
				continue;
			}
			json_array.append(json_sold_player);
		}
		if (json_array.empty() )
		{
			LOG_O(Log_error) << "json array竟然为空!";
		}
		else
		{
			func_obj->GetCoordinate().SellPlayersSwitch(json_array);
		}
	}
	return cnt < 1;
}

bool GPlayer::SalePlayersAsDefault( double price_lower, double price_upper )
{
	if ( price_lower > price_upper || price_lower < 100 || price_upper < 100 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "指定的价格范围不对，price lower->" << price_lower << "\tprice upper->" << price_upper;
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		auto player_price = player->GetPrice();
		if ( player_price < price_lower || player_price > price_upper )
			continue;
		if ( !player->CanBeSale_NoStrict() )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}

		if ( !player->SaleSelfAs_NoStrict(price_range->price_default_, false, false) )
			continue;
		sale_players.push_back(player->id_);
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::HasSamePlayerAtPos( GType::enPlayerPosition player_position[], int position_cnt, const stPlayer& player, const PlayerContT& players )
{
	if ( !player_position || position_cnt <= 0 )
	{
		assert(false);
		return false;
	}

	for ( auto& p : players )
	{
		if ( !p.IsInPos(player_position, position_cnt) )
			continue;
		if ( player.GetSpid() == p.GetSpid() )
			return true;
	}
	return false;
}

std::pair<bool, bool> GPlayer::DegradeFirstAndSecondSync()
{
	std::pair<bool, bool> res(false, false);

	GType::enPlayerPosition pos_first_team[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second, GType::enPlayerPosition_Third
	};
	PlayerContT first_second_players;
	for ( const auto& player : players_ )
	{
		if (player.IsInPos(pos_first_team, _countof(pos_first_team)))
		{
			first_second_players.push_back(player);
		}
	}
	if ( first_second_players.size() < 18 )
	{
		assert(false);
		return res;
	}
	//排序球员
	first_second_players.sort([]( const stPlayer& lhs, const stPlayer& rhs ){
		return lhs.spid_ < rhs.spid_;
	});
	//去除相同的球员
	auto it = std::unique(first_second_players.begin(), first_second_players.end(), []( const stPlayer& lhs, const stPlayer& rhs ){
		return lhs.spid_ == rhs.spid_;
	});
	//新球员信息
	PlayerContT new_players;
	if ( it != first_second_players.end() )
	{
		const auto kItEnd = first_second_players.end();
		auto it_tmp = it;
		while ( it_tmp != kItEnd )
		{
			new_players.push_back(*it_tmp++);
		}
		first_second_players.erase(it, kItEnd);
	}
	if ( first_second_players.size() < 18 )
	{
		assert(false);
		return res;
	}
	//按照剩余场次从大到小排序
	first_second_players.sort([]( const stPlayer& lhs, const stPlayer& rhs ){
		return lhs.life_ > rhs.life_;
	});
	
	//删除多余的球员，保留前18个球员
	while ( first_second_players.size() > 18 )
	{
		new_players.push_back(first_second_players.back());
		first_second_players.pop_back();
	}
	assert(18 == first_second_players.size());

	GType::enPlayerPosition positions_upper[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	//首发和替补
	auto iter_first_second = CreatePlayerIterByPosition(positions_upper, _countof(positions_upper));
	if ( !iter_first_second )
		return res;
	
	for ( iter_first_second->First(), it = first_second_players.begin(); !iter_first_second->IsDone()
		&& it != first_second_players.end(); iter_first_second->Next(), ++it )
	{
		auto player = iter_first_second->CurItem();
		if ( !player )
		{
			assert(false);
			return res;
		}
		//设置位置信息
		it->player_pos_ = player->player_pos_;
		it->position_info_ = player->position_info_;
	}
	assert(it == first_second_players.end());
	assert(iter_first_second->IsDone());

	GType::enPlayerPosition positions_lower[] = {
		GType::enPlayerPosition_Third
	};
	//职位低的
	auto iter_lower_player = CreatePlayerIterByPosition(positions_lower, _countof(positions_lower));
	if ( !iter_lower_player )
		return res;

	//设置候补信息
	{
		for ( iter_lower_player->First(), it = new_players.begin(); !iter_lower_player->IsDone() &&
			it != new_players.end(); iter_lower_player->Next(), ++it )
		{
			auto player = iter_lower_player->CurItem();
			if ( !player )
			{
				assert(false);
				return res;
			}
			//设置位置信息
			it->player_pos_ = player->player_pos_;
			it->position_info_ = player->position_info_;
		}
	}
	assert(it == new_players.end());
	assert(iter_lower_player->IsDone());

	//添加首发和替补
	for ( const auto& player : first_second_players )
	{
		new_players.push_back(player);
	}
	//添加没有职位的球员
	GType::enPlayerPosition pos_no[] = {
		GType::enPlayerPosition_No
	};
	auto player_no = CreatePlayerIterByPosition(pos_no, _countof(pos_no));
	if ( !player_no )
		return res;
	for ( player_no->First(); !player_no->IsDone(); player_no->Next() )
	{
		auto player = player_no->CurItem();
		if ( !player )
		{
			assert(false);
			return res;
		}
		new_players.push_back(*player);
	}
	assert(new_players.size() == players_.size());

	//发送首发和替补消息
	if ( !SendFirstAndSecondPlayersSync(first_second_players) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "发送首发和替补消息失败";
		return res;
	}

	std::swap(new_players, players_);

	res.first = true;
	res.second = !IsNeedAutoSelectPlayer();
	return res;
}

bool GPlayer::SendFirstAndSecondPlayersSync( const PlayerContT& first_second_players )
{
	if ( 18 != first_second_players.size() )
	{
		assert(false);
		return false;
	}
	Json::Value squad_info;
	squad_info["_method"] = "PUT";
	Json::Value squard;
	squard["formation"] = 1;
	squard["tactic"] = 0;

	//positions
	Json::Value positions;
	//kicker
	Json::Value kickers;
	auto kicker_cnt = 5;

	for ( const auto& player : first_second_players )
	{
		Json::Value pos_info;
		pos_info["id"] = player.GetId();
		pos_info["pos"] = player.position_info_.pos_;
		if ( !player.position_info_.starting_.isNull() && !player.position_info_.starting_.empty() )
			pos_info["starting"] = player.position_info_.starting_;

		positions.append(pos_info);
		if ( kicker_cnt > 0 )
		{
			if ( GType::enPlayerPosition_First == player.player_pos_ )
			{
				kickers.append(player.id_);
				--kicker_cnt;
			}
		}
	}
	assert(positions.size() == 18);
	assert(5 == kickers.size());

	squard["positions"] = positions;
	squad_info["captain"] = kickers[1];
	squad_info["kickers"] = kickers;
	squad_info["squad"] = squard;
	
	Json::Value respond;
	if ( !GClass::HttpPackageSend::Post_EncryptJsonMsgSync("/coach/me/squad", squad_info, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
	{
		LOG_O(Log_warning) << "交换球员失败，则进行续约";
		ContractFirstAndSecondPlayers();
		return false;
	}

	static Json::Path path_team_squad(".resources.[2].path");
	const auto& team_squad = path_team_squad.resolve(respond);
	if ( team_squad.isNull() || !team_squad.isString() || team_squad.asString() != "team.squad" )
		return false;
	return true;
}

bool GPlayer::HasSamePlayerAtPos( const stPlayer& player, const PlayerContT& players )
{
	for ( auto& p : players )
	{
		if ( player.GetSpid() == p.GetSpid() )
			return true;
	}
	return false;
}

bool GPlayer::DegradeAllThirdSamePlayers()
{
	GType::enPlayerPosition positions_upper[] = {
		GType::enPlayerPosition_Third
	};
	//职位高的
	auto player_iter = CreatePlayerIterByPosition(positions_upper, _countof(positions_upper));
	LOG_ENTER_FUNC;
	if ( !player_iter )
	{
		LOG_O(Log_error) << "降级侯补球员中，创建迭代器失败";
		return false;
	}

	GType::enPlayerPosition pos_first_second[] = {
		GType::enPlayerPosition_First, GType::enPlayerPosition_Second
	};
	for ( player_iter->First(); !player_iter->IsDone(); player_iter->Next() )
	{
		auto player = player_iter->CurItem();
		assert(player);
		if (player)
		{
			if ( !HasSamePlayerAtPos(pos_first_second, _countof(pos_first_second), *player, players_) )
				continue;
			//降级
			assert(player->CanDegradeFromThirdToNo());
			if ( !player->DegradeFromThirdToNo() )
				return false;
		}
	}
	return true;
}

void GPlayer::LuaDegradeFirstAndSecondSync()
{
	DegradeFirstAndSecondSync();
}

bool GPlayer::ChooseExchPlayer()
{
	Json::Value msg, respond;
	msg["select"] = int(0);
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/players/trade/contract", &msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_player(".resources.[0].body");
	const auto& json_player = path_player.resolve(respond);
	if ( json_player.isNull() || !json_player.isObject() )
		return false;
	stPlayer player_tmp;
	if ( !player_tmp.ParseFromJsonObj(json_player) )
		return false;
	players_.push_back(player_tmp);
	return true;
}

double GPlayer::GetHighPriceFilter() const
{
	return high_price_filter_;
}

void GPlayer::SetHighPriceFilter( double price_filter )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置高价值过滤,之前->" << high_price_filter_ << "\t之后->" << price_filter;
	high_price_filter_ = price_filter;
}

bool GPlayer::ExchPlayersInPosNo( int price_filter, int cnt )
{
	if ( price_filter < 100 || cnt < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "参数判定失败, price filter->" << price_filter << "\tcnt->" << cnt;
		return false;
	}
	cnt = std::min(cnt, 10);
	PlayerContT players_tmp;
	for ( const auto& player : players_ )
	{
		if ( GType::enPlayerPosition_No != player.GetPosition() )
			continue;
		if ( player.GetPrice() >= price_filter )
			continue;
		players_tmp.push_back(player);
		if ( --cnt < 1 )
			break;
	}
	return stPlayer::ExchangePlayers(players_tmp);
}

double GPlayer::GetAgainstOfferPrice() const
{
	return against_offer_price_;
}

void GPlayer::SetAgainstOfferPrice( double price )
{
	against_offer_price_ = price;
}

bool GPlayer::UnpackAllBags()
{
	Json::Value respond;
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/bag/unpackall", nullptr, respond) )
	{
		LOG_O(Log_error) << "接收保管箱物品失败";
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	const auto& json_res = respond["resources"];
	if ( json_res.isNull() || !json_res.isArray() || json_res.empty() )
		return false;
	auto& gp = GPlayer::GetMe();
	for ( const auto& json_value : json_res )
	{
		const auto& json_path = json_value["path"];
		if ( json_path.isNull() || !json_path.isString() )
			continue;
		const auto& str_path = json_path.asString();
		if ( str_path == "money" )
		{
			const auto& json_body = json_value["body"];
			if ( json_body.isNull() || !json_body.isObject() )
				continue;
			const auto& json_ep = json_body["ep"];
			if ( json_ep.isNull() || !json_ep.isNumeric() )
				continue;
			gp.SetEp(json_ep.asDouble());
		} 
		else if ( str_path == "items" )
		{
			const auto& json_body = json_value["body"];
			if ( json_body.isNull() || !json_body.isArray() )
				continue;
			gp.SetBagItems(json_body);
		}
		else if ( str_path == "gpdata" )
		{
			const auto& json_body = json_value["body"];
			if ( json_body.isNull() || !json_body.isObject() )
				continue;
			const auto& json_point = json_body["point"];
			if ( json_point.isNull() || !json_point.isInt() )
				continue;
			gp.SetGp(json_point.asInt());
		}
	}

	return true;
}

bool GPlayer::RefreshCoachMeImpl(bool force_refresh)
{
	Poco::DateTime time_now;
	auto time_span = time_now - time_refresh_coach_me_;
	//每隔一段时间后，才允许刷新教练信息
	if (!force_refresh && time_span.totalMinutes() < 10)
		return false;

	using namespace GClass;
	Json::Value respond;
	if (!HttpPackageSend::Get_JsonMsgSync("/coach/me", respond))
		return false;
	if (respond.isNull() || !respond.isObject())
		return false;
	time_refresh_coach_me_ = time_now;

	//money
	const auto& json_money = respond["money"];
	if (json_money.isNull() || !json_money.isObject())
		return false;
	//ep
	const auto& json_ep = json_money["ep"];
	if (json_ep.isNull() || !json_ep.isNumeric())
		return false;
	SetEp(json_ep.asDouble());

	//level info
	const auto& json_level_info = respond["levelinfo"];
	if (json_level_info.isNull() || !json_level_info.isObject())
		return false;
	stLevelInfo level_info;
	if (!level_info.ParseFromJsonObj(json_level_info))
		return false;
	SetLevelInfo(level_info);

	//name
	const auto& json_name = respond["name"];
	if (json_name.isNull() || !json_name.isString())
		return false;
	SetName(json_name.asCString());

	//team
	const auto& json_team = respond["team"];
	if (json_team.isNull() || !json_team.isObject())
		return false;
	//players
	const auto& json_players_array = json_team["players"];
	if (json_players_array.isNull() || !json_players_array.isArray())
		return false;
	if (json_players_array.size() < 18)
	{
		assert(false);
		return false;
	}
	if (!Init_SetAllPlayers(json_players_array))
		return false;

	//squad
	const auto& json_squad = json_team["squad"];
	if (json_squad.isNull() || !json_squad.isObject())
		return false;
	//squad positions
	const auto& json_squad_pos = json_squad["positions"];
	if (!SetFirstSecondPlayers(json_squad_pos))
		return false;

	//items
	const auto& json_items = respond["items"];
	if (json_items.isNull() || !json_items.isArray())
		return false;
	if (!SetBagItems(json_items))
		return false;

	//gp
	const auto& json_gp = respond["gp"];
	if (json_gp.isNull() || !json_gp.isInt())
		return false;
	SetGp(json_gp.asInt());

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "刷新教练所有信息成功";
	return true;
}
bool GPlayer::RefreshCoachMe()
{
	return RefreshCoachMeImpl(false);
}

bool GPlayer::SetJsonLevelInfo( const Json::Value& json_level_info )
{
	if ( !level_info_.ParseFromJsonObj(json_level_info) )
		return false;

	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP("SetTrainerLevel")<<level_info_.GetLevel());
	}
	return true;
}

void GPlayer::SetTimeWaitSalePlayer( int time_hour )
{
	if ( time_hour < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "设置等待拍卖球员的时间，参数不正确, time hour->" << time_hour;
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置等待拍卖球员的时间，之前->" << hour_wait_sale_player_ << "\t之后->" << time_hour;
	hour_wait_sale_player_ = time_hour;
}

int GPlayer::GetTimeWaitSalePlayer() const
{
	return hour_wait_sale_player_;
}

void GPlayer::HandleTimeCancelSale()
{
	Poco::DateTime time_now;
	auto time_span = time_now - time_cancel_sale_wait_players_;
	if ( time_span.totalMinutes() < 10 )
		return;
	time_cancel_sale_wait_players_ = time_now;
	if ( GetAllPlayersCnt() >= max_players_cnt_ )
		return;

	std::vector<std::string> cancel_players;
	for ( auto& player : self_offer_list_ )
	{
		if (player.IsWaitSaleTimeTooLong())
		{
			if (player.CancelSale_Waiting(false))
			{
				cancel_players.push_back(player.id_);
			}
		}
	}
	for ( const auto& player_id : cancel_players )
	{
		CancelSale(player_id);
	}
}

bool GPlayer::GenAllSoldPlayersInfo( Json::Value& sold_players_info ) const
{
	if (self_offer_list_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "我的出售列表为空";
		return false;
	}
	Json::Value sold_player;
	for ( const auto& offer_item : self_offer_list_ )
	{
		if ( !offer_item.ConvertToJson(sold_player) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "can not convert";
			return false;
		}
		sold_players_info.append(sold_player);
	}
	return true;
}

void GPlayer::SetMatchLimitCnt( int cnt )
{
	assert(cnt > 0);
	match_cnt_limit_ = cnt;
}

int GPlayer::GetMatchLimitCnt() const
{
	return match_cnt_limit_;
}

bool GPlayer::SaleSomePlayersAsMaxPrice( int player_cnt )
{
	if ( player_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以最高价拍卖一些球员之要拍卖的球员数量参数不正确";
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		if ( !player->CanBeSale_NoStrict() )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}

		if ( !player->SaleSelfAs_NoStrict(price_range->price_upper_, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( (int)sale_players.size() >= player_cnt )
			break;
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::SaleThePlayersAsMaxPrice( int player_cnt, int player_id )
{
	if ( player_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以最高价拍卖指定的球员之要拍卖的球员数量参数不正确";
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		//LOG_O(Log_trace) << "spid_->" << player->spid_;
		if ( !player->CanBeSale_NoStrict() )
			continue;
		if ( player->spid_ != player_id )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}

		if ( !player->SaleSelfAs_NoStrict(price_range->price_upper_, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( (int)sale_players.size() >= player_cnt )
			break;
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::IsFirstCalcDoubleTime() const
{
	return !double_time_;
}

bool GPlayer::IsAtDoubleTime() const
{
	if ( !double_time_ )
		return true;
	DateTimeLocal time_now;
	if ( time_now.hour() != double_time_->hour() && 0 == time_now.hour() % 2 )
	{
		if ( time_now.minute() > 5 )
			return true;
	}
	return false;
}

void GPlayer::UpdateDoubleTime()
{
	double_time_ = DateTimeLocal();
}

bool GPlayer::IsAtTheTimeSpan( int time_minutes )
{
	assert(time_minutes >= 0);
	DateTimeLocal time_now;
	if ( !time_back_select_coach_ )
		time_back_select_coach_ = time_now;

	auto time_span = time_now - *time_back_select_coach_;
	if ( time_span.totalMinutes() >= time_minutes )
	{
		time_back_select_coach_ = time_now;
		return true;
	}
	return false;
}

bool GPlayer::SaleSomePlayersAsMinPrice( int player_cnt )
{
	if ( player_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以最低价拍卖一些球员之要拍卖的球员数量参数不正确";
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		if ( !player->CanBeSale_NoStrict() )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}

		if ( !player->SaleSelfAs_NoStrict(price_range->price_lower_, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( (int)sale_players.size() >= player_cnt )
			break;
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::SaleThePlayersAsMinPrice( int player_cnt, int player_id )
{
	if ( player_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以最低价拍卖指定的球员之要拍卖的球员数量参数不正确";
		return false;
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		//LOG_O(Log_trace) << "spid_->" << player->spid_;
		if ( !player->CanBeSale_NoStrict() )
			continue;
		if ( player->spid_ != player_id )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}

		if ( !player->SaleSelfAs_NoStrict(price_range->price_lower_, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( (int)sale_players.size() >= player_cnt )
			break;
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::SaleThePlayersAsPrice_Cnt( int player_id, int player_cnt , double price )
{
	if ( player_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以指定的价格拍卖一些指定的球员之要拍卖的球员数量参数不正确";
		return false;
	}
	if ( price <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以指定的价格拍卖一些指定的球员之价格参数不正确";
		return false;
	}

	price = stPlayer::AdjustPrice(price);

	for ( const auto& offer_item : self_offer_list_ )
	{
		if ( offer_item.spid_ == player_id && offer_item.price_ == price )
		{
			if ( --player_cnt < 1 )
				return true;
		}		
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if ( !iter )
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto player = iter->CurItem();
		if ( !player )
		{
			assert(false);
			continue;
		}
		//LOG_O(Log_trace) << "spid_->" << player->spid_;
		if ( !player->CanBeSale_NoStrict() )
			continue;
		if ( player->spid_ != player_id )
			continue;

		auto price_range = FindSellingPrice(player->spid_, player->grade_);
		if ( !price_range )
		{
			if ( !UpdateSellingPrice(player->spid_, player->grade_) )
			{
				LOG_O(Log_warning) << "无法更新球员的价格信息，spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
			price_range = FindSellingPrice(player->spid_, player->grade_);
			if ( !price_range )
			{
				LOG_O(Log_error) << "竟然查询不到球员价格范围信息,spid->" << player->spid_ << "\tgrade->" << player->grade_;
				continue;
			}
		}
		if ( !price_range->InRange(price) )
			continue;

		if ( !player->SaleSelfAs_NoStrict(price, false, false) )
			continue;
		sale_players.push_back(player->id_);
		if ( --player_cnt < 1 )
			break;
	}

	if ( !sale_players.empty() )
		return true;

	RefreshSelfOfferList();
	for ( const auto& player_id : sale_players )
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

bool GPlayer::IsJieCardTheTimeSpan( int time_minutes )
{
	using namespace GType;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return false;
	if ( norc->GetGameState() < enGameState_Managers )
		return false;
	assert(time_minutes >= 0);
	DateTimeLocal time_now;
	if ( !time_jie_card_ )
		time_jie_card_ = time_now;

	auto time_span = time_now - *time_jie_card_;
	if ( time_span.totalMinutes() >= time_minutes )
	{
		time_jie_card_ = time_now;
		return true;
	}
	return false;
}

bool GPlayer::JieCardThePlayer( int player_id, int grade, double price )
{
	stSellingPlayerInfo::ContT the_selling_players;
	if ( !UpdateSpecifySellingPlayer(player_id, the_selling_players) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "对指定的球员进行截卡之刷新球员的价格失败";
		return false;
	}
	for ( auto& it : the_selling_players )
	{
		if ( it.second.grade_ == grade )
		{
			if ( it.second.price_ <= price )
			{
				if (it.second.CanBuy())
					return it.second.Buy();
			}
		}
	}
	return false;
}

void GPlayer::Test()
{
	//UpdateSpecifySellingPlayer(88184881);
	DegradeFirstAndSecondSync();
}

bool GPlayer::BuyThePlayerAsPrice( int player_id, int grade, double price_lower, double price_upper )
{
	stSellingPlayerInfo::ContT the_selling_players;
	if ( !UpdateSpecifySellingPlayer(player_id, the_selling_players) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "对指定的球员进行截卡之刷新球员的价格失败";
		return false;
	}
	for ( auto& it : the_selling_players )
	{
		if ( it.second.grade_ == grade )
		{
			if ( it.second.price_ >= price_lower && it.second.price_ <= price_upper )
			{
				if (it.second.CanBuy())
					return it.second.Buy();
			}
		}
	}
	return false;
}

int GPlayer::UseSomeItem( int cnt )
{
	if ( cnt < 1 )
		return 0;
	const auto kMaxCnt = 50;
	cnt = std::min(cnt, kMaxCnt);
	int use_cnt = 0;
	auto items_tmp = items_;
	auto specifc_use = stItem::GetSpecificUse();
	for ( auto& item : items_tmp )
	{
		auto item_remains = item.remains_;
		while ( item_remains > 0 )
		{
			if ( !item.CanUse() )
				break;
			if ( !item.SmartUse() )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "使用物品失败2,item id->" << item.itemid_;
				break;
			}
			--item_remains;
			--cnt;
			++use_cnt;
			if ( cnt < 1 )
				break;
		}
		if ( cnt < 1 )
			break;
	}
	
	return use_cnt;
}

bool GPlayer::IsJieCardTheTime_Sec( int time_seconds )
{
	using namespace GType;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return false;
	if ( norc->GetGameState() < enGameState_Managers )
		return false;
	assert(time_seconds >= 0);
	DateTimeLocal time_now;
	if ( !time_jie_card_ )
		time_jie_card_ = time_now;

	auto time_span = time_now - *time_jie_card_;
	if ( time_span.totalSeconds() >= time_seconds )
	{
		time_jie_card_ = time_now;
		return true;
	}
	return false;
}

bool GPlayer::SetTotalPlayersCnt( const Json::Value& json_players_cnt )
{
	if ( json_players_cnt.isNull() || !json_players_cnt.isInt() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "json_players_cnt类型不是int";
		return false;
	}
	max_players_cnt_ = json_players_cnt.asInt();
	LOG_O(Log_trace) << "最大球员数量：" << max_players_cnt_;
	if ( max_players_cnt_ < 100 )
	{
		max_players_cnt_ = 100;
		assert(false);
		return false;
	}
	return true;
}

void GPlayer::EnhanceThePlayer(int player_id, int dst_grade_min, int dst_grade_max, int src_grade_min, int src_grade_max)
{
	if (players_.size() < 3)
	{
		assert(false);
		return;
	}
	SharedPlayerIter iter(new stPlayersNormalIter([player_id](const stPlayer& player){
		return player.spid_ == player_id;
	}));
	if (!iter)
	{
		assert(false);
		return;
	}
	boost::optional<stPlayer> dst_player, src_player;
	for (iter->First(); !iter->IsDone(); iter->Next())
	{
		const auto& item = iter->CurItem();
		if (!item)
		{
			assert(false);
			continue;
		}
		if (item->grade_ >= dst_grade_min && item->grade_ <= dst_grade_max || 
			item->grade_ >= src_grade_min && item->grade_ <= src_grade_max)
		{
			if (!dst_player)
				dst_player = *item;
			else if (!src_player)
				src_player = *item;
		}
		
		if (dst_player && src_player)
			break;
	}
	if (!dst_player || !src_player)
		return;
	dst_player->EnhancePlayer(*src_player);
}

int GPlayer::GetThePlayerCnt(int player_id, int grade_min, int grade_max)
{
	SharedPlayerIter iter(new stPlayersNormalIter([player_id](const stPlayer& player){
		return player.spid_ == player_id;
	}));
	if (!iter)
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (iter->First(); !iter->IsDone(); iter->Next())
	{
		const auto& item = iter->CurItem();
		if (!item)
		{
			assert(false);
			continue;
		}
		if (item->grade_ >= grade_min && item->grade_ <= grade_max)
		{
			++cnt;
		}
	}
	return cnt;
}

bool GPlayer::SaleThePlayersSpecific(int player_id, int grade_min, int grade_max, int player_cnt, double price)
{
	if (player_cnt <= 0)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以指定的价格拍卖指定的球员之要拍卖的球员数量参数不正确";
		return false;
	}
	if (price <= 0)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "以指定的价格拍卖指定的球员之价格参数不正确";
		return false;
	}

	for (const auto& offer_item : self_offer_list_)
	{
		if (offer_item.spid_ == player_id && offer_item.price_ == price)
		{
			if (offer_item.grade_ < grade_min || offer_item.grade_ > grade_max)
				continue;
			if (--player_cnt < 1)
				return true;
		}
	}

	GType::enPlayerPosition player_positions[] = {
		GType::enPlayerPosition_No
	};
	auto iter = CreatePlayerIterByPosition(player_positions, _countof(player_positions));
	if (!iter)
	{
		assert(false);
		return false;
	}

	std::vector<std::string> sale_players;
	for (iter->First(); !iter->IsDone(); iter->Next())
	{
		auto player = iter->CurItem();
		if (!player)
		{
			assert(false);
			continue;
		}
		//LOG_O(Log_trace) << "spid_->" << player->spid_;
		if (!player->CanBeSale_NoStrict())
			continue;
		if (player->spid_ != player_id)
			continue;
		if (player->grade_ < grade_min || player->grade_ > grade_max)
			continue;

		if (!player->SaleSelfAs_NoStrict(price, false, false))
			continue;
		sale_players.push_back(player->id_);
		if (--player_cnt < 1)
			break;
	}

	if (!sale_players.empty())
		return true;

	RefreshSelfOfferList();
	for (const auto& player_id : sale_players)
	{
		ErasePlayerById(player_id.c_str());
	}
	return true;
}

GPlayer::stLevelInfo::stLevelInfo()
{
	level_ = 0;
	exp_ = 0;
	to_ = 0.;
}

GPlayer::stLevelInfo::stLevelInfo( int level, int exp, int to )
{
	level_ = level;
	exp_ = exp;
	to_ = to;
}

int GPlayer::stLevelInfo::GetLevel() const
{
	return level_;
}

void GPlayer::stLevelInfo::SetLevel( int level )
{
	level_ = level;
}

int GPlayer::stLevelInfo::GetExp() const
{
	return exp_;
}

void GPlayer::stLevelInfo::SetExp( int exp )
{
	exp_ = exp;
}

double GPlayer::stLevelInfo::GetTo() const
{
	return to_;
}

void GPlayer::stLevelInfo::SetTo( double to )
{
	to_ = to;
}

bool GPlayer::stLevelInfo::ParseFromJsonObj( const Json::Value& json_level_info )
{
	if ( json_level_info.isNull() || !json_level_info.isObject() )
		return false;
	const auto& json_exp = json_level_info["exp"];
	if ( json_exp.isNull() || !json_exp.isInt() )
		return false;
	const auto& json_level = json_level_info["level"];
	if ( json_level.isNull() || !json_level.isInt() )
		return false;
	const auto& json_to = json_level_info["to"];
	if ( json_to.isNull() || !json_to.isNumeric() )
	{
		assert(false);
		return false;
	}
	exp_ = json_exp.asInt();
	level_ = json_level.asInt();
	to_ = json_to.asDouble();
	return true;
}

void GPlayer::stPlayer::SetId( int id )
{
	id_ = id;
}

const char* GPlayer::stPlayer::GetId() const
{
	return id_.c_str();
}

int GPlayer::stPlayer::GetGrade() const
{
	return grade_;
}

void GPlayer::stPlayer::SetGrade( int grade )
{
	grade_ = grade;
}

double GPlayer::stPlayer::GetPrice() const
{
	return price_;
}

void GPlayer::stPlayer::SetPrice( double price )
{
	price_ = price;
}

int GPlayer::stPlayer::GetLife() const
{
	return life_;
}

void GPlayer::stPlayer::SetLife( int life )
{
	life_ = life;
}

bool GPlayer::stPlayer::CanExchangePlayer() const
{
	if ( life_ == 0 && price_ != 0 && grade_ != 0 && player_pos_ == GType::enPlayerPosition_No )
	{
		/*auto ability = GetAbility();
		if ( kInvalidAbility == ability || ability < GPlayer::GetMe().GetAbilityFilter() )
			return true;*/
		if ( price_ < GPlayer::GetMe().GetExchPriceFilter() )
			return true;
	}
	return false;
}

void GPlayer::stPlayer::ExchangePlayer( const stPlayer& other )
{
	Json::Value msg, respond;
	auto& json_array = msg["players"];
	json_array[(UINT)0] = id_;
	json_array[1] = other.id_;
		
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/players/trade/proposal", &msg, respond) )
		return;
	if ( !CheckResFromRespond(respond) )
		return;
	static Json::Path path_money_ep(".resources.[0].path");
	const auto& json_money_ep = path_money_ep.resolve(respond);
	if ( json_money_ep.isNull() || !json_money_ep.isString() || json_money_ep.asString() != "money.ep" )
		return;
	static Json::Path path_ep(".resources.[0].body");
	const auto& json_ep = path_ep.resolve(respond);
	if ( json_ep.isNull() || !json_ep.isNumeric() )
		return;
	const auto& json_props = respond["props"];
	if ( json_props.isNull() || !json_props.isArray() || json_props.empty() )
		return;

	auto& gp = GPlayer::GetMe();
	gp.ErasePlayerById(id_.c_str(), other.id_.c_str());
	gp.SetEp(json_ep.asDouble());
	if ( !gp.ChooseExchPlayer() )
		return;

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "交换球员成功，第一个：grade->" << grade_ << "  price->" << price_ << "\t第二个:grade->" << other.grade_
		<< "  price->" << other.price_;
}

bool GPlayer::stPlayer::ParseFromJsonObj( const Json::Value& player_obj )
{
	const auto& id = player_obj["_id"];
	if ( id.isNull() || !id.isString() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& grade = player_obj["grade"];
	if ( grade.isNull() || !grade.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& price = player_obj["price"];
	if ( price.isNull() || !price.isNumeric() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& life = player_obj["life"];
	if ( life.isNull() || !life.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& state_team = player_obj["state"];
	if ( !state_team.isNull() )
	{
		if ( !state_team.isString() )
			goto LABEL_RET_ERROR;
		static const std::string kStrFirst("first");
		if ( kStrFirst == state_team.asCString() )
		{
			player_pos_ = GType::enPlayerPosition_Third;
		}
	}

	const auto& spid = player_obj["spid"];
	if ( spid.isNull() || !spid.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	id_ = id.asCString();
	grade_ = grade.asInt();
	price_ = price.asDouble();
	life_ = life.asInt();
	spid_ = spid.asInt();

	const auto& gp = GPlayer::GetMe();
	if ( price_ >= gp.GetHighPriceFilter() )
	{
		auto func_obj = FuncObj_NormalC::GetInstance();
		if (func_obj)
		{
			func_obj->GetSessionOper().ToCall(BC_MakeP("DiscoverGoodPlayer"));
		}
	}	
	return true;
LABEL_RET_ERROR:
	return false;
}

GType::enPlayerPosition GPlayer::stPlayer::GetPosition() const
{
	return player_pos_;
}

void GPlayer::stPlayer::SetPosition( GType::enPlayerPosition pos )
{
	if ( player_pos_ != pos )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "设置职位，之前：" << player_pos_ << "  之后：" << pos;
		player_pos_ = pos;
	}
}

bool GPlayer::stPlayer::CanExchangePosition( const stPlayer& other )
{
	if ( GType::enPlayerPosition_No == player_pos_ || GType::enPlayerPosition_No == other.player_pos_ )
	{
		//没有职位的球员之间不能进行交换职位
		return false;
	}
	return player_pos_ != other.player_pos_;
}

bool GPlayer::stPlayer::ExchangePostion( stPlayer& other, bool is_send )
{
	LOG_ENTER_FUNC;
	if ( !CanExchangePosition(other) )
	{
		LOG_O(Log_error) << "不能交换球员职位";
		return false;
	}
	LOG_O(Log_trace) << "交换球员的职位";
	std::swap(player_pos_, other.player_pos_);
	std::swap(position_info_, other.position_info_);

	if (is_send)
	{
		if ( !GPlayer::GetMe().SendFirstAndSecondPlayersInfo() )
		{
			std::swap(player_pos_, other.player_pos_);
			std::swap(position_info_, other.position_info_);
			return false;
		}
	}
	return true;
}

bool GPlayer::stPlayer::CanDegradeFromThirdToNo() const
{
	return GType::enPlayerPosition_Third == player_pos_;
}

bool GPlayer::stPlayer::DegradeFromThirdToNo()
{
	LOG_ENTER_FUNC;
	if ( !CanDegradeFromThirdToNo() )
	{
		LOG_O(Log_error) << "不能降级为无";
		return false;
	}
	std::string str_uri("/player/");
	str_uri += id_;
	str_uri += "/move/secondteam";
	assert(GType::enPlayerPosition_Third == player_pos_);

	Json::Value respond;
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(str_uri.c_str(), nullptr, respond) )
	{
		LOG_O(Log_error) << "降级失败";
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	player_pos_ = GType::enPlayerPosition_No;
	return true;
}

bool GPlayer::stPlayer::CanUpgradeFromNoToThird() const
{
	return GType::enPlayerPosition_No == player_pos_;
}

bool GPlayer::stPlayer::UpgradeFromNoToThird()
{
	if ( !CanUpgradeFromNoToThird() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不能升级为侯补";
		return false;
	}
	std::string str_uri("/player/");
	str_uri += id_;
	str_uri += "/move/firstteam";
	assert(GType::enPlayerPosition_No == player_pos_);
	
	Json::Value respond;
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(str_uri.c_str(), nullptr, respond) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无职位升候补失败";
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	player_pos_ = GType::enPlayerPosition_Third;
	return true;
}

bool GPlayer::stPlayer::IsFirstTeamPlayer() const
{
	return GType::enPlayerPosition_No != player_pos_;
}

bool GPlayer::stPlayer::IsSecondTeamPlayer() const
{
	return GType::enPlayerPosition_No == player_pos_;
}

void GPlayer::stPlayer::MatchEndUpdate( const stPlayer& new_player )
{
	assert(id_ == new_player.id_);
	grade_ = new_player.grade_;
	price_ = new_player.price_;
	life_ = new_player.life_;
}

bool GPlayer::stPlayer::CanBeFired() const
{
	 return 0 == grade_ && 0 == price_ && life_ < 1 && player_pos_ == GType::enPlayerPosition_No;
}

bool GPlayer::stPlayer::SendFireMsg() const
{
	Json::Value msg, respond;
	msg["_method"] = "DELETE";
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync((std::string("/player/") + id_).c_str(), &msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;
	const auto& json_resour = respond["resources"];
	if ( json_resour.isNull() || !json_resour.isArray() || json_resour.empty() )
		return false;
	const auto& json_one = json_resour[Json::UInt(0)];
	if ( json_one.isNull() || !json_one.isObject() )
		return false;
	const auto& json_one_res = json_one["resource"];
	if ( json_one_res.isNull() || !json_one_res.isString() )
		return false;
	const auto& json_body = json_one["body"];
	if ( !json_body.isNull() )
		return false;
	return true;
}

int GPlayer::stPlayer::GetAbility() const
{
	auto& players_data = stPlayersData::GetMe();
	auto it = std::find_if(players_data.players_data_.begin(), players_data.players_data_.end(),
		[=]( const stPlayersData::PlayerData& player_data ){
			return player_data.spid_ == this->GetSpid();
	});
	if ( it == players_data.players_data_.end() )
		return kInvalidAbility;
	else
		return it->player_ability_;
}

int GPlayer::stPlayer::GetSpid() const
{
	return spid_;
}

bool GPlayer::stPlayer::SaleSelfAs( double price )
{
	LOG_ENTER_FUNC;
	if ( price <= 100 )
	{
		LOG_O(Log_error) << "拍卖的价太不正确：" << price;
		return false;
	}
	if ( !CanBeSale() )
	{
		LOG_O(Log_error) << "球员不能被拍卖，其ID:" << id_ << "  职位：" << player_pos_ << "  剩余场数：" << life_;
		return false;
	}
	price = AdjustPrice(price);

	Json::Value msg;
	msg["price"] = price;
	msg["playerobjectid"] = id_;

	if ( !GClass::HttpPackageSend::Post_JsonMsgAsync("/transfer/offer", &msg) )
		return false;
	LOG_O(Log_trace) << "拍卖了球员，球员价值是：" << price_ << "  卖的价格是：" << price << "  spid:" << spid_ << " id->" << id_;
	return true;
}

bool GPlayer::stPlayer::CanBeSale() const
{
	if ( GType::enPlayerPosition_No != player_pos_ )
		return false;
	if ( life_ > 0 )
		return false;
	if ( grade_ <= 0 )
		return false;
	if ( price_ <= 0 )
		return false;
	/*auto& gp = GPlayer::GetMe();
	if ( price_ < gp.GetPriceFilter() )
		return true;*/
	return true;
}

const std::string GPlayer::stPlayer::GetPlayerName() const
{
	auto& players_data = stPlayersData::GetMe();
	auto it = std::find_if(players_data.players_data_.begin(), players_data.players_data_.end(),
		[=]( const stPlayersData::PlayerData& player_data ){
			return player_data.spid_ == this->GetSpid();
	});
	if ( it == players_data.players_data_.end() )
		return "球员数据库中找不到球员名";
	else
		return it->player_name_;
}

bool GPlayer::stPlayer::CanBeSale_NoStrict() const
{
	if ( GType::enPlayerPosition_No != player_pos_ )
		return false;
	if ( grade_ < 1 )
		return false;
	if ( price_ < 1 )
		return false;
	const auto& gp = GPlayer::GetMe();
	if ( gp.GetEp() < 500 )
		return false;
	/*if ( price_ >= gp.GetPriceFilter() )
		return false;*/
	return true;
}

bool GPlayer::stPlayer::SaleSelfAs_NoStrict( double price, bool notify_gplayer, bool notify_coor )
{
	LOG_ENTER_FUNC;
	if ( price <= 100 )
	{
		LOG_O(Log_error) << "拍卖的价太不正确：" << price;
		return false;
	}
	if ( !CanBeSale_NoStrict() )
	{
		LOG_O(Log_error) << "球员不能被拍卖，其ID:" << id_ << "  职位：" << player_pos_ << "  剩余场数：" << life_;
		return false;
	}
	price = AdjustPrice(price);

	Json::Value msg, respond;
	msg["price"] = price;
	msg["playerobjectid"] = id_;

	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/transfer/offer", &msg, respond) )
		return false;

	if ( !CheckResFromRespond(respond) )
		return false;

	static Json::Path path_body(".resources.[1].body");
	const auto& body = path_body.resolve(respond);
	if ( body.isNull() || !body.isNumeric() )
		return false;
	auto& gp = GPlayer::GetMe();
	gp.SetEp(body.asDouble());
	
	LOG_O(Log_trace) << "拍卖了球员，球员价值是：" << price_ << "  卖的价格是：" << price << "  spid:" << spid_ << " id->" << id_;

	if (notify_coor)
	{
		gp.RefreshSelfOfferList();

		auto func_obj = FuncObj_NormalC::GetInstance();
		if (func_obj)
		{
			auto sold_player = gp.FindSoldPlayer(id_);
			if ( !sold_player )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "拍卖了一个球员，竟然从我的拍卖列表中找不到,player id->" << id_;
				return false;
			}
			Json::Value json_sold_player, json_array;
			if ( !sold_player->ConvertToJson(json_sold_player) )
			{
				assert(false);
				return false;
			}
			json_array.append(json_sold_player);
			func_obj->GetCoordinate().SellPlayersSwitch(json_array);
		}
	}

	if (notify_gplayer)
	{
		gp.ErasePlayerById(id_.c_str());
	}
	return true;
}

double GPlayer::stPlayer::AdjustPrice( double price )
{
	//必须是100的整数
	price /= 100;
	price *= 100;
	return price;
}

bool GPlayer::stPlayer::IsInPos( GType::enPlayerPosition player_position[], int position_cnt ) const
{
	assert(player_position && position_cnt >= 1);
	for ( auto i = 0; i < position_cnt; ++i )
	{
		if ( player_position[i] == player_pos_ )
			return true;
	}
	return false;
}

bool GPlayer::stPlayer::ExchangePlayers( const PlayerContT& players )
{
	if ( players.size() < 2 )
	{
		//assert(false);
		return false;
	}
	Json::Value msg, respond;
	auto& json_array = msg["players"];
	for ( const auto& player : players )
	{
		json_array.append(player.id_);
	}

	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/players/trade/proposal", &msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_money_ep(".resources.[0].path");
	const auto& json_money_ep = path_money_ep.resolve(respond);
	if ( json_money_ep.isNull() || !json_money_ep.isString() || json_money_ep.asString() != "money.ep" )
		return false;
	static Json::Path path_ep(".resources.[0].body");
	const auto& json_ep = path_ep.resolve(respond);
	if ( json_ep.isNull() || !json_ep.isNumeric() )
		return false;
	const auto& json_props = respond["props"];
	if ( json_props.isNull() || !json_props.isArray() || json_props.empty() )
		return false;

	auto& gp = GPlayer::GetMe();
	
	for ( const auto& player : players )
	{
		gp.ErasePlayerById(player.id_.c_str());
	}
	gp.SetEp(json_ep.asDouble());
	if ( !gp.ChooseExchPlayer() )
		return false;

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "交换球员成功，数量->" << players.size();
	return true;
}

void GPlayer::stPlayer::EnhancePlayer(const stPlayer& other)
{
	Json::Value msg, respond;
	msg["_method"] = "PUT";
	CStringA msg_uri;
	msg_uri.Format("/player/%s/enhance/%s", id_.c_str(), other.id_.c_str());
	if (!GClass::HttpPackageSend::Post_JsonMsgSync(msg_uri, &msg, respond))
		return;
	if (!CheckResFromRespond(respond))
		return;
	GPlayer::GetMe().RefreshCoachMeImpl(true);
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "强化球员成功，第一个：grade->" << grade_ << "  price->" << price_ << "\t第二个:grade->" << other.grade_
		<< "  price->" << other.price_;
}

bool GPlayer::stItem::Use()
{
	LOG_ENTER_FUNC;
	if ( !CanUse() )
	{
		LOG_O(Log_error) << "id:" << id_ << "   remains:" << remains_ << "   expired:" << expired_ << "  itemid_:" << itemid_ << "的物品不能被使用";
		return false;
	}

	Json::Value msg, respond;
	msg["id"] = id_;
	
	CStringA str_uri;
	str_uri.Format("/item/%s/use", id_.c_str());
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(str_uri, &msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
	{
		LOG_O(Log_error) << "使用物品，服务器返回失败";
		return false;
	}

	LOG_O(Log_trace) << "使用物品之物品数量减1, item_id:" << this->itemid_ << "原来的数量是：" << this->remains_;
	//--remains_;

	//更新物品信息
	static Json::Path path_items(".resources.[1].body");
	const auto& body_items = path_items.resolve(respond);
	if ( body_items.isNull() || !body_items.isArray() )
	{
		LOG_O(Log_error) << "使用物品返回的包中，找不到物品信息";
		return false;
	}
	auto& gp = GPlayer::GetMe();

	const auto& json_player = respond["player"];
	if ( !json_player.isNull() && json_player.isObject() )
	{
		stPlayer new_player;
		if ( !new_player.ParseFromJsonObj(json_player) )
		{
			LOG_O(Log_error) << "使用物品返回的包中，找不到新的球员的信息";
			return false;
		}
		gp.GetAllPlayers().push_back(new_player);
	}

	const auto& json_ep = respond["ep"];
	if ( !json_ep.isNull() && json_ep.isNumeric() )
	{
		gp.SetEp(gp.GetEp() + json_ep.asDouble());
	}

	gp.SetBagItems(body_items);
	return true;
}

bool GPlayer::stItem::CanUse() const
{
	if ( id_.empty() || remains_ <= 0 || expired_ )
	{
		return false;
	}
	else
	{
		return true;
	}
}

const char* GPlayer::stItem::GetId() const
{
	return id_.c_str();
}

void GPlayer::stItem::SetId( const char* id )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置ID，之前：" << id_ << "\t之后：" << id;
	id_ = id;
}

int GPlayer::stItem::GetItemId() const
{
	return itemid_;
}

void GPlayer::stItem::SetItemId( int item_id )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置物品ID，之前：" << itemid_ << "\t之后：" << item_id;
	itemid_ = item_id;
}

int GPlayer::stItem::GetRemains() const
{
	return remains_;
}

void GPlayer::stItem::SetRemains( int remains )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置remains，之前：" << remains_ << "\t之后：" << remains;
	remains_ = remains;
}

bool GPlayer::stItem::GetExpired() const
{
	return expired_;
}

void GPlayer::stItem::SetExpired( bool expired )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置expired，之前：" << expired_ << "\t之后：" << expired;
	expired_ = expired;
}

int GPlayer::stItem::ItemId_BlueCard()
{
	return 150004;
}

int GPlayer::stItem::ItemId_GreenCard()
{
	return 150005;
}

int GPlayer::stItem::ItemId_RedCard()
{
	return 150006;
}

int GPlayer::stItem::ItemId_PurpleCard()
{
	return 150007;
}

int GPlayer::stItem::ItemId_EpCard()
{
	return 501;
}

int GPlayer::stItem::ItemId_EpCardNew()
{
	return 509;
}

int GPlayer::stItem::ItemId_ChangeCard()
{
	return 3001;
}

int GPlayer::stItem::ItemId_AutoMatchTicket()
{
	return 2001;
}

bool GPlayer::stItem::ParseFromJsonObj( const Json::Value& item_obj )
{
	const auto& id = item_obj["id"];
	if ( id.isNull() || !id.isString() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& item_id = item_obj["itemid"];
	if ( item_id.isNull() || !item_id.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& remains = item_obj["remains"];
	if ( !remains.isNull() && remains.isInt() )
	{
		remains_ = remains.asInt();
	}
	else
		remains_ = 1;

	const auto& expired = item_obj["expired"];
	if ( expired.isNull() || !expired.isBool() )
	{
		goto LABEL_RET_ERROR;
	}

	id_ = id.asCString();
	itemid_ = item_id.asInt();
	expired_ = expired.asBool();

	return true;
LABEL_RET_ERROR:
	return false;
}

int GPlayer::stItem::ItemId_BlueBallCard()
{
	return 150004;
}

int GPlayer::stItem::ItemId_GreenBallCard()
{
	return 150005;
}

int GPlayer::stItem::ItemId_RedBallCard()
{
	return 150006;
}

int GPlayer::stItem::ItemId_13ForwardCard()
{
	return 20047;
}

int GPlayer::stItem::ItemId_13MidCard()
{
	return 20048;
}

int GPlayer::stItem::ItemId_13BackCard()
{
	return 20049;
}

int GPlayer::stItem::ItemId_13GateCard()
{
	return 20050;
}

int GPlayer::stItem::ItemId_32CountryCard()
{
	return 82214105;
}

int GPlayer::stItem::ItemId_OrangeCard()
{
	return 150007;
}

int GPlayer::stItem::ItemId_WCard()
{
	return 82214100;
}

bool GPlayer::stItem::OpenTheSpecificCard200( Json::Value& msg, const stUseHelper& use_hlp )
{
	static const Json::Int kSpids[] = {
		88020801,88173731,88041236,88020289,88195864,88179844,88158023,88176580,88167495,88164240,88197445,88190871,88191202,88155862,88200104,88165153,88201535,88189596,88188350,88198710,88183277,88183907,88176676,88192119,88009014,88178603,88177003,88182521,88153079,88183898,88184943,88184432,88164169,88192505,88188545,88204963,88171833,88164468,88178372,88202126,88176944,88193080,88199304,88205988,88121939,88053302,88181872,88139720,88194765,88156616,88120533,88189332,88191910,88054050,88177509,88162895,88201862,88179944,88172871,88121944,88143001,88189511,88202556,88188942,88173771,88180403,88152729,88204311,88192318,88165229,88112316,88184941,88197781,88174543,88192985,88196144,88179527,88189362,88190460,88192448,88190149,88188567,88168651,88179813,88170890,88177610,88189963,88202652,88180283,88193082,88193301,88176635,88188152,88193747,88007826,88179846,88146954,88190531,88183711,88167664,88045197,88178224,88181783,88177358,88201399,88177413,88182493,88163631,88205069,88184111,88171688,88167397,88158625,88177458,88202515,88186627,88121170,88186351,88138956,88172879,88186942,88158626,88000041,88208330,88192387,88193130,88186561,88166706,88142784,88146741,88190815,88189509,88168542,88193352,88203747,88202166,88211176,88180216,88159065,88148803,88153244,88201153,88211110,88200949,88191740,88199074,88200529,88178088,88178509,88199189,88164477,88207865,88190584,88048940,88170783,88180930,88194644,88164859,88146530,88053612,88193348,88183556,88049369,88162835,88186581,88024630,88197853,88188377,88192883,88187607,88206058,88191043,88005479,88205566,88199556,88201455,88142754,88150724,88189250,88181458,88135475,88200215,88186146,88200724,88203485,88192620,88013732,88193041,88188428,88199835,88220253,88031432,88191180,88168435,88207566,88216736,88173221,88168609,88190490,88010535
	};
	static Json::Value json_spids;
	if (json_spids.isNull())
	{
		for ( auto i = 0; i != _countof(kSpids); ++i )
		{
			json_spids.append(kSpids[i]);
		}
	}
	msg["spids"] = json_spids;
	return true;
}

bool GPlayer::stItem::UseEx( const stUseHelper& use_hlp )
{
	LOG_ENTER_FUNC;
	if ( !CanUse() )
	{
		LOG_O(Log_error) << "id:" << id_ << "   remains:" << remains_ << "   expired:" << expired_ << "  itemid_:" << itemid_ << "的物品不能被使用";
		return false;
	}

	Json::Value msg, respond;
	msg["id"] = id_;

	CStringA str_uri;
	str_uri.Format("/item/%s/use", id_.c_str());
	if (use_hlp.msg_param_handler_)
	{
		if ( !use_hlp.msg_param_handler_(msg, use_hlp) )
		{
			LOG_O(Log_error) << "id:" << id_ << "\tmsg_param_handler_返回false";
			assert(false);
			return false;
		}
	}
	if (use_hlp.is_encrypt_)
	{
		if ( !GClass::HttpPackageSend::Post_EncryptJsonMsgSync(str_uri, msg, respond) )
			return false;
	} 
	else
	{
		if ( !GClass::HttpPackageSend::Post_JsonMsgSync(str_uri, &msg, respond) )
			return false;
	}
	
	if ( !CheckResFromRespond(respond) )
	{
		LOG_O(Log_error) << "使用物品，服务器返回失败";
		return false;
	}

	LOG_O(Log_trace) << "使用物品之物品数量减1, item_id:" << this->itemid_ << "原来的数量是：" << this->remains_;
	//--remains_;

	//更新物品信息
	static Json::Path path_items(".resources.[1].body");
	const auto& body_items = path_items.resolve(respond);
	if ( body_items.isNull() || !body_items.isArray() )
	{
		LOG_O(Log_error) << "使用物品返回的包中，找不到物品信息";
		return false;
	}
	auto& gp = GPlayer::GetMe();

	const auto& json_player = respond["player"];
	if ( !json_player.isNull() && json_player.isObject() )
	{
		stPlayer new_player;
		if ( !new_player.ParseFromJsonObj(json_player) )
		{
			LOG_O(Log_error) << "使用物品返回的包中，找不到新的球员的信息";
			return false;
		}
		gp.GetAllPlayers().push_back(new_player);
	}

	const auto& json_ep = respond["ep"];
	if ( !json_ep.isNull() && json_ep.isNumeric() )
	{
		gp.SetEp(gp.GetEp() + json_ep.asDouble());
	}

	gp.SetBagItems(body_items);
	return true;
}

bool GPlayer::stItem::OpenTheSpecificCard100( Json::Value& msg, const stUseHelper& use_hlp )
{
	static const Json::Int kSpids[] = {
		88020801,88173731,88041236,88020289,88195864,88179844,88158023,88176580,88167495,88164240,88197445,88190871,88155862,88191202,88165153,88201535,88200104,88188350,88189596,88198710,88176676,88183277,88183907,88192119,88009014,88178603,88182521,88183898,88177003,88153079,88164169,88184432,88188545,88184943,88204963,88164468,88178372,88171833,88176944,88192505,88193080,88202126,88120533,88205988,88121939,88181872,88121944,88189332,88156616,88139720,88053302,88054050,88199304,88177509,88201862,88172871,88162895,88179944,88173771,88143001,88165229,88197781,88191910,88202556,88112316,88174543,88196144,88192318,88179527,88193301,88189511,88180403,88194765,88192448,88190149,88184941,88192985,88188942,88152729,88168651,88190460,88204311,88007826,88178224,88188567,88189362,88176635,88193082,88177610,88189963,88179813,88170890,88179846,88193747,88183711,88171688,88202652,88158626,88180283,88188152
	};
	static Json::Value json_spids;
	if (json_spids.isNull())
	{
		for ( auto i = 0; i != _countof(kSpids); ++i )
		{
			json_spids.append(kSpids[i]);
		}
	}
	msg["spids"] = json_spids;
	return true;
}

std::pair<const GPlayer::stItem::stUseHelper*, int> GPlayer::stItem::GetSpecificUse()
{
	static const stItem::stUseHelper kItemIdsEx[] = {
		{ 200003, stItem::OpenTheSpecificCard100, false },
		{ 200004, stItem::OpenTheSpecificCard200, true },
		//{ 200005, stItem::OpenTheSpecificCard200, true },
	};
	std::pair<const GPlayer::stItem::stUseHelper*, int> res;
	res.first = kItemIdsEx;
	res.second = _countof(kItemIdsEx);
	return res;
}

bool GPlayer::stItem::SmartUse()
{
	auto specific_use = GetSpecificUse();
	if ( specific_use.first && specific_use.second > 0 )
	{
		for ( int i = 0; i != specific_use.second; ++i )
		{
			if ( specific_use.first[i].item_id_ == itemid_ )
			{
				return UseEx(specific_use.first[i]);
			}
		}
	}
	return Use();
}

bool GPlayer::stPlayer::PositionInfo::ParseFromJsonObj( const Json::Value& pos_info )
{
	const auto& pos = pos_info["pos"];
	if ( pos.isNull() || !pos.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& starting = pos_info["starting"];
	if ( !starting.isNull() && !starting.empty() )
	{
		starting_ = starting;
	}

	pos_ = pos.asInt();

	return true;
LABEL_RET_ERROR:
	return false;
}

void GPlayer::stPlayersNormalIter::First()
{
	it_ = GPlayer::GetMe().GetAllPlayers().begin();
	NextValid();
}

void GPlayer::stPlayersNormalIter::Next()
{
	++it_;
	NextValid();
}

bool GPlayer::stPlayersNormalIter::IsDone() const
{
	return it_ == GPlayer::GetMe().GetAllPlayers().end();
}

GPlayer::stPlayersNormalIter::value_type 
GPlayer::stPlayersNormalIter::CurItem() const
{
	assert(it_ != GPlayer::GetMe().GetAllPlayers().end());
	return &*it_;
}

void GPlayer::stPlayersNormalIter::NextValid()
{
	auto it_end = GPlayer::GetMe().GetAllPlayers().end();
	while ( it_ != it_end )
	{
		if (filter_fn_(*it_))
		{
			break;
		}
		++it_;
	}
}

GPlayer::Achievement::Achievement()
{
	grade_ = 0;
	available_ = false;
}

bool GPlayer::Achievement::ParseFromJsonObj( const Json::Value& achievement_item )
{
	const auto& progress = achievement_item["progress"];
	if ( progress.isNull() || !progress.isObject() )
		goto LABEL_RET_ERROR;

	const auto& grade = progress["grade"];
	if ( grade.isNull() || !grade.isInt() )
		goto LABEL_RET_ERROR;

	const auto& rewards = achievement_item["rewards"];
	if ( rewards.isNull() || !rewards.isObject() )
		goto LABEL_RET_ERROR;

	const auto& available = rewards["available"];
	if ( available.isNull() || !available.isBool() )
		goto LABEL_RET_ERROR;

	const auto& id = achievement_item["id"];
	if ( id.isNull() || !id.isString() )
		goto LABEL_RET_ERROR;

	id_ = id.asCString();
	grade_ = grade.asInt();
	available_ = available.asBool();

	return true;
LABEL_RET_ERROR:
	return false;
}

bool GPlayer::Achievement::CanBeCollect() const
{
	return available_;
}

bool GPlayer::Achievement::Collect()
{
	LOG_ENTER_FUNC;
	if ( !CanBeCollect() )
	{
		LOG_O(Log_error) << "不能领取此项成就，id:" << id_ << "\tgrade:" << grade_ << "\tavailable:" << available_;
		return false;
	}

	CStringA uri;
	uri.Format("/achievement/%s/reward/%d", id_.c_str(), grade_);
	if ( !GClass::HttpPackageSend::Post_JsonMsgAsync(uri, nullptr) )
	{
		LOG_O(Log_warning) << "领取成就时，发包失败，id:" << id_ << "\tgrade:" << grade_ << "\tavailable:" << available_;
		return false;
	}
	
	LOG_O(Log_trace) << "领取了成就，id:" << id_ << "\tgrade:" << grade_ << "\tavailable:" << available_;
	return true;
}

GPlayer::GiftPackage::GiftPackage()
{
	shop_id_ = 0;
	product_id_ = 0;
	price_ = 0;
}

bool GPlayer::GiftPackage::ParseFromJsonObj( const Json::Value& achievement_item )
{
	const auto& shop_id = achievement_item["shopid"];
	if ( shop_id.isNull() || !shop_id.isInt() )
		return false;

	const auto& product_id = achievement_item["productid"];
	if ( product_id.isNull() || !product_id.isInt() )
		return false;

	static const Json::Path json_path(".product.staticdata.bunches.[0].price");
	const auto& price = json_path.resolve(achievement_item);
	if ( price.isNull() || !price.isNumeric() )
		return false;

	shop_id_ = shop_id.asInt();
	product_id_ = product_id.asInt();
	price_ = price.asDouble();
	return true;	
}

bool GPlayer::GiftPackage::CanBuy() const
{
	return price_ > 0 && price_ <= GPlayer::GetMe().GetGp();
}

bool GPlayer::GiftPackage::Buy()
{
	LOG_ENTER_FUNC;
	if ( !CanBuy() )
	{
		LOG_O(Log_error) << "买礼包失败, shop_id:" << shop_id_ << "\tproduct_id:" << product_id_ << "\tprice:" << price_;
		return false;
	}

	Json::Value msg, respond;
	msg["productid"] = product_id_;
	msg["shopid"] = shop_id_;
	msg["bunch"] = (int)0;
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync("/shop/purchase", &msg, respond) )
	{
		LOG_O(Log_warning) << "买GP礼包，发包失败，shop_id:" << shop_id_ << "\tproduct_id:" << product_id_ << "\tprice:" << price_;
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_ep(".resources.[0].body.ep");
	static Json::Path path_items(".resources.[1].body");
	static Json::Path path_gpdata(".resources.[2].body.point");
	const auto& json_ep = path_ep.resolve(respond);
	if ( json_ep.isNull() || !json_ep.isNumeric() )
		return false;
	const auto& json_items = path_items.resolve(respond);
	if ( json_items.isNull() || !json_items.isArray() )
		return false;
	const auto& json_gp = path_gpdata.resolve(respond);
	if ( json_gp.isNull() || !json_gp.isInt() )
		return false;
	LOG_O(Log_trace) << "买GP礼包成功，shop_id:" << shop_id_ << "\tproduct_id:" << product_id_ << "\tprice:" << price_;
	auto& gplayer = GPlayer::GetMe();
	
	gplayer.SetEp(json_ep.asDouble());
	gplayer.SetGp(json_gp.asInt());
	gplayer.SetBagItems(json_items);
	return true;
}

bool GPlayer::stSelfOfferItem::ParseFromJsonObj( const Json::Value& sold_player )
{
	if ( !__super::ParseFromJsonObj(sold_player) )
		return false;

	const auto& player_id = sold_player["playerid"];
	if ( player_id.isNull() || !player_id.isString() )
		return false;

	const auto& state = sold_player["status"];
	if ( state.isNull() || !state.isString() )
		return false;
	const auto& str_state = state.asString();
	if ( str_state == "sold" )
		sold_state_ = kSS_Sold;
	else if ( str_state == "waiting" )
		sold_state_ = kSS_Waiting;
	else if ( str_state == "expired" )
		sold_state_ = kSS_Expired;
	else 
	{
		assert(false);
		return false;
	}

	player_id_ = player_id.asCString();
	return true;
}

bool GPlayer::stSelfOfferItem::CancelExpired( bool notify_gplayer )
{
	if ( kSS_Expired != sold_state_ )
		return false;
	CStringA uri;
	uri.Format("/transfer/my/offer/expired/%s", id_.c_str());

	Json::Value msg, respond;
	msg["_method"] = "DELETE";
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(uri, &msg, respond) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "取消拍卖过程的球员失败, id->" << id_ << "\tplayer id->" << player_id_;
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_body(".resources.[0].body");
	const auto& body = path_body.resolve(respond);
	if ( body.isNull() || !body.isObject() )
	{
		assert(false);
		return false;
	}
	stPlayer new_player;
	if ( !new_player.ParseFromJsonObj(body) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "解析球员失败";
		return false;
	}
	auto& gp = GPlayer::GetMe();
	gp.GetAllPlayers().push_back(new_player);

	//广播消息
	auto func_obj = FuncObj_NormalC::GetInstance();
	if (func_obj)
	{
		Json::Value sold_player;
		if (ConvertToJson(sold_player))
		{
			Json::Value json_array;
			json_array.append(sold_player);
			func_obj->GetCoordinate().CancelSale_OtherSide(json_array);
		}
	}

	if ( !notify_gplayer )
		return true;
	gp.CancelSale(id_);
	return true;
}

bool GPlayer::stSelfOfferItem::SoldDone( bool notify_gplayer )
{
	if ( kSS_Sold != sold_state_ )
		return false;
	CStringA uri;
	uri.Format("/transfer/my/offer/completed/%s", id_.c_str());

	Json::Value msg, respond;
	msg["_method"] = "DELETE";
	if ( !GClass::HttpPackageSend::Post_JsonMsgSync(uri, &msg, respond) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获得拍卖成功的球员的EP失败, id->" << id_ << "\tplayer id->" << player_id_;
		return false;
	}
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_money(".resources.[0].path");
	const auto& money_ep = path_money.resolve(respond);
	if ( money_ep.isNull() || !money_ep.isString() || money_ep.asString() != "money.ep" )
	{
		//很可能跟游戏中的发包产生冲突了
		return false;
	}

	static Json::Path path_body(".resources.[0].body");
	const auto& body = path_body.resolve(respond);
	if ( body.isNull() || !body.isNumeric() )
	{
		assert(false);
		return false;
	}

	auto& gp = GPlayer::GetMe();
	gp.SetEp(body.asDouble());

	//广播消息
	auto func_obj = FuncObj_NormalC::GetInstance();
	if (func_obj)
	{
		Json::Value sold_player;
		if (ConvertToJson(sold_player))
		{
			Json::Value json_array;
			json_array.append(sold_player);
			func_obj->GetCoordinate().CancelSale_OtherSide(json_array);
		}
	}

	if ( !notify_gplayer )
		return true;
	gp.CancelSale(id_);
	return true;
}

bool GPlayer::stSelfOfferItem::ConvertToJson( Json::Value& sold_player ) const
{
	if ( !__super::ConvertToJson(sold_player) )
		return false;
	sold_player["playerid"] = player_id_;
	Json::Value status;
	const char* kStrStatus = "";
	if ( kSS_Waiting == sold_state_ )
		kStrStatus = "waiting";
	else if ( kSS_Sold == sold_state_ )
		kStrStatus = "sold";
	else if ( kSS_Expired == sold_state_ )
		kStrStatus = "expired";
	status = kStrStatus;
	sold_player["status"] = status;
	return true;
}

bool GPlayer::stSelfOfferItem::IsWaitSaleTimeTooLong() const
{
	if ( kSS_Waiting != sold_state_ )
		return false;
	Poco::DateTime time_now;
	auto time_span = time_now - issued_at_;
	const auto& gp = GPlayer::GetMe();

	return time_span.totalHours() >= gp.GetTimeWaitSalePlayer();
}

bool GPlayer::stSelfOfferItem::CancelSale_Waiting( bool notify_gplayer )
{
	using namespace GClass;
	Json::Value msg, respond;
	CStringA str_uri;
	str_uri.Format("/transfer/my/offer/waiting/%s", id_.c_str());
	msg["_method"] = "DELETE";
	if ( !HttpPackageSend::Post_JsonMsgSync(str_uri, &msg, respond) )
		return false;
	if ( !CheckResFromRespond(respond) )
		return false;
	static Json::Path path_body(".resources.[0].body");
	const auto& json_body = path_body.resolve(respond);
	if ( json_body.isNull() || !json_body.isObject() )
		return false;
	stPlayer new_player;
	if ( !new_player.ParseFromJsonObj(json_body) )
		return false;
	auto& gp = GPlayer::GetMe();
	gp.GetAllPlayers().push_back(new_player);
	
	//广播消息
	auto func_obj = FuncObj_NormalC::GetInstance();
	if (func_obj)
	{
		Json::Value sold_player;
		if (ConvertToJson(sold_player))
		{
			Json::Value json_array;
			json_array.append(sold_player);
			func_obj->GetCoordinate().CancelSale_OtherSide(json_array);
		}
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "取消拍卖球员成功";
	if ( !notify_gplayer )
		return true;
	gp.CancelSale(id_);
	return true;
}

bool GPlayer::stOfferItem::ParseFromJsonObj( const Json::Value& sold_player )
{
	const auto& id = sold_player["_id"];
	if ( id.isNull() || !id.isString() )
		return false;
	const auto& price = sold_player["price"];
	if ( price.isNull() || !price.isNumeric() )
		return false;

	const auto& spid = sold_player["spid"];
	if ( spid.isNull() || !spid.isInt() )
		return false;

	const auto& issued_at = sold_player["issuedat"];
	if ( issued_at.isNull() || !issued_at.isString() )
		return false;

	const auto& grade = sold_player["grade"];
	if ( grade.isNull() || !grade.isInt() )
		return false;

	id_ = id.asCString();
	int time_zone = 0;
	issued_at_ = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, issued_at.asString(), time_zone);
	spid_ = spid.asInt();
	price_ = price.asDouble();
	grade_ = grade.asInt();
	return true;	
}

bool GPlayer::stOfferItem::ConvertToJson( Json::Value& sold_player ) const
{
	sold_player["_id"] = id_;
	sold_player["price"] = price_;
	sold_player["spid"] = spid_;
	sold_player["issuedat"] = Poco::DateTimeFormatter::format(issued_at_, Poco::DateTimeFormat::ISO8601_FORMAT);
	sold_player["grade"] = grade_;
	return true;
}

bool GPlayer::stOfferItem::CanBuy() const
{
	return GPlayer::CanBuy(price_);
}

bool GPlayer::stOfferItem::Buy( bool notify_coor ) const
{
	if ( !GPlayer::Buy(spid_, grade_, price_) )
		return false;

	if (notify_coor)
	{
		/*//购买的球员并不一定是本局域网出售的，所以先去掉这些，防错而已
		auto func_obj = FuncObj_NormalC::GetInstance();
		if (func_obj)
		{
			Json::Value sold_player;
			if (ConvertToJson(sold_player))
			{
				Json::Value json_array;
				json_array.append(sold_player);
				func_obj->GetCoordinate().CancelSale_SelfSide(json_array);
			}
		}*/
	}
	return true;
}

bool CheckResFromRespond( const Json::Value& respond_msg )
{
	if ( respond_msg.isNull() || !respond_msg.isObject() )
		return false;
	const auto& res = respond_msg["result"];
	if ( res.isNull() || !res.isBool() )
		return false;
	return res.asBool();
}

GPlayer::stSellingPlayerInfo::stSellingPlayerInfo()
{
	cnt_ = 0;
	grade_ = 0;
	price_ = 0;
	spid_ = 0;
	price_lower_ = 0;
	price_upper_ = 0;
	price_default_ = 0;
}

bool GPlayer::stSellingPlayerInfo::ParseFromJsonObj( const Json::Value& selling_player )
{
	const auto& id = selling_player["_id"];
	if ( id.isNull() || !id.isString() )
		return false;
	const auto& count = selling_player["count"];
	if ( count.isNull() || !count.isInt() )
		return false;
	const auto& grade = selling_player["grade"];
	if ( grade.isNull() || !grade.isInt() )
		return false;
	const auto& oldest_date = selling_player["oldestDate"];
	if ( oldest_date.isNull() || !oldest_date.isString() )
		return false;
	const auto& price = selling_player["price"];
	if ( price.isNull() || !price.isNumeric() )
		return false;
	const auto& spid = selling_player["spid"];
	if ( spid.isNull() || !spid.isInt() )
		return false;
	const auto& limit = selling_player["limit"];
	if ( limit.isNull() || !limit.isObject() )
		return false;
	const auto& lower = limit["lower"];
	if ( lower.isNull() || !lower.isNumeric() )
		return false;
	const auto& upper = limit["upper"];
	if ( upper.isNull() || !upper.isNumeric() )
		return false;
	const auto& dlt = limit["default"];
	if ( dlt.isNull() || !dlt.isNumeric() )
		return false;

	id_ = id.asString();
	cnt_ = count.asInt();
	grade_ = grade.asInt();
	int time_zone = 0;
	oldest_date_ = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, oldest_date.asString(), time_zone);
	price_ = price.asDouble();
	spid_ = spid.asInt();
	price_lower_ = lower.asDouble();
	price_upper_ = upper.asDouble();
	price_default_ = dlt.asDouble();
	return true;
}

bool GPlayer::stSellingPlayerInfo::Buy() const
{
	return GPlayer::Buy(spid_, grade_, price_);
}

bool GPlayer::stSellingPlayerInfo::CanBuy() const
{
	return GPlayer::CanBuy(price_);
}

GPlayer::stSellingPriceRange::stSellingPriceRange()
{
	price_lower_ = 0;
	price_upper_ = 0;
	price_default_ = 0;
	reg_fee_ = 0;
}

bool GPlayer::stSellingPriceRange::ParseFromJsonObj( const Json::Value& selling_price )
{
	const auto& lower = selling_price["lower"];
	if ( lower.isNull() || !lower.isNumeric() )
		return false;
	const auto& upper = selling_price["upper"];
	if ( upper.isNull() || !upper.isNumeric() )
		return false;
	const auto& dlt = selling_price["default"];
	if ( dlt.isNull() || !dlt.isNumeric() )
		return false;
	const auto& reg_fee = selling_price["registrationFee"];
	if ( reg_fee.isNull() || !reg_fee.isInt() )
		return false;

	price_lower_ = lower.asDouble();
	price_upper_ = upper.asDouble();
	price_default_ = dlt.asDouble();
	reg_fee_ = reg_fee.asInt();
	return true;
}

bool GPlayer::stSellingPriceRange::NeedUpdate() const
{
	Poco::DateTime time_now;
	auto time_span = time_now - lastest_time_;
	//5分钟后才允许刷新
	return time_span.totalMinutes() >= 5;
}

bool GPlayer::stSellingPriceRange::Update( int spid, int grade )
{
	Json::Value respond;
	CStringA uri;
	uri.Format("/transfer/price/%u/%u", spid, grade);
	if ( !GClass::HttpPackageSend::Get_JsonMsgSync(uri, respond) )
		return false;
	
	LOG_ENTER_FUNC;
	if ( !ParseFromJsonObj(respond) )
	{
		LOG_O(Log_error) << "更新价格信息失败，spid->" << spid << "\t grade->" << grade;
		return false;
	}
	price_upper_ = std::min(price_lower_ * 9, price_upper_);

	LOG_O(Log_trace) << "更新拍卖价格范围成功, spid->" << spid << "\tgrade->" << grade;

	//更新时间
	lastest_time_ = Poco::DateTime();
	return true;
}

bool GPlayer::stSellingPriceRange::InRange( double price ) const
{
	return price >= price_lower_ && price <= price_upper_;
}

double GPlayer::stSellingPriceRange::GetPriceLower() const
{
	return price_lower_;
}

void GPlayer::stSellingPriceRange::SetPriceLower( double price_lower )
{
	price_lower_ = price_lower;
}

double GPlayer::stSellingPriceRange::GetPriceUpper() const
{
	return price_upper_;
}

void GPlayer::stSellingPriceRange::SetPriceUpper( double price_upper )
{
	price_upper_ = price_upper;
}

double GPlayer::stSellingPriceRange::GetPriceDefault() const
{
	return price_default_;
}

void GPlayer::stSellingPriceRange::SetPriceDefault( double price_default )
{
	price_default_ = price_default;
}

/*
bool operator < ( const GPlayer::stOfferItem& lhs, const GPlayer::stOfferItem& rhs )
{
	if ( lhs.spid_ < rhs.spid_ )
		return true;
	if ( lhs.grade_ < rhs.grade_ )
		return true;
	if ( lhs.price_ < rhs.price_ )
		return true;
	if ( lhs.issued_at_ < rhs.issued_at_ )
		return true;
	return false;
}*/

bool operator == ( const GPlayer::stOfferItem& lhs, const GPlayer::stOfferItem& rhs ){
	return lhs.spid_ == rhs.spid_ && lhs.grade_ == rhs.grade_ &&
		lhs.price_ == rhs.price_ && lhs.issued_at_ == rhs.issued_at_
		&& lhs.id_ == rhs.id_;
}