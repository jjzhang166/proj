#pragma once
/*
@author zhp
@date 2017/1/24 17:22
*/
#include "CD_Serialize.hpp"
#include "CD_StructBase.h"

#pragma pack(push)
#pragma pack(1)

/*
PreGameState和InGameState分别是两个套接字，消息ID是不同的，但是底层协议是一样的。
每次进入地图都会断开之前的连接，并建立一个新的连接，说明一个地图就是一个服务器。
*/
//////////////////////////////////////////////////////////////////////////
enum CD_enPreMsgIdS : pt_word{
	kPMIS_PlayGame = 0xa,			//选择角色进入游戏
};
//////////////////////////////////////////////////////////////////////////
enum CD_enMsgIdS : pt_word{
#ifdef VER_GJ3_
	kMIS_FirstPacket = 0x1,			//这个包很多操作都会发，比如物品、使用技能之类的，它好像能够记住上次的发包操作吧，管它呢
	kMIS_EnterMapConnection,
	kMIS_EnterMap,					//切换地图的时候，先发这个，再发上面那个包，紧接着发送的。这两个包缺一不可。
	kMIS_OperCommand = 0x9,			//操作的命令，比如摧毁物品会发这个包
	kMIS_CreateRole = 0x0d,			//已更新
	kMIS_HeartBeat = 0x10,			//未更新，不需要更新
	kMIS_OpenObject = 0x13,
	kMIS_UseSkill = 0x14,
	kMIS_SkillUsing = 0x14,			//国际服没这个包
	kMIS_UseSkillEnd = 0x14,		//国际服没这个包
	kMIS_TakeupItem = 0x17,
	kMIS_ThrowItem = 0x18,
	kMIS_PutdownItem = 0x19,
	kMIS_TakeoffGem = 0x1a,
	kMIS_InsideGem = 0x1b,
	kMIS_LvlUpGem,
	kMIS_LvlUpPassiveSkill = 0x1e,		//发包最方面，点UI实现很麻烦的。
	kMIS_MergeItem = 0x26,
	kMIS_WayPoint = 0x29,
	kMIS_Revive = 0x2c,
	kMIS_UseItem = 0x2e,
	kMIS_UseFlask = 0x33,
	kMIS_UseItemTo = 0x34,
	kMIS_SplitItem = 0x3c,
	kMIS_ChangedMapFirstPackage = 0x3b,
	kMIS_NpcTalkItem = 0x40,
	kMIS_CloseNpc = 0x44,
	kMIS_NpcInventoryItemPutToSelfInventory = 0x46,
	kMIS_BuyItem_ToBag = 0x48,
	kMIS_InstanceManager = 0x5c,	//new fb
	kMIS_Sell_Cancle = 0x5e,
	kMIS_PutItemToShop = 0x5f,
	kMIS_PutbackItemFromShop = 0x60,
	kMIS_Sell_Accept,
	kMIS_UknItemOper = 0xb9,		//未更新，不需要更新
	kMIS_HelpKillNpc = 0x63,
	kMIS_SwitchStashPage = 0x6d,
	kMIS_SwitchStashPageIdxChange = 0x6e,
	kMIS_BagToStash = 0x79,
	kMIS_StashToBag = 0x7a,
#elif VER_GJ_
	kMIS_FirstPacket = 0x1,			//这个包很多操作都会发，比如物品、使用技能之类的，它好像能够记住上次的发包操作吧，管它呢
	kMIS_EnterMapConnection,
	kMIS_EnterMap,					//切换地图的时候，先发这个，再发上面那个包，紧接着发送的。这两个包缺一不可。
	kMIS_OperCommand = 0x8,			//操作的命令，比如摧毁物品会发这个包
	kMIS_CreateRole = 0x0c,			//已更新
	kMIS_HeartBeat = 0xf,			//未更新，不需要更新
	kMIS_OpenObject = 0x12,
	kMIS_UseSkill = 0x13,
	kMIS_SkillUsing = 0x13,			//国际服没这个包
	kMIS_UseSkillEnd = 0x13,		//国际服没这个包
	kMIS_TakeupItem = 0x16,
	kMIS_ThrowItem = 0x17,
	kMIS_PutdownItem = 0x18,
	kMIS_TakeoffGem = 0x19,
	kMIS_InsideGem = 0x1a,
	kMIS_LvlUpGem,
	kMIS_LvlUpPassiveSkill = 0x1d,		//发包最方面，点UI实现很麻烦的。
	kMIS_MergeItem = 0x25,
	kMIS_WayPoint = 0x28,
	kMIS_Revive = 0x2b,
	kMIS_UseItem = 0x2d,
	kMIS_UseFlask = 0x31,
	kMIS_UseItemTo = 0x32,
	kMIS_SplitItem = 0x3a,
	kMIS_ChangedMapFirstPackage = 0x3b,
	kMIS_NpcTalkItem = 0x40,
	kMIS_CloseNpc = 0x42,
	kMIS_NpcInventoryItemPutToSelfInventory = 0x44,
	kMIS_BuyItem_ToBag = 0x46,
	kMIS_InstanceManager = 0x5a,	//new fb
	kMIS_Sell_Cancle = 0x5c,
	kMIS_PutItemToShop = 0x5d,
	kMIS_PutbackItemFromShop = 0x5e,
	kMIS_Sell_Accept,
	kMIS_UknItemOper = 0xb7,			//未更新，不需要更新
	kMIS_HelpKillNpc = 0x65,
	kMIS_SwitchStashPage = 0x6b,
	kMIS_SwitchStashPageIdxChange = 0x70,
	kMIS_BagToStash = 0x7b,
	kMIS_StashToBag = 0x7c,
	//kMIS_
#else
	kMIS_FirstPacket = 0x1,			//这个包很多操作都会发，比如物品、使用技能之类的，它好像能够记住上次的发包操作吧，管它呢
	kMIS_EnterMapConnection,
	kMIS_EnterMap,					//切换地图的时候，先发这个，再发上面那个包，紧接着发送的。这两个包缺一不可。
	kMIS_OperCommand = 0xb,			//操作的命令，比如摧毁物品会发这个包
	kMIS_CreateRole = 0x0c,
	kMIS_HeartBeat = 0xf,
	kMIS_OpenObject = 0x16,
	kMIS_UseSkill = 0x17,
	kMIS_SkillUsing = 0x17,
	kMIS_UseSkillEnd = 0x17,
	kMIS_TakeupItem = 0x19,
	kMIS_ThrowItem = 0x1a,
	kMIS_PutdownItem = 0x1b,
	kMIS_TakeoffGem = 0x1c,
	kMIS_InsideGem = 0x1d,
	kMIS_LvlUpGem,
	kMIS_LvlUpPassiveSkill = 0x22,		//发包最方面，点UI实现很麻烦的。
	kMIS_MergeItem = 0x28,
	kMIS_WayPoint = 0x2b,
	kMIS_UseItem = 0x30,
	kMIS_UseFlask = 0x34,
	kMIS_UseItemTo = 0x35,
	kMIS_SplitItem = 0x3d,
	kMIS_ChangedMapFirstPackage = 0x3b,
	kMIS_NpcTalkItem = 0x40,
	kMIS_CloseNpc = 0x45,
	kMIS_NpcInventoryItemPutToSelfInventory = 0x47,
	kMIS_BuyItem_ToBag = 0x46,
	kMIS_InstanceManager = 0x5a,	//new fb
	kMIS_Sell_Cancle = 0x5c,
	kMIS_PutItemToShop = 0x60,
	kMIS_PutbackItemFromShop = 0x61,
	kMIS_Sell_Accept,
	kMIS_SwitchStashPage = 0x6e,
	kMIS_UknItemOper = 0xb7,
	kMIS_Revive = 0x12b,
	kMIS_HelpKillNpc = 0x65,
	kMIS_SwitchStashPageIdxChange = 0x70,
	kMIS_BagToStash = 0x7b,
	kMIS_StashToBag = 0x7c,
#endif
};

