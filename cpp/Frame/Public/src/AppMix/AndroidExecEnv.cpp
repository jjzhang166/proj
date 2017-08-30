#include "stdafx.h"
#include <AppMix/AndroidExecEnv.h>
#include <boost/filesystem.hpp>
#include <CmnMix/Cmn_LogOut.h>
#include <boost/property_tree/xml_parser.hpp>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <AppBase/Cmn_App.h>
#include <Common/UsefulClass.h>

static bool ExistThePath_(const wxString& str, const boost::filesystem::path& sub_str){
	auto& native_sub_str = sub_str.native();
	auto npos = str.find(native_sub_str); 
	if (npos == wxString::npos)
		return false;
	auto size = native_sub_str.size();
	return str.c_str()[npos + size] == wxT(';');
}

bool MEmuExecEnv::SetMEmuPath(const std::string& path)
{
	using namespace boost::filesystem;
	boost::filesystem::path p(path);
	if (!exists(p))
	{
		assert(false);
		return false;
	}
	path_memu_ = p.parent_path();
	path_memu_hy_ = path_memu_.parent_path() / "MEmuHyperv";
	if (!exists(path_memu_hy_))
	{
		assert(false);
		return false;
	}
	path_memu_vms_ = path_memu_ / "MemuHyperv VMs";
	if (!exists(path_memu_vms_))
	{
		assert(false);
		return false;
	}
	wxString str_path_old;
	wxString str_path(wxT("PATH"));
	if (!wxGetEnv(str_path, &str_path_old))
	{
		assert(false);
		return false;
	}
	wxString path_mix;
	if (!ExistThePath_(str_path_old, path_memu_))
	{
		path_mix = path_memu_.native();
		path_mix += wxT(";");
	}
	if (!ExistThePath_(str_path_old, path_memu_hy_))
	{
		path_mix += path_memu_hy_.native();
		path_mix += wxT(";");
	}
	if (!path_mix.empty())
	{
		path_mix += str_path_old;
		if (!wxSetEnv(str_path, path_mix))
		{
			assert(false);
			return false;
		}
	}
	
	if (!Init())
	{
		assert(false);
		return false;
	}
	normal_exec_env_.cwd = path_memu_.native();
	//////////////////////////////////////////////////////////////////////////
	const auto& path_config = path_memu_ / "config.ini";
	IniProfile ini_file(path_config.string());
	std::string closeConfirm("true");
	ini_file.Read("preference", "closeConfirm", closeConfirm, closeConfirm);
	if (closeConfirm == "true")
		ini_file.Write("preference", "closeConfirm", "false");
	return true;
}

bool MEmuExecEnv::ExecManager(const wxString& cmd, wxArrayString& out)
{
	return ExecCmd(wxT("MEmuManage ") + cmd, out);
}

