#include "stdafx.h"
#include <CmnAntiCheck/CmnRbuThread.h>
#include <CmnMix/Cmn_Hooker.h>
#include <CD_CommonFiles/Data/CD_PT_Updates.h>
#include <CD_CommonFiles/Data/CD_Structs.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include <Common/Toolhelp.h>
#include <Common/UsefulClass.h>
#include <afxmt.h>
#include <CommonX/CmnX_DllApi.h>
#include "FuncObjC.h"

//////////////////////////////////////////////////////////////////////////
//*************线程分析笔记************************************************
/*
CryGame.dll会创建2个线程，其中一个很早便中止了。
TenRpcs.dll的线程都处理掉，游戏运行中，有时会卡死，或黑屏。
*/
//////////////////////////////////////////////////////////////////////////
//*********************ntdll*********************
//这是个crc线程。
pt_dword	g_Ntdll_offset = 0x2233B;
//////////////////////////////////////////////////////////////////////////
//*********************TenSLX*********************
//可能是非法线程。会创建3个线程，其中两个线程的地址是一样的。需要全部关掉。
pt_cdword	g_slx_offset1 = 0x1bf994;	//有两个线程
pt_cdword	g_slx_offset2 = 0x3cdb0;
//////////////////////////////////////////////////////////////////////////
//*********************TerSafe*********************
//TerSafe.dll 总共会创建11个线程，其中一个线程执行没几次就终止了，剩下的10个线程是活跃的。其中只有一个线程不能够让它Sleep。
pt_cdword	g_TerSafe_offset1 = 0x42c0;
pt_cdword	g_TerSafe_offset2 = 0x10440;
pt_cdword	g_TerSafe_offset3 = 0x1c990;
pt_cdword	g_TerSafe_offset4 = 0x260a0;
pt_cdword	g_TerSafe_offset5 = 0x2f8c0;
pt_cdword	g_TerSafe_offset6 = 0x73930;
pt_cdword	g_TerSafe_offset7 = 0x75A90;
pt_cdword	g_TerSafe_offset8 = 0x86E5B;
pt_cdword	g_TerSafe_offset9 = 0x86E69;
/*
g_a_lpka_terSafe_CanNotSleep_,干掉这个线程一定会出环境弹框。
然后这个号就被加黑名单了，即便重启系统，并且不注入任何DLL，也会有这个环境弹框。
*/
extern pt_cdword g_a_lpka_terSafe_CanNotSleep_;
//这是一个已终止的线程
//pt_cdword	g_TerSafe_offset11 = 0x70f90;
//////////////////////////////////////////////////////////////////////////
//*********************MHOClientBase*********************
//会创建5个线程，其中一个线程在初期便终止了。
//这是个crc线程。这个线程是切换次数最多的一个，也就是线程入口地址最高的那个。这个线程所做的就是CRC，而且用OD下内存访问断点，有时还断不到。
extern pt_cdword g_a_lpka_cry_base_crc_;
//这两个线程应该不存在问题，因为游戏运行半天了，在副本打怪都好几次了，这两个线程的切换次数一个是2次，另一个是4次。
pt_cdword g_CryBase_thrd1_ = 0x8C9375;		//有两个线程
extern pt_cdword g_a_lpka_cry_base_thrd_;
//////////////////////////////////////////////////////////////////////////
//*********************TCJ*********************
//会创建两个线程，其中一个线程很快便已中止
//线程切换次数高的那个
pt_cdword g_TCJ_thrd1 = 0x8C74D;
//////////////////////////////////////////////////////////////////////////
//*********************TenRpcs*********************
//会创建4个线程
//切换次数第二
//g_TenRpcs_thrd2  g_TenRpcs_thrd3这两个线程选择角色时便会有，选择角色并封号时也会有。
pt_cdword g_TenRpcs_thrd2 = 0xAFD90;
//切换次数第三
pt_cdword g_TenRpcs_thrd3 = 0x54A71;
//下面这两个线程进入游戏后才会有，所以需要放到其他线程去处理。
//切换次数最高
pt_cdword g_TenRpcs_thrd1 = 0x2957;
//切换次数最少
pt_cdword g_TenRpcs_thrd4 = 0x1802D;
//////////////////////////////////////////////////////////////////////////
//*********************MHOClient*********************
extern pt_cdword	g_c_lpka_mhoclient_start_thrd_;
extern pt_cdword	g_c_lpka_mhoclient_start_thrd1_;
extern pt_cdword	g_c_lpka_mhoclient_start_thrd2_;
extern pt_cdword	g_c_lpka_mhoclient_start_thrd3_;
extern pt_cdword	g_c_lpka_mhoclient_no_show_npc_;
extern pt_cdword	g_c_lpka_mhoclient_render_thrd_;
//////////////////////////////////////////////////////////////////////////
//*********************CryGame*********************
//这个线程是可以干掉的
extern pt_cdword	g_c_lpka_crygame_start_thrd_;
//g_c_lpka_crygame_start_thrd1_线程没多久就中止了。
extern pt_cdword	g_c_lpka_crygame_start_thrd1_;
//一般会创建9个线程，运行一会便终止了。
extern pt_cdword	g_c_lpka_crygame_start_thrd2_;
//这个入口地址只有一个线程，很快便中止，怀疑它是用来创建g_c_lpka_crygame_start_thrd2_的。
extern pt_cdword	g_c_lpka_crygame_start_thrd3_;
//////////////////////////////////////////////////////////////////////////
//*********************CrossShell*********************
pt_cdword g_CrossShell_terminate_thrd = 0x7640;
//////////////////////////////////////////////////////////////////////////
//*********************SSOPlatform*********************
//只有一个线程创建没多久便中止了。
pt_cdword g_SSOPlatform_thrd = 0x7CC85;
//////////////////////////////////////////////////////////////////////////
//*********************rasman*********************
//只创建了一个线程。这是个系统dll，可能存在兼容性问题。
//pt_cdword g_Rasman_thrd = 0x32FB;
//////////////////////////////////////////////////////////////////////////
//*********************iips*********************
//只创建了一个线程。
pt_cdword g_Iips_thrd = 0x3BF0;
//////////////////////////////////////////////////////////////////////////
//*********************MMDevApi*********************
//只创建了一个线程。这是个系统dll，可能存在兼容性问题。
//pt_cdword g_MMDevApi_thrd = 0x27E1;
//////////////////////////////////////////////////////////////////////////
//*********************netbios*********************
//只创建了一个线程。这是个系统dll，可能存在兼容性问题。
//pt_cdword g_netbios_thrd = 0x1C2F;
//////////////////////////////////////////////////////////////////////////
struct ModuleOffsetHandlerHlpBase : ModuleOffsetHandler{
	ModuleOffsetHandlerHlpBase(){
		OffsetsT offsets;
		offsets.insert(stOffsetInfoT(0, -1));
		AddModuleOffset("BugTrack.dll", offsets);
		AddModuleOffset("BugReport.dll", offsets);
		AddModuleOffset("BugTrace.dll", offsets);
		AddModuleOffset("SSOPlatform.dll", offsets);
		AddModuleOffset("TenProxy.dll", offsets);
		AddModuleOffset("iips.dll", offsets);
		AddModuleOffset("ifs2.dll", offsets);
		AddModuleOffset("CrossShell.dll", offsets);
		AddModuleOffset("TASWTLoginHelper.dll", offsets);
		/*offsets.clear();
		offsets.insert(stOffsetInfoT(g_c_lpka_crygame_start_thrd2_, -1));
		offsets.insert(stOffsetInfoT(g_c_lpka_crygame_start_thrd3_, -1));*/
		AddModuleOffset("CryGame.dll", offsets);
	}
};
struct ModuleOffsetHandlerHlp_NtCreateThrd : ModuleOffsetHandlerHlpBase{
};
struct ModuleOffsetHandlerHlp_Msvc : ModuleOffsetHandler{
};
ModuleOffsetHandlerHlp_NtCreateThrd	g_do_pre_create_thrd_nt_;
ModuleOffsetHandlerHlp_Msvc	g_do_pre_create_thrd_msvc_;

