#pragma once
//for su hui
#include "IStagePosInfo.h"

class StagePosInfo : public IStagePosInfo{
public:
	//新建一个pos info对象，返回一个pos info的标识
	virtual DWORD OnNewPosInfoObj();
	//新建一个房间对象，返回一个room info的标识
	virtual DWORD OnNewRoomInfoObj();
	//第一个参数是pos info的标识
	virtual void OnAddVertex( DWORD pos_info_obj_id, int vertex_id, float x, float y, float z );
	//第一个参数是pos info的标识
	virtual void OnAddEdge( DWORD pos_info_obj_id, int vertex_id_from, int vertex_id_to );
	//第一个参数是房间对象标识
	virtual void OnAddPosInfo( DWORD room_info_obj_id, int room_id, DWORD pos_info_obj_id );
	//第二个参数是房间对象标识
	virtual void OnAddStageInfo( int stage_id, DWORD room_info_obj_id );

	static StagePosInfo& GetMe();
};
