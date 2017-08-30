#pragma once
#include <AppBase/AutoUpdateApp.h>

#define CheckExeAddr(addr) assert((pt_pcvoid)addr > (pt_pcvoid)0x00401000 && (pt_pcvoid)addr < exe_code_range_end_)
//Cmn_FeatureCodeScan::CallAddr2AbsoluteAddr
class CAutoUpdateApp : public AutoUpdateAppWithExe{
public:
	CAutoUpdateApp();

private:
	bool Scan_Addr(Cmn_FC_OFstream& ofs) override;
	bool Scan_Net(Cmn_FC_OFstream& ofs) override;
	//角色相关信息
	bool Scan_Role(Cmn_FC_OFstream& ofs) override;
	//物品相关信息
	bool Scan_Item(Cmn_FC_OFstream& ofs) override;
	//npc相关信息
	bool Scan_Npc(Cmn_FC_OFstream& ofs) override;
	//地图相关信息
	bool Scan_Map(Cmn_FC_OFstream& ofs) override;
	//技能相关信息
	bool Scan_Skill(Cmn_FC_OFstream& ofs) override;
	//任务相关信息
	bool Scan_Task(Cmn_FC_OFstream& ofs) override;
	//数据模板相关
	bool Scan_Db(Cmn_FC_OFstream& ofs) override;
	//ui相关
	bool Scan_Ui(Cmn_FC_OFstream& ofs) override;
	//vtbl
	bool Scan_Vtbl(Cmn_FC_OFstream& ofs) override;
	//hooks
	bool Scan_Hooks(Cmn_FC_OFstream& ofs) override;
	//patch
	bool Scan_Patch(Cmn_FC_OFstream& ofs) override;
	//其他信息
	bool Scan_Other(Cmn_FC_OFstream& ofs) override;
	bool Scan_Offset(Cmn_FC_OFstream& ofs) override;
	bool Scan_Call(Cmn_FC_OFstream& ofs) override;
};
CMN_MY_APP_DECLARE(CAutoUpdateApp);