#pragma once
/*
@author zhp
@date 2017/7/4 0:32
@purpose for send
*/
#include <memory>
#include <boost/optional.hpp>
#include <CD_CommonFiles/Data/CD_ProtocolS.h>
#include <CD_CommonFiles/Data/CD_Interface.h>
#include "GConst.h"

class GRecvMsgHandler;

template<typename MsgT>
class GS_HandlerT : public CD_CMDS_Serialize<MsgT>{
private:
	CMN_CLONE_IMPL(GS_HandlerT)
};

template<pt_word MsgId>
class GS_CheckAny : public GS_HandlerT<stCDS_CheckAny<MsgId> >{
private:
	CMN_CLONE_IMPL(GS_CheckAny)
};

class GSendMsgHandler{
public:
	void AddCmds(CD_Interface::Hooks& hooks);

public:
	static GSendMsgHandler& GetMe();	
};
