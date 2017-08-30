#include "stdafx.h"
#include "GPlayer.h"
#include "Common/LogOut.h"
#include "GClass.h"
#include "FuncObjC.h"
#include "CfgStructC.h"
#include <Common/MemOper.h>
#include "GAddr.h"
#include "GOffset.h"
#include "GFuncCall.h"
#include "GMap.h"
#include "GConst.h"
#include "GUnknown.h"
#include "GMapPath.h"
#include <Protocol/P_Defines.h>
#include <Common/CmnException.h>
#include "../EventsName.h"
#include <boost/math/special_functions/round.hpp>
#include "GCloneMap.h"
#include "GPacketTest.h"
#include <Common/EnsureCleanup.h>

//////////////////////////////////////////////////////////////////////////
const int kMyProductListMaxCnt = 10;
const int kTradeMinLvl = 21;
const int kMailMinLvl = 18;
const int kSendMailMaxItemCnt = 10;
//////////////////////////////////////////////////////////////////////////

GPlayer::GPlayer() : GameObjBase(kGOT_Player)
{
	player_lvl_ = 0;
}

bool GPlayer::Update()
{
	using namespace GClass;
	using namespace GOffset;
	using namespace GType;
	if ( !__super::Update() )
		return false;
	assert(game_obj_addr_);

	stGameVectorCopier* vec_copier = (stGameVectorCopier*)(game_obj_addr_ + g_o_CreatureDirectionX);
	assert(vec_copier);
	VERIFY(vec_copier->CopyTo(direction_));
	if ( !MemOperApplyR(player_lvl_, (int)game_obj_addr_, g_o_RoleLevel) )
	{
		assert(false);
		return false;
	}
	return true;
}

int GPlayer::GetLevel() const
{
	return player_lvl_;
}

// 角色静态信息
struct stGamePlayerInfo
{
	GAME_STRING_ID				str_thePlayerName;
	DWORD						str_dwSerial;

	struct
	{
		DWORD						str_dwVFunAddr;
		DWORD						str_dwUnKnow1;
		struct
		{
			DWORD					str_dwVFunAddr;
			GAME_STRING_ID			str_thePlayerName;
			LONGLONG				str_llPlayerSerial;
			DWORD					str_dwPlayerGender;
			GType::enJobCategory	str_dwClassType;			// 剑斗士 = 1,斩狂 = 3, 控魔师 = 9 和创建角色时的ClassType相同
			DWORD					str_dwCreatedDateTime[4];
			BYTE					str_bIsDeleted;
			DWORD					str_dwUnKnow2;
		}theStaticPlayerInfo;
		struct
		{
			DWORD					str_dwVFunAddr;
			DWORD					str_dwGameLv;
			DWORD					str_dwGameXp;
			DWORD					str_dwUnKnow3[2];
			int						str_dwGold;
			DWORD					str_dwFriendPoint;
			BYTE					str_bFriendApplyDenyOption;
			DWORD					str_dwFriendApplyDenyOption;
			DWORD					str_dwResurrectionStoneCount_PcCafe;
			DWORD					str_dwResurrectionStoneCount_Daily;
			DWORD					str_dwResurrectionStoneCount;
			DWORD					str_dwTalentLv;
			int						str_dwFatigue;
			DWORD					str_dwEventFatigue;
			DWORD					str_dwPremiumFatigue;
			DWORD					str_dwMembershipFatigue;
			DWORD					str_dwSpentFatigue;
			DWORD					str_dwTierAttackPower;
			// ....
		}theDynamicPlayerInfo;
		// ....
	}thePlayerInfo;

};

bool GPlayerSelf::Update()
{
	for ( int i = 0;i != 20; ++i )
	{
		if (TryUpdateImpl())
			return true;
		DummySleep(400);
		LOG_O(Log_debug) << "游戏对象更新失败，再次进行更新";
	}
	assert(false);
	return false;
}

bool GPlayerSelf::HitTargets( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs )
{
	using namespace GClass;
	using namespace GFuncCall;
	if ( !target_gameobjs )
	{
		assert(false);
		return false;
	}
	auto skillid_hash = skill_mgr_.GetSkillIdHashPair(skill_obj);
	if ( !skillid_hash )
	{
		assert(false);
		return false;
	}
	
	auto equip_weapon = packet_.GetTabEquiped().FindBySlotIndex(GType::kEP_Weapon);
	if ( !equip_weapon )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "不能没有穿戴武器";
		assert(false);
		return false;
	}
	auto weapon_status = equip_weapon->GetItemStatusValueMgr().FindByStatusId(kStatusName_AttackPower);
	if ( !weapon_status )
	{
		assert(false);
		return false;
	}
	auto weapon_attack_power = weapon_status->GetTheValue();;
	//LOG_ENTER_FUNC;
	//LOG_O(Log_trace) << "attack power->" << weapon_attack_power;

	CMDS_Msg_SkillHit skill_msg;
	DWORD projectile_hash = skillid_hash->second;
	const auto& kSkillName = skill_obj.GetStrId();
	/*DWORD projectile_hash = CalcHash_DWORD(L"HandShot");
	std::wstring kSkillName = L"HandShot";*/

#ifdef _ENABLE_GAME_PACKET_TEST_
	DWORD dwOffset = 0;
	dwOffset = JFZR_SkillHitPacketHeader(g_send_packet_buffer_test_, 1);
	auto first_offset = dwOffset;
	int obj_test_cnt = 0;
#endif

	float attack_power_rate = 0.;
	auto skill_template = GSkillTemplateMgr::GetMe().FindByStrId(kSkillName);
	if (skill_template)
	{
		auto skill_step = skill_template->FindStepByLvl(skill_obj.GetLvl());
		if (skill_step)
			attack_power_rate = skill_step->GetAttackPowerRate();
	}

	auto projectile_serial = GetProjectileSerial();
	for ( target_gameobjs->First(); !target_gameobjs->IsDone(); target_gameobjs->Next()
#ifdef _ENABLE_GAME_PACKET_TEST_
		, ++obj_test_cnt 
#endif
		)
	{
		auto gameobj = target_gameobjs->CurItem();
		if ( !gameobj )
		{
			assert(false);
			return false;
		}
		auto obj_cur_hp = gameobj->GetCurHp();
		if ( obj_cur_hp <= 0 )
			continue;
		if ( obj_cur_hp > weapon_attack_power )
			obj_cur_hp = weapon_attack_power;
		//LOG_O(Log_trace) << "obj_cur_hp->" << obj_cur_hp;
		auto skill_timestamp = CMDS_Msg_SkillHit::GetSkillTimestamp();
		CMDS_Msg_SkillHit::HittedItem hitted_item;
		hitted_item.AcceptParams(gameobj->GetObjSerial(), projectile_serial,
			GetObjSerial(), projectile_hash, 
			CalcHash_DWORD(GetJobName(), kSkillName),
			skill_obj.GetId(), skill_obj.GetLvl(), skill_timestamp, attack_power_rate, obj_cur_hp);

		skill_msg.hitted_items_.PushBack(hitted_item);
		//////////////////////////////////////////////////////////////////////////
/*
#ifdef _ENABLE_GAME_PACKET_TEST_
		attack_power_rate = hitted_item.attack_power_rate_;
		dwOffset += JFZR_SkillHitPacketBody(
			g_send_packet_buffer_test_+dwOffset,
			obj_test_cnt,
			projectile_serial,
			gameobj->GetObjSerial(),
			GetObjSerial(),
			attack_power_rate,				// 攻击速度
			projectile_hash,		// GetSigHash(L"DoubleSlash_B") 具体技能具体添加 技能名为 DoubleSlash
			GetHash(GetJobName().c_str(), kSkillName.c_str()),
			skill_obj.GetId(), skill_obj.GetLvl(),
			skill_timestamp,
			false,
			false,		 //,
			4, // 			wPhysicalAttribute,
			L"ELEC" // 			wszElementalAttribute
			, gameobj->GetCurHp());
#endif*/
		//////////////////////////////////////////////////////////////////////////
	}
	if (skill_msg.hitted_items_.Empty())
	{
		//assert(false);
		return false;
	}
	
	auto& tcp_send = TcpSend::GetMe();
/*
#ifdef _ENABLE_GAME_PACKET_TEST_
	JFZR_SkillHitPacketHeader(g_send_packet_buffer_test_, obj_test_cnt);
	auto error_buff = tcp_send.SendMsgTest(skill_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + dwOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{
			auto psendbuff = (SEND_REQUEST_PROJECTILE_HIT_BODY*)(g_send_packet_buffer_test_ + first_offset);
			psendbuff += 0;
		}
		return false;
	}
#endif*/
	return tcp_send.SendMsg(skill_msg);
}

GSkillMgr& GPlayerSelf::GetSkillMgr()
{
	return skill_mgr_;
}

void GPlayerSelf::Test()
{	
	using namespace GClass;
	auto& item_mgr = packet_.GetTabEquiped();
	item_mgr.RebuildUpdateAll();
	auto weapon_equip = item_mgr.FindBySlotIndex(GType::kEP_Weapon);
	if ( !weapon_equip )
	{
		assert(false);
		return;
	}
	auto& status_cont = weapon_equip->GetItemStatusValueMgr().GetAllNodes();
	LOG_O(Log_trace) << status_cont.size();
	for ( auto& v : status_cont )
	{
		LOG_O(Log_trace) << "status id->" << v.GetStatusId() << "\t value->" << v.GetTheValue();
	}
}

bool GPlayerSelf::HitTarget( const GSkillObj& skill_obj, const GameObjBase::SelfPtrT& target_gameobj )
{
	using namespace GClass;
	using namespace GFuncCall;
	if ( !target_gameobj )
	{
		assert(false);
		return false;
	}
	auto skillid_hash = skill_mgr_.GetSkillIdHashPair(skill_obj);
	if ( !skillid_hash )
	{
		assert(false);
		return false;
	}
	float attack_power_rate = 0.;
	auto skill_template = GSkillTemplateMgr::GetMe().FindByStrId(skill_obj.GetStrId());
	if (skill_template)
	{
		auto skill_step = skill_template->FindStepByLvl(skill_obj.GetLvl());
		if (skill_step)
			attack_power_rate = skill_step->GetAttackPowerRate();
	}
	//////////////////////////////////////////////////////////////////////////
	//修改角色本地坐标
	if ( !ChangeRoleCurPos(target_gameobj->GetPos()) )
	{
		assert(false);
		return false;
	}
	DummySleep(50);
	//////////////////////////////////////////////////////////////////////////

	CMDS_Msg_SkillHit skill_msg;
	DWORD projectile_hash = skillid_hash->second;
	//下面这个地方以后找到相关数据后再修正
	const auto& kSkillName = skill_obj.GetStrId();
	CMDS_Msg_SkillHit::HittedItem hitted_item;
	auto projectile_serial = GetProjectileSerial();
	hitted_item.AcceptParams(target_gameobj->GetObjSerial(), projectile_serial,
		GetObjSerial(), projectile_hash, 
		CalcHash_DWORD(GetJobName(), kSkillName),
		skill_obj.GetId(), skill_obj.GetLvl(), CMDS_Msg_SkillHit::GetSkillTimestamp(), attack_power_rate, target_gameobj->GetCurHp());

	skill_msg.hitted_items_.PushBack(hitted_item);

	auto& tcp_send = TcpSend::GetMe();
	return tcp_send.SendMsg(skill_msg);
}

bool GPlayerSelf::HitToDie( const GSkillObj& skill_obj, const GameObjBase::SelfPtrT& target_gameobj )
{
	if ( !target_gameobj )
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	auto obj_serial = target_gameobj->GetObjSerial();
	auto& obj_mgr = GameObjMgr::GetMe();
	do 
	{
		auto the_obj = obj_mgr.FindBySerial(obj_serial);
		if ( !the_obj )
			break;
		if ( !HitTarget(skill_obj, the_obj) )
			break;
		DummySleep(100);
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
	} while (true);
	//////////////////////////////////////////////////////////////////////////
	/*GameObjBase::WeakPtrT gameobj_weak(target_gameobj);
	target_gameobj->Update();
	while ( target_gameobj->GetCurHp() > 0 )
	{
		HitTarget(skill_obj, target_gameobj);
		DummySleep(50);
		if (gameobj_weak.expired())
			break;
		target_gameobj->UpdateHpMp();
	}*/
	return true;
}

bool GPlayerSelf::HitMultiToDie( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs )
{
	if ( !target_gameobjs )
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	struct stTargetInfo{
		DWORD			obj_serial_;
		int				hp_;
		int				hp_cnt_;
		bool			discard_;
	};
	std::vector<stTargetInfo> targets_info;
	for ( target_gameobjs->First(); !target_gameobjs->IsDone(); target_gameobjs->Next() )
	{
		auto& gameobj = target_gameobjs->CurItem();
		if ( !gameobj )
		{
			assert(false);
			continue;
		}
		stTargetInfo tmp_target = {
			gameobj->GetObjSerial(), gameobj->GetCurHp(), 0, false
		};
		targets_info.push_back(tmp_target);
	}
	if (targets_info.empty())
		return true;
	
	//packet_.GetTabEquiped().RebuildUpdateAllSync();

	auto& obj_mgr = GameObjMgr::GetMe();
	do 
	{
		auto target_it = targets_info.begin(), target_it_end = targets_info.end();
		auto the_targets = obj_mgr.TraverseIf([target_it, target_it_end]( GameObjBase::SelfPtrT& the_obj ){
			assert(the_obj);
			auto it = std::find_if(target_it, target_it_end, [&the_obj]( stTargetInfo& the_target ){
				if (the_target.discard_)
					return false;
				if ( the_target.obj_serial_ == the_obj->GetObjSerial() )
				{
					auto cur_hp = the_obj->GetCurHp();
					if ( cur_hp <= 0 )
					{
						the_target.discard_ = true;
						return false;
					}
					if ( the_target.hp_ == cur_hp )
					{
						if ( ++the_target.hp_cnt_ >= 30 )
						{
							the_target.discard_ = true;
							return false;
						}
					}
					else
					{
						assert(the_target.hp_ > cur_hp);
						the_target.hp_ = cur_hp;
						the_target.hp_cnt_ = 0;
					}
					return true;
				}
				return false;
			});
			return it != target_it_end;
		});
		assert(the_targets);
		if ( !HitTargets(skill_obj, the_targets) )
			break;
		//DummySleep(50);
		DummySleep(150);
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
	} while (true);
	
	//////////////////////////////////////////////////////////////////////////
	/*while (true)
	{
		if ( !HitTargets(skill_obj, target_gameobjs) )
			break;
		DummySleep(50);
		for ( target_gameobjs->First(); !target_gameobjs->IsDone(); target_gameobjs->Next() )
		{
			auto& gameobj = target_gameobjs->CurItem();
			if ( !gameobj )
				continue;
			if ( !gameobj->UpdateHpMp() )
			{
				assert(false);
				break;
			}
		}
	}*/
	return true;
}

GPlayerSelf::GPlayerSelf()
{
	fatigue_data_ = 0;
	left_sp_ = 0;
	job_ = GType::kJC_Invalid;
	gold_ = 0;
	job_branch_skill_template_ = nullptr;
	buy_other_min_gold_ = 1000;
	player_died_ = false;
}

int GPlayerSelf::GetFatigueData() const
{
	return fatigue_data_;
}

GPacket& GPlayerSelf::GetPacket()
{
	return packet_;
}

