#pragma once
/*
@author zhp
@date 2014/6/10 15:24
@purpose for safe check
*/
#include "stdafx.h"
#include <Common/UsefulClass.h>
#include "../CrackData/CD_CommonFiles/CD_AntiCrack.h"

//防止别人用自己的安全检查的LPK
const char* const kMemMapName_CheckLpk = kCheckFileMapName;
//异或的常量
const unsigned int kMemMap_ConstValue = kXorConstValue;
#ifdef OPEN_CHECK_LPK_
//验证lpkb的uuid string
const char* const kUuidStr_SafeCheckLpk = 
	/*kUuidStr_SafeCheckLpk*/"1f987860-23f7-5ef0-865a-135df1e65aff";
#endif

//主窗口类名
const TCHAR* const kMainWndClsName = _T("fuckathecrack");

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