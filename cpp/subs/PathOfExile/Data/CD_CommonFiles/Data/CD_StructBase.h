#pragma once
/*
@author zhp
@date 2017/2/5 13:11
@purpose for struct base
*/
#include <string>
#include <ProtocolX/P_DefinesX.h>
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Offset.h"
#include "CD_Const.h"
#include <WgFacility/CmnGameStruct.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include "CD_Data.h"
#include <CommonX/CmnX_Math.h>
#include <CommonX/CmnX_Allocator.h>
#include <StdEx/XstddefEx.h>

#pragma pack(push)
#pragma pack(1)
struct stCD_Life;
struct stCD_EntityBase;
struct stCD_World;
struct stCD_EntityPlayer;
struct stCD_EntityNpc;
struct stCD_EntityWaypoint;
struct stCD_EntityAreaTransition;
struct stCD_EntityMonster;
struct stCD_AttrDbQuality;
struct stCD_AttrDbAttributeRequirements;
struct stCD_EntityAttrMods;
struct stCD_EntityAttrStack;
struct stCD_EntityAttrSockets;
struct stCD_EntityItemBased;
struct stCD_TblItemBaseItemTypes;
struct stCD_AttrDbSkillGem;
struct stCD_Render;
struct stCD_EntityNoHuman;
struct stCD_TblItemWorldArea;
struct stCD_EntityChest;
struct stCD_EntityWorldItem;
struct stCD_EntityTransitionable;
struct stCD_Actor;
struct stCD_EntityAttrUsable;
struct stCD_EntityAttrCharges;
struct stCD_EntityAttrDbFlask;
struct stCD_EntityAttrItemBase;
struct stCD_EntityAttrNPC;
struct stCD_EntityAttrClientNpc;
struct stCD_EntityAttrMonster;
struct stCD_EntityAttrDiesAfterTime;
struct stCD_EntityAttrSkillGem;
struct stCD_TblItemGrantedEffectsPerLevel;
struct stCD_EntityAttrPortal;
struct stCD_PassiveSkillVertex;
struct stCD_DbTblPassiveSkillGraph;
struct stCD_Pathfinding;
struct stCD_EntityAttrTargetable;
struct stCD_EntityAttrTransitionable;
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct stCD_GameDeque{
	typedef T* pointer_type;

	pointer_type		values_;
	int					buffer_cnt_;
	pt_dword			idx_;
	int					cnt_;
};

template<class _Kty, class _Ty, 
class _Tr = RStd::hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
using stCD_GameHash_map = GameHash_map<_Kty, _Ty, RStd::enXHashMemberTag::kBucketSizeFirst, _Tr, _Alloc>;
typedef stCD_GameHash_map<void*, void*> stCD_UknGameHash_map;

struct stCD_GameAllocatorImpl{
	static void* Malloc(pt_dword size);
	static void Free(void* ptr);
};
template<typename T>
using stCD_GameAllocator = CmnAllcator<T, stCD_GameAllocatorImpl>;
template<typename _Elem>
using stCD_SafeGameStringT = GameStringT<_Elem, std::char_traits<_Elem>, stCD_GameAllocator<_Elem> >;
typedef stCD_SafeGameStringT<wchar_t> stCD_SafeGameWString;
template<typename T>
using stCD_SafeVector = GameVector<T, stCD_GameAllocator<T> >;

typedef GameArray<void*> stCD_UknGameArray;
typedef GameArrayEx<void*> stCD_UknGameArrayEx;
typedef GameVector<void*> stCD_UknGameVector;
//////////////////////////////////////////////////////////////////////////
template<typename T>
using stCD_Vec2 = stCmnVec2<T>;
typedef stCD_Vec2<int> stCD_VecInt;
typedef stCD_Vec2<float> stCD_VecFloat;
typedef stCD_Vec2<pt_byte> stCD_VecByte;
typedef stCD_Vec2<pt_short> stCD_VecShort;
static_assert(sizeof(stCD_VecByte) == 2, "");
static_assert(sizeof(stCD_VecInt) == 4 * 2, "");
//////////////////////////////////////////////////////////////////////////
template<typename RealObjT, typename AboveT>
struct stCD_PairAdjacent{
	RealObjT*		real_obj_;
	AboveT*			above_part_;
};
typedef stCD_PairAdjacent<void, void> stCD_UknSmartPtr;

struct stCD_StringEnChPair{
	pt_cwsz			en_str_;		
	pt_cwsz			ch_str_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_StringEnChPair& v);
	void DebugInfo() const;
};

struct stCD_DbTblBase{
	CMN_CAST_TO
};

struct stCD_DbTblVtblBase : stCD_DbTblBase{
	pt_pcvoid					vtbl_;
};

template<typename TblItemT>
struct stCD_DbTblBaseT : stCD_DbTblVtblBase{
	typedef GameVector<TblItemT> TblItemContT;

	TblItemContT	tbl_items_;		//行信息

	void DebugInfo() const{
		LOG_O(Log_debug) << "DbTblAddr:" << this;
		if (tbl_items_.IsValid())
		{
			for (auto& v : tbl_items_)
				v.DebugInfo();
			LOG_O(Log_debug) << "总数：" << tbl_items_.size();
		}
		else
		{
			assert(false);
		}
	}
	void DebugInfo(int counter) const{
		LOG_O(Log_debug) << "DbTblAddr:" << this;
		if (tbl_items_.IsValid())
		{
			for (auto& v : tbl_items_)
			{
				v.DebugInfo(counter);
				++counter;
			}
			LOG_O(Log_debug) << "总数：" << tbl_items_.size();
		}
		else
		{
			assert(false);
		}
	}
};

//模型表中存放数据的vector没找到
template<typename TblItemT>
struct stCD_DbTblModelT : stCD_DbTblVtblBase{
	p_mem_placeholder(10 * 4);
	void DebugInfo() const{
		LOG_O(Log_debug) << "stCD_DbTblModelAddr:" << this;
	}
};

//value一般用在tips中的数值
struct stCD_Value{
	enCD_ValueType		type_;
	pt_int				value_;
};
//////////////////////////////////////////////////////////////////////////
//这个结构体可以被外挂使用
struct stCD_ItemGridsIdxInfo{
	//索引从0开始的
	int		left_x_idx_;		//最左边格子的x索引
	int		up_y_idx_;			//最上边格子的y索引
	int		right_x_end_idx_;	//最右边格子的x索引+1
	int		down_y_end_idx_;	//最下边格子的y索引+1

	stCD_ItemGridsIdxInfo();
	inline int GetLeftX() const{ return left_x_idx_; }
	inline int GetTopY() const{ return up_y_idx_; }
	inline int GetRightX() const{ return right_x_end_idx_ - 1; }
	inline int GetDownY() const{ return down_y_end_idx_ - 1; }
	//宽度
	inline int GetWidth() const{ return right_x_end_idx_ - left_x_idx_; }
	//高度
	inline int GetHeight() const{ return down_y_end_idx_ - up_y_idx_; }
	//物品所占格子数
	inline int GetItemGridCnt() const{ return GetWidth() * GetHeight(); }
	//遍历物品格子的索引，每次遍历返回一个索引，直到返回-1
	//若是想得到第一个索引，则cursor = 0
	int TraItemGridIdx(int& cursor, int bag_width) const;
	bool IsValid() const;
	void DebugInfo() const;
};

struct stCD_GemSlot{
	typedef GameVector<pt_byte> SlotInfoContT;
	typedef enCD_GemSlotColor GemSlotVecT[CD_kGemSlotMaxCnt];

	GemSlotVecT					slot_;
	stCD_EntityItemBased*		gems_[CD_kGemSlotMaxCnt];		//宝石镶嵌在第一个槽里面，则宝石指针放在索引0处。
	SlotInfoContT				slot_connect_info_;				//插槽连接信息，是个数组，第一个元素如果是3，则表示前3个插槽是连接在一起的。

	stCD_GemSlot();
	//得到插槽数量
	int GetSlotCnt() const;
	//得到已用插槽数量
	int GetUsedSlotCnt() const;
	//遍历镶嵌的宝石
	//cursor,游标，若要从头开始遍历，则它必须=0
	//若返回不为空，则此时cursor=index + 1，cursor表示第N个宝石，N的最小值为1，最大值为CD_kGemSlotMaxCnt
	stCD_EntityItemBased* TraInsideGem(int& cursor) const;
	void DebugInfo() const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_LifeData{
	pt_pcvoid		vtbl_;
	pt_dword		num1_;
	pt_dword		num2_;
	stCD_Life*		parent_;
	pt_dword		max_value_;
	pt_dword		cur_value_;
	pt_dword		ukn_dw_;
	pt_dword		reserved_;		//保留，对于蓝来说，就是保留的数值。
	pt_dword		ukn_dw1_;

	stCD_LifeData();
	void DebugInfo() const;
	friend std::ostream& operator << (std::ostream& ostm, const stCD_LifeData& v);
};
static_assert(sizeof(stCD_LifeData) == 0x24, "");

#ifdef VER3_
struct stCD_LifeDataHp : stCD_LifeData{
	pt_bool			ukn_bool_;
	p_mem_placeholder(3);
};
#else
typedef stCD_LifeData stCD_LifeDataHp;
#endif

struct stCD_EntityAttrDbBase{
	pt_pcvoid		vtbl_;

	CMN_CAST_TO
};

struct stCD_EntityAttrBase{
	pt_pcvoid				vtbl_;
	stCD_EntityBase*		entity_;
	
