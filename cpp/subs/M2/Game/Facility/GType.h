#pragma once
/*
@author zhp
@date 2017/2/25 18:21
@purpose for some little types
*/
#include <Common/UsefulClass.h>
#include <WgFacility/CmnFilter.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GConst.h"
#include <CommonX/CmnX_StaticConts.h>
#include <CmnMix/Cmn_Iterator.h>
#include <StdEx/VectorEx.h>
#include <Actions/CA_Action.h>

//////////////////////////////////////////////////////////////////////////
class GameObjBase;
typedef boost::shared_ptr<GameObjBase> GameObjBasePtrT;
typedef CmnBufferVector<GameObjBasePtrT, 100> GameObjsBufferCont;
class GameObjMgrBase;
typedef boost::shared_ptr<GameObjMgrBase> GameObjMgrPtr;
typedef boost::weak_ptr<GameObjMgrBase> GameObjMgrWeakPtr;
class GameObjMgr;
class GSkillObj;
class GNpc;
class GAreaTransition;
class GWorldItem;
class GChest;
class GTranstionable;
class GItemBase;
struct stPathMarkParam;
struct stItemSlot;
class GSlotGem;
class GPackageBase;
class GItemBase;
typedef boost::shared_ptr<GItemBase> GItemBasePtr;
typedef std::vector<GItemBasePtr> GItemContT;
typedef CmnStaticVector<GItemBasePtr, CD_kBagItemArrayMaxSize> GItemStaticContT;
class GItemWearable;
class GItemEquipBase;
class GItemUsable;
class GItemFlask;
class GItemGem;
class GItemDecoration;
class GItemRing;
class GItemArmor;
class GItemWeapon;
class GItemQuest;
class GItemMgrBase;
class GPackageBase;
class GPackageWearable;
class GPackageMain;
class GPackageEquips;
class GPackageFlasks;
class GPackageMgrBase;
class GPackageWeaponBase;
class GPackageLeft;
class GPackageRight;
class GSwitchPagesMgrBase;
class GServicePage;
class FilterItemBase;
class GSkillMgr;
class GPlayerMe;
class GBuffMgr;
typedef boost::shared_ptr<GPackageBase> GPackageBasePtr;
typedef boost::weak_ptr<GPackageBase> GPackageBaseWeakPtr;
typedef CmnStaticVector<GPackageBasePtr, 15> PackagesContT;
typedef CmnStaticVector<GPackageBasePtr, 2> PackagesThinContT;
typedef std::map<enCD_InventoryType, std::vector<int>>  GemEquipConT;
typedef CmnStaticVector<GameObjType, 2> GameObjTypeThinCont;

template<typename T>
using GItemSellPriceCont = CmnStaticVector<T, 5>;

class GA_KillBase;
class GA_KillMonster;
class GA_DangerKill;
class GA_OpenObjs;
class GA_SkillBase;
class GA_Skills;
class GA_SaferMoveTo;
typedef CA_SmartConsumer GA_SmartConsumer;
typedef CA_T<GA_SmartConsumer> GA_SmartConsumerT;
typedef CA_WeakT<GA_SmartConsumer> GA_SmartConsumerWeakT;
//////////////////////////////////////////////////////////////////////////
typedef CF_Traits<GameObjBasePtrT> GFilterGameObjTraits;
typedef CF_IFilter<GFilterGameObjTraits> IFilterGameObj;
typedef IFilterGameObj::FilterPtr GFilterGameObjPtr;
typedef CF_Cont<GFilterGameObjTraits, false, const IFilterGameObj*,
	CmnStaticVector<const IFilterGameObj*, 20>
> FilterGameObjCont;
typedef CF_Ensure<FilterGameObjCont> FilterGameObjEnsure;

typedef CF_Traits<stCD_VecInt> GFilterPosTraits;
typedef CF_IFilter<GFilterPosTraits> IFilterPos;
//////////////////////////////////////////////////////////////////////////
template<typename ValueType_, ValueType_ MaxValueCnt, ValueType_ InvalidValue>
using GFilterValue = CF_Value<true, ValueType_, MaxValueCnt, InvalidValue>;

