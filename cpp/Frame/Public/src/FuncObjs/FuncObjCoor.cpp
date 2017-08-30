#include "stdafx.h"
#include <FuncObjs/FuncObjCoor.h>

P_CoorIdT CoorTrans::GetCoorId() const
{
	return (P_CoorIdT)this;
}

bool CoorTrans::OnInner( P_MsgNT msg_num, void* param, size_t param_size )
{
	if ( kInnerMsg_CoorTrans == msg_num )
	{
		assert(param_size >= sizeof(stCoorPacketInfo));
		return OnRecvTrans(param, param_size, this);
	}
	assert(false);
	return __super::OnInner(msg_num, param, param_size);
}

CoorTrans::CoorTrans( FuncObj* parent ) : FuncObj(parent, kFuncObjTemplateId_CoorTrans)
{
}

bool CoorTrans::DoTransImpl( void* param, size_t param_size, const CoorTrans* but_coor )
{
	return Inner(kInnerMsg_CoorTrans, param, param_size); 
}

P_CoorTypeT CoorTrans::GetCoorType() const
{
	return kPCT_Any;
}

bool CoorEndpoint::DoTrans( const P_CoorPacketToT& trans_route, P_FuncObjOper msg, const void* param, size_t param_size )
{
	stCoorPacketInfo packet_info;
	packet_info.way_to_ = trans_route;
	packet_info.oper_src_ = msg;
	cur_packet_info_ = &packet_info;
	return Inner(kInnerMsg_CoorTrans, param, param_size); 
}

bool CoorEndpoint::OnRecvTrans( void* param, size_t param_size, const CoorTrans* but_coor )
{
	assert(but_coor);
	assert(param);
	assert(param_size >= sizeof(*cur_packet_info_));
	auto packet_info = (stCoorPacketInfo*)((char*)param + param_size - sizeof(*cur_packet_info_));
	assert(3 == packet_info->way_from_.size());
	assert(packet_info->way_to_.empty());
	while ( !packet_info->way_from_.empty() )
	{
		packet_info->way_to_.push(packet_info->way_from_.top());
		packet_info->way_from_.pop();
	}
	return OnTrans(packet_info->way_to_, packet_info->oper_src_, (char*)param, param_size - sizeof(*cur_packet_info_));
}

CoorEndpoint::CoorEndpoint( FuncObj* parent ) : CoorTrans(parent)
{
	cur_packet_info_ = nullptr;
}

int CoorEndpoint::GetAdditionalParamSize() const
{
	return sizeof(*cur_packet_info_);
}

void CoorEndpoint::EncodeAddtionalParam( void* additional_param ) const
{
	assert(cur_packet_info_);
	CopyMemory(additional_param, cur_packet_info_, sizeof(*cur_packet_info_));
}

bool CoorTransTrack::OnRecvTrans( void* param, size_t param_size, const CoorTrans* but_coor )
{
	assert(but_coor);
	assert(param);
	assert(param_size >= sizeof(stCoorPacketInfo));
	auto packet_info = (stCoorPacketInfo*)((char*)param + param_size - sizeof(stCoorPacketInfo));
	P_WayToNextCoor this_way;
	this_way.method_ = kWTNC_ById;
	this_way.coor_id_ = GetCoorId();
	packet_info->way_from_.push(this_way);

	assert(!packet_info->way_to_.empty());
	const auto cur_way = packet_info->way_to_.front();
	packet_info->way_to_.pop();

	assert(track_iter_);
	if ( kWTNC_ByType == cur_way.method_ )
	{
		for ( track_iter_->First(); !track_iter_->IsDone(); track_iter_->Next() )
		{
			auto track_ptr = track_iter_->CurItem();
			assert(track_ptr);
			if ( but_coor != track_ptr && (kPCT_Any == cur_way.coor_type_ || cur_way.coor_type_ == track_ptr->GetCoorType()) )
			{
				assert(this != track_ptr);
				if ( !track_ptr->DoTransImpl(param, param_size, this) )
				{
					assert(false);
					return false;
				}
			}
		}
		return true;
	}
	else if ( kWTNC_ById == cur_way.method_ )
	{
		for ( track_iter_->First(); !track_iter_->IsDone(); track_iter_->Next() )
		{
			auto track_ptr = track_iter_->CurItem();
			assert(track_ptr);
			if ( cur_way.coor_id_ == track_ptr->GetCoorId() && but_coor != track_ptr )
			{
				assert(this != track_ptr);
				if ( !track_ptr->DoTransImpl(param, param_size, this) )
				{
					assert(false);
					return false;
				}
				return true;
			}
		}
		return true;
	}
	else
	{
		assert(false);
		return false;
	}
}

CoorTransTrack::CoorTransTrack( FuncObj* parent, const TrackIterPtrT& track_iter ) : CoorTrans(parent), track_iter_(track_iter)
{
	assert(track_iter);
}

CoorTransC_Local::CoorTransC_Local( FuncObj* parent, const TrackIterPtrT& track_iter ) : CoorTransTrack(parent, track_iter)
{
}

bool CoorTransC_Local::DoTransImpl( void* param, size_t param_size, const CoorTrans* but_coor )
{
	assert(param);
	assert(param_size >= sizeof(stCoorPacketInfo));
	auto packet_info = (stCoorPacketInfo*)((char*)param + param_size - sizeof(stCoorPacketInfo));
	assert(2 == packet_info->way_to_.size());
	const auto& next_way = packet_info->way_to_.front();
	if ( kWTNC_ByType == next_way.method_ )
	{
		if ( !__super::DoTransImpl(param, param_size, but_coor) )
		{
			assert(false);
			return false;
		}
	}
	else if ( kWTNC_ById == next_way.method_ )
	{
		if ( kPCI_Ignore != next_way.coor_id_ )
			return __super::DoTransImpl(param, param_size, but_coor);
	}
	else
	{
		assert(false);
		return false;
	}

	std::string buffer_tmp((const char*)param, param_size);
	param = const_cast<char*>(buffer_tmp.c_str());
	packet_info = (stCoorPacketInfo*)((char*)param + param_size - sizeof(stCoorPacketInfo));

	packet_info->way_to_.pop();

	P_WayToNextCoor this_way;
	this_way.method_ = kWTNC_ById;
	this_way.coor_id_ = kPCI_Ignore;
	packet_info->way_from_.push(this_way);

	return OnRecvTrans(param, param_size, but_coor);
}
