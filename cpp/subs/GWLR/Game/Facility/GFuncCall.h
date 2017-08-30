#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 游戏功能调用，调用游戏内的函数
*/
#include "GTypes.h"

//GFuncCall->GFC_
//任务章节
//const ZHANG_TASK_FB_INFO* FindChapterInfo( int fb_id );
//剧情已完成
bool IsTheFbIdJuQingDone( int fb_id );
//是否属于剧情任务
bool IsTheFbIdJuQingTask( int fb_id );
int GFC_GetShangJinCompleteCount();
int GFC_GetShangJinTotalCount();
// 判断赏金任务的状态  状态=1 表示已接  状态=10 表示已经完成
int GFC_GetShangJinTaskStatus(DWORD dwShangJinTaskId);
int GFC_GetMsgRoleCityRoomIndex();
void GFC_TidyInventoryItem(CD_En_TabCategory nTabSoltIndex);
LONGLONG GFC_GetSynCoorDinatesRandom();
//////////////////////////////////////////////////////////////////////////
void GFC_EnableWuDi( bool enable_wu_di );
//////////////////////////////////////////////////////////////////////////
//**********************UI************************************************
stCD_UiCtrl* GFC_Ui_TraUi(const char* cls_name);
bool GFC_Ui_ClickButton( const char* cls_name );
bool GFC_Ui_PutString( const char* cls_name, const wchar_t* the_str );
//////////////////////////////////////////////////////////////////////////