typedef GFilterValue<GameObjType, kGOT_Max, kGOT_Invalid> GFilterGameObjType;
typedef GFilterValue<enCD_ItemType, kITYPE_Max, kITYPE_Invalid> GFilterItemType;
typedef CD_Interface::FilterInventoryType FilterInventoryType;
//////////////////////////////////////////////////////////////////////////
class GF_FilterItemBased : public CF_BasedLua{
public:
	virtual bool Before(const GItemBase& obj) const { return true; }
};
typedef CF_Traits<GItemBase, GF_FilterItemBased> GFilterItemTraits;
typedef CF_IFilter<GFilterItemTraits> GIFilterItem;
typedef boost::shared_ptr<FilterItemBase> GFilterItemPtr;

typedef CF_Cont<GFilterItemTraits, false, const GIFilterItem*,
	CmnStaticVector<const GIFilterItem*, 10>
> GFilterItemCont;
typedef CF_Ensure<GFilterItemCont> GFilterItemEnsure;
//////////////////////////////////////////////////////////////////////////
template<typename IterContTag_, typename IterFilterTag_>
using GItemIteratorTraitsT = IteratorTraits<GItemBasePtr, IterContTag_, IterFilterTag_, IterFilter_SmartGetter>;
typedef IteratorTraits<GItemBasePtr>::iterator GItemIterator;
typedef IteratorTraits<GItemBasePtr>::iterator_pointer GItemIteratorPtr;

typedef IteratorTraits<GPackageBasePtr> GPackageIterTraits;
typedef GPackageIterTraits::iterator GPackageIterator;
typedef GPackageIterTraits::iterator_pointer GPackageIteratorPtr;

struct stBodyEquipsInfo{
	enCD_InventoryType		position_;
	GItemBasePtr			equip_;
};
typedef CmnStaticVector<stBodyEquipsInfo, kIT_Belt - 1> GBodyTotalEquipsCont;
struct stBodyFlaskInfo{
	stCD_VecInt		pos_;
	GItemBasePtr	flask_;
};
typedef CmnStaticVector<stBodyFlaskInfo, CD_kBodyFlaskMaxCnt> GBodyFlasksCont;
//////////////////////////////////////////////////////////////////////////
typedef CmnStaticVector<enCD_InventoryType, 2> GEquipPackageTypeCont;
typedef CmnStaticVector<enCD_ItemType, 2> GEquipItemTypeThinCont;
struct stPositionItemTypesInfo{
	enCD_InventoryType		position_;
	enWeaponCompositeCate	composite_cate_;
	GEquipItemTypeThinCont	item_types_;
};
typedef CmnStaticVector<stPositionItemTypesInfo, 2> GEquipPositionItemTypes;
//////////////////////////////////////////////////////////////////////////
template<typename T>
using GMapShortestPathsCont = CmnStaticVector<T, 2000>;

template<typename T>
using GMapAreaShortestPathsCont = CmnStaticVector<T, 20>;

template<typename T>
using GPassiveSkillPathsCont = CmnStaticVector<T, 2000>;
//////////////////////////////////////////////////////////////////////////
struct stPathMarkParam{
	int	zoom_rate_;
	int range_step_;
};

template<typename T>
class NameListVector : public NameList<T, std::vector<T> >{};
typedef NameListVector<pt_dword> NameListIdT;
typedef NameListVector<std::string> NameListStringT;

namespace GClass{
	struct stLastMapInfo{
		stCD_VecInt				last_map_size_;
		stCD_World*				last_world_;
		stCD_TblItemWorldArea*	last_world_area_;
		std::string				last_map_name_;
		int						last_area_id_;

		stLastMapInfo();
		void clear();
		friend bool operator == (const stLastMapInfo& lhs, const stLastMapInfo& rhs);
		friend bool operator != (const stLastMapInfo& lhs, const stLastMapInfo& rhs);
	};
}
//////////////////////////////////////////////////////////////////////////
struct stAreaVertex{
	GameObjBasePtrT		transitionable_;
	stCD_VecInt			transitionable_pos_;

	bool operator == (const stAreaVertex& rhs) const;
	bool IsValid() const;
	const stCD_VecInt& GetValidPos() const;
};
//////////////////////////////////////////////////////////////////////////