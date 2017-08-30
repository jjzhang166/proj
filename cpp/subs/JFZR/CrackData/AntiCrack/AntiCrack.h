#pragma once
/*
@author zhp
@date 2015/1/16 10:04
@purpose comunicate, for anti crack
*/

#if defined(__PROJ_JFZR_)	//多开
/*
const BYTE* file_map_ptr;
assert((file_map_ptr + kKeyValueOffset) ^ kXorConstValue == GetCurrentProcessId())
*/
//内存映射文件名
const char*const kCheckFileMapName = "97A9D36A13F0";
//需要校验的数据的偏移
const int kKeyValueOffset = 0x12;
//异或的常量
const int kXorConstValue = 0xf1;

#elif defined(__PROJ_JFZR_SM_)	//单开
const char*const kCheckFileMapName = "7F2C2A7EBF5F";
const int kKeyValueOffset = 0x19;
const int kXorConstValue = 0x31;

#elif defined(__PROJ_JFZR_TWO_)		//双开
const char*const kCheckFileMapName = "87105A02B5B0";
const int kKeyValueOffset = 0x41;
const int kXorConstValue = 0x95;
#endif