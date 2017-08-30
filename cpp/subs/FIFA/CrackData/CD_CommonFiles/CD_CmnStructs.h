#pragma once
/*
@author zhp
@date 2016/1/26 19:05
@purpose 一些LPK、数据分析通用结构体
*/
#include <string>

typedef struct  
{
	WORD	wPacketLen;
	WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
	WORD	wUrlType;				// 请求实现方法具体类型
}PACKET_TYPE,*PPACKET_TYPE;