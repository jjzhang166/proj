#pragma once
/*
@author zhp
@date 2015/1/15 19:20
@purpose for anti crack
*/
#include <Common/CmnPriLuaData.h>

#define _ANTI_CRACK_LEVEL_0_

#ifdef _ANTI_CRACK_LEVEL_0_

void CheckLocalMachinePort();
//////////////////////////////////////////////////////////////////////////
//lua private data,anti crack
CmnPriLuaData& GetLPD_AC();
//check login失败次数
const std::string kLPD_CheckLoginFailedCnt = "36770294BAD5";
//得到验证失败数据
int GetLPD_CheckLoginFailedCnt();
void SetLPD_CheckLoginFailedCnt( int cnt );
//////////////////////////////////////////////////////////////////////////
#endif