#pragma once
/*
@author zhp
@date 2014/7/1 20:41
@purpose for map/clone map
*/
#include <Common/UsefulClass.h>
#include <Common/GameObjBase.h>

class GVillage : public GGameObjBase, public Singleton<GVillage, Singleton_MakeMe>{
public:
	GVillage();

	//interface
public:
	bool Update();

	//static
public:
};