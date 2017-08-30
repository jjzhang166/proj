#pragma once
/*
@author zhp
@date 2016/4/12 5:10
*/
#include <ProtocolX/P_DefinesX.h>
#include <CD_CommonFiles/Data/CD_StructsBase.h>


void LoadScriptFile( const char* file_name );
//////////////////////////////////////////////////////////////////////////
//µ¼³öº¯Êý
pd_export_c const stCD_Vector3* DllGetRolePosPtr();
pd_export_c int DllGetMapId();
pd_export_c int DllGetRoomId();
//////////////////////////////////////////////////////////////////////////