bool GPlayerSelf::PickUpItem( const GameObjBase::SelfPtrT& drop_item )
{
	if ( !drop_item )
	{
		assert(false);
		return false;
	}
	if ( kGOT_Unknown != drop_item->GetGameObjType() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "obj is not kGOT_Unknown";
		assert(false);
		return false;
	}
	auto serial = drop_item->GetObjSerial();
	if ( !serial )
	{
		assert(false);
		return false;
	}
	CMDS_Msg_PickUpItem msg;
	msg.item_serial_ = serial;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

void GPlayerSelf::Test1()
{
	using namespace GClass;
	auto& obj_serial_mgr = stPreActionPlayTimeMgr::GetMe();
	if ( !obj_serial_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return;
	}

	auto& gom = GameObjMgr::GetMe();
	GameObj_ParserAny obj_parse;
	const auto& reset_obj_parse = MakeCleanAtScopeExit([&gom](){
		gom.SetGameObjParser(nullptr);
	});
	gom.SetGameObjParser(&obj_parse);
	gom.ResetForceUpdate();
	gom.RebuildUpdateAll();
	const auto& gameobjs = gom.TraverseIf([]( GameObjBase::SelfPtrT& game_obj ){
		return kGOT_Monster == game_obj->GetGameObjType() && game_obj->GetCurHp() > 0;
	});
	FbMonstersMove(gameobjs);
	DummySleep(500);
}

bool GPlayerSelf::PickUpItems( GameObjMgr::GameObjIterPtr& drop_items )
{
	for ( drop_items->First(); !drop_items->IsDone(); drop_items->Next() )
	{
		auto gameobj = drop_items->CurItem();
		if ( !gameobj )
		{
			assert(false);
			return false;
		}
		if ( !PickUpItem(gameobj) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "can not pick up item";
			assert(false);
			return false;
		}
	}
	return true;
}

bool GPlayerSelf::FbStandToPos( const GType::stGameVector& pos )
{
	if ( !obj_serial_ || obj_job_name_.empty() )
	{
		assert(false);
		return false;
	}
	auto& tcp_send = GClass::TcpSend::GetMe();
	CMDS_Msg_DirectionSyncPacket second_msg(obj_serial_, pos);
	if ( !tcp_send.SendMsg(second_msg) )
	{
		assert(false);
		return false;
	}
	return true;
	/*CMDS_Msg_FbWalk msg;
	msg.AcceptStand(obj_serial_, obj_job_name_, pos);
	return tcp_send.SendMsg(msg);*/
}

bool GPlayerSelf::FbStandToGameObj( const GameObjBase::SelfPtrT& gameobj )
{
	if ( !gameobj )
	{
		assert(false);
		return false;
	}
	return FbStandToPos(gameobj->GetPos());
}

bool GPlayerSelf::FbStandToGameObjSide( const GameObjBase::SelfPtrT& gameobj )
{
	if ( !gameobj )
	{
		assert(false);
		return false;
	}
	auto pos = gameobj->GetPos();
	pos.z_ += 1;
	return FbStandToPos(pos);
}

bool GPlayerSelf::StandAndPickUpItem( const GameObjBase::SelfPtrT& drop_item )
{
	if ( !drop_item )
	{
		assert(false);
		return false;
	}
	if ( kGOT_Unknown != drop_item->GetGameObjType() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "obj is not kGOT_Unknown";
		assert(false);
		return false;
	}
	if ( !FbStandToGameObj(drop_item) )
	{
		assert(false);
		return false;
	}
	auto serial = drop_item->GetObjSerial();
	if ( !serial )
	{
		assert(false);
		return false;
	}
	DummySleep(50);
	CMDS_Msg_PickUpItem msg;
	msg.item_serial_ = serial;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::StandAndPickUpItems( GameObjMgr::GameObjIterPtr& drop_items )
{
	int cnt = 0;
	for ( drop_items->First(); !drop_items->IsDone(); drop_items->Next() )
	{
		auto gameobj = drop_items->CurItem();
		if ( !gameobj )
		{
			assert(false);
			return false;
		}
		if ( !StandAndPickUpItem(gameobj) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "can not pick up item";
			assert(false);
			return false;
		}
		DummySleep(50);
		++cnt;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "捡物品的数量" << cnt;
	return true;
}

bool GPlayerSelf::EnterCloneMap( const GClass::AllowedVillageCloneMap& clone_map, GType::enFbDifficulty difficulty )
{
	using namespace GType;
	if ( !clone_map.IsCloneMap() )
	{
		assert(false);
		return false;
	}
	const auto& clone_map_id = clone_map.GetMapId();
	if (clone_map_id.empty())
	{
		assert(false);
		return false;
	}
	auto clone_map_difficulty = clone_map.GetLvl();
	if ( difficulty < kFD_Normal )
	{
		assert(false);
		return false;
	}
	if ( difficulty > clone_map_difficulty )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "难度等级过高，挑战不了，difficulty->" << difficulty << "\tclone map difficulty->" << clone_map_difficulty;
		assert(false);
		return false;
	}
	//assert(false);
	CMDS_Msg_EnterCloneMap msg;
	msg.AcceptEnter(clone_map_id, difficulty);
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::EnterAnotherRoom( const GRoomDoor& door )
{
	using namespace GClass;
	using namespace GFuncCall;
	using namespace GAddr;
	LOG_ENTER_FUNC;
	//////////////////////////////////////////////////////////////////////////
	if (player_died_)
	{
		player_died_ = false;
		LOG_O(Log_warning) << "角色死亡了，直接返回城镇";
		if ( !ReturnCity() )
		{
			assert(false);
			return false;
		}
		Update();
		if ( cur_hp_ != hp_limit_ )
		{
			LOG_O(Log_warning) << "角色死亡返回城镇后，虚弱，等待恢复一定体力后再继续刷副本或任务";
			TimerDoWaitUntil([this](){
				return cur_hp_ > hp_limit_;
			}, [this](){
				UpdateHpMp();
			}, 1000, 1000 * 60 * 4);
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	LOG_O(Log_trace) << "清空对象黑名单";
	GameObjMgr::GetMe().ResetNpcBlacklist();
	if (obj_name_.empty())
	{
		assert(false);
		return false;
	}
	const auto& kGateName = door.GetGateName();
	if (kGateName.empty())
	{
		assert(false);
		return false;
	}
	if ( !obj_serial_ )
	{
		assert(false);
		return false;
	}
	auto& tcp_send = GClass::TcpSend::GetMe();
	int nOffset = 0;
	DWORD error_buff = 0;
	//////////////////////////////////////////////////////////////////////////
	auto& local_room = LocalRoomPosMgr::GetMe();
	GType::stGameVector the_gate_pos(door.GetGatePos());
	the_gate_pos.z_ = 0.f;
	if ( !local_room.FindByXY(the_gate_pos.x_, the_gate_pos.y_, the_gate_pos.z_) )
	{
		//assert(false);
		LOG_O(Log_debug) << "local_room.FindByXY没找到z坐标";
		//return false;
	}
	/*CMDS_Msg_FbWalk fb_walk_msg;
	auto pre_action_time = GetPreActionPlayTime();
	auto action_serial = LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	auto rand_seed = g_pRandomSeekCall_.Call<DWORD>();
	auto tick_count = (float)GetTickCount();*/
	//////////////////////////////////////////////////////////////////////////
	/*fb_walk_msg.AcceptFastRun(obj_serial_, obj_job_name_, the_gate_pos, direction_, pre_action_time, action_serial, rand_seed, tick_count);
	if ( !tcp_send.SendMsg(fb_walk_msg) )
	{
		assert(false);
		return false;
	}*/
	/*pre_action_time = GetPreActionPlayTime();
	action_serial = LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	rand_seed = g_pRandomSeekCall_.Call<DWORD>();
	tick_count = (float)GetTickCount();*/
	//////////////////////////////////////////////////////////////////////////
	/*fb_walk_msg.AcceptStand(obj_serial_, obj_job_name_, the_gate_pos, pre_action_time, action_serial, rand_seed, tick_count);
#ifdef _ENABLE_GAME_PACKET_TEST_
	nOffset = JFZR_ActionSyncPacket(g_send_packet_buffer_test_, obj_serial_, obj_job_name_.c_str(), L"Stand", the_gate_pos.x_, the_gate_pos.y_, the_gate_pos.z_,
		0, 0, 0, 0x399E, 0xB9B2, 1, pre_action_time, action_serial, rand_seed, tick_count);
	error_buff = tcp_send.SendMsgTest(fb_walk_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{

		}
		return false;
	}
#endif
	LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	if ( !tcp_send.SendMsg(fb_walk_msg) )
	{
		assert(false);
		return false;
	}*/

	LOG_O(Log_trace) << "过图坐标信息,x:" << the_gate_pos.x_ << " y:" << the_gate_pos.y_ << " z:" << the_gate_pos.z_;
	CMDS_Msg_DirectionSyncPacket second_msg(obj_serial_, the_gate_pos);
	if ( !tcp_send.SendMsg(second_msg) )
	{
		assert(false);
		return false;
	}
#ifdef _ENABLE_GAME_PACKET_TEST_
	nOffset = JFZR_GotoNextRoom2(g_send_packet_buffer_test_, obj_serial_, the_gate_pos.x_, the_gate_pos.y_, the_gate_pos.z_, 0x3BFA, 0x2CB5);
	error_buff = tcp_send.SendMsgTest(second_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{

		}
		return false;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	CMDS_Msg_StepInWarpGate step_msg(obj_name_, kGateName);
	if ( !tcp_send.SendMsg(step_msg) )
	{
		assert(false);
		return false;
	}

#ifdef _ENABLE_GAME_PACKET_TEST_
	nOffset = JFZR_GotoNextRoom1(g_send_packet_buffer_test_, obj_name_.c_str(), kGateName.c_str());
	error_buff = tcp_send.SendMsgTest(step_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{
			
		}
		return false;
	}
#endif
	
	using namespace GAddr;
	CMDS_Msg_ChangeSector third_msg(kGateName);
	auto attack_speed = GetSpeed(JFZR_REVERSAL_GET_ATTACK_SPEED_ADDR);
	auto move_speed = GetSpeed(JFZR_REVERSAL_GET_MOVE_SPEED_ADDR);
	auto evade_rating = GetSpeed(JFZR_REVERSAL_GET_EVADE_RATING_ADDR);
	third_msg.AcceptParam(g_through_room_valid_data_x, g_through_room_valid_data_y, g_through_room_dummy_exist, attack_speed, move_speed, evade_rating);
	auto valid_x = g_through_room_valid_data_x;
	auto valid_y = g_through_room_valid_data_y;
	auto dummy_exist = g_through_room_dummy_exist;
	g_through_room_valid_data_x = 0;
	g_through_room_valid_data_y = 0;
	g_through_room_dummy_exist = false;
	if ( !tcp_send.SendMsg(third_msg) )
	{
		assert(false);
		return false;
	}
#ifdef _ENABLE_GAME_PACKET_TEST_
	nOffset = JFZR_ChangeSector(g_send_packet_buffer_test_, kGateName.c_str(), valid_y, valid_x, dummy_exist, attack_speed, move_speed, evade_rating);
	error_buff = tcp_send.SendMsgTest(third_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{

		}
		return false;
	}
#endif
	return true;
}

bool GPlayerSelf::ReturnCity()
{
	using namespace GClass;
	//assert(false);
	auto& gm = GMap::GetMe();
	if (gm.IsTown())
		return true;
	CMDS_Msg_ReturnCity msg;
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}

	if ( !GAmbient::GetMe().UntilCityUiNow(1000 * 60 * 5) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "直到返回城镇超时失败";
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::RestartCloneMap()
{
	if ( !UpdateFatigueData() )
	{
		assert(false);
		return false;
	}
	if ( fatigue_data_ <= 0 )
	{
		assert(0 == fatigue_data_);
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有疲劳值了，不能进入副本";
		return false;
	}
	auto gmap = GMap::GetInstance();
	if ( !gmap )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "gmap is empty";
		assert(false);
		return false;
	}
	if (gmap->IsTown())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "在城镇中，无法重启副本";
		assert(false);
		return false;
	}
	auto difficulty = gmap->GetDifficulty();
	if ( difficulty < GType::kFD_Normal || difficulty >= GType::kFD_Invalid )
	{
		assert(false);
		return false;
	}
	const auto& clone_map_id = gmap->GetMapId();
	if (clone_map_id.empty())
	{
		assert(false);
		return false;
	}
	/*auto room = GRoom::GetInstance();
	if ( !room )
	{
		assert(false);
		return false;
	}
	const auto& kRoomId = room->GetRoomId();
	if (kRoomId.empty())
	{
		assert(false);
		return false;
	}*/
	CMDS_Msg_RestartStage msg;
	msg.clone_map_name_ = clone_map_id;
	msg.difficulty_ = difficulty;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::RepairAllWearEquip()
{
	CMDS_Msg_RepairWearEquip msg;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::EquipItemSync( const GItem& item )
{
	using namespace GType;
	if ( !item.IsEquip() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不是装备类型，物品名->" << item.GetItemId();
		assert(false);
		return false;
	}
	auto tab_category = item.GetCurTabCategory();
	if ( GType::kTC_Normal != tab_category )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab category不是普通，物品名->" << item.GetItemId() << "\ttab category->" << tab_category;
		assert(false);
		return false;
	}
	auto equip_part = item.GetEquipPart();
	if ( equip_part >= kEP_Invalid )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "equip slot error，物品名->" << item.GetItemId() << "\tequip part->" << equip_part;
		assert(false);
		return false;
	}
	auto inventory_slot = item.GetSlotIndex();
	if ( inventory_slot < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "inventory slot error，物品名->" << item.GetItemId() << "\tindex->" << inventory_slot;
		assert(false);
		return false;
	}
	const auto item_serial = item.GetItemSerial();
	if ( !item_serial )
	{
		assert(false);
		return false;
	}
	auto& tab_equiped = packet_.GetTabEquiped();
	if (tab_equiped.FindBySerial(item_serial))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品已经穿在身上了，无法再次穿戴,item id->" << item.GetItemId();
		assert(false);
		return false;
	}
	CMDS_Msg_EquipItem msg;
	msg.equip_part_ = equip_part;
	msg.inven_slot_ = inventory_slot;
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	if ( !TimerDoWaitUntil([&tab_equiped, item_serial](){
		return !!tab_equiped.FindBySerial(item_serial);
	}, [&tab_equiped](){
		if ( !tab_equiped.RebuildUpdateAllSync() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新身上装备数据失败";
		}
	}, 80, 4 * 1000) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "装备物品超时失败,item serial->" << item_serial;
		return false;
	}
	if ( !packet_.GetTabNormal().RebuildUpdateAllSync() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "更新普通栏数据失败";
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::UnEquipItemSync( const GItem& item )
{
	using namespace GType;
	if ( !item.IsEquip() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不是装备类型，物品名->" << item.GetItemId();
		assert(false);
		return false;
	}
	auto tab_category = item.GetCurTabCategory();
	if ( GType::kTC_Equiped != tab_category )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab category不是equip，物品名->" << item.GetItemId() << "\ttab category->" << tab_category;
		assert(false);
		return false;
	}
	auto equip_part = item.GetEquipPart();
	if ( equip_part >= kEP_Invalid )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "equip slot error，物品名->" << item.GetItemId() << "\tequip part->" << equip_part;
		assert(false);
		return false;
	}
	auto item_mgr = item.GetParent();
	if ( !item_mgr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "item mgr is empty";
		assert(false);
		return false;
	}
	auto inventory_slot = item_mgr->GetFreeSlotIndex();
	if ( inventory_slot < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "背包已满，无法脱掉装备，物品名->" << item.GetItemId();
		//assert(false);
		return false;
	}
	auto& tab_normal = packet_.GetTabNormal();
	const auto item_serial = item.GetItemSerial();
	if (tab_normal.FindBySerial(item_serial))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品已经在背包里了，无法脱下,item id->" << item.GetItemId();
		assert(false);
		return false;
	}
	CMDS_Msg_UnEquipItem msg;
	msg.equip_part_ = equip_part;
	msg.inven_slot_ = inventory_slot;
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	
	if ( !TimerDoWaitUntil([&tab_normal, item_serial](){
		return !!tab_normal.FindBySerial(item_serial);
	}, [&tab_normal](){
		if ( !tab_normal.RebuildUpdateAllSync() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新身上装备数据失败";
		}
	}, 80, 10 * 1000) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "脱掉装备超时失败,item id->" << item_serial;
		return false;
	}
	if ( !packet_.GetTabEquiped().RebuildUpdateAllSync() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "更新身上装备数据失败";
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::ChangeVillage( const std::wstring& village_gate_id )
{
	auto& tr = GTownRoom::GetMe();
	if ( !tr.Update() )
	{
		assert(false);
		return false;
	}
	auto gate = tr.GetDoorById(village_gate_id);
	if ( !gate )
	{
		assert(false);
		return false;
	}
	if ( !VillageStandToPos(gate->GetGatePos()) )
	{
		assert(false);
		return false;
	}
	CMDS_Msg_ChangeVillage msg;
	msg.village_gate_id_ = village_gate_id;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::LearnSkill( const GSkillObj& skill_obj, int lvl_addition )
{
	if ( lvl_addition <= 0 )
	{
		assert(false);
		return false;
	}
	const auto& skill_name = skill_obj.GetStrId();
	if (skill_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "技能等级名为空";
		assert(false);
		return false;
	}
	auto skill_lvl = skill_obj.GetLvl();
	if ( skill_lvl < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "技能等级数值不正确，无法升级";
		assert(false);
		return false;
	}
	CMDS_Msg_LearnSkill msg;
	msg.skill_name_ = skill_name;
	msg.skill_lvl_ = skill_lvl + lvl_addition;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::SendMail( const std::wstring& receive_role_name, const std::wstring& mail_title, const std::wstring& mail_content, DWORD gold, const GItem* item )
{
	using namespace GType;
	if ( player_lvl_ < kMailMinLvl )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "未达到21级，不能发邮件";
		assert(false);
		return false;
	}
	if (receive_role_name.empty())
	{
		assert(false);
		return false;
	}
	if (mail_title.empty())
	{
		assert(false);
		return false;
	}
	if (mail_content.empty())
	{
		assert(false);
		return false;
	}
	CMDS_Msg_SendMail msg;
	if (item)
	{
		auto tab_category = item->GetCurTabCategory();
		if ( kTC_Normal != tab_category && kTC_Consume != tab_category && kTC_Material != tab_category )
		{
			assert(false);
			return false;
		}
		auto item_mgr = item->GetParent();
		if ( !item_mgr )
		{
			assert(false);
			return false;
		}
		const auto& tab_name = item_mgr->GetTabName();
		if (tab_name.empty())
		{
			assert(false);
			return false;
		}
		auto slot_index = item->GetSlotIndex();
		if ( slot_index < 0 )
		{
			assert(false);
			return false;
		}
		auto item_cnt = item->GetItemCnt();
		if ( item_cnt < 1 )
		{
			assert(false);
			return false;
		}
		msg.items_.PushBack(CMDS_Msg_SendMail::stMailItems(tab_name, slot_index, item_cnt));
	}
	msg.receiver_role_name_ = receive_role_name;
	msg.mail_title_ = mail_title;
	msg.mail_content_ = mail_content;
	msg.gold_ = gold;

	auto& tcp_send = GClass::TcpSend::GetMe();
#ifdef /*_ENABLE_GAME_PACKET_TEST_*/false
	typedef struct  
	{
		wchar_t		str_wszReceiverId[256];
		wchar_t		str_wszTitle[256];
		wchar_t		str_wszMessage[256];
		UINT64		str_unGold;
		struct  
		{
			wchar_t		str_wszInventoryTab[256];
			int			str_nItemSlotIndex;
			int			str_nCount;
		}str_Item[100];
		int				str_nItemCount;
	}MAIL_SEND_ITEM,*PMAIL_SEND_ITEM;
	MAIL_SEND_ITEM tmp_mail_send = { 0 };
	std::copy(receive_role_name.begin(), receive_role_name.end(), tmp_mail_send.str_wszReceiverId);
	std::copy(mail_title.begin(), mail_title.end(), tmp_mail_send.str_wszTitle);
	std::copy(mail_content.begin(), mail_content.end(), tmp_mail_send.str_wszMessage);
	tmp_mail_send.str_unGold = gold;
	tmp_mail_send.str_nItemCount = !!item;
	if (item)
	{
		auto item_mgr = item->GetParent();
		if ( !item_mgr )
		{
			assert(false);
			return false;
		}
		const auto& tab_name = item_mgr->GetTabName();
		if (tab_name.empty())
		{
			assert(false);
			return false;
		}
		std::copy(tab_name.begin(), tab_name.end(), tmp_mail_send.str_Item[0].str_wszInventoryTab);

		auto slot_index = item->GetSlotIndex();
		if ( slot_index < 0 )
		{
			assert(false);
			return false;
		}
		tmp_mail_send.str_Item[0].str_nItemSlotIndex = slot_index;

		auto item_cnt = item->GetItemCnt();
		if ( item_cnt < 1 )
		{
			assert(false);
			return false;
		}
		tmp_mail_send.str_Item[0].str_nCount = item_cnt;
	}

	PMAIL_SEND_ITEM ptheMailSend = &tmp_mail_send;
	int nOffset = 0;
	nOffset = JFZR_MailSendHeader(	g_send_packet_buffer_test_,
		ptheMailSend->str_wszTitle,
		ptheMailSend->str_wszReceiverId,
		ptheMailSend->str_wszMessage,
		ptheMailSend->str_unGold,
		ptheMailSend->str_nItemCount);
	auto first_offset = nOffset;
	for( int i=0; i<ptheMailSend->str_nItemCount; i++ )
	{
		nOffset += JFZR_MailSendBody(	g_send_packet_buffer_test_+nOffset,
			i,
			ptheMailSend->str_Item[i].str_wszInventoryTab,
			ptheMailSend->str_Item[i].str_nItemSlotIndex,
			ptheMailSend->str_Item[i].str_nCount);
	}

	nOffset += JFZR_MailSendTail(g_send_packet_buffer_test_+nOffset);
	auto error_buff = tcp_send.SendMsgTest(msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{
			auto psendbuff = (SEND_REQUEST_PROJECTILE_HIT_BODY*)(g_send_packet_buffer_test_ + first_offset);
			psendbuff += 0;
		}
		return false;
	}
#endif

	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::SendMail( const std::wstring& receive_role_name, const std::wstring& mail_title, const std::wstring& mail_content,
						   DWORD gold, GItemMgr::GameItemIterPtr& item_iter )
{
	using namespace GType;
	if ( player_lvl_ < kMailMinLvl )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "未达到21级，不能发邮件";
		assert(false);
		return false;
	}
	if (receive_role_name.empty())
	{
		assert(false);
		return false;
	}
	if (mail_title.empty())
	{
		assert(false);
		return false;
	}
	if (mail_content.empty())
	{
		assert(false);
		return false;
	}
	CMDS_Msg_SendMail msg;
	int item_mail_cnt = 0;
	for ( item_iter->First(); !item_iter->IsDone(); item_iter->Next() )
	{
		auto& item = item_iter->CurItem();
		auto tab_category = item.GetCurTabCategory();
		if ( kTC_Normal != tab_category && kTC_Consume != tab_category && kTC_Material != tab_category )
		{
			assert(false);
			return false;
		}
		auto item_mgr = item.GetParent();
		if ( !item_mgr )
		{
			assert(false);
			return false;
		}
		const auto& tab_name = item_mgr->GetTabName();
		if (tab_name.empty())
		{
			assert(false);
			return false;
		}
		auto slot_index = item.GetSlotIndex();
		if ( slot_index < 0 )
		{
			assert(false);
			return false;
		}
		auto item_cnt = item.GetItemCnt();
		if ( item_cnt < 1 )
		{
			assert(false);
			return false;
		}
		msg.items_.PushBack(CMDS_Msg_SendMail::stMailItems(tab_name, slot_index, item_cnt));
		if ( ++item_mail_cnt >= kSendMailMaxItemCnt )
			break;
	}
	if (msg.items_.Empty())
	{
		assert(false);
		return false;
	}
	msg.receiver_role_name_ = receive_role_name;
	msg.mail_title_ = mail_title;
	msg.mail_content_ = mail_content;
	msg.gold_ = gold;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::AcceptTask( const std::wstring& quest_id )
{
	if ( !GetTaskTemplate(quest_id) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的任务，task id->" << quest_id;
		return false;
	}
	return SendAcceptTask(quest_id);
}

bool GPlayerSelf::CompleteTask( const std::wstring& quest_id, const std::wstring& reward_item_id )
{
	auto the_quest_id = quest_id;
	auto the_reward_item_id = reward_item_id;
	if (the_quest_id.empty())
	{
		assert(false);
		return false;
	}
	if ( !the_reward_item_id.empty() )
	{
		if (packet_.GetTabNormal().PacketBeFulled())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "背包已满，无法提交完成任务";
			assert(false);
			return false;
		}
	}
	auto accepted_task = received_tasks_.FindTask(the_quest_id);
	if ( !accepted_task )
	{
		//assert(false);
		return false;
	}
	if ( !accepted_task->TaskCompletable() )
	{
		assert(false);
		return false;
	}

	auto task_template = accepted_task->GetTaskTemplate();
	if ( !task_template )
	{
		//assert(false);
		return false;
	}
	const auto& complete_npc = task_template->GetCompleteTaskNpc();
	if (complete_npc.empty())
	{
		assert(false);
		return false;
	}
	const auto& npc_obj = GameObjMgr::GetMe().FindByName(complete_npc);
	if ( !npc_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "城镇->" << village_name_ << " 中找不到完成->" << the_quest_id << " 任务的npc->" << complete_npc;
		return true;
	}
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if (gm.IsTown())
	{
		if ( !VillageStandToPos(npc_obj->GetPos()) )
		{
			assert(false);
			return false;
		}
	}
	else
	{
		if ( !FbStandToPos(npc_obj->GetPos()) )
		{
			assert(false);
			return false;
		}
	}

	CMDS_Msg_CompleteTask msg;
	msg.quest_id_ = the_quest_id;
	msg.reward_item_id_ = the_reward_item_id;
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}

	DummySleep(200);
	for ( int i = 0;i != 50; ++i )
	{
		if ( !received_tasks_.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		if ( !received_tasks_.FindTask(the_quest_id) )
			return true;
		//LOG_O(Log_trace) << "提交完成任务成功，任务id->" << quest_id << "\t等待次数->" << i + 1;
		DummySleep(200);
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "提交完成任务超时失败,任务ID->" << the_quest_id;
	return false;
}

void GPlayerSelf::TestAllowedV_CM()
{
	using namespace GClass;
	AllowedVillageCloneMapMgr& mgr = AllowedVillageCloneMapMgr::GetMe();
	mgr.RebuildUpdateAll();
}

bool GPlayerSelf::EnterCloneMapDefault( const GClass::AllowedVillageCloneMap& clone_map )
{
	return EnterCloneMap(clone_map, clone_map.GetLvl());
}

void GPlayerSelf::TestEnterCloneMap()
{
	using namespace GClass;
	auto& mgr = AllowedVillageCloneMapMgr::GetMe();
	auto iter = mgr.TraverseCloneMap();
	AllowedVillageCloneMap* cm_ptr = nullptr;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto& cm = iter->CurItem();
		if ( cm.GetMapId() == L"Ep02Stage02" )
		{
			cm_ptr = &cm;
			break;
		}
	}
	if ( !cm_ptr )
	{
		assert(false);
		return;
	}
	EnterCloneMap(*cm_ptr, GType::kFD_Master);
}

bool GPlayerSelf::KillAllMonster()
{
	auto& gom = GameObjMgr::GetMe();
	if (gom.IsGameObjsEmpty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "game objs is empty";
		return false;
	}
	auto the_skill = skill_mgr_.GetDefaultUseSkill();
	if ( !the_skill )
	{
		if ( !LearnAllSkills() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "无技能，学习技能竟然失败";
			return false;
		}
		if ( !TimerDoWaitUntil([this]() -> bool {
			return !!skill_mgr_.GetDefaultUseSkill();
		}, [this](){
			Update();
		}, 500, 1000 * 60 * 5) )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "无技能，等待学习技能成功超时";
			return false;
		}
		the_skill = skill_mgr_.GetDefaultUseSkill();
		assert(the_skill);
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "开始发送怪物的移动包";
		GameObj_ParserAny obj_parse;
		const auto& reset_obj_parse = MakeCleanAtScopeExit([&gom](){
			gom.SetGameObjParser(nullptr);
		});
		gom.SetGameObjParser(&obj_parse);
		gom.ResetForceUpdate();
		gom.RebuildUpdateAll();
		const auto& gameobjs = gom.TraverseIf([]( GameObjBase::SelfPtrT& game_obj ){
			return kGOT_Monster == game_obj->GetGameObjType() && game_obj->GetCurHp() > 0;
		});
		for ( int i = 0; i != 3; ++i )
		{
			FbMonstersMove(gameobjs);
			DummySleep(500);
		}
	}	
	gom.RebuildUpdateAll();*/
	//////////////////////////////////////////////////////////////////////////

	return HitMultiToDie(*the_skill, gom.TraverseAliveGameObj(kGOT_Monster));
}

bool GPlayerSelf::PickUpAllItems()
{
	auto& mgr = GameObjMgr::GetMe();
	if (mgr.IsGameObjsEmpty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "game objs is empty";
		return true;
	}
	return StandAndPickUpItems(mgr.TraverseGameObjByType(kGOT_Unknown));
}

bool GPlayerSelf::FightThroughAllRooms_( const std::function<void()>& func )
{
	using namespace GAddr;
	using namespace luabind;
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal竟然为空";
		return false;
	}
	/*auto lua_script = glua.GetLuaScript();
	if ( !lua_script )
	{
		assert(false);
		return false;
	}*/
	struct RoomHelper{
		std::wstring				room_id_;
		std::vector<std::wstring>	entered_door_name_;
	};

	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if (gm.IsTown())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "尚未进入到副本，无法打完所有房间";
		return false;
	}
	const auto& kBossRoomId = gm.GetBossRoomName();
	if (kBossRoomId.empty())
	{
		assert(false);
		return false;
	}
	LOG_ENTER_FUNC;
	RoomHelper room_hlp_tmp;
	std::vector<RoomHelper> entered_rooms;
	auto& gr = GCloneMap::GetMe();
	const std::size_t kDelayTime = 1500;
	auto& obj_mgr = GameObjMgr::GetMe();
	while (true)
	{
		if ( !gr.Update() )
		{
			assert(false);
			return false;
		}
		const auto& cur_room_id = gr.GetRoomId();
		if (cur_room_id.empty())
		{
			assert(false);
			return false;
		}
		auto it = std::find_if(entered_rooms.begin(), entered_rooms.end(), [&cur_room_id]( const RoomHelper& room_hlp ){
			return room_hlp.room_id_ == cur_room_id;
		});
		RoomHelper* cur_room_hlp;
		if ( it == entered_rooms.end() )
		{
			room_hlp_tmp.room_id_ = cur_room_id;
			entered_rooms.push_back(room_hlp_tmp);
			cur_room_hlp = &entered_rooms.back();
		}
		else
			cur_room_hlp = &*it;
		if (cur_room_hlp->entered_door_name_.empty())
		{
			obj_mgr.RebuildUpdateAll();
			/*if ( player_lvl_ >= 5 )
				LocalMoveToMonsters();*/
			LOG_O(Log_trace) << "新进一个房间，开始KillAllMonster";
			KillAllMonster();
			//拾取所有金钱
			obj_mgr.RebuildUpdateAll();
			DestroyAllMisc();
			SendSkillActivityCntAsDlt();
			if ( kBossRoomId == cur_room_hlp->room_id_ )
				return true;
			DummySleep(300);
			obj_mgr.RebuildUpdateAll();
			AcceptLocalMapAnyAllTasks();
			CommitCompleteTasks();
			AcceptLocalMapAnyAllTasks();
			CommitCompleteTasks();
			LOG_O(Log_trace) << "新进一个房间，接受房间中任意任务完成";
			HitUntilClearCurSector(gr.GetRoomId());
		}
		auto next_door = gr.GetDoorExcept(cur_room_hlp->entered_door_name_);
		if ( !next_door )
		{
			assert(false);
			return false;
		}
		auto& door_name = next_door->GetGateName();
		assert(!door_name.empty());
		if ( std::find(cur_room_hlp->entered_door_name_.begin(), cur_room_hlp->entered_door_name_.end(), door_name) == cur_room_hlp->entered_door_name_.end() )
			cur_room_hlp->entered_door_name_.push_back(door_name);
		auto door_tmp = *next_door;
		DummySleep(300);
		obj_mgr.RebuildUpdateAll();
		KillAllMonster();
		PickUpAllItems();
		LOG_O(Log_trace) << "开始过图";
		if ( !EnterAnotherRoom(*next_door) )
		{
			//assert(false);
			LOG_O(Log_debug) << "进入下一个房间失败";
			return false;
		}
		DummySleep(kDelayTime);
		for ( int i = 0; i != 100; ++i )
		{
			if ( g_pLocalRoomObjAddr && !gr.Update() )
			{
				assert(false);
				return false;
			}
			if ( gr.GetRoomId() != cur_room_hlp->room_id_ )
				break;
			DummySleep(kDelayTime);
		}
	}
	return false;
}

bool GPlayerSelf::AutoLvlUpSkills()
{
	assert(left_sp_ >= 0);
	if ( 0 == left_sp_ )
		return true;

	auto dlt_skill = skill_mgr_.GetDefaultUseSkill();
	if ( !dlt_skill )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "无默认的技能，升级技能失败";
		return false;
	}
	if ( !UseOutSkillSp(*dlt_skill) )
		return false;
	assert(left_sp_ >= 0);
	if ( 0 == left_sp_ )
		return true;

	auto& skill_cont = skill_mgr_.GetSkills();
	for ( auto& skill : skill_cont )
	{
		if ( skill.GetId() == dlt_skill->GetId() )
		{
			assert(&skill == dlt_skill);
			continue;
		}
		if ( !UseOutSkillSp(skill) )
			break;
		assert(left_sp_ >= 0);
		if ( 0 == left_sp_ )
			break;
	}

	return true;
}

bool GPlayerSelf::UseOutSkillSp( GSkillObj& skill_obj )
{
	if ( !job_branch_skill_template_ )
	{
		assert(false);
		return false;
	}
	assert(player_lvl_ > 0);
	const auto& skill_id = skill_obj.GetStrId();
	assert(!skill_id.empty());
	auto& skill_template_mgr = GSkillTemplateMgr::GetMe();
	auto skill_template = skill_template_mgr.FindByStrId(skill_id);
	if ( !skill_template )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "技能模板不存在->" << skill_id;*/
		return true;
	}
	auto branch_skill_info = job_branch_skill_template_->GetSkillBranchs().FindSkillInfoById(skill_id);
	if ( !branch_skill_info )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "技能分支模板中，不存在这样的技能ID->" << skill_id;*/
		//assert(false);
		return true;
	}
	const auto& skill_steps = skill_template->GetSkillSteps().GetCont();
	if (skill_steps.empty())
	{
		assert(false);
		return false;
	}
	const auto kSkillCurLvl = skill_obj.GetLvl();
	const auto kSkillMaxLvl = branch_skill_info->GetSkillMaxLvl();
	if ( kSkillCurLvl >= kSkillMaxLvl )
	{
		assert(kSkillCurLvl == kSkillMaxLvl);
		return true;
	}
	assert(kSkillCurLvl >= 0);
	int left_sp = left_sp_;
	int skill_lvl_addition = 0;
	for ( const auto& skill_step : skill_steps )
	{
		const auto kStepLvl = skill_step.GetLvl();
		if ( kStepLvl > kSkillMaxLvl )
			break;
		if ( kStepLvl <= kSkillCurLvl )
			continue;
		if ( skill_step.GetMinLvlToLearn() > player_lvl_ )
			break;
		auto left_sp_tmp = left_sp - skill_step.GetSpToLearn();
		if ( left_sp_tmp < 0 )
			break;
		++skill_lvl_addition;
		left_sp = left_sp_tmp;
	}
	if ( 0 == skill_lvl_addition )
		return true;
	assert(skill_lvl_addition > 0 && skill_lvl_addition < 100);
	if ( !LearnSkill(skill_obj, skill_lvl_addition) )
	{
		assert(false);
		return false;
	}
	assert(left_sp >= 0);
	left_sp_ = left_sp;
	return true;
}

