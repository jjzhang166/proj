#pragma once
/*
@author zhp
@date 2017/2/5 22:36
@purpose 数据方面的接口
*/
#include <CD_CommonFiles/Data/CD_Interface.h>
#include "GConst.h"
#include <Net/PocoMore.h>
#include "GType.h"

class GItemBase;
namespace GInterface{
	TimeDuration& GetSendSkillTimeDuration();
	template<typename T>
	bool Send(const T& msg){
		//LOG_O(Log_trace) << "发包：" << typeid(T).name();
		return GWndExecSync([&msg](){
			return CD_Interface::Send(msg);
		});
	}
	template<typename T>
	bool SendSpan(TimeDuration& span, const T& msg){
		return span.TimeToDoSync([&msg](){
			return Send(msg);
		});
	}
	template<typename T>
	bool SendSpan(TimeDuration& span, const T& msg, const TimeDuration::WaitFuncT& wait_fn){
		return span.TimeToDoSync([&msg](){
			return Send(msg);
		}, 0, wait_fn);
	}
	template<typename T>
	bool SendSpan(TimeDuration& span, const T& msg, pt_dword time_duration, const TimeDuration::WaitFuncT& wait_fn){
		return span.TimeToDoSync([&msg](){
			return Send(msg);
		}, time_duration, wait_fn);
	}
	template<typename T>
	bool SendSpan(TimeDuration& span, pt_dword time_duration, const T& msg){
		return span.TimeToDoSync([&msg](){
			return Send(msg);
		}, time_duration);
	}
	//////////////////////////////////////////////////////////////////////////
	bool PlayerMove(const stCD_VecInt& dst_pos);
	//游戏里面会这样调用发次包，不知为何
	//用某个技能打开某个对象，一般是尸体
	bool OpenObj(enCD_SkillId skill_id, pt_dword net_id, enCD_TouchTargetSmallFlag flag);
	int GetInGameMainUiDlgCnt(enCD_TraUiShownFlag show_flag);
	bool CloseNpcPanel();
	bool EqualNpcPanelTitle(bool& panel_opened, const std::wstring& npc_title);
	bool MainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag);
	bool WaitMainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag, pt_dword timeout, bool is_dummy_sleep);
	enTalkToNpcRes TalkToNpc(enCD_NpcPanelItemsContainer panel, const std::wstring& talk_label, bool check_recv_msg);
	bool NpcPanelSubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag);
	bool WaitNpcPanelsubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag, pt_dword timeout);
	bool WaitPreGameState(pt_dword timeout);
	bool IsPreGameState();
	bool IsInGameState();
	bool IsLoginState();
	bool TopStateSameName(const std::wstring& state_name);
	//是否是创建角色状态(没有角色时进入的状态)
	bool IsCreateStateWithEmptyRole();
	int PressAllLvlUpSkillGem();
	bool ChooseKillTheMonster();
	enLoginAccountRes LoginAccount(const std::wstring& ac, const std::wstring& pwd, const std::wstring& gateway);
	bool CanUseSkill();
	bool WaitCanUseSkill();
	//操作命令
	bool OperateCommand(const wchar_t* command);
	//剩余怪物数量
	int GetRemaining();
	//////////////////////////////////////////////////////////////////////////
	bool MouseAttachedItem();
	GItemBasePtr GetMouseAttachItem();
	bool MouseAttachedItemCanPutTo(enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y);
	bool CanPutItemToTheInventory(enCD_InventoryType src_inventory_type, pt_dword item_id, 
		enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y);
	//拿起物品
	bool TakeupItem(const GItemBase& item);
	//放下物品
	bool PutItemDownTo(const GPackageBase& packet, const stCD_VecInt& dst_pos);
	//镶嵌宝石、取出宝石
	bool Takeoff_InsideGemImpl(const GItemBase& equip, pt_dword slot_idx, bool is_takeoff);
	//使用药剂
	bool UseFlask(const GItemBase& flask);
	//使用物品
	bool UseItem(const GItemBase& item);
	//使用物品到目标
	//使用物品作用于另一个物品，比如鉴定、升级装备等
	bool UseItemTo(const GItemBase& item_src, const GItemBase& item_dst);
	//合并物品
	bool MergeItem(const GItemBase& dst_item);
	//////////////////////////////////////////////////////////////////////////
	bool Revive(enCD_ReviveAtPosition at_position);
	bool PressInventoryItem(const GPackageBase& package, const GItemBase& item, enCD_PressItemWithKey key_state);
	stCD_UiGridPanel* FindGridPanelFromDlgProp(enCD_InventoryType inventory_type);
	bool LvlUpPassiveSkill(pt_word skill_id);
	//////////////////////////////////////////////////////////////////////////
	//选择角色并进入游戏
	bool EnterGame(const std::string& role_name);
	//创建角色
	enCmnCreateRoleRes CreateRole(const std::string& game_area, enCD_ProfessionType profession_type, const std::string& role_name);
	//////////////////////////////////////////////////////////////////////////
}
