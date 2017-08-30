#include "stdafx.h"
#include "GameObjMgr.h"
#include <Common/LogOut.h>
#include "GNpc.h"
#include "GPlayer.h"
#include <Common/UsefulFunc.h>
#include "GMonster.h"
#include "GConst.h"
#include <CD_CommonFiles/Data/CD_Traverse.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>

bool GameObjMgrBase::UpdateAll()
{
	assert(false);
	if (btree_.empty())
		return false;

	return true;
}

GameObjMgrBase::GameObjMgrBase()
{
}

bool GameObjMgrBase::IsGameObjsEmpty() const
{
	return btree_.empty();
}

std::size_t GameObjMgrBase::GetAllGameObjCnt() const
{
	return btree_.size();
}

GameObjMgrBase::GameObjIterPtr GameObjMgrBase::FromIterImpl( GameObjIterPtrImpl& iter_impl )
{
	if ( !iter_impl )
	{
		assert(false);
		return GameObjIterPtr();
	}
	return MakeWrapperIterator<GameObjTraTraits>(iter_impl, []( GameObjIterItemImpl iter_wrapper ) ->GameObjIterItem{
		auto& wra = iter_wrapper.GetHandler();
		assert(wra);
		return *wra;
	});
}

GameObjMgrBase::GameObjIterPtr GameObjMgrBase::TraverseAll()
{
	return FromIterImpl(MakeIterator<GameObjTraTraitsImpl>(btree_));
}

void GameObjMgrBase::TraverseAllDebugInfo()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "total obj cnt->" << btree_.size();
	const auto& iter = TraverseAll();
	if ( !iter )
	{
		assert(false);
		return;
	}
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto& obj = iter->CurItem();
		obj.DebugInfo();
	}
}

GameObjBase* GameObjMgrBase::FindIf( const DoAnyGameObjFuncT& func )
{
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if (func(*real_obj))
				return real_obj.get();
		}
	}
	return nullptr;
}

GameObjBase* GameObjMgrBase::FindByName( const std::string& cn_name )
{
	if (cn_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if (cn_name == real_obj->GetName())
				return real_obj.get();
		}
	}
	return nullptr;
}

GameObjMgrBase::GameObjIterPtr GameObjMgrBase::TraverseBeatableMonster()
{
	return FromIterImpl(MakeIterator<GameObjTraTraitsImpl>(btree_, [this]( GameObjWrapper& obj ){
		auto& obj_handler = obj.GetHandler();
		if ( !obj_handler )
		{
			assert(false);
			return false;
		}
		if ( kGOT_Monster == obj_handler->GetGameObjType() )
		{
			auto obj_monster = obj_handler->CastTo<GMonster>();
			if ( !obj_monster )
			{
				assert(false);
				return false;
			}
			if (IsDeadMonster(*obj_monster))
				return false;
			auto sub_type = obj_monster->GetSubType();
			if (CD_En_ObjSubType::kXiaoGuai == sub_type || CD_En_ObjSubType::kBoss == sub_type)
				return true;
		}
		return false;
	}));
}

GameObjBase* GameObjMgrBase::FindNearestBeatableMonster_RoomNum( GameObjBase& obj_src, int room_num )
{
	return FindNearestImpl_RoomNum(TraverseBeatableMonster(), obj_src, room_num);
}

void GameObjMgrBase::ClearDeadMonsters()
{
	dead_monsters_.clear();
}

