#include "stdafx.h"
#include "DllApp.h"
#include <Common/UsefulClass.h>
#include "Facility/GPlayer.h"
#include <Scmd.h>
#include "Facility/GProtocolR.h"
#include "Facility/GProtocolS.h"
#include "Facility/GTask.h"
#include "Facility/GMap.h"
#include <boost/thread.hpp>
#include <AppMix/AppExecEnv.h>
#include <CommonX/CmnX_HookerMore.h>

//#define _NOT_HOOK_

CMN_MY_DLLAPP_DEFINE(App);

BC_REG_CLS_DEFINE(MyConnectionOper){
	bufcall::class_<MyConnectionOper, GameConnectionOper>("MyConnectionOper")
	.def("GetMapData", &MyConnectionOper::GetMapData)
	.def("GetPlayerPos", &GetPlayerPos);
}

void MyConnectionOper::GetMapData()
{
	if (!GInterface::TopStateSameName(CD_StateNameInGameState))
		return;
	auto c = GetMyApp().GetConnection();
	if (!c)
	{
		assert(false);
		return;
	}
	c->SendMapData();
}

void MyConnectionOper::GetPlayerPos()
{
	if (!GInterface::TopStateSameName(CD_StateNameInGameState))
		return;
	auto c = GetMyApp().GetConnection();
	if (!c)
	{
		assert(false);
		return;
	}
	c->SendPlayerPos();
}

bool GWndExecSync(const FunctionBool& func)
{
	bool res = false;
	GetMyApp().GWndExecSyncT(res, func);
	return res;
}

bool App::OnInit_()
{
	CmnX_AntiHookCheck::GetRealMe().AddThreadWhiteList(::GetCurrentThreadId());
	if (!__super::OnInit_())
		return false;
#ifndef _NOT_HOOK_
	LOG_O(Log_trace) << "begin DoGetGameHwnd";
	if (!DoGetGameHwnd())
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "开始等待游戏状态：PreGameState";
	if (!GInterface::WaitPreGameState(1000 * 60 * 10))
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "到了游戏状态：PreGameState";
#endif
	hook_.DoTraCheckThread();
	SendGameLoginSucceed();
	GQuestResolver::GetMe();
	boost::thread t_static_queue(ThreadRunStaticQueue);
	return true;
}

App::App()
{
	gwnd_ = nullptr;
	io_thread_stack_size_ = 1024 * 1024 * 10;
	init_thread_stack_size_ = 1024 * 1024 * 3;
	def_profession_type_ = kPT_NvWu;
#ifdef VER_GJ_
	def_area_name_ = "Legacy";
#else
	//def_area_name_ = "永久";
	def_area_name_ = "先驱者";
#endif
	RandomCombineStr::GetDefMe().SetTotalLen_MinMax(4, kRoleNameMaxCnt);
	CD_Interface::Hooks::SetInterfaceSink(this);
}

void App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<App, MyAppGameBase>("App"),
			def("GetMyApp", &GetMyApp)
		];
}

bool App::AddHookRecvMsg()
{
	GRecvMsgHandler::GetMe().AddCmds(hook_);
	GSendMsgHandler::GetMe().AddCmds(hook_);
	return true;
}

bool App::DoInit()
{
#ifdef _NOT_HOOK_
	auto the_key = AppExecEnvBase::GetEnvAcItemKey();
	if (the_key.empty())
		wxSetEnv(wxT("__MyZhpAcItemKey__"), "11Auto-select Gateway");
#endif
	if (!__super::DoInit())
	{
		assert(false);
		return false;
	}
	assert(wxGetInstance());
	CmnX_AntiHookCheck::SetRealMudule(wxGetInstance());
	HookSomeThing::GetMe().SetMachineCodeBased(ac_item_key_);
	auto& anti_crc = CmnX_AntiHookCheck::GetRealMe();
	auto load_thread_id = ::GetCurrentThreadId();
	anti_crc.AddThreadWhiteList(load_thread_id);
	if (!AddHookRecvMsg())
	{
		assert(false);
		return false;
	}
#ifndef _NOT_HOOK_
	if (!hook_.DoHooks())
	{
		assert(false);
		return false;
	}
#endif
	hook_.SetOnInitMapSink(this);
	anti_crc.EraseThreadWhiteList(load_thread_id);
	return true;
}

bool App::DoGetGameHwnd()
{
	HWND wnd_handle = nullptr;
	if (!TimerDo(1000, 1000 * 60 * 5, [&wnd_handle](){
		wnd_handle = CD_Interface::GetGameWndHandle();
		return wnd_handle != nullptr;
	}))
	{
		assert(false);
		return false;
	}
	assert(wnd_handle);
	gwnd_ = new FuncObj_InterInjectComposite(nullptr, wnd_handle);
	return true;
}