enum CDS_enOperCommandSubMsgId : pt_byte{
	kOCSMID_DestroyItem = 9,
};
//////////////////////////////////////////////////////////////////////////
template<pt_word MsgId>
struct stCDS_CheckAny : stCDS_MsgBase<MsgId, 0, false>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		ar.IgnoreAll();
		return true;
	}
};
//////////////////////////////////////////////////////////////////////////
struct stCDS_CreateRole : stCDS_MsgBase<kMIS_CreateRole, 1>{
	stCDS_CreateRole();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & role_name_
			& area_name_
			& ukn_zero_
			& ukn_stack_addr_
			& str_name_
			& all_zero_;
	}

	pt_cwsz		role_name_;
	pt_cwsz		area_name_;
	pt_dword	ukn_zero_;					//=0
	pt_dword	ukn_stack_addr_;			//这里竟然是堆栈的地址
	pt_cwsz		str_name_;		//StrDexIntDefault
	pt_byte		all_zero_[0x20];
};

//这个包的大小不固定，有时候多，有时候少，如果把这个包返回掉，调试断多久，就都不会掉线了。
struct stCDS_FirstPacket : stCDS_MsgBase<kMIS_FirstPacket>{

};

struct stCDS_EnterMapConnection : stCDS_CheckAny<kMIS_EnterMapConnection>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		//assert(!"这个包不会单独发送的吧？");
		ar.IgnoreAll();
		return true;
	}
};

