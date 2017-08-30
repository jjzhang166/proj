#include "stdafx.h"
#include "GameObj.h"
#include <Common/LogOut.h>
#include "GOffset.h"
#include <Common/MemOper.h>
#include "GMap.h"
#include "GConst.h"
#include "GameObjMgr.h"

void GameObjBase::SetGameObjAddr( void* obj_addr )
{
	game_obj_addr_ = (char*)obj_addr;
	assert(game_obj_addr_);
}

void* GameObjBase::GetGameObjAddr() const
{
	return game_obj_addr_;
}

void GameObjBase::SetObjType( GameObjType obj_type )
{
	assert(kGOT_Invalid == obj_type_ || obj_type_ == obj_type);
	obj_type_ = obj_type;
}

GameObjType GameObjBase::GetGameObjType() const
{
	return obj_type_;
}

bool GameObjBase::Update()
{
	using namespace GOffset;
	using namespace GClass;
	if ( !game_obj_addr_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game object addr is empty";
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(obj_serial_, (int)game_obj_addr_, g_o_CreatureSerial) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取game obj之serial失败";
		assert(false);
		return false;
	}
	auto game_str_tmp = (stGameString*)(game_obj_addr_ + g_o_CreatureJob_Name);
	assert(game_str_tmp);
	if ( !game_str_tmp->CopyTo(obj_job_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read obj job name error";
		assert(false);
		return false;
	}
	game_str_tmp = (stGameString*)(game_obj_addr_ + g_o_CreatureName);
	if ( !game_str_tmp->CopyTo(obj_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read obj name error";
		assert(false);
		return false;
	}
	stGameVectorCopier* vec_copier = (stGameVectorCopier*)(game_obj_addr_ + g_o_CreaturePosX);
	assert(vec_copier);
	vec_copier->CopyTo(obj_pos_);
	game_str_tmp = (stGameString*)(game_obj_addr_ + g_o_CreatureType);
	if ( !game_str_tmp->CopyTo(game_obj_type_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read obj type name error";
		assert(false);
		return false;
	}
	auto obj_type = CalcGameObjType(game_obj_type_name_);
	if ( obj_type_ != obj_type )
	{
		LOG_O(Log_debug) << "对象类型不正确了，之前->" << obj_type_ << "\t之后->" << obj_type;
		//assert(false);
		return false;
	}
	if ( !UpdateHpMp() )
	{
		assert(false);
		return false;
	}

	if ( !UpdateNpcValidTypeData() )
	{
		assert(false);
		return false;
	}
	return true;
}

void GameObjBase::SetObjSerial( DWORD obj_serial )
{
	obj_serial_ = obj_serial;
}

DWORD GameObjBase::GetObjSerial() const
{
	return obj_serial_;
}

GameObjType GameObjBase::CalcGameObjType( const std::wstring& game_obj_type_name )
{
	if (game_obj_type_name.empty())
		return kGOT_Invalid;
	if ( L"MISC" == game_obj_type_name )
		return kGOT_MISC;
	if ( kGameObjType_NPC == game_obj_type_name )
	{
		auto gmap = GMap::GetInstance();
		if ( !gmap )
		{
			assert(false);
			return kGOT_Invalid;
		}
		if (gmap->IsTown())
			return kGOT_Npc;
		else
			return kGOT_Monster;
	}
	if ( L"PLAYER" == game_obj_type_name )
		return kGOT_Player;
	if ( L"UNKNOWN" == game_obj_type_name )
		return kGOT_Unknown;
	assert(false);
	return kGOT_Invalid;
}

DWORD GameObjBase::GetCurHp() const
{
	return cur_hp_;
}

DWORD GameObjBase::GetHpLimit() const
{
	return hp_limit_;
}

DWORD GameObjBase::GetCurMp() const
{
	return cur_mp_;
}

DWORD GameObjBase::GetMpLimit() const
{
	return mp_limit_;
}

const std::wstring& GameObjBase::GetJobName() const
{
	return obj_job_name_;
}

bool GameObjBase::UpdateHpMp()
{
	using namespace GOffset;
	if ( !MemOperApplyR(cur_hp_, (int)game_obj_addr_, g_o_CurHp) )
	{
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(hp_limit_, (int)game_obj_addr_, g_o_HpLimit) )
	{
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(cur_mp_, (int)game_obj_addr_, g_o_CurMp) )
	{
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(mp_limit_, (int)game_obj_addr_, g_o_MpLimit) )
	{
		assert(false);
		return false;
	}
	return true;
}

const GType::stGameVector& GameObjBase::GetPos() const
{
	return obj_pos_;
}

const std::wstring& GameObjBase::GetObjName() const
{
	return obj_name_;
}

bool GameObjBase::UpdateNpcValidTypeData()
{
	using namespace GOffset;
	if ( !MemOperApplyR(npc_type_9d4_, (int)game_obj_addr_, g_o_game_obj_npc_type) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取game obj之g_o_game_obj_npc_type失败";
		assert(false);
		return false;
	}
	return true;
}

bool GameObj_Parser::Update()
{
	using namespace GOffset;
	using namespace GClass;
	if ( !game_obj_addr_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game object addr is empty";
		assert(false);
		return false;
	}
	obj_valid_ = true;

	auto game_str_tmp = (stGameString*)(game_obj_addr_ + g_o_CreatureType);
	if ( !game_str_tmp->CopyTo(game_obj_type_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read obj type name error";
		assert(false);
		return false;
	}
	obj_type_ = CalcGameObjType(game_obj_type_name_);

	if ( !UpdateNpcValidTypeData() )
	{
		assert(false);
		return false;
	}
	if ( !CalcObjValid() )
		return false;

	return true;
}

GameObj_Parser::GameObj_Parser() : GameObjBase(kGOT_ParseObjType)
{
	obj_valid_ = true;
}

bool GameObj_Parser::ObjValid() const
{
	return obj_valid_;
}

bool GameObj_Parser::CalcObjValid()
{
	using namespace GClass;
	using namespace GOffset;
	if ( !GMap::GetMe().IsTown() )
	{
		auto game_str_tmp = (stGameString*)(game_obj_addr_ + g_o_CreatureName);
		if ( !game_str_tmp->CopyTo(obj_name_) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "read obj name error";
			assert(false);
			return false;
		}
		if ( !obj_name_.empty() )
		{
			//过滤掉工地钻孔机
			if ( obj_name_ == L"S_Drill_Ep4" )
				obj_valid_ = true;
			else
				obj_valid_ = !GameObjMgr::GetMe().ExistTheBlacklist(obj_name_) && 0 == npc_type_9d4_ || kGameObjType_NPC != game_obj_type_name_;
		}
	}
	return true;
}

bool GameObj_ParserAny::CalcObjValid()
{
	return true;
}
