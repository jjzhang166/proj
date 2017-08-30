#include "stdafx.h"
#include "GPlayer.h"
#include "GClass.h"
#include "GameObjMgr.h"
#include "../FuncObjC.h"
#include "GMapPath.h"
#include "GMonster.h"
#include "GFuncCall.h"
#include "GConst.h"
#include <Common/EnsureCleanup.h>
#include <dinput.h>
#include "../Preprocessor.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include <CD_CommonFiles/Data/CD_ProtocolS.h>
#include "../GLua/Scmd.h"

bool GPlayerMe::SendWalk( const GType::stGameVector3& dst_pos, const GType::stDirection2d& dir )
{
	assert(false);
	return false;
	stCD_Msg_Walk walk_msg;
	//walk_msg.llDynamic = GFC_GetDynamicValue();
	walk_msg.dst_pos = dst_pos;
	walk_msg.dir = dir;
	return CD_FuncCall::SendT(walk_msg);
}

bool GPlayerMe::SendSay( const std::string& say_msg )
{
	stCD_Msg_SayMsg msg;
	msg.say_type_ = stCD_Msg_SayMsg::kNormal;
	TStrCpy(msg.msg_buffer_, say_msg.c_str(), sizeof(msg.msg_buffer_));
	msg.array_size_ = 0xa;
	*(int*)&msg.array_[0] = 0xaa;
	*(int*)&msg.array_[4] = 0x3000100a;
	*(short*)&msg.array_[8] = 0xfdd;
	return CD_FuncCall::SendT(msg);
}

bool GPlayerMe::Update()
{
	auto old_name = name_;
	auto old_player_lvl = role_lvl_;
	auto old_free_lie_shou_cnt = left_shou_lie_quan_cnt_;
	auto old_single_exp = left_single_exp_;
	auto old_double_exp = left_double_exp_;
	auto old_map = cur_map_name_;
	auto old_gold = gold_;
	if ( !__super::Update() )
	{
		//assert(false);
		return false;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}
	auto& session_oper = norc->GetSessionOper();
	if ( old_name != name_ )
	{
		session_oper.ToCall(BC_MakeP("SetRoleName") << name_);
	}
	if ( old_player_lvl != role_lvl_ )
	{
		session_oper.ToCall(BC_MakeP("SetRoleLvl") << role_lvl_);
	}
	bool is_tell_fatigue = false;
	if ( old_free_lie_shou_cnt != left_shou_lie_quan_cnt_ )
	{
		session_oper.ToCall(BC_MakeP("SetFreeLieShouCnt") << left_shou_lie_quan_cnt_);
		is_tell_fatigue = true;
	}
	if ( old_single_exp != left_single_exp_ )
	{
		session_oper.ToCall(BC_MakeP("SetSingleExp") << left_single_exp_);
		is_tell_fatigue = true;
	}
	if ( old_double_exp != left_double_exp_ )
	{
		session_oper.ToCall(BC_MakeP("SetDoubleExp") << left_double_exp_);
		is_tell_fatigue = true;
	}
	if ( old_map != cur_map_name_ )
	{
		session_oper.ToCall(BC_MakeP("SetCurMapRoomName")<<cur_map_name_);
	}
	if ( gold_ != old_gold )
	{
		session_oper.ToCall(BC_MakeP("SetCurGold")<<gold_);
	}
	/*if (is_tell_fatigue)
		TellFatigueData(CalcFatigueData());*/
	return true;
}

GPlayerMe::GPlayerMe() : time_to_refresh_my_sell_list_(1000 * 30)
{
	using namespace GType;
	delta_angle_ = (float)(kPi / 4);
	cur_hp_ = std::numeric_limits<int>::max();
	last_update_hit_hp_ = cur_hp_;
	feeb_hp_ = std::numeric_limits<int>::max();
	white_list_item_lvl_ = 0;
	white_list_item_quality_ = CD_En_ItemQuality::kIQ_Red;
	white_list_craft_item_lvl_ = -1;
	white_list_craft_item_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	white_list_lvl_up_equip_lvl_ = -1;
	white_list_lvl_up_equip_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	wait_safe_time_cnt_ = 0;
	white_list_facture_lvl_ = -1;
	white_list_facture_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	is_right_bt_hit_ = true;
	move_step_dist_ = 10.;
	//monster_most_high_ = 8.0f;
	//monster_most_high_fail_cnt_ = 0;
	is_send_move_room_ = false;
	last_room_id_for_send_room_ = -1;
	last_valid_pos_.second = 0;
	is_active_cross_map_ = false;
	is_hitting_monster_ = false;
	is_throw_data_error_ = false;
	min_gold_to_keep_buy_ = 20;
	min_silver_to_keep_buy_ = 150;
	cur_hit_mode_ = &hit_mode_mouse_;
	is_open_village_path_ = false;
	fb_server_full_cnt_ = 0;
	is_dealing_handle_ = false;
	handle_can_not_move_state_ = false;
	fb_consume_mode_ = GType::kFCM_All;
	fb_consume_mode_min_silver_ = 1000;
	move_fb_strict_ = false;

	ResetAllUpdateData();
	//////////////////////////////////////////////////////////////////////////
	/*monster_most_high_black_list_.insert("大怪鸟");
	monster_most_high_black_list_.insert("银眠鸟");
	monster_most_high_black_list_.insert("冰雷鸟");
	monster_most_high_black_list_.insert("黑狼鸟");
	monster_most_high_black_list_.insert("紫毒鸟");
	monster_most_high_black_list_.insert("沙雷鸟");
	monster_most_high_black_list_.insert("青怪鸟");
	monster_most_high_black_list_.insert("眠鸟");
	monster_most_high_black_list_.insert("毒怪鸟");*/
	//////////////////////////////////////////////////////////////////////////
	RandomCombineStr::GetDefMe().SetTotalLen_MinMax(kRoleNameMinLen, kRoleNameMaxLen);
}

void GPlayerMe::DebugInfo() const
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return;
	}
	auto obj_status = CD_En_ObjStatus::kInvalid;
	if (obj_status_)
		obj_status = *obj_status_;
	GType::stGameVector3 pos = GetPos();
	LOG_O(Log_debug) << "PlayerMe id:" << obj_id_ << 
		")\tname:" << name_ << "  cur map name:" << cur_map_name_ << "  obj status:" << obj_status
		<< " pos(" << pos.fx << "| " << pos.fy << "| " << pos.fz << ")" << " cur_map_id_:" << cur_map_id_;
}

bool GPlayerMe::DoWalk( const GType::stGameVector3& dst_pos )
{
	assert(false);
	return false;
	//return SetPosEnsure(dst_pos);
}

bool GPlayerMe::DeltaWalk( const GType::stGameVector3& delta_pos )
{
	assert(false);
	return false;
	/*if (IsUnValidPtr(pos_ptr_))
	{
		assert(false);
		return false;
	}
	auto pos_new = GetPos() + delta_pos;
	return DoWalk(pos_new);*/
}

bool GPlayerMe::SetPosInfoAsAnotherObj( const GameObjBase& obj )
{
	assert(false);
	return false;
	/*if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	auto pos_info = obj.GetPosInfo();
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	if ( !SetPosEnsure(obj.GetPos()) )
	{
		assert(false);
		return false;
	}

	return SetDir(obj.GetDirLhs(), obj.GetDirRhs());*/
}

void GPlayerMe::Test( int param )
{
	FuncObj_NormalC::GetInstance()->SendChooseRole(1, param);
	return;
	using namespace luabind;
	if ( param == 1 )
	{
		WaitUntil([](){
			DummySleep(1000);
			::Sleep(1000);
			GetIoService().PollOne();
			return true;
		}, 2000);
	}
	else if ( param == 2 )
	{
		auto nor = FuncObj_NormalC::GetInstance();
		const auto& tbl_globals = globals(nor->GetGLua().lua());
		const object& func_exist = tbl_globals["zhptest"];
		assert(LUA_TFUNCTION == type(func_exist));
		auto str = CallFunction(func_exist, std::string(""));	
	}
	else if ( param == 3 )
	{
		WaitUntil([](){
			DummySleep(10000);
			return true;
		}, 5000);

	}
	else if ( param == 4 )
	{
		DoWaitUntil([](){
			return false; 
		}, [](){
			DummySleep(10000);
		}, 5000);
	}
	else if ( param == 5 )
	{
		DoWaitUntil([](){
			return false; 
		}, [](){
			DummySleep(3000);
		}, 5000);
	}
}

bool GPlayerMe::UseSkill(CD_En_SkillId skill_category, WORD hit_times)
{
	
	if (IsUnValidPtr(game_skill_))
	{
		assert(false);
		return false;
	}
	while (hit_times)
	{
		if ( !GWndExecSync([=](){
			return game_skill_->UseSkill(skill_category);
		}) )
		{
			assert(false);
			return false;
		}
		if ( hit_times > 1 )
			DummySleep(250);
		--hit_times;
	}
	return true;
}

bool GPlayerMe::HitByLeftBt( WORD hit_times )
{
	return UseSkill(CD_En_SkillId::kSC_LeftBt, hit_times);
}

bool GPlayerMe::HitByRightBt( WORD hit_times )
{
	return UseSkill(CD_En_SkillId::kSC_RightBt, hit_times);
}

bool GPlayerMe::SwitchKnife()
{
	return UseSkill(CD_En_SkillId::kSC_KeyQ, 1);
}

OptinalBool GPlayerMe::HitMonster( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, int role_room_num, bool is_any_direction )
{
	using namespace GType;
	auto& hit_part = GMonster_HitPart::GetMonsterHitPart();
	bool is_do_hit_body_part = hit_part.HasHitCnt();
	if (is_do_hit_body_part)
	{
		//ReadLock read_lock(hit_part.GetBodyPartMutex());
		is_do_hit_body_part = hit_part.Update();
		if (is_do_hit_body_part)
		{
			auto bu_wei = hit_part.GetCurHitPart();
			if (bu_wei)
			{
				auto pos_info = obj.GetPosInfo();
				if (IsUnValidPtr(pos_info))
				{
					assert(false);
					return kOptionalBoolNil;
				}
				stGameVector3 the_pos;
				GameObjDirT the_dir;
				if ( !BodyPartInfo2RolePos(*pos_info, *bu_wei, the_pos, the_dir) )
				{
					assert(false);
					return kOptionalBoolNil;
				}
				if ( obj_room_num >= 0 )
				{
					obj.UpdateOwnerRoomNum();
					if ( obj.GetOwnerRoomIdx() != obj_room_num )
						return false;
				}
				//read_lock.unlock();
				//////////////////////////////////////////////////////////////////////////

				auto obj_packet_id = obj.GetPacketId();
				SetRolePosCallBackT call_back = [=, &the_pos](){
					return TestObjBodyPartPosInfoAtFindingPath(obj_packet_id, obj_mgr, obj_room_num, the_pos);
				};
				auto res = SetRoleToPosIndirect(the_pos, obj_room_num, role_room_num, -1, (obj_room_num < 0 ? nullptr : &call_back), IsTestTheMonsterPosZ(obj));
				if ( !CheckOptionalBoolRes(res) )
				{
					//assert(false);
					return res;
				}
				if ( !SetDir(the_dir.first, the_dir.second) )
				{
					assert(false);
					return kOptionalBoolNil;
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else
				is_do_hit_body_part = false;
		}
		else
			is_do_hit_body_part = false;
	}
	
	if ( !is_do_hit_body_part )
	{
		assert(cur_hit_mode_);
		auto res = SetRoleToBackIndirect(obj, obj_mgr, obj_room_num, role_room_num, is_any_direction, cur_hit_mode_ ? cur_hit_mode_->GetSteps() : 1);
		if ( !CheckOptionalBoolRes(res) )
		{
			//assert(false);
			return res;
		}
	}

	if ( !cur_hit_mode_ )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	if ( !cur_hit_mode_->DoHitOnce() )
	{
		CMN_ASSERT(false);
		return kOptionalBoolNil;
	}
	return true;
}

void GPlayerMe::SetDeltaAngle( float delta_angle )
{
	delta_angle_ = delta_angle;
}

GameObjDirT GPlayerMe::TwoPos2Dir( const GType::stGameVector3& vec_src, const GType::stGameVector3& vec_dst ) const
{
	/*
	此函数用的是角度、正弦、余弦的方式去求的，
	而Dir2Vec用的是向量旋转的方式去求的
	*/
	using namespace GType;
	return Vec2Dir(vec_dst - vec_src);
}

GType::stGameVector3 GPlayerMe::Dir2Vec( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs ) const
{
	auto vec_dir = dir_lhs;
	vec_dir.PlusXY(dir_rhs);
	const auto angle_45 = kPi / 4;
	//关于正北方向对称后
	vec_dir.RotateXY_Origin(float((angle_45 - vec_dir.GetAngleXY()) * 2));
	//世界坐标系中的向量
	vec_dir.RotateXY_Origin(delta_angle_);
	return vec_dir;
}

bool GPlayerMe::SetRoleForward( const GameObjBase& obj )
{
	if (IsUnValidPtr(pos_ptr_))
	{
		assert(false);
		return false;
	}
	return SetRoleForwardByPos(GetPos(), obj.GetPos());
}

bool GPlayerMe::MoveForward( int steps )
{
	assert(false);
	return false;
	/*if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	if ( steps < 1 )
	{
		assert(false);
		return false;
	}
	GType::stGameVector3 dir_lhs = dir_pos_info_->dir_1, dir_rhs = dir_pos_info_->dir_2;
	auto dir_vec = Dir2Vec(dir_lhs, dir_rhs);
	dir_vec.fx *= steps;
	dir_vec.fy *= steps;
	auto pos_tmp = GetPos();
	pos_tmp.PlusXY(dir_vec);
	return SetPosEnsure(pos_tmp);*/
}

bool GPlayerMe::AttrackGameObj( GameObjBase& obj )
{
	using namespace GType;
	auto pos_info = GetPosInfo();
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	if ( !GWndExecSync([this, pos_info, &obj](){
		using namespace GType;
		const WORD kSteps = 1;

		stGameVector3 dir_lhs = pos_info->dir_1;
		stGameVector3 dir_rhs = pos_info->dir_2;
		auto dir_vec = Dir2Vec(dir_lhs, dir_rhs);
		dir_vec.fx *= kSteps;
		dir_vec.fy *= kSteps;
		stGameVector3 dst_obj_pos;
		dst_obj_pos.ConvertToPos(*pos_info);
		auto pos_new = dst_obj_pos;
		pos_new.PlusXY(dir_vec);
		if ( !obj.SetPos(pos_new) )
		{
			assert(false);
			return false;
		}
		auto& dir_info = TwoPos2Dir(dst_obj_pos, pos_new);
		if ( !obj.SetDir(dir_info.first, dir_info.second) )
		{
			assert(false);
			return false;
		}
		return true;
	}) )
	{
		assert(false);
		return false;
	}

	return true;	
}

bool GPlayerMe::SetRoleForwardByPos( const GType::stGameVector3& pos_src, const GType::stGameVector3& pos_dst )
{
	auto& dir = TwoPos2Dir(pos_src, pos_dst);
	return SetDir(dir.first, dir.second);
}

bool GPlayerMe::StepVillageDoor( const stCrossDoorPosInfo& door_pos_info )
{
	assert(false);
	return false;
	/*if ( !SetPosEnsure(door_pos_info.door_dst_pos_) )
	{
		assert(false);
		return false;
	}
	if ( !SetRoleForwardByPos(door_pos_info.door_prev_pos_, door_pos_info.door_dst_pos_) )
	{
		assert(false);
		return false;
	}
	return MoveForward(10);*/
}

bool GPlayerMe::UpdateGrids()
{
	DO_EXCEPTION_TRY;
	if ( !grids_package_.RebuildAll() )
	{
		assert(false);
		return false;
	}
	if ( !grids_equip_.RebuildAll() )
	{
		assert(false);
		return false;
	}
	if ( !grids_store_.RebuildAll() )
	{
		assert(false);
		return false;
	}
	if ( !grids_su_cai_.RebuildAll() )
	{
		assert(false);
		return false;
	}
	if ( !grids_task_.RebuildAll() )
	{
		assert(false);
		return false;
	}
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

void GPlayerMe::DebugGridsInfo() const
{
	grids_package_.DebugInfo();
	grids_equip_.DebugInfo();
	grids_store_.DebugInfo();
	grids_su_cai_.DebugInfo();
	grids_task_.DebugInfo();
}

bool GPlayerMe::SendAcceptTask( int task_id )
{
	msg_on_accept_task_.reset();
	msg_commit_task_failed_.reset();

	stCD_Msg_AcceptTask	theAccpetTask;
	theAccpetTask.task_id_ = task_id;
	theAccpetTask.nFlag = 0;
	if (!CD_FuncCall::SendT(theAccpetTask))
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_accept_task_)
			return true;
		if (msg_commit_task_failed_)
			return true;
		return false;
	}, 1000 * 60) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "接受任务等待超时失败，任务ID：" << task_id;
		return false;
	}
	if (msg_commit_task_failed_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "接受任务失败，任务ID:" << task_id << " 错误码：" << msg_commit_task_failed_->res_;
		return false;
	}
	if ( !msg_on_accept_task_ )
	{
		assert(false);
		return false;
	}
	if ( msg_on_accept_task_->task_id_ != task_id )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "接受任务返回的任务ID不同了，要接受的任务ID：" << task_id << " 返回的任务ID：" << msg_on_accept_task_->task_id_;
		assert(false);
		return false;
	}
	return true;
}

int GPlayerMe::GetHitHp() const
{
	return cur_hp_;
}

void GPlayerMe::SetHitHp( DWORD package_id, int hit_hp )
{
	if ( package_id != packet_id_ )
	{
		if ( packet_id_ == std::numeric_limits<DWORD>::max() )
		{
			UpdatePacketIdSync();
			if ( package_id != packet_id_ )
			{
				return;
			}
		}
	}
	if ( packet_id_ == package_id )
	{
		if ( hit_hp != cur_hp_ )
		{
			if ( abs(abs(last_update_hit_hp_) - abs(hit_hp)) >= 30 )
			{
				LOG_O(Log_trace) << "hit hp, old:" << cur_hp_ << "  new hp:" << hit_hp;
				last_update_hit_hp_ = hit_hp;
			}
			if ( cur_hp_ <= 0 || feeb_hp_ <= 0 )
				feeb_hp_ = hit_hp;
			cur_hp_ = hit_hp;
			if ( GetCurHp() <= 0 )
			{
				LOG_O(Log_warning) << "角色死亡了hit";
				//角色死亡了
				if ( !is_hitting_monster_ )
				{
					ResetPassRoomData();
					return;
				}
				throw CmnPauseLogicException(kPauseId_RoleDead, "角色死亡了hit");
			}
		}
	}
}

int GPlayerMe::GetFeebHp() const
{
	return feeb_hp_;
}

void GPlayerMe::SetFeebHp( DWORD package_id, int feeb_hp )
{
	if ( package_id != packet_id_ )
	{
		if ( packet_id_ == std::numeric_limits<DWORD>::max() )
		{
			UpdatePacketIdSync();
			if ( package_id != packet_id_ )
			{
				return;
			}
		}
	}
	if ( packet_id_ == package_id )
	{
		if ( feeb_hp != feeb_hp_ )
		{
			feeb_hp_ = feeb_hp;

			if ( GetCurHp() <= 0 )
			{
				LOG_O(Log_warning) << "角色死亡了feeb";
				//角色死亡了
				if ( !is_hitting_monster_ )
				{
					ResetPassRoomData();
					return;
				}
				throw CmnPauseLogicException(kPauseId_RoleDead, "角色死亡了feeb");
			}
		}
	}
}

bool GPlayerMe::UpdateReceivedTasks()
{
	return received_task_mgr_.RebuildAll();
}

void GPlayerMe::DebugReceivedTaskInfo() const
{
	received_task_mgr_.DebugInfo();
}

bool GPlayerMe::UpdateAcceptableTasks()
{
	return acceptable_tasks_.Update();
}

void GPlayerMe::DebugAcceptableTasksInfo() const
{
	acceptable_tasks_.DebugInfo();
}

bool GPlayerMe::SendOpenNpc( DWORD npc_packet_id, GType::enNpcOpenType open_type )
{
	msg_on_open_npc_.reset();
	stCD_Msg_OpenNpc msg;
	msg.ukn_value_ = 2;
	msg.ukn_1_ = 1;
	msg.npc_id_ = npc_packet_id;
	msg.npc_type_ = (int)open_type;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_on_open_npc_)
			return true;
		return false;
	}, 1000 * 60) )
	{
		CMN_ASSERT(false);
		return false;
	}
	if ( !msg_on_open_npc_ )
	{
		assert(false);
		return false;
	}
	if ( 2 == msg_on_open_npc_->res_ )
		return true;
	//已经打开的状态
	if ( 0x00000502 == msg_on_open_npc_->res_ )
		return true;
	return false;
}

bool GPlayerMe::OpenNpc( const GameObjBase& npc, GType::enNpcOpenType open_type )
{
	if ( !npc.IsPackageIdValid() )
	{
		assert(false);
		return false;
	}
	return SendOpenNpc(npc.GetPacketId(), open_type);
}

GReceivedTaskMgr& GPlayerMe::GetReceivedTaskMgr()
{
	return received_task_mgr_;
}

GAcceptableTasks& GPlayerMe::GetAcceptableTasks()
{
	return acceptable_tasks_;
}

bool GPlayerMe::SendCommitTask( int task_id )
{
	msg_commit_task_failed_.reset();
	stCD_Msg_CommitTask msg;
	msg.task_id_ = task_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !TimerDoWaitUntil([this, task_id](){
		if ( !received_task_mgr_.FindById(task_id) )
			return true;
		if (msg_commit_task_failed_)
			return true;
		return false;
	}, [this](){
		UpdateReceivedTasks();
	}, 900, 1000 * 20) )
	{
		return false;
	}
	DummySleep(800);
	UpdateReceivedTasks();
	if ( !received_task_mgr_.FindById(task_id) )
		return true;
	if (msg_commit_task_failed_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "提交任务失败，错误码：" << msg_commit_task_failed_->res_;
		//3==物品携带数量过多
		if ( 3 == msg_commit_task_failed_->res_ )
		{
			AutoStoreItemsToStored();
		}
	}
	return false;
}

OptinalBool GPlayerMe::MoveToTheVillage( const std::string& dst_village_name, int room_num )
{
	using namespace GClass;
	assert(room_num >= 0);
	if (dst_village_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的村庄之参数不能为空";
		return kOptionalBoolNil;
	}
	UpdateRoleLvl_Notify();
	if ( role_lvl_ < GVillage2RoleLvlInfo::GetMe().Village2RoleLvl(dst_village_name) )
		return false;

	UpdateCurMapInfo_Notify();	
	if ( cur_map_name_ == dst_village_name )
	{
		if ( !MoveToTheRoom(room_num) )
			return kOptionalBoolNil;
		return true;
	}

	if ( !GiveUpFb() )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	if ( !LeaveFarm() )
	{
		CMN_ASSERT(false);
		return kOptionalBoolNil;
	}

	auto& gsp_village = GSP_Village::GetMe();
	if ( !gsp_village.Construct() )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	GSP_Stage::StepsT steps;
	if ( !gsp_village.VillageMove(cur_map_name_, dst_village_name, steps) )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	while ( !steps.empty() )
	{
		const auto& front_step = steps.front();
		if ( !front_step.step_stage_ )
		{
			assert(false);
			return kOptionalBoolNil;
		}
		if ( !front_step.step_gate_ )
			break;
		if ( !MoveToTheRoom(0) )
		{
			assert(false);
			return kOptionalBoolNil;
		}
#ifdef PREP_OPEN_VILLAGE_SHORTEST_PATH
		if (is_open_village_path_)
		{
			const auto& the_gate_pos = gsp_village.Gate2PosInfo(front_step.step_gate_->wrap_gate_id_);
			if ( !the_gate_pos )
			{
				assert(false);
				return kOptionalBoolNil;
			}
			UpdateOwnerRoomNum();
			if ( !CheckOptionalBoolRes(SetRoleToPosIndirect(*the_gate_pos, -1, GetOwnerRoomIdx(), -1, nullptr, false)) )
			{
				assert(false);
				return kOptionalBoolNil;
			}
		}
#endif
		if ( !SendMoveVillage(front_step.step_gate_->wrap_gate_id_) )
		{
			assert(false);
			return kOptionalBoolNil;
		}
		if ( !TimerDoWaitUntil([this, &front_step](){
			return cur_map_name_ == front_step.step_gate_->next_stage_name_;
		}, [this](){
			UpdateCurMapInfo_Notify();
		}, 500, 1000 * 60) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "移动到指定的村庄之过门失败，可能是超时所致";
			throw CmnPauseLogicException(kPauseId_PassMapErr, "移动到指定的村庄之过门失败，可能是超时所致ab");
			return kOptionalBoolNil;
		}
		steps.pop_front();
	}
	//DummySleep(1000);
	if ( !MoveToTheRoom(room_num) )
		return kOptionalBoolNil;
	return true;
}

bool GPlayerMe::IsInVillage() const
{
	if (cur_map_name_.empty())
	{
		if (!const_cast<GPlayerMe&>(*this).UpdateCurMapInfo_Notify())
		{
			assert(false);
			return false;
		}
		if (cur_map_name_.empty())
		{
			assert(false);
			return false;
		}
	}
	auto& gsp = GSP_Village::GetMe();
	if ( !gsp.Construct() )
	{
		assert(false);
		return false;
	}
	return !!gsp.FindStage(cur_map_name_);
}

bool GPlayerMe::SendMoveVillage( const std::string& gate_name )
{
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};
	
	stCD_Msg_MoveVillage msg;
	TStrCpy(msg.village_name_, gate_name.c_str(), sizeof(msg.village_name_));
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 30) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的村庄之过门失败，等待时间过长";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "过城镇等待时间过长");
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "过城镇等待可移动时间：" << kWaitTime_RoleCanMove << " 门的名字:" << gate_name;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "过城镇等待超时";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "过城镇等待时间过长b");
		return false;
	}
	return true;
}

bool GPlayerMe::UpdateCurMapInfo()
{
	DO_EXCEPTION_TRY;
	cur_map_db_ = CD_FuncCall::GetCurMapDb();
	if (!cur_map_db_)
	{
		assert(false);
		return false;
	}
	cur_map_name_.assign(CDF_EmptyStr(cur_map_db_->map_name_));
	cur_map_id_ = cur_map_db_->map_id_;
	cur_template_map_id_ = cur_map_db_->map_db_info_id_;
	if (cur_map_db_->map_db_info_)
		cur_map_db_info_name_.assign(CDF_EmptyStr(cur_map_db_->map_db_info_->name_));
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::AcceptTask( int task_id )
{
	received_task_mgr_.RebuildAll();
	if (received_task_mgr_.FindById(task_id))
		return true;
	if ( !SendAcceptTask(task_id) )
		return false;

	return TimerDoWaitUntil([this, task_id](){
		return !!received_task_mgr_.FindById(task_id);
	}, [this](){
		UpdateReceivedTasks();
	}, 400, 1000 * 10);
}

bool GPlayerMe::CommitTask( int task_id )
{
	if (IsInFb())
		return false;
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	auto task_template = task_template_mgr.FindById(task_id);
	if ( !task_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的任务信息，无法提交任务，任务ID:" << task_id;
		return false;
	}
	if ( !task_template->task_rewards_.empty() )
	{
		grids_package_.RebuildAll();
		for ( auto& reward_item_info : task_template->task_rewards_ )
		{
			/*//狩猎券要特殊处理
			if ( 405 == reward_item_info.item_id_ )
			{
				auto& item = FindTotalItem(reward_item_info.item_id_);
				if ( item.item_cnt_ > 0 )
				{
					auto the_cnt = item.item_cnt_ + reward_item_info.item_cnt_ - item.carry_limit_;
					if ( the_cnt > 0 )
					{

					}
				}
			}*/
			auto item_ptr = grids_package_.FindById(reward_item_info.item_id_);
			if ( !item_ptr )
				continue;
			assert(reward_item_info.item_cnt_ > 0);
			assert(item_ptr->item_cnt_ > 0);
			if ( item_ptr->carry_limit_ <= 0 )
				continue;
			assert(item_ptr->carry_limit_ >= item_ptr->item_cnt_);
			auto the_cnt = grids_package_.GetTheItemTotalCnt(reward_item_info.item_id_) + reward_item_info.item_cnt_ - item_ptr->carry_limit_;
			if ( the_cnt <= 0 )
				continue;
			CD_En_TabCategory tbl_category_dst;
			if (item_ptr->IsSuCai())
				tbl_category_dst = CD_En_TabCategory::kTC_SuCai;
			else
				tbl_category_dst = CD_En_TabCategory::kTC_Store;
			auto item = *item_ptr;
			item.item_cnt_ = the_cnt;
			if ( !SmartMoveItem(item, tbl_category_dst, -1) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "提交任务时移动物品到仓库失败，移动的物品：" << item.item_name_;
				return false;
			}
		}
	}
	return SendCommitTask(task_id);
}

OptinalBool GPlayerMe::MoveToTheFb( const std::string& fb_name )
{
	if (fb_name.empty())
	{
		//assert(false);
		return kOptionalBoolNil;
	}
	UpdateCurMapInfo_Notify();
	if ( cur_map_name_ == fb_name )
	{
		GSP_StagePosPath::GetMe().TestValidPosToDoorDist(GetCurTemplateMapId());
		return true;
	}
	if ( !GiveUpFb() )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	AutoStoreItemsToStored();

	enterable_fb_.RebuildAll();
	auto fb_info = enterable_fb_.FindByName(fb_name);

	/*const GFbInfo* fb_info = enterable_fb_.FindByName(fb_name);
	if ( !fb_info )
	{
		enterable_fb_.RebuildAll();
		fb_info = enterable_fb_.FindByName(fb_name);
		if ( !fb_info )
		{
			auto& all_fb = AllFbInfo::GetMe();
			all_fb.Update();
			fb_info = all_fb.FindByName(fb_name);
		}
	}*/
	
	if ( !fb_info )
	{
		auto& all_fb = MapDbMgr::GetMe();
		all_fb.Update();
		fb_info = all_fb.FindByName(fb_name);
		if ( fb_info && IsTheFbIdJuQingDone(fb_info->map_id_) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "该副本剧情已完成，无需再进";
			return false;
		}
		if ( !fb_info )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "无法进入该副本：" << fb_name;
			return false;
		}
		if (move_fb_strict_)
			return false;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "可接任务中没有遍历到该副本，竟然想要进入该副本，副本名：" << fb_name;
	}
	
	return MoveToTheFbImpl(*fb_info);
}

OptinalBool GPlayerMe::SendMoveToTheRoom( const std::string& move_cmd, const GType::stGameVector3& door_pos, int target_room_num, bool force_send )
{
	msg_on_send_move_ret_.reset();
	UpdateOwnerRoomNum();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "SendMoveToTheRoom, 当前所在房间号：" << GetOwnerRoomIdx() << " 目标房间号：" << target_room_num << " 门坐标：" << door_pos;
	stLastSendMoveRoomInfo tmp;
	tmp.cmd_ = move_cmd;
	tmp.dst_room_num_ = target_room_num;
	last_send_move_cmd_ = tmp;
	/*if ( !force_send )
		return SetRoleToPosIndirect(door_pos, -1, GetOwnerRoomIdx(), target_room_num, nullptr);*/
	auto res = SetRoleToPosIndirect(door_pos, -1, GetOwnerRoomIdx(), target_room_num, nullptr, false, true);
	if ( !CheckOptionalBoolRes(res) )
		return res;
	//////////////////////////////////////////////////////////////////////////
	/*for ( auto cnt = 0; cnt != 2; ++cnt )
	{
		if (PressKeyUntilSendMoveRoom(DIK_W, door_pos))
			return true;
		if (PressKeyUntilSendMoveRoom(DIK_A, door_pos))
			return true;
		if (PressKeyUntilSendMoveRoom(DIK_S, door_pos))
			return true;
		if (PressKeyUntilSendMoveRoom(DIK_D, door_pos))
			return true;
	}
	LOG_O(Log_debug) << "这样都没过成房间，我fuck";
	return false;*/
	//////////////////////////////////////////////////////////////////////////
	if (move_cmd.empty())
	{
		assert(false);
		return kOptionalBoolNil;
	}
	stCD_Msg_MoveRoom msg;
	msg.theDoorCoord = door_pos;
	memcpy_s(msg.szDoorCmd, sizeof(msg.szDoorCmd), move_cmd.c_str(), move_cmd.size());
	//////////////////////////////////////////////////////////////////////////
	//WriteLock wl(mutex_msg_on_hook_move_room_);
	if ( _strnicmp(msg.szDoorCmd, msg_on_hook_move_room_.msg_.szDoorCmd, _countof(msg.szDoorCmd)) == 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "游戏里已经发过过房间封包了，过房间命令：" << msg.szDoorCmd;
		return false;
	}
	msg_on_hook_move_room_.msg_ = msg;
	msg_on_hook_move_room_.msg_time_ = DateTimeLocal();
	//wl.unlock();*/
	//////////////////////////////////////////////////////////////////////////
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "过图发包信息，cmd:" << move_cmd << " pos(" << door_pos.fx << ", " << door_pos.fx << ", " << door_pos.fz << ")";*/
#ifdef PREP_UNOPEN_SHORTEST_PATH
	if ( !SendToSerThePos(door_pos, true) )
	{
		assert(false);
		return kOptionalBoolNil;
	}
#endif
	//DummySleep(500);
	if (CD_FuncCall::SendT(msg))
		return true;
	return kOptionalBoolNil;
}

