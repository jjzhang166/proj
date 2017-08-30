#pragma once
/*
@author zhp
@date 2014/5/18 21:00
@purpose for coordinate service,for example FIFAÊÕ½ð
*/
#include "Net/server.h"
#include "Common/UsefulClass.h"
#include <set>
#include <FuncObjs/FuncObjCoor.h>

class CoordinateS;
class CoordinateServer : public Server, public Singleton<CoordinateServer, Singleton_Instance>{
public:
	CoordinateServer();

	//interfaces
public:
	const IoConnectIterPtr& GetIoConnectIterPtr() const;
	CoorTrans::TrackIterPtrT MakeTransTrackIterPtr();

private:
	virtual Session* CreateSession();

private:
	IoConnectIterPtr		io_connects_iter_;
};
//////////////////////////////////////////////////////////////////////////
class CoordinateS;
class CoorTransTrack_CoorS : public CoorTransTrack{
public:
	CoorTransTrack_CoorS( CoordinateS& coor, const TrackIterPtrT& track_iter );
};
//////////////////////////////////////////////////////////////////////////
class CoordinateS : public Session{
public:
	CoordinateS( CoordinateServer& server );

	CoorTransTrack_CoorS& GetCoorTrans();

private:
	CoorTransTrack_CoorS	coor_trans_;
};