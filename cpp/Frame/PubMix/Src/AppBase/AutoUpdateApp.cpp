#include "stdafx.h"
#include <AppBase/AutoUpdateApp.h>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/filesystem/path.hpp>

bool CmnAutoUpdateAppBase::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	LOG_O(Log_info) << "开始更新";
	if (!InitFeatureCodeScan())
	{
		assert(false);
		return false;
	}
	try{
		if (!ScanFeatureCode())
		{
			assert(false);
			return false;
		}
	}
	catch (...)
	{
		LOG_O(Log_error) << "更新出异常了";
		assert(false);
	}
	LOG_O(Log_info) << "更新正常结束了";
	return false;
}

pt_dword CmnAutoUpdateAppBase::Addr2Offset(pt_pcvoid module_base, pt_pcvoid addr)
{
	assert(addr >= module_base);
	return (char*)addr - (char*)module_base;
}

int CmnAutoUpdateApp::OnExit_()
{
	if (out_file_impl_)
	{
		delete out_file_impl_;
		out_file_impl_ = nullptr;
	}
	return __super::OnExit_();
}

CmnAutoUpdateApp::CmnAutoUpdateApp()
{
	update_out_file_dir_ = "f:\\tmp";
	update_out_file_name_ = "CD_Updates.h";
	out_file_impl_ = nullptr;
}

bool CmnAutoUpdateApp::InitFeatureCodeScan()
{
	if (out_file_impl_)
	{
		assert(false);
		return false;
	}
	assert(update_out_file_dir_ && update_out_file_name_);
	boost::filesystem::path p(update_out_file_dir_);
	p /= update_out_file_name_;
	out_file_impl_ = new Cmn_FC_OFstream(p.string(), std::ios_base::out | std::ios_base::trunc);
	if (!out_file_impl_)
	{
		assert(false);
		return false;
	}
	*out_file_impl_ << R"(#pragma once
#include <ProtocolX/P_BaseTypeX.h>
/*
g_a->g_address.
g_o->g_offset.
g_va->g_vtbl_address.
g_vo->g_vtbl_offset.
g_h->g_hook.
g_c->g_call.
g_p->g_patch.
g_v->g_value.
*/

)";
	return true;
}

CmnAutoUpdateApp::~CmnAutoUpdateApp()
{
	if (out_file_impl_)
	{
		delete out_file_impl_;
		out_file_impl_ = nullptr;
	}
}

bool CmnAutoUpdateApp::ScanFeatureCode()
{
	if (!out_file_impl_)
	{
		assert(false);
		return false;
	}
	return ScanFeatureCode(*out_file_impl_);
}

AutoUpdateAppWithExe::AutoUpdateAppWithExe()
{
	SetExeModule((void*)0x00400000, 0x2000);
}

bool AutoUpdateAppWithExe::InitFeatureCodeScan()
{
	if (!__super::InitFeatureCodeScan())
	{
		assert(false);
		return false;
	}
	if (exe_code_range_end_ <= exe_code_range_begin_)
	{
		assert(false);
		return false;
	}
	scan_exe_.SetCodeRange(exe_code_range_begin_, exe_code_range_end_);
	scan_exe_.SetModuleBegin(exe_module_base_);
	return true;
}

pt_dword AutoUpdateAppWithExe::ExeAddr2Offset(pt_pcvoid addr)
{
	assert(addr >= exe_code_range_begin_ && addr < exe_code_range_end_);
	return (char*)addr - (char*)exe_module_base_;
}

pt_dword AutoUpdateAppWithExe::ExeAddr2Offset(pt_dword addr)
{
	return ExeAddr2Offset((pt_pcvoid)addr);
}

void AutoUpdateAppWithExe::SetExeModule(void* module_base, pt_dword module_size)
{
	assert(module_base);
	assert(module_size > 0x1000);
	exe_module_base_ = module_base;
	exe_code_range_begin_ = (char*)module_base + 0x1000;
	exe_code_range_end_ = (char*)module_base + module_size;
}

bool CmnAutoUpdateApp::ScanFeatureCode(Cmn_FC_OFstream& ofs)
{
	ofs << "////////////////////////////////////\n//hooks\n";
	if (!Scan_Hooks(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//patch\n";
	if (!Scan_Patch(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//call\n";
	if (!Scan_Call(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//addr\n";
	if (!Scan_Addr(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//offset\n";
	if (!Scan_Offset(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//vtbl\n";
	if (!Scan_Vtbl(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//net\n";
	if (!Scan_Net(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//role\n";
	if (!Scan_Role(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//item\n";
	if (!Scan_Item(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//npc\n";
	if (!Scan_Npc(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//map\n";
	if (!Scan_Map(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//Skill\n";
	if (!Scan_Skill(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//Task\n";
	if (!Scan_Task(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//db\n";
	if (!Scan_Db(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//ui\n";
	if (!Scan_Ui(ofs))
	{
		assert(false);
		return false;
	}
	ofs << "\n////////////////////////////////////\n//other\n";
	if (!Scan_Other(ofs))
	{
		assert(false);
		return false;
	}
	return true;
}

bool CmnAutoUpdateApp::Scan_Hooks(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Patch(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Call(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Addr(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Offset(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Vtbl(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Net(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Role(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Item(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Npc(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Map(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Db(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Other(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Skill(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Task(Cmn_FC_OFstream& ofs)
{
	return true;
}

bool CmnAutoUpdateApp::Scan_Ui(Cmn_FC_OFstream& ofs)
{
	return true;
}
