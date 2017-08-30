#include "stdafx.h"
#include "GameObjMgr.h"
#include <Common/LogOut.h>
#include "GAddr.h"
#include "GOffset.h"
#include "GMiscObj.h"
#include "GNpc.h"
#include "GPlayer.h"
#include "GUnknown.h"
#include "GMonster.h"

bool GameObjMgr::UpdateAll()
{
	assert(false);
	if (game_objs_.empty())
		return false;
	for ( auto& obj : game_objs_ )
	{
		if ( !obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "game obj is zero";
			assert(false);
			continue;
		}
		if ( !obj->Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "obj update failed";
			assert(false);
			return false;
		}
	}
	return true;
}

bool GameObjMgr::RebuildUpdateAll()
{
	using namespace GAddr;
	using namespace GOffset;
	if ( !g_pLocalObjAddr )
	{
		assert(false);
		return false;
	}
	if (force_update_)
	{
		force_update_ = false;
	}
	else
	{
		return timer_sync_exec_.TimerGWndExecSync([this](){
			return DoUpdateImpl();
		});
	}

	return GWndExecSync([this](){
		return DoUpdateImpl();
	});
}

GameObjMgr::GameObjContT& GameObjMgr::GetAllGameObjs()
{
	return game_objs_;
}

std::size_t GameObjMgr::GetGameObjCntByType( GameObjType gameobj_type )
{
	std::size_t cnt = 0;
	for ( const auto& game_obj : game_objs_ )
	{
		if ( gameobj_type == game_obj->GetGameObjType() )
			++cnt;
	}
	return cnt;
}