bool GPlayerMe::UpdateEnterableFb()
{
	return enterable_fb_.RebuildAll();
}

void GPlayerMe::DebugEnterableFb() const
{
	enterable_fb_.DebugInfo();
}

OptinalBool GPlayerMe::SendMoveToTheFb( const GMapDb& fb_info, GType::enFbConsumeMode fb_mode )
{
	msg_on_enter_fb_.reset();
	msg_on_enter_fb_err_.reset();
	stCD_Msg_MoveFb1 msg_fb1;
	msg_fb1.dwUnknow1 = 2;
	msg_fb1.dwUnKnow2 = 0;
	msg_fb1.dwFbId = fb_info.map_id_;
	msg_fb1.nFbDiffLevel = (int)fb_info.fb_diff_level_;
	msg_fb1.dwFbType = (int)fb_info.fb_type_;
	if (!CD_FuncCall::SendT(msg_fb1) )
	{
		assert(false);
		return kOptionalBoolNil;
	}

	DummySleep(500);
	stCD_Msg_MoveFb2 msg_fb2;
	msg_fb2.dwFbId = fb_info.map_id_;
	msg_fb2.dwFbType = (int)AcceptTaskType::kNormal;
	if (!CD_FuncCall::SendT(msg_fb2))
	{
		assert(false);
		return kOptionalBoolNil;
	}
	if ( !WaitUntil([this](){
		if (msg_on_enter_fb_)
			return true;
		if (msg_on_enter_fb_err_)
			return true;
		return false;
	}, 1000 * 60 * 5) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "进入副本等待超时失败";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "进入副本等待超时失败a");
		return kOptionalBoolNil;
	}
	if (DoSendMoveToFbErr())
		return false;

	if ( CMDR_OnEnterFb::kFT_Normal == msg_on_enter_fb_->fb_type_ )
	{
		if ( kMsgRetError_XingDongLiBuZu == msg_on_enter_fb_->error_res_ )
		{
			LOG_O(Log_warning) << "进入普通副本之行动力不足，该换角色了";
			return kOptionalBoolNil;
		}
		else if ( kMsgRetError_DoneIt == msg_on_enter_fb_->error_res_ )
			//一个任务需要做两个副本的，如果已做过其中之一，再进就会失败
		{
			LOG_O(Log_error) << "任务中已打通过该副本，无需再进";
			return false;
		}
		else if ( msg_on_enter_fb_->error_res_ != 0 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "进入普通副本失败，错误码：" << msg_on_enter_fb_->error_res_;
			return false;
		}
	}
	else if ( CMDR_OnEnterFb::kFT_Sociaty == msg_on_enter_fb_->fb_type_ )
	{
		if ( msg_on_enter_fb_->error_res_ != 0 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "进入公会副本失败，错误码：" << msg_on_enter_fb_->error_res_;
			return false;
		}
	}
	else
	{
		if ( msg_on_enter_fb_->error_res_ != 0 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "进入未知类型的副本失败";
			return false;
		}
	}
	stCD_Msg_ChooseFbMode msg_fb_mode;
	msg_fb_mode.fb_id_ = fb_info.map_id_;
	msg_fb_mode.dwMode = (int)fb_mode;
	if (!CD_FuncCall::SendT(msg_fb_mode))
	{
		assert(false);
		return false;
	}
	//DummySleep(2000);
	if (!CD_FuncCall::SendT(msg_fb_mode))
	{
		assert(false);
		return false;
	}
	stCD_Msg_MoveFb3 msg_fb3;
	msg_fb3.dwFbType = (int)AcceptTaskType::kNormal;
	if (!CD_FuncCall::SendT(msg_fb3))
	{
		assert(false);
		return kOptionalBoolNil;
	}

	WaitUntil([this](){
		if (msg_on_enter_fb_err_)
			return true;
		return false;
	}, 1000 * 5);

	if (DoSendMoveToFbErr())
		return false;
	return true;
}

bool GPlayerMe::MoveToTheRoom( int room_num )
{
	if ( room_num < 0 || room_num > kMaxRoomId )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的房间之房间号不能小于0，且不能大于" << kMaxRoomId;
		return false;
	}
	UpdateOwnerRoomNum();
	if ( owner_room_num_ == room_num )
		return true;
	if ( owner_room_num_ < 0 )
	{
		if (IsInVillage())
			return true;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "自己的房间号小于0，无法移动到目标房间，当前所在地图：" << cur_map_name_ << " 当前所在房间号：" << owner_room_num_ << " 目标房间号：" << room_num;
		return false;
	}

	auto& fb_path = GSP_FbRoom::GetMe();
	if ( !fb_path.Construct() )
	{
		assert(false);
		return false;
	}
	GSP_Stage::StepsT steps;
	if ( !fb_path.VillageMove(std::to_string(owner_room_num_), std::to_string(room_num), steps) )
	{
		assert(false);
		return false;
	}
	auto& fb_mgr = FbDoorsMgr::GetMe();
	fb_mgr.RebuildAll();
	BOOST_SCOPE_EXIT_ALL(&){
		last_send_move_cmd_.reset();
		msg_on_send_move_ret_.reset();
		is_send_move_room_ = false;
	};
	LOG_ENTER_FUNC;
	const int kMaxCnt = 3;
	while ( !steps.empty() )
	{
		is_send_move_room_ = false;
		auto& cur_step = steps.front();
		if ( !cur_step.step_stage_ )
		{
			assert(false);
			return false;
		}
		if ( !cur_step.step_gate_ )
			break;
		auto fb_door = fb_mgr.FindByName(cur_step.step_gate_->wrap_gate_id_);
		if ( !fb_door )
		{
			assert(false);
			return false;
		}
		auto door_pos = fb_door->GetPos();
		//door_pos.fz += 5;
		//auto old_room_num = owner_room_num_;
		//int i = 0;
		//for ( ; i != kMaxCnt; ++i )
		{
			auto target_room_num = boost::lexical_cast<int>(cur_step.step_gate_->next_stage_name_);
			auto cur_room_num = boost::lexical_cast<int>(cur_step.step_stage_->stage_id_);
			UpdateOwnerRoomNum();
			if ( GetOwnerRoomIdx() != cur_room_num )
			{
				LOG_O(Log_error) << "移动房间失败1，目的房间号：" << cur_step.step_gate_->next_stage_name_ << " 原来所在房间：" << cur_room_num << " 当前房间：" << GetOwnerRoomIdx();
				return false;
			}
			auto send_move_res = SendMoveToTheRoom(cur_step.step_gate_->wrap_gate_id_, door_pos, target_room_num, false);
			if ( !CheckOptionalBoolRes(send_move_res) )
			{
				//assert(false);
				return false;
			}
			//DummySleep(1000);
			/*WaitUntil([this](){
				return is_send_move_room_;
			}, kWaitSendMoveRoolAfterSetPos);*/
			/*if ( !is_send_move_room_ )
			{
				UpdateOwnerRoomNum();
				if ( GetOwnerRoomIdx() != cur_room_num )
				{
					LOG_O(Log_error) << "移动房间失败2，目的房间号：" << cur_step.step_gate_->next_stage_name_ << " 原来所在房间：" << cur_room_num << " 当前房间：" << GetOwnerRoomIdx();
					return false;
				}
				LOG_O(Log_debug) << "从房间号[" << cur_room_num << "]移动到目标房间号[" << cur_step.step_gate_->next_stage_name_ << "]失败一次了，再尝试不断按键过房间一下";
				send_move_res = SendMoveToTheRoom(cur_step.step_gate_->wrap_gate_id_, door_pos, target_room_num, true);
				if ( !CheckOptionalBoolRes(send_move_res) )
				{
					//assert(false);
					return send_move_res;
				}
			}*/
			UpdateOwnerRoomNum();
			if ( cur_room_num != GetOwnerRoomIdx() && target_room_num != GetOwnerRoomIdx() )
			{
				LOG_O(Log_warning) << "从房间号[" << cur_room_num << "]移动到目标房间号[" << cur_step.step_gate_->next_stage_name_ << "]失败了，反而移动到该房间：" << GetOwnerRoomIdx();
				return false;
			}
			if ( !TimerDoWaitUntil([this, cur_step](){
				if ( msg_on_send_move_ret_ && cur_step.step_gate_->next_stage_name_ == std::to_string(owner_room_num_) )
					return true;
				return false;
			}, [this](){
				UpdateOwnerRoomNum();
			}, 100, 1000 * 60 * 5) )
			{
				LOG_O(Log_error) << "从房间号[" << cur_room_num << "]移动到目标房间号[" << cur_step.step_gate_->next_stage_name_ << "]失败1了,当前所在房间号：" << GetOwnerRoomIdx();
				throw CmnPauseLogicException(kPauseId_PassMapErr, "移动到目标房间失败了");
				return false;
			}
			//break;
		}
		/*if ( i >= kMaxCnt )
		{
			assert(false);
			LOG_O(Log_error) << "从房间号[" << cur_room_num << "]移动到目标房间号[" << cur_step.step_gate_->next_stage_name_ << "]失败2";
			return false;
		}*/
		//LOG_O(Log_trace) << "step room id:" << cur_step.step_stage_->stage_id_;
		steps.pop_front();
	}
	return true;
}

const std::string& GPlayerMe::GetCurMapName() const
{
	return cur_map_name_;
}

int GPlayerMe::GainSupplyItem( const std::string& item_name )
{
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获取补给箱物品之物品名参数不能为空";
		return 0;
	}
	auto& supply_info = GSupplyBox::GetMe();
	supply_info.Update();
	auto& item_mgr = ItemDbMgr::GetMe();
	if ( !item_mgr.RebuildAll() )
	{
		assert(false);
		return 0;
	}
	grids_package_.RebuildAll();
	int cnt = 0;
	for ( auto& v : supply_info.items_info_ )
	{
		auto item_template = item_mgr.FindById(v.item_id_);
		if ( !item_template )
		{
			assert(false);
			continue;
		}
		if ( item_template->item_name_ != item_name )
			continue;
		auto carried_cnt = v.item_cnt_;
		if ( item_template->carry_limit_ > 0 )
		{
			carried_cnt = grids_package_.GetTheItemTotalCnt(v.item_id_);
			if ( carried_cnt >= item_template->carry_limit_ )
				return cnt;
			carried_cnt = std::min(item_template->carry_limit_ - carried_cnt, v.item_cnt_);
			if ( carried_cnt <= 0 )
				return cnt;
		}		
		if ( !SendGainSupplyItem(v.item_id_, carried_cnt, v.slot_key_) )
		{
			//assert(false);
			return cnt;
		}
		cnt += carried_cnt;
		//DummySleep(1300);
		return cnt;
	}
	return cnt;	
}

bool GPlayerMe::SendGainSupplyItem( int item_id, int item_cnt, int slot_idx )
{
	assert(item_cnt > 0);
	stCD_Msg_GainSupplyItem theBuJiXiang;

	theBuJiXiang.slog_key_ = slot_idx;
	theBuJiXiang.dwItemId = item_id;
	theBuJiXiang.dwItemCount = item_cnt;
	grids_package_.RebuildAll();
	auto carried_cnt = grids_package_.GetTheItemTotalCnt(item_id);

	if (!CD_FuncCall::SendT(theBuJiXiang))
	{
		assert(false);
		return false;
	}

	if ( !TimerDoWaitUntil([=](){
		return carried_cnt != grids_package_.GetTheItemTotalCnt(item_id);
	}, [=](){
		grids_package_.RebuildAll();
	}, 100, 1000 * 20) )
	{
		LOG_O(Log_error) << "获取补给箱物品超时失败";
		return false;
	}
	return true;
}

int GPlayerMe::HitTheMonsterToDie( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int sleep_time, bool is_any_direction )
{
	int hit_cnt = 0;
	if ( !obj_mgr )
	{
		assert(false);
		return hit_cnt;
	}
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return hit_cnt;
	obj_mgr->DoRebuildAll();
	auto obj = obj_mgr->FindByPacketId(packet_id);
	if ( !obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "左键攻击指定的怪物直到死亡之已不存在该对象了a";
		return hit_cnt;
	}
	if ( kGOT_Monster != obj->GetGameObjType() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "左键攻击指定的怪物直到死亡之参数不是怪物类型";
		return hit_cnt;
	}
	auto the_monster = obj->CastTo<GMonster>();
	if (the_monster)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "当前要攻击的怪物：" << the_monster->GetCnName();
	}
	//////////////////////////////////////////////////////////////////////////
	/*monster_old_pos_ = obj->GetPos();
	BOOST_SCOPE_EXIT_ALL(this){
		monster_old_pos_.reset();
	};*/
	//////////////////////////////////////////////////////////////////////////
	/*monster_most_high_fail_cnt_ = 0;
	BOOST_SCOPE_EXIT_ALL(this){
		monster_most_high_fail_cnt_ = 0;
	};*/
LABLE_ROLE_IS_DEAD:
	try{
		assert(!is_hitting_monster_);
		is_hitting_monster_ = true;
		BOOST_SCOPE_EXIT_ALL(&){
			HitRaii();			
		};
		scmd::GetMe().WaitRoleCanMove(1000 * 60 * 2);
		int err_cnt = 0;
		while ( !obj_mgr->IsDeadMonsterByPacketId(packet_id) )
		{
			if (ambient.IsGameOver())
				break;
			auto res = HitMonsterByPacketId(packet_id, obj_mgr, is_any_direction);
			if ( !res || !*res )
			{
				if (obj_mgr->IsDeadMonsterByPacketId(packet_id))
					return hit_cnt;
				//assert(false);
				if ( !res )
					return hit_cnt;
				/*else
				++hit_cnt;*/

				if ( ++err_cnt >= 500 )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "打怪时，失败次数过多，放弃此次副本";
					CMN_ASSERT(false);
					if (GiveUpFb())
					{
						throw CmnSafeRun_SkipExcept("打怪时，失败次数过多，放弃此次副本");
					}
					else
					{
						LOG_O(Log_fatal) << "打怪时，失败次数过多，放弃副本失败，所以关闭此客户端";
						FuncObj_NormalC::Disconnect_Prompt("打怪失败次数过多");
						DummySleep(100000);
					}
				}
				DummySleep(50);
			}
			else
			{
				++hit_cnt;
				err_cnt = 0;
				if ( sleep_time > 0 )
					DummySleep(sleep_time);
			}
		}
	} DO_CATCH_THE_PAUSE_ID(kPauseId_RoleDead, {
		OnRoleDead();
		goto LABLE_ROLE_IS_DEAD;
	});
	return hit_cnt;
}

bool GPlayerMe::CollectTheCollectableObj( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int collect_time )
{
	return CollectTheCollectableObjById(obj.GetPacketId(), obj_mgr, collect_time);
}

bool GPlayerMe::PressKeyZ()
{
	//return UseSkill(GType::kSC_KeyZ, 1);
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return false;
	norc->DxPressKeyZ();
	return true;
}

bool GPlayerMe::TakeInKnife()
{
	using namespace GType;
	auto obj_status = GetObjStatus();
	if ( !obj_status )
	{
		Update();
		obj_status = GetObjStatus();
		if ( !obj_status )
		{
			assert(false);
			return false;
		}
	}
	if (CD_En_ObjStatus::kFree == *obj_status)
		return true;
	for ( int cnt = 0; cnt != 150; ++cnt )
	{
		if ( cnt % 8 == 0 )
		{
			if ( !SwitchKnife() )
			{
				assert(false);
				return false;
			}
		}
		DummySleep(100);
		if (CD_En_ObjStatus::kFree == *GetObjStatus())
		{
			DummySleep(1000);
			return true;
		}
	}
	//assert(false);
	LOG_O(Log_warning) << "收刀失败";
	return false;
}

bool GPlayerMe::UseItem(CD_En_ItemPackageCategory ipc, int item_slot_idx)
{
	assert(CD_En_ItemPackageCategory::kIPC_Item == ipc || CD_En_ItemPackageCategory::kIPC_Stored == ipc || CD_En_ItemPackageCategory::kIPC_Task == ipc);
	assert(item_slot_idx >= 0);
	msg_on_use_item_ret1_.reset();
	msg_on_use_item_ret2_.reset();
	msg_left_time_.reset();
	if ( !GWndExecSync([=](){
		auto use_mgr = CD_FuncCall::GetUseItemMgr();
		if (!use_mgr)
		{
			assert(false);
			return false;
		}
		return use_mgr->UseItem(ipc, item_slot_idx);
	}) )
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_use_item_ret1_)
			return true;
		if (msg_on_use_item_ret2_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 3) )
	{
		LOG_O(Log_debug) << "使用物品超时失败";
		return false;
	}
	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	if (msg_on_use_item_ret1_)
	{
		if ( !msg_on_use_item_ret1_->res_ )
			return false;
		return true;
	}
	if (msg_on_use_item_ret2_)
		return true;
	return false;
}

GItemMgr_Package& GPlayerMe::GetGridPackage()
{
	return grids_package_;
}

bool GPlayerMe::CutSkin( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int wait_time )
{
	return CutSkinById(obj.GetPacketId(), obj_mgr, wait_time);
	/*if ( kGOT_Monster != obj.GetGameObjType() )
	{
		assert(false);
		return false;
	}
	if ( !WaitToFree(0) )
	{
		assert(false);
		return false;
	}
	auto monster = obj.CastTo<GMonster>();
	if ( !monster )
	{
		assert(false);
		return false;
	}
	assert(monster->GetOwnerRoomIdx() >= 0);
	if ( monster->GetOwnerRoomIdx() != owner_room_num_ )
	{
		Update();
		if ( monster->GetOwnerRoomIdx() != owner_room_num_ )
		{
			if ( !MoveToTheRoom(monster->GetOwnerRoomIdx()) )
			{
				assert(false);
				return false;
			}
		}
	}
	if ( monster->GetOwnerRoomIdx() != owner_room_num_ )
	{
		assert(false);
		return false;
	}
	if ( !SetRoleToBack(obj, 0) )
	{
		assert(false);
		return false;
	}
	if ( !PressKeyZ() )
	{
		assert(false);
		return false;
	}
	if ( !WaitToFree(0) )
	{
		assert(false);
		return false;
	}
	if ( wait_time > 0 )
		DummySleep(wait_time);
	return true;*/
}

void GPlayerMe::DebugObjStatus() const
{
	LOG_O(Log_trace) << "obj status:" << *GetObjStatus();
}

bool GPlayerMe::WaitToFree( int wait_time )
{
	using namespace GType;
	//Update();
	auto obj_status = GetObjStatus();
	if ( !obj_status )
	{
		Update();
		obj_status = GetObjStatus();
		if ( !obj_status )
		{
			CMN_ASSERT(false);
			return false;
		}
	}
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "对象状态：" << *obj_status;*/
	if (CD_En_ObjStatus::kFree == *obj_status)
		return true;
	else if (CD_En_ObjStatus::kFighting== *obj_status)
	{
		if ( !TakeInKnife() )
		{
			CMN_ASSERT(false);
			return false;
		}
		return true;
	}
	else if (CD_En_ObjStatus::kDoing == *obj_status)
	{
		auto real_wait_time = wait_time <= 0 ? 1000 * 60 * 2 : wait_time;
		 if ( !TimerDoWaitUntil([this](){
			 auto status = *GetObjStatus();
			 if (CD_En_ObjStatus::kFree == status)
				 return true;
			 else if (CD_En_ObjStatus::kFighting == status)
			 {
				 if ( !TakeInKnife() )
				 {
					 CMN_ASSERT(false);
					 return false;
				 }
			 }
			 return false;
		 }, kFunc_DoNothing, 100, real_wait_time) )
		 {
			 LOG_O(Log_debug) << "等待到空闲状态之超时未能等待到空间状态1，等待时间：" << real_wait_time;
			 return false;
		 }
	}
	else
	{
		auto real_wait_time = wait_time <= 0 ? 1000 * 30 : wait_time;
		if ( !TimerDoWaitUntil([this](){
			auto status = *GetObjStatus();
			if (CD_En_ObjStatus::kFree == status)
				return true;
			else if (CD_En_ObjStatus::kFighting == status)
			{
				if ( !TakeInKnife() )
				{
					CMN_ASSERT(false);
					return false;
				}
			}
			return false;
		}, kFunc_DoNothing, 300, real_wait_time) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "等待到空闲状态之不能解析该状态：" << *obj_status;
			return false;
		}
	}
	//从非空闲状态到空闲状态的最后等待时间
	DummySleep(1500);
	return true;
}

bool GPlayerMe::UseItemByName( const std::string& item_name )
{
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "使用背包物品之物品名不能为空";
		return false;
	}

	auto& grid_package = GetGridPackage();
	grid_package.RebuildAll();
	auto item = grid_package.FindByName(item_name);
	if ( !item )
	{
		const auto& store_item = scmd::GetMe().FindStoreItem(item_name);
		if ( store_item.item_cnt_ > 0 )
		{
			if ( !store_item.CanBeUsed() )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "目前无法使用该物品：" << item_name;
				return false;
			}
		}
		scmd::GetMe().GainSomeItemsFromStore(item_name, 1);
		grid_package.RebuildAll();
		item = grid_package.FindByName(item_name);
		if ( !item )
			return false;
	}
	if ( !item->CanBeUsed() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "目前无法使用该物品：" << item_name;
		return false;
	}

	if (is_hitting_monster_)
	{
		UpdateCurMapInfo();
		if (IsInFb())
		{
			SetPosEnsureSync(GSP_StagePosPath::GetMe().FindNearestPos(cur_template_map_id_, GetOwnerRoomIdx(), GetPos()));
		}
		else
			assert(false);
	}

	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		return false;

	//因为要使用香油小刀，所以需要把延迟给注释掉
	//DummySleep(1000);
	item = grid_package.FindByName(item_name);
	if ( !item )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "使用背包物品之没有这样的物品2：" << item_name;*/
		CMN_ASSERT(false);
		return false;
	}
	if (!UseItem(CD_En_ItemPackageCategory::kIPC_Item, item->item_idx_))
	{
		//assert(false);
		return false;
	}
	//DummySleep(500);
	return WaitToFree(kUnFreeToFreeWaitTime);
}

OptinalBool GPlayerMe::HitMonsterByPacketId( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, bool is_any_direction )
{
	if ( !IsInFb())
		return kOptionalBoolNil;
	if ( !WaitRoleAlive() )
	{
		//assert(false);
	}
	
	obj_mgr->DoRebuildAll();
	auto obj = obj_mgr->FindByPacketId(packet_id);
	if ( !obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "左键攻击指定的怪物直到死亡之已不存在该对象了1";
		return kOptionalBoolNil;
	}

	HandleAtHitting();

	obj_mgr->DoRebuildAll();
	obj = obj_mgr->FindByPacketId(packet_id);
	if ( !obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "左键攻击指定的怪物直到死亡之已不存在该对象了2";
		return kOptionalBoolNil;
	}
	//////////////////////////////////////////////////////////////////////////
	obj->UpdateOwnerRoomNum();
	auto obj_room_idx = obj->GetOwnerRoomIdx();
	if ( obj_room_idx < 0 )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	UpdateOwnerRoomNum();
	if ( obj_room_idx != owner_room_num_ )
	{
		if ( !MoveToTheRoom(obj_room_idx) )
		{
			//assert(false);
			LOG_O(Log_debug) << "HitMonsterByPacketId，移动到[" << obj_room_idx << "]失败";
			return false;
		}
		scmd::GetMe().WaitRoleCanMove(1000 * 60 * 2);
		UpdateOwnerRoomNum();
		if ( obj_room_idx != owner_room_num_ )
		{
			//assert(false);
			return false;
		}
		/*if (monster_old_pos_)
			monster_old_pos_ = GetPos();*/
		//monster_most_high_fail_cnt_ = 0;

		obj_mgr->DoRebuildAll();
		obj = obj_mgr->FindByPacketId(packet_id);
		if ( !obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "左键攻击指定的怪物直到死亡之已不存在该对象了3";
			return kOptionalBoolNil;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	auto obj_shared_ptr = obj->shared_from_this();
	assert(obj_shared_ptr);
	return HitMonster(*obj_shared_ptr, obj_mgr, obj_room_idx, owner_room_num_, is_any_direction);
}

bool GPlayerMe::DoNpcChat( int task_id )
{
	if ( !SendDoNpcChat(task_id) )
	{
		//assert(false);
		return false;
	}
	return true;
	/*return TimerDoWaitUntil([this, task_id](){
		auto task = received_task_mgr_.FindById(task_id);
		if ( !task )
			return false;
		if (task->IsDone())
			return true;
		return false;
	}, [this](){
		UpdateReceivedTasks();
	}, 200, 1000 * 5);*/
}

bool GPlayerMe::SendDoNpcChat( int task_id )
{
	msg_on_do_npc_chat_.reset();
	stCD_Msg_DoNpcChat msg;
	msg.ukn_ = 0x2;
	msg.task_id_ = task_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_on_do_npc_chat_)
			return true;
		return false;
	}, 1000 * 10) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "与NPC进行对话，没有收到是否对话成功的收包";
		return false;
	}
	DummySleep(2000);
	return true;
}

GItemMgr_Store& GPlayerMe::GetStorePackage()
{
	return grids_store_;
}

bool GPlayerMe::DoCraft( const std::string& item_name )
{
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "锻造装备之装备名不能为空";
		return false;
	}
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "锻造装备之不存在这样的装备，装备名：" << item_name;
		return false;
	}
	auto& craft_mgr = GClass::CraftDbMgr::GetMe();
	craft_mgr.Update();
	auto craft_item = craft_mgr.FindByDstItemId(item_template->item_id_);
	if ( !craft_item )
	{
		auto& item_mgr = ItemDbMgr::GetMe();
		item_mgr.RebuildAll();
		auto the_template = item_mgr.FindById(item_template->item_id_);
		if ( !the_template )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "没有相应的锻造信息，装备ID：" << item_template->item_id_;
			return false;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有相应的锻造信息，装备名：" << item_template->item_name_;
		return false;
	}
	return DoCraftByDstEquipItemId(*craft_item);
}

bool GPlayerMe::DoBuy( const std::string& shop_category_name, const std::string& item_name, int buy_cnt, const std::string& village_name, const std::string& npc_name )
{
	/*if (IsInFb())
		return false;*/
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买物品之物品名不能为空";
		return false;
	}
	if ( buy_cnt <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买物品之购买数量不能小于1";
		return false;
	}
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买物品之不存在这样的物品，物品名：" << item_name;
		return false;
	}
	auto& shop_info = GClass::ShopDb::GetMe();
	shop_info.Update();
	auto shop_db = shop_info.FindByName(shop_category_name);
	if ( !shop_db )
		return false;
	const auto shop_item = shop_db->FindByItemId(item_template->item_id_);
	if (!shop_item)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买物品之没有相应的购买信息，物品名：" << item_name;
		return false;
	}
	
	auto old_buy_cnt = buy_cnt;
	buy_cnt = std::min(buy_cnt, shop_item->item_cnt_);
	if ( buy_cnt > 0 )
	{
		if (!scmd::GetMe().OpenTheNpcOfVillage(village_name, 0, npc_name, kEmptyStr, true))
			return false;
	}
	else
		return true;
	for ( ; old_buy_cnt > 0; old_buy_cnt -= buy_cnt )
	{
		msg_left_time_.reset();
		msg_on_buy_item_.reset();
		stCD_Msg_BuyItem theBuyItem;
		theBuyItem.dwBuyItemFlag = (int)shop_db->cate_id_;
		theBuyItem.dwShopId = shop_item->shop_id_;
		theBuyItem.nCount = buy_cnt;
		theBuyItem.dwFFFFFFFF1 = -1;
		theBuyItem.dwFFFFFFFF2 = -1;
		if (!CD_FuncCall::SendT(theBuyItem))
		{
			assert(false);
			return false;
		}
		if ( !TimerDoWaitUntil([this](){
			if (msg_on_buy_item_)
				return true;
			if (IsSafeTime())
				return true;
			return false;
		}, kFunc_DoNothing, 800, 1000 * 50) )
		{
			LOG_O(Log_error) << "购买商店物品超时失败，物品名：" << item_name;
			//continue;
			return false;
		}
		if (IsSafeTime())
		{
			WaitSafeTime();
			continue;
		}
		if ( !msg_on_buy_item_ )
		{
			assert(false);
			return false;
		}
		if ( 0 != msg_on_buy_item_->res_ )
		{
			LOG_O(Log_error) << "购买商店失败，商品名：" << item_name << " 错误码：" << msg_on_buy_item_->res_;
			return false;
		}
	}	
	return true;
}

bool GPlayerMe::LvlUpEquip(  const std::string& equip_name, int lvl_up_equip_idx )
{
	if (equip_name.empty())
	{
		assert(false);
		return false;
	}
	grids_equip_.RebuildAll();
	auto the_item = grids_equip_.FindByName(equip_name);
	if ( !the_item )
	{
		grids_package_.RebuildAll();
		the_item = grids_package_.FindByName(equip_name);
		if ( !the_item )
		{
			grids_store_.RebuildAll();
			the_item = grids_store_.FindByName(equip_name);
			if ( !the_item )
			{
				grids_su_cai_.RebuildAll();
				the_item = grids_su_cai_.FindByName(equip_name);
			}
		}
	}
	if ( !the_item )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的装备，无法升级：" << equip_name;
		return false;
	}
	if ( lvl_up_equip_idx < 0 )
		lvl_up_equip_idx = 0;
	auto item_tmp = *the_item;
	return LvlUpTheEquip(item_tmp, lvl_up_equip_idx, true);
}

int GPlayerMe::GetCurTemplateMapId() const
{
	return cur_template_map_id_;
}