bool MEmuExecEnv::ExecCmd(const wxString& cmd, wxArrayString& out)
{
	if (0 != wxExecute(cmd, out, wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE, &normal_exec_env_))
	{
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::ExecCmd(const wxString& cmd)
{
	if (0 != wxExecute(cmd, wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE, nullptr, &normal_exec_env_))
	{
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::Manager_ListVms(VmsOper& out)
{
	return Manager_ListVmsImpl(wxT("list vms"), out);
}

const boost::regex& MEmuExecEnv::RegexPattern_YinHao()
{
	static boost::regex reg(R"("\S+")", boost::regex::perl);
	return reg;
}

const boost::regex& MEmuExecEnv::RegexPattern_DaKuoHao()
{
	static boost::regex reg(R"(\{\S+\})", boost::regex::perl);
	return reg;
}

std::string MEmuExecEnv::SearchSub_YinHao(const std::string& str, char seg)
{
	return SearchSub_Impl(RegexPattern_YinHao, str, seg);
}

std::string MEmuExecEnv::SearchSub_Impl(const boost::regex&(*f)(), const std::string& str, char seg)
{
	boost::smatch what;
	std::string str_res;
	if (boost::regex_search(str, what, f()))
	{
		for (auto& res : what)
		{
			if (res.second - res.first >= 2)
			{
				if (!str_res.empty())
					str_res.append(seg, 1);
				str_res.append(res.first + 1, res.second - 1);
			}
		}
	}
	return str_res;
}

std::string MEmuExecEnv::SearchSub_DaKuoHao(const std::string& str, char seg)
{
	return SearchSub_Impl(RegexPattern_DaKuoHao, str, seg);
}

bool MEmuExecEnv::Manager_ListRunningVms(VmsOper& out)
{
	return Manager_ListVmsImpl(wxT("list runningvms"), out);
}

long MEmuExecEnv::ExecAsync(const wxString& cmd)
{
	auto pid = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_NOEVENTS | wxEXEC_HIDE_CONSOLE, nullptr, &normal_exec_env_);
	assert(pid != 0);
	return pid;
}

long MEmuExecEnv::Memu_Create(const wxString& item, const std::string& item_key)
{
	std::string file_path("MEmu ");
	file_path += item;
	return EnvCreateProcess(file_path, item_key);
}

const boost::filesystem::path& MEmuExecEnv::GetMemuPath() const
{
	return path_memu_;
}

const boost::filesystem::path& MEmuExecEnv::GetMemuHyPath() const
{
	return path_memu_hy_;
}

bool MEmuExecEnv::Manager_CloseMenu(const std::string& item_name)
{
	wxString cmd(wxT("MEmuManage controlvm "));
	cmd.append(item_name);
	cmd.append(wxT(" poweroff"));
	return 0 != ExecAsync(cmd);
}

bool MEmuExecEnv::Manager_ListVmsImpl(const wxString& cmd, VmsOper& out)
{
	wxArrayString str_out;
	if (!ExecManager(cmd, str_out))
		return false;
	if (str_out.empty())
	{
		out.vms_.clear();
		return true;
	}
	stVmItem tmp;
	std::string str_tmp;
	for (auto& v : str_out)
	{
		if (v.empty())
			continue;
		str_tmp = v;
		tmp.vm_name_ = SearchSub_YinHao(str_tmp);
		if (tmp.vm_name_.empty())
			continue;
		if (tmp.vm_name_ == "<inaccessible>")
			continue;
		tmp.vm_uuid_ = SearchSub_DaKuoHao(str_tmp);
		if (tmp.vm_uuid_.empty())
			continue;
		out.vms_.push_back(tmp);		
	}
	return true;
}

std::string MEmuExecEnv::CreateVm()
{
	/*if (0 != wxExecute(wxT("MEmuManage import MEmu.ova --dry-run"), wxEXEC_BLOCK | wxEXEC_HIDE_CONSOLE, nullptr, &normal_exec_env_))
	{
		assert(false);
		return kEmptyStr;
	}*/
	wxArrayString out;
	if (!ExecManager(wxT("import MEmu.ova"), out))
	{
		assert(false);
		return kEmptyStr;
	}
	if (out.empty())
	{
		assert(false);
		return kEmptyStr;
	}
	std::string str_out;
	for (auto& v : out)
	{
		str_out += v;
	}
	if (str_out.find("Successfully imported the appliance.") == str_out.npos)
	{
		assert(false);
		return kEmptyStr;
	}
	static const std::string kTheNameTag("Suggested VM name \"");
	auto n_pos = str_out.find(kTheNameTag);
	if (n_pos == str_out.npos)
	{
		assert(false);
		return kEmptyStr;
	}
	auto the_name_begin = str_out.c_str() + n_pos + kTheNameTag.size();
	auto the_name_end = strstr(the_name_begin, "\"");
	if (!the_name_end)
	{
		assert(false);
		return kEmptyStr;
	}
	std::string new_memu(the_name_begin, the_name_end);
	if (!ModifyVm(new_memu))
	{
		assert(false);
		return new_memu;
	}
	return new_memu;
}

bool MEmuExecEnv::ModifyVm(const std::string& vm_name)
{
	if (memu_vm_trees_.find(vm_name) != memu_vm_trees_.end())
		return true;
	auto pt = GetMemuXmlCfg(vm_name);
	if (!pt)
	{
		assert(false);
		return false;
	}
	auto pt_machine = pt->get_child_optional("MemuHyperv.Machine");
	if (!pt_machine)
	{
		assert(false);
		return false;
	}
	if (!DoModifyVm(*pt_machine, vm_name))
		return true;
	return true;
	//return SaveMemuXmlCfg(vm_name);
}

MEmuExecEnv::~MEmuExecEnv()
{

}

MEmuExecEnv::XmlTreeT* MEmuExecEnv::GetMemuXmlCfg(const std::string& vm_name)
{
	using namespace boost::filesystem;
	using namespace boost::property_tree;
	if (vm_name.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = memu_vm_trees_.find(vm_name);
	if (it != memu_vm_trees_.end())
		return &it->second;
	auto path_cfg = path_memu_vms_ / vm_name;
	if (!exists(path_cfg))
	{
		assert(false);
		return nullptr;
	}
	path_cfg /= vm_name;
	path_cfg += ".memu";
	if (!exists(path_cfg))
	{
		assert(false);
		return nullptr;
	}
	XmlTreeT pt;
	try
	{
		read_xml(path_cfg.string(), pt);
	}
	catch (std::exception& e)
	{
		LOG_O(Log_debug) << e.what();
		assert(false);
		return nullptr;
	}
	auto& real_pt = memu_vm_trees_[vm_name];
	real_pt.swap(pt);
	return &real_pt;
}

bool MEmuExecEnv::SaveMemuXmlCfg(const std::string& vm_name) const
{
	//直接保存xml文件不行，会有问题，必须得用MEmuManage提供的方式去修改xml。
	assert(false);
	return false;
	auto it = memu_vm_trees_.find(vm_name);
	if (it == memu_vm_trees_.end())
	{
		assert(false);
		return false;
	}
	auto path_cfg = path_memu_vms_ / vm_name;
	path_cfg /= vm_name;
	path_cfg += ".memu";
	if (!exists(path_cfg))
	{
		assert(false);
		return false;
	}
	try
	{
		write_xml(path_cfg.string(), it->second);
	}
	catch (std::exception& e)
	{
		LOG_O(Log_debug) << e.what();
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::DoModifyVm(XmlTreeT& pt_machine, const std::string& vm_name)
{
	if (!DoModify_GuestProperty(pt_machine, vm_name))
	{
		assert(false);
		return false;
	}
	if (!DoModify_SharedDir(pt_machine, vm_name))
	{
		assert(false);
		return false;
	}
	return true;
}

MEmuExecEnv::IpInfoT MEmuExecEnv::GetAdbIpInfo(const std::string& vm_name)
{
	IpInfoT ip_info;
	auto pt = GetMemuXmlCfg(vm_name);
	if (!pt)
	{
		assert(false);
		return ip_info;
	}
	auto pt_nat = pt->get_child_optional("MemuHyperv.Machine.Hardware.Network.Adapter.NAT");
	if (!pt_nat)
	{
		assert(false);
		return ip_info;
	}
	for (auto& v : *pt_nat)
	{
		if (v.first != "Forwarding")
			continue;
		const auto& pt_name = v.second.get_optional<std::string>("<xmlattr>.name");
		if (!pt_name)
		{
			assert(false);
			continue;
		}
		if (*pt_name != "ADB")
			continue;
		const auto& hostport = v.second.get_optional<pt_word>("<xmlattr>.hostport");
		if (!hostport)
		{
			assert(false);
			continue;
		}
		const auto& hostip = v.second.get_optional<std::string>("<xmlattr>.hostip");
		if (!hostip)
		{
			assert(false);
			continue;
		}
		ip_info.first = *hostip;
		ip_info.second = *hostport;
		return ip_info;
	}
	assert(false);
	return ip_info;
}

bool AdbExecEnv::AdbHasTheDeviceInfo(const std::string& device_info)
{
	static const wxString cmd(wxT("adb devices"));
	wxArrayString out;
	if (!AdbExecCmd(cmd, out))
	{
		assert(false);
		return false;
	}
	for (auto& v : out)
	{
		if (v.find(device_info) != v.npos)
			return true;
	}
	return false;
}

bool AdbExecEnv::AdbStartServer(bool is_async)
{
	if (is_async)
		return AdbExecAsync(wxT("adb start-server")) != 0;
	else
		return AdbExecCmd(wxT("adb start-server"));
}

int AdbExecEnv::AdbListPackages(const std::string& device, StringContT& packages)
{
	wxArrayString out;
	//shell pm list packages -f  会把文件夹也列出来。
	if (!AdbExec(device, wxT("shell pm list packages"), out))
	{
		assert(false);
		return 0;
	}
	static const wxString kPackage(wxT("package:"));
	auto cnt = 0;
	wxString str_tmp;
	for (auto& v : out)
	{
		auto n_pos = v.find(kPackage);
		if (n_pos == v.npos)
			continue;
		auto size = v.size();
		n_pos += kPackage.size();
		if (n_pos >= size)
		{
			assert(false);
			continue;
		}
		auto cstr = v.c_str();
		str_tmp.assign(cstr + n_pos, cstr + size);
		packages.push_back(kEmptyStr);
		auto& back = packages.back();
		back = str_tmp;
		++cnt;
	}
	assert(cnt > 0);
	return cnt;
}

bool AdbExecEnv::AdbInstallApk(const std::string& device, const std::string& apk_path)
{
	if (apk_path.empty())
	{
		assert(false);
		return false;
	}
	wxArrayString out;
	wxString cmd(wxT("install \""));
	cmd += apk_path;
	cmd += wxT("\"");
	if (!AdbExec(device, cmd, out))
	{
		assert(false);
		return false;
	}
	for (auto& v : out)
	{
		if (v == wxT("Success"))
			return true;
	}
	assert(false);
	return false;
}

bool AdbExecEnv::AdbPmInstallApk(const std::string& device, const std::string& apk_name)
{
	if (apk_name.empty())
	{
		assert(false);
		return false;
	}
	wxArrayString out;
	wxString cmd(wxT("shell pm install /sdcard/Download/"));
	cmd += apk_name;
	if (!AdbExec(device, cmd, out))
	{
		assert(false);
		return false;
	}
	for (auto& v : out)
	{
		if (v == wxT("Success"))
			return true;
	}
	assert(false);
	return false;
}

bool AdbExecEnv::AdbStartApp(const std::string& device, const std::string& package_name, const std::string& activity_name)
{
	if (package_name.empty() || activity_name.empty())
	{
		assert(false);
		return false;
	}
	wxArrayString out;
	wxString cmd(wxT("shell am start -n "));
	cmd += package_name;
	cmd += wxT('/');
	cmd += activity_name;
	if (!AdbExec(device, cmd, out))
	{
		assert(false);
		return false;
	}
	for (auto& v : out)
	{
		if (v.find(wxT("Error:")) != v.npos)
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool AdbExecEnv::AdbExec(const std::string& device, const wxChar* cmd, wxArrayString& out)
{
	if (!cmd || device.empty())
	{
		assert(false);
		return false;
	}
	wxString the_cmd(wxT("adb -s "));
	the_cmd += device;
	the_cmd += wxT(" ");
	the_cmd += cmd;
	return AdbExecCmd(the_cmd, out);
}

bool AdbExecEnv::AdbUnInstallApk(const std::string& device, const std::string& package_name)
{
	if (package_name.empty())
	{
		assert(false);
		return false;
	}
	wxArrayString out;
	wxString cmd(wxT("shell pm uninstall "));
	cmd += package_name;
	if (!AdbExec(device, cmd, out))
	{
		assert(false);
		return false;
	}
	for (auto& v : out)
	{
		if (v == wxT("Success"))
			return true;
	}
	assert(false);
	return false;
}

bool AdbExecEnv::AdbStopApp(const std::string& device, const std::string& package_name)
{
	if (package_name.empty())
	{
		assert(false);
		return false;
	}
	wxArrayString out;
	wxString cmd(wxT("shell am force-stop "));
	cmd += package_name;
	if (!AdbExec(device, cmd, out))
	{
		assert(false);
		return false;
	}
	return true;
}

AdbExecEnv::~AdbExecEnv()
{

}

bool AdbExecEnv::AdbConnect(const std::string& device)
{
	wxString cmd(wxT("adb connect "));
	cmd += device;
	if (!AdbExecCmd(cmd))
	{
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::GetSharedDirInfo(const std::string& vm_name, stShareDirInfo& dir_info)
{
	IpInfoT ip_info;
	auto pt = GetMemuXmlCfg(vm_name);
	if (!pt)
	{
		assert(false);
		return false;
	}
	auto pt_folders = pt->get_child_optional("MemuHyperv.Machine.Hardware.SharedFolders");
	if (!pt_folders)
	{
		assert(false);
		return false;
	}
	int cnt = 0;
	for (auto& v : *pt_folders)
	{
		if (v.first != "SharedFolder")
		{
			assert(false);
			continue;
		}
		const auto& pt_name = v.second.get_optional<std::string>("<xmlattr>.name");
		if (!pt_name)
		{
			assert(false);
			continue;
		}
		const auto& host_path = v.second.get_optional<std::string>("<xmlattr>.hostPath");
		if (!host_path)
		{
			assert(false);
			continue;
		}
		const auto& the_host_path = *host_path;
		const auto& the_name = *pt_name;
		if (the_name == "music")
		{
			dir_info.dir_music_ = Utf8ToGbk(the_host_path.c_str());
			++cnt;
		}
		else if (the_name == "movie")
		{
			dir_info.dir_movie_ = Utf8ToGbk(the_host_path.c_str());
			++cnt;
		}
		else if (the_name == "picture")
		{
			dir_info.dir_picture_ = Utf8ToGbk(the_host_path.c_str());
			++cnt;
		}
		else if (the_name == "download")
		{
			dir_info.dir_download_ = Utf8ToGbk(the_host_path.c_str());
			++cnt;
		}
		else
		{
			assert(false);
		}
	}
	if (4 != cnt)
	{
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::AdbExecCmd(const wxString& cmd, wxArrayString& out)
{
	return ExecCmd(cmd, out);
}

bool MEmuExecEnv::AdbExecCmd(const wxString& cmd)
{
	return ExecCmd(cmd);
}

long MEmuExecEnv::AdbExecAsync(const wxString& cmd)
{
	return ExecAsync(cmd);
}

bool MEmuExecEnv::DmExecCmd(const wxString& cmd, wxArrayString& out)
{
	return ExecCmd(cmd, out);
}

bool MEmuExecEnv::DoModify_GuestProperty(XmlTreeT& pt_machine, const std::string& vm_name)
{
	auto op_pt = pt_machine.get_child_optional("Hardware.GuestProperties");
	if (!op_pt)
	{
		assert(false);
		return false;
	}
	std::vector<const stGuestProperty*> modified;
	for (auto& v : *op_pt)
	{
		if (v.first != "GuestProperty")
			continue;
		const auto& pt_name = v.second.get_optional<std::string>("<xmlattr>.name");
		if (!pt_name)
		{
			assert(false);
			continue;
		}
		const auto& the_name = *pt_name;
		for (auto& item : guest_property_)
		{
			if (the_name == item.name_)
			{
				auto& op_value = v.second.get_optional<std::string>("<xmlattr>.value");
				if (!op_value)
				{
					assert(false);
					return false;
				}
				if (*op_value != item.value_)
				{
					wxString cmd(wxT("MEmuManage guestproperty set \""));
					cmd += vm_name;
					cmd += wxT("\" \"");
					cmd += the_name;
					cmd += wxT("\" \"");
					cmd += item.value_;
					cmd += wxT('\"');
					if (!ExecCmd(cmd))
					{
						assert(false);
						return false;
					}
				}
				modified.push_back(&item);
			}
		}
	}
	if (modified.size() == guest_property_.size())
		return true;
	for (auto& item : guest_property_)
	{
		for (auto v : modified)
		{
			if (v == &item)
				goto LABLE_HEAR;
		}
		goto LABLE_DO;
	LABLE_HEAR:
		continue;
	LABLE_DO:
		wxString cmd(wxT("MEmuManage guestproperty set \""));
		cmd += vm_name;
		cmd += wxT("\" \"");
		cmd += item.name_;
		cmd += wxT("\" \"");
		cmd += item.value_;
		cmd += wxT('\"');
		if (!ExecCmd(cmd))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

MEmuExecEnv::MEmuExecEnv()
{
	create_flags_ |= wxEXEC_HIDE_CONSOLE;
	AddGuestProperty("is_customed_resolution", "1");		//必须加上这个属性，否则resolution_height、resolution_width等将不起作用。
	AddGuestProperty("resolution_height", "480");
	AddGuestProperty("resolution_width", "800");
	AddGuestProperty("vbox_dpi", "192");
}

bool MEmuExecEnv::AddGuestProperty(const std::string& name, const std::string& value)
{
	for (auto& v : guest_property_)
	{
		if (name == v.name_)
		{
			assert(false);
			return false;
		}
	}
	guest_property_.push_back(stGuestProperty());
	auto& item = guest_property_.back();
	item.name_ = name;
	item.value_ = value;
	return true;
}

bool MEmuExecEnv::DoModify_SharedDir(XmlTreeT& pt_machine, const std::string& vm_name)
{
	auto pt_folders = pt_machine.get_child_optional("Hardware.SharedFolders");
	if (!pt_folders)
	{
		assert(false);
		return false;
	}
	if (!pt_folders->empty())
		return true;
	if (share_dir_info_.dir_download_.empty())
	{
		wxString str_tmp;
		if (!wxGetEnv(wxT("USERPROFILE"), &str_tmp))
		{
			assert(false);
			return false;
		}
		boost::filesystem::path path(str_tmp);
		share_dir_info_.dir_download_ = (path / "Downloads" / "逍遥安卓下载").string();
		share_dir_info_.dir_movie_ = (path / "Videos" / "逍遥安卓视频").string();
		share_dir_info_.dir_music_ = (path / "Music" / "逍遥安卓音乐").string();
		share_dir_info_.dir_picture_ = (path / "Pictures" / "逍遥安卓照片").string();
	}
	if (!DoModify_PerSharedDir(vm_name, "download", share_dir_info_.dir_download_))
	{
		assert(false);
		return false;
	}
	if (!DoModify_PerSharedDir(vm_name, "picture", share_dir_info_.dir_picture_))
	{
		assert(false);
		return false;
	}
	if (!DoModify_PerSharedDir(vm_name, "movie", share_dir_info_.dir_movie_))
	{
		assert(false);
		return false;
	}
	if (!DoModify_PerSharedDir(vm_name, "music", share_dir_info_.dir_music_))
	{
		assert(false);
		return false;
	}
	return true;
}

bool MEmuExecEnv::DoModify_PerSharedDir(const std::string& vm_name, const std::string& name, const std::string& hostpath)
{
	wxString cmd(wxT("MEmuManage sharedfolder add \""));
	cmd += vm_name;
	cmd += wxT("\" --name \"");
	cmd += name;
	cmd += wxT("\" --hostpath \"");
	cmd += hostpath;
	cmd += wxT('\"');
	if (!ExecCmd(cmd))
	{
		assert(false);
		return false;
	}
	return true;
}

bool operator == (const MEmuExecEnv::stVmItem& lhs, const MEmuExecEnv::stVmItem& rhs){
	return lhs.vm_name_ == rhs.vm_name_ && lhs.vm_uuid_ == rhs.vm_uuid_;
}

void MEmuExecEnv::VmsOper::swap(VmsOper& rhs)
{
	vms_.swap(rhs.vms_);
}

bool MEmuExecEnv::VmsOper::operator==(const VmsOper& rhs) const
{
	if (vms_.size() != rhs.vms_.size())
		return false;
	return std::equal(vms_.begin(), vms_.end(), rhs.vms_.begin());
}

bool MEmuExecEnv::VmsOper::operator!=(const VmsOper& rhs) const
{
	return !(*this == rhs);
}

bool MEmuExecEnv::VmsOper::ExistByName(const std::string& vm_name) const
{
	if (vm_name.empty())
	{
		assert(false);
		return false;
	}
	for (auto& v : vms_)
	{
		if (v.vm_name_ == vm_name)
			return true;
	}
	return false;
}

bool MEmuExecEnv::VmsOper::ExistByItem(const stVmItem& vm_item) const
{
	return std::find(vms_.begin(), vms_.end(), vm_item) != vms_.end();
}

DmExecEnv::~DmExecEnv()
{

}

bool DmExecEnv::DmRegsvr32()
{
	auto& app = Cmn_AppBase::GetAppBase();
	auto node = app.GetAssetDir();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->FindPosteritySmart("dm.dll");
	if (!node)
	{
		assert(false);
		return false;
	}
	wxString cmd(wxT("regsvr32 /s \""));
	cmd += node->GetAbsolutePath_().native();
	cmd += wxT('\"');
	wxArrayString out;
	if (!DmExecCmd(cmd, out))
	{
		assert(false);
		return false;
	}
	return true;
}
