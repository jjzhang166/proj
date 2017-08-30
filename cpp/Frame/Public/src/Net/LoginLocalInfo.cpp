#include "stdafx.h"
#include <Net/LoginLocalInfo.h>
#include <protect/Protect.h>
#include <Common/Toolhelp.h>
#include <CommonX/CmnX_DllApi.h>
#include <Common/UsefulFunc.h>
#include <Common/LogOut.h>
#include <Net/IoTimer.h>
#include <Common/UsefulClass.h>
#include <wx/wxprec.h>
#include <AssetSys/AS_Sys.h>

BC_REG_CLS_DEFINE(GainLocalInfo){
	bufcall::class_<GainLocalInfo>("GainLocalInfo")
	.def("OGMC", &GainLocalInfo::OnGetMachineCode)
	.def("OGC", &GainLocalInfo::OnGetCookie)
	.def("OSC", &GainLocalInfo::OnSetCookie);
}

void GainLocalInfo::OnGetMachineCode(const MachineCodeT& machine_code)
{
	VMP_BEGIN;
	if (get_machine_code_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("ureiqajdfk");
	}
	strcpy_s(str_tmp_, machine_code.value_);
	get_machine_code_res_ = true;
	VMP_END;
}

void GainLocalInfo::OnGetCookie(const CookieT& cookie)
{
	VMP_BEGIN;
	if (get_cookie_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("jruiwqj");
	}
	strcpy_s(str_tmp_, cookie.value_);
	get_cookie_res_ = true;
	VMP_END;
}

void GainLocalInfo::OnSetCookie()
{
	VMP_BEGIN;
	if (set_cookie_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("fjdkals");
	}
	set_cookie_res_ = true;
	VMP_END;
}

bool GainLocalInfo::InitSelf()
{
	VMP_BEGIN_EX;
	bool is_err = false;
	if (!PrepareTo(kLoginLocalInfo_FileMapNameS))
		is_err = true;
	if (!is_err)
	{
		if (!IsValid() && !CreateFileMap(self_name_))
			is_err = true;
	}

	if (is_err)
	{
		if (getter_ == &local_getter_)
			return true;
		assert(false);
		GetIoService().post([](){
			MessageBoxTimeout(nullptr, VMP_PROTECT_STR_A("发生错误了，需要重启电脑"), nullptr, MB_OK, 0, 1000 * 10);
			RestartSystem();
		});
		return false;
	}
	return true;
	VMP_END;
}

GainLocalInfo::GainLocalInfo() : remote_getter_(*this)
{
	get_machine_code_res_ = false;
	get_cookie_res_ = false;
	set_cookie_res_ = false;
	idle_do_ = &wait_do_io_;
	getter_ = &remote_getter_;
}

std::string GainLocalInfo::GetMachineCode()
{
	VMP_BEGIN;
	assert(getter_);
	return getter_->GetMachineCode();
	VMP_END;
}

std::string GainLocalInfo::GetCookie(const std::string& whose)
{
	VMP_BEGIN;
	assert(getter_);
	return getter_->GetCookie(whose);
	VMP_END;
}

void GainLocalInfo::SetCookie(const std::string& whose, const std::string& cookie)
{
	VMP_BEGIN;
	assert(getter_);
	return getter_->SetCookie(whose, cookie);
	VMP_END;
}

bool GainLocalInfo::PrepareTo(const std::string& to_name)
{
	if (!to_.IsValid() && !to_.OpenFileMap(to_name))
	{
		//assert(false);
		return false;
	}
	return true;
}