bool GPlayerMe::UpdateImpl()
{
	//////////////////////////////////////////////////////////////////////////
	cur_role_ = CD_FuncCall::GetCurRole();
	if (IsUnValidPtr(cur_role_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(cur_role_->local_obj_))
	{
		assert(false);
		return false;
	}
	if (!SetCurLocalObj(cur_role_->local_obj_))
	{
		assert(false);
		return false;
	}
	if ( !__super::UpdateImpl() )
	{
		assert(false);
		return false;
	}
	if (!local_obj_)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(local_obj_->skil_obj_))
		game_skill_ = nullptr;
	else
		game_skill_ = local_obj_->skil_obj_;

	if (!UpdateCurMapInfo())
	{
		assert(false);
		return false;
	}
	if (!UpdateOwnerRoomNum())
	{
		assert(false);
		return false;
	}
	if ( !UpdateRoleLvl() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateExp() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateGold() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateLianQiCao() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateZhanWeiZhi() )
	{
		assert(false);
		return false;
	}
	if ( !UpdatePoisoningState() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateJingSuanDian() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateShouLieQuanCnt() )
	{
		assert(false);
		return false;
	}
	if ( !UpdateHp() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::SendMoveItem(const LONGLONG& item_serial, CD_En_TabCategory tbl_category_src, int slot_idx_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst)
{
	stCD_Msg_MoveItemTo theItemMoveTo;
	theItemMoveTo.item_serial_ = item_serial;
	theItemMoveTo.bScrItemTabIndex = (BYTE)tbl_category_src;
	theItemMoveTo.wScrItemSlotIndex = (WORD)slot_idx_src;
	theItemMoveTo.bDstItemTabIndex = (BYTE)tbl_category_dst;
	theItemMoveTo.wDstItemSlotIndex = (WORD)slot_idx_dst;

	return CD_FuncCall::SendT(theItemMoveTo);
}

bool GPlayerMe::MoveTotalItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst)
{
	using namespace GType;
	if ( slot_idx_dst < 0 )
	{
		assert(false);
		return false;
	}
	if ( item_src.item_cnt_ <= 0 )
	{
		assert(false);
		return false;
	}
	GItemMgrBase* item_mgr = GetItemMgr(tbl_category_dst);
	if ( !item_mgr )
	{
		assert(false);
		return false;
	}
	item_mgr->RebuildAll();

	if ( slot_idx_dst >= item_mgr->GetGridMaxCnt() )
	{
		assert(false);
		return false;
	}
	if ( !item_src.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	/*if ( CD_En_TabCategory::kTC_Package == tbl_category_dst && item_src.carry_limit_ > 0 )
	{
		auto exist_cnt = item_mgr->GetTheItemTotalCnt(item_src.item_id_);
		exist_cnt += item_src.item_mgr_ptr_->GetTheItemTotalCnt(item_src.item_id_);
		if ( exist_cnt > item_src.carry_limit_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "超过携带上限，取物品失败，物品名：" << item_src.item_name_;
			return false;
		}	
	}*/	
	if ( !SendMoveItem(item_src.item_serial_, item_src.item_mgr_ptr_->GetTabCategory(), item_src.item_idx_, tbl_category_dst, slot_idx_dst) )
	{
		assert(false);
		return false;
	}
	if ( !TimerDoWaitUntil([&item_src, item_mgr, slot_idx_dst](){
		auto item = item_mgr->FindBySerial(item_src.item_serial_);
		if ( !item )
			return false;
		if ( !item->item_mgr_ptr_ )
			return false;
		if ( item->item_idx_ != slot_idx_dst )
			return false;
		return true;
	}, [item_mgr](){
		item_mgr->RebuildAll();
	}, 700, 1000 * 5) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动整个物品之超时失败,物品名：" << item_src.item_name_ /*<< " 目标栏目：" << tbl_category_dst << " 目标位置索引：" << slot_idx_dst*/;
		return false;
	}
	return true;
}

GItemMgrBase* GPlayerMe::GetItemMgr(CD_En_TabCategory tab_category)
{
	if (CD_En_TabCategory::kTC_Package == tab_category)
	{
		return &grids_package_;
	}
	else if (CD_En_TabCategory::kTC_Task == tab_category)
		return &grids_task_;
	else if (CD_En_TabCategory::kTC_Store == tab_category)
	{
		return &grids_store_;
	}
	else if (CD_En_TabCategory::kTC_SuCai == tab_category)
		return &grids_su_cai_;
	else if (CD_En_TabCategory::kTC_Equip == tab_category)
	{
		return &grids_equip_;
	}
	return nullptr;
}

GItemMgr_Equip& GPlayerMe::GetEquipPackage()
{
	return grids_equip_;
}

bool GPlayerMe::SetRoleToBackDirect( const GameObjBase& obj, int steps /*= 1 */ )
{
	assert(false);
	return false;
	/*CloseAllUi();
	
	auto& dir_lhs = obj.GetDirLhs();
	auto& dir_rhs = obj.GetDirRhs();
	auto dir_vec = Dir2Vec(dir_lhs, dir_rhs);
	dir_vec.RotateXY_Origin((float)kPi);
	dir_vec.fx *= steps;
	dir_vec.fy *= steps;
	auto pos_tmp = obj.GetPos();
	pos_tmp.PlusXY(dir_vec);
	if ( !SetPosEnsure(pos_tmp) )
	{
		assert(false);
		return false;
	}
	auto& dir_info = TwoPos2Dir(pos_tmp, obj.GetPos());
	if ( !SetDir(dir_info.first, dir_info.second) )
	{
		assert(false);
		return false;
	}
	return true;*/
}

OptinalBool GPlayerMe::SetRoleToBackIndirect( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, int role_room_num, bool is_any_direction, int steps /*= 1 */ )
{
	GType::stGameVector3 dir_lhs;
	GType::stGameVector3 dir_rhs;
	if (is_any_direction)
		//这里是任意的面向
	{
		dir_lhs = GetDirLhs();
		dir_rhs = GetDirRhs();
	}
	else
		//这里是怪物对象的面向
	{
		dir_lhs = obj.GetDirLhs();
		dir_rhs = obj.GetDirRhs();
	}
	
	auto dir_vec = Dir2Vec(dir_lhs, dir_rhs);
	dir_vec.RotateXY_Origin((float)kPi);
	dir_vec.fx *= steps;
	dir_vec.fy *= steps;

	const auto& dst_obj_pos = obj.GetPos();
	obj.UpdateOwnerRoomNum();
	auto new_obj_room_num = obj.GetOwnerRoomIdx();
	if ( obj_room_num >= 0 )
	{
		if ( new_obj_room_num != obj_room_num )
			return false;
		if (IsInFb())
		{
			UpdateOwnerRoomNum();
			if ( new_obj_room_num != GetOwnerRoomIdx() )
				return false;
		}
	}

	auto pos_tmp = dst_obj_pos;
	pos_tmp.PlusXY(dir_vec);
	/*if (monster_old_pos_)
	{
		auto the_z = monster_old_pos_->fz + monster_most_high_;
		if ( pos_tmp.fz > the_z )
			pos_tmp.fz = the_z;
	}*/
	/*if ( kGOT_Monster == obj.GetGameObjType() )
	{
		auto the_boss = obj.CastTo<GMonster>();
		if ( the_boss && the_boss->IsBoss() && IsInMonsterHighBlackList(the_boss->GetCnName()) )
		{
			auto the_z = GetMapCoorDinates_Z() + monster_most_high_;
			if ( pos_tmp.fz > the_z )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "怪物Z坐标太高了2，最大Z值: " << the_z << " 而怪物的Z值：" << pos_tmp.fz;
				pos_tmp.fz = the_z;
				if ( ++monster_most_high_fail_cnt_ >= kMonsterMostHighFaileMaxCnt )
				{
					monster_most_high_fail_cnt_ = 0;
				}
				else
				{
					DummySleep(kSleepTimeAtPosZError);
					return false;
				}
			}
		}
		else
			monster_most_high_fail_cnt_ = 0;
	}
	else
		monster_most_high_fail_cnt_ = 0;*/

	auto obj_packet_id = obj.GetPacketId();
	SetRolePosCallBackT call_back = [=, &pos_tmp](){
		return TestObjPosInfoAtFindingPath(obj_packet_id, obj_mgr, obj_room_num, pos_tmp);
	};
	auto res = SetRoleToPosIndirect(pos_tmp, obj_room_num, role_room_num, -1, ((obj_room_num < 0 || !IsInFb()) ? nullptr : &call_back), IsTestTheMonsterPosZ(obj));
	if ( !CheckOptionalBoolRes(res) )
	{
		//assert(false);
		return res;
	}
	if ( !is_hitting_monster_ )
	{
		if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		{
			CMN_ASSERT(false);
			return false;
		}
	}
	//设置坐标的一定都得放到SetRoleToPosIndirect里
	/*if ( !SetPosEnsureSync(pos_tmp) )
		return kOptionalBoolNil;*/

	auto& dir_info = TwoPos2Dir(pos_tmp, dst_obj_pos);
	if ( !SetDirEnsure(dir_info.first, dir_info.second) )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	return true;
}

bool GPlayerMe::Dist2Steps( const GType::stGameVector3& pos_src, const GType::stGameVector3& pos_dst, PosQueueT& pos_steps )
{
	using namespace GType;
	if ( !pos_steps.empty() )
		return true;
	//////////////////////////////////////////////////////////////////////////
	//****************以下是两点间按距离瞬移***********************************
	auto pos_z = std::max(pos_src.fz, pos_dst.fz);
	pos_z += 15.;
	/*auto the_z = GetMapCoorDinates_Z() + monster_most_high_;
	if ( pos_z > the_z )
		pos_z = the_z;*/

	auto pos_dist = pos_src.Distance2d(pos_dst);
	auto move_cnt = (int)(pos_dist / move_step_dist_);
	float angle = 0.;
	if ( move_cnt > 0 )
	{
		auto vec_dir = pos_dst - pos_src;
		angle = atan2(vec_dir.fy, vec_dir.fx);
	}
	stGameVector3 pos_tmp = pos_src;
	pos_tmp.fz = pos_z;
	while ( move_cnt > 0 )
	{
		pos_tmp.fx += move_step_dist_ * cos(angle);
		pos_tmp.fy += move_step_dist_ * sin(angle);
		pos_steps.push_back(pos_tmp);
		--move_cnt;
	}
	return true;
	//////////////////////////////////////////////////////////////////////////
}

void GPlayerMe::SetMoveStepDist( float step_dist )
{
	move_step_dist_ = step_dist;
}

bool GPlayerMe::EnterSelfFarm()
{
	//////////////////////////////////////////////////////////////////////////
	//使用精算点物品
	UseSomeItems(kItemName_JingSuan, std::numeric_limits<int>::max());
	//////////////////////////////////////////////////////////////////////////
	UpdateCurMapInfo_Notify();
	if ( cur_map_id_ == kSelfFarmFbId )
		return true;
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};
	auto cur_role = CD_FuncCall::GetCurRole();
	if (!cur_role)
	{
		assert(false);
		return false;
	}
	auto the_id = cur_role->farm_facility_.self_farm_id_;
	if ( 0 == the_id )
	{
		LOG_O(Log_debug) << "farm id为0，无法进入庄园";
		return false;
	}
	stCD_Msg_EnterFarm msg;
	msg.farm_id_ = the_id;
	if (!CD_FuncCall::SendT(msg))
	{
		//assert(false);
		return false;
	}

	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "进入到自己的庄园之过门失败，等待30秒钟未能成功移动";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "庄园之过门失败");
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入自己的庄园等待可移动时间：" << kWaitTime_RoleCanMove;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "进入自己的庄园等待超时";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "进入自己的庄园等待超时b");
		return false;
	}

	if ( !TimerDoWaitUntil([this](){
		return cur_map_id_ == kSelfFarmFbId;
	}, [this](){
		UpdateCurMapInfo_Notify();
	}, 500, 1000 * 15) )
	{
		LOG_O(Log_error) << "进入到自己的庄园之过门失败，可能是超时所致";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "庄园之过门失败b");
		return false;
	}
	return true;
}

bool GPlayerMe::LeaveFarm()
{
	UpdateCurMapInfo_Notify();
	if ( cur_map_id_ != kSelfFarmFbId )
		return true;
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};

	CalcNextCollectFarmTime();
	stCD_Msg_GiveUpFb msg;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "离开庄园之过门失败，等待2分钟未能成功移动";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "离开庄园之过门失败a");
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "离开庄园等待可移动时间：" << kWaitTime_RoleCanMove;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "离开庄园等待超时";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "离开庄园之过门失败b");
		return false;
	}

	if ( !TimerDoWaitUntil([this](){
		return IsInVillage();
	}, [this](){
		UpdateCurMapInfo_Notify();
	}, 500, 1000 * 60) )
	{
		LOG_O(Log_error) << "离开庄园之过门失败，可能是超时所致";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "离开庄园之过门失败c");
		return false;
	}
	return true;
}

void GPlayerMe::HandleAtHitting()
{
	using namespace luabind;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	auto& handle_at_hitting = norc->GetHandleAtHitting();
	if ( !handle_at_hitting )
	{
		assert(false);
		return;
	}
	auto& handle_tbl = handle_at_hitting->GetLuaObj(kHandleAtHitting);
	if ( !handle_tbl.is_valid() )
		return;
	if ( LUA_TTABLE != type(handle_tbl) )
		return;
	for ( iterator it(handle_tbl), it_end; it != it_end; ++it )
	{
		auto& tbl_item = *it;
		assert(LUA_TTABLE == type(tbl_item));
		auto& judge_func = tbl_item[1];
		assert(LUA_TFUNCTION == type(judge_func));
		if (CallFunction(judge_func, false))
		{
			auto& do_func = tbl_item[2];
			CallFunction(do_func, object());
		}
	}
}

bool GPlayerMe::SendOpenMail()
{
	stCD_Msg_OpenMail msg;
	msg.flag_ = 0xff;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	return true;
}

bool GPlayerMe::SendDeleteMail( const LONGLONG& mail_serial )
{
	stCD_Msg_DeleteMail msg;
	msg.nMailDeleteFlag = 1;
	msg.llMailId = mail_serial;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	auto& mail_mgr = GMailMgr::GetMe();
	mail_mgr.RebuildAll();
	return TimerDoWaitUntil([&mail_serial, &mail_mgr](){
		return !mail_mgr.FindBySerial(mail_serial);
	}, [&mail_mgr](){
		mail_mgr.RebuildAll();
	}, 200, 1000 * 60);
}

bool GPlayerMe::SendGainMailItem( const LONGLONG& mail_serial )
{
	using namespace GClass;
	stCD_Msg_GainMailItem theMailItem;

	theMailItem.llMailId = mail_serial;
	theMailItem.bFlag = 0;
	theMailItem.nUnKnow = -1;
	theMailItem.nItemTabIndex = 1;		// 0 表示收件到背包， 1表示收件到仓库
	theMailItem.ukn_ = 2;
	msg_on_accept_mail_.reset();
	if (!CD_FuncCall::SendT(theMailItem))
	{
		assert(false);
		return false;
	}
	
	if ( !WaitUntil([this](){
		if (msg_on_accept_mail_)
			return true;
		return false;
	}, 1000 * 60) )
	{
		assert(false);
		return false;
	}
	if ( !msg_on_accept_mail_ )
	{
		assert(false);
		return false;
	}
	if ( 0 == msg_on_accept_mail_->res_ )
		return true;
	
	if ( -2 == msg_on_accept_mail_->error_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "提取附件失败:超出携带上限";
	}
	return false;
}

int GPlayerMe::LvlUpCollectOfFarm(CD_En_FarmFacilityIdx idx, int lvl_limit, bool is_open_npc)
{
	using namespace GClass;
	if (idx < CD_En_FarmFacilityIdx::kOre || idx >= CD_En_FarmFacilityIdx::kInvalid)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "升级庄园采集之索引必须在1到" << CD_En_FarmFacilityIdx::kInvalid << "之间";
		return 0;
	}
	if ( !IsInSelfFarm() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不在庄园内，无法升级";
		return 0;
	}
	cur_farm_facility_info_.Update();
	auto the_jsd = cur_farm_facility_info_.FindByIdx(idx);
	if ( !the_jsd )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的索引，无法升级：" << (int)idx + 1;
		return 0;
	}
	if ( lvl_limit > 0 )
	{
		if ( the_jsd->lvl_ >= lvl_limit )
			return 0;
	}
	else
		lvl_limit = the_jsd->lvl_ + 1;
	auto& template_mgr = FarmDbMgr::GetMe();
	template_mgr.Update();
	bool exec_once = false;
	auto cnt = 0;
	stCD_Msg_UpCollect msg;
	for ( auto lvl = the_jsd->lvl_; lvl <= lvl_limit; ++lvl )
	{
		auto the_template = template_mgr.FindById(the_jsd->farm_db_id_);
		if ( !the_template )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "没有这样的升级信息：" << (int)idx + 1;
			return cnt;
		}
		UpdateRoleLvl_Notify();
		if ( role_lvl_ < the_template->role_lvl_ )
			return cnt;
		UpdateJingSuanDianSync();
		if ( jing_suan_dian_ < the_template->jing_suan_dian_ )
			return cnt;
		UpdateGold_Notify();
		if ( silver_ < the_template->bind_gold_ )
			return cnt;
		assert(the_jsd->lvl_ == the_template->the_lvl_);

		auto old_lvl = the_jsd->lvl_;
		if ( !exec_once )
		{
			exec_once = true;
			if (is_open_npc)
			{
				if (!scmd::GetMe().OpenTheNpcOfVillage(kEmptyStr, 0, kFarmGaoShiPai, kEmptyStr, true))
				{
					return cnt;
				}
			}
		}
		msg.collect_idx_ = (int)idx;
		if (!CD_FuncCall::SendT(msg))
		{
			assert(false);
			return cnt;
		}
		if ( !TimerDoWaitUntil([this, old_lvl, idx](){
			auto the_jsd = cur_farm_facility_info_.FindByIdx(idx);
			if ( !the_jsd )
				return false;
			return old_lvl != the_jsd->lvl_;
		}, [this](){
			cur_farm_facility_info_.Update();
		}, 700, 1000 * 30) )
		{
			assert(false);
			return cnt;
		}
		++cnt;
		if ( lvl_limit - lvl > 1 )
		{
			cur_farm_facility_info_.Update();
			the_jsd = cur_farm_facility_info_.FindByIdx(idx);
			if ( !the_jsd )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "没有这样的索引，无法升级2：" << (int)idx + 1;
				return cnt;
			}
		}
	}
	return cnt;
}

bool GPlayerMe::CollectTheCollectableObjById( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int collect_time )
{
	if ( !obj_mgr )
	{
		assert(false);
		return false;
	}
	if ( !WaitRoleAlive() )
	{
		assert(false);
	}
	HandleAtHitting();

	scmd::GetMe().CloseAllUi();
	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		assert(false);
		return false;
	}

	obj_mgr->DoRebuildAll();
	auto collect_item = obj_mgr->FindByPacketId(packet_id);
	if ( !collect_item )
		return false;
	if ( !collect_item->UpdateOwnerRoomNum() )
		return false;
	auto obj_room_idx = collect_item->GetOwnerRoomIdx();
	if ( obj_room_idx != -1 )
	{
		if ( obj_room_idx < 0 )
		{
			assert(false);
			return false;
		}
		UpdateOwnerRoomNum();

		if ( obj_room_idx != owner_room_num_ )
		{
			if ( !MoveToTheRoom(obj_room_idx) )
			{
				//assert(false);
				return false;
			}
			UpdateOwnerRoomNum();
			if ( obj_room_idx != owner_room_num_ )
			{
				assert(false);
				return false;
			}
			obj_mgr->DoRebuildAll();
			collect_item = obj_mgr->FindByPacketId(packet_id);
			if ( !collect_item )
				return false;
		}
	}
	
	auto the_obj = collect_item->shared_from_this();
	if ( !the_obj )
	{
		assert(false);
		return false;
	}
	return CollectTheCollectableObjImpl(*the_obj, obj_mgr, collect_time);
}

bool GPlayerMe::CollectTheCollectableObjImpl( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int collect_time )
{
	if ( kGOT_Collect != obj.GetGameObjType() )
	{
		assert(false);
		return false;
	}
	UpdateOwnerRoomNum();
	if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
	{
		//assert(false);
		return false;
	}
	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		assert(false);
		return false;
	}
	msg_on_collect_ret_.reset();
	if ( !PressKeyZ() )
	{
		assert(false);
		return false;
	}
	if ( !WaitToDoingStatus(1500) )
	{
		LOG_O(Log_debug) << "没有采集到，再次采集次1";
		if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
		{
			//assert(false);
			return false;
		}
		scmd::GetMe().PressKeyW();
		if ( !PressKeyZ() )
		{
			assert(false);
			return false;
		}
		if ( !WaitToDoingStatus(1500) )
		{
			LOG_O(Log_debug) << "没有采集到，再次采集次2";
			if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
			{
				//assert(false);
				return false;
			}
			scmd::GetMe().PressKeyW();
			if ( !PressKeyZ() )
			{
				assert(false);
				return false;
			}
			if ( !WaitToDoingStatus(1500) )
				return false;
		}
	}

	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		if ( !PressKeyZ() )
		{
			assert(false);
			return false;
		}
		if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		{
			assert(false);
			return false;
		}
	}
	if (msg_on_collect_ret_)
	{
		if ( !msg_on_collect_ret_->IsSucceed() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "采集失败，错误码：" << msg_on_collect_ret_->res_ << " 获取的物品ID：" << msg_on_collect_ret_->gain_item_id_;
			return false;
		}
	}
	if ( collect_time > 0 )
		DummySleep(collect_time);

	if ( !msg_on_collect_ret_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "采集失败，不知为何没收到采集结果封包";
		return false;
	}
	//LOG_O(Log_trace) << "采集了一次,返回真";
	return true;
}

int GPlayerMe::GetCurHp() const
{
	return std::min(cur_hp_, feeb_hp_);
}

bool GPlayerMe::WaitRoleAlive()
{
	if ( GetCurHp() > 0 )
		return true;
	//msg_on_buff_status_.reset();
	poisoning_state_ = CD_En_PoisonStatus::kOk;
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "角色死亡了，等待角色复活";
	/*return TimerDoWaitUntil([this](){
		return GetCurHp() > 0;
	}, kFunc_DoNothing, 300, 1000 * 60 * 2);*/
	return scmd::GetMe().WaitRoleCanMove(1000 * 60 * 3);
}

OptinalBool GPlayerMe::SetRoleToPosIndirect( const GType::stGameVector3& dst_pos, int obj_room_num, int role_room_num,
	int target_room_num, const SetRolePosCallBackT* call_back, bool is_test_dst_pos_z, bool is_pass_room)
{
	using namespace GClass;
	auto& ambinet = GAmbient::GetMe();
	if (scmd::GetMe().IsInFb() && ambinet.cond_fb_game_over_lose_.CondtionTrue())
		return kOptionalBoolNil;
	scmd::GetMe().CloseAllUi();
	ResetSendRoomTime();

	const auto& role_pos = GetPos();
	UpdateOwnerRoomNum();
	auto old_role_room_num = GetOwnerRoomIdx();
	if ( role_room_num >= 0 )
	{
		if ( old_role_room_num != role_room_num )
			return false;
	}
	if ( obj_room_num >= 0 )
	{
		if ( old_role_room_num != obj_room_num )
			return false;
	}

	
#ifdef PREP_UNOPEN_SHORTEST_PATH
	bool is_shortest_path = false;
#else

	bool is_shortest_path = IsInFb();

#ifdef PREP_OPEN_VILLAGE_SHORTEST_PATH
	if ( !is_shortest_path )
	{
		is_shortest_path = is_open_village_path_;
	}
#endif

#endif	

	PosQueueT pos_steps;
	if (is_shortest_path)
	{
		auto the_dist = role_pos.Distance3d(dst_pos);
		//obj_room_num大于等于0表示在打怪
		if ( obj_room_num >= 0 )
		{
			is_shortest_path = the_dist >= kFindPathMinDistLimitOnHitting;
			if ( !is_shortest_path )
				//目标点是否出地图了
			{
				is_shortest_path = GSP_StagePosPath::GetMe().CalcNearestVertexDist(cur_template_map_id_, GetOwnerRoomIdx(), dst_pos) >= kFindPathMinDistLimitOnHitting;
			}
		}
		else
			is_shortest_path = the_dist > kFindPathMinDistLimitNormal;
		if (is_shortest_path)
		{
			pos_steps = GSP_StagePosPath::GetMe().DijkstraShortestSteps(cur_template_map_id_, GetOwnerRoomIdx(), role_pos, dst_pos);
			//assert(pos_steps.size() > 1);
			is_shortest_path = !pos_steps.empty();
		}
	}
	if ( !Dist2Steps(role_pos, dst_pos, pos_steps) )
	{
		assert(false);
		return kOptionalBoolNil;
	}

	auto& dir_info = TwoPos2Dir(role_pos, dst_pos);
	if ( !SetDirEnsure(dir_info.first, dir_info.second) )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	if (is_shortest_path)
	{
		DxKeyUpDown(DIK_W, true);
		//LOG_O(Log_trace) << "is_shortest_path:" << is_shortest_path;
	}
	else
	{
		/*if ( target_room_num >= 0 && target_room_num != role_room_num )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "SetRoleToPosIndirect,按下W键， is_shortest_path：" << is_shortest_path;
			//按W是为了同步坐标的吗？但打怪时设置了坐标，再同步方向容易被拉回的
			PressKeyW();
		}
		else
		{
			//LOG_O(Log_trace) << "is_shortest_path:" << is_shortest_path;
		}*/
		if (is_test_dst_pos_z)
		{
			if ( !TestTheMonsterPosZ(dst_pos) )
				return false;
		}
	}
	BOOST_SCOPE_EXIT_ALL(=){
		if (is_shortest_path)
		{
			DxKeyUpDown(DIK_W, false);
		}
	};

	while ( !pos_steps.empty() )
	{
		/*if ( !SendToSerThePos(pos_steps.front(), false) )
		{
			assert(false);
			return kOptionalBoolNil;
		}*/
		/*auto& dir_info = TwoPos2Dir(GetPos(), pos_steps.front());
		if ( !SetDir(dir_info.first, dir_info.second) )
		{
			assert(false);
			return kOptionalBoolNil;
		}*/
		if (is_shortest_path)
		{
			//过房间的时候不判断距离，只有最后一个坐标才判断距离
			if (!is_pass_room && 1 == pos_steps.size())
			{
				auto& cur_role_pos = GetPos();
				auto cur_role_dist = cur_role_pos.Distance3d(pos_steps.front());
				if (cur_role_dist >= kFindPathMaxDangerDist || 
					GSP_StagePosPath::GetMe().CalcNearestVertexDist(cur_template_map_id_, GetOwnerRoomIdx(), pos_steps.front()) >= kFindPathMaxDangerDist)
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "当前人物坐标与目的坐标，或此次设置的坐标与最近坐标距离过大，所以忽略此次设置，当前角色坐标：" << cur_role_pos << " 目的坐标：" << pos_steps.front() << " 距离是：" << cur_role_dist;
					if (++last_valid_pos_.second > 3)
					{
						//last_valid_pos_.second = 0;
						DxKeyUpDown(DIK_W, false);
						//超出距离几次后就拉回
						//SetPosEnsure(last_valid_pos_.first);
						if (scmd::GetMe().IsInFb() && ambinet.cond_fb_game_over_lose_.CondtionTrue())
							return kOptionalBoolNil;


						//SetDirEnsure(dir_info.first, dir_info.second);
						/*if ( !SetPosEnsureSync(last_valid_pos_.first) )
						return kOptionalBoolNil;*/
						LOG_O(Log_debug) << "失败次数过多，坐标拉回到最新的安全点";
						if (!SetPosEnsureSync(GSP_StagePosPath::GetMe().FindNearestPos(cur_template_map_id_, GetOwnerRoomIdx(), GetPos())))
							return kOptionalBoolNil;
						/*if (is_in_fb)
						SetPosEnsureSync(last_valid_pos_.first);
						else
						SetPosEnsure(last_valid_pos_.first);*/
					}
					DummySleep(1000 * 2);
					//assert(false);
					return false;
				}
			}
			
			if ( is_test_dst_pos_z && dst_pos.FloatAllEqual(pos_steps.front()) )
			{
				if ( !TestTheMonsterPosZ(dst_pos) )
					return false;
			}
		}
		/*if (is_in_fb)
		{
			if ( !SetPosEnsureSync(pos_steps.front()) )
				return false;
		}
		else
			SetPosEnsure(pos_steps.front());*/
		if (scmd::GetMe().IsInFb() && ambinet.cond_fb_game_over_lose_.CondtionTrue())
			return kOptionalBoolNil;
		if ( is_hitting_monster_ && !handle_can_not_move_state_ )
		{
			if (IsNiRenStatus())
			{
				LOG_O(Log_debug) << "寻路时发现自己是泥人状态";
				return false;
			}
			if (IsSnowManStatus())
			{
				LOG_O(Log_debug) << "寻路时发现自己是雪人状态";
				return false;
			}
			if (IsChanRaoStatus())
			{
				LOG_O(Log_debug) << "寻路时发现自己是缠绕状态";
				return false;
			}
		}
		//SetDirEnsure(dir_info.first, dir_info.second);
		if ( !SetPosEnsureSync(pos_steps.front()) )
			return kOptionalBoolNil;
		//last_valid_pos_.first = pos_steps.front();
		last_valid_pos_.second = 0;

		pos_steps.pop_front();

		/*DateTimeLocal time_now;
		ReadLock rl(mutex_msg_on_hook_move_room_);
		auto time_span = time_now - msg_on_hook_move_room_.msg_time_;
		rl.unlock();*/

		if (is_shortest_path)
		{
			//这里的延迟必须得放在这个地方，因为后面需要判断游戏是否已经发过过房间封包了，否则每设置一次坐标就会发送一次过房间封包
			DxKeyUpDown(DIK_W, true);
			//调整这个延迟时间，会影响设置角色坐标的速度
			int delay_time;
			/*if (pos_steps.empty())
				delay_time = 50;
			else
				delay_time = 400;*/
			//目前先把延迟调高些吧，求稳
			if (pos_steps.empty())
				delay_time = 100;
			else
				delay_time = 500;
			DummySleep(delay_time);
		}
		if ( obj_room_num >= 0 && call_back )
		{
			if ( !(*call_back)() )
				return false;
		}

		//auto elapse_time = time_span.totalMilliseconds();
		if ( role_room_num >= 0 || target_room_num >= 0 || is_send_move_room_ )
		{
			if (is_send_move_room_)
			{
				//DummySleep(kSendMoveRoomSpan - elapse_time);
				if (is_shortest_path)
				{
					DxKeyUpDown(DIK_W, false);
				}
				if ( !TimerDoWaitUntil([=](){
					if ( msg_on_send_move_ret_ && GetOwnerRoomIdx() != old_role_room_num )
						return true;
					return false;
				}, [this](){
					UpdateOwnerRoomNum();
				}, 100, 1000 * 15) )
				{
					LOG_O(Log_warning) << "从房间号[" << old_role_room_num << "]移动到未知目标房间失败a";
					return false;
				}
				//必须去掉下面的return true;因为有时会被怪物挤到其他房间去，或会路过门的坐标而进入到其他房间去
				/*LOG_O(Log_debug) << "SetRoleToPosIndirect，通过过房间时间间隔判断出过房间了,old_role_room_num：" << old_role_room_num << " 现在的房间号：" << GetOwnerRoomIdx();
				return true;*/
			}
			UpdateOwnerRoomNum();
			if ( GetOwnerRoomIdx() != role_room_num && GetOwnerRoomIdx() != target_room_num )
			{
				LOG_O(Log_debug) << "SetRoleToPosIndirect,当前所在房间与角色房间和目标房间都不一致了，返回假,old_role_room_num：" << old_role_room_num << " 现在的房间号：" << GetOwnerRoomIdx()
					<< " 参数中的起始角色房间号：" << role_room_num << " 目标房间：" << target_room_num;
				return false;
			}
			if ( role_room_num != target_room_num && target_room_num >= 0 )
			{
				if ( target_room_num == GetOwnerRoomIdx() )
				{
					LOG_O(Log_debug) << "SetRoleToPosIndirect,和目标房间都一致，返回真";
					return true;
				}
			}
		}
	}

	/*if ( !SendToSerThePos(dst_pos, false) )
	{
		assert(false);
		return kOptionalBoolNil;
	}*/
	if ( !is_shortest_path )
	{
		/*if (is_in_fb)
		{
			if ( !SetPosEnsureSync(dst_pos) )
				return false;
		}
		else
			SetPosEnsure(dst_pos);*/
		//SetPosEnsure(dst_pos);
		if (scmd::GetMe().IsInFb() && ambinet.cond_fb_game_over_lose_.CondtionTrue())
			return kOptionalBoolNil;
		//SetDirEnsure(dir_info.first, dir_info.second);
		if ( !SetPosEnsureSync(dst_pos) )
			return kOptionalBoolNil;
		//last_valid_pos_.first = dst_pos;
		last_valid_pos_.second = 0;
	}
	
	return true;
}

int GPlayerMe::StepOverObjs( const GameObjsContT& game_objs )
{
	using namespace GType;
	int cnt = 0;
	if (game_objs.empty())
		return cnt;
	std::queue<stGameVector3> pos_cont;
	for ( auto& v : game_objs )
	{
		if ( !v )
		{
			assert(false);
			continue;
		}
		pos_cont.push(v->GetPos());
	}
	while ( !pos_cont.empty() )
	{
		UpdateOwnerRoomNum();
		if ( !CheckOptionalBoolRes(SetRoleToPosIndirect(pos_cont.front(), -1, GetOwnerRoomIdx(), -1, nullptr, false)) )
		{
			//assert(false);
			return cnt;
		}
		++cnt;
		pos_cont.pop();
		//if ( !pos_cont.empty() )
		DummySleep(2000);
	}
	return cnt;
}

