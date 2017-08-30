#pragma once
/*
@author zhp
@date 2017/1/24 17:22

CDF->common data function
*/
#include <ProtocolX/P_BaseTypeX.h>
#include "CD_Const.h"
#include <WgFacility/CmnGameStruct.h>
#include <CmnMix/Cmn_TaskQueue.h>
//////////////////////////////////////////////////////////////////////////
template<typename CharT>
const CharT* CD_SafeStr(const CharT* str){
	return StdEx::TEmptyStrAsMax(str, CD_kMaxStrLen);
}
template<typename CharT>
const CharT* CD_SafeName(const CharT* str){
	return StdEx::TEmptyStrAsMax(str, CD_kMaxNameLen);
}
template<typename CharT>
const CharT* CD_SafePath(const CharT* str){
	return StdEx::TEmptyStrAsMax(str, CD_kMaxFilePathLen);
}
template<typename CharT>
const CharT* CD_SafeStr(const GameStringT<CharT>& str){
	return StdEx::TEmptyStrAsMax(str.c_str(), CD_kMaxStrLen);
}
template<typename CharT>
const CharT* CD_SafeName(const GameStringT<CharT>& str){
	return StdEx::TEmptyStrAsMax(str.c_str(), CD_kMaxNameLen);
}
template<typename CharT>
const CharT* CD_SafePath(const GameStringT<CharT>& str){
	return StdEx::TEmptyStrAsMax(str.c_str(), CD_kMaxFilePathLen);
}
pt_dword CD_GetExeModule();
pt_dword CD_GetPathOfExileBase();
typedef CmnTaskStaticQueue_Atomic<100, 100> StaticTaskQueueT;
StaticTaskQueueT& GetStaticTaskQueue();