	CMN_CAST_TO
};

//很多实体属性对象里面都有一个stCD_EntityAttrDbBase指针，有的放在0xc偏移处，有的放在更大的偏移处。
struct stCD_EntityAttrBaseDb : stCD_EntityAttrBase{
	p_mem_placeholder(4);				//对于player来说，这个值是1。怀疑此处是GameArray<stCD_EntityAttrDbBase>
	stCD_EntityAttrDbBase*		attr_db_;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_Positioned : stCD_EntityAttrBase{
	p_mem_placeholder(g_so_positioned_pos_ - sizeof(stCD_EntityAttrBase));
	stCD_VecInt		pos_;
	p_mem_placeholder(4);
	stCD_VecFloat	float_pos_;

	void DebugInfo() const;
};
static_assert(g_so_positioned_pos_ == offsetof(stCD_Positioned, pos_), "");
static_assert(g_so_positioned_float_pos_ == offsetof(stCD_Positioned, float_pos_), "");
//////////////////////////////////////////////////////////////////////////
//以链表的形式保存的
struct stCD_EntityAttrInfo{
	pt_dword					array_idx_;		//属性对象的数组索引
};

struct stCD_DbTblEntity : stCD_DbTblVtblBase{
	//通过属性名得到数组的索引
	typedef stCD_GameHash_map<pt_csz, stCD_EntityAttrInfo> AttrInfoCont;
	typedef GameVector<stCD_EntityAttrDbBase*> AttrDbContT;

	p_mem_placeholder(4);
	AttrDbContT		attr_db_cont_;		//row_info_大小与stCD_EntityBase::entity_cont_一样
	AttrInfoCont	attr_idx_;

	stCD_EntityAttrInfo* FindAttrIdxInfo(pt_csz name);
	int GetAttrCnt() const;
	void DebugInfo() const;
};
static_assert(g_so_entity_attri_db_cont_ == offsetof(stCD_DbTblEntity, attr_db_cont_), "");
static_assert(g_so_get_entity_attri_info_cont_ == offsetof(stCD_DbTblEntity, attr_idx_), "");

struct stCD_Db{
	pt_pcvoid						vtbl_;
	p_mem_placeholder(4 * 3);
	GameWString						path_;	//"Metadata/Characters/Int/Int"
	pt_dword						ukn_dw_;
	stCD_DbTblBase*					db_tbl_;	//$30
	pt_dword						ukn_dw1_;	//查找stCD_Db_TblItemPair时，会从一个全局变量里取值，并覆盖掉它
	pt_dword						ref_cnt_;	//$38,数据库的引用计数，每次查找stCD_Db_TblItemPair，会+1

	void DebugInfo() const;
	void lock();
	friend std::ostream& operator << (std::ostream& ostm, const stCD_Db& v);
	CMN_CAST_TO
};
//static_assert(g_so_entity_meta_info_name_ == offsetof(stCD_Db, path_), "");
//static_assert(g_so_get_entity_attri_info_ == offsetof(stCD_Db, db_tbl_), "");
//static_assert(0x38 == offsetof(stCD_Db, ref_cnt_), "");

template<typename TblT>
struct stCD_DbBaseT : stCD_Db{
	typedef TblT DbTblType;

	inline DbTblType* GetDbTbl() const{ assert(!IsUnValidPtr(db_tbl_)); return db_tbl_->CastTo<DbTblType>(); }
	void DebugInfo() const{
		__super::DebugInfo();
		auto tbl = GetDbTbl();
		if (tbl)
			tbl->DebugInfo();
	}
	void DebugInfo(int counter) const{
		__super::DebugInfo();
		auto tbl = GetDbTbl();
		if (tbl)
			tbl->DebugInfo(counter);
	}
};

template<typename TblItemT>
struct stCD_DbT : stCD_DbBaseT<stCD_DbTblBaseT<TblItemT> >{};

template<typename TblItemT>
struct stCD_DbModelT : stCD_DbBaseT<stCD_DbTblModelT<TblItemT> >{};

//起到点位符的作用
struct stCD_TblItemUkn{
	void DebugInfo() const;
};
typedef stCD_DbT<stCD_TblItemUkn> stCD_DbUkn;

template<typename TblItemT, typename DbType_ = stCD_DbT<TblItemT> >
struct stCD_Db_TblItemPair{
	typedef DbType_ DbType;
	typedef TblItemT TblItemType;

	stCD_Db_TblItemPair(){
		db_ = nullptr;
		tbl_item_ = nullptr;
	}
	bool IsValid() const{
		if (IsUnValidPtr(db_) || IsUnValidPtr(tbl_item_))
			return false;
		return true;
	}

	DbType*			db_;
	TblItemType*	tbl_item_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemUkn> stCD_UknTblItemPair;

template<typename TblItemT, typename DbType_ = stCD_DbT<TblItemT> >
struct stCD_DbPairT{
	typedef DbType_ DbType;
	typedef TblItemT TblItemType;

	pt_cwsz			db_file_path_;		//此处的字符串与stCD_Db.path_是相同的
	DbType*			db_;
};

typedef stCD_DbPairT<stCD_TblItemUkn, stCD_DbModelT<stCD_TblItemUkn> > stCD_DbPairModelUkn;
//"Art/Models/Items/Armours/Shields/ShieldInt/ShieldI"
typedef stCD_DbModelT<stCD_TblItemUkn> stCD_DbModelArmoursShields;
typedef stCD_DbPairT<stCD_TblItemUkn, stCD_DbModelArmoursShields> stCD_DbPairArmoursShields;
//UNICODE "Metadata/EnvironmentSettings/1_1_7_3.env"
typedef stCD_DbUkn stCD_Db_Metadata_EnvironmentSettings;
//////////////////////////////////////////////////////////////////////////
struct stCD_TblItemClientStrings{
	pt_cwsz			en_name_;
	pt_cwsz			ch_str_;

	void DebugInfo() const;
	void DebugInfo(int idx) const;
};
static_assert(8 == sizeof(stCD_TblItemClientStrings), "");
typedef stCD_DbT<stCD_TblItemClientStrings> stCD_DbClientStrings;
typedef stCD_Db_TblItemPair<stCD_TblItemClientStrings> stCD_ClientStringPair;

struct stCD_TblItemStats{
	pt_cwsz				en_name_;			//"level"
	pt_dword			ukn_dw_;			//好像里面有bool值之类的
	pt_dword			ukn_dw11_;
	pt_cwsz				ch_name_;			//"等级",$c
	pt_dword			ukn_dw1_;
	pt_dword			ukn_dw2_;			//$14
	pt_word				ukn_w_;				//$18
	pt_byte				ukn_byte_;
	pt_pcvoid			ukn_ptr_;			//$1b
	pt_dword			ukn_dw4_;
	pt_cwsz				slot_en_name_;		//"item_drop_slots"
	pt_cwsz				slot_en_name1_;		//"item_drop_slots"
	pt_dword			ukn_dw5_;			//$2b
	pt_dword			ukn_dw6_;
	pt_cwsz				slot_ch_name_;		//"物品掉落槽",$33
	pt_dword			ukn_dw7_;
	pt_dword			ukn_dw8_;			//$3b
	p_mem_placeholder(3);
	pt_dword			ukn_dw9_;			//$42
	pt_dword			ukn_dw10_;
	pt_cwsz				next_state_en_name_;	//UNICODE "main_hand_weapon_type",$4a

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_stats_size_ == sizeof(stCD_TblItemStats), "");
typedef stCD_Db_TblItemPair<stCD_TblItemStats> stCD_StatsPair;
typedef stCD_DbT<stCD_TblItemStats> stCD_DbStats;
typedef GameArray<stCD_StatsPair> stCD_StatsArray;

struct stCD_TblItemAchievementItems{
};
typedef stCD_Db_TblItemPair<stCD_TblItemAchievementItems> stCD_AchievementItemsPair;
typedef GameArray<stCD_AchievementItemsPair> stCD_AchievementItemsPairArray;

//大小是0x80
struct stCD_TblItemCharacters{
	pt_cwsz			meta_path_;				//UNICODE "Metadata/Characters/Int/Int"
	pt_cwsz			profession_ch_name_;	//职业名，比如"女巫"
	pt_cwsz			meta_ao_path_;			//UNICODE "Metadata/Characters/Int/Int.ao"
	pt_cwsz			meta_act_path_;			//UNICODE "Metadata/Characters/Int/Int.act"
	p_mem_placeholder(4 * 7);
	enCD_ProfessionType		profession_type_;
	p_mem_placeholder(3);					//对齐，profession_type_其实是4个字节长度
};
static_assert(0x2c == offsetof(stCD_TblItemCharacters, profession_type_), "");
typedef stCD_Db_TblItemPair<stCD_TblItemCharacters> stCD_CharactersPair;
typedef GameArray<stCD_CharactersPair> stCD_CharactersPairArray;

struct stCD_TblItemPassiveSkill{
	pt_cwsz			en_name_;			//技能英文名
	pt_cwsz			dds_file_path_;		//熔岩之力  $+4 UNICODE "Art/2DArt/SkillIcons/passives/lavalash.dds"
	stCD_StatsArray	stats_;
	pt_dword		delta_value_[4];	//比如：增加百分之20的最大生命。总共有4个值，其含义是与上面的stats相对应的。
	pt_word			skill_id_;			//$20
	pt_word			ukn_w_;
	pt_cwsz						ch_name_;			//技能中文名
	stCD_CharactersPairArray	professions_;		//$28，所属职业，起点也是个stCD_TblItemPassiveSkill，也只有起点，这个字段才不为空。
	pt_byte				ukn_b_;
	pt_byte				ukn_b1_;
	pt_pcvoid			ukn_ptr_;		//$32
	pt_byte				ukn_b2_;
	stCD_AchievementItemsPair	achievement_item_;		//$37
	p_mem_placeholder(1);
	p_mem_placeholder(4 * 6);
	pt_cwsz					en_name1_;
	stCD_UknTblItemPair		ukn_array1_;	//$5c
	pt_bool					ukn_bool1_;		//如果为真，则不允许点该天赋吧
	p_mem_placeholder(4);
	stCD_UknGameArray		ukn_array2_;	//$69
	p_mem_placeholder(4);
	p_mem_placeholder(1);
	p_mem_placeholder(1);
	
