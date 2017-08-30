#include "stdafx.h"
#include "GMonster.h"
#include <Common/MemOper.h>
#include "GameObjMgr.h"
#include <CD_CommonFiles/Data/CD_UsefulFns.h>

GMonster::GMonster()
{
	cur_hp_ = std::numeric_limits<int>::max();
}

bool GMonster::UpdateImpl()
{
	if ( !__super::UpdateImpl() )
		return false;

	if (!net_obj_ || !game_obj_db_info_)
	{
		assert(false);
		return false;
	}
	obj_db_info_.obj_db_info_ = game_obj_db_info_;
	if (!obj_db_info_.Update())
	{
		assert(false);
		return false;
	}
	name_ = obj_db_info_.ch_name_;
	return true;
}

void GMonster::DebugInfo() const
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return;
	}
	GType::stGameVector3 pos = GetPos(), dir_lhs = dir_pos_info_->dir_1, dir_rhs = dir_pos_info_->dir_2;
	LOG_O(Log_debug) << "obj id:" << obj_id_ << "\tpos:(" << pos.fx << "," << pos.fy << "," << pos.fz << ")\t" << "dir lhs:("
		<< dir_lhs.fx << "," << dir_lhs.fy << ")\tdir rhs:(" <<
		dir_rhs.fx << "," << dir_rhs.fy
		<<
		")\tname:" << name_
		<< "\t ch name:" << name_ << "cur hp:" << cur_hp_;
}

CD_En_ObjSubType GMonster::GetSubType() const
{
	return obj_db_info_.type_;
}

int GMonster::GetCurHp() const
{
	return cur_hp_;
}

void GMonster::SetCurHp( int cur_hp )
{
	//LOG_O(Log_trace) << "monster hp:" << cur_hp;
	if ( cur_hp_ != cur_hp )
	{
		static int last_hp = -1;

		if ( cur_hp % 200 < 20 && last_hp != cur_hp )
		{
			LOG_O(Log_trace) << "set monster new hp, old hp:" << cur_hp_ << "  new hp:" << cur_hp;
		}
		cur_hp_ = cur_hp;
		last_hp = cur_hp;
		if ( cur_hp == 0 )
		{
			GameObjMgr::GetMe().AddDeadMonster(GetPacketId());
		}
	}	
}

bool GMonster::IsBoss() const
{
	return obj_db_info_.type_ == CD_En_ObjSubType::kBoss;
}

const std::string& GMonster::GetTitle() const
{
	return obj_db_info_.title_;
}

const std::string& GMonster::GetCnName() const
{
	return name_;
}

int GMonster::GetObjDbId() const
{
	return obj_db_info_.obj_db_id_;
}

bool GMonster::ObjValid()
{
	if (!__super::ObjValid())
		return false;
	if (!net_obj_)
		return false;
	return true;
}

bool GMonster_HitPartBase::ObjValid()
{
	if (IsUnValidPtr(body_parts_obj_) || IsUnValidPtr(body_parts_info_.value_) || body_parts_info_.cnt_ <= 0)
		return false;
	obj_type_ = kGOT_Invalid;
	if (!p_supper::ObjValid())
		return false;
	if (!UpdateImpl())
		return false;
	if (!UpdatePacketId())
		return false;

	obj_type_ = kGOT_Monster;
	return true;
}

GMonster_HitPart::GMonster_HitPart()
{
	is_new_hit_ = false;
	cur_hit_part_idx_ = -1;
	hit_cnt_ = 0;
	cur_hit_obj_packet_id_ = -1;
}

