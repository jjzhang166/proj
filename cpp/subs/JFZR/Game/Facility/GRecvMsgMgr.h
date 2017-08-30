#pragma once
/*
@author zhp
@date 2014/8/19 17:27
@purpose for handle recv msg
*/
#include "GProtocolR.h"
#include <Common/UsefulClass.h>

class GRecvMsgMgrBase{
	struct stMsgInfo{
		CMDR_MsgBase*		handler_msg_;
		WORD				ret_offset_;
	};
	typedef std::map<WORD, stMsgInfo> CmdMapT;

public:
	WORD HandleMsg( const CMDR_HeaderBase* cmd_header );
	WORD RecvPacket( const void* packet );

protected:
	template<typename CmdDerived>
	void AddTemplateCmd( const wchar_t* hash_uri ){
		static_assert(std::is_base_of<CMDR_MsgBase, CmdDerived>::value, "");
		static CmdDerived static_cmd;
		AddCmd(hash_uri, &static_cmd, 0);
	}
	template<typename CmdDerived>
	void AddTemplateCmd( const wchar_t* hash_uri, WORD ret_offset ){
		static_assert(std::is_base_of<CMDR_MsgBase, CmdDerived>::value, "");
		static CmdDerived static_cmd;
		AddCmd(hash_uri, &static_cmd, ret_offset);
	}
	void AddCmd( const wchar_t* hash_uri, CMDR_MsgBase* cmd_placeholder, WORD ret_offset );
	void AddCmd( WORD hash_uri, CMDR_MsgBase* cmd_placeholder, WORD ret_offset );

private:
	CmdMapT			cmd_map_;
};

class GRecvMsgMgr : public GRecvMsgMgrBase, public Singleton<GRecvMsgMgr, Singleton_Me>{
public:
	GRecvMsgMgr();

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