void GameObjMgrBase::AddDeadMonster( DWORD packet_id )
{
	auto the_dead_monster = FindByPacketId(packet_id);
	if ( !the_dead_monster )
	{
		CMN_ASSERT_(false, "AddDeadMonster");
		return;
	}
	auto& shared_obj = the_dead_monster->shared_from_this();
	if ( !shared_obj )
	{
		CMN_ASSERT_(false, "AddDeadMonster1");
		return;
	}
	dead_monsters_.insert(shared_obj);
	if ( kGOT_Monster == shared_obj->GetGameObjType() )
	{
		auto monster_obj = shared_obj->CastTo<GMonster>();
		if ( !monster_obj )
		{
			CMN_ASSERT_(false, "AddDeadMonster2");
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "怪物死亡了，ppacket_id:" << packet_id << " 怪物名：" << monster_obj->GetCnName();
	}
}

bool GameObjMgrBase::IsDeadMonster( GameObjBase& obj )
{
	return IsDeadMonsterByPacketId(obj.GetPacketId());
}

void GameObjMgrBase::UpdateMonsterHp( DWORD monster_packet_id, int cur_hp )
{
	auto obj = FindByPacketId(monster_packet_id);
	if ( !obj )
	{
		if ( !RebuildAll() )
		{
			assert(false);
			return;
		}
		obj = FindByPacketId(monster_packet_id);
		if ( !obj )
		{
			//assert(false);
			return;
		}
	}
	auto monster_obj = obj->CastTo<GMonster>();
	if ( !monster_obj )
	{
		assert(false);
		return;
	}
	monster_obj->SetCurHp(cur_hp);
}

GameObjBase* GameObjMgrBase::FindByPacketId( DWORD packet_id )
{
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if (packet_id == real_obj->GetPacketId())
			{
				return real_obj.get();
			}
		}
	}
	return nullptr;
}

GameObjMgrBase::GameObjIterPtr GameObjMgrBase::TraverseDeadMonster()
{
	return FromIterImpl(MakeIterator<GameObjTraTraitsImpl>(btree_, [this]( GameObjWrapper& obj ){
		auto& obj_handler = obj.GetHandler();
		if ( !obj_handler )
		{
			assert(false);
			return false;
		}
		if ( kGOT_Monster == obj_handler->GetGameObjType() )
		{
			auto obj_monster = obj_handler->CastTo<GMonster>();
			if ( !obj_monster )
			{
				assert(false);
				return false;
			}
			if ( !IsDeadMonster(*obj_monster) )
				return false;
			auto sub_type = obj_monster->GetSubType();
			if (CD_En_ObjSubType::kXiaoGuai == sub_type || CD_En_ObjSubType::kBoss == sub_type)
				return true;
		}
		return false;
	}));
}

GameObjBase* GameObjMgrBase::FindNearestImpl_RoomNum( GameObjIterPtr& obj_ptr, GameObjBase& obj_src, int room_num )
{
	if ( !obj_ptr )
	{
		assert(false);
		return nullptr;
	}
	const auto& pos_src = obj_src.GetPos();

	struct stHelper{
		GameObjBase*		obj_ptr_;
		double				the_dist_;
	};
	std::vector<stHelper> dist_cont;
	for ( obj_ptr->First(); !obj_ptr->IsDone(); obj_ptr->Next() )
	{
		auto& obj = obj_ptr->CurItem();
		auto monster_room_num = obj.GetOwnerRoomIdx();
		if ( monster_room_num < 0 )
		{
			assert(false);
			continue;
		}
		if ( room_num >= 0 && room_num != monster_room_num )
			continue;
		
		stHelper hlp = {
			&obj, GameObjMgrBase::CalcObjDistance(obj.GetPos(), pos_src, std::abs(monster_room_num - obj_src.GetOwnerRoomIdx()))
		};
		dist_cont.push_back(hlp);
	}
	if (dist_cont.empty())
		return nullptr;
	auto it = std::min_element(dist_cont.begin(), dist_cont.end(), []( const stHelper& lhs, const stHelper& rhs ){
		return lhs.the_dist_ < rhs.the_dist_;
	});
	if ( it != dist_cont.end() )
	{
		//auto& the_pos = it->obj_ptr_->GetPos();
		return it->obj_ptr_;
	}
	assert(false);
	return nullptr;
}

GameObjBase* GameObjMgrBase::FindNearestImpl_RoomNum( GameObjTmpContT& objs, GameObjBase& obj_src, int room_num )
{
	if (objs.empty())
		return nullptr;
	const auto& pos_src = obj_src.GetPos();

	struct stHelper{
		GameObjBase*		obj_ptr_;
		double				the_dist_;
	};
	std::vector<stHelper> dist_cont;
	for ( auto& obj : objs )
	{
		if ( !obj )
		{
			assert(false);
			continue;
		}
		auto monster_room_num = obj->GetOwnerRoomIdx();
		if ( monster_room_num < 0 )
		{
			//assert(false);
			continue;
		}
		if ( room_num >= 0 && room_num != monster_room_num )
			continue;
		
		stHelper hlp = {
			obj, GameObjMgrBase::CalcObjDistance(obj->GetPos(), pos_src, std::abs(monster_room_num - obj_src.GetOwnerRoomIdx()))
		};
		assert(hlp.the_dist_ > 0);
		dist_cont.push_back(hlp);
	}
	if (dist_cont.empty())
		return nullptr;
	auto it = std::min_element(dist_cont.begin(), dist_cont.end(), []( const stHelper& lhs, const stHelper& rhs ){
		return lhs.the_dist_ < rhs.the_dist_;
	});
	if ( it != dist_cont.end() )
	{
		//auto& the_pos = it->obj_ptr_->GetPos();
		return it->obj_ptr_;
	}
	assert(false);
	return nullptr;
}

