#pragma once
/*
@author zhp
@date 2016/12/22 11:04
@purpose app execute environment
*/
#include <string>
#include <boost/filesystem/path.hpp>
#include <wx/wxprec.h>
#include <vector>
#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ProtocolX/P_BaseTypeX.h>
#include "AppExecEnv.h"


class AdbExecEnv{
public:
	typedef std::vector<std::string> StringContT;

public:
	virtual ~AdbExecEnv();
	bool AdbExec(const std::string& device, const wxChar* cmd, wxArrayString& out);
	bool AdbConnect(const std::string& device);
	bool AdbHasTheDeviceInfo(const std::string& device_info);
	bool AdbStartServer(bool is_async = true);
	int AdbListPackages(const std::string& device, StringContT& packages);
	bool AdbInstallApk(const std::string& device, const std::string& apk_path);
	bool AdbPmInstallApk(const std::string& device, const std::string& apk_name);
	bool AdbUnInstallApk(const std::string& device, const std::string& package_name);
	//若app已启动，则会把它带到前台。
	bool AdbStartApp(const std::string& device, const std::string& package_name, const std::string& activity_name);
	bool AdbStopApp(const std::string& device, const std::string& package_name);
	//遍历进程还未支持，尚未用到
	bool AdbTraverseProcessInfo();

protected:
	virtual bool AdbExecCmd(const wxString& cmd) = 0;
	virtual bool AdbExecCmd(const wxString& cmd, wxArrayString& out) = 0;
	virtual long AdbExecAsync(const wxString& cmd) = 0;
};

class DmExecEnv{
public:
	virtual ~DmExecEnv();
	bool DmRegsvr32();

protected:
	virtual bool DmExecCmd(const wxString& cmd, wxArrayString& out) = 0;
};

//只能运行在主线程中。
class MEmuExecEnv : public AdbExecEnv, public DmExecEnv, public AppExecEnvBase{
public:
	struct stVmItem{
		std::string		vm_name_;
		std::string		vm_uuid_;
		friend bool operator == (const stVmItem& lhs, const stVmItem& rhs);
	};
	typedef std::vector<stVmItem> VmsContT;
	struct VmsOper{
		VmsContT	vms_;

		void swap(VmsOper& rhs);
		bool operator == (const VmsOper& rhs) const;
		bool operator != (const VmsOper& rhs) const;
		bool ExistByName(const std::string& vm_name) const;
		bool ExistByItem(const stVmItem& vm_item) const;
	};
	typedef boost::property_tree::ptree XmlTreeT;
	typedef std::map<std::string, XmlTreeT> XmlTreeContT;
	typedef std::pair<std::string, pt_word> IpInfoT;
	struct stShareDirInfo{
		std::string		dir_music_;
		std::string		dir_movie_;
		std::string		dir_picture_;
		std::string		dir_download_;
	};
	struct stGuestProperty{
		std::string		name_;
		std::string		value_;
	};
	typedef std::vector<stGuestProperty> GuestPropertyContT;

public:
	MEmuExecEnv();
	virtual ~MEmuExecEnv();
	bool SetMEmuPath(const std::string& path);
	const boost::filesystem::path& GetMemuPath() const;
	const boost::filesystem::path& GetMemuHyPath() const;
	bool ExecCmd(const wxString& cmd, wxArrayString& out);
	bool ExecCmd(const wxString& cmd);
	bool ExecManager(const wxString& cmd, wxArrayString& out);
	long ExecAsync(const wxString& cmd);

public:
	long Memu_Create(const wxString& item, const std::string& item_key);
	bool Manager_ListVms(VmsOper& out);
	bool Manager_ListRunningVms(VmsOper& out);
	bool Manager_CloseMenu(const std::string& item_name);
	std::string CreateVm();
	bool ModifyVm(const std::string& vm_name);

public:
	XmlTreeT* GetMemuXmlCfg(const std::string& vm_name);
	//直接保存xml文件不行，会有问题，必须得用MEmuManage提供的方式去修改xml。
	bool SaveMemuXmlCfg(const std::string& vm_name) const;
	IpInfoT GetAdbIpInfo(const std::string& vm_name);
	bool GetSharedDirInfo(const std::string& vm_name, stShareDirInfo& dir_info);
	bool AddGuestProperty(const std::string& name, const std::string& value);

protected:
	virtual bool DoModifyVm(XmlTreeT& pt_machine, const std::string& vm_name);
	bool AdbExecCmd(const wxString& cmd) override;
	bool AdbExecCmd(const wxString& cmd, wxArrayString& out) override;
	long AdbExecAsync(const wxString& cmd) override;
	bool DmExecCmd(const wxString& cmd, wxArrayString& out) override;
	bool DoModify_GuestProperty(XmlTreeT& pt_machine, const std::string& vm_name);
	bool DoModify_SharedDir(XmlTreeT& pt_machine, const std::string& vm_name);

private:
	static std::string SearchSub_Impl(const boost::regex&(*f)(), const std::string& str, char seg);
	static std::string SearchSub_YinHao(const std::string& str, char seg = ' ');
	static std::string SearchSub_DaKuoHao(const std::string& str, char seg = ' ');
	bool Manager_ListVmsImpl(const wxString& cmd, VmsOper& out);
	bool DoModify_PerSharedDir(const std::string& vm_name, const std::string& name, const std::string& hostpath);

private:
	static const boost::regex& RegexPattern_YinHao();
	static const boost::regex& RegexPattern_DaKuoHao();

private:
	boost::filesystem::path		path_memu_;
	boost::filesystem::path		path_memu_hy_;
	boost::filesystem::path		path_memu_vms_;
	wxExecuteEnv				normal_exec_env_;
	XmlTreeContT				memu_vm_trees_;
	GuestPropertyContT			guest_property_;
	stShareDirInfo				share_dir_info_;
};