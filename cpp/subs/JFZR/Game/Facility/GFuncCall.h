#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 游戏功能调用，调用游戏内的函数
*/
#include "GTypes.h"
#include <WinDef.h>
#include <string>

namespace GFuncCall{
	extern const WORD kGameHashArray;
	//////////////////////////////////////////////////////////////////////////
	//得到窗口句柄
	HWND GetHwnd();
	//计算哈希值
	WORD CalcHash_WORD( const wchar_t* wstr );
	WORD CalcHash_WORD( const std::wstring& wstr );
	DWORD CalcHash_DWORD( const wchar_t* wstr_lhs, const wchar_t* wstr_rhs );
	DWORD CalcHash_DWORD( const std::wstring& wstr_lhs, const std::wstring& wstr_rhs );
	DWORD CalcHash_DWORD( const wchar_t* wstr );
	DWORD CalcHash_DWORD( const std::wstring& wstr );
	//////////////////////////////////////////////////////////////////////////
	//副本未知对象
	//void* GetFbUnknownObjByName( const std::wstring& room_id );
	//////////////////////////////////////////////////////////////////////////
	float GetPreActionPlayTime();
}