int GPlayerMe::StepOverTheStaticObjs( const std::string& obj_name )
{
	if (obj_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "步过指定名字的静止对象之对象名不能为空";
		return 0;
	}
	UpdateOwnerRoomNum();
	GObjStaticMgr static_mgr(obj_name);
	static_mgr.RebuildAll();
	GameObjsContT game_objs;
	static_mgr.Traverse([this, &game_objs](GameObjBase& v){
		auto the_static = v.CastTo<GObjStatic>();
		if (!the_static)
		{
			assert(false);
			return true;
		}
		if (the_static->GetOwnerRoomIdx() != owner_room_num_)
			return true;
		game_objs.push_back(the_static);
		return true;
	});
	return StepOverObjs(game_objs);
}

GItemMgr_Task& GPlayerMe::GetGridsTask()
{
	return grids_task_;
}

bool GPlayerMe::SendAcceptSociatyTask( GType::enSociatyTaskLvl sociaty_task_lvl, DWORD task_fb_id )
{
	using namespace GClass;
	stCD_Msg_AcceptSociatyTask theTaskAddr;
	theTaskAddr.dwTaskFbId = task_fb_id;
	theTaskAddr.dwTaskFbLevel = sociaty_task_lvl;
	theTaskAddr.dwTaskFlag = 1/*MMOCLIENT_SOCIATY_TASK_FLAG*/;
	theTaskAddr.dwUnKnow = 0x985e/*MMOCLIENT_SOCIATY_TASK_UNKNOWN_DATA*/;
	if (!CD_FuncCall::SendT(theTaskAddr))
	{
		assert(false);
		return false;
	}
	DummySleep(700);
	return true;
}

bool GPlayerMe::AcceptSociatyTask( const std::string& task_lvl, const std::string& task_name )
{
	using namespace GType;
	if (task_lvl.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "任务等级不能为空";
		return false;
	}
	if (task_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "任务任务名不能为空";
		return false;
	}
	enSociatyTaskLvl sociaty_task_lvl = kSTL_Invalid;
	for ( auto& v : g_sociaty_task_name2lvl )
	{
		if ( v.task_lvl_name_ == task_lvl )
		{
			sociaty_task_lvl = v.task_lvl_;
			break;
		}
	}
	if ( kSTL_Invalid == sociaty_task_lvl )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的任务等级：" << task_lvl;
		return false;
	}
	auto& all_fb_info = MapDbMgr::GetMe();
	all_fb_info.Update();
	auto fb_info = all_fb_info.FindByName(task_name);
	if ( !fb_info )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的公会任务：" << task_name;
		return false;
	}
	UpdateRoleLvl_Notify();
	if ( fb_info->GetRequireRoleLvl() > role_lvl_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "等级不够，需要等级：" << fb_info->GetRequireRoleLvl() << "  而角色等级才：" << role_lvl_;
		return false;
	}

	if ( !CheckOptionalBoolRes(MoveToTheVillage(kVillageName_MeiJiePoErTan, 0)) )
	{
		assert(false);
		return false;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	auto npc = obj_mgr.FindByName(kNpcName_Sociaty);
	if ( !npc )
	{
		obj_mgr.RebuildAll();
		npc = obj_mgr.FindByName(kNpcName_Sociaty);
		if ( !npc )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "找不到这样的NPC：" << kNpcName_Sociaty;
			return false;
		}
	}
	if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(*npc, &obj_mgr, -1, -1, false, -1)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法移动到npc身前，NPC：" << kNpcName_Sociaty;
		return false;
	}
	npc = obj_mgr.FindByName(kNpcName_Sociaty);
	if ( !npc )
	{
		assert(false);
		return false;
	}
	if ( !OpenNpc(*npc) )
	{
		assert(false);
		return false;
	}

	if ( !SendAcceptSociatyTask(sociaty_task_lvl, fb_info->map_id_) )
		return false;
	return true;
}

bool GPlayerMe::UpdateRoleLvl()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	role_lvl_ = cur_role_->lvl_;
	if ( role_lvl_ <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "角色等级不正确，游戏数据不正常了，等级是：" << role_lvl_;
		//assert(false);
		if ( !is_throw_data_error_ && LuasScriptEchoC::IsRunning() )
		{
			is_throw_data_error_ = true;
			GetIoService().post([this](){
				throw CmnPauseLogicException(kPauseId_GameDataErr, "游戏数据不正常了");
			});
		}
	}
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::UpdateExp()
{
	//数据没找到
	left_single_exp_ = kDefRetValueAtDataErr;
	left_double_exp_ = kDefRetValueAtDataErr;
	return true;
	DO_EXCEPTION_TRY;
	//满级后，EXP数值就不正常了
	if ( role_lvl_ >= kMaxRoleLvl )
	{
		left_single_exp_ = kDefRetValueAtDataErr;
		left_double_exp_ = kDefRetValueAtDataErr;
		return true;
	}
	//left_single_exp_ = CD_FuncCall::DecryptValue(exp_addr + g_o_single_exp, kDefRetValueAtDataErr);
	//left_double_exp_ = CD_FuncCall::DecryptValue(exp_addr + g_o_double_exp, kDefRetValueAtDataErr);
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

void GPlayerMe::SetOnEnterFb( const MsgOnEnterFbT& msg )
{
	msg_on_enter_fb_ = msg;
}

bool GPlayerMe::AutoToDoTheSociatyTask( const std::string& task_lvl, const std::string& task_name, const luabind::object& do_func )
{
	using namespace GType;
	using namespace luabind;
	if ( LUA_TFUNCTION != type(do_func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "公会任务的处理函数必须是函数类型";
		return false;
	}
	if ( !AcceptSociatyTask(task_lvl, task_name) )
		return false;
	if ( !CheckOptionalBoolRes(MoveToTheFb(task_name)) )
		return false;
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "开始处理公会任务：" << task_name;
	return CallFunction(do_func, true, task_name);
}

bool GPlayerMe::UpdatePacketId()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (IsUnValidPtr(cur_role_))
	{
		assert(false);
		return false;
	}
	packet_id_ = cur_role_->packet_id_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
	ResetAllUpdateData();
	return false;);
}

/*
bool GPlayerMe::IsNormalFbOver() const
{
	if ( left_shou_lie_quan_cnt_ <= 0 )
		return true;
	LONGLONG the_exp = left_single_exp_;
	the_exp += left_double_exp_;
	if ( the_exp <= 0 )
		return true;
	return false;
}*/

bool GPlayerMe::WaitToDoingStatus( int wait_time )
{
	using namespace GType;
	//Update();
	auto obj_status = GetObjStatus();
	if ( !obj_status )
	{
		Update();
		obj_status = GetObjStatus();
		if ( !obj_status )
		{
			assert(false);
			return false;
		}
	}
	if (CD_En_ObjStatus::kDoing == *obj_status)
		return true;
	else
	{
		return TimerDoWaitUntil([this](){
			auto status = *GetObjStatus();
			if (CD_En_ObjStatus::kDoing == status)
				return true;
			return false;
		}, kFunc_DoNothing, 100, wait_time);
	}
}

bool GPlayerMe::SendGainFbInfo(CD_En_FbType fb_type)
{
	using namespace GType;
	using namespace GClass;
	const stFbTypeKeyMap* the_fb_map = nullptr;
	for ( auto& v : g_fb_type_key_map )
	{
		if ( v.fb_type_ == fb_type )
		{
			the_fb_map = &v;
			break;
		}
	}
	if ( !the_fb_map )
	{
		assert(false);
		return false;
	}
	msg_last_fb_info_.reset();
	stCD_Msg_QuestEntrustList msg;
	msg.dwFbKeyFlag = the_fb_map->fb_key_flag_;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_last_fb_info_)
			return true;
		return false;
	}, 1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "GFC_MMO_RequestEntrustList长时间未收到返回封包";
		return false;
	}
	if ( !msg_last_fb_info_ )
	{
		assert(false);
		return false;
	}
	auto& msg_ptr = *msg_last_fb_info_;
	if ( !msg_ptr )
	{
		assert(false);
		return false;
	}
	return true;
}

void GPlayerMe::SetOnFbInfo( const MsgOnFbInfoOptionalT& fb_info )
{
	using namespace GType;
	msg_last_fb_info_ = fb_info;
	if ( !fb_info )
		return;
	auto& fb_info_ptr = *fb_info;
	if ( !fb_info_ptr )
		return;
	if ( 0 != fb_info_ptr->res_ )
		return;
	fb_info_cat_ = fb_info_ptr->GetFbInfo_Cat();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "获取喵喵副本总数：" << fb_info_cat_.size();
	for ( auto& v : fb_info_cat_ )
	{
		LOG_O(Log_trace) << "获取到的喵喵副本ID：" << v;
	}
	/*if ( CD_En_FbType::kFT_Sociaty == fb_info_ptr->fb_type_ )
		msg_fb_info_sociaty_ = fb_info_ptr;
	else if ( CD_En_FbType::kFT_MiaoMiao == fb_info_ptr->fb_type_ )
		msg_fb_info_miao_miao_ = fb_info_ptr;
	else if ( CD_En_FbType::kFT_QianShou == fb_info_ptr->fb_type_ )
		msg_fb_info_qian_shou_ = fb_info_ptr;
	else
	{
		assert(false);
		return;
	}*/
}

bool GPlayerMe::GainTheFbInfo(CD_En_FbType fb_type)
{
	assert(false);
	return false;
	/*using namespace GType;
	if ( CD_En_FbType::kFT_Sociaty == fb_type )
	{
		if (msg_fb_info_sociaty_)
			return true;
	}
	else if ( kFT_MiaoMiao == fb_type )
	{
		if (msg_fb_info_miao_miao_)
			return true;
	}
	else if ( CD_En_FbType::kFT_QianShou == fb_type )
	{
		if (msg_fb_info_qian_shou_)
			return true;
	}
	else
	{
		assert(false);
		return false;
	}
	if ( !SendGainFbInfo(fb_type) )
	{
		assert(false);
		return false;
	}
	return true;*/
}

bool GPlayerMe::SendSellToAuction( const GItem& item, int begin_price, int smart_price )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	if ( !item.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	stCD_Msg_ShangJiaItem msg;
	msg.bTabIndex = (byte)item.item_mgr_ptr_->GetTabCategory();
	msg.wSlotIndex = (WORD)item.item_idx_;
	msg.dwItemId = item.item_id_;
	msg.wItemCount = item.item_cnt_;
	msg.nPrice1 = begin_price;
	msg.nPrice2 = smart_price;
	//拍卖时间 2小时=0 8小时=1 24小时=2
	msg.nTimeFalg = 2;

	msg_on_sold_auction_.reset();
	msg_left_time_.reset();
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_sold_auction_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 40) )
	{
		assert(false);
		return false;
	}
	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	if ( !msg_on_sold_auction_ )
	{
		assert(false);
		return false;
	}
	if ( 0 == msg_on_sold_auction_->res_ )
	{
		RefreshMySellList(true);
		return true;
	}

	LOG_O(Log_debug) << "挂拍卖失败，错误码：" << msg_on_sold_auction_->res_;
	return false;
}

bool GPlayerMe::SendRefreshMySellList( bool force_fresh )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	if ( !force_fresh )
	{
		if ( !time_to_refresh_my_sell_list_.DoTimeout() )
			return false;
	}

	msg_my_sell_list_old_ = msg_on_my_sell_list_;
	msg_on_my_sell_list_.reset();
	msg_left_time_.reset();
	stCD_Msg_RefreshMySellList msg;
	msg.ukn_ = 8;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	
	if ( !WaitUntil([this](){
		if (msg_on_my_sell_list_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 20) )
	{
		LOG_O(Log_debug) << "刷新我的拍卖列表超时失败";
		return false;
	}
	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}

	NotifyOtherSideByMySellListChanged();

	return true;
}

bool GPlayerMe::SendSmartBuy( const stAuctionItemInfo& item )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	stCD_Msg_ReqBuyItem msg;
	msg.dwReqItemId = item.req_item_id_;
	msg.dwReqItemKey = item.req_item_key_;
	msg.nPrice = item.smart_price_;
	msg.dwItemId = item.item_id_;
	msg.nItemCount = item.item_cnt_;

	msg_left_time_.reset();
	msg_on_buy_auction_.reset();
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_on_buy_auction_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 20) )
	{
		assert(false);
		return false;
	}
	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	if ( msg_on_buy_auction_->res_ != 0 )
	{
		LOG_O(Log_debug) << "购买物品返回错误，错误码：" << msg_on_buy_auction_->res_;
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::SendGainAuctionInfo( const ItemDb& item_template )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	assert(IsInVillage());

	msg_last_page_info_.reset();
	msg_auction_pages_info_.clear();
	msg_left_time_.reset();
	stCD_Msg_ReqItem msg;

	static int nReqCount = 0;
	nReqCount++;

	msg.nReqCount = nReqCount;
	msg.bTabIndex = 0;
	msg.bTypeIndex = 0;
	msg.bSlotIndex = 0;
	msg.wUnKnow1 = 0;
	msg.wGuDingValue1 = 0x03E7;
	msg.wGuDingValue2 = 0x0009;
	memcpy_s(msg.szSearchName, sizeof(msg.szSearchName), item_template.item_name_.c_str(), item_template.item_name_.size());		// 这个关键字的作用并不大
	msg.bItemIdCount = 1;		// 搜索的物品id数量
	msg.dwItemId[0] = item_template.item_id_;	// 万能虫网

	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_last_page_info_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 40) )
	{
		assert(false);
		return false;
	}

	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	return true;
}

bool GPlayerMe::SendGainAuctionThePageInfo( int page_idx )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	assert(IsInVillage());

	if ( page_idx < 1 )
	{
		assert(false);
		return false;
	}
	msg_last_page_info_.reset();
	msg_left_time_.reset();
	stCD_Msg_SearchItemList theSearchItemPage;
	theSearchItemPage.nTabIndex = 4;
	theSearchItemPage.nPageIndex = page_idx * kAuctionPageCnt;
	theSearchItemPage.nPageCount = 8;
	if (!CD_FuncCall::SendT(theSearchItemPage))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_last_page_info_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 40) )
	{
		assert(false);
		return false;
	}
	
	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	return true;
}

void GPlayerMe::SetOnRefreshMySellList( const MsgMySellListT& msg )
{
	msg_on_my_sell_list_ = msg;
	NotifyOtherSideByMySellListChanged();
}

void GPlayerMe::SetOnGainAuctionPageInfo( const MsgAuctionPageInfoT& msg )
{
	msg_last_page_info_ = msg;
	if (msg)
		msg_auction_pages_info_.push_back(msg);
}

void GPlayerMe::ResetNofityDatas()
{
	role_lvl_ = kDefRetValueAtDataErr;
	left_shou_lie_quan_cnt_ = kDefRetValueAtDataErr;
	left_single_exp_ = kDefRetValueAtDataErr;
	left_double_exp_ = kDefRetValueAtDataErr;
	cur_map_name_.clear();
	name_.clear();
	gold_ = -1;
	silver_ = -1;
	wait_safe_time_cnt_ = 0;
	msg_on_enter_fb_.reset();
	packet_id_ = std::numeric_limits<DWORD>::max();
}

bool GPlayerMe::CutSkinById( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int wait_time )
{
	if ( !WaitRoleAlive() )
	{
		assert(false);
	}
	scmd::GetMe().CloseAllUi();
	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		assert(false);
		return false;
	}
	obj_mgr->DoRebuildAll();
	auto collect_item = obj_mgr->FindByPacketId(packet_id);
	if ( !collect_item )
		return false;
	if ( !collect_item->UpdateOwnerRoomNum() )
		return false;
	auto obj_room_idx = collect_item->GetOwnerRoomIdx();
	if ( obj_room_idx < 0 )
	{
		assert(false);
		return false;
	}
	UpdateOwnerRoomNum();

	if ( obj_room_idx != owner_room_num_ )
	{
		if ( !MoveToTheRoom(obj_room_idx) )
		{
			//assert(false);
			return false;
		}
		UpdateOwnerRoomNum();
		if ( obj_room_idx != owner_room_num_ )
		{
			assert(false);
			return false;
		}
		obj_mgr->DoRebuildAll();
		collect_item = obj_mgr->FindByPacketId(packet_id);
		if ( !collect_item )
			return false;
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto dead_monster = collect_mgr.FindDeadMonsterByPacketId(packet_id);
	if ( !dead_monster )
		return false;
	auto the_obj = dead_monster->shared_from_this();
	if ( !the_obj )
	{
		assert(false);
		return false;
	}
	return CutSkinImpl(*the_obj, &collect_mgr, wait_time);
}

bool GPlayerMe::CutSkinImpl( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int wait_time )
{
	if ( kGOT_Collect != obj.GetGameObjType() )
	{
		assert(false);
		return false;
	}
	UpdateOwnerRoomNum();
	if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
	{
		//assert(false);
		return false;
	}
	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		//assert(false);
		return false;
	}
	msg_on_collect_ret_.reset();
	if ( !PressKeyZ() )
	{
		assert(false);
		return false;
	}
	if ( !WaitToDoingStatus(1000) )
	{
		LOG_O(Log_debug) << "没有剥皮到，再一次剥皮1";
		if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
		{
			//assert(false);
			return false;
		}
		scmd::GetMe().PressKeyW();
		if ( !PressKeyZ() )
		{
			assert(false);
			return false;
		}
		if ( !WaitToDoingStatus(1000) )
		{
			LOG_O(Log_debug) << "没有剥皮到，再一次剥皮2";
			if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(obj, obj_mgr, -1, GetOwnerRoomIdx(), true, 0)) )
			{
				//assert(false);
				return false;
			}
			scmd::GetMe().PressKeyW();
			if ( !PressKeyZ() )
			{
				assert(false);
				return false;
			}
			if ( !WaitToDoingStatus(1000) )
				return false;
		}
	}

	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
	{
		if ( !PressKeyZ() )
		{
			CMN_ASSERT(false);
			return false;
		}
		if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		{
			CMN_ASSERT(false);
			return false;
		}
	}
	if (msg_on_collect_ret_)
	{
		if ( !msg_on_collect_ret_->IsSucceed() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "剥皮失败，错误码：" << msg_on_collect_ret_->res_ << " 获取的物品ID：" << msg_on_collect_ret_->gain_item_id_;
			return false;
		}
	}
	if ( wait_time > 0 )
		DummySleep(wait_time);

	if ( !msg_on_collect_ret_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "剥皮失败，不知为何没收到剥皮结果封包";
		return false;
	}

	//LOG_O(Log_trace) << "剥皮了一次,返回真";
	return true;
}

void GPlayerMe::DebugMySellList() const
{
	if (msg_on_my_sell_list_)
		msg_on_my_sell_list_->DebugInfo();
}

void GPlayerMe::DebugGainAuctionPagesInfo() const
{
	if (msg_last_page_info_)
		msg_last_page_info_->DebugInfo();
}

void GPlayerMe::SetOnLeftTime( const MsgOnLeftTimeT& msg )
{
	msg_left_time_ = msg;
}

void GPlayerMe::WaitSafeTime()
{
	if ( !msg_left_time_ )
		return;
	if ( msg_left_time_->left_time_ <= 0 )
		return;
	if ( !LuasScriptEchoC::IsRunning() )
		return;
	if ( 0 == wait_safe_time_cnt_++ )
		return;
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "帐号处理安全时间，等待过安全时间，等待的剩余时间：" << msg_left_time_->left_time_ << "秒";
	DummySleep(1000 * msg_left_time_->left_time_);
}

bool GPlayerMe::SellToAuction( const std::string& item_name, int item_cnt, int begin_price, int smart_price, bool private_sell, 
							  const boost::optional<int>& relative_smart_price, const boost::optional<int>& min_smart_price, const boost::optional<int>& average_cnt, bool begin_price_direct )
{
	using namespace GType;
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	if ( !IsInVillage() )
		return false;
	if ( item_cnt < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "挂拍卖失败，物品数量不能少于1";
		return false;
	}
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的物品信息：" << item_name;
		return false;
	}
	auto& the_item = FindItemFromPacketAndStore(item_name, false, item_cnt);
	if ( !the_item )
		return false;
	if (!the_item->IsFree())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "挂拍卖失败，该物品已绑定，不能交易：" << item_name;
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	if ( !private_sell && relative_smart_price && min_smart_price && average_cnt )
	{
		if ( !SendGainAuctionInfo(*item_template) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "挂拍卖时，未找到这样的已拍卖信息，物品名：" << item_template->item_name_;
		}
		if ( !msg_auction_pages_info_.empty() )
		{
			const auto& the_first_info = msg_auction_pages_info_[0];
			if (the_first_info)
			{
				auto& the_items_info = the_first_info->items_info_;
				if ( !the_items_info.empty() )
				{
					__int64 average_smart_price = 0;
					auto cnt = 0;
					const auto the_max_cnt = *average_cnt;
					for ( ; cnt < (int)the_items_info.size() && cnt < the_max_cnt; ++cnt )
					{
						average_smart_price += the_items_info[cnt].smart_price_;
					}
					assert(cnt > 0);
					cnt = std::max(cnt, 1);
					average_smart_price /= cnt;
					average_smart_price += *relative_smart_price;
					if ( average_smart_price >= *min_smart_price )
					{
						assert(average_smart_price <= std::numeric_limits<int>::max());
						smart_price = (int)average_smart_price;
					}
				}
			}
			else
				assert(false);
		}		
	}
	//////////////////////////////////////////////////////////////////////////
	UpdateGold_Notify();
	smart_price = std::max(1, smart_price);
	begin_price = std::max(1, begin_price);
	if ( !begin_price_direct )
	{
		begin_price = std::min(100, begin_price);
		begin_price = (int)(smart_price * (begin_price / double(100.0)));
		begin_price = std::max(1, begin_price);
	}
	
	if (private_sell)
	{
		begin_price = MakePrivateSellPrice(begin_price);
		if ( smart_price < begin_price )
			smart_price = begin_price;
	}
	else
	{
		begin_price = MakePublicSellPrice(begin_price);
		if ( smart_price > 0 && smart_price < begin_price )
			smart_price = begin_price;
	}
	//保管24小时，收取10分之1的保管费
	auto keep_price = (int)(begin_price / 10.0);
	if ( keep_price > (LONGLONG)silver_ + gold_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "挂拍卖失败，身上银币无法支付保管费，身上银币为：" << silver_ << "金币为：" << gold_ << "  所需保管费为：" << keep_price;
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	if ( smart_price < begin_price )
		smart_price = begin_price;
	the_item->item_cnt_ = std::min(item_cnt, the_item->item_cnt_);
	return SendSellToAuction(*the_item, begin_price, smart_price);
}

boost::optional<GItem> GPlayerMe::FindItemFromPacketAndStore( const std::string& item_name, bool is_bind, int item_cnt )
{
	auto& packet_item = GPlayerMe::GetMe().GetGridPackage();
	grids_package_.RebuildAll();
	auto item_ptr = grids_package_.FindByName_Cnt(item_name, is_bind, item_cnt);
	if (item_ptr)
	{
		return *item_ptr;
	}
	else
	{
		grids_store_.RebuildAll();
		item_ptr = grids_store_.FindByName_Cnt(item_name, is_bind, item_cnt);
		if (item_ptr)
			return *item_ptr;

		grids_su_cai_.RebuildAll();
		item_ptr = grids_su_cai_.FindByName_Cnt(item_name, is_bind, item_cnt);
		if (item_ptr)
			return *item_ptr;
	}
	return boost::optional<GItem>();
}

bool GPlayerMe::UpdateGold()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}

	gold_ = cur_role_->gold_;
	silver_ = cur_role_->silver_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::RefreshMySellList( bool force_fresh )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	if ( !IsInVillage() )
		return false;
	return SendRefreshMySellList(force_fresh);
}

int GPlayerMe::SmartBuy( const std::string& item_name, int seach_page_cnt, const std::string& seller, int price_limit, int buy_cnt )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return 0;
	if ( !IsInVillage() )
		return 0;
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "一口价购买之没有这样的物品信息：" << item_name;
		return 0;
	}
	if ( 0 == seach_page_cnt)
		seach_page_cnt = 1;
	buy_cnt = std::max(1, buy_cnt);
	if ( price_limit <= 0 )
		price_limit = std::numeric_limits<int>::max();
	if ( !SendGainAuctionInfo(*item_template) )
		return 0;
	if (msg_auction_pages_info_.empty())
		return 0;
	if ( !msg_last_page_info_ )
	{
		assert(false);
		return 0;
	}
	auto total_page_cnt = msg_last_page_info_->total_item_cnt_ / kAuctionPageCnt + !!(msg_last_page_info_->cur_page_item_cnt_ % kAuctionPageCnt);
	int cnt = 0;
	if ( seach_page_cnt >= 0 )
	{
		seach_page_cnt = std::min(seach_page_cnt, total_page_cnt);
		auto page_item = SmartBuyOneImpl(seller, price_limit);
		if (page_item)
		{
			if (SendSmartBuy(*page_item))
			{
				++cnt;
				--buy_cnt;
				if ( buy_cnt <= 0 )
					return cnt;
			}
		}
		for ( int i = 2; i <= seach_page_cnt; ++i )
		{
			if ( !SendGainAuctionThePageInfo(i) )
			{
				assert(false);
				break;
			}
			page_item = SmartBuyOneImpl(seller, price_limit);
			if ( !page_item )
				continue;
			
			if ( !SendSmartBuy(*page_item) )
				continue;
			++cnt;
			--buy_cnt;
			if ( buy_cnt <= 0 )
				break;
		}
	}
	else
	{
		msg_auction_pages_info_.clear();
		seach_page_cnt = std::min(abs(seach_page_cnt), total_page_cnt);
		seach_page_cnt = total_page_cnt - seach_page_cnt;
		for ( int i = total_page_cnt; i >= seach_page_cnt; ++i )
		{
			if ( !SendGainAuctionThePageInfo(i) )
			{
				assert(false);
				break;
			}
			auto page_item = SmartBuyOneImpl(seller, price_limit);
			if ( !page_item )
				continue;

			if ( !SendSmartBuy(*page_item) )
				continue;
			++cnt;
			--buy_cnt;
			if ( buy_cnt <= 0 )
				break;
		}
	}
	return cnt;
}

void GPlayerMe::SetOnBuyAuction( const MsgOnBuyAuctionT& msg )
{
	msg_on_buy_auction_ = msg;
}

const stAuctionItemInfo* GPlayerMe::SmartBuyOneImpl( const std::string& seller, int price_limit )
{
	if ( !msg_last_page_info_ )
	{
		assert(false);
		return nullptr;
	}
	const stAuctionItemInfo* page_item = nullptr;
	if ( !seller.empty() )
	{
		page_item = msg_last_page_info_->FindByName(seller);
		if ( !page_item )
			return nullptr;
		if ( page_item->smart_price_ > price_limit )
			return nullptr;
	}
	else
	{
		page_item = msg_last_page_info_->FindByLimitPrice(price_limit);
	}
	if ( !page_item )
		return nullptr;
	if ( !CanSmartBuy(*page_item) )
		return nullptr;
	return page_item;
}

int GPlayerMe::QuerySoldCnt( const std::string& item_name )
{
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的物品信息：" << item_name;
		return 0;
	}
	if ( !msg_on_my_sell_list_ )
	{
		RefreshMySellList();
		if ( !msg_on_my_sell_list_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "查询拍卖条目：" << item_name << " 时，刷新我的拍卖列表失败";
			return 0;
		}
	}
	return msg_on_my_sell_list_->QuerySoldCnt(item_template->item_id_);
}

bool GPlayerMe::IsPrivateSell( int price )
{
	return !!(price % 2);
}

int GPlayerMe::MakePrivateSellPrice( int price )
{
	return price + !(price % 2);
}

int GPlayerMe::MakePublicSellPrice( int price )
{
	return price + (price % 2);
}

