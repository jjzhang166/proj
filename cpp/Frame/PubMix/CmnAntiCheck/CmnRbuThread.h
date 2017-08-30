#pragma once
/*
@author zhp
@date 2016/6/15 12:26
RbuThread->rubbish thread
*/
#include <set>
#include <ProtocolX/P_DefinesX.h>
#include <CommonX/CmnX_Lock.h>
#include <CommonX/CmnX_UsefulClass.h>
#include <list>
#include "../DependSelfLib.h"

class ThrdHandleAndId{
public:
	ThrdHandleAndId(DWORD thrd_id);
	ThrdHandleAndId(HANDLE thrd);
	DWORD GetId() const;
	HANDLE GetHandle() const;
	void CloseHandle();
	bool IsDiscard() const;
	void Discard();
	friend bool operator < (const ThrdHandleAndId& lhs, const ThrdHandleAndId& rhs);

private:
	DWORD		thrd_id_;
	HANDLE		thrd_;
	bool		discard_;
};

class ModuleOffsetHolder;
class HandleRubThread{
	friend class ModuleOffsetHolder;
public:
	HandleRubThread() = default;
	HandleRubThread(const HandleRubThread&) = delete;
	HandleRubThread& operator = (const HandleRubThread&) = delete;
	~HandleRubThread();
	void AddThrdId(pt_dword thrd_id);
	void EraseThrdId(pt_dword thrd_id);
	void EraseFirst(bool is_close_handle);
	bool HasThrdId(pt_dword thrd_id) const;
	int GetThrdTotalCnt() const;
	int GetUnhandledThrdCnt() const;
	void ClrAll(bool is_close_handle = true);
	void AddFrom(const HandleRubThread& rhs);

public:
	int DoThrdSleep(pt_qword sleep_time = (pt_qword)-1);
	bool DoAllThrdSleepByOffset(pt_dword timeout, ModuleOffsetHolder& module_offset, SleepFunctionT sleep_fn);

private:
	std::set<ThrdHandleAndId>			thrd_ids_;
	mutable Cmnx_Mutex_CriticalSection	thrd_id_mutex_;
	std::set<ThrdHandleAndId>			handled_thrd_ids_;
};

class ModuleOffsetHolder{
public:
	enum enOffsetMode{
		kOM_Offset,
		kOM_Address,
		kOM_NegOffset,
		kOM_NegAddress,
	};
	enum enModuleMode{
		kMM_Specific,
		kOM_NegModule,
	};
	struct stOffsetInfoT{
		pt_dword		offset_ = 0;		//==0表示任意偏移或地址
		int				occur_cnt_ = 1;		//==-1表示任意次数
		enOffsetMode	offset_mode_ = kOM_Offset;

		stOffsetInfoT(pt_dword offset, int occur_cnt);
		stOffsetInfoT(pt_dword offset, int occur_cnt, enOffsetMode offset_mode);
		bool HitTheAddress(HMODULE hmodule, pt_pcvoid address) const;
	};
	friend bool operator <(const stOffsetInfoT& lhs, const stOffsetInfoT& rhs);
	typedef std::set<stOffsetInfoT> OffsetsT;
	struct stOffsetInfo{
		pt_csz			module_name_;
		OffsetsT		offsets_;
		enModuleMode	mm_;

		bool IsTheModule(pt_csz module_name, bool* need_erase) const;
	};
	typedef std::list<stOffsetInfo> ModuleOffsetT;

public:
	void AddModuleOffset(pt_csz module, const OffsetsT& offsets, enModuleMode mm = kMM_Specific);
	bool ExistByTheAddress(pt_pcvoid address) const;
	bool EraseByTheAddress(pt_pcvoid address);
	bool IsEmpty() const;
	int GetOffsetTotalCnt() const;
	void DebugUnhandledOffsets(const HandleRubThread& rub_thrd) const;
	void DebugInfo() const;

protected:
	ModuleOffsetT		module_offset_;
};

class ModuleOffsetHandler : public ModuleOffsetHolder{
public:
	typedef void(*StartFn_Cdecl)(void*);
	typedef unsigned(__stdcall* StartFn_Std)(void*);

public:
	bool DoPreCreateThrd_Std(StartFn_Std& start_addr, void*& thrd_param,
		HMODULE* out_module, char* out_module_name, int module_name_max_size);
	bool DoPreCreateThrd_Cdecl(StartFn_Cdecl& start_addr, void*& thrd_param,
		HMODULE* out_module, char* out_module_name, int module_name_max_size);

private:
	bool DoPreCreateThrd(void*& start_addr, void*& thrd_param, void* dummy_thrd_fn,
		HMODULE* out_module, char* out_module_name, int module_name_max_size);

private:
	mutable Cmnx_Mutex_CriticalSection	mutex_;
};