bool GMonster_HitPartBase::SetBodyPartsObjAddr(stCD_BodyPartObj* body_part_obj)
{
	if (IsUnValidPtr(body_part_obj))
		return false;
	if (IsUnValidPtr(body_part_obj->obj_info_))
		return false;
	if (body_part_obj->obj_info_->GetType() != CD_En_LocalObjType::kNpc)
		return false;
	auto the_net_obj = body_part_obj->obj_info_->net_obj_;
	if (IsUnValidPtr(the_net_obj))
		return false;
	auto the_obj_db_info = the_net_obj->CastTo<stCD_NetObj_Npc>()->db_info_;
	if (IsUnValidPtr(the_obj_db_info))
		return false;
	/*if (the_obj_db_info->type_ != CD_En_ObjSubType::kXiaoGuai)
		return false;*/
	//只是boss
	if (the_obj_db_info->type_ != CD_En_ObjSubType::kBoss)
		return false;
	/*攻击怪物和怪物攻击角色时会到，攻击怪物时部位的数量会小于3。
	不管是攻击怪物，还是怪物攻击，body_parts_obj_对象都是一样的，而stCD_BodyPartObj::body_part_info_array_字段会变，即攻击怪物和怪物攻击时不一样，
	但每次攻击怪物stCD_BodyPartObj::body_part_info_array_是一样的，每次怪物攻击stCD_BodyPartObj::body_part_info_array_是一样的。
	*/
	if (body_part_obj->body_part_cnt_ <= 3)
		return false;
	if (IsUnValidPtr(body_part_obj->body_part_info_array_))
		return false;
	if (!SetCurLocalObjInfo(body_part_obj->obj_info_))
		return false;
	body_parts_obj_ = body_part_obj;
	body_parts_info_.value_ = body_part_obj->body_part_info_array_;
	body_parts_info_.cnt_ = body_part_obj->body_part_cnt_;
	return true;
}

void GMonster_HitPart::SetHitInfo( int cur_hit_part_idx, int hit_cnt, DWORD cur_packet_id )
{
	cur_hit_part_idx_ = cur_hit_part_idx;
	hit_cnt_ = hit_cnt;
	cur_hit_obj_packet_id_ = cur_packet_id;
}

const stCD_BodyPartInfo* GMonster_HitPart::GetCurHitPart() const
{
	if ( !is_new_hit_ )
		return nullptr;
	if ( hit_cnt_ <= 0 )
		return nullptr;
	auto it = std::find_if(body_parts_info_.begin(), body_parts_info_.end(), [this](const stCD_BodyPartInfo& v){
		return v.dwBuWeiId == cur_hit_part_idx_;
	});
	if (it == body_parts_info_.end())
		return nullptr;
	return &*it;
}

void GMonster_HitPart::DecHitCnt()
{
	if (is_new_hit_)
	{
		is_new_hit_ = false;
		if ( hit_cnt_ <= 0 )
			return;
		--hit_cnt_;
	}	
}

GMonster_HitPart& GMonster_HitPart::GetMonsterHitPart()
{
	static std::shared_ptr<GMonster_HitPart> monster_ptr(new GMonster_HitPart);
	return *monster_ptr;
}

bool GMonster_HitPart::HasHitCnt() const
{
	return hit_cnt_ > 0;
}

int GMonster_HitPart::FindThePartIdx( const std::string& monster_name, const std::string& part_name ) const
{
	if (monster_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "怪物名不能为空";
		return -1;
	}
	const MonsterName2BodyPartInfoT::value_type* the_value = FindByMonsterName(monster_name);
	if ( !the_value )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的部位信息，因为索引不到这样的怪物，怪物名：" << monster_name << " 部位名：" << part_name;
		return -1;
	}
	auto it_part = the_value->second.find(part_name);
	if ( it_part == the_value->second.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的部位信息，因为索引不到这样的部位，怪物名：" << monster_name << " 部位名：" << part_name;
		return -1;
	}
	return it_part->second;
}

int GMonster_HitPart::FindThePartIdx( const std::string& part_name ) const
{
	return FindThePartIdx(name_, part_name);
}