class HandleRubThread_ByModuleOffset : public HandleRubThread{
};

class HandleRubThread_OnlyDoSleep : public HandleRubThread, public Singleton<HandleRubThread_OnlyDoSleep, Singleton_MakeMe>{
};

#ifdef _DEBUG
struct stRubThrdCntMgr{
	HandleRubThread		mhoclient_start_;
	HandleRubThread		mhoclient_render_;
};
#endif

//HandleMsvcrThrd 这个函数里也有些偏移需要关注
bool DoTraCheckThread(){
	/*
	GetProcAddress(LoadLibraryA("ntdll.dll"),"memcpy") - 5 == 0x20233B + LoadLibraryA("ntdll.dll");
	$ ==>    7783233B                      90              NOP
	$+1      7783233C                      90              NOP
	$+2      7783233D                      90              NOP
	$+3      7783233E                      90              NOP
	$+4      7783233F                      90              NOP
	$+5      77832340 ntdll.memcpy       - E9 66674392     JMP game.09C68AAB                        ; memcpy
	$+A      77832345                      8B75 0C         MOV ESI, DWORD PTR SS:[EBP+C]
	$+D      77832348                      8B4D 10         MOV ECX, DWORD PTR SS:[EBP+10]
	*/
	auto h_ntdll = LoadLibraryA("ntdll.dll");
	if (!h_ntdll)
	{
		assert(false);
		return false;
	}
	auto fn_memcpy = GetProcAddress(h_ntdll, "memcpy");
	if (!fn_memcpy)
	{
		assert(false);
		return false;
	}
	g_Ntdll_offset = (pt_cdword)fn_memcpy - 5 - (pt_cdword)h_ntdll;
	//////////////////////////////////////////////////////////////////////////
	ModuleOffsetHolder module_offset;
	ModuleOffsetHolder::OffsetsT offsets;

	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_a_lpka_cry_base_crc_, 1));
	//offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_CryBase_thrd1_, 2));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_a_lpka_cry_base_thrd_, 1));
	module_offset.AddModuleOffset("MHOClientBase.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_slx_offset1, 2));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_slx_offset2, 1));
	module_offset.AddModuleOffset("TenSLX.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset1, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset2, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset3, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset4, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset5, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset6, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset7, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset8, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TerSafe_offset9, 1));
	//offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_a_lpka_terSafe_CanNotSleep_, 1));
	module_offset.AddModuleOffset("TerSafe.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_Ntdll_offset, 1));
	module_offset.AddModuleOffset("ntdll.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TCJ_thrd1, 1));
	module_offset.AddModuleOffset("TCJ.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TenRpcs_thrd2, 1));
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_TenRpcs_thrd3, 1));
	//暂时注释掉
	//module_offset.AddModuleOffset("TenRpcs.dll", offsets);

	offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_Iips_thrd, 1));
	//module_offset.AddModuleOffset("iips.dll", offsets);

	//这个是系统线程，应该是正常的。
	/*offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_Rasman_thrd, 1));
	module_offset.AddModuleOffset("rasman.dll", offsets);*/

	//这个是系统线程，应该是正常的。
	/*offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_MMDevApi_thrd, 1));
	module_offset.AddModuleOffset("MMDevApi.dll", offsets);*/

	//不是每台机器都会有这个线程的。
	/*offsets.clear();
	offsets.insert(ModuleOffsetHolder::stOffsetInfoT(g_netbios_thrd, 1));
	module_offset.AddModuleOffset("netbios.dll", offsets);*/
	//////////////////////////////////////////////////////////////////////////
	LOG_O(Log_debug) << "开始遍历所有垃圾线程";
	HandleRubThread_ByModuleOffset rub_thrd;
	auto res = rub_thrd.DoAllThrdSleepByOffset(1000 * 60 * 3, module_offset, DummySleep);
	rub_thrd.ClrAll();
	LOG_O(Log_debug) << "所有垃圾线程处理结果:" << res;
	return res;
	//////////////////////////////////////////////////////////////////////////
}

CEvent*	g_event_handle_rub_thrd_ = nullptr;
bool g_handle_rub_thrd_end_ = false;
#ifdef _DEBUG
stRubThrdCntMgr	g_rub_thrd_cnt_mgr_;
#endif

void BeginHandleRubThrd(){
	if (!g_event_handle_rub_thrd_)
	{
		assert(false);
		return;
	}
	g_event_handle_rub_thrd_->SetEvent();
}
void HandleSleepRubThrd(CEvent* the_event){
	assert(the_event);
	the_event->Lock();
	the_event->Unlock();
	LOG_O(Log_trace) << "开始处理那些创建的垃圾线程";
	auto& rub_thrd = HandleRubThread_OnlyDoSleep::GetMe();
	assert(4 == g_rub_thrd_cnt_mgr_.mhoclient_render_.GetThrdTotalCnt());

	auto cnt = rub_thrd.GetThrdTotalCnt();
	int succeed_cnt = 0;
	auto res = TimerDo_Sleep(100, 1000 * 60, [&rub_thrd, &succeed_cnt](){
		succeed_cnt += rub_thrd.DoThrdSleep();
		return rub_thrd.GetUnhandledThrdCnt() <= 0;
	});
	if (res)
		LOG_O(Log_trace) << "那些创建的垃圾线程处理完毕，总个数：" << cnt << " 成功个数：" << succeed_cnt;
	else
	{
		LOG_O(Log_debug) << "那些创建的垃圾线程未能处理成功";
		assert(false);
		GetIoService().post([](){
			throw CmnTerminateProcessException();
		});
	}

	HandleRubThread_ByModuleOffset the_rub_thrd;
	ModuleOffsetHolder the_module_offset;
	ModuleOffsetHolder::OffsetsT the_offsets;
	//把主线程给干掉。MHOClient.exe只有一个线程，就是主线程，其他线程都是用_beginthread创建的。
	the_offsets.insert(ModuleOffsetHolder::stOffsetInfoT(0, -1));
	the_module_offset.AddModuleOffset("MHOClient.exe", the_offsets);

	LOG_O(Log_debug) << "开始遍历剩下的垃圾线程";
	auto the_res = the_rub_thrd.DoAllThrdSleepByOffset(1000 * 60 * 2, the_module_offset, ::Sleep);
	//判断这个主线程在MHOClient.exe模块中是否唯一
	if (the_res)
	{
		/*if (!TimerDo_Sleep(500, 5000, [&](){
			if (the_rub_thrd.DoAllThrdSleepByOffset(1000, the_module_offset, ::Sleep))
				return false;
			return true;
		}))*/
		if (the_rub_thrd.DoAllThrdSleepByOffset(1000, the_module_offset, ::Sleep))
			assert(!"剩下的垃圾线程竟然还有");
	}
	the_rub_thrd.ClrAll();
	if (the_res)
	{
		LOG_O(Log_debug) << "剩下的垃圾线程处理成功";
	}
	else
	{
		LOG_O(Log_debug) << "剩下的垃圾线程未能处理成功";
		assert(false);
		GetIoService().post([](){
			throw CmnTerminateProcessException();
		});
	}

	while (true)
	{
		rub_thrd.DoThrdSleep();
		::Sleep(3000);
	}

	rub_thrd.ClrAll();
}

void HandleAddRubThrdSleep(DWORD thrd_id, void* start_addr, pt_csz modue_name){
	if (g_handle_rub_thrd_end_)
	{
		assert(false);
		return;
	}
	LOG_O(Log_trace) << "HandleAddRubThrdSleep，线程ID：" << std::dec << thrd_id << " start_addr:" << start_addr << " 模块名：" << modue_name;
	auto& rub_thrd = HandleRubThread_OnlyDoSleep::GetMe();
	rub_thrd.AddThrdId(thrd_id);
}

