#include "stdafx.h"
#include <CmnAntiCheck/CmnRbuThread.h>
#include <CmnMix/Cmn_EnsureCleanup.h>
#include <CmnMix/Cmn_LogOut.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <CmnMix/Cmn_Toolhelp.h>

void HandleRubThread::AddThrdId(pt_dword thrd_id)
{
	ThrdHandleAndId tmp(thrd_id);
	if (!tmp.GetHandle())
	{
		assert(false);
		return;
	}
	Cmnx_Lockable lock(thrd_id_mutex_);
	thrd_ids_.insert(tmp);
}

void HandleRubThread::EraseThrdId(pt_dword thrd_id)
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	for (auto it = thrd_ids_.begin(); it != thrd_ids_.end(); ++it)
	{
		if (it->GetId() == thrd_id)
		{
			const_cast<ThrdHandleAndId&>(*it).CloseHandle();
			thrd_ids_.erase(it);
		}
	}
}

void __DoThreadSleepImpl___(pt_qword sleep_time){
	const auto kTime = 1000 * 60;
	const pt_dword kEveryTime = sleep_time > kTime ? kTime : 50;
#ifdef _DEBUG
	auto thrd_addr = GetThreadStartAddr(GetCurrentThreadId());
	auto the_module = ModuleFromAddress(thrd_addr);
	char buffer[MAX_PATH];
	buffer[0] = 0;
	GetModuleName(the_module, buffer, MAX_PATH);
	LOG_O(Log_trace) << "__DoThreadSleepImpl___,线程ID:" << GetCurrentThreadId() << ",ThrdAddr:" << thrd_addr << ",module:" << buffer;
#endif
	for (pt_qword cur = 0; cur < sleep_time; cur += kEveryTime)
	{
		::Sleep(kEveryTime);
	}
#ifdef _DEBUG
	LOG_O(Log_trace) << "__DoThreadSleepImpl___结束,线程ID:" << GetCurrentThreadId() << ",ThrdAddr:" << thrd_addr << ",module:" << buffer;
#endif
	assert(false);
}
pc_naked void _DoThrdSleepImpl__(pt_qword sleep_time){
	__asm{
		push ebp;
		mov ebp, esp;
		pushad;
		pushfd;
	}
	__DoThreadSleepImpl___(sleep_time);
	__asm{
		popfd;
		popad;
		mov esp, ebp;
		pop ebp;
		ret 8;
	}
}
int HandleRubThread::DoThrdSleep(pt_qword sleep_time /*= (pt_qword)-1*/)
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	//std::vector<pt_dword> succeed_ids, fail_ids;
	int cnt = 0;
	for (auto& v : thrd_ids_)
	{
		auto hthrd = v.GetHandle();
		assert(hthrd);
		/*BOOST_SCOPE_EXIT_ALL(&v){
			const_cast<ThrdHandleAndId&>(v).Discard();
		};*/
		if (-1 != SuspendThread(hthrd))
		{
			CONTEXT ct = { 0 };
			ct.ContextFlags = CONTEXT_CONTROL;
			if (GetThreadContext(hthrd, &ct))
			{
				*(pt_qword*)(ct.Esp - sizeof(pt_qword)) = sleep_time;
				*(pt_dword*)(ct.Esp - sizeof(pt_qword) - sizeof(pt_dword)) = ct.Eip;
				ct.Esp -= sizeof(pt_qword) + sizeof(pt_dword);
				ct.Eip = (DWORD)(const void*)_DoThrdSleepImpl__;
				if (SetThreadContext(hthrd, &ct))
				{
					//succeed_ids.push_back(v);
					++cnt;
				}
				else
				{
					assert(false);
				}
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			auto err_code = GetLastError();
			auto start_addr = GetThreadStartAddr(hthrd);
			//拒绝访问
			if (5 == err_code)
			{
				DWORD exist_code = STILL_ACTIVE;
				if (GetExitCodeThread(hthrd, &exist_code))
				{
					if (exist_code != STILL_ACTIVE)
					{
						LOG_O(Log_debug) << "Sleep线程时，无法挂起线程，因为线程已结束，线程ID：" << v.GetId() << " 线程入口：" << start_addr;
						continue;
					}
				}
			}
			LOG_O(Log_debug) << "Sleep线程时，无法挂起线程，线程ID：" << v.GetId() << " 线程入口：" << start_addr;
			assert(false);
			continue;
		}
		ResumeThread(hthrd);
	}
	for (auto& v : thrd_ids_)
	{
		auto& the_v = const_cast<ThrdHandleAndId&>(v);
		the_v.Discard();
		auto old_size = handled_thrd_ids_.size();
		handled_thrd_ids_.insert(v);
		if (old_size == handled_thrd_ids_.size())
		{
			LOG_O(Log_debug) << "handled_thrd_ids_竟然有重复的";
			assert(false);
		}
	}
	thrd_ids_.clear();
	return (int)cnt;
}

