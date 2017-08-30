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
const char*const kCheckFileMapName = "28C159FD41C5";
//需要校验的数据的偏移
const int kKeyValueOffset = 0x109;
//异或的常量
const int kXorConstValue = 0x56;