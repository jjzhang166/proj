#include "stdafx.h"
#include "GProtocolS.h"
#include "GFuncCall.h"
#include "GAddr.h"
#include "GConst.h"
#include "GProtocolR.h"
#include "GOffset.h"

using namespace GFuncCall;
typedef CMDR_HeaderBase CMDS_HeaderBase;
static_assert(sizeof(CMDS_HeaderBase) == sizeof(PACKET_HEADER), "");

CMDS_Msg_PickUpItem::CMDS_Msg_PickUpItem()
{
	item_serial_ = 0;
}

struct stCmd_PickUpItem : CMDS_HeaderBase{
	DWORD	str_dwSerial;

	stCmd_PickUpItem( const CMDS_Msg_PickUpItem& proxy_msg ){
		str_dwSerial = proxy_msg.item_serial_;
	}
};
static_assert(sizeof(stCmd_PickUpItem) == sizeof(SEND_PICK_UP_ITEM), "");
void CMDS_Msg_PickUpItem::Write( StrOutputBStream& ostm ) const
{
	//static auto uri_hash = CalcHash_WORD(L"Apple::Protocol::ToServer::InventorySystem::PickUpItem");
	//static auto uri_hash = CalcHash_WORD(L"76883bc7e7b32d16bdf424a2071c1ee8");
	ostm << item_serial_;
}

struct stCmd_SkillHitHeader : CMDS_HeaderBase{
	WORD	wRequestHitInfos;
	WORD	warray;
	DWORD	dwsize;
};
static_assert(sizeof(stCmd_SkillHitHeader) == sizeof(SEND_SKILL_HIT_HEADER), "");

void CMDS_Msg_SkillHit::Write( StrOutputBStream& ostm ) const
{
	//static auto uri_hash = CalcHash_WORD(L"Apple::Protocol::ToServer::ActionSystem::RequestProjectileHit");
	//static auto uri_hash = CalcHash_WORD(L"c492f0d9b91babcb562db514f3a9a86b");
	static auto hash_request_hit_infos = CalcHash_WORD(L"RequestHitInfos");
	static_assert(sizeof(decltype(hitted_items_.Size())) == sizeof(DWORD), "size error");
	ostm << hash_request_hit_infos << hitted_items_;
}

struct stCmd_SkillHitBody{
	WORD	wItem_i;
	WORD	wHitInfo;
	DWORD	dwProjectileSerial;
	DWORD	dwDefenderSerial;
	DWORD	dwAttackerSerial;
	WORD	wHitResultLen;
	std::wstring	strHitResult;
	DWORD	dwHitType;
	DWORD	dwDeltaHp;
	DWORD	dwRandomSeed;
	WORD	wHitProjectileInfo;
	WORD	wAppleHitProjectileInfo;
	FLOAT	fAttackPowerRate;
	FLOAT	fDamagePlusRateToSuperArmor;
	FLOAT	fDamagePlusRateForCounter;
	WORD	wPhysicalAttribute;
	//WORD	wPhysicalAttributeLen;
	//std::wstring 	strPhysicalAttribute;
	WORD	wElementalAttributeLen;
	std::wstring	strElementalAttribute;
	DWORD	dwEventOptionSerial;
	DWORD	dwEventOptionDamageReduceRate;
	BYTE	bIsCounterAttack;
	BYTE	bIsHitSuperArmor;
	DWORD	dwProjectileHash;
	BYTE	bFirstHit;
	DWORD	dwActionHash;
	DWORD	dwSkillHash;
	DWORD	dwSkillLevel;
	FLOAT	fProjectileScalex;
	FLOAT	fProjectileScaley;
	FLOAT	fProjectileScalez;
	DWORD	dwTimeStamp;
};
static_assert(sizeof(stCmd_SkillHitBody) == sizeof(SEND_REQUEST_PROJECTILE_HIT_BODY), "");

DWORD CMDS_Msg_SkillHit::GetSkillTimestamp()
{
	using namespace GAddr;
	using namespace GOffset;
	if ( (int)g_pSkillTimestampValue < (WORD)-1 )
	{
		assert(false);
		return 0;
	}
	auto timestamp_ptr = (DWORD*)((int)g_pSkillTimestampValue + g_o_skill_timestamp);
	return (*timestamp_ptr)++;
}

void CMDS_Msg_SkillHit::HittedItem::Write( StrOutputBStream& ostm ) const
{
	static auto hash_hit_info = CalcHash_WORD(L"Apple::HitInfo");
	static auto hash_hit_projectile_info = CalcHash_WORD(L"HitProjectileInfo");
	static auto hash_apple_hti_info = CalcHash_WORD(L"Apple::HitProjectileInfo");
	static const MAKE_CHAR_ARRAY_GAME(L"HIT_TARGET", kCA_HitTarget);
	//static const MAKE_CHAR_ARRAY_GAME(L"SLASH", kCA_PhysicalAttribute_);
	static const MAKE_CHAR_ARRAY_GAME(L"ELEC", kCA_ElementalAttribute_);
	ostm << hash_hit_info << projectile_serial_ << defender_serial_ << attacker_serial_;
		ostm << kCA_HitTarget << hit_type_ << delta_hp_ << random_seed_;
		ostm << hash_hit_projectile_info << hash_apple_hti_info;
		ostm << attack_power_rate_ << damage_plus_rate_to_super_armor_ << damage_plua_rate_for_counter_;
		ostm << (WORD)4;
		//ostm << kCA_PhysicalAttribute_;
		ostm << kCA_ElementalAttribute_ << event_option_serial_ << event_option_damage_reduce_rate_;
		ostm << is_counter_attack_ << is_hit_super_armor_ << projectile_hash_ << first_hit_;
		ostm << action_hash_ << skill_hash_ << skill_level_ << projectile_scale_ << float_unkown1_;
		ostm << float_unkown2_ << timestamp_
		;
}

CMDS_Msg_SkillHit::HittedItem::HittedItem()
{
	projectile_serial_ = 0;
	defender_serial_ = 0;
	attacker_serial_ = 0;
	static_assert(sizeof(DWORD) == sizeof(decltype(hit_type_)), "");
	hit_type_ = GType::kHT_Unknown;
	delta_hp_ = -1;
	random_seed_ = 0;
	attack_power_rate_ = 0.580000f;
	damage_plus_rate_to_super_armor_ = 0.;
	//damage_plus_rate_to_super_armor_ = 1.;
	damage_plua_rate_for_counter_ = 0.;
	event_option_serial_ = 0;
	event_option_damage_reduce_rate_ = 0;
	is_counter_attack_ = false;
	is_hit_super_armor_ = false;
	//is_hit_super_armor_ = true;
	projectile_hash_ = 0;
	first_hit_ = 0;
	action_hash_ = 0;
	skill_hash_ = 0;
	skill_level_ = 0;
	projectile_scale_ = 1.0f;
	float_unkown1_ = 1.0f;
	float_unkown2_ = 1.0f;
	timestamp_ = 0;
}