bool GPlayerSelf::PickUpAllGold()
{
	auto& mgr = GameObjMgr::GetMe();
	if (mgr.IsGameObjsEmpty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "game objs is empty";
		return false;
	}
	return StandAndPickUpItems(mgr.TraverseIf([]( GameObjBase::SelfPtrT& game_obj ){
		if ( kGOT_Unknown == game_obj->GetGameObjType() )
		{
			auto drop_item = (GUnknown*)(game_obj.get());
			if (drop_item->IsGold())
				return true;
		}
		return false;
	}));
}

const std::wstring& GPlayerSelf::ChooseTaskDftReward( const std::wstring& quest_id )
{
	auto task_template = GetTaskTemplate(quest_id);
	if ( !task_template )
	{
		//assert(TaskCurJobTemplateMgr::GetMe().GetAllNodes().size() > 90);
		if ( TaskCurJobTemplateMgr::GetMe().GetAllNodes().size() <= 90 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "ChooseTaskDftReward, quest id->" << quest_id << "\tall node size->" << TaskCurJobTemplateMgr::GetMe().GetAllNodes().size();
		}		
		return kEmptyWStr;
	}
	return ChooseTaskDftReward(*task_template);
}

const std::wstring& GPlayerSelf::ChooseTaskDftReward( const TaskCurJobTemplate& task_template )
{
	auto& quest_reward = task_template.GetRewardItems().GetAllNodes();
	if (quest_reward.empty())
		return kEmptyWStr;
	if ( !job_branch_skill_template_ )
	{
		assert(false);
		goto LABEL_SELECT_FIRST;
	}
	auto& item_template_mgr = GItemTemplateMgr::GetMe();
	const auto& item_template_cont = item_template_mgr.GetAllNodes();
	if (item_template_cont.empty())
	{
		assert(false);
		goto LABEL_SELECT_FIRST;
	}
	for ( const auto& quest_r : quest_reward )
	{
		const auto& reward_id = quest_r.GetRewardItemId();
		auto item_template = item_template_mgr.FindById(reward_id);
		if ( !item_template )
		{
			assert(false);
			goto LABEL_SELECT_FIRST;
		}
		if (job_branch_skill_template_->CanEquipTheCategory(item_template->equip_category_))
			return reward_id;
	}	
LABEL_SELECT_FIRST:
	return quest_reward[0].GetRewardItemId();
}

