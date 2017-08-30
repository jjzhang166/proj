#include "stdafx.h"
#include <Common/UsefulFunc.h>
#include <TlHelp32.h>
#include <StrSafe.h>
#include "Protocol/P_Defines.h"
#include <Common/LogOut.h>
#include <cstring>
#include <boost/locale/encoding.hpp>
#include <ShlObj.h>
//#include <boost/random/random_device.hpp>
//#include <boost/random/uniform_int_distribution.hpp>
#include "Net/IoTimer.h"
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <WinIoCtl.h>
#include <Nb30.h>
#include <NTDDSCSI.h>
#include <protect/Protect.h>
#include <Poco/LocalDateTime.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <Common/UsefulClass.h>
#include <Common/EnsureCleanup.h>
#include <Common/CmnException.h>
#include <Net/IoConnect.h>
#include <boost/lexical_cast.hpp>
#include <comdef.h>
#include <Wbemidl.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Crypto/RSAKey.h>
#include <Script/LuaScript.h>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>  
#include <boost/algorithm/string.hpp>
#include <Common/UsefulClass.h>
#include <ras.h>
#include <CommonX/CmnX_HookerMore.h>
#include <Common/CmnDllApi.h>
#include <CmnMix/Cmn_Toolhelp.h>
#include <AppBase/Cmn_App.h>
//#include <VersionHelpers.h>
#include <Shlobj.h>

#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Rasapi32.lib")
#pragma comment(lib, "shell32.lib")

#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088
#define  IDE_ATAPI_IDENTIFY  0xA1
#define  IDE_ATA_IDENTIFY    0xEC
#define  SENDIDLENGTH  ( sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE )
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ( ( FILE_DEVICE_SCSI << 16 ) + 0x0501 )
typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

void WINAPI SupendProcess( bool is_suspend )
{
	LOG_O_EMPTY_MSG_FAST(Log_trace);

	const auto kProcessId = GetCurrentProcessId();
	const auto kThreadId = GetCurrentThreadId();
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, kProcessId);
	if ( snapshot != INVALID_HANDLE_VALUE )
	{
		THREADENTRY32 te = { sizeof(te) };
		BOOL is_ok = Thread32First(snapshot, &te);
		for ( ; is_ok; is_ok = Thread32Next(snapshot, &te) )
		{
			if ( te.th32OwnerProcessID == kProcessId )
			{
				if ( kThreadId == te.th32ThreadID )
					continue;
				HANDLE thread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
				if (thread)
				{
					if (is_suspend)
						SuspendThread(thread);
					else
						ResumeThread(thread);
				}
				CloseHandle(thread);
			}
		}
		CloseHandle(snapshot);
	}
}

BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) {
	if (!pszLibFile)
	{
		assert(false);
		return FALSE;
	}
	BOOL bOk = FALSE; // Assume that the function fails
	HANDLE hthSnapshot = NULL;
	HANDLE hProcess = NULL, hThread = NULL;
	SIZE_T mem_size = 0;
	LPVOID mem_addr = nullptr;
	__try {
		// Grab a new snapshot of the process
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if (hthSnapshot != INVALID_HANDLE_VALUE)
		{
			// Get the HMODULE of the desired library
			MODULEENTRY32W me = { sizeof(me) };
			BOOL bFound = FALSE;
			BOOL bMoreMods = Module32FirstW(hthSnapshot, &me);
			for (; bMoreMods; bMoreMods = Module32NextW(hthSnapshot, &me)) {
				bFound = (_wcsicmp(me.szModule, pszLibFile) == 0) ||
					(_wcsicmp(me.szExePath, pszLibFile) == 0);
				if (bFound) break;
			}
			if (bFound)
			{
				bOk = TRUE;
				__leave;
			}
		}

		// Get a handle for the target process.
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |   // Required by Alpha
			PROCESS_CREATE_THREAD | // For CreateRemoteThread
			PROCESS_VM_OPERATION | //for VirtualAllocEx/VirtualFreeEx
			PROCESS_VM_WRITE,		  	//for WriteProcessMemory
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;

		// Get the real address of FreeLibrary in Kernel32.dll
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			LibFuncLoader::GetTheProcAddress(LibraryLoader::GetLib_Kernel32().GetLibrary(), "LoadLibraryW");
		if (pfnThreadRtn == NULL) __leave;

		//////////////////////////////////////////////////////////////////////////
		mem_size = (wcslen(pszLibFile) + 1) * sizeof(*pszLibFile);
		mem_addr = VirtualAllocEx(hProcess, nullptr, mem_size, MEM_COMMIT, PAGE_READWRITE);
		if (!mem_addr)
		{
			assert(false);
			__leave;
		}
		if (!WriteProcessMemory_(hProcess, mem_addr, pszLibFile, mem_size, nullptr))
		{
			assert(false);
			__leave;
		}
		//////////////////////////////////////////////////////////////////////////
		// Create a remote thread that calls FreeLibrary()
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			pfnThreadRtn, mem_addr, 0, NULL);
		if (hThread == nullptr)
		{
			auto nt_create_thread = GetNtCreateThreadEx();
			if (!nt_create_thread)
				__leave;
			nt_create_thread(&hThread, 0x1FFFFF, nullptr, hProcess, pfnThreadRtn, mem_addr, 0, 0, 0, 0, nullptr);
		}
		if (hThread == NULL) __leave;

		// Wait for the remote thread to terminate
		WaitForSingleObject(hThread, INFINITE);

		bOk = TRUE; // Everything executed successfully
	}
	__finally { // Now we can clean everything up

		if (hthSnapshot != NULL && hthSnapshot != INVALID_HANDLE_VALUE)
			CloseHandle(hthSnapshot);
		if (mem_addr != nullptr)
			VirtualFreeEx(hProcess, mem_addr, mem_size, MEM_RELEASE);

		if (hThread != NULL)
			CloseHandle(hThread);

		if (hProcess != NULL)
			CloseHandle(hProcess);
	}

	return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {
	const auto& wstr = GbkToWideChar(pszLibFile);
	return(InjectLibW(dwProcessId, wstr.c_str()));
}


///////////////////////////////////////////////////////////////////////////////

bool AdjustProcessTokenPrivilege()
{
	LUID luidTmp;
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		assert(false);
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidTmp))
	{
		assert(false);
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luidTmp;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		assert(false);
		CloseHandle(hToken);
		return false;
	}
	return true;
}

BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) {
	BOOL bOk = FALSE; // Assume that the function fails
	HANDLE hthSnapshot = NULL;
	HANDLE hProcess = NULL, hThread = NULL;

	__try {
		// Grab a new snapshot of the process
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if (hthSnapshot == INVALID_HANDLE_VALUE) __leave;

		// Get the HMODULE of the desired library
		MODULEENTRY32W me = { sizeof(me) };
		BOOL bFound = FALSE;
		BOOL bMoreMods = Module32FirstW(hthSnapshot, &me);
		for (; bMoreMods; bMoreMods = Module32NextW(hthSnapshot, &me)) {
			bFound = (_wcsicmp(me.szModule, pszLibFile) == 0) ||
				(_wcsicmp(me.szExePath, pszLibFile) == 0);
			if (bFound) break;
		}
		if (!bFound) __leave;

		// Get a handle for the target process.
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION |
			PROCESS_CREATE_THREAD |
			PROCESS_VM_OPERATION,  // For CreateRemoteThread
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;

		// Get the real address of FreeLibrary in Kernel32.dll
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			LibFuncLoader::GetTheProcAddress(LibraryLoader::GetLib_Kernel32().GetLibrary(), "FreeLibrary");
		if (pfnThreadRtn == NULL) __leave;

		// Create a remote thread that calls FreeLibrary()
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			pfnThreadRtn, me.modBaseAddr, 0, NULL);
		if (hThread == NULL) __leave;

		// Wait for the remote thread to terminate
		WaitForSingleObject(hThread, INFINITE);

		bOk = TRUE; // Everything executed successfully
	}
	__finally { // Now we can clean everything up

		if (hthSnapshot != NULL)
			CloseHandle(hthSnapshot);

		if (hThread != NULL)
			CloseHandle(hThread);

		if (hProcess != NULL)
			CloseHandle(hProcess);
	}

	return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI EjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {
	const auto& wstr = GbkToWideChar(pszLibFile);
	return(EjectLibW(dwProcessId, wstr.c_str()));
}

CString GetBrowseFolder( HWND pParent )
{
	TCHAR        szDir[MAX_PATH] = {0};
	BROWSEINFO    bi;
	ITEMIDLIST    *pidl = NULL;

	bi.hwndOwner = pParent;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = nullptr;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);
	if(NULL == pidl)
		return _T("");
	if(!SHGetPathFromIDList(pidl, szDir))  
		return _T("");

	return CString(szDir) + '\\';
}

std::random_device& GetDltRandomDevice()
{
	static std::random_device rng;
	return rng;
}

std::string Random_Str( size_t cnt_min, size_t cnt_max )
{
	static const char kChars[] =
		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	using namespace std;
	uniform_int_distribution<> index_dist(0, _countof(kChars) - sizeof(char));
	std::string res;
	auto& rng = GetDltRandomDevice();
	uniform_int_distribution<> char_cnt(cnt_min, cnt_max);
	cnt_max = char_cnt(rng);
	for( size_t i = 0; i < cnt_max; ++i )
	{
		res.push_back(kChars[index_dist(rng)]);
	}
	return std::move(res);
}

