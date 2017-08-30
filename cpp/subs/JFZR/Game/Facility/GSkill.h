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
	bool Update();
	//输出到josn
	bool ConvertToJson( Json::Value& out_json ) const{
		out_json["name"] = WideCharToGbk(skill_name_);
		out_json["lvl"] = (Json::UInt)skill_lvl_;
		out_json["id"] = (Json::UInt)skill_id_;
		return true;
	}
	bool ParseFromJson( const Json::Value& json_v ){
		const auto& json_name = json_v["name"];
		if ( json_name.isNull() || !json_name.isString() )
		{
			assert(false);
			return false;
		}
		const auto& json_lvl = json_v["lvl"];
		if ( json_lvl.isNull() || !json_lvl.isIntegral() )
		{
			assert(false);
			return false;
		}
		const auto& json_id = json_v["id"];
		if ( json_id.isNull() || !json_id.isIntegral() )
		{
			assert(false);
			return false;
		}
		skill_name_ = GbkToWideChar(json_name.asString());
		skill_lvl_ = json_lvl.asUInt();
		skill_id_ = json_id.asUInt();
		return true;
	}
	//从json解析
	
public:
	char*					skill_obj_addr_;
	std::wstring			skill_name_;
	int						skill_lvl_;
	DWORD					skill_id_;		//技能名的hash，==skill hash
};

//可接受使用的技能分支
class AcceptableUseSkillBranch{
public:
	typedef std::map<std::wstring, DWORD> SkillId_HashMapT;
	typedef SkillId_HashMapT::value_type SkillId_HashPairT;

public:
	virtual const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const;
	const SkillId_HashMapT& GetSkillIdHashMap() const;
	virtual const SkillId_HashPairT* GetBasicSkillPair() const = 0;

protected:
	void AddSkillId_Hash( const std::wstring& skill_id, const std::wstring& real_skill_id );

private:
	SkillId_HashMapT			skillid_hash_map_;
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
	//更新所有
	bool UpdateAll();
	//重新构建所有
	bool RebuildUpdateAll();

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
	const AcceptableUseSkillBranch::SkillId_HashPairT* GetDefaultSkillIdHashPair();
	const AcceptableUseSkillBranch::SkillId_HashPairT* GetSkillIdHashPair( const GSkillObj& skill_obj );
	//自动切换可接受的技能
	bool SwtichAcceptableSkillBranch();

private:
	SkillContT				skills_;
	const AcceptableUseSkillBranch*			acceptable_skill_;
};

//技能模板
class GSkillTemplate : public GClass::BTreeBase{
public:
	class SkillStep : public GClass::GGameTypeBase{
	public:
		struct stSdbSkillStep{
			DWORD					str_dwVFunAddr;
			int					str_dwLevel;				// 技能等级
			int					str_dwMinLevelToLearn;		// 学习技能需要的角色等级
			int					str_dwSpToLearn;			// 学习技能需要的sp点
			int					str_dwGoldToLearn;			// 学习技能所需金币
			int					str_dwAwakeningPointToLearn;// 学习技能所需觉醒点 
			int					str_dwTierAttackPowerToLearn;	// 10.30不删档新增字段
			const GClass::stGameString			str_theActionId;
			FLOAT					str_fConsumeHp;				// 消耗hp
			FLOAT					str_fConsumeMp;				// 消耗mp
			FLOAT					str_fConsumeRage;			// 消耗愤怒
			FLOAT					str_fConsumeBloodPoint;		// 消耗血值
			FLOAT					str_fAvailablePeriod;		// 使用时间
			FLOAT					str_fCooldown;				// 冷却时间
			FLOAT					str_fReusingInterval;		// 重用间隔
			FLOAT					str_dwAttackPowerRate;		// 攻击速度，封包伤害攻击需要用到
			GClass::GGameTArrayPlaceholder		str_theActionIdList;
			GClass::BTreeMgrBase::stGBTreeNodes				str_theDescParams;			// 攻击力和攻击次数
			GClass::BTreeMgrBase::stGBTreeNodes				str_theParams;
			GClass::GGameTArrayPlaceholder		str_theIncreaseStatus;		// 增加状态
			const GClass::stGameString				str_theOverrideActionId;
			GClass::GGameTArrayPlaceholder		str_theEventOptionIds;
			GClass::GGameTArrayPlaceholder		str_theReduceCooldowns;
			DWORD								str_dwIncreaseStatusByEquipCategory;
			const GClass::stGameStringEx		str_theUnKnow1;
			DWORD								str_dwUnKnow1[3];
		};
		static_assert(sizeof(stSdbSkillStep) == sizeof(SKILL_INFO_PER_LEVEL), "SKILL_INFO_PER_LEVEL");
		typedef stSdbSkillStep GameType;

