#include "stdafx.h"
#include "GAddr.h"

namespace GAddr{
	void*	g_pSendObjAddr = nullptr;
	void*	g_pLocalObjAddr = nullptr;
	void*	g_pLocalRoomObjAddr = nullptr;
	void*	g_pLocalTownRoomObjAddr = nullptr;
	//void*	g_pGameMapObjAddr = nullptr;
	void*	g_pCurMapObjAddr = nullptr;
	void*	g_pLocalAllObjMapAddr = nullptr;
	void* g_pSkillTimestampValue = nullptr;
	DWORD g_through_room_valid_data_x = 0;
	DWORD g_through_room_valid_data_y = 0;
	bool g_through_room_dummy_exist = false;
	void* g_pPreActionPlayTimeAddr = nullptr;
	void* g_pLocalRoomPos = nullptr;
	unsigned int g_pSendCallSocket = (unsigned int)(WORD)-1 + 0xff;
}