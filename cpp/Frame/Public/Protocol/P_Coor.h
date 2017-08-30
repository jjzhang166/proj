#pragma once
/*
@author zhp
@date 2014/9/25 23:50
@purpose protocol of coordinate
*/
#include <Common/CmnConts.h>
#include "P_Defines.h"


#pragma pack(push)
#pragma pack(1)

typedef CmnStaticQueue<P_WayToNextCoor, 3> P_CoorPacketToT;
typedef CmnStaticStack<P_WayToNextCoor, 3> P_CoorPacketFromT;

struct stCoorPacketInfo{
	P_CoorPacketToT		way_to_;
	P_CoorPacketFromT	way_from_;
	P_FuncObjOper		oper_src_;
};

#pragma pack(pop)