bool HandleRubThread::HasThrdId(pt_dword thrd_id) const
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	for (auto& v : thrd_ids_)
	{
		if (v.GetId() == thrd_id)
			return true;
	}
	for (auto& v : handled_thrd_ids_)
	{
		if (v.GetId() == thrd_id)
			return true;
	}
	return false;
}

int HandleRubThread::GetThrdTotalCnt() const
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	return thrd_ids_.size() + handled_thrd_ids_.size();
}

int HandleRubThread::GetUnhandledThrdCnt() const
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	return thrd_ids_.size();
}

int _DoTraCheckThreadImpl__(HandleRubThread& rub_thrd, ModuleOffsetHolder& module_offset){
	const auto kCurrentThreadId = GetCurrentThreadId();
	const auto process_id = GetCurrentProcessId();
	CToolhelp th(TH32CS_SNAPTHREAD, process_id);
	THREADENTRY32 te = { sizeof(te) };
	int cnt = 0;
	for (BOOL fOk = th.ThreadFirst(&te); fOk; fOk = th.ThreadNext(&te)) {
		if (te.th32OwnerProcessID == process_id && kCurrentThreadId != te.th32ThreadID) {
			if (!rub_thrd.HasThrdId(te.th32ThreadID))
			{
				auto addr = GetThreadStartAddr(te.th32ThreadID);
				if (addr && module_offset.ExistByTheAddress(addr))
				{
#ifdef _DEBUG
					auto the_module = ModuleFromAddress(addr);
					char buffer[MAX_PATH];
					buffer[0] = 0;
					GetModuleName(the_module, buffer, MAX_PATH);
					LOG_O(Log_trace) << "DoTraCheckThreadImpl 找到这样的线程，线程ID：" << te.th32ThreadID 
						<< " 线程入口：" << addr << ",Module:" << buffer;
#endif
					rub_thrd.AddThrdId(te.th32ThreadID);
					++cnt;
				}
			}
		}
	}
	return cnt;
}

bool HandleRubThread::DoAllThrdSleepByOffset(pt_dword timeout, ModuleOffsetHolder& module_offset, SleepFunctionT sleep_fn)
{
	if (!sleep_fn)
	{
		assert(false);
		return false;
	}
	const auto kEveryTime = 100;
	auto offset_cnt = module_offset.GetOffsetTotalCnt();
	bool res = false;
	int handled_cnt = 0;
	for (pt_dword cur = 0; cur < timeout; cur += kEveryTime)
	{
		auto add_cnt = _DoTraCheckThreadImpl__(*this, module_offset);
		if (GetThrdTotalCnt() >= offset_cnt)
		{
			res = true;
			break;
		}
		if (GetUnhandledThrdCnt())
			handled_cnt += DoThrdSleep();
#ifdef _DEBUG
		if (add_cnt)
			module_offset.DebugUnhandledOffsets(*this);
#endif
		sleep_fn(kEveryTime);
	}

	LOG_O(Log_debug) << "开始处理垃圾线程";
	if (res)
	{
		res = false;
		for (pt_dword cur = 0; cur < 1000 * 60; cur += 500)
		{
			handled_cnt += DoThrdSleep();
			if (GetUnhandledThrdCnt() <= 0)
			{
				res = true;
				break;
			}
			sleep_fn(500);
		}
	}
	//return res && handled_cnt > 0;
	return res && handled_cnt >= 0;
}

