#pragma once
/*
@author zhp
@date 2016/3/24 18:48
@purpose for some dll api
*/
#include <Windows.h>
#include <NTSecAPI.h>
#include "../DependSelfLib.h"
//////////////////////////////////////////////////////////////////////////
class LibraryLoader{
public:
	explicit LibraryLoader( const char* str_library );
	HMODULE GetLibrary() const;

public:
	static HMODULE LoadTheLibrary( const char* str_library );
	static HMODULE LoadTheLibrary( const wchar_t* str_library );
	static const LibraryLoader& GetLib_Kernel32();
	static const LibraryLoader& GetLib_User32();
	static const LibraryLoader& GetLib_Ntdll();

private:
	HMODULE		the_library_;
};

//libray functions
class LibFuncLoader{
public:
	LibFuncLoader( const char* str_library, const char* func_name );
	LibFuncLoader( HMODULE hlib, const char* func_name );
	LibFuncLoader( const LibraryLoader& lib_loader, const char* func_name );

public:
	template<typename FuncT_>
	FuncT_ GetFuncT() const{
		return (FuncT_)the_func_addr_;
	}

public:
	static FARPROC GetTheProcAddress( HMODULE hlib, const char* func_name );

private:
	FARPROC		the_func_addr_;
};

template<typename FuncT_>
class LibFuncLoaderX : public LibFuncLoader{
public:
	typedef FuncT_ FuncType;

public:
	LibFuncLoaderX( const char* str_library, const char* func_name ) : LibFuncLoader(str_library, func_name){}
	LibFuncLoaderX( HMODULE hlib, const char* func_name ) : LibFuncLoader(hlib, func_name){}
	LibFuncLoaderX( const LibraryLoader& lib_loader, const char* func_name ) : LibFuncLoader(lib_loader, func_name){}

public:
	FuncType GetFunc() const{
		return GetFuncT<FuncType>();
	}
};
//////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI* WriteProcessMemoryT)( _In_   HANDLE hProcess, _In_   LPVOID lpBaseAddress, _In_   LPCVOID lpBuffer, _In_   SIZE_T nSize, _Out_  SIZE_T *lpNumberOfBytesWritten );
WriteProcessMemoryT GetWriteProcessMemory();
BOOL WINAPI WriteProcessMemory_( _In_   HANDLE hProcess, _In_   LPVOID lpBaseAddress, _In_ LPCVOID lpBuffer, _In_   SIZE_T nSize, _Out_  SIZE_T *lpNumberOfBytesWritten );

typedef BOOL (WINAPI* ReadProcessMemoryT)( _In_   HANDLE hProcess, _In_   LPCVOID lpBaseAddress, _Out_  LPVOID lpBuffer, _In_   SIZE_T nSize, _Out_  SIZE_T *lpNumberOfBytesRead );
ReadProcessMemoryT GetReadProcessMemory();
BOOL WINAPI ReadProcessMemory_( _In_   HANDLE hProcess, _In_   LPCVOID lpBaseAddress, _Out_  LPVOID lpBuffer, _In_   SIZE_T nSize, _Out_  SIZE_T *lpNumberOfBytesRead );
bool ReadMemory( const void* base_addr, void* out_buffer, SIZE_T read_size );

typedef int (WINAPI *MessageBoxTimeoutT)( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds );
MessageBoxTimeoutT GetMessageBoxTimeout();
int MessageBoxTimeout( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds );

typedef LONG NTSTATUS_;
#define NT_SUCCESS(Status) (((NTSTATUS_)(Status)) >= 0)

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
}CLIENT_ID, *PCLIENT_ID;

