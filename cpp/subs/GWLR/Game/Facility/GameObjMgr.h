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

//factory
class GameObjFactory : public Singleton<GameObjFactory, Singleton_MakeMe>{
public:
	~GameObjFactory();
	GameObjType GetObjType(CD_En_LocalObjType type) const;
	GameObjBase::SelfPtrT CreateGameObj(GameObjType obj_type) const;
};

class GameObjWrapper : public GameObjBase_Base{
public:
	GameObjWrapper();
	GameObjBase::SelfPtrT& GetHandler();
	const GameObjBase::SelfPtrT& GetHandler() const;
	bool ObjValid() override;
	bool Update() override;
	bool SetCurLocalObjInfo(stCD_ObjBase* local_obj_info) override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

private:
	GameObjBase::SelfPtrT	handler_;
	GameObjType				obj_type_;
};

class GameObjMgrBase : public GClass::BTreeMgr<std::vector<GameObjWrapper>, GClass::GameObjMgrFilterBase>,
	public GNormalGameObjMgrBase{
private:
	typedef GClass::BTreeMgr<std::vector<GameObjWrapper>, GClass::GameObjMgrFilterBase>::ContT GameObjContT;
	typedef TraverseTraits<GameObjWrapper&, GameObjWrapper&> GameObjTraTraitsImpl;
	typedef GameObjTraTraitsImpl::IterItemT GameObjIterItemImpl;
	typedef GameObjTraTraitsImpl::IteratorBasePtr GameObjIterPtrImpl;
	typedef IteratorImpl<GameObjTraTraitsImpl, GameObjContT> GameObjIteratorImpl;
	typedef std::vector<GameObjBase*> GameObjTmpContT;

	//type definitions
public:
	typedef TraverseTraits<GameObjBase&, GameObjBase&> GameObjTraTraits;
	typedef GameObjTraTraits::IterItemT	GameObjIterItem;
	typedef GameObjTraTraits::IteratorBasePtr GameObjIterPtr;
	typedef GameObjTraTraits::DoEveryTraverse DoEveryGameObjFuncT;
	typedef GameObjTraTraits::DoAnyTraverse DoAnyGameObjFuncT;
	
public:
	GameObjMgrBase();

	//构建与刷新游戏对象
public:
	//更新所有game object
	bool UpdateAll() override;
	//重新构建所有
	bool DoRebuildAll() override;
	//更新怪物血值
	void UpdateMonsterHp( DWORD monster_packet_id, int cur_hp );
	//清空可接受类型
	void ClearAccetableTypes();
	//添加可接受类型
	void AddAcceptableType(GameObjType obj_type);

private:
	bool RebuildAllImpl() override;

protected:
	bool AcceptTheType(CD_En_LocalObjType game_type, GameObjType& obj_type) const override;

	//游戏对象的索引与迭代
public:
	//////////////////////////////////////////////////////////////////////////
	GameObjIterPtr TraverseAll();
	//////////////////////////////////////////////////////////////////////////
	bool IsGameObjsEmpty() const;
	std::size_t GetAllGameObjCnt() const;
	//查找第一个
	GameObjBase* FindIf( const DoAnyGameObjFuncT& func );
	GameObjBase* FindByName(const std::string& cn_name) override;
	GameObjBase* FindMonsterByName_Title( const std::string& cn_name, const std::string& title );
	GameObjBase* FindBeatableMonsterByName( const std::string& cn_name );
	GameObjBase* FindByPacketId(DWORD packet_id) override;
	GameObjBase* FindMonsterByObjDbId(int obj_db_id);
	int Traverse(const DoAnyGameObjFuncT& func);
	//遍历可打的怪物
	GameObjIterPtr TraverseBeatableMonster();
	GameObjIterPtr TraverseDeadMonster();
	GameObjIterPtr TraverseDeadCutableMonster();
	//计算距离
	static double CalcObjDistance( const GType::stGameVector3& pos_lhs, const GType::stGameVector3& pos_rhs, int room_distance );

public:
	GameObjBase* FindNearestBeatableMonster_RoomNum( GameObjBase& obj_src, int room_num );
	//boss最后打
	GameObjBase* FindNearestBeatableSortedMonster_RoomNum( GameObjBase& obj_src, int room_num );
	GameObjBase* FindNearestDeadMonster_RoomNum( GameObjBase& obj_src, int room_num );
	//可剥皮的对象
	GameObjBase* FindNearestDeadCutableMonster_RoomNum( GameObjBase& obj_src, int room_num );
	//////////////////////////////////////////////////////////////////////////
	//dead monsters
public:
	void ClearDeadMonsters();
	void AddDeadMonster( DWORD packet_id );
	bool IsDeadMonster( GameObjBase& obj );
	bool IsDeadMonsterByPacketId(DWORD packet_id) override;
	int GetTheDeadMonsterCnt_ByMatchCnName( const std::string& monster_cn_name ) const;
	int GetTheDeadMonsterCnt_ByName( const std::string& monster_name ) const;

	//test
public:
	void TraverseAllDebugInfo();
	//////////////////////////////////////////////////////////////////////////
	void DebugInfo_Name();
	void DebugInfo() const override;
	void DebugInfo_DeadMonsters() const;
	//////////////////////////////////////////////////////////////////////////
private:
	GameObjBase* FindNearestImpl_RoomNum( GameObjIterPtr& obj_ptr, GameObjBase& obj_src, int room_num );
	GameObjBase* FindNearestImpl_RoomNum( GameObjTmpContT& objs, GameObjBase& obj_src, int room_num );

	//static
public:

private:
	static GameObjIterPtr FromIterImpl( GameObjIterPtrImpl& iter_impl );
	//////////////////////////////////////////////////////////////////////////
private:
	std::set<GameObjBase::SelfPtrT>			dead_monsters_;
	NameList<GameObjType, std::vector<GameObjType> >	acceptable_types_;
};

