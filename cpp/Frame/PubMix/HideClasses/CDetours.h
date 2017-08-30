#pragma once
/*
@author zhp
@date 2014-1-02
@purpose encapsulation detours
*/
#include <CmnMix/Cmn_Hooker.h>
#include "../DependSelfLib.h"

class CDetours : public IHooker{
public:
	//update_thread_id==0表示更新所有线程，否则更新指定线程
	CDetours(const char* hooker_name, void* hooked_addr, void* detour_addr, unsigned int update_thread_id, void** code_orig = nullptr);
	~CDetours();

protected:
	virtual void* GetHookedCodeAddr();
	virtual void* GetCodeOrig();
	virtual bool IsValid();
	virtual void SetResume( bool is_resume );

private:
	void*					trampoline_addr_;
	void*					hooked_addr_;
	void*					hooked_addr_tmp_;
	void*					detour_addr_;
	unsigned int			update_thread_id_;
	bool					is_resume_;
};