void CMDS_Msg_SkillHit::HittedItem::AcceptParams( DWORD defender_serial, DWORD projectile_serial, DWORD attacker_serial,
												 DWORD projectile_hash, DWORD action_hash, DWORD skill_hash, DWORD skill_level, DWORD time_stamp, float attack_power_rate
												 , int delta_hp )
{
	defender_serial_ = defender_serial;
	attacker_serial_ = attacker_serial;
	projectile_hash_ = projectile_hash;
	projectile_serial_ = projectile_serial;
	action_hash_ = action_hash;
	skill_hash_ = skill_hash;
	skill_level_ = skill_level;
	timestamp_ = time_stamp;
	if ( attack_power_rate > 0.580000f )
		attack_power_rate_ = attack_power_rate;
	delta_hp_ = (int)(0 - (DWORD)delta_hp + 1);
}

CMDS_Msg_FbWalk::CMDS_Msg_FbWalk()
{
	role_serial_ = 0;
	skip_validation_ = false;
	direction2d_.dir_x_ = 0x399E;
	direction2d_.dir_y_ = 0xB9B2;
	pre_action_play_time_ = 0.f;
	action_serial_ = 0;
	rand_seed_ = 0;
	client_absolute_time_ = 0.f;
	exist_params_ = true;
	pre_action_serial_ = 0;
}

void CMDS_Msg_FbWalk::AcceptParams( DWORD role_serial, const std::wstring& job_name, const std::wstring& cmd_name, 
								 const GType::stGameVector& action_start_position, const GType::stGameVector& direction, bool skip_validation )
{
	role_serial_ = role_serial;
	role_job_name_ = job_name;
	cmd_name_ = cmd_name;
	action_start_position_ = action_start_position;
	direction_ = direction;
	skip_validation_ = skip_validation;
}

void CMDS_Msg_FbWalk::AcceptStand( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos, float pre_action_time, 
								  DWORD action_serial, DWORD rand_seed, float client_absolute_time )
{
	static const GType::stGameVector kDirection;
	AcceptParams(role_serial, job_name, kSendMsg_CmdNameStand, pos, kDirection, true);
	pre_action_play_time_ = pre_action_time;
	action_serial_ = action_serial;
	rand_seed_ = rand_seed;
	client_absolute_time_ = client_absolute_time;
	exist_params_ = true;
}

void CMDS_Msg_FbWalk::AcceptFastRun( DWORD role_serial, const std::wstring& job_name, 
									const GType::stGameVector& action_start_position, const GType::stGameVector& direction, float pre_action_time
									, DWORD action_serial, DWORD rand_seed, float client_absolute_time)
{
	static const std::wstring kCmdName(L"FastRun");
	AcceptParams(role_serial, job_name, kCmdName, action_start_position, direction, false);
	pre_action_play_time_ = pre_action_time;
	action_serial_ = action_serial;
	rand_seed_ = rand_seed;
	client_absolute_time_ = client_absolute_time;
	exist_params_ = true;
}

struct stCmd_ActionSyncPacket : CMDS_HeaderBase{
	DWORD			dwSerial;
	DWORD			dwActionSerial;
	DWORD			dwActionHash;
	BYTE			bExistParams;
	WORD			wParams;
	WORD			wassociative_array;
	DWORD			dwsize;
	struct
	{
		WORD			witem_first_iLen;
		std::wstring	stritem_first_i;
		WORD			witem_second_iLen;
		std::wstring	stritem_second_i;
	}Item[10];
	XYZ				theActionStartPosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
	DWORD			dwClientAbsoluteTime;
	DWORD			dwRandomSeed;
	DWORD			dwPrevActionSerial;
	DWORD			dwPrevActionPlayTime;
	BYTE			bSkipValidation;
};
static_assert(sizeof(stCmd_ActionSyncPacket) == sizeof(SEND_ACTION_SYNC_PACKET), "");

void CMDS_Msg_FbWalk::Write( StrOutputBStream& ostm ) const
{
	using namespace GAddr;
	assert(role_serial_);
	assert(!cmd_name_.empty());
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::Both::ActionSystem::ActionSyncPacket");
	//static const auto kHashUri = CalcHash_WORD(L"038f2071d608b0d6942d097326ccbb27");
	ostm << role_serial_;
	//行动序号
	ostm << action_serial_;
	ostm << CalcHash_DWORD(role_job_name_, cmd_name_) << exist_params_;
	static const auto kHashParams = CalcHash_WORD(L"Params");
	static const auto kHashAssoci = CalcHash_WORD(L"associative_array");
	static const MAKE_CHAR_ARRAY_GAME(L"Direction", kCA_Direction);
	if (exist_params_)
	{
		ostm << kHashParams << kHashAssoci << (DWORD)1;
		ostm << kCA_Direction;
		CStringW str_tmp;
		str_tmp.Format(L"(%f, %f, %f)", direction_.x_, direction_.z_, direction_.y_);
		ostm << GameCharRuntimeCStr(str_tmp);
	}
	ostm << action_start_position_;
	ostm << direction2d_ << moving_direction_;
	ostm << client_absolute_time_;
	// RandomSeed
	ostm << rand_seed_;
	// PrevActionSerial
	//ostm << action_serial_ - 1;
	ostm << pre_action_serial_;
	ostm << pre_action_play_time_;
	ostm << skip_validation_;
}

void CMDS_Msg_FbWalk::AcceptRealize( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& action_start_position, 
									float pre_action_time, DWORD action_serial, DWORD pre_action_serial,  DWORD rand_seed, float client_absolute_time )
{
	static const GType::stGameVector kDirection;
	AcceptParams(role_serial, job_name, kSendMsg_CmdNameRealize, action_start_position, kDirection, true);
	pre_action_play_time_ = pre_action_time;
	action_serial_ = action_serial;
	rand_seed_ = rand_seed;
	client_absolute_time_ = client_absolute_time;
	exist_params_ = false;
	pre_action_serial_ = pre_action_serial;
}


CMDS_Msg_EnterCloneMap::CMDS_Msg_EnterCloneMap()
{
	difficulty_ = GType::kFD_Invalid;
	is_first_player_ = false;
	is_restart_ = false;
}

