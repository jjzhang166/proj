#pragma once
/*
@author zhp
@date 2016/4/11 21:38
*/
#include <ProtocolX/P_BaseTypeX.h>
#include "CD_Structs.h"

#ifdef _DEBUG
extern "C" __declspec(dllexport)
#endif
void FnForOd(const char* str);

class CD_FuncCall{
	friend struct stCD_MsgBase;
public:
	typedef std::array<stCD_ChapterInfo, 3> ChapterInfoT;

public:
	static stCD_GloRoleInfoData* GetGloRoleInfoData();
	static stCD_NetObjRealCont* GetAllNetObjs();
	static stCD_GloLocalIdMgr* GetGloLocalIdMgr();
	static stCD_LocalObjNode* GetLocalObjsNode();
	static stCD_MapDb* GetCurMapDb();
	static stCD_NetObj_Role* GetCurRole();
	static stCD_PackagesMgr* GetPackagesMgr();
	static stCD_TaskNode* GetReceivedTaskRootNode();
	static ChapterInfoT* GetChapterInfo();
	static stCD_EnterableFbMgr* GetEnterableFbMgr();
	static stCD_FarmFacility* GetCurFarmFacility();
	static stCD_MenufactureObj* GetMenufactureObj();
	static stCD_GiftNode* GetGiftRootNode();
	static stCD_GridItemNode* GetSupplyFirstItemNode();
	static stCD_MailNode* GetMailRootNode();
	static stCD_BuffMgr* GetBuffMgr();
	static stCD_AllLocalObjMgr* GetAllLocalObjMgr();
	static stCD_SocketMgr3* GetSocketMgr();
	static stCD_ShangJinInfoMgr* GetShangJinInfoMgr();
	static bool GetShangJinInfoArray(stCD_ShangJinArrayT& out_array);
	static stCD_KingQuestFbInfo* GetKingQuestFbInfo(CD_En_KingFbType king_fb_type);
	static stCD_UseItemMgr* GetUseItemMgr();
	static stCD_UiMgr* GetUiMgr();
	static stCD_UiGlobalMng* GetUIGlobalMng();

public:
	static stCD_NetObj* FindNetObj(pt_dword packet_id);
	static stCD_LocalObj* FindLocalObj(pt_dword local_id);
	static stCD_ItemDb* FindItemDb(int item_id);
	static stCD_TaskDb* FindTaskDb(int task_id);
	static stCD_MapDb* FindMapDb(int fb_id);
	static stCD_FarmDb* FindFarmDb(int farm_db_id);
	static stCD_FarmTimeDb* FindFarmTimeDbBy(int farm_db_id);
	static stCD_ShopDb* FindShopDb(CD_En_ShopInfoCategory shop_db_id);
	static stCD_SeedDb* FindSeedDb(int item_id);
	static stCD_CraftDb* FindCraftDb(int craft_id);
	static stCD_MenufactureDb* FindMenufactureDb(int id);
	static stCD_BuffDb* FindBuffDb(int id);
	static stCD_ReqItem* FindReqItem(pt_word ukn, pt_pcint id_ptr);
	static stCD_UiCtrl* FindUiCtrl(const pt_csz cls_name[], int cnt);
	static stCD_UiCtrl* FindUiCtrlByFullClsName(pt_csz full_cls_name);
	static stCD_UiMng_Minimap* FindUiMng_Minimap();

public:
	static int DecryptValue(const stCD_EncryptInt* addr);
	static void EncryptInt(const stCD_EncryptInt* addr, int value);
	static bool CanAcceptTheTask(int task_id);
	static bool CanAcceptTheTask(const stCD_TaskDb& task_db);
	static bool IsTheFbDone(int fb_id);
	static int GetFarmDbId(CD_En_FarmFacilityIdx facility_idx, int facility_lvl);
	static int GetFarmLeftTime(int farm_db_id, int cur_time);
	static int GetFarmFieldLeftTime(stCD_FarmFacility* farm_facility, CD_En_FarmFieldIdx idx);
	static stCD_Direction2 CalcHitSendDir(const stCD_PosDirInfo& pos_dir_info);
	//获取剩余可领取周狩猎券
	static int GetWeekCreditCnt();
	//已领取周狩猎券数量
	static int GetGainedWeekCreditCnt();
	static bool HandleMouseMsg(int x, int y, CD_En_MouseOper oper);
	static bool PressKey(DWORD virtual_key, bool is_down);
	static int GetShangJinCompleteCnt();
	static int GetShangJinTotalCnt();
	static int GetShangJinTaskState(int task_id);
	//整理包裹
	static bool TidyInventoryItem(CD_En_TabCategory tab_cate);
	static pt_dint GetSynCoorDinatesRandom();
	static bool UiCtrl_LeftBtClick(const pt_csz cls_name[], int cnt);
	static bool UiCtrl_LeftBtClickByFullClsName(pt_csz cls_full_name);
	static bool UiCtrlEdit_PutString(stCD_UiCtrl* ui_ctrl, pt_cwsz str);

	//发包
public:
	template<typename T>
	static bool SendT(const T& msg){
		auto ptr_msg = (stCD_MsgBase*)GetSendMsgBuffer();
		if (!ptr_msg->EncodeMsg(msg))
		{
			assert(false);
			return false;
		}
		return SendImplSync(*ptr_msg);
	}

private:
	static stCD_GlobalData* GetGlobalData();
	static stCD_NetObjMgr* GetNetObjsMgr();
	static stCD_UknRoleInfo1* GetUknRoleInfo1();
	static bool SendImplSync(stCD_MsgBase& msg);
	static bool SendImpl(stCD_MsgBase& msg);
	static CD_MsgBufferType& GetSendMsgBuffer();
	static CD_MsgBufferType& GetEncodeMsgBuffer();
	static stCD_WeekCreditInfoImpl* GetWeekCreditInfo();
};
