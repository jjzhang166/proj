#include "stdafx.h"
#include "GSkill.h"
#include "GProtocolS.h"
#include "GClass.h"
#include "GOffset.h"
#include <Common/LogOut.h>
#include <Common/MemOper.h>
#include "GTypes.h"
#include "GAddr.h"
#include "GStrMap.h"
#include "GPlayer.h"
#include "GConst.h"

using namespace GClass;

void GSkillObj::SetSkillObjAddr( void* obj_addr )
{
	skill_obj_addr_ = (char*)obj_addr;
}

void* GSkillObj::GetSkillObjAddr() const
{
	return skill_obj_addr_;
}

bool GSkillObj::Update()
{
	using namespace GOffset;
	using namespace GClass;
	if ( !skill_obj_addr_ )
	{
		assert(false);
		return false;
	}
	stGameString* game_str_tmp = (stGameString*)(skill_obj_addr_ + g_o_SkillName);
	assert(game_str_tmp);
	if ( !game_str_tmp->CopyTo(skill_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read obj job name error";
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(skill_lvl_, (int)skill_obj_addr_, g_o_SkillLevel) )
	{
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(skill_id_, (int)skill_obj_addr_, g_o_SkillId) )
	{
		assert(false);
		return false;
	}

	return true;
}

const std::wstring& GSkillObj::GetStrId() const
{
	return skill_name_;
}

int GSkillObj::GetLvl() const
{
	return skill_lvl_;
}

DWORD GSkillObj::GetId() const
{
	return skill_id_;
}

bool GSkillMgr::UpdateAll()
{
	if (skills_.empty())
		return false;
	for ( auto& skill_obj : skills_ )
	{
		if ( !skill_obj.Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "obj update failed";
			assert(false);
			return false;
		}
	}
	return true;
}

bool GSkillMgr::RebuildUpdateAll()
{
	using namespace GType;
	using namespace GAddr;
	using namespace GOffset;
	/*GameSkillObjPtr* game_skill_obj = nullptr;
	if ( !MemOperApplyR(game_skill_obj, g_pRoleObjAddr, g_o_SkillOffset1 + g_o_SkillOffset2) || !game_skill_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取所有技能基址失败";
		assert(false);
		return false;
	}*/
	DWORD cmp_flag = 0;
	auto game_skill_obj = (GameSkillObjPtr*)GetSkillObjAddr(g_pRoleObjAddr + g_o_SkillOffset1 + g_o_SkillOffset2, &cmp_flag);
	assert(game_skill_obj);
	if ( !game_skill_obj->skill_begin_addr_ || !game_skill_obj->skill_end_addr_ )
	{
		assert(false);
		return false;
	}
	if ( game_skill_obj->skill_end_addr_ < game_skill_obj->skill_begin_addr_ )
	{
		assert(false);
		return false;
	}
	auto skill_begin_tmp = game_skill_obj->skill_begin_addr_;
	assert(((char*)game_skill_obj->skill_end_addr_ - (char*)skill_begin_tmp) % sizeof(GameSkillObjT) == 0);
	SkillContT skills_tmp;
	GSkillObj skill_obj_tmp;
	while ( skill_begin_tmp < game_skill_obj->skill_end_addr_ )
	{
		skill_obj_tmp.SetSkillObjAddr(skill_begin_tmp);
		if ( !skill_obj_tmp.Update() )
		{
			assert(false);
			return false;
		}
		skills_tmp.push_back(skill_obj_tmp);
		++skill_begin_tmp;
	}
	std::swap(skills_tmp, skills_);
	return true;
}

GSkillObj* GSkillMgr::FindSkillObjIf( const DoAnySkillObjT& func )
{
	for ( auto& skillobj : skills_ )
	{
		if (func(skillobj))
			return &skillobj;
	}
	return nullptr;
}

GSkillObj* GSkillMgr::FindSkillObjByStrId( const std::wstring& skill_name )
{
	return FindSkillObjIf([&skill_name]( GSkillObj& skillobj ){
		return skill_name == skillobj.GetStrId();
	});
}

GSkillObj* GSkillMgr::FindSkillObjById( DWORD skill_id )
{
	return FindSkillObjIf([skill_id]( GSkillObj& skillobj ){
		return skill_id == skillobj.GetId();
	});
}

GSkillMgr::SkillContT& GSkillMgr::GetSkills()
{
	return skills_;
}

bool GSkillMgr::IsSkillEmpty() const
{
	return skills_.empty();
}

GSkillObj* GSkillMgr::GetDefaultUseSkill()
{
	/*if (skills_.empty())
		return nullptr;
	auto it_max = std::max_element(skills_.begin(), skills_.end(), []( GSkillObj& lhs, GSkillObj& rhs ){
		return lhs.GetLvl() < rhs.GetLvl();
	});
	if ( it_max == skills_.end() )
	{
		assert(false);
		return nullptr;
	}
	return &*it_max;*/

	const auto kSkillSize = skills_.size();
	if ( kSkillSize <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "技能为空";
		return nullptr;
	}
	auto& gps = GPlayerSelf::GetMe();
	auto& skill_fac = SkillAcceptableFactory::GetMe();
	auto main_job = gps.GetMainJob();
	auto main_skill_branch = skill_fac.FindBranchByJob(main_job);
	if ( !main_skill_branch )
	{
		assert(false);
		return nullptr;
	}
	auto basic_skill = main_skill_branch->GetBasicSkillPair();
	if ( !basic_skill )
	{
		assert(false);
		return nullptr;
	}
	for ( auto& skill_obj : skills_ )
	{
		if ( GetSkillIdHashPair(skill_obj) || basic_skill->first == skill_obj.GetStrId() )
			return &skill_obj;
	}
	return nullptr;
	//assert(false);

	auto skill_idx = std::min(1, (int)kSkillSize - 1);
	return &skills_[skill_idx];
}

GSkillMgr::GSkillMgr()
{
	acceptable_skill_ = nullptr;
}

bool GSkillMgr::SwtichAcceptableSkillBranch()
{
	using namespace GType;
	auto& gps = GPlayerSelf::GetMe();
	auto job = gps.GetJob();
	if ( job > kJC_Any )
	{
		if ( !gps.Update() )
		{
			assert(false);
			return false;
		}
		job = gps.GetJob();
		if ( job > kJC_Any )
		{
			assert(false);
			return false;
		}
	}
	acceptable_skill_ = SkillAcceptableFactory::GetMe().FindBranchByJob(job);
	if ( !acceptable_skill_ )
	{
		assert(false);
		return false;
	}
	return true;
}

const AcceptableUseSkillBranch::SkillId_HashPairT* GSkillMgr::GetDefaultSkillIdHashPair()
{
	auto dlt_skill = GetDefaultUseSkill();
	if ( !dlt_skill )
	{
		assert(false);
		return nullptr;
	}
	return GetSkillIdHashPair(*dlt_skill);
}

const AcceptableUseSkillBranch::SkillId_HashPairT* GSkillMgr::GetSkillIdHashPair( const GSkillObj& skill_obj )
{
	if ( !acceptable_skill_ )
	{
		assert(false);
		return nullptr;
	}
	return acceptable_skill_->FindSkillIdHashPair(skill_obj.GetStrId());
}

bool GSkillTemplateMgr::RebuildUpdateAll()
{
	using namespace GClass;
	using namespace GAddr;
	using namespace GOffset;
	const auto nodes = (const BTreeMgrBase::stGBTreeNodes*)g_pSkillTemplate;
	if ( nodes->node_cnt_ == btree_.size() )
		return true;
	SetGameNodes(nodes);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	assert(!btree_.empty());
	return true;
}

bool GSkillTemplateMgr::UpdateAll()
{
	if ( !btree_.empty() )
		return true;
	return __super::UpdateAll();
}

const GSkillTemplate* GSkillTemplateMgr::FindById( DWORD skill_id ) const
{
	for ( auto& skill : btree_ )
	{
		if ( skill.GetSkillId() == skill_id )
			return &skill;
	}
	return nullptr;
}

const GSkillTemplate* GSkillTemplateMgr::FindByStrId( const std::wstring& skill_id ) const
{
	for ( auto& skill : btree_ )
	{
		if ( skill.GetSkillStrId() == skill_id )
			return &skill;
	}
	return nullptr;
}

struct stSdbSkillInfo{
	const GClass::stGameStringEx	skill_id_;
	const GClass::stGameStringEx	skill_name_;
	DWORD							str_dwNeedTarget;			
	DWORD							str_dwMaxRangeOnTarget;		
	const GClass::stGameStringEx	str_theDesc;
	DWORD							str_dwElementalAttribute;
	DWORD							str_dwTargetTypes;
	DWORD							str_dwSkillType;
	DWORD							str_dwTargetInfo[6];
	GSkillTemplate::SkillStepTArray::stGameTArray	skill_info_per_lvl_;			//技能各等级详细信息
	GClass::GGameTArrayPlaceholder	str_thePrecedingSkills;
	GClass::GGameTArrayPlaceholder	str_theSecondaryAwakeningSkills;
	BYTE							str_bEnableAutoTarget;
	BYTE							str_OneClickUse;
	WORD							str_wUnKnowFlag;
	FLOAT							str_fAutoTargetDistanceRangeX;
	FLOAT							str_fAutoTargetDistanceRangeY;
	FLOAT							str_fAutoTargetAngleRangeX;
	FLOAT							str_fAutoTargetAngleRangeY;
	BYTE					str_bAutoTargetingIsOnlyFirstAction;
	BYTE					str_bEnableAOEAutoTarget;	
	WORD					str_wZeor2;
	FLOAT					str_fAOEAutoTargetDistanceRangeX;
	FLOAT					str_fAOEAutoTargetDistanceRangeY;
	FLOAT					str_fAOEAutoTargetAngleRangeX;
	FLOAT					str_fAOEAutoTargetAngleRangeY;
	GAME_STRING_ID			str_theIconPath;
	DWORD					str_dwZeor1;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwEnable;
		GAME_STRING_ID		str_theScriptFuncName;		// 脚本函数的名字
	}theSkillActivateCondition;
	struct
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theUnKnow;
	}theUnKnow1;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwEnable;				// 启用
		DWORD				str_dwLevel;
		DWORD				str_dwRadius;				// 半径范围
		DWORD				str_dwMaxRange;				// 最大范围
		DWORD				str_dwCheckSectorLimit;
		GAME_STRING_VF_ID	str_theAoeVisualId;			// 效果范围视觉id
	}theAoeInfo;
	GAME_STRUCT_ARRAY_POINT	str_theOverrideAoeInfos;	// 覆盖效果范围信息
	DWORD					str_dwEnableSkillUseWhenCooldown;		// 使技能冷却时使用
	GAME_STRING_ID			str_theCommandTag;						// 命令标签
	BYTE					str_bIsDeadlySkill;						// 是否为致命技能
	DWORD					str_dwDeadlySkillFullGaugeSpending;
	GAME_STRING_ID			str_theDeadlySkillId;
	BYTE					str_bAffectedByAwaken;
	BYTE					str_bIsAwakeningSkill;
	BYTE					str_bBeAwakenToLearn;
	BYTE					str_bIsGuildSkill;
	GAME_STRING_ID_			str_theMoviePath;
	struct  
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theInstructionDesc;
	}theInstructionDesc;
	GAME_STRUCT_ARRAY_POINT	str_theIncompatibleSkills;
	DWORD					str_dwPvpPrecooldown;
	DWORD					str_dwPvpAttackPowerRate;
};
static_assert(sizeof(stSdbSkillInfo) == sizeof(SDB_SKILL), "");

