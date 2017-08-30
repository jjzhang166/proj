#include "stdafx.h"
#include "GVillage.h"

GVillage::GVillage()
{
}

bool GVillage::Update()
{
	return true;
}

VillageDoorPosInfo::VillageDoorPosInfo()
{
	xi_mei_lun2mi_la_de_cun_.door_dst_pos_.SetXYZ(365.962f, 370.224f, 89.375f);
	xi_mei_lun2mi_la_de_cun_.door_prev_pos_.SetXYZ(369.638f, 367.689f, 89.3546f);

	mi_la_de_cun2xi_mei_lun_.door_dst_pos_.SetXYZ(694.272f, 674.013f, 134.875f);
	mi_la_de_cun2xi_mei_lun_.door_prev_pos_.SetXYZ(690.604f, 674.597f, 134.875f);
}