void HandleBeginThrd_MhoClient_Start(stCD_Common_ThrdStart* thrd_obj, DWORD thrd_id)
{
	if (IsUnValidPtr(thrd_obj->vtbl_))
	{
		assert(false);
		return;
	}
	auto the_run_fn = (pt_cdword)thrd_obj->vtbl_->fn_run_thrd_;
	
	if (g_c_lpka_mhoclient_no_show_npc_ == the_run_fn)
	{
		//总共创建了10个线程，这些线程都可以干掉，但是在城镇中玩家只能显示名字和称号，在副本中的NPC只有头可显示，怪物是正常的。
		//不能全搞掉，否则 梦的起点 这个任务 新手香猪菇 打不到，还有其他副本的BOSS也打不到。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run0");
	}
#ifdef _DEBUG
	else if (g_c_lpka_mhoclient_render_thrd_ == the_run_fn)
	{
		//总共创建了4个线程，全都搞了就会黑屏。猜它应该是渲染线程，4个同时渲染，只要有一个正常便没事。本以为搞3个没事，其实不然，只搞3个启动游戏后直接黑屏。
		g_rub_thrd_cnt_mgr_.mhoclient_render_.AddThrdId(thrd_id);
		auto cnt = g_rub_thrd_cnt_mgr_.mhoclient_render_.GetThrdTotalCnt();
		assert(cnt <= 4);
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run1");
	}
#endif
	//else if (0x0045E120 == the_run_fn)
	{
		//只会创建一个线程，这个线程还不能搞，否则黑屏。如果把它搞掉，加载地图的时候有问题，而平常没问题。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run2");
	}
	//else if (0x00504920 == the_run_fn)
	{
		//总共创建了3个线程，全都不能搞，否则黑屏。过图的时候也会黑屏。三个线程中的任何一个被搞，都会黑屏。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run3");
	}
	//else if (0x00504600 == the_run_fn)
	{
		//总共创建了2个线程，这两个也不能搞，虽然角色界面没问题，但是进入游戏的时候会黑屏。过图的时候也会黑屏。两个线程中的任何一个被搞，都会黑屏。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run4");
	}
	//else if (0x0089BFB0 == the_run_fn)
	{
		//总共创建了一个线程，不能搞掉，虽然选择角色界面没问题，但进入游戏时，会卡在loading的地方。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run5");
	}
	//else if (0x00874910 == the_run_fn)
	{
		//总共创建了一个线程，不能搞掉，虽然选择角色界面没问题，但进入游戏时，会卡在loading的地方。
		//return HandleAddRubThrdSleep(thrd_id, (pt_pvoid)the_run_fn, "MHOClient.exe_run6");
	}

	LOG_O(Log_trace) << "HandleBeginThrd_MhoClient_Start:" << thrd_obj << " thrd_id:" << std::dec << thrd_id
		<< std::hex
		<< " vtbl:" << thrd_obj->vtbl_ << " run_fn:" << thrd_obj->vtbl_->fn_run_thrd_ <<
		" on_exit:" << thrd_obj->vtbl_->fn_on_exit_thrd_;
}

void HandleBeginThrd_CryGame_Start(stCD_CryGame_ThrdStart* thrd_obj, void* start_addr, DWORD thrd_id)
{
	LOG_O(Log_trace) << "HandleBeginThrd_CryGame_Start:" << thrd_obj << " thrd_id:" << std::dec << thrd_id
		<< std::hex
		<< " start_addr:" << start_addr
		<< " vtbl:" << thrd_obj->vtbl_ << " run_fn:" << thrd_obj->vtbl_->fn_run_thrd_;
}

void HandleBeginThrd_CryGame_Start1(stCD_Common_ThrdStart* thrd_obj, void* start_addr, DWORD thrd_id)
{
	LOG_O(Log_trace) << "HandleBeginThrd_CryGame_Start1:" << thrd_obj << " thrd_id:" << std::dec << thrd_id
		<< std::hex
		<< " start_addr:" << start_addr
		<< " vtbl:" << thrd_obj->vtbl_ << " run_fn:" << thrd_obj->vtbl_->fn_run_thrd_ <<
		" on_exit:" << thrd_obj->vtbl_->fn_on_exit_thrd_;
}

/*
Cmnx_Mutex_CriticalSection g_cri_section_thrd_id_;
NameList<int, std::vector<int> > g_the_thrd_ids_;*/
void HandleMsvcrThrd(void* start_addr, DWORD thrd_id, unsigned stack_size, void* arg_list,
	boost::optional<void*> security, boost::optional<unsigned> init_flag, boost::optional<unsigned*> ptr_thrd_id)
{
	auto hmodule = ModuleFromAddress(start_addr);
	if (!hmodule)
	{
		assert(false);
		return;
	}
	char buffer[MAX_PATH];
	if (!GetModuleName(hmodule, buffer, sizeof(buffer)))
		return;

	const auto addr_offset = (pt_cdword)start_addr - (pt_cdword)hmodule;

	/*BOOST_SCOPE_EXIT_ALL(thrd_id){
		Cmnx_Lockable lock(g_cri_section_thrd_id_);
		auto old_size = g_the_thrd_ids_.Size();
		g_the_thrd_ids_.Add(thrd_id);
		auto new_size = g_the_thrd_ids_.Size();
		if (old_size == new_size)
		{
			LOG_O(Log_debug) << "已经存在该线程ID了：" << std::dec << thrd_id;
		}
		else
		{
			LOG_O(Log_trace) << "现在的线程数量为：" << std::dec << new_size;
		}
	};*/

	if (0 == _stricmp(buffer, "MHOClient.exe"))
	{
		if (!IsUnValidPtr(arg_list))
		{
			if (g_c_lpka_mhoclient_start_thrd_ == (pt_cdword)start_addr)
				return HandleBeginThrd_MhoClient_Start((stCD_Common_ThrdStart*)arg_list, thrd_id);
			else if (g_c_lpka_mhoclient_start_thrd1_ == (pt_cdword)start_addr)
			{
				//这个入口创建了三个线程，这三个线程好像都不活跃，进到游戏里面了，也没运行过。
				//return HandleBeginThrd_MhoClient_Start1((stCD_MhoClient_ThrdStart1*)arg_list, thrd_id);
				return HandleAddRubThrdSleep(thrd_id, start_addr, "MHOClient.exe_start_thrd1");
			}
			else if (g_c_lpka_mhoclient_start_thrd2_ == (pt_cdword)start_addr)
			{
				//这个入口创建了七个线程，在选择角色界面时这七个线程就已经全都死了。
				//return HandleBeginThrd_MhoClient_Start2((stCD_MhoClient_ThrdStart2*)arg_list, thrd_id);
				return HandleAddRubThrdSleep(thrd_id, start_addr, "MHOClient.exe_start_thrd2");
			}
			else if (g_c_lpka_mhoclient_start_thrd3_ == (pt_cdword)start_addr)
			{
				//这个入口只创建了一个线程，这个线程好像不活跃，进到游戏里面了，也没运行过。
				//return HandleBeginThrd_MhoClient_Start3((stCD_MhoClient_ThrdStart3*)arg_list, thrd_id);
				return HandleAddRubThrdSleep(thrd_id, start_addr, "MHOClient.exe_start_thrd3");
			}
			assert(false);
		}
		else
		{
			//只有一个线程，入口函数不远处就有："计算机内存小于2G，不满足游戏正常运行所需的配置要求，是否继续运行游戏？"。
			//这个线程不能搞，否则会黑屏，可能是渲染线程。
			//这个线程经常会调用NtQueryInformationProcess(ProcessBasicInformation)，怀疑这个线程不只是起到窗口线程的作用。
			//return HandleAddRubThrdSleep(thrd_id, start_addr, "MHOClient.exe_start_thrd_else");
		}
#ifdef _DEBUG
		g_rub_thrd_cnt_mgr_.mhoclient_start_.AddThrdId(thrd_id);
		assert(g_rub_thrd_cnt_mgr_.mhoclient_start_.GetThrdTotalCnt() <= 12 + 1);
#endif
	}
	else if (0 == _stricmp(buffer, "CryGame.dll"))
	{
		//总共会创建两个线程。
		assert((pt_cdword)hmodule == CDF_GetCryGameBaseAddr());
		//g_c_lpka_crygame_start_thrd1_线程没多久就中止了。
		if (g_c_lpka_crygame_start_thrd1_ != addr_offset)
		{
			return HandleAddRubThrdSleep(thrd_id, start_addr, "CryGame.dll");
		}
		/*else if (g_c_lpka_crygame_start_thrd1_ == addr_offset)
		{
			return HandleBeginThrd_CryGame_Start1((stCD_Common_ThrdStart*)arg_list, start_addr, thrd_id);
		}
		else if (g_c_lpka_crygame_start_thrd_ == addr_offset)
		{
			return HandleBeginThrd_CryGame_Start((stCD_CryGame_ThrdStart*)arg_list, start_addr, thrd_id);
			return HandleAddRubThrdSleep(thrd_id, start_addr, "CryGame.dll");
		}*/
	}
	else if (0 == _stricmp(buffer, "iips.dll"))
	{
		return HandleAddRubThrdSleep(thrd_id, start_addr, "iips.dll");
	}
	else if (0 == _stricmp(buffer, "CrossShell.dll"))
	{
		if (g_CrossShell_terminate_thrd != addr_offset)
			return HandleAddRubThrdSleep(thrd_id, start_addr, "CrossShell.dll");
	}
	else if (0 == _stricmp(buffer, "CryAction.dll"))
	{
		return HandleAddRubThrdSleep(thrd_id, start_addr, "CryAction.dll");
	}

	if (security)
	{
		LOG_O(Log_debug) << "游戏创建了线程，start_addr:" << start_addr << " thrd_id:" << std::dec << thrd_id << " 模块：" << buffer
			<< " stack_size:" << stack_size << " arg_list:" << arg_list <<
			" security:" << *security << " init_flag:" << *init_flag << " ptr_thrd_id:" << *ptr_thrd_id;
	}
	else
	{
		LOG_O(Log_debug) << "游戏创建了线程，start_addr:" << start_addr << " thrd_id:" << std::dec << thrd_id << " 模块：" << buffer
			<< " stack_size:" << stack_size << " arg_list:" << arg_list;
	}
}

