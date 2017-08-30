#include "StdAfx.h"
#include "reversalpacket.h"
#include "reversalsock.h"
#include <CD_CommonFiles/CD_Updates.h>

//////////////////////////////////////////////////////////////////////////
// 写包，读包

reversalpacket::reversalpacket(void) : PacketString((const BYTE *)"")
{
}

reversalpacket::reversalpacket(const BYTE* lpPacket) : PacketString(lpPacket)
{
}

reversalpacket::~reversalpacket(void)
{
}

//////////////////////////////////////////////////////////////////////////
// 组包
//Packet*			pthePacket = NULL;

FIFAPacket		theFIFAPacket;

Packet::Packet(void)
{
	
}

Packet::~Packet(void)
{
}

JFZRPacket::JFZRPacket(void)
{

}

JFZRPacket::~JFZRPacket(void)
{
}


int JFZRPacket::connect(char* pszPacketBuffer,char* pszKey,WORD wUrlType)
{
	try
	{
		JFZR_PACKET_CONNECT	thePacket = {0};
		thePacket.thePacketType.wPacketLen = 0;
		thePacket.thePacketType.wMethodType	= REVERSAL_PACKET_SEND_METHOD;
		thePacket.thePacketType.wUrlType	= wUrlType;
		thePacket.strConnect = "connect";
		thePacket.strKey	 = pszKey;

		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket << thePacket.thePacketType.wPacketLen;
		theReversaPacket << thePacket.thePacketType.wMethodType;
		theReversaPacket << thePacket.thePacketType.wUrlType;
		theReversaPacket << thePacket.strConnect;
		theReversaPacket << thePacket.strKey;

		thePacket.dwGameMainThreadLoopRunHook		= 0;
		thePacket.dwGameStaticVertexBufferDrawHook	= 0;
		thePacket.dwGameGpuDrawHook					= 0;
		thePacket.dwGameViceThreadLoopRunHook		= 0;
		theReversaPacket << thePacket.dwGameMainThreadLoopRunHook;
		theReversaPacket << thePacket.dwGameStaticVertexBufferDrawHook;
		theReversaPacket << thePacket.dwGameGpuDrawHook;
		theReversaPacket << thePacket.dwGameViceThreadLoopRunHook;


		thePacket.str_snprintf	="";
		theReversaPacket << thePacket.str_snprintf;

		thePacket.strZwCreateMutant = "";
		theReversaPacket << thePacket.strZwCreateMutant;

		thePacket.strCreateToolhelp32Snapshot	= "";
		theReversaPacket << thePacket.strCreateToolhelp32Snapshot;

		thePacket.strEnumProcesses	= "";
		theReversaPacket << thePacket.strEnumProcesses;

		thePacket.strZwQuerySystemInformation	= "";
		theReversaPacket << thePacket.strZwQuerySystemInformation;

		thePacket.strZwQueryInformationProcess	= "";
		theReversaPacket << thePacket.strZwQueryInformationProcess;

		thePacket.strNtCreateFile	= "";
		theReversaPacket << thePacket.strNtCreateFile;

		thePacket.strZwCreateEvent	= "";
		theReversaPacket << thePacket.strZwCreateEvent;

		thePacket.strFindNextFileA	= "";
		theReversaPacket << thePacket.strFindNextFileA;

		thePacket.strDeviceIoControl	= "";
		theReversaPacket << thePacket.strDeviceIoControl;

		thePacket.strZwDeviceIoControlFile	= "";
		theReversaPacket << thePacket.strZwDeviceIoControlFile;

		thePacket.strZwCreateSection	= "";
		theReversaPacket << thePacket.strZwCreateSection;

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}
	
	return 0;
}

