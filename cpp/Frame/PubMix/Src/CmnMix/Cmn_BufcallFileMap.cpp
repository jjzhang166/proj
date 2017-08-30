#include "stdafx.h"
#include <CmnMix/Cmn_BufcallFileMap.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <protect/Protect.h>

bool ToCallSync_FileMapSocketImpl_(pt_dword wait_timeout, FileMapDoAtIdle* do_other, CmnFileMapQueue& file_map_queue, BC_BufCallBase& bc,
	const BC_AsStreamW& stm, boost::optional<bool>& oper_res){
	VMP_BEGIN_EX;
	oper_res.reset();
	auto task_mgr = file_map_queue.GetTaskQueue();
	if (!task_mgr)
	{
		assert(false);
		throw CmnExitIoThreadException("reuiqnjkm");
	}
	if (!bc.ToCall(stm))
	{
		assert(false);
		throw CmnExitIoThreadException("fdaser");
	}
	if (do_other)
	{
		FunctionVoid idle_fn = [do_other](){
			do_other->Do();
		};
		task_mgr->RunWait([&](){
			if (oper_res)
				return true;
			return false;
		}, &idle_fn, wait_timeout);
	}
	else
	{
		task_mgr->RunWait([&](){
			if (oper_res)
				return true;
			return false;
		}, nullptr, wait_timeout);
	}
	
	if (!CheckOptionalBoolRes(oper_res))
	{
		assert(false);
		throw CmnExitIoThreadException("jfdkslaoiquir");
	}
	VMP_END;
	return true;
}
