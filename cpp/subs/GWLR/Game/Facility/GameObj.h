#pragma once
/*
@author zhp
@date 2014/7/6 19:30
@purpose for all game object base
*/
#include <string>
#include "GClass.h"
#include <memory>
#include <boost/shared_ptr.hpp>
#include <Common/UsefulFunc.h>
#include <Common/GameObjBase.h>
#include <CmnMix/Cmn_TemplateUtility.h>
#include <CD_CommonFiles/Data/CD_Structs.h>

class GameObjBase_Base : public GClass::BTreeBaseT<GClass::GameObjMgrFilterBase>{
public:
	GameObjBase_Base();
	bool SetCurNode(const stBTreeNodeBase* game_cur_node) override;
	virtual bool SetCurLocalObjInfo(stCD_ObjBase* local_obj_info);
	stCD_ObjBase* GetObjBase() const;
	stCD_NetObj* GetNetObj() const;

protected:
	bool SetCurLocalObj(stCD_LocalObj* local_obj);

protected:
	stCD_LocalObj*				local_obj_;
	stCD_ObjBase*				local_obj_info_;
	stCD_NetObj*				net_obj_;
};
//game object base
class GameObjBase : public GameObjBase_Base, public boost::enable_shared_from_this<GameObjBase>{
	//type definitions
public:
	typedef boost::shared_ptr<GameObjBase> SelfPtrT;
	typedef boost::weak_ptr<GameObjBase>	WeakPtrT;
	typedef std::vector<GameObjBase*> GameObjsContT;

public:
	GameObjBase( GameObjType gameobj_type );
	~GameObjBase(){}
	GameObjBase(const GameObjBase&) = delete;
	GameObjBase& operator = (const GameObjBase&) = delete;
	void ResetAllUpdateData();
	//set object type
	void SetObjType( GameObjType obj_type );
	GameObjType GetGameObjType() const;
	const stCD_PosDirInfo* GetPosInfo() const;
	virtual GType::stGameVector3 GetPos() const;
	virtual GType::stGameVector3 GetDirLhs() const;
	virtual GType::stGameVector3 GetDirRhs() const;
	DWORD GetPacketId() const;
	DWORD GetLocalId() const;
	bool SetPos(const stCD_Vector3& pos);
	//用来确保坐标设置成功，但这个函数最好是在自己的线程里使用，而不要在游戏的线程中使用
	bool SetPosEnsure(const stCD_Vector3& pos);
	//设置方向
	bool SetDir( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs );
	bool SetDirEnsure( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs );
	bool SetDirAsync(const stCD_PosDirInfo* pos_info);
	bool SetDirSync(const stCD_PosDirInfo* pos_info);
	bool IsPackageIdValid() const;
	const std::string& GetName() const;
	int GetOwnerRoomIdx() const;
	boost::optional<CD_En_ObjStatus> GetObjStatus() const;
	//向下转换
	template<typename DerivedT>
	DerivedT* CastTo(){
		return PointerCast<DerivedT>(this);
	}	

public:
	virtual bool UpdateOwnerRoomNum();
	virtual bool UpdatePacketId();

protected:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	//debug
	void DebugInfo() const override;

	//static
public:

protected:
	mutable GameObjType			obj_type_;

	//游戏变量
protected:
	DWORD						obj_id_;
	std::string					name_;
	DWORD						packet_id_;
	int							owner_room_num_;	//所属房间序号

protected:
	stCD_PosDirInfo*			dir_pos_info_;
	GType::stGameVector3*		pos_ptr_;
	const CD_En_ObjStatus*		obj_status_;
};

class GNormalGameObjMgrBase{
public:
	virtual ~GNormalGameObjMgrBase(){}
	virtual GameObjBase* FindByPacketId( DWORD packet_id );
	virtual GameObjBase* FindByName( const std::string& name );
	virtual bool DoRebuildAll() = 0;
	virtual bool IsDeadMonsterByPacketId( DWORD packet_id );
};

class GAnyObj : public GameObjBase{
public:
	GAnyObj();
	void DebugInfo() const override;
	bool UpdateOwnerRoomNum() override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;
};

class GObjDbInfo : public GGameObjBase{
public:
	GObjDbInfo();

protected:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	stCD_ObjDbInfo*	obj_db_info_;

public:
	int				obj_db_id_;
	std::string		ch_name_;
	CD_En_ObjSubType	type_;
	std::string		en_name_;
	std::string		title_;
};