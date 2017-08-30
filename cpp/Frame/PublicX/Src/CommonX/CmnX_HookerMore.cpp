#include <stdafx.h>
#include <CommonX/CmnX_HookerMore.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <cassert>
#include <Windows.h>
#include <WinBase.h>
//#include <boost/scope_exit.hpp>	//不能用这个，考虑hook memcpy的时候，容易产生递归

//////////////////////////////////////////////////////////////////////////
//const char* const kHookerName_NoName = "无名字的hook";
//////////////////////////////////////////////////////////////////////////
class AntiHookRTTI_ReleaseSRWLockExclusive_{
public:
	explicit AntiHookRTTI_ReleaseSRWLockExclusive_( PSRWLOCK the_lock ){
		the_lock_ = the_lock;
	}
	~AntiHookRTTI_ReleaseSRWLockExclusive_(){
		ReleaseSRWLockExclusive(the_lock_);
	}

private:
	PSRWLOCK		the_lock_;
};

class AntiHookRTTI_ReleaseSRWLockShared_{
public:
	explicit AntiHookRTTI_ReleaseSRWLockShared_( PSRWLOCK the_lock ){
		the_lock_ = the_lock;
	}
	~AntiHookRTTI_ReleaseSRWLockShared_(){
		ReleaseSRWLockShared(the_lock_);
	}

private:
	PSRWLOCK		the_lock_;
};
//////////////////////////////////////////////////////////////////////////

CmnX_AntiHookCheck& CmnX_AntiHookCheck::GetMe()
{
	static CmnX_AntiHookCheck me;
	return me;
}

CmnX_AntiHookCheck::CmnX_AntiHookCheck()
{
	ZeroMemory(&hook_lock_, sizeof(hook_lock_));
	InitializeSRWLock(&hook_lock_);
	ZeroMemory(&thread_white_list_lock_, sizeof(thread_white_list_lock_));
	InitializeSRWLock(&thread_white_list_lock_);
	log_sink_ = nullptr;
	enable_output_hook_name_ = true;
}

bool CmnX_AntiHookCheck::AddHookCodeInfo( const char* hooker_name, const void* the_hook_addr, pt_dword hook_cpy_size /*= kAntiHookCheck_DefHookCpySize */ )
{
	if (IsUnValidPtr(the_hook_addr))
	{
		assert(false);
		return false;
	}
	if ( hook_cpy_size <= 0 )
	{
		assert(false);
		return false;
	}
	if ( hook_cpy_size > kAntiHookCheck_MaxHookCpySize )
	{
		assert(false);
		return false;
	}
	::AcquireSRWLockExclusive(&hook_lock_);
	AntiHookRTTI_ReleaseSRWLockExclusive_ the_rtti(&hook_lock_);

	CmnX_CodeInfo code_info((void*)the_hook_addr, hook_cpy_size);
	if (IsFindFirstAddrInfo(code_info))
	{
		assert(false);
		return false;
	}

	stHookItemInfo item_info;
	item_info.code_info_ = code_info;
	if ( !Cmnx_CpyMem(&item_info.src_code_, the_hook_addr, hook_cpy_size) )
	{
		assert(false);
		return false;
	}
	if (enable_output_hook_name_)
	{
		if ( !hooker_name )
			hooker_name = "";
		if ( !StdEx::TStrCpy(item_info.hooker_name_, hooker_name, sizeof(item_info.hooker_name_)) )
		{
			assert(false);
			return false;
		}
	}
	
	if (AddOneHookCodeInfo(item_info))
		return true;
	return false;
}

bool CmnX_AntiHookCheck::IsFindFirstAddrInfo( const CmnX_CodeInfo& the_code_info ) const
{
	auto it_end = hooks_info_.end();
	for ( auto it = hooks_info_.begin(); it != it_end; ++it )
	{
		for ( auto it2 = it->begin(), it2_end = it->end(); it2 != it2_end; ++it2 )
		{
			auto& v = *it2;
			if (v.code_info_.ExistIntersection(the_code_info))
				return true;
		}
	}
	return false;
}

