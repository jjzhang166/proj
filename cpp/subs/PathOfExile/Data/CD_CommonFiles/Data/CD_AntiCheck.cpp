#include "stdafx.h"
#include "CD_Interface.h"
#include <CommonX/CmnX_DllApi.h>
#include <CmnAntiCheck/CmnRbuThread.h>
#include <TlHelp32.h>
#include <CommonX/CmnX_HookerMore.h>

using namespace CD_Interface;
extern InterfaceSink* g_interface_sink_;
stDetourHooker<NtCreateThread> hooker_info_NtCreateThread_;
stDetourHooker<NtCreateThreadEx> hooker_info_NtCreateThreadEx_;
stDetourHooker<NtQueryInformationProcessT> g_hooker_NtQueryInformationProcess_;
stDetourHooker<decltype(CreateToolhelp32Snapshot)*> g_hooker_CreateToolhelp32Snapshot_;
typedef BOOL(WINAPI* EnumProcessesFnT)(_Out_  DWORD *pProcessIds, _In_   DWORD cb, _Out_  DWORD *pBytesReturned);
stDetourHooker<EnumProcessesFnT> g_hooker_EnumProcesses1_;
typedef BOOL(WINAPI* EnumWindowsFnT)(WNDENUMPROC lpEnumFunc, LPARAM lParam);
stDetourHooker<EnumWindowsFnT> g_hooker_EnumWindows_;
stDetourHooker<decltype(FindWindowExW)*> g_hooker_FindWindowEx_;
stDetourHooker<decltype(FindWindowW)*> g_hooker_FindWindowW_;

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
		AddModuleOffset("PathOfExileBase.dll", offsets);
		AddModuleOffset("TenSLX.dll", offsets);
		AddModuleOffset("TenRpcs.dll", offsets);
		AddModuleOffset("TCJ.dll", offsets);
		//AddModuleOffset("qbclient.dll", offsets);		//若是不放行该线程，则会卡死在启动客户端后的腾讯logo动画上。
		AddModuleOffset("GbSpy.dll", offsets);
		//AddModuleOffset("TerSafe.dll", offsets);		//选择角色进入游戏后会创建5个线程，如果不放行，会崩掉。
		//AddModuleOffset("ntdll.dll", offsets);		//若是不放行，客户端都启动不起来。
	}
};
struct ModuleOffsetHandlerHlp_NtCreateThrd : ModuleOffsetHandlerHlpBase{};
class HandleRubThread_ByModuleOffset : public HandleRubThread{};
ModuleOffsetHandlerHlp_NtCreateThrd	g_do_pre_create_thrd_nt_;

void CD_Interface::Hooks::DoTraCheckThread()
{
	typedef ModuleOffsetHandler::stOffsetInfoT stOffsetInfoT;

	ModuleOffsetHolder module_offset;
	ModuleOffsetHolder::OffsetsT offsets;
	offsets.insert(stOffsetInfoT(0, -1));
	module_offset.AddModuleOffset("PathOfExileBase.dll", offsets);
	module_offset.AddModuleOffset("TenSLX.dll", offsets);
	module_offset.AddModuleOffset("TenRpcs.dll", offsets);
	module_offset.AddModuleOffset("TCJ.dll", offsets);
	module_offset.AddModuleOffset("qbclient.dll", offsets);
	module_offset.AddModuleOffset("GbSpy.dll", offsets);
	module_offset.AddModuleOffset("TerSafe.dll", offsets);
	module_offset.AddModuleOffset("ntdll.dll", offsets);

	LOG_O(Log_debug) << "开始遍历所有垃圾线程";
	static HandleRubThread_ByModuleOffset rub_thrd;
	auto res = rub_thrd.DoAllThrdSleepByOffset(1000 * 60 * 3, module_offset, DummySleep);
	//rub_thrd.ClrAll();
	LOG_O(Log_debug) << "所有垃圾线程处理结果:" << res;
	assert(res);
}