struct stCmd_StartStage : CMDS_HeaderBase{
	DWORD	dwStageHash;
	DWORD	dwDifficulty;
	BYTE	bIsFirstPlay;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
	BYTE			bIsRestart;
	BYTE			bUseIntrusionItem;
};
static_assert(sizeof(stCmd_StartStage) == sizeof(SEND_STAGE_SYSTEM_START_STAGE), "");

void CMDS_Msg_EnterCloneMap::Write( StrOutputBStream& ostm ) const
{
	assert(!clone_map_name_.empty());
	assert(difficulty_ >= GType::kFD_Normal && difficulty_ < GType::kFD_Invalid);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::StartStage");
	//static const auto kUri = CalcHash_WORD(L"dd246d8973092d9a60c0bddb6f8f39b0");
	ostm << CalcHash_DWORD(clone_map_name_) << difficulty_;
	ostm << is_first_player_;
	ostm << GameCharRuntime(wrap_gate_id_);
	ostm << is_restart_;
	//bUseIntrusionItem
	ostm << false;
	//bIsMaxLevelExpendTutorialPlay
	ostm << false;
}

void CMDS_Msg_EnterCloneMap::AcceptEnter( const std::wstring& clonemap_id, GType::enFbDifficulty difficulty )
{
	clone_map_name_ = clonemap_id;
	difficulty_ = difficulty;
	wrap_gate_id_ = L"StageStartWarpGate01";
}

void CMDS_Msg_EnterCloneMap::AcceptChangeJob( const std::wstring& clonemap_id )
{
	clone_map_name_ = clonemap_id;
	difficulty_ = GType::kFD_Normal;
}

CMDS_Msg_StepInWarpGate::CMDS_Msg_StepInWarpGate( const std::wstring& role_name, const std::wstring& gate_name ) : role_name_(role_name), gate_name_(gate_name)
{

}

struct stCmd_StepInWarpGate : CMDS_HeaderBase{
	WORD			wPlayerIdLen;
	std::wstring	strPlayerId;
	BYTE			bStepIn;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
};
static_assert(sizeof(stCmd_StepInWarpGate) == sizeof(SEND_STEP_IN_WARP_GATE), "");

void CMDS_Msg_StepInWarpGate::Write( StrOutputBStream& ostm ) const
{
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ActorSystem::StepInWarpGate");
	//static const auto kUri = CalcHash_WORD(L"6490af8101d3c41132b3366b2a46a72d");
	ostm << GameCharRuntime(role_name_);
	ostm << true;
	ostm << GameCharRuntime(gate_name_);
}

CMDS_Msg_DirectionSyncPacket::CMDS_Msg_DirectionSyncPacket( DWORD role_serial, const GType::stGameVector& role_pos )
	: role_pos_(role_pos)
{
	assert(role_serial);
	role_serial_ = role_serial;
	direction_.dir_x_ = 0x3BFA;
	direction_.dir_y_ = 0x2CB5;
}

struct stCmd_DirectionSyncPacket : CMDS_HeaderBase{
	DWORD			dwSerial;
	XYZ				thePosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
};
static_assert(sizeof(stCmd_DirectionSyncPacket) == sizeof(SEND_DIRECTION_SYNC_PACKET), "");

void CMDS_Msg_DirectionSyncPacket::Write( StrOutputBStream& ostm ) const
{
	assert(role_serial_);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::Both::ActionSystem::DirectionSyncPacket");
	//static const auto kUri = CalcHash_WORD(L"e1ddc887f465dd79053399f012a3cac9");
	ostm << role_serial_ << role_pos_ << direction_ << moving_direction_;
}

CMDS_Msg_ChangeSector::CMDS_Msg_ChangeSector( const std::wstring& gate_name ) : gate_name_(gate_name)
{
	valid_data_x_ = 0;
	valid_data_y_ = 0;
	is_dummy_exist_ = false;
	attack_speed_ = 1.0;
	move_speed_ = 1.0;
	evade_rating_ = 36.;
}

struct stCmd_ChangeSector : CMDS_HeaderBase{
	WORD				wWarpGateIdLen;
	std::wstring		strWarpGateId;
	FLOAT				fAttackSpeed;
	FLOAT				fMoveSpeed;
	FLOAT				fEvadeRating;
	BYTE				bIsDummyExist;
	DWORD				dwValidationData_Y;
	DWORD				dwValidationData_X;
};
static_assert(sizeof(stCmd_ChangeSector) == sizeof(SEND_CHANGE_SECTOR), "");