struct stSkillTemplate : GSkillTemplate::stBTreeNodeBase{
	const DWORD			skill_id_;
	const stSdbSkillInfo*	skill_obj_addr_;
	const void*			skill_unknown_obj_addr_;
	const DWORD			unknown_flag_;
};
static_assert(sizeof(stSkillTemplate) == sizeof(SDB_INDEX), "");

bool GSkillTemplate::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto skill_template = (const stSkillTemplate*)game_cur_node_;
	if ( skill_template->skill_id_ <= 0 )
	{
		assert(false);
		return false;
	}
	skill_id_ = skill_template->skill_id_;
	const auto skill_info = skill_template->skill_obj_addr_;
	if ( !skill_info )
	{
		assert(false);
		return false;
	}
	if ( !skill_info->skill_id_.game_str_.CopyTo(str_skill_id_) )
	{
		assert(false);
		return false;
	}
	if ( !skill_info->skill_name_.game_str_.CopyTo(skill_name_) )
	{
		assert(false);
		return false;
	}
	skill_steps_.SetGameTArray(&skill_info->skill_info_per_lvl_);
	if ( !skill_steps_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

GSkillTemplate::GSkillTemplate()
{
	skill_id_ = 0;
}

const GSkillTemplate::SkillStepTArray& GSkillTemplate::GetSkillSteps() const
{
	return skill_steps_;
}

DWORD GSkillTemplate::GetSkillId() const
{
	return skill_id_;
}

const std::wstring& GSkillTemplate::GetSkillStrId() const
{
	return str_skill_id_;
}

const GSkillTemplate::SkillStep* GSkillTemplate::FindStepByLvl( int lvl ) const
{
	if ( lvl <= 0 )
	{
		assert(false);
		return nullptr;
	}
	const SkillStep* cur_step = nullptr;
	for ( const auto& skill_step : skill_steps_.GetCont() )
	{
		if ( skill_step.GetLvl() > lvl )
			break;
		cur_step = &skill_step;
	}
	return cur_step;
}

GSkillTemplate::SkillStep::SkillStep()
{
	skill_lvl_ = 0;
	min_lvl_to_learn_ = 0;
	sp_to_learn_ = 0;
	attack_power_rate_ = 0.3f;
}

bool GSkillTemplate::SkillStep::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
		return false;
	}
	const auto skill_step = (const stSdbSkillStep*)cur_obj_addr_;
	if ( skill_step->str_dwLevel < 0 )
	{
		assert(false);
		return false;
	}
	skill_lvl_ = skill_step->str_dwLevel;
	if ( skill_step->str_dwMinLevelToLearn <= 0 )
	{
		assert(false);
		return false;
	}
	min_lvl_to_learn_ = skill_step->str_dwMinLevelToLearn;
	if ( skill_step->str_dwSpToLearn < 0 )
	{
		assert(false);
		return false;
	}
	sp_to_learn_ = skill_step->str_dwSpToLearn;
	attack_power_rate_ = skill_step->str_dwAttackPowerRate;
	//assert(attack_power_rate_ > 0.0f);
	return true;
}