int CmnX_AntiHookCheck::AddOther( const CmnX_AntiHookCheck& other )
{
	if (other.hooks_info_.empty())
		return 0;
	::AcquireSRWLockExclusive(&hook_lock_);
	AntiHookRTTI_ReleaseSRWLockExclusive_ the_rtti(&hook_lock_);

	int add_cnt = 0;
	for ( auto& v1 : other.hooks_info_ )
	{
		for ( auto v : v1 )
		{
			if (IsFindFirstAddrInfo(v.code_info_))
			{
				assert(false);
				continue;
			}

			stHookItemInfo item_info;
			item_info.code_info_ = v.code_info_;
			item_info.is_out_direct_ = v.is_out_direct_;
			if ( !Cmnx_CpyMem(&item_info.src_code_, v.src_code_, (unsigned int)v.code_info_.end_addr_ - (unsigned int)v.code_info_.begin_addr_) )
			{
				assert(false);
				continue;
			}
			if ( !StdEx::TStrCpy(item_info.hooker_name_, v.hooker_name_, sizeof(item_info.hooker_name_)) )
			{
				assert(false);
				continue;
			}

			if ( !AddOneHookCodeInfo(item_info) )
			{
				assert(false);
				return add_cnt;
			}
			++add_cnt;
		}
	}
	return add_cnt;
}

void CmnX_AntiHookCheck::GenSrcCodeInfoByAddr( const HooksContT& hooks_cont, CmnX_AntiHookLogSink* log_sink, const void* addr, int src_code_size, SrcCodeInfoContT& out_src_code_info, void* out_buffer )
{
	assert(out_src_code_info.empty());
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return;
	}
	if ( src_code_size <= 0 )
	{
		assert(false);
		return;
	}
	
	CmnX_CodeInfo dst_code_info((void*)addr, src_code_size);
	CodeInfoContT code_infos;
	GetAllAddrInfo(hooks_cont, dst_code_info, code_infos, out_buffer, log_sink);
	if (code_infos.empty())
		return;
	for ( auto& v : code_infos )
	{
		auto& stored_code_info = v->code_info_;
		auto& the_code_info = stored_code_info.CalcIntersection(dst_code_info);
		if ( the_code_info.begin_addr_ == the_code_info.end_addr_ )
		{
			assert(false);
			continue;
		}
		stSrcCodeInfo tmp;
		tmp.offset_to_begin_addr_ = dst_code_info.OffsetToBeginAddr(the_code_info.begin_addr_);
		if ( tmp.offset_to_begin_addr_ < 0 )
		{
			assert(false);
			continue;
		}
		auto the_src_code_addr = (char*)v->src_code_;
		tmp.code_info_.begin_addr_ = the_src_code_addr + stored_code_info.OffsetToBeginAddr(the_code_info.begin_addr_);
		tmp.code_info_.end_addr_ = the_src_code_addr + stored_code_info.OffsetToBeginAddr(the_code_info.end_addr_);
		assert(tmp.code_info_.GetCodeSize() <= kAntiHookCheck_MaxHookCpySize);
		if (log_sink)
		{
			log_sink->OnHookChecked(v->hooker_name_, the_code_info.begin_addr_, the_code_info.GetCodeSize(), stored_code_info.begin_addr_, stored_code_info.GetCodeSize());
		}

		out_src_code_info.push(tmp);
	}
	return;
}

void CmnX_AntiHookCheck::GetAllAddrInfo( const HooksContT& hooks_cont, const CmnX_CodeInfo& the_code_info, CodeInfoContT& out_code_info, void* out_buffer, CmnX_AntiHookLogSink* log_sink )
{
	assert(out_code_info.empty());
	auto it_end = hooks_cont.end();
	for ( auto it = hooks_cont.begin(); it != it_end; ++it )
	{
		auto& v = *it;
		if (v.code_info_.ExistIntersection(the_code_info))
		{
			if (v.is_out_direct_)
			{
				if (IsUnValidPtr(out_buffer))
				{
					assert(false);
					continue;
				}
				const auto& the_intersection = v.code_info_.CalcIntersection(the_code_info);
				if ( the_intersection.begin_addr_ == the_intersection.end_addr_ )
				{
					assert(false);
					continue;
				}
				if (log_sink)
				{
					log_sink->OnHookChecked(v.hooker_name_, the_intersection.begin_addr_,
						the_intersection.GetCodeSize(), v.code_info_.begin_addr_, v.code_info_.GetCodeSize());
				}
				if ( !Cmnx_CpyMemRecycle((char*)out_buffer + the_code_info.OffsetToBeginAddr(the_intersection.begin_addr_), the_intersection.GetCodeSize(), v.src_code_, sizeof(v.src_code_)) )
				{
					assert(false);
					continue;
				}
			}
			else
				out_code_info.push(it);
		}
	}
}

