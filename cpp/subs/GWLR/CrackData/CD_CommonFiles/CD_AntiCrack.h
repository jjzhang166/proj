#pragma once
/*
@author zhp
@date 2015/1/16 10:04
@purpose comunicate, for anti crack
*/

/*
const BYTE* file_map_ptr;
assert((file_map_ptr + kKeyValueOffset) ^ kXorConstValue == GetCurrentProcessId())
*/
//内存映射文件名
const char*const kCheckFileMapName = "847C01AC8C623E883BFD";
//需要校验的数据的偏移
const int kKeyValueOffset = 0x105;
//异或的常量
const int kXorConstValue = 0x91;

//启用优化的事件名
const char* const kUseYouHuaEventName = "B9D7F066BDD32C40";
