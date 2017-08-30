#pragma once
//for su hui

class IStagePosInfo{
public:
	virtual ~IStagePosInfo(){}
	//新建一个pos info对象，返回一个pos info的标识
	virtual DWORD OnNewPosInfoObj() = 0;
	//新建一个房间对象，返回一个room info的标识
	virtual DWORD OnNewRoomInfoObj() = 0;
	//第一个参数是pos info的标识
	virtual void OnAddVertex( DWORD pos_info_obj_id, int vertex_id, float x, float y, float z ) = 0;
	//第一个参数是pos info的标识
	virtual void OnAddEdge( DWORD pos_info_obj_id, int vertex_id_from, int vertex_id_to ) = 0;
	//第一个参数是房间对象标识
	virtual void OnAddPosInfo( DWORD room_info_obj_id, int room_id, DWORD pos_info_obj_id ) = 0;
	//第二个参数是房间对象标识
	virtual void OnAddStageInfo( int stage_id, DWORD room_info_obj_id ) = 0;
};

//获取这个函数地址：SetStagesPosInfoInterface,下面是函数的声明，初始化用，只能调用一次
typedef void (*SetStagesPosInfoInterface_FuncT)( IStagePosInfo* stage_ptr );
//获取坐标信息，可多次调用,函数名：DoGainTheStagesPosInfo
typedef void (*DoGainTheStagesPosInfo_FuncT)( const char* lua_full_file_name );