#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"
#include "Facility/GPlayer.h"
#include "Facility/GClass.h"
#include "Facility/GMapPath.h"

void GLuaC::BindStruct()
{
	__super::BindStruct();
	using namespace luabind;
	using namespace GType;
	module(lua())
		[
			class_<stGameVector3>("stGameVector3")
			.def(constructor<>())
			.def(constructor<float, float, float>())
			.property("fx", &stGameVector3::GetPosX, &stGameVector3::SetPosX)
			.property("fy", &stGameVector3::GetPosY, &stGameVector3::SetPosY)
			.property("fz", &stGameVector3::GetPosZ, &stGameVector3::SetPosZ),

			class_<stDirection2d>("stDirection2d")
			.def(constructor<>())
			.def(constructor<float, float>())
			.property("dir_x", &stDirection2d::GetDirX, &stDirection2d::SetDirX)
			.property("dir_y", &stDirection2d::GetDirY, &stDirection2d::SetDirY),

			class_<GSP_StagePosPath::PosInfo>("GSP__PosInfo")
			.def(constructor<>())
			.def("AddVertex", &GSP_StagePosPath::PosInfo::AddVertex)
			.def("AddEdge", &GSP_StagePosPath::PosInfo::AddEdge),

			class_<GSP_StagePosPath::RoomInfo>("GSP__RoomInfo")
			.def(constructor<>())
			.def("AddPosInfo", &GSP_StagePosPath::RoomInfo::AddPosInfo),

			class_<GSP_StagePosPath>("GSP_StagePosPath")
			.def("AddStageInfo", &GSP_StagePosPath::AddStageInfo),
			def("GSP_StagePosPath_Me", &GSP_StagePosPath::GetMe),

			class_<GameObjMgr_AnyObj::PacketIdT>("GameObjMgr_AnyObj__PacketIdT")
		];
}