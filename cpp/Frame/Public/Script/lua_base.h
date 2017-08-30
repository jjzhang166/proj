#pragma once
/*
@author zhp
@date 2014/1/20
@purpose use lua
*/
#include <CmnMix/Cmn_LuaOper.h>

//LuaVM这个类已基本不用了，除了老项目
class LuaVM : public LuaThrdData{
public:

	//for注册函数、类、结构等
protected:
	//全局函数
	void BindGlobalFn() override;
	//类
	void BindClass() override;
};