#include "stdafx.h"
#include "GTypes.h"
#include "Common/LogOut.h"
#include "GFuncCall.h"

GType::stHttpSendCall_Content::stHttpSendCall_Content()
{
	ZeroMemory(this, sizeof(*this));
}

GType::stMatchInfo::stMatchInfo()
{
	match_id = 0;
	room_id = 0;
}

DWORD GType::stMatchInfo::GetRoomId() const
{
	return room_id;
}

void GType::stMatchInfo::SetRoomId( DWORD id )
{
	room_id = id;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置房间ID：" << room_id;
}

DWORD GType::stMatchInfo::GetMatchId() const
{
	return match_id;
}

void GType::stMatchInfo::SetMatchId( DWORD id )
{
	match_id = id;
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "设置比赛ID：" << match_id;
}

GType::stHttpSendCall_NeededObj3::stHttpSendCall_NeededObj3()
{
	static_assert(sizeof(*this) == 0x60, "size not equal 0x60");
	char** str_accept = (char**)((char*)this + 0x18);
	*str_accept = "Accept: */*\r\n";
}

/*
void* GType::stHttpSendCall_NeededObj3::operator new( size_t size )
{
	return GFuncCall::CallocOfGame(1, size);
}

void GType::stHttpSendCall_NeededObj3::operator delete( void* pointee )
{
	GFuncCall::FreeOfGame(pointee);
}
*/