	void DebugInfo() const;
};
static_assert(g_so_tbl_skill_item_id_ == offsetof(stCD_TblItemPassiveSkill, skill_id_), "");
static_assert(g_so_tbl_skill_item_ch_name_ == offsetof(stCD_TblItemPassiveSkill, ch_name_), "");
static_assert(g_v_tbl_item_passive_skill_size_ == sizeof(stCD_TblItemPassiveSkill), "");
typedef stCD_DbT<stCD_TblItemPassiveSkill> stCD_DbPassiveSkill;
typedef stCD_Db_TblItemPair<stCD_TblItemPassiveSkill> stCD_TblItemPassiveSkillPair;
//skill_id到stCD_TblItemPassiveSkillPair的映射
typedef stCD_GameHash_map<pt_word, stCD_TblItemPassiveSkillPair> stCD_PassiveSkillHashMap;


struct stCD_TblItemTags{
	pt_cwsz		en_name_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemTags> stCD_TagsPair;
typedef GameArray<stCD_TagsPair> stCD_TagsPairArray;

struct stCD_TblItemDropPool{
};
typedef stCD_Db_TblItemPair<stCD_TblItemDropPool> stCD_DropPoolPair;
typedef GameArray<stCD_DropPoolPair> stCD_DropPoolPairArray;

struct stCD_TblItemItemClasses{
	pt_cwsz			class_name_;		//英文名
	pt_cwsz			ch_name_;			//中文名
	pt_cwsz			base_class_name_;	//父类名

	void DebugInfo() const;
};
typedef stCD_DbT<stCD_TblItemItemClasses> stCD_DbItemClasses;
typedef stCD_Db_TblItemPair<stCD_TblItemItemClasses> stCD_ItemClassesPairT;
typedef GameArrayEx<stCD_ItemClassesPairT> stCD_ItemClassesPairArrayEx;

struct stCD_TblItemModSellPriceTypes{
	/*
	"Low"
	"Medium"
	"High"
	"VeryHigh"
	"Special"
	"UniqueLow"
	"UniqueMedium"
	"UniqueHigh"
	"UniqueVeryHigh"
	"Kingmaker"
	"BreachFire"
	"BreachCold"
	"BreachLightning"
	"BreachPhysical"
	"BreachChaos"
	"BreachFireUnleashed"
	"BreachColdUnleashed"
	"BreachLightningUnleashed"
	"BreachPhysicalUnleashed"
	"BreashChaosUnleashed"
	*/
	pt_cwsz		price_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemModSellPriceTypes> stCD_ModSellPriceTypesPair;
typedef GameArray<stCD_ModSellPriceTypesPair> stCD_ModSellPriceTypesArray;
static_assert(g_so_tbl_item_mod_sell_price_type_size_ == sizeof(stCD_TblItemModSellPriceTypes), "");

struct stCD_TblItemModType{
	pt_cwsz							mod_en_name_;			//UNICODE "IncreasedLife"
	stCD_ModSellPriceTypesArray		mod_sell_price_type_;
};
static_assert(g_so_tbl_item_mod_type_size_ == sizeof(stCD_TblItemModType), "");
typedef stCD_Db_TblItemPair<stCD_TblItemModType> stCD_ModTypePair;

struct stCD_TblItemMods{
	pt_cwsz				attr_en_name_;			//UNICODE "Strength1"
	pt_dword			ukn_dw_;				//好像id啊
	stCD_ModTypePair	mod_type_;
	stCD_StatsArray		stats_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemMods> stCD_ModsPair;
typedef GameArray<stCD_ModsPair> stCD_ModsPairArray;
typedef GameArrayEx<stCD_ModsPair> stCD_ModsPairArrayEx;

struct stCD_TblItemBaseItemTypes{
	pt_cwsz						meta_path1_;
	stCD_ItemClassesPairT		item_class_pair_;
	pt_dword					grid_width_;	//格子宽度
	pt_dword					grid_height_;	//格子高度
	pt_cwsz						item_name_;		//UNICODE "磨刀石"
	pt_cwsz						meta_path2_;	//$18
	pt_dword					ukn_dw_;
	stCD_UknTblItemPair			flavour_text_;	//UNICODE "Data/FlavourText.dat"
	stCD_ModsPairArray			mods_;			//$28
	pt_dword					ukn_dw1_;
	stCD_UknTblItemPair			sound_effects_pair_;	//$34,"Data/SoundEffects.dat"
	p_mem_placeholder(g_so_tbl_item_base_item_type_size_ - g_so_tbl_item_base_item_type_db_sound_effects_ - 8);

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_base_item_type_item_name_ == offsetof(stCD_TblItemBaseItemTypes, item_name_), "");
static_assert(g_so_tbl_item_base_item_type_db_sound_effects_ == offsetof(stCD_TblItemBaseItemTypes, sound_effects_pair_), "");
static_assert(g_so_tbl_item_base_item_type_size_ == sizeof(stCD_TblItemBaseItemTypes), "");
typedef stCD_DbT<stCD_TblItemBaseItemTypes> stCD_DbBaseItemTypes;
typedef stCD_Db_TblItemPair<stCD_TblItemBaseItemTypes> stCD_BaseItemTypesPairT;
typedef GameArray<stCD_BaseItemTypesPairT> stCD_BaseItemTypesArray;

struct stCD_TblItemWeaponTypes{
	stCD_BaseItemTypesPairT		item_;
	pt_int						critical_;
	pt_int						speed_;
	pt_int						damage_min_;
	pt_int						damage_max_;
	pt_int						range_max_;		//武器范围
	pt_int						ukn_dw_;

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_weapon_types_size_ == sizeof(stCD_TblItemWeaponTypes), "");
typedef stCD_DbT<stCD_TblItemWeaponTypes> stCD_DbWeaponTypes;

struct stCD_TblItemItemVisualIdentity{
};
typedef stCD_Db_TblItemPair<stCD_TblItemItemVisualIdentity> stCD_TblItemItemVisualIdentityPair;

struct stCD_TblItemNPCShop{
	pt_cwsz					npc_en_name_;
	pt_dword				ukn_dw_;		//40
	stCD_TagsPairArray			tags_;
	stCD_UknGameArray		ukn_array_;
	stCD_DropPoolPairArray		drop_pool_;		//offset=0x18
	stCD_UknGameArray		ukn_id_array_;	
	pt_dword				ukn_dw1_;		//offset=0x28
	stCD_BaseItemTypesArray	items_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemNPCShop> stCD_NPCShopPair;
static_assert(g_so_tbl_item_npc_shop_size_ == sizeof(stCD_TblItemNPCShop), "");

struct stCD_TblItemNPCAudio{
	pt_cwsz			en_name_;
};
typedef stCD_Db_TblItemPair<stCD_TblItemNPCAudio> stCD_NPCAudioPair;

struct stCD_TblItemTextAudio{
	pt_cwsz						en_name_;
	stCD_TblItemCharacters		character_;
	pt_cwsz						say_;				//UNICODE "我亲眼看到你放倒了西拉克, 女巫. 虽然你的黑暗技艺让你成为被流放的一份子, 然而, 如果没有这样的"
	pt_cwsz						ogg_path_;			//UNICODE "Audio/Dialogue/NPC/Nessa/Ne01c.ogg"
};
typedef stCD_Db_TblItemPair<stCD_TblItemNPCAudio> stCD_TextAudioPair;
typedef GameArray<stCD_TextAudioPair> stCD_TextAudioArray;

struct stCD_TblItemNPCs{
	pt_cwsz			meta_path_name_;			//"Metadata/NPC/Act1/Tarkleigh"
	pt_cwsz			ch_name_;					//npc中文名
	pt_cwsz			meta_npc_path_;				//"Metadata/NPC/NPC"
	pt_dword		ukn_dw_;					//0x3e1,offset=0xc
	pt_dword		ukn_dw1_;
	stCD_UknGameArray	ukn_array_;
	pt_dword		ukn_dw2_;					//0x3e1,offset=0x1c
	pt_dword		ukn_dw3_;
	stCD_NPCShopPair	npc_shop_;
	stCD_NPCAudioPair	npc_audio_;				//offset=0x2c
	stCD_NPCAudioPair	npc_audio1_;

	void DebugInfo() const;
};
typedef stCD_Db_TblItemPair<stCD_TblItemNPCs> stCD_NPCsPair;
static_assert(g_so_tbl_item_npcs_size_ == sizeof(stCD_TblItemNPCs), "");

struct stCD_TblItemTopologies{

};
typedef stCD_Db_TblItemPair<stCD_TblItemTopologies> stCD_TopologiesPair;
typedef GameArray<stCD_TopologiesPair> TopologiesPairArray;

struct stCD_TblItemSoundEffects{

};
typedef stCD_Db_TblItemPair<stCD_TblItemSoundEffects> stCD_SoundEffectsPair;
typedef GameArray<stCD_SoundEffectsPair> SoundEffectsPairArray;

struct stCD_TblItemDifficulties{
	pt_cwsz			en_name_;			//UNICODE "Normal"
	pt_dword		ukn_dw_;
	pt_dword		ukn_dw1_;
	pt_cwsz			ch_name_;			//$+C UNICODE "一般"，中文名以这个为准
	pt_cwsz			ch_name1_;			//$+10 UNICODE "一般"
	enCD_kMapDifficulty GetDifficulty() const;
};
static_assert(0x14 == sizeof(stCD_TblItemDifficulties), "");
typedef stCD_Db_TblItemPair<stCD_TblItemDifficulties> stCD_TblItemDifficultiesPair;

struct stCD_TblItemWorldArea{
	typedef GameArray<stCD_TblItemWorldArea*> AreaContT;

