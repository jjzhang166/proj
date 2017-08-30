#include "stdafx.h"
#include "AntiCrack.h"
#include <Protocol/P_Defines.h>
#include <protect/Protect.h>
#include <Net/IoTimer.h>
#include <Common/CmnException.h>
#include "GLuaS.h"
#include "ServerIpInfo.h"
#include <Common/CmnDllApi.h>

void CheckLocalMachinePort()
{
#ifdef _DEBUG
	return;
#endif
	if ( !VMP_CRC_VALID() || VMP_IS_DEBUGGER() )
	{
		SetIoTimer(10 * 1000, []( IoTimer* timer ){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"is on debug or crc1"
#else
				"iodoc1"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}
	SOCKET listenSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(P_Port_CheckServer);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	int nRet = bind( listenSock, (sockaddr*)&sin, (int)(sizeof(sin)));
	if ( nRet == SOCKET_ERROR )
	{
		auto errCode =  WSAGetLastError();
		if ( WSAEADDRINUSE == errCode )
		{
			assert(false);
			GetIoService().post([](){
				MessageBoxTimeout(nullptr, "系统环境异常，你需要重启电脑", nullptr, MB_OK, 0, 1000 * 30);
			});
			SetIoTimer(13 * 1000, []( IoTimer* timer ){
				VMP_BEGIN_EX;
				const char* const kThrowStr =
#ifdef _DEBUG
					"CheckLocalMachinePort error"
#else
					"clmpe"
#endif
					;
				throw CmnExitIoThreadException(kThrowStr);
				VMP_END;
			});
		}
		return;
	}

	closesocket(listenSock);
}

CmnPriLuaData* g_lpd_ac = nullptr;

CmnPriLuaData& GetLPD_AC()
{
	if (g_lpd_ac)
		return *g_lpd_ac;
	auto lua_vm = GLuaVM_S::GetInstance();
	if ( !lua_vm )
	{
		assert(false);
		throw CmnGeneralRunException("lua vm not exist");
	}
	g_lpd_ac = new CmnPriLuaData(lua_vm->lua());
	if ( !g_lpd_ac )
	{
		assert(false);
		throw CmnGeneralRunException("cplde");
	}
	return *g_lpd_ac;
}

void SetLPD_CheckLoginFailedCnt( int cnt )
{
	using namespace luabind;
	auto& pld = GetLPD_AC();
#ifdef _DEBUG
	auto top_old = lua_gettop(pld.lua());
#endif
	object lua_cnt(pld.lua(), cnt);	
	pld.SetLuaObj(kLPD_CheckLoginFailedCnt, lua_cnt);
	assert(top_old == lua_gettop(pld.lua()));
}

int GetLPD_CheckLoginFailedCnt()
{
	return 1;
	auto& pld = GetLPD_AC();
	auto cnt = 0;
	return ObjectCast(pld.GetLuaObj(kLPD_CheckLoginFailedCnt), cnt);
}
