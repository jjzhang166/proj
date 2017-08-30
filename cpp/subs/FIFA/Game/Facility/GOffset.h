#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 存放偏移地址
*/
#include "GTypes.h"
#include <CD_CommonFiles/CD_Updates.h>

//g_o_->偏移地址一律以g_o_开头
namespace GOffset{
	//http发包call所需对象1
	//[[[01568df8]+24]+34]+86c4
	//const int g_o_HttpSendCall_NeededObj1[] = { 0x24, 0x34, 0x86c4 };
	//http发包call所需对象2
	//[[[[01568df8]+24]+34]+8688]
	//const int g_o_HttpSendCall_NeededObj2[] = { 0x24, 0x34, 0x8688 };
	//[[[[0x156cd5c]+0x24]+0x34]+0x8684]
	//const int g_o_HttpSendCall_NeededObj3[] = { 0x24, 0x34, 0x8684 };
	//////////////////////////////////////////////////////////////////////////
}