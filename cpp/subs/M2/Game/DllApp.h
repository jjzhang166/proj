#pragma once
/*
@author zhp
@date 2016/12/23 14:34
@purpose dll app
*/
#include <AppMix/AppAtpGame.h>
#include "Facility/GInterface.h"
#include <FuncObjs/FuncObjInter.h>
#include "GloFuncs.h"

class AppFactory : public AtpAppGameFactory{
	GameDllLuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
};

class App : public AtpAppGame, public CD_Interface::OnHookAfterInitMap{
	friend bool GWndExecSync(const FunctionBool& func);
public:
	App();
	~App();
	template<typename ResultT, typename FuncT>
	bool GWndExecSyncT(ResultT& res, const FuncT& func)
	{
		/*res = func();
		return true;*/
		auto& the_gwnd = gwnd_->GetInterGWnd();
		if (!gwnd_ || !::IsWindow(the_gwnd.GetWndHandle()))
		{
			//assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << "游戏发生错误，关掉游戏jfdka";
			GetIoService().post([this](){
				auto p = this->GetGameConnection();
				if (!p)
					return;
				Disconnect_Prompt("未知错误2");
			});
			//throw CmnCloseProcessException("jfdc,/");
			return false;
		}
		if (!the_gwnd.SyncExec(res, func))
		{
			bool is_executed = false;
			the_gwnd.SyncExec(is_executed, [&is_executed](){
				return true;
			});
			if (!is_executed)
			{
				//assert(false);
				VERIFY(the_gwnd.ResetHwnd(nullptr));
				if (the_gwnd.SyncExec(res, func))
					return true;
				assert(false);
				Disconnect_Prompt("未知错误3");
			}
			//assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "同步执行返回假";
			return false;
		}
		return true;
	}
	void Disconnect_Prompt(const std::string& prompt_str);

public:
	static enCD_ProfessionType ProfessionName2Type(const std::string& profession_name);
	void SetDefCreateProfessionType(const std::string& profession_name);
	void SetDefCreateGameArea(const std::string& game_area);
	bool AutoCreateSelectRole() override;
	bool SelectEnterToGame(const CmnGameRoleInfo& role_info) override;
	enCmnCreateRoleRes CreateEnterToGame(const std::string& role_name) override;

private:
	bool DoInit() override;
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	bool AddHookRecvMsg();
	bool DoGetGameHwnd();
	AppFactory* CreateAppFactory() override;
	void OnBeforeInitMap(stCD_GameLogic& new_game_logic, stCD_World& new_world) override;
	void OnAfterInitMap() override;

	//初始化游戏里的数据
private:
	//进入游戏只初始化一次
	void EnterGameExecOnce();
	//每次切换地图都会执行
	void OnChangeMap();

private:
	CD_Interface::Hooks						hook_;
	FuncObj_InterInjectComposite*			gwnd_;
	enCD_ProfessionType						def_profession_type_;
	std::string								def_area_name_;

public:

};
CMN_MY_APP_DECLARE(App);
template<typename ResultT, typename FuncT>
inline bool GWndExecSyncT(ResultT& res, const FuncT& func){
	return GetMyApp().GWndExecSyncT(res, func);
}
