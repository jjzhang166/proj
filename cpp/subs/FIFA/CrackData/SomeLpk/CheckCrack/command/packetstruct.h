
#ifndef PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A
#define PACKET_STRUCT_D4CAFD91259D4dc398209AEAC762655A

#include <string>
#include <CD_CommonFiles/CD_CmnDefines.h>
#include <CD_CommonFiles/CD_CmnStructs.h>

typedef struct
{
	PACKET_TYPE		thePacketType;
	WORD			wstrConnectLen;
	std::string		strConnect;
	WORD			wstrKeyLen;
	std::string		strKey;				// 加密解密key值

	// 需要的关键字段
	DWORD			dwGameMainThreadLoopRunHook;
	DWORD			dwGameStaticVertexBufferDrawHook;
	DWORD			dwGameGpuDrawHook;
	DWORD			dwGameViceThreadLoopRunHook;

	WORD			w_snprintfLen;
	std::string		str_snprintf;		// "_snprintf"

	WORD			wZwCreateMutantLen;
	std::string		strZwCreateMutant;	// "ZwCreateMutant"

	WORD			wCreateToolhelp32SnapshotLen;
	std::string		strCreateToolhelp32Snapshot;	// "CreateToolhelp32Snapshot"

	WORD			wEnumProcessesLen;
	std::string		strEnumProcesses;

	WORD			wZwQuerySystemInformationLen;
	std::string		strZwQuerySystemInformation;

	WORD			wZwQueryInformationProcessLen;
	std::string		strZwQueryInformationProcess;

	WORD			wNtCreateFileLen;
	std::string		strNtCreateFile;

	WORD			wZwCreateEventLen;
	std::string		strZwCreateEvent;

	WORD			wFindNextFileALen;
	std::string		strFindNextFileA;

	WORD			wDeviceIoControlLen;
	std::string		strDeviceIoControl;

	WORD			wZwDeviceIoControlFileLen;
	std::string		strZwDeviceIoControlFile;

	WORD			wZwCreateSectionLen;
	std::string		strZwCreateSection;
}JFZR_PACKET_CONNECT,*PJFZR_PACKET_CONNECT;	


typedef struct
{
	PACKET_TYPE		thePacketType;
	WORD			wstrConnectLen;
	std::string		strConnect;
	WORD			wstrKeyLen;
	std::string		strKey;				// 加密解密key值

	DWORD			dwGameSetStreamSourceAddr;
	DWORD			dwGameMainThreadSleepHook;
	DWORD			dwFIFAZFBaseSprintfAddr;
	
	WORD			wCreateToolhelp32SnapshotLen;
	std::string		strCreateToolhelp32Snapshot;

	WORD			wEnumProcessesLen;
	std::string		strEnumProcesses;

	WORD			wEnumProcessModulesLen;
	std::string		strEnumProcessModules;

	WORD			wNtCreateFileLen;
	std::string		strNtCreateFile;

	WORD			wZwQuerySystemInformationLen;
	std::string		strZwQuerySystemInformation;

	WORD			wZwCreateMutantLen;
	std::string		strZwCreateMutant;

	WORD			wstrVersionLen;
	std::string		strVersion;			// 版本号
}FIFA_PACKET_CONNECT,*PFIFA_PACKET_CONNECT;



#endif