int GSkillTemplate::SkillStep::GetLvl() const
{
	return skill_lvl_;
}

int GSkillTemplate::SkillStep::GetMinLvlToLearn() const
{
	return min_lvl_to_learn_;
}

int GSkillTemplate::SkillStep::GetSpToLearn() const
{
	return sp_to_learn_;
}

float GSkillTemplate::SkillStep::GetAttackPowerRate() const
{
	return attack_power_rate_;
}

void AcceptableUseSkillBranch::AddSkillId_Hash( const std::wstring& skill_id, const std::wstring& real_skill_id )
{
	using namespace GFuncCall;
	assert(!skill_id.empty());
	assert(!real_skill_id.empty());
	assert(skillid_hash_map_.find(skill_id) == skillid_hash_map_.end());
	auto real_hash = CalcHash_DWORD(real_skill_id);
	assert(real_hash);
	for ( const auto& v : skillid_hash_map_ )
	{
		if ( v.second == real_hash )
		{
			assert(false);
			return;
		}
	}
	skillid_hash_map_[skill_id] = real_hash;
}

const AcceptableUseSkillBranch::SkillId_HashPairT* AcceptableUseSkillBranch::FindSkillIdHashPair( const std::wstring& skill_id ) const
{
	auto it = skillid_hash_map_.find(skill_id);
	if ( it == skillid_hash_map_.end() )
	{
		auto basic_skill = GetBasicSkillPair();
		if ( !basic_skill )
		{
			assert(false);
			return nullptr;
		}
		if ( basic_skill->first == skill_id )
			return basic_skill;
		return nullptr;
	}
	return &*it;
}

