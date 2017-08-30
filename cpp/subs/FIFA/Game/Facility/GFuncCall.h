#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 游戏功能调用，调用游戏内的函数
*/
#include "GTypes.h"
namespace GFuncCall{
	//////////////////////////////////////////////////////////////////////////
	//得到窗口句柄
	HWND GetHwnd();
	//关闭公告dlg
	void CloseNoticeDlg();
	//是否存在教练
	bool IsExistTrainer();
	//是否存在有效教练
	bool IsExistValidTrainer();
	
	//是否是教练选定场景
	bool IsTrainerSelectedScene();
	//malloc of game
	void* MallocOfGame( size_t size );
	//calloc of game
	void* CallocOfGame( size_t n, size_t size );
	//free of game
	void FreeOfGame( void* p );
	//SSL_read，得到ssl handle
	void* GetSllReadHandle();
	//////////////////////////////////////////////////////////////////////////
	//按键
	void PressKeyImpl( DWORD v_k, UINT msg );
	//鼠标左键
	void LeftBtClickImpl( int x, int y );
	//////////////////////////////////////////////////////////////////////////
}