void HandleRubThread::ClrAll(bool is_close_handle)
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	if (is_close_handle)
	{
		for (auto& v : thrd_ids_)
		{
			auto& the_v = const_cast<ThrdHandleAndId&>(v);
			the_v.CloseHandle();
		}
		for (auto& v : handled_thrd_ids_)
		{
			auto& the_v = const_cast<ThrdHandleAndId&>(v);
			the_v.CloseHandle();
		}
	}

	thrd_ids_.clear();
	handled_thrd_ids_.clear();
}

HandleRubThread::~HandleRubThread()
{
	ClrAll();
}

void HandleRubThread::AddFrom(const HandleRubThread& rhs)
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	Cmnx_Lockable lock2(rhs.thrd_id_mutex_);
	for (auto& v : rhs.thrd_ids_)
	{
		thrd_ids_.insert(v);
	}
	for (auto& v : rhs.handled_thrd_ids_)
	{
		handled_thrd_ids_.insert(v);
	}
}

void HandleRubThread::EraseFirst(bool is_close_handle)
{
	Cmnx_Lockable lock(thrd_id_mutex_);
	auto it = thrd_ids_.begin();
	if (it != thrd_ids_.end())
	{
		if (is_close_handle)
			const_cast<ThrdHandleAndId&>(*it).CloseHandle();
		thrd_ids_.erase(it);
	}
}

unsigned __stdcall DoPreCreateThrd_Std__(void* param){
	auto sleep_time = (pt_dword)param;
	if (-1 == sleep_time)
	{
		while (true)
		{
			::Sleep(1000 * 60 * 10);
		}
	}
	else
	{
		::Sleep(sleep_time);
	}
	return 0;
}

void __cdecl DoPreCreateThrd_cdecl__(void* param){
	DoPreCreateThrd_Std__(param);
}

bool ModuleOffsetHandler::DoPreCreateThrd_Std(StartFn_Std& start_addr, void*& thrd_param,
	HMODULE* out_module, char* out_module_name, int module_name_max_size)
{
	return DoPreCreateThrd((void*&)start_addr, thrd_param, (StartFn_Std)DoPreCreateThrd_Std__, out_module, out_module_name, module_name_max_size);
}

bool ModuleOffsetHandler::DoPreCreateThrd_Cdecl(StartFn_Cdecl& start_addr, void*& thrd_param,
	HMODULE* out_module, char* out_module_name, int module_name_max_size)
{
	return DoPreCreateThrd((void*&)start_addr, thrd_param, (StartFn_Cdecl)DoPreCreateThrd_cdecl__, out_module, out_module_name, module_name_max_size);
}

bool ModuleOffsetHandler::DoPreCreateThrd(void*& start_addr, void*& thrd_param, void* dummy_thrd_fn,
	HMODULE* out_module, char* out_module_name, int module_name_max_size)
{
	assert(start_addr);
	assert(dummy_thrd_fn);
	assert(out_module_name);
	assert(module_name_max_size > 5);
	auto the_module = ModuleFromAddress(start_addr);
	if (out_module)
		*out_module = the_module;
	out_module_name[0] = 0;

	if (!GetModuleName(the_module, out_module_name, module_name_max_size))
	{
		assert(false);
		return false;
	}
	Cmnx_Lockable lock(mutex_);
	for (auto it = module_offset_.begin(); it != module_offset_.end(); ++it)
	{
		bool need_erase;
		if (it->IsTheModule(out_module_name, &need_erase))
		{
			for (auto& offset : it->offsets_)
			{
				if (offset.HitTheAddress(the_module, start_addr))
				{
					--const_cast<int&>(offset.occur_cnt_);
					if (offset.occur_cnt_ == 0)
					{
						it->offsets_.erase(offset);
						if (it->offsets_.empty() && need_erase)
							module_offset_.erase(it);
					}
					LOG_O(Log_trace) << "DoPreCreateThrd, old_start_addr:" << start_addr << " 模块名：" << out_module_name;
					start_addr = dummy_thrd_fn;
					thrd_param = (void*)(1000 * 2);
					return true;
				}
			}
		}
	}
	return false;
}

