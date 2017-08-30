#include "stdafx.h"
#include <CmnMix/Cmn_Allocator.h>
#include <cassert>
#include <limits>
#include <CommonX/CmnX_UsefulFunc.h>

bool CmnAllocateBufferMgr::Reset(void* buffer_begin, void* buffer_end, bool force_reset)
{
	CmnX_CodeInfo tmp(buffer_begin, buffer_end);
	const auto buffer_size = tmp.GetCodeSize();
	if (buffer_size < sizeof(stPageInfo))
	{
		assert(false);
		return false;
	}
	if (buffer_size >= std::numeric_limits<size_type>::max())
	{
		assert(false);
		return false;
	}
	if (buffer_info_.begin_addr_ && force_reset)
	{
		const auto& first_free = FindFirstUsedFrom(buffer_info_.begin_addr_);
		//必须保证内存都是释放完的了
		if (first_free.begin_addr_ != first_free.end_addr_)
		{
			assert(false);
			return false;
		}
	}

	if (force_reset)
	{
		auto page_info = (stPageInfo*)buffer_begin;
		page_info->page_size_ = buffer_size;
	}

	buffer_info_ = tmp;
	return true;
}

bool CmnAllocateBufferMgr::Reset()
{
	return Reset(buffer_info_.begin_addr_, buffer_info_.end_addr_, true);
}

CmnX_CodeInfo CmnAllocateBufferMgr::FindNextFirstFree(const void* begin_addr, const void* end_addr, size_type size) const
{
	CmnX_CodeInfo res;
	if (IsUnValidPtr(buffer_info_.begin_addr_) || IsUnValidPtr(buffer_info_.end_addr_))
	{
		assert(false);
		return res;
	}
	if (!buffer_info_.IsInRange(begin_addr))
	{
		assert(false);
		return res;
	}
	/*if ( !buffer_info_.IsInRange(end_addr) )
	{
	assert(false);
	return res;
	}*/
	if (size <= 0)
	{
		assert(false);
		return res;
	}
	size += sizeof(stPageInfo);
	auto the_begin_addr = (char*)begin_addr;

	while (true)
	{
		if (the_begin_addr >= buffer_info_.end_addr_)
		{
			assert(the_begin_addr == buffer_info_.end_addr_);
			break;
		}
		if (the_begin_addr >= end_addr)
			break;
		auto page_info = (const stPageInfo*)the_begin_addr;
		if (page_info->page_size_ <= 0)
		{
			assert("出错了");
			break;
		}
		if (true == page_info->is_used_)
		{
			the_begin_addr += page_info->page_size_;
		}
		else
		{
			if (page_info->page_size_ >= size)
			{
				res.begin_addr_ = the_begin_addr;
				res.end_addr_ = the_begin_addr + page_info->page_size_;
				break;
			}
			the_begin_addr += page_info->page_size_;
		}
	}
	return res;
}

CmnX_CodeInfo CmnAllocateBufferMgr::FindFirstUsedFrom(const void* begin_addr) const
{
	CmnX_CodeInfo res;
	if (IsUnValidPtr(buffer_info_.begin_addr_) || IsUnValidPtr(buffer_info_.end_addr_))
	{
		assert(false);
		return res;
	}
	if (!buffer_info_.IsInRange(begin_addr))
	{
		assert(false);
		return res;
	}

	auto the_begin_addr = (char*)begin_addr;
	while (true)
	{
		if (the_begin_addr >= buffer_info_.end_addr_)
		{
			assert(the_begin_addr == buffer_info_.end_addr_);
			break;
		}

		auto page_info = (const stPageInfo*)the_begin_addr;
		if (page_info->page_size_ <= 0)
		{
			assert("出错了");
			break;
		}
		if (true == page_info->is_used_)
		{
			res.begin_addr_ = the_begin_addr;
			res.end_addr_ = the_begin_addr + page_info->page_size_;
			break;
		}
		else
		{
			the_begin_addr += page_info->page_size_;
		}
	}
	return res;
}

void* CmnAllocateBufferMgr::Malloc(size_type size)
{
	const auto& first_free = FindNextFirstFree(buffer_info_.begin_addr_, buffer_info_.end_addr_, size);
	if (first_free.begin_addr_ == first_free.end_addr_)
	{
		assert(false);
		return nullptr;
	}
	auto page_info = (stPageInfo*)first_free.begin_addr_;
	assert(false == page_info->is_used_);
	assert(page_info->page_size_ >= size + sizeof(stPageInfo));
	page_info->is_used_ = true;
	page_info->page_size_ = size + sizeof(stPageInfo);
	auto res = page_info + 1;

	auto code_size = (size_type)first_free.GetCodeSize();
	if (code_size > page_info->page_size_)
	{
		auto left_code_size = code_size - page_info->page_size_;
		if (left_code_size > sizeof(stPageInfo))
		{
			page_info = (stPageInfo*)((char*)first_free.end_addr_ - left_code_size);
			page_info->is_used_ = false;
			page_info->page_size_ = left_code_size;
		}
		else
		{
			page_info->page_size_ = code_size;
		}
	}
	else
	{
		assert(code_size == page_info->page_size_);
	}
	return res;
}

