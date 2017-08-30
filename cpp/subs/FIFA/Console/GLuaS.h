#pragma once
/*
@author zhp
@date 2014-2-24 13:49
@purpose for game lua
*/
#include "FuncObjs/FuncObj.h"
//#include "Script/lua_base.h"
#include <Script/LuaScript.h>
#include <Common/CmnPriLuaData.h>
#include <Script/lua_base.h>

class GLuaS : public GLua{
public:
	GLuaS( FuncObj* parent );
};

class FuncObj_NormalS;
class LuaScriptS : public LuasScriptEchoS{
public:
	LuaScriptS(FuncObj_NormalS& func_obj );
	bool OutputScriptInfo( const std::string& script_info ) override;

private:
	FuncObj_NormalS&			func_obj_;

	BC_REG_CLS_DECLARE(LuaScriptS);
};

class LuaAll : public LuaVM{
	void BindClass() override;
};

class GLuaVM_S : public LuaAll, public Singleton<GLuaVM_S, Singleton_Instance>{
public:
	CmnPriLuaData* GetLoginData() const;

protected:
	bool InitThrdData() override;

private:
	std::shared_ptr<CmnPriLuaData>			login_data_;
};