struct stCloseWndHelper{
	DWORD				process_id_;
	HWND				hwnd_but_;
	std::vector<HWND>	but_hwnds_;

public:
	bool FindTheHwnd( HWND hwnd ) const{
		assert(hwnd);
		auto it_end = but_hwnds_.end();
		auto it = std::find(but_hwnds_.begin(), it_end, hwnd);
		return it != it_end;
	}
	static BOOL CALLBACK EnumWindowsProc__( HWND hwnd, LPARAM lParam ){
		auto ptr_hlp = (stCloseWndHelper*)lParam;
		DWORD the_id;
		GetWindowThreadProcessId(hwnd, &the_id);
		if ( the_id != ptr_hlp->process_id_ )
			return TRUE;
		assert(ptr_hlp);
		if (ptr_hlp->FindTheHwnd(hwnd))
			return TRUE;
		HWND hwnd_parent = GetParent(hwnd);
		while (hwnd_parent)
		{
			hwnd = hwnd_parent;
			hwnd_parent = GetParent(hwnd);
		}
		if (ptr_hlp->FindTheHwnd(hwnd))
			return TRUE;
		ptr_hlp->but_hwnds_.push_back(hwnd);
		//SendMessage不好使，有时会卡在这个地方
		PostMessage(hwnd, WM_CLOSE, 0, 0);
		return TRUE;
	}
};

bool WaitUntil(const FunctionBool& func_until)
{
	auto& io_ser = GetIoService();
	while (!func_until() && io_ser.RunOne()){}
	return true;
}

bool WaitUntil(pt_dword timeout, const FunctionBool& func_until)
{
	if (timeout <= 0)
		return WaitUntil(func_until);

	auto& io_ser = GetIoService();
	DEFINE_DEFAULT_PAUSE_ID(timeout);
	try{
		auto timer = CreateIoTimer(timeout, false, [pause_id](const IoTimerPtr&){
			throw CmnPauseLogicException(pause_id, "waituntil");
		});
		if ( !timer )
		{
			assert(false);
			return false;
		}
		timer->Continue();
		BOOST_SCOPE_EXIT_ALL(&timer){
			assert(timer);
			timer->Cancel();
		};
		while ( !func_until() && io_ser.RunOne() ) {}
		return true;
	} DO_CATCH_DEFAULT_PAUSE_ID({
		return func_until();
	});
	
	assert(false);
	return false;
}

bool DoWaitUntil(const FunctionBool& func_until, const FunctionVoid& func_do, pt_dword timeout)
{
	auto& io_ser = GetIoService();
	if ( timeout > 0 )
	{
		DEFINE_THE_PAUSE_ID(pause_id_timeout, timeout);
		try{
			auto timer_timeout = CreateIoTimer(timeout, false, [pause_id_timeout](const IoTimerPtr&){
				throw CmnPauseLogicException(pause_id_timeout, "dowaituntil");
			});
			if ( !timer_timeout )
			{
				assert(false);
				return false;
			}
			timer_timeout->Continue();
			BOOST_SCOPE_EXIT_ALL(&timer_timeout){
				assert(timer_timeout);
				timer_timeout->Cancel();
			};
			while (true)
			{
				if (func_until())
					return true;
				io_ser.PollOne();
				DummySleep(50);
				func_do();
			}
		} DO_CATCH_THE_PAUSE_ID(pause_id_timeout, {
			return func_until();
		});
	} 
	else
	{
		while (true)
		{
			if (func_until())
				return true;
			io_ser.PollOne();
			DummySleep(50);
			func_do();
		}
	}
	
	assert(false);
	return false;
}

bool TimerDoWaitUntil(pt_dword timer_time, pt_dword timeout, const FunctionBool& func_until, const FunctionVoid& func_do)
{
	if ( timer_time <= 0 )
	{
		assert(false);
		return false;
	}
	if (func_until())
		return true;
	IoTimerPtr timer_do, timer_timeout;
	DEFINE_THE_PAUSE_ID(pause_id_timer_do, timer_time);
	DEFINE_THE_PAUSE_ID(pause_id_timeout, timeout);
	try{
		timer_do = CreateIoTimer(timer_time, false,
			[&func_do, &func_until, pause_id_timer_do](const IoTimerPtr& timer_ptr){
			func_do();
			if (func_until())
				throw CmnPauseLogicException(pause_id_timer_do, "timerdowaituntil1");
			if (!timer_ptr)
			{
				assert(false);
				return;
			}
			assert(timer_ptr);
			timer_ptr->Continue();
		});
		if ( !timer_do )
		{
			assert(false);
			return false;
		}
		timer_do->Continue();
		BOOST_SCOPE_EXIT_ALL(&timer_do, &timer_timeout){
			assert(timer_do);
			timer_do->Cancel();
			if (timer_timeout)
				timer_timeout->Cancel();
		};	
		if ( timeout > 0 )
		{
			timer_timeout = CreateIoTimer(timeout, false, [pause_id_timeout](const IoTimerPtr&){
				throw CmnPauseLogicException(pause_id_timeout, "timerdowaituntil2");
			});
			if ( !timer_timeout )
			{
				assert(false);
				return false;
			}
			timer_timeout->Continue();
		}
		auto& io_ser = GetIoService();
		while ( io_ser.RunOne() > 0 ){}
	} DO_CATCH_THE_EXCEPTION( const CmnPauseLogicException& e, {
		if (e.IsIdentityEqual(pause_id_timer_do))
			return true;
		else if (e.IsIdentityEqual(pause_id_timeout))
			return func_until();
		throw;
	});
	
	assert(false);
	return false;
}

unsigned __int64 MakeInt64( Poco::UInt32 lower, Poco::UInt32 upper )
{
	unsigned __int64 value = upper;
	value <<= sizeof(Poco::UInt32) * 8;
	value += lower;
	return value;
}

std::pair<Poco::UInt32, Poco::UInt32> SplitInt64( Poco::UInt64 big_value )
{
	std::pair<Poco::UInt32, Poco::UInt32> res;
	res.first = (Poco::UInt32)(big_value & (Poco::UInt32)-1);
	res.second = (Poco::UInt32)(big_value >> (sizeof(Poco::UInt32) * 8));
	return res;
}

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//读取硬盘的序列号要调用的子函数3
BOOL DoIdentifyEx( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				  PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
				  PDWORD lpcbBytesReturned )
{
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg     = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg       = 0;
	pSCIP->irDriveRegs.bCylHighReg      = 0;

	// calc the drive number.
	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 );

	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	BOOL bRet = DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		( LPVOID ) pSCIP,
		sizeof( SENDCMDINPARAMS ) - 1,
		( LPVOID ) pSCOP,
		sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL );

	return bRet;
}

//读取硬盘的序列号要调用的子函数1
BOOL WinNTHDSerialNumAsPhysicalReadEx( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen)
{
#define  DFP_GET_VERSION  0x00074080

	BOOL bInfoLoaded = FALSE;

	for( UINT uDrive = 0; uDrive < 4; ++ uDrive )
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		char szDriveName [256];
		sprintf_s( szDriveName, "\\\\.\\PhysicalDrive%d", uDrive );

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFileA( szDriveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);		

		if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE )
		{
			GETVERSIONOUTPARAMS VersionParams = { 0 };
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL

			if( DeviceIoControl( hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof( GETVERSIONOUTPARAMS ),
				&cbBytesReturned, NULL ) )
			{
				// If there is a IDE device at number "i" issue commands
				// to the device
				if( VersionParams.bIDEDeviceMap != 0 )
				{
					BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
					SENDCMDINPARAMS  scip = { 0 };

					// Now, get the ID sector for all IDE devices in the system.
					// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
					// otherwise use the IDE_ATA_IDENTIFY command
					bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
					BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };

					if( DoIdentifyEx( hPhysicalDriveIOCTL, 
						&scip, 
						( PSENDCMDOUTPARAMS )&IdOutCmd, 
						( BYTE )bIDCmd,
						( BYTE )uDrive,
						&cbBytesReturned ) )
					{
						if( * puSerialLen + 20U <= uMaxSerialLen )
						{
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 10, 20 );  // 序列号

							// Cut off the trailing blanks
							auto idx = 20;
							for( ; idx != 0U && ' ' == dwSerial[* puSerialLen + idx - 1]; --idx )  {}
							* puSerialLen += idx;

							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 27, 40 ); // 型号

							// Cut off the trailing blanks
							idx = 40;
							for( ; idx != 0U && ' ' == dwSerial[* puSerialLen + idx - 1]; -- idx )  {}
							* puSerialLen += idx;

							bInfoLoaded = TRUE;
						}
						else
						{
							::CloseHandle( hPhysicalDriveIOCTL );
							return bInfoLoaded;
						}
					}
				}
			}
			CloseHandle( hPhysicalDriveIOCTL );
		}
	}
	return bInfoLoaded;
}

