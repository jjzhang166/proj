#pragma once
/*
@author zhp
@date 2017/2/5 22:34
@purpose for all game object base
*/
#include <string>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <Common/UsefulFunc.h>
#include <Common/GameObjBase.h>
#include <CmnMix/Cmn_TemplateUtility.h>
#include "GConst.h"
#include "GType.h"


class GOpenVisitor;
class GCloseVisitor;
class GameObjBase : public GGameObjBase, public boost::enable_shared_from_this<GameObjBase>{
	friend class GameObjMgrBase;
public:
	GameObjBase();
	GameObjBase(GameObjType objtype);
	
public:
	enCD_SkillId GetOpenSkillId() const{ return open_skill_id_; }
	void SetOpenSkillId(enCD_SkillId skill_id);
	void SetOwner(const GameObjMgrPtr& obj_mgr);
	GameObjMgrPtr GetOwner() const;
	inline stCD_EntityPosBased*  GetGameEntityBase() { return game_entity_; }
	inline const std::string& GetCnName() const { return ch_name_; }
	inline const stCD_VecInt* GetPos() const { if (pos_) return pos_; return &pos_backup_; }
	void ResetPos();
	inline GameObjType GetObjType() const { return obj_type_; }
	inline pt_dword GetGameId() const { /*assert(id_ > 0);*/ return id_; }
	//这个不能删
	inline const stCD_VecInt& GetBackupPos() const{ return pos_backup_; }
	//是否可攻击
	virtual bool IsHitable() const;
	//需要访问的，比如NPC
	virtual bool NeedVisitable() const;
	virtual int GetCurHp() const;
	virtual int GetCurMp() const;
	//得到能被打开的距离
	virtual int GetCanOpenedDistance() const;
	float Distance(const stCD_VecInt& pos) const;
	float Distance(const GameObjBase& rhs) const;
	virtual float WeightDistance(const stCD_VecInt& pos) const;
	virtual bool IsTransitonable() const;
	virtual bool IsTownTranstionable() const;
	virtual bool CanBreakOpenObj() const;
	template<bool IsByWeight, typename ContT>
	static void SortAssist(ContT& objs, const stCD_VecInt& pos){
		struct stHelper{
			GameObjBasePtrT		obj_;
			float				weight_;
		};
		CmnBufferVector<stHelper, 400> vec_tmp;
		if (IsByWeight)
		{
			for (auto& o : objs)
			{
				assert(o);
				vec_tmp.push_back({ o, o->WeightDistance(pos) });
			}
		}
		else
		{
			for (auto& o : objs)
			{
				assert(o);
				vec_tmp.push_back({ o, o->Distance(pos) });
			}
		}
		std::sort(vec_tmp.begin(), vec_tmp.end(), [](const stHelper& lhs, const stHelper& rhs){
			return lhs.weight_ < rhs.weight_;
		});
		objs.clear();
		for (auto& o : vec_tmp)
			objs.push_back(o.obj_);
	}

public:
	virtual bool DoOpen();
	virtual bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res);
	virtual bool CloseObject() const;
	virtual bool MovingDoTryOpen() const;
	virtual bool DoFilt() const;
	//virtual bool DoFilt(const GWorldItem& world) const;
	void DebugInfo() const override;

protected:
	void SetName(const std::string& name);
	bool UpdateImpl() override;
	bool SetGameEntity(stCD_EntityPosBased* entity);

private:
	stCD_EntityPosBased*	game_entity_;
	
private:
	std::string				ch_name_;
	GameObjType				obj_type_;
	pt_dword				id_;
	stCD_VecInt*			pos_;
	stCD_VecInt				pos_backup_;
	GameObjMgrWeakPtr		owner_;
	enCD_SkillId			open_skill_id_;
};