#include "stdafx.h"
#include "GProtocolS.h"

GSendMsgHandler& GSendMsgHandler::GetMe()
{
	static GSendMsgHandler me;
	return me;
}

void GSendMsgHandler::AddCmds(CD_Interface::Hooks& hooks)
{
	hooks.AddCmdS<GS_HandlerT<stCDS_UseSkill> >();
	//hooks.AddCmdS<GS_CheckAny<kMIS_FirstPacket> >();
	hooks.AddCmdS<GS_CheckAny<kMIS_OperCommand> >();			//消息长度不是固定的
	hooks.AddCmdS<GS_HandlerT<stCDS_EnterMap> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_EnterMapConnection> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_OpenObject> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_ChangeMapFirstPacket> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_TakeupItem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_PutdownItem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_TakeoffGem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_InsideGem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_LvlUpPassiveSkill> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_UseItem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_UseFlask> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_UseItemTo> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_PutItemToShop> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_CloseNpc> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_Waypoint> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_NpcInventoryItemPutToSelfInventory> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_SwitchStashPage> >();
	hooks.AddCmdS<GS_CheckAny<kMIS_SwitchStashPageIdxChange> >();		//消息中有数组，不是固定长度的
	hooks.AddCmdS<GS_HandlerT<stCDS_BagToStash> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_StashToBag> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_NpcTalkItem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_Sell_Accept> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_Sell_Cancle> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_BuyItem_ToBag> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_LvlUpGem> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_HelpKillNpc> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_Revive> >();
	hooks.AddCmdS<GS_HandlerT<stCDS_InstanceManager> >();
}
