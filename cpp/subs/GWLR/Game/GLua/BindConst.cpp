#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"

using namespace GType;
void GLuaC::BindConst()
{
	__super::BindConst();
	using namespace luabind;
	using namespace GType;
	/*module(lua())
		[
			class_<CD_En_FbDifficulty>("CD_En_FbDifficulty")
			.enum_("constants")
			[
				value("kFD_ShiLian", kFD_ShiLian),
				value("kFD_Fight", kFD_Fight),
				value("kFD_JiXianOrJingYing", kFD_JiXianOrJingYing),
				value("kFD_Invalid", kFD_Invalid)
			]
		];*/
}