void ModuleOffsetHolder::AddModuleOffset(pt_csz module, const OffsetsT& offsets, enModuleMode mm)
{
	if (offsets.empty())
		return;
	if (!module)
	{
		assert(false);
		return;
	}
	for (auto& v : module_offset_)
	{
		if (0 == _stricmp(v.module_name_, module))
		{
			assert(false);
			return;
		}
	}
	stOffsetInfo offset_info = {
		module, offsets, mm
	};
	module_offset_.push_back(offset_info);
}

bool ModuleOffsetHolder::ExistByTheAddress(pt_pcvoid address) const
{
	if (IsUnValidPtr(address))
	{
		assert(false);
		return false;
	}
	auto hmodule = ModuleFromAddress(address);
	if (!hmodule)
		return false;
	char buffer[MAX_PATH];
	if (!GetModuleName(hmodule, buffer, sizeof(buffer)))
		return false;

	//LOG_O(Log_trace) << "ExistByTheAddress之dll_name:" << it_c << " address:" << address;
	for (auto it = module_offset_.begin(); it != module_offset_.end(); ++it)
	{
		if (it->IsTheModule(buffer, nullptr))
		{
			for (auto& offset : it->offsets_)
			{
				if (offset.HitTheAddress(hmodule, address))
					return true;
			}
		}
	}
	return false;
}

bool ModuleOffsetHolder::IsEmpty() const
{
	return module_offset_.empty();
}

bool ModuleOffsetHolder::EraseByTheAddress(pt_pcvoid address)
{
	if (IsUnValidPtr(address))
	{
		assert(false);
		return false;
	}
	auto hmodule = ModuleFromAddress(address);
	if (!hmodule)
		return false;
	char buffer[MAX_PATH];
	if (!GetModuleName(hmodule, buffer, sizeof(buffer)))
		return false;

	for (auto it = module_offset_.begin(); it != module_offset_.end(); ++it)
	{
		bool need_erase;
		if (it->IsTheModule(buffer, &need_erase))
		{
			for (auto& offset : it->offsets_)
			{
				if (offset.HitTheAddress(hmodule, address))
				{
					--const_cast<int&>(offset.occur_cnt_);
					if (offset.occur_cnt_ == 0)
					{
						it->offsets_.erase(offset);
						if (need_erase && it->offsets_.empty())
							module_offset_.erase(it);
					}
					return true;
				}
			}
		}
	}
	return false;
}

int ModuleOffsetHolder::GetOffsetTotalCnt() const
{
	int cnt = 0;
	for (auto& v : module_offset_)
	{
		for (auto& offset : v.offsets_)
		{
			if (offset.occur_cnt_ >= 0)
				cnt += offset.occur_cnt_;
			else
				++cnt;
		}
	}
	return cnt;
}

void ModuleOffsetHolder::DebugUnhandledOffsets(const HandleRubThread& rub_thrd) const
{
	auto tmp_offset = *this;
	{
		Cmnx_Lockable lock(rub_thrd.thrd_id_mutex_);
		for (auto& v : rub_thrd.thrd_ids_)
		{
			auto addr = GetThreadStartAddr(v.GetHandle());
			if (ExistByTheAddress(addr))
				tmp_offset.EraseByTheAddress(addr);
		}
		for (auto& v : rub_thrd.handled_thrd_ids_)
		{
			auto addr = GetThreadStartAddr(v.GetHandle());
			if (ExistByTheAddress(addr))
				tmp_offset.EraseByTheAddress(addr);
		}
	}	
	LOG_O(Log_trace) << "还有这些个线程未处理：" << tmp_offset.GetOffsetTotalCnt();
	tmp_offset.DebugInfo();
}