static void* Hooker_EnumProcesses(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	LOG_O(Log_debug) << "EnumProcesses";
	ambient.eax_ = 0;
	//ambient.eax_ = -1;
	return NakedRetP3;
}
BOOL WINAPI Hooker_EnumProcesses1(_Out_  DWORD *pProcessIds, _In_   DWORD cb, _Out_  DWORD *pBytesReturned){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_EnumProcesses1_.code_orig_(pProcessIds, cb, pBytesReturned);
	LOG_O(Log_debug) << "Hooker_EnumProcesses1 直接返回失败，线程ID：" << GetCurrentThreadId();
	return FALSE;
}
BOOL WINAPI Hooker_EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_EnumWindows_.code_orig_(lpEnumFunc, lParam);
	LOG_O(Log_debug) << "Hooker_EnumWindows 直接返回失败，线程ID：" << GetCurrentThreadId();
	return FALSE;
}
HWND WINAPI Hooker_FindWindowExW(__in_opt HWND hWndParent, __in_opt HWND hWndChildAfter, __in_opt LPCWSTR lpszClass, __in_opt LPCWSTR lpszWindow){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_FindWindowEx_.code_orig_(hWndParent, hWndChildAfter, lpszClass, lpszWindow);
	LOG_O(Log_debug) << "Hooker_FindWindowExW 直接返回失败，线程ID：" << GetCurrentThreadId();
	return nullptr;
}
HWND WINAPI Hooker_FindWindowW(__in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_FindWindowW_.code_orig_(lpClassName, lpWindowName);
	auto thrd_id = GetCurrentThreadId();
	if (lpWindowName)
	{
		if (StdEx::TStristr(lpWindowName, L"流放之路登录程序"))
		{
			auto wnd_handle = g_hooker_FindWindowW_.code_orig_(lpClassName, lpWindowName);
			if (wnd_handle)
			{
				DWORD process_id = 0;
				GetWindowThreadProcessId(wnd_handle, &process_id);
				if (0 != process_id)
				{
					LOG_O(Log_debug) << "Terminate登陆程序，pid:" << process_id;
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
static void* Hooker_EnumProcessModules(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	LOG_O(Log_debug) << "EnumProcessModules";
	//ambient.eax_ = 0;
	ambient.eax_ = 0;
	return NakedRetP4;
}
static void* Hooker_ZwQuerySystemInformation(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	auto SystemInformationClass = ambient.EspParamAs<1, SYSTEM_INFORMATION_CLASS>();
	auto SystemInformation = ambient.EspParamAs<2, PVOID>();
	auto SystemInformationLength = ambient.EspParamAs<3, ULONG>();
	auto ReturnLength = ambient.EspParamAs<4, PULONG>();
	auto old_fn = (ZwQuerySystemInformation)hooker.GetCodeOrig();
	LONG status = old_fn(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	PSYSTEM_PROCESSES pInfo = (PSYSTEM_PROCESSES)SystemInformation;
	if (SystemModuleInformation == SystemInformationClass)
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
	else if (SystemProcessInformation == SystemInformationClass && status != 0x0C0000004)
	{
		while (true)
		{
			if (pInfo->NextEntryDelta == 0)
			{
				break;
			}

			pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);

			const wchar_t* pszProcessName = pInfo->ProcessName.Buffer;

			if (wcsstr(pszProcessName, L"控制台") != 0)
				//if (pszProcessName)
			{
				//LOG_O(Log_trace) << "成功的干掉了一次检测进程,进程名：" << pszProcessName;// << ",扣压线程：" << GetCurrentThreadId();
				//pInfo->NextEntryDelta = 0;
				pInfo->ProcessId = 0;
				*(wchar_t*)pszProcessName = 0;
				/*while (true)
				{
				::Sleep(1000 * 10);
				}*/
			}
		}
	}
	ambient.eax_ = status;
	return NakedRetP4;
}
static void* Hooker_NtCreateFile(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	auto FileHandle = ambient.EspParamAs<1, PHANDLE>();
	auto DesiredAccess = ambient.EspParamAs<2, ACCESS_MASK>();
	auto ObjectAttributes = ambient.EspParamAs<3, POBJECT_ATTRIBUTES>();
	auto IoStatusBlock = ambient.EspParamAs<4, PIO_STATUS_BLOCK>();
	auto AllocationSize = ambient.EspParamAs<5, PLARGE_INTEGER>();
	auto FileAttributes = ambient.EspParamAs<6, ULONG>();
	auto ShareAccess = ambient.EspParamAs<7, ULONG>();
	auto CreateDisposition = ambient.EspParamAs<8, ULONG>();
	auto CreateOptions = ambient.EspParamAs<9, ULONG>();
	auto EaBuffer = ambient.EspParamAs<10, PVOID>();
	auto EaLength = ambient.EspParamAs<11, ULONG>();
	auto old_fn = (NtCreateFile)hooker.GetCodeOrig();
	if (ObjectAttributes && ObjectAttributes->ObjectName && ObjectAttributes->ObjectName->Buffer && ObjectAttributes->ObjectName->MaximumLength > 0)
	{
		auto pwszPatch = wcsstr(ObjectAttributes->ObjectName->Buffer, L"控制台");
		if (pwszPatch)
		{
			LOG_O(Log_debug) << "NtCreateFile打开了控制台";
			auto old = *pwszPatch;
			*pwszPatch = L'a';
			ambient.eax_ = old_fn(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes,
				ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
			*pwszPatch = old;
			return NakedRetP11;
		}
	}
	ambient.eax_ = old_fn(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes,
		ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
	return NakedRetP11;
}
static void* Hooker_RtlQueryProcessDebugInformation(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	auto dwFlags = ambient.EspParamAs<2, DWORD>();
	if (dwFlags != 4)
	{
		LOG_O(Log_trace) << "非线程遍历，直接返回失败！";
		ambient.eax_ = (pt_dword)INVALID_HANDLE_VALUE;
		return NakedRetP3;
	}
	return hooker;
}
HANDLE WINAPI Hooker_CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_CreateToolhelp32Snapshot_.code_orig_(dwFlags, th32ProcessID);
	LOG_O(Log_debug) << "Hooker_CreateToolhelp32Snapshot 直接返回失败，线程ID：" << GetCurrentThreadId();
	return INVALID_HANDLE_VALUE;
}
static void* Hooker_GetUserDefaultLCID(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	auto dwCallAddr = ambient.GetFnRetAddrAsEsp();
	MEMORY_BASIC_INFORMATION memory_basic = { 0 };
	VirtualQueryEx((HANDLE)-1, (LPCVOID)dwCallAddr, &memory_basic, sizeof(memory_basic));

	if (PAGE_READWRITE == memory_basic.AllocationProtect)
	{
		//sprintf_s(szPrint, "[LOG]  process id = 0x%p thread id = 0x%p call addr = 0x%p memory = 0x%d %d", GetCurrentProcessId(), GetCurrentThreadId(), dwCallAddr, memory_basic.AllocationProtect, memory_basic.Protect);
		LOG_O(Log_trace) << "扣押线程:" << GetCurrentThreadId();
		while (true)
		{
			Sleep(5000);
		}
	}
	return hooker;
}
static void* Hooker_ZwOpenProcess(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	auto ProcessHandle = ambient.EspParamAs<1, PHANDLE>();
	auto DesiredAccess = ambient.EspParamAs<2, ACCESS_MASK>();
	auto ObjectAttributes = ambient.EspParamAs<3, POBJECT_ATTRIBUTES>();
	auto ClientId = ambient.EspParamAs<4, PCLIENT_ID>();
	CLIENT_ID theClientId;
	theClientId.UniqueProcess = ClientId->UniqueProcess;
	theClientId.UniqueThread = ClientId->UniqueThread;

	PVOID pProcessId = (PVOID)0;
	if (ClientId->UniqueProcess == pProcessId)
	{
		theClientId.UniqueProcess = (PVOID)(8);
		theClientId.UniqueThread = 0;

		//sprintf_s(szPrint, "[LOG] UniqueProcess = 0x%p = %d UniqueThread = 0x%p = %d nRet = %d  ThreadId=%d", ClientId->UniqueProcess, ClientId->UniqueProcess, ClientId->UniqueThread, ClientId->UniqueThread, GetCurrentThreadId());
		LOG_O(Log_trace) << "ZwOpenProcess扣线程：" << GetCurrentThreadId();
		while (true)
		{
			Sleep(1000 * 100);
		}
	}
	auto fn = (ZwOpenProcess)hooker.GetCodeOrig();
	ambient.eax_ = fn(ProcessHandle, DesiredAccess, ObjectAttributes, &theClientId);
	return NakedRetP4;
}
void HandleNtCreateThread(char module_name[MAX_PATH], HMODULE the_module, void* start_addr, DWORD thrd_id){
	/*if (0 == _stricmp(module_name, "TenRpcs.dll"))
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
	}*/
}
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
	//assert(false);
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
		LOG_O(Log_trace) << "NtCreateThread,addr:" << (void*)ThreadContext->Eax << ",module:" << buffer;
		HandleNtCreateThread(buffer, the_module, (void*)ThreadContext->Eax, GetThreadId(thrd_handle));
	}
	return res;
}

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
	if (!pre_res && g_interface_sink_)
	{
		if (0 != _stricmp(the_module_name, "MSVCR90.dll") && 0 != _stricmp(the_module_name, "MSVCR80.dll") && 0 != _stricmp(the_module_name, "msvcrt.dll")
			&& the_module != g_interface_sink_->GetSelfModule())
			LOG_O(Log_trace) << "Hooker_NtCreateThreadEx创建线程，入口地址：" << (void*)StartRoutine << " 模块名：" << the_module_name;
	}
#endif
	return res;
}
NTSTATUS_ WINAPI Hooker_NtQueryInformationProcess(IN HANDLE ProcessHandle, //进程句柄
	IN PROCESSINFOCLASS InformationClass, //信息类型
	OUT PVOID ProcessInformation, //缓冲指针
	IN ULONG ProcessInformationLength, //以字节为单位的缓冲大小
	OUT PULONG ReturnLength OPTIONAL //写入缓冲的字节数
	){
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return g_hooker_NtQueryInformationProcess_.code_orig_(ProcessHandle, InformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
	//处理了NtQueryInformationProcess后，游戏走起路来，或打起怪来，就显得有点卡了，但是CPU并未增高。
	//ProcessBasicInformation == InformationClass 去掉后，就不卡了
	if (/*ProcessBasicInformation == InformationClass ||*/ ProcessDebugPort == InformationClass)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (g_interface_sink_ && g_interface_sink_->GetSelfModule() != the_module)
		{
#ifdef _DEBUG
			if (ProcessDebugPort == InformationClass)
			{
				LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，ProcessDebugPort 线程ID：" << thrd_id;
				return -1;
			}
			if (g_interface_sink_->GetGWndThrdId() != thrd_id || ProcessBasicInformation != InformationClass)
				LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，InformationClass:" << InformationClass << " 线程ID：" << thrd_id;
#endif
			return -1;
		}
	}
	else if (ProcessImageFileName == InformationClass)
	{
		auto thrd_id = GetCurrentThreadId();
		auto the_module = ModuleFromAddress(GetThreadStartAddr(thrd_id));
		if (g_interface_sink_ && g_interface_sink_->GetSelfModule() != the_module)
		{
			ULONG ret_length = 0;
			auto res = g_hooker_NtQueryInformationProcess_.code_orig_(ProcessHandle, InformationClass, ProcessInformation, ProcessInformationLength, &ret_length);
			if (NT_SUCCESS(res))
			{
				if (StdEx::TStristr((wchar_t*)ProcessInformation, L"控制台"))
				{
					LOG_O(Log_debug) << "Hooker_NtQueryInformationProcess返回失败，因为遍历到了 控制台.exe 线程ID：" << thrd_id;
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
void* Hooker_NtReadVirtualMemory(IHooker& hooker, JmpHookerAmbient& ambient)
{
	if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;
	//NtReadVirtualMemory参数意义与ReadProcessMemory一样
	typedef int(__stdcall *NtReadVirtualMemory)(IN HANDLE ProcessHandle, IN PVOID BaseAddress, OUT PVOID Buffer, IN ULONG NumberOfBytesToRead, OUT PULONG NumberOfBytesReaded OPTIONAL);

	const int kParam_ProcessHandle = 1;
	const int kParam_BaseAddr = 2;
	const int kParam_Buffer = 3;
	const int kParam_Size = 4;
	const int kParam_NumberOfBytesReaded = 5;

	//////////////////////////////////////////////////////////////////////////
	auto the_process_handle = ambient.EspParamAs<kParam_ProcessHandle, HANDLE>();
	auto the_process_id = ::GetProcessId(the_process_handle);
	if (the_process_id != GetCurrentProcessId())
	{
		//LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，读取了其他进程内存，进程ID：" << the_process_id;
		return hooker;
	}
	auto cur_thread_id = ::GetCurrentThreadId();
	auto& anti_hook = CmnX_AntiHookCheck::GetRealMe();
	if (anti_hook.IsInThreadWhiteList(cur_thread_id))
		return hooker;
	//////////////////////////////////////////////////////////////////////////

	auto base_addr = ambient.EspParamAs<kParam_BaseAddr, void*>();
	if (IsUnValidPtr(base_addr))
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，base addr参数不正确：" << base_addr << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker;
	}
	auto out_buffer = ambient.EspParamAs<kParam_Buffer, void*>();
	if (IsUnValidPtr(out_buffer))
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，out_buffer参数不正确：" << out_buffer << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker;
	}
	auto n_size = ambient.EspParamAs<kParam_Size, int>();
	if (n_size <= 0)
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，n_size参数不正确：" << n_size << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker;
	}
	auto the_number = ambient.EspParamAs<kParam_NumberOfBytesReaded, PULONG>();

	ULONG read_size = 0;
	ambient.eax_ = ((NtReadVirtualMemory)hooker.GetCodeOrig())(the_process_handle, base_addr, out_buffer, n_size, &read_size);
	if (0 == ambient.eax_)
	{
		if (anti_hook.OutputSrcCodeInfoByAddr(base_addr, read_size, out_buffer))
		{
			/*LOG_O(Log_debug) << "NtReadVirtualMemory检测到hook了，地址：" << base_addr << " 大小：" << n_size << " 读取到的大小：" << read_size << " 缓冲区地址：" <<
			out_buffer << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);*/
		}
	}
	else
	{
		LOG_O(Log_debug) << "读取内存返回失败，地址：" << base_addr << " 大小：" << n_size << " 读取到的大小：" << read_size << " 缓冲区地址：" << out_buffer
			<< " LastError：" << GetLastError() << " 结果：" << ambient.eax_ << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
	}

	if (the_number)
		*the_number = read_size;
	return NakedRetP5;
}
struct stMemCpyParam{
	const char*		str_info_;
	const void*		dst_addr_;
	const void*		src_addr_;
	int				size_;
	DWORD			thread_id_;
};
void MemCpy_PromptInfo(void* param, unsigned short param_size)
{
	auto the_param = (const stMemCpyParam*)param;
	assert(param_size == sizeof(stMemCpyParam));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " dst addr:" << the_param->dst_addr_ << " src addr:" << the_param->src_addr_ << " 大小：" << the_param->size_;
}
//Hooker_Memcpy里做事要非常小心，因为这个API太常用了
void* Hooker_Memcpy(IHooker& hooker, JmpHookerAmbient& ambient)
{
	//加上下面这段，程序都启动不起来。
	/*if (g_interface_sink_ && g_interface_sink_->IsAddrInSelfModule())
		return hooker;*/
	typedef DWORD(__cdecl *Nt_memcpy)(void* dst, const void* src, size_t count);

	//////////////////////////////////////////////////////////////////////////
	auto& anti_hook = CmnX_AntiHookCheck::GetRealMe();
	auto cur_thread_id = ::GetCurrentThreadId();
	if (anti_hook.IsInThreadWhiteList(cur_thread_id))
		return hooker;
	//////////////////////////////////////////////////////////////////////////
	const int kParam_Dst = 1;
	const int kParam_src = 2;
	const int kParam_count = 3;

	auto dst_addr = ambient.EspParamAs<kParam_Dst, void*>();
	auto src_addr = ambient.EspParamAs<kParam_src, const void*>();
	auto n_size = ambient.EspParamAs<kParam_count, int>();
	if (IsUnValidPtr(dst_addr))
	{
#ifdef _DEBUG
		stMemCpyParam param = { "Hooker_Memcpy，dst_addr参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));
#endif
		return hooker;
	}
	if (IsUnValidPtr(src_addr))
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy，src_addr参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		return hooker;
	}
	if (n_size <= 0)
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy，n_size参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		return hooker;
	}

	ambient.eax_ = ((Nt_memcpy)hooker.GetCodeOrig())(dst_addr, src_addr, n_size);

	if (!IsUnValidPtr(ambient.eax_))
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy开始反crc：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		if (anti_hook.OutputSrcCodeInfoByAddr(src_addr, n_size, (void*)ambient.eax_))
		{
#ifdef _DEBUG
			/*stMemCpyParam param = { "Hooker_Memcpy发现检测：", dst_addr, src_addr, n_size, cur_thread_id };
			GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		}
	}
	else
	{
#ifdef _DEBUG
		stMemCpyParam param = { "Hooker_Memcpy返回值不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));
#endif
	}
	return NakedRetP0;
}
struct stLogSinkParam{
	const char*		str_info_;
	char			hooker_name_[kAntiHookCheck_MaxHookerNameSize + 1];
	const void*		hooker_addr_;
	int				size_;
	DWORD			thread_id_;
};

void LogSink_PromptInfo(void* param, unsigned short param_size)
{
	auto the_param = (const stLogSinkParam*)param;
	assert(param_size == sizeof(stLogSinkParam));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " hook名:" << the_param->hooker_name_ << " hooker addr:" << the_param->hooker_addr_ << " 大小：" << the_param->size_;
}
struct stLogSinkParam_OnCheck{
	const char*		str_info_;
	char			hooker_name_[kAntiHookCheck_MaxHookerNameSize + 1];
	const void*		check_addr_;
	int				check_size_;
	const void*		hooked_addr_;
	int				hooked_cpy_size_;
	DWORD			thread_id_;
};
void LogSink_PromptInfo_OnCheck(void* param, unsigned short param_size)
{
	auto the_param = (const stLogSinkParam_OnCheck*)param;
	assert(param_size == sizeof(stLogSinkParam_OnCheck));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " hook名:" << the_param->hooker_name_ << " check addr:" << the_param->check_addr_ << " 检查大小：" << the_param->check_size_
		<< " hooked addr:" << the_param->hooked_addr_ << " hooked copy size:" << the_param->hooked_cpy_size_;
}
class HookerLogSink : public CmnX_AntiHookLogSink{
	void OnAddHookCodeInfo(const char* hooker_name, const void* the_hook_addr, std::size_t hook_cpy_size) const{
		stLogSinkParam param;
		param.str_info_ = "添加了一个hook：";
		StdEx::TStrCpy(param.hooker_name_, hooker_name, sizeof(param.hooker_name_));
		param.hooker_addr_ = the_hook_addr;
		param.size_ = hook_cpy_size;
		param.thread_id_ = ::GetCurrentThreadId();
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(LogSink_PromptInfo, param));
	}
	void OnHookChecked(const char* hooker_name, const void* check_addr, std::size_t checked_size, const void* hooked_addr, std::size_t hooked_cpy_size) const{
		stLogSinkParam_OnCheck param;
		param.str_info_ = "检测到hook：";
		StdEx::TStrCpy(param.hooker_name_, hooker_name, sizeof(param.hooker_name_));
		param.check_addr_ = check_addr;
		param.check_size_ = checked_size;
		param.hooked_addr_ = hooked_addr;
		param.hooked_cpy_size_ = hooked_cpy_size;
		param.thread_id_ = ::GetCurrentThreadId();
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(LogSink_PromptInfo_OnCheck, param));
	}
};
void CD_Interface::Hooks::HookAntiCheck_GF()
{
#ifdef _DEBUG
	auto& anti_crc = CmnX_AntiHookCheck::GetRealMe();
	static HookerLogSink hooker_log_sink;
	anti_crc.SetLogSink(&hooker_log_sink);
	anti_crc.EnableOutputHookName(true);
#else
	CmnX_AntiHookCheck::GetRealMe().EnableOutputHookName(false);
#endif

	auto lib = GetModuleHandleA("PSAPI.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	void* addr = GetProcAddress(lib, "EnumProcesses");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_EnumProcesses, addr, Hooker_EnumProcesses);

	addr = GetProcAddress(lib, "EnumProcessModules");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_EnumProcessModules, addr, Hooker_EnumProcessModules);

	lib = GetModuleHandleA("ntdll.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	addr = GetProcAddress(lib, "ZwQuerySystemInformation");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_ZwQuerySystemInformation, addr, Hooker_ZwQuerySystemInformation);

	addr = LibFuncLoader::GetTheProcAddress(lib, "NtReadVirtualMemory");
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_NtReadVirtualMemory, addr, Hooker_NtReadVirtualMemory);

	addr = LibFuncLoader::GetTheProcAddress(lib, "memcpy");
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return;
	}
	//LOG_O底层会调用memcpy的
	//CopyMemory和RtlCopyMemory底层用的都是memcpy。
	HOOKER_SetCodeHooker(Hooker_Memcpy, addr, Hooker_Memcpy);

	addr = GetProcAddress(lib, "NtCreateFile");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_NtCreateFile, addr, Hooker_NtCreateFile);

	addr = GetProcAddress(lib, "RtlQueryProcessDebugInformation");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_RtlQueryProcessDebugInformation, addr, Hooker_RtlQueryProcessDebugInformation);

	addr = GetProcAddress(lib, "ZwOpenProcess");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_ZwOpenProcess, addr, Hooker_ZwOpenProcess);

	addr = GetProcAddress(lib, "ZwCreateThread");
	if (!addr)
	{
		assert(false);
		return;
	}
	hooker_info_NtCreateThread_.hooked_addr_ = (NtCreateThread)addr;
	hooker_info_NtCreateThread_.detour_addr_ = Hooker_NtCreateThread;
	if (!SetDetourHook("Hooker_NtCreateThread", hooker_info_NtCreateThread_))
	{
		assert(false);
		return;
	}

	addr = GetProcAddress(lib, "ZwCreateThreadEx");
	if (!addr)
	{
		assert(false);
		return;
	}
	hooker_info_NtCreateThreadEx_.hooked_addr_ = (NtCreateThreadEx)addr;
	hooker_info_NtCreateThreadEx_.detour_addr_ = Hooker_NtCreateThreadEx;
	if (!SetDetourHook("Hooker_NtCreateThreadEx", hooker_info_NtCreateThreadEx_))
	{
		assert(false);
		return;
	}

	g_hooker_NtQueryInformationProcess_.hooked_addr_ = GetNtQueryInformationProcess();
	if (!g_hooker_NtQueryInformationProcess_.hooked_addr_)
	{
		assert(false);
		return;
	}
	g_hooker_NtQueryInformationProcess_.detour_addr_ = Hooker_NtQueryInformationProcess;
	if (!SetDetourHook("Hooker_NtQueryInformationProcess", g_hooker_NtQueryInformationProcess_))
	{
		assert(false);
		return;
	}

	lib = GetModuleHandleA("KernelBase.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	addr = GetProcAddress(lib, "GetUserDefaultLCID");
	if (!addr)
	{
		assert(false);
		return;
	}
	/*addr = (char*)addr + 0x16;
	if (AsByte(addr, 0) != 0xc3)		//c3=ret
	{
	assert(false);
	return;
	}*/
	HOOKER_SetCodeHooker(Hooker_GetUserDefaultLCID, addr, Hooker_GetUserDefaultLCID);

	lib = GetModuleHandleA("Kernel32.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	addr = GetProcAddress(lib, "CreateToolhelp32Snapshot");
	if (!addr)
	{
		assert(false);
		return;
	}
	g_hooker_CreateToolhelp32Snapshot_.hooked_addr_ = (decltype(CreateToolhelp32Snapshot)*)addr;
	g_hooker_CreateToolhelp32Snapshot_.detour_addr_ = Hooker_CreateToolhelp32Snapshot;
	if (!SetDetourHook("Hooker_CreateToolhelp32Snapshot", g_hooker_CreateToolhelp32Snapshot_))
	{
		assert(false);
		return;
	}

	addr = GetProcAddress(lib, "K32EnumProcesses");
	if (!addr)
	{
		assert(false);
		return;
	}
	g_hooker_EnumProcesses1_.hooked_addr_ = (EnumProcessesFnT)addr;
	g_hooker_EnumProcesses1_.detour_addr_ = Hooker_EnumProcesses1;
	if (!SetDetourHook("Hooker_EnumProcesses1", g_hooker_EnumProcesses1_))
	{
		assert(false);
		return;
	}

	lib = GetModuleHandleA("user32.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	addr = GetProcAddress(lib, "EnumWindows");
	if (!addr)
	{
		assert(false);
		return;
	}
	g_hooker_EnumWindows_.hooked_addr_ = (EnumWindowsFnT)addr;
	g_hooker_EnumWindows_.detour_addr_ = Hooker_EnumWindows;
	if (!SetDetourHook("Hooker_EnumWindows", g_hooker_EnumWindows_))
	{
		assert(false);
		return;
	}

	addr = GetProcAddress(lib, "FindWindowExW");
	if (!addr)
	{
		assert(false);
		return;
	}
	g_hooker_FindWindowEx_.hooked_addr_ = (decltype(g_hooker_FindWindowEx_.hooked_addr_))addr;
	g_hooker_FindWindowEx_.detour_addr_ = Hooker_FindWindowExW;
	if (!SetDetourHook("Hooker_FindWindowExW", g_hooker_FindWindowEx_))
	{
		assert(false);
		return;
	}

	addr = GetProcAddress(lib, "FindWindowW");
	if (!addr)
	{
		assert(false);
		return;
	}
	g_hooker_FindWindowW_.hooked_addr_ = (decltype(g_hooker_FindWindowW_.hooked_addr_))addr;
	g_hooker_FindWindowW_.detour_addr_ = Hooker_FindWindowW;
	if (!SetDetourHook("Hooker_FindWindowW", g_hooker_FindWindowW_))
	{
		assert(false);
		return;
	}
}
