#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GFuncCall.h"
#include "Facility/GClass.h"
#include <boost/thread/thread.hpp>
#include "FuncObjC.h"
#include "Facility/GClass.h"
#include "Facility/GPlayer.h"
#include "Facility/GMap.h"

using namespace GFuncCall;
using namespace GClass;

bool is_terminate_script_run = false;

//设置脚本输出信息
void SetScriptOutputInfo( const char* output_info )
{
	if ( !output_info )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "output_info为空";
		return;
	}
	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ThisMemCall(nullptr, "SetScriptOutputInfo", &JsonEcho::MakeParams(output_info));
	}
}

//echo message
void EchoMsg( const char* msg )
{
	if ( !msg )
		return;
	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ThisMemCall(nullptr, 
			"EchoMsg", &JsonEcho::MakeParams(msg));
	}
}

//换号
void ExchangeGameAc()
{
	auto func_obj = FuncObj_NormalC::GetInstance();
	if ( !func_obj )
		return;
	func_obj->GetSessionOper().ThisMemCall(nullptr, "ExchangeGameAc", nullptr);
}

bool RefreshMyProductList()
{
	return GPlayerSelf::GetMe().RefreshMyProductList();
}

void ResetTaskBlacklist()
{
	auto& gps = GPlayerSelf::GetMe();
	gps.Update();
	gps.ResetTaskBlacklist();
}

bool AddTaskBlacklist( const luabind::object& quest_ch_name_obj, const luabind::object& key_content_obj, const std::wstring& match_quest_id )
{
	using namespace luabind;
	auto obj_type = type(quest_ch_name_obj);
	if ( LUA_TSTRING != obj_type )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加任务黑名单之任务名必须是字符串类型";
		return false;
	}
	std::string quest_cn_name = ObjectCast(quest_ch_name_obj, kEmptyStr);
	if (quest_cn_name.empty())
		return false;
	std::string key_content;
	if (key_content_obj.is_valid())
	{
		obj_type = type(key_content_obj);
		if ( LUA_TNIL != obj_type )
		{
			if ( LUA_TSTRING != obj_type )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "添加任务黑名单之关键内容必须是字符串类型";
				return false;
			}
			key_content = ObjectCast(key_content_obj, kEmptyStr);
		}
	}

	const auto& quest_index = GameEn2ChNodeMgr::GetMe().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}
	const auto& quest_id = quest_index->ThreeCh2EnA_(quest_cn_name, key_content, match_quest_id);
	if (quest_id.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加任务黑名单失败->" << quest_cn_name;
		return false;
	}
	GPlayerSelf::GetMe().AddTaskBlacklist(quest_id);
	return true;
}

bool AddTaskBlacklist_1_( const luabind::object& quest_ch_name_obj )
{
	return AddTaskBlacklist(quest_ch_name_obj, luabind::object(), kEmptyWStr);
}

bool AddTaskBlacklist_2_( const luabind::object& quest_ch_name_obj, const luabind::object& key_content_obj )
{
	return AddTaskBlacklist(quest_ch_name_obj, key_content_obj, kEmptyWStr);
}

bool AddTaskBlacklist_MatchMainJob_1_( const luabind::object& quest_ch_name_obj )
{
	auto& job_name = GPlayerSelf::GetMe().GetJobName();
	if (job_name.empty())
	{
		assert(false);
		return false;
	}
	std::wstring str_tmp(L".*");
	str_tmp += job_name;
	str_tmp += L".*";
	return AddTaskBlacklist(quest_ch_name_obj, luabind::object(), str_tmp);
}

bool AddTaskBlacklist_MatchMainJob_2_( const luabind::object& quest_ch_name_obj, const luabind::object& key_content_obj )
{
	auto& job_name = GPlayerSelf::GetMe().GetJobName();
	if (job_name.empty())
	{
		assert(false);
		return false;
	}
	std::wstring str_tmp(L".*");
	str_tmp += job_name;
	str_tmp += L".*";
	return AddTaskBlacklist(quest_ch_name_obj, key_content_obj, str_tmp);
}

