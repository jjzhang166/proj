#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"
#include "Facility/GameObj.h"

using namespace GType;
void GLuaC::BindConst()
{
	using namespace GType;
	__super::BindConst();
	using namespace luabind;
	module(lua())
		[
			class_<GameObjType>("GameObjType")
			.enum_("constants")
			[
				value("kGOT_Invalid", kGOT_Invalid),
				value("kGOT_MISC", kGOT_MISC),
				value("kGOT_Npc", kGOT_Npc),
				value("kGOT_Player", kGOT_Player),
				value("kGOT_Unknown", kGOT_Unknown),
				value("kGOT_Monster", kGOT_Monster),
				value("kGOT_ParseObjType", kGOT_ParseObjType)
			],
			class_<enFbDifficulty>("enFbDifficulty")
			.enum_("constants")
			[
				value("kFD_Normal", kFD_Normal),
				value("kFD_Fight", kFD_Fight),
				value("kFD_Master", kFD_Master),
				value("kFD_Overmaster", kFD_Overmaster),
				value("kFD_Invalid", kFD_Invalid)
			],
			/*class_<enEquipSlot>("enEquipSlot")
			.enum_("constants")
			[
				value("kES_Weapon", kES_Weapon),
				value("kES_Head", kES_Head),
				value("kES_Upper", kES_Upper),
				value("kES_Lower", kES_Lower),
				value("kES_Hand", kES_Hand),
				value("kES_Foot", kES_Foot),
				value("kES_Necklace", kES_Necklace),
				value("kES_Earring", kES_Earring),
				value("kES_Ring", kES_Ring),
				value("kES_Invalid", kES_Invalid)
			],*/
			class_<enGameState>("enGameState")
			.enum_("constants")
			[
				value("kGS_Invalid", kGS_Invalid),
				value("kGS_SelectChannel", kGS_SelectChannel),
				value("kGS_SelectRole", kGS_SelectRole),
				value("kGS_InGame", kGS_InGame)
			],
			class_<enItemQuality>("enItemQuality")
			.enum_("constants")
			[
				value("kIQ_Low", kIQ_Low),
				value("kIQ_Middle", kIQ_Middle),
				value("kIQ_High", kIQ_High),
				value("kIQ_Rare", kIQ_Rare),
				value("kIQ_Legendary", kIQ_Legendary)
			]
		];
}