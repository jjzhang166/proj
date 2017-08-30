#include "stdafx.h"
#include "CoordinateC.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include <Common/TemplateUtility.h>
#include "TabOption.h"

CoordinateC::CoordinateC( const std::string& str_coor_ip, enDstServerType dst_ser_type ) : Connection(str_coor_ip, P_Port_Coordinate), 
	trans_impl_(*this, CTabOption::GetInstance()->MakeTransTrackIterPtr())
{
	dst_ser_type_ = dst_ser_type;
}

bool CoordinateC::ConnectDone()
{
	AddSink(CoorCMgr::GetInstance());
	return __super::ConnectDone();
}

CoorTransC_Local_Impl& CoordinateC::GetCoorTrans()
{
	return trans_impl_;
}

P_CoorTypeT CoorTransC_Local_Impl::GetCoorType() const
{
	return coor_.dst_ser_type_;
}

CoorTransC_Local_Impl::CoorTransC_Local_Impl( CoordinateC& coor, const TrackIterPtrT& track_iter ) : CoorTransC_Local(&coor, track_iter), coor_(coor)
{

}

CoorCMgr::CoorCMgr()
{
	io_connects_iter_ = CreateIoConnectIterPtr();
	assert(io_connects_iter_);
}

CoorTrans::TrackIterPtrT CoorCMgr::MakeTransTrackIterPtr()
{
	return MakeWrapperIterator<CoorTrans::TrackTraits>(io_connects_iter_, []( IoConnectBase* io_connect ) -> CoorTrans*{
		assert(io_connect);
		return &PointerCast<CoordinateC>(io_connect)->GetCoorTrans();
	});
}

CoordinateC_Normal::CoordinateC_Normal( const std::string& str_coor_ip ) : CoordinateC(str_coor_ip, kDST_Normal)
{

}