struct stCDS_EnterMap : stCDS_MsgBase<kMIS_EnterMap, 0, false>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(ukn_dw_))
			return false;
		//LOG_O(Log_trace) << "msg, stCDS_EnterMap, ukn_dw_:" << (pt_pcvoid)ukn_dw_;
		return true;
	}

	enCD_EnterMapUknId		ukn_dw_;
};

//摧毁物品也是这个包
struct stCDS_OperCommand : stCDS_MsgBase<kMIS_OperCommand>{
	stCDS_OperCommand();
	template<class Archive>
	void serialize(Archive & ar) const{
		if (sub_msg_id_ == kOCSMID_DestroyItem)
		{
			assert(0 == ukn_w_);
			ar & sub_msg_id_ & ukn_w_;
		}
		else
			assert(false);
	}

	CDS_enOperCommandSubMsgId		sub_msg_id_;

private:
	pt_word			ukn_w_;			//一直=0
};

//这个包的大小是固定的，就一个字段
struct stCDS_Heartbeat : stCDS_MsgBase<kMIS_HeartBeat>{
	pt_dword		ukn_dw_;		//一直等于0，对这个变量进行了两次加密
};

/*
若是鼠标只点击一下，那么先发0xcb、再发0xcf，只发两个包。
若是鼠标按下不放，先发0xcb，一直发0xcd，松开鼠标发0xcf。
*/
//走路也会发这个包，第一个发包，必发。
struct stCDS_UseSkill : stCDS_MsgBase<kMIS_UseSkill>{
	stCDS_UseSkill();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & dst_pos_ & skill_id_ 
			& skill_use_cnt_
			& ukn_flag_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(dst_pos_))
			return false;
		if (!ar.Read(skill_id_))
			return false;
		if (!ar.Read(skill_use_cnt_))
			return false;
		if (!ar.Read(ukn_flag_))
			return false;
		if (ukn_flag_ != kTTSF_Normal)
		{
			assert(false);
			//ukn_flag_ = kTTSF_Normal;
			return false;
		}
		//LOG_O(Log_trace) << "UseSkill,skillId:" << (pt_pcvoid)skill_id_ << ",dstPos:" << dst_pos_ << ",SkillCnt:" << skill_use_cnt_ << ",flag:" << ukn_flag_;
		return ar.empty();
	}
	
	stCD_VecInt	dst_pos_;
	/*
	0x2909		走路时
	*/
	pt_word		skill_id_;			//0x8401，怀疑是技能ID
	pt_word		skill_use_cnt_;		//技能使用次数

private:
	enCD_TouchTargetSmallFlag	ukn_flag_;			//是个标志，一直=0x8。 OR BL, 0x8
};
static_assert(sizeof(stCDS_UseSkill) == 0xf, "");

//使用之第二个发包
struct stCDS_SkillUsing : stCDS_MsgBase<kMIS_SkillUsing>{
	stCD_VecInt		dst_pos_;

	template<class Archive>
	void serialize(Archive & ar) const{
		ar & dst_pos_;
	}
};

//使用技能结束
struct stCDS_UseSkillEnd : stCDS_MsgBase<kMIS_UseSkillEnd>{
	template<class Archive>
	void serialize(Archive & ar) const{}
};

