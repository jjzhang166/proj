// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#include <config/targetver.h>

#include <afxwin.h>

template<typename ResultT, typename FuncT>
bool GWndExecSyncT(ResultT& res, const FuncT& func){
	res = func();
	return true;
}