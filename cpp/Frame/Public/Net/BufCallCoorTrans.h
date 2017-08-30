#pragma once
/*
@author zhp
@date 2016/4/23 12:39
@purpose for BufCall coordinate translation
*/
#include <FuncObjs/FuncObjCoor.h>
#include <Net/Bufcall_Socket.h>
#include "../DependSelfLib.h"

class BufCallCoorTrans;
class BufCallCoorEndpoint : public CoorEndpoint{
public:
	BufCallCoorEndpoint( FuncObj* parent, BufCallCoorTrans& trans );

private:
	//通过BufCallCoorTrans来解包
	bool OnTrans( const P_CoorPacketToT& source_route, P_FuncObjOper oper_src, const void* param, size_t param_size );

private:
	BufCallCoorTrans&			trans_buf_call_;
};

class BufCallCoorTrans : public BufCall_SocketExcept{
public:
	BufCallCoorTrans( FuncObj* parent );
	void SetCurRoute( const P_CoorPacketToT& cur_route );
	const P_CoorPacketToT& GetCurRoute() const;
	void SetSourceRoute( const P_CoorPacketToT& source_route );
	const P_CoorPacketToT& GetSourceRoute() const;

private:
	//通过trans_endpoint_来发包
	bool Inner( P_MsgNT msg_num, const void* param, size_t param_size );
	bool UserMsg( P_MsgNT msg_num, const void* param, size_t param_size );

private:
	BufCallCoorEndpoint		trans_endpoint_;
	P_CoorPacketToT			cur_route_;
	P_CoorPacketToT			source_route_;
};