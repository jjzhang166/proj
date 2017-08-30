#pragma once
/*
@author zhp
@date 2016/6/19 21:07
*/
#include <Script/lua_base.h>
#include <CommonX/CmnX_UsefulClass.h>

class RECM_LuaVM : public LuaVM{
protected:
	void BindClass() override;
};