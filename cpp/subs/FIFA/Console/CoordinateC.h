#pragma once
/*
@author zhp
@date 2014/5/18 21:05
@purpose @purpose for coordinate connection,for example FIFAÊÕ½ð
*/
#include "Net/client.h"
#include "Common/UsefulClass.h"
#include <FuncObjs/FuncObjCoor.h>
#include <string>
#include <Communicate/CG_Defines.h>


class CoorCMgr : public IoConnectsMgr, public Singleton<CoorCMgr, Singleton_MakeMe>{
public:
	CoorCMgr();
	CoorTrans::TrackIterPtrT MakeTransTrackIterPtr();

private:
	IoConnectIterPtr		io_connects_iter_;
};
//////////////////////////////////////////////////////////////////////////
class CoordinateC;
class CoorTransC_Local_Impl : public CoorTransC_Local{
public:
	CoorTransC_Local_Impl( CoordinateC& coor, const TrackIterPtrT& track_iter );
	P_CoorTypeT GetCoorType() const;

private:
	CoordinateC&		coor_;
};
//////////////////////////////////////////////////////////////////////////
class CoordinateC : public Connection{
	friend class CoorTransC_Local_Impl;

public:
	CoorTransC_Local_Impl& GetCoorTrans();

protected:
	CoordinateC( const std::string& str_coor_ip, enDstServerType dst_ser_type );
	bool ConnectDone();

private:
	enDstServerType			dst_ser_type_;
	CoorTransC_Local_Impl	trans_impl_;
};

class CoordinateC_Normal : public CoordinateC, public Singleton<CoordinateC_Normal, Singleton_Instance>{
public:
	CoordinateC_Normal( const std::string& str_coor_ip );
};