enum THREADINFOCLASS_{
	ThreadBasicInformation,  
	ThreadTimes,  
	ThreadPriority,  
	ThreadBasePriority,  
	ThreadAffinityMask,  
	ThreadImpersonationToken,  
	ThreadDescriptorTableEntry,  
	ThreadEnableAlignmentFaultFixup,  
	ThreadEventPair_Reusable,  
	ThreadQuerySetWin32StartAddress,  
	ThreadZeroTlsCell,  
	ThreadPerformanceCount,  
	ThreadAmILastThread,  
	ThreadIdealProcessor,  
	ThreadPriorityBoost,  
	ThreadSetTlsArrayAddress,   // Obsolete  
	ThreadIsIoPending,  
	ThreadHideFromDebugger,  
	ThreadBreakOnTermination,  
	ThreadSwitchLegacyState,  
	ThreadIsTerminated,  
	ThreadLastSystemCall,  
	ThreadIoPriority,  
	ThreadCycleTime,  
	ThreadPagePriority,  
	ThreadActualBasePriority,  
	ThreadTebInformation,  
	ThreadCSwitchMon,          // Obsolete  
	ThreadCSwitchPmu,  
	ThreadWow64Context,  
	ThreadGroupInformation,  
	ThreadUmsInformation,      // UMS  
	ThreadCounterProfiling,  
	ThreadIdealProcessorEx,  
	MaxThreadInfoClass  
};
typedef NTSTATUS_ (WINAPI *NtQueryInformationThreadT)( HANDLE ThreadHandle, THREADINFOCLASS_ ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength );
NtQueryInformationThreadT GetNtQueryInformationThread();
typedef struct  
{  
	DWORD ExitStatus;  
	DWORD PebBaseAddress;  
	DWORD AffinityMask;  
	DWORD BasePriority;  
	ULONG UniqueProcessId;  
	ULONG InheritedFromUniqueProcessId;  
}PROCESS_BASIC_INFORMATION;
enum PROCESSINFOCLASS{
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27,
};
typedef NTSTATUS_ (WINAPI* NtQueryInformationProcessT)( IN HANDLE ProcessHandle, //进程句柄
													   IN PROCESSINFOCLASS InformationClass, //信息类型
													   OUT PVOID ProcessInformation, //缓冲指针
													   IN ULONG ProcessInformationLength, //以字节为单位的缓冲大小
													   OUT PULONG ReturnLength OPTIONAL //写入缓冲的字节数
													   );
NtQueryInformationProcessT GetNtQueryInformationProcess();

BOOL Get_GetVersionExW(LPOSVERSIONINFOW lpVersionInformation );

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	PVOID RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef struct _NT_PROC_THREAD_ATTRIBUTE_ENTRY {
	ULONG Attribute;    // PROC_THREAD_ATTRIBUTE_XXX，参见MSDN中UpdateProcThreadAttribute的说明
	SIZE_T Size;        // Value的大小
	ULONG_PTR Value;    // 保存4字节数据（比如一个Handle）或数据指针
	ULONG Unknown;      // 总是0，可能是用来返回数据给调用者
} PROC_THREAD_ATTRIBUTE_ENTRY, *PPROC_THREAD_ATTRIBUTE_ENTRY;
typedef struct _NT_PROC_THREAD_ATTRIBUTE_LIST {
	ULONG Length;       // 结构总大小
	PROC_THREAD_ATTRIBUTE_ENTRY Entry[1];
} NT_PROC_THREAD_ATTRIBUTE_LIST, *PNT_PROC_THREAD_ATTRIBUTE_LIST;
typedef NTSTATUS_ (WINAPI* NtCreateThreadEx)(
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
	);
NtCreateThreadEx GetNtCreateThreadEx();

typedef struct _INITIAL_TEB {
	PVOID                StackBase;
	PVOID                StackLimit;
	PVOID                StackCommit;
	PVOID                StackCommitMax;
	PVOID                StackReserved;
} INITIAL_TEB, *PINITIAL_TEB;
typedef NTSTATUS_(WINAPI* NtCreateThread)(                     // <相关参数说明>
	__out PHANDLE ThreadHandle,     //返回创建线程的句柄
	__in ACCESS_MASK DesiredAccess, //对新线程的访问权限
	__in_opt POBJECT_ATTRIBUTES ObjectAttributes, //指定了线程对象的属性
	__in HANDLE ProcessHandle,      //进程句柄
	__out PCLIENT_ID ClientId,      //返回新线程的ClientId 结构
	__in PCONTEXT ThreadContext,    //新线程的执行环境
	__in PINITIAL_TEB InitialTeb,   //提供新线程的TEB初始值
	__in BOOLEAN CreateSuspended    //新创建的线程是否要先被挂起
	);

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllInformation,
	ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;
