#pragma once
/*
@author zhp
@date 2017/2/5 22:34
@purpose manage game objects
*/
#include <Common/UsefulClass.h>
#include "GameObj.h"
#include <list>
#include <Common/Iterator.h>
#include <boost/shared_ptr.hpp>
#include <Common/GameObjBase.h>
#include "GObjFilter.h"

class GameObjMgrBase : public GGameObjMgrBase, public boost::enable_shared_from_this<GameObjMgrBase>{
public:
	typedef std::vector<GameObjBasePtrT> ObjContT;

public:
	~GameObjMgrBase();
	GameObjMgrBase& operator += (const GameObjMgrBase& rhs);

public:
	GameObjBasePtrT FindById(pt_dword id) const;
	GameObjBasePtrT FindByName(const std::string& name) const;
	GameObjBasePtrT FindObjByFilter(const IFilterGameObj& filter) const;
	GameObjBasePtrT FindObjInCircle(const stCD_VecInt& center, int radius) const;
	GameObjBasePtrT FindObjInCircleByType(GameObjType obj_type, const stCD_VecInt& center, int radius) const;
	bool HasObjInCircle(const stCD_VecInt& center, int radius, int count) const;
	void GetInCircleObjs(const stCD_VecInt& center, int radius, int count, GameObjsBufferCont& out_objs);
	int GetObjCntByName(const std::string& name) const;
	GameObjBasePtrT GetFirstObj() const;
	const ObjContT& GetObjs() const;
	bool empty() const;
	void clear();
	void SortByPos(const stCD_VecInt& pos);
	//排序最好都用这个
	void SortByWeightDistance(const stCD_VecInt& pos);
	void ResetAll();
	void DebugInfo() const override;

public:
	virtual FilterGameObjCont& GetFilter() const = 0;
	bool FilterObj(const GameObjBasePtrT& obj) const;

protected:
	bool RebuildAllImpl() override;

protected:
	virtual GameObjTypeThinCont FilterGameObjType(const GameObjTypeThinCont& types) const;

private:
	bool CreateObjsAtTra(GBuffMgr& buff_mgr, stCD_EntityPosBased* entity);
	GameObjBase* CreateGameObj(GameObjType type) const;
	GameObjTypeThinCont CalcObjNodeType(stCD_EntityPosBased* entity) const;

private:
	ObjContT		objs_;
};

//只能是new出来的
class GameObjMgr : public GameObjMgrBase{
public:
	GameObjMgr() = default;
	GameObjMgr(const GameObjMgrBase& rhs);
	GameObjMgr(const GameObjMgr& rhs) = default;
	template<typename... Args>
	GameObjMgr(GameObjType type, const Args... types) : filter_type_(type, types...){}
	template<typename... Args>
	void AddFilterType(const Args... types){
		filter_type_.Add(types...);
	}
	void ClearFilterType();
	void IncludeNoHumanFilter();
	void IncludeOpenableFilter();
	void IncludeLifedFilter();
	FilterGameObjCont& GetFilter() const override;

protected:
	GameObjTypeThinCont FilterGameObjType(const GameObjTypeThinCont& types) const override;

private:
	GFilterGameObjType				filter_type_;
	mutable FilterGameObjCont		filters_;
};

class GameObjMgrCont : public GameObjMgrBase{
public:
	GameObjMgrCont() = default;
	GameObjMgrCont(const GameObjMgrBase& rhs);
	GameObjMgrCont& operator = (const GameObjMgrBase& rhs);
	GameObjMgrCont(const GameObjMgrCont& rhs) = default;
	GameObjMgrCont& operator = (const GameObjMgrCont& rhs) = default;

private:
	FilterGameObjCont& GetFilter() const override;
};