const AcceptableUseSkillBranch::SkillId_HashMapT& AcceptableUseSkillBranch::GetSkillIdHashMap() const
{
	return skillid_hash_map_;
}

SkillAcceptable_ZK::SkillAcceptable_ZK()
{
	/*AddSkillId_Hash(L"DoubleSlash", L"DoubleSlash_B");
	AddSkillId_Hash(L"ShoveStrike", L"ShoveStrike_Fix");*/
	AddSkillId_Hash(L"ChargeHack", L"ChargeHack_A");
}

SkillAcceptable_QS::SkillAcceptable_QS()
{
	AddSkillId_Hash(L"HandShot", L"HandShot");
	AddSkillId_Hash(L"TekkenPunch", L"TekkenPunch");
}

SkillAcceptable_JM::SkillAcceptable_JM()
{
	/*AddSkillId_Hash(L"BeatingSoul", L"BeatingSoul_SetActivateSpecialAttack");
	AddSkillId_Hash(L"XSlash", L"WarriorXSlash1");*/
	AddSkillId_Hash(L"Horizon", L"Horizon");
}

SkillAcceptable_FWX::SkillAcceptable_FWX()
{
	AddSkillId_Hash(L"HalfMoonKick", L"HalfMoonKick1");
	AddSkillId_Hash(L"5Slash", L"5SlashEnd");
}

SkillAcceptable_TLX::SkillAcceptable_TLX()
{
	AddSkillId_Hash(L"LightningSlash", L"Wolf_LightningSlash");
	/*AddSkillId_Hash(L"WolfDownAttack", L"WolfDownAttack1");*/
}

SkillAcceptable_LRX::SkillAcceptable_LRX()
{
	AddSkillId_Hash(L"UpperClaw", L"UpperClaw");
	/*AddSkillId_Hash(L"ShadowAttack", L"ShadowAttack");*/
}

SkillAcceptable_MYS::SkillAcceptable_MYS()
{
	AddSkillId_Hash(L"Stinger", L"Stinger_Bullet_Long");
	/*AddSkillId_Hash(L"ShadowStorm", L"ShadowStorm");*/
}

SkillAcceptable_SYZ::SkillAcceptable_SYZ()
{
	AddSkillId_Hash(L"IcePick", L"IcePick");
	AddSkillId_Hash(L"CrystalShower", L"CrystalShower");
}

SkillAcceptable_SKSS::SkillAcceptable_SKSS()
{
	/*AddSkillId_Hash(L"SmallMeteor", L"SmallMeteor_FallDown");
	AddSkillId_Hash(L"TimeBomb", L"TimeBomb_Bullet");*/
	AddSkillId_Hash(L"SmallMeteor", L"SmallMeteor_Explosion");
}

