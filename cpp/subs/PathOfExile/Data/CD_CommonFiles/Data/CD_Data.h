#pragma once
/*
@author zhp
@date 2017/5/28 16:44
@purpose for data
*/

namespace CD_Data{
#ifdef VER_GJ3_
#include "CD_Updates_gj3.h"
#elif VER_GJ_
#include "CD_Updates_gj.h"
#else
#include "CD_Updates.h"
#endif
}
pt_dword CD_GetExeModule();
pt_dword CD_GetPathOfExileBase();

#define CD_DATA(value) CD_Data::value
#define CD_EXE_DATA(value) (CD_GetExeModule() + CD_Data::value)
#define CD_Base_DATA(value) (CD_GetPathOfExileBase() + CD_Data::value)
