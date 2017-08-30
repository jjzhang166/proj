#pragma once
/*
@author zhp
@date 2017/1/23 19:34
@purpose for auto update featurea code
*/
#include "CmnWxDllApp.h"
#include <CmnMix/Cmn_FeatureCode.h>

class CmnAutoUpdateAppBase : public Cmn_DllApp{
protected:
	bool OnInit_() override;
	virtual bool InitFeatureCodeScan() = 0;
	virtual bool ScanFeatureCode() = 0;
	static pt_dword Addr2Offset(pt_pcvoid module_base, pt_pcvoid addr);
};

class CmnAutoUpdateApp : public CmnAutoUpdateAppBase{
public:
	CmnAutoUpdateApp();
	~CmnAutoUpdateApp();

protected:
	bool InitFeatureCodeScan() override;
	bool ScanFeatureCode(Cmn_FC_OFstream& ofs);
	int OnExit_() override;

protected:
	//hooks
	virtual bool Scan_Hooks(Cmn_FC_OFstream& ofs);
	//patch
	virtual bool Scan_Patch(Cmn_FC_OFstream& ofs);
	//call
	virtual bool Scan_Call(Cmn_FC_OFstream& ofs);
	//地址
	virtual bool Scan_Addr(Cmn_FC_OFstream& ofs);
	//偏移
	virtual bool Scan_Offset(Cmn_FC_OFstream& ofs);
	//虚表相关
	virtual bool Scan_Vtbl(Cmn_FC_OFstream& ofs);
	//网络相关
	virtual bool Scan_Net(Cmn_FC_OFstream& ofs);
	//角色相关信息
	virtual bool Scan_Role(Cmn_FC_OFstream& ofs);
	//物品相关信息
	virtual bool Scan_Item(Cmn_FC_OFstream& ofs);
	//npc相关信息
	virtual bool Scan_Npc(Cmn_FC_OFstream& ofs);
	//地图相关信息
	virtual bool Scan_Map(Cmn_FC_OFstream& ofs);
	//技能相关信息
	virtual bool Scan_Skill(Cmn_FC_OFstream& ofs);
	//任务相关信息
	virtual bool Scan_Task(Cmn_FC_OFstream& ofs);
	//数据模板相关
	virtual bool Scan_Db(Cmn_FC_OFstream& ofs);
	//ui相关
	virtual bool Scan_Ui(Cmn_FC_OFstream& ofs);
	//其他信息
	virtual bool Scan_Other(Cmn_FC_OFstream& ofs);

private:
	bool ScanFeatureCode() override final;

protected:
	const char*			update_out_file_dir_;
	const char*			update_out_file_name_;

private:
	Cmn_FC_OFstream*	out_file_impl_;
};

class AutoUpdateAppWithExe : public CmnAutoUpdateApp{
public:
	AutoUpdateAppWithExe();

protected:
	bool InitFeatureCodeScan() override;
	void SetExeModule(void* module_base, pt_dword module_size);
	pt_dword ExeAddr2Offset(pt_pcvoid addr);
	pt_dword ExeAddr2Offset(pt_dword addr);

protected:
	Cmn_FeatureCodeScan_Direct	scan_exe_;
	void*						exe_code_range_begin_;
	void*						exe_code_range_end_;
	void*						exe_module_base_;
};