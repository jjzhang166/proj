#pragma once
/*
@author zhp
@date 2017/4/3 14:46
@purpose for app atp game
*/
#include "OnlyGameComponent.h"


class AtpAppGameFactory : public OnlyGameAppFactory{
public:
};

class AtpAppGame : public Cmn_OnlyGameDllApp{
protected:
	AtpAppGameFactory* CreateAppFactory() override;
	void BindFrame(lua_State* l) override;
};
