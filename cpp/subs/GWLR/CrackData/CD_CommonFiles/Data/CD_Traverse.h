#pragma once
/*
@author zhp
@date 2016/4/11 21:40
*/
#include "CD_Structs.h"


class CD_Traverse{
public:
	typedef std::function<bool(stCD_NetObj*)> DoNetObjFuncT;
	typedef std::function<bool(stCD_Node*)> DoNodeFuncT;
	typedef std::function<bool(stCD_Item*, int idx)> DoItemFuncT;
	typedef std::function<bool(stCD_ItemDb*)> DoItemDbFuncT;
	typedef std::function<bool(stCD_TaskDb*)> DoTaskDbFuncT;
	typedef std::function<bool(stCD_MapDb*)> DoMapDbFuncT;
	typedef std::function<bool(stCD_FarmDb*)> DoFarmDbFuncT;
	typedef std::function<bool(stCD_ShopDb*)> DoShopDbFuncT;
	typedef std::function<bool(stCD_SeedDb*)> DoSeedDbFuncT;
	typedef std::function<bool(stCD_CraftDb*)> DoCraftDbFuncT;
	typedef	std::function<bool(stCD_MenufactureDb*)> DoMenufactureDbFuncT;
	typedef std::function<bool(stCD_GridItemInfo*)> DoGridItemInfoT;
	typedef std::function<bool(stCD_BuffDb*)> DoBuffDbT;
	typedef std::function<bool(stCD_ObjBase*)> DoLocalObjT;
	typedef	std::function<bool(stCD_ShangJinInfo*)> DoShangJinInfoT;
	typedef	std::function<bool(stCD_KingQuestInfo* king_info)> DoKingQuestInfoT;
	typedef std::function<bool(stCD_UiCtrl*)> DoUiCtrlT;
	typedef std::function<bool(stCD_UiMng*)> DoUiMngT;

private:
	struct TraverseExcept : std::exception{
		TraverseExcept(const char* str) : std::exception(str){}
	};

public:
	//通过packet_id组织起来的数据结构，一般只能遍历到NPC
	static int TraverseNetObjs(const DoNetObjFuncT& func);
	//通过id(local id)组织起来的数据结构，可遍历出所有本地对象
	static int TraverseLocalObjs(const DoNodeFuncT& func);
	//遍历物品
	static int TraverseItems(const stCD_GridsMgr& grids_mgr, const DoItemFuncT& func);
	//遍历物品模板
	static int TraverseItemDb(const DoItemDbFuncT& func);
	//遍历副本模板
	static int TraverseMapDb(const DoMapDbFuncT& func);
	//遍历任务模板
	static int TraverseTasksDb(const DoTaskDbFuncT& func);
	//遍历已接任务
	static int TraverseReceivedTask(const DoNodeFuncT& func);
	//遍历farm db
	static int TraverseFarmDb(const DoFarmDbFuncT& func);
	//遍历商品模板
	static int TraverseShopDb(const DoShopDbFuncT& func);
	//遍历种子模板
	static int TraverseSeedDb(const DoSeedDbFuncT& func);
	//遍历锻造模板
	static int TraverseCraftDb(const DoCraftDbFuncT& func);
	//遍历调和模板
	static int TraverseMenufactureDb(const DoMenufactureDbFuncT& func);
	//遍历礼包
	static int TraverseGiftInfo(const DoNodeFuncT& func);
	//遍历补给箱
	static int TraverseSupplyBox(const DoGridItemInfoT& func);
	//遍历邮件
	static int TraverseMails(const DoNodeFuncT& func);
	//遍历buff db
	static int TraverseBuffDb(const DoBuffDbT& func);
	//遍历当前buff
	static int TraverseCurBuff(const DoNodeFuncT& func);
	//遍历所有本地对象
	static int TraverseAllLocalObjs(const DoLocalObjT& func);
	//遍历当前赏金信息
	static int TraverseCurShangJinInfo(const DoShangJinInfoT& func);
	//遍历王立副本
	static int TraverseKingQuestInfo(const DoKingQuestInfoT& func);
	//遍历UI控件
	static int TraverseUiCtrls(const DoUiCtrlT& func);
	//遍历ui mng
	static int TraverseUiMng(const DoUiMngT& func);

public:
	static void DebugInfo_NetObjs();
	static void DebugInfo_LocalObjs();
	static void DebugInfo_CurRoleInfo();
	static void DebugInfo_CurMapInfo();
	static void DebugInfo_AllPackagesMgr();
	static void DebugInfo_TheItemDb(int item_id_min, int item_id_max);
	static void DebugInfo_TheTaskDb(int task_id_min, int task_id_max);
	static void DebugInfo_TheMapDb(int map_id_min, int map_id_max);
	static void DebugInfo_ReceivedTask();
	static void DebugInfo_AccetableTask();
	static void DebugInfo_ChapterInfo();
	static void DebugInfo_EnterableFbInfo();
	static void DebugInfo_TheFarmDb(int farm_db_id_min, int farm_db_id_max);
	static void DebugInfo_CurFarmFacility();
	static void DebugInfo_TheShopDb(int min, int max);
	static void DebugInfo_TheSeedDb(int min, int max);
	static void DebugInfo_TheCraftDb(int min, int max);
	static void DebugInfo_TheMenufactureDb(int min, int max);
	static void DebugInfo_GiftInfo();
	static void DebugInfo_SupplyBox();
	static void DebugInfo_Mails();
	static void DebugInfo_TheBuffDb(int min, int max);
	static void DebugInfo_CurBuff();
	static void DebugInfo_AllLocalObjs();
	static void DebugInfo_TheLocalObjs(CD_En_LocalObjType types[], int cnt);
	static void DebugInfo_CurShangJinInfo();
	static void DebugInfo_KingQuestInfo();
	static void DebugInfo_CurUiCtrlInfo();
	static void DebugInfo_UiMng();

	//其他
public:
	static void TraTest(int a);

public:
	static int NodeTraverse(stCD_Node* sub_tree_root, const DoNodeFuncT& func);

private:
	static int NodeRecurseTraverse(stCD_Node* sub_tree_root, stCD_Node* null_node, const DoNodeFuncT& func);
	static int TraverseItemDbImpl(int item_id_min, int item_id_max, const DoItemDbFuncT& func);
	static int TraverseTaskDbImpl(int task_id_min, int task_id_max, const DoTaskDbFuncT& func);
	static int TraverseMapDbImpl(int map_id_min, int map_id_max, const DoMapDbFuncT& func);
	static int TraverseFarmDbImpl(int farm_db_id_min, int farm_db_id_max, const DoFarmDbFuncT& func);
	static int TraverseShopDbImpl(CD_En_ShopInfoCategory cate_min, CD_En_ShopInfoCategory cate_max, const DoShopDbFuncT& func);
	static int TraverseSeedDbImpl(int item_id_min, int item_id_max, const DoSeedDbFuncT& func);
	static int TraverseCraftDbImpl(int craft_id_min, int craft_id_max, const DoCraftDbFuncT& func);
	static int TraverseMenufactureDbImpl(int id_min, int id_max, const DoMenufactureDbFuncT& func);
	static int TraverseBuffDbImpl(int id_min, int id_max, const DoBuffDbT& func);
	static void DebugInfoTheObj(stCD_ObjBase* obj);
};