#pragma once
/*
@author zhp
@date 2016/5/10 14:59
@purpose 获得本地信息
*/
#include "Bufcall_FileMap.h"
#include "../DependSelfLib.h"

class GainLocalInfoS : public FileMapSocketBase<BufCall_FileMap>{
public:
	GainLocalInfoS();
	bool InitSelf() override;
	void GetMachineCode(const std::string& session_src);
	void GetCookie(const std::string& session_src, const std::string& whose);
	void SetCookie(const std::string& session_src, const std::string& whose, const std::string& cookie);

	BC_REG_CLS_DECLARE(GainLocalInfoS);
};