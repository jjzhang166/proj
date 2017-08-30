#include "stdafx.h"
#include "StagePosInfo.h"
#include "../commad//CoorDinatesPos.h"



DWORD StagePosInfo::OnNewPosInfoObj()
{
	return 0;
}

DWORD StagePosInfo::OnNewRoomInfoObj()
{
	return 0;
}

void StagePosInfo::OnAddVertex( DWORD pos_info_obj_id, int vertex_id, float x, float y, float z )
{
	COORDINATES_POS_DATA theCoorDinatesPosData;
	theCoorDinatesPosData.dwMapId = 0x3EA;		// 读取游戏中的数据
	theCoorDinatesPosData.dwRoomId = 1;			// 读取游戏中的数据
	theCoorDinatesPosData.fx = x;
	theCoorDinatesPosData.fy = y;
	theCoorDinatesPosData.fz = z;

	theCmdCoorDinatesPos.ReadLuaInit(&theCoorDinatesPosData,vertex_id);
}

void StagePosInfo::OnAddEdge( DWORD pos_info_obj_id, int vertex_id_from, int vertex_id_to )
{
	theCmdCoorDinatesPos.AddConnectPos(vertex_id_from,vertex_id_to);
}

void StagePosInfo::OnAddPosInfo( DWORD room_info_obj_id, int room_id, DWORD pos_info_obj_id )
{

}

void StagePosInfo::OnAddStageInfo( int stage_id, DWORD room_info_obj_id )
{

}


StagePosInfo& StagePosInfo::GetMe()
{
	static StagePosInfo me;
	return me;
}
