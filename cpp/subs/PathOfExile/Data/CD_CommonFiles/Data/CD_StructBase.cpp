#include "stdafx.h"
#include "CD_StructBase.h"
#include <cassert>
#include "CD_UsefulFns.h"
#include <CommonX/CmnX_StaticConts.h>
#include "CD_Structs.h"
#include <StdEx/XstddefEx.h>

std::ostream& operator << (std::ostream& ostm, const stCD_LifeData& v){
	ostm << "(cur_value:" << v.cur_value_ << ",max_value:" << v.max_value_ << ")";
	return ostm;
}

void stCD_LifeData::DebugInfo() const
{
	LOG_O(Log_debug) << "addr:" << this << ",num1:" << num1_ << ",num2:" << num2_ << ",max_value:" << max_value_ << ",cur_value:" << cur_value_;
}

stCD_LifeData::stCD_LifeData()
{
	Cmnx_MemSetValue(this, sizeof(*this), 0);
}

std::ostream& operator << (std::ostream& ostm, const stCD_Db& v){
	ostm << "db_addr:" << &v << ",path:" << v.path_.GetStdString() << ",以下是EntityAttrInfo:" << std::flush;
	LOG_O(Log_debug) << "\n";
	return ostm;
}
void stCD_Db::DebugInfo() const
{
	LOG_O(Log_debug) << "db_addr:" << this << ",path:" << path_.GetStdString() << ",DbTblBaseAddr:" << db_tbl_;
}

void stCD_Db::lock()
{
	InterlockedIncrement(&ref_cnt_);
}

std::ostream& operator << (std::ostream& ostm, const stCD_EntityBase& v){
	assert(false);
	return ostm;
}
void stCD_EntityBase::DebugInfo() const
{
	LOG_O(Log_debug) << "Entity_addr:" << this << ",meta info:";
	if (!IsUnValidPtr(db_))
		db_->DebugInfo();
}

void stCD_Positioned::DebugInfo() const
{
	LOG_O(Log_debug) << "positioned addr:" << this << ",pos:" << pos_;
	/*LOG_O(Log_debug) << "float_pos:";
	float_pos_.DebugInfo();*/
}

stCD_Positioned* stCD_EntityPosBased::GetPositioned() const
{
	return FindAttrComponent<stCD_Positioned>(CD_kAttrInfoPositioned);
}

void stCD_EntityPosBased::DebugInfo() const
{
	__super::DebugInfo();
	auto p = GetPositioned();
	auto r = GetRender();

	LOG_O(Log_debug) << "EntityPosBasedAddr:" << this << ",Id:" << (void*)id_
		<< ",PositionedAddr:" << p << ",RenderAddr:" << r
		<< ",ActorAddr:" << GetActor() << ",NpcAddr:" << GetNpc()
		<< ",ClientNpcAddr:" << GetClientNpc() << ",MonsterAddr:" << GetMonster();
	if (p)
		p->DebugInfo();
	if (r)
		r->DebugInfo();
}

stCD_EntityAttrInfo* stCD_EntityBase::FindAttrInfo(pt_csz name) const
{
	assert(name);
	if (IsUnValidPtr(db_))
	{
		assert(false);
		return nullptr;
	}
	return db_->FindAttrInfo(name);
}

stCD_EntityAttrBase* stCD_EntityBase::FindAttrComponent(pt_csz name) const
{
	auto p = FindAttrInfo(name);
	if (!p)
		return nullptr;
	assert(p->array_idx_ >= 0 && p->array_idx_ < 1000);
	if (!entity_cont_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	auto res = entity_cont_[p->array_idx_];
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

bool stCD_EntityBase::IsValid() const
{
	if (IsUnValidPtr(db_))
		return false;
	if (!entity_cont_.IsValid())
		return false;
	if (!db_->GetDbTbl())
		return false;
	return true;
}

bool stCD_EntityPosBased::IsNpc() const
{
	assert(db_);
	assert(db_->db_tbl_);
	//地图装置 可以当作npc处理
	if (db_->path_.FindStr(CD_kMetaMapDevice))
		return true;
	if (db_->path_.FindStr(CD_kMetMappingDevice))
		return true;
	if (db_->path_.FindStr(CD_kMetMaligaroOrrery))
		return true;
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Npc::kEntityAttrMaxCnt)
		return false;
	/*if (!db_->path_.FindStr(CD_kMetaNpc))
		return false;*/
	if (!FindAttrInfo(CD_kAttrInfoClientNpc))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsPlayer() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Player::kEntityAttrMaxCnt)
		return false;
	/*if (!db_->path_.FindStr(CD_kMetaPlayer))
		return false;*/
	if (!FindAttrInfo(CD_kAttrInfoPlayer))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsMonster() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Monster::kEntityAttrMaxCnt)
		return false;
	/*if (!db_->path_.FindStr(CD_kMetaMonster))
		return false;*/
	if (db_->path_.FindStr(CD_kMetKaruiSlave))
		return false;
	if (!FindAttrInfo(CD_kAttrInfoMonster))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsWaypoint() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Waypoint::kEntityAttrMaxCnt)
		return false;
	if (!db_->path_.FindStr(CD_kMetaWaypoint))
		return false;
	if (!FindAttrInfo(CD_kAttrInfoMinimapIcon))
		return false;
	if (FindAttrInfo(CD_kAttrInfoChest))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsAreaTransition() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_AreaTransition::kEntityAttrMaxCnt)
		return false;
	if (db_->path_.FindStr(CD_kMetBellyArenaTransition))
		return false;
	if (GetPortal())
	{
		auto render = GetRender();
		if (!render)
			return false;
		if (CD_RenderName_CityPortalDoor == render->name_.c_str())
			return false;
		if (CD_RenderName_YiJieDoor == render->name_.c_str())
			return false;
		return true;
	}
	if (!FindAttrInfo(CD_kAttrInfoAreaTransition))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsNoHumain(pt_cwsz meta_name) const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() != (int)enCD_EntityAttr_NoHuman::kEntityAttrMaxCnt)
		return false;
	if (!db_->path_.FindStr(meta_name))
		return false;
	if (!FindAttrInfo(CD_kAttrInfoRender))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsStash() const
{
	return IsNoHumain(CD_kMetaStash);
}

bool stCD_EntityPosBased::IsNoticeBoard() const
{
	return IsNoHumain(CD_kMetaNoticeBoard);
}

bool stCD_EntityPosBased::IsNoticeBoardPvP() const
{
	return IsNoHumain(CD_kMetaNoticeBoardPvP);
}

bool stCD_EntityPosBased::IsChest() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Chest::kEntityAttrMaxCnt)
		return false;
	/*if (!db_->path_.FindStr(CD_kMetaChest))
		return false;*/
	if (db_->path_.FindStr(CD_kMetaWaypoint))
		return false;
	if (!FindAttrInfo(CD_kAttrInfoChest))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsWorldItem() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_WorldItem::kEntityAttrMaxCnt)
		return false;
	/*if (!db_->path_.FindStr(CD_kMetaWorldItem))
		return false;*/
	if (!FindAttrInfo(CD_kAttrInfoWorldItem))
		return false;
	return true;
}