int GPlayerMe::SmartBuyOtherSide()
{
	int cnt = 0;
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return cnt;
	if ( !IsInVillage() )
		return cnt;

	//刷新的地方暂且写在此处
	RefreshMySellList();

	if (other_side_items_.empty())
		return cnt;
	auto other_side_tmp = other_side_items_;
	AuctionItemsContT erase_tmp;
	for ( auto& v : other_side_tmp )
	{
		auto it = FindOtherSideItem(v);
		if ( !it )
			continue;
		if ( !CanSmartBuy(v) )
			continue;
		EraseOtherSideItem(v);
		if ( !SendSmartBuy(v) )
		{
			//assert(false);
			continue;
		}
		erase_tmp.push_back(v);
		++cnt;
	}
	Json::Value j_value, j_tmp;
	for ( auto& v : erase_tmp )
	{
		if ( !v.ConvertToJson(j_tmp) )
		{
			assert(false);
			continue;
		}
		j_value.append(j_tmp);
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return cnt;
	norc->GetCoorTrans().SendErasePrivateItems_AllSide(j_value);
	return cnt;
}

const stAuctionItemInfo* GPlayerMe::FindOtherSideItem( const stAuctionItemInfo& item_info )
{
	auto it = std::find(other_side_items_.begin(), other_side_items_.end(), item_info);
	if ( it == other_side_items_.end() )
		return nullptr;
	return &*it;
}

void GPlayerMe::AddOtherSideItem( const stAuctionItemInfo& item_info )
{
	auto it = std::find(other_side_items_.begin(), other_side_items_.end(), item_info);
	if ( it != other_side_items_.end() )
	{
		*it = item_info;
	}
	else
	{
		other_side_items_.push_back(item_info);
	}
}

void GPlayerMe::EraseOtherSideItem( const stAuctionItemInfo& item_info )
{
	auto it = std::find(other_side_items_.begin(), other_side_items_.end(), item_info);
	if ( it != other_side_items_.end() )
	{
		other_side_items_.erase(it);
	}
}

bool GPlayerMe::CanSmartBuy( const stAuctionItemInfo& item )
{
	if ( role_lvl_ < kRoleLvlLimit_Auction )
		return false;
	if ( !IsInVillage() )
		return false;
	if ( item.smart_price_ <= 0 )
		return false;
	UpdateGold_Notify();
	if ( gold_ < item.smart_price_ )
	{
		//金币不足
		return false;
	}
	return true;
}

bool GPlayerMe::AddOtherSideItemsFromJson( const Json::Value& j_value )
{
	stAuctionItemInfo item_info_tmp;
	for ( auto& v : j_value )
	{
		if ( !item_info_tmp.ParseFromJsonObj(v) )
		{
			assert(false);
			return false;
		}
		AddOtherSideItem(item_info_tmp);
	}
	return true;
}

void GPlayerMe::EraseOtherSideItemsFromJson( const Json::Value& j_value )
{
	stAuctionItemInfo item_info_tmp;
	AuctionItemsInfoT items;
	for ( auto& v : j_value )
	{
		if ( !item_info_tmp.ParseFromJsonObj(v) )
		{
			assert(false);
			return;
		}
		items.push_back(item_info_tmp);
	}
	for ( auto& v : items )
	{
		EraseOtherSideItem(v);
		EraseMySellItem(v);
	}
}

void GPlayerMe::GenPrivateToJson( Json::Value& j_value )
{
	if ( !msg_on_my_sell_list_ )
	{
		//assert(false);
		return;
	}
	Json::Value j_tmp;
	for ( auto& v : msg_on_my_sell_list_->items_info_ )
	{
		if ( !IsPrivateSell(v.begin_price_) )
			continue;
		if ( !v.ConvertToJson(j_tmp) )
		{
			assert(false);
			return;
		}
		j_value.append(j_tmp);
	}
}

void GPlayerMe::ClrMySellList()
{
	time_to_refresh_my_sell_list_.ResetLastTime();
	msg_on_my_sell_list_.reset();
	msg_my_sell_list_old_.reset();
}

void GPlayerMe::NotifyOtherSideByMySellListChanged()
{
	using namespace Json;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	auto& coor_trans = norc->GetCoorTrans();
	Value j_value;
	if ( !msg_my_sell_list_old_ )
	{
		GenPrivateToJson(j_value);
		coor_trans.SendAddPrivateItems(j_value);
		return;
	}
	if ( !msg_on_my_sell_list_ )
	{
		assert(false);
		return;
	}
	msg_on_my_sell_list_->Sort();
	msg_my_sell_list_old_->Sort();
	AuctionItemsInfoT items_tmp;
	std::set_difference(msg_on_my_sell_list_->items_info_.begin(), msg_on_my_sell_list_->items_info_.end(),
		msg_my_sell_list_old_->items_info_.begin(), msg_my_sell_list_old_->items_info_.end(), std::back_inserter(items_tmp));
	Value j_tmp;
	for ( auto& v : items_tmp )
	{
		if ( !IsPrivateSell(v.begin_price_) )
			continue;
		if ( !v.ConvertToJson(j_tmp) )
		{
			assert(false);
			return;
		}
		j_value.append(j_tmp);
	}
	coor_trans.SendAddPrivateItems(j_value);

	items_tmp.clear();
	j_value.clear();

	std::set_difference(msg_my_sell_list_old_->items_info_.begin(), msg_my_sell_list_old_->items_info_.end(),
		msg_on_my_sell_list_->items_info_.begin(), msg_on_my_sell_list_->items_info_.end(), std::back_inserter(items_tmp));
	for ( auto& v : items_tmp )
	{
		if ( !IsPrivateSell(v.begin_price_) )
			continue;
		if ( !v.ConvertToJson(j_tmp) )
		{
			assert(false);
			return;
		}
		j_value.append(j_tmp);
	}
	coor_trans.SendErasePrivateItems_OtherSide(j_value);
}

bool GPlayerMe::UpdateCurMapInfo_Notify()
{
	auto old_map = cur_map_name_;
	if ( !GWndExecSync([this](){
		return UpdateCurMapInfo();
	}) )
	{
		assert(false);
		return false;
	}
	if ( old_map != cur_map_name_ )
	{
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
		{
			assert(false);
			return false;
		}
		auto& session_oper = norc->GetSessionOper();
		session_oper.ToCall(BC_MakeP("SetCurMapRoomName")<<cur_map_name_);
	}
	return true;
}

bool GPlayerMe::UpdateExp_Notify()
{
	auto old_single_exp = left_single_exp_;
	auto old_double_exp = left_double_exp_;
	if ( !GWndExecSync([this](){
		return UpdateExp();
	}) )
	{
		assert(false);
		return false;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}
	auto& session_oper = norc->GetSessionOper();
	bool is_tell_fatigue = false;
	if ( old_single_exp != left_single_exp_ )
	{
		session_oper.ToCall(BC_MakeP("SetSingleExp") << left_single_exp_);
		is_tell_fatigue = true;
	}
	if ( old_double_exp != left_double_exp_ )
	{
		session_oper.ToCall(BC_MakeP("SetDoubleExp") << left_double_exp_);
		is_tell_fatigue = true;
	}
	/*if (is_tell_fatigue)
		TellFatigueData(CalcFatigueData());*/
	return true;
}

bool GPlayerMe::UpdateGold_Notify()
{
	auto old_gold = gold_;
	if ( !GWndExecSync([this](){
		return UpdateGold();
	}) )
	{
		assert(false);
		return false;
	}
	if ( gold_ != old_gold )
	{
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
		{
			assert(false);
			return false;
		}
		auto& session_oper = norc->GetSessionOper();
		session_oper.ToCall(BC_MakeP("SetCurGold")<<gold_);
	}
	return true;
}

bool GPlayerMe::UpdateRoleLvl_Notify()
{
	auto old_player_lvl = role_lvl_;
	if ( !GWndExecSync([this](){
		return UpdateRoleLvl();
	}) )
	{
		assert(false);
		return false;
	}

	if ( old_player_lvl != role_lvl_ )
	{
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
		{
			assert(false);
			return false;
		}
		auto& session_oper = norc->GetSessionOper();
		session_oper.ToCall(BC_MakeP("SetRoleLvl") << role_lvl_);
	}
	return true;
}

void GPlayerMe::SetOnAcceptMail( const MsgOnAcceptMailT& msg )
{
	msg_on_accept_mail_ = msg;
}

void GPlayerMe::SetOnOpenNpc( const MsgOnOpenNpcT& msg )
{
	msg_on_open_npc_ = msg;
}

void GPlayerMe::SetOnSoldAuction( const MsgOnSoldAuctionT& msg )
{
	msg_on_sold_auction_ = msg;
}

void GPlayerMe::SetHitBodyPart( int body_part_idx, int hit_cnt, DWORD cur_packet_id )
{
	GMonster_HitPart::GetMonsterHitPart().SetHitInfo(body_part_idx, hit_cnt, cur_packet_id);
}

bool GPlayerMe::PosInfo2Dir2d(const stCD_PosDirInfo& pos_info_src, const stCD_BodyPartInfo& body_part_info, GType::stDirection2d& out_dir)
{
	using namespace GType;
	if (IsUnValidPtr(dir_pos_info_))
	{
		UpdateImpl();
		if (IsUnValidPtr(dir_pos_info_))
		{
			assert(false);
			return false;
		}
	}
	auto role_pos_info = *dir_pos_info_;
	stGameVector3 pos_src, pos_center;
	pos_src.ConvertToPos(pos_info_src);
	pos_center.fx = (body_part_info.theRangeLeft.fx + body_part_info.theRangeRight.fx) / 2;
	pos_center.fy = (body_part_info.theRangeLeft.fy + body_part_info.theRangeRight.fy) / 2;
	//auto vec_body = pos_center - pos_src;
	auto vec_body = Dir2Vec(pos_info_src.dir_1, pos_info_src.dir_2);
	auto vec_left = body_part_info.theRangeLeft - pos_src;
	//auto vec_left = pos_center - pos_src;
	auto included_angle = vec_body.IncludedAngleXY(vec_left);
	vec_left.RotateXY_Origin(float(kPi / 2 - included_angle));

	const auto& dir_left_move = Vec2Dir(vec_left);
	auto unit_left_dir = Dir2Vec(dir_left_move.first, dir_left_move.second);
	vec_left = body_part_info.theRangeLeft;
	vec_left.PlusXY(unit_left_dir);

	/*role_pos_info.fx = vec_left.fx;
	role_pos_info.fy = vec_left.fy;
	role_pos_info.fz = body_part_info.theRangeLeft.fz;*/
	role_pos_info.fx = pos_center.fx;
	role_pos_info.fy = pos_center.fy;
	role_pos_info.fz = dir_pos_info_->fz;

	unit_left_dir.RotateXY_Origin((float)kPi);
	const auto& the_dir = Vec2Dir(unit_left_dir);

	role_pos_info.dir_1.fx = the_dir.first.fx;
	role_pos_info.dir_1.fy = the_dir.first.fy;
	role_pos_info.dir_2.fx = the_dir.second.fx;
	role_pos_info.dir_2.fy = the_dir.second.fy;
	role_pos_info.dir_3 = pos_info_src.dir_3;

	if (!GWndExecSyncT(out_dir, [&role_pos_info](){
		return CD_FuncCall::CalcHitSendDir(role_pos_info);
	}))
		return false;
	return true;
}

GameObjDirT GPlayerMe::Vec2Dir( const GType::stGameVector3& vec_dir ) const
{
	/*
	此函数用的是角度、正弦、余弦的方式去求的，
	而Dir2Vec用的是向量旋转的方式去求的
	*/
	using namespace GType;
	auto angle = atan2(vec_dir.fy, vec_dir.fx);
	
	angle -= delta_angle_;
	//关于正北方向对称后
	angle = (float)(kPi / 2 - angle);

	stGameVector3 dir_lhs, dir_rhs;
	const auto angle_45 = kPi / 4;
	auto angle_lhs = angle_45 - angle;
	angle_lhs = -angle_lhs;
	dir_lhs.fx = (float)cos(angle_lhs);
	dir_lhs.fy = (float)sin(angle_lhs);

	auto angle_rhs = angle + angle_45;
	dir_rhs.fx = (float)cos(angle_rhs);
	dir_rhs.fy = (float)sin(angle_rhs);
	return GameObjDirT(dir_lhs, dir_rhs);
}

void GPlayerMe::InitItemBlackWhiteList()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return;
	}
	
	item_black_white_list_.reset(new CmnPriLuaData(norc->GetGLua().lua(), kItemBlackWhiteList_PriTblName));
}

void GPlayerMe::AfterRunScript()
{
	using namespace GType;
	if ( !item_black_white_list_ )
	{
		assert(false);
		return;
	}
	item_black_white_list_->SetLuaObj(kItemBlackListName, luabind::object());
	item_black_white_list_->SetLuaObj(kItemWhiteListName, luabind::object());
	item_name_black_list_.Clear();
	item_name_white_list_.Clear();
	white_list_item_lvl_ = 0;
	white_list_item_quality_ = CD_En_ItemQuality::kIQ_Red;
	white_list_craft_item_lvl_ = -1;
	white_list_craft_item_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	white_list_lvl_up_equip_lvl_ = -1;
	white_list_lvl_up_equip_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	white_list_facture_lvl_ = -1;
	white_list_facture_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	auto_lvl_up_equip_black_list_.Clear();
	store_white_name_list_.Clear();

	//ZeroMemory(&msg_on_hook_move_room_.msg_, sizeof(msg_on_hook_move_room_.msg_));
	last_valid_pos_.second = 0;
	is_send_move_room_ = false;
	last_room_id_for_send_room_ = -1;
}

int GPlayerMe::SellStored_PacketRubbishItems( int packet_reserve_cnt, int stored_reserve_cnt, const std::string& village_name, const std::string& npc_name )
{
	using namespace GType;
	using namespace luabind;
	using namespace GClass;
	packet_reserve_cnt = std::max(0, packet_reserve_cnt);
	stored_reserve_cnt = std::max(0, stored_reserve_cnt);
	grids_package_.RebuildAll();
	grids_store_.RebuildAll();
	grids_su_cai_.RebuildAll();
	packet_reserve_cnt = std::min(packet_reserve_cnt, grids_package_.GetGridMaxCnt());
	stored_reserve_cnt = std::min(stored_reserve_cnt, grids_store_.GetGridMaxCnt());
	stored_reserve_cnt = std::min(stored_reserve_cnt, grids_su_cai_.GetGridMaxCnt());
	packet_reserve_cnt -= grids_package_.GetFreeCnt();
	stored_reserve_cnt -= std::min(grids_store_.GetFreeCnt(), grids_su_cai_.GetFreeCnt());
	if ( packet_reserve_cnt <= 0 && stored_reserve_cnt <= 0 )
		return 0;
	/*if (village_name.empty())
	{
		assert(false);
		return 0;
	}*/
	if (npc_name.empty())
	{
		assert(false);
		return 0;
	}
	object* filter_white_list_ptr = nullptr, *filter_black_list_ptr = nullptr;;
	object filter_white_list, filter_black_list;
	if (item_black_white_list_)
	{
		auto& black_obj = item_black_white_list_->GetLuaObj(kItemBlackListName);
		if ( black_obj.is_valid() && LUA_TFUNCTION == type(black_obj) )
		{
			filter_black_list.swap(black_obj);
			filter_black_list_ptr = &filter_black_list;
		}
		auto& white_obj = item_black_white_list_->GetLuaObj(kItemWhiteListName);
		if ( white_obj.is_valid() && LUA_TFUNCTION == type(white_obj) )
		{
			filter_white_list.swap(white_obj);
			filter_white_list_ptr = &filter_white_list;
		}
	}

	GItemMgrBase::ContT black_list_cont;
	grids_equip_.RebuildAll();
	if ( packet_reserve_cnt > 0 )
		SellStored_PacketRubbishItemsImpl(packet_reserve_cnt, grids_package_.GetItems(), black_list_cont, filter_white_list_ptr, filter_black_list_ptr);
	if ( stored_reserve_cnt > 0 )
	{
		SellStored_PacketRubbishItemsImpl(stored_reserve_cnt, grids_store_.GetItems(), black_list_cont, filter_white_list_ptr, filter_black_list_ptr);
		SellStored_PacketRubbishItemsImpl(stored_reserve_cnt, grids_su_cai_.GetItems(), black_list_cont, filter_white_list_ptr, filter_black_list_ptr);
	}
	//尚不知是否所有物品都可以卖给商店
	return SellItemsToShop(black_list_cont, village_name, npc_name);
}

bool GPlayerMe::SendSellToShop( const GItem& item )
{
	if (IsInFb())
	{
		assert(false);
		return false;
	}
	if ( !item.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	msg_on_sold_to_shop_.reset();
	msg_left_time_.reset();
	stCD_Msg_SellItem msg;
	msg.nItemTabIndex = (int)item.item_mgr_ptr_->GetTabCategory();
	msg.nItemSlotIndex = item.item_idx_;
	msg.nItemCount = item.item_cnt_;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_on_sold_to_shop_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 40) )
	{
		assert(false);
		return false;
	}

	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}
	if ( !msg_on_sold_to_shop_ )
	{
		assert(false);
		return false;
	}
	if ( 0 == msg_on_sold_to_shop_->res_ )
		return true;
	if ( 8 == msg_on_sold_to_shop_->res_ )
	{
		LOG_O(Log_warning) << "该物品不可售卖：" << item.item_name_;
	}
	return false;
}

int GPlayerMe::SellItemsToShop( const GItemMgrBase::ContT& items, const std::string& village_name, const std::string& npc_name )
{
	if (items.empty())
		return 0;
	LOG_ENTER_FUNC;
	for ( auto& v : items )
	{
		if (v.item_mgr_ptr_)
		{
			if (CD_En_TabCategory::kTC_Package == v.item_mgr_ptr_->GetTabCategory())
			{
				LOG_O(Log_info) << "要被卖掉的背包物品名：" << v.item_name_ << " 格子索引：" << v.item_idx_;
				continue;
			}
			else if (CD_En_TabCategory::kTC_Store == v.item_mgr_ptr_->GetTabCategory() || CD_En_TabCategory::kTC_SuCai == v.item_mgr_ptr_->GetTabCategory())
			{
				LOG_O(Log_info) << "要被卖掉的仓库物品名：" << v.item_name_ << " 格子索引：" << v.item_idx_;
				continue;
			}
		}
		LOG_O(Log_info) << "要被卖掉的物品名：" << v.item_name_ << " 格子索引：" << v.item_idx_;
	}
	if ( !village_name.empty() && !CheckOptionalBoolRes(MoveToTheVillage(village_name, 0)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法移动到指定的城镇：" << village_name;
		return 0;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	auto npc = obj_mgr.FindByName(npc_name);
	if ( !npc )
	{
		obj_mgr.RebuildAll();
		npc = obj_mgr.FindByName(npc_name);
		if ( !npc )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "找不到这样的NPC：" << npc_name;
			return false;
		}
	}
	if ( !CheckOptionalBoolRes(SetRoleToBackIndirect(*npc, &obj_mgr, -1, -1, false, -1)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法移动到npc身前，NPC：" << npc_name;
		return 0;
	}
	npc = obj_mgr.FindByName(npc_name);
	if ( !npc )
	{
		assert(false);
		return 0;
	}
	if ( !OpenNpc(*npc) )
	{
		assert(false);
		return 0;
	}

	int cnt = 0, failed_cnt = 0;
	for ( auto& v : items )
	{
		if (SendSellToShop(v))
		{
			++cnt;
		}
		else
		{
			++failed_cnt;
			if ( failed_cnt >= 6 )
			{
				LOG_O(Log_warning) << "卖物品失败次数过多，物品名：" << v.item_name_;
				break;
			}
		}
	}
	return cnt;
}

void GPlayerMe::SetItemFilterBlackList( const luabind::object& item_func )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(item_func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "设置物品过滤黑名单之参数必须是函数类型";
		return;
	}
	if ( !item_black_white_list_ )
	{
		assert(false);
		return;
	}
	item_black_white_list_->SetLuaObj(kItemBlackListName, item_func);
}

void GPlayerMe::SetItemFilterWhiteList( const luabind::object& item_func )
{
	using namespace luabind;
	if ( LUA_TFUNCTION != type(item_func) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "设置物品过滤白名单之参数必须是函数类型";
		return;
	}
	if ( !item_black_white_list_ )
	{
		assert(false);
		return;
	}
	item_black_white_list_->SetLuaObj(kItemWhiteListName, item_func);
}

void GPlayerMe::AddItemNameBlackList( const std::string& item_name )
{
	item_name_black_list_.Add(item_name);
}

void GPlayerMe::AddItemNameWhiteList( const std::string& item_name )
{
	item_name_white_list_.Add(item_name);
}

bool SetItemQualityImpl(int item_quality, CD_En_ItemQuality& out_item_quality)
{
	using namespace GType;
	auto quality = (CD_En_ItemQuality)item_quality;
	if (quality < CD_En_ItemQuality::kIQ_White || quality > CD_En_ItemQuality::kIQ_JinYellow)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品品质的数值，从1到8之间，依次表示：白色、绿色、淡玫红、蓝色、深玫红、紫色、橘黄色、金黄色";
		return false;
	}
	out_item_quality = quality;
	return true;
}

void GPlayerMe::SetItemInfoWhiteList( int item_lvl, int item_quality )
{
	white_list_item_lvl_ = item_lvl;
	SetItemQualityImpl(item_quality, white_list_item_quality_);
}

void GPlayerMe::SetCraftItemWhiteList( int item_lvl, int item_quality )
{
	white_list_craft_item_lvl_ = item_lvl;
	SetItemQualityImpl(item_quality, white_list_craft_item_quality_);
}

void GPlayerMe::SellStored_PacketRubbishItemsImpl( int reserve_cnt, const GItemMgrBase::ContT& items, GItemMgrBase::ContT& black_list, luabind::object* filter_white_ptr, luabind::object* filter_black_ptr )
{
	using namespace GType;
	using namespace GClass;
	auto& craft_mgr = CraftDbMgr::GetMe();
	craft_mgr.Update();
	auto& item_template_mgr = ItemDbMgr::GetMe();
	item_template_mgr.RebuildAll();
	auto& facture_info = GManufactureDbMgr::GetMe();
	facture_info.Update();
	auto& seed_info = FarmSeedDbMgr::GetMe();
	seed_info.Update();
	int cnt = 0;
	for ( auto it = items.begin(); cnt < reserve_cnt && it != items.end(); ++it )
	{
		auto& item = *it;
		if ( item.sell_price_ <= 0 )
			continue;
		//////////////////////////////////////////////////////////////////////////
		//*******************************处理黑名单*******************************
		if (filter_black_ptr)
		{
			if (CallFunction(*filter_black_ptr, false, item))
			{
				++cnt;
				black_list.push_back(item);
				continue;
			}
		}
		if (item_name_black_list_.IsIn(item.item_name_))
		{
			++cnt;
			black_list.push_back(item);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//*******************************处理白名单*******************************
		if (filter_white_ptr)
		{
			if (CallFunction(*filter_white_ptr, true, item))
				continue;
		}
		if (item_name_white_list_.IsInByMatch(item.item_name_))
			continue;
			
		if (InStoreWhiteList(item))
			continue;
		if (FindAutoWearWhiteListItem(item.item_name_))
			continue;

		if (white_list_item_lvl_ >= 0 && white_list_item_quality_ >= CD_En_ItemQuality::kIQ_Red && white_list_item_quality_ != CD_En_ItemQuality::kIQ_Invalid)
		{
			if ( item.item_lvl_ >= white_list_item_lvl_ && item.item_quality_ >= white_list_item_quality_ )
				continue;
		}

		if (CD_En_ItemCategory::kIT_Equip != item.item_type_)
		{
			//种子不能卖
			if (seed_info.FindByItemId(item.item_id_))
				continue;

			//可以用来升级自己身上装备的材料也得过滤
			auto the_weapon = grids_equip_.FindBySlotIndex(GType::kEPI_Weapon);
			if (the_weapon)
			{
				auto item_template = the_weapon->GetItemTemplate();
				if (item_template)
				{
					if (item_template->FindLvlUpInfoByMaterialId(item.item_id_))
						continue;
				}
			}

			//在材料获取映射中的材料也不能卖
			if (scmd::GetMe().MM_ExistTheItem(item.item_name_))
				continue;

			if (white_list_craft_item_lvl_ >= 0 && white_list_craft_item_quality_ >= CD_En_ItemQuality::kIQ_Red && white_list_craft_item_quality_ != CD_En_ItemQuality::kIQ_Invalid)
			{
				if (craft_mgr.FindByMaterial(item.item_id_, white_list_craft_item_lvl_, white_list_craft_item_quality_))
					continue;
			}
			if (white_list_lvl_up_equip_lvl_ >= 0 && white_list_lvl_up_equip_quality_ >= CD_En_ItemQuality::kIQ_Red && white_list_lvl_up_equip_quality_ != CD_En_ItemQuality::kIQ_Invalid)
			{
				if (item_template_mgr.FindByMaterial(item.item_id_, white_list_lvl_up_equip_lvl_, white_list_lvl_up_equip_quality_))
					continue;
			}
			if (white_list_facture_lvl_ >= 0 && white_list_facture_quality_ >= CD_En_ItemQuality::kIQ_Red && white_list_facture_quality_ != CD_En_ItemQuality::kIQ_Invalid)
			{
				if (facture_info.FindByMaterial(item.item_id_, white_list_facture_lvl_, white_list_facture_quality_))
					continue;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//*******************************剩下的统一加到黑名单中********************
		++cnt;
		black_list.push_back(item);
		//////////////////////////////////////////////////////////////////////////
	}
}

void GPlayerMe::SetOnSoldToShop( const MsgOnSoldToShopT& msg )
{
	msg_on_sold_to_shop_ = msg;
}

void GPlayerMe::SetCommitTaskFailed( const MsgCommitTaskFailedT& msg )
{
	msg_commit_task_failed_ = msg;
}

void GPlayerMe::AutoStoreItemsToStored()
{
	//StoreSomePackageItemsToStore(6);
}

void GPlayerMe::TellFatigueData( pt_qword fatigue_data )
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return;
	}
	auto& session_oper = norc->GetSessionOper();
	const auto& the_data = SplitInt64(fatigue_data);
	session_oper.ToCall(BC_MakeP("SetFatigueData") << fatigue_data);
}

unsigned __int64 GPlayerMe::CalcFatigueData()
{
	assert(false);
	return 1;
	/*if (msg_on_enter_fb_)
	{
		/ *if ( kMsgRetError_XingDongLiBuZu == msg_on_enter_fb_->error_res_ )
			return 0;* /
		if ( 0 != msg_on_enter_fb_->error_res_ )
		{
			if ( GetLeftShouLieQuanCnt() <= 0 )
				return 0;
		}
	}
	/ *if ( double_exp_ <= 0 || single_exp_ <= 0 )
		return 0;* /
	unsigned __int64 fatigue_data = left_double_exp_;
	fatigue_data *= 2;
	fatigue_data += left_single_exp_;
	if ( role_lvl_ >= kMaxRoleLvl || fatigue_data > 0 )
		fatigue_data += left_shou_lie_quan_cnt_ * 10000;
	return fatigue_data;*/
}

bool GPlayerMe::SendLvlUpEquip( const GItem& item, const ItemDb::stEquipLvlUpInfo& lvl_up_info )
{
	if ( !item.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	stCD_Msg_LvlUpEquip theUpEquip;
	theUpEquip.bSoltTableIndex = (byte)item.item_mgr_ptr_->GetTabCategory();
	theUpEquip.wSoltIndex = item.item_idx_;
	theUpEquip.dwEquipItemId = lvl_up_info.dst_item_id_;
	theUpEquip.llUpEquipSeiral = item.item_serial_;
	if (!CD_FuncCall::SendT(theUpEquip))
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	return true;
}

bool GPlayerMe::ZuoReQiQiu()
{
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};

	stCD_Msg_ZuoQiQiu msg;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "做热气球之过门失败，等待2分钟未能成功移动";
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "做热气球等待可移动时间：" << kWaitTime_RoleCanMove;
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "做热气球等待超时";
		//assert(false);
		return false;
	}

	return true;
}

void GPlayerMe::SetOnDoNpcChat( const MsgOnDoNpcChatT& msg )
{
	msg_on_do_npc_chat_ = msg;
}

const GPlayerMe::MsgOnEnterFbT& GPlayerMe::GetMsg_OnEnterFb() const
{
	return msg_on_enter_fb_;
}

bool GPlayerMe::SendGiveUpFb()
{
	stCD_Msg_GiveUpFb msg;
	if (!CD_FuncCall::SendT(msg))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "放弃副本失败";
		//assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::GiveUpFb()
{
	if ( !IsInFb() )
		return true;
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};
	if ( !SendGiveUpFb() )
		return false;
	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "放弃关卡失败，等待2分钟未能成功，等待时间是2分钟";
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "放弃关卡等待可移动时间：" << kWaitTime_RoleCanMove;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "放弃关卡等待超时";
		//assert(false);
		return false;
	}
	UpdateCurMapInfo_Notify();
	if (IsInFb())
		return false;
	return true;
}

void GPlayerMe::SetLvlUpEquipWhiteList( int item_lvl, int item_quality )
{
	white_list_lvl_up_equip_lvl_ = item_lvl;
	SetItemQualityImpl(item_quality, white_list_lvl_up_equip_quality_);
}

void GPlayerMe::SetFactureWhiteList( int item_lvl, int item_quality )
{
	white_list_facture_lvl_ = item_lvl;
	SetItemQualityImpl(item_quality, white_list_facture_quality_);
}

bool GPlayerMe::IsSafeTime()
{
	if ( !msg_left_time_ )
		return false;
	return msg_left_time_->left_time_ > 0;
}

bool GPlayerMe::UpdateLianQiCao()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	lian_qi_cao_ = cur_role_->lian_qi_cao_;
	if ( lian_qi_cao_ < 0 || lian_qi_cao_ > kLianQiCaoFullValue )
	{
		LOG_O(Log_debug) << "剑气槽的值不在有效范围之内：" << lian_qi_cao_;
	}
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::UpdateZhanWeiZhi()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	zhan_wei_zhi_ = cur_role_->zhan_wei_zhi_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::SetLianQiCao()
{
	
	
	if ( !GWndExecSync([this](){
		auto cur_role = CD_FuncCall::GetCurRole();
		if (!cur_role)
			return false;
		CD_FuncCall::EncryptInt(&cur_role->lian_qi_cao_, kLianQiCaoFullValue);
		//assert((lian_qi_cao_ = ::GetDecryValue(the_addr), lian_qi_cao_ == kFullValue));
		lian_qi_cao_ = kLianQiCaoFullValue;
		return true;
	}) )
		return false;
	return true;
}

bool GPlayerMe::UpdateLianQiCaoSync()
{
	return GWndExecSync([this](){
		return UpdateLianQiCao();
	});
}

const GMapDb* GPlayerMe::FindSuitableFbInfo()
{
	UpdateRoleLvl_Notify();
	enterable_fb_.RebuildAll();
	struct stHelp{
		int CalcValue() const{
			return fb_info_->GetFbRoleLvlLimit() * 10 + weight_;
		}

		const GMapDb* fb_info_;
		int				weight_;
	};
	std::vector<stHelp> fbs;
	for ( auto v : enterable_fb_.enterable_fb_ )
	{
		stHelp tmp;
		/*if ( v.fb_name_.find(kFbSubName_JiXian) != std::string::npos )
		{
			tmp.weight_ = 3;
		}
		else */if ( v->map_name_.find(kFbSubName_TiaoZhan) != std::string::npos )
		{
			tmp.weight_ = 2;
		}
		else if ( v->map_name_.find(kFbSubName_ShiLian) != std::string::npos )
		{
			tmp.weight_ = 1;
		}
		else
			continue;
		if ( v->GetRequireRoleLvl() > role_lvl_ )
		{
			LOG_O(Log_debug) << "可进副本等级竟然大于角色等级，可进等级：" << v->GetRequireRoleLvl() << " 角色等级：" << role_lvl_;
			continue;
		}
		tmp.fb_info_ = v;
		fbs.push_back(tmp);
	}
	auto it = std::max_element(fbs.begin(), fbs.end(), []( const stHelp& lhs, const stHelp& rhs ){
		return lhs.CalcValue() < rhs.CalcValue();
	});
	if ( it != fbs.end() )
		return it->fb_info_;
	return nullptr;
}

bool GPlayerMe::LvlUpTheEquip( GItem& the_equip, int lvl_up_equip_idx, bool is_log )
{
	auto the_item_template = the_equip.GetItemTemplate();
	if ( !the_item_template )
	{
		if (is_log)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "没有这样的装备信息，无法升级：" << the_equip.item_name_;
		}		
		return false;
	}
	if (the_item_template->equip_lvl_up_.empty())
	{
		if (is_log)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "这个装备没有升级信息，无法升级：" << the_equip.item_name_;
		}
		return false;
	}
	const ItemDb::stEquipLvlUpInfo* lvl_up_info = GetTheEquipLvlUpInfo(*the_item_template, lvl_up_equip_idx);
	if ( !lvl_up_info )
	{
		if (is_log)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "这个装备材料不足，无法升级：" << the_equip.item_name_;
		}
		return false;
	}
	auto& item_template_mgr = ItemDbMgr::GetMe();
	auto dst_item = item_template_mgr.FindById(lvl_up_info->dst_item_id_);
	if ( !dst_item )
	{
		assert(false);
		return false;
	}
	UpdateRoleLvl_Notify();
	if ( dst_item->player_lvl_ > role_lvl_ )
	{
		if (is_log)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "角色等级不够穿升级后的装备，无法升级，装备名：" << the_equip.item_name_ << " 角色等级：" << role_lvl_ << " 目标装备的角色等级：" << the_equip.player_lvl_;
		}
		return false;
	}
	return SendLvlUpEquip(the_equip, *lvl_up_info);
}

const ItemDb::stEquipLvlUpInfo* GPlayerMe::GetTheEquipLvlUpInfo( const ItemDb& the_equip, int lvl_up_equip_idx )
{
	std::vector<ItemDb::stEquipLvlUpInfo> lvl_up_info_cont;
	if ( lvl_up_equip_idx < 0 )
	{
		lvl_up_info_cont = the_equip.equip_lvl_up_;
	}
	else
	{
		if (the_equip.equip_lvl_up_.empty())
			return nullptr;
		lvl_up_equip_idx = std::min(lvl_up_equip_idx, (int)the_equip.equip_lvl_up_.size() - 1);
		lvl_up_info_cont.push_back(the_equip.equip_lvl_up_[lvl_up_equip_idx]);
	}

	for ( auto& v : lvl_up_info_cont )
	{
		bool has_all = true;
		for ( auto& material : v.materials_ )
		{
			const auto& item = FindTotalItem(material.material_item_id_);
			if ( item.item_cnt_ >= material.item_cnt_ )
				continue;
			has_all = false;
			break;
		}
		if (has_all)
		{
			//重新定位并返回
			for ( auto& the_v : the_equip.equip_lvl_up_ )
			{
				if ( the_v.dst_item_id_ == v.dst_item_id_ )
					return &the_v;
			}
			assert(false);
			return nullptr;
		}
	}
	return nullptr;
}

int GPlayerMe::GetRoleLvl() const
{
	return role_lvl_;
}

void GPlayerMe::Test1( const luabind::object& func )
{
	auto lua_st = func.interpreter();
	auto top_index_old = lua_gettop(lua_st);
	try{
		CallFunction(func, false);
	}
	catch ( ... )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "fdjajfdaj";
		try{
			CmnExceptionBase::HandleDelayException();
		}
		catch ( CmnGeneralRunException& )
		{
			LOG_O(Log_error) << "12355";
		}
	}
	lua_settop(lua_st, top_index_old);
}

void GPlayerMe::AddAutoLvlUpEquipBlackList( const std::string& item_name )
{
	auto_lvl_up_equip_black_list_.Add(item_name);
}

bool GPlayerMe::IsInAutoLvlUpEquipBlackList( const GItem& the_item ) const
{
	return auto_lvl_up_equip_black_list_.IsInByMatch(the_item.item_name_);
}

void GPlayerMe::OnCrossMap()
{
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	Update();
	//更新身上装备，这个不能轻易去掉。因为攻击时会用到它。
	grids_equip_.RebuildAll();
	GMonster_HitPart::GetMonsterHitPart().ClrAll();
	//////////////////////////////////////////////////////////////////////////
	cur_hp_ = std::numeric_limits<int>::max();
	feeb_hp_ = std::numeric_limits<int>::max();
	poisoning_state_ = CD_En_PoisonStatus::kOk;
	RefreshZhanWeiZhiMax();
	ResetSendRoomTime();
	msg_on_send_move_ret_.reset();
	ZeroMemory(&msg_on_hook_move_room_.msg_, sizeof(msg_on_hook_move_room_.msg_));
	is_send_move_room_ = false;
	last_room_id_for_send_room_ = -1;
	last_valid_pos_.second = 0;

	GSP_StagePosPath::GetMe().TestValidPosToDoorDist(GetCurTemplateMapId());
	//if (IsInFb())
	{
		auto& ambient = GClass::GAmbient::GetMe();
		//ambient.cond_fb_game_over_win_.ResetCondition();
		ambient.cond_fb_game_over_lose_.ResetCondition();
	}

	//最好在这个时候去清空所有死亡对象
	obj_mgr.ClearDeadMonsters();
	if (IsInFb())
	{
		fb_server_full_cnt_ = 0;
	}

	msg_on_deal_status_self_.reset();
	msg_on_deal_status_other_.reset();
}