typedef NTSTATUS_(WINAPI*NtQueryObject)(
IN HANDLE               ObjectHandle,
IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
OUT PVOID               ObjectInformation,
IN ULONG                Length,
OUT PULONG              ResultLength);
NtQueryObject GetNtQueryObject();
/*一个内核对象有两个计数器：一个是句柄计数，句柄是给用户态用的；另一个是指针计数，也叫引用计数，因为核心态也常常用到内核对象，
为了方便，在核心态的代码用指针直接访问对象，所以Object   Manager维护了这个指针引用计数。
只有在句柄计数和引用计数都为0时，对象才被释放。一般而言，指针引用计数值比句柄计数值大。*/
typedef struct _OBJECT_BASIC_INFORMATION {
	ULONG                   Attributes;
	ACCESS_MASK             DesiredAccess;
	ULONG                   HandleCount;	 //句柄计数 
	ULONG                   ReferenceCount;	//指针引用计数 
	ULONG                   PagedPoolUsage;
	ULONG                   NonPagedPoolUsage;
	ULONG                   Reserved[3];
	ULONG                   NameInformationLength;
	ULONG                   TypeInformationLength;
	ULONG                   SecurityDescriptorLength;
	LARGE_INTEGER           CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;
ULONG GetNtObjectHandleCount(HANDLE ObjectHandle);

typedef NTSTATUS_(WINAPI* ZwOpenProcess)(
	_Out_     PHANDLE ProcessHandle,
	_In_      ACCESS_MASK DesiredAccess,
	_In_      POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_  PCLIENT_ID ClientId
	);

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;

typedef LONG       KPRIORITY;
typedef struct _SYSTEM_THREADS{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	ULONG WaitReason;
	ULONG Reversed;//add by achillis
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREADS;
typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize; //虚拟存储峰值大小；
	ULONG VirtualSize; //虚拟存储大小；
	ULONG PageFaultCount; //页故障数目；
	ULONG PeakWorkingSetSize; //工作集峰值大小；
	ULONG WorkingSetSize; //工作集大小；
	ULONG QuotaPeakPagedPoolUsage; //分页池使用配额峰值；
	ULONG QuotaPagedPoolUsage; //分页池使用配额；
	ULONG QuotaPeakNonPagedPoolUsage;//非分页池使用配额峰值；
	ULONG QuotaNonPagedPoolUsage;//非分页池使用配额；
	ULONG PagefileUsage;//页文件使用情况；
	ULONG PeakPagefileUsage;//页文件使用峰值；
}VM_COUNTERS, *PVM_COUNTERS;
typedef struct _SYSTEM_PROCESSES{
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	LARGE_INTEGER Reserved1[3];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR PageDirectoryBase;
	VM_COUNTERS VmCounters;
	ULONG PrivatePageCount;// add by achillis
	IO_COUNTERS IoCounters;
	_SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

#define MAXIMUM_FILENAME_LENGTH 256
typedef struct _SYSTEM_MODULE {
	ULONG                Reserved1;
	ULONG                Reserved2;
	PVOID                ImageBaseAddress;
	ULONG                ImageSize;
	ULONG                Flags;
	WORD                 Id;
	WORD                 Rank;
	WORD                 w018;
	WORD                 NameOffset;
	BYTE                 Name[MAXIMUM_FILENAME_LENGTH];
} SYSTEM_MODULE, *PSYSTEM_MODULE;

#pragma warning(push)
#pragma warning(disable:4200)
typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG                ModulesCount;
	SYSTEM_MODULE        Modules[0];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;
#pragma warning(pop)

typedef struct
{
	ULONG Unknown; //Always contains zero
	ULONG MaximumIncrement; //一个时钟的计量单位
	ULONG PhysicalPageSize; //一个内存页的大小
	ULONG NumberOfPhysicalPages; //系统管理着多少个页
	ULONG LowestPhysicalPage; //低端内存页
	ULONG HighestPhysicalPage; //高端内存页
	ULONG AllocationGranularity;
	ULONG LowestUserAddress; //地端用户地址
	ULONG HighestUserAddress; //高端用户地址
	ULONG ActiveProcessors; //激活的处理器
	UCHAR NumberProcessors; //有多少个处理器
}SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

typedef LONG(WINAPI* ZwQuerySystemInformation)(
	__in        SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__inout     PVOID SystemInformation,
	__in          ULONG SystemInformationLength,
	__out_opt    PULONG ReturnLength
	);

typedef struct _IO_STATUS_BLOCK
{
	LONG Status;
	ULONG_PTR Information;
}
IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
typedef LONG (NTAPI* NtCreateFile)(
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
	IN ULONG                EaLength);

typedef HANDLE (WINAPI* RtlQueryProcessDebugInformation)(DWORD th32ProcessID, DWORD dwFlags, LPVOID Buffer);
//////////////////////////////////////////////////////////////////////////