void CMDS_Msg_ChangeSector::Write( StrOutputBStream& ostm ) const
{
	assert(!gate_name_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::ChangeSector");
	//static const auto kUri = CalcHash_WORD(L"51d107379e1955235e4e5dc41dd2e391");
	ostm << GameCharRuntime(gate_name_);
	// AttackSpeed
	ostm << attack_speed_;
	// MoveSpeed
	ostm << move_speed_;
	// EvadeRating
	ostm << evade_rating_;
	ostm << is_dummy_exist_;
	ostm << valid_data_y_;
	ostm << valid_data_x_;
}

void CMDS_Msg_ChangeSector::AcceptParam( DWORD valid_data_x, DWORD valid_data_y, bool is_dummy_exist, float attack_speed, float move_speed, float evade_rating )
{
	is_dummy_exist_ = is_dummy_exist;
	valid_data_y_ = valid_data_y;
	valid_data_x_ = valid_data_x;
	attack_speed_ = attack_speed;
	move_speed_ = move_speed;
	evade_rating_ = evade_rating;
}

struct stCmd_RetCity : CMDS_HeaderBase{
	BYTE			bAlone;
	DWORD			dwSumOfDeltaHp;
};
static_assert(sizeof(stCmd_RetCity) == sizeof(SEND_RETURN_VILLAGE), "");

void CMDS_Msg_ReturnCity::Write( StrOutputBStream& ostm ) const
{
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::ReturnVillage");
	//static const auto kUri = CalcHash_WORD(L"85ac893b6ce6de3a6dee371c56807d8e");
	//ostm << kUri;
	// Alone
	ostm << (BYTE)1;
	//sum of delta hp
	ostm << 0;
}

CMDS_Msg_RestartStage::CMDS_Msg_RestartStage()
{
	difficulty_ = GType::kFD_Invalid;
}

struct stCmd_RestartStage : CMDS_HeaderBase{
	DWORD	dwStageHash;
	DWORD	dwDifficulty;
	BYTE	bIsFirstPlay;
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
	BYTE			bIsRestart;
	BYTE			bUseIntrusionItem;
	BYTE			bIsMaxLevelExpendTutorialPlay;
};
static_assert(sizeof(stCmd_RestartStage) == sizeof(SEND_STAGE_SYSTEM_START_STAGE), "");

void CMDS_Msg_RestartStage::Write( StrOutputBStream& ostm ) const
{
	assert(!clone_map_name_.empty());
	assert(difficulty_ >= GType::kFD_Normal && difficulty_ < GType::kFD_Invalid);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::StartStage");
	//static const auto kUri = CalcHash_WORD(L"dd246d8973092d9a60c0bddb6f8f39b0");
	ostm << CalcHash_DWORD(clone_map_name_) << difficulty_;
	// IsFirstPlay
	ostm << false;
	// WarpGateId
	ostm << (DWORD)1;
	// IsRestart
	ostm << true;
	// UseIntrusionItem
	ostm << false;
	//bIsMaxLevelExpendTutorialPlay
	ostm << false;
}

struct stCmd_RepairItem : CMDS_HeaderBase{
	BYTE			bIsEquipmentRepair;
};
static_assert(sizeof(stCmd_RepairItem) == sizeof(SEND_REPAIR_ITEM), "");

void CMDS_Msg_RepairWearEquip::Write( StrOutputBStream& ostm ) const
{
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ShopSystem::RepairItem");
	//static const auto kUri = CalcHash_WORD(L"a1cf156de551bb58d0271c523b5ad578");
	//ostm << kUri;
	// 修理已穿装备
	ostm << true;
}

CMDS_Msg_EquipItem::CMDS_Msg_EquipItem()
{
	equip_part_ = GType::kEP_Invalid;
	inven_slot_ = 0;
}

struct stCmd_EquipItem : CMDS_HeaderBase{
	WORD			wEquipTabLen;
	std::wstring	strEquipTab;
	WORD			wEquipSlot;
	WORD			wInventoryTabLen;
	std::wstring	strInventoryTab;
	WORD			wInvenSlot;
};
static_assert(sizeof(stCmd_EquipItem) == sizeof(SEND_EQUIP_ITEM), "");

static const MAKE_CHAR_ARRAY_GAME(L"BASIC", kCA_EquipTab);
static const MAKE_CHAR_ARRAY_GAME(L"EQUIPMENT_TAB", kCA_InvenTab);
void CMDS_Msg_EquipItem::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	static_assert(8 == kEP_Ring, "");
	assert(equip_part_ <= kEP_Ring);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::InventorySystem::EquipItem");
	//static const auto kUri = CalcHash_WORD(L"cebca38adfe1a51de79f579873e41133");
	//ostm << kUri;
	ostm << kCA_EquipTab << (WORD)equip_part_;
	ostm << kCA_InvenTab << inven_slot_;
}

CMDS_Msg_UnEquipItem::CMDS_Msg_UnEquipItem()
{
	equip_part_ = GType::kEP_Invalid;
	inven_slot_ = 0;
}

struct stCmd_UnequipItem : CMDS_HeaderBase{
	WORD			wEquipTabLen;
	std::wstring	strEquipTab;
	WORD			wEquipSlot;
	WORD			wInventoryTabLen;
	std::wstring	strInventoryTab;
	WORD			wInvenSlot;
};
static_assert(sizeof(stCmd_UnequipItem) == sizeof(SEND_UN_EQUIP_ITEM), "");

void CMDS_Msg_UnEquipItem::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	assert(equip_part_ <= kEP_Ring);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::InventorySystem::UnequipItem");
	//static const auto kUri = CalcHash_WORD(L"d5b24ed93d2a59535d3291aca7173c8e");
	//ostm << kUri;
	ostm << kCA_EquipTab << (WORD)equip_part_;
	ostm << kCA_InvenTab << inven_slot_;
}

CMDS_Msg_UseForItem::CMDS_Msg_UseForItem()
{
	using_slot_ = 0;
	target_slot_ = 0;
}

struct stCmd_UseForItem : CMDS_HeaderBase{
	DWORD			dwUsingTabIndex;		// 鉴定书所在背包栏序号
	WORD			wUsingSlotIndex;		// 鉴定书位置
	DWORD			dwTargetTabIndex;		// 被鉴定物品所在背包栏序号
	WORD			wTargetSlotIndex;		// 被鉴定的物品位置
};
static_assert(sizeof(stCmd_UseForItem) == sizeof(SEND_USE_FOR_ITEM), "");

void CMDS_Msg_UseForItem::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	static_assert(sizeof(kTC_Consume) == sizeof(DWORD), "");
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::InventorySystem::UseForItem");
	//static const auto kUri = CalcHash_WORD(L"fcf22fe368eaea7f7a2433ab1fea257a");
	//ostm << kUri;
	// UsingTabIndex
	ostm << kTC_Consume;
	// UsingSlotIndex
	ostm << using_slot_;
	// TargetTabIndex
	ostm << kTC_Normal;
	// TargetSlotIndex
	ostm << target_slot_;
}

struct stCmd_ChangeVillage : CMDS_HeaderBase{
	WORD			wWarpGateIdLen;
	std::wstring	strWarpGateId;
};
static_assert(sizeof(stCmd_ChangeVillage) == sizeof(SEND_CHANGE_VILLGAE), "");