void* Hooker_beginthread(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto start_addr = ambient.EspParamAs<1, void*>();
	if (IsUnValidPtr(start_addr))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	typedef void(*StartFn)(void*);
	typedef uintptr_t (__cdecl *Fn_beginthread)(_In_ void(__cdecl* _StartAddress) (void *), _In_ unsigned _StackSize, _In_opt_ void * _ArgList);
	auto the_fn = (Fn_beginthread)hooker.GetCodeOrig();
	auto stack_size = ambient.EspParamAs<2, unsigned>();
	auto arg_list = ambient.EspParamAs<3, void*>();
	char module_name[MAX_PATH];
	auto pre_res = g_do_pre_create_thrd_msvc_.DoPreCreateThrd_Cdecl((StartFn&)start_addr, arg_list, nullptr, module_name, MAX_PATH);
	auto res = the_fn((StartFn)start_addr, stack_size, arg_list);
	ambient.eax_ = res;

	if (!pre_res)
	{
		if (res)
			HandleMsvcrThrd(start_addr, GetThreadId((HANDLE)res), stack_size, arg_list,
			boost::optional<void*>(), boost::optional<unsigned>(), boost::optional<unsigned*>());
		else
			HandleMsvcrThrd(start_addr, 0, stack_size, arg_list,
			boost::optional<void*>(), boost::optional<unsigned>(), boost::optional<unsigned*>());
	}
	return NakedRetP0;
}

void* Hooker_beginthreadex(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto start_addr = ambient.EspParamAs<3, void*>();
	if (IsUnValidPtr(start_addr))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	typedef unsigned(__stdcall* StartFn) (void *);
	typedef uintptr_t(__cdecl *Fn_beginthread)(_In_opt_ void * _Security, _In_ unsigned _StackSize,
		_In_ unsigned(__stdcall * _StartAddress) (void *), _In_opt_ void * _ArgList,
		_In_ unsigned _InitFlag, _Out_opt_ unsigned * _ThrdAddr);
	auto the_fn = (Fn_beginthread)hooker.GetCodeOrig();
	unsigned thrd_id = 0;
	auto security = ambient.EspParamAs<1, void*>();
	auto stack_size = ambient.EspParamAs<2, unsigned>();
	auto arg_list = ambient.EspParamAs<4, void*>();
	auto init_flag = ambient.EspParamAs<5, unsigned>();
	char module_name[MAX_PATH];
	auto pre_res = g_do_pre_create_thrd_msvc_.DoPreCreateThrd_Std((StartFn&)start_addr, arg_list, nullptr, module_name, MAX_PATH);
	auto res = the_fn(security, stack_size, (StartFn)start_addr, arg_list, init_flag, &thrd_id);
	auto ptr_thrd_id = ambient.EspParamAs<6, unsigned*>();
	if (ptr_thrd_id)
		*ptr_thrd_id = thrd_id;
	ambient.eax_ = res;

	if (!pre_res)
	{
		if (res)
			HandleMsvcrThrd(start_addr, thrd_id, stack_size, arg_list, security, init_flag, ptr_thrd_id);
		else
			HandleMsvcrThrd(start_addr, 0, stack_size, arg_list, security, init_flag, ptr_thrd_id);
	}
	return NakedRetP0;
}

void HandleNtCreateThread(char module_name[MAX_PATH], HMODULE the_module, void* start_addr, DWORD thrd_id){
	if (0 == _stricmp(module_name, "TenRpcs.dll"))
	{
		auto addr_offset = (pt_cdword)start_addr - (pt_cdword)the_module;
		if (g_TenRpcs_thrd4 == addr_offset)
		{
			HandleAddRubThrdSleep(thrd_id, start_addr, "TenRpcs.dll g_TenRpcs_thrd4");
		}
		else if (g_TenRpcs_thrd1 == addr_offset)
		{
			HandleAddRubThrdSleep(thrd_id, start_addr, "TenRpcs.dll g_TenRpcs_thrd1");
		}
	}
}

stDetourHooker<NtCreateThread> hooker_info_NtCreateThread_;
NTSTATUS_ WINAPI Hooker_NtCreateThread(                     // <相关参数说明>
	__out PHANDLE ThreadHandle,     //返回创建线程的句柄
	__in ACCESS_MASK DesiredAccess, //对新线程的访问权限
	__in_opt POBJECT_ATTRIBUTES ObjectAttributes, //指定了线程对象的属性
	__in HANDLE ProcessHandle,      //进程句柄
	__out PCLIENT_ID ClientId,      //返回新线程的ClientId 结构
	__in PCONTEXT ThreadContext,    //新线程的执行环境
	__in PINITIAL_TEB InitialTeb,   //提供新线程的TEB初始值
	__in BOOLEAN CreateSuspended    //新创建的线程是否要先被挂起
	){
	assert(false);
	HANDLE thrd_handle = INVALID_HANDLE_VALUE;
	//auto pre_res = HandlePreCreateThread(buffer, the_module, (void*&)StartRoutine, (pt_dword&)StartContext);
	auto res = hooker_info_NtCreateThread_.code_orig_(&thrd_handle, DesiredAccess, ObjectAttributes, ProcessHandle, ClientId, ThreadContext, InitialTeb, CreateSuspended);
	if (ThreadHandle)
		*ThreadHandle = thrd_handle;
	if (ThreadContext)
	{
		auto the_module = ModuleFromAddress((void*)ThreadContext->Eax);
		char buffer[MAX_PATH];
		buffer[0] = 0;
		GetModuleName(the_module, buffer, MAX_PATH);
		HandleNtCreateThread(buffer, the_module, (void*)ThreadContext->Eax, GetThreadId(thrd_handle));
	}
	return res;
}

