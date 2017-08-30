#include "stdafx.h"
#include <Net/BufCallCoorTrans.h>

BufCallCoorTrans::BufCallCoorTrans( FuncObj* parent ) : BufCall_SocketExcept(parent, kFuncObjTemplateId_BufCallCoorTrans), trans_endpoint_(parent, *this)
{

}

void BufCallCoorTrans::SetCurRoute( const P_CoorPacketToT& cur_route )
{
	cur_route_ = cur_route;
}

const P_CoorPacketToT& BufCallCoorTrans::GetCurRoute() const
{
	return cur_route_;
}

void BufCallCoorTrans::SetSourceRoute( const P_CoorPacketToT& source_route )
{
	source_route_ = source_route;
}

const P_CoorPacketToT& BufCallCoorTrans::GetSourceRoute() const
{
	return source_route_;
}

bool BufCallCoorTrans::Inner( P_MsgNT msg_num, const void* param, size_t param_size )
{
	assert(3 == cur_route_.size());
	return trans_endpoint_.DoTrans(cur_route_, P_FuncObjOper::MakeOper(P_Operate_Inner, msg_num), param, param_size);
}

bool BufCallCoorTrans::UserMsg( P_MsgNT msg_num, const void* param, size_t param_size )
{
	return trans_endpoint_.DoTrans(cur_route_, P_FuncObjOper::MakeOper(P_Operate_UserMsg, msg_num), param, param_size);
}

BufCallCoorEndpoint::BufCallCoorEndpoint( FuncObj* parent, BufCallCoorTrans& trans ) : CoorEndpoint(parent), trans_buf_call_(trans)
{

}

bool BufCallCoorEndpoint::OnTrans( const P_CoorPacketToT& source_route, P_FuncObjOper oper_src, const void* param, size_t param_size )
{
	trans_buf_call_.SetSourceRoute(source_route);
	return trans_buf_call_.DoDecode(oper_src, (void*)param, param_size);
}