bool GPlayerMe::IsHotStatus()
{
	using namespace GClass;
	auto& buff_mgr = BuffStateMgr::GetMe();
	buff_mgr.RebuildAll();
	auto the_buff = buff_mgr.FindByName(kBuffName_Hot);
	if ( the_buff && the_buff->MatchDesc(kBuffDesc_Hot) )
	{
		if ( !buff_mgr.FindByName(kBuffName_LengYin) )
			return true;
	}
	return false;
}

bool GPlayerMe::IsPoisoningStatus()
{
	UpdatePoisoningStateSync();
	if (CD_En_PoisonStatus::kOk != poisoning_state_)
	{
		poisoning_state_ = CD_En_PoisonStatus::kOk;
		return true;
	}
	return false;
}

bool GPlayerMe::SetAfterEnteredFb( const luabind::object& func )
{
	using namespace luabind;
	auto the_type = type(func);
	if ( LUA_TNIL != the_type && LUA_TFUNCTION != the_type )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "设置进入副本后的处理之参数必须是函数类型";
		return false;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return false;
	auto& handle_after_entered_fb = norc->GetHandleAfterEnteredFb();
	if ( !handle_after_entered_fb )
	{
		assert(false);
		return false;
	}
	handle_after_entered_fb->SetLuaObj(kHandleAfterEnterFb, func);
	return true;
}

bool GPlayerMe::UpdateZhanWeiZhiSync()
{
	return GWndExecSync([this](){
		return UpdateZhanWeiZhi();
	});
}

bool GPlayerMe::NeedMoDao()
{
	UpdateZhanWeiZhiSync();
#ifdef _DEBUG
	if ( zhan_wei_zhi_ > zhan_wei_zhi_max_ )
	{
		LOG_O(Log_debug) << "斩味值数值异常，最大斩味值：" << zhan_wei_zhi_max_ << "  当前斩味值：" << zhan_wei_zhi_;
	}
#endif

	if ( zhan_wei_zhi_max_ <= 0 )
		zhan_wei_zhi_max_ = -1;
	auto zhan_wei_rate = zhan_wei_zhi_ / (double)zhan_wei_zhi_max_;
	if ( zhan_wei_rate <= 0.0f )
		return zhan_wei_zhi_ < kZhanWeiZhiMin;
	else
		return zhan_wei_rate <= 0.6f;
}

void GPlayerMe::AddStoreWhiteList( const std::string& item_name )
{
	store_white_name_list_.Add(item_name);
}

bool GPlayerMe::InStoreWhiteList( const GItem& item )
{
	return store_white_name_list_.IsInByMatch(item.item_name_);
}

int GPlayerMe::StoreToPackageTheWhileList()
{
	if (IsInFb())
		return 0;
	if (store_white_name_list_.Empty())
		return 0;

	grids_store_.RebuildAll();
	std::queue<GItem> items_tmp;
	for ( auto& item : grids_store_.GetItems() )
	{
		if (InStoreWhiteList(item))
			items_tmp.push(item);
	}

	grids_su_cai_.RebuildAll();
	for ( auto& item : grids_su_cai_.GetItems() )
	{
		if (InStoreWhiteList(item))
			items_tmp.push(item);
	}
	if (items_tmp.empty())
		return 0;
	int cnt = 0;
	while ( !items_tmp.empty() )
	{
		if (!SmartMoveItem(items_tmp.front(), CD_En_TabCategory::kTC_Package, -1))
			break;
		items_tmp.pop();
		++cnt;
	}
	return cnt;
}

GPlayerMe::enSendMail GPlayerMe::SendMail(const SendMailContT* items, int item_reach_cnt,
	int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name)
{
	if (IsInFb())
		return kSM_Error;
	if (!items && item_reach_cnt <= 0)
	{
		LOG_O(Log_error) << "指定邮寄数量参数不能小于等于0";
		return kSM_Error;
	}
	using namespace GType;
	if (target_role_name.empty())
	{
		assert(false);
		return kSM_Error;
	}
	int send_gold = 0;
	if ( gold != 0 )
	{
		UpdateGold_Notify();
		if ( gold < 0 )
		{
			send_gold = gold_ + gold;
		}
		else
		{
			send_gold = std::min(gold, gold_);
		}
		if ( send_gold < 0 )
			send_gold = 0;
	}
	if (items)
	{
		for (auto& v : *items)
		{
			//先存放到仓库中，优先从仓库中邮寄
			PackageSomeItemToStore(v.item_name_, v.item_cnt_, [](const GItem& the_move_item){
				return the_move_item.IsFree();
			});
		}
	}
	else
	{
		grids_package_.RebuildAll();
		std::vector<GClass::stItemInfo> the_items;
		for (auto& v : grids_package_.GetItems())
		{
			if (!v.IsFree())
				continue;
			if (v.item_cnt_ < item_reach_cnt)
				continue;
			the_items.push_back(GClass::stItemInfo());
			the_items.back().item_name_ = v.item_name_;
			the_items.back().item_cnt_ = v.item_cnt_;
		}
		for (auto& v : the_items)
		{
			//先存放到仓库中，优先从仓库中邮寄
			PackageSomeItemToStore(v.item_name_, v.item_cnt_, [](const GItem& the_move_item){
				return true;
			});
		}
	}

	ArrangeItems(grids_su_cai_.GetTabCategory());
	ArrangeItems(grids_store_.GetTabCategory());
	ArrangeItems(grids_package_.GetTabCategory());

	//按数量排序
	grids_su_cai_.GetItems().sort([]( const GItem& lhs, const GItem& rhs ){
		return lhs.item_cnt_ > rhs.item_cnt_;
	});
	grids_store_.GetItems().sort([]( const GItem& lhs, const GItem& rhs ){
		return lhs.item_cnt_ > rhs.item_cnt_;
	});
	grids_package_.GetItems().sort([]( const GItem& lhs, const GItem& rhs ){
		return lhs.item_cnt_ > rhs.item_cnt_;
	});

	std::deque<GItem> send_items;
	if (items)
	{
		for (auto& v : *items)
		{
			auto send_item_cnt = v.item_cnt_;
			if (v.item_cnt_ > 0 && v.reach_cnt_)
			{
				send_item_cnt = FindTotalItemByMatchName(v.item_name_).item_cnt_;
				if (send_item_cnt < v.item_cnt_)
					continue;
			}
			else
			{
				if (v.item_cnt_ <= 0)
				{
					send_item_cnt = FindTotalItemByMatchName(v.item_name_).item_cnt_;
					if (send_item_cnt <= 0)
						continue;
				}
			}

			if (!SendMailHelper(send_item_cnt, send_items, grids_su_cai_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
			if (send_item_cnt <= 0)
				continue;
			if (!SendMailHelper(send_item_cnt, send_items, grids_store_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
			if (send_item_cnt <= 0)
				continue;
			if (!SendMailHelper(send_item_cnt, send_items, grids_package_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
		}
	}
	else
	{
		auto the_items = grids_package_.GetItems();
		grids_store_.AppendItems(the_items);
		grids_su_cai_.AppendItems(the_items);
		for (auto& v : the_items)
		{
			auto send_item_cnt = v.item_cnt_;
			if (send_item_cnt < item_reach_cnt)
				continue;

			if (!SendMailHelper(send_item_cnt, send_items, grids_su_cai_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
			if (send_item_cnt <= 0)
				continue;
			if (!SendMailHelper(send_item_cnt, send_items, grids_store_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
			if (send_item_cnt <= 0)
				continue;
			if (!SendMailHelper(send_item_cnt, send_items, grids_package_.TraverseByMatchName_Cnt(v.item_name_, send_item_cnt)))
				break;
		}
	}
	
	if ( send_items.empty() && send_gold <= 0 )
		return kSM_Empty;
	LOG_O(Log_info) << "邮寄的金币是：" << send_gold;
	stCD_Msg_SendMail theSendMail;
	theSendMail.nGold = send_gold;
	if ( target_role_name.size() >= sizeof(theSendMail.szMailActorName) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "角色名长度太大";
		return kSM_Error;
	}
	if ( target_role_name.size() >= sizeof(theSendMail.szMailTitle) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "邮件标题长度太大";
		return kSM_Error;
	}
	if ( target_role_name.size() >= sizeof(theSendMail.szMailData) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "邮件内容长度太大";
		return kSM_Error;
	}

	if (!scmd::GetMe().OpenTheNpcOfVillage(village_name, 0, npc_name, kEmptyStr, true))
	{
		return kSM_Error;
	}

	memcpy_s(theSendMail.szMailActorName, sizeof(theSendMail.szMailActorName), target_role_name.c_str(), target_role_name.size());
	memcpy_s(theSendMail.szMailTitle, sizeof(theSendMail.szMailTitle), target_role_name.c_str(), target_role_name.size());
	memcpy_s(theSendMail.szMailData, sizeof(theSendMail.szMailData), target_role_name.c_str(), target_role_name.size());
	do
	{
		if ( !BuildSendMailMsg(send_items, theSendMail) )
		{
			assert(false);
			return kSM_Error;
		}
		msg_on_send_mail_.reset();
		msg_on_send_mail2_.reset();
		//msg_left_time_.reset();
		if (!CD_FuncCall::SendT(theSendMail))
		{
			assert(false);
			return kSM_Error;
		}

		if ( !WaitUntil([this](){
			if (msg_on_send_mail_)
				return true;
			if (msg_on_send_mail2_)
				return true;
			/*if (IsSafeTime())
				return true;*/
			return false;
		}, 1000 * 40) )
		{
			LOG_O(Log_debug) << "发送邮件超时失败";
			return kSM_Error;
		}
		/*if (IsSafeTime())
		{
			WaitSafeTime();
			return false;
		}*/
		if ( msg_on_send_mail2_ )
		{
			if (msg_on_send_mail2_->MailIsQuick())
			{
				LOG_O(Log_warning) << "邮件发送太频繁，等待30秒后，尝试再一次发送";
				DummySleep(1000 * 30);
				return kSM_Again;
			}
			else if (msg_on_send_mail2_->NoExistRole())
			{
				LOG_O(Log_error) << "角色色名不存在，角色名：" << target_role_name;
				return kSM_Error;
			}
			else if (msg_on_send_mail2_->IsInSafeTime())
			{
				LOG_O(Log_warning) << "在安全时间，无法邮寄，等待30秒";
				DummySleep(1000 * 30);
				return kSM_Again;
			}
			else if (msg_on_send_mail2_->MailItemWrong())
			{
				LOG_O(Log_warning) << "附件不合法";
				return kSM_Error;
			}
			else if (msg_on_send_mail2_->MailLimit())
			{
				LOG_O(Log_warning) << "已达每日邮寄上限";
				return kSM_Error;
			}
			else
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "未知不能邮寄原因，错误码：" << msg_on_send_mail2_->res_.dwErrorValue;
				return kSM_Error;
			}
		}
		if ( !msg_on_send_mail_ )
		{
			assert(false);
			return kSM_Error;
		}
		if ( 0 != msg_on_send_mail_->res_ )
		{
			LOG_O(Log_debug) << "发送邮件返回失败：" << msg_on_send_mail_->res_;
			return kSM_Error;
		}
		theSendMail.nGold = 0;
	}while ( !send_items.empty() );
	return kSM_Succeed;
}

void GPlayerMe::SetOnSendMail( const MsgOnSendMailT& msg )
{
	msg_on_send_mail_ = msg;
}

bool GPlayerMe::BuildSendMailMsg(std::deque<GItem>& send_items, stCD_Msg_SendMail& msg)
{
	msg.items_.cnt_ = 0;
	//NameList<int, std::vector<int> > build_items;
	for (int i = 0; i != msg.items_.value_.max_size() && !send_items.empty(); ++i, send_items.pop_front())
	{
		auto& item = send_items.front();
		if ( !item.item_mgr_ptr_ )
		{
			assert(false);
			return false;
		}
		/*if (build_items.IsIn(item.item_id_))
			continue;*/
		auto& the_item = msg.items_.value_[msg.items_.cnt_];
		the_item.bTableIndex = (pt_byte)item.item_mgr_ptr_->GetTabCategory();
		the_item.nItemCount = (pt_word)item.item_cnt_;
		the_item.llItemSerial = (pt_dint)item.item_serial_;
		the_item.wSlotIndex = (pt_word)item.item_idx_;
		the_item.wUnKnow = 0;

		//build_items.Add(item.item_id_);

		++msg.items_.cnt_;

		LOG_O(Log_info) << "此次邮寄的物品：" << item.item_name_ << " 数量：" << item.item_cnt_;
	}
	return true;
}

bool GPlayerMe::UpdatePoisoningState()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	poisoning_state_ = cur_role_->poison_status_;
	if (CD_En_PoisonStatus::kOk != poisoning_state_ && CD_En_PoisonStatus::kPoisoning != poisoning_state_)
	{
		LOG_O(Log_debug) << "中毒状态数值异常：" << poisoning_state_;
	}
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

void GPlayerMe::SetOnUseItemRet1( const MsgOnUseItemRet1T& msg )
{
	msg_on_use_item_ret1_ = msg;
}

void GPlayerMe::SetOnUseItemRet2( const MsgOnUseItemRet2T& msg )
{
	msg_on_use_item_ret2_ = msg;
}

void GPlayerMe::OnBuyItem( const MsgOnBuyItemT& msg )
{
	msg_on_buy_item_ = msg;
}

bool GPlayerMe::ManuFactureItem( const std::string& item_name )
{
	if ( !TakeInKnife() )
		return false;
	auto& facture_info = GManufactureDbMgr::GetMe();
	facture_info.Update();
	auto the_facture = facture_info.FindByDstItemName(item_name);
	if ( !the_facture )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的调和物品信息，制作调和物品失败，物品名：" << item_name;
		return false;
	}
	auto& item_mgr = ItemDbMgr::GetMe();
	auto item_template = item_mgr.FindByName(item_name);
	if ( !item_template )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的物品信息，无法调和，物品名：" << item_name;
		return false;
	}
	grids_package_.RebuildAll();
	if ( grids_package_.GetTheItemTotalCnt(item_template->item_id_) >= item_template->carry_limit_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "身上物品已达上限，无法调和该物品：" << item_name;
		return false;
	}
	for ( auto& v : the_facture->materials_ )
	{
		const auto& the_item = FindTotalItem(v.material_item_id_);
		if ( the_item.item_cnt_ < v.item_cnt_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "缺少材料，无法调和该物品：" << item_name;
			return false;
		}
	}
	stCD_Msg_MakeFactureItem msg;
	msg.facture_id_ = the_facture->facture_id_;
	msg.tab_idx_ = (int)CD_En_TabCategory::kTC_Store;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	return true;
}

bool GPlayerMe::IsColdStatus()
{
	using namespace GClass;
	auto& buff_mgr = BuffStateMgr::GetMe();
	buff_mgr.RebuildAll();
	auto the_buff = buff_mgr.FindByName(kBuffName_Cold);
	if ( the_buff && the_buff->MatchDesc(kBuffDesc_Cold) )
	{
		if ( !buff_mgr.FindByName(kBuffName_ReYin) )
			return true;
	}
	return false;
}

void GPlayerMe::ClrShangJinWhiteList()
{
	shang_jin_white_list_.Clear();
}

void GPlayerMe::AddShangJinWhiteList(const std::string& task_name, CD_En_FbDifficulty fb_diff, bool is_king_fb)
{
	using namespace GType;
	if (task_name.empty())
	{
		assert(false);
		return;
	}
	if (fb_diff <= CD_En_FbDifficulty::kFD_Zero || fb_diff > CD_En_FbDifficulty::kFD_Invalid)
	{
		CMN_ASSERT(false);
		return;
	}
	stShangJinNameListItem tmp;
	tmp.task_sub_name_ = task_name;
	tmp.fb_diff_ = fb_diff;
	tmp.is_king_fb_ = is_king_fb;
	shang_jin_white_list_.Add(tmp);
}

const GTaskDb* GPlayerMe::FilterShangJinWhiteList(const ShanJinTaskInfoT& shang_jin_task_info, int& the_fb_id) const
{
	using namespace GType;
	if (shang_jin_task_info.empty())
	{
		CMN_ASSERT(false);
		return nullptr;
	}
	for ( auto& v : shang_jin_white_list_.GetValues() )
	{
		for ( auto& info : shang_jin_task_info )
		{
			if ( !info.task_template_ )
			{
				CMN_ASSERT(false);
				continue;
			}
			if ( info.task_template_->task_name_.find(v.task_sub_name_) != std::string::npos )
			{
				if (v.is_king_fb_)
				{
					/*if (!info.fb_info_.IsKingFb() && !info.fb_info_.IsKingCreamFb())
						continue;*/
					if (CD_En_FbType::kFT_WangLi != info.fb_info_.fb_type_)
						continue;
				}
				else
				{
					/*if (info.fb_info_.IsKingFb())
						continue;*/
					if (CD_En_FbType::kFT_WangLi == info.fb_info_.fb_type_)
						continue;
				}
				if (CD_En_FbDifficulty::kFD_Invalid == v.fb_diff_ || info.fb_info_.fb_diff_level_ == v.fb_diff_)
				{
					the_fb_id = info.fb_info_.map_id_;
					return info.task_template_;
				}
				else if (!info.test_fb_diff_lvl_)
				{
					the_fb_id = info.fb_info_.map_id_;
					return info.task_template_;
				}
			}
		}
	}
	return nullptr;
}

void GPlayerMe::ClrStoreWhiteList()
{
	store_white_name_list_.Clear();
}

void GPlayerMe::OnCollectRet( const MsgOnCollectRetT& msg )
{
	msg_on_collect_ret_ = msg;
}

bool GPlayerMe::ArrangeItems(CD_En_TabCategory tab_cate)
{
	auto the_packet = GetItemMgr(tab_cate);
	if ( !the_packet )
	{
		assert(false);
		return false;
	}
	msg_on_arrange_items_ret_.reset();
	msg_left_time_.reset();
	GFC_TidyInventoryItem(tab_cate);

	if ( !WaitUntil([this](){
		if (msg_on_arrange_items_ret_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 10) )
	{
		LOG_O(Log_debug) << "整理物品超时失败";
		return false;
	}

	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}

	if ( !msg_on_arrange_items_ret_ )
	{
		assert(false);
		return false;
	}

	the_packet->RebuildAll();
	return true;
	/*auto items = the_packet->GetItems();
	if (items.empty())
		return true;
	
	if (the_packet->AllItemNextTo())
	{
		auto items_old = items;
		items.sort([]( const GItem& lhs, const GItem& rhs ){
			return lhs.item_id_ < rhs.item_id_;
		});
		if ( items == items_old )
			return true;
	}
	else
	{
		items.sort([]( const GItem& lhs, const GItem& rhs ){
			return lhs.item_id_ < rhs.item_id_;
		});
	}
		
	MMO_TIDY_ITEM theTidyItem = { 0 };
	theTidyItem.wTabSoltIndex = (WORD)tab_cate;
	theTidyItem.wSoltCount = the_packet->GetGridMaxCnt() - 1;
	auto it = items.begin(), it_end = items.end();
	for ( ; theTidyItem.wItemCount < _countof(theTidyItem.theTidyItemInfo) && it != it_end; ++it, ++theTidyItem.wItemCount )
	{
		theTidyItem.theTidyItemInfo[theTidyItem.wItemCount].llkey = it->item_serial_;
		theTidyItem.theTidyItemInfo[theTidyItem.wItemCount].nSloltIndex = it->item_idx_;
	}
	
	msg_on_arrange_items_ret_.reset();
	msg_left_time_.reset();
	if ( !GWndExecSync([&theTidyItem](){
		return ::GFC_MMO_TidyInventoryItem(&theTidyItem);
	}) )
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_arrange_items_ret_)
			return true;
		if (IsSafeTime())
			return true;
		return false;
	}, 1000 * 10) )
	{
		LOG_O(Log_debug) << "整理物品超时失败";
		return false;
	}

	if (IsSafeTime())
	{
		WaitSafeTime();
		return false;
	}

	if ( !msg_on_arrange_items_ret_ )
	{
		assert(false);
		return false;
	}

	DummySleep(700);
	the_packet->RebuildAll();
	return true;*/
}

void GPlayerMe::OnArrangeItemsRet( const MsgOnArrangeItemsRetT& msg )
{
	msg_on_arrange_items_ret_ = msg;
}


bool GPlayerMe::UpdateShouLieQuanCnt()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	left_shou_lie_quan_cnt_ = cur_role_->left_fb_cnt_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}


bool GPlayerMe::UpdateLeftShouLieQuanCnt_Notify()
{
	auto old_value = left_shou_lie_quan_cnt_;
	if ( !GWndExecSync([this](){
		return UpdateShouLieQuanCnt();
	}) )
	{
		assert(false);
		return false;
	}
	if ( old_value != left_shou_lie_quan_cnt_ )
	{
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
		{
			assert(false);
			return false;
		}
		auto& session_oper = norc->GetSessionOper();
		session_oper.ToCall(BC_MakeP("SetFreeLieShouCnt") << left_shou_lie_quan_cnt_);
	}
	return true;
}

int GPlayerMe::GetLeftShouLieQuanCnt() const
{
	return left_shou_lie_quan_cnt_;
}

OptinalBool GPlayerMe::MoveToTheFbImpl( const GMapDb& fb_info )
{
	if (IsInFbBlackList(fb_info.map_id_))
		return false;
	auto valid_fb_consume_mode = GetCurFbValidConsumeMode();
	if ((int)valid_fb_consume_mode < 0)
	{
		LOG_O(Log_warning) << "狩猎券或银币已不足，无法进入副本";
		return kOptionalBoolNil;
	}
	//////////////////////////////////////////////////////////////////////////
	auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
	task_fb_info.Update();
	int fb_village_id = -1;
	{
		auto task_fb = task_fb_info.FindByFbId(fb_info.map_id_);
		if (task_fb)
			fb_village_id = task_fb->village_id_;
		else
		{
			auto king_fb = task_fb_info.FindByFbId_King(fb_info.map_id_);
			if (king_fb)
				fb_village_id = king_fb->village_id_;
		}
	}
	if ( -1 != fb_village_id )
	{
		auto& gsp_village = GSP_Village::GetMe();
		if ( !gsp_village.Construct() )
		{
			assert(false);
			return kOptionalBoolNil;
		}
		const auto& the_fb_village = gsp_village.VillageId2Name(fb_village_id);
		if (the_fb_village)
		{
			if ( !CheckOptionalBoolRes(MoveToTheVillage(*the_fb_village, 0)) )
				return kOptionalBoolNil;
			//反正城镇又不是寻路的，这里不需要加
//#ifdef PREP_OPEN_VILLAGE_SHORTEST_PATH
			const auto& the_fb_pos = gsp_village.VillageFbPos(*the_fb_village);
			if (the_fb_pos)
			{
				UpdateOwnerRoomNum();
				if ( !CheckOptionalBoolRes(SetRoleToPosIndirect(*the_fb_pos, -1, GetOwnerRoomIdx(), -1, nullptr, false)) )
				{
					assert(false);
					return kOptionalBoolNil;
				}
			}
			else
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "任务副本信息中没有遍历到副本所在城镇的副本入口坐标信息，可能还不支持吧,副本ID：" << fb_info.map_id_ << " 所在城镇：" << *the_fb_village;
			}
//#endif
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "任务副本信息中没有遍历到副本所在城镇信息，可能还不支持吧,副本ID：" << fb_info.map_id_ << " 所在城镇ID：" << fb_village_id;
		}
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "任务副本信息中找不到该副本信息,副本ID：" << fb_info.map_id_;
	}
	//////////////////////////////////////////////////////////////////////////
	if (is_active_cross_map_)
	{
		assert(false);
		return kOptionalBoolNil;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};

	UpdateRoleLvl_Notify();
	if ( role_lvl_ < fb_info.GetRequireRoleLvl() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "角色等级不够，无法进入该副本：" << fb_info.map_name_ << " 当前角色等级：" << role_lvl_ << " 副本要求等级：" << fb_info.GetRequireRoleLvl();
		return false;
	}

	auto res = SendMoveToTheFb(fb_info, valid_fb_consume_mode);
	if ( !res || !*res )
	{
		//assert(false);
		return res;
	}
	
	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的副本失败，等待2分钟未能成功，等待时间是2分钟";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "移动到指定的副本失败，等待2分钟未能成功b");
		return kOptionalBoolNil;
	}

	LOG_ENTER_FUNC;
	const int kMaxWaitMinutes = 10;
	LOG_O(Log_trace) << "等待进入到副本之角色可移动，最大等待时间是" << kMaxWaitMinutes << " 副本名：" << fb_info.map_name_;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "等待进入副本超时失败";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "移动到指定的副本失败，等待2分钟未能成功c");
		return kOptionalBoolNil;
	}
	//////////////////////////////////////////////////////////////////////////
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return kOptionalBoolNil;
	auto& handle_after_entered_fb = norc->GetHandleAfterEnteredFb();
	if ( !handle_after_entered_fb )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	auto& the_after_handler = handle_after_entered_fb->GetLuaObj(kHandleAfterEnterFb);
	if ( the_after_handler.is_valid() && LUA_TFUNCTION == luabind::type(the_after_handler) )
		CallFunction(the_after_handler, false);
	//////////////////////////////////////////////////////////////////////////
	//奥义觉醒  GFC_MMO_AddBuffer
	//AddRoleTheBuff(MMOCLIENT_REVERSAL_SKILL_BUFF_ID_F);
	return true;
}

OptinalBool GPlayerMe::MoveToTheFbById( int fb_id )
{
	UpdateCurMapInfo_Notify();
	if ( cur_map_id_ == fb_id )
	{
		GSP_StagePosPath::GetMe().TestValidPosToDoorDist(GetCurTemplateMapId());
		return true;
	}
	if ( !GiveUpFb() )
	{
		assert(false);
		return kOptionalBoolNil;
	}
	AutoStoreItemsToStored();

	enterable_fb_.RebuildAll();
	auto fb_info = enterable_fb_.FindById(fb_id);

	/*const GFbInfo* fb_info = enterable_fb_.FindById(fb_id);
	if ( !fb_info )
	{
		enterable_fb_.RebuildAll();
		fb_info = enterable_fb_.FindById(fb_id);
		if ( !fb_info )
		{
			auto& all_fb = AllFbInfo::GetMe();
			all_fb.Update();
			fb_info = all_fb.FindById(fb_id);
		}
	}*/

	if ( !fb_info )
	{
		auto& all_fb = MapDbMgr::GetMe();
		all_fb.Update();
		fb_info = all_fb.FindById(fb_id);
		if ( fb_info && IsTheFbIdJuQingDone(fb_info->map_id_) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "该副本剧情已完成，无需再进";
			return false;
		}
		if ( !fb_info )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "无法进入该副本，副本ID：" << fb_id;
			return false;
		}
		if (move_fb_strict_)
			return false;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "可接任务中没有遍历到该副本，竟然想要进入该副本2，副本名：" << fb_info->map_name_;
	}
	return MoveToTheFbImpl(*fb_info);
}

bool GPlayerMe::IsInSelfFarm() const
{
	return cur_map_id_ == kSelfFarmFbId;
}

bool GPlayerMe::UpdateJingSuanDian()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	jing_suan_dian_ = cur_role_->jing_suan_point_;
	CMN_ASSERT(jing_suan_dian_ >= 0);
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::UpdateCurFarmFacilityInfo()
{
	return cur_farm_facility_info_.Update();
}

void GPlayerMe::DebugCurFarmFacilityInfo() const
{
	cur_farm_facility_info_.DebugInfo();
}

bool GPlayerMe::PlantFarmSeed( const std::string& seed_name, const std::string& land_name )
{
	using namespace GClass;
	if (seed_name.empty())
		return false;
	auto& collect_mgr = GFarmCollectNameMap::GetMe();
	auto collect_info = collect_mgr.FindByCnName(land_name);
	if (!collect_info || collect_info->cao_yao_tian_idx_ >= CD_En_FarmFieldIdx::kInvalid)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "无法种植，因为没有这样的土地名：" << land_name;
		collect_mgr.DebugInfo_CnName();
		return false;
	}	
	assert(collect_info->cao_yao_tian_idx_ >= CD_En_FarmFieldIdx::kField1 && collect_info->cao_yao_tian_idx_ < CD_En_FarmFieldIdx::kInvalid);

	cao_yao_tian_info_.Update();
	auto the_info = cao_yao_tian_info_.FindByIdx(collect_info->cao_yao_tian_idx_);
	if ( !the_info )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的草药田索引：" << (int)collect_info->cao_yao_tian_idx_ + 1;
		assert(false);
		return false;
	}
	if ( !the_info->CanBePlanted() )
		return false;

	auto& item = FindItemFromPacketAndStore(seed_name);
	if ( !item )
		return false;
	stCD_Msg_FarmSeed msg;
	msg.seed_id_ = item->item_id_;
	msg.farm_idx_ = (int)collect_info->cao_yao_tian_idx_;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	
	if ( !TimerDoWaitUntil([=](){
		auto the_info = cao_yao_tian_info_.FindByIdx(collect_info->cao_yao_tian_idx_);
		if ( !the_info )
			return false;
		if (the_info->CanBePlanted())
			return false;
		return true;
	}, [this](){
		cao_yao_tian_info_.Update();
	}, 700, 1000 * 7) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "种植超时失败";
		return false;
	}
	return true;
}

std::string GPlayerMe::GetExistFarmSeedName()
{
	using namespace GClass;
	auto& seed_info = FarmSeedDbMgr::GetMe();
	seed_info.Update();
	grids_package_.RebuildAll();
	for ( auto& v : grids_package_.GetItems() )
	{
		if (seed_info.FindByItemId(v.item_id_))
			return v.item_name_;
	}
	grids_store_.RebuildAll();
	for ( auto& v : grids_store_.GetItems() )
	{
		if (seed_info.FindByItemId(v.item_id_))
			return v.item_name_;
	}
	grids_su_cai_.RebuildAll();
	for ( auto& v : grids_su_cai_.GetItems() )
	{
		if (seed_info.FindByItemId(v.item_id_))
			return v.item_name_;
	}
	return kEmptyStr;
}

GClass::CurFarmFieldInfo& GPlayerMe::GetCaoYaoTianInfo()
{
	return cao_yao_tian_info_;
}

bool GPlayerMe::GiveUpTask( int task_id )
{
	received_task_mgr_.RebuildAll();
	if ( !received_task_mgr_.FindById(task_id) )
		return true;

	msg_on_giveup_task_.reset();
	stCD_Msg_GiveUpTask msg;
	msg.task_id_ = task_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_giveup_task_)
			return true;
		return false;
	}, 1000 * 60) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "放弃任务等待超时失败，任务ID：" << task_id;
		return false;
	}
	if ( !msg_on_giveup_task_ )
	{
		assert(false);
		return false;
	}
	if ( msg_on_giveup_task_->task_id_ != task_id )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "放弃任务返回的任务ID不同了，要放弃的任务ID：" << task_id << " 返回的任务ID：" << msg_on_accept_task_->task_id_;
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::GiveUpXinShouXunLian()
{
	if (IsInVillage())
		return true;
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};
	if ( !SendGiveUpXinShouXunLian() )
		return false;
	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "退出新手训练失败，等待2分钟未能成功，等待时间是2分钟";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "退出新手训练失败a");
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "离开新手训练等待可移动时间：" << kWaitTime_RoleCanMove;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "离开新手训练等待超时";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "退出新手训练失败b");
		return false;
	}
	UpdateCurMapInfo_Notify();
	if ( kXinShouXunLianTemplateMapId == cur_template_map_id_ )
	{
		LOG_O(Log_error) << "退出新手训练失败,未知原因";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "退出新手训练失败c");
		return false;
	}
	return true;
}