bool CmnX_AntiHookCheck::OutputSrcCodeInfoByAddr( const void* addr, int src_code_size, void* out_buffer_src ) const
{
	if (IsUnValidPtr(out_buffer_src))
	{
		assert(false);
		return false;
	}
	::AcquireSRWLockShared(&hook_lock_);
	AntiHookRTTI_ReleaseSRWLockShared_ the_rtti(&hook_lock_);
	
	int cnt = 0;
	{
		for ( auto& v : hooks_info_ )
		{
			SrcCodeInfoContT src_code_info;
			GenSrcCodeInfoByAddr(v, log_sink_, addr, src_code_size, src_code_info, out_buffer_src);
			if (src_code_info.empty())
				continue;
			cnt += (int)src_code_info.size();
			auto buffer_tmp = (char*)out_buffer_src;
			for ( auto& v : src_code_info )
			{
				if ( !Cmnx_CpyMem(buffer_tmp + v.offset_to_begin_addr_, v.code_info_.begin_addr_, v.code_info_.GetCodeSize()) )
				{
					assert(false);
				}
			}
		}
	}
	return cnt > 0;
}

HMODULE	g_real_module_ = nullptr;
void CmnX_AntiHookCheck::SetRealMudule(HMODULE h)
{
	g_real_module_ = h;
}

CmnX_AntiHookCheckBase& CmnX_AntiHookCheck::GetRealMe()
{
	static CmnX_AntiHookCheckBase* the_ptr = nullptr;
	if (the_ptr)
		return *the_ptr;
	//auto hlib = LoadLibraryA("lpk.dll");
	if (g_real_module_)
	{
		typedef CmnX_AntiHookCheckBase& (*LpkFunc)();
		auto the_func = (LpkFunc)::GetProcAddress(g_real_module_, "_lpkisoptimiz_");
		if (the_func)
		{
			the_ptr = &the_func();
			assert(the_ptr);
			if (the_ptr)
				return *the_ptr;
		}
	}
	the_ptr = &GetMe();
	assert(the_ptr);
	if (the_ptr)
		return *the_ptr;

	assert(!"不能执行到这里的");
	return *the_ptr;
}

void CmnX_AntiHookCheck::SetLogSink( CmnX_AntiHookLogSink* log_sink )
{
	log_sink_ = log_sink;
}

bool CmnX_AntiHookCheck::AddOneHookCodeInfo( const stHookItemInfo& hook_code_info )
{
	auto the_cont = FindFirstFreeCont();
	if ( !the_cont )
		return false;
	assert(!the_cont->is_full());
	the_cont->push(hook_code_info);
	if (log_sink_)
		log_sink_->OnAddHookCodeInfo(hook_code_info.hooker_name_, hook_code_info.code_info_.begin_addr_, hook_code_info.code_info_.GetCodeSize());
	return true;
}

CmnX_AntiHookCheck::HooksContT* CmnX_AntiHookCheck::FindFirstFreeCont()
{
	auto it_end = hooks_info_.end();
	auto it = hooks_info_.begin();
	for ( ; it != it_end; ++it )
	{
		if ( !it->is_full() )
			return &*it;
	}
	assert(it == it_end);
	if (hooks_info_.is_full())
	{
		assert(false);
		return nullptr;
	}
	hooks_info_.push(HooksContT());
	return &hooks_info_.back();
}

bool CmnX_AntiHookCheck::AddThreadWhiteList( unsigned int thread_id )
{
	::AcquireSRWLockExclusive(&thread_white_list_lock_);
	AntiHookRTTI_ReleaseSRWLockExclusive_ the_rtti(&thread_white_list_lock_);
	for ( auto v : thread_white_list_ )
	{
		if ( v == thread_id )
		{
			assert(false);
			return false;
		}
	}
	thread_white_list_.push(thread_id);
	return true;
}

bool CmnX_AntiHookCheck::IsInThreadWhiteList( unsigned int thread_id ) const
{
	::AcquireSRWLockShared(&thread_white_list_lock_);
	AntiHookRTTI_ReleaseSRWLockShared_ the_rtti(&thread_white_list_lock_);
	for ( auto v : thread_white_list_ )
	{
		if ( v == thread_id )
			return true;
	}
	return false;
}