	pt_cwsz			en_area_name_;		//英文区域名，相同中文名的地图是由不同的WorldArea组成的，其中这个英文名是唯一的
	pt_cwsz			ch_map_name_;		//$4,中文地图名
	pt_dword		chapter_;			//$8,怀疑是章节id，第1章是1，第2章是2
	bool			is_town_;			//$c
	bool			ukn_bool_;			//$d
	AreaContT		next_area_;			//$e, 通向的下一区域，通常是下一个地图,offset=0xe
#ifndef VER3_
	pt_byte			ukn_byte_;			//$16
	pt_dword		ukn_dw1_;			//$17
#endif
	pt_dword		ukn_dw2_;			//$1b
	pt_dword		world_area_id_;			//$1a,id，传送点发包的时候用得到
	pt_dword		ukn_dw3_;			//这里的偏移有问题吧
	pt_dword		ukn_dw5_;			//$23
	pt_cwsz			loading_image_path_;		//"Art/Textures/Interface/LoadingImages/Act1.dds",$26
	pt_dword		ukn_dw6_;
	stCD_UknGameArray	ukn_id_array_;
	pt_dword		ukn_dw7_;
	TopologiesPairArray		topologies_;		//$3a
	stCD_TblItemWorldArea*				belong_town_;		//所属城镇,$42
#ifndef VER3_
	stCD_TblItemDifficultiesPair		difficult_;
#endif
	p_mem_placeholder(4 * 3);
	stCD_UknGameArray	ukn_array_;			//$52
	stCD_UknGameArray	ukn_array1_;
	stCD_UknGameArray	ukn_array2_;
	stCD_UknGameArray	ukn_array3_;
	pt_byte			ukn_byte1_;				//$72
	stCD_UknGameArray	ukn_array4_;		//$73
	p_mem_placeholder(4 * 4);				//$7b
	stCD_UknGameArray	ukn_array5_;			//$8b
	p_mem_placeholder(4 * 2);
	stCD_UknGameArray	ukn_array6_;			//$9b
	p_mem_placeholder(4 * 3);
	stCD_UknGameArray	ukn_id_array1_;			//$af
	pt_byte				ukn_byte2_;				//$b7
	p_mem_placeholder(4 * 2);
	stCD_UknGameArray	ukn_array8_;			//$c0
	pt_dword			ukn_dw23_;		//$c8
	pt_byte				ukn_byte3_;		//$cc
	pt_dword			ukn_dw24_;			//$cd
	pt_bool				ukn_bool1_;			//$d1,丢物品的时候会判断这个字段
	p_mem_placeholder(4 * 3);			//$d2
	stCD_UknGameArray	ukn_array9_;			//offset=0xeb
	pt_dword			ukn_dw28_;
	pt_dword			ukn_dw29_;
	pt_dword			ukn_dw30_;
	stCD_UknGameArray	ukn_array10_;			//offset=0xff
	p_mem_placeholder(4 * 9 + 3);
	stCD_UknGameArray	ukn_array11_;		//$12e
	p_mem_placeholder(4 * 5);
	stCD_UknGameArray	ukn_array12_;		//$14a
	p_mem_placeholder(4 * 2 + 2);
	stCD_UknGameArray	ukn_array13_;		//$15c
	p_mem_placeholder(4 * 2);
	stCD_SoundEffectsPair	sound_effects_;		//$16c
	pt_pcvoid				ukn_ptr_;
	p_mem_placeholder(8);
/*
#ifdef VER_GJ_
	p_mem_placeholder(0xc);
#endif*/
#ifdef VER3_
	p_mem_placeholder(0xc);
	p_mem_placeholder(4 * 2);
#endif
#ifdef VER3_
	p_mem_placeholder(8);
#endif
	enCD_kMapDifficulty GetDiffLvl() const;
	void DebugInfo() const;		//0x180
};
static_assert(g_so_world_area_area_id_ == offsetof(stCD_TblItemWorldArea, world_area_id_), "");
#ifndef VER3_
static_assert(g_so_tbl_item_world_area_ukn_ == offsetof(stCD_TblItemWorldArea, ukn_array5_), "");
#endif
static_assert(g_so_tbl_item_world_area_size_ == sizeof(stCD_TblItemWorldArea), "");
typedef stCD_DbT<stCD_TblItemWorldArea> stCD_DbWorldArea;
typedef stCD_Db_TblItemPair<stCD_TblItemWorldArea> stCD_WorldAreaPairT;
typedef GameArray<stCD_WorldAreaPairT> stCD_WorldAreaPairArray;

struct stCD_TblItemMusic{
};
typedef stCD_Db_TblItemPair<stCD_TblItemMusic> stCD_TblItemMusicPair;
typedef GameArray<stCD_TblItemMusicPair> stCD_TblItemMusicPairArray;

struct stCD_TblItemEnvironments{
	pt_cwsz						en_name_;						//UNICODE "1_1"
	pt_cwsz						path_audio_sound_effects_;		//$+4 UNICODE "Audio/Sound Effects/Ambient/1_1_1_The_Twilight_Str"
	stCD_TblItemMusicPairArray	musics_;
	pt_cwsz						meta_env_setting_;				//$+10 UNICODE "Metadata/EnvironmentSettings/1_1_1.env"
	pt_pcvoid					ukn_ptr_;
	pt_dword					ukn_dw_;
	pt_pcvoid					ukn_ptrs_[3];
};
static_assert(0x28 == sizeof(stCD_TblItemEnvironments), "");
typedef stCD_DbT<stCD_TblItemEnvironments> stCD_DbEnvironments;
typedef stCD_Db_TblItemPair<stCD_TblItemEnvironments> stCD_TblItemEnvironmentsPair;

struct stCD_TblItemCurrencyItems{

};
typedef stCD_Db_TblItemPair<stCD_TblItemCurrencyItems> stCD_CurrencyItemsPair;
typedef GameArray<stCD_CurrencyItemsPair> stCD_CurrencyItemsPairArray;

struct stCD_TblItemNPCMaster{
	pt_cwsz			en_name_;		//UNICODE "DexIntMaster"
	pt_cwsz			meta_path_;		//UNICODE "Metadata/NPC/Missions/Hideout/DexInt"
	pt_word			ukn_w_;
	stCD_ModsPair	mods_;			//offset=0xa
	pt_byte			ukn_byte_;
	pt_cwsz			meta_path1_;	//UNICODE "Metadata/NPC/Missions/Hideout/DexIntDemo", offset=13
	stCD_WorldAreaPairArray			world_areas_;
	pt_dword						item_cnt_;				//猜测是奖励的物品数量
	stCD_AchievementItemsPair		achievement_items_;
	stCD_TagsPairArray				tags_;				//offset=0x2b
	stCD_UknGameArray				ukn_id_array_;
	stCD_DropPoolPairArray			drop_pool_;			//offset=0x3b
	stCD_UknGameArray				ukn_id_array1_;
	stCD_CurrencyItemsPair			currency_items_;
	stCD_AchievementItemsPair		achievement_items1_;
	stCD_AchievementItemsPairArray	achievement_items_array_;
};
static_assert(g_so_tbl_item_npc_master_size_ == sizeof(stCD_TblItemNPCMaster), "");

struct stCD_TblItemNPCMasterExperiencePerLevel{

};

typedef GameArray<pt_dword> stCD_QuestIdArrayT;
struct stCD_TblItemNPCTalk{
	stCD_NPCsPair			npc_;
	pt_dword				npc_panel_menu_item_idx_;		//从0开始的
	pt_cwsz					npc_panel_menu_item_name_;		//"初次见面"
	stCD_QuestIdArrayT		ukn_id_array_;
	stCD_QuestIdArrayT		branch_id_;		//offset=0x18，与QuestState表里的brach_id含义一样。
	stCD_QuestIdArrayT		ukn_id_array1_;
	pt_pcvoid				ukn_ptr_;		//上面这个array跟这个指针好像是个vector
	pt_dword				ukn_dw1_;		//offset=0x2c
	pt_dword				talk_state_;		//$30，npc面板根据该值来判断是否显示该talk
	pt_dword				ukn_dw3_;
	pt_dword				ukn_dw4_;
	pt_dword				ukn_dw5_;
	stCD_UknGameArray		ukn_array_;		//offset=0x40
	stCD_TextAudioArray		text_audio_;
	pt_pcvoid				ukn_ptr1_;		//像是上面的字段的缓冲区结束地址
	pt_dword				ukn_dw6_;
	pt_word					ukn_w_;
	pt_pcword				item_ch_name_;		//offset=0x5a，UNICODE "医药箱"
	pt_dword				ukn_dw7_;
	pt_pcword				item_ch_name1_;
	pt_dword				ukn_dw8_;
	pt_dword				ukn_dw9_;
	pt_dword				ukn_dw10_;
	pt_dword				ukn_dw11_;
	pt_pcword				item_ch_name2_;		//offset=0x7a
	pt_dword				ukn_dw12_;
	pt_dword				ukn_dw13_;
	pt_byte					ukn_byte_;
	pt_pcword				item_ch_name3_;		//offset=0x83
};
typedef stCD_Db_TblItemPair<stCD_TblItemNPCTalk> stCD_NPCTalkPair;
static_assert(g_so_tbl_item_npc_talk_size_ == sizeof(stCD_TblItemNPCTalk), "");

//可能存放了传送门、传送点传送的地方
struct stCD_TblItemMapPins{
	typedef GameArray<stCD_WorldAreaPairT>	WorldAreaCont;