//读取硬盘的序列号要调用的子函数2
BOOL WinNTHDSerialNumAsScsiReadEx( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen)
{
	BOOL bInfoLoaded = FALSE;
	UINT i=NULL;
	for( int iController = 0; iController < 2; ++ iController )
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   szDriveName[256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf_s( szDriveName, "\\\\.\\Scsi%d:", iController );

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFileA( szDriveName,GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,OPEN_EXISTING, 0, NULL);	

		// if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
		//            controller, GetLastError ());

		if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE )
		{
			int iDrive = 0;
			for( iDrive = 0; iDrive < 2; ++ iDrive )
			{
				char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 };

				SRB_IO_CONTROL* p = ( SRB_IO_CONTROL* )szBuffer;
				SENDCMDINPARAMS* pin = ( SENDCMDINPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
				DWORD dwResult;

				p->HeaderLength = sizeof( SRB_IO_CONTROL );
				p->Timeout = 10000;
				p->Length = SENDIDLENGTH;
				p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				const char the_str[] = "SCSIDISK";
				static_assert(_countof(the_str) == 9, "");
				static_assert(_countof(p->Signature) == 8, "");
				Cmnx_CpyMem(p->Signature, the_str, _countof(p->Signature));

				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin->bDriveNumber = iDrive;

				if( DeviceIoControl( hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					szBuffer,
					sizeof( SRB_IO_CONTROL ) + sizeof( SENDCMDINPARAMS ) - 1,
					szBuffer,
					sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH,
					&dwResult, NULL ) )					
				{
					SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
					IDSECTOR* pId = ( IDSECTOR* )( pOut->bBuffer );
					if( pId->sModelNumber[0] )
					{
						if( * puSerialLen + 20U <= uMaxSerialLen )
						{
							// 序列号
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 10, 20 );

							// Cut off the trailing blanks
							for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
							{}
							* puSerialLen += i;

							// 型号
							CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 27, 40 );
							// Cut off the trailing blanks
							for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
							{}
							* puSerialLen += i;

							bInfoLoaded = TRUE;
						}
						else
						{
							::CloseHandle( hScsiDriveIOCTL );
							return bInfoLoaded;
						}
					}
				}
			}
			::CloseHandle( hScsiDriveIOCTL );
		}
	}
	return bInfoLoaded;
}

bool GetPhyHardDiskId( std::string& str_buffer )
{
	BYTE szSystemInfo[4096]={0}; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度

	/*OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &ovi );*/

	if( !WinNTHDSerialNumAsPhysicalReadEx( szSystemInfo, &uSystemInfoLen, 1024 ) )
	{
		if (!WinNTHDSerialNumAsScsiReadEx( szSystemInfo, &uSystemInfoLen, 1024 ) )
		{
			return false;
		}
	}

	// 去掉空格
	for ( UINT i = 0; i < uSystemInfoLen; ++i )
	{
		if ( szSystemInfo[i] != ' ' )
		{
			str_buffer.push_back(szSystemInfo[i]);
		}
	}
	return true;
}

bool GetCpuId( std::string& str_buffer )
{
	__try
	{
		//获取CPU序列号
		char szBuf[100];
		unsigned long s1, s2;     
		__asm{   
			mov eax,01h   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,eax   
		}   
		sprintf_s(szBuf, "%08X%08X", s1, s2); 
		str_buffer = szBuf;

		__asm{   
			mov eax,03h   
			xor ecx,ecx   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,ecx   
		}   
		sprintf_s(szBuf, "%08X%08X", s1, s2);  
		str_buffer += szBuf;
	}
	__except(1)
	{
		return false;
	}
	return true;
}

typedef struct _tagNetAdapterInfo
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
}NETADAPTERINFO, *PNETADAPTERINFO;

bool GetMac( std::string& str_buffer )
{
	NCB ncb;
	UCHAR uRetCode;
	LANA_ENUM lana_enum;
	memset( &ncb, 0, sizeof(ncb) );

	// 向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡、每张网卡的编号等 
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *) &lana_enum;
	ncb.ncb_length = sizeof(lana_enum);
	uRetCode = Netbios( &ncb );

	// 对每一张网卡，以其网卡编号为输入编号，获取其MAC地址 
	for ( int i = 0; i < lana_enum.length; ++i )
	{
		UCHAR lana_num = lana_enum.lana[i];
		NCB ncb;
		UCHAR uRetCode;
		NETADAPTERINFO m_NetAdapterInfo;

		// 首先对选定的网卡发送一个NCBRESET命令，以便进行初始化 
		memset( &ncb, 0, sizeof(ncb) );
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_num;   // 指定网卡编号       
		uRetCode = Netbios( &ncb );

		// 接着，可以发送NCBASTAT命令以获取网卡的信息 
		memset( &ncb, 0, sizeof(ncb) );
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = lana_num;  // 指定网卡号
		StrCpyA((char *)ncb.ncb_callname, "* " );
		ncb.ncb_buffer = (unsigned char *) &m_NetAdapterInfo;
		// 指定返回的信息存放的变量 
		ncb.ncb_length = sizeof(m_NetAdapterInfo);    
		uRetCode = Netbios( &ncb );  


		if ( uRetCode == 0 )
		{
			// 把网卡MAC地址格式化成常用的16进制形式，如0010-A4E4-5802 
			CStringA str_tmp;
			str_tmp.Format("%02X-%02X-%02X-%02X-%02X-%02X",        
				m_NetAdapterInfo.adapt.adapter_address[0],
				m_NetAdapterInfo.adapt.adapter_address[1],
				m_NetAdapterInfo.adapt.adapter_address[2],
				m_NetAdapterInfo.adapt.adapter_address[3],
				m_NetAdapterInfo.adapt.adapter_address[4],
				m_NetAdapterInfo.adapt.adapter_address[5] );
			str_buffer = str_tmp;
			return true;
		}
	}
	return false;
}

