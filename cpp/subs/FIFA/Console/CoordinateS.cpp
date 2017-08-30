#include "stdafx.h"
#include "CoordinateS.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include <CmnMix/Cmn_TemplateUtility.h>

Session* CoordinateServer::CreateSession()
{
	return new CoordinateS(*this);
}

CoordinateServer::CoordinateServer() : Server(P_Port_Coordinate)
{
	io_connects_iter_ = CreateIoConnectIterPtr();
	assert(io_connects_iter_);
}

const CoordinateServer::IoConnectIterPtr& CoordinateServer::GetIoConnectIterPtr() const
{
	return io_connects_iter_;
}

CoorTrans::TrackIterPtrT CoordinateServer::MakeTransTrackIterPtr()
{
	return MakeWrapperIterator<CoorTrans::TrackTraits>(io_connects_iter_, []( IoConnectBase* io_connect ) -> CoorTrans*{
		assert(io_connect);
		return &PointerCast<CoordinateS>(io_connect)->GetCoorTrans();
	});
}

CoordinateS::CoordinateS( CoordinateServer& server ) : Session(server), coor_trans_(*this, server.MakeTransTrackIterPtr())
{

}

CoorTransTrack_CoorS& CoordinateS::GetCoorTrans()
{
	return coor_trans_;
}

CoorTransTrack_CoorS::CoorTransTrack_CoorS( CoordinateS& coor, const TrackIterPtrT& track_iter ) : CoorTransTrack(&coor, track_iter)
{

}
