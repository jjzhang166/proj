#pragma once
/*
@author zhp
@date 2014/7/1 20:44
@purpose for skills
*/
#include <string>
#include <json/value.h>
#include <Common/UsefulFunc.h>
#include <Common/GameObjBase.h>
#include "GClass.h"

//已学skill
class GSkillObj{
public:
	GSkillObj(){
		skill_obj_addr_ = nullptr;
		skill_lvl_ = 0;
		skill_id_ = 0;
	}
	void SetSkillObjAddr( void* obj_addr );
	void* GetSkillObjAddr() const;
	const std::wstring& GetStrId() const;
	int GetLvl() const;
	DWORD GetId() const;
	//bool Update();
	
	//从json解析
	
public:
	char*					skill_obj_addr_;
	std::wstring			skill_name_;
	int						skill_lvl_;
	DWORD					skill_id_;		//技能名的hash，==skill hash
};

//已学技能
class GSkillMgr : public GGameObjMgrBase{
public:
	typedef std::vector<GSkillObj> SkillContT;
	typedef TraverseTraits<GSkillObj&, GSkillObj&> SkillTraTraits;
	typedef SkillTraTraits::DoAnyTraverse DoAnySkillObjT;

	//构建与刷新游戏对象
public:
	GSkillMgr();
	/*//更新所有
	bool UpdateAll();
	//重新构建所有
	bool RebuildAll();*/

	//遍历与索引
public:
	//寻找技能对象
	GSkillObj* FindSkillObjIf( const DoAnySkillObjT& func );
	GSkillObj* FindSkillObjByStrId( const std::wstring& skill_name );
	GSkillObj* FindSkillObjById( DWORD skill_id );
	SkillContT& GetSkills();
	bool IsSkillEmpty() const;

public:
	//得到默认使用的技能
	GSkillObj* GetDefaultUseSkill();

	//自动切换可接受的技能
	bool SwtichAcceptableSkillBranch();

private:
	SkillContT				skills_;
};