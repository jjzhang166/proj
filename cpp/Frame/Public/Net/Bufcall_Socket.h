#pragma once
/*
@author zhp
@date 2016/5/9 23:38
@purpose bufcall by socket
*/
#include <BufCall/BufCall.h>
#include <FuncObjs/FuncObj.h>

template<typename Base>
class BufCall_SocketT : public Base, public FuncObj{
public:
	BufCall_SocketT(FuncObj* parent, P_ObjId id) : FuncObj(parent, id){}

public:
	bool ToCall(const BC_AsStreamW& stm) override{
		auto& buffer = stm.GetStrBuffer();
		return Inner(kInnerMsg_BufCall, buffer.c_str(), buffer.size());
	}
	bool ToCallSync(const BC_AsStreamW& stm, boost::optional<bool>& oper_res) override{
		bool ToCallSync_SocketImpl_(FuncObj* parent, BC_BufCallBase& bc, const BC_AsStreamW& stm, boost::optional<bool>& oper_res);
		return ToCallSync_SocketImpl_(parent_, *this, stm, oper_res);
	}

private:
	bool OnInner(P_MsgNT msg_num, void* param, size_t param_size) override{
		if (kInnerMsg_BufCall == msg_num)
			return Decode(param, param_size);
		return __super::Inner(msg_num, param, param_size);
	}
};

//不会抛出异常
typedef BufCall_SocketT<BC_BufCallBase> BufCall_Socket;
//会抛出异常
typedef BufCall_SocketT<BC_BufCallBase_Except> BufCall_SocketExcept;
