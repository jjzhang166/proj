#pragma once
/*
@author zhp
@date 2014/7/26 12:27
@purpose communicate of login.dll and LoginH.dll
*/
#include <Common/FileMap.h>
#include <Common/UsefulClass.h>

class LLH_MemFileMap : public MemFileMap, public Singleton<LLH_MemFileMap, Singleton_MakeMe>{
	//type definition
public:
	struct stCommunicate{
		bool			safe_check_done_;			//安全检查
		bool			pwd_input_error_;			//密码输入错误
		bool			ac_freeze_;					//帐号被冻结
		bool			login_succeed_;				//登录成功

		stCommunicate();
	};

public:
	LLH_MemFileMap();
	//Read
	const stCommunicate* ReadAsCommu() const;
	//Write
	stCommunicate* WriteAsCommu();
};