bool ProhibitAcceptTheTask( const std::string& quest_ch_name )
{
	if (quest_ch_name.empty())
	{
		assert(false);
		return false;
	}
	GPlayerSelf::GetMe().AddProhibitTask(GbkToWideChar(quest_ch_name));
	return true;
}

bool SendMailGold( const std::string& role_name, int reverse_gold, int delta_gold )
{
	DummySleep(7000);
	if (role_name.empty())
	{
		assert(false);
		return false;
	}
	if ( reverse_gold <= 0 )
		return false;
	return GPlayerSelf::GetMe().SendMailGold(GbkToWideChar(role_name), reverse_gold, delta_gold);
}

int GetRoleLevel()
{
	return GPlayerSelf::GetMe().GetLevel();
}

void ClearBuyProductItems()
{
	GPlayerSelf::GetMe().ClearBuyProductItems();
}

bool AddBuyProductItem( const std::string& item_name, int min_price, int item_cnt, int item_cnt_limit )
{
	return GPlayerSelf::GetMe().AddBuyProductItem(item_name, min_price, item_cnt, item_cnt_limit);
}

void DoBuyProductItems()
{
	GPlayerSelf::GetMe().DoBuyProductItem();
}

void DoUnUseFatigueFb()
{
	struct stUnUseFatigue{
		std::wstring	fb_id_;
	};
	typedef std::deque<stUnUseFatigue> UnUseFatigueFb;
	auto& allowed_mgr = AllowedVillageCloneMapMgr::GetMe();
	if ( !allowed_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return;
	}
	auto iter_fb = allowed_mgr.TraverseCloneMap();
	assert(iter_fb);
	UnUseFatigueFb un_use_fatigue_fb;
	auto& stage_template_mgr = GStageTemplateMgr::GetMe();
	if ( !stage_template_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return;
	}
	auto& fb_map = GVillageFbMap::GetMe();
	stUnUseFatigue un_use_tmp;
	auto& gps = GPlayerSelf::GetMe();
	gps.Update();
	auto player_lvl = gps.GetLevel();
	static const wchar_t* blacklist_fb_ids[] = {
		L"Rg01_Arena", L"Rg05_Arena", L"Rg04_Arena", L"Rg06_Arena", L"Rg03_Arena"
	};
	for ( iter_fb->First(); !iter_fb->IsDone(); iter_fb->Next() )
	{
		auto& allowed = iter_fb->CurItem();
		auto& fb_id = allowed.GetMapId();
		auto state_template = stage_template_mgr.FindStage(fb_id);
		if ( !state_template )
		{
			//assert(false);
			continue;
		}
		if ( state_template->GetRecommandLvlLower() < 20 )
			continue;
		if (state_template->IsArenaStage())
			continue;
		if (state_template->FatigueOfUse())
			continue;
		auto lvl_dist = player_lvl - state_template->GetRecommandLvlLower();
		if ( lvl_dist >= 10 )
			continue;
		auto village_dst_name = fb_map.Find_Fb2Village(fb_id);
		if ( !village_dst_name )
			continue;
		const auto& it = std::find(blacklist_fb_ids, blacklist_fb_ids + _countof(blacklist_fb_ids), fb_id);
		if ( it != blacklist_fb_ids + _countof(blacklist_fb_ids) )
			continue;
		if (allowed_mgr.FindByName(*village_dst_name))
		{
			un_use_tmp.fb_id_ = fb_id;
			un_use_fatigue_fb.push_back(un_use_tmp);
		}	
	}

	if (un_use_fatigue_fb.empty())
		return;
	std::reverse(un_use_fatigue_fb.begin(), un_use_fatigue_fb.end());
	
	auto village_id_old = gps.GetCurVillageName();
	LOG_ENTER_FUNC;
	while ( !un_use_fatigue_fb.empty() )
	{
		while (gps.DoTheFb(un_use_fatigue_fb.front().fb_id_))
		{
			if ( !gps.Update() )
			{
				assert(false);
				return;
			}
		}
		un_use_fatigue_fb.pop_front();
	}
	gps.MoveToTheStage(village_id_old);
	LOG_O(Log_info) << "刷不消耗疲劳的副本之完成";
}