	public:
		SkillStep();
		bool Update();
		int GetLvl() const;
		int GetMinLvlToLearn() const;
		int GetSpToLearn() const;
		float GetAttackPowerRate() const;

	private:
		int					skill_lvl_;
		int					min_lvl_to_learn_;
		int					sp_to_learn_;
		float				attack_power_rate_;
	};
	class SkillStepTArray : public GClass::GGameTArray<std::vector<SkillStep> >{};

public:
	GSkillTemplate();
	bool Update();
	const SkillStepTArray& GetSkillSteps() const;
	DWORD GetSkillId() const;
	const std::wstring& GetSkillStrId() const;
	const SkillStep* FindStepByLvl( int lvl ) const;

private:
	DWORD				skill_id_;
	std::wstring		str_skill_id_;
	std::wstring		skill_name_;
	SkillStepTArray		skill_steps_;
};

//所有技能
class GSkillTemplateMgr : public GClass::BTreeMgr_SkipRoot<GSkillTemplate>, public Singleton<GSkillTemplateMgr, Singleton_MakeMe>{
public:
	bool RebuildUpdateAll();
	bool UpdateAll();
	const GSkillTemplate* FindById( DWORD skill_id ) const;
	const GSkillTemplate* FindByStrId( const std::wstring& skill_id ) const;
};

//游戏class info
class GClassInfoTemplate : public GClass::BTreeBase{
public:
	class SkillBranch : public GClass::BTreeBase{
	public:
		class SkillInfo : public GClass::BTreeBase{
		public:
			SkillInfo();
			bool Update();
			const std::wstring& GetSkillId() const;
			int GetSkillMaxLvl() const;

		private:
			std::wstring				skill_id_;
			int							skill_max_level_;			//最大等级
		};
		class BranchSkillsMgr : public GClass::BTreeMgr_SkipRoot<SkillInfo>{
		public:
			const SkillInfo* FindSkillInfoById( const std::wstring& skill_id ) const;
		};

	public:
		SkillBranch();
		bool Update();
		bool BuildBranchJobType();
		const std::wstring& GetBranchId() const;
		const std::wstring& GetBranchNameIdx() const;
		const BranchSkillsMgr& GetBranchSkills() const;
		GType::enJobCategory GetBranchJobType() const;

	private:
		std::wstring				branch_id_;
		GType::enJobCategory		branch_job_type_;			//分支隶属职业
		std::wstring				branch_name_idx_;			//分支名称的索引
		BranchSkillsMgr				branch_skills_mgr_;
	};
	class SkillBranchsMgr : public GClass::BTreeMgr_SkipRoot<SkillBranch>{
	public:
		const SkillBranch* FindByBranchJobType( GType::enJobCategory job ) const;
		const SkillBranch::SkillInfo* FindSkillInfoById( const std::wstring& skill_id ) const;
	};
	//可穿戴的装备类别
	class EquipCategory : public GClass::GGameTypeBase{
	public:
		typedef GClass::stGameString GameType;

	public:
		bool Update();
		const std::wstring& GetEquipCategory() const;

	private:
		std::wstring			equip_category_;
	};
	class EquipCategories : public GClass::GGameTArray<std::vector<EquipCategory> >{
	public:
		const EquipCategory* FindEquipCategory( const std::wstring& equip_category ) const;
	};

public:
	GClassInfoTemplate();
	bool Update();
	const std::wstring& GetJobId() const;
	GType::enJobCategory GetJob() const;
	bool IsCreatableJob() const;
	const std::wstring& GetParentJobId() const;
	GType::enJobCategory GetParentJob() const;
	const std::wstring& GetJobClassName() const;
	const SkillBranchsMgr& GetSkillBranchs() const;
	SkillBranchsMgr& GetSkillBranchs();
	bool CanEquipTheCategory( const std::wstring& equip_category ) const;

private:
	std::wstring			job_cls_id_;
	GType::enJobCategory	job_;
	GType::enJobCategory	parent_job_;
	bool					creatable_job_;			//是否可创建职业
	std::wstring			parent_job_cls_id_;		//job_id_是从parent_job_id_转过来的
	std::wstring			job_class_name_;		//可以得到职业对象的中文名
	SkillBranchsMgr			skill_branch_mgr_;
	EquipCategories			support_equip_categories_;
};