stDetourHooker<NtCreateThreadEx> hooker_info_NtCreateThreadEx_;
NTSTATUS_ WINAPI Hooker_NtCreateThreadEx(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN HANDLE ProcessHandle,
	IN PTHREAD_START_ROUTINE StartRoutine,
	IN PVOID StartContext,
	IN ULONG CreateThreadFlags,
	IN ULONG ZeroBits OPTIONAL,
	IN ULONG StackSize OPTIONAL,
	IN ULONG MaximumStackSize OPTIONAL,
	IN PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList
	){
	HANDLE thrd_handle = INVALID_HANDLE_VALUE;
	HMODULE the_module = nullptr;
	char the_module_name[MAX_PATH];
	auto pre_res = g_do_pre_create_thrd_nt_.DoPreCreateThrd_Std((ModuleOffsetHandler::StartFn_Std&)StartRoutine, (void*&)StartContext, 
		&the_module, the_module_name, MAX_PATH);
	auto res = hooker_info_NtCreateThreadEx_.code_orig_(&thrd_handle, DesiredAccess, ObjectAttributes, ProcessHandle, 
		StartRoutine, StartContext, CreateThreadFlags,
		ZeroBits, StackSize, MaximumStackSize, AttributeList);
	if (ThreadHandle)
		*ThreadHandle = thrd_handle;
	if (StartRoutine && !pre_res)
		HandleNtCreateThread(the_module_name, the_module, StartRoutine, GetThreadId(thrd_handle));

#ifdef _DEBUG
	if (!pre_res)
	{
		if (0 != _stricmp(the_module_name, "MSVCR90.dll") && 0 != _stricmp(the_module_name, "MSVCR80.dll") && 0 != _stricmp(the_module_name, "msvcrt.dll")
			&& the_module != GlobalSetter::GetMe().GetSelfModuleHandle())
			LOG_O(Log_trace) << "Hooker_NtCreateThreadEx创建线程，入口地址：" << (void*)StartRoutine << " 模块名：" << the_module_name;
	}
#endif
	return res;
}

stDetourHooker<NtQueryInformationProcessT> g_hooker_NtQueryInformationProcess_;
NTSTATUS_ WINAPI Hooker_NtQueryInformationProcess(IN HANDLE ProcessHandle, //进程句柄
	IN PROCESSINFOCLASS InformationClass, //信息类型
	OUT PVOID ProcessInformation, //缓冲指针
	IN ULONG ProcessInformationLength, //以字节为单位的缓冲大小
	OUT PULONG ReturnLength OPTIONAL //写入缓冲的字节数
	){
	//处理了NtQueryInformationProcess后，游戏走起路来，或打起怪来，就显得有点卡了，但是CPU并未增高。
	//ProcessBasicInformation == InformationClass 去掉后，就不卡了
	if (/*ProcessBasicInformation == InformationClass ||*/ ProcessDebugPort == InformationClass)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
		{
#ifdef _DEBUG
			if (ProcessDebugPort == InformationClass)
			{
				LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，ProcessDebugPort 线程ID：" << thrd_id;
				return -1;
			}
			DWORD gwnd_thrd_id = 0;
			auto nor = FuncObj_NormalC::GetInstance();
			if (nor)
			{
				auto gwnd = nor->GetInterGWnd();
				if (gwnd)
					gwnd_thrd_id = gwnd->GetWndThrdId();
			}			
			if (gwnd_thrd_id != thrd_id || ProcessBasicInformation != InformationClass)
				LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，InformationClass:" << InformationClass << " 线程ID：" << thrd_id;
#endif
			return -1;
		}
	}
	else if (ProcessImageFileName == InformationClass)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
		{
			ULONG ret_length = 0;
			auto res = g_hooker_NtQueryInformationProcess_.code_orig_(ProcessHandle, InformationClass, ProcessInformation, ProcessInformationLength, &ret_length);
			if (NT_SUCCESS(res))
			{
				if (Cmnx_Stristr((wchar_t*)ProcessInformation, L"Console.exe"))
				{
					LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，因为遍历到了Console.exe 线程ID：" << thrd_id;
					if (ReturnLength)
						*ReturnLength = 0;
					return -1;
				}
			}
			if (ReturnLength)
				*ReturnLength = ret_length;
			return res;
		}
	}
	return g_hooker_NtQueryInformationProcess_.code_orig_(ProcessHandle, InformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
}

void* Hooker_Sprintf(IHooker& hooker, JmpHookerAmbient& ambient)
{
	static bool exec_once = false;
	if (exec_once)
		return hooker.GetCodeOrig();
	//JFZRBASE_SPRINTF
	struct TheSprintfP{
		char*	pScrCur;
		int		nRemainBufLen;
		char*	pScrBuf;
	};
	if (IsUnValidPtr(ambient.edi_))
	{
		//assert(false);
		return hooker.GetCodeOrig();
	}
	auto the_param = (TheSprintfP*)ambient.edi_;
	//LOG_O(Log_trace) << "Hooker_Sprintf, the_str:" << std::string(the_param->pScrBuf, 10);
	if (strstr(the_param->pScrBuf, "Global-Event-") != 0)
	{
		/*
		原来是：\Sessions\1\BaseNamedObjects\Global-Event-0000002E0systemevent
		修改后：\Sessions\1\BaseNamedObjects\Global-Event-0000183C0000002E0systemevent
		*/
		exec_once = true;
		char szProcessId[256] = { 0 };
		sprintf_s(szProcessId, "%p", GetCurrentProcessId());

		int nLen = strlen(szProcessId);
		memcpy(the_param->pScrCur, szProcessId, nLen);
		the_param->nRemainBufLen -= nLen;
		the_param->pScrCur += nLen;
	}
	return hooker.GetCodeOrig();
}

void* Hooker_CpuId(IHooker& hooker, JmpHookerAmbient& ambient)
{
	LOG_O(Log_trace) << "Hooker_CpuId, eax:" << ambient.eax_;
	assert(false);
	return hooker.GetCodeOrig();
}

void* Hooker_GetUserDefaultLCID(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto the_call_addr = ambient.EspStackValueAs<void*>(0);
	if (IsUnValidPtr(the_call_addr))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() == the_module)
		return hooker.GetCodeOrig();

	MEMORY_BASIC_INFORMATION memory_basic = { 0 };
	VirtualQueryEx(INVALID_HANDLE_VALUE, the_call_addr, &memory_basic, sizeof(memory_basic));
	if (PAGE_READWRITE != memory_basic.AllocationProtect)
		return hooker.GetCodeOrig();
	LOG_O(Log_trace) << "Hooker_GetUserDefaultLCID扣押线程，线程ID:" << std::dec << GetCurrentThreadId() << " 返回地址：" << the_call_addr;
	while (true)
	{
		Sleep(1000 * 60 * 5);
	}
	return hooker.GetCodeOrig();
}

void* Hooker_TimeGetTime(IHooker& hooker, JmpHookerAmbient& ambient)
{
	return hooker.GetCodeOrig();
}

