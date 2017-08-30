#pragma once
/*
@author zhp
@date 2014/7/26 12:31
@purpose memory file map
*/
#include <string>
#include "stdafx.h"
#include "../DependSelfLib.h"
#include "Cmn_EnsureCleanup.h"
#include "Cmn_TaskQueue.h"
#include <CommonX/CmnX_UsefulClass.h>
#include "Cmn_Allocator.h"
#include <array>
#include <ProtocolX/P_DefinesX.h>
#include <CmnMix/Cmn_TemplateUtility.h>
//////////////////////////////////////////////////////////////////////////
const unsigned int kFileMap_OnePageSize = 1024;
const unsigned int kFileMap_StaticQueueMaxMsgCnt = 50;
const unsigned int kFileMap_StaticQueueDefaultSize = kFileMap_OnePageSize * 2;
//////////////////////////////////////////////////////////////////////////
class MemFileMap{
public:
	explicit MemFileMap(const std::string& file_map_name);

	//initialization
public:
	//create
	bool CreateFileMap(pt_dword size_of_file_map = kFileMap_OnePageSize);
	//open
	bool OpenFileMap(pt_dword size_of_file_map = kFileMap_OnePageSize);
	HANDLE GetHandle() const;

	//interfaces
public:
	//是否有效
	bool IsValid() const;
	//内存映射文件大小
	pt_dword FileMapSize() const;
	//内存映射文件名
	const std::string& GetFileMapName() const;
	//索引内存
	void* IndexAsVoidPtr(pt_dword idx);
	const void* IndexAsVoidPtr(pt_dword idx) const;
	template<typename T>
	T* IndexAs(pt_dword idx){
		return static_cast<T*>(IndexAsVoidPtr(idx));
	}
	template<typename T>
	const T* IndexAs(pt_dword idx) const{
		return static_cast<const T*>(IndexAsVoidPtr(idx));
	}

private:
	std::string				file_map_name_;
	CEnsureCloseHandle		handle_file_map_;
	CEnsureUnmapViewOfFile	ptr_map_view_;
	pt_dword				file_map_size_;
};

class CmnFileMapMsgSink{
public:
	virtual ~CmnFileMapMsgSink();
	virtual void OnHandleMsg(unsigned int msg_id, const void* param, int param_size) = 0;
};

class CmnFileMapStaticQueue : public CmnTaskStaticQueue_Msg<kFileMap_StaticQueueMaxMsgCnt,
	Cmnx_TaskQueue_TasksNil<typename CmnTaskStaticQueue_Msg_Helper<kFileMap_StaticQueueMaxMsgCnt>::TasksT_>, Cmnx_TaskQueueBase_MultiProcess,
	Cmnx_TaskQueue_AlgoMore_Normal<typename CmnTaskStaticQueue_Msg_Helper<kFileMap_StaticQueueMaxMsgCnt>::TasksT_> >{
private:
	class CmnFileMapOnFree{
	public:
		virtual void OnFreeMem(void* addr) = 0;
	};
	friend class CmnFileMapQueue;
	static_assert(sizeof(stFuncInfo) == 6, "");

public:
	CmnFileMapStaticQueue(TasksT& tasks, CmnFileMapMsgSink* msg_sink, CmnFileMapOnFree* on_free);
	TasksT& GetTasks();
	const TasksT& GetTasks() const;

public:
	stFuncInfo MakeFuncInfo(MsgIdT msg_id, const CmnX_CodeInfo* param);
	void SetMsgSink(CmnFileMapMsgSink* msg_sink);

private:
	void DoExecTask(TaskFuncT& cur_task);
	bool PreAddTask();

private:
	TasksT&				tasks_;
	CmnFileMapMsgSink*	msg_sink_;
	CmnFileMapOnFree*	on_free_;
};

class CmnFileMapQueue : private CmnFileMapStaticQueue::CmnFileMapOnFree, protected CmnFileMapMsgSink{
public:
	typedef std::shared_ptr<MemFileMap> FileMapImplT;
	typedef CmnFileMapStaticQueue::size_type size_type;
	typedef CmnFileMapStaticQueue::MsgIdT MsgIdT;
	typedef CmnFileMapStaticQueue::TasksT TasksT;
	typedef std::array<char, kUuidStrLen> LockNameT;
	static_assert(sizeof(LockNameT) == kUuidStrLen, "");
	typedef std::shared_ptr<CmnFileMapStaticQueue> TaskQueueT;
	typedef CmnAllocateBufferMgrT<Cmnx_MutexRefPtr> AllocateMgrT;

public:
	CmnFileMapQueue();
	CmnFileMapQueue(const CmnFileMapQueue&) = delete;
	CmnFileMapQueue& operator= (const CmnFileMapQueue&) = delete;
	virtual ~CmnFileMapQueue();
	bool CreateFileMap(const std::string& file_map_name, size_type size_of_file_map = kFileMap_StaticQueueDefaultSize);
	bool OpenFileMap(const std::string& file_map_name, size_type size_of_file_map = kFileMap_StaticQueueDefaultSize);
	HANDLE GetHandle() const;
	void Close();
	bool IsValid() const;
	Cmnx_TaskQueueBase* GetTaskQueue();
	void SetMsgSink(CmnFileMapMsgSink* msg_sink);
	bool PostMsg(MsgIdT msg_id, const void* param, size_type param_size);
	template<typename ParamT_>
	bool PostMsg(MsgIdT msg_id, const ParamT_& param){
		return PostMsg(msg_id, &param, sizeof(param));
	}

protected:
	bool DoInit();
	void* DoNewMem(const void* param, size_type param_size);

private:
	void OnFreeMem(void* addr) override final;
	void OnHandleMsg(unsigned int msg_id, const void* param, int param_size) override final;

protected:
	AllocateMgrT			allocate_mgr_;
	FileMapImplT			file_map_impl_;
	TaskQueueT				task_queue_;
	CmnFileMapMsgSink*		msg_sink_;

private:
	static const unsigned char	kXorData = 35;
};