GameObjBase* GameObjMgrBase::FindNearestDeadMonster_RoomNum( GameObjBase& obj_src, int room_num )
{
	return FindNearestImpl_RoomNum(TraverseDeadMonster(), obj_src, room_num);
}

bool GameObjMgrBase::IsDeadMonsterByPacketId( DWORD packet_id )
{
	for ( auto& v : dead_monsters_ )
	{
		if ( !v )
		{
			CMN_ASSERT(false);
			continue;
		}
		if ( v->GetPacketId() == packet_id )
			return true;
	}
	return false;
}

void GameObjMgrBase::DebugInfo_Name()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "遍历所有怪物信息之总数为：" << btree_.size();
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if ( !real_obj )
		{
			assert(false);
			continue;
		}
		if ( kGOT_Monster == real_obj->GetGameObjType() )
		{
			auto monster = real_obj->CastTo<GMonster>();
			if (monster)
			{
				LOG_O(Log_info) << "遍历所有怪物信息之怪物名：" << monster->GetCnName() << " 标题：" << monster->GetTitle();
			}
		}
	}
}

GameObjBase* GameObjMgrBase::FindBeatableMonsterByName( const std::string& cn_name )
{
	if (cn_name.empty())
	{
		assert(false);
		return nullptr;
	}
	GameObjTmpContT game_objs;
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if ( kGOT_Monster == real_obj->GetGameObjType() )
			{
				auto npc_obj = real_obj->CastTo<GMonster>();
				if (npc_obj)
				{
					if ( npc_obj->GetCnName().find(cn_name) != std::string::npos )
					{
						if ( !IsDeadMonster(*npc_obj) )
							game_objs.push_back(real_obj.get());
					}
				}
			}
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	return FindNearestImpl_RoomNum(game_objs, gpm, -1);
}

GameObjBase* GameObjMgrBase::FindNearestDeadCutableMonster_RoomNum( GameObjBase& obj_src, int room_num )
{
	return FindNearestImpl_RoomNum(TraverseDeadCutableMonster(), obj_src, room_num);
}

GameObjMgrBase::GameObjIterPtr GameObjMgrBase::TraverseDeadCutableMonster()
{
	auto& collect_mgr = GCollectObjMgr::GetMe();
	return FromIterImpl(MakeIterator<GameObjTraTraitsImpl>(btree_, [&collect_mgr, this]( GameObjWrapper& obj ){
		auto& obj_handler = obj.GetHandler();
		if ( !obj_handler )
		{
			assert(false);
			return false;
		}
		if ( kGOT_Monster == obj_handler->GetGameObjType() )
		{
			auto obj_monster = obj_handler->CastTo<GMonster>();
			if ( !obj_monster )
			{
				assert(false);
				return false;
			}
			if ( !IsDeadMonster(*obj_monster) )
				return false;
			if ( !collect_mgr.FindDeadMonster(*obj_monster) )
				return false;

			auto sub_type = obj_monster->GetSubType();
			if (CD_En_ObjSubType::kXiaoGuai == sub_type || CD_En_ObjSubType::kBoss == sub_type)
				return true;
		}
		return false;
	}));
}

bool GameObjMgrBase::RebuildAllImpl()
{
	auto node = CD_FuncCall::GetLocalObjsNode();
	if (IsUnValidPtr(node))
		return false;
	SetRootNode(node);
	if ( !__super::RebuildAllImpl() )
	{
		assert(false);
		return false;
	}
	return true;
}

