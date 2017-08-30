#pragma once
/*
@author zhp
@date 2015-3-27
@purpose for server ip info
*/

#ifdef __PROJ_JFZR_
	const unsigned short int P_Port_CheckServer = 45600;
	const char* const kRemoteCheckServerIp = "162.251.95.27";
#elif defined(__PROJ_JFZR_SM_)	//给小二的版本
	const unsigned short int P_Port_CheckServer = 45601;
	const char* const kRemoteCheckServerIp = "162.251.95.27";
#elif defined(__PROJ_JFZR_TWO_)
	const unsigned short int P_Port_CheckServer = 45602;
	const char* const kRemoteCheckServerIp = "162.251.95.27";
#endif