bool CmnX_AntiHookCheck::EraseThreadWhiteList( unsigned int thread_id )
{
	::AcquireSRWLockExclusive(&thread_white_list_lock_);
	AntiHookRTTI_ReleaseSRWLockExclusive_ the_rtti(&thread_white_list_lock_);
	ThreadWhiteListContT whilte_list_tmp;
	for ( auto v : thread_white_list_ )
	{
		if ( v == thread_id )
			continue;
		whilte_list_tmp.push(v);
	}
	thread_white_list_ = whilte_list_tmp;
	return true;
}

void CmnX_AntiHookCheck::EnableOutputHookName( bool enable )
{
	enable_output_hook_name_ = enable;
}

bool CmnX_AntiHookCheck::AddDirectHookCodeInfo( const char* hooker_name, const void* the_hook_addr, pt_dword the_hook_size, const void* hook_src_info, pt_dword hook_src_info_size /*= kAntiHookCheck_DefHookCpySize */ )
{
	if (IsUnValidPtr(the_hook_addr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(hook_src_info))
	{
		assert(false);
		return false;
	}
	if ( hook_src_info_size <= 0 || the_hook_size <= 0 )
	{
		assert(false);
		return false;
	}
	::AcquireSRWLockExclusive(&hook_lock_);
	AntiHookRTTI_ReleaseSRWLockExclusive_ the_rtti(&hook_lock_);

	CmnX_CodeInfo code_info((void*)the_hook_addr, the_hook_size);
	if (IsFindFirstAddrInfo(code_info))
	{
		assert(false);
		return false;
	}

	stHookItemInfo item_info;
	item_info.code_info_ = code_info;
	item_info.is_out_direct_ = true;
	if ( !Cmnx_CpyMemRecycle(&item_info.src_code_, sizeof(item_info.src_code_), hook_src_info, hook_src_info_size) )
	{
		assert(false);
		return false;
	}
	if (enable_output_hook_name_)
	{
		if ( !hooker_name )
			hooker_name = "";
		if ( !StdEx::TStrCpy(item_info.hooker_name_, hooker_name, sizeof(item_info.hooker_name_)) )
		{
			assert(false);
			return false;
		}
	}

	if (AddOneHookCodeInfo(item_info))
		return true;
	return false;
}

bool CmnX_AntiHookCheck::ExistIntersection(const CmnX_CodeInfo& addr_info, CmnX_CodeInfo* out_intersection) const
{
	::AcquireSRWLockShared(&hook_lock_);
	AntiHookRTTI_ReleaseSRWLockShared_ the_rtti(&hook_lock_);

	for (auto& hooks_cont : hooks_info_)
	{
		for (auto& v : hooks_cont)
		{
			if (v.code_info_.ExistIntersection(addr_info))
			{
				if (out_intersection)
				{
					*out_intersection = v.code_info_.CalcIntersection(addr_info);
					assert(out_intersection->begin_addr_ != out_intersection->end_addr_);
				}
				
				if (log_sink_)
				{
					if (out_intersection)
					{
						log_sink_->OnHookChecked(v.hooker_name_, out_intersection->begin_addr_,
							out_intersection->GetCodeSize(), v.code_info_.begin_addr_, v.code_info_.GetCodeSize());
					}
					else
					{
						const auto& the_intersection = v.code_info_.CalcIntersection(addr_info);
						log_sink_->OnHookChecked(v.hooker_name_, the_intersection.begin_addr_,
							the_intersection.GetCodeSize(), v.code_info_.begin_addr_, v.code_info_.GetCodeSize());
					}
				}
				return true;
			}
		}
	}
	return false;
}

CmnX_AntiHookCheck::stHookItemInfo::stHookItemInfo()
{
	Cmnx_MemSetValue(src_code_, sizeof(src_code_), 0);
	//Cmnx_MemSetValue(hooker_name_, sizeof(hooker_name_), 0);
	StdEx::TStrCpy(hooker_name_, "", sizeof(hooker_name_));
	is_out_direct_ = false;
}

CmnX_AntiHookCheck::stSrcCodeInfo::stSrcCodeInfo()
{
	offset_to_begin_addr_ = -1;
}

CmnX_AntiHookCheckBase::~CmnX_AntiHookCheckBase()
{

}

CmnX_AntiHookLogSink::~CmnX_AntiHookLogSink()
{

}