void* Hooker_ZwOpenProcess(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto ProcessHandle = ambient.EspParamAs<1, PHANDLE>();
	auto DesiredAccess = ambient.EspParamAs<2, ACCESS_MASK>();
	auto ObjectAttributes = ambient.EspParamAs<3, POBJECT_ATTRIBUTES>();
	auto ClientId = ambient.EspParamAs<4, PCLIENT_ID>();
	auto real_fn = (ZwOpenProcess)hooker.GetCodeOrig();
	if (IsUnValidPtr(ClientId))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() == the_module)
		return hooker.GetCodeOrig();
	//////////////////////////////////////////////////////////////////////////
	CLIENT_ID theClientId;
	theClientId.UniqueProcess = ClientId->UniqueProcess;
	theClientId.UniqueThread = ClientId->UniqueThread;

	PVOID pProcessId = (PVOID)0;
	if (ClientId->UniqueProcess == pProcessId)
	{
		theClientId.UniqueProcess = (PVOID)(8);
		theClientId.UniqueThread = 0;

#ifdef _DEBUG
		char buffer[MAX_PATH];
		if (GetModuleName(the_module, buffer, sizeof(buffer)))
		{
			LOG_O(Log_debug) << "Hooker_ZwOpenProcess扣押线程，线程ID：" << std::dec << thrd_id << " 模块名：" << buffer;
		}
#endif

		while (true)
		{
			Sleep(1000 * 60 * 10);
		}
	}
	auto res = real_fn(ProcessHandle, DesiredAccess, ObjectAttributes, &theClientId);
	ambient.eax_ = res;
	//////////////////////////////////////////////////////////////////////////
	return NakedRetP4;
}

void* Hooker_ZwQuerySystemInformation(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto real_fn = (ZwQuerySystemInformation)hooker.GetCodeOrig();
	auto SystemInformationClass = ambient.EspParamAs<1, SYSTEM_INFORMATION_CLASS>();
	auto SystemInformation = ambient.EspParamAs<2, PVOID>();
	if (IsUnValidPtr(SystemInformation))
		return hooker.GetCodeOrig();
	auto SystemInformationLength = ambient.EspParamAs<3, ULONG>();
	auto ReturnLength = ambient.EspParamAs<4, PULONG>();

	auto status = real_fn(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	ambient.eax_ = status;
	auto pInfo = (PSYSTEM_PROCESSES)SystemInformation;

	if (SystemModuleInformation == SystemInformationClass)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
		{
			LOG_O(Log_debug) << "Hooker_ZwQuerySystemInformation遍历模块" << " thrd_id:" << GetCurrentThreadId();
			PSYSTEM_MODULE_INFORMATION pModle = (PSYSTEM_MODULE_INFORMATION)SystemInformation;
			if (pModle != nullptr)
			{
				pModle->ModulesCount = 1;
				SystemInformation = nullptr;
			}
			else
			{
				//OutputDebugString("[LOG] 遍历模块 SystemInformation = NULL");
			}
		}
	}
	else if (SystemProcessInformation == SystemInformationClass && status != 0x0C0000004)
	{
		while (true)
		{
			if (pInfo->NextEntryDelta == 0)
			{
				break;
			}

			pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);

			wchar_t* pszProcessName = ((wchar_t*)(&pInfo->ProcessName) + 2);

			if (Cmnx_Stristr((wchar_t*)*(DWORD*)pszProcessName, L"Console.exe"))
			{
				auto thrd_id = GetCurrentThreadId();
				auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
				if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
				{
					//pInfo->NextEntryDelta = 0;
					pInfo->ProcessId = 0;
					__asm
					{
						lea eax, pszProcessName
						mov eax, dword ptr[eax]
						mov dword ptr[eax], 0
					}
					/*char buffer[MAX_PATH];
					auto thrd_id = GetCurrentThreadId();
					if (GetAddressModuleName(GetThreadStartAddr(thrd_id), buffer, sizeof(buffer)))
					{
					LOG_O(Log_debug) << "Hooker_ZwQuerySystemInformation成功的干掉了一次检测进程，线程ID：" << std::dec << thrd_id << " 模块名：" << buffer;
					}*/
				}
			}
		}
	}
	/*else if (SystemProcessInformation == SystemInformationClass)
	{
	}
	else if( SystemBasicInformation == SystemInformationClass )
	{
		OutputDebugString("[LOG] 获取系统信息");
		PSYSTEM_BASIC_INFORMATION pBasic = (PSYSTEM_BASIC_INFORMATION)SystemInformation;
		pBasic->NumberProcessors = 4;
	}*/
	return NakedRetP4;
}

stDetourHooker<NtCreateFile> g_hooker_NtCreateFile;
LONG NTAPI Hooker_NtCreateFile(
	OUT PHANDLE             FileHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PLARGE_INTEGER       AllocationSize OPTIONAL,
	IN ULONG                FileAttributes,
	IN ULONG                ShareAccess,
	IN ULONG                CreateDisposition,
	IN ULONG                CreateOptions,
	IN PVOID                EaBuffer OPTIONAL,
	IN ULONG                EaLength){
	if (ObjectAttributes->ObjectName->Buffer != NULL && ObjectAttributes->ObjectName->MaximumLength > 0)
	{
		auto pwszPatch = Cmnx_Stristr(ObjectAttributes->ObjectName->Buffer, L"Console.exe");
		if (pwszPatch)
		{
			auto thrd_id = GetCurrentThreadId();
			auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
			if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
			{
#ifdef _DEBUG
				char buffer[MAX_PATH];
				if (GetModuleName(the_module, buffer, sizeof(buffer)))
				{
					LOG_O(Log_debug) << "Hooker_NtCreateFile发现检测控制台" << " 线程ID：" << thrd_id << " 模块名：" << buffer;
				}
#endif
				*(wchar_t*)pwszPatch = L'D';
				LONG lRet = g_hooker_NtCreateFile.code_orig_(FileHandle, DesiredAccess, ObjectAttributes,
					IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
				*(wchar_t*)pwszPatch = L'C';
				return lRet;
			}			
		}
	}

	return g_hooker_NtCreateFile.code_orig_(FileHandle, DesiredAccess, ObjectAttributes, 
		IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
}

stDetourHooker<RtlQueryProcessDebugInformation> g_hooker_RtlQueryProcessDebugInformation;
HANDLE WINAPI Hooker_RtlQueryProcessDebugInformation(DWORD th32ProcessID, DWORD dwFlags, LPVOID Buffer)
{
	//CreateToolhelp32Snapshot底层会调用RtlQueryProcessDebugInformation
	if (dwFlags != 4)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
		{
			LOG_O(Log_debug) << "Hooker_RtlQueryProcessDebugInformation 非线程遍历，直接返回失败！线程ID：" << std::dec << thrd_id;
			return INVALID_HANDLE_VALUE;
		}
	}
	return g_hooker_RtlQueryProcessDebugInformation.code_orig_(th32ProcessID, dwFlags, Buffer);
}

stDetourHooker<decltype(CreateToolhelp32Snapshot)*> g_hooker_CreateToolhelp32Snapshot_;
HANDLE WINAPI Hooker_CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		LOG_O(Log_debug) << "Hooker_CreateToolhelp32Snapshot 直接返回失败，线程ID：" << std::dec << thrd_id;
		return INVALID_HANDLE_VALUE;
	}
	return g_hooker_CreateToolhelp32Snapshot_.code_orig_(dwFlags, th32ProcessID);
}

typedef BOOL (WINAPI* EnumProcessesFnT)(_Out_  DWORD *pProcessIds, _In_   DWORD cb, _Out_  DWORD *pBytesReturned);
stDetourHooker<EnumProcessesFnT> g_hooker_EnumProcesses_;
BOOL WINAPI Hooker_EnumProcesses(_Out_  DWORD *pProcessIds, _In_   DWORD cb, _Out_  DWORD *pBytesReturned){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		LOG_O(Log_debug) << "Hooker_EnumProcesses 直接返回失败，线程ID：" << std::dec << thrd_id;
		return FALSE;
	}
	return g_hooker_EnumProcesses_.code_orig_(pProcessIds, cb, pBytesReturned);
}

