#pragma once
/*
@author zhp
@date 2014/8/19 17:27
@purpose for handle recv msg
*/
#include "GProtocolR.h"
#include <Common/UsefulClass.h>
#include <CommonX/CmnX_Lock.h>

class GRecvMsgMgrBase{
	struct stMsgInfo{
		CMDR_MsgBase*		handler_msg_;
		WORD				ret_offset_;
	};
	typedef std::map<WORD, stMsgInfo> CmdMapT;
	struct stNewRetAddrInfo{
		const char*		new_addr_;
		WORD			ret_offset_;
	};

public:
	stNewRetAddrInfo HandleMsg( const CMDR_HeaderBase* cmd_header );
	static void EnableOutput(bool enable);

public:
	template<typename CmdDerived>
	void AddTemplateCmd( WORD ret_offset = 0 ){
		static_assert(std::is_base_of<CMDR_MsgBase, CmdDerived>::value, "");
		static CmdDerived static_cmd;
		AddCmd(CmdDerived::kMsgId, &static_cmd, ret_offset);
	}

protected:
	void AddCmd( WORD msg_id, CMDR_MsgBase* cmd_placeholder, WORD ret_offset );

private:
	CmdMapT			cmd_map_;
};

class GRecvMsgMgr : public GRecvMsgMgrBase, public Singleton<GRecvMsgMgr, Singleton_Me>{
public:
	GRecvMsgMgr();
	DateTimeLocal GetLastRecvTime();
	void RefreshLastRecvTime();

private:
	Cmnx_Mutex_CriticalSection		time_lock_;
	DateTimeLocal					last_recv_;

private:
	static GRecvMsgMgr	msg_mrg_;
};

//game send only
class GHookGameSendMsgMgr : public GRecvMsgMgrBase, public Singleton<GHookGameSendMsgMgr, Singleton_Me>{
public:
	GHookGameSendMsgMgr();

private:
	static GHookGameSendMsgMgr	msg_mrg_;
};

//game send and self send
class GHookAllSendMsgMgr : public GRecvMsgMgrBase, public Singleton<GHookAllSendMsgMgr, Singleton_MakeMe>{
public:
	GHookAllSendMsgMgr();

private:
	static GHookAllSendMsgMgr	msg_mrg_;
};