struct stCDGS_TakeupItem : stCD_GameMsgBase{
	enCD_InventoryType		inventory_type_;
	pt_dword				item_id_;
	enCD_SendItemOperFlag	ukn_flag_;
};
static_assert(0x10 == offsetof(stCDGS_TakeupItem, ukn_flag_), "");
//拿起物品
struct stCDS_TakeupItem : stCDS_MsgBase<kMIS_TakeupItem>{
	stCDS_TakeupItem();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & CD_Encrypt<CD_TagEncrypt1>(inventory_type_) & item_id_ & CD_Encrypt<CD_TagEncrypt1>(ukn_flag_);
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(inventory_type_))
			return false;
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(ukn_flag_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType		inventory_type_;		//物品所在背包类型
	pt_dword				item_id_;				//每拿起一次就加2

private:
	enCD_SendItemOperFlag	ukn_flag_;				//一直等于0
};

//丢物品到地上
struct stCDS_ThrowItem : stCDS_MsgBase<kMIS_ThrowItem>{
	template<class Archive>
	void serialize(Archive & ar) const{}
};

//放下物品
/*若是放下的目标位置有物品b，则放下物品，然后拿起物品b*/
struct stCDS_PutdownItem : stCDS_MsgBase<kMIS_PutdownItem>{
	stCDS_PutdownItem();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & dst_inventory_type_ & dst_left_top_idx_ & ukn_bool_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(dst_inventory_type_))
			return false;
		if (!ar.Read(dst_left_top_idx_))
			return false;
		if (!ar.Read(ukn_bool_))
			return false;
		if ((int)ukn_bool_ != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	enCD_InventoryType		dst_inventory_type_;	//物品要放的目标inventory_type
	stCD_VecInt				dst_left_top_idx_;		//要放到的左上的位置，均以0开始索引

private:
	pt_bool					ukn_bool_;				//一直等于0
};

template<pt_word MsgId>
struct stCDS_Takeoff_InsideGem : stCDS_MsgBase<MsgId>{
	stCDS_Takeoff_InsideGem(){
		CD_ZERO_MSGS();
	}
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & inventory_type_ & equip_item_id_ & slot_idx_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(inventory_type_))
			return false;
		if (!ar.Read(equip_item_id_))
			return false;
		if (!ar.Read(slot_idx_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType	inventory_type_;	//装备所在inventory_type
	pt_dword			equip_item_id_;
	pt_dword			slot_idx_;			//第几个槽，从0开始
};

//取出宝石
struct stCDS_TakeoffGem : stCDS_Takeoff_InsideGem<kMIS_TakeoffGem>{};

//镶嵌宝石
struct stCDS_InsideGem : stCDS_Takeoff_InsideGem<kMIS_InsideGem>{};

struct stCDS_LvlUpGem : stCDS_MsgBase<kMIS_LvlUpGem>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(inventory_type_))
			return false;
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(gem_idx_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType		inventory_type_;
	pt_dword				item_id_;
	pt_dword				gem_idx_;		//slot_idx
};

//升级天赋
struct stCDS_LvlUpPassiveSkill : stCDS_MsgBase<kMIS_LvlUpPassiveSkill>{
	stCDS_LvlUpPassiveSkill();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & skill_id_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(skill_id_))
			return false;
		return ar.empty();
	}

	pt_dword		skill_id_;
};

struct stCDS_Revive : stCDS_MsgBase<kMIS_Revive>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(revive_mode_))
			return false;
		return ar.empty();
	}

	pt_byte		revive_mode_;		//复活模式
};

//使用物品，只是通过右键鼠标即可使用物品
struct stCDS_UseItem : stCDS_MsgBase<kMIS_UseItem>{
	stCDS_UseItem();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & inventory_type_ & item_id_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(inventory_type_))
			return false;
		if (!ar.Read(item_id_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType		inventory_type_;
	pt_dword				item_id_;
};

//使用药剂、使用瓶子
struct stCDS_UseFlask : stCDS_MsgBase<kMIS_UseFlask>{
	stCDS_UseFlask();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & item_idx_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(item_idx_))
			return false;
		return ar.empty();
	}

	pt_dword		item_idx_;			//物品在inventory的索引，从0开始
};

