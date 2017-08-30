#define __in_out

//4. ZwQuerySystemInformation

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
}SYSTEM_BASIC_INFORMATION,*PSYSTEM_BASIC_INFORMATION;

//
// System Information Classes.
//
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

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
typedef LONG       KPRIORITY;

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
}VM_COUNTERS,*PVM_COUNTERS;

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
} SYSTEM_THREAD_INFORMATION,*PSYSTEM_THREADS;

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


//  typedef struct _SYSTEM_PROCESS_INFORMATION
//  {
//      ULONG NextEntryOffset;
//      ULONG NumberOfThreads;
//      LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
//      ULONG HardFaultCount; // since WIN7
//      ULONG NumberOfThreadsHighWatermark; // since WIN7
//      ULONGLONG CycleTime; // since WIN7
//      LARGE_INTEGER CreateTime;
//      LARGE_INTEGER UserTime;
//      LARGE_INTEGER KernelTime;
//      UNICODE_STRING ImageName;
//      KPRIORITY BasePriority;
//      HANDLE UniqueProcessId;
//      HANDLE InheritedFromUniqueProcessId;
//      ULONG HandleCount;
//      ULONG SessionId;
//      ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
//      SIZE_T PeakVirtualSize;
//      SIZE_T VirtualSize;
//      ULONG PageFaultCount;
//      SIZE_T PeakWorkingSetSize;
//      SIZE_T WorkingSetSize;
//      SIZE_T QuotaPeakPagedPoolUsage;
//      SIZE_T QuotaPagedPoolUsage;
//      SIZE_T QuotaPeakNonPagedPoolUsage;
//      SIZE_T QuotaNonPagedPoolUsage;
//      SIZE_T PagefileUsage;
//      SIZE_T PeakPagefileUsage;
//      SIZE_T PrivatePageCount;
//      LARGE_INTEGER ReadOperationCount;
//      LARGE_INTEGER WriteOperationCount;
//      LARGE_INTEGER OtherOperationCount;
//      LARGE_INTEGER ReadTransferCount;
//      LARGE_INTEGER WriteTransferCount;
//      LARGE_INTEGER OtherTransferCount;
//      SYSTEM_THREAD_INFORMATION Threads[1];
// } SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _IO_STATUS_BLOCK
{
	LONG Status;
	ULONG_PTR Information;
}IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;


#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth

