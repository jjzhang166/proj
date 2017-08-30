#include "stdafx.h"
#include <CmnMix/Cmn_FileMap.h>
#include <cassert>
#include <CmnMix/Cmn_UsefulFunc.h>

MemFileMap::MemFileMap(const std::string& file_map_name) : file_map_name_("Global\\")
{
	assert(!file_map_name.empty());
	file_map_name_ += file_map_name;
	handle_file_map_ = nullptr;
	ptr_map_view_ = nullptr;
	file_map_size_ = 0;
}

bool MemFileMap::CreateFileMap(pt_dword size_of_file_map)
{
	if (handle_file_map_ || ptr_map_view_ || file_map_name_.empty())
	{
		assert(false);
		return false;
	}
	if (size_of_file_map < 0)
	{
		assert(false);
		return false;
	}
	handle_file_map_ = CreateFileMappingA(
		INVALID_HANDLE_VALUE,   // Use paging file - shared memory
		GetNullDACL(),                   // Default security attributes
		PAGE_READWRITE,         // Allow read and write access
		0,                      // High-order DWORD of file mapping max size
		size_of_file_map,					// Low-order DWORD of file mapping max size
		file_map_name_.c_str()           // Name of the file mapping object
		);
	if (!handle_file_map_)
	{
		assert(false);
		return false;
	}
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		assert(false);
		handle_file_map_.Cleanup();
		return false;
	}
	// Map a view of the file mapping into the address space of the current 
	// process.
	ptr_map_view_ = MapViewOfFile(
		handle_file_map_,               // Handle of the map object
		FILE_MAP_READ | FILE_MAP_WRITE,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		0,		// Low-order DWORD of the file offset 
		size_of_file_map					// The number of bytes to map to view
		);
	if (!ptr_map_view_)
	{
		assert(false);
		handle_file_map_.Cleanup();
		return false;
	}
	file_map_size_ = size_of_file_map;
	return true;
}

bool MemFileMap::OpenFileMap(pt_dword size_of_file_map)
{
	if (handle_file_map_ || ptr_map_view_ || file_map_name_.empty())
	{
		assert(false);
		return false;
	}
	if (size_of_file_map < 0)
	{
		assert(false);
		return false;
	}
	handle_file_map_ = OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, file_map_name_.c_str());
	if (!handle_file_map_)
	{
		return false;
	}
	ptr_map_view_ = MapViewOfFile(
		handle_file_map_,               // Handle of the map object
		FILE_MAP_READ | FILE_MAP_WRITE,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		0,						// Low-order DWORD of the file offset 
		size_of_file_map						// The number of bytes to map to view
		);
	if (!ptr_map_view_)
	{
		assert(false);
		handle_file_map_.Cleanup();
		return false;
	}
	file_map_size_ = size_of_file_map;
	return true;
}

bool MemFileMap::IsValid() const
{
	return handle_file_map_ && ptr_map_view_ && file_map_size_ > 0;
}

pt_dword MemFileMap::FileMapSize() const
{
	return file_map_size_;
}

void* MemFileMap::IndexAsVoidPtr(pt_dword idx)
{
	if (idx >= file_map_size_)
	{
		assert(false);
		return nullptr;
	}
	return (char*)(void*)ptr_map_view_ + idx;
}

const void* MemFileMap::IndexAsVoidPtr(pt_dword idx) const
{
	if (idx >= file_map_size_)
	{
		assert(false);
		return nullptr;
	}
	return (const char*)(const void*)ptr_map_view_ + idx;
}

const std::string& MemFileMap::GetFileMapName() const
{
	return file_map_name_;
}

HANDLE MemFileMap::GetHandle() const
{
	return handle_file_map_;
}

CmnFileMapQueue::~CmnFileMapQueue()
{
	//test
	//allocate_mgr_.Reset();

	if (file_map_impl_)
	{
		auto tasks_ptr = file_map_impl_->IndexAs<TasksT>(sizeof(LockNameT) * 2);
		if (tasks_ptr)
			tasks_ptr->~TasksT();
	}

	task_queue_.reset();
	file_map_impl_.reset();
}

bool CmnFileMapQueue::CreateFileMap(const std::string& file_map_name, size_type size_of_file_map)
{
	if (size_of_file_map < (sizeof(LockNameT) * 2 + sizeof(TasksT)) * 2)
	{
		assert(false);
		return false;
	}
	file_map_impl_.reset(new MemFileMap(file_map_name));
	if (!file_map_impl_)
	{
		assert(false);
		return false;
	}
	if (!file_map_impl_->CreateFileMap(size_of_file_map))
	{
		assert(false);
		file_map_impl_.reset();
		return false;
	}
	auto tasks_ptr = file_map_impl_->IndexAs<TasksT>(sizeof(LockNameT) * 2);
	if (!tasks_ptr)
	{
		assert(false);
		return false;
	}
	new (tasks_ptr)TasksT();
	task_queue_.reset(new CmnFileMapStaticQueue(*tasks_ptr, nullptr, this));
	if (!task_queue_)
	{
		assert(false);
		return false;
	}
	const auto& str_lock_name = RandomUniqueStr(kUuidStrLen);
	const auto& str_event_name = RandomUniqueStr(kUuidStrLen);
	task_queue_->SetLockName(str_lock_name, str_event_name);
	XorBufferData((void*)str_lock_name.c_str(), str_lock_name.c_str(), str_lock_name.size(), kXorData);
	XorBufferData((void*)str_event_name.c_str(), str_event_name.c_str(), str_event_name.size(), kXorData);
	if (!Cmnx_CpyMem(file_map_impl_->IndexAs<TasksT>(0), str_lock_name.c_str(), str_lock_name.size()))
	{
		assert(false);
		return false;
	}
	if (!Cmnx_CpyMem(file_map_impl_->IndexAs<TasksT>(sizeof(LockNameT)), str_event_name.c_str(), str_event_name.size()))
	{
		assert(false);
		return false;
	}
	if (!DoInit())
	{
		assert(false);
		return false;
	}
	const auto kBeginSize = sizeof(LockNameT) * 2 + sizeof(TasksT);
	auto the_buffer_begin = file_map_impl_->IndexAs<void>(kBeginSize);
	if (!the_buffer_begin)
	{
		assert(false);
		return false;
	}
	if (!allocate_mgr_.Reset(the_buffer_begin, (char*)the_buffer_begin + file_map_impl_->FileMapSize() - kBeginSize, true))
	{
		assert(false);
		return false;
	}

	task_queue_->SetMsgSink(this);
	return true;
}

bool CmnFileMapQueue::OpenFileMap(const std::string& file_map_name, size_type size_of_file_map)
{
	if (size_of_file_map < (sizeof(LockNameT) * 2 + sizeof(TasksT)) * 2)
	{
		assert(false);
		return false;
	}
	file_map_impl_.reset(new MemFileMap(file_map_name));
	if (!file_map_impl_)
	{
		assert(false);
		return false;
	}
	if (!file_map_impl_->OpenFileMap(size_of_file_map))
	{
		//assert(false);
		file_map_impl_.reset();
		return false;
	}
	auto tasks_ptr = file_map_impl_->IndexAs<TasksT>(sizeof(LockNameT) * 2);
	if (!tasks_ptr)
	{
		assert(false);
		return false;
	}
	task_queue_.reset(new CmnFileMapStaticQueue(*tasks_ptr, nullptr, this));
	if (!task_queue_)
	{
		assert(false);
		return false;
	}
	std::string str_lock_name(sizeof(LockNameT), '\t');
	std::string str_event_name(sizeof(LockNameT), '\n');
	if (!Cmnx_CpyMem((void*)str_lock_name.c_str(), file_map_impl_->IndexAs<TasksT>(0), sizeof(LockNameT)))
	{
		assert(false);
		return false;
	}
	if (!Cmnx_CpyMem((void*)str_event_name.c_str(), file_map_impl_->IndexAs<TasksT>(sizeof(LockNameT)), sizeof(LockNameT)))
	{
		assert(false);
		return false;
	}
	XorBufferData((void*)str_lock_name.c_str(), str_lock_name.c_str(), str_lock_name.size(), kXorData);
	XorBufferData((void*)str_event_name.c_str(), str_event_name.c_str(), str_event_name.size(), kXorData);
	task_queue_->SetLockName(str_lock_name, str_event_name);

	if (!DoInit())
	{
		assert(false);
		return false;
	}

	const auto kBeginSize = sizeof(LockNameT) * 2 + sizeof(TasksT);
	auto the_buffer_begin = file_map_impl_->IndexAs<void>(kBeginSize);
	if (!the_buffer_begin)
	{
		assert(false);
		return false;
	}
	if (!allocate_mgr_.Reset(the_buffer_begin, (char*)the_buffer_begin + file_map_impl_->FileMapSize() - kBeginSize, false))
	{
		assert(false);
		return false;
	}

	task_queue_->SetMsgSink(this);
	return true;
}

bool CmnFileMapQueue::PostMsg(MsgIdT msg_id, const void* param, size_type param_size)
{
	if (!task_queue_)
	{
		assert(false);
		return false;
	}
	if (param && param_size >= 0)
	{
		auto new_mem = (char*)DoNewMem(param, param_size);
		if (!new_mem)
		{
			assert(false);
			return false;
		}
		XorBufferData(new_mem, new_mem, param_size, kXorData);
		CmnX_CodeInfo mem_addr(new_mem, new_mem + param_size);
		return task_queue_->AddTask(task_queue_->MakeFuncInfo(msg_id, &mem_addr));
	}
	return task_queue_->AddTask(task_queue_->MakeFuncInfo(msg_id, nullptr));
}

Cmnx_TaskQueueBase* CmnFileMapQueue::GetTaskQueue()
{
	return task_queue_.get();
}

void CmnFileMapQueue::SetMsgSink(CmnFileMapMsgSink* msg_sink)
{
	msg_sink_ = msg_sink;
}

bool CmnFileMapQueue::DoInit()
{
	if (!task_queue_ || !file_map_impl_)
	{
		assert(false);
		return false;
	}
	if (!task_queue_->InitTaskQueue())
	{
		assert(false);
		return false;
	}
	allocate_mgr_.SetLockImpl(task_queue_->GetTaskLock());
	return true;
}

void* CmnFileMapQueue::DoNewMem(const void* param, size_type param_size)
{
	if (!param || param_size <= 0)
	{
		assert(false);
		return nullptr;
	}
	auto res = allocate_mgr_.Malloc(param_size);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!Cmnx_CpyMem(res, param, param_size))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void CmnFileMapQueue::OnFreeMem(void* addr)
{
	if (!addr)
	{
		assert(false);
		return;
	}
	allocate_mgr_.Free(addr);
}

CmnFileMapQueue::CmnFileMapQueue()
{
	msg_sink_ = nullptr;
}

void CmnFileMapQueue::OnHandleMsg(unsigned int msg_id, const void* param, int param_size)
{
	if (!msg_sink_)
		return;
	if (param && param_size > 0)
		XorBufferData((void*)param, param, param_size, kXorData);
	msg_sink_->OnHandleMsg(msg_id, param, param_size);
}

bool CmnFileMapQueue::IsValid() const
{
	if (!file_map_impl_)
		return false;
	return file_map_impl_->IsValid();
}

void CmnFileMapQueue::Close()
{
	file_map_impl_.reset();
	task_queue_.reset();
}

HANDLE CmnFileMapQueue::GetHandle() const
{
	if (file_map_impl_)
		return file_map_impl_->GetHandle();
	return nullptr;
}

CmnFileMapStaticQueue::CmnFileMapStaticQueue(TasksT& tasks, CmnFileMapMsgSink* msg_sink, CmnFileMapOnFree* on_free)
	: tasks_(tasks), msg_sink_(msg_sink), on_free_(on_free)
{
	assert(on_free);
}

CmnFileMapStaticQueue::TasksT& CmnFileMapStaticQueue::GetTasks()
{
	return tasks_;
}

const CmnFileMapStaticQueue::TasksT& CmnFileMapStaticQueue::GetTasks() const
{
	return tasks_;
}

CmnFileMapStaticQueue::stFuncInfo CmnFileMapStaticQueue::MakeFuncInfo(MsgIdT msg_id, const CmnX_CodeInfo* param)
{
	CmnX_CodeInfo dummy_param((void*)&tasks_, (void*)&tasks_);
	if (param)
		dummy_param -= *param;
	return __super::MakeFuncInfo(msg_id, (size_type)dummy_param.begin_addr_, (size_type)dummy_param.end_addr_);
}

void CmnFileMapStaticQueue::DoExecTask(TaskFuncT& cur_task)
{
	if (cur_task.offset_begin_ == cur_task.offset_end_)
	{
		if (msg_sink_)
			msg_sink_->OnHandleMsg(cur_task.msg_id_, nullptr, 0);
	}
	else
	{
		const int kBufferSize = 100;
		CmnX_CodeInfo real_param((void*)&tasks_, (void*)&tasks_);
		real_param -= cur_task.ToCodeInfo();
		auto param_size = real_param.GetCodeSize();
		if (param_size > kBufferSize || !msg_sink_)
		{
			assert(on_free_);
			BOOST_SCOPE_EXIT_ALL(&, param_size){
				if (param_size > 0)
					std::memset(real_param.begin_addr_, 0, param_size);
				on_free_->OnFreeMem(real_param.begin_addr_);
			};
			if (msg_sink_)
				msg_sink_->OnHandleMsg(cur_task.msg_id_, real_param.begin_addr_, param_size);
		}
		else
		{
			char buffer[kBufferSize];
			Cmnx_CpyMem(buffer, real_param.begin_addr_, param_size);
			if (param_size > 0)
				std::memset(real_param.begin_addr_, 0, param_size);
			assert(on_free_);
			on_free_->OnFreeMem(real_param.begin_addr_);
			assert(msg_sink_);
			msg_sink_->OnHandleMsg(cur_task.msg_id_, buffer, param_size);
		}
	}
}

void CmnFileMapStaticQueue::SetMsgSink(CmnFileMapMsgSink* msg_sink)
{
	msg_sink_ = msg_sink;
}

bool CmnFileMapStaticQueue::PreAddTask()
{
	if (!__super::PreAddTask())
	{
		assert(false);
		return false;
	}
	auto& tasks = GetTasks();
	if (tasks.is_full())
	{
		auto cur_task = tasks.front();
		tasks.pop();
		if (cur_task.offset_begin_ != cur_task.offset_end_)
		{
			CmnX_CodeInfo real_param((void*)&tasks_, (void*)&tasks_);
			real_param -= cur_task.ToCodeInfo();
			assert(on_free_);
			on_free_->OnFreeMem(real_param.begin_addr_);
		}
	}
	return true;
}

CmnFileMapMsgSink::~CmnFileMapMsgSink()
{

}