	pt_cwsz					en_map_name_;
	int						pos_x_;		//可能是小地图的尺寸之类的吧
	int						pos_y_;
	//传送点所在区域
	stCD_WorldAreaPairT		waypoint_world_area_;		//若是某地图有可被传送过去，则该值不为空，否则为0
	WorldAreaCont			world_areas_;
	pt_cwsz					map_ch_name_;
	pt_cwsz					format_desc_;			//offset=0x20
	stCD_UknGameArray		ukn_array_;
	pt_dword				ukn_dw_;				//offset=0x2c
	WorldAreaCont			world_areas1_;
	WorldAreaCont			world_areas2_;
	stCD_WorldAreaPairT		world_area_1_;			//offset=0x40
	stCD_WorldAreaPairT		world_area_2_;
	pt_cwsz					en_map_name1_;			//与en_map_name_的字符相同，但是地址的值不相同,offset=0x50
	pt_dword				ukn_dw1_;
	WorldAreaCont			world_ares3_;
	stCD_UknGameArray		ukn_ids_;

	void DebugInfo() const;
	//gj3.0
	stCD_WorldAreaPairT GetTransferWorldItemPair() const;
};
static_assert(g_so_tbl_item_map_pins_size_ == sizeof(stCD_TblItemMapPins), "");
typedef stCD_DbT<stCD_TblItemMapPins> stCD_DbMapPins;
struct stCD_MapPinsPair : stCD_Db_TblItemPair<stCD_TblItemMapPins>{
	const stCD_TblItemWorldArea* GetTransferWorldItem(enCD_kMapDifficulty area_type) const;
	const stCD_TblItemWorldArea* GetTransferWorldItem() const;
	stCD_WorldAreaPairT GetTransferWorldItemPair(enCD_kMapDifficulty area_type) const;
	stCD_WorldAreaPairT GetTransferWorldItemPair() const;
};

//这几个dword应该是标志
struct stCD_TblItemMapConnections{
	stCD_MapPinsPair		map_dst_;
	stCD_MapPinsPair		map_src_;
	pt_dword				ukn_dw_;		//==1,3e1
	pt_cwsz					dst_map_condition_;		//$+14 "你必须先完成任务【大门口的敌人】. "
	pt_dword				ukn_dw1_;		//==2,3e1
	pt_dword				ukn_dw2_;		//0x3e1
	pt_dword				ukn_dw3_;		//0x3e1,offset=0x20

	void DebugInfo() const;
};
typedef stCD_DbT<stCD_TblItemMapConnections> stCD_DbMapConnections;
typedef stCD_Db_TblItemPair<stCD_TblItemMapConnections> stCD_MapConnectionsPair;
static_assert(g_so_tbl_item_map_connections_size_ == sizeof(stCD_TblItemMapConnections), "");

struct stCD_TblItemQuest{
	pt_cwsz		en_name_;				//2291D700 UNICODE "a1q1"
	pt_dword	chapter_id_;				//第几章，有的任务存在于多个章节中
	pt_cwsz		ch_name_;				//2291D708 UNICODE "大门口的敌人"
	pt_dword	flag_;					//应该是标志
	pt_cwsz		dds_file_path_;			//2291D710 UNICODE "Art/2DArt/QuestIcons/hillock.dds"
	pt_dword	sub_id_;				//章节内的任务id，相同的任务名，sub_id相同
	bool		is_branch_line_;		//是否是支线
	p_mem_placeholder(3);
	pt_dword	ukn4_;
	pt_dword	ukn5_;					//这个字段可能是由两个word组成的

	void DebugInfo() const;
};
static_assert(sizeof(stCD_TblItemQuest) == g_so_tbl_item_quest_size_, "");
typedef stCD_Db_TblItemPair<stCD_TblItemQuest> stCD_QuestPair;
typedef stCD_DbT<stCD_TblItemQuest> stCD_DbQuest;

struct stCD_TblItemQuestStates{
	typedef stCD_Db_TblItemPair<stCD_TblItemQuest> QuestPairT;
	typedef GameArray<stCD_MapPinsPair> MapPinsCont;
	typedef GameArray<pt_cwsz> QuestPromptCont;

	QuestPairT				quest_pair_;
	pt_dword				quest_state_;		//任务状态，完成状态是0，从开始做某个任务，到完成状态，数值是依次减1的。
	stCD_QuestIdArrayT		branch_ids_;		//数组，存放了分支任务id
	stCD_UknGameArray		ukn_array_;			//一直=0,$14
	//打开世界地图面板，下面显示的任务详细描述信息
	pt_cwsz					state_desc_;		//$+1C，详细详述，比如 "任务完成 - 你已经击倒了西拉克而且从塔格拉那里取得任务奖励. "
	bool					is_complete_state_;	//是否完成状态,$20
	//任务的总体提示吧
	pt_cwsz					simple_prompt_;		//$21,状态的简要提示，比如"任务完成"、"击败海妖.莫薇儿"、"与奈莎交谈"、"回到城镇"
	MapPinsCont				map_pins_;			//offset=0x25
	pt_dword				ukn_dw1_;			//offset=0x2d
	//状态提示，显示在右边的那个面板中的
	QuestPromptCont			quest_prompt_;
	MapPinsCont				map_pins1_;			//$39
	stCD_UknGameArrayEx		ukn_arrayex_;		//$41,这个数组都是为空的
	pt_byte					ukn_byte_;			//$4d
	stCD_UknGameArray		ukn_ids_;

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_quest_states_desc_ == offsetof(stCD_TblItemQuestStates, state_desc_), "");
static_assert(g_so_tbl_item_quest_states_size_ == sizeof(stCD_TblItemQuestStates), "");
typedef stCD_Db_TblItemPair<stCD_TblItemQuestStates> stCD_QuestStatesPair;

//里面没有id字段，id可能就是数组的索引。
struct stCD_TblItemActiveSkills{
	typedef GameArray<pt_dword> ValueContT;
	typedef GameArray<pt_pcvoid> PtrContT;

	pt_cwsz			en_name_;			//UNICODE "double_strike"
	pt_cwsz			ch_name_;			//UNICODE "双重打击"
	pt_cwsz			use_desc_;			//技能使用的描述	 UNICODE "使用你的主手近战武器对着目标进行两次快速的攻击."
	pt_cwsz			en_name1_;			//$c,UNICODE "DoubleStrike"
	pt_cwsz			dds_file_path_;		//UNICODE "Art/2DArt/SkillIcons/icondoubleswing.dds"
	ValueContT		ukn_ids1_;
	ValueContT		ukn_ids2_;
	stCD_ItemClassesPairArrayEx		item_classes_;			//$24，技能要求的武器类型
	pt_cwsz			http_image_path_;	//UNICODE "http://s3.amazonaws.com/pathofexile/image/skills/s"
	pt_byte			ukn_byte1_;			
	pt_cwsz			en_name2_;			//$35
	pt_byte			ukn_byte2_;			//$39
	pt_dword		ukn_dw1_;		//1,$3a
	pt_byte			ukn_byte3_;		//1,$3e
	stCD_StatsArray		status1_;		//3,$3f
	stCD_StatsArray		status2_;
	stCD_UknGameArray	ukn_array_;

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_active_skill_dds_file_path_ == offsetof(stCD_TblItemActiveSkills, dds_file_path_), "");
static_assert(g_so_tbl_item_active_skill_size_ == sizeof(stCD_TblItemActiveSkills), "");
typedef stCD_Db_TblItemPair<stCD_TblItemActiveSkills> stCD_ActiveSkillPairT;

typedef stCD_StringEnChPair stCD_TblItemGemTags;
typedef stCD_DbT<stCD_TblItemGemTags> stCD_DbGemTags;

struct stCD_TblItemGrantedEffects{
	typedef GameVector<pt_byte> ByteContT;

	pt_cwsz						en_name_;
	pt_dword					ukn_dw_;
	pt_byte						ukn_byte_;
	pt_pcvoid					ukn_ptr_;		//$9,这两个指针相等
	pt_pcvoid					ukn_ptr1_;		//$d
	pt_dword					ukn_dw1_;		//$11
	pt_dword					ukn_dw2_;		//$15
#ifdef VER3_
	p_mem_placeholder(4 * 2);
#endif
	pt_cwsz						en_name1_;			//$19,UNICODE "MonsterFireball"
	pt_dword					ukn_dw3_;
	pt_cwsz						en_name2_;			//UNICODE "MonsterFireball"
	pt_pcvoid					ukn_ptr2_;
	pt_dword					ukn_dw4_;
	pt_pcvoid					ukn_ptr3_;
	pt_word						ukn_w_;
	pt_dword					ukn_dw6_;
	pt_dword					ukn_dw5_;
	stCD_ActiveSkillPairT		active_skill_pair_;
	pt_byte						ukn_byte1_;
#ifdef VER3_
	p_mem_placeholder(1);
#endif

