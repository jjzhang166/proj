// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <config/targetver.h>

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <assert.h>
#pragma comment(lib,"User32.lib")

// TODO: 在此处引用程序需要的其他头文件

/*++
// ================================================================= 
// SYSTEM INFO CLASSES 
// ================================================================= 
 
typedef enum _SYSTEMINFOCLASS 
    { 
    SystemBasicInformation,             // 0x002C 
    SystemProcessorInformation,         // 0x000C 
    SystemPerformanceInformation,       // 0x0138 
    SystemTimeInformation,              // 0x0020 
    SystemPathInformation,              // not implemented 
    SystemProcessInformation,           // 0x00C8+ per process 
    SystemCallInformation,              // 0x0018 + (n * 0x0004) 
    SystemConfigurationInformation,     // 0x0018 
    SystemProcessorCounters,            // 0x0030 per cpu 
    SystemGlobalFlag,                   // 0x0004 
    SystemInfo10,                       // not implemented 
    SystemModuleInformation,            // 0x0004 + (n * 0x011C) 
    SystemLockInformation,              // 0x0004 + (n * 0x0024) 
    SystemInfo13,                       // not implemented 
    SystemPagedPoolInformation,         // checked build only 
    SystemNonPagedPoolInformation,      // checked build only 
    SystemHandleInformation,            // 0x0004  + (n * 0x0010) 
    SystemObjectInformation,            // 0x0038+ + (n * 0x0030+) 
    SystemPageFileInformation,          // 0x0018+ per page file 
    SystemInstemulInformation,          // 0x0088 
    SystemInfo20,                       // invalid info class 
    SystemCacheInformation,             // 0x0024 
    SystemPoolTagInformation,           // 0x0004 + (n * 0x001C) 
    SystemInfo23,                       // 0x0000, or 0x0018 per cpu 
    SystemDpcInformation,               // 0x0014 
    SystemInfo25,                       // checked build only 
    SystemLoadDriver,                   // 0x0018, set mode only 
    SystemUnloadDriver,                 // 0x0004, set mode only 
    SystemTimeAdjustmentInformation,    // 0x000C, 0x0008 writeable 
    SystemInfo29,                       // checked build only 
    SystemInfo30,                       // checked build only 
    SystemInfo31,                       // checked build only 
    SystemCrashDumpInformation,         // 0x0004 
    SystemInfo33,                       // 0x0010 
    SystemCrashDumpStateInformation,    // 0x0004 
    SystemDebuggerInformation,          // 0x0002 
    SystemThreadSwitchInformation,      // 0x0030 
    SystemRegistryQuotaInformation,     // 0x000C 
    SystemAddDriver,                    // 0x0008, set mode only 
    SystemPrioritySeparationInformation,// 0x0004, set mode only 
    SystemInfo40,                       // not implemented 
    SystemInfo41,                       // not implemented 
    SystemInfo42,                       // invalid info class 
    SystemInfo43,                       // invalid info class 
    SystemTimeZoneInformation,          // 0x00AC 
    SystemLookasideInformation,         // n * 0x0020 
    MaxSystemInfoClass 
    } 
    SYSTEMINFOCLASS, *PSYSTEMINFOCLASS, **PPSYSTEMINFOCLASS; 
 
// ================================================================= 
// SYSTEM INFO STRUCTURES 
// ================================================================= 
// 00: SystemBasicInformation 
 
typedef struct _SYSTEM_BASIC_INFORMATION 
    { 
    DWORD d00;                      // 0 
    DWORD dKeMaximumIncrement;      // x86: 0x0002625A or 0x00018730 
    DWORD dPageSize;                // bytes 
    DWORD dMmNumberOfPhysicalPages; 
    DWORD dMmLowestPhysicalPage; 
    DWORD dMmHighestPhysicalPage; 
    DWORD dAllocationGranularity;   // bytes 
    PVOID pLowestUserAddress; 
    PVOID pMmHighestUserAddress; 
    DWORD dKeActiveProcessors; 
    BYTE  bKeNumberProcessors; 
    BYTE  bReserved01; 
    WORD  wReserved02; 
    } 
        SYSTEM_BASIC_INFORMATION, 
     * PSYSTEM_BASIC_INFORMATION, 
    **PPSYSTEM_BASIC_INFORMATION; 
 
#define SYSTEM_BASIC_INFORMATION_ \ 
        sizeof (SYSTEM_BASIC_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 01: SystemProcessorInformation 
 
typedef struct _SYSTEM_PROCESSOR_INFORMATION 
    { 
    WORD  wKeProcessorArchitecture; // 0=x86, 1=MIPS, 2=ALPHA, 3=PPC 
    WORD  wKeProcessorLevel;        // x86: family (5=Pentium) 
    WORD  wKeProcessorRevision;     // Pentium: H=model, L=stepping 
    WORD  w06;                      // 0 
    DWORD dKeFeatureBits; 
    } 
        SYSTEM_PROCESSOR_INFORMATION, 
     * PSYSTEM_PROCESSOR_INFORMATION, 
    **PPSYSTEM_PROCESSOR_INFORMATION; 
 
#define SYSTEM_PROCESSOR_INFORMATION_ \ 
        sizeof (SYSTEM_PROCESSOR_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 02: SystemPerformanceInformation 
 
typedef struct _MM_INFO_COUNTERS 
    { 
    DWORD dPageFaults; 
    DWORD d04; 
    DWORD d08; 
    DWORD d0C; 
    DWORD d10; 
    DWORD d14; 
    DWORD d18; 
    DWORD d1C; 
    DWORD d20; 
    DWORD d24; 
    DWORD d28; 
    DWORD d2C; 
    DWORD d30; 
    } 
        MM_INFO_COUNTERS, 
     * PMM_INFO_COUNTERS, 
    **PPMM_INFO_COUNTERS; 
 
#define MM_INFO_COUNTERS_ \ 
        sizeof (MM_INFO_COUNTERS) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_PERFORMANCE_INFORMATION 
    { 
    QWORD            qIdleTime;             // 100 nsec units 
    QWORD            qIoReadTransferCount; 
    QWORD            qIoWriteTransferCount; 
    QWORD            qIoOtherTransferCount; 
    DWORD            dIoReadOperationCount; 
    DWORD            dIoWriteOperationCount; 
    DWORD            dIoOtherOperationCount; 
    DWORD            dMmAvailablePages; 
    DWORD            dMmTotalCommittedPages; 
    DWORD            dMmTotalCommitLimit;   // pages 
    DWORD            dMmPeakCommitLimit;    // pages 
    MM_INFO_COUNTERS MmInfoCounters; 
    DWORD            dPoolPaged;            // pages 
    DWORD            dPoolNonPaged;         // pages 
    DWORD            d078; 
    DWORD            d07C; 
    DWORD            d080; 
    DWORD            d084; 
    DWORD            dMmTotalFreeSystemPages; 
    DWORD            dMmSystemCodePage; 
    DWORD            dMmTotalSystemDriverPages; 
    DWORD            dMmTotalSystemCodePages; 
    DWORD            d098; 
    DWORD            d09C;                  // pages 
    DWORD            d0A0; 
    DWORD            dMmSystemCachePage; 
    DWORD            dMmPagedPoolPage; 
    DWORD            dMmSystemDriverPage; 
    DWORD            dCcFastReadNoWait; 
    DWORD            dCcFastReadWait; 
    DWORD            dCcFastReadResourceMiss; 
    DWORD            dCcFastReadNotPossible; 
    DWORD            dCcFastMdlReadNoWait; 
    DWORD            dCcFastMdlReadWait; 
    DWORD            dCcFastMdlReadResourceMiss; 
    DWORD            dCcFastMdlReadNotPossible; 
    DWORD            dCcMapDataNoWait; 
    DWORD            dCcMapDataWait; 
    DWORD            dCcMapDataNoWaitMiss; 
    DWORD            dCcMapDataWaitMiss; 
    DWORD            dCcPinMappedDataCount; 
    DWORD            dCcPinReadNoWait; 
    DWORD            dCcPinReadWait; 
    DWORD            dCcPinReadNoWaitMiss; 
    DWORD            dCcPinReadWaitMiss; 
    DWORD            dCcCopyReadNoWait; 
    DWORD            dCcCopyReadWait; 
    DWORD            dCcCopyReadNoWaitMiss; 
    DWORD            dCcCopyReadWaitMiss; 
    DWORD            dCcMdlReadNoWait; 
    DWORD            dCcMdlReadWait; 
    DWORD            dCcMdlReadNoWaitMiss; 
    DWORD            dCcMdlReadWaitMiss; 
    DWORD            dCcReadAheadIos; 
    DWORD            dCcLazyWriteIos; 
    DWORD            dCcLazyWritePages; 
    DWORD            dCcDataFlushes; 
    DWORD            dCcDataPages; 
    DWORD            dTotalContextSwitches; // total across cpus 
    DWORD            d12C;                  // total across cpus 
    DWORD            d130;                  // total across cpus 
    DWORD            d134;                  // total across cpus 
    } 
        SYSTEM_PERFORMANCE_INFORMATION, 
     * PSYSTEM_PERFORMANCE_INFORMATION, 
    **PPSYSTEM_PERFORMANCE_INFORMATION; 
 
#define SYSTEM_PERFORMANCE_INFORMATION_ \ 
        sizeof (SYSTEM_PERFORMANCE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 03: SystemTimeInformation 
 
typedef struct _SYSTEM_TIME_INFORMATION 
    { 
    QWORD qKeBootTime;          // relative to 01-01-1601 
    QWORD qKeSystemTime;        // relative to 01-01-1601 
    QWORD dExpTimeZoneBias;     // utc time = local time + bias 
    DWORD dExpCurrentTimeZoneId; 
    DWORD dReserved01; 
    } 
        SYSTEM_TIME_INFORMATION, 
     * PSYSTEM_TIME_INFORMATION, 
    **PPSYSTEM_TIME_INFORMATION; 
 
#define SYSTEM_TIME_INFORMATION_ \ 
        sizeof (SYSTEM_TIME_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 04: SystemPathInformation (not implemented) 
 
// ----------------------------------------------------------------- 
// 05: SystemProcessInformation 
//     see ExpGetProcessInformation() 
//     see also ExpCopyProcessInfo(), ExpCopyThreadInfo() 
 
typedef struct _SYSTEM_THREAD 
    { 
    QWORD        qKernelTime;       // 100 nsec units 
    QWORD        qUserTime;         // 100 nsec units 
    QWORD        qCreateTime;       // relative to 01-01-1601 
    DWORD        d18; 
    PVOID        pStartAddress; 
    CLIENT_ID    Cid;               // process/thread ids 
    DWORD        dPriority; 
    DWORD        dBasePriority; 
    DWORD        dContextSwitches; 
    DWORD        dThreadState;      // 2=running, 5=waiting 
    KWAIT_REASON WaitReason; 
    DWORD        dReserved01; 
    } 
        SYSTEM_THREAD, 
     * PSYSTEM_THREAD, 
    **PPSYSTEM_THREAD; 
 
#define SYSTEM_THREAD_ \ 
        sizeof (SYSTEM_THREAD) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_PROCESS_INFORMATION 
    { 
    DWORD          dNext;           // relative offset 
    DWORD          dThreadCount; 
    DWORD          dReserved01; 
    DWORD          dReserved02; 
    DWORD          dReserved03; 
    DWORD          dReserved04; 
    DWORD          dReserved05; 
    DWORD          dReserved06; 
    QWORD          qCreateTime;     // relative to 01-01-1601 
    QWORD          qUserTime;       // 100 nsec units 
    QWORD          qKernelTime;     // 100 nsec units 
    UNICODE_STRING usName; 
    KPRIORITY      BasePriority; 
    DWORD          dUniqueProcessId; 
    DWORD          dInheritedFromUniqueProcessId; 
    DWORD          dHandleCount; 
    DWORD          dReserved07; 
    DWORD          dReserved08; 
    VM_COUNTERS    VmCounters; 
    DWORD          dCommitCharge;   // bytes 
    SYSTEM_THREAD  ast []; 
    } 
        SYSTEM_PROCESS_INFORMATION, 
     * PSYSTEM_PROCESS_INFORMATION, 
    **PPSYSTEM_PROCESS_INFORMATION; 
 
#define SYSTEM_PROCESS_INFORMATION_ \ 
        sizeof (SYSTEM_PROCESS_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 06: SystemCallInformation 
//     see KeServiceDescriptorTable, KeServiceDescriptorTableShadow 
 
typedef struct _SYSTEM_CALL_INFORMATION 
    { 
    DWORD dSize;                // bytes, including all entries 
    DWORD dTables;              // 2 or 4, 0=ntoskrnl, 1=win32k 
    DWORD adData [];            // table sizes, table data 
    } 
        SYSTEM_CALL_INFORMATION, 
     * PSYSTEM_CALL_INFORMATION, 
    **PPSYSTEM_CALL_INFORMATION; 
 
#define SYSTEM_CALL_INFORMATION_ \ 
        sizeof (SYSTEM_CALL_INFORMATION) 
 
#define SYSTEM_CALL_INFORMATION_VALUES($psci) \ 
        (($psci)->adData + ($psci)->dTables) 
 
// ----------------------------------------------------------------- 
// 07: SystemConfigurationInformation 
//     see IoGetConfigurationInformation () 
 
typedef struct _SYSTEM_CONFIGURATION_INFORMATION 
    { 
    DWORD dDiskCount; 
    DWORD dFloppyCount; 
    DWORD dCDRomCount; 
    DWORD dTapeCount; 
    DWORD dSerialCount;         // com port with mouse not included 
    DWORD dParallelCount; 
    } 
        SYSTEM_CONFIGURATION_INFORMATION, 
     * PSYSTEM_CONFIGURATION_INFORMATION, 
    **PPSYSTEM_CONFIGURATION_INFORMATION; 
 
#define SYSTEM_CONFIGURATION_INFORMATION_ \ 
        sizeof (SYSTEM_CONFIGURATION_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 08: SystemProcessorCounters 
//     see KiProcessorBlock 
 
typedef struct _SYSTEM_PROCESSOR_COUNTERS 
    { 
    QWORD qProcessorTime;       // 100 nsec units 
    QWORD qTotalTime;           // 100 nsec units 
    QWORD qUserTime;            // 100 nsec units 
    QWORD qDpcTime;             // 100 nsec units 
    QWORD qInterruptTime;       // 100 nsec units 
    DWORD dInterruptCount; 
    DWORD dReserved01; 
    } 
        SYSTEM_PROCESSOR_COUNTERS, 
     * PSYSTEM_PROCESSOR_COUNTERS, 
    **PPSYSTEM_PROCESSOR_COUNTERS; 
 
#define SYSTEM_PROCESSOR_COUNTERS_ \ 
        sizeof (SYSTEM_PROCESSOR_COUNTERS) 
 
// ----------------------------------------------------------------- 
// 09: SystemGlobalFlag 
//     see NtGlobalFlag 
 
typedef struct _SYSTEM_GLOBAL_FLAG 
    { 
    DWORD dNtGlobalFlag;        // see Q147314, Q102985, Q105677 
    } 
        SYSTEM_GLOBAL_FLAG, 
     * PSYSTEM_GLOBAL_FLAG, 
    **PPSYSTEM_GLOBAL_FLAG; 
 
#define SYSTEM_GLOBAL_FLAG_ \ 
        sizeof (SYSTEM_GLOBAL_FLAG) 
 
// ----------------------------------------------------------------- 
// 10: SystemInfo10 (not implemented) 
 
// ----------------------------------------------------------------- 
// 11: SystemModuleInformation 
//     see ExpQueryModuleInformation 
 
typedef struct _SYSTEM_MODULE 
    { 
    DWORD dReserved01; 
    DWORD d04; 
    PVOID pAddress; 
    DWORD dSize;                // bytes 
    DWORD dFlags; 
    WORD  wId;                  // zero based 
    WORD  wRank;                // 0 if not assigned 
    WORD  w18; 
    WORD  wNameOffset; 
    BYTE  abName [MAXIMUM_FILENAME_LENGTH]; 
    } 
        SYSTEM_MODULE, 
     * PSYSTEM_MODULE, 
    **PPSYSTEM_MODULE; 
 
#define SYSTEM_MODULE_ \ 
        sizeof (SYSTEM_MODULE) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_MODULE_INFORMATION 
    { 
    DWORD         dCount; 
    SYSTEM_MODULE asm []; 
    } 
        SYSTEM_MODULE_INFORMATION, 
     * PSYSTEM_MODULE_INFORMATION, 
    **PPSYSTEM_MODULE_INFORMATION; 
 
#define SYSTEM_MODULE_INFORMATION_ \ 
        sizeof (SYSTEM_MODULE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 12: SystemLockInformation 
//     see ExpGetLockInformation () 
 
typedef struct _SYSTEM_LOCK 
    { 
    union 
        { 
        PERESOURCE_OLD  pEResourceOld;  // old ERESOURCE format 
        PERESOURCE_LITE pEResourceLite; // new "lite" format 
        PERESOURCE      pEResource;     // current format 
        }; 
    WORD  w04;                  // 1 
    WORD  w06;                  // 0 
    DWORD dOwner; 
    DWORD dActiveCount; 
    DWORD dContentionCount; 
    DWORD dReserved01; 
    DWORD dReserved02; 
    DWORD dNumberOfSharedWaiters; 
    DWORD dNumberOfExclusiveWaiters; 
    } 
        SYSTEM_LOCK, 
     * PSYSTEM_LOCK, 
    **PPSYSTEM_LOCK; 
 
#define SYSTEM_LOCK_ \ 
        sizeof (SYSTEM_LOCK) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_LOCK_INFORMATION 
    { 
    DWORD       dCount; 
    SYSTEM_LOCK asr []; 
    } 
        SYSTEM_LOCK_INFORMATION, 
     * PSYSTEM_LOCK_INFORMATION, 
    **PPSYSTEM_LOCK_INFORMATION; 
 
#define SYSTEM_LOCK_INFORMATION_ \ 
        sizeof (SYSTEM_LOCK_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 13: SystemInfo13 (not implemented) 
 
// ----------------------------------------------------------------- 
// 14: SystemPagedPoolInformation (checked build only) 
//     see ExpGetPoolInformation (PagedPool) 
 
// ----------------------------------------------------------------- 
// 15: SystemNonPagedPoolInformation (checked build only) 
//     see ExpGetPoolInformation (NonPagedPool) 
 
// ----------------------------------------------------------------- 
// 16: SystemHandleInformation 
//     see ExpGetHandleInformation () 
 
typedef struct _SYSTEM_HANDLE 
    { 
    DWORD       dIdProcess; 
    BYTE        bObjectType;    // OB_TYPE_* 
    BYTE        bFlags;         // bits 0..2 HANDLE_FLAG_* 
    WORD        wValue;         // multiple of 4 
    POBJECT     pObject; 
    ACCESS_MASK GrantedAccess; 
    } 
        SYSTEM_HANDLE, 
     * PSYSTEM_HANDLE, 
    **PPSYSTEM_HANDLE; 
 
#define SYSTEM_HANDLE_ \ 
        sizeof (SYSTEM_HANDLE) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_HANDLE_INFORMATION 
    { 
    DWORD         dCount; 
    SYSTEM_HANDLE ash []; 
    } 
        SYSTEM_HANDLE_INFORMATION, 
     * PSYSTEM_HANDLE_INFORMATION, 
    **PPSYSTEM_HANDLE_INFORMATION; 
 
#define SYSTEM_HANDLE_INFORMATION_ \ 
        sizeof (SYSTEM_HANDLE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 17: SystemObjectInformation 
//     see ExpGetObjectInformation() 
 
typedef struct _SYSTEM_OBJECT 
    { 
    DWORD                dNext; // absolute offset 
    POBJECT              pObject; 
    DWORD                dUniqueProcessId; 
    WORD                 w0C; 
    WORD                 wObjectFlags; 
    DWORD                dPointerCount; 
    DWORD                dHandleCount; 
    DWORD                dPagedPoolCharge; 
    DWORD                dNonPagedPoolCharge; 
    DWORD                d20;   // undefined if default pool charges 
    PSECURITY_DESCRIPTOR pSecurityDescriptor; 
    UNICODE_STRING       usObjectName; 
    } 
        SYSTEM_OBJECT, 
     * PSYSTEM_OBJECT, 
    **PPSYSTEM_OBJECT; 
 
#define SYSTEM_OBJECT_ \ 
        sizeof (SYSTEM_OBJECT) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_OBJECT_INFORMATION 
    { 
    DWORD           dNext;          // absolute offset 
    DWORD           dObjectCount; 
    DWORD           dHandleCount; 
    DWORD           dObjectTypeCode;// OB_TYPE_* 
    DWORD           dInvalidAttributes; 
    GENERIC_MAPPING GenericMapping; 
    ACCESS_MASK     ValidAccessMask; 
    POOL_TYPE       PoolType; 
    BOOLEAN         bSecurityRequired; 
    BYTE            bReserved01; 
    WORD            wReserved02; 
    UNICODE_STRING  usTypeName; 
    WORD            awTypeName [];  // usTypeName.MaximumLength / 2 
    } 
        SYSTEM_OBJECT_INFORMATION, 
     * PSYSTEM_OBJECT_INFORMATION, 
    **PPSYSTEM_OBJECT_INFORMATION; 
 
#define SYSTEM_OBJECT_INFORMATION_ \ 
        sizeof (SYSTEM_OBJECT_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 18: SystemPageFileInformation 
//     see MmGetPageFileInformation() 
 
typedef struct _SYSTEM_PAGE_FILE_INFORMATION 
    { 
    DWORD          dNext;       // relative offset 
    DWORD          dTotal;      // pages 
    DWORD          dInUse;      // pages 
    DWORD          dPeak;       // pages 
    UNICODE_STRING usName; 
    } 
        SYSTEM_PAGE_FILE_INFORMATION, 
     * PSYSTEM_PAGE_FILE_INFORMATION, 
    **PPSYSTEM_PAGE_FILE_INFORMATION; 
 
#define SYSTEM_PAGE_FILE_INFORMATION_ \ 
        sizeof (SYSTEM_PAGE_FILE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 19: SystemInstemulInformation 
//     see ExpGetInstemulInformation () 
 
typedef struct _SYSTEM_INSTEMUL_INFORMATION 
    { 
    BOOL  fExVdmSegmentNotPresent; 
    DWORD dOpcode0FV86; 
    DWORD dOpcodeESPrefixV86; 
    DWORD dOpcodeCSPrefixV86; 
    DWORD dOpcodeSSPrefixV86; 
    DWORD dOpcodeDSPrefixV86; 
    DWORD dOpcodeFSPrefixV86; 
    DWORD dOpcodeGSPrefixV86; 
    DWORD dOpcodeOPER32PrefixV86; 
    DWORD dOpcodeADDR32PrefixV86; 
    DWORD dOpcodeINSBV86; 
    DWORD dOpcodeINSWV86; 
    DWORD dOpcodeOUTSBV86; 
    DWORD dOpcodeOUTSWV86; 
    DWORD dOpcodePUSHFV86; 
    DWORD dOpcodePOPFV86; 
    DWORD dOpcodeINTnnV86; 
    DWORD dOpcodeINTOV86; 
    DWORD dOpcodeIRETV86; 
    DWORD dOpcodeINBimmV86; 
    DWORD dOpcodeINWimmV86; 
    DWORD dOpcodeOUTBimmV86; 
    DWORD dOpcodeOUTWimmV86; 
    DWORD dOpcodeINBV86; 
    DWORD dOpcodeINWV86; 
    DWORD dOpcodeOUTBV86; 
    DWORD dOpcodeOUTWV86; 
    DWORD dOpcodeLOCKPrefixV86; 
    DWORD dOpcodeREPNEPrefixV86; 
    DWORD dOpcodeREPPrefixV86; 
    DWORD dOpcodeHLTV86; 
    DWORD dOpcodeCLIV86; 
    DWORD dOpcodeSTIV86; 
    DWORD dVdmBopCount; 
    } 
        SYSTEM_INSTEMUL_INFORMATION, 
     * PSYSTEM_INSTEMUL_INFORMATION, 
    **PPSYSTEM_INSTEMUL_INFORMATION; 
 
#define SYSTEM_INSTEMUL_INFORMATION_ \ 
        sizeof (SYSTEM_INSTEMUL_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 20: SystemInfo20 (invalid info class) 
 
// ----------------------------------------------------------------- 
// 21: SystemCacheInformation 
//     see MmSystemCacheWs 
 
typedef struct _SYSTEM_CACHE_INFORMATION 
    { 
    DWORD dFileCache;           // bytes 
    DWORD dFileCachePeak;       // bytes 
    DWORD d08; 
    DWORD d0C; 
    DWORD d10; 
    DWORD d14; 
    DWORD d18; 
    DWORD d1C; 
    DWORD d20; 
    } 
        SYSTEM_CACHE_INFORMATION, 
     * PSYSTEM_CACHE_INFORMATION, 
    **PPSYSTEM_CACHE_INFORMATION; 
 
#define SYSTEM_CACHE_INFORMATION_ \ 
        sizeof (SYSTEM_CACHE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 22: SystemPoolTagInformation 
//     see ExpGetPoolTagInfo () 
 
typedef struct _SYSTEM_POOL_TAG 
    { 
    DWORD dTag; 
    DWORD dPagedAllocs; 
    DWORD dPagedFrees; 
    DWORD dPagedBytes; 
    DWORD dNonPagedAllocs; 
    DWORD dNonPagedFrees; 
    DWORD dNonPagedBytes; 
    } 
        SYSTEM_POOL_TAG, 
     * PSYSTEM_POOL_TAG, 
    **PPSYSTEM_POOL_TAG; 
 
#define SYSTEM_POOL_TAG_ \ 
        sizeof (SYSTEM_POOL_TAG) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_POOL_TAG_INFORMATION 
    { 
    DWORD           dCount; 
    SYSTEM_POOL_TAG aspt []; 
    } 
        SYSTEM_POOL_TAG_INFORMATION, 
     * PSYSTEM_POOL_TAG_INFORMATION, 
    **PPSYSTEM_POOL_TAG_INFORMATION; 
 
#define SYSTEM_POOL_TAG_INFORMATION_ \ 
        sizeof (SYSTEM_POOL_TAG_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 24: SystemDpcInformation 
 
typedef struct _SYSTEM_DPC_INFORMATION 
    { 
    DWORD dReserved01; 
    DWORD dKiMaximumDpcQueueDepth; 
    DWORD dKiMinimumDpcRate; 
    DWORD dKiAdjustDpcThreshold; 
    DWORD dKiIdealDpcRate; 
    } 
        SYSTEM_DPC_INFORMATION, 
     * PSYSTEM_DPC_INFORMATION, 
    **PPSYSTEM_DPC_INFORMATION; 
 
#define SYSTEM_DPC_INFORMATION_ \ 
        sizeof (SYSTEM_DPC_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 25: SystemInfo25 (checked build only) 
 
// ----------------------------------------------------------------- 
// 26: SystemLoadDriver (set mode only) 
//     see MmLoadSystemImage() 
//     user mode: STATUS_PRIVILEGE_NOT_HELD returned 
 
typedef struct _SYSTEM_LOAD_DRIVER 
    { 
    UNICODE_STRING usImageFile;     // input 
    PVOID          pBaseAddress;    // output 
    HANDLE         hSystemImage;    // output 
    PVOID          pEntryPoint;     // output 
    PVOID          pDirectoryEntry; // output 
    } 
        SYSTEM_LOAD_DRIVER, 
     * PSYSTEM_LOAD_DRIVER, 
    **PPSYSTEM_LOAD_DRIVER; 
 
#define SYSTEM_LOAD_DRIVER_ \ 
        sizeof (SYSTEM_LOAD_DRIVER) 
 
// ----------------------------------------------------------------- 
// 27: SystemUnloadDriver (set mode only) 
//     see MmUnloadSystemImage() 
//     user mode: STATUS_PRIVILEGE_NOT_HELD returned 
 
typedef struct _SYSTEM_UNLOAD_DRIVER 
    { 
    HANDLE hSystemImage;            // received via SystemLoadDriver 
    } 
        SYSTEM_UNLOAD_DRIVER, 
     * PSYSTEM_UNLOAD_DRIVER, 
    **PPSYSTEM_UNLOAD_DRIVER; 
 
#define SYSTEM_UNLOAD_DRIVER_ \ 
        sizeof (SYSTEM_UNLOAD_DRIVER) 
 
// ----------------------------------------------------------------- 
// 28: SystemTimeAdjustmentInformation 
 
typedef struct _SYSTEM_TIME_ADJUSTMENT_PARAMS 
    { 
    DWORD dKeTimeAdjustment; 
    DWORD dKeMaximumIncrement; 
    } 
        SYSTEM_TIME_ADJUSTMENT_PARAMS, 
     * PSYSTEM_TIME_ADJUSTMENT_PARAMS, 
    **PPSYSTEM_TIME_ADJUSTMENT_PARAMS; 
 
#define SYSTEM_TIME_ADJUSTMENT_PARAMS_ \ 
        sizeof (SYSTEM_TIME_ADJUSTMENT_PARAMS) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_TIME_ADJUSTMENT_INFORMATION 
    { 
    SYSTEM_TIME_ADJUSTMENT_PARAMS TimeAdjustmentParams; 
    BOOLEAN                       bKeTimeSynchronization; 
    BYTE                          bReserved01; 
    WORD                          wReserved02; 
    } 
        SYSTEM_TIME_ADJUSTMENT_INFORMATION, 
     * PSYSTEM_TIME_ADJUSTMENT_INFORMATION, 
    **PPSYSTEM_TIME_ADJUSTMENT_INFORMATION; 
 
#define SYSTEM_TIME_ADJUSTMENT_INFORMATION_ \ 
        sizeof (SYSTEM_TIME_ADJUSTMENT_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 29: SystemInfo29 (checked build only) 
 
// ----------------------------------------------------------------- 
// 30: SystemInfo30 (checked build only) 
 
// ----------------------------------------------------------------- 
// 31: SystemInfo31 (checked build only) 
 
// ----------------------------------------------------------------- 
// 32: SystemCrashDumpInformation 
//     see MmGetCrashDumpInformation () 
 
typedef struct _SYSTEM_CRASH_DUMP_INFORMATION 
    { 
    HANDLE hMmCrashDumpSection; 
    } 
        SYSTEM_CRASH_DUMP_INFORMATION, 
     * PSYSTEM_CRASH_DUMP_INFORMATION, 
    **PPSYSTEM_CRASH_DUMP_INFORMATION; 
 
#define SYSTEM_CRASH_DUMP_INFORMATION_ \ 
        sizeof (SYSTEM_CRASH_DUMP_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 34: SystemCrashDumpStateInformation 
//     see MmGetCrashDumpStateInformation () 
 
typedef struct _SYSTEM_CRASH_DUMP_STATE_INFORMATION 
    { 
    BOOL fCrashDumpEnabled; 
    } 
        SYSTEM_CRASH_DUMP_STATE_INFORMATION, 
     * PSYSTEM_CRASH_DUMP_STATE_INFORMATION, 
    **PPSYSTEM_CRASH_DUMP_STATE_INFORMATION; 
 
#define SYSTEM_CRASH_DUMP_STATE_INFORMATION_ \ 
        sizeof (SYSTEM_CRASH_DUMP_STATE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 35: SystemDebuggerInformation 
 
typedef struct _SYSTEM_DEBUGGER_INFORMATION 
    { 
    BOOLEAN bKdDebuggerEnabled; 
    BOOLEAN bKdDebuggerNotPresent; 
    } 
        SYSTEM_DEBUGGER_INFORMATION, 
     * PSYSTEM_DEBUGGER_INFORMATION, 
    **PPSYSTEM_DEBUGGER_INFORMATION; 
 
#define SYSTEM_DEBUGGER_INFORMATION_ \ 
        sizeof (SYSTEM_DEBUGGER_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 36: SystemThreadSwitchInformation 
//     see KiProcessorBlock, KeThreadSwitchCounters 
 
typedef struct _SYSTEM_THREAD_SWITCH_INFORMATION 
    { 
    DWORD dTotalContextSwitches; 
    DWORD dFindAnyProcessor; 
    DWORD dFindLastProcessor; 
    DWORD d0C; 
    DWORD dIdleAnyProcessor; 
    DWORD dIdleCurrentProcessor; 
    DWORD dIdleLastProcessor; 
    DWORD d1C; 
    DWORD dPreemptAnyProcessor; 
    DWORD dPreemptCurrentProcessor; 
    DWORD dPreemptLastProcessor; 
    DWORD dSwitchToIdle; 
    } 
        SYSTEM_THREAD_SWITCH_INFORMATION, 
     * PSYSTEM_THREAD_SWITCH_INFORMATION, 
    **PPSYSTEM_THREAD_SWITCH_INFORMATION; 
 
#define SYSTEM_THREAD_SWITCH_INFORMATION_ \ 
        sizeof (SYSTEM_THREAD_SWITCH_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 37: SystemRegistryQuotaInformation 
//     see CmQueryRegistryQuotaInformation () 
 
typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION 
    { 
    DWORD dCmpGlobalQuota;              // bytes 
    DWORD dCmpGlobalQuotaUsed;          // bytes 
    DWORD dMmSizeOfPagedPoolInBytes;    // bytes 
    } 
        SYSTEM_REGISTRY_QUOTA_INFORMATION, 
     * PSYSTEM_REGISTRY_QUOTA_INFORMATION, 
    **PPSYSTEM_REGISTRY_QUOTA_INFORMATION; 
 
#define SYSTEM_REGISTRY_QUOTA_INFORMATION_ \ 
        sizeof (SYSTEM_REGISTRY_QUOTA_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 38: SystemAddDriver (set mode only) 
//     see MmLoadSystemImage(), MmUnloadSystemImage() 
//     user mode: SeLoadDriverPrivilege required 
 
typedef struct _SYSTEM_ADD_DRIVER 
    { 
    UNICODE_STRING usImageFile; 
    } 
        SYSTEM_ADD_DRIVER, 
     * PSYSTEM_ADD_DRIVER, 
    **PPSYSTEM_ADD_DRIVER; 
 
#define SYSTEM_ADD_DRIVER_ \ 
        sizeof (SYSTEM_ADD_DRIVER) 
 
// ----------------------------------------------------------------- 
// 39: SystemPrioritySeparationInformation (set mode only) 
//     see PsPrioritySeparation 
 
typedef struct _SYSTEM_PRIORITY_SEPARATION_INFORMATION 
    { 
    DWORD dPsPrioritySeparation;        // 0..2 
    } 
        SYSTEM_PRIORITY_SEPARATION_INFORMATION, 
     * PSYSTEM_PRIORITY_SEPARATION_INFORMATION, 
    **PPSYSTEM_PRIORITY_SEPARATION_INFORMATION; 
 
#define SYSTEM_PRIORITY_SEPARATION_INFORMATION_ \ 
        sizeof (SYSTEM_PRIORITY_SEPARATION_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 40: SystemInfo40 (not implemented) 
 
// ----------------------------------------------------------------- 
// 41: SystemInfo41 (not implemented) 
 
// ----------------------------------------------------------------- 
// 42: SystemInfo42 (invalid info class) 
 
// ----------------------------------------------------------------- 
// 43: SystemInfo43 (invalid info class) 
 
// ----------------------------------------------------------------- 
// 44: SystemTimeZoneInformation 
//     see ExpTimeZoneInformation 
 
typedef struct _SYSTEM_TIME_ZONE_INFORMATION 
    { 
    LONG        lBias; 
    WORD        awStandardName [32]; 
    TIME_FIELDS tfStandardDate; 
    LONG        lStandardBias; 
    WORD        awDaylightName [32]; 
    TIME_FIELDS tfDaylightDate; 
    LONG        lDaylightBias; 
    } 
        SYSTEM_TIME_ZONE_INFORMATION, 
     * PSYSTEM_TIME_ZONE_INFORMATION, 
    **PPSYSTEM_TIME_ZONE_INFORMATION; 
 
#define SYSTEM_TIME_ZONE_INFORMATION_ \ 
        sizeof (SYSTEM_TIME_ZONE_INFORMATION) 
 
// ----------------------------------------------------------------- 
// 45: SystemLookasideInformation 
//     see ExpGetLookasideInformation () 
 
typedef struct _SYSTEM_LOOKASIDE 
    { 
    WORD      wDepthListHead; 
    WORD      wDepth; 
    DWORD     dTotalAllocates; 
    DWORD     dAllocateNoMisses; 
    DWORD     dTotalFrees; 
    DWORD     dFreeNoMisses; 
    POOL_TYPE PoolType; 
    DWORD     dPoolTag; 
    DWORD     dSize; 
    } 
        SYSTEM_LOOKASIDE, 
     * PSYSTEM_LOOKASIDE, 
    **PPSYSTEM_LOOKASIDE; 
 
#define SYSTEM_LOOKASIDE_ \ 
        sizeof (SYSTEM_LOOKASIDE) 
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
typedef struct _SYSTEM_LOOKASIDE_INFORMATION 
    { 
    SYSTEM_LOOKASIDE asl []; 
    } 
        SYSTEM_LOOKASIDE_INFORMATION, 
     * PSYSTEM_LOOKASIDE_INFORMATION, 
    **PPSYSTEM_LOOKASIDE_INFORMATION; 
 
#define SYSTEM_LOOKASIDE_INFORMATION_ \ 
        sizeof (SYSTEM_LOOKASIDE_INFORMATION) 
 
// ================================================================= 
// API PROTOTYPES 
// ================================================================= 
 
NTSTATUS NTAPI 
NtQuerySystemInformation (SYSTEMINFOCLASS sic, 
                          PVOID           pData, 
                          DWORD           dSize, 
                          PDWORD          pdSize); 
 
NTSTATUS NTAPI 
NtSetSystemInformation (SYSTEMINFOCLASS sic, 
                        PVOID           pData, 
                        DWORD           dSize); 
 
// ================================================================= 
// END OF MAIN #ifndef CLAUSE 
// ================================================================= 
 
#endif // _NTSYS_H_ 
 
// ================================================================= 
// END OF FILE 
// ================================================================= 

 
--*/


typedef enum _FILE_INFORMATION_CLASS { 
	FileDirectoryInformation                 = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileObjectIdInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileAttributeTagInformation,
	FileTrackingInformation,
	FileIdBothDirectoryInformation,
	FileIdFullDirectoryInformation,
	FileValidDataLengthInformation,
	FileShortNameInformation,
	FileIoCompletionNotificationInformation,
	FileIoStatusBlockRangeInformation,
	FileIoPriorityHintInformation,
	FileSfioReserveInformation,
	FileSfioVolumeInformation,
	FileHardLinkInformation,
	FileProcessIdsUsingFileInformation,
	FileNormalizedNameInformation,
	FileNetworkPhysicalNameInformation,
	FileIdGlobalTxDirectoryInformation,
	FileIsRemoteDeviceInformation,
	FileAttributeCacheInformation,
	FileNumaNodeInformation,
	FileStandardLinkInformation,
	FileRemoteProtocolInformation,
	FileReplaceCompletionInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;