#pragma once
/*
@author zhp
@date 2014/8/5 8:32
@purpose for¸±±¾
*/
#include <Common/UsefulClass.h>
#include "GRoom.h"

class GCloneMap : public GRoomBase, public Singleton<GCloneMap, Singleton_MakeMe>{
public:
	GCloneMap();

	//interface
public:
	bool Update();
private:
};