bool GPlayerMe::SendGiveUpXinShouXunLian()
{
	stCD_Msg_XinShouXunLian msg;
	msg.ukn_1_ = 2;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::EnterXinShou()
{
	UpdateCurMapInfo_Notify();
	if ( !IsInVillage() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "不在城镇中无法进入新手训练";
		return false;
	}
	if ( cur_template_map_id_ == kXinShouXunLianTemplateMapId )
		return true;
	if (is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_active_cross_map_ = false;
	};

	stCD_Msg_XinShouXunLian msg;
	msg.ukn_1_ = 1;
	msg.ukn_2_ = 0x00022664;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !GClass::GAmbient::GetMe().WaitCrossedGraph(1000 * 60 * 2) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "进入训练场地失败，等待2分钟未能成功移动";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "进入训练场地失败a");
		return false;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入新手训练等待可移动时间：" << kWaitTime_RoleCanMove;
	//WaitRoleCanMove这个不能去掉，因为其底层有OnCrossMap的调用
	if (!scmd::GetMe().WaitRoleCanMove(kWaitTime_RoleCanMove))
	{
		LOG_O(Log_error) << "进入新手训练等待超时";
		throw CmnPauseLogicException(kPauseId_PassMapErr, "进入训练场地失败b");
		return false;
	}

	UpdateCurMapInfo_Notify();
	if ( cur_template_map_id_ == kXinShouXunLianTemplateMapId )
		return true;
	LOG_O(Log_error) << "进入训练场地失败，未知原因";
	throw CmnPauseLogicException(kPauseId_PassMapErr, "进入训练场地失败c");
	return false;
}

GItemMgr_SuCai& GPlayerMe::GetGridSuCai()
{
	return grids_su_cai_;
}

GItem GPlayerMe::FindTotalItem( int item_id )
{
	grids_package_.RebuildAll();
	auto item_it_ptr = grids_package_.TraverseByItemId(item_id);
	if ( !item_it_ptr )
	{
		assert(false);
		return kItemInvalid;
	}
	GItem item_res;
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_equip_.RebuildAll();
	item_it_ptr = grids_equip_.TraverseByItemId(item_id);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_store_.RebuildAll();
	item_it_ptr = grids_store_.TraverseByItemId(item_id);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_su_cai_.RebuildAll();
	item_it_ptr = grids_su_cai_.TraverseByItemId(item_id);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_task_.RebuildAll();
	item_it_ptr = grids_task_.TraverseByItemId(item_id);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}
	return item_res;
}

bool GPlayerMe::GainGiftBag( int gift_bag_id )
{
	auto& gift_mgr = GGiftItemMgr::GetMe();
	auto the_gift = gift_mgr.FindGift(gift_bag_id);
	if ( !the_gift )
	{
		gift_mgr.RebuildAll();
		the_gift = gift_mgr.FindGift(gift_bag_id);
		if ( !the_gift )
		{
			assert(false);
			return false;
		}
	}
	if ( !the_gift->CanGain() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "这个礼包不能被领取：" << the_gift->gifg_name_;
		return false;
	}
	stCD_Msg_GetGiftBag msg;
	msg.gift_bag_id_ = gift_bag_id;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !TimerDoWaitUntil([&gift_mgr, gift_bag_id](){
		auto gift = gift_mgr.FindGift(gift_bag_id);
		if ( !gift )
			return true;
		if ( !gift->CanGain() )
			return true;
		return false;
	}, [&gift_mgr](){
		gift_mgr.RebuildAll();
	}, 900, 1000 * 20) )
	{
		return false;
	}
	return true;
}

int GPlayerMe::AutoGainGiftBag()
{
	if ( !IsInVillage() )
		return 0;
	auto& gift_mgr = GGiftItemMgr::GetMe();
	gift_mgr.RebuildAll();
	auto gift_tmp = gift_mgr.GetAllNodes();
	auto cnt = 0;
	for ( auto& v : gift_tmp )
	{
		if (v.CanGain())
		{
			if (GainGiftBag(v.gift_bag_id_))
				++cnt;
		}
	}
	return cnt;
}

int GPlayerMe::UseSomeItems( const std::string& item_name, int use_cnt )
{
	if ( use_cnt <= 0 )
	{
		assert(false);
		return 0;
	}
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "使用一些物品之物品名不能为空";
		return 0;
	}
	auto& grid_package = GetGridPackage();
	grid_package.RebuildAll();
	auto item_ptr = grid_package.TraverseByMatchName(item_name);
	if ( !item_ptr )
	{
		assert(false);
		return 0;
	}
	std::vector<GItem> items;
	for ( item_ptr->First(); !item_ptr->IsDone(); item_ptr->Next() )
	{
		auto& item = item_ptr->CurItem();
		if ( !item.CanBeUsed() )
			continue;
		items.push_back(item);
	}

	grids_store_.RebuildAll();
	item_ptr = grids_store_.TraverseByMatchName(item_name);
	if ( !item_ptr )
	{
		assert(false);
		return 0;
	}
	for ( item_ptr->First(); !item_ptr->IsDone(); item_ptr->Next() )
	{
		auto& item = item_ptr->CurItem();
		if ( !item.CanBeUsed() )
			continue;
		items.push_back(item);
	}

	grids_su_cai_.RebuildAll();
	item_ptr = grids_su_cai_.TraverseByMatchName(item_name);
	if ( !item_ptr )
	{
		assert(false);
		return 0;
	}
	for ( item_ptr->First(); !item_ptr->IsDone(); item_ptr->Next() )
	{
		auto& item = item_ptr->CurItem();
		if ( !item.CanBeUsed() )
			continue;
		items.push_back(item);
	}

	if (items.empty())
		return 0;

	scmd::GetMe().GainSomeItemsFromStore(item_name, use_cnt);
	items.clear();

	grid_package.RebuildAll();
	item_ptr = grid_package.TraverseByMatchName(item_name);
	if ( !item_ptr )
	{
		assert(false);
		return 0;
	}
	for ( item_ptr->First(); use_cnt > 0 && !item_ptr->IsDone(); item_ptr->Next() )
	{
		auto& item = item_ptr->CurItem();
		if ( !item.CanBeUsed() )
			continue;
		items.push_back(item);
		items.back().item_cnt_ = std::min(use_cnt, item.item_cnt_);
		use_cnt -= items.back().item_cnt_;
	}
	
	if (items.empty())
		return 0;

	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		return 0;
	
	int cnt = 0;
	for ( auto& item : items )
	{
		while ( item.item_cnt_ > 0 )
		{
			if (!UseItem(CD_En_ItemPackageCategory::kIPC_Item, item.item_idx_))
				break;
			--item.item_cnt_;
		}		
		++cnt;
	}
	return cnt;
}

bool GPlayerMe::BodyPartInfo2RolePos(const stCD_PosDirInfo& pos_info_src, const stCD_BodyPartInfo& body_part_info, GType::stGameVector3& out_pos, GameObjDirT& out_dir)
{
	using namespace GType;
	stGameVector3 pos_src;
	pos_src.ConvertToPos(pos_info_src);
	out_pos.fx = (body_part_info.theRangeLeft.fx + body_part_info.theRangeRight.fx) / 2;
	out_pos.fy = (body_part_info.theRangeLeft.fy + body_part_info.theRangeRight.fy) / 2;
	out_pos.fz = (body_part_info.theRangeLeft.fz + body_part_info.theRangeRight.fz) / 2;
	//从部位到身体中心点的方向
	out_dir = TwoPos2Dir(out_pos, pos_src);

	//部位到身体中心相反方向退一步
	auto dir_vec = Dir2Vec(out_dir.first, out_dir.second);
	dir_vec.RotateXY_Origin((float)kPi);
	assert(cur_hit_mode_);
	const int kDist = cur_hit_mode_ ? cur_hit_mode_->GetSteps() : 1;
	dir_vec.fx *= kDist;
	dir_vec.fy *= kDist;
	out_pos.PlusXY(dir_vec);
	return true;
}

void GPlayerMe::GainWeaponNeedLvlUpInfo( std::vector<stNeedMaterialInfo>& out_info )
{
	auto& tab_equip = GetEquipPackage();
	tab_equip.RebuildAll();
	auto the_weapon = tab_equip.FindBySlotIndex(GType::kEPI_Weapon);
	if ( !the_weapon )
		return;
	if (IsInAutoLvlUpEquipBlackList(*the_weapon))
		return;
	auto weapon_tmp = *the_weapon;
	auto item_template = weapon_tmp.GetItemTemplate();
	if ( !item_template )
	{
		assert(false);
		return;
	}
	struct stHelp{
		double	done_cale_;
		const ItemDb::stEquipLvlUpInfo*	the_ptr_;
	};
	std::vector<stHelp> done_cales;
	auto& item_db_mgr = ItemDbMgr::GetMe();
	for ( auto& v : item_template->equip_lvl_up_ )
	{
		//判断是否在升级装备白名单中
		auto dst_item_db = item_db_mgr.FindById(v.dst_item_id_);
		if (!dst_item_db)
		{
			assert(false);
			continue;
		}
		if (!FindAutoWearWhiteListItem(dst_item_db->item_name_))
			continue;

		stHelp tmp;
		tmp.the_ptr_ = &v;
		int owner_cnt, total_cnt;
		owner_cnt = 0;
		total_cnt = 0;
		for ( auto& v1 : v.materials_ )
		{
			const auto& the_item = FindTotalItem(v1.material_item_id_);
			if ( the_item.item_cnt_ > v1.item_cnt_ )
				owner_cnt += v1.item_cnt_;
			else
				owner_cnt += the_item.item_cnt_;
			total_cnt += v1.item_cnt_;
		}
		tmp.done_cale_ = owner_cnt / double(total_cnt);
		done_cales.push_back(tmp);
	}
	auto it = std::max_element(done_cales.begin(), done_cales.end(), []( const stHelp& lhs, const stHelp& rhs ){
		return lhs.done_cale_ < rhs.done_cale_;
	});
	if ( it == done_cales.end() )
		return;
	for ( auto& v : it->the_ptr_->materials_ )
	{
		const auto& the_item = FindTotalItem(v.material_item_id_);
		if ( the_item.item_cnt_ < v.item_cnt_ )
		{
			stNeedMaterialInfo tmp = { 0 };
			tmp.material_item_id_ = v.material_item_id_;
			tmp.total_item_cnt_ = v.item_cnt_;
			tmp.need_cnt_ = v.item_cnt_ - the_item.item_cnt_;
			out_info.push_back(tmp);
		}
	}
}

bool GPlayerMe::IsFreeTimeToCollectOfFarm(CD_En_FarmFacilityIdx idx)
{
	cur_farm_facility_info_.Update();
	auto the_jsd = cur_farm_facility_info_.FindByIdx(idx);
	if ( !the_jsd )
	{
		assert(false);
		return false;
	}
	return the_jsd->left_seconds_ <= 0;
}

bool GPlayerMe::MoveMergeItem( GItem& item_src, const GItem& item_dst )
{
	using namespace GType;
	if ( !item_src.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	if ( !item_dst.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	if ( item_src.item_cnt_ <= 0 )
	{
		assert(false);
		return false;
	}
	auto item_name = item_src.item_name_;
	auto cnt_src = item_src.item_cnt_, cnt_dst = item_dst.item_cnt_;
	auto total_cnt = cnt_dst + cnt_src;
	auto serial_dst = item_dst.item_serial_;
	auto dst_tbl_category = item_dst.item_mgr_ptr_->GetTabCategory();
	if ( !SendMoveMergeItem(item_src, item_src.item_mgr_ptr_->GetTabCategory(), item_src.item_idx_, dst_tbl_category, item_dst.item_idx_, serial_dst) )
	{
		assert(false);
		return false;
	}
	GItemMgrBase* item_mgr = GetItemMgr(dst_tbl_category);
	if ( !item_mgr )
	{
		assert(false);
		return false;
	}
	if ( !TimerDoWaitUntil([=](){
		auto item = item_mgr->FindBySerial(serial_dst);
		if ( !item )
			return false;
		if ( !item->item_mgr_ptr_ )
			return false;
		if ( item->item_cnt_ != total_cnt )
			return false;
		return true;
	}, [item_mgr](){
		item_mgr->RebuildAll();
	}, 700, 1000 * 8) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "合并移动物品超时失败,物品名：" << item_name;
		return false;
	}
	return true;
}

bool GPlayerMe::SendMoveMergeItem(const GItem& item, CD_En_TabCategory tbl_category_src, int slot_idx_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst, const LONGLONG& item_serial_dst)
{
	stCD_Msg_MoveItemCountTo theItemMoveTo;
	theItemMoveTo.item_serial_ = item.item_serial_;
	theItemMoveTo.bScrItemTabIndex = (BYTE)tbl_category_src;
	theItemMoveTo.wScrItemSlotIndex = (WORD)slot_idx_src;
	theItemMoveTo.wMoveItmeCount = (WORD)item.item_cnt_;

	theItemMoveTo.bDstItemTabIndex = (BYTE)tbl_category_dst;
	theItemMoveTo.wDstItemSlotIndex = (WORD)slot_idx_dst;
	theItemMoveTo.to_item_serial_ = item_serial_dst;

	return CD_FuncCall::SendT(theItemMoveTo);
}

bool GPlayerMe::SendMoveSplitItem(const GItem& item, CD_En_TabCategory tbl_category_src, int slot_idx_src, 
	CD_En_TabCategory tbl_category_dst, int slot_idx_dst)
{
	stCD_Msg_MoveItemCountToEmpty theItemMoveTo;
	theItemMoveTo.item_serial_ = item.item_serial_;
	theItemMoveTo.bScrItemTabIndex = (BYTE)tbl_category_src;
	theItemMoveTo.wScrItemSlotIndex = (WORD)slot_idx_src;
	theItemMoveTo.wMoveItmeCount = (WORD)item.item_cnt_;

	theItemMoveTo.bDstItemTabIndex = (BYTE)tbl_category_dst;
	theItemMoveTo.wDstItemSlotIndex = (WORD)slot_idx_dst;
	
	return CD_FuncCall::SendT(theItemMoveTo);
}

bool GPlayerMe::MoveSplitItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst)
{
	using namespace GType;
	if ( !item_src.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	if ( item_src.item_cnt_ <= 0 )
	{
		assert(false);
		return false;
	}

	auto item_name = item_src.item_name_;
	auto cnt = item_src.item_cnt_;
	if ( !SendMoveSplitItem(item_src, item_src.item_mgr_ptr_->GetTabCategory(), item_src.item_idx_, tbl_category_dst, slot_idx_dst) )
	{
		assert(false);
		return false;
	}
	GItemMgrBase* item_mgr = GetItemMgr(tbl_category_dst);
	if ( !item_mgr )
	{
		assert(false);
		return false;
	}
	if ( !TimerDoWaitUntil([=](){
		auto item = item_mgr->FindBySlotIndex(slot_idx_dst);
		if ( !item )
			return false;
		if ( !item->item_mgr_ptr_ )
			return false;
		if ( item->item_cnt_ != cnt )
			return false;
		return true;
	}, [item_mgr](){
		item_mgr->RebuildAll();
	}, 700, 1000 * 8) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "分割移动物品超时失败,物品名：" << item_name;
		return false;
	}
	return true;
}

bool GPlayerMe::SmartMoveItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst)
{
	GItemMgrBase* item_mgr_dst = GetItemMgr(tbl_category_dst);
	if ( !item_mgr_dst )
	{
		assert(false);
		return false;
	}
	const auto old_move_cnt = item_src.item_cnt_;
	auto has_item_cnt = old_move_cnt;
	BOOST_SCOPE_EXIT_ALL(&item_src, old_move_cnt){
		item_src.item_cnt_ = old_move_cnt;
	};

	auto cnt_limit_per_grid = item_src.die_jia_limit_;
	auto can_move_cnt_limit = item_src.own_limit_ > 0 ? item_src.own_limit_ : std::numeric_limits<int>::max();
	if (CD_En_TabCategory::kTC_Package == tbl_category_dst)
	{
		if ( item_src.carry_limit_ > 0 )
		{
			cnt_limit_per_grid = std::min(item_src.carry_limit_, item_src.die_jia_limit_);
			can_move_cnt_limit = item_src.carry_limit_;
		}
	}
	else if (CD_En_TabCategory::kTC_Equip == tbl_category_dst)
	{
		assert(1 == cnt_limit_per_grid);
		cnt_limit_per_grid = 1;
		can_move_cnt_limit = 1;
	}

	auto total_move_cnt = old_move_cnt;
	total_move_cnt = std::min(total_move_cnt, can_move_cnt_limit);
	if ( total_move_cnt <= 0 )
		return true;
	item_mgr_dst->RebuildAll();
	auto dst_exist_cnt = item_mgr_dst->GetTheItemTotalCnt(item_src.item_id_);
	total_move_cnt = std::min(can_move_cnt_limit - dst_exist_cnt, total_move_cnt);
	if ( total_move_cnt <= 0 )
		return true;

	if ( slot_idx_dst < 0 )
	{
		if ( item_src.die_jia_limit_ > 1 )
		{
			auto tra_ptr = item_mgr_dst->TraverseByItemId(item_src.item_id_);
			if ( !tra_ptr )
			{
				assert(false);
				return false;
			}
			std::vector<GItem> items;
			for ( tra_ptr->First(); !tra_ptr->IsDone(); tra_ptr->Next() )
			{
				auto& item = tra_ptr->CurItem();
				if ( item.item_cnt_ < cnt_limit_per_grid )
				{
					if ( item.item_serial_ != item_src.item_serial_ )
						items.push_back(item);
				}
			}

			for ( auto& item : items )
			{
				if ( item.item_cnt_ < cnt_limit_per_grid )
				{
					auto cur_move_cnt = std::min(cnt_limit_per_grid - item.item_cnt_, total_move_cnt);
					if ( cur_move_cnt <= 0 )
						continue;
					//绑定类型不同的不要合在一起
					if ( item_src.bind_type_ != item.bind_type_ )
						continue;

					item_src.item_cnt_ = cur_move_cnt;
					if ( !MoveMergeItem(item_src, item) )
						return false;
					total_move_cnt -= cur_move_cnt;
					has_item_cnt -= cur_move_cnt;
					if ( total_move_cnt <= 0 )
					{
						assert(0 == total_move_cnt);
						return true;
					}
				}
			}
		}		
	}

	if ( slot_idx_dst >= 0 )
	{
		auto exist_item = item_mgr_dst->FindBySlotIndex(slot_idx_dst);
		if (exist_item)
		{
			if ( exist_item->item_serial_ == item_src.item_serial_ )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "物品没有移动到一个新的位置，移动物品失败，物品名：" << item_src.item_name_;
				return false;
			}
			if ( exist_item->item_id_ != item_src.item_id_ )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "目标位置的物品与要移动的物品不是一类物品，无法叠加，物品名：" << item_src.item_name_;
				return false;
			}
			auto cur_move_cnt = std::min(cnt_limit_per_grid - exist_item->item_cnt_, total_move_cnt);
			if ( cur_move_cnt <= 0 )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "目标位置的物品无法叠加，移动物品失败，物品名：" << item_src.item_name_;
				return false;
			}			

			//绑定类型不同的不要合在一起
			assert(item_src.bind_type_ == exist_item->bind_type_);
			item_src.item_cnt_ = cur_move_cnt;
			if ( !MoveMergeItem(item_src, *exist_item) )
				return false;
			total_move_cnt -= cur_move_cnt;
			has_item_cnt -= cur_move_cnt;
			if ( total_move_cnt <= 0 )
				return true;
			slot_idx_dst = -1;
		}
		else
		{
			dst_exist_cnt = item_mgr_dst->GetTheItemTotalCnt(item_src.item_id_);
			if ( 1 == cnt_limit_per_grid || (has_item_cnt + dst_exist_cnt) <= cnt_limit_per_grid )
			{
				item_src.item_cnt_ = has_item_cnt;
				return MoveTotalItem(item_src, tbl_category_dst, slot_idx_dst);
			}
			auto cur_move_cnt = std::min(total_move_cnt, cnt_limit_per_grid);
			item_src.item_cnt_ = cur_move_cnt;
			if ( !MoveSplitItem(item_src, tbl_category_dst, slot_idx_dst) )
				return false;
			total_move_cnt -= cur_move_cnt;
			has_item_cnt -= cur_move_cnt;
			slot_idx_dst = -1;
		}
	}

	if ( slot_idx_dst < 0 )
		//这里就是随机位置了
	{
		while ( total_move_cnt > 0 )
		{
			item_mgr_dst->RebuildAll();
			slot_idx_dst = item_mgr_dst->GetFreeSlotIndex();
			if ( slot_idx_dst < 0 )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "目标位置已满，无空闲位置，移动物品失败，物品名：" << item_src.item_name_;
				return false;
			}
			dst_exist_cnt = item_mgr_dst->GetTheItemTotalCnt(item_src.item_id_);
			if ( (has_item_cnt + dst_exist_cnt) <= cnt_limit_per_grid )
			{
				item_src.item_cnt_ = has_item_cnt;
				return MoveTotalItem(item_src, tbl_category_dst, slot_idx_dst);
			}
			auto cur_move_cnt = std::min(total_move_cnt, cnt_limit_per_grid);
			item_src.item_cnt_ = cur_move_cnt;
			if ( !MoveSplitItem(item_src, tbl_category_dst, slot_idx_dst) )
				return false;
			total_move_cnt -= cur_move_cnt;
			has_item_cnt -= cur_move_cnt;
		}
		if ( total_move_cnt <= 0 )
			return true;
	}

	assert(false);
	return false;
}

GItem GPlayerMe::FindTotalItemByName( const std::string& item_name )
{
	if (item_name.empty())
	{
		assert(false);
		return kItemInvalid;
	}
	grids_package_.RebuildAll();
	auto item_it_ptr = grids_package_.TraverseByName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return kItemInvalid;
	}
	GItem item_res;
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_equip_.RebuildAll();
	item_it_ptr = grids_equip_.TraverseByName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_store_.RebuildAll();
	item_it_ptr = grids_store_.TraverseByName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_su_cai_.RebuildAll();
	item_it_ptr = grids_su_cai_.TraverseByName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_task_.RebuildAll();
	item_it_ptr = grids_task_.TraverseByName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}
	return item_res;
}

bool GPlayerMe::RefreshZhanWeiZhiMax()
{
	if ( !UpdateZhanWeiZhiSync() )
	{
		assert(false);
		return false;
	}
	zhan_wei_zhi_max_ = zhan_wei_zhi_;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "最大斩味值：" << zhan_wei_zhi_max_;
	return true;
}

bool GPlayerMe::CalcNextCollectFarmTime()
{
	if ( !IsInSelfFarm() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不在庄园内，无法计算下次庄园采集的最短时间";
		return false;
	}
	cur_farm_facility_info_.Update();
	if ( !cur_farm_facility_info_.CalcNextCollectTime() )
		return false;
	cao_yao_tian_info_.Update();
	return cao_yao_tian_info_.CalcNextCollectTime();
}

void GPlayerMe::ExecAtEveryRoleEnterGame()
{
	fb_server_full_cnt_ = 0;
	fb_info_cat_.clear();

	cur_farm_facility_info_.ResetAllCollectTime();
	cao_yao_tian_info_.ResetAllCollectTime();
	ResetNofityDatas();
	Update();
	UpdateGrids();
	UpdateReceivedTasks();
	UpdateAcceptableTasks();
	UpdateEnterableFb();
	ClrMySellList();
	RefreshMySellList();

	msg_on_deal_status_self_.reset();
	msg_on_deal_status_other_.reset();
}

bool GPlayerMe::IsTimeToCollectFarm( const std::string& collect_name )
{
	using namespace GClass;
	if (collect_name.empty())
	{
		auto time_to_collect_tmp = cur_farm_facility_info_.time_to_collect_;
		//把最后的那项草药田去掉
		time_to_collect_tmp.pop_back();
		std::copy(cao_yao_tian_info_.time_to_collect_.begin(), cao_yao_tian_info_.time_to_collect_.end(), std::back_inserter(time_to_collect_tmp));
		std::sort(time_to_collect_tmp.begin(), time_to_collect_tmp.end());
		auto it = std::min_element(time_to_collect_tmp.begin(), time_to_collect_tmp.end());
		if ( it == time_to_collect_tmp.end() )
		{
			assert(false);
			return false;
		}
		return DateTimeLocal() >= *it;
	}
	auto& name_mgr = GFarmCollectNameMap::GetMe();
	auto info = name_mgr.FindByCnName(collect_name);
	if ( !info )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "到了庄园采集的时间，不支持这样的名称：" << collect_name;
		name_mgr.DebugInfo_CnName();
		return false;
	}
	if (info->cao_yao_tian_idx_ < CD_En_FarmFieldIdx::kInvalid)
	{
		if ( (int)info->cao_yao_tian_idx_ >= (int)cao_yao_tian_info_.time_to_collect_.size() )
		{
			assert(false);
			return false;
		}
		return DateTimeLocal() >= cao_yao_tian_info_.time_to_collect_[(int)info->cao_yao_tian_idx_];
	}
	if ( (int)info->collect_idx_ >= (int)cur_farm_facility_info_.time_to_collect_.size() )
	{
		assert(false);
		return false;
	}
	return DateTimeLocal() >= cur_farm_facility_info_.time_to_collect_[(int)info->collect_idx_];
}

bool GPlayerMe::HasAoYiJueXing()
{
	return IsTheCurStatus(kBuffName_AoYiJueXing);
}

bool GPlayerMe::SendMailHelper( int& send_item_cnt, std::deque<GItem>& send_items, GItemMgrBase::GameItemIterPtr& tra_it )
{
	using namespace GType;
	if ( !tra_it )
	{
		assert(false);
		return false;
	}
	for ( tra_it->First(); !tra_it->IsDone(); tra_it->Next() )
	{
		auto& the_item = tra_it->CurItem();
		if (!the_item.IsFree())
			continue;
		for (auto& v : send_items)
		{
			if (v.item_mgr_ptr_ && the_item.item_mgr_ptr_)
			{
				if (v.item_mgr_ptr_->GetTabCategory() == the_item.item_mgr_ptr_->GetTabCategory())
				{
					if (v.item_idx_ == the_item.item_idx_)
						goto LABLE_CONTINUE;
				}
			}
		}

		auto cnt = std::min(the_item.item_cnt_, send_item_cnt);
		send_items.push_back(the_item);
		send_items.back().item_cnt_ = cnt;
		send_item_cnt -= cnt;
		if (the_item.item_mgr_ptr_)
			LOG_O(Log_trace) << "准备邮寄这个物品：物品名->" << the_item.item_name_ << "  物品数量：" << the_item.item_cnt_ << "  物品所在栏:" << the_item.item_mgr_ptr_->GetTabCategory() << "  物品索引：" << the_item.item_idx_;
		if ( send_items.size() >= kSendMailMaxCnt )
			return false;
		if ( send_item_cnt <= 0 )
			return true;
	LABLE_CONTINUE:
		continue;
	}
	return true;	
}

void GPlayerMe::SetOnSendMail2( const MsgOnSendMail2T& msg )
{
	msg_on_send_mail2_ = msg;
}

bool GPlayerMe::GainWeekShouLieQuan()
{
	if ( !IsInVillage() )
		return false;
	UpdateLeftShouLieQuanCnt_Notify();
	if ( left_shou_lie_quan_cnt_ >= kShouLieQuanMax )
		return false;
	auto old_cnt = left_shou_lie_quan_cnt_;
	stCD_Msg_WeekCredit theWeekCredit;
	if (!GWndExecSync([&theWeekCredit](){
		return theWeekCredit.GainCnt(1);
	}) )
	{
		//assert(false);
		LOG_O(Log_debug) << "领取周狩猎券失败";
		return false;
	}
	
	if (!CD_FuncCall::SendT(theWeekCredit))
	{
		assert(false);
		return false;
	}
	return TimerDoWaitUntil([=](){
		if ( old_cnt != left_shou_lie_quan_cnt_ )
			return true;
		return false;
	}, [=](){
		UpdateLeftShouLieQuanCnt_Notify();
	}, 500, 1000 * 10);
}

bool GPlayerMe::UpdatePacketIdSync()
{
	return GWndExecSync([this](){
		return UpdatePacketId();
	});
}

bool GPlayerMe::UpdatePoisoningStateSync()
{
	return GWndExecSync([this](){
		return UpdatePoisoningState();
	});
}

bool GPlayerMe::UpdateJingSuanDianSync()
{
	return GWndExecSync([this](){
		return UpdateJingSuanDian();
	});
}

GPlayerMe::MsgOnCollectRetT& GPlayerMe::GetCollectRet()
{
	return msg_on_collect_ret_;
}

void GPlayerMe::SetRightBtHit( bool is_right_hit )
{
	is_right_bt_hit_ = is_right_hit;
}

void GPlayerMe::SetMonsterMaxHigh( float high )
{
	//monster_most_high_ = high;
}

bool GPlayerMe::IsInMonsterHighBlackList( const std::string& boss_name ) const
{
	/*for ( auto& v : monster_most_high_black_list_ )
	{
		if ( boss_name.find(v) != std::string::npos )
			return true;
	}*/
	assert(false);
	return false;
}