stDetourHooker<EnumProcessesFnT> g_hooker_EnumProcesses1_;
BOOL WINAPI Hooker_EnumProcesses1(_Out_  DWORD *pProcessIds, _In_   DWORD cb, _Out_  DWORD *pBytesReturned){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		LOG_O(Log_debug) << "Hooker_EnumProcesses1 直接返回失败，线程ID：" << std::dec << thrd_id;
		return FALSE;
	}
	return g_hooker_EnumProcesses1_.code_orig_(pProcessIds, cb, pBytesReturned);
}

typedef BOOL(WINAPI* EnumWindowsFnT)(WNDENUMPROC lpEnumFunc, LPARAM lParam);
stDetourHooker<EnumWindowsFnT> g_hooker_EnumWindows_;
BOOL WINAPI Hooker_EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		LOG_O(Log_debug) << "Hooker_EnumWindows 直接返回失败，线程ID：" << std::dec << thrd_id;
		return FALSE;
	}
	return g_hooker_EnumWindows_.code_orig_(lpEnumFunc, lParam);
}

stDetourHooker<decltype(FindWindowExW)*> g_hooker_FindWindowEx_;
HWND WINAPI Hooker_FindWindowExW(__in_opt HWND hWndParent, __in_opt HWND hWndChildAfter, __in_opt LPCWSTR lpszClass, __in_opt LPCWSTR lpszWindow){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		LOG_O(Log_debug) << "Hooker_FindWindowExW 直接返回失败，线程ID：" << thrd_id;
		return nullptr;
	}
	return g_hooker_FindWindowEx_.code_orig_(hWndParent, hWndChildAfter, lpszClass, lpszWindow);
}

stDetourHooker<decltype(FindWindowW)*> g_hooker_FindWindowW_;
HWND WINAPI Hooker_FindWindowW(__in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName){
	auto thrd_id = GetCurrentThreadId();
	auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
	if (GlobalSetter::GetMe().GetSelfModuleHandle() != the_module)
	{
		if (lpWindowName)
		{
			if (Cmnx_Stristr(lpWindowName, L"怪物猎人Online登录程序"))
			{
				auto wnd_handle = g_hooker_FindWindowW_.code_orig_(lpClassName, lpWindowName);
				if (wnd_handle)
				{
					DWORD process_id = 0;
					GetWindowThreadProcessId(wnd_handle, &process_id);
					if (0 != process_id)
					{
						TerminateTheProcess(process_id);
					}
				}
				return nullptr;
			}
		}		
#ifdef _DEBUG
		std::string cls_name;
		if (lpClassName)
			cls_name = CW2A(lpClassName);
		std::string wnd_name;
		if (lpWindowName)
		wnd_name = CW2A(lpWindowName);
		LOG_O(Log_debug) << "Hooker_FindWindowW 直接返回失败，线程ID：" << thrd_id << " cls_name:" << cls_name << " wnd_name:" << wnd_name;
#endif
		return nullptr;
	}
	return g_hooker_FindWindowW_.code_orig_(lpClassName, lpWindowName);
}

void AntiCheckThrd_ClientBase(HookerPtrStored& hooks){
	HMODULE h_client_base = GetModuleHandleA("MHOClientBase.dll");
	while (!h_client_base)
	{
		::Sleep(1);
		h_client_base = GetModuleHandleA("MHOClientBase.dll");
	}
	auto& hooker_factory = HookerFactory::GetMe();
	//多开	
	struct tagSprintf{};
	//三开会被封
	/*hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagSprintf, (char*)h_client_base + g_h_lpkb_sprintf_,
		Hooker_Sprintf, true));*/

	LOG_O(Log_trace) << "AntiCheckThrd_ClientBase的hook成功";
}

void AntiCheckThrd_TerSafe(HookerPtrStored& hooks){
	HMODULE h_tersafe = GetModuleHandleA("terSafe.dll");
	while (!h_tersafe)
	{
		::Sleep(1);
		h_tersafe = GetModuleHandleA("terSafe.dll");
	}
	auto& hooker_factory = HookerFactory::GetMe();
	//得到机器码
	struct tagCpuId{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagCpuId, (char*)h_tersafe + g_h_lpkb_tersafe_get_cpuid_,
		Hooker_CpuId, true));

	LOG_O(Log_trace) << "AntiCheckThrd_TerSafe的hook成功";
}

void AntiCheckThrd_vcr80(HookerPtrStored& hooks){
	auto h_module = GetModuleHandleA("MSVCR80.dll");
	while (!h_module)
	{
		::Sleep(1);
		h_module = GetModuleHandleA("MSVCR80.dll");
		if (h_module)
			break;
		CToolhelp th(TH32CS_SNAPMODULE, GetCurrentProcessId());
		MODULEENTRY32 m_entry;
		m_entry.dwSize = sizeof(m_entry);
		if (th.ModuleFind(_T("MSVCR80.dll"), &m_entry))
			h_module = m_entry.hModule;
	}
	LOG_O(Log_trace) << "AntiCheckThrd_vcr80 找到了模块";
	auto& hooker_factory = HookerFactory::GetMe();
	auto the_addr = (void*)GetProcAddress(h_module, "_beginthread");
	if (!the_addr)
	{
		assert(false);
		return;
	}
	struct tagBeginThread80{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThread80, the_addr,
		Hooker_beginthread, true));

	the_addr = (void*)GetProcAddress(h_module, "_beginthreadex");
	if (!the_addr)
	{
		assert(false);
		return;
	}
	struct tagBeginThreadEx80{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThreadEx80, the_addr,
		Hooker_beginthreadex, true));

	LOG_O(Log_trace) << "AntiCheckThrd_vcr80的hook成功";
}