bool stCD_EntityPosBased::IsTransitionable() const
{
	assert(db_);
	assert(db_->db_tbl_);
	/*if (db_->GetDbTbl()->GetAttrCnt() < (int)enCD_EntityAttr_Transitionable::kEntityAttrMaxCnt)
		return false;*/
	if (db_->path_.FindStr(CD_kMetBellyArenaTransition))
		return true;
	if (!FindAttrInfo(CD_kAttrInfoTransitionable))
		return false;
	if (FindAttrInfo(CD_kAttrInfoAreaTransition))
		return false;
	if (FindAttrInfo(CD_kAttrInfoPortal))
		return false;
	return true;
}

stCD_EntityNpc* stCD_EntityPosBased::CastToNpc() const
{
	if (!IsNpc())
		return nullptr;
	return const_cast<stCD_EntityNpc*>(CastTo<stCD_EntityNpc>());
}

stCD_EntityPlayer* stCD_EntityPosBased::CastToPlayer() const
{
	if (!IsPlayer())
		return nullptr;
	return const_cast<stCD_EntityPlayer*>(CastTo<stCD_EntityPlayer>());
}

stCD_EntityMonster* stCD_EntityPosBased::CastToMonster() const
{
	if (!IsMonster())
		return nullptr;
	return const_cast<stCD_EntityMonster*>(CastTo<stCD_EntityMonster>());
}

stCD_EntityWaypoint* stCD_EntityPosBased::CastToWaypoint() const
{
	if (!IsWaypoint())
		return nullptr;
	return const_cast<stCD_EntityWaypoint*>(CastTo<stCD_EntityWaypoint>());
}

stCD_EntityAreaTransition* stCD_EntityPosBased::CastToAreaTransition() const
{
	if (!IsAreaTransition())
		return nullptr;
	return const_cast<stCD_EntityAreaTransition*>(CastTo<stCD_EntityAreaTransition>());
}

stCD_EntityNoHuman* stCD_EntityPosBased::CastToNoHuman(pt_cwsz meta_name) const
{
	if (!IsNoHumain(meta_name))
		return nullptr;
	return const_cast<stCD_EntityNoHuman*>(CastTo<stCD_EntityNoHuman>());
}

stCD_EntityNoHuman* stCD_EntityPosBased::CastToStash() const
{
	return CastToNoHuman(CD_kMetaStash);
}

stCD_EntityNoHuman* stCD_EntityPosBased::CastToNoticeBoard() const
{
	return CastToNoHuman(CD_kMetaNoticeBoard);
}

stCD_EntityNoHuman* stCD_EntityPosBased::CastToNoticeBoardPvP() const
{
	return CastToNoHuman(CD_kMetaNoticeBoardPvP);
}

stCD_EntityChest* stCD_EntityPosBased::CastToChest() const
{
	if (!IsChest())
		return nullptr;
	return const_cast<stCD_EntityChest*>(CastTo<stCD_EntityChest>());
}

stCD_EntityWorldItem* stCD_EntityPosBased::CastToWorldItem() const
{
	if (!IsWorldItem())
		return nullptr;
	return const_cast<stCD_EntityWorldItem*>(CastTo<stCD_EntityWorldItem>());
}

stCD_EntityTransitionable* stCD_EntityPosBased::CastToTransitionable() const
{
	if (!IsTransitionable())
		return nullptr;
	return const_cast<stCD_EntityTransitionable*>(CastTo<stCD_EntityTransitionable>());
}