int IsLocalDisk(char* szDriveName)     
{     
	////本段程序的前提是DriveName是已经过GetDriveType的判断是本地磁盘,否则报错,作用是判断是否是真正的本地磁盘////////      
	//////////////////111111111111111111111111111111111111111/////////////////////////////////////////////      
	///////////////////////获得某分区(目的地址)的信息/////////////////////////    
	HANDLE   hDeviceDest   =   NULL;     
	DWORD   nBytesRead   =   0;//预设为0，当缓冲区的长度不够时，该值为所需的缓冲区的长度      
	DWORD   nBufferSize   =   sizeof(PARTITION_INFORMATION);     
	PPARTITION_INFORMATION   lpPartInfo   =   (PPARTITION_INFORMATION)malloc(nBufferSize);     
	if(lpPartInfo   ==   NULL)     
	{     
		return   0;     
	}     
	memset(lpPartInfo,   0,   nBufferSize);//将缓冲区lpPartInfo的内容设为nDiskBufferSize个NULL      
	//CString   DriveName="J:";//为判断提供接口      
	//    DriveName=_T("\\\\.\\")+DriveName;     

	char szDiskName[250]={0};
	sprintf_s(szDiskName,"\\\\.\\%s",szDriveName);

	hDeviceDest   =   CreateFileA(szDiskName,//注意一定要是\\\\.\\的形式,CreateFile的要求   ""\\??\\Volume{e9233817-90be-11d6-88b7-00e04c3de005}      
		GENERIC_READ,     
		FILE_SHARE_READ   |   FILE_SHARE_WRITE,     
		NULL,   OPEN_EXISTING,     
		0,   NULL);   

	if(hDeviceDest   ==   NULL)     
	{     
		return   0;     
	}     
	/////////////获得该分区信息/////////////////////////    

	BOOL   ret1=DeviceIoControl(     
		hDeviceDest,     
		IOCTL_DISK_GET_PARTITION_INFO,     
		NULL,     
		0,     
		(LPVOID)   lpPartInfo,     
		(DWORD)   nBufferSize,     
		(LPDWORD)   &nBytesRead,     
		NULL//指向一个异步的结构体      
		);     
	if   (!ret1)     
	{     
		LPVOID   lpMsgBuf;     
		FormatMessage(       
			FORMAT_MESSAGE_ALLOCATE_BUFFER   |       
			FORMAT_MESSAGE_FROM_SYSTEM   |       
			FORMAT_MESSAGE_IGNORE_INSERTS,     
			NULL,     
			GetLastError(),     
			MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),   //   Default   language      
			(LPTSTR)   &lpMsgBuf,     
			0,     
			NULL       
			);     
		LocalFree(   lpMsgBuf   );     
		return     0;     
	}     
	///////////////////导出该分区信息///////////////////////////////////      
	LARGE_INTEGER   StartingOffset=lpPartInfo->StartingOffset;     
	LONGLONG   QuadPart=StartingOffset.QuadPart;//取上面的值之一情形,支持64位整型      
	LARGE_INTEGER   PartitionLength=lpPartInfo->PartitionLength;     
	LONGLONG   QuadPart1=PartitionLength.QuadPart;//取上面的值之一情形,支持64位整型      
	DWORD   HiddenSectors=lpPartInfo->HiddenSectors;     
	DWORD   PartitionNumber=lpPartInfo->PartitionNumber;     
	BYTE     PartitionType=lpPartInfo->PartitionType;     
	BOOLEAN   BootIndicator=lpPartInfo->BootIndicator;     
	BOOLEAN   RecognizedPartition=lpPartInfo->RecognizedPartition;     
	BOOLEAN   RewritePartition=lpPartInfo->RewritePartition;     

	free(lpPartInfo);     
	CloseHandle(hDeviceDest);     

	/////////////////////查询注册表中COUNT(Disk)的值//////////////////////////////////////      
	UINT   IDESeqNum;//IDE的序号      
	BOOL   FindIDE=FALSE;     

	HKEY   hKEY;     
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum"),   0,   KEY_READ,   &hKEY);       
	///////////接收DWORD型/////////////      

	DWORD   Type;//仅仅用于接收数据类型      
	DWORD   dwValue;     
	DWORD   dwBufLen   =   sizeof(DWORD);     
	long   ret2=::RegQueryValueEx(hKEY,   _T("Count"),   NULL,   &Type,   (BYTE*)&dwValue,   &dwBufLen);     
	if(ret2!=ERROR_SUCCESS)     
	{     
		return   0;//失败      
	}     
	for   (UINT   k=0;   k<dwValue;   k++)     
	{         
		///////////接收字符型/////////////      
		char   str[256];     
		DWORD     sl   =   256;         
		char    nDisk[200]={0};     
		sprintf_s(nDisk, "%d",k);     
		RegQueryValueExA(hKEY,     nDisk,     NULL,     NULL,     (LPBYTE)str,     &sl);   //注意第三项必须设为NULL,否则接收到的字符数据出错        
		//CString   temp=str;     
		//if   (temp.Left(3)== _T("IDE") )    
		if   (strstr(str,"IDE")!=NULL) 
		{     
			IDESeqNum=k;//IDE的序号      
			FindIDE=TRUE;     
		}     

	}     
	if   (!FindIDE)         
		return   0;       //     IDESeqNum=0;      
	RegCloseKey(hKEY);         

	char temp[200];
	//     CString   temp;     
	//     temp.Format(_T("%u"),IDESeqNum);     
	//     temp=_T("\\\\.\\PHYSICALDRIVE")+temp;//为下一步检测作准备    

	sprintf_s(temp,"\\\\.\\PHYSICALDRIVE%u",IDESeqNum);
	//////////////////22222222222222222222222222222222222222222   /////////////////////////////////////////      


	HANDLE   hDevice   =   NULL;     
	DWORD   nDiskBytesRead   =   0;//预设为0，当缓冲区的长度不够时，该值为所需的缓冲区的长度      
	DWORD   nDiskBufferSize   =   sizeof(DRIVE_LAYOUT_INFORMATION)   +   sizeof(PARTITION_INFORMATION)*104;//26*4      
	PDRIVE_LAYOUT_INFORMATION   lpDiskPartInfo   =   (PDRIVE_LAYOUT_INFORMATION)malloc(nDiskBufferSize);     

	if(lpDiskPartInfo   ==   NULL)     
	{     
		return   0;     
	}     
	memset(lpDiskPartInfo,   0,   nDiskBufferSize);//将缓冲区lpDiskPartInfo的内容设为nDiskBufferSize个NULL      

	//////////////////////获得所有分区的信息///////////////////////////////////////   
	hDevice   =   CreateFileA(temp,//注意一定要是\\\\.\\的形式,CreateFile的要求   ""\\??\\Volume{e9233817-90be-11d6-88b7-00e04c3de005}      
		GENERIC_READ,     
		FILE_SHARE_READ   |   FILE_SHARE_WRITE,     
		NULL,   OPEN_EXISTING,     
		0,   NULL);  	

	if(hDevice   ==   NULL)     
	{     
		return   0;     
	}     

	/////////////获得某磁盘上的所有分区信息///////////////////////// 

	BOOL   ret=DeviceIoControl(     
		hDevice,     
		IOCTL_DISK_GET_DRIVE_LAYOUT,     
		NULL,     
		0,     
		(LPVOID)   lpDiskPartInfo,     
		(DWORD)   nDiskBufferSize,     
		(LPDWORD)   &nDiskBytesRead,     
		NULL ); 

	if   (!ret)     
	{     
		LPVOID   lpMsgBuf;     
		FormatMessage(       
			FORMAT_MESSAGE_ALLOCATE_BUFFER   |       
			FORMAT_MESSAGE_FROM_SYSTEM   |       
			FORMAT_MESSAGE_IGNORE_INSERTS,     
			NULL,     
			GetLastError(),     
			MAKELANGID(LANG_NEUTRAL,   SUBLANG_DEFAULT),   //   Default   language      
			(LPTSTR)   &lpMsgBuf,     
			0,     
			NULL       
			);     
		LocalFree(   lpMsgBuf   );     
		return   0;     
	}     

	//////////////////////////////导出分区信息///////////////////////////////////////      
	DWORD   PartitionCount=lpDiskPartInfo->PartitionCount;     //永远是实际的分区数的4倍,不能用的分区将会显示类型PARTITION_ENTRY_UNUSED,即分区类型为0      
	///////////////////依次获取导出某分区信息,并与目的驱动器进行比较///////////////////////////////////      
	for   (UINT   i=0;   i<PartitionCount;   i=i+4)//+4是因为只有下标为4的整数倍的值才是正确的引用      
	{     
		PARTITION_INFORMATION   DiskPartInfo=lpDiskPartInfo->PartitionEntry[i];//0为C:,4为D:,8为e:,12为F      
		LARGE_INTEGER   DiskStartingOffset   =   DiskPartInfo.StartingOffset;     
		LONGLONG   DiskQuadPart   =   DiskStartingOffset.QuadPart; //取上面的值之一情形,支持64位整型      
		LARGE_INTEGER   DiskPartitionLength   =   DiskPartInfo.PartitionLength;     
		LONGLONG   DiskQuadPart1   =   DiskPartitionLength.QuadPart; //取上面的值之一情形,支持64位整型      
		DWORD   DiskHiddenSectors   =   DiskPartInfo.HiddenSectors;     
		DWORD   DiskPartitionNumber   =   DiskPartInfo.PartitionNumber;     
		BYTE     DiskPartitionType   =   DiskPartInfo.PartitionType;     
		BOOLEAN   DiskBootIndicator   =   DiskPartInfo.BootIndicator;     
		BOOLEAN   DiskRecognizedPartition   =   DiskPartInfo.RecognizedPartition;     
		BOOLEAN   DiskRewritePartition   =   DiskPartInfo.RewritePartition;     
		if     ((DiskQuadPart==QuadPart)   &&   (DiskQuadPart1==QuadPart1)     
			&&   (DiskHiddenSectors==HiddenSectors)   &&   (DiskPartitionNumber==PartitionNumber)     
			&&   (DiskPartitionType==PartitionType   )   &&   (DiskBootIndicator==BootIndicator)     
			&&   (DiskRecognizedPartition==RecognizedPartition)   &&   (DiskRewritePartition==RewritePartition))     
		{     
			free(lpDiskPartInfo);     
			CloseHandle(hDevice);           
			return   1;     
		}     
	}     
	free(lpDiskPartInfo);     
	CloseHandle(hDevice);     
	return   2;  
}  

DWORD GetDiskSubareaLogicSn(char *_szSubareaName)
{
	LPCSTR lpRootPathName=_szSubareaName; //取C盘
	char lpVolumeNameBuffer[13] = {0};
	DWORD nVolumeNameSize=12;// 卷标的字符串长度
	DWORD VolumeSerialNumber;//硬盘序列号
	DWORD MaximumComponentLength;// 最大的文件长度
	char lpFileSystemNameBuffer[11]={0};// 存储所在盘符的分区类型的长指针变量
	DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
	DWORD FileSystemFlags;// 文件系统的一此标志

	::GetVolumeInformationA(lpRootPathName,lpVolumeNameBuffer, nVolumeNameSize,&VolumeSerialNumber, 
		&MaximumComponentLength,&FileSystemFlags,lpFileSystemNameBuffer, nFileSystemNameSize);
	return VolumeSerialNumber;
}

bool GetAllDiskSubareaSN( std::string& str_buffer )
{
	int nValue = 0;
	DWORD dwDiskCount= 0;
	char szBuf[100]={0};
	DWORD dwTm;
	DWORD dwBitMask = GetLogicalDrives();
	for (DWORD dw = 0 ;dw<26;dw++)
	{
		if (dwBitMask&1)
		{
			dwDiskCount++;
		}
		dwBitMask=dwBitMask>>1;
	}

	DWORD dwDriveStrLen = 0;

	dwDriveStrLen= GetLogicalDriveStringsA(0,NULL);	

	char *szDriveName = new char[dwDriveStrLen];
	char *pDriveName=NULL;
	if (GetLogicalDriveStringsA(dwDriveStrLen,szDriveName))
	{
		pDriveName = szDriveName;
		while (*pDriveName!=NULL)
		{
			switch (GetDriveTypeA(pDriveName))		
			{
			case DRIVE_UNKNOWN :
				break;
			case DRIVE_NO_ROOT_DIR: 
				break;
			case DRIVE_REMOVABLE :
				break;
			case DRIVE_REMOTE: 
				break;
			case DRIVE_CDROM: 
				break;
			case DRIVE_RAMDISK: 
				break;
			case DRIVE_FIXED :
				strcpy_s(szBuf,pDriveName);
				szBuf[strlen(szBuf)-1] = 0;
				nValue= IsLocalDisk(szBuf);
				if (nValue!=2)
				{
					//主机上的真实硬盘
					memset(szBuf,0,sizeof(szBuf));
					dwTm = GetDiskSubareaLogicSn(pDriveName);
					sprintf_s(szBuf,"%X-",dwTm);
					str_buffer += szBuf;
				}
				break;
			default:
				break;
			}
			pDriveName+=lstrlenA(pDriveName)+1;
		}
	} 	
	delete []szDriveName;
	
	return !str_buffer.empty();
}