void CMDS_Msg_ChangeVillage::Write( StrOutputBStream& ostm ) const
{
	assert(!village_gate_id_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::ChangeVillage");
	//static const auto kUri = CalcHash_WORD(L"8700c48e9de9aa244ec92d34c498fe78");
	ostm << GameCharRuntime(village_gate_id_);
}

CMDS_Msg_SellItem::CMDS_Msg_SellItem()
{
	tab_category_ = GType::kTC_Invalid;
	slot_index_ = -1;
	item_cnt_ = 0;
	static_assert(sizeof(tab_category_) == sizeof(DWORD), "");
}

struct stCmd_SellItem : CMDS_HeaderBase{
	DWORD			dwFixedNpcHash;
	DWORD			dwTabCategory;
	DWORD			dwSlotIndex;
	DWORD			dwCount;
};
static_assert(sizeof(stCmd_SellItem) == sizeof(SEND_SELL_ITEM), "");

void CMDS_Msg_SellItem::Write( StrOutputBStream& ostm ) const
{
	assert(!fixed_npc_name_.empty());
	assert(tab_category_ < GType::kTC_Invalid);
	assert(item_cnt_ > 0);
	assert(slot_index_ >= 0);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ShopSystem::SellItem");
	//static const auto kUri = CalcHash_WORD(L"2f6401c12fc6a415ea86fada78c3617c");
	//ostm << kUri;
	// FixedNpcHash
	ostm << CalcHash_DWORD(fixed_npc_name_);
	// TabCategory
	ostm << tab_category_;
	// SlotIndex
	ostm << slot_index_;
	// Count
	ostm << item_cnt_;
}

CMDS_Msg_BuyItem::CMDS_Msg_BuyItem()
{
	item_id_ = 0;
	item_cnt_ = 0;
}

struct stCmd_BuyItem : CMDS_HeaderBase{
	DWORD			dwFixedNpcHash;
	WORD			wSellId;
	WORD			wCount;
};
static_assert(sizeof(stCmd_BuyItem) == sizeof(SEND_BUY_ITEM), "");

void CMDS_Msg_BuyItem::Write( StrOutputBStream& ostm ) const
{
	assert(!fixed_npc_name_.empty());
	assert(item_id_);
	assert(item_cnt_ > 0);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ShopSystem::BuyItem");
	//static const auto kUri = CalcHash_WORD(L"35bf1b03b248e6642008740c7cca0f01");
	ostm << CalcHash_DWORD(fixed_npc_name_);
	// SellId
	ostm << item_id_;
	// Count
	ostm << item_cnt_;
}

CMDS_Msg_LearnSkill::CMDS_Msg_LearnSkill()
{
	skill_lvl_ = 0;
}

struct stCmd_LearnSkill : CMDS_HeaderBase{
	DWORD			dwSkillSet;			// 11.30 新增数据段
	WORD			wRequestLearnSkillList;
	WORD			warray;
	DWORD			dwsize;
	struct
	{
		WORD		witem_i;
		WORD		wpair;
		DWORD		dwkey;
		DWORD		dwvalue;
	}Item[1];		// 此结构只适合学一个技能
};
static_assert(sizeof(stCmd_LearnSkill) == sizeof(SEND_LEARN_SKILL_MULTIPLE), "");

void CMDS_Msg_LearnSkill::Write( StrOutputBStream& ostm ) const
{
	assert(!skill_name_.empty());
	assert(skill_lvl_);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ShopSystem::LearnSkillMultiple");
	//static const auto kUri = CalcHash_WORD(L"a31bfe5025bd74f8cfd945c56fb74251");
	static const auto kRequestLearnSkill = CalcHash_WORD(L"RequestLearnSkillList");
	//ostm << kUri;
	ostm << DWORD(0);
	ostm << kRequestLearnSkill << kGameHashArray;
	ostm << (DWORD)1;
	static const auto kHashItem = CalcHash_WORD(L"item_0");
	static const auto kHashPair = CalcHash_WORD(L"pair");
	ostm << kHashItem << kHashPair;
	ostm << CalcHash_DWORD(skill_name_);
	ostm << skill_lvl_;
}

CMDS_Msg_CreatePlayer::CMDS_Msg_CreatePlayer()
{
	job_category_ = GType::kJC_Invalid;
	selected_skin_color_index_ = 2;
}

void CMDS_Msg_CreatePlayer::Write( StrOutputBStream& ostm ) const
{
	assert(!player_feature_hlp_.player_features_.Empty());
	assert(!role_name_.empty());
	assert(job_category_ < GType::kJC_Invalid);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::PlayerManagementSystem::CreatePlayer");
	//static const auto kUri = CalcHash_WORD(L"cc4c3e9a5e0b08ccae365cba61f5e5c3");
	ostm << GameCharRuntime(role_name_);
	// PlayerGender
	ostm << 0;
	// ClassType
	ostm << job_category_;
	static const auto kHashCustom = CalcHash_WORD(L"CustomizingData");
	static const auto kHashAppleCurtom = CalcHash_WORD(L"Apple::CustomizingData");
	static const auto kHashCustomPart = CalcHash_WORD(L"CustomizingPartDatas");
	ostm << kHashCustom << kHashAppleCurtom << kHashCustomPart;
	ostm << player_feature_hlp_.player_features_;
	ostm << selected_skin_color_index_;
}

const CMDS_Msg_CreatePlayer::stPlayerFeatureHlp CMDS_Msg_CreatePlayer::player_feature_hlp_;

void CMDS_Msg_CreatePlayer::stPlayerFeature::Write( StrOutputBStream& ostm ) const
{
	assert(!body_part_name_.empty());
	ostm << hash_feature_uri_ << GameCharRuntime(body_part_name_);
	ostm << selected_mesh_info_index_ << selected_part1_color_index_ << selected_part2_color_index_;
}

CMDS_Msg_CreatePlayer::stPlayerFeature::stPlayerFeature( WORD feature_uri, const std::wstring& body_part_name,
														BYTE selected_mesh, BYTE selected_part1, BYTE selected_part2 )
														: body_part_name_(body_part_name)
{
	hash_feature_uri_ = feature_uri;
	selected_mesh_info_index_ = selected_mesh;
	selected_part1_color_index_ = selected_part1;
	selected_part2_color_index_ = selected_part2;
}

CMDS_Msg_CreatePlayer::stPlayerFeatureHlp::stPlayerFeatureHlp()
{
	const auto kHashPartUri = CalcHash_WORD(L"Apple::CustomizingData::CustomizingPartData");
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Head, 0, 0x9, 0xff));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Upper, 0, 0, 0x9));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Lower, 0, 1, 0));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Hand, 0, 9, 9));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Foot, 0, 9, 9));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Face, 0, 0xff, 0xff));
	player_features_.PushBack(stPlayerFeature(kHashPartUri, kBodyPartName_Eye, 0, 0, 0xff));
}

struct stCmd_SelectPlayer : CMDS_HeaderBase{
	WORD			wPlayerIdLen;
	std::wstring	strPlayerId;
};
static_assert(sizeof(stCmd_SelectPlayer) == sizeof(SEND_SELECT_PLAYER), "");

