#include "stdafx.h"
#include "HandleUiScene.h"
#include "Facility/GFuncCall.h"

UiSceneMgr::UiSceneMgr()
{
	using namespace GType;
	break_at_do_until_ = false;
	//////////////////////////////////////////////////////////////////////////
	AddGenUiState(gen_ui_state_select_role_, kUiStr_StartGame, kUS_StartGame);
	AddGenUiState(gen_ui_state_select_role_, kUiStr_IamOlPlayer, kUS_OldPlayer);
	AddGenUiState(gen_ui_state_select_role_, kUiStr_RandomSkin, kUS_RandomSkin);
	AddGenUiState(gen_ui_state_select_role_, kUiStr_CreateRoleOk, kUS_CreateRoleOk);
	AddGenUiState(gen_ui_state_select_role_, kRoleListButtons[_countof(kRoleListButtons) - 1], kUS_CreateRoleBt6);
	//////////////////////////////////////////////////////////////////////////
}

void UiSceneMgr::ReplaceUiStateHandler( GType::enUiState ui_state, const SceneHandlerT& handler, int do_cnt )
{
	stSceneHandlerInfo tmp;
	tmp.handler_ = handler;
	tmp.do_cnt_ = do_cnt;
	assert(tmp.do_cnt_ > 0);
	ui_state_handlers_[ui_state] = tmp;
	break_at_do_until_ = false;
}

bool UiSceneMgr::DoHandleUiState_SelectRole() const
{
	using namespace GType;
	auto ui_state = CalcCurUiState_SelectRole();
	if ( kUS_Invalid == ui_state )
		return false;
	return DoExecOneUiStateHandler(ui_state);
}

void UiSceneMgr::AddGenUiState( GenUiStateMapT& gen_ui_state_map, const char* cls_name, GType::enUiState ui_state )
{
	auto it = std::find_if(gen_ui_state_map.begin(), gen_ui_state_map.end(), [cls_name]( const stCalcUiState& v ){
		if ( cls_name == v.cls_name_ )
			return true;
		return 0 == strcmp(cls_name, v.cls_name_);
	});
	if ( it != gen_ui_state_map.end() )
	{
		assert(false);
		return;
	}
	stCalcUiState tmp = {
		cls_name, ui_state
	};
	gen_ui_state_map.push_back(tmp);
}

GType::enUiState UiSceneMgr::CalcCurUiState_SelectRole() const
{
	return CalcCurUiState(ui_state_handlers_, gen_ui_state_select_role_);
}

GType::enUiState UiSceneMgr::CalcCurUiState( const UiSceneMapT& ui_scene_map, const GenUiStateMapT& gen_ui_state_map )
{
	for ( auto& v : gen_ui_state_map )
	{
		if (ExistTheUiStateHandler(ui_scene_map, v.ui_state_))
		{
			if ( !IsUnValidPtr(GFC_Ui_TraUi(v.cls_name_)) )
				return v.ui_state_;
		}
	}
	return GType::kUS_Invalid;
}

bool UiSceneMgr::DoExecOneUiStateHandler( GType::enUiState ui_state ) const
{
	auto it = ui_state_handlers_.find(ui_state);
	if ( it == ui_state_handlers_.end() )
	{
		//assert(false);
		return false;
	}
	if ( it->second.do_cnt_ <= 0 )
		return false;
	--it->second.do_cnt_;

	auto the_func = it->second.handler_;
	return the_func();
}

void UiSceneMgr::EraseUiStateHandler( GType::enUiState ui_state )
{
	auto it = ui_state_handlers_.find(ui_state);
	if ( it != ui_state_handlers_.end() )
		ui_state_handlers_.erase(it);
	break_at_do_until_ = false;
}

void UiSceneMgr::ClearUiStateHandlers()
{
	ui_state_handlers_.clear();
	break_at_do_until_ = false;
}

bool UiSceneMgr::DoWaitHandleUntil( DoHandleUiStateFuncT do_func, bool the_res, int wait_time )
{
	if ( !do_func )
	{
		assert(false);
		return false;
	}
	return TimerDoWaitUntil([=](){
		if (break_at_do_until_)
			return true;
		if ( the_res == (this->*do_func)() )
			return true;
		return false;
	}, kFunc_DoNothing, 500, wait_time);
}

bool UiSceneMgr::ExistTheUiStateHandler( const UiSceneMapT& ui_scene_map, GType::enUiState ui_state )
{
	auto it = ui_scene_map.find(ui_state);
	if ( it == ui_scene_map.end() )
		return false;
	return true;
}

void UiSceneMgr::SetBreakAtDoUntil( bool is_break )
{
	break_at_do_until_ = is_break;
}