bool GetAllDiskSubareaSize( std::string& str_buffer )
{
	DWORD dwDiskCount= 0;
	char szBuf[100]={0};
	int nValue = 0;

	DWORD dwBitMask = GetLogicalDrives();
	for (DWORD dw = 0 ;dw<26;dw++)
	{
		if (dwBitMask&1)
		{
			dwDiskCount++;
		}
		dwBitMask=dwBitMask>>1;
	}


	DWORD dwDriveStrLen = 0;

	dwDriveStrLen= GetLogicalDriveStrings(0,NULL);

	char *szDriveName = new char[dwDriveStrLen];
	char *pDriveName=NULL;

	if (GetLogicalDriveStringsA(dwDriveStrLen,szDriveName))	
	{
		pDriveName = szDriveName;
		while (*pDriveName!=NULL)
		{
#ifdef _ENABLE_MY_WIN32_API_
			switch (MyApi.GetDriveType(pDriveName))		
#else
			switch (GetDriveTypeA(pDriveName))		
#endif
			{
			case DRIVE_UNKNOWN :
				break;
			case DRIVE_NO_ROOT_DIR: 
				break;
			case DRIVE_REMOVABLE :
				break;
			case DRIVE_REMOTE: 
				break;
			case DRIVE_CDROM: 
				break;
			case DRIVE_RAMDISK: 
				break;
			case DRIVE_FIXED :

				strcpy_s(szBuf,pDriveName);
				szBuf[strlen(szBuf)-1] = 0;
				nValue= IsLocalDisk(szBuf);
				if (nValue!=2)
				{
					//主机上的真实硬盘
					memset(szBuf,0,sizeof(szBuf));
					ULARGE_INTEGER nFreeBytesAvailable;
					ULARGE_INTEGER nTotalNumberOfBytes;
					ULARGE_INTEGER nTotalNumberOfFreeBytes;
					if (GetDiskFreeSpaceExA(pDriveName, &nFreeBytesAvailable, &nTotalNumberOfBytes, &nTotalNumberOfFreeBytes))
					{
						//wsprintf(szBuf,_T("%I64d"),nTotalNumberOfBytes);//I64d 表示输出64位的大数 10进制
						sprintf_s(szBuf, "%I64X",nTotalNumberOfBytes);//I64X 表示输出64位的大数 16进制
						str_buffer += szBuf;
					}
				}



				break;
			default:
				break;
			}
			pDriveName+=lstrlenA(pDriveName)+1;
		}
	} 	
	delete []szDriveName;
	
	return !str_buffer.empty();
}

bool GetOsVersionStr( std::string& str_buffer )
{
	char szOs[255]={0};
	OSVERSIONINFOEXW os; 
	os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEXW);   
	if (!Get_GetVersionExW((OSVERSIONINFO*)&os))
		return false;
	CStringA str_tmp;
	str_tmp.Format("%d.%d.%d %s", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber, os.szCSDVersion);
	str_buffer = str_tmp;
	return true;
}