bool GMonster_HitPart::AddMonsterBodyPartInfo( const std::string& monster_name, const std::string& part_name, int body_part_idx )
{
	if (monster_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加怪物部位信息失败，怪物名不能为空,怪物名：" << monster_name << " 部位名：" << part_name << "  部位索引：" << body_part_idx;
		assert(false);
		return false;
	}
	if (part_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加怪物部位信息失败，部位名不能为空,怪物名：" << monster_name << " 部位名：" << part_name << "  部位索引：" << body_part_idx;
		assert(false);
		return false;
	}
	if ( body_part_idx < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加怪物部位信息失败，部位索引必须大于等于0,怪物名：" << monster_name << " 部位名：" << part_name << "  部位索引：" << body_part_idx;
		assert(false);
		return false;
	}
	auto& body_parts = monster_part_info_[monster_name];
	auto it = body_parts.find(part_name);
	if ( it != body_parts.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加怪物部位信息失败，已存在这样的部位信息了，怪物名：" << monster_name << " 部位名：" << part_name << "  部位索引：" << body_part_idx;
		return false;
	}
	body_parts[part_name] = body_part_idx;
	return true;
}

bool GMonster_HitPart::UpdateImpl()
{
	if (IsUnValidPtr(local_obj_info_))
		return false;
	if (IsUnValidPtr(net_obj_))
		return false;
	if ( !__super::UpdateImpl() )
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(body_parts_info_.value_))
		return false;

	if ( packet_id_ != cur_hit_obj_packet_id_ )
		return false;
	if (body_parts_info_.cnt_ < 0 || body_parts_info_.cnt_ > 0x40)
	{
		assert(false);
		return false;
	}

	/*LOG_O(Log_trace) << "此部位信息的所属对象名：" << name_ << std::hex << " packet_id:" << packet_id_ <<
		" local_obj_info:" << local_obj_info_ << " body_parts_obj:" << body_parts_obj_ << " body_part_addr:" << body_parts_info_.value_;
	for (auto& body_part_addr : body_parts_info_)
	{
		LOG_O(Log_trace) << "部位ID:" << body_part_addr.dwBuWeiId << " pos右边：" << body_part_addr.theRangeRight
			<< " pos左边:" << body_part_addr.theRangeLeft;
	}*/

	/*body_parts_.clear();
	for (auto& body_part_addr : body_parts_info_)
	{
		body_parts_.push_back(body_part_addr);
		LOG_O(Log_trace) << "部位ID:" << body_part_addr.dwBuWeiId << " pos右边：" << body_part_addr.theRangeRight
			<< " pos左边:" << body_part_addr.theRangeLeft;
	}*/

	//有可能更新好几次，即这个函数会进来好几次
	//assert(!is_new_hit_);
	is_new_hit_ = true;
	return true;
}

void GMonster_HitPart::DebuInfo_AllBodyPart()
{
	LOG_ENTER_FUNC;
	for ( auto& v : monster_part_info_ )
	{
		for ( auto& the_v : v.second )
		{
			LOG_O(Log_info) << "怪物名：" << v.first << " 部位名：" << the_v.first;
		}
	}
}

bool GMonster_HitPart::TestCurBodyPartInfo() const
{
	auto body_part = FindByMonsterName(name_);
	if (body_part)
	{
		LOG_O(Log_trace) << "测试怪物部位开始，怪物名：" << name_;
		int the_cnt = 0;
		for (auto& v : body_parts_info_)
		{
			bool is_find = false;
			for (auto& part_v : body_part->second)
			{
				if (v.dwBuWeiId == part_v.second)
				{
					is_find = true;
					break;
				}
			}
			if (!is_find)
			{
				++the_cnt;
				LOG_O(Log_debug) << "未知部位ID：" << (int)v.dwBuWeiId;
			}
		}
		int valid_cnt = 0;
		for (auto& part_v : body_part->second)
		{
			bool is_find = false;
			for (auto& v : body_parts_info_)
			{
				if (part_v.second == v.dwBuWeiId)
				{
					is_find = true;
					break;
				}
			}
			if (!is_find)
			{
				++the_cnt;
				LOG_O(Log_debug) << "添加了多余的部位ID：" << part_v.second;
			}
			else
			{
				++valid_cnt;
			}
		}
		LOG_O(Log_trace) << "有效部位数：" << valid_cnt;
		return 0 == the_cnt;
	}
	else
	{
		LOG_O(Log_debug) << "BodyPartInfo.lua中没有这样的怪物名：" << name_;
		for (auto& v : body_parts_info_)
		{
			LOG_O(Log_debug) << "1未知部位ID：" << (int)v.dwBuWeiId;
		}
		return true;
	}
}