bool CmnAllocateBufferMgr::Free(void* addr)
{
	if (0 == addr)
		return true;
	if (!buffer_info_.IsInRange(addr))
	{
		assert(false);
		return false;
	}
	auto cur_page_info = (stPageInfo*)((char*)addr - sizeof(stPageInfo));
	if (!buffer_info_.IsInRange(cur_page_info))
	{
		assert(false);
		return false;
	}
	if (true != cur_page_info->is_used_)
	{
		assert(false);
		return false;
	}
	if (cur_page_info->page_size_ <= 0 || cur_page_info->page_size_ > std::numeric_limits<size_type>::max() - sizeof(stPageInfo))
	{
		assert(false);
		return false;
	}

	auto old_end_addr = (const char*)cur_page_info + cur_page_info->page_size_;
	auto another = FindPreFirstFree(addr);
	if (another.begin_addr_ != another.end_addr_)
	{
		if (another.end_addr_ == (void*)cur_page_info)
		{
			auto old_page_size = cur_page_info->page_size_;
			cur_page_info = (stPageInfo*)another.begin_addr_;
			if (false != cur_page_info->is_used_)
			{
				assert(false);
				return false;
			}
			if (cur_page_info->page_size_ + old_page_size > std::numeric_limits<size_type>::max() - (size_type)sizeof(stPageInfo))
			{
				assert(false);
				return false;
			}
			cur_page_info->page_size_ += old_page_size;
		}
		else
		{
			assert(another.end_addr_ < cur_page_info);
		}
	}

	another = FindNextFirstFree((const char*)cur_page_info + cur_page_info->page_size_, old_end_addr + 1, 1);
	if (another.begin_addr_ != another.end_addr_)
	{
		auto the_end_addr = (const char*)cur_page_info + cur_page_info->page_size_;
		if (another.begin_addr_ == the_end_addr)
		{
			auto the_next_page_info = (stPageInfo*)the_end_addr;
			if (false != the_next_page_info->is_used_)
			{
				assert(false);
				return false;
			}
			if (the_next_page_info->page_size_ + cur_page_info->page_size_ > std::numeric_limits<size_type>::max() - (size_type)sizeof(stPageInfo))
			{
				assert(false);
				return false;
			}
			cur_page_info->page_size_ += the_next_page_info->page_size_;
		}
		else
		{
			assert(another.begin_addr_ > the_end_addr);
		}
	}

	cur_page_info->is_used_ = false;
	return true;
}

CmnX_CodeInfo CmnAllocateBufferMgr::FindPreFirstFree(const void* the_addr) const
{
	CmnX_CodeInfo res;
	if (IsUnValidPtr(buffer_info_.begin_addr_) || IsUnValidPtr(buffer_info_.end_addr_))
	{
		assert(false);
		return res;
	}
	if (!buffer_info_.IsInRange(the_addr))
	{
		assert(false);
		return res;
	}
	auto the_begin_addr = (char*)buffer_info_.begin_addr_;
	while (true)
	{
		if (the_begin_addr >= buffer_info_.end_addr_)
		{
			assert(the_begin_addr == buffer_info_.end_addr_);
			break;
		}
		auto page_info = (const stPageInfo*)the_begin_addr;
		if (page_info->page_size_ <= 0)
		{
			assert("出错了");
			break;
		}
		auto the_end_addr = the_begin_addr + page_info->page_size_;
		CmnX_CodeInfo tmp(the_begin_addr, the_end_addr);
		if (tmp.IsInRange(the_addr))
			break;

		if (the_end_addr >= buffer_info_.end_addr_)
		{
			assert(the_end_addr == buffer_info_.end_addr_);
			break;
		}
		if (false == page_info->is_used_)
		{
			tmp.begin_addr_ = the_end_addr;
			auto next_page_info = (const stPageInfo*)the_end_addr;
			if (next_page_info->page_size_ <= 0)
			{
				assert("出错了");
				break;
			}
			tmp.end_addr_ = the_end_addr + next_page_info->page_size_;
			if (tmp.IsInRange(the_addr))
			{
				res.begin_addr_ = the_begin_addr;
				res.end_addr_ = the_end_addr;
				break;
			}
		}
		the_begin_addr = the_end_addr;
	}
	return res;
}

CmnAllocateBufferMgr::~CmnAllocateBufferMgr()
{

}