bool GetHWID( char* out_buffer, pt_dword buffer_size )
{
	if ( !out_buffer || buffer_size < 1 )
		return false;

	std::string str_tmp;
	auto buffer_old = out_buffer;

	if ( !GetPhyHardDiskId(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	pt_dword cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	if ( !GetCpuId(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	if ( !GetMac(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	if ( !GetAllDiskSubareaSN(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	if ( !GetAllDiskSubareaSize(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	if ( !GetOsVersionStr(str_tmp) )
		return false;
	if (str_tmp.empty())
		return false;
	cur_size = std::min(str_tmp.size(), buffer_size);
	CopyMemory(out_buffer, str_tmp.c_str(), cur_size);
	buffer_size -= cur_size;
	out_buffer += cur_size;
	if ( buffer_size < 1 )
		return true;

	for ( pt_dword i = 0; i < buffer_size; ++i )
	{
		*(out_buffer + i) = *(buffer_old + i);
	}
	return true;
}

bool GetTheSerialNumber( const char* const class_name, std::string& out_str )
{
	assert(class_name);
	out_str.clear();

	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
	BOOST_SCOPE_EXIT_ALL(&){
		CoUninitialize();
	};
	if (FAILED(hres))
	{
		assert(false);
		return false;                  // Program has failed.
	}	

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------

	hres =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);

	if (FAILED(hres))
	{
		if ( RPC_E_TOO_LATE != hres )
		{
			assert(false);
			return false;  
		}
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);
	BOOST_SCOPE_EXIT_ALL(pLoc){
		if (pLoc)
			pLoc->Release();
	};
	if (FAILED(hres))
	{
		assert(false);
		return false;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);
	BOOST_SCOPE_EXIT_ALL(pSvc){
		if (pSvc)
			pSvc->Release();
	};
	if (FAILED(hres))
	{
		assert(false);
		return false;                // Program has failed.
	}

	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		assert(false);
		return false;               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	CStringA str_tmp;
	str_tmp.Format("SELECT * FROM %s", class_name);
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t(str_tmp),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);
	BOOST_SCOPE_EXIT_ALL(pEnumerator){
		if (pEnumerator)
			pEnumerator->Release();
	};
	if (FAILED(hres))
	{
		assert(false);
		return false;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = nullptr;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);
		BOOST_SCOPE_EXIT_ALL(&pclsObj){
			if (pclsObj)
				pclsObj->Release();
			pclsObj = nullptr;
		};
		if (FAILED(hr))
		{
			break;
		}

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp = { 0 };

		// Get the value of the Name property
		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		if ( !FAILED(hres) && vtProp.bstrVal )
		{
			std::wstring c_tmp(vtProp.bstrVal);
			out_str.append(WideCharToGbk(c_tmp));
		}		
		VariantClear(&vtProp);
		break;
	}

	return !out_str.empty();
}

bool GetMixHWID( char* out_buffer, pt_dword buffer_size )
{
	VMP_BEGIN_EX;
	if ( !out_buffer || buffer_size < 1 )
	{
		assert(false);
		return false;
	}

	typedef bool (*FnT)( std::string& );
	FnT funcs[] = {
		&GetPhyHardDiskId, &GetCpuId,
		/*&GetMac,*/ &GetAllDiskSubareaSN,
		&GetAllDiskSubareaSize, &GetOsVersionStr,
		&GetBiosNumber, GetBaseBoardNumber, GetPhysicalMediaNumber
	};
	/*auto buffer_average = buffer_size / _countof(funcs);
	buffer_average = std::max(buffer_average, (decltype(buffer_average))1);*/

	std::string str_total, str_tmp;

	//////////////////////////////////////////////////////////////////////////
	/*std::string kMatchInfoPath;
	TCHAR cfg_file_name[MAX_PATH];
	if (GetModulePath(nullptr, cfg_file_name, sizeof(cfg_file_name)))
	{
		_tcscat_s(cfg_file_name, _T("Cfg\\"));
		kMatchInfoPath = CT2CA(cfg_file_name);
		CreateDirectory(cfg_file_name, nullptr);
	}
	std::ofstream match_info_file;
	match_info_file.open((kMatchInfoPath + "mcodxx.txt").c_str(), std::ios::app | std::ios::out);
	DateTimeLocal time_now;
	match_info_file << Poco::DateTimeFormatter::format(time_now, Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;*/
	//////////////////////////////////////////////////////////////////////////

	for ( auto i = 0; i != _countof(funcs); ++i )
	{
		if ( !(*funcs[i])(str_tmp) )
		{
			str_tmp.append(10 + i, 'A' + i);
		}
		//LOG_O(Log_trace) << "GetMixHWID, idx:" << i << "  before:" << str_tmp;
		str_tmp.append(2 + i, 'A' + i);
		str_tmp = GenUuidStr(str_tmp);
		//match_info_file << "idx:" << i << " " << str_tmp << std::endl;
		//LOG_O(Log_trace) << "GetMixHWID, idx:" << i << "  after:" << str_tmp;
		assert(!str_tmp.empty());
		/*if ( !str_tmp.empty() )
		{
			auto valid_size = std::min(str_tmp.size(), buffer_average);
			CopyMemory(out_buffer, str_tmp.c_str(), valid_size);
			buffer_size -= valid_size;
			assert(buffer_size >= 0);
			out_buffer += valid_size;
			if ( buffer_size < 1 )
				return true;
		}*/
		str_total += str_tmp;
		str_tmp.clear();
	}
	if (str_total.empty())
	{
		assert(false);
		return false;
	}
	str_total = GenUuidStr(str_total);

	auto idx = 0;
	const auto kTotalSize = str_total.size();
	while ( buffer_size > 0 )
	{
		*(out_buffer + idx) = str_total[idx % kTotalSize];
		++idx;
		--buffer_size;
	}
	VMP_END;
	return true;
}

std::string GenUuidStr( const std::string& str_src )
{
	return GenUuidStr(str_src.c_str(), str_src.size());
}

std::string GenUuidStr( const void* buffer, pt_dword buffer_size )
{
	const auto& uuid = GenUuidFromBuff(buffer, buffer_size);
	if (uuid.is_nil())
	{
		assert(false);
		return "error";
	}
	auto res = boost::uuids::to_string(uuid);
	assert(res.size() == kUuidStrLen);
	return res;
}

std::string GenUuidStr( std::istream& istm )
{
	std::string str_tmp;
	Poco::StreamCopier::copyToString(istm, str_tmp);
	if (str_tmp.empty())
		return str_tmp;
	return GenUuidStr(str_tmp);
}

std::string GenUuidStrOfFile( const std::string& file_name ){
	std::ifstream stm_file(file_name, std::ios_base::in | std::ios_base::binary);
	if ( !stm_file )
	{
		assert(false);
		return kEmptyStr;
	}
	return GenUuidStr(stm_file);
}

void ShowPopupMenu( UINT resource_id, CWnd* parent )
{
	CMenu menu;
	VERIFY(menu.LoadMenuW(resource_id));
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);

	DWORD dwPos = GetMessagePos();
	CPoint mouse_pos(LOWORD(dwPos), HIWORD(dwPos));
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, mouse_pos.x, mouse_pos.y, parent);
}

bool CheckSingletonHandle( const std::string& str_handle_name )
{
	std::string event_name = "Global\\";
	event_name += GenUuidStr(str_handle_name);
	if ( !::CreateEvent(GetNullDACL(), FALSE, TRUE, CA2CT(event_name.c_str())) )
	{
		ASSERT(FALSE);
		return false;
	}

	if ( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "控制台实例已存在";
		return false;
	}
	return true;
}

std::string GenUuidStrFromGenerator( const std::string& str_src, const std::string& str_generator )
{
	static boost::uuids::string_generator gen;
	boost::uuids::uuid obj_uuid = gen(GenUuidStr(str_generator));
	boost::uuids::name_generator name_gen(obj_uuid);
	boost::uuids::uuid u = name_gen(str_src);
	return boost::uuids::to_string(u);
}

bool GenNewRsaKeyFile( const std::string& str_out_to_pub_file, const std::string& str_out_to_pri_file, const std::string& str_private_key )
{
	using namespace std;
	using namespace Poco::Crypto;
	RSAKey gen_rsa(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	std::fstream f_pub(str_out_to_pub_file, ios_base::in | ios_base::out | ios_base::trunc /*| ios_base::_Nocreate*/), 
		f_pri(str_out_to_pri_file, ios_base::in | ios_base::out | ios_base::trunc);
	if ( !f_pub )
	{
		assert(false);
		return false;
	}
	if ( !f_pri )
	{
		assert(false);
		return false;
	}
	gen_rsa.save(&f_pub, &f_pri, str_private_key);
	f_pub.close();
	f_pri.close();
	return true;
}

bool SaveRcdataAsFile(LPCTSTR module_name, LPCTSTR res_name, const CPathW& full_file_name)
{
	return SaveRcdataAsFile(GetModuleHandle(module_name), res_name, full_file_name);
}

bool SaveRcdataAsFile(HMODULE module, LPCTSTR res_name, const CPathW& full_file_name)
{
	auto res_data = LoadRcdataRes(module, res_name);
	if ( !res_data.first || res_data.second <= 0 )
	{
		assert(false);
		return false;
	}
	using namespace std;
	ofstream fstm((const char*)CT2CA(full_file_name.m_strPath), ios_base::out | ios_base::trunc | ios_base::binary);
	fstm.write((const char*)res_data.first, res_data.second);
	fstm.close();
	return true;
}

bool SaveRcdataAsFile(LPCTSTR module_name, LPCTSTR res_name, const CPathA& full_file_name)
{
	return SaveRcdataAsFile(GetModuleHandle(module_name), res_name, full_file_name);
}

bool SaveRcdataAsFile(HMODULE module, LPCTSTR res_name, const CPathA& full_file_name)
{
	auto res_data = LoadRcdataRes(module, res_name);
	if (!res_data.first || res_data.second <= 0)
	{
		assert(false);
		return false;
	}
	using namespace std;
	ofstream fstm((const char*)full_file_name.m_strPath, ios_base::out | ios_base::trunc | ios_base::binary);
	fstm.write((const char*)res_data.first, res_data.second);
	fstm.close();
	return true;
}

std::wstring Random_StrW( size_t cnt_min, size_t cnt_max )
{
	static const wchar_t kChars[] =
		L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	using namespace std;
	uniform_int_distribution<> index_dist(0, _countof(kChars) - sizeof(wchar_t));
	std::wstring res;
	auto& rng = GetDltRandomDevice();
	uniform_int_distribution<> char_cnt(cnt_min, cnt_max);
	cnt_max = char_cnt(rng);
	for( size_t i = 0; i < cnt_max; ++i )
	{
		res.push_back(kChars[index_dist(rng)]);
	}
	return std::move(res);
}

bool DoFileItem( const std::string& path_file, const std::function<void( const boost::filesystem::directory_iterator& )>& func )
{
	using namespace boost::filesystem;
	boost::filesystem::path file_path(path_file);

	try
	{
		if (exists(file_path))    // does p actually exist?
		{
			if (is_directory(file_path))      // is p a directory?
			{
				directory_iterator it(file_path), it_end;

				while ( it != it_end )
				{
					func(it);
					++it;
				}
			}
			else
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << file_path << " exists, but is not a directory";
				return false;
			}
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << file_path << " does not exist";
			return false;
		}
	}
	DO_CATCH_THE_EXCEPTION( const filesystem_error& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.what();
		return false;
	})
	return true;
}

bool DoExtensionFileItem( const std::string& path_file, const std::string& file_extension, const std::function<void( const boost::filesystem::directory_iterator& )>& func )
{
	using namespace boost::filesystem;
	boost::filesystem::path file_path(path_file);

	try
	{
		if (exists(file_path))    // does p actually exist?
		{
			if (is_directory(file_path))      // is p a directory?
			{
				directory_iterator it(file_path), it_end;

				for ( ; it != it_end; ++it )
				{
					const auto& file_path = it->path();
					if ( 0 != _stricmp(file_path.extension().string().c_str(), file_extension.c_str()) )
						continue;
					func(it);
				}
			}
			else
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << file_path << " exists, but is not a directory";
				return false;
			}
		}
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << file_path << " does not exist";
			return false;
		}
	}
	DO_CATCH_THE_EXCEPTION( const filesystem_error& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.what();
		return false;
	})
	return true;
}

void CopyAllFilesOfDirectory( const boost::filesystem::path &src, const boost::filesystem::path &dst, const std::function<bool( const boost::filesystem::directory_iterator& )>* but_func )
{
	if (!boost::filesystem::exists(dst))
	{
		boost::filesystem::create_directories(dst);
	}

	for ( boost::filesystem::directory_iterator it(src), it_end; it != it_end; ++it )
	{
		if ( but_func && (*but_func)(it) )
			continue;
		const boost::filesystem::path newSrc = src / it->path().filename();
		const boost::filesystem::path newDst = dst / it->path().filename();
		if (boost::filesystem::is_directory(newSrc))
		{
			CopyAllFilesOfDirectory(newSrc, newDst, but_func);
		}
		else if (boost::filesystem::is_regular_file(newSrc))
		{
			boost::filesystem::copy_file(newSrc, newDst, boost::filesystem::copy_option::overwrite_if_exists);
		}
	}
}

std::string Domain2Ip( const std::string& domain, unsigned short int port /*= 0 */ )
{ 
	using namespace std;  
	using namespace boost::asio;

	std::string str_ip;
	try{
		ip::tcp::resolver slv(GetIoService().GetImpl());  
		ip::tcp::resolver::query qry(domain, boost::lexical_cast<string>(port));
		ip::tcp::resolver::iterator it = slv.resolve(qry);  
		ip::tcp::resolver::iterator end;  
		for( ; it !=end; ++it )  
		{  
			str_ip += it->endpoint().address().to_string();  
		}
	}
	catch ( ... )
	{
		try{
			int i = 0;
			i *= 3;
		}
		catch ( ... )
		{
			throw;
		}
	}
	return str_ip;
}  

int RandomNum( int min_num, int max_num )
{
	if ( min_num > max_num )
	{
		assert(false);
		return min_num;
	}
	else if ( min_num == max_num )
		return min_num;

	using namespace std;
	uniform_int_distribution<> index_dist(min_num, max_num);
	auto res = index_dist(GetDltRandomDevice());
	assert(res >= min_num && res <= max_num);
	return res;
}

void AutoRegLpkImpl( CmnPauseLogicException::PauseIdentity pause_id )
{
	HKEY the_key = nullptr;
	TCHAR szlpk[] = _T("lpk.dll\0");
	TCHAR sztmp[] = _T("ExcludeFromKnownDlls\0");
	TCHAR szbuffer[MAX_PATH] = { 0 };

	int reg_lpk_cnt = 0;
	if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\ControlSet001\\Control\\Session Manager"), 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &the_key) )
	{
		if (the_key)
		{
			DWORD value_type = REG_MULTI_SZ;
			//这里必须得设置长度，否则RegQueryValueEx会返回失败
			DWORD the_size = _countof(szbuffer);
			if ( ERROR_SUCCESS == RegQueryValueEx(the_key, sztmp, nullptr, &value_type, (BYTE*)szbuffer, &the_size) )
			{
				if ( the_size > 0 && 0 == _tcsicmp(szlpk, szbuffer) )
					++reg_lpk_cnt;
			}
			VERIFY(ERROR_SUCCESS == RegSetValueEx(the_key, sztmp, 0, REG_MULTI_SZ, (const BYTE*)szlpk, sizeof(szlpk)));
		}
		RegCloseKey(the_key);
		the_key = nullptr;
	}
	if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\ControlSet002\\Control\\Session Manager"), 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &the_key) )
	{
		if (the_key)
		{
			DWORD value_type = REG_MULTI_SZ;
			DWORD the_size = _countof(szbuffer);
			if ( ERROR_SUCCESS == RegQueryValueEx(the_key, sztmp, nullptr, &value_type, (BYTE*)szbuffer, &the_size) )
			{
				if ( the_size > 0 && 0 == _tcsicmp(szlpk, szbuffer) )
					++reg_lpk_cnt;
			}
			VERIFY(ERROR_SUCCESS == RegSetValueEx(the_key, sztmp, 0, REG_MULTI_SZ, (const BYTE*)szlpk, sizeof(szlpk)));
		}
		RegCloseKey(the_key);
		the_key = nullptr;
	}
	if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\ControlSet003\\Control\\Session Manager"), 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &the_key) )
	{
		if (the_key)
		{
			DWORD value_type = REG_MULTI_SZ;
			DWORD the_size = _countof(szbuffer);
			if ( ERROR_SUCCESS == RegQueryValueEx(the_key, sztmp, nullptr, &value_type, (BYTE*)szbuffer, &the_size) )
			{
				if ( the_size > 0 && 0 == _tcsicmp(szlpk, szbuffer) )
					++reg_lpk_cnt;
			}
			VERIFY(ERROR_SUCCESS == RegSetValueEx(the_key, sztmp, 0, REG_MULTI_SZ, (const BYTE*)szlpk, sizeof(szlpk)));
		}
		RegCloseKey(the_key);
		the_key = nullptr;
	}
	if ( reg_lpk_cnt <= 0 )
		throw CmnPauseLogicException(pause_id);
}

void AutoRegLpk()
{
	volatile int value_addr = 0;
	DEFINE_DEFAULT_PAUSE_ID(value_addr);
	try{
		AutoRegLpkImpl(pause_id);
		value_addr = 3;
	} DO_CATCH_DEFAULT_PAUSE_ID( {
		if ( IDCANCEL != MessageBoxTimeout(nullptr, "确定重启？", "您的机器需要重启", MB_OKCANCEL, 0, 1000 * 30) )
		{
			//重启系统
			RestartSystem();
		}
	});
}

bool GetBiosNumber( std::string& str_buffer )
{
	return GetTheSerialNumber("Win32_BIOS", str_buffer);
}

bool GetBaseBoardNumber( std::string& str_buffer )
{
	return GetTheSerialNumber("Win32_BaseBoard", str_buffer);
}

bool GetPhysicalMediaNumber( std::string& str_buffer )
{
	return GetTheSerialNumber("Win32_PhysicalMedia", str_buffer);
}

void AutoRunAtSystemBegin( const std::string& reg_value, const std::string& run_param  )
{
	HKEY the_key = nullptr;
	if ( ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &the_key) )
	{
		assert(false);
		return;
	}
	if ( !the_key )
	{
		assert(false);
		return;
	}
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, sizeof(buffer));
	CStringA str_buffer = buffer;
	str_buffer.AppendChar(' ');
	str_buffer.Append(run_param.c_str());

	VERIFY(ERROR_SUCCESS == RegSetValueExA(the_key, reg_value.c_str(), 0, REG_SZ, (const BYTE*)(const char*)str_buffer, (DWORD)str_buffer.GetLength() + 1));

	RegCloseKey(the_key);
}