std::size_t GameObjMgr::GetGameObjCntByTypeBut( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter )
{
	std::size_t cnt = 0;
	for ( auto& game_obj : game_objs_ )
	{
		if ( game_obj && gameobj_type == game_obj->GetGameObjType() )
		{
			if ( !except_filter(game_obj) )
				++cnt;
		}
	}
	return cnt;
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseGameObjByType( GameObjType gameobj_type )
{
	return TraverseIf([gameobj_type]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType();
	});
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseIf( const DoAnyGameObjFuncT& func )
{
	return MakeIterator<GameObjTraTraits>(game_objs_, func);
}

GameObjBase::SelfPtrT GameObjMgr::FindIf( const DoAnyGameObjFuncT& func )
{
	for ( auto& gameobj : game_objs_ )
	{
		if ( gameobj && func(gameobj) )
			return gameobj;
	}
	return GameObjBase::SelfPtrT();
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseGameObjByTypeBut( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter )
{
	return TraverseIf([gameobj_type, except_filter]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType() && !except_filter(game_obj);
	});
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseGameObjByTypeLimit( GameObjType gameobj_type, std::size_t max_cnt )
{
	return FindGameObjLimit([gameobj_type]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType();
	}, max_cnt);
}

GameObjMgr::GameObjIterPtr GameObjMgr::FindGameObjLimit( const DoAnyGameObjFuncT& func, std::size_t max_valid_cnt )
{
	return MakeIterator<GameObjTraTraits>(game_objs_, func, max_valid_cnt);
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseGameObjByTypeBut_Limit( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter, std::size_t max_cnt )
{
	return FindGameObjLimit([gameobj_type, except_filter]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType() && !except_filter(game_obj);
	}, max_cnt);
}

void GameObjMgr::DoItemByType( GameObjType gameobj_type, const DoEveryGameObjFuncT& func )
{
	for ( auto& game_obj : game_objs_ )
	{
		if ( game_obj && gameobj_type == game_obj->GetGameObjType() )
			func(game_obj);
	}
}

void GameObjMgr::DoItemByTypeLimit( GameObjType gameobj_type, const DoEveryGameObjFuncT& func, std::size_t max_cnt )
{
	assert(max_cnt > 0);
	std::size_t cnt = 0;
	for ( auto& game_obj : game_objs_ )
	{
		if ( game_obj && gameobj_type == game_obj->GetGameObjType() )
		{
			func(game_obj);
			if ( ++cnt >= max_cnt )
				return;
		}
	}
}

void GameObjMgr::DoItemUntilByType( GameObjType gameobj_type, const DoAnyGameObjFuncT& func )
{
	for ( auto& game_obj : game_objs_ )
	{
		if ( game_obj && gameobj_type == game_obj->GetGameObjType() )
		{
			if (func(game_obj))
				return;
		}
	}
}

GameObjMgr::GameObjMgr()
{
	force_update_ = false;
	obj_parser_ = nullptr;
}

bool GameObjMgr::IsGameObjsEmpty() const
{
	return game_objs_.empty();
}

std::size_t GameObjMgr::GetAllGameObjCnt() const
{
	return game_objs_.size();
}

void GameObjMgr::OnGameObjDestroy( void* gameobj_addr )
{
	if ( !gameobj_addr )
	{
		assert(false);
		return;
	}
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "游戏对象销毁了,addr->" << gameobj_addr;*/
	auto& gameobj = FindGameObjByAddr(gameobj_addr);
	if ( !gameobj )
	{
		//assert(false);
		return;
	}
	DeleteGameObj(gameobj);
}

GameObjBase::SelfPtrT GameObjMgr::FindGameObjByAddr( void* gameobj_addr )
{
	if ( !gameobj_addr )
	{
		assert(false);
		return GameObjBase::SelfPtrT();
	}
	return FindIf([gameobj_addr]( GameObjBase::SelfPtrT& gameobj ){
		return gameobj->GetGameObjAddr() == gameobj_addr;
	});
}

void GameObjMgr::DeleteGameObj( GameObjBase::SelfPtrT& gameobj )
{
	if ( !gameobj )
	{
		assert(false);
		return;
	}
	auto it = std::find_if(game_objs_.begin(), game_objs_.end(), [&gameobj]( GameObjBase::SelfPtrT& game_o ){
		return game_o == gameobj;
	});
	if ( it != game_objs_.end() )
		game_objs_.erase(it);
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseAliveGameObj( GameObjType gameobj_type )
{
	return TraverseIf([gameobj_type]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType() && game_obj->GetCurHp() > 0;
	});
}

GameObjMgr::GameObjIterPtr GameObjMgr::TraverseAliveGameObjLimit( GameObjType gameobj_type, std::size_t max_cnt )
{
	return FindGameObjLimit([gameobj_type]( GameObjBase::SelfPtrT& game_obj ){
		return gameobj_type == game_obj->GetGameObjType() && game_obj->GetCurHp() > 0;
	}, max_cnt);
}

GameObjBase::SelfPtrT GameObjMgr::FindFirstShopNpc()
{
	using namespace GType;
	auto& shop_mgr = GShopTemplageMgr::GetMe();
	if ( !shop_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return GameObjBase::SelfPtrT();
	}
	const auto& npc_obj = TraverseGameObjByType(kGOT_Npc);
	for ( npc_obj->First(); !npc_obj->IsDone(); npc_obj->Next() )
	{
		auto& cur_npc = npc_obj->CurItem();
		assert(cur_npc);
		auto shop_template = shop_mgr.FindById(cur_npc->GetObjName());
		if (shop_template)
			return cur_npc;
	}
	return  GameObjBase::SelfPtrT();
}

GameObjBase::SelfPtrT GameObjMgr::FindByName( const std::wstring& obj_name )
{
	for ( auto& gameobj : game_objs_ )
	{
		if ( gameobj && obj_name == gameobj->GetObjName() )
			return gameobj;
	}
	return GameObjBase::SelfPtrT();
}

bool GameObjMgr::DoUpdateImpl()
{
	using namespace GAddr;
	using namespace GOffset;
	using namespace GClass;
	GGameObjTraverse** first_game_obj = nullptr;
	if ( !MemOperApplyR(first_game_obj, (int)g_pLocalObjAddr, g_o_GainCreatureListInRoom, 0) || !first_game_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read fist game obj failed";
		assert(false);
		return false;
	}
	GameObj_Parser gameobj_tmp;
	GameObj_Parser* obj_parser;
	if (obj_parser_)
		obj_parser = obj_parser_;
	else
		obj_parser = &gameobj_tmp;
	auto& gameobj_factory = GameObjFactory::GetMe();
	GameObjContT gameobjs_tmp;
	for ( auto cur_game_obj = (const GGameObjTraverse*)first_game_obj; *first_game_obj != cur_game_obj; cur_game_obj->NextGameObj(&cur_game_obj) )
	{
		if ( !cur_game_obj )
		{
			assert(false);
			return false;
		}
		void* real_obj_addr;
		if ( !MemOperApplyR(real_obj_addr, (int)cur_game_obj, g_o_CreatureInRoom) )
		{
			/*LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "MemOperApplyR某个game object失败";*/
			//assert(false);
			continue;
		}
		if ( !real_obj_addr )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "MemOperApplyR某个game object，得到的real_obj_addr为0";
			//assert(false);
			continue;
		}

		obj_parser->SetGameObjAddr(real_obj_addr);
		if ( !obj_parser->Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "临时游戏对象更新失败";
			assert(false);
			return false;
		}
		if ( !obj_parser->ObjValid() )
		{
			/*LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "对象无效，所以忽略该对象，对象ID->" << gameobj_tmp.GetObjName();*/
			continue;
		}
		auto& gameobj_new = gameobj_factory.CreateGameObj(obj_parser->GetGameObjType());
		if ( !gameobj_new )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "创建新的游戏对象失败";
			assert(false);
			return false;
		}
		gameobj_new->SetGameObjAddr(real_obj_addr);
		if ( !gameobj_new->Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新新的游戏对象失败";
			assert(false);
			return false;
		}
		gameobjs_tmp.push_back(gameobj_new);
	}
	gameobjs_tmp.swap(game_objs_);
	return true;
}

