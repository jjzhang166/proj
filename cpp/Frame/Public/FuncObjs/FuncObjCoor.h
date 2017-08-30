#pragma once
/*
@author zhp
@date 2014/9/26 13:27
@purpose for support coordinate
*/
#include "FuncObj.h"
#include <boost/noncopyable.hpp>
#include <Protocol/P_Coor.h>
#include <Common/GameObjBase.h>
#include "../DependSelfLib.h"

/*
CoorEndpoint->CoorTransTrack->CoorTransC_Local->CoorTransTrack->CoorEndpoint.
CoorEndpoint只用于Game.dll
*/
class CoorTrans : public FuncObj, public boost::noncopyable{
public:
	typedef IteratorTraits<CoorTrans*, IterContTag_ByRef> TrackTraits;
	typedef TrackTraits::iterator_pointer TrackIterPtrT;

public:
	CoorTrans( FuncObj* parent );
	virtual P_CoorTypeT GetCoorType() const;
	P_CoorIdT GetCoorId() const;
	virtual bool DoTransImpl( void* param, size_t param_size, const CoorTrans* but_coor );

protected:
	bool OnInner( P_MsgNT msg_num, void* param, size_t param_size );
	virtual bool OnRecvTrans( void* param, size_t param_size, const CoorTrans* but_coor ) = 0;
};

//协调终端
class CoorEndpoint : protected CoorTrans{
public:
	CoorEndpoint( FuncObj* parent );
	bool DoTrans( const P_CoorPacketToT& trans_route, P_FuncObjOper msg, const void* param, size_t param_size );
	virtual bool OnTrans( const P_CoorPacketToT& source_route, P_FuncObjOper msg, const void* param, size_t param_size ) = 0;

private:
	bool OnRecvTrans( void* param, size_t param_size, const CoorTrans* but_coor );

private:
	int GetAdditionalParamSize() const;
	void EncodeAddtionalParam( void* additional_param ) const;

private:
	const stCoorPacketInfo*		cur_packet_info_;
};

//用于协调服务器的track
//也用于从CoorEndpoint->CoorTransTrack
class CoorTransTrack : public CoorTrans{
public:
	CoorTransTrack( FuncObj* parent, const TrackIterPtrT& track_iter );

protected:
	bool OnRecvTrans( void* param, size_t param_size, const CoorTrans* but_coor );

protected:
	TrackIterPtrT			track_iter_;
};

//从CoorTransC_Local连接到协调服务器
class CoorTransC_Local : public CoorTransTrack{
public:
	CoorTransC_Local( FuncObj* parent, const TrackIterPtrT& track_iter );

protected:
	bool DoTransImpl( void* param, size_t param_size, const CoorTrans* but_coor );
};