void stCD_EntityPosBased::DebugInfoAsNpc() const
{
	auto p = CastToNpc();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoAsPlayer() const
{
	auto p = CastToPlayer();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoAsMonster() const
{
	auto p = CastToMonster();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoAsWaypoint() const
{
	auto p = CastToWaypoint();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoAsAreaTransition() const
{
	auto p = CastToAreaTransition();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoNoHuman() const
{
	auto p = CastToStash();
	if (p)
	{
		p->DebugInfo();
		return;
	}
	p = CastToStash();
	if (p)
	{
		p->DebugInfo();
		return;
	}
	p = CastToStash();
	if (p)
	{
		p->DebugInfo();
		return;
	}
}

void stCD_EntityPosBased::DebugInfoChest() const
{
	auto p = CastToChest();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoWorldItem() const
{
	auto p = CastToWorldItem();
	if (p)
		p->DebugInfo();
}

void stCD_EntityPosBased::DebugInfoTransitionable() const
{
	auto p = CastToTransitionable();
	if (p)
		p->DebugInfo();
}

stCD_Render* stCD_EntityPosBased::GetRender() const
{
	return FindAttrComponent<stCD_Render>(CD_kAttrInfoRender);
}

stCD_Actor* stCD_EntityPosBased::GetActor() const
{
	return FindAttrComponent<stCD_Actor>(CD_kAttrInfoActor);
}

stCD_EntityAttrNPC* stCD_EntityPosBased::GetNpc() const
{
	return FindAttrComponent<stCD_EntityAttrNPC>(CD_kAttrInfoNpc);
}

stCD_EntityAttrClientNpc* stCD_EntityPosBased::GetClientNpc() const
{
	return FindAttrComponent<stCD_EntityAttrClientNpc>(CD_kAttrInfoClientNpc);
}

stCD_EntityAttrMonster* stCD_EntityPosBased::GetMonster() const
{
	return FindAttrComponent<stCD_EntityAttrMonster>(CD_kAttrInfoMonster);
}

void stCD_EntityPosBased::DebugMonsterSkillInfo() const
{
	auto m = CastToMonster();
	if (!m)
		return;
	auto actor = GetActor();
	if (!actor)
	{
		assert(false);
		return;
	}
	auto render = GetRender();
	if (!render)
	{
		assert(false);
		return;
	}
	auto& skill_mgr = actor->GetMySkillMgr();
	LOG_O(Log_debug) << "MonsterEntityAddr:" << m << ",Name:" << CD_SafeName(render->name_)
		<< ",SkillMgrAddr:" << &skill_mgr << ",WeaponSkillType:" << (pt_pcvoid)skill_mgr.weapon_skill_type_
		<< ",WeaponSkillValue:" << (pt_pcvoid)skill_mgr.weapon_skill_value_;
}

stCD_EntityAttrDiesAfterTime* stCD_EntityPosBased::GetDiesAfterTime() const
{
	if (db_ && db_->path_.FindStr(L"Monsters/Missions/TemplarMissionRelic"))
		return FindAttrComponent<stCD_EntityAttrDiesAfterTime>(CD_kAttrInfoDiesAfterTime);
	return nullptr;
	//if (db_ && db_->path_.FindStr(L"Monsters/Missions/TemplarMissionRelic"))
	//{
	//	if (FindAttrComponent<stCD_EntityAttrDiesAfterTime>(CD_kAttrInfoDiesAfterTime))
	//		return nullptr;
	//}
	//return FindAttrComponent<stCD_EntityAttrDiesAfterTime>(CD_kAttrInfoDiesAfterTime);
}

stCD_EntityAttrPortal* stCD_EntityPosBased::GetPortal() const
{
	return FindAttrComponent<stCD_EntityAttrPortal>(CD_kAttrInfoPortal);
}

stCD_Pathfinding* stCD_EntityPosBased::GetPathfinding() const
{
	return FindAttrComponent<stCD_Pathfinding>(CD_kAttrInfoPathfinding);
}

stCD_EntityAttrTargetable* stCD_EntityPosBased::GetTargetable() const
{
	return FindAttrComponent<stCD_EntityAttrTargetable>(CD_kAttrInfoTargetable);
}

stCD_EntityAttrTransitionable* stCD_EntityPosBased::GetTransitionable() const
{
	return FindAttrComponent<stCD_EntityAttrTransitionable>(CD_kAttrInfoTransitionable);
}

bool stCD_EntityPosBased::IsMovement() const
{
	if (!FindAttrInfo(CD_kAttrInfoMovement))
		return false;
	return true;
}

stCD_EntityAttrDbBase* stCD_EntityBase::FindAttrDb(pt_csz name) const
{
	if (IsUnValidPtr(db_))
	{
		assert(false);
		return nullptr;
	}
	return db_->FindAttrDb(name);
}

stCD_DbTblEntity* stCD_EntityBase::GetDbTbl() const
{
	if (IsUnValidPtr(db_))
	{
		assert(false);
		return nullptr;
	}
	return db_->GetDbTbl();
}

const GameWString* stCD_EntityBase::GetDbPath() const
{
	if (IsUnValidPtr(db_))
	{
		assert(false);
		return nullptr;
	}
	return &db_->path_;
}

void stCD_EntityItemBased::DebugInfo() const
{
	__super::DebugInfo();
	auto base = GetAttrDbBase();
	if (!base)
	{
		assert(false);
		return;
	}
	base->DebugInfo();
	auto m = GetAttrMods();
	if (m)
		m->DebugInfo();
	auto re = GetAttrDbAttributeRequirements();
	if (re)
		re->DebugInfo();
	auto s = GetAttrStack();
	if (s)
		s->DebugInfo();
	auto sockets = GetAttrSockets();
	if (sockets)
		sockets->DebugInfo();
	auto skill_gem = GetAttrDbSkillGem();
	if (skill_gem)
		skill_gem->DebugInfo();
}

stCD_AttrDbItemBase* stCD_EntityItemBased::GetAttrDbBase() const
{
	return FindAttrDb<stCD_AttrDbItemBase>(CD_kAttrInfoBase);
}

stCD_AttrDbQuality* stCD_EntityItemBased::GetAttrDbQuality() const
{
	return FindAttrDb<stCD_AttrDbQuality>(CD_kAttrInfoQuality);
}

stCD_AttrDbAttributeRequirements* stCD_EntityItemBased::GetAttrDbAttributeRequirements() const
{
	return FindAttrDb<stCD_AttrDbAttributeRequirements>(CD_kAttrInfoAttributeRequirements);
}

stCD_EntityAttrMods* stCD_EntityItemBased::GetAttrMods() const
{
	return FindAttrComponent<stCD_EntityAttrMods>(CD_kAttrInfoMods);
}

stCD_EntityAttrStack* stCD_EntityItemBased::GetAttrStack() const
{
	return FindAttrComponent<stCD_EntityAttrStack>(CD_kAttrInfoStack);
}

stCD_EntityAttrSockets* stCD_EntityItemBased::GetAttrSockets() const
{
	return FindAttrComponent<stCD_EntityAttrSockets>(CD_kAttrInfoSockets);
}

bool stCD_EntityItemBased::IsArmour() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (!FindAttrInfo(CD_kAttrInfoArmour))
		return false;
	return true;
}

bool stCD_EntityItemBased::IsSkillGem() const
{
	assert(db_);
	assert(db_->db_tbl_);
	if (!FindAttrInfo(CD_kAttrInfoSkillGem))
		return false;
	return true;
}

stCD_AttrDbSkillGem* stCD_EntityItemBased::GetAttrDbSkillGem() const
{
	return FindAttrDb<stCD_AttrDbSkillGem>(CD_kAttrInfoSkillGem);
}

stCD_TblItemBaseItemTypes* stCD_EntityItemBased::GetBaseItemType() const
{
	auto db_base = GetAttrDbBase();
	if (!db_base)
	{
		assert(false);
		return nullptr;
	}
	if (!db_base->base_item_type_pair_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return db_base->base_item_type_pair_.tbl_item_;
}

stCD_EntityAttrUsable* stCD_EntityItemBased::GetAttrUsable() const
{
	return FindAttrComponent<stCD_EntityAttrUsable>(CD_kAttrInfoUsable);
}

stCD_EntityAttrCharges* stCD_EntityItemBased::GetAttrCharges() const
{
	return FindAttrComponent<stCD_EntityAttrCharges>(CD_kAttrInfoCharges);
}

stCD_EntityAttrDbFlask* stCD_EntityItemBased::GetAttrDbFlask() const
{
	return FindAttrDb<stCD_EntityAttrDbFlask>(CD_kAttrInfoFlask);
}

enCD_ItemType stCD_EntityItemBased::GetItemType() const
{
	typedef std::map<const wchar_t*, enCD_ItemType, StdEx::LessChar<wchar_t> > MapT;
	struct stHelper{
		stHelper(){
			types_[L"LifeFlask"] = kITYPE_LifeFlask;
			types_[L"ManaFlask"] = kITYPE_ManaFlask;
			types_[L"HybridFlask"] = kITYPE_HybridFlask;
			types_[L"StackableCurrency"] = kITYPE_StackableCurrency;
			types_[L"Active Skill Gem"] = kITYPE_ActiveSkillGem;
			types_[L"Support Skill Gem"] = kITYPE_SupportSkillGem;
			types_[L"Helmet"] = kITYPE_Helmet;
			types_[L"Amulet"] = kITYPE_Amulet;
			types_[L"Ring"] = kITYPE_Ring;
			types_[L"Belt"] = kITYPE_Belt; 
			types_[L"Boots"] = kITYPE_Boots; 
			types_[L"Gloves"] = kITYPE_Gloves;
			types_[L"Body Armour"] = kITYPE_BodyArmour;
			types_[L"Shield"] = kITYPE_Shield;
			types_[L"Currency"] = kITYPE_Currency;
			types_[L"QuestItem"] = kITYPE_QuestItem; 
			types_[L"Claw"] = kITYPE_Claw;
			types_[L"Dagger"] = kITYPE_Dagger;
			types_[L"Wand"] = kITYPE_Wand;
			types_[L"One Hand Sword"] = kITYPE_OneHandSword;
			types_[L"Thrusting One Hand Sword"] = kITYPE_ThrustingOneHandSword;
			types_[L"One Hand Axe"] = kITYPE_OneHandAxe;
			types_[L"One Hand Mace"] = kITYPE_OneHandMace;
			types_[L"Bow"] = kITYPE_Bow;
			types_[L"Staff"] = kITYPE_Staff;
			types_[L"Two Hand Sword"] = kITYPE_TwoHandSword;
			types_[L"Two Hand Axe"] = kITYPE_TwoHandAxe;
			types_[L"Two Hand Mace"] = kITYPE_TwoHandMace;
			types_[L"Quiver"] = kITYPE_Quiver;
			types_[L"SmallRelic"] = kITYPE_SmallRelic;
			types_[L"MediumRelic"] = kITYPE_MediumRelic;
			types_[L"LargeRelic"] = kITYPE_LargeRelic;
			types_[L"Sceptre"] = kITYPE_Sceptre;
			types_[L"UtilityFlask"] = kITYPE_UtilityFlask;
			types_[L"UtilityFlaskCritical"] = kITYPE_UtilityFlaskCritical;
			types_[L"Map"] = kITYPE_Map;
			types_[L"Unarmed"] = kITYPE_Unarmed;
			types_[L"FishingRod"] = kITYPE_FishingRod;
			types_[L"MapFragment"] = kITYPE_MapFragment;
			types_[L"HideoutDoodad"] = kITYPE_HideoutDoodad;
			types_[L"Microtransaction"] = kITYPE_Microtransaction;
			types_[L"Jewel"] = kITYPE_Jewel;
			types_[L"DivinationCard"] = kITYPE_DivinationCard;
			types_[L"LabyrinthItem"] = kITYPE_LabyrinthItem;
			types_[L"LabyrinthTrinket"] = kITYPE_LabyrinthTrinket;
			types_[L"LabyrinthMapItem"] = kITYPE_LabyrinthMapItem;
			types_[L"Breachstone"] = kITYPE_Breachstone;
			types_[L"Leaguestone"] = kITYPE_Leaguestone;
		}

		MapT	types_;
	};
	auto base_type = GetBaseItemType();
	if (!base_type)
	{
		assert(false);
		return kITYPE_Invalid;
	}
	if (!base_type->item_class_pair_.IsValid())
	{
		assert(false);
		return kITYPE_Invalid;
	}
	auto class_ch_name = base_type->item_class_pair_.tbl_item_->class_name_;
	static stHelper helper;
	auto it = helper.types_.find(CD_SafeName(class_ch_name));
	if (it == helper.types_.end())
		return kITYPE_Invalid;
	return it->second;
}

stCD_EntityAttrItemBase* stCD_EntityItemBased::GetAttrBase() const
{
	auto res = FindAttrComponent<stCD_EntityAttrItemBase>(CD_kAttrInfoBase);
	assert(res);
	return res;
}

stCD_EntityAttrSkillGem* stCD_EntityItemBased::GetAttrSkillGem() const
{
	return FindAttrComponent<stCD_EntityAttrSkillGem>(CD_kAttrInfoSkillGem);
}

int stCD_ItemGridsIdxInfo::TraItemGridIdx(int& cursor, int bag_width) const
{
	assert(bag_width > 0);
	auto width = right_x_end_idx_ - left_x_idx_;
	if (width <= 0)
		return -1;
	//assert(width > 0);
	auto height = down_y_end_idx_ - up_y_idx_;
	if (height <= 0)
		return -1;
	//assert(height > 0);
	auto h_idx = cursor / width;
	if (h_idx >= height)
	{
		assert(cursor % width == 0);
		return -1;
	}
	auto w_idx = cursor % width;
	w_idx += left_x_idx_;
	h_idx += up_y_idx_;
	++cursor;
	return bag_width * h_idx + w_idx;
}

void stCD_ItemGridsIdxInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "ItemGridsIdxInfoAddr:" << this << ",left_x_idx:" << left_x_idx_
		<< ",up_y_idx:" << up_y_idx_ << ",right_x_end_idx:" << right_x_end_idx_ << ",down_y_end_idx:" << down_y_end_idx_;
}

stCD_ItemGridsIdxInfo::stCD_ItemGridsIdxInfo()
{
	left_x_idx_ = -1;
	up_y_idx_ = -1;
	right_x_end_idx_ = -1;
	down_y_end_idx_ = -1;
}

bool stCD_ItemGridsIdxInfo::IsValid() const
{
	return left_x_idx_ >= 0 && up_y_idx_ >= 0 && right_x_end_idx_ > left_x_idx_ && down_y_end_idx_ > up_y_idx_;
}

int stCD_GemSlot::GetSlotCnt() const
{
	int cnt = 0;
	for (auto v : slot_)
	{
		assert(v >= kGSC_Red);
		if (v >= kGSC_Invalid)
		{
			assert(v == kGSC_Invalid);
			break;
		}
		++cnt;
	}
#ifdef _DEBUG
	if (slot_connect_info_.IsValid())
	{
		pt_byte b_cnt = 0;
		for (auto v : slot_connect_info_)
		{
			b_cnt += v;
		}
		assert(cnt == b_cnt);
	}
#endif
	return cnt;
}

int stCD_GemSlot::GetUsedSlotCnt() const
{
	int cnt = 0;
	for (auto v : gems_)
	{
		if (IsUnValidPtr(v))
			continue;
		++cnt;
	}
	return cnt;
}

stCD_EntityItemBased* stCD_GemSlot::TraInsideGem(int& cursor) const
{
	if (cursor >= _countof(gems_))
	{
		assert(cursor == CD_kGemSlotMaxCnt);
		return nullptr;
	}
	do{
		auto res = gems_[cursor % _countof(gems_)];
		++cursor;
		if (!IsUnValidPtr(res))
			return res;
	} while (cursor < _countof(gems_));
	return nullptr;
}

std::ostream& operator << (std::ostream& ostm, const stCD_GemSlot::GemSlotVecT& v){
	for (auto tmp : v)
	{
		ostm << tmp << ",";
	}
	return ostm;
}
void stCD_GemSlot::DebugInfo() const
{
	LOG_O(Log_debug) << "GemSlot_color:" << slot_ << "\t slot_connect_info_:" << slot_connect_info_ << ",以下是镶嵌的宝石：";
	int cursor = 0;
	while (true)
	{
		auto gem = TraInsideGem(cursor);
		if (!gem)
			break;
		gem->DebugInfo();
	}
	LOG_O(Log_debug) << "镶嵌的宝石总数为：" << GetUsedSlotCnt();
}

stCD_GemSlot::stCD_GemSlot()
{
	for (auto& v : slot_)
	{
		v = kGSC_Invalid;
	}

	for (auto& v : gems_)
	{
		v = nullptr;
	}
}

std::ostream& operator << (std::ostream& ostm, const stCD_StringEnChPair& v){
	ostm << "(ch:" << WideCharToGbk(CD_SafeStr(v.ch_str_)) << ",en:" << WideCharToGbk(CD_SafeStr(v.en_str_)) << ")";
	return ostm;
}

std::ostream& operator<<(std::ostream& ostm, const stCD_PassiveSkillEdge& v)
{
	if (IsUnValidPtr(v.from_))
	{
		assert(false);
		return ostm;
	}
	if (IsUnValidPtr(v.to_))
	{
		assert(false);
		return ostm;
	}
	if (!v.from_->passive_skill_.IsValid())
	{
		//assert(false);
		return ostm;
	}
	if (!v.to_->passive_skill_.IsValid())
	{
		//assert(false);
		return ostm;
	}
	ostm << "(" << WideCharToGbk(CD_SafeName(v.from_->passive_skill_.tbl_item_->ch_name_)) << " | "
		<< WideCharToGbk(CD_SafeName(v.from_->passive_skill_.tbl_item_->en_name_)) << " , "
		<< WideCharToGbk(CD_SafeName(v.to_->passive_skill_.tbl_item_->ch_name_)) << " | "
		<< WideCharToGbk(CD_SafeName(v.to_->passive_skill_.tbl_item_->en_name_)) << ")";
	return ostm;
}

void stCD_StringEnChPair::DebugInfo() const
{
	LOG_O(Log_debug) << "(ch:" << CD_SafeStr(ch_str_) << ",en:" << CD_SafeStr(en_str_) << ")";
}

void stCD_BuffInfo::DebugInfo() const
{
	if (IsUnValidPtr(db_buff_definition_))
	{
		assert(false);
		return;
	}
	if (!db_buff_definition_->IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "BuffInfoAddr:" << this;
	db_buff_definition_->tbl_item_->DebugInfo();
}

pt_cwsz stCD_BuffInfo::GetBuffCnName() const
{
	if (IsUnValidPtr(db_buff_definition_))
	{
		assert(false);
		return nullptr;
	}
	if (!db_buff_definition_->IsValid())
	{
		assert(false);
		return nullptr;
	}
	auto name = db_buff_definition_->tbl_item_->ch_name_;
	if (IsUnValidPtr(name))
	{
		assert(false);
		return nullptr;
	}
	return name;
}

pt_cwsz stCD_BuffInfo::GetBuffDesc() const
{
	if (IsUnValidPtr(db_buff_definition_))
	{
		assert(false);
		return nullptr;
	}
	if (!db_buff_definition_->IsValid())
	{
		assert(false);
		return nullptr;
	}
	auto desc = db_buff_definition_->tbl_item_->ch_desc_;
	if (IsUnValidPtr(desc))
	{
		assert(false);
		return nullptr;
	}
	return desc;
}

pt_cwsz stCD_BuffInfo::GetBuffEnName() const
{
	if (IsUnValidPtr(db_buff_definition_))
	{
		assert(false);
		return nullptr;
	}
	if (!db_buff_definition_->IsValid())
	{
		assert(false);
		return nullptr;
	}
	auto name = db_buff_definition_->tbl_item_->en_name_;
	if (IsUnValidPtr(name))
	{
		assert(false);
		return nullptr;
	}
	return name;
}

void stCD_TblItemPassiveSkill::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemPassiveSkill:" << this << ",en_name:" << CD_SafeName(en_name_)
		<< ",ch_name:" << CD_SafeName(ch_name_) << ",id:" << skill_id_
		<< ",ukn_b_:" << (int)ukn_b_ << ",ukn_b1_:" << (int)ukn_b1_
		;
	if (professions_.size() > 0)
	{
		for (auto& v : professions_)
		{
			LOG_O(Log_debug) << "所属职业：" << v.tbl_item_->profession_ch_name_;
		}
	}
}

void stCD_DbEntity::DebugInfo() const
{
	__super::DebugInfo();
	auto tbl = GetDbTbl();
	if (!tbl)
	{
		assert(false);
		return;
	}
	tbl->DebugInfo();
}

stCD_EntityAttrInfo* stCD_DbEntity::FindAttrInfo(pt_csz name) const
{
	assert(name);
	auto tbl = GetDbTbl();
	if (!tbl)
	{
		assert(false);
		return nullptr;
	}
	return tbl->FindAttrIdxInfo(name);
}

stCD_EntityAttrDbBase* stCD_DbEntity::FindAttrDb(pt_csz name) const
{
	auto p = FindAttrInfo(name);
	if (!p)
		return nullptr;
	assert(p->array_idx_ >= 0 && p->array_idx_ < 1000);
	auto ptr = GetDbTbl();
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}
	if (!ptr->attr_db_cont_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	auto res = ptr->attr_db_cont_[p->array_idx_];
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_TblItemQuestStates::DebugInfo() const
{
	if (!quest_pair_.IsValid() || !branch_ids_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "TblItemQuestStatesAddr:" << this
		<< ",quest_name:" << CD_SafeName(quest_pair_.tbl_item_->ch_name_)
		<< ",quest_state:" << quest_state_
		<< ",branch_cnt:" << branch_ids_.size()
		<< ",is_complete_state_:" << is_complete_state_
		<< ",simple_prompt_:" << CD_SafeStr(simple_prompt_) << ",ukn_dw1_:" << ukn_dw1_
		<< ",ukn_byte_:" << (int)ukn_byte_;
		//<< ",state_desc_:" << CD_SafeStr(state_desc_);
	if (!branch_ids_.IsValid())
	{
		assert(false);
		return;
	}
	if (!map_pins_.empty() && map_pins_.IsValid())
	{
		LOG_O(Log_debug) << "map_pins info:";
		map_pins_.begin()->tbl_item_->DebugInfo();
	}
	if (!map_pins1_.empty() && map_pins1_.IsValid())
	{
		LOG_O(Log_debug) << "map_pins1 info:";
		map_pins1_.begin()->tbl_item_->DebugInfo();
	}
	/*if (!quest_prompt_.empty() && quest_prompt_.IsValid())
	{
		quest_prompt_.begin()->DebugInfo();
	}*/
	/*for (auto v : branch_ids_)
	{
		LOG_O(Log_debug) << "分支任务id:" << v;
	}*/
}

void stCD_TblItemQuest::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemQuestAddr:" << this << ",en_name:" << CD_SafeName(en_name_) << ",ch_name:" << CD_SafeName(ch_name_)
		<< ",chapter_id:" << chapter_id_ << ",sub_id:" << sub_id_ << ",flag:" << flag_ 
		<< ",is_branch_line_:" << is_branch_line_ << ",ukn4:" << ukn4_
		<< ",ukn5:" << ukn5_ << ",dds_file_path:" << CD_SafePath(dds_file_path_);
}

void stCD_TblItemSkillGems::DebugInfo() const
{
	if (!item_pair_.IsValid())
	{
		LOG_O(Log_debug) << "TblItemSkillGemsAddr:" << this;
		assert(false);
		return;
	}
	if (!granted_effects_pair_.IsValid())
	{
		LOG_O(Log_debug) << "TblItemSkillGemsAddr:" << this;
		assert(false);
		return;
	}
	if (!gem_tag_info_.IsValid())
	{
		LOG_O(Log_debug) << "TblItemSkillGemsAddr:" << this;
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "TblItemSkillGemsAddr:" << this
		<< ",item_name:" << CD_SafeName(item_pair_.tbl_item_->item_name_)
		<< ",gem_tag_cnt:" << gem_tag_info_.size()
		<< ",ukn_dw1_:" << ukn_dw1_ << ",ukn_dw2_:" << ukn_dw2_
		<< ",ukn_dw3_:" << ukn_dw3_ << ",ukn_byte_:" << (int)ukn_byte_;
	if (item_pair1_.IsValid())
	{
		LOG_O(Log_debug) << "item_name1:" << CD_SafeName(item_pair1_.tbl_item_->item_name_);
	}
	if (gem_tag_info_.IsValid())
	{
		for (auto& v : gem_tag_info_)
		{
			v.tbl_item_->DebugInfo();
		}
	}
}

void stCD_TblItemActiveSkills::DebugInfo() const
{
	if (!ukn_ids1_.IsValid() || !ukn_ids2_.IsValid()/* || !item_classes_.IsValid()*/)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "TblItemActiveSkillsAddr:" << this << ",en_name_:" << CD_SafeName(en_name_)
		<< ",ch_name_:" << CD_SafeName(ch_name_) 
		<< ",ukn_byte1_:" << (int)ukn_byte1_ << ",ukn_byte2_:" << int(ukn_byte2_)
		<< ",ukn_dw1_:" << ukn_dw1_ << ",ukn_byte3_:" << (int)ukn_byte3_ 
		<< ",en_name1_:" << CD_SafeName(en_name1_) << ",en_name2_:" << CD_SafeName(en_name2_)
		<< ",use_desc_:" << CD_SafeStr(use_desc_) 
		/*<< ",dds_file_path_:" << CD_SafePath(dds_file_path_)
		<< ",http_image_path_:" << CD_SafeStr(http_image_path_)*/;
}

void stCD_TblItemItemClasses::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemItemClassesAddr:" << this << ",class_name_:" << CD_SafeName(class_name_)
		<< ",ch_name_:" << CD_SafeName(ch_name_) << ",base_class_name_:" << CD_SafeName(base_class_name_);
}

void stCD_TblItemBaseItemTypes::DebugInfo() const
{
	if (!item_class_pair_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "BaseItemTypesAddr:" << this << ",item_name_:" << CD_SafeName(item_name_)
		<< ",item_class_name:" << CD_SafeName(item_class_pair_.tbl_item_->ch_name_) << ",grid_width_:" << grid_width_
		<< ",grid_height_:" << grid_height_ << ",ukn_dw_:" << ukn_dw_
		<< ",ukn_dw1_:" << ukn_dw1_
		<< ",meta_path1_:" << CD_SafePath(meta_path1_) << ",meta_path2_:" << CD_SafePath(meta_path2_);
}

stCD_TblItemSkillGems* stCD_DbSkillGems::FindByBaseItemType(stCD_TblItemBaseItemTypes* base_item_ptr) const
{
	auto tbl = GetDbTbl();
	if (!tbl)
	{
		assert(false);
		return nullptr;
	}
	for (auto& v : tbl->tbl_items_)
	{
		if (v.item_pair_.tbl_item_ == base_item_ptr || v.item_pair1_.tbl_item_ == base_item_ptr)
			return &v;
	}
	return nullptr;
}

void stCD_AttrDbItemBase::DebugInfo() const
{
	LOG_O(Log_debug) << "ItemAttrDbItemBase_Addr:" << this << ",name:" << CD_SafeName(name_)
		<< ",grid_width_:" << grid_width_ << ",grid_height_:" << grid_height_
		<< ",desc:" << CD_SafeStr(desc_);
	if (!base_item_type_pair_.IsValid())
	{
		assert(false);
		return;
	}
	base_item_type_pair_.tbl_item_->DebugInfo();
}

void stCD_DbTblEntity::DebugInfo() const
{
	LOG_O(Log_debug) << "DbTblEntityAddr:" << this << ",addr_idx_addr:" << &attr_idx_;
	int cnt = 0;
	for (auto& v : attr_idx_)
	{
		++cnt;
		LOG_O(Log_debug) << "属性名：  " << v.first << ",索引：" << v.second.array_idx_;
	}
	LOG_O(Log_debug) << "遍历到的属性总数：" << cnt << "，游戏中的总数：" << attr_idx_.size();
	assert(cnt == attr_idx_.size());
}

stCD_EntityAttrInfo* stCD_DbTblEntity::FindAttrIdxInfo(pt_csz name)
{
	assert(name);
	for (auto& v : attr_idx_)
	{
		if (IsUnValidPtr(v.first))
		{
			assert(false);
			return nullptr;
		}
		if (0 == strcmp(name, v.first))
			return &v.second;
	}
	return nullptr;
}

int stCD_DbTblEntity::GetAttrCnt() const
{
	return attr_idx_.size();
}

void stCD_TblItemGrantedEffects::DebugInfo() const
{
	pt_cwsz skill_name;
	if (active_skill_pair_.IsValid())
		skill_name = CD_SafeName(en_name_);
	else
		skill_name = L"";
	LOG_O(Log_debug) << "TblItemGrantedEffectsAddr:" << this << ",en_name_:" << en_name_ << ",ActiveSkillName:" << skill_name
		<< ",ukn_dw_:" << ukn_dw_ << ",ukn_byte_:" << (int)ukn_byte_ << ",ukn_dw1_:" << ukn_dw1_
		<< ",ukn_dw2_:" << ukn_dw2_ << ",ukn_dw3_:" << ukn_dw3_
		//最后几项的时候，这两个名字有问题
		//<< ",en_name1_:" << CD_SafeName(en_name1_) << ",en_name2_:" << CD_SafeName(en_name2_) 
		<< ",ukn_dw4_:" << ukn_dw4_ << ",ukn_w_:" << ukn_w_
		<< ",ukn_dw6_:" << ukn_dw6_ << ",ukn_dw5_:" << ukn_dw5_ << ",ukn_byte1_:" << (int)ukn_byte1_;
}

void stCD_TblItemWorldArea::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemWorldAreaAddr:" << this << ",en_area_name_:" << CD_SafeName(en_area_name_)
		<< ",ch_name:" << CD_SafeName(ch_map_name_)
		<< ",chapter_:" << chapter_ << ", is_town:" << is_town_ << ",world_area_id:" << world_area_id_;
}

enCD_kMapDifficulty stCD_TblItemWorldArea::GetDiffLvl() const
{
#ifdef VER3_
	if (chapter_ <= 5)
		return kMD_Normal;
	else if(chapter_ <= 10)
		return kMD_Cruel;
	else
		return kMD_Merciless;
#else
	if (difficult_.IsValid())
		return difficult_.tbl_item_->GetDifficulty();
	return kMD_Invalid;
#endif
}

void stCD_TblItemMapPins::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemMapPinsAddr:" << this << ",en_map_name_:" << CD_SafeName(en_map_name_)
		<< ",pos_x_:" << pos_x_ << ",pos_y_:" << pos_y_ << ",map_ch_name_:" << CD_SafeName(map_ch_name_)
		<< ",ukn_dw_:" << ukn_dw_ << ",en_map_name1_:" << CD_SafeName(en_map_name1_) << ",ukn_dw1_:" << ukn_dw1_;
	/*if (world_area_.IsValid())
	{
		LOG_O(Log_debug) << "TblItemMapPins_world_area_name:" << CD_SafeName(world_area_.tbl_item_->ch_map_name_);
	}
	if (world_areas_.IsValid())
	{
		for (auto& v : world_areas_)
		{
			LOG_O(Log_debug) << "TblItemMapPins_world_areas_name:" << CD_SafeName(v.tbl_item_->ch_map_name_);
		}
	}*/
}

stCD_WorldAreaPairT stCD_TblItemMapPins::GetTransferWorldItemPair() const
{
	//还是别用游戏的函数了，因为它会db引用+1
	/*stCD_WorldAreaPairT res;
	g_c_map_pins_get_waypoint_world_area.Call<void>(this, &res);
	return res;*/
	return waypoint_world_area_;
}

void stCD_TblItemMapConnections::DebugInfo() const
{
	if (!map_dst_.IsValid() || !map_src_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "MapConnectionsAddr:" << this << ",map_src_name:" << CD_SafeName(map_src_.tbl_item_->map_ch_name_)
		<< ",map_dst_name:" << CD_SafeName(map_dst_.tbl_item_->map_ch_name_)
		<< ",ukn_dw_:" << ukn_dw_ << ",ukn_dw1_:" << ukn_dw1_ << ",ukn_dw2_:" << ukn_dw2_ << ",ukn_dw3_:" << ukn_dw3_;
}

void stCD_TblItemNPCs::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemNPCsAddr:" << this << ",ch_name_:" << CD_SafeName(ch_name_)
		<< ",ukn_dw_:" << ukn_dw_ << ",ukn_dw1_:" << ukn_dw1_ << ",ukn_dw2_:" << ukn_dw2_ << ",ukn_dw3_:" << ukn_dw3_;
}

void stCD_TblItemChests::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemChestsAddr:" << this << ",ukn_bool_:" << ukn_bool_ << ",type_:" << type_
		<< ",name:" << CD_SafeName(ch_name_)
		<< ",ukn_b_:" << ukn_b_ << ",ukn_b1_:" << ukn_b1_ << ",ukn_b2_:" << ukn_b2_
		<< ",ukn_b3_:" << ukn_b3_ << ",ukn_w1_:" << ukn_w1_
		<< ",ukn_w_:" << ukn_w_ << ",ukn_dw2_:" << ukn_dw2_ << ",ukn_dw3_:" << ukn_dw3_
		<< ",ukn_dw4_:" << ukn_dw4_ << ",ukn_bool1_:" << ukn_bool1_
		<< ",meta_so_path_:" << CD_SafePath(meta_so_path_);
}

void stCD_TblItemStats::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemStatsAddr:" << this << ",en_name_:" << CD_SafeName(en_name_)
		<< ",ch_name_:" << CD_SafeName(ch_name_) 
		//<< ",slot_en_name_:" << CD_SafeName(slot_en_name_)
		<< ",slot_en_name1_:" << CD_SafeName(slot_en_name1_) << ",slot_ch_name_:" << CD_SafeName(slot_ch_name_)
		<< ",ukn_dw_:" << ukn_dw_ << ",ukn_dw11_:" << ukn_dw11_ << ",ukn_dw1_:" << ukn_dw1_
		<< ",ukn_dw2_:" << ukn_dw2_ << ",ukn_w_:" << ukn_w_;
}

const stCD_TblItemWorldArea* stCD_MapPinsPair::GetTransferWorldItem(enCD_kMapDifficulty difficulty) const
{
	const auto& res = GetTransferWorldItemPair(difficulty);
	return res.tbl_item_;
}

const stCD_TblItemWorldArea* stCD_MapPinsPair::GetTransferWorldItem() const
{
	const auto& res = GetTransferWorldItemPair();
	return res.tbl_item_;
}

stCD_WorldAreaPairT stCD_MapPinsPair::GetTransferWorldItemPair() const
{
	if (!IsValid())
	{
		assert(false);
		return stCD_WorldAreaPairT();
	}
	return tbl_item_->GetTransferWorldItemPair();
}

void stCD_TblItemWeaponTypes::DebugInfo() const
{
	if (!item_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "TblItemWeaponTypesAddr:" << this << ",item_type_name:" << CD_SafeName(item_.tbl_item_->item_name_)
		<< ",critical_:" << critical_ << ",speed_:" << speed_ << ",damage_min_:" << damage_min_
		<< ",damage_max_:" << damage_max_ << ",range_max_:" << range_max_ << ",ukn_dw_:" << ukn_dw_;
}

void stCD_TblItemMonsterVarieties::DebugInfo() const
{
	if (!monster_type_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "TblItemMonsterVarietiesAddr:" << this
		<< ",ch_name_:" << CD_SafeName(ch_name_)
		<< ",monster_typeAddr:" << monster_type_.tbl_item_ 
		<< ",monster_type_name:" << CD_SafeName(monster_type_.tbl_item_->en_name_);
}

void stCD_TblItemMonsterTypes::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemMonsterTypesAddr:" << this << ",en_name_:" << CD_SafeName(en_name_)
		<< ",ukn_dw_:" << ukn_dw_ << ",is_called_:" << (int)is_called_
		<< ",ukn_dw1_:" << ukn_dw1_ << ",ukn_dw2_:" << ukn_dw2_
		<< ",ukn_dw3_:" << ukn_dw3_ << ",ukn_dw4_:" << ukn_dw4_;
}

void stCD_TblItemClientStrings::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemClientStringsAddr:" << this << ",en_name:" << CD_SafeName(en_name_)
		<< ",ch_str:" << CD_SafeStr(ch_str_);
}

void stCD_TblItemClientStrings::DebugInfo(int idx) const
{
	LOG_O(Log_debug) << "TblItemClientStringsAddr:" << this
		<< ",idx:" << (void*)idx
		<< ",en_name:" << CD_SafeName(en_name_)
		<< ",ch_str:" << CD_SafeStr(ch_str_);
}

stCD_TblItemGrantedEffectsPerLevel* stCD_GrantedEffectsPairT::FindDbPerLevelBySkillGemLvl(pt_dword lvl) const
{
	struct stHelper : stCD_GrantedEffectsPerLevelPairT{
		char	buffer_[100];

		stHelper(){
			ZeroMemory(buffer_, sizeof(buffer_));
		}
	};

	stHelper res;
	SlowExecute([&res, this, lvl](){
		auto res_ptr = &res;
		auto this_ptr = this;
		auto lvl_tmp = lvl;
		auto call_addr = CD_EXE_DATA(g_c_find_GrantedEffectsPerLevelPair_);
		__asm{
			mov edx, this_ptr
			mov ecx, res_ptr
			push lvl_tmp
			call call_addr
			add esp, 4
		}
	});
	
	if (!res.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return res.tbl_item_;
}

enCD_kMapDifficulty stCD_TblItemDifficulties::GetDifficulty() const
{
	typedef std::map<const wchar_t*, enCD_kMapDifficulty, StdEx::LessChar<wchar_t> > MapT;
	struct stHelper{
		stHelper(){
#ifdef VER_GJ_			//国际服
			types_[L"Normal"] = kMD_Normal;
			types_[L"Merciless"] = kMD_Merciless;
			types_[L"Cruel"] = kMD_Cruel;
#else					//国服
			types_[L"一般"] = kMD_Normal;
			types_[L"无情"] = kMD_Merciless;
			types_[L"残酷"] = kMD_Cruel;
#endif
		}
		MapT	types_;
	};
	static stHelper helper;
	auto it_tmp = helper.types_.find(CD_SafeName(ch_name_));
	if (it_tmp == helper.types_.end())
		return kMD_Invalid;
	return it_tmp->second;
}

void stCD_DbTblPassiveSkillGraph::DebugInfo() const
{
	LOG_O(Log_debug) << "DbTblPassiveSkillGraphAddr:" << this << ",Vertices总个数：" << vertices_.size();
	for (auto& v : vertices_)
	{
		auto vertex = v.second;
		if (IsUnValidPtr(vertex))
		{
			assert(false);
			return;
		}
		vertex->DebugInfo();
	}
}

void stCD_PassiveSkillVertex::DebugInfo() const
{
	if (!edges_in_.IsValid())
	{
		assert(false);
		return;
	}
	if (!edges_out_.IsValid())
	{
		assert(false);
		return;
	}
	if (!passive_skill_.IsValid())
	{
		//assert(false);
		return;
	}
	std::ostringstream ostm_edge_in, ostm_edge_out;
	for (auto edge : edges_in_)
	{
		if (IsUnValidPtr(edge))
		{
			assert(false);
			return;
		}
		ostm_edge_in << *edge;
	}
	for (auto edge : edges_out_)
	{
		if (IsUnValidPtr(edge))
		{
			assert(false);
			return;
		}
		ostm_edge_out << *edge;
	}
	LOG_O(Log_debug) << "PassiveSkillVertexAddr:" << this 
		<< ",passive_skill_CnName:" << CD_SafeName(passive_skill_.tbl_item_->ch_name_)
		<< ",passive_skill_EnName:" << CD_SafeName(passive_skill_.tbl_item_->en_name_)
		<< ",edges_in_:" << ostm_edge_in.str() << ",edges_out_:" << ostm_edge_out.str();
}

void stCD_TblItemBuffDefinitions::DebugInfo() const
{
	LOG_O(Log_debug) << "TblItemBuffDefinitionsAddr:" << this
		<< ",en_name:" << CD_SafeName(en_name_) << ",ch_name:" << CD_SafeName(ch_name_)
		<< ",desc:" << CD_SafeStr(ch_desc_);
}

const stCD_MySkillAttrNameCont* stCD_DbSkillStatDescriptions::FindSkillAttr(pt_cwsz skill_en_name)
{
	assert(skill_en_name);
	assert(!my_hash_map_.empty());
	auto it = my_hash_map_.find(skill_en_name);
	if (it == my_hash_map_.end())
	{
		//assert(false);
		return nullptr;
	}
	return &it->second.attr_name_cont_;
}

stCD_SkillAttr* stCD_DbTblMinionSpellSkillStat::FindSkillAttr(pt_cwsz attr_name) const
{
	typedef std::hash_map<pt_cwsz, stCD_SkillAttr*, RStd::hash_compare<pt_cwsz, StdEx::LessChar<wchar_t> > > MyHashMap;
	static MyHashMap my_hash_map;
	if (my_hash_map.empty())
	{
		for (auto& v : skill_attr_hash_map_)
		{
			if (IsUnValidPtr(v.second.real_obj_))
			{
				assert(false);
				continue;
			}
			my_hash_map.insert(std::make_pair(v.first.c_str(), v.second.real_obj_));
		}
	}
	assert(attr_name);
	auto it = my_hash_map.find(attr_name);
	if (it == my_hash_map.end())
	{
		assert(!"难道该db文件被删除了？");
		return nullptr;
	}
	return it->second;
}

stCD_DbSkillStatDescriptions::MyHashMap stCD_DbSkillStatDescriptions::my_hash_map_;

bool stCD_DbSkillStatDescriptions::ResolveMyHashMap()
{
	if (my_hash_map_.empty())
	{
		auto db_tbl = GetDbTbl();
		if (!db_tbl)
		{
			assert(false);
			return false;
		}
		for (auto& v : db_tbl->skill_stat_desc_map_)
		{
			auto it = my_hash_map_.insert(std::make_pair(v.first.c_str(), stAttrNameInfo()));
			auto& ptr = it.first;
			ptr->second.attr_name_cont_.reserve(v.second.skill_attr_names_.size());
			for (auto& attr_name : v.second.skill_attr_names_)
			{
				ptr->second.attr_name_cont_.push_back(attr_name.c_str());
			}
			ptr->second.skill_name_ = v.first.c_str();
			*(pt_cwsz*)&ptr->first = ptr->second.skill_name_.c_str();
		}
	}
	return true;
}

stCD_GameMsgBase::stCD_GameMsgBase()
{
	ZeroMemory(this, sizeof(*this));
	vtbl_ = (pt_pcvoid)CD_EXE_DATA(g_va_msg_receive_obj_vtbl_);
}

bool stCD_EntityMsgInfo::InitAsSelf()
{
	ZeroMemory(this, sizeof(*this));
	auto player = stCD_GameBase::GetMe().GetPlayerEntity();
	if (!player)
	{
		assert(false);
		return false;
	}
	entity_id_ = player->id_;
	return true;
}

bool stCD_GameEntityMsg::InitAsSelf()
{
	mem_placeholder_ = 0;
	return entity_.InitAsSelf();
}

stCD_EntityMsgInfo::stCD_EntityMsgInfo()
{
	ZeroMemory(this, sizeof(*this));
}

stCD_GameEntityMsg::stCD_GameEntityMsg()
{
	mem_placeholder_ = 0;
}

void stCD_DataCraftingBenchOptions::DebugInfo() const
{
	LOG_O(Log_debug) << "-------------------";
	LOG_O(Log_debug) << "DataCraftingBenchOptionsAddr:" << this << ",title:" << CD_SafeName(title_);
	for (auto& v : require_item_type_)
	{
		v.tbl_item_->DebugInfo();
	}
	for (auto v : require_item_cnt_)
	{
		LOG_O(Log_debug) << v;
	}
}