	//如果返回空，则不是主动技能
	inline stCD_TblItemActiveSkills* GetActiveSkill() const{ if (IsUnValidPtr(active_skill_pair_.tbl_item_)) return nullptr; return active_skill_pair_.tbl_item_; }
	void DebugInfo() const;
};
static_assert(g_so_tbl_item_granted_effects_active_skill_pair_ == offsetof(stCD_TblItemGrantedEffects, active_skill_pair_), "");
static_assert(sizeof(stCD_TblItemGrantedEffects) == g_so_tbl_item_granted_effects_size_, "");
typedef stCD_DbT<stCD_TblItemGrantedEffects> stCD_DbGrantedEffects;
typedef GameArray<stCD_TblItemGrantedEffects> stCD_TblItemGrantedEffectsArray;
typedef GameArrayEx<stCD_TblItemGrantedEffects> stCD_TblItemGrantedEffectsArrayEx;
struct stCD_GrantedEffectsPairT : stCD_Db_TblItemPair<stCD_TblItemGrantedEffects>{
	stCD_TblItemGrantedEffectsPerLevel* FindDbPerLevelBySkillGemLvl(pt_dword lvl) const;
};

//CD_kFileDbPath_GrantedEffectsPerLevel
struct stCD_TblItemGrantedEffectsPerLevel{
	stCD_GrantedEffectsPairT			granted_effects_;
	pt_dword							ukn_dw_;
	stCD_StatsArray						stats_;		//$c
	p_mem_placeholder(g_so_GrantedEffectsPerLevel_require_lvl_ - 0xc - sizeof(stCD_StatsArray));
	pt_dword							require_lvl_;		//需求等级,$3c
	pt_dword							ukn_dw1_;			//数值跟require_lvl_一样
	stCD_StatsArray						stats1_;	//$44
	stCD_UknGameArray					ukn_ids_;	//$4c
	p_mem_placeholder(6 * 4);		//都是数值
	stCD_StatsArray						stats2_;	//$6c
	p_mem_placeholder(8 * 4);	//都是数值
	pt_byte								ukn_b_;		//$94
};
static_assert(g_so_GrantedEffectsPerLevel_require_lvl_ == offsetof(stCD_TblItemGrantedEffectsPerLevel, require_lvl_), "");
#ifndef VER3_
static_assert(g_so_tbl_item_granted_effects_per_level_size_ == sizeof(stCD_TblItemGrantedEffectsPerLevel), "");
#endif
typedef stCD_DbT<stCD_TblItemGrantedEffectsPerLevel> stCD_DbGrantedEffectsPerLevel;
typedef stCD_Db_TblItemPair<stCD_TblItemGrantedEffectsPerLevel> stCD_GrantedEffectsPerLevelPairT;

//宝石的名字跟技能的名字是一样的，所以有了物品名就可以得到相应的技能了。
struct stCD_TblItemSkillGems{
	typedef stCD_Db_TblItemPair<stCD_TblItemGemTags> GemTagPairT;
	typedef GameArray<GemTagPairT> GemTagInfoT;

	stCD_BaseItemTypesPairT			item_pair_;
	stCD_GrantedEffectsPairT		granted_effects_pair_;
	pt_dword						ukn_dw1_;
	pt_dword						ukn_dw2_;
	pt_dword						ukn_dw3_;
	GemTagInfoT						gem_tag_info_;				//宝石名字下面的灰色字体，投射物、法术、冰霜等
	stCD_BaseItemTypesPairT			item_pair1_;				//有的这个属性有效，有的这个属性为空
	pt_byte							ukn_byte_;

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_skill_gems_gem_tag_ == offsetof(stCD_TblItemSkillGems, gem_tag_info_), "");
static_assert(sizeof(stCD_TblItemSkillGems) == g_so_tbl_item_skill_gems_size_, "");
typedef stCD_Db_TblItemPair<stCD_TblItemSkillGems> stCD_SkillGemsPairT;

struct stCD_DbSkillGems : stCD_DbT<stCD_TblItemSkillGems>{
	stCD_TblItemSkillGems* FindByBaseItemType(stCD_TblItemBaseItemTypes* base_item_ptr) const;
};

struct stCD_TblItemChests{
	pt_cwsz				meta_path_;		//UNICODE "Metadata/Chests/ProdWounded1"
	bool				ukn_bool_;
	pt_dword			type_;			//$5,猜测是类型
	pt_cwsz				ch_name_;		//UNICODE "流放者的遗体"
	pt_cwsz				meta_so_path_;	//UNICODE "Metadata/Chests/DeadPlayerProds/ProdWounded1.ao"
	bool				ukn_b_;			//$11
	bool				ukn_b1_;		//可能是任务标志
	bool				ukn_b2_;		//$13，可能是类型
	bool				ukn_b3_;
	pt_word				ukn_w1_;
	pt_word				ukn_w_;
	pt_dword			ukn_dw2_;		//$19
	stCD_UknGameArray	ukn_array_;		//$1d
	stCD_UknGameArray	ukn_array1_;
	pt_dword			ukn_dw3_;
	pt_dword			ukn_dw4_;
	bool				ukn_bool1_;		//$35
	stCD_UknGameArray	ukn_array2_;	//$36
	stCD_UknGameArray	ukn_array3_;
	pt_dword			ukn_dw5_;
	pt_dword			ukn_dw6_;
	stCD_UknGameArray	ukn_array4_;	//$4e
	pt_dword			ukn_dw7_;
	pt_dword			ukn_dw8_;
	pt_dword			ukn_dw9_;
	pt_dword			ukn_dw10_;
	pt_dword			ukn_dw11_;
	pt_dword			ukn_dw12_;
	pt_cwsz				next_chest_meta_path_;		//UNICODE "Metadata/Chests/ProdWounded2"
	pt_dword			ukn_dw13_;
	pt_dword			ukn_dw14_;

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_chests_size_ == sizeof(stCD_TblItemChests), "");
typedef stCD_Db_TblItemPair<stCD_TblItemChests> stCD_TblItemChestsPair;
typedef stCD_DbT<stCD_TblItemChests> stCD_DbChests;

struct stCD_TblItemBloodTypes{

};
typedef stCD_Db_TblItemPair<stCD_TblItemBloodTypes> stCD_TblItemBloodTypesPair;

struct stCD_TblItemMonsterResistances{

};
typedef stCD_Db_TblItemPair<stCD_TblItemMonsterResistances> stCD_TblItemMonsterResistancesPair;

struct stCD_TblItemMonsterTypes{
	pt_cwsz				en_name_;		//"BloodMonkey"
	pt_dword			ukn_dw_;
	bool				is_called_;			//$8，是否是召唤物
	pt_dword			ukn_dw1_;
	pt_dword			ukn_dw2_;
	pt_dword			ukn_dw3_;
	pt_dword			ukn_dw4_;		//$15
	stCD_TagsPairArray	tags_;			//$19
	stCD_TblItemMonsterResistancesPair		resistance_;	//$21

	void DebugInfo() const;
};
static_assert(g_so_tbl_item_monster_types_size_ == sizeof(stCD_TblItemMonsterTypes), "");
typedef stCD_Db_TblItemPair<stCD_TblItemMonsterTypes> stCD_TblItemMonsterTypesPair;
typedef stCD_DbT<stCD_TblItemMonsterTypes> stCD_DbMonsterTypes;

//怪物品种
struct stCD_TblItemMonsterVarieties{
	pt_cwsz							meta_path_;			//"Metadata/Monsters/Bandits/BanditBossHeavyStrike_"
	stCD_TblItemMonsterTypesPair	monster_type_;
	pt_dword			ukn_dw1_;
	pt_dword			ukn_dw2_;
	pt_dword			ukn_dw3_;
	pt_dword			ukn_dw4_;
	pt_cwsz				meta_act_path_;				//$1c
	pt_cwsz				meta_ao_path_;
	pt_cwsz				meta_path1_;
	stCD_UknGameArray	ukn_array_;					//$28
	stCD_UknGameArrayEx	ukn_array1_;
	pt_dword			ukn_dw5_;			//$3c
	pt_dword			ukn_dw6_;
	pt_dword			ukn_dw7_;
	pt_dword			ukn_dw8_;
	pt_dword			ukn_dw9_;
	pt_dword			ukn_dw10_;			//$50
	stCD_TagsPairArray	tags_;				//$54
	pt_dword			ukn_dw11_;
	pt_dword			ukn_dw12_;
	pt_pcvoid			ukn_ptr_;			//$64
	pt_dword			ukn_dw13_;
	pt_dword			ukn_dw14_;
	pt_dword			ukn_dw15_;
	pt_dword			ukn_dw16_;
	pt_dword			ukn_dw17_;			//$78
	stCD_TblItemGrantedEffectsArrayEx		granted_effects_;
	stCD_ModsPairArrayEx	mods_;
	stCD_TblItemBloodTypesPair		blood_type_;		//$94
	pt_cwsz					ch_name_;		//$9c,"裂颅者卡拉夫"
	p_mem_placeholder(4 * 3);
	stCD_UknGameArray	ukn_array2_;		//$ac,"Data/ItemVisualIdentity.dat"
	stCD_UknGameArray	ukn_array3_;		//$ac,"Data/ItemVisualIdentity.dat"
	p_mem_placeholder(4 * 2);
	stCD_ItemClassesPairT	item_classes_;	//$c4
	stCD_ItemClassesPairT	item_classes1_;
	p_mem_placeholder(4 * 5);
	stCD_AchievementItemsPairArray	achievement_items_;		//$e8
	stCD_UknGameArray				ukn_array4_;
	stCD_UknGameArray				ukn_array5_;
	pt_byte							ukn_b_;				//$100
	p_mem_placeholder(4 * 6);		//$101
	pt_byte			ukn_;		//$119
	pt_dword		ukn_dw18_;		//$11a
	pt_pcvoid		ukn_ptr1_;
	stCD_UknGameArray				ukn_array6_;
	stCD_UknGameArray				ukn_array7_;
	stCD_UknGameArray				ukn_array8_;
	p_mem_placeholder(4 * 2);		//$13a
	stCD_UknTblItemPair				monster_segments_;		//"Data/MonsterSegments.dat"
	stCD_UknTblItemPair				monster_armours_;		//"Data/MonsterArmours.dat"
	p_mem_placeholder(4 * 2);		//$152
	stCD_UknGameArray				ukn_array9_;			//$15a
	stCD_UknGameArray				ukn_array10_;
	stCD_UknGameArray				ukn_array11_;
	stCD_UknGameArray				ukn_array12_;
	stCD_UknGameArray				ukn_array13_;
	stCD_UknGameArray				ukn_array14_;
	stCD_UknGameArray				ukn_array15_;
	p_mem_placeholder(4 * 2);		//$192
	stCD_UknGameArray				ukn_array16_;