void ModuleOffsetHolder::DebugInfo() const
{
	for (auto& v : module_offset_)
	{
		for (auto& offset : v.offsets_)
		{
			LOG_O(Log_trace) << "dll_name:" << v.module_name_ << std::hex << " offset:" << offset.offset_ << std::dec << 
				" occur_cnt:" << offset.occur_cnt_ << " offset_mode:" << offset.offset_mode_;
		}
	}
}

bool operator < (const ThrdHandleAndId& lhs, const ThrdHandleAndId& rhs){
	return lhs.thrd_ < rhs.thrd_;
}

ThrdHandleAndId::ThrdHandleAndId(DWORD thrd_id)
{
	discard_ = true;
	thrd_ = OpenThread(THREAD_SET_CONTEXT | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME, FALSE, thrd_id);
	if (!thrd_)
	{
		thrd_id_ = 0;
		assert(false);
		return;
	}
	thrd_id_ = thrd_id;
	discard_ = false;
}

ThrdHandleAndId::ThrdHandleAndId(HANDLE thrd)
{
	discard_ = true;
	thrd_ = nullptr;
	thrd_id_ = 0;
	if (!thrd)
	{
		assert(false);
		return;
	}
	thrd_id_ = GetThreadId(thrd);
	if (!thrd_id_)
	{
		assert(false);
		return;
	}
	thrd_ = thrd;
	discard_ = false;
}

DWORD ThrdHandleAndId::GetId() const
{
	/*auto res = GetThreadId(thrd_);
	if (res != thrd_id_)
		return 0;
	return res;*/
	return thrd_id_;
}

HANDLE ThrdHandleAndId::GetHandle() const
{
	return thrd_;
}

void ThrdHandleAndId::CloseHandle()
{
	if (thrd_)
		::CloseHandle(thrd_);
	thrd_ = nullptr;
	thrd_id_ = 0;
}

bool ThrdHandleAndId::IsDiscard() const
{
	return discard_;
}

void ThrdHandleAndId::Discard()
{
	assert(!discard_);
	discard_ = true;
}

bool operator <(const ModuleOffsetHolder::stOffsetInfoT& lhs, const ModuleOffsetHolder::stOffsetInfoT& rhs){
	return lhs.offset_ < rhs.offset_;
}

ModuleOffsetHolder::stOffsetInfoT::stOffsetInfoT(pt_dword offset, int occur_cnt)
{
	offset_ = offset;
	occur_cnt_ = occur_cnt;
	offset_mode_ = kOM_Offset;
}

ModuleOffsetHolder::stOffsetInfoT::stOffsetInfoT(pt_dword offset, int occur_cnt, enOffsetMode offset_mode)
{
	offset_ = offset;
	occur_cnt_ = occur_cnt;
	offset_mode_ = offset_mode;
}

bool ModuleOffsetHolder::stOffsetInfoT::HitTheAddress(HMODULE hmodule, pt_pcvoid address) const
{
	if (0 == offset_)
		return true;
	else
	{
		if (kOM_Offset == offset_mode_)
		{
			if (((pt_csz)hmodule + offset_) == address)
				return true;
		}
		else if (kOM_Address == offset_mode_)
		{
			if (offset_ == (pt_cdword)address)
				return true;
		}
		else if (kOM_NegOffset == offset_mode_)
		{
			if (((pt_csz)hmodule + offset_) != address)
				return true;
		}
		else if (kOM_NegAddress == offset_mode_)
		{
			if (offset_ != (pt_cdword)address)
				return true;
		}
		else
		{
			assert(false);
		}
	}
	return false;
}

bool ModuleOffsetHolder::stOffsetInfo::IsTheModule(pt_csz module_name, bool* need_erase) const
{
	if (kMM_Specific == mm_)
	{
		if (0 != _stricmp(module_name_, module_name))
			return false;
		if (need_erase)
			*need_erase = true;
	}
	else if (kOM_NegModule == mm_)
	{
		if (0 == _stricmp(module_name_, module_name))
			return false;
		if (need_erase)
			*need_erase = false;
	}
	else
	{
		assert(false);
		return false;
	}
	return true;
}
