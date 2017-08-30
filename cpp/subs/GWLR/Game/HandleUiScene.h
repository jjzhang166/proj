#pragma once
/*
@author zhp
@date 2016/2/14 21:46
@purpose for handle ui scene
*/
#include "Facility/GTypes.h"
#include <functional>
#include <map>

class UiSceneMgr : public Singleton<UiSceneMgr, Singleton_MakeMe>{
public:
	typedef std::function<bool()> SceneHandlerT;
	struct stSceneHandlerInfo{
		SceneHandlerT		handler_;
		mutable int			do_cnt_;
	};
	typedef std::map<GType::enUiState, stSceneHandlerInfo> UiSceneMapT;
	struct stCalcUiState{
		const char*			cls_name_;
		GType::enUiState	ui_state_;
	};
	typedef std::vector<stCalcUiState> GenUiStateMapT;
	typedef bool (UiSceneMgr::*DoHandleUiStateFuncT)() const;

public:
	UiSceneMgr();

	//注册相关
public:
	void ReplaceUiStateHandler( GType::enUiState ui_state, const SceneHandlerT& handler, int do_cnt = 1 );
	void EraseUiStateHandler( GType::enUiState ui_state );
	void ClearUiStateHandlers();

	//判断环境并处理
public:
	bool DoWaitHandleUntil( DoHandleUiStateFuncT do_func, bool the_res, int wait_time );
	GType::enUiState CalcCurUiState_SelectRole() const;
	bool DoHandleUiState_SelectRole() const;
	bool DoHandleUiState_InVillage() const;
	bool DoHandleUiState_InFb() const;

	//another
public:
	void SetBreakAtDoUntil( bool is_break );

private:
	bool DoExecOneUiStateHandler( GType::enUiState ui_state ) const;
	static bool ExistTheUiStateHandler( const UiSceneMapT& ui_scene_map, GType::enUiState ui_state );
	static void AddGenUiState( GenUiStateMapT& gen_ui_state_map, const char* cls_name, GType::enUiState ui_state );
	static GType::enUiState CalcCurUiState( const UiSceneMapT& ui_scene_map, const GenUiStateMapT& gen_ui_state_map );

private:
	UiSceneMapT			ui_state_handlers_;
	GenUiStateMapT		gen_ui_state_select_role_;
	bool				break_at_do_until_;
};