const TaskCurJobTemplate* GPlayerSelf::GetTaskTemplate( const std::wstring& quest_id )
{
	auto& task_mgr = TaskCurJobTemplateMgr::GetMe();
	if ( !task_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return nullptr;
	}
	return task_mgr.FindByTaskId(quest_id);
}

bool GPlayerSelf::CommitTask( const std::wstring& quest_id )
{
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}
	return CompleteTask(quest_id, ChooseTaskDftReward(quest_id));
}

bool GPlayerSelf::CommitCompleteTasks()
{
	if (received_tasks_.GetAllNodes().empty())
		return true;
	const auto task_cont = received_tasks_.GetAllNodes();
	assert(!task_cont.empty());
	for ( const auto& task_received : task_cont )
	{
		if (task_received.TaskCompletable())
		{
			if ( !CommitTask(task_received.GetStrTaskId()) )
			{
				//assert(false);
				return false;
			}
		}
	}
	return true;
}

bool GPlayerSelf::AcceptTheTask( const std::string& quest_ch_name )
{
	auto& quest_index = GetEn2Ch().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}

	const auto& quest_id = quest_index->Ch2En(GbkToWideChar(quest_ch_name));
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}
	return AcceptTheTaskW(quest_id);
}

TaskReceivedMgr& GPlayerSelf::GetReceivedTask()
{
	return received_tasks_;
}

bool GPlayerSelf::IsAcceptedTheTask( const std::string& quest_ch_name )
{
	const auto& quest_index = GetEn2Ch().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}
	const auto& quest_id = quest_index->Ch2En(GbkToWideChar(quest_ch_name));
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}
	return !!received_tasks_.FindTask(quest_id);
}

GameEn2ChNodeMgr& GPlayerSelf::GetEn2Ch()
{
	auto& en2ch_mgr = GameEn2ChNodeMgr::GetMe();
	VERIFY(en2ch_mgr.RebuildUpdateAll());
	return en2ch_mgr;
}

bool GPlayerSelf::TheTaskCompletable( const std::string& quest_ch_name )
{
	const auto& quest_index = GetEn2Ch().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}
	const auto& quest_id = quest_index->Ch2En(GbkToWideChar(quest_ch_name));
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}
	auto task_r = received_tasks_.FindTask(quest_id);
	if ( !task_r )
	{
		assert(false);
		return false;
	}
	return task_r->TaskCompletable();
}

bool GPlayerSelf::MoveToTheVillage( const std::string& village_ch_name )
{
	const auto& village_index = GetEn2Ch().GetVillageIndex();
	if ( !village_index )
	{
		assert(false);
		return false;
	}
	const auto& village_dst_id = village_index->Ch2En(GbkToWideChar(village_ch_name));
	if (village_dst_id.empty())
	{
		assert(false);
		return false;
	}
	return MoveToTheVillageW(village_dst_id);
}

bool GPlayerSelf::MoveToTheCloneMap( const std::string& clone_map_ch_name )
{
	const auto& fb_index = GetEn2Ch().GetStageIndex();
	if ( !fb_index )
	{
		assert(false);
		return false;
	}
	return MoveToTheCloneMapW(fb_index->Ch2En(GbkToWideChar(clone_map_ch_name)));
}

bool GPlayerSelf::MoveToTheVillageW( const std::wstring& village_dst_id )
{
	using namespace GClass;
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		if ( !ReturnCity() )
		{
			assert(false);
			return false;
		}
	}
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		assert(false);
		return false;
	}
	const auto& kCurMapId = gm.GetMapId();
	if (kCurMapId.empty())
	{
		assert(false);
		return false;
	}
	if ( kCurMapId == village_dst_id )
		return true;
	GSP_Resolver res;
	GSP_Stage::StepsT steps;
	if ( !res.VillageMove(kCurMapId, village_dst_id, steps) )
	{
		assert(false);
		return false;
	}
	auto& ambient = GAmbient::GetMe();
	while ( !steps.empty() )
	{
		const auto& front_step = steps.front();
		if ( !front_step.step_stage_ )
		{
			assert(false);
			return false;
		}
		if ( !gm.Update() )
		{
			assert(false);
			return false;
		}
		if ( gm.GetMapId() != front_step.step_stage_->stage_id_ )
		{
			assert(false);
			return false;
		}		
		if ( !front_step.step_gate_ )
			break;
		if ( !ChangeVillage(front_step.step_gate_->wrap_gate_id_) )
		{
			assert(false);
			return false;
		}
		steps.pop_front();
		ambient.UntilCityUiNow();		
	}
	//assert(1 == steps.size());
	return true;
}

