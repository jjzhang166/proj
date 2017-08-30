#include "stdafx.h"
#include "CD_Traverse.h"
#include "CD_FuncCall.h"
#include <CommonX/CmnX_UsefulFunc.h>
#include <CmnMix/Cmn_LogOut.h>
#include <stack>
#include "CD_UsefulFns.h"

int CD_Traverse::TraverseNetObjs(const DoNetObjFuncT& func)
{
	//这是通过packet id获得对象地址的算法
	//汇编引用
	/*
	$ ==>    119DB250     55              PUSH EBP
	$+1      119DB251     8BEC            MOV EBP, ESP
	$+3      119DB253     8B45 0C         MOV EAX, DWORD PTR SS:[EBP+0xC]
	$+6      119DB256     56              PUSH ESI
	$+7      119DB257     8B30            MOV ESI, DWORD PTR DS:[EAX]
	$+9      119DB259     57              PUSH EDI
	$+A      119DB25A     8B79 0C         MOV EDI, DWORD PTR DS:[ECX+0xC]          ; MHOClien.01021000
	$+D      119DB25D     2B79 08         SUB EDI, DWORD PTR DS:[ECX+0x8]
	$+10     119DB260     33D2            XOR EDX, EDX
	$+12     119DB262     C1FF 02         SAR EDI, 0x2
	$+15     119DB265     4F              DEC EDI
	$+16     119DB266     8BC6            MOV EAX, ESI
	$+18     119DB268     F7F7            DIV EDI
	$+1A     119DB26A     8B49 08         MOV ECX, DWORD PTR DS:[ECX+0x8]
	$+1D     119DB26D     8D0491          LEA EAX, DWORD PTR DS:[ECX+EDX*4]
	$+20     119DB270     8B08            MOV ECX, DWORD PTR DS:[EAX]
	$+22     119DB272     8B40 04         MOV EAX, DWORD PTR DS:[EAX+0x4]
	$+25     119DB275     3BC8            CMP ECX, EAX
	$+27     119DB277     0F95C2          SETNE DL
	$+2A     119DB27A     84D2            TEST DL, DL
	$+2C     119DB27C     74 12           JE SHORT CryGame.119DB290
	$+2E     119DB27E     8BFF            MOV EDI, EDI
	$+30     119DB280     3971 04         CMP DWORD PTR DS:[ECX+0x4], ESI
	$+33     119DB283     74 0B           JE SHORT CryGame.119DB290
	$+35     119DB285     8B09            MOV ECX, DWORD PTR DS:[ECX]
	$+37     119DB287     3BC8            CMP ECX, EAX
	$+39     119DB289     0F95C2          SETNE DL
	$+3C     119DB28C     84D2            TEST DL, DL
	$+3E     119DB28E   ^ 75 F0           JNZ SHORT CryGame.119DB280
	$+40     119DB290     5F              POP EDI                                  ; ntdll_1.77B8FD91
	$+41     119DB291     3BC8            CMP ECX, EAX
	$+43     119DB293     8B45 08         MOV EAX, DWORD PTR SS:[EBP+0x8]
	$+46     119DB296     5E              POP ESI                                  ; ntdll_1.77B8FD91
	$+47     119DB297     74 06           JE SHORT CryGame.119DB29F
	$+49     119DB299     8908            MOV DWORD PTR DS:[EAX], ECX              ; ntdll_1.77B801F7
	$+4B     119DB29B     5D              POP EBP                                  ; ntdll_1.77B8FD91
	$+4C     119DB29C     C2 0800         RETN 0x8
	$+4F     119DB29F     C700 00000000   MOV DWORD PTR DS:[EAX], 0x0
	$+55     119DB2A5     5D              POP EBP                                  ; ntdll_1.77B8FD91
	$+56     119DB2A6     C2 0800         RETN 0x8
	*/
	auto objs = CD_FuncCall::GetAllNetObjs();
	if (!objs)
		return 0;
	if (IsUnValidPtr(objs->obj_begin_ptr_) || IsUnValidPtr(objs->obj_end_ptr_))
	{
		assert(false);
		return 0;
	}
	if (objs->obj_begin_ptr_ > objs->obj_end_ptr_)
	{
		assert(false);
		return 0;
	}
	//LOG_O(Log_trace) << "stCD_ObjRealCont Addr:" << objs;
	int cnt = 0;
	for (auto beg = objs->obj_begin_ptr_; beg < objs->obj_end_ptr_; ++beg)
	{
		auto node = *beg;
		if (IsUnValidPtr(node))
			continue;
		auto next_node = *(beg + 1);
		if (next_node == node)
			continue;
		if (IsUnValidPtr(node->game_obj_))
		{
			assert(false);
			break;
		}
		if (!node->game_obj_->ObjValid())
			continue;
		++cnt;
		if (!func(node->game_obj_))
			break;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_NetObjs()
{
	TraverseNetObjs([](stCD_NetObj* obj){
		/*if (CD_En_NetObjType::kNpc == obj->type_)
		{
			LOG_O(Log_trace) << std::hex << " net obj:" << *obj->CastTo<stCD_NetObj_Npc>();
		}
		else*/
		{
			//auto local_obj = CD_FuncCall::FindLocalObj(obj->id_);
			if (IsUnValidPtr(obj->local_obj_))
			{
				assert(false);
				return true;
			}
			auto obj_info = obj->local_obj_->obj_info_;
			if (IsUnValidPtr(obj_info))
			{
				LOG_O(Log_trace) << std::hex << "没有obj_info对象 net obj:" << *obj;
				return true;
			}
			auto the_type = obj_info->GetType();
			if (CD_En_LocalObjType::kNpc == the_type)
			{
				LOG_O(Log_trace) << std::hex << " 本地npc对象：" << obj_info << " local_obj_type:" << obj_info->type_ <<
					" npc_obj:" << *stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kNpc>::get(obj);
			}
			else if (CD_En_LocalObjType::kCollect == the_type)
			{
				LOG_O(Log_trace) << std::hex << " 本地采集对象：" << obj_info << " local_obj_type:" << obj_info->type_ <<
					" obj:" << *stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kCollect>::get(obj);
			}
			else if (CD_En_LocalObjType::kDoor1 == the_type || CD_En_LocalObjType::kDoor2 == the_type)
			{
				LOG_O(Log_trace) << std::hex << " 本地门对象：" << obj_info << " local_obj_type:" << obj_info->type_ <<
					" obj:" << *stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kDoor1>::get(obj);
			}
			else
			{
				LOG_O(Log_trace) << std::hex << " 本地对象地址：" << obj_info << " local_obj_type:" << obj_info->type_ << " net_obj:" << *obj;
			}
		}
		return true;
	});
}

int CD_Traverse::NodeRecurseTraverse(stCD_Node* sub_tree_root, stCD_Node* null_node, const DoNodeFuncT& func)
{
	int cnt = 0;
	if (sub_tree_root != null_node)
	{
		if (!func(sub_tree_root))
			return 0;
		cnt = 1;

		if (!IsUnValidPtr(sub_tree_root->left_))
			cnt += NodeRecurseTraverse(sub_tree_root->left_, null_node, func);
		if (!IsUnValidPtr(sub_tree_root->right_))
			cnt += NodeRecurseTraverse(sub_tree_root->right_, null_node, func);
	}
	return cnt;
}

int CD_Traverse::TraverseLocalObjs(const DoNodeFuncT& func)
{
	auto node = CD_FuncCall::GetLocalObjsNode();
	if (IsUnValidPtr(node))
		return 0;
	
	return NodeTraverse(node, func);
	//return NodeRecurseTraverse(node->parent_, node, func);
}

void CD_Traverse::DebugInfo_LocalObjs()
{
	TraverseLocalObjs([](stCD_Node* cur_node){
		auto the_obj = cur_node->CastTo<stCD_LocalObjNode>();
		if (IsUnValidPtr(the_obj->obj_))
			return false;
		auto local_obj = the_obj->obj_;
		if (IsUnValidPtr(local_obj->obj_info_))
			return false;
		if (!local_obj->ObjValid())
			return false;
		LOG_O(Log_trace) << std::hex << " stCD_LocalObjNode:" << *the_obj;
		return true;
	});
}

int CD_Traverse::NodeTraverse(stCD_Node* tree_root, const DoNodeFuncT& func)
{
	if (IsUnValidPtr(tree_root) || IsUnValidPtr(tree_root->parent_))
	{
		//assert(false);
		return 0;
	}
	if (tree_root == tree_root->parent_)
		return 0;
	//LOG_O(Log_trace) << "root_node_addr:" << tree_root;

	std::stack<stCD_Node*> btree_stack;
	btree_stack.push(tree_root->parent_);
	int cnt = 0;
	while (!btree_stack.empty())
	{
		auto cur_node_ptr = btree_stack.top();
		while (true)
		{
			if (!cur_node_ptr || cur_node_ptr == tree_root)
				break;
			if (func(cur_node_ptr))
				++cnt;

			btree_stack.push(cur_node_ptr->left_);
			cur_node_ptr = cur_node_ptr->left_;
		}
		btree_stack.pop();
		if (!btree_stack.empty())
		{
			cur_node_ptr = btree_stack.top();
			btree_stack.pop();
			btree_stack.push(cur_node_ptr->right_);
		}
	}
	return cnt;
}

void CD_Traverse::DebugInfo_CurRoleInfo()
{
	auto role = CD_FuncCall::GetCurRole();
	if (!role)
	{
		assert(false);
		return;
	}
	LOG_O(Log_trace) << std::hex << *role;
}

void CD_Traverse::DebugInfo_CurMapInfo()
{
	auto map_info = CD_FuncCall::GetCurMapDb();
	if (!map_info)
		return;
	LOG_O(Log_trace) << std::hex << *map_info;
}

int CD_Traverse::TraverseItems(const stCD_GridsMgr& grids_mgr, const DoItemFuncT& func)
{
	auto grids_max_cnt = grids_mgr.grids_max_cnt_;
	if (grids_max_cnt <= 0 || grids_max_cnt >= 900)
	{
		assert(false);
		return 0;
	}
	if (IsUnValidPtr(grids_mgr.item_cont_))
	{
		assert(false);
		return 0;
	}
	int idx = 0, cnt = 0;
	for (auto grid_item = grids_mgr.item_cont_->grid_items_; idx < grids_max_cnt; ++idx, ++grid_item)
	{
		if (IsUnValidPtr(grid_item->item_))
		{
			//assert(false);
			continue;
		}
		if (IsUnValidPtr(grid_item->item_->item_db_))
		{
			assert(false);
			continue;
		}
		/*if (grid_item->item_->item_cnt_ <= 0)
		{
			assert(false);
			continue;
		}*/
		if (!func(grid_item->item_, idx))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_AllPackagesMgr()
{
	auto mgr = CD_FuncCall::GetPackagesMgr();
	if (!mgr)
		return;
	mgr->DebugInfo();
}

void CD_Traverse::DebugInfo_TheItemDb(int item_id_min, int item_id_max)
{
	TraverseItemDbImpl(item_id_min, item_id_max, [](stCD_ItemDb* item_db){
		LOG_O(Log_trace) << std::hex << *item_db;
		return true;
	});
}

int CD_Traverse::TraverseItemDb(const DoItemDbFuncT& func)
{
	return TraverseItemDbImpl(0, kMaxItemDbCnt, func);
}

int CD_Traverse::TraverseItemDbImpl(int item_id_min, int item_id_max, const DoItemDbFuncT& func)
{
	int cnt = 0;
	for (; item_id_min <= item_id_max; ++item_id_min)
	{
		auto item_db = CD_FuncCall::FindItemDb(item_id_min);
		if (!item_db)
			continue;
		if (!func(item_db))
			break;
		++cnt;
	}
	return cnt;
}

int CD_Traverse::TraverseTasksDb(const DoTaskDbFuncT& func)
{
	return TraverseTaskDbImpl(0, kMaxTaskDbId, func);
}

void CD_Traverse::DebugInfo_TheTaskDb(int task_id_min, int task_id_max)
{
	TraverseTaskDbImpl(task_id_min, task_id_max, [](stCD_TaskDb* task_db){
		LOG_O(Log_trace) << std::hex << *task_db;
		return true;
	});
}

int CD_Traverse::TraverseTaskDbImpl(int task_id_min, int task_id_max, const DoTaskDbFuncT& func)
{
	int cnt = 0;
	for (; task_id_min <= task_id_max; ++task_id_min)
	{
		auto task_db = CD_FuncCall::FindTaskDb(task_id_min);
		if (!task_db)
			continue;
		if (!func(task_db))
			break;
		++cnt;
	}
	return cnt;
}

int CD_Traverse::TraverseMapDbImpl(int map_id_min, int map_id_max, const DoMapDbFuncT& func)
{
	int cnt = 0;
	for (; map_id_min <= map_id_max; ++map_id_min)
	{
		auto fb_db = CD_FuncCall::FindMapDb(map_id_min);
		if (!fb_db)
			continue;
		if (IsUnValidPtr(fb_db->map_name_))
			continue;
		if (!func(fb_db))
			break;
		++cnt;
	}
	return cnt;
}

int CD_Traverse::TraverseMapDb(const DoMapDbFuncT& func)
{
	return TraverseMapDbImpl(1, kMaxFbDbId, func);
}

void CD_Traverse::DebugInfo_TheMapDb(int map_id_min, int map_id_max)
{
	TraverseMapDbImpl(map_id_min, map_id_max, [](stCD_MapDb* fb_db){
		LOG_O(Log_trace) << std::hex << *fb_db;
		return true;
	});
}

int CD_Traverse::TraverseReceivedTask(const DoNodeFuncT& func)
{
	auto node = CD_FuncCall::GetReceivedTaskRootNode();
	if (IsUnValidPtr(node))
		return 0;

	return NodeTraverse(node, func);
}

void CD_Traverse::DebugInfo_ReceivedTask()
{
	TraverseReceivedTask([](stCD_Node* cur_node){
		auto the_obj = cur_node->CastTo<stCD_TaskNode>();
		if (IsUnValidPtr(the_obj->task_))
			return false;
		LOG_O(Log_trace) << std::hex << " stCD_TaskNode:" << *the_obj->task_;
		return true;
	});
}

void CD_Traverse::DebugInfo_AccetableTask()
{
	TraverseTasksDb([](stCD_TaskDb* task_db){
		if (CD_FuncCall::CanAcceptTheTask(*task_db))
		{
			LOG_O(Log_trace) << std::hex << " accetable:" << *task_db;
		}
		return true;
	});
}

void CD_Traverse::DebugInfo_ChapterInfo()
{
	auto chapter_info = CD_FuncCall::GetChapterInfo();
	if (!chapter_info)
		return;
	for (auto& v : *chapter_info )
	{
		v.DebugInfo();
	}
}

void CD_Traverse::DebugInfo_EnterableFbInfo()
{
	auto mgr = CD_FuncCall::GetEnterableFbMgr();
	if (!mgr)
		return;
	mgr->DebugInfo();
}

int CD_Traverse::TraverseFarmDbImpl(int farm_db_id_min, int farm_db_id_max, const DoFarmDbFuncT& func)
{
	int cnt = 0;
	for (; farm_db_id_min <= farm_db_id_max; ++farm_db_id_min)
	{
		auto db = CD_FuncCall::FindFarmDb(farm_db_id_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_TheFarmDb(int farm_db_id_min, int farm_db_id_max)
{
	TraverseFarmDbImpl(farm_db_id_min, farm_db_id_max, [](stCD_FarmDb* db){
		db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseFarmDb(const DoFarmDbFuncT& func)
{
	return TraverseFarmDbImpl(0, kMaxFarmDbId, func);
}

void CD_Traverse::DebugInfo_CurFarmFacility()
{
	auto farm_facility = CD_FuncCall::GetCurFarmFacility();
	if (!farm_facility)
	{
		assert(false);
		return;
	}
	farm_facility->DebugInfo();
}

int CD_Traverse::TraverseShopDbImpl(CD_En_ShopInfoCategory cate_min, CD_En_ShopInfoCategory cate_max, const DoShopDbFuncT& func)
{
	int cnt = 0;
	for (; cate_min <= cate_max; cate_min = CD_En_ShopInfoCategory((int)cate_min + 1))
	{
		auto db = CD_FuncCall::FindShopDb(cate_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

int CD_Traverse::TraverseShopDb(const DoShopDbFuncT& func)
{
	return TraverseShopDbImpl(CD_En_ShopInfoCategory(0), CD_En_ShopInfoCategory(kMaxShopDbId), func);
}

void CD_Traverse::DebugInfo_TheShopDb(int min, int max)
{
	TraverseShopDbImpl(CD_En_ShopInfoCategory(min), CD_En_ShopInfoCategory(max), [](stCD_ShopDb* shop_db){
		shop_db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseSeedDbImpl(int item_id_min, int item_id_max, const DoSeedDbFuncT& func)
{
	int cnt = 0;
	for (; item_id_min <= item_id_max; ++item_id_min)
	{
		auto db = CD_FuncCall::FindSeedDb(item_id_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_TheSeedDb(int min, int max)
{
	TraverseSeedDbImpl(min, max, [](stCD_SeedDb* db){
		db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseSeedDb(const DoSeedDbFuncT& func)
{
	return TraverseSeedDbImpl(0, kMaxSeedDbId, func);
}

int CD_Traverse::TraverseCraftDbImpl(int craft_id_min, int craft_id_max, const DoCraftDbFuncT& func)
{
	int cnt = 0;
	for (; craft_id_min <= craft_id_max; ++craft_id_min)
	{
		auto db = CD_FuncCall::FindCraftDb(craft_id_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_TheCraftDb(int min, int max)
{
	TraverseCraftDbImpl(min, max, [](stCD_CraftDb* db){
		db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseCraftDb(const DoCraftDbFuncT& func)
{
	return TraverseCraftDbImpl(0, kMaxCraftDbId, func);
}

int CD_Traverse::TraverseMenufactureDbImpl(int id_min, int id_max, const DoMenufactureDbFuncT& func)
{
	int cnt = 0;
	for (; id_min <= id_max; ++id_min)
	{
		auto db = CD_FuncCall::FindMenufactureDb(id_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_TheMenufactureDb(int min, int max)
{
	TraverseMenufactureDbImpl(min, max, [](stCD_MenufactureDb* db){
		db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseMenufactureDb(const DoMenufactureDbFuncT& func)
{
	return TraverseMenufactureDbImpl(0, kMaxMenufactureDbId, func);
}

void CD_Traverse::DebugInfo_GiftInfo()
{
	TraverseGiftInfo([](stCD_Node* ptr){
		auto node = ptr->CastTo<stCD_GiftNode>();
		node->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseGiftInfo(const DoNodeFuncT& func)
{
	auto node = CD_FuncCall::GetGiftRootNode();
	if (!node)
		return 0;
	return NodeTraverse(node, func);
}

int CD_Traverse::TraverseSupplyBox(const DoGridItemInfoT& func)
{
	auto node = CD_FuncCall::GetSupplyFirstItemNode();
	if (!node)
		return 0;
	int cnt = 0;
	for (auto it = node->next_node_; !IsUnValidPtr(it) && it != node; it = it->next_node_)
	{
		if (IsUnValidPtr(it->cur_item_))
			continue;
		if (!func(it->cur_item_))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_SupplyBox()
{
	TraverseSupplyBox([](stCD_GridItemInfo* item){
		item->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseMails(const DoNodeFuncT& func)
{
	auto node = CD_FuncCall::GetMailRootNode();
	if (!node)
		return 0;
	return NodeTraverse(node, func);
}

void CD_Traverse::DebugInfo_Mails()
{
	TraverseMails([](stCD_Node* ptr){
		auto node = ptr->CastTo<stCD_MailNode>();
		node->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseBuffDbImpl(int id_min, int id_max, const DoBuffDbT& func)
{
	int cnt = 0;
	for (; id_min <= id_max; ++id_min)
	{
		auto db = CD_FuncCall::FindBuffDb(id_min);
		if (!db)
			continue;
		if (!func(db))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_TheBuffDb(int min, int max)
{
	TraverseBuffDbImpl(min, max, [](stCD_BuffDb* db){
		db->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseBuffDb(const DoBuffDbT& func)
{
	return TraverseBuffDbImpl(0, kMaxBuffDbId, func);
}

int CD_Traverse::TraverseCurBuff(const DoNodeFuncT& func)
{
	auto buf_mgr = CD_FuncCall::GetBuffMgr();
	if (!buf_mgr)
		return 0;
	auto res = NodeTraverse(&buf_mgr->buf_root_node1_, func);
	res += NodeTraverse(&buf_mgr->buf_root_node2_, func);
	return res;
}

void CD_Traverse::DebugInfo_CurBuff()
{
	TraverseCurBuff([](stCD_Node* node){
		auto ptr = node->CastTo<stCD_BuffNode>();
		ptr->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseAllLocalObjs(const DoLocalObjT& func)
{
	auto mgr = CD_FuncCall::GetAllLocalObjMgr();
	if (!mgr)
		return 0;
	return mgr->all_objs_.Traverse([&func](stCD_ObjBase** obj){
		auto ptr = *obj;
		if (IsUnValidPtr(ptr))
			return true;
		return func(ptr);
	});
}

void CD_Traverse::DebugInfo_AllLocalObjs()
{
	TraverseAllLocalObjs([](stCD_ObjBase* obj){
		DebugInfoTheObj(obj);
		return true;
	});
}

int CD_Traverse::TraverseCurShangJinInfo(const DoShangJinInfoT& func)
{
	stCD_ShangJinArrayT shang_jin_array = { 0 };
	if (!CD_FuncCall::GetShangJinInfoArray(shang_jin_array))
		return 0;
	return shang_jin_array.Traverse([&func](stCD_ShangJinInfo** ptr){
		auto info = *ptr;
		if (IsUnValidPtr(info))
		{
			assert(false);
			return true;
		}
		return func(info);
	});
}

void CD_Traverse::DebugInfo_CurShangJinInfo()
{
	TraverseCurShangJinInfo([](stCD_ShangJinInfo* ptr){
		ptr->DebugInfo();
		return true;
	});
}

int CD_Traverse::TraverseKingQuestInfo(const DoKingQuestInfoT& func)
{
	int cnt = 0;
	auto ptr = CD_FuncCall::GetKingQuestFbInfo(CD_En_KingFbType::kKingOneTwo);
	if (ptr)
	{
		cnt = ptr->fb_info_.Traverse(func);
	}
	else
		assert(false);
	ptr = CD_FuncCall::GetKingQuestFbInfo(CD_En_KingFbType::kKingThreeFour);
	if (ptr)
	{
		cnt = ptr->fb_info_.Traverse(func);
	}
	else
		assert(false);

	ptr = CD_FuncCall::GetKingQuestFbInfo(CD_En_KingFbType::kKingFive);
	if (ptr)
	{
		cnt = ptr->fb_info_.Traverse(func);
	}
	else
		assert(false);

	ptr = CD_FuncCall::GetKingQuestFbInfo(CD_En_KingFbType::kKingSix);
	if (ptr)
	{
		cnt = ptr->fb_info_.Traverse(func);
	}
	else
		assert(false);
	return cnt;
}

void CD_Traverse::DebugInfo_KingQuestInfo()
{
	TraverseKingQuestInfo([](stCD_KingQuestInfo* ptr){
		ptr->DebugInfo();
		return true;
	});
}

void CD_Traverse::TraTest(int a)
{
	pt_csz cls_name[] = { "item1", "rolelist", "root1" };
	TraverseUiCtrls([&cls_name](stCD_UiCtrl* ui_ctrl){
		if (ui_ctrl->MatchTheFullClassName(cls_name, _countof(cls_name)))
		{
			char buffer[0xff];
			ui_ctrl->GetFullClassName(buffer, sizeof(buffer));
			LOG_O(Log_trace) << "找到了，full_class_name:" << buffer;
			return false;
		}
		return true;
	});
}

void CD_Traverse::DebugInfoTheObj(stCD_ObjBase* obj)
{
	auto the_type = obj->GetType();
	if (CD_En_LocalObjType::kStatic == the_type)
	{
		LOG_O(Log_trace) << "static：" << *obj;
	}
	else if (CD_En_LocalObjType::kNpc == the_type)
	{
		LOG_O(Log_trace) << "npc：" << *obj;
	}
	else if (CD_En_LocalObjType::kPlayer1 == the_type || CD_En_LocalObjType::kPlayer2 == the_type)
	{
		LOG_O(Log_trace) << "player：" << *obj;
	}
	else if (CD_En_LocalObjType::kCollect == the_type)
	{
		auto collect_obj = stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kCollect>::get(obj->net_obj_);
		LOG_O(Log_trace) << "采集：" << " 采集物：" << *collect_obj << " 本地对象：" << *obj;
	}
	else if (CD_En_LocalObjType::kDoor1 == the_type || CD_En_LocalObjType::kDoor2 == the_type)
	{
		LOG_O(Log_trace) << "门：" << *obj;
	}
	else if (CD_En_LocalObjType::kAppear == the_type)
	{
		LOG_O(Log_trace) << "appear：" << *obj;
	}
	else if (CD_En_LocalObjType::kRock == the_type)
	{
		LOG_O(Log_trace) << "rock：" << *obj;
	}
	else
	{
		LOG_O(Log_trace) << "未知：" << *obj;
	}
}

void CD_Traverse::DebugInfo_TheLocalObjs(CD_En_LocalObjType types[], int cnt)
{
	TraverseAllLocalObjs([=](stCD_ObjBase* obj){
		auto the_type = obj->GetType();
		for (int i = 0; i < cnt; ++i)
		{
			if (the_type == types[i])
			{
				DebugInfoTheObj(obj);
				return true;
			}
		}
		return true;
	});
}

void CD_Traverse::DebugInfo_CurUiCtrlInfo()
{
	TraverseUiCtrls([](stCD_UiCtrl* ui_ctrl){
		char buffer[0xff];
		if (ui_ctrl->GetFullClassName(buffer, sizeof(buffer)) > 0)
		{
			LOG_O(Log_trace) << "class name:" << buffer;
		}
		return true;
	});
}

int CD_Traverse::TraverseUiCtrls(const DoUiCtrlT& func)
{
	auto mgr = CD_FuncCall::GetUiMgr();
	if (!mgr)
	{
		return 0;
	}
	auto the_ui_ctrl = mgr->ui_ctrl_;
	int cnt = 0;
	for (int i = 0; i != 10000 && !IsUnValidPtr(the_ui_ctrl); ++i, the_ui_ctrl = the_ui_ctrl->next_)
	{
		if (!the_ui_ctrl->IsVisible())
			continue;
		if (!func(the_ui_ctrl))
			break;
		++cnt;
	}
	return cnt;
}

int CD_Traverse::TraverseUiMng(const DoUiMngT& func)
{
	auto ui_glo_mng = CD_FuncCall::GetUIGlobalMng();
	if (!ui_glo_mng)
		return 0;
	int cnt = 0;
	for (auto it = ui_glo_mng->next_; it != ui_glo_mng && !IsUnValidPtr(it); it = it->next_)
	{
		if (IsUnValidPtr(it->ui_mng_))
			continue;
		if (!func(it->ui_mng_))
			break;
		++cnt;
	}
	return cnt;
}

void CD_Traverse::DebugInfo_UiMng()
{
	TraverseUiMng([](stCD_UiMng* ui_mng){
		if (IsUnValidPtr(ui_mng->name_))
			return true;
		if (strcmp(ui_mng->name_, "Minimap") == 0)
		{
			auto minimap = ui_mng->CastTo<stCD_UiMng_Minimap>();
			LOG_O(Log_trace) << *minimap;
		}
		else
		{
			LOG_O(Log_trace) << *ui_mng;
		}
		return true;
	});
}