GameObjBase* GameObjMgrBase::FindNearestBeatableSortedMonster_RoomNum( GameObjBase& obj_src, int room_num )
{
	GameObjTmpContT game_objs_normal, game_objs_boss;
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if ( kGOT_Monster == real_obj->GetGameObjType() )
			{
				auto obj_monster = real_obj->CastTo<GMonster>();
				if (obj_monster)
				{
					if (IsDeadMonster(*obj_monster))
						continue;
					auto sub_type = obj_monster->GetSubType();
					if (CD_En_ObjSubType::kXiaoGuai == sub_type)
						game_objs_normal.push_back(real_obj.get());
					else if (CD_En_ObjSubType::kBoss == sub_type)
						game_objs_boss.push_back(real_obj.get());
				}
			}
		}
	}
	if ( !game_objs_normal.empty() )
	{
		auto the_monster = FindNearestImpl_RoomNum(game_objs_normal, obj_src, room_num);
		if (the_monster)
			return the_monster;
		if ( !game_objs_boss.empty() )
		{
			return FindNearestImpl_RoomNum(game_objs_boss, obj_src, room_num);
		}
		return nullptr;
	}
	if ( !game_objs_boss.empty() )
	{
		return FindNearestImpl_RoomNum(game_objs_boss, obj_src, room_num);
	}
	return nullptr;
}

GameObjBase* GameObjMgrBase::FindMonsterByName_Title( const std::string& cn_name, const std::string& title )
{
	if (cn_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if ( kGOT_Monster == real_obj->GetGameObjType() )
			{
				auto npc_obj = real_obj->CastTo<GMonster>();
				if (npc_obj)
				{
					if ( cn_name == npc_obj->GetCnName() )
					{
						if (title.empty())
							return real_obj.get();
						if ( npc_obj->GetTitle().find(title) != std::string::npos )
						{
							return real_obj.get();
						}
					}
				}
			}
		}
	}
	return nullptr;
}

void GameObjMgrBase::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "遍历总数为：" << btree_.size();
	for ( auto& obj : btree_ )
	{
		auto& real_obj = obj.GetHandler();
		if ( !real_obj )
		{
			assert(false);
			continue;
		}
		real_obj->DebugInfo();
	}
}

bool GameObjMgrBase::DoRebuildAll()
{
	return RebuildAll();
}

double GameObjMgrBase::CalcObjDistance( const GType::stGameVector3& pos_lhs, const GType::stGameVector3& pos_rhs, int room_distance )
{
	return pos_lhs.Distance3d_ZoomInZ(pos_rhs, 2) + room_distance * kRoomNum2DistWeight;
}

int GameObjMgrBase::GetTheDeadMonsterCnt_ByMatchCnName( const std::string& monster_cn_name ) const
{
	int cnt = 0;
	if (monster_cn_name.empty())
	{
		CMN_ASSERT(false);
		return cnt;
	}
	for ( auto& v : dead_monsters_ )
	{
		if ( !v )
		{
			CMN_ASSERT(false);
			continue;
		}
		if ( kGOT_Monster != v->GetGameObjType() )
		{
			CMN_ASSERT(false);
			continue;
		}
		auto monster_obj = v->CastTo<GMonster>();
		if ( !monster_obj )
		{
			CMN_ASSERT(false);
			continue;
		}
		if ( monster_obj->GetCnName().find(monster_cn_name) != std::string::npos )
			++cnt;
	}
	return cnt;
}

int GameObjMgrBase::GetTheDeadMonsterCnt_ByName( const std::string& monster_name ) const
{
	int cnt = 0;
	if (monster_name.empty())
	{
		CMN_ASSERT(false);
		return cnt;
	}
	for ( auto& v : dead_monsters_ )
	{
		if ( !v )
		{
			CMN_ASSERT(false);
			continue;
		}
		if ( kGOT_Monster != v->GetGameObjType() )
		{
			CMN_ASSERT(false);
			continue;
		}
		auto monster_obj = v->CastTo<GMonster>();
		if ( !monster_obj )
		{
			CMN_ASSERT(false);
			continue;
		}
		if ( monster_obj->GetName() == monster_name )
			++cnt;
	}
	return cnt;
}

void GameObjMgrBase::ClearAccetableTypes()
{
	acceptable_types_.Clear();
}

void GameObjMgrBase::AddAcceptableType(GameObjType obj_type)
{
	acceptable_types_.Add(obj_type);
}

bool GameObjMgrBase::AcceptTheType(CD_En_LocalObjType game_type, GameObjType& obj_type) const
{
	return acceptable_types_.IsIn(obj_type);
}