int JFZRPacket::key(char* pszPacketBuffer,char* pszKey,WORD wUrlType)
{
	try
	{
		JFZR_PACKET_CONNECT	thePacket = {0};
		thePacket.thePacketType.wPacketLen = 0;
		thePacket.thePacketType.wMethodType	= REVERSAL_PACKET_SEND_METHOD;
		thePacket.thePacketType.wUrlType	= wUrlType;
		thePacket.strConnect = "connect";
		thePacket.strKey	 = pszKey;

		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket << thePacket.thePacketType.wPacketLen;
		theReversaPacket << thePacket.thePacketType.wMethodType;
		theReversaPacket << thePacket.thePacketType.wUrlType;
		theReversaPacket << thePacket.strConnect;
		theReversaPacket << thePacket.strKey;

		// 填写lpk需要的关键数据
		/*thePacket.dwGameMainThreadLoopRunHook		= UPDATES_JFZR_REVERSAL_MAIN_THREAD_LOOP_RUN_HOOK;
		thePacket.dwGameStaticVertexBufferDrawHook	= UPDATES_JFZR_REVERSAL_STATIC_VERTEX_BUFFER_DRAW_HOOK;
		thePacket.dwGameGpuDrawHook					= UPDATES_JFZR_REVERSAL_GPU_DRAW_HOOK;
		thePacket.dwGameViceThreadLoopRunHook		= UPDATES_JFZR_REVERSAL_VICE_THREAD_LOOP_RUN_HOOK;*/
		thePacket.dwGameMainThreadLoopRunHook		= 0;
		thePacket.dwGameStaticVertexBufferDrawHook	= 0;
		thePacket.dwGameGpuDrawHook					= 0;
		thePacket.dwGameViceThreadLoopRunHook		= 0;
#ifdef _DEBUG_LOG_
		OutputDebugStringA("[LOG] 不能执行到这的");
#endif
		assert(false);

		theReversaPacket << thePacket.dwGameMainThreadLoopRunHook;
		theReversaPacket << thePacket.dwGameStaticVertexBufferDrawHook;
		theReversaPacket << thePacket.dwGameGpuDrawHook;
		theReversaPacket << thePacket.dwGameViceThreadLoopRunHook;


		thePacket.str_snprintf	="printf";
		theReversaPacket << thePacket.str_snprintf;

		thePacket.strZwCreateMutant = "Mutant";
		theReversaPacket << thePacket.strZwCreateMutant;

		thePacket.strCreateToolhelp32Snapshot	= "shot";
		theReversaPacket << thePacket.strCreateToolhelp32Snapshot;

		thePacket.strEnumProcesses	= "Processes";
		theReversaPacket << thePacket.strEnumProcesses;

		thePacket.strZwQuerySystemInformation	= "formation";
		theReversaPacket << thePacket.strZwQuerySystemInformation;

		thePacket.strZwQueryInformationProcess	= "Process";
		theReversaPacket << thePacket.strZwQueryInformationProcess;

		thePacket.strNtCreateFile	= "File";
		theReversaPacket << thePacket.strNtCreateFile;

		thePacket.strZwCreateEvent	= "Event";
		theReversaPacket << thePacket.strZwCreateEvent;

		thePacket.strFindNextFileA	= "A";
		theReversaPacket << thePacket.strFindNextFileA;

		thePacket.strDeviceIoControl	= "Control";
		theReversaPacket << thePacket.strDeviceIoControl;

		thePacket.strZwDeviceIoControlFile	= "File";
		theReversaPacket << thePacket.strZwDeviceIoControlFile;

		thePacket.strZwCreateSection	= "Section";
		theReversaPacket << thePacket.strZwCreateSection;

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}


int JFZRPacket::unconnect(char* pszPacketBuffer)
{
	try
	{
		JFZR_PACKET_CONNECT	thePacket = {0};
		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket >> thePacket.thePacketType.wPacketLen;
		theReversaPacket >> thePacket.thePacketType.wMethodType;
		theReversaPacket >> thePacket.thePacketType.wUrlType;
		theReversaPacket >> thePacket.wstrConnectLen;
		theReversaPacket >> thePacket.strConnect;
		theReversaPacket >> thePacket.wstrKeyLen;
		theReversaPacket >> thePacket.strKey;
		theReversaPacket >> thePacket.dwGameMainThreadLoopRunHook;
		theReversaPacket >> thePacket.dwGameStaticVertexBufferDrawHook;
		theReversaPacket >> thePacket.dwGameGpuDrawHook;
		theReversaPacket >> thePacket.dwGameViceThreadLoopRunHook;
		theReversaPacket >> thePacket.w_snprintfLen;
		theReversaPacket >> thePacket.str_snprintf;
		theReversaPacket >> thePacket.wZwCreateMutantLen;
		theReversaPacket >> thePacket.strZwCreateMutant;
		theReversaPacket >> thePacket.wCreateToolhelp32SnapshotLen;
		theReversaPacket >> thePacket.strCreateToolhelp32Snapshot;
		theReversaPacket >> thePacket.wEnumProcessesLen;
		theReversaPacket >> thePacket.strEnumProcesses;
		theReversaPacket >> thePacket.wZwQuerySystemInformationLen;
		theReversaPacket >> thePacket.strZwQuerySystemInformation;
		theReversaPacket >> thePacket.wZwQueryInformationProcessLen;
		theReversaPacket >> thePacket.strZwQueryInformationProcess;
		theReversaPacket >> thePacket.wNtCreateFileLen;
		theReversaPacket >> thePacket.strNtCreateFile;
		theReversaPacket >> thePacket.wZwCreateEventLen;
		theReversaPacket >> thePacket.strZwCreateEvent;
		theReversaPacket >> thePacket.wFindNextFileALen;
		theReversaPacket >> thePacket.strFindNextFileA;
		theReversaPacket >> thePacket.wDeviceIoControlLen;
		theReversaPacket >> thePacket.strDeviceIoControl;
		theReversaPacket >> thePacket.wZwDeviceIoControlFileLen;
		theReversaPacket >> thePacket.strZwDeviceIoControlFile;
		theReversaPacket >> thePacket.wZwCreateSectionLen;
		theReversaPacket >> thePacket.strZwCreateSection;


		assert(thePacket.thePacketType.wPacketLen == (theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin));

		theFIFADBConnectionManager.dotask(thePacket.thePacketType.wUrlType,&thePacket);

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}



FIFAPacket::FIFAPacket(void)
{

}

FIFAPacket::~FIFAPacket(void)
{
}


int FIFAPacket::connect(char* pszPacketBuffer,char* pszKey,WORD wUrlType)
{
	try
	{
		FIFA_PACKET_CONNECT	thePacket = {0};
		thePacket.thePacketType.wPacketLen = 0;
		thePacket.thePacketType.wMethodType	= REVERSAL_PACKET_SEND_METHOD;
		thePacket.thePacketType.wUrlType	= wUrlType;
		thePacket.strConnect = "connect";
		thePacket.strKey	 = pszKey;

		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket << thePacket.thePacketType.wPacketLen;
		theReversaPacket << thePacket.thePacketType.wMethodType;
		theReversaPacket << thePacket.thePacketType.wUrlType;
		theReversaPacket << thePacket.strConnect;
		theReversaPacket << thePacket.strKey;

		thePacket.dwGameSetStreamSourceAddr		= 0;
		thePacket.dwGameMainThreadSleepHook	= 0;
		thePacket.dwFIFAZFBaseSprintfAddr = 0;
		theReversaPacket << thePacket.dwGameSetStreamSourceAddr;
		theReversaPacket << thePacket.dwGameMainThreadSleepHook;
		theReversaPacket << thePacket.dwFIFAZFBaseSprintfAddr;

		thePacket.strCreateToolhelp32Snapshot	="";
		theReversaPacket << thePacket.strCreateToolhelp32Snapshot;

		thePacket.strEnumProcesses = "";
		theReversaPacket << thePacket.strEnumProcesses;

		thePacket.strEnumProcessModules	= "";
		theReversaPacket << thePacket.strEnumProcessModules;

		thePacket.strNtCreateFile	= "";
		theReversaPacket << thePacket.strNtCreateFile;

		thePacket.strZwQuerySystemInformation	= "";
		theReversaPacket << thePacket.strZwQuerySystemInformation;

		thePacket.strZwCreateMutant	= "";
		theReversaPacket << thePacket.strZwCreateMutant;

		thePacket.strVersion	= "";
		theReversaPacket << thePacket.strVersion;

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}

int FIFAPacket::key(char* pszPacketBuffer,char* pszKey,WORD wUrlType)
{
	try
	{
		FIFA_PACKET_CONNECT	thePacket = {0};
		thePacket.thePacketType.wPacketLen = 0;
		thePacket.thePacketType.wMethodType	= REVERSAL_PACKET_SEND_METHOD;
		thePacket.thePacketType.wUrlType	= wUrlType;
		thePacket.strConnect = "connect";
		thePacket.strKey	 = pszKey;

		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket << thePacket.thePacketType.wPacketLen;
		theReversaPacket << thePacket.thePacketType.wMethodType;
		theReversaPacket << thePacket.thePacketType.wUrlType;
		theReversaPacket << thePacket.strConnect;
		theReversaPacket << thePacket.strKey;

		// 填写lpk需要的关键数据
		thePacket.dwGameSetStreamSourceAddr		= 0x01120280;
		thePacket.dwGameMainThreadSleepHook		= 0x0078C3A7;
		thePacket.dwFIFAZFBaseSprintfAddr		= 0;
		theReversaPacket << thePacket.dwGameSetStreamSourceAddr;
		theReversaPacket << thePacket.dwGameMainThreadSleepHook;
		theReversaPacket << thePacket.dwFIFAZFBaseSprintfAddr;

		thePacket.strCreateToolhelp32Snapshot	= "Snapshot";
		theReversaPacket << thePacket.strCreateToolhelp32Snapshot;

		thePacket.strEnumProcesses = "Processes";
		theReversaPacket << thePacket.strEnumProcesses;

		thePacket.strEnumProcessModules	= "Modules";
		theReversaPacket << thePacket.strEnumProcessModules;

		thePacket.strNtCreateFile	= "File";
		theReversaPacket << thePacket.strNtCreateFile;

		thePacket.strZwQuerySystemInformation	= "Information";
		theReversaPacket << thePacket.strZwQuerySystemInformation;

		thePacket.strZwCreateMutant	= "Mutant";
		theReversaPacket << thePacket.strZwCreateMutant;

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}


int FIFAPacket::unconnect(char* pszPacketBuffer)
{
	try
	{
		FIFA_PACKET_CONNECT	thePacket = {0};
		reversalpacket	theReversaPacket;
		theReversaPacket = (const BYTE*)pszPacketBuffer;
		theReversaPacket >> thePacket.thePacketType.wPacketLen;
		theReversaPacket >> thePacket.thePacketType.wMethodType;
		theReversaPacket >> thePacket.thePacketType.wUrlType;
		theReversaPacket >> thePacket.wstrConnectLen;
		theReversaPacket >> thePacket.strConnect;
		theReversaPacket >> thePacket.wstrKeyLen;
		theReversaPacket >> thePacket.strKey;
		theReversaPacket >> thePacket.dwGameSetStreamSourceAddr;
		theReversaPacket >> thePacket.dwGameMainThreadSleepHook;
		theReversaPacket >> thePacket.dwFIFAZFBaseSprintfAddr;
		theReversaPacket >> thePacket.wCreateToolhelp32SnapshotLen;
		theReversaPacket >> thePacket.strCreateToolhelp32Snapshot;
		theReversaPacket >> thePacket.wEnumProcessesLen;
		theReversaPacket >> thePacket.strEnumProcesses;
		theReversaPacket >> thePacket.wEnumProcessModulesLen;
		theReversaPacket >> thePacket.strEnumProcessModules;
		theReversaPacket >> thePacket.wNtCreateFileLen;
		theReversaPacket >> thePacket.strNtCreateFile;
		theReversaPacket >> thePacket.wZwQuerySystemInformationLen;
		theReversaPacket >> thePacket.strZwQuerySystemInformation;
		theReversaPacket >> thePacket.wZwCreateMutantLen;
		theReversaPacket >> thePacket.strZwCreateMutant;
		theReversaPacket >> thePacket.wstrVersionLen;
		theReversaPacket >> thePacket.strVersion;

		assert(thePacket.thePacketType.wPacketLen == (theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin));

		theFIFADBConnectionManager.dotask(thePacket.thePacketType.wUrlType,&thePacket);

		return theReversaPacket.m_lpPacketCurrent-theReversaPacket.m_lpPacketBegin;
	}
	catch (...)
	{
		assert(false);
	}

	return 0;
}