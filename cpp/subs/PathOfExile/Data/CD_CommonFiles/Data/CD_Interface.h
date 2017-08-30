#pragma once
/*
@author zhp
@date 2017/2/4 11:18
@purpose for interface
*/
#include "CD_Structs.h"
#include <luabind/luabind.hpp>
#include "CD_ProtocolS.h"
#include "CD_ProtocolR.h"
#include <CmnMix/Cmn_Hooker.h>
#include <CmnMix/Cmn_UsefulClass.h>
#include "CD_UsefulFns.h"

namespace CD_Interface{
	typedef stCD_SceneHolder::IterFnT TraFn_Scene;
	typedef stCD_ObjMgr::Fn_TraObj Fn_TraObj;
	typedef stCD_InventoryMgr::Fn_TraItem Fn_TraItem;
	typedef stCD_Inventory::Fn_TraEquipsInsideGem Fn_TraEquipsInsideGem;
	typedef stCD_Inventory::Fn_TraSkillGem Fn_TraSkillGem;
	typedef stCD_Inventory::Fn_TraGemSkill Fn_TraGemSkill;
	typedef stCD_Skill_IdInfo::Fn_TraMySkill Fn_TraMySkill;
	typedef stCD_PlayerMe::Fn_TraCurQuestInfo Fn_TraCurQuestInfo;
	typedef stCD_UiQuestLvlStatePanel::Fn_TraQuestItem Fn_TraQuestItem;
	typedef stCD_DlgWaypoint::Fn_TraQuestItem Fn_TraUiQuestItem;
	typedef stCD_InventoryMgr::Fn_TraInventory Fn_TraInventory;
	typedef stCD_InventoryMgr::FilterInventoryType FilterInventoryType;
	typedef stCD_UiBuff::Fn_TraCallerBuff Fn_TraCallerBuff;
	//////////////////////////////////////////////////////////////////////////
	class CalcAvoidPos{
	public:
		virtual const stCD_VecInt* GetLovedDstPos() const = 0;
		virtual bool CanShooted(const stCD_VecInt& center, float radius,
			const stCD_VecInt& pos1, const stCD_VecInt& pos2) const = 0;
		virtual bool GetAvoidPos(const stCD_VecInt& center, int half_width, int half_height, float center_distance,
			const stCD_VecInt* dst_pos,
			const stCD_VecInt& pos_from, const stCD_VecInt& pos_to, stCD_VecInt& out_res) const = 0;
		virtual bool MoveTo(const stCD_VecInt& pos, pt_dword entity_id, pt_word skill_id) const = 0;
	};
	class OnMsgBase{};

	class OnRecvMoveMsg : public OnMsgBase{
	public:
		virtual void OnSelfMoveMsg(const stCDGR_OnMove& msg) = 0;
	};

