#include "stdafx.h"
#include "CD_ProtocolS.h"
#include <cassert>

stCDS_CreateRole::stCDS_CreateRole()
{
	CD_ZERO_MSGS();
}

stCDS_UseSkill::stCDS_UseSkill()
{
	CD_ZERO_MSGS();
	ukn_flag_ = kTTSF_Normal;
}

stCDS_TakeupItem::stCDS_TakeupItem()
{
	CD_ZERO_MSGS();
	ukn_flag_ = kSIOF_Normal;
}

stCDS_PutdownItem::stCDS_PutdownItem()
{
	CD_ZERO_MSGS();
}

stCDS_UseFlask::stCDS_UseFlask()
{
	CD_ZERO_MSGS();
}

stCDS_PutItemToShop::stCDS_PutItemToShop()
{
	CD_ZERO_MSGS();
}

stCDS_OpenObject::stCDS_OpenObject()
{
	CD_ZERO_MSGS();
	skill_id_ = kSI_OpenObj;
	ukn_byte_ = kTTSF_Normal;
}

stCDS_Waypoint::stCDS_Waypoint()
{
	CD_ZERO_MSGS();
}

stCDS_NpcInventoryItemPutToSelfInventory::stCDS_NpcInventoryItemPutToSelfInventory()
{
	CD_ZERO_MSGS();
	tbl_idx_ = 0;
	ukn_byte1_ = 0;
}

stCDS_UseItem::stCDS_UseItem()
{
	CD_ZERO_MSGS();
}

stCDS_UseItemTo::stCDS_UseItemTo()
{
	CD_ZERO_MSGS();
}

stCDS_MergeItem::stCDS_MergeItem()
{
	CD_ZERO_MSGS();
	ukn_flag_ = kSIOF_Normal;
}

stCDS_OperCommand::stCDS_OperCommand()
{
	CD_ZERO_MSGS();
}

stCDS_SwitchStashPage::stCDS_SwitchStashPage()
{
	CD_ZERO_MSGS();
}

stCDS_LvlUpPassiveSkill::stCDS_LvlUpPassiveSkill()
{
	CD_ZERO_MSGS();
}