bool GPlayerMe::SendToSerThePos( const GType::stGameVector3& the_pos, bool send_force )
{
	/*if ( !send_force && last_send_role_pos_ )
	{
		if ( last_send_role_pos_->Distance3d(the_pos) < move_step_dist_ / 5 )
			return true;
	}
	last_send_role_pos_ = the_pos;*/

	stCD_Msg_SYN_COOR_DINATES theSynCoorDinates;

	theSynCoorDinates.ptheAcrot = the_pos;
	auto pos_info = GetPosInfo();
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	theSynCoorDinates.theDir1.dir_x = pos_info->dir_1.fx;	// dir   dir和dir1可以取当前角色的
	theSynCoorDinates.theDir1.dir_y = pos_info->dir_1.fy;

	theSynCoorDinates.theDir2.dir_x = pos_info->dir_2.fx;	// dir1
	theSynCoorDinates.theDir2.dir_y = pos_info->dir_2.fy;
	for ( int i = 0; i != 1; ++i, DummySleep(500) )
	{
		// 取同步坐标随机数
		theSynCoorDinates.llSysRandom = GFC_GetSynCoorDinatesRandom();
		if (!CD_FuncCall::SendT(theSynCoorDinates))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GPlayerMe::IsInFb() const
{
	if (IsInVillage())
		return false;
	if (IsInSelfFarm())
		return false;
	//新手武器训练
	if ( kXinShouXunLianTemplateMapId == cur_template_map_id_ )
		return false;
	return true;
}

void GPlayerMe::ResetSendRoomTime()
{
	/*WriteLock wl(mutex_msg_on_hook_move_room_);
	msg_on_hook_move_room_.msg_time_ = DateTimeLocal(2000, 1, 1);*/
}

const std::string& GPlayerMe::GetCurTemplateMapName() const
{
	return cur_map_db_info_name_;
}

int GPlayerMe::GetCurMapId() const
{
	return cur_map_id_;
}

void GPlayerMe::OnGameSendMoveRoom( const CMDR_Hook_SendMoveRoom& msg )
{
	is_send_move_room_ = true;
	UpdateOwnerRoomNum();
	last_room_id_for_send_room_ = GetOwnerRoomIdx();

	if (last_send_move_cmd_)
	{
		if ( last_send_move_cmd_->cmd_ != msg.msg_.szDoorCmd )
		{
			UpdateCurMapInfo();
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "过房间时自己的过门命令与游戏发包不符，自己的：" << last_send_move_cmd_->cmd_ << " 游戏的：" << msg.msg_.szDoorCmd << " 当前地图模板ID:" << cur_template_map_id_
				<< " 当前地图名：" << cur_map_name_ << " 当前房间：" << GetOwnerRoomIdx() << " 目标房间：" << last_send_move_cmd_->dst_room_num_;
			return;
		}
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "游戏过房间命令：" << msg.msg_.szDoorCmd;
}

void GPlayerMe::AddCollectBlackList( const std::string& collect_name )
{
	collect_black_list_.Add(collect_name);
}

void GPlayerMe::ClrCollectBlackList()
{
	collect_black_list_.Clear();
}

bool GPlayerMe::IsInCollectBlackList( const std::string& collect_full_name ) const
{
	return collect_black_list_.IsInByMatch(collect_full_name);
}

bool GPlayerMe::PressKeyUntilSendMoveRoom( DWORD v_k, const GType::stGameVector3& door_pos )
{
	BOOST_SCOPE_EXIT_ALL(&){
		DxKeyUpDown(v_k, false);
	};
	SetPosEnsure(door_pos);
	DxKeyUpDown(v_k, true);
	if (WaitUntil([this](){
		return is_send_move_room_;
	}, kWaitSendMoveRoolAfterSetPos))
		return true;
	else
		return false;
}

bool GPlayerMe::TestObjPosInfoAtFindingPath( DWORD obj_packet_id, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, const GType::stGameVector3& pos_tmp )
{
	//修改的时候记得去看下TestObjBodyPartPosInfoAtFindingPath
	if ( !obj_mgr )
	{
		assert(false);
		return false;
	}
	obj_mgr->DoRebuildAll();
	auto the_obj = obj_mgr->FindByPacketId(obj_packet_id);
	if ( !the_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物不见了";
		return false;
	}
	if ( the_obj->GetOwnerRoomIdx() != obj_room_num )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物房间号变了，obj_room_num：" << obj_room_num << " 怪物现在所在房间：" << the_obj->GetOwnerRoomIdx();
		return false;
	}
	const auto& obj_now_pos = the_obj->GetPos();
	auto dist = obj_now_pos.Distance3d(pos_tmp);
	if ( dist >= kFindingPathRoleMonsterMaxDist )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物与之前的坐标变化太大，之前：" << pos_tmp << " 现在:" << obj_now_pos << " 距离：" << dist;
		return false;
	}
	return true;
}

bool GPlayerMe::SetPosEnsureSync( const GType::stGameVector3& pos )
{
	return GWndExecSync([this, &pos](){
		return SetPosEnsure(pos);
	});
	/*if ( !GWndExecSync([this, &pos](){
		if (is_send_move_room_)
			return false;
		return SetPosEnsure(pos);
	}) )
	{
		if (is_send_move_room_)
		{
			UpdateOwnerRoomNum();
			if ( last_room_id_for_send_room_ != GetOwnerRoomIdx() )
			{
				is_send_move_room_ = false;
				//本来想设置下坐标的，想想还是算了，因为这个坐标有可能还是上一个房间的坐标
				//SetPosEnsure(pos)
			}
			LOG_O(Log_debug) << "设置坐标的时候，发现游戏发过房间包了，所以忽略此次设置，坐标：" << pos;
		}
		else
			assert(false);
		return false;
	}
	return true;*/
}

void GPlayerMe::PreRunScript()
{
	last_valid_pos_.second = 0;
	is_send_move_room_ = false;
	last_room_id_for_send_room_ = -1;
}

bool GPlayerMe::UpdateRolePos()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (IsUnValidPtr(cur_role_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(cur_role_->local_obj_))
	{
		assert(false);
		return false;
	}
	if (!SetCurLocalObj(cur_role_->local_obj_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(local_obj_info_))
	{
		assert(false);
		return false;
	}
	dir_pos_info_ = &local_obj_info_->pos_dir_info;
	pos_ptr_ = local_obj_info_->pos_.CastTo<GType::stGameVector3>();
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
		return false;);
}

bool GPlayerMe::UpdateRolePosSync()
{
	return GWndExecSync([this](){
		return UpdateRolePos();
	});
}

void GPlayerMe::ResetAllUpdateData()
{
	using namespace GType;
	cur_role_ = nullptr;
	game_skill_ = nullptr;
	cur_map_id_ = std::numeric_limits<int>::max();
	cur_template_map_id_ = std::numeric_limits<int>::max();
	role_lvl_ = kDefRetValueAtDataErr;
	left_single_exp_ = kDefRetValueAtDataErr;
	left_double_exp_ = kDefRetValueAtDataErr;
	left_shou_lie_quan_cnt_ = kDefRetValueAtDataErr;
	gold_ = -1;
	silver_ = -1;
	lian_qi_cao_ = 0;
	zhan_wei_zhi_ = 0;
	poisoning_state_ = CD_En_PoisonStatus::kOk;
	jing_suan_dian_ = 0;
	zhan_wei_zhi_max_ = 0;
}

GEnterableFbMgr& GPlayerMe::GetEnterableFbMgr()
{
	return enterable_fb_;
}

bool GPlayerMe::TestTheMonsterPosZ( const GType::stGameVector3& dst_pos )
{
	const auto& the_nearest_pos = GSP_StagePosPath::GetMe().FindNearestPos(cur_template_map_id_, GetOwnerRoomIdx(), dst_pos);
	auto the_z = the_nearest_pos.fz + kMaxPosZHigh;
	if ( dst_pos.fz > the_z )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "怪物Z坐标太高了，最大Z值: " << the_z << " 而怪物的Z值：" << dst_pos.fz;
		DummySleep(kSleepTimeAtPosZError);
		return false;
	}
	the_z = the_nearest_pos.fz + kMinPosZHigh;
	if ( dst_pos.fz < the_z )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "怪物Z坐标太低了，最小Z值: " << the_z << " 而怪物的Z值：" << dst_pos.fz;
		DummySleep(kSleepTimeAtPosZError);
		return false;
	}
	return true;
}

bool GPlayerMe::IsTestTheMonsterPosZ( GameObjBase& obj )
{
	if ( kGOT_Monster == obj.GetGameObjType() )
	{
		auto the_boss = obj.CastTo<GMonster>();
		if ( the_boss && the_boss->IsBoss() /*&& IsInMonsterHighBlackList(the_boss->GetCnName())*/ )
			return true;
	}
	return false;
}

int GPlayerMe::GetLeftSingleExp() const
{
	return left_single_exp_;
}

int GPlayerMe::GetLeftDoubleExp() const
{
	return left_double_exp_;
}

bool GPlayerMe::TestObjBodyPartPosInfoAtFindingPath( DWORD obj_packet_id, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, const GType::stGameVector3& pos_tmp )
{
	//修改的时候记得去看下TestObjPosInfoAtFindingPath
	if ( !obj_mgr )
	{
		assert(false);
		return false;
	}
	obj_mgr->DoRebuildAll();
	auto the_obj = obj_mgr->FindByPacketId(obj_packet_id);
	if ( !the_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物不见了1";
		return false;
	}
	if ( the_obj->GetOwnerRoomIdx() != obj_room_num )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物房间号变了1，obj_room_num：" << obj_room_num << " 怪物现在所在房间：" << the_obj->GetOwnerRoomIdx();
		return false;
	}

	auto& hit_part = GMonster_HitPart::GetMonsterHitPart();
	if ( !hit_part.HasHitCnt() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "没有攻击部位剩余数量了";
		return false;
	}
	if ( !hit_part.Update() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "更新攻击部位失败了";
		return false;
	}
	auto bu_wei = hit_part.GetCurHitPart();
	if ( !bu_wei )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "当前并没有指定攻击部位";
		return false;
	}
	auto pos_info = the_obj->GetPosInfo();
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "攻击部位之没有posinfo";
		return false;
	}
	GType::stGameVector3 the_pos;
	GameObjDirT the_dir;
	if ( !BodyPartInfo2RolePos(*pos_info, *bu_wei, the_pos, the_dir) )
	{
		assert(false);
		return kOptionalBoolNil;
	}

	auto dist = the_pos.Distance3d(pos_tmp);
	if ( dist >= kFindingPathRoleMonsterMaxDist )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "寻路时怪物攻击部位坐标与之前的坐标变化太大1，之前：" << pos_tmp << " 现在:" << the_pos << " 距离：" << dist;
		return false;
	}
	return true;
}

int GPlayerMe::GetCurGold() const
{
	return gold_;
}

int GPlayerMe::GetCurSilver() const
{
	return silver_;
}

GItem GPlayerMe::FindTotalItemByMatchName( const std::string& item_name )
{
	if (item_name.empty())
	{
		assert(false);
		return kItemInvalid;
	}
	grids_package_.RebuildAll();
	auto item_it_ptr = grids_package_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return kItemInvalid;
	}
	GItem item_res;
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_equip_.RebuildAll();
	item_it_ptr = grids_equip_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_store_.RebuildAll();
	item_it_ptr = grids_store_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_su_cai_.RebuildAll();
	item_it_ptr = grids_su_cai_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_task_.RebuildAll();
	item_it_ptr = grids_task_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}
	return item_res;
}

void GPlayerMe::DoUpdateClear()
{
	__super::DoUpdateClear();
	ResetAllUpdateData();
}

void GPlayerMe::AddShangJinDstFbBlackList( int fb_id )
{
	/*if (IsInShangJinDstFbBlackList(fb_id))
		return;*/
	shang_jin_dst_fb_black_list_.Add(fb_id);
}

void GPlayerMe::ClrShangJinDstFbBlackList()
{
	shang_jin_dst_fb_black_list_.Clear();
}

bool GPlayerMe::IsInShangJinDstFbBlackList( int fb_id ) const
{
	/*auto it = std::find(shang_jin_dst_fb_black_list_.begin(), shang_jin_dst_fb_black_list_.end(), fb_id);
	if ( it != shang_jin_dst_fb_black_list_.end() )
		return true;
	return false;*/
	return shang_jin_dst_fb_black_list_.IsIn(fb_id);
}

void GPlayerMe::AddShangJinDstFbStableBlackList( int fb_id )
{
	/*if (IsInShangJinDstFbStableBlackList(fb_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "已存在这样的副本黑名单了，副本ID：" << fb_id;
		return;
	}*/
	shang_jin_dst_fb_stable_black_list_.Add(fb_id);
}

void GPlayerMe::ClrShangJinDstFbStableBlackList()
{
	shang_jin_dst_fb_stable_black_list_.Clear();
}

bool GPlayerMe::IsInShangJinDstFbStableBlackList( int fb_id ) const
{
	return shang_jin_dst_fb_stable_black_list_.IsIn(fb_id);
}

bool GPlayerMe::AddNewFriend( const std::string& role_name )
{
	stCD_Msg_AddFriend msg;
	memcpy_s(msg.name_, sizeof(msg.name_), role_name.c_str(), role_name.size());
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	DummySleep(3000);
	return true;
}

bool GPlayerMe::DoCraftByDstEquipItemId( const GClass::CraftDbMgr::stCraftDb& craft_item )
{
	if ( !IsInVillage() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "必须在城镇中";
		return false;
	}
	bool has_all = true;
	for ( auto& material : craft_item.materials_ )
	{
		const auto& item = FindTotalItem(material.material_item_id_);
		if ( item.item_cnt_ >= material.item_cnt_ )
			continue;
		has_all = false;
		break;
	}
	if ( !has_all )
	{
		auto& item_mgr = ItemDbMgr::GetMe();
		item_mgr.RebuildAll();
		auto item_template = item_mgr.FindById(craft_item.dst_item_id_);
		if ( !item_template )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "缺少材料，无法锻造，装备ID：" << craft_item.dst_item_id_;
			return false;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "缺少材料，无法锻造，装备名：" << item_template->item_name_;
		return false;
	}
	stCD_Msg_CraftItem msg;
	msg.craft_id_ = craft_item.craft_id_;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	DummySleep(1000);
	return true;
}

void GPlayerMe::AddFbBlackList_Cat( int fb_id )
{
	cat_fb_black_list_.Add(fb_id);
}

void GPlayerMe::ClrFbBlackList_Cat()
{
	cat_fb_black_list_.Clear();
}

bool GPlayerMe::IsInFbBlackList_Cat( int fb_id ) const
{
	return cat_fb_black_list_.IsIn(fb_id);
}

GItem GPlayerMe::FindTotalItem_TaskByMatchName( const std::string& item_name )
{
	GItem item_res;
	grids_task_.RebuildAll();
	auto item_it_ptr = grids_task_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}
	return item_res;
}

bool GPlayerMe::UseTaskItem( const std::string& item_name )
{
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品名不能为空";
		return false;
	}

	grids_task_.RebuildAll();
	auto item = grids_task_.FindByMatchName(item_name);
	if ( !item )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在该物品：" << item_name;
		return false;
	}
	if ( !item->CanBeUsed() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "目前无法使用该物品：" << item_name;
		return false;
	}

	if (is_hitting_monster_)
	{
		UpdateCurMapInfo();
		if (IsInFb())
		{
			SetPosEnsureSync(GSP_StagePosPath::GetMe().FindNearestPos(cur_template_map_id_, GetOwnerRoomIdx(), GetPos()));
		}
		else
			assert(false);
	}

	if ( !WaitToFree(kUnFreeToFreeWaitTime) )
		return false;

	//DummySleep(1000);
	item = grids_task_.FindByMatchName(item_name);
	if ( !item )
	{
		CMN_ASSERT(false);
		return false;
	}
	if (!UseItem(CD_En_ItemPackageCategory::kIPC_Task, item->item_idx_))
	{
		//assert(false);
		return false;
	}
	//DummySleep(500);
	return WaitToFree(kUnFreeToFreeWaitTime);
}

void GPlayerMe::AddFbBlackList( int fb_id )
{
	fb_black_list_.Add(fb_id);
}

void GPlayerMe::ClrFbBlackList()
{
	fb_black_list_.Clear();
}

bool GPlayerMe::IsInFbBlackList( int fb_id ) const
{
	return fb_black_list_.IsIn(fb_id);
}

bool GPlayerMe::SetHitMode_UsePacketItem( const std::string& item_name, int steps )
{
	if (item_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品名不能为空";
		return false;
	}

	grids_package_.RebuildAll();
	auto item = grids_package_.FindByMatchName(item_name);
	if ( !item )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "背包中不存在该物品：" << item_name;
		return false;
	}
	if ( !item->CanBeUsed() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "无法使用该物品：" << item_name;
		return false;
	}
	hit_mode_use_packet_item_.SetCurItem(item_name, steps);
	cur_hit_mode_ = &hit_mode_use_packet_item_;
	return true;
}

bool GPlayerMe::DoSendMoveToFbErr()
{
	if ( !msg_on_enter_fb_err_ )
		return false;
	//服务器已满
	if ( 0x73 == msg_on_enter_fb_err_->error_res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "服务器已满，无法进入副本";
		++fb_server_full_cnt_;
		return true;
	}
	else if ( 0x6a == msg_on_enter_fb_err_->error_res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "放弃了副本";
		return true;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "未知不能进入副本原因：" << msg_on_enter_fb_err_->error_res_;
		return true;
	}
}

void GPlayerMe::AddAutoWearBlackList( const std::string& item_name )
{
	auto_wear_black_list_.Add(item_name);
}

void GPlayerMe::ClrAutoWearBlackList()
{
	auto_wear_black_list_.Clear();
}

bool GPlayerMe::IsInAutoWearBlackList( const GItem& item ) const
{
	return auto_wear_black_list_.IsInByMatch(item.item_name_);
}

void GPlayerMe::ClrItemNameBlackList()
{
	item_name_black_list_.Clear();
}

void GPlayerMe::ClrAutoLvlUpEquipBlackList()
{
	auto_lvl_up_equip_black_list_.Clear();
}

void GPlayerMe::ClrItemNameWhiteList()
{
	item_name_white_list_.Clear();
}

bool GPlayerMe::UpdateOwnerRoomNum()
{
	if ( !IsInVillage() )
	{
		return __super::UpdateOwnerRoomNum();
	}
	owner_room_num_ = GFC_GetMsgRoleCityRoomIndex();
	if ( owner_room_num_ >= 0 )
		return true;
	assert(false);
	return false;
}

void GPlayerMe::ResetPassRoomData()
{
	ZeroMemory(&msg_on_hook_move_room_.msg_, sizeof(msg_on_hook_move_room_.msg_));
	is_send_move_room_ = false;
	last_room_id_for_send_room_ = -1;
	last_valid_pos_.second = 0;
}

void GPlayerMe::OnRoleDead()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << "响应角色死亡，等待复活";
	ResetPassRoomData();
	WaitRoleAlive();
	TimerDoWaitUntil([this](){
		return 0 == GetOwnerRoomIdx();
	}, [this](){
		UpdateOwnerRoomNum();
	}, 1000, 1000 * 60 * 10);
	scmd::GetMe().WaitRoleCanMove(1000 * 60 * 5);
}

bool GPlayerMe::UnlockKingCream(int fb_id, const std::string& unlock_item_name)
{
	const std::string* the_item_name;
	if (unlock_item_name.empty())
		the_item_name = &kItemName_GuWenShu;
	else
		the_item_name = &unlock_item_name;
	const auto& the_item = FindTotalItemFromPackeAndStoreNormalByMatchName(*the_item_name);
	if ( the_item.item_cnt_ < kUseCnt_Unlock_KingCream )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_debug) << kItemName_GuWenShu << " 数量不够，需要这些数量" << kUseCnt_Unlock_KingCream << "，但只有这些数量：" << the_item.item_cnt_;*/
		return false;
	}
	stCD_Msg_UnLockKingFb msg;
	msg.king_fb_id_ = fb_id;
	if (!CD_FuncCall::SendT(msg))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "解锁王立精英失败";
		return false;
	}
	if ( !TimerDoWaitUntil([=](){
		if (enterable_fb_.FindById(fb_id))
			return true;
		return false;
	}, [=](){
		enterable_fb_.RebuildAll();
	}, 1500, 1000 * 15) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "解锁王立精英副本等待超时失败";
		return false;
	}
	return true;
}

GItem GPlayerMe::FindTotalItemFromPackeAndStoreByMatchName( const std::string& item_name )
{
	if (item_name.empty())
	{
		assert(false);
		return kItemInvalid;
	}
	grids_package_.RebuildAll();
	auto item_it_ptr = grids_package_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return kItemInvalid;
	}
	GItem item_res;
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_store_.RebuildAll();
	item_it_ptr = grids_store_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_su_cai_.RebuildAll();
	item_it_ptr = grids_su_cai_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	return item_res;
}

GItem GPlayerMe::FindTotalItemFromPackeAndStoreNormalByMatchName( const std::string& item_name )
{
	if (item_name.empty())
	{
		assert(false);
		return kItemInvalid;
	}
	grids_package_.RebuildAll();
	auto item_it_ptr = grids_package_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return kItemInvalid;
	}
	GItem item_res;
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	grids_store_.RebuildAll();
	item_it_ptr = grids_store_.TraverseByMatchName(item_name);
	if ( !item_it_ptr )
	{
		assert(false);
		return item_res;
	}
	for ( item_it_ptr->First(); !item_it_ptr->IsDone(); item_it_ptr->Next() )
	{
		if ( item_res.item_cnt_ > 0 )
			item_res.item_cnt_ += item_it_ptr->CurItem().item_cnt_;
		else
			item_res = item_it_ptr->CurItem();
	}

	return item_res;
}

bool GPlayerMe::RequestDeal( const GPlayer& other )
{
	msg_on_deal_status_self_.reset();
	msg_on_deal_status_other_.reset();

	msg_on_request_deal_.reset();
	msg_on_request_deal_fail_.reset();
	stCD_Msg_RequestDeal msg;
	msg.dst_role_id_ = other.GetPacketId();
	if (!CD_FuncCall::SendT(msg))
		return false;
	if ( !WaitUntil([this](){
		if (msg_on_request_deal_)
			return true;
		if (msg_on_request_deal_fail_)
			return true;
		return false;
	}, 1000 * 35) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "请求交易等待超时失败";
		return  false;
	}
	if ( msg_on_request_deal_fail_ && CMDR_OnRequestDealFail::kEC_Succeed != msg_on_request_deal_fail_->res_ )
	{
		if ( CMDR_OnRequestDealFail::kEC_Refuse == msg_on_request_deal_fail_->res_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "请求交易失败，交易被拒绝";
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "请求交易失败，被请求玩家名：" << msg_on_request_deal_fail_->the_role_name_ << " 原因：" << msg_on_request_deal_fail_->res_;
		}
		return false;
	}
	if ( !msg_on_request_deal_ )
	{
		assert(false);
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "请求交易成功，请求方：" << msg_on_request_deal_->deal_info_.dwRequestDealRoleId << " 被请求方：" << msg_on_request_deal_->deal_info_.dwByRequestDealRoleId;
	return true;
}

bool GPlayerMe::MoveItemToDealTab( const GItem& item, int dst_slot_idx )
{
	if ( dst_slot_idx < 0 || dst_slot_idx >= kDealMaxGridCnt )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "交易栏索引不正确，格子索引：" << dst_slot_idx;
		assert(false);
		return false;
	}
	if ( item.item_cnt_ <= 0 )
	{
		assert(false);
		return false;
	}
	if ( !item.item_mgr_ptr_ )
	{
		assert(false);
		return false;
	}
	stCD_Msg_DealMoveItem move_msg;
	move_msg.wScrItemTabIndex = (decltype(move_msg.wScrItemSlotIndex))item.item_mgr_ptr_->GetTabCategory();
	move_msg.wScrItemSlotIndex = (decltype(move_msg.wScrItemSlotIndex))item.item_idx_;
	move_msg.wMoveItemCount = (decltype(move_msg.wMoveItemCount))item.item_cnt_;
	move_msg.wDealSlotaIndex = (decltype(move_msg.wDealSlotaIndex))dst_slot_idx;

	for ( auto i = 0;i != 3; ++i )
	{
		msg_on_move_item_deal_tab_.reset();
		msg_left_time_.reset();
		if (!CD_FuncCall::SendT(move_msg))
		{
			assert(false);
			return false;
		}

		if ( !WaitUntil([this](){
			if (msg_on_move_item_deal_tab_)
				return true;
			if (IsSafeTime())
				return true;
			return false;
		}, 1000 * 60) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "移动物品到交易栏超时失败，物品名：" << item.item_name_;
			return false;
		}

		if (IsSafeTime())
		{
			WaitSafeTime();
			continue;
		}
		break;
	}
	if ( !msg_on_move_item_deal_tab_ )
	{
		assert(false);
		return false;
	}

	return true;
}

bool GPlayerMe::LockDealTab()
{
	msg_on_recv_deal_status_.reset();
	stCD_Msg_LockDeal msg;
	msg.lock_deal_idx_ = 2;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}
	if ( !WaitUntil([this](){
		if (msg_on_recv_deal_status_)
			return true;
		return false;
	}, 1000 * 60) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "锁定交易等待超时失败";
		return false;
	}
	if ( !msg_on_recv_deal_status_ )
	{
		assert(false);
		return false;
	}
	if ( CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_recv_deal_status_->deal_oper_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "锁定交易未能成功，原因：" << msg_on_recv_deal_status_->deal_oper_;
		return false;
	}
	return true;
}

bool GPlayerMe::EnterDeal()
{
	if ( !WaitUntil([this](){
		if ( !msg_on_deal_status_self_ )
			return false;
		if ( !msg_on_deal_status_other_ )
			return false;
		if ( CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_deal_status_self_->deal_oper_ )
			return false;
		if ( CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_deal_status_other_->deal_oper_ && CMDR_OnRecvDealStatus::kO_Enter != msg_on_deal_status_other_->deal_oper_ )
			return false;
		return true;
	}, 1000 * 60) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "等待双方锁定交易状态超时失败";
		return false;
	}
	if ( !msg_on_deal_status_self_ || !msg_on_deal_status_other_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不知道双方的交易状态，所以不能确定交易";
		return false;
	}
	if ( CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_deal_status_other_->deal_oper_ || CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_deal_status_self_->deal_oper_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "双方必须都是交易锁定时才能确认交易";
		return false;
	}

	msg_on_recv_deal_end_.reset();
	msg_on_recv_deal_status_.reset();
	msg_on_deal_fail_.reset();
	stCD_Msg_EnterDeal msg;
	msg.lock_deal_idx_ = 3;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	BOOST_SCOPE_EXIT_ALL(&){
		msg_on_deal_status_self_.reset();
		msg_on_deal_status_other_.reset();
	};
	if ( !WaitUntil([this](){
		if (msg_on_recv_deal_end_)
			return true;
		if (msg_on_recv_deal_status_)
			return true;
		if (msg_on_deal_fail_)
			return true;
		return false;
	}) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "确认交易等待超时失败";
		return false;
	}

	if (msg_on_deal_fail_)
	{
		if ( CMDR_OnDealFail::kEC_ItemCntLimit == msg_on_deal_fail_->res_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "交易失败，物品超出携带上限";
			return false;
		}
		else if ( CMDR_OnDealFail::kEC_DstDealing == msg_on_deal_fail_->res_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "交易失败，因为对方正在交易";
			return false;
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "未知交易失败原因：" << msg_on_deal_fail_->res_;
			return false;
		}
	}

	if (msg_on_deal_status_self_)
	{
		if ( CMDR_OnRecvDealStatus::kO_Enter != msg_on_deal_status_self_->deal_oper_  && CMDR_OnRecvDealStatus::kO_LockDeal != msg_on_deal_status_self_->deal_oper_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "确定交易未能成功";
			return false;
		}
	}

	if ( !WaitUntil([this](){
		if (msg_on_recv_deal_end_)
			return true;
		return false;
	}) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "确认交易之等待完成交易超时失败";
		return false;
	}
	
	if ( !msg_on_recv_deal_end_ )
	{
		assert(false);
		return false;
	}
	/*if ( CMDR_OnRecvDealEnd::kEC_Succeed != msg_on_recv_deal_end_->res_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "确认交易结果失败，原因：" << msg_on_recv_deal_end_->res_;
		return false;
	}*/
	
	return true;
}

bool GPlayerMe::AcceptOneDealRequest()
{
	if ( !msg_on_recv_quest_deal_ )
		return false;
	BOOST_SCOPE_EXIT_ALL(&){
		msg_on_recv_quest_deal_.reset();
	};
	DateTimeLocal time_now;
	if ( (time_now - msg_on_recv_quest_deal_->recv_time_).totalSeconds() > kQuestDealValidTime )
		return false;

	msg_on_request_deal_.reset();
	stCD_Msg_AcceptDeal msg;
	msg.no_accept_ = false;
	msg.deal_key_ = msg_on_recv_quest_deal_->deal_key_;
	if (!CD_FuncCall::SendT(msg))
	{
		assert(false);
		return false;
	}

	if ( !WaitUntil([this](){
		if (msg_on_request_deal_)
			return true;
		return false;
	}, 1000 * 5) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "接受交易请求等待超时失败";
		return false;
	}
	if ( !msg_on_request_deal_ )
	{
		assert(false);
		return false;
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "接受交易成功，请求方：" << msg_on_request_deal_->deal_info_.dwRequestDealRoleId << " 被请求方：" << msg_on_request_deal_->deal_info_.dwByRequestDealRoleId;
	return true;
}

bool GPlayerMe::DealGold( int gold )
{
	UpdateGold_Notify();
	if ( gold > 0 )
	{
		gold = std::min(gold, gold_);
	}
	else if ( gold < 0 )
	{
		gold = gold_ + gold;
	}
	if ( gold <= 0 )
		return true;
	stCD_Msg_DealGold msg;
	for ( auto i = 0;i != 3; ++i )
	{
		msg_on_move_deal_gold_.reset();
		msg_left_time_.reset();
		msg.gold_ = gold_;
		if (!CD_FuncCall::SendT(msg))
		{
			assert(false);
			return false;
		}

		if ( !WaitUntil([this](){
			if (msg_on_move_deal_gold_)
				return true;
			if (IsSafeTime())
				return true;
			return false;
		}, 1000 * 60) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "交易金币超时失败，要交易的金币数量：" << gold;
			return false;
		}

		if (IsSafeTime())
		{
			WaitSafeTime();
			continue;
		}
		break;
	}
	if ( !msg_on_move_deal_gold_ )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPlayerMe::IsNiRenStatus()
{
	return IsTheCurStatus(kBuffName_NiRen);
}

bool GPlayerMe::UpdateHp()
{
	DO_EXCEPTION_TRY;
	cur_role_ = CD_FuncCall::GetCurRole();
	if (!cur_role_)
	{
		assert(false);
		return false;
	}
	cur_hp_ = cur_role_->cur_hp_;
	feeb_hp_ = cur_role_->nei_shang_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		ResetAllUpdateData();
	return false;);
}

bool GPlayerMe::UpdateHpSync()
{
	return GWndExecSync([this](){
		return UpdateHp();
	});
}

bool GPlayerMe::IsTheCurStatus( const std::string& buff_name )
{
	using namespace GClass;
	auto& buff_mgr = BuffStateMgr::GetMe();
	buff_mgr.RebuildAll();
	auto the_buff = buff_mgr.FindByName(buff_name);
	if (the_buff)
	{
		return true;
	}
	return false;
}

bool GPlayerMe::IsRanShaoStatus()
{
	return IsTheCurStatus(kBuffName_RanShao);
}

bool GPlayerMe::IsErChouStatus()
{
	return IsTheCurStatus(kBuffName_ErChou);
}

bool GPlayerMe::IsSnowManStatus()
{
	return IsTheCurStatus(kBuffName_SnowMan);
}

void GPlayerMe::GenMySellListToJson( Json::Value& j_value )
{
	if ( !msg_on_my_sell_list_ )
	{
		//assert(false);
		return;
	}
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();

	Json::Value j_tmp;
	for ( auto& v : msg_on_my_sell_list_->items_info_ )
	{
		stAuctionItemInfoEx item_info(v);
		if ( !item_info.ConvertToJson(j_tmp) )
		{
			assert(false);
			continue;
		}
		auto the_item = item_mgr.FindById(item_info.item_id_);
		if ( !the_item )
		{
			assert(false);
			continue;
		}
		item_info.item_name_ = the_item->item_name_;
		if ( !item_info.ConvertToJsonEx(j_tmp) )
		{
			assert(false);
			continue;
		}

		j_value.append(j_tmp);
	}
}

void GPlayerMe::GenOthersSellToJson( Json::Value& j_value )
{
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();

	Json::Value j_tmp;
	for ( auto& v : other_side_items_ )
	{
		if ( !IsPrivateSell(v.begin_price_) )
			continue;
		stAuctionItemInfoEx item_info(v);
		if ( !item_info.ConvertToJson(j_tmp) )
		{
			assert(false);
			continue;
		}
		auto the_item = item_mgr.FindById(item_info.item_id_);
		if ( !the_item )
		{
			assert(false);
			continue;
		}
		item_info.item_name_ = the_item->item_name_;
		if ( !item_info.ConvertToJsonEx(j_tmp) )
		{
			assert(false);
			continue;
		}

		j_value.append(j_tmp);
	}
}

void GPlayerMe::EraseMySellItem( const stAuctionItemInfo& item_info )
{
	if ( !msg_on_my_sell_list_ )
		return;

	auto it = std::find(msg_on_my_sell_list_->items_info_.begin(), msg_on_my_sell_list_->items_info_.end(), item_info);
	if ( it != msg_on_my_sell_list_->items_info_.end() )
		msg_on_my_sell_list_->items_info_.erase(it);
}

bool GPlayerMe::IsChanRaoStatus()
{
	return IsTheCurStatus(kBuffName_ChanRao);
}

const GPlayerMe::WearWhiteListPairT* GPlayerMe::FindAutoWearWhiteListItem( const std::string& equip_name )
{
	if (equip_name.empty())
	{
		assert(false);
		return nullptr;
	}
	return auto_wear_white_list_.FindBy([&equip_name]( const WearWhiteListPairT& v ){
		return v.first == equip_name;
	});
}

void GPlayerMe::AddAutoWearWhiteList( const std::string& item_name, const std::string& item_type )
{
	auto_wear_white_list_.Add(WearWhiteListPairT(item_name, item_type));
}

GType::enFbConsumeMode GPlayerMe::GetCurFbValidConsumeMode()
{
	using namespace GType;
	if (kFCM_ShouLieQuan == fb_consume_mode_)
	{
		if (left_shou_lie_quan_cnt_ <= 0)
			return enFbConsumeMode(-1);
		return fb_consume_mode_;
	}
	else if (kFCM_Silver == fb_consume_mode_)
	{
		UpdateGold_Notify();
		if (silver_ < fb_consume_mode_min_silver_)
			return enFbConsumeMode(-1);
		return fb_consume_mode_;
	}
	else if (kFCM_All == fb_consume_mode_)
	{
		if (left_shou_lie_quan_cnt_ > 0)
			return kFCM_ShouLieQuan;
		UpdateGold_Notify();
		if (silver_ < fb_consume_mode_min_silver_)
			return enFbConsumeMode(-1);
		return kFCM_Silver;
	}
	assert(false);
	return enFbConsumeMode(-1);
}

void GPlayerMe::HitRaii()
{
	is_hitting_monster_ = false;
}

void GPlayerMe::SetCurHitMode(GClass::GHitMonsterMode_Mouse::enHitMode hit_mide)
{
	assert(hit_mide <= GClass::GHitMonsterMode_Mouse::kInvalid);
	hit_mode_mouse_.SetCurTimeHitMode(hit_mide);
}

GPlayer::GPlayer() : GameObjBase(kGOT_Player)
{

}

GPlayerMe::stShangJinTaskInfo::stShangJinTaskInfo()
{
	task_template_ = nullptr;
	test_fb_diff_lvl_ = true;
}
