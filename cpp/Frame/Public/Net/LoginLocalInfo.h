#pragma once
/*
@author zhp
@date 2016/5/10 13:47
@purpose 登陆时的本地信息
*/
#include "Bufcall_FileMap.h"

class FileMapDoIoAtIdle : public FileMapDoAtIdle{
	void Do() override;
};

class GainLocalInfo : public FileMapSocketBaseC<BufCall_FileMapExcept>{
public:
	typedef BC_CHECK_SIZE(pt_csz, kMachineCodeLen, kMachineCodeLen) MachineCodeT;
	typedef BC_CHECK_SIZE(pt_csz, kCookieStrLen, 0) CookieT;

	class LocalGetter{
	public:
		virtual std::string GetMachineCode();
		virtual std::string GetCookie(const std::string& whose);
		virtual void SetCookie(const std::string& whose, const std::string& cookie);
	};
	class LocalGetterRemote : public LocalGetter{
	public:
		LocalGetterRemote(GainLocalInfo& parent);
		std::string GetMachineCode() override;
		std::string GetCookie(const std::string& whose) override;
		void SetCookie(const std::string& whose, const std::string& cookie) override;

	private:
		GainLocalInfo&		parent_;
	};

public:
	GainLocalInfo();
	GainLocalInfo(const GainLocalInfo&) = delete;
	GainLocalInfo& operator =(const GainLocalInfo&) = delete;
	std::string GetMachineCode();
	std::string GetCookie(const std::string& whose);
	void SetCookie(const std::string& whose, const std::string& cookie);

public:
	//程序启动的时候调用
	static void TimerCheck();
	static GainLocalInfo& GetMe(bool do_init = true);

private:
	void OnGetMachineCode(const MachineCodeT& machine_code);
	void OnGetCookie(const CookieT& cookie);
	void OnSetCookie();

private:
	bool PrepareTo(const std::string& to_name) override;
	bool InitSelf() override;

private:
	OptionalBool		get_machine_code_res_;
	OptionalBool		get_cookie_res_;
	OptionalBool		set_cookie_res_;

private:
	char				str_tmp_[kUuidStrLen + 1];
	FileMapDoIoAtIdle	wait_do_io_;
	LocalGetter*		getter_;
	LocalGetter			local_getter_;
	LocalGetterRemote	remote_getter_;

	BC_REG_CLS_DECLARE(GainLocalInfo);
};