	class OnHookAfterInitMap{
	public:
		virtual void OnBeforeInitMap(stCD_GameLogic& new_game_logic, stCD_World& new_world) = 0;
		virtual void OnAfterInitMap() = 0;
	};
	class InterfaceSink{
	public:
		virtual void OnModifyHp(pt_dword new_hp) = 0;
		virtual void OnModifyMp(pt_dword new_mp) = 0;
		//剩余怪物数量
		virtual void OnPromptRemaining(int remaining) = 0;
		virtual HMODULE GetSelfModule() = 0;
		virtual bool IsAddrInSelfModule(pt_pcvoid addr = nullptr) = 0;
		virtual pt_dword GetGWndThrdId() const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	//功能函数
	template<typename T>
	bool Send(const T& msg){
		auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
		if (!ptr)
		{
			assert(false);
			return false;
		}
		auto send_impl = ptr->GetSendPacket();
		if (!send_impl)
		{
			assert(false);
			return false;
		}
		return send_impl->Send(msg);
	}
	template<typename T>
	T* GetStateByName(const std::wstring& state_name){
		auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
		if (!ptr)
		{
			assert(false);
			return nullptr;
		}
		auto res = ptr->GetStateByName(state_name);
		if (!res)
			return nullptr;
		return res->CastTo<T>();
	}
	stCD_SceneHolder* GetSceneHolder();
	bool TopStateSameName(const std::wstring& state_name);
	stCD_ScenePreGameState* GetPreGameState();
	stCD_SceneInGameState* GetInGameState();
	stCD_SceneEscapeState* GetEscapeState();
	stCD_SceneCreateCharacterState* GetCreateCharacterState();
	stCD_SceneSelectCharacterState* GetSelectCharacterState();
	stCD_SceneLoginState* GetLoginState();
	stCD_GameLogic* GetGameLogic();
	stCD_EntityPlayerMe* GetPlayerEntity();
	stCD_PlayerMe* GetPlayer();
	stCD_Positioned* GetPlayerPositioned();
	stCD_Life* GetPlayerLife();
	stCD_Pathfinding* GetPlayerPathfinding();
	stCD_World* GetWorld();
	stCD_InventoryMgr* GetInventoryMgr();
	stCD_Inventory* GetInventory(enCD_InventoryType inventory_type);
	//这个函数用起来要小心，一定要同步使用
	stCD_EntityItemBased* FindInventoryItem(enCD_InventoryType inventory_type, pt_dword item_id);
	HWND GetGameWndHandle();
	stCD_GameBaseDb& GetDbMgr();
	//寻路
	bool PlayerMove(const stCD_VecInt& dst_pos);
	//移动技能id
	pt_word GetSkillIdMove();
	//打开传送点 npc 打开仓库 捡取物品 打开箱子
	bool DoTouchTargetObj(enCD_SkillId flag, pt_dword net_id, enCD_TouchTargetSmallFlag small_flag);
	stCD_StateUiLogic* GetInGameUiLogic();
	stCD_InGameTopUi* GetInGameTopUi();
	stCD_EscapeTopUi* GetEscapeTopUi();
	stCD_UiPreInGameTop* GetPreInGameTopUi();
	stCD_UiSelectRoleTop* GetSelectRoleTop();
	//是个ui容器
	stCD_InGameMainUi* GetInGameMainUi();
	bool MainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag);
	stCD_DlgWaypoint* GetDlgWayPoint();
	bool CloseNpcPanel();
	//切换到EscapeState，相当于打开了返回角色面板
	bool SwitchToEscapeState();
	bool PressEscapeBtItem(enCD_EscapeMenuButton bt_idx);
	int GetInGameMainUiDlgCnt(enCD_TraUiShownFlag show_flag);
	stCD_UiNpcPanelContainer* GetNpcPanelContainer();
	stCD_UiNpcPanel* GetNpcPanel(bool must_shown = true);
	bool NpcPanelSubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag);
	stCD_Inventory* GetQuestAwardInventory();
	//不要直接用这个函数，一定要同步使用
	stCD_EntityItemBased* GetMouseAttachItem();
	//不要直接用这个函数，一定要同步使用
	bool CanPutItemToTheInventory(stCD_EntityItemBased* item, enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y);
	bool CanPutItemToTheInventory(enCD_InventoryType src_inventory_type, pt_dword item_id, enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y);
	bool Revive(enCD_ReviveAtPosition at_position);
	bool CloseSomeBoringUi();
	int PressAllLvlUpSkillGem();
	//选择击败克雷顿
	bool ChooseKillTheMonster();
	stCD_DlgProp* GetDlgProp();
	stCD_DlgSell* GetDlgSell();
	stCD_DlgBuy* GetDlgBuy();
	stCD_DlgStash* GetDlgStash();
	stCD_UiDlgQuestAward* GetDlgQuestAward();
	stCD_UiGridPanel* FindGridPanelFromDlgProp(enCD_InventoryType inventory_type);
	stCD_DlgMapLocator* GetDlgMapLocator();
	stCD_DlgSelfMapLocator* GetDlgSelfMapLocator();
	stCD_DlgLabyrinthSelect* GetDlgLabyrinthSelect();
	stCD_DlgTrialPlaque* GetDlgTrialPlaque();
	stCD_DlgInstanceMgr* GetDlgInstanceMgr();
	stCD_UiItemTips* GetFirstItemTips();
	stCD_Socket* GetInGameStateSocket();
	stCD_Socket* GetPreGameStateSocket();
	void RegSomeForLua(lua_State* l);
	//////////////////////////////////////////////////////////////////////////
	//遍历
	void TraverseState(const TraFn_Scene& fn);
	int TraEntityObj(const Fn_TraObj& fn, bool is_obj);
	int TraInventory(const Fn_TraInventory& fn);
	int TraInventory(const FilterInventoryType& filter, const Fn_TraInventory& fn);
	//遍历inventory中的物品
	int TraInventoryItems(enCD_InventoryType type, const Fn_TraItem& fn);
	//遍历身上物品
	int TraBodyItems(const Fn_TraItem& fn, bool include_flask = false, bool include_hide_weapon = false);
	//遍历inventory中装备镶嵌的宝石
	int TraEquipsInsideGem(enCD_InventoryType type, const Fn_TraEquipsInsideGem& fn);
	//遍历身上所有装备镶嵌的宝石
	int TraBodyEquipsGem(const Fn_TraEquipsInsideGem& fn, bool include_hide_weapon = false);
	//遍历技能宝石
	int TraSkillGems(enCD_InventoryType type, const Fn_TraSkillGem& fn);
	//遍历宝石技能
	int TraGemSkill(enCD_InventoryType type, const Fn_TraGemSkill& fn, enCD_TraGemSkillFlag flag);
	//遍历身上宝石技能
	//只需要关注kTGSF_ActiveSkill、kTGSF_PassiveSkill即可
	int TraBodyGemSkill(const Fn_TraGemSkill& fn, bool active_skill, bool passive_skill = false, bool include_hide_weapon = false);
	//遍历我的技能
	int TraMySkill(const Fn_TraMySkill& fn);
	//遍历当前任务
	int TraCurQuestInfo(const Fn_TraCurQuestInfo& fn);
	//遍历右侧的UI状态信息
	int TraUiQuestStateItem(const Fn_TraQuestItem& fn);
	//世界地图上的任务
	int TraUiQuestItem(enCD_TraUiShownFlag show_flag, enCD_kMapDifficulty difficulty, int chapter,
		const Fn_TraUiQuestItem& fn);
	//遍历召唤buff
	int TraCallerBuff(const Fn_TraCallerBuff& fn);
	//////////////////////////////////////////////////////////////////////////
	//hooks
	class Hooks : public Singleton<Hooks, Singleton_Instance>{
	public:
		struct stRecvPacketRead{
			pt_pcvoid		msg_data_;
			pt_dword		msg_len_;
		};

	public:
		template<typename MsgT>
		void AddCmdR(){
			return recv_packet_mgr_in_game_.AddCmd<MsgT>();
		}
		template<typename MsgT>
		void AddCmdPreR(){
			return recv_packet_mgr_pre_in_game_.AddCmd<MsgT>();
		}
		template<typename MsgT>
		void AddCmdS(){
			return send_packet_mgr_.AddCmd<MsgT>();
		}
		CmnRecvPacketMgr& GetRecvPacketMgr(enCD_SocketType socket_type);
		CmnRecvPacketMgr& GetSendPacketMgr();

	public:
		bool DoHooks();
		void HookForTest();
		void HookRecvPacketRead();
		void HookCheckPacket_Heartbeat();
		void HookOnMoveToAvoidDamage();
		void HookInitMap();
		void HookModifyGameEnv();
		void HookAnalysisRecvPacket();
		void HookAnalysisSendPacket();
		void HookSkillObjUpdateMousePos();
		void HookGetForegroundWindow();
		void HookIsWow64Process();
		void HookOperateCommand();
		void HookDisableClientSwitching();
		void HookAntiCollision();
		void HookGetKeyState();
		void HookModifyLifeData();
		void HookPromptInfo();
		//为了多开
		void HookForDuoKai();

		//anti check
	public:
		void HookAntiCheck_GF();
		void HookForAntiCheckPacket();
		//遍历检测线程
		void DoTraCheckThread();

	public:
		static void SetRecvOnMoveMsg(OnRecvMoveMsg* reciever);
		static void SetOnInitMapSink(OnHookAfterInitMap* sink);
		static void EnableDummyCtrlPress(bool ctrl_pressed);
		static void SetInterfaceSink(InterfaceSink* sink);

	private:
		CmnRecvPacketMgr	recv_packet_mgr_pre_in_game_;
		CmnRecvPacketMgr	recv_packet_mgr_in_game_;
		CmnRecvPacketMgr	send_packet_mgr_;
	};
	//////////////////////////////////////////////////////////////////////////
	//test
	void RegTestForLua(lua_State* l);
	void TestCreateRole();
	void TestHooks(int tag);
	void TestFindPath(int x, int y);
	void TestPressNpcItem(int panel_type, const std::string& lable);
	void TestRecvOutDebugInfo(bool open);
	void TestAnalysisRecvPacket(bool enable, pt_word break_id, const luabind::object& tbl);
	void TestAnalysisSendPacket(bool enable, pt_word break_id, const luabind::object& tbl);
	void TestAnalysisSendPacketBlocked(bool enable, const luabind::object& tbl);
	void TestShowUi(pt_dword ui_addr, bool show);
	void TestShowUi_(pt_dword ui_addr);
	//////////////////////////////////////////////////////////////////////////
	//debug
	void RegDbgInfoForLua(lua_State* l);
	void DebugStateInfo();
	void DebugAddrInfo();
	void DebugSendPacket();
	void DebugPlayerInfo();
	void DebugPlayerEntityInfo();
	void DebugWorldInfo();
	void DebugGameBaseWndInfo();
	void DebugPlayerPosInfo();
	void DebugDstMapDataInfo(const stCD_VecInt& pos, bool is_src_map_data);
	void DebugEntityObjs(int type);
	void DebugPlayerAttrInfo(int type);
	void DebugPassiveSkill();
	void DebugDbInfo(const char* file_path);
	void DebugDbPassiveSkill();
	void DebugDbQuest();
	void DebugDbQuestStates();
	void DebugDbActiveSkills();
	void DebugDbBaseItemTypes();
	void DebugDbItemClasses();
	void DebugDbSkillGems();
	void DebugDbGenTags();
	void DebugDbGrantedEffects();
	void DebugDbWorldArea();
	void DebugTheDbWorldArea(const std::string& world_area_name);
	void DebugDbStats();
	void DebugDbMapConnections();
	void DebugDbMapPins();
	void DebugDbWeaponTypes();
	void DebugDbClientStrings();
	void DebugDbPassiveSkillGraph();
	void DebugDbChests();
	void DebugMonsterTypes();
	void DebugMonsterVarieties();
	void DebugInventoryItems(int type);
	void DebugFindGridPanelFromDlgProp(int type);
	void DebugDlgItems(int dlg_type);
	void DebugBodyItems(bool include_flask, bool include_hide_weapon);
	void DebugEquipsInsideGem(int type);
	void DebugBodyEquipsGem(bool include_hide_weapon);
	void DebugSkillGems(int type);
	void DebugGemSkill(int type, int flag);
	void DebugBodyGemSkill(bool active_skill, bool passive_skill, bool include_hide_weapon);
	void DebugInventorySkill(int type);
	void DebugMySkillInfo();
	void DebugTheObjSkillInfo(pt_dword entity_id);
	void DebugCurQuestInfo();
	void DebugMapModels(int radius, bool has_obj);
	void DbgPlayerPosModel();
	void DebugInGameState();
	void DebugDlgWaypoint();
	void DebugUiWaypointItems(int difficulty, int flag);
	void DebugTheWaypointItem(int difficulty, const std::string& map_name);
	void DebugUiFullInfo(pt_dword ui_addr, int flag, int state_cate);
	void DebugTheUiInfo(int level, pt_dword root_ui_addr, pt_dword addr, int filter_type, int flag);
	void DebugTheParentUiInfo(pt_dword ui_addr);
	void DebugInGameMainUiAddrInfo();
	void DebugNpcPanel(int panel_type);
	void DebugNpcPanelMenuItemInfo(int type);
	void DebugQuestAwardInventory();
	void DebugUiQuestStateItem();
	void DebugUiQuestItem(int show_flag, int difficulty, int chapter);
	void DebugCallerBuff();
	void DebugQuestLvlStatePanel();
	void DebugDlgSelfMapLocator();
	void DebugLoginState();
	void DebugPreGameState();
	void DebugSelectRoleTop();
	void DebugDlgSelectRole();
	void DebugUiBottom();
	void DebugDlgLabyrinthSelect();
	void DebugDlgTrialPlaque();
	void DebugDlgStash();
	void DebugCreateCharacterState();
	//////////////////////////////////////////////////////////////////////////
}
