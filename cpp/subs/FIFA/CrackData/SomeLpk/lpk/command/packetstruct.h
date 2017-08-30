
#ifndef PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A
#define PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A

#include <string>
#include <CD_CommonFiles/CD_CmnDefines.h>
#include <CD_CommonFiles/CD_CmnStructs.h>


typedef struct
{
	PACKET_TYPE		thePacketType;
	WORD			wstrConnectLen;
	lpkstring		strConnect;
	WORD			wstrKeyLen;
	lpkstring		strKey;				// 加密解密key值

	// 需要的关键字段
	DWORD			dwGameMainThreadLoopRunHook;
	DWORD			dwGameStaticVertexBufferDrawHook;
	DWORD			dwGameGpuDrawHook;
	DWORD			dwGameViceThreadLoopRunHook;

	WORD			w_snprintfLen;
	lpkstring		str_snprintf;		// "_snprintf"

	WORD			wZwCreateMutantLen;
	lpkstring		strZwCreateMutant;	// "ZwCreateMutant"

	WORD			wCreateToolhelp32SnapshotLen;
	lpkstring		strCreateToolhelp32Snapshot;	// "CreateToolhelp32Snapshot"

	WORD			wEnumProcessesLen;
	lpkstring		strEnumProcesses;

	WORD			wZwQuerySystemInformationLen;
	lpkstring		strZwQuerySystemInformation;

	WORD			wZwQueryInformationProcessLen;
	lpkstring		strZwQueryInformationProcess;

	WORD			wNtCreateFileLen;
	lpkstring		strNtCreateFile;

	WORD			wZwCreateEventLen;
	lpkstring		strZwCreateEvent;

	WORD			wFindNextFileALen;
	lpkstring		strFindNextFileA;

	WORD			wDeviceIoControlLen;
	lpkstring		strDeviceIoControl;

	WORD			wZwDeviceIoControlFileLen;
	lpkstring		strZwDeviceIoControlFile;

	WORD			wZwCreateSectionLen;
	lpkstring		strZwCreateSection;
}JFZR_PACKET_CONNECT,*PJFZR_PACKET_CONNECT;	


typedef struct
{
	PACKET_TYPE		thePacketType;
	WORD			wstrConnectLen;
	lpkstring		strConnect;
	WORD			wstrKeyLen;
	lpkstring		strKey;				// 加密解密key值

	DWORD			dwGameSetStreamSourceAddr;
	DWORD			dwGameMainThreadSleepHook;
	DWORD			dwFIFAZFBaseSprintfAddr;		//JFZR_REVERSAL_JFZRBASE_SPRINTF_ADDR___
	
	WORD			wCreateToolhelp32SnapshotLen;
	lpkstring		strCreateToolhelp32Snapshot;

	WORD			wEnumProcessesLen;
	lpkstring		strEnumProcesses;

	WORD			wEnumProcessModulesLen;
	lpkstring		strEnumProcessModules;

	WORD			wNtCreateFileLen;
	lpkstring		strNtCreateFile;

	WORD			wZwQuerySystemInformationLen;
	lpkstring		strZwQuerySystemInformation;

	WORD			wZwCreateMutantLen;
	lpkstring		strZwCreateMutant;

	WORD			wstrVersionLen;
	lpkstring		strVersion;			// 版本号
}FIFA_PACKET_CONNECT,*PFIFA_PACKET_CONNECT;



#endif