bool DoAntiCheck(HookerPtrStored& hooks){
	//////////////////////////////////////////////////////////////////////////
	auto& hooker_factory = HookerFactory::GetMe();

	auto h_msvcr = GetModuleHandleA("MSVCR90.dll");
	if (!h_msvcr)
	{
		assert(false);
		return false;
	}
	auto the_addr = (void*)GetProcAddress(h_msvcr, "_beginthread");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagBeginThread90{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThread90, the_addr,
		Hooker_beginthread, true));

	the_addr = (void*)GetProcAddress(h_msvcr, "_beginthreadex");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagBeginThreadEx90{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThreadEx90, the_addr,
		Hooker_beginthreadex, true));

	h_msvcr = GetModuleHandleA("msvcrt.dll");
	if (!h_msvcr)
	{
		assert(false);
		return false;
	}
	the_addr = (void*)GetProcAddress(h_msvcr, "_beginthread");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagBeginThread_Crt{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThread_Crt, the_addr,
		Hooker_beginthread, true));

	the_addr = (void*)GetProcAddress(h_msvcr, "_beginthreadex");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagBeginThreadEx_Crt{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThreadEx_Crt, the_addr,
		Hooker_beginthreadex, true));

	h_msvcr = GetModuleHandleA("msvcr80.dll");
	if (!h_msvcr)
	{
		CToolhelp th(TH32CS_SNAPMODULE, GetCurrentProcessId());
		MODULEENTRY32 m_entry;
		m_entry.dwSize = sizeof(m_entry);
		if (th.ModuleFind(_T("msvcr80.dll"), &m_entry))
		{
			h_msvcr = m_entry.hModule;
			LOG_O(Log_trace) << "找到了msvcr80";
		}
		else
		{
			LOG_O(Log_debug) << "没找到msvcr80";
		}
	}
	else
	{
		LOG_O(Log_trace) << "1找到了msvcr80";
	}

	if (h_msvcr)
	{
		the_addr = (void*)GetProcAddress(h_msvcr, "_beginthread");
		if (!the_addr)
		{
			assert(false);
			return false;
		}
		struct tagBeginThread80{};
		hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThread80, the_addr,
			Hooker_beginthread, true));

		the_addr = (void*)GetProcAddress(h_msvcr, "_beginthreadex");
		if (!the_addr)
		{
			assert(false);
			return false;
		}
		struct tagBeginThreadEx80{};
		hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBeginThreadEx80, the_addr,
			Hooker_beginthreadex, true));
	}
	else
	{
		boost::thread t_msvcr80([&hooks](){
			AntiCheckThrd_vcr80(hooks);
		});
	}
	//////////////////////////////////////////////////////////////////////////
	boost::thread t_handle_sleep_thrd([](){
		CEvent event_handle;
		g_event_handle_rub_thrd_ = &event_handle;
		BOOST_SCOPE_EXIT_ALL(&){
			g_event_handle_rub_thrd_ = nullptr;
			g_handle_rub_thrd_end_ = true;
		};
		HandleSleepRubThrd(&event_handle);
	});
	//////////////////////////////////////////////////////////////////////////
	auto h_inst = GetModuleHandleA("Kernel32.dll");
	if (!h_inst)
	{
		assert(false);
		return false;
	}
	the_addr = GetProcAddress(h_inst, "GetUserDefaultLCID");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagGetUserDefaultLCID{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGetUserDefaultLCID, (char*)the_addr,
		Hooker_GetUserDefaultLCID, true));

	the_addr = GetProcAddress(h_inst, "CreateToolhelp32Snapshot");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_CreateToolhelp32Snapshot_.hooked_addr_ = (decltype(CreateToolhelp32Snapshot)*)the_addr;
	g_hooker_CreateToolhelp32Snapshot_.detour_addr_ = Hooker_CreateToolhelp32Snapshot;
	if (!SetDetourHook("Hooker_CreateToolhelp32Snapshot", g_hooker_CreateToolhelp32Snapshot_))
	{
		assert(false);
		return false;
	}

	h_inst = GetModuleHandleA("winmm.dll");
	if (!h_inst)
	{
		assert(false);
		return false;
	}
	the_addr = GetProcAddress(h_inst, "timeGetTime");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagtimeGetTime{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagtimeGetTime, (char*)the_addr,
		Hooker_TimeGetTime, true));

	h_inst = GetModuleHandleA("ntdll.dll");
	if (!h_inst)
	{
		assert(false);
		return false;
	}
	the_addr = GetProcAddress(h_inst, "ZwOpenProcess");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagZwOpenProcess{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagZwOpenProcess, (char*)the_addr,
		Hooker_ZwOpenProcess, true));

	the_addr = GetProcAddress(h_inst, "ZwQuerySystemInformation");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	struct tagZwQuerySystemInformation{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagZwQuerySystemInformation, (char*)the_addr,
		Hooker_ZwQuerySystemInformation, true));

	the_addr = GetProcAddress(h_inst, "NtCreateFile");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_NtCreateFile.hooked_addr_ = (NtCreateFile)the_addr;
	g_hooker_NtCreateFile.detour_addr_ = Hooker_NtCreateFile;
	if (!SetDetourHook("Hooker_NtCreateFile", g_hooker_NtCreateFile))
	{
		assert(false);
		return false;
	}
	
	//CreateToolhelp32Snapshot底层会调用RtlQueryProcessDebugInformation
	the_addr = GetProcAddress(h_inst, "RtlQueryProcessDebugInformation");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_RtlQueryProcessDebugInformation.hooked_addr_ = (RtlQueryProcessDebugInformation)the_addr;
	g_hooker_RtlQueryProcessDebugInformation.detour_addr_ = Hooker_RtlQueryProcessDebugInformation;
	if (!SetDetourHook("Hooker_RtlQueryProcessDebugInformation", g_hooker_RtlQueryProcessDebugInformation))
	{
		assert(false);
		return false;
	}

	the_addr = GetProcAddress(h_inst, "ZwCreateThread");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	hooker_info_NtCreateThread_.hooked_addr_ = (NtCreateThread)the_addr;
	hooker_info_NtCreateThread_.detour_addr_ = Hooker_NtCreateThread;
	if (!SetDetourHook("Hooker_NtCreateThread", hooker_info_NtCreateThread_))
	{
		assert(false);
		return false;
	}

	the_addr = GetProcAddress(h_inst, "ZwCreateThreadEx");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	hooker_info_NtCreateThreadEx_.hooked_addr_ = (NtCreateThreadEx)the_addr;
	hooker_info_NtCreateThreadEx_.detour_addr_ = Hooker_NtCreateThreadEx;
	if (!SetDetourHook("Hooker_NtCreateThreadEx", hooker_info_NtCreateThreadEx_))
	{
		assert(false);
		return false;
	}

	g_hooker_NtQueryInformationProcess_.hooked_addr_ = GetNtQueryInformationProcess();
	if (!g_hooker_NtQueryInformationProcess_.hooked_addr_)
	{
		assert(false);
		return false;
	}
	g_hooker_NtQueryInformationProcess_.detour_addr_ = Hooker_NtQueryInformationProcess;
	if (!SetDetourHook("Hooker_NtQueryInformationProcess", g_hooker_NtQueryInformationProcess_))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	h_inst = GetModuleHandleA("PSAPI.dll");
	if (h_inst)
	{
		the_addr = GetProcAddress(h_inst, "EnumProcesses");
		if (the_addr)
		{
			g_hooker_EnumProcesses_.hooked_addr_ = (EnumProcessesFnT)the_addr;
			g_hooker_EnumProcesses_.detour_addr_ = Hooker_EnumProcesses;
			if (!SetDetourHook("Hooker_EnumProcesses", g_hooker_EnumProcesses_))
			{
				assert(false);
				return false;
			}
		}
	}
	h_inst = GetModuleHandleA("Kernel32.dll");
	if (!h_inst)
	{
		assert(false);
		return false;
	}
	the_addr = GetProcAddress(h_inst, "K32EnumProcesses");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_EnumProcesses1_.hooked_addr_ = (EnumProcessesFnT)the_addr;
	g_hooker_EnumProcesses1_.detour_addr_ = Hooker_EnumProcesses1;
	if (!SetDetourHook("Hooker_EnumProcesses1", g_hooker_EnumProcesses1_))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	h_inst = GetModuleHandleA("user32.dll");
	if (!h_inst)
	{
		assert(false);
		return false;
	}
	the_addr = GetProcAddress(h_inst, "EnumWindows");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_EnumWindows_.hooked_addr_ = (EnumWindowsFnT)the_addr;
	g_hooker_EnumWindows_.detour_addr_ = Hooker_EnumWindows;
	if (!SetDetourHook("Hooker_EnumWindows", g_hooker_EnumWindows_))
	{
		assert(false);
		return false;
	}

	the_addr = GetProcAddress(h_inst, "FindWindowW");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_FindWindowW_.hooked_addr_ = (decltype(g_hooker_FindWindowW_.hooked_addr_))the_addr;
	g_hooker_FindWindowW_.detour_addr_ = Hooker_FindWindowW;
	if (!SetDetourHook("Hooker_FindWindowW", g_hooker_FindWindowW_))
	{
		assert(false);
		return false;
	}

	the_addr = GetProcAddress(h_inst, "FindWindowExW");
	if (!the_addr)
	{
		assert(false);
		return false;
	}
	g_hooker_FindWindowEx_.hooked_addr_ = (decltype(g_hooker_FindWindowEx_.hooked_addr_))the_addr;
	g_hooker_FindWindowEx_.detour_addr_ = Hooker_FindWindowExW;
	if (!SetDetourHook("Hooker_FindWindowExW", g_hooker_FindWindowEx_))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	/*boost::thread t_1([&hooks](){
	AntiCheckThrd_ClientBase(hooks);
	});*/

	/*boost::thread t_2([&hooks](){
		AntiCheckThrd_TerSafe(hooks);
	});*/
	return true;
}