void CMDS_Msg_SelectPlayer::Write( StrOutputBStream& ostm ) const
{
	assert(!role_name_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::SessionSystem::SelectPlayer");
	//static const auto kUri = CalcHash_WORD(L"5cefcbe0615c112023bbcca2d1cbd8bc");
	ostm << GameCharRuntime(role_name_);
}

struct stCmd_RequestSwitchChannel : CMDS_HeaderBase{
	WORD			wChannelIdLen;
	std::wstring	strChannelId;
};
static_assert(sizeof(stCmd_RequestSwitchChannel) == sizeof(SEND_REQUEST_SWITCH_CHANNEL), "");

void CMDS_Msg_RequestSwitchChannel::Write( StrOutputBStream& ostm ) const
{
	assert(!channel_name_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::SessionSystem::RequestSwitchChannel");
	//static const auto kUri = CalcHash_WORD(L"8bedfbed6e9106a8a208e391c60cfeb3");
	ostm << GameCharRuntime(channel_name_);
}

struct stCmd_SwitchChannel : CMDS_HeaderBase{
	WORD			wChannelIdLen;
	std::wstring	strChannelId;
	WORD			wAccountIdLen;
	std::wstring	strAccountId;
	WORD			wSessionKeyLen;
	std::wstring	strSessionKey;
};
static_assert(sizeof(stCmd_SwitchChannel) == sizeof(SEND_SWITCH_CHANNEL), "");

void CMDS_Msg_SwitchChannel::Write( StrOutputBStream& ostm ) const
{
	assert(!channel_name_.empty());
	assert(!account_id_.empty());
	assert(!session_key_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::SessionSystem::SwitchChannel");
	//static const auto kUri = CalcHash_WORD(L"827d94dc425544c8bf32122e7d3f0f84");
	ostm << GameCharRuntime(channel_name_) << GameCharRuntime(account_id_) << GameCharRuntime(session_key_);
}

CMDS_Msg_SendMail::CMDS_Msg_SendMail()
{
	gold_ = 0;
}

struct stCmd_SendMailHeader : CMDS_HeaderBase{
	DWORD			dwType;
	DWORD			dwSystemMailType;
	WORD			wTitleLen;
	std::wstring	strTitle;
	LONGLONG		llOidReceiver;
	WORD			wReceiverIdLen;
	std::wstring	strReceiverId;
	WORD			wMessageLen;
	std::wstring	strMessage;
	LONGLONG		llGold;
	DWORD			dwOidMailItemList;
	WORD			wItems;
	WORD			warray;
	DWORD			dwsize;
};
static_assert(sizeof(stCmd_SendMailHeader) == sizeof(SEND_MAIL_SEND_HEADER), "");

void CMDS_Msg_SendMail::Write( StrOutputBStream& ostm ) const
{
	assert(!receiver_role_name_.empty());
	assert(!mail_title_.empty());
	assert(!mail_content_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MailSystem::MailSend");
	//static const auto kUri = CalcHash_WORD(L"5fc62cc2d7a859c3e039eb81b3235d3c");
	//ostm << kUri;
	// Type
	ostm << 0;
	// SystemMailType
	ostm << 0;
	ostm << GameCharRuntime(mail_title_);

	static wchar_t receive_role_buffer[50] = { 0 };
	wcscpy_s(receive_role_buffer, receiver_role_name_.c_str());
	LONGLONG oid_receiver = (LONGLONG)(wchar_t*)receive_role_buffer;
	oid_receiver <<= 32;
	// OidReceiver
	ostm << oid_receiver;
	//ostm << (UINT64)0;
	ostm << GameCharRuntime(receiver_role_name_) << GameCharRuntime(mail_content_);
	ostm << (LONGLONG)gold_;
	// OidMailItemList
	ostm << 1;
	static const auto kHashItem = CalcHash_WORD(L"Items");
	ostm << kHashItem;
	ostm << items_;
	static const auto kHashExpireDate = CalcHash_WORD(L"ExpireDate");
	static const auto kHashAppleDateTime = CalcHash_WORD(L"Apple::DateTime");

	ostm << kHashExpireDate << kHashAppleDateTime;
	// Year
	ostm << (WORD)0x578;
	// Month
	ostm << (WORD)1;
	// Day
	ostm << (WORD)1;
	// Hour
	ostm << 0;
	// Minute
	ostm << 0;
	// Second
	ostm << 0;
	// MilliSec
	ostm << 0;
	// IsInfinity
	ostm << false;
}

CMDS_Msg_SendMail::stMailItems::stMailItems( const std::wstring& tab_name, int item_slot_index, int item_cnt )
	: tab_name_(tab_name)
{
	assert(!tab_name.empty());
	item_slot_index_ = item_slot_index;
	item_cnt_ = item_cnt;
}

void CMDS_Msg_SendMail::stMailItems::Write( StrOutputBStream& ostm ) const
{
	assert(item_slot_index_ >= 0);
	static const auto kUri = CalcHash_WORD(L"Apple::SendingMailItem");
	ostm << kUri << GameCharRuntime(tab_name_) << item_slot_index_ << item_cnt_;
}

struct stCmd_SkipStoryAnimate : CMDS_HeaderBase{
	BYTE			bIgnoreFistPlay;
	BYTE			bSwitchChannelInVillage;
};
static_assert(sizeof(stCmd_SkipStoryAnimate) == sizeof(SEND_JOIN_VILLAGE), "");

void CMDS_Msg_SkipStoryAnimate::Write( StrOutputBStream& ostm ) const
{
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::JoinVillage");
	//static const auto kUri = CalcHash_WORD(L"e56a7bfbcf0df1471b9c59d6b0444703");
	//ostm << kUri;
	// IgnoreFistPlay
	ostm << false;
	// SwitchChannelInVillage
	ostm << false;
}

struct stCmd_AcceptTask : CMDS_HeaderBase{
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
	DWORD			dwRewardItemId;
	DWORD			dwInventoryTabIndex;
	DWORD			dwInventorySlotIndex;
};
static_assert(sizeof(stCmd_AcceptTask) == sizeof(SEND_ACCEPT), "");

void CMDS_Msg_AcceptTask::Write( StrOutputBStream& ostm ) const
{
	assert(!quest_id_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::QuestSystem::Accept");
	//static const auto kUri = CalcHash_WORD(L"6c32a7184cafa0cfd343ecc1a6ea3a8d");
	ostm << GameCharRuntime(quest_id_);
	//reward item id
	ostm << 1;
	//inventory tab index
	ostm << -1;
	//inventory slot index
	ostm << -1;
}

struct stCmd_TaskComplete : CMDS_HeaderBase{
	WORD			wQuestIdLen;
	std::wstring	strQuestId;
	WORD			wRewardItemIdLen;
	std::wstring	strRewardItemId;
};
static_assert(sizeof(stCmd_TaskComplete) == sizeof(SEND_COMPLETE), "");

void CMDS_Msg_CompleteTask::Write( StrOutputBStream& ostm ) const
{
	assert(!quest_id_.empty());
	//assert(!reward_item_id_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::QuestSystem::Complete");
	//static const auto kUri = CalcHash_WORD(L"7824a9c8b4fc5eff77a1a20c43fe6135");
	ostm << GameCharRuntime(quest_id_) << GameCharRuntime(reward_item_id_);
}

CMDS_Msg_DisassemblyItem::CMDS_Msg_DisassemblyItem()
{
	tab_category_ = GType::kTC_Invalid;
}

struct stCmd_DisassemblyItem : CMDS_HeaderBase{
	DWORD			dwTabCategory;
	DWORD			dwSlotIndex;
	WORD			wIsAvatarItem;
};
static_assert(sizeof(stCmd_DisassemblyItem) == sizeof(SEND_DISASSEMBLY), "");

void CMDS_Msg_DisassemblyItem::Write( StrOutputBStream& ostm ) const
{
	assert(GType::kTC_Normal == tab_category_);
	assert(slot_index_ >= 0);
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ItemTransformSystem::Disassembly");
	//static const auto kUri = CalcHash_WORD(L"920f9bdd034c400bd59dc8c770e73d5a");
	ostm << tab_category_ << slot_index_;
	//is avatar item
	ostm << (WORD)0;
}

void CMDS_Msg_VillageWalk::AcceptStand( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos )
{
	role_serial_ = role_serial;
	role_job_name_ = job_name;
	cmd_name_ = &kSendMsg_CmdNameStand;
	action_start_position_ = pos;
	direction_.x_ = 0.;
	direction_.y_ = 0.;
	direction_.z_ = 0.;
}

CMDS_Msg_VillageWalk::CMDS_Msg_VillageWalk()
{
	cmd_name_ = nullptr;
	role_serial_ = 0;
	direction2d_.dir_x_ = 0x0BB89;
	direction2d_.dir_y_ = 0x355D;
}

struct stCmd_VillageActionSyncPaket : CMDS_HeaderBase{
	DWORD			dwSerial;
	DWORD			dwActionSerial;
	DWORD			dwActionHash;
	BYTE			bExistParams;		// 是否存在参数的标志 存在=1 不存在=0
	WORD			wParams;
	WORD			wassociative_array;
	DWORD			dwsize;
	struct
	{
		WORD			witem_first_iLen;
		std::wstring	stritem_first_i;
		WORD			witem_second_iLen;
		std::wstring	stritem_second_i;
	}Item[10];
	XYZ				theActionStartPosition;
	WORD			wDirectionX;
	WORD			wDirectionY;
	WORD			wMovingDirectionX;
	WORD			wMovingDirectionY;
	DWORD			dwClientAbsoluteTime;
};
static_assert(sizeof(stCmd_VillageActionSyncPaket) == sizeof(SEND_VILLAGE_ACTION_SYNC_PACKET), "");

void CMDS_Msg_VillageWalk::Write( StrOutputBStream& ostm ) const
{
	using namespace GAddr;
	using namespace GFuncCall;
	assert(role_serial_);
	assert(cmd_name_);
	assert(!role_job_name_.empty());
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::Both::ActionSystem::VillageActionSyncPacket");
	//static const auto kUri = CalcHash_WORD(L"982d94fcd2d7650a650a2644ff41d9ac");
	ostm << role_serial_;
	UserSkillLockAdd();
	//行动序号
	auto action_serial = *g_pRoleSkillRunSerialAddr;
	ostm << action_serial;
	UserSkillLockAdd();
	ostm << CalcHash_DWORD(role_job_name_, *cmd_name_);
	// ExistParams
	ostm << true;
	// Params
	static const auto kHashParams = CalcHash_WORD(L"Params");
	ostm << kHashParams;
	// associative_array
	static const auto kHashAssoArray = CalcHash_WORD(L"associative_array");
	ostm << kHashAssoArray;
	ostm << int(1);
	static const MAKE_CHAR_ARRAY_GAME(L"Direction", kCA_Direction);
	ostm << kCA_Direction;
	CStringW str_tmp;
	str_tmp.Format(L"(%f, %f, %f)", direction_.x_, direction_.z_, direction_.y_);
	ostm << GameCharRuntimeCStr(str_tmp);
	ostm << action_start_position_;
	ostm << direction2d_ << moving_direction_;
	auto tick_count = (float)GetTickCount();
	ostm << tick_count;
}

void CMDS_Msg_VillageWalk::AcceptFastRun( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos, const GType::stGameVector& direction )
{
	role_serial_ = role_serial;
	role_job_name_ = job_name;
	cmd_name_ = &kSendMsg_CmdNameFastRun;
	action_start_position_ = pos;
	direction_ = direction;
}

CMDS_Msg_StageReward::CMDS_Msg_StageReward()
{
	reward_index = 0;
}

struct stCmd_ChooseReward : CMDS_HeaderBase{
	DWORD			dwRewardIndex;
};
static_assert(sizeof(stCmd_ChooseReward) == sizeof(SEND_STAGE_REWARD), "");

void CMDS_Msg_StageReward::Write( StrOutputBStream& ostm ) const
{
	assert(reward_index >= 0 && reward_index < 6);
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::StageSystem::StageReward");
	//static const auto kHashUri = CalcHash_WORD(L"a8d7b003267a116610630b3c583ee7b3");
	ostm << reward_index;
}

CMDS_Msg_SkillActivity::CMDS_Msg_SkillActivity()
{
	activity_count_ = 50;
}

struct stCmd_SkillActivity : CMDR_HeaderBase{
	DWORD	dwType;
	DWORD	dwCount;
	DWORD	dwDefenderSerial;
	DWORD	dwAttackerSerial;
};
static_assert(sizeof(stCmd_SkillActivity) == sizeof(SEND_ACTOR_SYSTEM_ACTIVITY), "");

void CMDS_Msg_SkillActivity::Write( StrOutputBStream& ostm ) const
{
	assert(activity_count_ > 0);
	//static auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::ActorSystem::Activity");
	//static auto kHashUri = CalcHash_WORD(L"1127a4bae67312f0d7a2fd8ffdf915dd");
	ostm << 0 << activity_count_ << 0 << 0;
}

CMDS_Msg_GetAttachments::CMDS_Msg_GetAttachments()
{
	mail_serial_ = 0;
}

void CMDS_Msg_GetAttachments::Write( StrOutputBStream& ostm ) const
{
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MailSystem::GetAttachments");
	//static const auto kHashUri = CalcHash_WORD(L"f8260de7249e4d60ab7ee464e5f499e1");
	assert(mail_serial_);
	ostm <<  mail_serial_;
	ostm << true;
}

CMDS_Msg_RemoveMail::CMDS_Msg_RemoveMail()
{
	mail_serial_ = 0;
}

void CMDS_Msg_RemoveMail::Write( StrOutputBStream& ostm ) const
{
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MailSystem::MailRemove");
	//static const auto kHashUri = CalcHash_WORD(L"246578b8220355a2c542e01995fe3eff");
	assert(mail_serial_);
	ostm << mail_serial_;
	ostm << true;
}

CMDS_Msg_BuyProduct::CMDS_Msg_BuyProduct()
{
	product_market_oid_ = 0;
	item_cnt_ = 0;
}

void CMDS_Msg_BuyProduct::Write( StrOutputBStream& ostm ) const
{
	assert(product_market_oid_);
	assert(item_cnt_ > 0);
	assert(item_cnt_ <= (WORD)-1);
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::BuyProduct");
	//static const auto kHashUri = CalcHash_WORD(L"9a6e248c23b03341309edf0af3d02c93");
	ostm << product_market_oid_ << (WORD)item_cnt_;
}

CMDS_Msg_RegisterProduct::CMDS_Msg_RegisterProduct()
{
	tab_category_ = GType::kTC_Invalid;
	slot_index_ = -1;
	price_ = 0;
	period_ = 7;
	item_cnt_ = 0;
}

void CMDS_Msg_RegisterProduct::Write( StrOutputBStream& ostm ) const
{
	assert(!item_cn_name_.empty());
	assert(GType::kTC_Invalid != tab_category_);
	assert(slot_index_ >= 0);
	assert(price_ > 0);
	assert(period_ > 0);
	assert(item_cnt_ > 0);
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::RegisterProduct");
	//static const auto kHashUri = CalcHash_WORD(L"c9c08a120d4964bd79fe1ee858d6a665");
	ostm << GameCharRuntime(item_cn_name_) << tab_category_
		<< slot_index_ << price_ << period_ << item_cnt_;
}

void CMDS_Msg_RequestMyProductList::Write( StrOutputBStream& ostm ) const
{
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::RequestMyProductList");
	//static const auto kHashUri = CalcHash_WORD(L"7fe1f4a833cc612bbef1a9e14aecdc2c");
	//ostm << kHashUri;
}

CMDS_Msg_RequestProductInfo::CMDS_Msg_RequestProductInfo()
{
	tab_category_ = GType::kTC_Invalid;
	slot_index_ = -1;
}

void CMDS_Msg_RequestProductInfo::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	assert(kTC_Normal == tab_category_ || kTC_Consume == tab_category_ || kTC_Material == tab_category_);
	assert(slot_index_ >= 0);
	//static const auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::RequestProductInfo");
	//static const auto kHashUri = CalcHash_WORD(L"71cc57ea3c00703152f84cc428ae97ef");
	ostm << tab_category_ << slot_index_;
}

CMDS_Msg_RequestProductList::CMDS_Msg_RequestProductList()
{
	page_num_ = 1;
}

void CMDS_Msg_RequestProductList::Write( StrOutputBStream& ostm ) const
{
	//static auto kHashUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::RequestProductList");
	//static auto kHashUri = CalcHash_WORD(L"f93c6af2752f096fd94bde35d0422520");
	ostm << 10 << page_num_ << GameCharRuntime(market_category_);
	ostm << 0x1f << false << 0 << 0x3C << 0 << 0x27;
	ostm << GameCharRuntime(item_ch_name_) << 0;
	static auto kHash_OptionList = CalcHash_WORD(L"OptionList");
	static auto kHash_AssoArray = CalcHash_WORD(L"associative_array");
	ostm << kHash_OptionList << kHash_AssoArray << 0;
}

CMDS_Msg_OpenItemPackage::CMDS_Msg_OpenItemPackage()
{
	main_slot_ = 0;
	require_tab_ = -1;
	require_slot_ = 0;
}

void CMDS_Msg_OpenItemPackage::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	static_assert(sizeof(kTC_Consume) == sizeof(require_tab_), "");
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::MarketSystem::RequestProductInfo");
	//static const auto kUri = CalcHash_WORD(L"415c2a704e50d150c6a26919d9c32405");
	//ostm << kUri;
	// UsingTabIndex
	ostm << kTC_Consume;
	// UsingSlotIndex
	ostm << main_slot_;
	// TargetTabIndex
	ostm << require_tab_;
	// TargetSlotIndex
	ostm << require_slot_;
	//nCount
	ostm << 0;
}

void CMDS_Msg_OpenItemPackage::AcceptSingleUse( WORD main_slot )
{
	main_slot_ = main_slot;
	require_tab_ = -1;
	require_slot_ = 0;
}

void CMDS_Msg_OpenItemPackage::AcceptRequireUse( WORD main_slot, WORD require_slot )
{
	main_slot_ = main_slot;
	require_tab_ = GType::kTC_Consume;
	require_slot_ = require_slot;
}

CMDS_Msg_UseItem::CMDS_Msg_UseItem()
{
	item_slot_ = 0;
}

void CMDS_Msg_UseItem::Write( StrOutputBStream& ostm ) const
{
	using namespace GType;
	static_assert(sizeof(kTC_Consume) == sizeof(enTabCategory), "");
	//static const auto kUri = CalcHash_WORD(L"Apple::Protocol::ToServer::InventorySystem::UseItem");
	//static const auto kUri = CalcHash_WORD(L"3ed486eb9fcc46121a77b379e97f3b61");
	//ostm << kUri;
	// UsingTabIndex
	ostm << kTC_Consume;
	// UsingSlotIndex
	ostm << item_slot_;
}

void CMDS_Msg_DestroyActors::Write( StrOutputBStream& ostm ) const
{
	//static const auto kUri = CalcHash_WORD(L"99a17b89ddaa7cddb22110d75f526fa0");
	//ostm << kUri;
	assert(0 != npc_serial_);
	static const auto kHashSerial = CalcHash_WORD(L"Serials");
	ostm << kHashSerial;
	ostm << kGameHashArray;
	ostm << 1;
	ostm << npc_serial_;
	//max dt
	ostm << 0.300258f;
	static_assert(sizeof(float) == sizeof(0.300258f), "");
	ostm << (float)GetTickCount();
	static const auto kHashSumOfDeltaHps = CalcHash_WORD(L"SumOfDeltaHps");
	static const auto kHashAsso = CalcHash_WORD(L"associative_array");
	ostm << kHashSumOfDeltaHps << kHashAsso;
	ostm << 1;
	ostm << npc_serial_;
	ostm << 0;
}

CMDS_Msg_DestroyActors::CMDS_Msg_DestroyActors()
{
	npc_serial_ = 0;
}

StrOutputBStream& operator << ( StrOutputBStream& ostm, const CMDS_MsgBase& msg )
{
	ostm << msg.CalcUriHash();
	msg.Write(ostm);
	return ostm;
}