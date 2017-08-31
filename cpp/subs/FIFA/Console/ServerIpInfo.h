#pragma once
/*
@author zhp
@date 2015-3-27
@purpose for server ip info
*/

const int kConsoleMinVer = 30;
#ifdef __PROJ_JIN_GANG_
	const unsigned short int P_Port_CheckServer = 59421;
	//const char* const kRemoteCheckServerIp = "162.251.95.27";
	const char* const kRemoteCheckSvrDomain[] = {
		"23.234.25.16", "jg.hotmyfz.com", "jg1.hotmyfz.com"
};
#elif defined(__PROJ_XIAO_ER_)	//给小二的版本
	const unsigned short int P_Port_CheckServer = 45221;
	const char* const kRemoteCheckSvrDomain[] = {
		"jgxiaoer.hotyxfz.com", "jgxiaoer1.hotyxfz.com"
};
#endif