#pragma once
/*
@author zhp
@date 2016/1/27 15:55
@purpose hook的一些辅助，比如过CRC
*/
#include "../DependSelfLib.h"
#include "CmnX_StaticConts.h"
#include "CmnX_UsefulClass.h"
#include <Windows.h>

#pragma pack(push)
#pragma pack(1)

//默认要拷贝的hook字节长度
const pt_dword kAntiHookCheck_DefHookCpySize = 20;
//最大拷贝长度
const pt_dword kAntiHookCheck_MaxHookCpySize = 0x30;
//hooker名字的最大长度
const pt_dword kAntiHookCheck_MaxHookerNameSize = 50;
//这个数量不要轻易改变，因为若是太大，可能造成线程堆栈内存不够。请考虑hook ntdll.dll memcpy的情况，最大是不能超过50的
const pt_dword kAntiHookCheck_EveryHookerCnt = 5;
//最大hook的数量 = kAntiHookCheck_EveryHookerCnt * kAntiHookCheck_CntOfEveryHookerCnt
const pt_dword kAntiHookCheck_CntOfEveryHookerCnt = 200;
//线程白名单最大个数，最多kAntiHookCheck_ThreadWhiteListMaxCnt个线程
const pt_dword kAntiHookCheck_ThreadWhiteListMaxCnt = 50;


//用于输出log
class CmnX_AntiHookLogSink{
public:
	virtual ~CmnX_AntiHookLogSink();
	virtual void OnAddHookCodeInfo( const char* hooker_name, const void* the_hook_addr, pt_dword hook_cpy_size ) const = 0;
	virtual void OnHookChecked( const char* hooker_name, const void* check_addr, pt_dword checked_size, const void* hooked_addr, pt_dword hooked_cpy_size ) const = 0;
};

class CmnX_AntiHookCheckBase{
public:
	virtual ~CmnX_AntiHookCheckBase();
	virtual bool AddHookCodeInfo( const char* hooker_name, const void* the_hook_addr, pt_dword hook_cpy_size = kAntiHookCheck_DefHookCpySize ) = 0;
	virtual bool AddDirectHookCodeInfo( const char* hooker_name, const void* the_hook_addr, pt_dword the_hook_size, const void* hook_src_info, pt_dword hook_src_info_size = kAntiHookCheck_DefHookCpySize ) = 0;
	virtual bool OutputSrcCodeInfoByAddr( const void* addr, int src_code_size, void* out_buffer_src ) const = 0;
	virtual void SetLogSink( CmnX_AntiHookLogSink* log_sink ) = 0;
	virtual bool AddThreadWhiteList( unsigned int thread_id ) = 0;
	virtual bool EraseThreadWhiteList( unsigned int thread_id ) = 0;
	virtual bool IsInThreadWhiteList( unsigned int thread_id ) const = 0;
	virtual void EnableOutputHookName( bool enable ) = 0;
	virtual bool ExistIntersection(const CmnX_CodeInfo& addr_info, CmnX_CodeInfo* out_intersection) const = 0;
};

//为了反CRC，反hook检测
class CmnX_AntiHookCheck : public CmnX_AntiHookCheckBase{
private:
	CmnX_AntiHookCheck();

private:
	struct stHookItemInfo{
		unsigned char	src_code_[kAntiHookCheck_MaxHookCpySize];
		char			hooker_name_[kAntiHookCheck_MaxHookerNameSize + 1];
		CmnX_CodeInfo	code_info_;
		bool			is_out_direct_;

		stHookItemInfo();
	};
	typedef CmnStaticQueue<stHookItemInfo, kAntiHookCheck_EveryHookerCnt> HooksContT;
	typedef CmnStaticQueue<HooksContT, kAntiHookCheck_CntOfEveryHookerCnt> TotalHooksContT;
	typedef TotalHooksContT::iterator TotalHooksIteratorT; 
	typedef HooksContT::const_iterator HooksIteratorT;
	typedef CmnStaticQueue<HooksIteratorT, kAntiHookCheck_EveryHookerCnt> CodeInfoContT;
	typedef CmnStaticQueue<unsigned int, kAntiHookCheck_ThreadWhiteListMaxCnt> ThreadWhiteListContT;

public:
	struct stSrcCodeInfo{
		CmnX_CodeInfo		code_info_;
		int					offset_to_begin_addr_;

		stSrcCodeInfo();
	};
	typedef CmnStaticQueue<stSrcCodeInfo, kAntiHookCheck_EveryHookerCnt> SrcCodeInfoContT;

public:
	bool AddHookCodeInfo(const char* hooker_name, const void* the_hook_addr, pt_dword hook_cpy_size = kAntiHookCheck_DefHookCpySize) override;
	bool AddDirectHookCodeInfo(const char* hooker_name, const void* the_hook_addr, 
		pt_dword the_hook_size, const void* hook_src_info, pt_dword hook_src_info_size = kAntiHookCheck_DefHookCpySize) override;
	int AddOther( const CmnX_AntiHookCheck& other );
	bool OutputSrcCodeInfoByAddr(const void* addr, int src_code_size, void* out_buffer_src) const override;
	void SetLogSink(CmnX_AntiHookLogSink* log_sink) override;
	bool AddThreadWhiteList(unsigned int thread_id) override;
	bool EraseThreadWhiteList(unsigned int thread_id) override;
	bool IsInThreadWhiteList(unsigned int thread_id) const override;
	void EnableOutputHookName(bool enable) override;
	bool ExistIntersection(const CmnX_CodeInfo& addr_info, CmnX_CodeInfo* out_intersection) const override;

public:
	static CmnX_AntiHookCheck& GetMe();
	static void SetRealMudule(HMODULE h);
	static CmnX_AntiHookCheckBase& GetRealMe();

private:
	//通过一个地址找到原始地址信息。这个函数不要随便开放出来，一定要考虑线程同步的问题
	static void GenSrcCodeInfoByAddr( const HooksContT& hooks_cont, CmnX_AntiHookLogSink* log_sink, const void* addr, int src_code_size, SrcCodeInfoContT& out_src_code_info, void* out_buffer );
	bool IsFindFirstAddrInfo( const CmnX_CodeInfo& the_code_info ) const;
	static void GetAllAddrInfo( const HooksContT& hooks_cont, const CmnX_CodeInfo& the_code_info, CodeInfoContT& out_code_info, void* out_buffer, CmnX_AntiHookLogSink* log_sink );
	bool AddOneHookCodeInfo( const stHookItemInfo& hook_code_info );
	HooksContT* FindFirstFreeCont();

private:
	TotalHooksContT				hooks_info_;
	ThreadWhiteListContT		thread_white_list_;
	mutable SRWLOCK				hook_lock_;
	mutable SRWLOCK				thread_white_list_lock_;
	CmnX_AntiHookLogSink*		log_sink_;
	bool						enable_output_hook_name_;
};

#pragma pack(pop)