//使用该物品作用于另一个物品，比如鉴定
struct stCDS_UseItemTo : stCDS_MsgBase<kMIS_UseItemTo>{
	stCDS_UseItemTo();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & inventory_type_src_ & item_id_src_ & inventory_type_dst_ & item_id_dst_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(inventory_type_src_))
			return false;
		if (!ar.Read(item_id_src_))
			return false;
		if (!ar.Read(inventory_type_dst_))
			return false;
		if (!ar.Read(item_id_dst_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType		inventory_type_src_;
	pt_dword				item_id_src_;
	enCD_InventoryType		inventory_type_dst_;
	pt_dword				item_id_dst_;
};

struct stCDS_MergeItem : stCDS_MsgBase<kMIS_MergeItem>{
	stCDS_MergeItem();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & CD_Encrypt<CD_TagEncrypt2>(dst_inventory_type_) & dst_item_id_ & CD_Encrypt<CD_TagEncrypt2>(ukn_flag_);
	}

	enCD_InventoryType		dst_inventory_type_;		//被合并物品的目标所在
	pt_dword				dst_item_id_;

private:
	enCD_SendItemOperFlag	ukn_flag_;
};

struct stCDS_SplitItem : stCDS_MsgBase<kMIS_SplitItem>{};

//把物品放到商店物品栏
struct stCDS_PutItemToShop : stCDS_MsgBase<kMIS_PutItemToShop>{
	stCDS_PutItemToShop();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & item_inventory_type_ & item_id_ & dst_pos_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(item_inventory_type_))
			return false;
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(dst_pos_))
			return false;
		return ar.empty();
	}

	enCD_InventoryType		item_inventory_type_;	//物品所在inventory_type
	pt_dword				item_id_;
	stCD_VecByte			dst_pos_;				//从0开始
};

//这个包貌似用不到，所以就不分析了
//把你放到商店物品栏的物品放回到你的背包
struct stCDS_PutbackItemFromShop : stCDS_MsgBase<kMIS_PutbackItemFromShop>{
	stCDS_PutbackItemFromShop();
};

struct stCDS_Sell_Accept : stCDS_MsgBase<kMIS_Sell_Accept>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(sell_id_))
			return false;
		return ar.empty();
	}

	pt_dword		sell_id_;
};

struct stCDS_InstanceManager : stCDS_MsgBase<kMIS_InstanceManager>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(instance_idx_))
			return false;
		return ar.empty();
	}

	pt_byte			instance_idx_;		//从0开始，地图实例的索引
};

struct stCDS_Sell_Cancle : stCDS_MsgBase<kMIS_Sell_Cancle>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(ukn_dw_))
			return false;
		if (!ar.Read(ukn_by_))
			return false;
		if (ukn_dw_ != 0)
		{
			assert(false);
			return false;
		}
		if (ukn_by_ != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_dword		ukn_dw_;		//=0
	pt_byte			ukn_by_;		//=0;
};

struct stCDS_HelpKillNpc : stCDS_MsgBase<kMIS_HelpKillNpc>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(is_help_))
			return false;
		return ar.empty();
	}

	bool		is_help_;		//1=help,0=kill
};

struct stCDS_SwitchStashPage : stCDS_MsgBase<kMIS_SwitchStashPage>{
	stCDS_SwitchStashPage();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & tab_idx_ & ukn_byte_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(tab_idx_))
			return false;
		if (!ar.Read(ukn_byte_))
			return false;
		if (ukn_byte_ != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_word			tab_idx_;
	pt_byte			ukn_byte_;	//=0
};

//比如选择任务奖励，购买物品
struct stCDS_NpcInventoryItemPutToSelfInventory : stCDS_MsgBase<kMIS_NpcInventoryItemPutToSelfInventory>{
	stCDS_NpcInventoryItemPutToSelfInventory();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & item_id_ & dst_inventory_type_& dst_pos_ & tbl_idx_ & ukn_byte1_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(dst_inventory_type_))
			return false;
		if (!ar.Read(dst_pos_))
			return false;
		if (!ar.Read(tbl_idx_))
			return false;
		if (!ar.Read(ukn_byte1_))
			return false;
		if (ukn_byte1_ != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_dword			item_id_;
	enCD_InventoryType	dst_inventory_type_;		//要放往哪个inventory_type
	stCD_VecInt			dst_pos_;

	//购买物品、选择任务奖励，下面的值都是一样的
private:
	pt_byte				tbl_idx_;			//table_page索引，即是第几个页面，索引从0开始
	pt_byte				ukn_byte1_;			//0
};

