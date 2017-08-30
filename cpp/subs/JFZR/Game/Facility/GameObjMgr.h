#pragma once
/*
@author zhp
@date 2014/7/6 21:36
@purpose manage game objects
*/
#include <Common/UsefulClass.h>
#include "GameObj.h"
#include <list>
#include <Common/Iterator.h>
#include <boost/shared_ptr.hpp>
#include <Common/GameObjBase.h>

class GameObjMgr : public GGameObjMgrBase, public Singleton<GameObjMgr, Singleton_MakeMe>{
	//type definitions
public:
	typedef std::list<GameObjBase::SelfPtrT> GameObjContT;
	typedef TraverseTraits<GameObjBase::SelfPtrT, GameObjBase::SelfPtrT&> GameObjTraTraits;
	typedef GameObjTraTraits::IteratorBase GameObjIterBase;
	typedef GameObjTraTraits::IteratorBasePtr GameObjIterPtr;
	typedef GameObjTraTraits::DoEveryTraverse DoEveryGameObjFuncT;
	typedef GameObjTraTraits::DoAnyTraverse DoAnyGameObjFuncT;
	typedef IteratorImpl<GameObjTraTraits, GameObjContT> GameObjIteratorImpl;
	typedef IteratorCounterImpl<GameObjTraTraits, GameObjContT> GameObjIterCounter;
	
public:
	GameObjMgr();

	//构建与刷新游戏对象
public:
	//更新所有game object
	bool UpdateAll();
	//重新构建所有
	bool RebuildUpdateAll();
	void ResetForceUpdate();
	//游戏对象销毁
	void OnGameObjDestroy( void* gameobj_addr );
	//删除游戏对象
	void DeleteGameObj( GameObjBase::SelfPtrT& gameobj );
	void SetGameObjParser( GameObj_Parser* game_obj_parser );

	//游戏对象的索引与迭代
public:
	//得到所有游戏对象
	GameObjContT& GetAllGameObjs();
	bool IsGameObjsEmpty() const;
	std::size_t GetAllGameObjCnt() const;
	//得到某种类型的游戏对象数量
	std::size_t GetGameObjCntByType( GameObjType gameobj_type );
	//得到某种类型的游戏对象数量，除了
	std::size_t GetGameObjCntByTypeBut( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter );
	//遍历某种类型的游戏对象
	GameObjIterPtr TraverseGameObjByType( GameObjType gameobj_type );
	//遍历某种类型的游戏对象除了
	GameObjIterPtr TraverseGameObjByTypeBut( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter );
	//遍历某种类型的游戏对象，最多数量
	GameObjIterPtr TraverseGameObjByTypeLimit( GameObjType gameobj_type, std::size_t max_cnt );
	//遍历除了某些条件的某种类型的游戏对象，最多数量
	GameObjIterPtr TraverseGameObjByTypeBut_Limit( GameObjType gameobj_type, const DoAnyGameObjFuncT& except_filter, std::size_t max_cnt );
	//根据类型做事
	void DoItemByType( GameObjType gameobj_type, const DoEveryGameObjFuncT& func );
	void DoItemByTypeLimit( GameObjType gameobj_type, const DoEveryGameObjFuncT& func, std::size_t max_cnt );
	void DoItemUntilByType( GameObjType gameobj_type, const DoAnyGameObjFuncT& func );
	//遍历
	GameObjIterPtr TraverseIf( const DoAnyGameObjFuncT& func );
	//查找第一个
	GameObjBase::SelfPtrT FindIf( const DoAnyGameObjFuncT& func );
	GameObjIterPtr FindGameObjLimit( const DoAnyGameObjFuncT& func, std::size_t max_valid_cnt );
	GameObjBase::SelfPtrT FindGameObjByAddr( void* gameobj_addr );
	GameObjBase::SelfPtrT FindByName( const std::wstring& obj_name );
	GameObjBase::SelfPtrT FindBySerial( DWORD obj_serial );
	GameObjBase::SelfPtrT FindFirstByJob( const std::wstring& obj_job );
	//未死亡的游戏对象
	GameObjIterPtr TraverseAliveGameObj( GameObjType gameobj_type );
	GameObjIterPtr TraverseAliveGameObjLimit( GameObjType gameobj_type, std::size_t max_cnt );

public:
	//寻找一个本地图的商品NPC
	GameObjBase::SelfPtrT FindFirstShopNpc();

	//遍历黑名单
public:
	void ResetNpcBlacklist();
	void AddNpcBlacklist( std::wstring&& obj_id );
	bool ExistTheBlacklist( const std::wstring& obj_id ) const;

private:
	bool DoUpdateImpl();

private:
	GameObjContT				game_objs_;
	GClass::TimerSyncExec		timer_sync_exec_;
	bool						force_update_;
	std::set<std::wstring>		npc_blacklist_;
	GameObj_Parser*				obj_parser_;
};

//factory
class GameObjFactory : public Singleton<GameObjFactory, Singleton_MakeMe>{
public:
	~GameObjFactory();
	GameObjBase::SelfPtrT CreateGameObj( GameObjType obj_type );
};