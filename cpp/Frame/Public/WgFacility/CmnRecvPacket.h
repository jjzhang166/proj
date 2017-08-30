#pragma once
/*
@author zhp
@date 2017/2/4 17:49
@purpose for common wg recv packet
*/
#include <map>
#include <ProtocolX/P_DefinesX.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/optional.hpp>
#include "../DependSelfLib.h"

//CMN_CLONE_IMPL
class Cmn_CMDR_MsgBase : public boost::enable_shared_from_this<Cmn_CMDR_MsgBase>{
public:
	typedef boost::shared_ptr<Cmn_CMDR_MsgBase> SelfPtr;

public:
	virtual ~Cmn_CMDR_MsgBase();
	virtual Cmn_CMDR_MsgBase* Clone() const = 0;
	//返回值大于0表示成功，表示读取的字节数，则OnReceived会被调用，否则不会调用；
	//返回值小于等于0表示读取的了多少字节数才出错的。
	virtual int DoDirect(pt_pcvoid msg_data, pt_dword msg_size) = 0;
	virtual void OnReceived() const;
	virtual pt_csz NewRetAddr() const;
	virtual bool IsPostToIoService() const;
};

template<int Size_>
struct Cmn_CMDR_Ignore{
	static_assert(Size_ > 0, "");
	enum{
		Size = Size_
	};
	static pt_dword GetSize(){
		return Size;
	}
};

class Cmn_CMDR_Serialize{
public:
	Cmn_CMDR_Serialize();
	void SetMsg(pt_pcvoid msg_data, pt_dword msg_size);
	pt_dword LeftSize() const;
	template<typename MsgT>
	int ReadMsg(MsgT& msg){
		assert(false);
		return 0;
	}
	pt_dword GetMsgSize() const;
	pt_dword GetCurReadSize() const;
	bool Ignore(pt_dword size);
	void IgnoreAll();

protected:
	bool ReadCopyTo(pt_pvoid data, pt_dword size);


protected:
	pt_pcvoid		msg_data_;
	pt_dword		msg_size_;
	pt_dword		cur_read_size_;
};

template<typename MsgT, typename SerializeT = Cmn_CMDR_Serialize>
class Cmn_CMDR_MsgSeria : public Cmn_CMDR_MsgBase{
public:
	typedef MsgT MsgType;
	enum{
		kMsgId = MsgT::kMsgId
	};

public:
	const MsgT& GetMsg() const{ return msg_; }
	MsgT& GetMsg(){ return msg_; }

protected:
	virtual bool DirectMsg() const{
		return true;
	}
	int DoDirect(pt_pcvoid msg_data, pt_dword msg_size) override{
		SerializeT s;
		s.SetMsg(msg_data, msg_size);
		int read_cnt = s.ReadMsg(msg_);
		if (read_cnt < 0)
			return read_cnt;
		if (DirectMsg()) return read_cnt;
		//出错了
		read_cnt = -read_cnt;
		return read_cnt;
	}
	CMN_CLONE_IMPL(Cmn_CMDR_MsgSeria)

public:
	MsgT		msg_;
};

class CmnRecvMsgHandler{
public:
	CmnRecvMsgHandler();
	CmnRecvMsgHandler(const CmnRecvMsgHandler&) = delete;
	CmnRecvMsgHandler& operator = (const CmnRecvMsgHandler&) = delete;
	virtual ~CmnRecvMsgHandler();
	void RecvMsg();
	CMN_CAST_TO

public:
	static CmnRecvMsgHandler* GetInstance();

private:
	static CmnRecvMsgHandler*		instance_;
};

template<typename MsgT, typename MsgHandlerT, typename SerializeT = Cmn_CMDR_Serialize>
class Cmn_CMDR_MsgSeriaHandler : public Cmn_CMDR_MsgSeria<MsgT, SerializeT>{
public:
	typedef boost::optional<MsgType> OptionMsgType;
	typedef MsgHandlerT MsgHandlerType;

private:
	void OnReceived() const override final{
		auto mgr = CmnRecvMsgHandler::GetInstance();
		if (!mgr)
		{
			assert(false);
			return;
		}
		auto real_mgr = mgr->CastTo<MsgHandlerType>();
		assert(real_mgr);
		OptionMsgType op_msg = GetMsg();
		real_mgr->RecvMsg(op_msg);
	}

private:
	CMN_CLONE_IMPL(Cmn_CMDR_MsgSeriaHandler)
};

class CmnRecvPacketMgr{
	struct stMsgInfo{
		Cmn_CMDR_MsgBase*		handler_msg_;
		pt_word					ret_offset_;
	};
	typedef std::map<pt_dword, stMsgInfo> CmdMapT;
	struct stNewRetAddrInfo{
		pt_csz		new_addr_;
		pt_word		ret_offset_;
	};

public:
	enum enHandleMsgRes{
		kHMR_HasNotMsgId,
		kHMR_DoDirectFaild,
		kHMR_Failed,
		kHMR_Succeed,
	};
	struct stHandleMsgRes{
		pt_pvoid		ret_addr_;
		int				read_cnt_;
		enHandleMsgRes	reason_;
	};

public:
	stHandleMsgRes HandleMsg(pt_pvoid def_ret_addr, pt_dword msg_id, pt_pcvoid msg_data, pt_dword msg_size);
	template<typename MsgT>
	void AddCmd(pt_word ret_offset = 0){
		static MsgT static_cmd;
		AddCmdImpl(MsgT::kMsgId, &static_cmd, ret_offset);
	}
	bool Empty() const;

private:
	void AddCmdImpl(pt_dword msg_id, Cmn_CMDR_MsgBase* cmd_placeholder, pt_word ret_offset);

private:
	CmdMapT			cmd_map_;
};