const SkillAcceptable_JDS::SkillId_HashPairT* SkillAcceptable_JDS::FindSkillIdHashPair( const std::wstring& skill_id ) const
{
	auto skill_hash_pair = SkillAcceptable_ZK::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_QS::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_JM::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	return nullptr;
}

const SkillAcceptable_JDS::SkillId_HashPairT* SkillAcceptable_JDS::GetBasicSkillPair() const
{
	using namespace GFuncCall;
	const wchar_t* const hash_id_ptr = L"HandShot";
	static SkillId_HashPairT hash_pair(std::wstring(hash_id_ptr), CalcHash_DWORD(hash_id_ptr));
	return &hash_pair;
}

const SkillAcceptable_RWZ::SkillId_HashPairT* SkillAcceptable_RWZ::FindSkillIdHashPair( const std::wstring& skill_id ) const
{
	auto skill_hash_pair = SkillAcceptable_FWX::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_TLX::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_LRX::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	return nullptr;
}

const SkillAcceptable_RWZ::SkillId_HashPairT* SkillAcceptable_RWZ::GetBasicSkillPair() const
{
	using namespace GFuncCall;
	const wchar_t* const hash_id_ptr = L"HalfMoonKick";
	static SkillId_HashPairT hash_pair(std::wstring(hash_id_ptr), CalcHash_DWORD(hash_id_ptr));
	return &hash_pair;
}

const SkillAcceptable_KMS::SkillId_HashPairT* SkillAcceptable_KMS::FindSkillIdHashPair( const std::wstring& skill_id ) const
{
	auto skill_hash_pair = SkillAcceptable_MYS::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_SYZ::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	skill_hash_pair = SkillAcceptable_SKSS::GetMe().FindSkillIdHashPair(skill_id);
	if (skill_hash_pair)
		return skill_hash_pair;
	return nullptr;
}

const SkillAcceptable_KMS::SkillId_HashPairT* SkillAcceptable_KMS::GetBasicSkillPair() const
{
	using namespace GFuncCall;
	const wchar_t* const hash_id_ptr = L"IcePick";
	static SkillId_HashPairT hash_pair(std::wstring(hash_id_ptr), CalcHash_DWORD(hash_id_ptr));
	return &hash_pair;
}

SkillAcceptableFactory SkillAcceptableFactory::static_factory_;

const AcceptableUseSkillBranch* SkillAcceptableFactory::FindBranchByJob( GType::enJobCategory job ) const
{
	using namespace GType;
	switch (job)
	{
	case GType::kJC_JianDouShi:
		return &SkillAcceptable_JDS_Impl::GetMe();
	case GType::kJC_RenWuZhe:
		return &SkillAcceptable_RWZ_Impl::GetMe();
	case GType::kJC_ZhanKuang:
		return &SkillAcceptable_ZK::GetMe();
	case GType::kJC_QuanSha:
		return &SkillAcceptable_QS::GetMe();
	case GType::kJC_FengWuXing:
		return &SkillAcceptable_FWX::GetMe();
	case GType::kJC_LieRenXing:
		return &SkillAcceptable_LRX::GetMe();
	case GType::kJC_TianLangXing:
		return &SkillAcceptable_TLX::GetMe();
	case GType::kJC_JianMo:
		return &SkillAcceptable_JM::GetMe();
	case GType::kJC_KongMoShi:
		return &SkillAcceptable_KMS_Impl::GetMe();
	case GType::kJC_MoYingShi:
		return &SkillAcceptable_MYS::GetMe();
	case GType::kJC_ShiKongShuShi:
		return &SkillAcceptable_SKSS::GetMe();
	case GType::kJC_ShuangYuZhe:
		return &SkillAcceptable_SYZ::GetMe();
	default:
		assert(false);
		return nullptr;
	}
}

