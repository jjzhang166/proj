#pragma once
/*
@author zhp
@date 2016/4/12 4:08
@purpose lua for test
*/
#include <CmnMix/Cmn_LuaBase.h>
#include <CommonX/CmnX_UsefulClass.h>

class CD_LuaVM : public LuaVM_Base, public Singleton<CD_LuaVM, Singleton_MakeMe>{
public:
	CD_LuaVM();

protected:
	void BindGlobalFn() override;
	void BindClass() override;
	bool DoPrivateLuaFileByFile() override;
};