void DoBuyProductsOfTheRoles( const luabind::object& tbl_roles, const luabind::object& tbl_products )
{
	using namespace luabind;
	if ( LUA_TTABLE != type(tbl_roles) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买某些角色的某些物品之角色参数必须是表类型";
		assert(false);
		return;
	}
	if ( LUA_TTABLE != type(tbl_products) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "购买某些角色的某些物品之拍卖物品参数必须是表类型";
		assert(false);
		return;
	}
	std::vector<std::wstring> cont_roles;
	/*auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	auto& lua_c = nor->GetGLua();
	auto lua_state = lua_c.lua();
	if ( !lua_state )
	{
		assert(false);
		return;
	}*/
	LOG_ENTER_FUNC;
	iterator it_end;
	std::string str_tmp;
	for ( iterator it(tbl_roles); it != it_end; ++it )
	{
		if ( LUA_TSTRING != type(*it) )
		{
			LOG_O(Log_warning) << "购买某些角色的某些物品之角色参数的每一项必须是字符串类型";
			assert(false);
			continue;
		}
		str_tmp = ObjectCast(*it, kEmptyStr);
		if (str_tmp.empty())
		{
			assert(false);
			continue;
		}
		cont_roles.push_back(GbkToWideChar(str_tmp));
	}
	if (cont_roles.empty())
	{
		LOG_O(Log_warning) << "购买某些角色的某些物品之合法的角色信息为空";
		assert(false);
		return;
	}
	struct stFilterProductInfo{
		std::wstring		item_ch_name;
		DWORD				item_grade;
	};
	std::vector<stFilterProductInfo> cont_product_info;
	stFilterProductInfo product_tmp;
	for ( iterator it(tbl_products); it != it_end; ++it )
	{
		auto& product_item = *it;
		if ( LUA_TTABLE != type(product_item) )
		{
			LOG_O(Log_warning) << "购买某些角色的某些物品之拍卖物品参数的每一项必须是表类型";
			assert(false);
			continue;
		}
		auto& item_cn_name = product_item[1];
		if ( LUA_TSTRING != type(item_cn_name) )
		{
			LOG_O(Log_warning) << "购买某些角色的某些物品之拍卖物品参数的物品名的元素必须是字符串类型";
			assert(false);
			continue;
		}
		auto& item_grade = product_item[2];
		if ( LUA_TNUMBER != type(item_grade) )
		{
			LOG_O(Log_warning) << "购买某些角色的某些物品之拍卖物品参数的物品等级的元素必须是数字类型";
			assert(false);
			continue;
		}		
		str_tmp = ObjectCast(item_cn_name, kEmptyStr);
		if (str_tmp.empty())
		{
			assert(false);
			continue;
		}
		int item_grade_tmp = -1;
		item_grade_tmp = ObjectCast(item_grade, item_grade_tmp);

		product_tmp.item_ch_name = GbkToWideChar(str_tmp);
		product_tmp.item_grade = DWORD(item_grade_tmp);
		cont_product_info.push_back(product_tmp);
	}
}

void OpenConsumeItem( const std::string& main_item_name, const luabind::object& require_item_name )
{
	using namespace luabind;
	auto& gps = GPlayerSelf::GetMe();
	auto& package = gps.GetPacket();
	auto& tab_consume = package.GetTabConsume();
	tab_consume.RebuildUpdateAllSync();
	if ( LUA_TSTRING != type(require_item_name) )
		tab_consume.OpenItemA(main_item_name, kEmptyStr);
	else
		tab_consume.OpenItemA(main_item_name, ObjectCast(require_item_name, kEmptyStr));
}

void UseConsumeItem( const std::string& item_ch_name )
{
	using namespace luabind;
	if (item_ch_name.empty())
	{
		assert(false);
		return;
	}
	auto& gps = GPlayerSelf::GetMe();
	auto& package = gps.GetPacket();
	auto& tab_consume = package.GetTabConsume();
	tab_consume.RebuildUpdateAllSync();
	tab_consume.UseItem(GbkToWideChar(item_ch_name));
}