const GMonster_HitPart::MonsterName2BodyPartInfoT::value_type* GMonster_HitPart::FindByMonsterName( const std::string& monster_name ) const
{
	for ( auto& v : monster_part_info_ )
	{
		if ( monster_name.find(v.first) != std::string::npos )
			return &v;
	}
	return nullptr;
}

void GMonster_HitPart::ClrAll()
{
	body_parts_obj_ = nullptr;
	body_parts_info_.value_ = nullptr;
	body_parts_info_.cnt_ = 0;
	SetHitInfo(-1, 0, -1);
	//body_parts_.clear();
}

bool GMonster_HitPart::From( const GMonster_HitPartBase& rhs )
{
	body_parts_obj_ = rhs.GetBodyPartsObj();
	body_parts_info_ = rhs.body_parts_info_;
	local_obj_info_ = rhs.GetObjBase();
	game_obj_db_info_ = rhs.game_obj_db_info_;
	net_obj_ = rhs.GetNetObj();
	if (!net_obj_ || !local_obj_info_)
		return false;
	obj_type_ = rhs.GetGameObjType();
	obj_db_info_ = rhs.obj_db_info_;
	return true;
}

int GMonster_HitPart::FindPartIdxByTaskDesc( const std::string& monster_name, const std::string& task_desc ) const
{
	if (monster_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "怪物名不能为空";
		return -1;
	}
	if (task_desc.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "任务描述不能为空";
		return -1;
	}
	const MonsterName2BodyPartInfoT::value_type* the_value = FindByMonsterName(monster_name);
	if ( !the_value )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的部位信息，因为索引不到这样的怪物，怪物名：" << monster_name;
		return -1;
	}
	for ( auto& part : the_value->second )
	{
		if ( task_desc.find(part.first) != std::string::npos )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "找到了这样的部位信息，部位名：" << part.first << " 任务描述：" << task_desc;
			return part.second;
		}
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "不存在这样的部位信息，因为索引不到这样的部位，怪物名：" << monster_name << " 任务描述：" << task_desc;
	return -1;
}

void GMonster_HitPart::DoUpdateClear()
{
	//body_parts_.clear();
	body_parts_info_.value_ = nullptr;
	body_parts_info_.cnt_ = 0;
}

GMonster_HitPartBase::GMonster_HitPartBase()
{
	body_parts_obj_ = nullptr;
}

stCD_BodyPartObj* GMonster_HitPartBase::GetBodyPartsObj() const
{
	return body_parts_obj_;
}

bool GMonster_HitPartBase::UpdatePacketId()
{
	return __super::UpdatePacketId();
	/*if (!local_obj_info_)
	{
		assert(false);
		return false;
	}
	packet_id_ = local_obj_info_->tmp_packet_id_;
	return true;*/
}

GMonsterBase::GMonsterBase() : GameObjBase(kGOT_Monster)
{
	game_obj_db_info_ = nullptr;
}

bool GMonsterBase::ObjValid()
{
	if (!local_obj_info_)
		return false;
	if (local_obj_info_->GetType() != CD_En_LocalObjType::kNpc)
		return false;
	if (IsUnValidPtr(local_obj_info_->net_obj_))
		return false;
	auto npc = local_obj_info_->net_obj_->CastTo<stCD_NetObj_Npc>();
	if (!npc->ObjValid())
		return false;
	if (IsUnValidPtr(npc->db_info_))
		return false;
	if (npc->db_info_->type_ > CD_En_ObjSubType::kBuJiXiang)
		return false;
	game_obj_db_info_ = npc->db_info_;
	obj_type_ = kGOT_Monster;
	return true;
}