void GainLocalInfo::TimerCheck()
{
	VMP_BEGIN_EX;
	static bool init_once = false;
	if (init_once)
		return;
	init_once = true;
	//////////////////////////////////////////////////////////////////////////
	/////复制DLL文件
	char szbuffer[MAX_PATH] = { 0 };
	GetSystemDirectoryA(szbuffer, sizeof(szbuffer));
	boost::filesystem::path path_sys = szbuffer;
	if (!boost::filesystem::is_directory(path_sys))
	{
		assert(false);
		throw CmnExitIoThreadException("dsfafdqwre");
	}
	path_sys /= "msgtcoreeapi.dll";
	auto str_path_sys = path_sys.string();
	bool need_cpy = true;

	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		throw CmnExitIoThreadException("dsfafdqwre1");
	}
	node = node->FindChild("LoginHlp");
	if (!node)
	{
		assert(false);
		throw CmnExitIoThreadException("dsfafdqwre2");
	}
	const auto& str_login_help = node->GetString();
	if (str_login_help.empty())
	{
		assert(false);
		throw CmnExitIoThreadException("dsfafdqwre3");
	}

	if (boost::filesystem::exists(path_sys))
	{
		const auto& rc_uuid = GenUuidStr(str_login_help);
		const auto& old_uuid = GenUuidStrOfFile(str_path_sys);
		if (rc_uuid == old_uuid)
			need_cpy = false;
		else
		{
			boost::system::error_code err;
			boost::filesystem::remove_all(path_sys, err);
			if (err)
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "删除文件出错,目标文件：" << path_sys << " 错误信息：" << err.message();
				assert(false);
				GetIoService().post([](){
					MessageBoxTimeout(nullptr, VMP_PROTECT_STR_A("您的电脑需要重启"), nullptr, MB_OK, 0, 1000 * 5);
					RestartSystem();
					throw CmnExitIoThreadException("fdjqpi");
				});
				return;
			}
		}
	}
	if (need_cpy)
	{
		std::ofstream ostm(str_path_sys, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (!ostm)
		{
			assert(false);
			throw CmnExitIoThreadException("fdjqpidfda");
		}
		ostm.write(str_login_help.c_str(), str_login_help.size());
	}
	//////////////////////////////////////////////////////////////////////////
	auto fn = [=](IoTimer* timer){
		if (timer)
			timer->Continue();
		auto& local_info = GainLocalInfo::GetMe(false);
		auto file_map_handle = local_info.to_.GetHandle();
		static int err_cnt = 0;
		if (local_info.PrepareTo(kLoginLocalInfo_FileMapNameS))
			err_cnt = 0;
		else
		{
			if (local_info.getter_ == &local_info.remote_getter_)
				++err_cnt;
		}
		if (err_cnt > 5)
		{
			assert(false);
			GetIoService().post([](){
				MessageBoxTimeout(nullptr, "发生了错误，需要重启电脑", nullptr, MB_OK, 0, 1000 * 10);
				RestartSystem();
			});
			return;
		}

		//保证有两个ser是被注入的
		if (GetNtObjectHandleCount(file_map_handle) >= 3)
			return;

		int cnt = 0;
		auto cur_process_id = GetCurrentProcessId();
		//CToolhelp::DoProcessIdUntilAsName(CPath(_T("test.exe")),
		CToolhelp::DoAllProcessIdUntil(
			[str_path_sys, &cnt, cur_process_id](DWORD process_id){
			if (process_id == cur_process_id)
				return false;
			cnt += !!InjectLibA(process_id, str_path_sys.c_str());
			return cnt >= 2;
		});
		if (cnt == 0)
			local_info.getter_ = &local_info.local_getter_;
		else
			local_info.getter_ = &local_info.remote_getter_;
	};

	SetIoTimer(1000 * 5, fn);

	if (!GainLocalInfo::GetMe(false).PrepareTo(kLoginLocalInfo_FileMapNameS))
		fn(nullptr);
	VMP_END;
}

GainLocalInfo& GainLocalInfo::GetMe(bool do_init)
{
	static GainLocalInfo me;
	if (do_init)
		me.InitSelf();
	return me;
}

void FileMapDoIoAtIdle::Do()
{
	GetIoService().Poll();
}

std::string GainLocalInfo::LocalGetter::GetMachineCode()
{
	VMP_BEGIN;
	return LocalInfo::GetTheMachineCode();
	VMP_END;
}

std::string GainLocalInfo::LocalGetter::GetCookie(const std::string& whose)
{
	VMP_BEGIN;
	return LocalInfo::GetTheCookie(GenUuidStr(whose));
	VMP_END;
}

void GainLocalInfo::LocalGetter::SetCookie(const std::string& whose, const std::string& cookie)
{
	VMP_BEGIN;
	LocalInfo::SetNewCookie(whose, cookie);
	VMP_END;
}

GainLocalInfo::LocalGetterRemote::LocalGetterRemote(GainLocalInfo& parent) : parent_(parent)
{

}

std::string GainLocalInfo::LocalGetterRemote::GetMachineCode()
{
	VMP_BEGIN;
	parent_.ToCallSync(BC_MakeP("GMC") << parent_.self_name_, parent_.get_machine_code_res_);
	std::string res(parent_.str_tmp_, kMachineCodeLen);
	std::memset(parent_.str_tmp_, 0, sizeof(parent_.str_tmp_));
	return res;
	VMP_END;
}

std::string GainLocalInfo::LocalGetterRemote::GetCookie(const std::string& whose)
{
	VMP_BEGIN;
	parent_.ToCallSync(BC_MakeP("GC") << parent_.self_name_ << whose, parent_.get_cookie_res_);
	std::string res(parent_.str_tmp_, kCookieStrLen);
	std::memset(parent_.str_tmp_, 0, sizeof(parent_.str_tmp_));
	return res;
	VMP_END;
}

void GainLocalInfo::LocalGetterRemote::SetCookie(const std::string& whose, const std::string& cookie)
{
	VMP_BEGIN;
	parent_.ToCallSync(BC_MakeP("SC") << parent_.self_name_ << whose << cookie, parent_.set_cookie_res_);
	VMP_END;
}