void DeleteAutoRunAtSystemBegin( const std::string& reg_value )
{
	HKEY the_key = nullptr;
	if ( ERROR_SUCCESS != RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &the_key) )
	{
		assert(false);
		return;
	}
	if ( !the_key )
	{
		assert(false);
		return;
	}
	RegDeleteValueA(the_key, reg_value.c_str());

	RegCloseKey(the_key);
}

///////////////////////////////////////////////////////////////////////////////
int CreateDepthDirectory( const CPath& dir_path ){
	auto dir_tmp = dir_path;
	std::stack<CString> stack_paths;
	while ( !dir_tmp.IsRoot() )
	{
		stack_paths.push(dir_tmp.m_strPath);
		dir_tmp.RemoveFileSpec();
		if ( stack_paths.size() >= 50 )
		{
			assert(false);
			return -1;
		}
	}
	if ( !stack_paths.empty() )
	{
		dir_tmp.Append(stack_paths.top());
		stack_paths.pop();
		stack_paths.push(dir_tmp);
	}
	int res = 0;
	while ( !stack_paths.empty() )
	{
		if (CreateDirectory(stack_paths.top(), nullptr))
		{
			++res;
		}
		else
		{
			if ( ERROR_ALREADY_EXISTS != GetLastError() )
				break;
		}
		stack_paths.pop();
	}
	return res;
}

void RestartSystem()
{
	//重启系统
	std::system("shutdown -r -t 0");
	::Sleep(1000 * 30);
}

OptionalBool Regex_MatchAllSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& matched_str )
{
	using namespace boost;
	OptionalBool res;
	wregex reg(str_pattern, wregex::perl);
	for ( boost::wsregex_iterator it(full_str.begin(), full_str.end(), reg, match_any), it_end; it != it_end; ++it )
	{
		for ( auto it_p = it->begin(); it_p != it->end(); ++it_p )
		{
			std::wstring the_sub_str(it_p->first, it_p->second);
			if ( the_sub_str != matched_str )
				return false;
			res = true;
		}
	}
	return res;
}

std::wstring Regex_ReplaceSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& str_fmt )
{
	using namespace boost;
	wregex reg(str_pattern, wregex::perl);
	return boost::regex_replace(full_str, reg, str_fmt, match_any);
}

boost::optional<std::wstring> Regex_ReplaceUnsameSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& str_fmt )
{
	boost::optional<std::wstring> new_res_str;
	auto match_res = Regex_MatchAllSubStr(full_str, str_pattern, str_fmt);
	if ( !match_res || *match_res )
		return new_res_str;
	new_res_str = Regex_ReplaceSubStr(full_str, str_pattern, str_fmt);
	return new_res_str;
}

bool Regex_ModifySubStrOfFile( const std::string& full_file_name, const std::string& str_pattern, const std::string& str_fmt )
{
	std::fstream fstr(full_file_name, std::ios_base::in);
	if ( !fstr )
		return false;
	std::string full_str;
	Poco::StreamCopier::copyToString(fstr, full_str);
	const auto& new_str = Regex_ReplaceUnsameSubStr(GbkToWideChar((const char*)full_str.c_str()), GbkToWideChar((const char*)str_pattern.c_str()), GbkToWideChar((const char*)str_fmt.c_str()));
	if ( !new_str )
		return false;
	fstr.close();
	fstr.open(full_file_name, std::ios_base::out | std::ios_base::trunc);
	const auto& new_str_a = WideCharToGbk(*new_str);
	fstr.write(new_str_a.c_str(), new_str_a.size());

	return true;
}

bool CopyUnsameFile( const std::string& file_src, const std::string& file_dst )
{
	using namespace boost::filesystem;
	if ( exists(file_src) && exists(file_dst) )
	{
		if ( GenUuidStrOfFile(file_src) == GenUuidStrOfFile(file_dst) )
			return false;
	}
	
	boost::system::error_code err;
	boost::filesystem::remove_all(file_dst, err);
	if (err)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "删除文件出错,目标文件：" << file_dst << " 错误信息：" << err.message();
		return false;
	}
	err.clear();
	boost::filesystem::copy_file(file_src, file_dst, err);
	if (err)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "复制文件出错,源文件：" << file_src << " 目标文件：" << file_dst << " 错误信息：" << err.message();
		return false;
	}
	return true;
}

boost::optional<std::wstring> Regex_SearchFirstSubStr( const std::wstring& full_str, const std::wstring& str_pattern )
{
	using namespace boost;
	wregex reg(str_pattern, wregex::perl);
	boost::optional<std::wstring> res_str;
	for ( boost::wsregex_iterator it(full_str.begin(), full_str.end(), reg, match_any), it_end; it != it_end; ++it )
	{
		for ( auto it_p = it->begin(); it_p != it->end(); ++it_p )
		{
			std::wstring the_sub_str(it_p->first, it_p->second);
			res_str = the_sub_str;
			return res_str;
		}
	}

	return res_str;
}