//npc、monster
class GameObjMgr : public GameObjMgrBase, public Singleton<GameObjMgr, Singleton_MakeMe>{
public:
	GameObjMgr();
};

class GameObjMgr_Role : public GameObjMgrBase, public Singleton<GameObjMgr_Role, Singleton_MakeMe>{
public:
	GameObjMgr_Role();
};

//all local objs
class GameObjAllLocalObjs : public GameObjMgrBase{
private:
	bool RebuildAllImpl() override;
};

class GameObjMgr_AnyObj : public GameObjAllLocalObjs{
public:
	struct ObjInfo{
		std::string			obj_name_;
		GameObjMgr_AnyObj*	obj_mgr_;

		ObjInfo();
	};

	typedef AtomicValue<ObjInfo> PacketIdT;
	typedef std::pair<CD_En_LocalObjType, GameObjType> TypePairT;

public:
	void AddTypeMap(TypePairT type);
	void SetFilterObjName(const std::string& obj_name);

public:
	static int DoEveryObj(GameObjMgr_AnyObj& obj_mgr, const std::function<bool(const PacketIdT&)>& do_func, int do_max_cnt);
	static int LuaDoEveryObj(GameObjMgr_AnyObj& obj_mgr, const luabind::object& lua_func, int do_max_cnt);

private:
	bool AcceptTheType(CD_En_LocalObjType game_type, GameObjType& obj_type) const override;

public:
	std::string			obj_name_;

private:
	NameList<TypePairT, std::vector<TypePairT> >	the_type_;
};

class GStaticBeatableObj : public GameObjBase{
public:
	GStaticBeatableObj();
	//bool Update();
	void DebugInfo() const override;
};

//静态可打的对象，比如莉莉的风格这个任务需要打的石头
class GStaticBeatableObjMgr : public GameObjMgr_AnyObj, public Singleton<GStaticBeatableObjMgr, Singleton_MakeMe>{
public:
	GStaticBeatableObjMgr();
};
