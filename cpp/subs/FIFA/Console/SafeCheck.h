#pragma once
/*
@author zhp
@date 2014/6/10 15:24
@purpose for safe check
*/
#include "stdafx.h"
#include <Common/UsefulClass.h>
#include <CD_CommonFiles/CD_AntiCrack.h>

//防止别人用自己的安全检查的LPK
const char* const kMemMapName_CheckLpk = kCheckFileMapName;
//异或的常量
const unsigned int kMemMap_ConstValue = kXorConstValue;
#ifdef OPEN_CHECK_LPK_
//验证lpk的uuid string
const char* const kUuidStr_SafeCheckLpk = 
	"79a7b3c6-40a2-5267-8935-be0fb1a714bc";
#endif

//主窗口类名
const TCHAR* const kMainWndClsName = _T("fuckthecrack");

class SafeCheckLpk : public Singleton<SafeCheckLpk, Singleton_MakeMe>{
public:
	SafeCheckLpk();
	~SafeCheckLpk();

public:
	//创建验证lpk的内存映射文件
	bool CreateCheckLpkFileMap();
	//写入数值
	bool WriteValue( unsigned int value );
	//验证LPK UUID，看其是否被改写
	static bool SafeCheckLpkValid();
	//内存映射文件是否有效
	bool IsValid() const;

private:
	HANDLE				handle_file_map_;
	LPVOID				ptr_map_view_;
};