bool GPlayerSelf::SellNormalTabAllEquips()
{
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "你没有在城镇，不能卖东西";
		assert(false);
		return false;
	}
	auto equips = packet_.GetTabNormal().GetItems();
	if (equips.empty())
		return true;
	auto& obj_mgr = GameObjMgr::GetMe();
	if ( !obj_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	int cnt = 0;
	GameObjBase::SelfPtrT npc_obj;
	while (true)
	{
		if ( ++cnt == 20 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "在城镇中找不到可卖东西的NPC，卖物品失败";
			return false;
		}
		npc_obj = obj_mgr.FindFirstShopNpc();
		if (npc_obj)
			break;
		DummySleep(500);
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
	}
	if ( !VillageStandToGameObj(npc_obj) )
	{
		assert(false);
		return false;
	}
	for ( auto& item : equips )
	{
		auto item_mgr = item.GetParent();
		if ( item_mgr && item_mgr->TryTradeFilter(item) )
			continue;
		if ( !item.Sell(npc_obj) )
		{
			assert(false);
			return false;
		}
		DummySleep(300);
	}
	
	if ( !packet_.RebuildUpdateNormal() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::LearnTheSkill( const std::string& skill_ch_name )
{
	if (skill_ch_name.empty())
	{
		assert(false);
		return false;
	}
	const auto& skill_index = GetEn2Ch().GetSkillIndex();
	if ( !skill_index )
	{
		assert(false);
		return false;
	}
	const auto& skill_id = skill_index->Ch2En(GbkToWideChar(skill_ch_name));
	if (skill_id.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "未找到指定的技能ID->" << skill_ch_name;
		assert(false);
		return false;
	}
	return LearnTheSkillW(skill_id);
}

bool GPlayerSelf::VillageStandToPos( const GType::stGameVector& pos )
{
	if ( !obj_serial_ || obj_job_name_.empty() )
	{
		assert(false);
		return false;
	}
	auto& tcp_send = GClass::TcpSend::GetMe();
	CMDS_Msg_VillageWalk msg;
	msg.AcceptFastRun(obj_serial_, obj_job_name_, pos, direction_);
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}

	msg.AcceptStand(obj_serial_, obj_job_name_, pos);
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::VillageStandToGameObj( const GameObjBase::SelfPtrT& gameobj )
{
	if ( !gameobj )
	{
		assert(false);
		return false;
	}
	return VillageStandToPos(gameobj->GetPos());
}

bool GPlayerSelf::FbBossRoomReward()
{
	CMDS_Msg_StageReward msg;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::AcceptTheTaskW( const std::wstring& quest_id )
{
	if (received_tasks_.IsFulled())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "任务接满了，不能再接了";
		return false;
	}
	auto task_r = received_tasks_.FindTask(quest_id);
	if (task_r)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "已接过任务了，id->" << quest_id;
		assert(false);
		return false;
	}
	if ( !AcceptTask(quest_id) )
	{
		assert(false);
		return false;
	}
	DummySleep(300);
	for ( int i = 0;i != 50; ++i )
	{
		if ( !received_tasks_.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		if (received_tasks_.FindTask(quest_id))
		{
			auto& quest_index = GetEn2Ch().GetQuestIndex();
			if (quest_index)
			{
				const auto& ch_name = quest_index->En2Ch(quest_id);
				if ( !ch_name.empty() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_info) << "成功接到任务->" << ch_name;
				}
			}
			return true;
		}
		//LOG_O(Log_trace) << "等待接受任务成功，任务id->" << quest_id << "\t等待次数->" << i + 1;
		DummySleep(300);
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "等待接受成功任务超时失败,任务ID->" << quest_id;
	return false;
}

int GPlayerSelf::AcceptLocalMapAnyAllTasks()
{
	if ( !GMap::GetMe().Update() )
	{
		assert(false);
		return 0;
	}
	if ( !GameObjMgr::GetMe().RebuildUpdateAll() )
	{
		assert(false);
		return 0;
	}
	int accept_cnt = 0;
	while (AcceptLocalMapAnyTask())
	{
		++accept_cnt;
		if ( accept_cnt >= 50 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "接受任务次数过多，次数->" << accept_cnt;
			return accept_cnt;
		}
		//DummySleep(200);
	}
	return accept_cnt;
}

bool GPlayerSelf::AcceptLocalMapAnyTask()
{
	if ( !received_tasks_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto& npc_mgr = TaskNpcMgr::GetMe();
	if ( !npc_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto& npc_cont = npc_mgr.GetAllNodes();
	if (npc_cont.empty())
		return false;
	std::wstring quest_id;
	{
		auto npc_iter = npc_mgr.TraverseLocalMapAcceptableNpc();
		assert(npc_iter);
		for ( npc_iter->First(); !npc_iter->IsDone(); npc_iter->Next() )
		{
			auto& task_npc = npc_iter->CurItem();
			assert(!task_npc.GetNpcId().empty());
			auto& task_mgr_cont = task_npc.GetAcceptableMgr().GetAllNodes();
			assert(!task_mgr_cont.empty());
			for ( auto& task_node : task_mgr_cont )
			{
				const auto& task_template = task_node.GetTaskTemplate();
				if ( !task_template )
				{
					assert(false);
					continue;
				}
				if ( !task_template->IsAcceptable() )
					continue;
				const auto& the_task_id = task_node.GetTaskId();
				if (TaskBlacklistExistTheQuest(the_task_id))
					continue;
				if (received_tasks_.FindTask(the_task_id))
					continue;
				if ( !CanAcceptTheTask(the_task_id) )
					continue;
				quest_id = the_task_id;
				break;
			}
			if ( !quest_id.empty() )
				break;
		}
	}
	if (quest_id.empty())
		return false;
	if ( !AcceptTheTaskW(quest_id) )
		return false;
	DummySleep(400);
	return true;
}

bool GPlayerSelf::CommitNoCompleteHandlerTasks()
{
	if (received_tasks_.GetAllNodes().empty())
		return true;
	const auto task_cont = received_tasks_.GetAllNodes();
	assert(!task_cont.empty());
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal竟然为空";
		return false;
	}
	auto& glua = func_obj_normal->GetGLua();
	for ( const auto& task_received : task_cont )
	{
		if (task_received.TaskCompletable())
		{
			const auto& quest_id = task_received.GetStrTaskId();
			if (glua.Call<bool>("ExistTaskCompleteHandler", true, quest_id, player_lvl_))
				continue;
			if ( !CommitTask(quest_id) )
			{
				//assert(false);
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "提交任务失败，任务ID->" << quest_id;
				return false;
			}
		}
	}
	return true;
}

bool GPlayerSelf::DestroyAllMisc()
{
	auto& mgr = GameObjMgr::GetMe();
	if (mgr.IsGameObjsEmpty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "game objs is empty";
		return false;
	}
	auto the_skill = skill_mgr_.GetDefaultUseSkill();
	if ( !the_skill )
	{
		if ( !LearnAllSkills() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "无技能，学习技能竟然失败";
			return false;
		}
		if ( !TimerDoWaitUntil([this]() -> bool {
			return !!skill_mgr_.GetDefaultUseSkill();
		}, [this](){
			Update();
		}, 500, 1000 * 60 * 5) )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "无技能，等待学习技能成功超时";
			return false;
		}
		the_skill = skill_mgr_.GetDefaultUseSkill();
		assert(the_skill);
	}

	return HitMultiToDie(*the_skill, mgr.TraverseAliveGameObj(kGOT_MISC));
}

bool GPlayerSelf::UpdateFatigueData()
{
	using namespace GAddr;
	using namespace GOffset;
	auto fatigue_data_old = fatigue_data_;
	//疲劳值
	auto fatigue_ptr = (const ROLE_FATIGUE*)(g_pRoleObjAddr + g_o_FatigueOffset);
	assert(fatigue_ptr->str_nFatigue >= 0);
	assert(fatigue_ptr->str_nMembershipFatigue >= -1);
	if ( fatigue_ptr->str_nMembershipFatigue > 0 )
		fatigue_data_ = fatigue_ptr->str_nFatigue + fatigue_ptr->str_nMembershipFatigue;
	else
		fatigue_data_ = fatigue_ptr->str_nFatigue;
	if ( 0 == fatigue_data_ || fatigue_data_old != fatigue_data_ )
	{
		auto funcobj = FuncObj_NormalC::GetInstance();
		if ( !funcobj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "func obj normalC不存在";
			return false;
		}
		funcobj->GetSessionOper().ThisMemCall(nullptr, "SetFatigueData", &JsonEcho::MakeParams(fatigue_data_));
	}
	return true;
}

GType::enJobCategory GPlayerSelf::GetJob() const
{
	return job_;
}

GType::enJobCategory GPlayerSelf::GetMainJob() const
{
	return JobToMainJob(job_);
}

GType::enJobCategory GPlayerSelf::JobToMainJob( GType::enJobCategory job )
{
	using namespace GType;
	auto branch_template = GClassInfoTemplateMgr::GetMe().FindByJob(job);
	if ( !branch_template )
	{
		assert(false);
		return kJC_Invalid;
	}
	return branch_template->GetParentJob();
	/*if ( kJC_JianDouShi == job || kJC_ZhanKuang == job || kJC_QuanSha == job || kJC_JianMo == job )
	{
		return kJC_JianDouShi;
	}
	if ( kJC_RenWuZhe == job || kJC_FengWuXing == job || kJC_TianLangXing == job || kJC_LieRenXing == job )
	{
		return kJC_RenWuZhe;
	}
	if ( kJC_KongMoShi == job || kJC_MoYingShi == job || kJC_ShiKongShuShi == job || kJC_ShuangYuZhe == job )
	{
		return kJC_KongMoShi;
	}
	assert(false);
	return kJC_Invalid;
	*/
}

bool GPlayerSelf::AutoLearnLvlUpSkills()
{
	AutoLvlUpSkills();
	LearnAllSkills();
	return true;
}

bool GPlayerSelf::LearnTheSkillW( const std::wstring& skill_id )
{
	GSkillObj skill_tmp;
	auto skill_exist = skill_mgr_.FindSkillObjByStrId(skill_id);
	if (skill_exist)
		skill_tmp = *skill_exist;
	else
	{
		skill_tmp.skill_name_ = skill_id;
	}
	if ( !UseOutSkillSp(skill_tmp) )
		return false;
	return true;
}

bool GPlayerSelf::LearnAllSkills()
{
	auto& skill_factory = SkillAcceptableFactory::GetMe();
	auto skill_acceptable = skill_factory.FindBranchByJob(GetMainJob());
	if ( !skill_acceptable )
	{
		assert(false);
		return false;
	}
	auto basic_skill = skill_acceptable->GetBasicSkillPair();
	if ( !basic_skill )
	{
		assert(false);
		return false;
	}
	const auto& skill_id = basic_skill->first;
	if (skill_id.empty())
	{
		assert(false);
		return false;
	}
	if ( !skill_mgr_.FindSkillObjByStrId(skill_id) )
	{
		if ( !LearnTheSkillW(skill_id) )
		{
			assert(false);
			return false;
		}
		DummySleep(1000);
		if ( !Update() )
		{
			assert(false);
			return false;
		}
	}

	auto branch_template = GClassInfoTemplateMgr::GetMe().FindByJob(job_);
	if ( !branch_template )
	{
		assert(false);
		return false;
	}
	if (branch_template->IsCreatableJob())
		//主职业，可创建的职业
	{
		assert(branch_template->GetJob() == branch_template->GetParentJob());
		const auto& skill_branchs = branch_template->GetSkillBranchs().GetAllNodes();
		if (skill_branchs.empty())
		{
			assert(false);
			return false;
		}
		for ( const auto& skill_branch : skill_branchs )
		{
			if ( !LearnTheSkillBranch(skill_branch) )
			{
				assert(false);
				return false;
			}
		}
	}
	else
	{
		assert(branch_template->GetJob() != branch_template->GetParentJob());
		auto skill_brand = branch_template->GetSkillBranchs().FindByBranchJobType(job_);
		if ( !skill_brand )
		{
			assert(false);
			return false;
		}
		if ( !LearnTheSkillBranch(*skill_brand) )
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GPlayerSelf::LearnTheSkillBranch( const GClassInfoTemplate::SkillBranch& skill_branch )
{
	const auto& branch_skills = skill_branch.GetBranchSkills().GetAllNodes();
	if (branch_skills.empty())
	{
		assert(false);
		return false;
	}
	for ( const auto& branch_skill : branch_skills )
	{
		const auto& skill_id = branch_skill.GetSkillId();
		assert(!skill_id.empty());
		if ( !skill_mgr_.FindSkillObjByStrId(skill_id) )
		{
			if ( !LearnTheSkillW(skill_id) )
			{
				assert(false);
				return false;
			}
		}
	}
	return true;
}

bool GPlayerSelf::DoUpdateImpl()
{
	using namespace GAddr;
	using namespace GOffset;
	using namespace GClass;
	auto funcobj = FuncObj_NormalC::GetInstance();
	if ( !funcobj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "func obj normalC不存在";
		return false;
	}
	auto cur_hp_old = cur_hp_;
	auto role_name_old = obj_name_;
	auto role_lvl_old = player_lvl_;
	//auto map_room_name_old
	//auto cur_gold_old
	auto gold_old = gold_;
	auto job_old = job_;
	//////////////////////////////////////////////////////////////////////////
	if ( !__super::Update() )
		return false;
	/*if (skill_mgr_.IsSkillEmpty())
	{
		if ( !skill_mgr_.RebuildUpdateAll() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "重新构建所有技能失败";
			assert(false);
			return false;
		}
	}
	else
	{
		if ( !skill_mgr_.UpdateAll() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新角色信息时，更新技能的时候出错";
			assert(false);
			return false;
		}
	}*/
	if ( !skill_mgr_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "重新构建所有技能失败";
		assert(false);
		return false;
	}
	//城镇名称
	auto game_str_tmp = (stGameString*)(g_pRoleObjAddr + g_o_VillageName);
	assert(game_str_tmp);
	if ( !game_str_tmp->CopyTo(village_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read village_name_ error";
		assert(false);
		return false;
	}

	//已接任务
	if ( !received_tasks_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	if ( !packet_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}

	const GGameTArrayPlaceholder* game_t_array;
	//sp剩余技能点
	{
		game_t_array = (const GGameTArrayPlaceholder*)(g_pRoleObjAddr + g_o_left_sp1 + g_o_left_sp2);
		auto sp_index = *(BYTE*)(g_pRoleObjAddr + g_o_left_sp1 + g_o_left_sp_index);
		if ( !game_t_array->obj_begin_ )
		{
			assert(false);
			return false;
		}
		auto left_sp_ptr = (int*)((int)game_t_array->obj_begin_ + sp_index);
		if ( (int)left_sp_ptr < (WORD)-1 )
		{
			assert(false);
			return false;
		}
		left_sp_ = *left_sp_ptr;
		if ( left_sp_ < 0 )
		{
			assert(false);
			return false;
		}
	}
	//金币和class type
	auto game_player_info = (const stGamePlayerInfo*)g_pRoleObjAddr;
	assert(game_player_info->thePlayerInfo.theStaticPlayerInfo.str_dwClassType < GType::kJC_Any);
	job_ = game_player_info->thePlayerInfo.theStaticPlayerInfo.str_dwClassType;
	assert(game_player_info->thePlayerInfo.theDynamicPlayerInfo.str_dwGold >= 0);
	gold_ = game_player_info->thePlayerInfo.theDynamicPlayerInfo.str_dwGold;
	//////////////////////////////////////////////////////////////////////////
	auto& session_oper = funcobj->GetSessionOper();
	if ( role_name_old != obj_name_ )
	{
		session_oper.ThisMemCall(nullptr, "SetRoleName", &JsonEcho::MakeParams(WideCharToGbk(obj_name_)));
	}
	if ( !UpdateFatigueData() )
	{
		assert(false);
		return false;
	}
	//assert(game_player_info->thePlayerInfo.theDynamicPlayerInfo.str_dwFatigue == fatigue_data_);
	//////////////////////////////////////////////////////////////////////////
	if ( cur_hp_old != cur_hp_ )
	{
		session_oper.ThisMemCall(nullptr, "SetCurHp", &JsonEcho::MakeParams((Json::UInt)cur_hp_));
	}
	if ( role_lvl_old != player_lvl_ )
	{
		session_oper.ThisMemCall(nullptr, "SetRoleLvl", &JsonEcho::MakeParams(player_lvl_));
	}
	if ( job_old != job_ )
	{
		if ( !skill_mgr_.SwtichAcceptableSkillBranch() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "切换可接受使用的技能分支失败";
			assert(false);
			return false;
		}
		job_branch_skill_template_ = GClassInfoTemplateMgr::GetMe().FindByJob(job_);
		if ( !job_branch_skill_template_ )
		{
			assert(false);
			return false;
		}
		const auto& job_name_map = GClass::JobNameMap::GetMe();
		session_oper.ThisMemCall(nullptr, "SetJobName", &JsonEcho::MakeParams(WideCharToGbk(job_name_map.GetChName(job_))));
	}
	if ( gold_old != gold_ )
	{
		session_oper.ThisMemCall(nullptr, "SetCurGold", &JsonEcho::MakeParams((int)gold_));
	}
	return true;
}

const GClassInfoTemplate* GPlayerSelf::GetClassInfoTemplate() const
{
	return job_branch_skill_template_;
}

const std::wstring GPlayerSelf::GetSpecifyJobName() const
{
	using namespace GClass;
	return JobNameMap::GetMe().GetEnName(job_);
}

bool GPlayerSelf::ChangeJob( int sub_job_idx )
{
	/*
	Class_Change_Magicgunner_001.BeginDesc >> [奥法之校场]魔影师之传承
	Class_Change_Magicgunner_002.BeginDesc >> [奥法之校场]时空术士之进化
	Class_Change_Magicgunner_003.BeginDesc >> [奥法之校场]霜语者之蜕变
	Information_Magicgunner_003.BeginDesc >> [兰特岔路]魔影师之传承
	Information_Magicgunner_004.BeginDesc >> [兰特岔路]时空术士之进化
	Information_Magicgunner_005.BeginDesc >> [兰特岔路]霜语者之蜕变

	Class_Change_Warrior_001.BeginDesc >> [勇武之校场]斩狂之秘技
	Class_Change_Warrior_002.BeginDesc >> [勇武之校场]拳刹之觉醒
	Class_Change_Warrior_003.BeginDesc >> [勇武之校场]剑魔之奥义
	Information_Warrior_003.BeginDesc >> [兰特岔路]斩狂之秘技
	Information_Warrior_004.BeginDesc >> [兰特岔路]拳刹之觉醒
	Information_Warrior_005.BeginDesc >> [兰特岔路]剑魔之奥义

	Class_Change_Thief_001.BeginDesc >> [斗志之校场] 风舞星的修罗道
	Class_Change_Thief_002.BeginDesc >> [斗志之校场]猎刃星的风之刃
	Class_Change_Thief_003.BeginDesc >> [斗志之校场]天狼星的宿命决
	Information_Thief_003.BeginDesc >> [兰特岔路]风舞星的修罗道
	Information_Thief_004.BeginDesc >> [兰特岔路]猎刃星的风之刃
	Information_Thief_005.BeginDesc >> [兰特岔路]天狼星的宿命决
	*/
	if ( job_ != GetMainJob() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "主职业与子职业不相同，意思已经转过职业了，无法再次转职";
		assert(false);
		return false;
	}
	auto& gm = GMap::GetMe();
	if ( !gm.IsTown() )
	{
		assert(false);
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "开始转职处理";
	if ( sub_job_idx < 1 || sub_job_idx > 3 )
	{
		assert(false);
		return false;
	}
	if (obj_job_name_.empty())
	{
		assert(false);
		return false;
	}
	std::wstring str_job_name;
	str_job_name.push_back(obj_job_name_[0]);
	const auto kJobNameSize = obj_job_name_.size();
	for ( auto i = 1; i < kJobNameSize; ++i )
	{
		str_job_name.push_back(tolower(obj_job_name_[i]));
	}
	assert(str_job_name.size() > 2);
	//////////////////////////////////////////////////////////////////////////
	CStringW cstr_fb_id;
	cstr_fb_id.Format(L"RgEvent_%stutorial", str_job_name.c_str());

	CMDS_Msg_EnterCloneMap msg_enter_clonemap;
	msg_enter_clonemap.AcceptChangeJob(std::wstring(cstr_fb_id));
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg_enter_clonemap) )
	{
		assert(false);
		return false;
	}
	/*if ( !GClass::GAmbient::GetMe().UntilEnteredFb() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "进入转职场景失败";
		assert(false);
		return false;
	}
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}*/
	//////////////////////////////////////////////////////////////////////////
	const auto& npc_name = GetChangeMainJobNpcName();
	auto& gom = GameObjMgr::GetMe();
	if ( !TimerDoWaitUntil([&gom, &npc_name](){
		if (gom.FindByName(npc_name))
			return true;
		return false;
	}, [&gom](){
		gom.RebuildUpdateAll();
	}, 5 * 1000, 1000 * 60 * 10) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "转职，等待转职NPC出现，超时失败";
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	CStringW cstr_quest_id;
	cstr_quest_id.Format(L"Class_Change_%s_%03d", str_job_name.c_str(), sub_job_idx);
	std::wstring str_quest_id = cstr_quest_id;

	auto& the_npc = gom.FindByName(npc_name);
	if ( !the_npc )
	{
		assert(false);
		return false;
	}
	if ( !FbStandToPos(the_npc->GetPos()) )
	{
		assert(false);
		return false;
	}

	CMDS_Msg_AcceptTask msg_accept;
	msg_accept.quest_id_ = str_quest_id;
	if ( !tcp_send.SendMsg(msg_accept) )
	{
		assert(false);
		return false;
	}
	DummySleep(200);

	CMDS_Msg_CompleteTask msg_complete;
	msg_complete.quest_id_ = str_quest_id;
	if ( !tcp_send.SendMsg(msg_complete) )
	{
		assert(false);
		return false;
	}

	CMDS_Msg_ReturnCity msg;
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}

	if ( !GClass::GAmbient::GetMe().UntilCityUiNow() )
	{
		assert(false);
		return false;
	}
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !Update() )
	{
		assert(false);
		return false;
	}

	if ( !LearnAllSkills() )
	{
		assert(false);
		return false;
	}
	if ( !AutoLvlUpSkills() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerSelf::ChangeJobAsDlt()
{
	return ChangeJob(1);
}

bool GPlayerSelf::NeedChangeJob() const
{
	return player_lvl_ >= 15 && job_ == GetMainJob();
}

bool GPlayerSelf::SendSkillActivityCntAsDlt() const
{
	using namespace std;
	CMDS_Msg_SkillActivity msg;
	/*uniform_int_distribution<> index_dist(0, 20);
	msg.activity_count_ += index_dist(GetDltRandomDevice());	*/
	msg.activity_count_ = 8;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::AutoDoneOneAcceptedTask()
{
	using namespace GClass;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}

	if ( !Update() )
	{
		assert(false);
		return false;
	}
	std::wstring the_stage_id;
	std::wstring quest_id;
	const auto& uncomplete_tasks = received_tasks_.TraverseUncompleteTasks();
	if ( !uncomplete_tasks )
	{
		assert(false);
		return false;
	}
	if ( !FindQuestIdOfVillage(uncomplete_tasks, village_name_, the_stage_id, quest_id) )
	{
		for ( uncomplete_tasks->First(); !uncomplete_tasks->IsDone(); uncomplete_tasks->Next() )
		{
			auto& cur_task = uncomplete_tasks->CurItem();
			assert(!cur_task.TaskCompletable());
			const auto& remain_npc = cur_task.GetRemainActiveNpcInfo().GetCont();
			for ( auto& v : remain_npc )
			{
				if ( v.remaint_cnt_ < 1 )
				{
					assert(0 == v.remaint_cnt_);
					continue;
				}
				assert(!v.stage_id_.empty());
				the_stage_id = v.stage_id_;
				quest_id = cur_task.GetStrTaskId();
				break;
			}
			if ( !the_stage_id.empty() )
				break;
			auto& complete_fb_id = cur_task.GetCompleteStageId();
			if (complete_fb_id.empty())
			{
				//assert(false);
				continue;
			}
			the_stage_id = complete_fb_id;
			quest_id = cur_task.GetStrTaskId();
			break;
		}
	}
	else
	{
		assert(!the_stage_id.empty());
		assert(!quest_id.empty());
	}
	if (quest_id.empty())
		return false;
	auto& quest_index = GetEn2Ch().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}
	auto task_template = TaskCurJobTemplateMgr::GetMe().FindByTaskId(quest_id);
	if ( !task_template )
	{
		assert(false);
		return false;
	}
	const auto& ch_name = quest_index->En2Ch(quest_id);
	LOG_ENTER_FUNC;
	assert(!ch_name.empty());
	LOG_O(Log_info) << "开始自动做任务->" << ch_name << "\t任务等级->" << task_template->GetTaskLevel();
	//////////////////////////////////////////////////////////////////////////
	using namespace luabind;
	if (call_function<bool>(norc->GetGLua().lua(), "RunTheAcceptedTaskHandler", quest_id))
	{
		LOG_O(Log_info) << "自动做任务完成->" << ch_name;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////

	if (the_stage_id.empty())
		return false;
	assert(!quest_id.empty());

	RepairAllWearEquip();
	
	if ( !DoTheFb(the_stage_id) )
		return false;
	LOG_O(Log_info) << "自动做一次任务完成->" << ch_name;
	return true;
}

bool GPlayerSelf::MoveToTheCloneMapW( const std::wstring& clone_map_id )
{
	using namespace GClass;
	if (clone_map_id.empty())
	{
		assert(false);
		return false;
	}
	if ( !UpdateFatigueData() )
	{
		assert(false);
		return false;
	}
	auto& allowed_mgr = AllowedVillageCloneMapMgr::GetMe();
	if ( !allowed_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto fb_allowed = allowed_mgr.FindByName(clone_map_id);
	if ( !fb_allowed )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "等级或其他条件不足，无法进入该副本->" << clone_map_id;
		assert(false);
		return false;
	}
	if ( !fb_allowed->IsCloneMap() )
	{
		assert(false);
		return false;
	}

	auto& stage_template_mgr = GStageTemplateMgr::GetMe();
	if ( !stage_template_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto state_template = stage_template_mgr.FindStage(clone_map_id);
	if ( !state_template )
	{
		assert(false);
		return false;
	}	

	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		if ( clone_map_id == gm.GetMapId() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "你已经在副本中了->" << clone_map_id;
			return true;
		}
		if ( !ReturnCity() )
		{
			assert(false);
			return false;
		}
		return true;
	}

	if ( fatigue_data_ <= 0 && state_template->FatigueOfUse() > 0 )
	{
		assert(0 == fatigue_data_);
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有疲劳值了，不能进入副本";
		return false;
	}
	
	auto village_dst_name = Fb2Village(clone_map_id);
	if ( !village_dst_name )
		return false;
	if ( !MoveToTheVillageW(*village_dst_name) )
	{
		assert(false);
		return false;
	}
	fb_allowed = allowed_mgr.FindByName(clone_map_id);
	if ( !fb_allowed )
	{
		assert(false);
		return false;
	}
	if ( !EnterCloneMapDefault(*fb_allowed) )
		return false;
	if ( !GAmbient::GetMe().UntilEnteredFb(1000 * 60 * 5) )
	{
		//assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "进入副本失败，可能是进入副本的次数已达上限->" << clone_map_id;
		return false;
	}
	return true;
}

bool GPlayerSelf::MoveToTheStage( const std::wstring& stage_id )
{
	using namespace GClass;
	if (stage_id.empty())
	{
		assert(false);
		return false;
	}
	auto& allowed_mgr = AllowedVillageCloneMapMgr::GetMe();
	if ( !allowed_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	auto fb_allowed = allowed_mgr.FindByName(stage_id);
	if ( !fb_allowed )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "等级或其他条件不足，无法进入->" << stage_id;
		assert(false);
		return false;
	}
	if (fb_allowed->IsCloneMap())
		return MoveToTheCloneMapW(stage_id);
	else
		return MoveToTheVillageW(stage_id);
}

bool GPlayerSelf::FightThroughAllRooms( const luabind::object& func )
{
	return FightThroughAllRooms_([&func](){
		luabind::call_function<void>(func);
	});
}

bool GPlayerSelf::FindQuestIdOfVillage( const TaskReceivedMgr::AcceptedTaskIterPtr& task_iter, const std::wstring& village_id, std::wstring& out_stage_id, std::wstring& out_quest_id )
{
	if (village_id.empty())
	{
		assert(false);
		return false;
	}
	assert(task_iter);
	auto& fb_map = GVillageFbMap::GetMe();
	for ( task_iter->First(); !task_iter->IsDone(); task_iter->Next() )
	{
		auto& cur_task = task_iter->CurItem();
		assert(!cur_task.TaskCompletable());
		const auto& remain_npc = cur_task.GetRemainActiveNpcInfo().GetCont();
		for ( auto& v : remain_npc )
		{
			if ( v.remaint_cnt_ < 1 )
			{
				assert(0 == v.remaint_cnt_);
				continue;
			}
			assert(!v.stage_id_.empty());
			if ( v.stage_id_ == village_id )
			{
				out_quest_id = cur_task.GetStrTaskId();
				out_stage_id = village_id;
				return true;
			}
			else
			{
				auto village_dst_name = fb_map.Find_Fb2Village(v.stage_id_);
				if (village_dst_name)
				{
					if ( *village_dst_name == village_id )
					{
						out_quest_id = cur_task.GetStrTaskId();
						out_stage_id = v.stage_id_;
						return true;
					}
				}

			}
		}
		auto& complete_fb_id = cur_task.GetCompleteStageId();
		if (complete_fb_id.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "任务->" << cur_task.GetStrTaskId() << " 没有完成任务要去的副本信息";
			//assert(false);
			continue;
		}
		if ( complete_fb_id == village_id )
		{
			out_quest_id = cur_task.GetStrTaskId();
			out_stage_id = village_id;
			return true;
		}
		else
		{
			auto village_dst_name = fb_map.Find_Fb2Village(complete_fb_id);
			if (village_dst_name)
			{
				if ( *village_dst_name == village_id )
				{
					out_quest_id = cur_task.GetStrTaskId();
					out_stage_id = complete_fb_id;
					return true;
				}
			}				
		}
	}
	return false;
}

void GPlayerSelf::ResetMailList( const MailListPtrT& mail_list )
{
	mail_list_ = mail_list;
	if ( !mail_list )
		return;
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
		return;
	const auto& cmn_event = events_mgr->FindByName(kPPEN_HandleMails);
	if (cmn_event)
		cmn_event->ExecAsyc();
}

void GPlayerSelf::ResetMyProductList( const MyProductListPtrT& product_list )
{
	my_product_list_ = product_list;
}

const GPlayerSelf::MailListPtrT& GPlayerSelf::GetMailList() const
{
	return mail_list_;
}

const GPlayerSelf::MyProductListPtrT& GPlayerSelf::GetMyProductList() const
{
	return my_product_list_;
}

bool GPlayerSelf::ReceiveOneMailAttachments()
{
	if ( !mail_list_ )
		return false;
	auto& item_cont = mail_list_->mail_items_.GetCont();
	if (item_cont.empty())
		return false;
	auto it_end = item_cont.end();
	for ( auto it = item_cont.begin(); it != it_end; ++it )
	{
		auto& item = *it;
		if ( item.gold_ > 0 )
		{
			CMDS_Msg_GetAttachments msg;
			auto& tcp_send = GClass::TcpSend::GetMe();
			msg.mail_serial_ = item.mail_serial_;
			if ( !tcp_send.SendMsg(msg) )
				return false;
			return true;
		}
		else if ( !item.sample_item_id_.empty() )
		{
			CMDS_Msg_GetAttachments msg;
			auto& tcp_send = GClass::TcpSend::GetMe();
			msg.mail_serial_ = item.mail_serial_;
			if ( !tcp_send.SendMsg(msg) )
				return false;
			return true;
		}
	}
	return false;
}

bool GPlayerSelf::RemoveOneRubbishMail()
{
	if ( !mail_list_ )
		return false;
	auto& item_cont = mail_list_->mail_items_.GetCont();
	if (item_cont.empty())
		return false;
	auto it_end = item_cont.end();
	for ( auto it = item_cont.begin(); it != it_end; ++it )
	{
		auto& item = *it;
		if ( item.gold_ <= 0 && item.sample_item_id_.empty() )
		{
			CMDS_Msg_RemoveMail msg;
			auto& tcp_send = GClass::TcpSend::GetMe();
			msg.mail_serial_ = item.mail_serial_;
			if ( !tcp_send.SendMsg(msg) )
				return false;
			item_cont.erase(it);
			return true;
		}
	}
	return false;
}

void GPlayerSelf::SucceedToGetAttachments( LONGLONG mail_serial )
{
	if ( !mail_list_ )
	{
		assert(false);
		return;
	}
	auto& item_cont = mail_list_->mail_items_.GetCont();
	if (item_cont.empty())
	{
		assert(false);
		return;
	}
	auto it_end = item_cont.end();
	for ( auto it = item_cont.begin(); it != it_end; ++it )
	{
		auto& item = *it;
		if ( item.mail_serial_ == mail_serial )
		{
			//assert(item.gold_ || !item.sample_item_id_.empty());
			item.gold_ = 0;
			item.sample_item_id_.clear();

			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if ( !events_mgr )
				return;
			const auto& cmn_event = events_mgr->FindByName(kPPEN_HandleMails);
			if (cmn_event)
				cmn_event->ExecAsyc();
			return;
		}
		
	}
	//assert(false);
}

GPlayerSelf::TradeEventQueueT& GPlayerSelf::GetTradeEventQueue()
{
	return trade_event_queue_;
}

void GPlayerSelf::PushTradeEvent( const std::string& event_name )
{
	if (event_name.empty())
	{
		assert(false);
		return;
	}
	trade_event_queue_.push(event_name);
}

bool GPlayerSelf::SaleProductAsPrice( GItem& item, double price, int item_cnt, int product_cnt, int ac_type )
{
	using namespace GType;
	if ( player_lvl_ < kTradeMinLvl )
		return false;
	if ( my_product_list_ && my_product_list_->my_products_.Size() >= kMyProductListMaxCnt )
		return false;

	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
	{
		assert(false);
		return false;
	}
	if ( product_cnt > 0 && /*CalcSellingProductCnt*/GetTheProductCnt(item.GetItemId()) >= product_cnt )
	{
		events_mgr->ActiveEvent(kPPEN_HandleTrade);
		return false;
	}
	assert(item_cnt > 0);
	assert(item.GetItemCnt() >= item_cnt);
	assert(ac_type >= kGAT_Normal && ac_type <= kGAT_Invalid);
	assert(price > 0);
	if ( price <= kGAT_Invalid )
		price = kGAT_Invalid + 1;
	price = (double)MakePriceByAcType((LONGLONG)price, (enGameAcType)ac_type);
	if ( !RichToSaleAsPrice(price * item_cnt) )
	{
		events_mgr->ActiveEvent(kPPEN_HandleTrade);
		return false;
	}
	CMDS_Msg_RegisterProduct msg;
	msg.item_cn_name_ = item.GetItemCnName();
	if (msg.item_cn_name_.empty())
	{
		assert(false);
		return false;
	}
	/*LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "SaleProductAsPrice";*/
	msg.tab_category_ = item.GetSrcTabCategory();
	msg.slot_index_ = item.GetSlotIndex();
	msg.price_ = (LONGLONG)price;
	msg.item_cnt_ = item_cnt;
	const auto& event_name = RandomUniqueStr();
	const auto& the_event = events_mgr->CreateGeneralEvent([msg]( ICmnEvent& ){
		/*LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "开始拍卖发包,item name->" << msg.item_cn_name_;*/
		GetGlobalTimeDuration().TimeToDoSync([&msg](){
			return GClass::TcpSend::GetMe().SendMsg(msg);
		});
		//LOG_O(Log_debug) << "结束拍卖发包,item name->" << msg.item_cn_name_;
	}, event_name);
	if ( !the_event )
	{
		assert(false);
		return false;
	}
	the_event->OwnershipLvlUp();
	the_event->SetUnNamed();
	PushTradeEvent(event_name);
	return true;
}

void GPlayerSelf::OnRegisterProductResult( const CMDR_OnRegisterProduct& on_result )
{
	switch (on_result.result_)
	{
	case CMDR_OnRegisterProduct::kRPR_Succeed:
		{
			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if ( !events_mgr )
				return;
			const auto& cmn_event = events_mgr->FindByName(kPPEN_HandleTrade);
			if (cmn_event)
				cmn_event->ExecAsyc();
		}
		break;
	case CMDR_OnRegisterProduct::kRPR_PerAcLimit:
	case CMDR_OnRegisterProduct::kRPR_PerRoleLimit:
	case CMDR_OnRegisterProduct::kRPR_CanNotMoveItem:
	case CMDR_OnRegisterProduct::kRPR_Invalid:
		{
			auto events_mgr = CmnEventsMgr_Impl::GetInstance();
			if ( !events_mgr )
				return;
			const auto& cmn_event = events_mgr->FindByName(kPEN_DoneTrade);
			if (cmn_event)
				cmn_event->ExecAsyc();
		}
		break;
	default:
		assert(false);
		break;
	}	
}

void GPlayerSelf::ClearTradeEventQueue()
{
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
	{
		TradeEventQueueT tmp;
		tmp.swap(trade_event_queue_);
		return;
	}
	while ( !trade_event_queue_.empty() )
	{
		const auto& the_event = events_mgr->FindByName(trade_event_queue_.front());
		if (the_event)
			the_event->Discard();
		trade_event_queue_.pop();
	}
}

const CMDR_OnMyProductList::stMyProduct* GPlayerSelf::FindMyProductByItemId( const std::wstring& item_id ) const
{
	if ( !my_product_list_ )
		return nullptr;
	auto& products = my_product_list_->my_products_.GetCont();
	for ( auto& product : products )
	{
		if ( item_id == product.item_id_ )
			return &product;
	}
	return nullptr;
}

int GPlayerSelf::GetTheProductCnt( const std::wstring& item_id ) const
{
	if ( !my_product_list_ )
		return 0;
	auto& products = my_product_list_->my_products_.GetCont();
	int cnt = 0;
	for ( auto& product : products )
	{
		if ( item_id == product.item_id_ )
			++cnt;
	}
	return cnt;
}

bool GPlayerSelf::RefreshMyProductList()
{
	CMDS_Msg_RequestMyProductList msg;
	return GClass::TcpSend::GetMe().SendMsg(msg);
}

bool GPlayerSelf::SaleProductAsPriceBase( GItem& item, int price_delta, double dlt_price, int item_cnt, int product_cnt, int ac_type )
{
	if ( player_lvl_ < kTradeMinLvl )
		return false;
	if ( my_product_list_ && my_product_list_->my_products_.Size() >= kMyProductListMaxCnt )
		return false;
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
	{
		assert(false);
		return false;
	}
	if ( product_cnt > 0 && /*CalcSellingProductCnt*/GetTheProductCnt(item.GetItemId()) >= product_cnt )
	{
		events_mgr->ActiveEvent(kPPEN_HandleTrade);
		return false;
	}
	assert(dlt_price >= 1);
	assert(item_cnt > 0);
	assert(item.GetItemCnt() >= item_cnt);
	assert(ac_type >= kGAT_Normal && ac_type <= kGAT_Invalid);
	CMDS_Msg_RegisterProduct msg;
	msg.item_cn_name_ = item.GetItemCnName();
	if (msg.item_cn_name_.empty())
	{
		assert(false);
		return false;
	}
	msg.tab_category_ = item.GetSrcTabCategory();
	msg.slot_index_ = item.GetSlotIndex();
	msg.item_cnt_ = item_cnt;

	CMDS_Msg_RequestProductInfo quest_msg;
	quest_msg.tab_category_ = msg.tab_category_;
	quest_msg.slot_index_ = msg.slot_index_;
	const auto& quest_product_event = RandomUniqueStr();
	const auto& event_on_quest = events_mgr->CreateGeneralEvent([quest_msg]( ICmnEvent& ){
		GClass::TcpSend::GetMe().SendMsg(quest_msg);
	}, quest_product_event);
	if ( !event_on_quest )
	{
		assert(false);
		return false;
	}
	event_on_quest->OwnershipLvlUp();
	event_on_quest->SetUnNamed();
	PushTradeEvent(quest_product_event);

	const auto& event_name = RandomUniqueStr();
	const auto& the_event = events_mgr->CreateGeneralEvent([events_mgr, msg, price_delta, dlt_price, this, ac_type]( ICmnEvent& ){
		using namespace GType;
		double price = 0;
		if (cur_request_product_info_)
			price = (double)cur_request_product_info_->lower_price_;
		if ( price < 1 )
			price = dlt_price;
		else
		{
			price += price_delta;
			if ( price < 1 )
				price = dlt_price;
		}
		if ( price <= kGAT_Invalid )
			price = kGAT_Invalid + 1;
		price = (double)MakePriceByAcType((LONGLONG)price, (enGameAcType)ac_type);
		if ( !RichToSaleAsPrice(price * msg.item_cnt_) )
		{
			events_mgr->ActiveEvent(kPPEN_HandleTrade);
			return;
		}
		const_cast<CMDS_Msg_RegisterProduct&>(msg).price_ = (LONGLONG)price;
		assert(msg.price_ == (LONGLONG)price);
		GetGlobalTimeDuration().TimeToDoSync([&msg](){
			return GClass::TcpSend::GetMe().SendMsg(msg);
		});
	}, event_name);
	if ( !the_event )
	{
		assert(false);
		return false;
	}
	the_event->OwnershipLvlUp();
	the_event->SetUnNamed();
	PushTradeEvent(event_name);
	return true;
}

bool GPlayerSelf::RichToSaleAsPrice( double total_price ) const
{
	auto res = boost::math::llround(total_price * 0.05);
	if ( res < 1 )
		res = 1;
	res += 10000;
	return res <= gold_;
}

void GPlayerSelf::ResetCurRequestProductInfo( const CurRequestProductInfoT& cur_info )
{
	cur_request_product_info_ = cur_info;
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if ( !events_mgr )
		return;
	const auto& cmn_event = events_mgr->FindByName(kPPEN_HandleTrade);
	if (cmn_event)
		cmn_event->ExecAsyc();
}

bool GPlayerSelf::GenPriProductsInfo( Json::Value& out_json ) const
{
	using namespace GType;
	if ( !my_product_list_ )
		return false;
	const auto& products = my_product_list_->my_products_.GetCont();
	if (products.empty())
		return false;
	stProductInfo private_product;
	Json::Value json_tmp;
	bool has_a = false;
	for ( auto& product : products )
	{
		if (IsPrivateProduct(product.price_))
		{
			private_product.serial_ = product.oid_market_;
			private_product.price_ = product.price_;
			private_product.item_cnt_ = product.item_cnt_;
			if ( !private_product.ConverToJson(json_tmp) )
			{
				assert(false);
				return false;
			}
			out_json.append(json_tmp);
			has_a = true;
		}
	}
	return has_a;
}

GType::RoleProductInfoContT& GPlayerSelf::GetOthersProductInfo()
{
	return others_product_info_;
}

void GPlayerSelf::AddOtherProductInfo( const CmnUuid& role_uuid, const GType::stProductInfo& other_product )
{
	others_product_info_[role_uuid].insert(other_product);
}

void GPlayerSelf::EraseOtherProductInfo( const CmnUuid& role_uuid, const GType::stProductInfo& other_product )
{
	auto it = others_product_info_.find(role_uuid);
	if ( it == others_product_info_.end() )
		return;
	it->second.erase(other_product);
	if (it->second.empty())
		others_product_info_.erase(it);
}

bool GPlayerSelf::GainPriProductInfoByAcType( int ac_type )
{
	if ( ac_type < kGAT_Normal || ac_type >= kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return false;
	}
	auto& coor_trans = nor->GetCoorTrans();
	coor_trans.SetCurRoute(MakeTransRoute((enGameAcType)ac_type));
	return coor_trans.ThisMemCall(nullptr, "GainPriProductsInfo", nullptr);
}

bool GPlayerSelf::TellSelfPriProductInfoToAcType( int ac_type )
{
	if ( ac_type < kGAT_Normal || ac_type >= kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	const auto& role_uuid = GetMyRoleUuid();
	if (role_uuid.is_nil())
	{
		assert(false);
		return false;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return false;
	}
	Json::Value out_json;
	if ( !GenPriProductsInfo(out_json) )
		return false;
	auto& coor_trans = nor->GetCoorTrans();
	coor_trans.SetCurRoute(MakeTransRoute((enGameAcType)ac_type));
	Json::Value json_uuid;
	VERIFY(role_uuid.ToJson(json_uuid));
	return coor_trans.ThisMemCall(nullptr, "OnProductsInfo", &JsonEcho::MakeParams(json_uuid, out_json));
}

void GPlayerSelf::ResetTaskBlacklist()
{
	task_blacklist_.clear();
	task_prohibited_ch_name_.clear();
}

void GPlayerSelf::AddTaskBlacklist( const std::wstring& quest_id )
{
	if (quest_id.empty())
	{
		assert(false);
		return;
	}
	task_blacklist_.insert(quest_id);
}

bool GPlayerSelf::TaskBlacklistExistTheQuest( const std::wstring& quest_id )
{
	const auto& it = task_blacklist_.find(quest_id);
	if ( it != task_blacklist_.end() )
		return true;
	auto& quest_index = GetEn2Ch().GetQuestIndex();
	if (quest_index)
	{
		const auto& ch_name = quest_index->En2Ch(quest_id);
		if (ch_name.empty())
		{
			assert(false);
			return true;
		}
		if ( task_prohibited_ch_name_.find(ch_name) != task_prohibited_ch_name_.end() )
			return true;
	}
	return false;
}

bool GPlayerSelf::GainProductInfoByAcType( int product_ac_type, int ac_type )
{
	if ( ac_type < kGAT_Normal || ac_type >= kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	if ( product_ac_type < kGAT_Normal || product_ac_type > kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return false;
	}
	auto& coor_trans = nor->GetCoorTrans();
	coor_trans.SetCurRoute(MakeTransRoute((enGameAcType)ac_type));
	return coor_trans.ThisMemCall(nullptr, "GainProductsInfo", &JsonEcho::MakeParams(product_ac_type));
}

bool GPlayerSelf::GenProductsInfoByAcType( Json::Value& out_json, enGameAcType ac_type ) const
{
	using namespace GType;
	assert(ac_type >= kGAT_Normal && ac_type <= kGAT_Invalid);
	if ( !my_product_list_ )
		return false;
	const auto& products = my_product_list_->my_products_.GetCont();
	if (products.empty())
		return false;
	stProductInfo private_product;
	Json::Value json_tmp;
	bool has_a = false;
	for ( auto& product : products )
	{
		if ( Price2GameAcType(product.price_) == ac_type )
		{
			private_product.serial_ = product.oid_market_;
			private_product.price_ = product.price_;
			private_product.item_cnt_ = product.item_cnt_;
			if ( !private_product.ConverToJson(json_tmp) )
			{
				assert(false);
				return false;
			}
			out_json.append(json_tmp);
			has_a = true;
		}
	}
	return has_a;
}

bool GPlayerSelf::TellSelfProductInfoToAcType( int product_ac_type, int ac_type )
{
	if ( ac_type < kGAT_Normal || ac_type >= kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	if ( product_ac_type < kGAT_Normal || product_ac_type > kGAT_Invalid )
	{
		assert(false);
		return false;
	}
	const auto& role_uuid = GetMyRoleUuid();
	if (role_uuid.is_nil())
	{
		assert(false);
		return false;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return false;
	}
	Json::Value out_json;
	if ( !GenProductsInfoByAcType(out_json, enGameAcType(product_ac_type)) )
		return false;
	auto& coor_trans = nor->GetCoorTrans();
	coor_trans.SetCurRoute(MakeTransRoute((enGameAcType)ac_type));
	Json::Value json_uuid;
	VERIFY(role_uuid.ToJson(json_uuid));
	return coor_trans.ThisMemCall(nullptr, "OnProductsInfo", &JsonEcho::MakeParams(json_uuid, out_json));
}

CmnUuid GPlayerSelf::GetMyRoleUuid() const
{
	CmnUuid uuid;
	if (obj_name_.empty())
	{
		assert(false);
		return uuid;
	}
	uuid.GT_Name(obj_name_);
	return uuid;
}

int GPlayerSelf::CalcSellingProductCnt( const std::wstring& item_id, int item_quality, int item_level ) const
{
	if ( !my_product_list_ )
		return 0;
	auto& products = my_product_list_->my_products_.GetCont();
	int cnt = 0;
	for ( auto& product : products )
	{
		if ( item_id == product.item_id_ )
			++cnt;
		//需要物品等级与物品品质的信息
		/*else if ( product.item_grade_ >= item_quality && product.item_lvl_ >= item_level )
			++cnt;*/
		else
			assert(false);
	}
	return cnt;
}

bool GPlayerSelf::BuyOneOtherProduct()
{
	if ( gold_ <= buy_other_min_gold_ )
		return false;
	if ( player_lvl_ < kTradeMinLvl )
		return false;
	for ( auto it = others_product_info_.begin(), it_end = others_product_info_.end(); it != it_end; ++it )
	{
		for ( auto product_it = it->second.begin(), product_end = it->second.end(); product_it != product_end; ++product_it )
		{
			assert(product_it->price_ > 0);
			auto buy_cnt = int(gold_ / product_it->price_);
			assert(product_it->item_cnt_ > 0);
			buy_cnt = std::min(buy_cnt, product_it->item_cnt_);
			if ( buy_cnt > 0 )
			{
				CMDS_Msg_BuyProduct msg;
				msg.product_market_oid_ = product_it->serial_;
				msg.item_cnt_ = buy_cnt;
				gold_ -= (decltype(gold_))(product_it->price_ * buy_cnt);

				it->second.erase(product_it);
				if (it->second.empty())
					others_product_info_.erase(it);

				GetGlobalTimeDuration().TimeToDoSync([&msg](){
					return GClass::TcpSend::GetMe().SendMsg(msg);
				});
				PushTradeEvent(kPPEN_HandleBuyOtherProducts);
				return true;
			}
		}
	}
	return false;
}

bool GPlayerSelf::SendMailImpl( const std::wstring& receive_role_name, int gold, const GItem* item )
{
	assert(!receive_role_name.empty());
	if ( player_lvl_ < kMailMinLvl )
		return false;
	if ( !SendMail(receive_role_name, Random_StrW(3, 6), Random_StrW(4, 7), (DWORD)gold, item) )
	{
		assert(false);
		return false;
	}
	assert(gold <= gold_);
	gold_ -= gold;
	return true;
}

bool GPlayerSelf::SendMailItem( const std::wstring& receive_role_name, const GItem& item )
{
	return SendMailImpl(receive_role_name, 0, &item);
}

bool GPlayerSelf::SendMailGold( const std::wstring& receive_role_name, int reverse_gold, int delta_gold )
{
	if ( gold_ <= reverse_gold + delta_gold )
		return false;
	return SendMailImpl(receive_role_name, gold_ - reverse_gold, nullptr);
}

bool GPlayerSelf::RequestProductList( const GItemTemplate& item_template )
{
	CMDS_Msg_RequestProductList msg;
	msg.market_category_ = item_template.market_category_;
	if (msg.market_category_.empty())
	{
		assert(false);
		return false;
	}
	msg.item_ch_name_ = item_template.GetChName();
	if (msg.item_ch_name_.empty())
	{
		assert(false);
		return false;
	}
	return GetGlobalTimeDuration().TimeToDoSync([&msg](){
		return GClass::TcpSend::GetMe().SendMsg(msg);
	});
}

bool GPlayerSelf::AddBuyProductItem( const std::string& item_name, int min_price, int item_cnt, int item_cnt_limit )
{
	if ( min_price <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加扫拍卖条目之价格限制参数无效";
		return false;
	}
	if ( item_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加扫拍卖条目之物品数量参数无效";
		return false;
	}
	if ( item_cnt_limit <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加扫拍卖条目之物品数量限制参数无效";
		return false;
	}
	const auto& item_index = GameEn2ChNodeMgr::GetMe().GetItemIndex();
	if ( !item_index )
	{
		assert(false);
		return false;
	}
	const auto& item_id = item_index->Ch2EnA(item_name);
	if (item_id.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加扫拍卖条目之物品名不存在->" << item_name;
		return false;
	}
	auto item_template = GItemTemplateMgr::GetMe().FindById(item_id);
	if ( !item_template )
	{
		assert(false);
		return false;
	}
	auto item_mgr = packet_.FindByTabCategory(item_template->inventory_tab_);
	if ( !item_mgr )
	{
		assert(false);
		return false;
	}
	buy_product_items_.AddItem([item_id, min_price, item_cnt_limit, item_mgr, item_template, item_cnt, this]() -> BuyProductItemsT::OptionalProductT{
		auto item_ptr = item_mgr->FindByItemId(item_id);
		int has_item_cnt = 0;
		if (item_ptr)
		{
			has_item_cnt = item_ptr->GetItemCnt();
			if ( has_item_cnt >= item_cnt_limit )
				return BuyProductItemsT::OptionalProductT();
		}
		LONGLONG gold_tmp = min_price;
		gold_tmp *= item_cnt;
		if ( gold_ <= gold_tmp )
			return BuyProductItemsT::OptionalProductT();
		cur_product_list_.reset();
		VERIFY(RequestProductList(*item_template));
		WaitUntil([this]() -> bool{
			return cur_product_list_;
		}, 10 * 1000);
		if ( !cur_product_list_ )
		{
			assert(false);
			return BuyProductItemsT::OptionalProductT();
		}
		assert(has_item_cnt < item_cnt_limit);
		auto left_need_buy = item_cnt_limit - has_item_cnt;
		for ( const auto& item_product : cur_product_list_->product_items_.GetCont() )
		{
			if ( item_product.price_ <= min_price )
			{
				auto real_cnt = std::min(item_cnt, item_product.item_cnt_);
				real_cnt = std::min(real_cnt, left_need_buy);
				if ( real_cnt <= 0 )
					continue;
				gold_tmp = item_product.price_;
				gold_tmp *= real_cnt;
				if ( gold_ <= gold_tmp )
					continue;
				CMDS_Msg_BuyProduct msg;
				msg.product_market_oid_ = item_product.oid_market_;
				msg.item_cnt_ = real_cnt;
				gold_ -= (decltype(gold_))(gold_tmp);
				assert(gold_ >= 0);
				GetGlobalTimeDuration().TimeToDoSync([&msg](){
					return GClass::TcpSend::GetMe().SendMsg(msg);
				});
				break;
			}
		}
		return BuyProductItemsT::OptionalProductT();
	}, []( BuyProductItemsT::ProductT& ){
		assert(false);
	});
	return true;
}

void GPlayerSelf::ResetCurRequestProductList( const CurRequestProductListT& cur_product_list )
{
	cur_product_list_ = cur_product_list;
}

void GPlayerSelf::DoBuyProductItem()
{
	if ( player_lvl_ < kTradeMinLvl )
		return;
	buy_product_items_.DoProduce();
}

void GPlayerSelf::ClearBuyProductItems()
{
	buy_product_items_.Clear();
}

bool GPlayerSelf::FightAllGraph_( const std::function<void()>& func )
{
	using namespace GAddr;
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	if ( !func_obj_normal )
	{
		LOG_O(Log_error) << "func_obj_normal竟然为空";
		return false;
	}

	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if (gm.IsTown())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "尚未进入到副本，无法打完所有房间";
		return false;
	}
	auto& gr = GCloneMap::GetMe();
	if ( !gr.Update() )
	{
		assert(false);
		return false;
	}

	auto stage_template = gm.GetStageTemplate();
	if ( !stage_template )
	{
		assert(false);
		return false;
	}
	GSP_ResolverEx resolver;
	GSP_StageEx::StepsT steps_out;
	if ( !resolver.StageMove(*stage_template, gr.GetRoomId(), steps_out) )
	{
		assert(false);
		return false;
	}

	//const std::size_t kDelayTime = 1500;
	auto& obj_mgr = GameObjMgr::GetMe();
	while (true)
	{
		const GSP_StepEx* step = nullptr;
		if ( !steps_out.empty() )
			step = &steps_out.front();
		if ( step && !step->is_pass_ || !step )
		{
			obj_mgr.RebuildUpdateAll();
			if (TouchConditionThatLocalMoveToMonsters())
				LocalMoveToMonsters();
			LOG_O(Log_trace) << "新进一个房间，开始KillAllMonster";
			KillAllMonster();
			//DummySleep(kDelayTime);
			//拾取所有金钱
			obj_mgr.RebuildUpdateAll();
			DestroyAllMisc();
		
			SendSkillActivityCntAsDlt();
		
			func();
			LOG_O(Log_trace) << "新进一个房间，完成调用打副本中的回调";
			DummySleep(300);
			obj_mgr.RebuildUpdateAll();
			AcceptLocalMapAnyAllTasks();
			CommitCompleteTasks();
			AcceptLocalMapAnyAllTasks();
			CommitCompleteTasks();
			LOG_O(Log_trace) << "新进一个房间，接受房间中任意任务完成";
			HitUntilClearCurSector(gr.GetRoomId());
		}
		if ( steps_out.size() <= 0 )
			return true;
		if ( !gr.Update() )
		{
			assert(false);
			return false;
		}
		assert(step);
		assert(step->step_gate_);
		assert(step->step_stage_);
		auto next_door = gr.GetDoorById(step->step_gate_->wrap_gate_id_);
		if ( !next_door )
		{
			//assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "找不到下一个房间了，fb id->" << gm.GetMapId() << "\t 当前房间->" << gr.GetRoomId() << "\t要进入的下一个门id->" << step->step_gate_->wrap_gate_id_;
			return false;
		}
		DummySleep(300);
		obj_mgr.RebuildUpdateAll();
		KillAllMonster();
		PickUpAllItems();
		LOG_O(Log_trace) << "开始过图";
		if ( !EnterAnotherRoom(*next_door) )
		{
			LOG_O(Log_debug) << "进入下一个房间失败";
			return false;
		}
		const auto& kStageId = step->step_stage_->stage_id_;
		for ( int i = 0; i != 100; ++i )
		{
			if ( g_pLocalRoomObjAddr && !gr.Update() )
			{
				assert(false);
				return false;
			}
			if ( gr.GetRoomId() != kStageId )
				break;
			DummySleep(700);
		}
		steps_out.pop_front();
	}
	return false;
}

bool GPlayerSelf::FightAllGraph( const luabind::object& func )
{
	return FightAllGraph_([&func](){
		luabind::call_function<void>(func);
	});
}

bool GPlayerSelf::SendAcceptTask( const std::wstring& quest_id )
{
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}

	auto& jt = TaskCurJobTemplateMgr::GetMe();
	auto task_template = jt.FindByTaskId(quest_id);
	if ( !task_template )
	{
		if ( !jt.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		task_template = jt.FindByTaskId(quest_id);
		if ( !task_template )
		{
			assert(false);
			return false;
		}
	}
	const auto& accept_npc = task_template->GetAcceptTaskNpc();
	if (accept_npc.empty())
	{
		assert(false);
		return false;
	}
	const auto& npc_obj = GameObjMgr::GetMe().FindByName(accept_npc);
	if ( !npc_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "城镇->" << village_name_ << " 中找不到完成->" << quest_id << " 任务的npc->" << accept_npc;
		return true;
	}
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if (gm.IsTown())
	{
		if ( !VillageStandToPos(npc_obj->GetPos()) )
		{
			assert(false);
			return false;
		}
	}
	else
	{
		if ( !FbStandToPos(npc_obj->GetPos()) )
		{
			assert(false);
			return false;
		}
	}

	CMDS_Msg_AcceptTask msg;
	msg.quest_id_ = quest_id;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GPlayerSelf::AutoCompleteOneCompletedTask()
{
	using namespace GClass;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}
	
	if ( !Update() )
	{
		assert(false);
		return false;
	}
	const auto& complete_tasks = received_tasks_.TraverseCompleteTasks();
	if ( !complete_tasks )
	{
		assert(false);
		return false;
	}
	for ( complete_tasks->First(); !complete_tasks->IsDone(); complete_tasks->Next() )
	{
		auto& cur_task = complete_tasks->CurItem();
		assert(cur_task.TaskCompletable());
		auto task_template = cur_task.GetTaskTemplate();
		if ( !task_template )
		{
			//assert(false);
			//return false;
			continue;
		}
		const auto& complete_npc = task_template->GetCompleteTaskNpc();
		if (complete_npc.empty())
		{
			assert(false);
			return false;
		}
		const auto& npc_obj = GameObjMgr::GetMe().FindByName(complete_npc);
		if ( !npc_obj )
			continue;

		auto quest_id_tmp = cur_task.GetStrTaskId();
		if ( !CommitTask(quest_id_tmp) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "提交任务失败->" << quest_id_tmp;
			//assert(false);
			return false;
		}
		auto& quest_index = GetEn2Ch().GetQuestIndex();
		if ( !quest_index )
		{
			assert(false);
			return false;
		}
		const auto& ch_name = quest_index->En2Ch(quest_id_tmp);
		assert(!ch_name.empty());
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "自动完成任务->" << ch_name;
		return true;
	}

	using namespace luabind;
	auto lua_state = norc->GetGLua().lua();
	assert(lua_state);
	for ( complete_tasks->First(); !complete_tasks->IsDone(); complete_tasks->Next() )
	{
		auto& cur_task = complete_tasks->CurItem();
		assert(cur_task.TaskCompletable());
		auto task_template = cur_task.GetTaskTemplate();
		if ( !task_template )
		{
			//assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "task_template is nil";
			return false;
		}
		const auto& complete_npc = task_template->GetCompleteTaskNpc();
		if (complete_npc.empty())
		{
			assert(false);
			return false;
		}
		const auto& npc_obj = GameObjMgr::GetMe().FindByName(complete_npc);
		if (npc_obj)
		{
			assert(false);
			continue;
		}
		auto the_quest_id = cur_task.GetStrTaskId();
		assert(!the_quest_id.empty());
		if (call_function<bool>(lua_state, "RunTheCompletableTaskHandler", the_quest_id))
		{
			auto& quest_index = GetEn2Ch().GetQuestIndex();
			if ( !quest_index )
			{
				assert(false);
				return false;
			}
			const auto& ch_name = quest_index->En2Ch(the_quest_id);
			assert(!ch_name.empty());
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "自动任务之完成任务处理之成功->" << ch_name;
			return true;
		}
	}
	
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "自动完成任务失败，找不到能够提交任务的任务处理了";
	return false;
}

const std::wstring& GPlayerSelf::GetChangeMainJobNpcName() const
{
	using namespace GType;
	const auto main_job = GetMainJob();
	if ( kJC_JianDouShi == main_job )
	{
		static std::wstring npc_name = L"SkillTrainer01_Class";
		return npc_name;
	}
	else if ( kJC_RenWuZhe == main_job )
	{
		static std::wstring npc_name = L"SkillTrainer02_Class";
		return npc_name;
	}
	else if ( kJC_KongMoShi == main_job )
	{
		static std::wstring npc_name = L"SkillTrainer03_Class";
		return npc_name;
	}
	else
	{
		assert(false);
		return kEmptyWStr;
	}
}

bool GPlayerSelf::PreDoTask()
{
	if ( player_lvl_ >= 15 && job_ == GetMainJob() )
	{
		auto& gm = GMap::GetMe();
		if ( !gm.IsTown() )
			return true;
		auto events_mgr = CmnEventsMgr_Impl::GetInstance();
		if ( !events_mgr )
			return false;

		auto& cmn_event = events_mgr->FindByName(kPEN_ChangeJob);
		cmn_event->Exec();
	}
	return true;
}

void GPlayerSelf::DoWhenAtRoom()
{
	/*if ( !Update() )
	{
		assert(false);
		return;
	}
	LOG_O(Log_trace) << "过房间回调之开始PickUpAllItems";
	auto& obj_mgr = GameObjMgr::GetMe();
	PickUpAllItems();
	DummySleep(200);
	LOG_O(Log_trace) << "过房间回调，延迟过后，接受房间中任意任务";
	AcceptLocalMapAnyAllTasks();
	DummySleep(200);
	obj_mgr.RebuildUpdateAll();
	LOG_O(Log_trace) << "过房间回调，延迟过后，毁掉所有固态物品";
	DestroyAllMisc();
	DummySleep(2000);
	obj_mgr.RebuildUpdateAll();
	LOG_O(Log_trace) << "过房间回调，延迟过后，KillAllMonster";
	KillAllMonster();
	DummySleep(2000);
	obj_mgr.RebuildUpdateAll();
	LOG_O(Log_trace) << "过房间回调，延迟过后，PickUpAllItems";
	PickUpAllItems();
	Update();
	LOG_O(Log_trace) << "过房间回调，CommitCompleteTasks";
	CommitCompleteTasks();*/
}

bool GPlayerSelf::DoTheFb( const std::wstring& fb_id )
{
	using namespace GClass;
	if ( !MoveToTheStage(fb_id) )
		return false;
	auto& obj_mgr = GameObjMgr::GetMe();
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		Update();
		if ( !FightAllGraph_([this, &obj_mgr](){
			DoWhenAtRoom();
		}) )
		{
			LOG_O(Log_debug) << "FightAllGraph_之返回失败";			
			return false;
		}
		LOG_O(Log_trace) << "从最后一个副本房间，开始等待直到返回了城镇";
		if ( !GAmbient::GetMe().UntilCanRetCity(1000 * 60 * 2) )
		{
			LOG_O(Log_debug) << "从最后一个副本房间，等待直到返回了城镇失败";
			//assert(false);
			return false;
		}
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		if ( !PickUpAllItems() )
		{
			assert(false);
			return false;
		}
		DummySleep(500);
		if ( !Update() )
		{
			assert(false);
			return false;
		}
		packet_.AutoReplaceEquip();
		RepairAllWearEquip();
		if ( !ReturnCity() )
		{
			assert(false);
			return false;
		}
		DummySleep(1000);
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		if ( !gm.Update() )
		{
			assert(false);
			return false;
		}
		if ( !Update() )
		{
			assert(false);
			return false;
		}
		if ( !SellNormalTabAllEquips() )
		{
			assert(false);
			return false;
		}
		if ( !AutoLearnLvlUpSkills() )
		{
			assert(false);
			return false;
		}
		DummySleep(1000);
	}
	if ( !CommitCompleteTasks() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "自动提交任务失败了";
		//assert(false);
		return true;
	}
	return true;
}

void GPlayerSelf::HitUntilClearCurSector( const std::wstring& cur_sector )
{
	LOG_O(Log_trace) << "等待打怪直到可以进入下一个房间,当前房间id->" << cur_sector;
	auto& gm = GMap::GetMe();
	auto& obj_mgr = GameObjMgr::GetMe();
	gm.UpdateClearSectorsSync();
	TimerDoWaitUntil([&cur_sector, &gm]() -> bool {
		return gm.ExistTheClearedSector(cur_sector);
	}, [&gm, this, &obj_mgr](){
		obj_mgr.RebuildUpdateAll();
		KillAllMonster();
		gm.UpdateClearSectorsSync();
	}, 500, 1000 * 10);
	obj_mgr.RebuildUpdateAll();
	KillAllMonster();
	DummySleep(1000);
	obj_mgr.RebuildUpdateAll();
	PickUpAllItems();
	DummySleep(1500);
	/*LOG_O(Log_info) << "等待开门，如果老是不开，建议手动过一下吧";
	WaitUntil([&cur_sector, &gm]() -> bool {
		return gm.ExistTheClearedSector(cur_sector);
	});*/
	if ( !gm.ExistTheClearedSector(cur_sector) )
	{
		CMDS_Msg_DestroyActors destroy_actors;
		if ( on_create_npc_by_client_ && on_create_npc_by_client_->task_npc_serial_ )
			destroy_actors.npc_serial_ = on_create_npc_by_client_->task_npc_serial_;
		else
		{
			GameObj_ParserAny obj_parse;
			auto& gom = GameObjMgr::GetMe();
			const auto& reset_obj_parse = MakeCleanAtScopeExit([&gom](){
				gom.SetGameObjParser(nullptr);
			});
			gom.SetGameObjParser(&obj_parse);
			gom.ResetForceUpdate();
			gom.RebuildUpdateAll();
			static const std::wstring kDestroyNpcs[] = {
				L"Region02_Air_Strike_T01"
			};
			for ( int i = 0;i != _countof(kDestroyNpcs); ++i )
			{
				const auto& the_need_destroy_obj = gom.FindFirstByJob(kDestroyNpcs[i]);
				if (the_need_destroy_obj)
				{
					LOG_O(Log_debug) << "存在->" << kDestroyNpcs[i] << "需要摧毁之，使之开门";
					destroy_actors.npc_serial_ = the_need_destroy_obj->GetObjSerial();
					break;
				}
			}
		}
		
		if ( 0 == destroy_actors.npc_serial_ )
		{
			assert(false);
			return;
		}
		GClass::TcpSend::GetMe().SendMsg(destroy_actors);
	}
}

bool GPlayerSelf::TryUpdateImpl()
{
	using namespace GAddr;
	if ( !MemOperApplyR(game_obj_addr_, g_pRoleBaseAddr) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "角色信息尚不能读取成功";
		return false;
	}
	if ( !timer_sync_exec_.TimerGWndExecSync([this](){
		return DoUpdateImpl();
	}) )
		return false;
	return true;
}

void GPlayerSelf::AddProhibitTask( const std::wstring& quest_ch_name )
{
	if (quest_ch_name.empty())
	{
		assert(false);
		return;
	}
	task_prohibited_ch_name_.insert(quest_ch_name);
}

const std::wstring& GPlayerSelf::GetCurVillageName() const
{
	return village_name_;
}

bool GPlayerSelf::CanAcceptTheTask( const std::wstring& quest_id )
{
	if (quest_id.empty())
	{
		assert(false);
		return false;
	}
	auto& task_mgr = TaskCurJobTemplateMgr::GetMe();
	auto task_template = task_mgr.FindByTaskId(quest_id);
	if ( !task_template )
	{
		task_mgr.RebuildUpdateAll();
		task_template = task_mgr.FindByTaskId(quest_id);
		if ( !task_template )
		{
			//assert(false);
			return false;
		}
	}
	auto& complete_fb_id = task_template->GetCloneMapId();	
	if ( !complete_fb_id.empty() )
	{
		auto the_stage = GStageTemplateMgr::GetMe().FindStage(complete_fb_id);
		if ( !the_stage )
			return false;
		if ( !the_stage->ValidRangeThanRecommandUpperLvl(player_lvl_) )
			return false;
	}
	return true;
}

void GPlayerSelf::ClearRecommandVillages()
{
	while ( !recommand_villages_.empty() )
		recommand_villages_.pop();
}

void GPlayerSelf::GenRecommandVillages()
{
	using namespace GClass;
	auto& allow_mgr = AllowedVillageCloneMapMgr::GetMe();
	allow_mgr.RebuildUpdateAll();
	auto iter = allow_mgr.TraverseRecommandLvlCloneMap(player_lvl_);
	if ( !iter )
	{
		assert(false);
		return;
	}
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto& cur_allow = iter->CurItem();
		auto& clone_map_id = cur_allow.GetMapId();
		if (clone_map_id.empty())
		{
			assert(false);
			continue;
		}
		auto village_dst_name = Fb2Village(clone_map_id);
		if ( !village_dst_name )
			continue;
		recommand_villages_.push(*village_dst_name);
	}
}

bool GPlayerSelf::DoOneRecommandVillage()
{
	while ( !recommand_villages_.empty() )
	{
		if ( !MoveToTheVillageW(recommand_villages_.front()) )
		{
			recommand_villages_.pop();
			continue;
		}
		recommand_villages_.pop();
		return true;
	}
	return false;
}

const std::wstring* GPlayerSelf::Fb2Village( const std::wstring& clone_map_id )
{
	auto& fb_map = GVillageFbMap::GetMe();
	auto village_dst_name = fb_map.Find_Fb2Village(clone_map_id);
	if ( !village_dst_name )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "副本到城镇的映射不存在，副本ID->" << clone_map_id;
		return nullptr;
	}
	return village_dst_name;
}

bool GPlayerSelf::AutoGotoCommitCompletedTask( bool village_move )
{
	using namespace GClass;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "开始自动寻找一个可提交任务的城镇，并移动过去";
	if ( !Update() )
	{
		assert(false);
		return false;
	}
	auto task_tmp = received_tasks_;
	const auto& complete_tasks = task_tmp.TraverseCompleteTasks();
	if ( !complete_tasks )
	{
		assert(false);
		return false;
	}
	for ( complete_tasks->First(); !complete_tasks->IsDone(); complete_tasks->Next() )
	{
		auto& cur_task = complete_tasks->CurItem();
		assert(cur_task.TaskCompletable());
		auto task_template = cur_task.GetTaskTemplate();
		if ( !task_template )
		{
			//assert(false);
			//return false;
			continue;
		}
		auto& complete_fb_id = cur_task.GetCompleteStageId();
		if (complete_fb_id.empty())
		{
			//assert(false);
			continue;
		}
		if ( !village_move )
			return DoTheFb(complete_fb_id);
		auto village_dst_name = Fb2Village(complete_fb_id);
		if ( !village_dst_name )
			continue;
		LOG_O(Log_debug) << "开始移动到城镇" << *village_dst_name;
		return MoveToTheVillageW(*village_dst_name);
	}
	return false;
}

bool GPlayerSelf::SellTheItemToShop( GItem& item )
{
	auto& gm = GMap::GetMe();
	if ( !gm.Update() )
	{
		assert(false);
		return false;
	}
	if ( !gm.IsTown() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "你没有在城镇，不能卖东西";
		assert(false);
		return false;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	if ( !obj_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	int cnt = 0;
	GameObjBase::SelfPtrT npc_obj;
	while (true)
	{
		if ( ++cnt == 20 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "在城镇中找不到可卖东西的NPC，卖物品失败";
			return false;
		}
		npc_obj = obj_mgr.FindFirstShopNpc();
		if (npc_obj)
			break;
		DummySleep(500);
		if ( !obj_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
	}
	if ( !VillageStandToGameObj(npc_obj) )
	{
		assert(false);
		return false;
	}
	if ( !item.Sell(npc_obj) )
	{
		assert(false);
		return false;
	}
	DummySleep(300);
	return true;
}

void GPlayerSelf::ResetOnCreateNpcByClient( const OnCreateNpcByClientT& the_msg )
{
	//LOG_O(Log_trace) << "ResetOnCreateNpcByClient";
	on_create_npc_by_client_ = the_msg;
}

bool GPlayerSelf::FbMonstersMove( const GameObjMgr::GameObjIterPtr& gameobjs )
{
	using namespace GClass;
	if ( !gameobjs )
	{
		assert(false);
		return false;
	}
	auto& obj_serial_mgr = stPreActionPlayTimeMgr::GetMe();
	obj_serial_mgr.RebuildUpdateAll();
	for ( gameobjs->First(); !gameobjs->IsDone(); gameobjs->Next() )
	{
		if ( !FbMonsterMove(gameobjs->CurItem()) )
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GPlayerSelf::FbMonsterMove( const GameObjBase::SelfPtrT& gameobj )
{
	using namespace GFuncCall;
	using namespace GAddr;
	using namespace GClass;
	if ( !gameobj )
	{
		assert(false);
		return false;
	}
	auto& obj_serial_mgr = stPreActionPlayTimeMgr::GetMe();
	auto osm1 = obj_serial_mgr.FindBySerial(gameobj->GetObjSerial());
	if ( !osm1 )
	{
		assert(false);
		return false;
	}
	auto papt = osm1->GetPapt();
	if ( !papt )
	{
		assert(false);
		return false;
	}
	if ( 0 == papt->pre_action_serial_ )
	{
		return true;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "发送怪物的移动封包, obj name->" << gameobj->GetObjName();
	auto& tcp_send = TcpSend::GetMe();
	int nOffset = 0;
	DWORD error_buff = 0;

	CMDS_Msg_FbWalk fb_walk_msg;
	auto action_serial = LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	auto rand_seed = g_pRandomSeekCall_.Call<DWORD>();
	auto tick_count = (float)GetTickCount();
	//////////////////////////////////////////////////////////////////////////
	const auto& obj_pos = gameobj->GetPos();
	fb_walk_msg.AcceptRealize(gameobj->GetObjSerial(), gameobj->GetJobName(), obj_pos, papt->action_play_time, action_serial, papt->pre_action_serial_, rand_seed, tick_count);
#ifdef _ENABLE_GAME_PACKET_TEST_
	nOffset = JFZR_ActionSyncPacket(g_send_packet_buffer_test_, gameobj->GetObjSerial(), CalcHash_DWORD(gameobj->GetJobName(), kSendMsg_CmdNameRealize), obj_pos.x_, obj_pos.y_, obj_pos.z_,
		0, 0, 0, 0x399E, 0xB9B2, 1, papt->action_play_time, action_serial, papt->pre_action_serial_, rand_seed, tick_count, 0);
	error_buff = tcp_send.SendMsgTest(fb_walk_msg, g_send_packet_buffer_test_, g_send_packet_buffer_test_ + nOffset);
	if ( 0 != error_buff )
	{
		if ( error_buff > 0xffff )
		{

		}
		return false;
	}
#endif
	LockAdd(JFZR_REVERSAL_ROLE_SKILL_RUN_SERIAL_ADDR);
	if ( !tcp_send.SendMsg(fb_walk_msg) )
	{
		assert(false);
		return false;
	}
	DummySleep(500);
	return true;
}

bool GPlayerSelf::ChangeRoleCurPos( const GType::stGameVector& new_pos )
{
	using namespace GType;
	using namespace GOffset;
	if ( !game_obj_addr_ )
	{
		assert(false);
		return false;
	}
	auto the_pos = (stGameVector*)(game_obj_addr_ + g_o_CreaturePosX);
	*the_pos = new_pos;
	return true;

	/*return GWndExecSync([this, &new_pos](){
		auto the_pos = (stGameVector*)(game_obj_addr_ + g_o_CreaturePosX);
		*the_pos = new_pos;
		return true;
	});*/
}

bool GPlayerSelf::HitTargetsOneByOne( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs )
{
	using namespace GClass;
	using namespace GFuncCall;
	if ( !target_gameobjs )
	{
		assert(false);
		return false;
	}
	int cnt = 0;
	for ( target_gameobjs->First(); !target_gameobjs->IsDone(); target_gameobjs->Next() )
	{
		if ( !HitTarget(skill_obj, target_gameobjs->CurItem()) )
		{
			assert(false);
			return false;
		}
		DummySleep(50);
		++cnt;
	}
	return cnt > 0;
}

bool GPlayerSelf::DisassemblyTheItem( GItem& item )
{
	return GetGlobalTimeDuration().TimeToDoSync([&item](){
		return item.Disassembly();
	});
}

bool GPlayerSelf::SendMailItemDelay( const std::wstring& receive_role_name, const GItem& item )
{
	assert(!receive_role_name.empty());
	if ( player_lvl_ < kMailMinLvl )
		return false;
	stSendMailList mail_list = {
		item, receive_role_name
	};
	send_mail_list_.push_back(mail_list);
	return true;
}

void GPlayerSelf::ClearSendMailList()
{
	send_mail_list_.clear();
}

void GPlayerSelf::DoSendMailList()
{
	if ( player_lvl_ < kMailMinLvl )
		return;
	if (send_mail_list_.empty())
		return;
	std::sort(send_mail_list_.begin(), send_mail_list_.end(), []( const stSendMailList& lhs, const stSendMailList& rhs ){
		return lhs.receive_role_name_ < rhs.receive_role_name_;
	});
	std::vector<SendMailListContT> send_mail_conts;
	send_mail_conts.push_back(SendMailListContT());
	const std::wstring* cur_receive_rol_name = nullptr;
	for ( auto& item : send_mail_list_ )
	{
		if ( !cur_receive_rol_name )
			cur_receive_rol_name = &item.receive_role_name_;
		auto the_cont = &send_mail_conts.back();
		if ( the_cont->size() >= kSendMailMaxItemCnt || *cur_receive_rol_name != item.receive_role_name_ )
		{
			send_mail_conts.push_back(SendMailListContT());
			the_cont = &send_mail_conts.back();
			cur_receive_rol_name = &item.receive_role_name_;
		}
		the_cont->push_back(item);
	}
	for ( auto& items : send_mail_conts )
	{
		assert(!items.empty());
		assert(!items.begin()->receive_role_name_.empty());
		
		auto& the_iter = MakeIterator<TraverseTraits<stSendMailList&, stSendMailList&> >(items, []( const stSendMailList& ){
			return true;
		});
		SendMail(items.begin()->receive_role_name_, Random_StrW(3, 6), Random_StrW(4, 7), 0, MakeWrapperIterator<GItemMgr::ItemTraTraits>(the_iter, []( stSendMailList& the_mail ) -> GItem&{
			return the_mail.item_;
		}));
	}
}

void GPlayerSelf::LocalMoveToMonsters()
{
	auto& gom = GameObjMgr::GetMe();
	/*if (gom.IsGameObjsEmpty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "game objs is empty";
		return;
	}*/

	auto target_gameobjs = gom.TraverseAliveGameObj(kGOT_Monster);
	if ( !target_gameobjs )
	{
		assert(false);
		return;
	}
	auto pos_old = GetPos();
	for ( target_gameobjs->First(); !target_gameobjs->IsDone(); target_gameobjs->Next() )
	{
		auto& gameobj = target_gameobjs->CurItem();
		if ( !gameobj )
		{
			assert(false);
			continue;
		}
		auto pos_new = gameobj->GetPos();
		pos_new.z_ += 50.f;
		if ( !ChangeRoleCurPos(pos_new) )
		{
			assert(false);
			return;
		}
		DummySleep(150);
	}
	//pos_old.z_ += 25.f;
	ChangeRoleCurPos(pos_old);
	DummySleep(3000);
}

bool GPlayerSelf::TouchConditionThatLocalMoveToMonsters()
{
	if ( player_lvl_ < 5 )
		return false;
	if ( 5 == player_lvl_ )
	{
		if ( village_name_ == L"Ep01Village01" )
			return false;
	}
	return true;
}

void GPlayerSelf::PlayerDied()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "角色死亡了";
	auto& gm = GMap::GetMe();
	if ( !gm.IsTown() )
	{
		const auto& fb_cn_name = gm.GetCurMapFbChName();
		if ( !fb_cn_name.empty() )
		{
			if ( no_ret_city_at_die_.find(fb_cn_name) != no_ret_city_at_die_.end() )
			{
				LOG_O(Log_debug) << "在副本:" << fb_cn_name << "\t存在死亡不回城这样的映射，所以不回城";
				return;
			}
		}
	}
	player_died_ = true;
}

void GPlayerSelf::ResetNoRetCityAtDie()
{
	no_ret_city_at_die_.clear();
}

void GPlayerSelf::AddNoRetCityAtDie( const std::wstring& fb_ch_name )
{
	if (fb_ch_name.empty())
	{
		assert(false);
		return;
	}
	no_ret_city_at_die_.insert(fb_ch_name);
}

bool GPlayerSelf::DoTheFbA( const std::string& fb_ch_name )
{
	const auto& fb_index = GetEn2Ch().GetStageIndex();
	if ( !fb_index )
	{
		assert(false);
		return false;
	}
	return DoTheFb(fb_index->Ch2En(GbkToWideChar(fb_ch_name)));
}