bool GClassInfoTemplateMgr::RebuildUpdateAll()
{
	using namespace GAddr;
	if ( !btree_.empty() )
		return true;
	const auto nodes = (const BTreeMgrBase::stGBTreeNodes*)g_pClassInfoTemplate;
	SetGameNodes(nodes);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	if (btree_.empty())
	{
		assert(false);
		return false;
	}
	if ( btree_.size() != GClass::GJobClassTemplateMgr::GetMe().GetCont().size() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GClassInfoTemplateMgr::UpdateAll()
{
	if ( !btree_.empty() )
		return true;
	return __super::UpdateAll();
}

const GClassInfoTemplate* GClassInfoTemplateMgr::FindByJob( GType::enJobCategory job ) const
{
	for ( const auto& v : btree_ )
	{
		if ( v.GetJob() == job )
			return &v;
	}
	assert(false);
	return nullptr;
}

bool GClassInfoTemplateMgr::BuildBranchsJobType()
{
	for ( auto& branch_template : btree_ )
	{
		auto& branch_skills = branch_template.GetSkillBranchs().GetAllNodes();
		for ( auto& branch_skill : branch_skills )
		{
			/*if ( !branch_skill.BuildBranchJobType() )
			{
				assert(false);
				return false;
			}*/
			branch_skill.BuildBranchJobType();
		}
	}
	return true;
}

static_assert(sizeof(GClass::stGameStringEx) == sizeof(GAME_STRING_VF_ID), "");
static_assert(sizeof(GClass::stGameString) == sizeof(GAME_STRING_ID), "");
static_assert(sizeof(GClass::stGameString_4) == sizeof(GAME_STRING_ID_), "");
static_assert(sizeof(GClass::BTreeMgrBase::stGBTreeNodes) == sizeof(BINARY_TREE_MAP), "");

struct stSdbClsInfo{
	DWORD					str_dwVFunAddr;
	GClass::stGameString			str_theId;				// 各职业分支id
	GAME_STRING_ID			str_theApDisplayType;
	GClass::stGameStringEx		str_theClassName;		// 中英文映射
	GAME_STRING_ID_			str_theModelPath;
	GAME_STRING_ID_			str_theVillageSimplifiedModelPath;
	GAME_STRING_ID_			str_theStageSimplifiedModelPath;
	XYZ						str_theScale;
	bool					str_theIsCreateableClass;		// 可创建角色职业 = 1 ，不可创建角色职业 = 0
	FLOAT					str_fJumpHeight;
	FLOAT					str_fJumpGravity;
	struct  
	{
		DWORD				str_dwVFunAddr;
		DWORD				str_dwType;
		FLOAT				str_fRadius;
		FLOAT				str_fHeight;
		XYZ					str_theBoxSize;
	}theCollisionShape;
	GAME_STRING_ID			str_theSkinType;		// +F0
	GClass::stGameString			str_theActionListId;				// 职业类型id
	GAME_STRING_ID			str_theStageDefaultActionId;		// 副本默认状态
	GAME_STRING_ID			str_theVillageDefaultActionId;		// 城镇默认状态
	GAME_STRING_ID			str_theLevelUpActionId;
	GAME_STRING_ID			str_theClassChangeActionId;			// +17C
	GAME_STRING_ID			str_theClassSelectSoundEventId;
	GAME_STRING_ID			str_theClassCreateSoundEventId;
	BINARY_TREE_MAP			str_theDefaultStatus;						// +1D0	 默认角色状态
	GClassInfoTemplate::EquipCategories::stGameTArray	str_theEquipCategories;						// +1E0  装备类型
	FLOAT					str_fSpecialAttackTargetingAngleRange[2];
	GClass::BTreeMgrBase::stGBTreeNodes			str_theSkillCategories;						// +1F8  职业分支
	BINARY_TREE_MAP			str_theSpecialAttackSettings;				// +208  特殊攻击设置
	GAME_STRUCT_ARRAY_POINT	str_theEffects;
	GAME_STRUCT_ARRAY_POINT	str_theDeadlySkillReadyEffects;
	BINARY_TREE_MAP			str_theDefaultMeshInfos;		// +238
	GAME_STRUCT_ARRAY_POINT	str_theDefaultBuffInfos;
	GAME_STRUCT_ARRAY_POINT	str_theDefaultSkills;			// +258  默认技能
	GAME_STRUCT_ARRAY_POINT	str_theAwakenDefaultSkills;		// +268  默认觉醒技能
	BINARY_TREE_MAP			str_theDefaultEquipment;		// +278	 默认装备
	BINARY_TREE_MAP			str_theDefaultQuickSlot;		// +288  默认快捷栏技能
	GAME_STRING_ID			str_theDefaultQuest;			// +298	 默认任务id	
	GAME_STRING_ID			str_theDefaultStage;			// +2B0	 默认副本id
	struct  
	{
		DWORD				str_dwVFunAddr;
		struct  
		{
			DWORD			str_dwVFunAddr;
			GAME_STRING_ID	str_theBrief;
		}theBrief;
		GAME_STRING_ID		str_theDifficulty;
	}theDescription;		// +2D0
	GAME_STRING_ID			str_theStartModeForStage;					// +310		副本模式为攻击模式
	GAME_STRING_ID			str_theStartModeForVillage;					// +32C		城镇模式为和平模式
	GAME_STRUCT_ARRAY_POINT	str_theRandomModeChange;
	GAME_STRING_ID_			str_theCriticalHitSoundPath;
	GAME_STRUCT_ARRAY_POINT	str_theOverrideActions;
	struct
	{
		DWORD				str_dwVFunAddr;
		BINARY_TREE_MAP		str_theCustomizingPartInfos;
		GAME_STRUCT_ARRAY_POINT	str_theSelectableSkinColors;
	}theCustomizingInfo;												// +388
	struct
	{
		DWORD				str_dwVFunAddr;
		BINARY_TREE_MAP		str_theCustomizingPartDatas;
		DWORD				str_dwSelectedSkinColorIndex;
	}theCustomizingData;												// +3AC
	FLOAT					str_fWeaponDurabilityDecreaseRate;
	FLOAT					str_fArmorDurabilityDecreaseRate;
	FLOAT					str_fAccessoryDurabilityDecreaseRate;
	struct
	{
		DWORD				str_dwVFunAddr;
		BYTE				str_bUseHpInsteadOfMp;
		GAME_STRUCT_ARRAY_POINT		str_theAddBuffs;
		GAME_STRUCT_ARRAY_POINT		str_theRemoveBuffs;
		GAME_STRUCT_ARRAY_POINT		str_theBuffListToBeErased;
	}theClassAwakenInfo;		// +3D0
	struct  
	{
		DWORD				str_dwVFunAddr;
		GAME_STRING_ID		str_theGaugeType;
		DWORD				str_dwGaugeMaxValue;
	}theSpecialGaugeInfo;		// +408
};
static_assert(sizeof(stSdbClsInfo) == sizeof(SDB_CLASS_INFO), "");

bool GClassInfoTemplate::Update()
{
	typedef GClass::SdbIndex<stSdbClsInfo> stSdbIndex;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto sdb_idx = (const stSdbIndex*)game_cur_node_;
	auto cls_info = sdb_idx->real_obj_;
	if ( !cls_info )
	{
		assert(false);
		return false;
	}
	if ( !cls_info->str_theId.CopyTo(job_cls_id_) )
	{
		assert(false);
		return false;
	}
	assert(0 == (int)cls_info->str_theIsCreateableClass || 1 == (int)cls_info->str_theIsCreateableClass);
	creatable_job_ = cls_info->str_theIsCreateableClass;
	if ( !cls_info->str_theActionListId.CopyTo(parent_job_cls_id_) )
	{
		assert(false);
		return false;
	}
	if ( !cls_info->str_theClassName.game_str_.CopyTo(job_class_name_) )
	{
		assert(false);
		return false;
	}
	skill_branch_mgr_.SetGameNodes(&cls_info->str_theSkillCategories);
	if ( !skill_branch_mgr_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}

	const auto& job_cls_mgr = GClass::GJobClassTemplateMgr::GetMe();
	auto job_cls_template = job_cls_mgr.FindByJobClsId(job_cls_id_);
	if ( !job_cls_template )
	{
		assert(false);
		return false;
	}
	job_ = job_cls_template->GetJobType();
	job_cls_template = job_cls_mgr.FindByJobClsId(parent_job_cls_id_);
	if ( !job_cls_template )
	{
		assert(false);
		return false;
	}
	parent_job_ = job_cls_template->GetJobType();

	//装备类别
	support_equip_categories_.SetGameTArray(&cls_info->str_theEquipCategories);
	if ( !support_equip_categories_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

GClassInfoTemplate::GClassInfoTemplate()
{
	creatable_job_ = false;
	job_ = GType::kJC_Invalid;
	parent_job_ = GType::kJC_Invalid;
}

const std::wstring& GClassInfoTemplate::GetJobId() const
{
	return job_cls_id_;
}

const std::wstring& GClassInfoTemplate::GetJobClassName() const
{
	return job_class_name_;
}

GType::enJobCategory GClassInfoTemplate::GetJob() const
{
	return job_;
}

GType::enJobCategory GClassInfoTemplate::GetParentJob() const
{
	return parent_job_;
}

const GClassInfoTemplate::SkillBranchsMgr& GClassInfoTemplate::GetSkillBranchs() const
{
	return skill_branch_mgr_;
}

GClassInfoTemplate::SkillBranchsMgr& GClassInfoTemplate::GetSkillBranchs()
{
	return skill_branch_mgr_;
}

bool GClassInfoTemplate::IsCreatableJob() const
{
	return creatable_job_;
}

bool GClassInfoTemplate::CanEquipTheCategory( const std::wstring& equip_category ) const
{
	if (equip_category.empty())
		return false;
	//assert(!equip_category.empty());
	return !!support_equip_categories_.FindEquipCategory(equip_category);
}

struct stSdbSkillCategory{
	NODE					str_theNode;
	GAME_STRING_ID			str_theId;
	struct
	{
		DWORD				str_dwVFunAddr;
		GClass::stGameString		str_theId;
		GClass::stGameStringEx	str_theName;
		GClass::BTreeMgrBase::stGBTreeNodes		str_theSkills;
	}theSkillCategory;
	DWORD					str_dwUnKnowFlag;
};
static_assert(sizeof(stSdbSkillCategory) == sizeof(SDB_SKILL_CATEGORY), "");

bool GClassInfoTemplate::SkillBranch::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto sdb_skill_category = (const stSdbSkillCategory*)game_cur_node_;
	if ( !sdb_skill_category->theSkillCategory.str_theId.CopyTo(branch_id_) )
	{
		assert(false);
		return false;
	}
	if ( !sdb_skill_category->theSkillCategory.str_theName.game_str_.CopyTo(branch_name_idx_) )
	{
		assert(false);
		return false;
	}
	branch_skills_mgr_.SetGameNodes(&sdb_skill_category->theSkillCategory.str_theSkills);
	if ( !branch_skills_mgr_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

GClassInfoTemplate::SkillBranch::SkillBranch()
{
	branch_job_type_ = GType::kJC_Invalid;
}

GType::enJobCategory GClassInfoTemplate::SkillBranch::GetBranchJobType() const
{
	return branch_job_type_;
}

bool GClassInfoTemplate::SkillBranch::BuildBranchJobType()
{
	if (branch_name_idx_.empty())
	{
		assert(false);
		return false;
	}
	//LOG_O(Log_trace) << "branch_name_idx_\t" << branch_name_idx_;
	const auto& en2ch = GameEn2ChNodeMgr::GetMe();
	const auto& ch_name = en2ch.FindChByFullPath(branch_name_idx_);
	if (ch_name.empty())
	{
		assert(false);
		return false;
	}
	if ( kSkillJobTypeName_Any == ch_name )
	{
		branch_job_type_ = GType::kJC_Any;
		return true;
	}
	branch_job_type_ = GClass::JobNameMap::GetMe().FromChName(ch_name);
	if ( GType::kJC_Invalid == branch_job_type_ )
	{
		//assert(false);
		LOG_O(Log_debug) << "branch_name_idx_->" << branch_name_idx_ << "(" << ch_name << ")无效的job type";
		return false;
	}
	return true;
}

const GClassInfoTemplate::SkillBranch::BranchSkillsMgr& GClassInfoTemplate::SkillBranch::GetBranchSkills() const
{
	return branch_skills_mgr_;
}

struct stSdbTheSkill{
	NODE					str_theNode;
	GClass::stGameString			str_theId;
	GAME_STRING_ID			str_theId2;
	int						str_dwMaxSkillLevel;
	DWORD					str_dwUnKnow2;
};
static_assert(sizeof(stSdbTheSkill) == sizeof(SKILL_CATEGORY), "");

bool GClassInfoTemplate::SkillBranch::SkillInfo::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto sdb_the_skill = (const stSdbTheSkill*)game_cur_node_;
	if ( !sdb_the_skill->str_theId.CopyTo(skill_id_) )
	{
		assert(false);
		return false;
	}
	assert(sdb_the_skill->str_dwMaxSkillLevel > 0);
	skill_max_level_ = sdb_the_skill->str_dwMaxSkillLevel;
	return true;
}

const std::wstring& GClassInfoTemplate::SkillBranch::SkillInfo::GetSkillId() const
{
	return skill_id_;
}

GClassInfoTemplate::SkillBranch::SkillInfo::SkillInfo()
{
	skill_max_level_ = 0;
}

int GClassInfoTemplate::SkillBranch::SkillInfo::GetSkillMaxLvl() const
{
	return skill_max_level_;
}

const GClassInfoTemplate::SkillBranch* GClassInfoTemplate::SkillBranchsMgr::FindByBranchJobType( GType::enJobCategory job ) const
{
	for ( const auto& v : btree_ )
	{
		if ( v.GetBranchJobType() == job )
			return &v;
	}
	assert(false);
	return nullptr;
}

const GClassInfoTemplate::SkillBranch::SkillInfo* GClassInfoTemplate::SkillBranchsMgr::FindSkillInfoById( const std::wstring& skill_id ) const
{
	for ( const auto& v : btree_ )
	{
		auto skill_info = v.GetBranchSkills().FindSkillInfoById(skill_id);
		if (skill_info)
			return skill_info;
	}
	return nullptr;
}

const GClassInfoTemplate::SkillBranch::SkillInfo* GClassInfoTemplate::SkillBranch::BranchSkillsMgr::FindSkillInfoById( const std::wstring& skill_id ) const
{
	for ( const auto& v : btree_ )
	{
		if ( v.GetSkillId() == skill_id )
			return &v;
	}
	return nullptr;
}

bool GClassInfoTemplate::EquipCategory::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
		return false;
	}
	auto game_str = (const GameType*)cur_obj_addr_;
	if ( !game_str->CopyTo(equip_category_) )
	{
		assert(false);
		return false;
	}
	return true;
}

const std::wstring& GClassInfoTemplate::EquipCategory::GetEquipCategory() const
{
	return equip_category_;
}

const GClassInfoTemplate::EquipCategory* GClassInfoTemplate::EquipCategories::FindEquipCategory( const std::wstring& equip_category ) const
{
	for ( const auto& v : cont_ )
	{
		if ( v.GetEquipCategory() == equip_category )
			return &v;
	}
	return nullptr;
}
