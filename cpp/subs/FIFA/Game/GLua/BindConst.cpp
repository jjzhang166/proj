#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"

using namespace GType;
void GLuaC::BindConst()
{
	__super::BindConst();
	using namespace luabind;
	module(lua())
		[
			//发包call执行结果
			class_<GType::enHttpSendCall_Result>("enHttpSendCall_Result")
			.enum_("constants")
			[
				value("enHttpSendCall_Result_Fail", enHttpSendCall_Result_Fail),
				value("enHttpSendCall_Result_Succeed", enHttpSendCall_Result_Succeed)
			],
			//游戏状态
			class_<GType::enGameState>("enGameState")
			.enum_("constants")
			[
				value("enGameState_Invalid", enGameState_Invalid),
				value("enGameState_PressOnClick", enGameState_PressOnClick),
				value("enGameState_NoticeDlg", enGameState_NoticeDlg),
				value("enGameState_TrainerSelect", enGameState_TrainerSelect),
				value("enGameState_TrainerSelected", enGameState_TrainerSelected),
				value("enGameState_Managers", enGameState_Managers),
				value("enGameState_Custom", enGameState_Custom),
				value("enGameState_Managers_Computer", enGameState_Managers_Computer),
				value("enGameState_Managers_Friend", enGameState_Managers_Friend),
				value("enGameState_Managers_Random", enGameState_Managers_Random),
				value("enGameState_Match_Wait", enGameState_Match_Wait),
				value("enGameState_Match_Choose_Player", enGameState_Match_Choose_Player),
				value("enGameState_Match_Ready", enGameState_Match_Ready),
				value("enGameState_Match_BeginFirtAnimate", enGameState_Match_BeginFirtAnimate),
				value("enGameState_MatchEnd", enGameState_MatchEnd),
				value("enGameState_MatchEnd_Prize", enGameState_MatchEnd_Prize),
				value("enGameState_MatchEnd_Back", enGameState_MatchEnd_Back),
				value("enGameState_Managers_Contract", enGameState_Managers_Contract),
				value("enGameState_ClubManage", enGameState_ClubManage),
				value("enGameState_AutoSelectPlayer", enGameState_AutoSelectPlayer),
				value("enGameState_CreateCoach", enGameState_CreateCoach)
			],
			//比赛模式
			class_<GType::enMatchMode>("enMatchMode")
			.enum_("constants")
			[
				value("enMatchMode_Invalud", enMatchMode_Invalid),
				value("enMatchMode_Custom", enMatchMode_Custom),
				value("enMatchMode_Managers", enMatchMode_Managers)
			],
			//对阵模式
			class_<GType::enFightMode>("enFightMode")
			.enum_("constants")
			[
				value("enFightMode_Invalid", enFightMode_Invalid),
				value("enFightMode_Friend", enFightMode_Friend),
				value("enFightMode_Computer", enFightMode_Computer)
			],

			class_<GType::enPlayerPosition>("enPlayerPosition")
			.enum_("constants")
			[
				value("enPlayerPosition_No", enPlayerPosition_No),
				value("enPlayerPosition_First", enPlayerPosition_First),
				value("enPlayerPosition_Second", enPlayerPosition_Second),
				value("enPlayerPosition_Third", enPlayerPosition_Third)
			],

			class_<GType::enCreateCoach_SubState>("enCreateCoach_SubState")
			.enum_("constants")
			[
				value("enCC_SubState_Invalid", enCC_SubState_Invalid),
				value("enCC_SubState_InputName", enCC_SubState_InputName),
				value("enCC_SubState_SelectTeam", enCC_SubState_SelectTeam)
			]
		];
}