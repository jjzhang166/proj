#pragma once
/*
@author zhp
@date 2014/7/1 20:41
@purpose for map/clone map
*/
#include <Common/UsefulClass.h>
#include <Common/GameObjBase.h>
#include "GTypes.h"

struct stCrossDoorPosInfo{
	GType::stGameVector3	door_dst_pos_;
	GType::stGameVector3	door_prev_pos_;
};

class VillageDoorPosInfo : public Singleton<VillageDoorPosInfo, Singleton_MakeMe>{
public:
	VillageDoorPosInfo();

public:
	stCrossDoorPosInfo	xi_mei_lun2mi_la_de_cun_;
	stCrossDoorPosInfo	mi_la_de_cun2xi_mei_lun_;
};

class GVillage : public GGameObjBase, public Singleton<GVillage, Singleton_MakeMe>{
public:
	GVillage();

	//interface
public:
	bool Update() override;

	//static
public:
};