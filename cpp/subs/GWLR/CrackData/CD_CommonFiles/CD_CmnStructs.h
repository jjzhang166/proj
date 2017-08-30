#pragma once
/*
@author zhp
@date 2016/1/22 8:54
@purpose 一些LPK、数据分析通用结构体
*/
#include <ProtocolX/P_BaseTypeX.h>
#include <string>

typedef struct  
{
	WORD	wPacketLen;
	WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
	WORD	wUrlType;				// 请求实现方法具体类型
}PACKET_TYPE,*PPACKET_TYPE;

const pt_csz kLpkVersion = "D5CFEE4C-8518-44BF-8198-A6EF06128D69";

struct stLpkData{
	std::string		version_;
	std::string		cry_render_d3d9_;

	pt_dword		hook_dx_press_key_;			// hook dx press key， 用于判断是否需要取消和恢复优化
	pt_dword		hook_render_;				// 渲染线程优化
	pt_dword		black_screen_render_;		// 黑屏优化的基址偏移地址  CryRenderD3D11.dll

	pt_dword		sprintf_func_;				// MHOClientBase.dll 中的sprintf功能，拼接字符串，并用于多开的字符串
	pt_dword		tersafe_get_cpuid_;			// tersafe.dll hook GetCpuId功能函数
	pt_dword		tersafe_code_crc_hook_;		// tersafe.dll hook 检测 crc 的代码

	std::string		snprintf_;					// 多开 "_snprintf"
	std::string		ZwCreateMutant_;			// "ZwCreateMutant"
	std::string		CreateToolhelp32Snapshot_;	// "CreateToolhelp32Snapshot"
	std::string		EnumProcesses_;
	std::string		ZwQuerySystemInformation_;
	std::string		ZwQueryInformationProcess_;
	std::string		NtCreateFile_;
	std::string		ZwCreateEvent_;
	std::string		FindNextFileA_;
	std::string		DeviceIoControl_;
	std::string		ZwDeviceIoControlFile_;
	std::string		ZwCreateSection_;
	std::string		MemCpy_;					// hook ntdll memcpy
	std::string		CreateThread_;
	std::string		ZwOpenProcess_;

	stLpkData(){
		hook_dx_press_key_ = 0;
		hook_render_ = 0;
		black_screen_render_ = 0;
		sprintf_func_ = 0;
		tersafe_get_cpuid_ = 0;
		tersafe_code_crc_hook_ = 0;
	}
	template<typename T>
	void serialize(T& stm){
		stm & version_
			& cry_render_d3d9_
			& hook_dx_press_key_
			& hook_render_
			& black_screen_render_
			& sprintf_func_
			& tersafe_get_cpuid_
			& tersafe_code_crc_hook_
			& snprintf_
			& ZwCreateMutant_
			& CreateToolhelp32Snapshot_
			& EnumProcesses_
			& ZwQuerySystemInformation_
			& ZwQueryInformationProcess_
			& NtCreateFile_
			& ZwCreateEvent_
			& FindNextFileA_
			& DeviceIoControl_
			& ZwDeviceIoControlFile_
			& ZwCreateSection_
			& MemCpy_
			& CreateThread_
			& ZwOpenProcess_
			;
	}
};