int GameObjMgrBase::Traverse(const DoAnyGameObjFuncT& func)
{
	int cnt = 0;
	for (auto& obj : btree_)
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if (func(*real_obj))
				++cnt;
			else
				break;
		}
	}
	return cnt;
}

void GameObjMgrBase::DebugInfo_DeadMonsters() const
{
	LOG_O(Log_trace) << "Dead Monster Cnt:" << dead_monsters_.size();
	for (auto& v : dead_monsters_)
	{
		if (!v)
		{
			assert(false);
			LOG_O(Log_trace) << v->GetName() << " room_num:" << v->GetOwnerRoomIdx();
		}
	}
}

GameObjBase* GameObjMgrBase::FindMonsterByObjDbId(int obj_db_id)
{
	if (kInvalidIntValue == obj_db_id)
	{
		assert(false);
		return nullptr;
	}
	for (auto& obj : btree_)
	{
		auto& real_obj = obj.GetHandler();
		if (real_obj)
		{
			if (kGOT_Monster == real_obj->GetGameObjType())
			{
				auto obj = real_obj->CastTo<GMonster>();
				if (obj)
				{
					if (obj_db_id == obj->GetObjDbId())
					{
						return real_obj.get();
					}
				}
			}
		}
	}
	return nullptr;
}

GameObjBase::SelfPtrT GameObjFactory::CreateGameObj( GameObjType obj_type ) const
{
	GameObjBase* game_obj = nullptr;
	switch (obj_type)
	{
	case kGOT_Invalid:
		assert(false);
		break;
	case kGOT_Npc:
		game_obj = new GNpc;
		break;
	case kGOT_Player:
		game_obj = new GPlayer;
		break;
	case kGOT_Monster:
		game_obj = new GMonster;
		break;
	case kGOT_AnyObj:
		game_obj = new GAnyObj;
		break;
	case kGOT_Door:
		game_obj = new FbDoor;
		break;
	case kGOT_Collect:
		game_obj = new GCollectObj;
		break;
	case kGOT_Static:
		game_obj = new GObjStatic;
		break;
	case kGOT_StaticBeatable:
		game_obj = new GStaticBeatableObj;
		break;
	default:
		break;
	}
	return GameObjBase::SelfPtrT(game_obj);
}

GameObjType GameObjFactory::GetObjType(CD_En_LocalObjType type) const
{
	GameObjType obj_type = kGOT_Invalid;
	switch (type)
	{
	case CD_En_LocalObjType::kNpc:
		obj_type = kGOT_Monster;
		break;
	case CD_En_LocalObjType::kPlayer1:
	case CD_En_LocalObjType::kPlayer2:
		obj_type = kGOT_Player;
		break;
	case CD_En_LocalObjType::kStatic:
		break;
	case CD_En_LocalObjType::kCollect:
		break;
	case CD_En_LocalObjType::kDoor1:
		break;
	case CD_En_LocalObjType::kDoor2:
		break;
	case CD_En_LocalObjType::kAppear:
		break;
	case CD_En_LocalObjType::kRock:
		break;
	default:
		break;
	}
	return obj_type;
}

bool GameObjWrapper::Update()
{
	if ( !handler_ )
	{
		assert(false);
		return false;
	}
	return handler_->Update();
}

GameObjBase::SelfPtrT& GameObjWrapper::GetHandler()
{
	return handler_;
}

const GameObjBase::SelfPtrT& GameObjWrapper::GetHandler() const
{
	return handler_;
}

bool GameObjWrapper::ObjValid()
{
	if (kGOT_Invalid == obj_type_)
	{
		assert(false);
		return false;
	}
	handler_ = GameObjFactory::GetMe().CreateGameObj(obj_type_);
	if (!handler_)
	{
		assert(false);
		return false;
		
	}
	handler_->SetOwner(GetOwner());
	auto cur_node = GetCurNode();
	if (cur_node)
	{
		if (!handler_->SetCurNode(cur_node))
			return false;
	}
	else
	{
		if (!local_obj_info_)
		{
			assert(false);
			return false;
		}
		if (!handler_->SetCurLocalObjInfo(local_obj_info_))
			return false;
	}
	
	return handler_->ObjValid();
}