	void DebugInfo() const;
};
static_assert(0x78 == offsetof(stCD_TblItemMonsterVarieties, ukn_dw17_), "");
static_assert(g_so_tbl_item_monster_varieties_size_ == sizeof(stCD_TblItemMonsterVarieties), "");
typedef stCD_Db_TblItemPair<stCD_TblItemMonsterVarieties> stCD_TblItemMonsterVarietiesPair;
typedef stCD_DbT<stCD_TblItemMonsterVarieties> stCD_DbMonsterVarieties;

struct stCD_TblItemBuffDefinitions{
	pt_cwsz				en_name_;
	pt_cwsz				ch_desc_;			//中文描述，比如 增加闪电抗性
	pt_word				ukn_w_;
	pt_cwsz				ch_name_;			//$a
	stCD_StatsArray		stats_;

	void DebugInfo() const;
};
typedef stCD_Db_TblItemPair<stCD_TblItemBuffDefinitions> stCD_TblItemBuffDefinitionsPair;

struct stCD_DataCraftingBenchOptions{
	typedef GameArray<pt_dword> RequireItemCnt;

	p_mem_placeholder(4 * 2);
	pt_dword					ukn_dw_;				//$8
	stCD_ModsPair				mods_;
	stCD_BaseItemTypesArray		require_item_type_;		//$14
	RequireItemCnt				require_item_cnt_;		//$1c
	pt_dword					ukn_dw1_;
	pt_cwsz						title_;					//$28

