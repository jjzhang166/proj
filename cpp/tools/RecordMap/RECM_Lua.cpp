#include "stdafx.h"
#include "RECM_Lua.h"
#include "RecMap.h"


void RECM_LuaVM::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	module(lstate_)
		[
			class_<RecordMap::GSP__PosInfo>("GSP__PosInfo__")
			.def("AddVertex", &RecordMap::GSP__PosInfo::AddVertex)
			.def("AddEdge", &RecordMap::GSP__PosInfo::AddEdge),
			def("GSP__PosInfo", &RecordMap::GSP__PosInfo::GetMe),

			class_<RecordMap::GSP__RoomInfo>("GSP__RoomInfo__")
			.def("AddPosInfo", &RecordMap::GSP__RoomInfo::AddPosInfo),
			def("GSP__RoomInfo", &RecordMap::GSP__RoomInfo::GetMe),

			class_<RecordMap::GSP_StagePosPath>("GSP_StagePosPath")
			.def("AddStageInfo", &RecordMap::GSP_StagePosPath::AddStageInfo),
			def("GSP_StagePosPath_Me", &RecordMap::GSP_StagePosPath::GetMe),

			class_<RecordMap::GSP__PosInfo2d>("GSP__PosInfo2d_")
			.def("AddVertex", &RecordMap::GSP__PosInfo2d::AddVertex)
			.def("AddEdge", &RecordMap::GSP__PosInfo2d::AddEdge),
			def("GSP__PosInfo2d", &RecordMap::GSP__PosInfo2d::GetMe),

			class_<RecordMap::GSP_StagePosPath2d>("GSP_StagePosPath2d")
			.def("AddStageInfo", &RecordMap::GSP_StagePosPath2d::AddStageInfo),
			def("GSP_StagePosPath2d_Me", &RecordMap::GSP_StagePosPath2d::GetMe)
		];
}