bool GameObjWrapper::UpdateImpl()
{
	assert(false);
	return false;
}

void GameObjWrapper::DoUpdateClear()
{
	assert(false);
}

bool GameObjWrapper::SetCurLocalObjInfo(stCD_ObjBase* local_obj_info)
{
	if (!owner_)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(local_obj_info))
	{
		assert(false);
		return false;
	}
	auto tmp = local_obj_info->GetType();
	obj_type_ = GameObjFactory::GetMe().GetObjType(tmp);
	if (!owner_->AcceptTheType(tmp, obj_type_))
		return false;
	return __super::SetCurLocalObjInfo(local_obj_info);
}

GameObjWrapper::GameObjWrapper()
{
	obj_type_ = kGOT_Invalid;
}

GStaticBeatableObjMgr::GStaticBeatableObjMgr()
{
	AddTypeMap(TypePairT(CD_En_LocalObjType::kRock, kGOT_StaticBeatable));
}

GStaticBeatableObj::GStaticBeatableObj() : GameObjBase(kGOT_StaticBeatable)
{

}

void GStaticBeatableObj::DebugInfo() const
{
	LOG_O(Log_info) << "静止可打对象房间号: " << owner_room_num_
		<< "  静止可打对象名称:" << name_;
}

GameObjMgr_Role::GameObjMgr_Role()
{
	AddAcceptableType(kGOT_Player);
}

GameObjMgr::GameObjMgr()
{
	//AddAcceptableType(kGOT_Npc);
	AddAcceptableType(kGOT_Monster);
}

bool GameObjMgr_AnyObj::AcceptTheType(CD_En_LocalObjType game_type, GameObjType& obj_type) const
{
	for (auto& v : the_type_.GetValues())
	{
		if (v.first == game_type)
		{
			obj_type = v.second;
			return true;
		}
	}
	return __super::AcceptTheType(game_type, obj_type);
}

int GameObjMgr_AnyObj::LuaDoEveryObj(GameObjMgr_AnyObj& obj_mgr, const luabind::object& lua_func, int do_max_cnt)
{
	return DoEveryObj(obj_mgr, [&lua_func](const PacketIdT& packet_id){
		return CallFunction(lua_func, true, packet_id);
	}, do_max_cnt);
}

int GameObjMgr_AnyObj::DoEveryObj(GameObjMgr_AnyObj& obj_mgr, const std::function<bool(const PacketIdT&)>& do_func, int do_max_cnt)
{
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return 0;
	std::vector<std::string> objs_id;
	obj_mgr.Traverse([&objs_id](GameObjBase& obj){
		auto& name = obj.GetName();
		if (name.empty())
		{
			assert(false);
			return true;
		}
		objs_id.push_back(name);
		return true;
	});
	if (do_max_cnt <= 0)
		do_max_cnt = 50;
	int res = 0;
	ObjInfo tmp;
	tmp.obj_mgr_ = &obj_mgr;
	for (auto v : objs_id)
	{
		auto do_cnt = 0;
		tmp.obj_name_ = v;
		while (obj_mgr.FindByName(v))
		{
			if (ambient.IsGameOver())
				break;
			if (do_func(PacketIdT(tmp)))
				++res;
			else
				break;
			if (++do_cnt >= do_max_cnt)
				break;
		}
		DummySleep(500);
		if (ambient.IsGameOver())
			break;
		obj_mgr.RebuildAll();
	}
	return res;
}

void GameObjMgr_AnyObj::AddTypeMap(TypePairT type)
{
	the_type_.Add(type);
	AddAcceptableType(type.second);
}

void GameObjMgr_AnyObj::SetFilterObjName(const std::string& obj_name)
{
	obj_name_ = obj_name;
}

GameObjMgr_AnyObj::ObjInfo::ObjInfo()
{
	obj_mgr_ = nullptr;
}

bool GameObjAllLocalObjs::RebuildAllImpl()
{
	btree_.clear();
	CD_Traverse::TraverseAllLocalObjs([this](stCD_ObjBase* obj){
		TreeNodeT value_tmp;
		value_tmp.SetOwner(this);
		if (!value_tmp.SetCurLocalObjInfo(obj))
			return true;
		if (value_tmp.ObjValid() && value_tmp.Update())
			btree_.push_back(value_tmp);
		return true;
	});
	return true;
}
