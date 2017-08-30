#include "stdafx.h"
#include "GCloneMap.h"

bool GCloneMap::Update()
{
	using namespace GAddr;
	room_obj_base_ = (int)g_pLocalRoomObjAddr;
	return __super::Update();
}

GCloneMap::GCloneMap()
{

}
