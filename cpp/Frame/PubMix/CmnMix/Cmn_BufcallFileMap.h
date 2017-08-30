#pragma once
/*
@author zhp
@date 2016/5/9 23:37
@purpose bufcall by file map
*/
#include <BufCall/BufCall.h>
#include "Cmn_FileMap.h"

class FileMapDoAtIdle{
public:
	virtual void Do() = 0;
};

template<typename Base>
class Bufcall_FileMapT : public Base, protected CmnFileMapQueue{
private:
	struct MsgHandler : CmnFileMapMsgSink{
		MsgHandler(Bufcall_FileMapT& owner) : owner_(owner){}
		void OnHandleMsg(unsigned int msg_id, const void* param, int param_size) override final{
			if (kMsg != msg_id)
			{
				assert(false);
				return;
			}
			if (!owner_.Decode(param, param_size))
				assert(false);
		}

		Bufcall_FileMapT& owner_;
	};
	friend bool ToCallSync_FileMapSocketImpl_(pt_dword wait_timeout, FileMapDoAtIdle* do_other, CmnFileMapQueue& file_map_queue, BC_BufCallBase& bc,
		const BC_AsStreamW& stm, boost::optional<bool>& oper_res);

	//接口
public:
	Bufcall_FileMapT() : msg_handler_(*this){
		SetMsgSink(&msg_handler_);
		idle_do_ = nullptr;
		wait_timeout_ = 1000 * 60 * 10;
	}
	using CmnFileMapQueue::CreateFileMap;
	using CmnFileMapQueue::OpenFileMap;
	using CmnFileMapQueue::GetHandle;
	using CmnFileMapQueue::Close;
	using CmnFileMapQueue::IsValid;
	using CmnFileMapQueue::GetTaskQueue;

public:
	//远程调用
	bool ToCall(const BC_AsStreamW& stm) override{
		auto& buffer = stm.GetStrBuffer();
		return PostMsg(kMsg, buffer.c_str(), buffer.size());
	}
	bool ToCallSync(const BC_AsStreamW& stm, OptionalBool& oper_res) override{
		return ToCallSync_FileMapSocketImpl_(wait_timeout_, idle_do_, *this, *this, stm, oper_res);
	}

protected:
	FileMapDoAtIdle*	idle_do_;
	pt_dword			wait_timeout_;

private:
	MsgHandler			msg_handler_;

private:
	static const MsgIdT		kMsg = 0;
};

//不会抛出异常
typedef Bufcall_FileMapT<BC_BufCallBase> BufCall_FileMap;
//会抛出异常
typedef Bufcall_FileMapT<BC_BufCallBase_Except> BufCall_FileMapExcept;

template<typename Base_, typename To_ = Base_>
class FileMapSocketBase : public Base_{
public:
	typedef Base_ base_type;
	typedef To_ to_type;

public:
	virtual bool InitSelf(){
		if (IsValid())
			return true;
		if (self_name_.empty())
		{
			assert(false);
			return false;
		}
		if (!CreateFileMap(self_name_))
		{
			assert(false);
			return false;
		}
		return true;
	}
	virtual bool PrepareTo(const std::string& to_name){
		if (to_.IsValid())
			to_.Close();
		return to_.OpenFileMap(to_name);
	}
	bool ToCallSync(const BC_AsStreamW& stm, OptionalBool& oper_res) override{
		return ToCallSync_FileMapSocketImpl_(wait_timeout_, idle_do_, *this, to_, stm, oper_res);
	}

protected:
	to_type				to_;
	std::string			self_name_;
};

template<typename Base_, typename To_ = Base_>
class FileMapSocketBaseC : public FileMapSocketBase<Base_, To_>{
public:
	FileMapSocketBaseC(){
		self_name_ = RandomUniqueStr(kUuidStrLen);
	}
};