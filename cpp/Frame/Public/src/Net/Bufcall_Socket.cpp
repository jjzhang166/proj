#include "stdafx.h"
#include <Net/Bufcall_Socket.h>
#include <protect/Protect.h>
#include <Common/UsefulFunc.h>

bool ToCallSync_SocketImpl_(FuncObj* parent, BC_BufCallBase& bc, const BC_AsStreamW& stm, boost::optional<bool>& oper_res){
	VMP_BEGIN_EX;
	if (!parent)
	{
		assert(false);
		return false;
	}
	oper_res.reset();
	if (!bc.ToCall(stm))
		return false;
	assert(parent);
	WaitUntil(kToServerWaitTime, [&, parent](){
		if (oper_res)
			return true;
		if (!parent->ObjValid())
			return true;
		return false;
	});
	if (!parent->ObjValid())
		return false;
	return CheckOptionalBoolRes(oper_res);
	VMP_END;
}