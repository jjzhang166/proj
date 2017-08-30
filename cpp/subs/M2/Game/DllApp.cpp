#include "stdafx.h"
#include "DllApp.h"
#include <Common/UsefulClass.h>
#include "Facility/GPlayer.h"
#include "Scmd.h"
#include "Facility/GProtocolR.h"
#include "Facility/GTask.h"
#include "Facility/GMap.h"

CMN_MY_DLLAPP_DEFINE(App);

bool GWndExecSync(const FunctionBool& func)
{
	bool res = false;
	GetMyApp().GWndExecSyncT(res, func);
	return res;
}

bool App::OnInit_()
{
	//this is test
#ifdef _DEBUG
	//wxSetEnv(wxT("__MyZhpAcItemKey__"), "11ab");
#endif
	if (!__super::OnInit_())
		return false;
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
	SendGameLoginSucceed();
	GQuestResolver::GetMe();
	return true;
}

App::App()
{
	gwnd_ = nullptr;
	io_thread_stack_size_ = 1024 * 1024 * 10;
	def_profession_type_ = kPT_NvWu;
	def_area_name_ = "永久";
}

void App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<App, AtpAppGame>("App"),
			def("GetMyApp", &GetMyApp)
		];
}

bool App::AddHookRecvMsg()
{
	hook_.AddCmd<GRecvMsgHandler::HandlerPromptInfo>();
	hook_.AddCmd<GRecvMsgHandler::HandlerOnItem>();
	hook_.AddCmd<GRecvMsgHandler::HandlerNpcPanelInfo>();
	hook_.AddCmd<GRecvMsgHandler::HandlerUseSkillEffectAtPos>();
	hook_.AddCmd<GRecvMsgHandler::HandlerOnHpMp>();
	return true;
}

bool App::DoInit()
{
	if (!__super::DoInit())
	{
		assert(false);
		return false;
	}
	if (!AddHookRecvMsg())
	{
		assert(false);
		return false;
	}
	if (!hook_.DoHooks())
	{
		assert(false);
		return false;
	}
	hook_.SetOnInitMapSink(this);
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
	struct stExecOnce{
		stExecOnce(App* app){
			app->EnterGameExecOnce();
		}
	};
	GetIoService().post([this](){
		static stExecOnce exec_once(this);
		OnChangeMap();
	});
}

void App::OnChangeMap()
{
	GMap::GetMe().DoUpdated();
	GPlayerMe::GetMe().Update();
	GPassiveSkillResolver::GetMe().Init();
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

GameDllLuaThrdData* AppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new MyLuaThrdData(lstate);
}