App::~App()
{
	delete gwnd_;
}

void App::Disconnect_Prompt(const std::string& prompt_str)
{
	auto p = GetGameConnection();
	if (!p)
		throw CmnTerminateProcessException("iueqpd;ac.");
	if (!prompt_str.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << prompt_str;
		std::string msg_prompt = prompt_str;
		msg_prompt += '\n';
		msg_prompt += "是否要关闭游戏";
		if (IDCANCEL == G_MessageBoxTimeoutA(msg_prompt.c_str(), nullptr, 1000 * 8))
			return;
	}

	/*LOG_O(Log_trace) << "想要关闭连接：" << prompt_str;
	return;*/

	p->Disconnect();
	/*SetIoTimer1(1000 * 60, []( IoTimer& ){
	throw CmnCloseProcessException("rehuioqsdfc");
	});*/
}

AppFactory* App::CreateAppFactory()
{
	return new AppFactory;
}

void App::OnBeforeInitMap(stCD_GameLogic& new_game_logic, stCD_World& new_world)
{
	LOG_O(Log_trace) << "hooker,OnBeforeInitMap";
	Pause();
	if (!GMap::GetMe().InitNewAstar(new_world))
	{
		assert(false);
		return;
	}
}

void App::OnAfterInitMap()
{
	LOG_O(Log_trace) << "hooker,OnAfterInitMap";
	if (!GMap::GetMe().UpdateImpl())
	{
		assert(false);
		return;
	}
	
	ActiveInitThread();
}

void App::OnChangeMap()
{
	hook_.DoTraCheckThread();
	GPlayerMe::GetMe().Update();
	GPassiveSkillResolver::GetMe().Init();
	//GA_OpenObjs::ClearGlobalBlackNameList();
}