void GameObjMgr::ResetForceUpdate()
{
	force_update_ = true;
}

GameObjBase::SelfPtrT GameObjMgr::FindBySerial( DWORD obj_serial )
{
	for ( auto& gameobj : game_objs_ )
	{
		if ( obj_serial == gameobj->GetObjSerial() )
			return gameobj;
	}
	return GameObjBase::SelfPtrT();
}

void GameObjMgr::ResetNpcBlacklist()
{
	npc_blacklist_.clear();
}

void GameObjMgr::AddNpcBlacklist( std::wstring&& obj_id )
{
	if (obj_id.empty())
	{
		assert(false);
		return;
	}
	npc_blacklist_.insert(obj_id);
}

bool GameObjMgr::ExistTheBlacklist( const std::wstring& obj_id ) const
{
	assert(!obj_id.empty());
	return npc_blacklist_.find(obj_id) != npc_blacklist_.end();
}

void GameObjMgr::SetGameObjParser( GameObj_Parser* game_obj_parser )
{
	obj_parser_ = game_obj_parser;
}

GameObjBase::SelfPtrT GameObjMgr::FindFirstByJob( const std::wstring& obj_job )
{
	if (obj_job.empty())
	{
		assert(false);
		return GameObjBase::SelfPtrT();
	}
	for ( auto& gameobj : game_objs_ )
	{
		if ( gameobj && obj_job == gameobj->GetJobName() )
			return gameobj;
	}
	return GameObjBase::SelfPtrT();
}

GameObjBase::SelfPtrT GameObjFactory::CreateGameObj( GameObjType obj_type )
{
	GameObjBase* game_obj = nullptr;
	switch ( obj_type )
	{
	case kGOT_MISC:
		{
			game_obj = new GObjMisc;
			break;
		}
	case kGOT_Npc:
		{
			game_obj = new GNpc;
			break;
		}
	case kGOT_Player:
		{
			game_obj = new GPlayer;
			break;
		}
	case kGOT_Unknown:
		{
			game_obj = new GUnknown;
			break;
		}
	case kGOT_Monster:
		{
			game_obj = new GMonster;
			break;
		}
	case kGOT_ParseObjType:
		{
			game_obj = new GameObj_Parser;
			break;
		}
	default:
		break;
	}
	return GameObjBase::SelfPtrT(game_obj);
}
