#pragma once
/*
@author zhp
@date 2014/7/1 20:44
@purpose for packet
*/
#include <Common/GameObjBase.h>
#include "GItem.h"

class GPacket : public GGameObjMgrBase{
public:
	GPacket();

	//获得每样物品栏
public:
	GItemMgr_Normal& GetTabNormal();
	GItemMgr_Consume& GetTabConsume();
	GItemMgr& GetTabTask();
	GItemMgr& GetTabMaterial();
	GItemMgr& GetTabAvatar();
	GItemMgr& GetTabEquiped();
	GItemMgr& GetTabAvatarEquiped();
	GItemMgr* FindByTabCategory( GType::enTabCategory tab_category );

	//更新与构建
public:
	bool UpdateAll();
	bool RebuildUpdateAll();
	bool UpdateNormal();
	bool RebuildUpdateNormal();
	bool UpdateConsume();
	bool RebuildUpdateConsume();
	bool UpdateTask();
	bool RebuildUpdateTask();
	bool UpdateMaterial();
	bool RebuildUpdateMaterial();
	bool UpdateAvatar();
	bool RebuildUpdateAvatar();
	bool UpdateEquiped();
	bool RebuildUpdateEquiped();
	bool UpdateAvatarEquiped();
	bool RebuildUpdateAvatarEquiped();

	//操作
public:
	//自动更换装备
	bool AutoReplaceEquip();

private:
	GItemMgr_Normal	tab_normal_;			//普通
	GItemMgr_Consume tab_consume_;			//消耗品
	GItemMgr		tab_task_;				//任务
	GItemMgr		tab_material_;			//材料
	GItemMgr		tab_avatar_;			//时装1,2,3，下面的
	GItemMgr		tab_equiped_;			//装备
	GItemMgr		tab_avatar_equiped_;	//时装，上面的
};