void DoTheRecommandFb()
{
	using namespace GClass;
	auto& gps = GPlayerSelf::GetMe();
	gps.Update();
	auto& allow_mgr = AllowedVillageCloneMapMgr::GetMe();
	allow_mgr.RebuildUpdateAll();
	auto iter = allow_mgr.TraverseRecommandLvlCloneMap(gps.GetLevel());
	if ( !iter )
	{
		assert(false);
		return;
	}
	LOG_ENTER_FUNC;
	for ( iter->First(); !iter->IsDone(); iter->Next() )
	{
		auto& cur_allow = iter->CurItem();
		auto& clone_map_id = cur_allow.GetMapId();
		if (clone_map_id.empty())
		{
			assert(false);
			continue;
		}
		LOG_O(Log_trace) << "刷建议副本->" << clone_map_id;
		if (gps.DoTheFb(clone_map_id))
		{
			return;
		}
	}
}

bool HandleCanNotRunTask()
{
	DoTheRecommandFb();
	return true;
}

bool FindSubWStr( const std::wstring& full_str, const std::wstring& sub_str )
{
	if (sub_str.empty())
	{
		assert(false);
		return false;
	}
	if (full_str.empty())
	{
		assert(false);
		return false;
	}
	auto cnt_pos = full_str.find(sub_str);
	return std::string::npos != cnt_pos;
}

bool DisassemblyTheItem( GItem& the_item )
{
	return GPlayerSelf::GetMe().DisassemblyTheItem(the_item);
}

void ResetNoRetCityAtDie()
{
	GPlayerSelf::GetMe().ResetNoRetCityAtDie();
}

void AddNoRetCityAtDie( const std::string& fb_ch_name )
{
	GPlayerSelf::GetMe().AddNoRetCityAtDie(GbkToWideChar(fb_ch_name));
}

bool DoTheFb( const std::string& fb_ch_name )
{
	return GPlayerSelf::GetMe().DoTheFbA(fb_ch_name);
}

void GLuaC::BindGlobalFn()
{
	__super::BindGlobalFn();
	using namespace luabind;

	module(lua())
		[
			def("SetScriptOutputInfo", &SetScriptOutputInfo),
			def("EchoMsg", &EchoMsg),
			def("ExchangeGameAc", &ExchangeGameAc),
			def("刷新我的拍卖列表", &RefreshMyProductList),
			def("重置任务黑名单", &ResetTaskBlacklist),
			def("添加任务黑名单", &AddTaskBlacklist_1_),
			def("添加任务黑名单", &AddTaskBlacklist_2_),
			def("添加职业任务黑名单", &AddTaskBlacklist_MatchMainJob_1_),
			def("添加职业任务黑名单", &AddTaskBlacklist_MatchMainJob_2_),
			def("邮寄金币给某个角色", &SendMailGold),
			def("角色等级", &GetRoleLevel),
			def("清空扫拍卖条目", &ClearBuyProductItems),
			def("添加扫拍卖条目", &AddBuyProductItem),
			def("进行扫拍卖", &DoBuyProductItems),
			def("刷不消耗疲劳的副本", &DoUnUseFatigueFb),
			def("购买某些角色的某些物品", &DoBuyProductsOfTheRoles),
			def("禁止接受指定的任务", &ProhibitAcceptTheTask),
			def("打开箱子", &OpenConsumeItem),
			def("使用消耗物品", &UseConsumeItem),
			def("处理无法继续任务", &HandleCanNotRunTask),
			def("刷建议副本", &DoTheRecommandFb),
			def("FindSubWStr", &FindSubWStr),
			def("DisassemblyTheItem", &DisassemblyTheItem),
			def("重置死亡不回城副本", ResetNoRetCityAtDie),
			def("添加死亡不回城副本", AddNoRetCityAtDie),
			def("执行打完指定的副本", DoTheFb)
		];
}