	void DebugInfo() const;
};
static_assert(0x14 == offsetof(stCD_DataCraftingBenchOptions, require_item_type_), "");
static_assert(0x1c == offsetof(stCD_DataCraftingBenchOptions, require_item_cnt_), "");
static_assert(0x28 == offsetof(stCD_DataCraftingBenchOptions, title_), "");
typedef stCD_Db_TblItemPair<stCD_DataCraftingBenchOptions> stCD_DataCraftingBenchOptionsPair;

struct stCD_DataChestEffects{
};
typedef stCD_Db_TblItemPair<stCD_DataChestEffects> stCD_DataChestEffectsPair;

struct stCD_DataStrongboxes{
};
typedef stCD_Db_TblItemPair<stCD_DataStrongboxes> stCD_DataStrongboxesPair;

struct stCD_DataWords{

};
typedef stCD_Db_TblItemPair<stCD_DataWords> stCD_DataWordsPair;

struct stCD_DataNPCTextAudio{

};
typedef stCD_Db_TblItemPair<stCD_DataNPCTextAudio> stCD_DataNPCTextAudioPair;

//试练
struct stCD_DataLabyrinthTrials{
	stCD_WorldAreaPairT			world_area_;
	p_mem_placeholder(4 * 3);		//pt_dword
	stCD_DataNPCTextAudioPair	npc_text_audio_;
	pt_cwsz						direction_str1_;
	pt_cwsz						direction_str2_;
};
static_assert(0x24 == sizeof(stCD_DataLabyrinthTrials), "");
typedef stCD_Db_TblItemPair<stCD_DataLabyrinthTrials> stCD_DataLabyrinthTrialsPair;
typedef GameArray<stCD_DataLabyrinthTrialsPair> stCD_DataLabyrinthTrialsArray;

struct stCD_DataLabyrinths{
	pt_dword						id_;
	pt_cwsz							title_;			//$+4      4DE67A7C   505F9330  錰偟  UNICODE "The Normal Labyrinth"
	stCD_BaseItemTypesPairT			cost_item_;
	p_mem_placeholder(4);
	stCD_DataLabyrinthTrialsArray	trials_;
	pt_dword						lvl_;			//所需角色等级
	p_mem_placeholder(4);		//pt_dword
	stCD_DataWordsPair				data_words_;
	stCD_UknGameArray				ukn_ids_;
	stCD_UknGameArray				ukn_ids1_;
};
static_assert(0x3c == sizeof(stCD_DataLabyrinths), "");
typedef stCD_Db_TblItemPair<stCD_DataLabyrinths> stCD_DataLabyrinthsPair;
//////////////////////////////////////////////////////////////////////////
struct stCD_BuffInfo{
	pt_pcvoid							vtbl_;

private:
	stCD_TblItemBuffDefinitionsPair*	db_buff_definition_;

public:
	pt_cwsz GetBuffCnName() const;
	pt_cwsz GetBuffEnName() const;
	pt_cwsz	GetBuffDesc() const;
	void DebugInfo() const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_AttrDbItemBase : stCD_EntityAttrDbBase{
	pt_pcvoid						ukn_;
	pt_dword						grid_width_;	//不老生命药剂 等于1
	pt_dword						grid_height_;	//不老生命药剂 等于2
	GameWString						name_;			//物品名
	pt_dword						ukn_dw_;
	GameWString						desc_;			//物品描述
	pt_dword						ukn_dw1_;
	stCD_BaseItemTypesPairT			base_item_type_pair_;

	void DebugInfo() const;
};
static_assert(g_so_attr_db_item_base_name_ == offsetof(stCD_AttrDbItemBase, name_), "");
static_assert(g_so_attr_db_item_base_item_type_pair_ == offsetof(stCD_AttrDbItemBase, base_item_type_pair_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_DbEntity : stCD_Db{
	inline stCD_DbTblEntity* GetDbTbl() const{ 
		if (IsUnValidPtr(db_tbl_))
		{
			assert(false);
			return nullptr;
		}
		return db_tbl_->CastTo<stCD_DbTblEntity>();
	}
	stCD_EntityAttrInfo* FindAttrInfo(pt_csz name) const;
	stCD_EntityAttrDbBase* FindAttrDb(pt_csz name) const;
	template<typename T>
	T* FindAttrDb(pt_csz name) const{
		auto res = FindAttrDb(name);
		if (!res) return nullptr;
		return res->CastTo<T>();
	}
	void DebugInfo() const;
};

struct stCD_EntityBase{
	typedef GameVector<stCD_EntityAttrBase*> AttrContT;

	stCD_DbEntity*	db_;		//同样类型的对象，则该地址是一样的，比如角色(包括自己)，该值都是一样的。怀疑是个角色db。这个值若是一样，则下面的数组类型也是一样的。
	AttrContT		entity_cont_;

	stCD_DbTblEntity* GetDbTbl() const;
	const GameWString* GetDbPath() const;
	stCD_EntityAttrDbBase* FindAttrDb(pt_csz name) const;
	template<typename T>
	T* FindAttrDb(pt_csz name) const{
		if (IsUnValidPtr(db_))
		{
			assert(false);
			return nullptr;
		}
		return db_->FindAttrDb<T>(name);
	}
	stCD_EntityAttrInfo* FindAttrInfo(pt_csz name) const;
	stCD_EntityAttrBase* FindAttrComponent(pt_csz name) const;
	template<typename T>
	T* FindAttrComponent(pt_csz name) const{
		auto res = FindAttrComponent(name);
		if (!res) return nullptr;
		return res->CastTo<T>();
	}
	//////////////////////////////////////////////////////////////////////////
	//调用IsNpc、CastToNpc等函数前，先调用IsValid判断一下。
	bool IsValid() const;
	//////////////////////////////////////////////////////////////////////////
	//std::ostream输出期间，若是再次LOG_O(Log_debug)，则会产生输出顺序的混乱。
	friend std::ostream& operator << (std::ostream& ostm, const stCD_EntityBase& v);
	void DebugInfo() const;
	CMN_CAST_TO
};

//基于坐标的实体，也就是该实体有坐标属性，比如：玩家、NPC、怪物、传送点等
struct stCD_EntityPosBased : stCD_EntityBase{
	typedef bool(stCD_EntityPosBased::*Fn_EntityIf)() const;
	typedef void(stCD_EntityPosBased::*Fn_EntityDebugInfo)() const;

#ifdef VER3_
	p_mem_placeholder(4 * 3);
#endif
	stCD_World*				world_;
	pt_dword				id_;			//对象id，网络id，可用于发包
	p_mem_placeholder(g_so_entity_base_pos_attr_ - g_so_entity_id_ - 4);
	stCD_Positioned*		attr_pos_;

	//////////////////////////////////////////////////////////////////////////
	/*流放者的信属于NPC*/
	bool IsNpc() const;
	bool IsPlayer() const;
	bool IsMonster() const;
	bool IsWaypoint() const;
	bool IsAreaTransition() const;
	//仓库
	bool IsStash() const;
	//告示板
	bool IsNoticeBoard() const;
	//活动告示板
	bool IsNoticeBoardPvP() const;
	//箱子、木桶、流放者的遗体
	bool IsChest() const;
	//掉落物品
	bool IsWorldItem() const;
	//可移动对象
	bool IsMovement() const;
	//可传送的对象
	bool IsTransitionable() const;
	stCD_EntityNpc* CastToNpc() const;
	//可以先不必判断player，因为用到的情况非常少
	stCD_EntityPlayer* CastToPlayer() const;
	stCD_EntityMonster* CastToMonster() const;
	stCD_EntityWaypoint* CastToWaypoint() const;
	stCD_EntityAreaTransition* CastToAreaTransition() const;
	stCD_EntityNoHuman* CastToStash() const;
	stCD_EntityNoHuman* CastToNoticeBoard() const;
	stCD_EntityNoHuman* CastToNoticeBoardPvP() const;
	stCD_EntityChest* CastToChest() const;
	stCD_EntityWorldItem* CastToWorldItem() const;
	stCD_EntityTransitionable* CastToTransitionable() const;
	void DebugInfoAsNpc() const;
	void DebugInfoAsPlayer() const;
	void DebugInfoAsMonster() const;
	void DebugInfoAsWaypoint() const;
	void DebugInfoAsAreaTransition() const;
	void DebugInfoNoHuman() const;
	void DebugInfoChest() const;
	void DebugInfoWorldItem() const;
	void DebugInfoTransitionable() const;
	void DebugMonsterSkillInfo() const;
	//////////////////////////////////////////////////////////////////////////
	stCD_Positioned* GetPositioned() const;
	stCD_Render* GetRender() const;
	stCD_Actor* GetActor() const;
	//这个属性好像跟服务端有关吧，可以使之表现得像NPC
	stCD_EntityAttrNPC* GetNpc() const;
	//有这个属性才有NPC面板
	stCD_EntityAttrClientNpc* GetClientNpc() const;
	stCD_EntityAttrMonster* GetMonster() const;
	//需要被保护的怪一般有这个属性
	stCD_EntityAttrDiesAfterTime* GetDiesAfterTime() const;
	//使用传送卷轴后出现的传送门才有这个属性
	stCD_EntityAttrPortal* GetPortal() const;
	stCD_Pathfinding* GetPathfinding() const;
	stCD_EntityAttrTargetable* GetTargetable() const;
	stCD_EntityAttrTransitionable* GetTransitionable() const;
	void DebugInfo() const;

private:
	bool IsNoHumain(pt_cwsz meta_name) const;
	stCD_EntityNoHuman* CastToNoHuman(pt_cwsz meta_name) const;
};
static_assert(g_so_entity_id_ == offsetof(stCD_EntityPosBased, id_), "");
static_assert(g_so_entity_base_pos_attr_ == offsetof(stCD_EntityPosBased, attr_pos_), "");

//物品实体基类
struct stCD_EntityItemBased : stCD_EntityBase{
	//////////////////////////////////////////////////////////////////////////
	//**********调用下面的函数之前，别忘了调用IsValid判断一下。
	enCD_ItemType GetItemType() const;
	//所有物品都有这个，若是返回空，表示这个物品有问题
	stCD_AttrDbItemBase* GetAttrDbBase() const;
	stCD_TblItemBaseItemTypes* GetBaseItemType() const;
	//品质用不上，不必管它。
	stCD_AttrDbQuality* GetAttrDbQuality() const;
	stCD_AttrDbAttributeRequirements* GetAttrDbAttributeRequirements() const;
	stCD_AttrDbSkillGem* GetAttrDbSkillGem() const;
	stCD_EntityAttrDbFlask* GetAttrDbFlask() const;
	stCD_EntityAttrItemBase* GetAttrBase() const;
	stCD_EntityAttrMods* GetAttrMods() const;
	stCD_EntityAttrStack* GetAttrStack() const;
	stCD_EntityAttrSockets* GetAttrSockets() const;
	stCD_EntityAttrUsable* GetAttrUsable() const;
	stCD_EntityAttrCharges* GetAttrCharges() const;
	stCD_EntityAttrSkillGem* GetAttrSkillGem() const;
	//////////////////////////////////////////////////////////////////////////
	//衣服盔甲
	bool IsArmour() const;
	//技能宝石
	bool IsSkillGem() const;
	//////////////////////////////////////////////////////////////////////////
	void DebugInfo() const;
};

//UNICODE "Metadata/Effects/PermanentEffect"
struct stCD_EntityEffectsBased : stCD_EntityBase{};

template<typename TypeT, typename EntityBaseT = stCD_EntityPosBased>
struct stCD_EntityBaseT : EntityBaseT{};

template<typename TypeT>
struct stCD_EntityItemBasedT : stCD_EntityBaseT<TypeT, stCD_EntityItemBased>{};

//////////////////////////////////////////////////////////////////////////
typedef GameVector<stCD_PassiveSkillVertex*> stCD_PassiveSkillVertexCont;
struct stCD_PassiveSkillVertices{
	p_mem_placeholder(4 * 2);
	pt_dword						ukn_dw_;
	pt_dword						ukn_dw1_;
	stCD_PassiveSkillVertexCont		vertices_;			//里面竟然也包含父stCD_PassiveSkillVertex顶点指针
	p_mem_placeholder(4 * 2);							//这两个字段有可能不属于这个结构体
};
typedef stCD_PairAdjacent<stCD_PassiveSkillVertices, void> stCD_PassiveSkillVerticesSmartPtr;

struct stCD_PassiveSkillEdge{
	stCD_PassiveSkillVertex*	from_;
	stCD_PassiveSkillVertex*	to_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_PassiveSkillEdge& v);
};
typedef GameVector<stCD_PassiveSkillEdge*> stCD_PassiveSkillEdgeCont;

struct stCD_PassiveSkillVertex{
	pt_dword							ukn_dw_;				//=0
	stCD_TblItemPassiveSkillPair		passive_skill_;
	p_mem_placeholder(4 * 3);				//都是数值
	stCD_PassiveSkillVerticesSmartPtr	vertices_;				//$18,可能是个智能指针
	stCD_DbTblPassiveSkillGraph*		db_tbl_graph_;
	stCD_PassiveSkillEdgeCont			edges_in_;				//$24，入边
	stCD_PassiveSkillEdgeCont			edges_out_;				//$30，出边

	void DebugInfo() const;
};
static_assert(0x30 == offsetof(stCD_PassiveSkillVertex, edges_out_), "");
typedef stCD_GameHash_map<stCD_TblItemPassiveSkillPair, stCD_PassiveSkillVertex*> stCD_PassiveSkillVertexHashMap;

struct stCD_DbTblPassiveSkillGraph : stCD_DbTblBase{
	stCD_UknGameVector				ukn_vector_;
	stCD_PassiveSkillVertexHashMap	vertices_;					//$c
	stCD_UknGameVector				ukn_vector1_;				//$2c
	GameWString						graph_file_path_;			//$38,"Metadata/PassiveSkillGraph.psg"
	pt_dword						ukn_dw_;
	stCD_DbPassiveSkill*			db_passive_skill_;			//$54
	stCD_PassiveSkillHashMap		hash_passive_skills_;		//$58
	stCD_UknGameHash_map			ukn_hash1_;					//$78
	stCD_UknGameVector				ukn_vector2_;				//$98

	void DebugInfo() const;
};
static_assert(0 == offsetof(stCD_DbTblPassiveSkillGraph, ukn_vector_), "");
static_assert(0x54 == offsetof(stCD_DbTblPassiveSkillGraph, db_passive_skill_), "");
static_assert(0x98 == offsetof(stCD_DbTblPassiveSkillGraph, ukn_vector2_), "");
typedef stCD_DbBaseT<stCD_DbTblPassiveSkillGraph> stCD_DbPassiveSkillGraph;
//////////////////////////////////////////////////////////////////////////
typedef stCD_SafeVector<stCD_SafeGameWString> stCD_SkillAttrNameCont;
struct stCD_TblItemSkillStatDescriptions{
	stCD_SkillAttrNameCont			skill_attr_names_;
	stCD_UknGameVector				ukn_vector_;
};
typedef std::vector<std::wstring> stCD_MySkillAttrNameCont;

struct stCD_DbTblSkillStatDescriptions : stCD_DbTblBase{
	//技能名到属性信息的映射
	typedef stCD_GameHash_map<stCD_SafeGameWString, stCD_TblItemSkillStatDescriptions> SkillStatDescHashMap;

	stCD_UknGameHash_map			ukn_hash_map_;
	GameWString						self_file_path_;		//"Metadata/StatDescriptions/skill_stat_descriptions."
	SkillStatDescHashMap			skill_stat_desc_map_;
};
static_assert(0x38 == offsetof(stCD_DbTblSkillStatDescriptions, skill_stat_desc_map_), "");
struct stCD_DbSkillStatDescriptions : stCD_DbBaseT<stCD_DbTblSkillStatDescriptions>{
	struct stAttrNameInfo{
		std::wstring				skill_name_;
		stCD_MySkillAttrNameCont	attr_name_cont_;
	};
	typedef std::hash_map<pt_cwsz, stAttrNameInfo, RStd::hash_compare<pt_cwsz, StdEx::LessChar<wchar_t> > > MyHashMap;

	static const stCD_MySkillAttrNameCont* FindSkillAttr(pt_cwsz skill_en_name);
	bool ResolveMyHashMap();

private:
	static MyHashMap		my_hash_map_;
};

struct stCD_SkillTipsRow{
	stCD_SafeGameWString		str_tips_row_;			//每行显示的字符串
	stCD_SafeGameWString		ukn_str_;				//一般都为空
};
static_assert(0x30 == sizeof(stCD_SkillTipsRow), "");
typedef stCD_SafeVector<stCD_SkillTipsRow> stCD_SkillTipsRowCont;

struct stCD_SkillAttr{
	typedef GameVector<pt_dword> AttrValueIdCont;

	pt_pcvoid			vtbl_;
	pt_dword			ukn_dw_;
	stCD_UknGameVector	ukn_vec1_;
	AttrValueIdCont		attr_value_ids_;
};
static_assert(0x14 == offsetof(stCD_SkillAttr, attr_value_ids_), "");
typedef stCD_PairAdjacent<stCD_SkillAttr, void> stCD_SkillAttrSmartPtr;
//技能属性名到属性的映射。
typedef stCD_GameHash_map<stCD_SafeGameWString, stCD_SkillAttrSmartPtr> stCD_SkillAttrHashMap;

struct stCD_DbTblMinionSpellSkillStat : stCD_DbTblBase{
	stCD_SkillAttrHashMap		skill_attr_hash_map_;

	stCD_SkillAttr* FindSkillAttr(pt_cwsz attr_name) const;
};
typedef stCD_DbBaseT<stCD_DbTblMinionSpellSkillStat> stCD_DbMinionSpellSkillStat;
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