struct stCDS_BuyItem_ToBag : stCDS_MsgBase<kMIS_BuyItem_ToBag>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(tbl_idx_))
			return false;
		if (!ar.Read(ukn_byte1_))
			return false;
		if (ukn_byte1_ != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_dword		item_id_;
	pt_byte			tbl_idx_;			//table_page索引，即是第几个页面，索引从0开始
	pt_byte			ukn_byte1_;			//0
};
//////////////////////////////////////////////////////////////////////////
//*********************任务相关
//发包打开npc不管用，得调用游戏中的函数。一发这个包就断线。
//打开npc
struct stCDS_OpenObject : stCDS_MsgBase<kMIS_OpenObject>{
	stCDS_OpenObject();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & dst_obj_id_ & skill_id_
//#ifdef VER_GJ_
			& skill_use_cnt_
//#endif
			& ukn_byte_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(dst_obj_id_))
			return false;
		if (!ar.Read(skill_id_))
			return false;
//#ifdef VER_GJ_
		if (!ar.Read(skill_use_cnt_))
			return false;
//#endif
		if (!ar.Read(ukn_byte_))
			return false;
		if (ukn_byte_ != kTTSF_Normal
			&& ukn_byte_ != kTTSF_WithLeftCtrl)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_dword					dst_obj_id_;
	enCD_SkillId				skill_id_;		//一直等于0x266
//#ifdef VER_GJ_
	pt_word						skill_use_cnt_;	//技能使用次数
//#endif

private:
	enCD_TouchTargetSmallFlag	ukn_byte_;		//一直等于0x8
};
static_assert(std::is_enum<enCD_SkillId>::value, "");
static_assert(std::is_enum<enCD_TouchTargetSmallFlag>::value, "");
static_assert(sizeof(enCD_SkillId) == 2, "");
static_assert(sizeof(enCD_TouchTargetSmallFlag) == 1, "");

struct stCDS_ChangeMapFirstPacket : stCDS_MsgBase<kMIS_ChangedMapFirstPackage>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(instance_id1_))
			return false;
		if (!ar.Read(instance_id2_))
			return false;
		return ar.empty();
	}

	//如果不新建一个地图实例，则进同一个地图，以下两个数值均不变，猜测是地图实例的ID吧。
	pt_dword		instance_id1_;
	pt_dword		instance_id2_;
};

struct stCDS_NpcTalkItem : stCDS_MsgBase<kMIS_NpcTalkItem>{
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(talk_idx_))
			return false;
		return ar.empty();
	}

	pt_byte			talk_idx_;
};

//关闭npc发包不管用。
//关闭npc，只有消息ID，没有消息内容
struct stCDS_CloseNpc : stCDS_MsgBase<kMIS_CloseNpc>{
	template<class Archive>
	void serialize(Archive & ar) const{}
	template<class Archive>
	bool CheckRead(Archive & ar){
		return ar.empty();
	}
};

struct stCDS_Waypoint : stCDS_MsgBase<kMIS_WayPoint>{
	stCDS_Waypoint();
	template<class Archive>
	void serialize(Archive & ar) const{
		ar & world_area_id_ & connections_num_ & ukn_byte_;
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(world_area_id_))
			return false;
		if (!ar.Read(connections_num_))
			return false;
		if (!ar.Read(ukn_byte_))
			return false;
		if (int(ukn_byte_) != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_dword			world_area_id_;		//=7ad2,3275,235c
	pt_dword			connections_num_;	//0x42		,2bd

private:
	pt_byte				ukn_byte_;			//=0
};

template<pt_word MsgId>
struct stCDS_BagToStashBase : stCDS_MsgBase<MsgId>{
	stCDS_BagToStashBase(){
		CD_ZERO_MSGS();
	}
	template<class Archive>
	bool CheckRead(Archive & ar){
		if (!ar.Read(ukn_w_))
			return false;
		if (!ar.Read(item_id_))
			return false;
		if (!ar.Read(ukn_b_))
			return false;
		if (int(ukn_b_) != 0)
		{
			assert(false);
			return false;
		}
		return ar.empty();
	}

	pt_word				ukn_w_;
	pt_dword			item_id_;
	pt_byte				ukn_b_;
};
struct stCDS_BagToStash : stCDS_BagToStashBase<kMIS_BagToStash>{};
struct stCDS_StashToBag : stCDS_BagToStashBase<kMIS_StashToBag>{};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
