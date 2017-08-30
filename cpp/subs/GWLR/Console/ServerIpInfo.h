#pragma once
/*
@author zhp
@date 2015-3-27
@purpose for server ip info
*/

const int kConsoleMinVer = /*kConsoleMinVer*/10;
#ifdef __PROJ_GWLR_MB_
const unsigned short int P_Port_CheckServer = 59420;
const char* const kRemoteCheckSvrDomain[] = { 
	"gw.hotmyfz.com", "gw1.hotmyfz.com", "gw2.hotmyfz.com"
};
#elif (__PROJ_GWLR_XE_)
const unsigned short int P_Port_CheckServer = 59421;
const char* const kRemoteCheckSvrDomain[] = { 
	"gwlrxe1.hotyxfz.com", "gwlrxe2.hotyxfz.com", "gwlrxe3.hotyxfz.com"
};
#elif (__PROJ_GWLR_MBP_)
const unsigned short int P_Port_CheckServer = 59422;
const char* const kRemoteCheckSvrDomain[] = { 
	"gwlrmbp1.hotyxfz.com", "gwlrmbp2.hotyxfz.com"
};
#endif