bool Regex_ModifySubStrOfFileByStrTag( const std::string& full_file_name, const std::string& str_tag, const std::string& new_str )
{
	std::string str_pre = "/\\*";
	str_pre += str_tag;
	str_pre += "\\*/\"";
	std::string str_pattern = str_pre + ".+\"";
	return Regex_ModifySubStrOfFile(full_file_name, str_pattern, "/*" + str_tag + "*/\"" + new_str + "\"");
}
bool Regex_ModifySubStrOfFileByStrTagIncInt( const std::string& full_file_name, const std::string& str_tag )
{
	std::string str_pre = "/\\*";
	str_pre += str_tag;
	str_pre += "\\*/";
	std::string str_pattern = str_pre + "\\d+;";

	std::fstream fstr(full_file_name, std::ios_base::in);
	if ( !fstr )
		return false;
	std::string full_str;
	Poco::StreamCopier::copyToString(fstr, full_str);
	auto w_full_str = GbkToWideChar(full_str);
	auto w_str_pattern = GbkToWideChar(str_pattern);
	const auto& the_sub_str = Regex_SearchFirstSubStr(w_full_str, w_str_pattern);
	if ( !the_sub_str )
	{
		assert(false);
		return false;
	}
	std::wstring the_sub_std_str = *the_sub_str;
	const auto& the_num_str = Regex_SearchFirstSubStr(*the_sub_str, L"\\d+\\D");
	if ( !the_num_str )
	{
		assert(false);
		return false;
	}
	std::wstring the_num_std_str = *the_num_str;
	auto the_value = _wtoi(the_num_std_str.c_str());
	++the_value;
	const auto& the_new_full_str = Regex_ReplaceSubStr(w_full_str, w_str_pattern, GbkToWideChar("/*" + str_tag + "*/" + std::to_string(the_value) + ";"));

	fstr.close();
	fstr.open(full_file_name, std::ios_base::out | std::ios_base::trunc);
	const auto& new_str_a = WideCharToGbk(the_new_full_str);
	fstr.write(new_str_a.c_str(), new_str_a.size());
	return true;
}

std::vector<std::wstring> Regex_FindAllTheStr( const std::wstring& full_str, const std::wstring& str_pattern )
{
	using namespace boost;
	wregex reg(str_pattern, wregex::perl);
	std::vector<std::wstring> res_str;
	for ( boost::wsregex_iterator it(full_str.begin(), full_str.end(), reg, match_any), it_end; it != it_end; ++it )
	{
		for ( auto it_p = it->begin(); it_p != it->end(); ++it_p )
		{
			res_str.push_back(kEmptyWStr);
			res_str.back().assign(it_p->first, it_p->second);
		}
	}
	return res_str;
}

bool IsOccuredByOccurRate( double occur_rate )
{
	if ( occur_rate <= 0 )
		return false;
	//放大比例，数值越大越精确
	const auto kZoomInRate = 10000000;
	auto the_real_rate = occur_rate * kZoomInRate;
	if ( the_real_rate >= kZoomInRate )
		return true;
	auto the_num = RandomNum(1, kZoomInRate);
	return the_num <= the_real_rate;
}

HRASCONN RasConnecting()
{
	RASCONNA rc[100] = { 0 };
	DWORD dwSize = 0;
	DWORD dwNumber = 0;
	rc[0].dwSize = sizeof(RASCONNA);
	dwSize = sizeof(rc);

	DWORD dwRet = RasEnumConnectionsA(rc, &dwSize, &dwNumber);		//枚举已连接的网络
	if( dwRet == 0 )
	{
		for(UINT i = 0; i < dwNumber; i++ )
		{
			if( strcmp(rc[i].szEntryName, "宽带连接") == 0 )
			{
				return rc[i].hrasconn;		//返回"我的连接"对应的句柄
			}
		}
	}

	return nullptr;
}
int CloseDialBroadband()
{
	int cnt = 0;
	while (true)
	{
		auto h = RasConnecting();
		if ( !h )
			break;
		if ( RasHangUp(h) == 0 )
			++cnt;
		else
			assert(false);
	}
	return cnt;
}

bool ReDialBroadband()
{
	if (IsDialBroadbandConnecting())
	{
		if ( CloseDialBroadband() <= 0 )
		{
			MessageBoxTimeout(NULL, "无法断开宽带连接", NULL, MB_OK, 0, 1000 * 30);
			return false;
		}
	}

	RASDIALPARAMSA rdParams;
	BOOL f_get_password;

	memset(&rdParams, 0, sizeof(rdParams));
	rdParams.dwSize = sizeof(RASDIALPARAMSA);
	strcpy_s(rdParams.szEntryName, "宽带连接");
	if( RasGetEntryDialParamsA(NULL, &rdParams, &f_get_password) != 0 )
	{
		assert(false);
		return false;
	}
	if( f_get_password == false )
	{
		MessageBoxTimeout(NULL, "宽带未设置密码", NULL, MB_OK, 0, 1000 * 30);
		return false;
	}

	HRASCONN hRasConn2 = nullptr;
	auto dwRet = RasDialA(NULL,NULL,&rdParams,0L, NULL, &hRasConn2);
	if( dwRet != 0 )
	{
		MessageBoxTimeout(NULL, "拨号失败", NULL, MB_OK, 0, 1000 * 30);
		return false;
	}
	return true;
}

bool IsDialBroadbandConnecting()
{
	return nullptr != RasConnecting();
}

bool LoadLib( const char* lib_name )
{
	if ( !lib_name )
	{
		assert(false);
		return false;
	}
	return !!LoadLibraryA(lib_name);
}

bool FreeLib( const char* lib_name )
{
	if ( !lib_name )
	{
		assert(false);
		return false;
	}
	CToolhelp th(TH32CS_SNAPMODULE, GetCurrentProcessId());

	MODULEENTRY32 me = { sizeof(me) };
	if ( !th.ModuleFind(CA2CT(lib_name), &me) )
		return false;
	if ( !FreeLibrary(me.hModule) )
	{
		assert(false);
		return false;
	}
	return true;
}

boost::optional<DateTimeLocal>		g_ser_cur_time_;
std::clock_t						g_ser_cur_clock_ = 0;
void G_SetCurTime(const std::time_t& time)
{
	if (g_ser_cur_time_)
		return;
	g_ser_cur_time_ = DateTimeLocal(Poco::Timestamp::fromEpochTime(time));
	g_ser_cur_clock_ = std::clock();
	//LOG_ENTER_FUNC;
	//LOG_O(Log_trace) << "G_SetCurTime:" << Poco::DateTimeFormatter::format(*g_ser_cur_time_, Poco::DateTimeFormat::SORTABLE_FORMAT);
}

DateTimeLocal G_GetSerCurTime()
{
	if (!g_ser_cur_time_)
		return DateTimeLocal();
	return *g_ser_cur_time_ + Poco::Timespan(Poco::Timespan::TimeDiff(std::clock() - g_ser_cur_clock_) * 1000);
}

bool Details::stTimerDoByPolicy<tagTimerDoDummySleep>::TimerDoImpl(pt_dword every_sleep, pt_dword time_out, const FunctionBool& func_do)
{
	return CmnTimerDoImpl_(every_sleep, time_out, ::DummySleep, func_do);
}

void WriteQqLoginRecord(const std::string& qq_num, pt_dword service_id, const std::string& full_file_name_path)
{
	pt_csz const kStrSection = "LoginUserRecord";
	pt_csz const kStrZero = "0";
	auto str_service_id = std::to_string(service_id);
	auto str_qq_num = "QQ" + std::to_string(qq_num);
	WritePrivateProfileStringA(kStrSection, str_qq_num.c_str(), str_service_id.c_str(), full_file_name_path.c_str());
	auto str_tmp = str_qq_num + "Mid";
	WritePrivateProfileStringA(kStrSection, str_tmp.c_str(), kStrZero, full_file_name_path.c_str());
	str_tmp = str_qq_num + "TH";
	WritePrivateProfileStringA(kStrSection, str_tmp.c_str(), kStrZero, full_file_name_path.c_str());
	//如此就可以不用输入帐号了。
	WritePrivateProfileStringA("Public", "LastQQUin", qq_num.c_str(), full_file_name_path.c_str());
}

std::string GetSystemFolderPath(int folder)
{
	char buffer[MAX_PATH] = { 0 };
	if (!SUCCEEDED(SHGetFolderPathA(nullptr, folder, nullptr, SHGFP_TYPE_CURRENT, buffer)))
	{
		assert(false);
		return kEmptyStr;
	}
	std::string res(buffer);
	assert(!res.empty());
	return res;
}

std::string GetMyDocumentsFolderPath()
{
	return GetSystemFolderPath(CSIDL_MYDOCUMENTS);
}

std::string Random_LetterStr(size_t cnt_min, size_t cnt_max)
{
	static const char kChars[] =
		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	using namespace std;
	uniform_int_distribution<> index_dist(0, _countof(kChars) - sizeof(char));
	std::string res;
	auto& rng = GetDltRandomDevice();
	uniform_int_distribution<> char_cnt(cnt_min, cnt_max);
	cnt_max = char_cnt(rng);
	for (size_t i = 0; i < cnt_max; ++i)
	{
		res.push_back(kChars[index_dist(rng)]);
	}
	return std::move(res);
}