class GClassInfoTemplateMgr : public GClass::BTreeMgr_SkipRoot<GClassInfoTemplate>, public Singleton<GClassInfoTemplateMgr, Singleton_MakeMe>{
	bool RebuildUpdateAll();
	bool UpdateAll();

public:
	const GClassInfoTemplate* FindByJob( GType::enJobCategory job ) const;
	bool BuildBranchsJobType();
};

//剑斗士
class SkillAcceptable_JDS : public AcceptableUseSkillBranch{
public:
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const;
	const SkillId_HashPairT* GetBasicSkillPair() const;
};
class SkillAcceptable_JDS_Impl : public SkillAcceptable_JDS, public Singleton<SkillAcceptable_JDS_Impl, Singleton_MakeMe>{
};
//刃武者
class SkillAcceptable_RWZ : public AcceptableUseSkillBranch{
public:
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const;
	const SkillId_HashPairT* GetBasicSkillPair() const;
};
class SkillAcceptable_RWZ_Impl : public SkillAcceptable_RWZ, public Singleton<SkillAcceptable_RWZ_Impl, Singleton_MakeMe>{
};
//控魔师
class SkillAcceptable_KMS : public AcceptableUseSkillBranch{
public:
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const;
	const SkillId_HashPairT* GetBasicSkillPair() const;
};
class SkillAcceptable_KMS_Impl : public SkillAcceptable_KMS, public Singleton<SkillAcceptable_KMS_Impl, Singleton_MakeMe>{
};

//斩狂
class SkillAcceptable_ZK : public SkillAcceptable_JDS, public Singleton<SkillAcceptable_ZK, Singleton_MakeMe>{
public:
	SkillAcceptable_ZK();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//拳刹
class SkillAcceptable_QS : public SkillAcceptable_JDS, public Singleton<SkillAcceptable_QS, Singleton_MakeMe>{
public:
	SkillAcceptable_QS();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//剑魔
class SkillAcceptable_JM : public SkillAcceptable_JDS, public Singleton<SkillAcceptable_JM, Singleton_MakeMe>{
public:
	SkillAcceptable_JM();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//风舞星
class SkillAcceptable_FWX : public SkillAcceptable_RWZ, public Singleton<SkillAcceptable_FWX, Singleton_MakeMe>{
public:
	SkillAcceptable_FWX();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//天狼星
class SkillAcceptable_TLX : public SkillAcceptable_RWZ, public Singleton<SkillAcceptable_TLX, Singleton_MakeMe>{
public:
	SkillAcceptable_TLX();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//猎刃星
class SkillAcceptable_LRX : public SkillAcceptable_RWZ, public Singleton<SkillAcceptable_LRX, Singleton_MakeMe>{
public:
	SkillAcceptable_LRX();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//魔影师
class SkillAcceptable_MYS : public SkillAcceptable_KMS, public Singleton<SkillAcceptable_MYS, Singleton_MakeMe>{
public:
	SkillAcceptable_MYS();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//霜语者
class SkillAcceptable_SYZ : public SkillAcceptable_KMS, public Singleton<SkillAcceptable_SYZ, Singleton_MakeMe>{
public:
	SkillAcceptable_SYZ();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};
//时空术士
class SkillAcceptable_SKSS : public SkillAcceptable_KMS, public Singleton<SkillAcceptable_SKSS, Singleton_MakeMe>{
public:
	SkillAcceptable_SKSS();
	const SkillId_HashPairT* FindSkillIdHashPair( const std::wstring& skill_id ) const{
		return AcceptableUseSkillBranch::FindSkillIdHashPair(skill_id);
	}
};

//可接受技能工厂
class SkillAcceptableFactory : public Singleton<SkillAcceptableFactory, Singleton_Me>{
public:
	const AcceptableUseSkillBranch* FindBranchByJob( GType::enJobCategory job ) const;

private:
	static SkillAcceptableFactory		static_factory_;
};