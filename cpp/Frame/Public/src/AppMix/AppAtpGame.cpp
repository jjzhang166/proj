#include "stdafx.h"
#include <AppMix/AppAtpGame.h>

AtpAppGameFactory* AtpAppGame::CreateAppFactory()
{
	return new AtpAppGameFactory;
}

void AtpAppGame::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)[
		class_<AtpAppGame, Cmn_OnlyGameDllApp>("AtpAppGame")
	];
}