void App::EnterGameExecOnce()
{	
	GDbPassiveSkillMgr::GetMe().RebuildAll();
	GSP_Resolver::GetMe().RebuildAll();
	if (!GWndExecSync([](){
		auto db_skill_desc = CD_Interface::GetDbMgr().GetDbSkillStatDescriptions();
		if (!db_skill_desc)
		{
			assert(false);
			return false;
		}
		if (!db_skill_desc->ResolveMyHashMap())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return;
	}
}

void App::SetDefCreateProfessionType(const std::string& profession_name)
{
	auto type = ProfessionName2Type(profession_name);
	if (type >= kPT_Mask)
	{
		assert(false);
		return;
	}
	def_profession_type_ = type;
}

void App::SetDefCreateGameArea(const std::string& game_area)
{
	if (game_area.empty())
	{
		assert(false);
		return;
	}
	def_area_name_ = game_area;
}

bool App::SelectEnterToGame(const CmnGameRoleInfo& role_info)
{
	return GInterface::EnterGame(role_info.role_key_);
}

enCmnCreateRoleRes App::CreateEnterToGame(const std::string& role_name)
{
	return GInterface::CreateRole(def_area_name_, def_profession_type_, role_name);
}

bool App::AutoCreateSelectRole()
{
	GameRolesKeyCont role_names;
	if (!GWndExecSync([&role_names](){
		auto select_top = CD_Interface::GetSelectRoleTop();
		if (!select_top)
		{
			//assert(false);
			return false;
		}
		select_top->TraRoleItems([&role_names](stCD_stCD_UiRoleListItem* role){
			role_names.push_back(WideCharToGbk(role->role_name_));
			return true;
		});
		return true;
	}))
	{
		//assert(false);
		if (!GInterface::IsCreateStateWithEmptyRole())
			return false;
	}
	
	AddGameRolesInfo(role_names);
	return __super::AutoCreateSelectRole();
}

enCD_ProfessionType App::ProfessionName2Type(const std::string& profession_name)
{
	struct stHelp{
		stHelp(){
			professions_.insert(std::make_pair("贵族", kPT_GuiZu));
			professions_.insert(std::make_pair("野蛮人", kPT_YeManRen));
			professions_.insert(std::make_pair("游侠", kPT_YouXia));
			professions_.insert(std::make_pair("女巫", kPT_NvWu));
			professions_.insert(std::make_pair("决斗者", kPT_JueDouZhe));
			professions_.insert(std::make_pair("圣堂武僧", kPT_ShengTangWuSeng));
			professions_.insert(std::make_pair("暗影", kPT_AnYing));
		}

		std::map<std::string, enCD_ProfessionType>	professions_;
	};
	static stHelp hlp;
	auto it = hlp.professions_.find(profession_name);
	if (it == hlp.professions_.end())
	{
		assert(false);
		return kPT_Mask;
	}
	return it->second;
}

void App::OnInitThreadRun()
{
	auto& gmap = GMap::GetMe();
	gmap.DoUpdated();
	struct stExecOnce{
		stExecOnce(App* app){
			app->EnterGameExecOnce();
		}
	};
	static stExecOnce exec_once(this);
	OnChangeMap();
	//////////////////////////////////////////////////////////////////////////
	//必须在外挂线程里做，如果在游戏线程里做，会崩溃的，因为堆栈空间要求比较大。
	auto map_data = gmap.GetMapData();
	if (IsUnValidPtr(map_data))
	{
		assert(false);
		return;
	}
	GTerrainParser::GetDefTerrainParser().Parse(gmap.GetMapName(), *map_data);
	//////////////////////////////////////////////////////////////////////////
	Resume();
}

#ifdef VER_GJ_
std::string App::RandomGameRoleName(pt_dword min_cnt, pt_dword max_cnt) const
{
	return Random_LetterStr(min_cnt, max_cnt);
}
#endif

MyGameConnection* App::GetConnection()
{
	auto c = __super::GetGameConnection();
	if (c)
		return c->CastTo<MyGameConnection>();
	return nullptr;
}

void App::OnModifyHp(pt_dword new_hp)
{
	//LOG_O(Log_debug) << "OnModifyHp,new_hp:" << new_hp;
	if (0 == new_hp)
	{
		LOG_O(Log_warning) << "角色死亡了";
		if (!LuasScriptEchoC::IsRunning())
			return;
		GetIoService().post([](){
			if (!LuasScriptEchoC::IsRunning())
				return;
			LuasScriptEchoC::TriggerEvent(kEventName_ReviveFront);
			GInterface::Revive(kRAP_AtRecord);
		});		
	}
	else
	{
		GetIoService().post([](){
			MyLuaThrdData::AutoUseFlack(stCDR_OnHpMp::kVT_Hp);
		});
	}
}

void App::OnModifyMp(pt_dword new_mp)
{
	//LOG_O(Log_debug) << "OnModifyMp,new_mp:" << new_mp;
	GetIoService().post([](){
		MyLuaThrdData::AutoUseFlack(stCDR_OnHpMp::kVT_Mp);
	});
}

void App::OnPromptRemaining(int remaining)
{
	LOG_O(Log_trace) << "剩余怪物数量：" << remaining;
	remaining_ = remaining;
}

HMODULE App::GetSelfModule()
{
	return wxGetInstance();
}

bool App::IsAddrInSelfModule(pt_pcvoid addr)
{
	if (!addr)
	{
		addr = GetThreadStartAddr(GetCurrentThreadId());
		assert(addr);
	}
	auto module = wxGetInstance();
	if (module && ModuleFromAddress(addr) == module)
		return true;
	return false;
}

pt_dword App::GetGWndThrdId() const
{
	if (gwnd_)
		return gwnd_->GetInterGWnd().GetWndThrdId();
	assert(false);
	return 0;
}

void App::ThreadRunStaticQueue()
{
	CmnX_AntiHookCheck::GetRealMe().AddThreadWhiteList(::GetCurrentThreadId());
	GetStaticTaskQueue().RunTasks();
}

GameDllLuaThrdData* AppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new MyLuaThrdData(lstate);
}

ScriptRunRAII* AppFactory::CreateScriptRunRAII()
{
	return new MyScriptRAII;
}

MyConnectionOper* AppFactory::CreateGameConnectionOper(GameConnectionC& connection)
{
	return new MyConnectionOper(connection);
}

MyGameConnection* AppFactory::CreateGameConnection(pt_word dest_port)
{
	return new MyGameConnection(GetMyApp(), dest_port);
}

bool MyScriptRAII::PreRun()
{
	assert(!stCD_GameLogic::enable_ignore_operate_command);
	stCD_GameLogic::enable_ignore_operate_command = true;
	return __super::PreRun();
}

void MyScriptRAII::AfterRun()
{
	assert(stCD_GameLogic::enable_ignore_operate_command);
	stCD_GameLogic::enable_ignore_operate_command = false;
	return __super::AfterRun();
}

void MyGameConnection::SendMapData()
{
	auto& map_name = GMap::GetMe().GetMapName();
	if (map_name.empty())
		return;
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	CG_MapBitset bset;
	auto& astar = G_AStar::GetMe();
	auto map_size = astar.ToBitset(bset);
	oper->ToCall(BC_MakeP("OnSetMapData") << bset << map_size << map_name);
}

void MyGameConnection::SendPlayerPos()
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
		return;
	auto oper = GetConnectionOper();
	if (!oper)
	{
		assert(false);
		return;
	}
	stCD_VecInt pos;
	G_AStar::GetMe().CastToMyPos(*player_pos, pos);
	oper->ToCall(BC_MakeP("OnSetPlayerPos") << pos);
}
