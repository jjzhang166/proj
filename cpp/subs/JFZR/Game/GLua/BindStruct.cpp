#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"
#include "Facility/GPlayer.h"
#include "Facility/GClass.h"

void GLuaC::BindStruct()
{
	__super::BindStruct();
	using namespace luabind;
	using namespace GType;
	module(lua())
		[
			class_<stGameVector>("stGameVector")
			.def(constructor<>())
			.def(constructor<float, float, float>())
			.property("x_", &stGameVector::GetPosX, &stGameVector::SetPosX)
			.property("y_", &stGameVector::GetPosY, &stGameVector::SetPosY)
			.property("z_", &stGameVector::GetPosZ, &stGameVector::SetPosZ)
		];
}