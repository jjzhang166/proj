#pragma once

// #ifdef _WIN64
// 	#pragma comment(lib, "VMProtectSDK64.lib")
// #else
// 	#pragma comment(lib, "VMProtectSDK32.lib")
// #endif

#ifdef __cplusplus
extern "C" {
#endif

// protection
__declspec(dllimport) void __stdcall VMProtectBegin(const char *);
__declspec(dllimport) void __stdcall VMProtectBeginVirtualization(const char *);
__declspec(dllimport) void __stdcall VMProtectBeginMutation(const char *);
__declspec(dllimport) void __stdcall VMProtectBeginUltra(const char *);
__declspec(dllimport) void __stdcall VMProtectBeginVirtualizationLockByKey(const char *);
__declspec(dllimport) void __stdcall VMProtectBeginUltraLockByKey(const char *);
__declspec(dllimport) void __stdcall VMProtectEnd(void);
__declspec(dllimport) BOOL __stdcall VMProtectIsDebuggerPresent(BOOL);
__declspec(dllimport) BOOL __stdcall VMProtectIsVirtualMachinePresent(void);
__declspec(dllimport) BOOL __stdcall VMProtectIsValidImageCRC(void);
__declspec(dllimport) char * __stdcall VMProtectDecryptStringA(const char *value);
__declspec(dllimport) wchar_t * __stdcall VMProtectDecryptStringW(const wchar_t *value);

// licensing
enum VMProtectSerialStateFlags
{
	SERIAL_STATE_FLAG_CORRUPTED			= 0x00000001,
	SERIAL_STATE_FLAG_INVALID			= 0x00000002,
	SERIAL_STATE_FLAG_BLACKLISTED		= 0x00000004,
	SERIAL_STATE_FLAG_DATE_EXPIRED		= 0x00000008,
	SERIAL_STATE_FLAG_RUNNING_TIME_OVER	= 0x00000010,
	SERIAL_STATE_FLAG_BAD_HWID			= 0x00000020,
	SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED	= 0x00000040,
};
#pragma pack(push, 1)
typedef struct
{
	WORD			wYear;
	BYTE			bMonth;
	BYTE			bDay;
} VMProtectDate;
typedef struct
{
	INT				nState;				// VMProtectSerialStateFlags
	wchar_t			wUserName[256];		// user name
	wchar_t			wEMail[256];		// email
	VMProtectDate	dtExpire;			// date of serial number expiration
	VMProtectDate	dtMaxBuild;			// max date of build, that will accept this key
	INT				bRunningTime;		// running time in minutes
	BYTE			nUserDataLength;	// length of user data in bUserData
	BYTE			bUserData[255];		// up to 255 bytes of user data
} VMProtectSerialNumberData;

#pragma pack(pop)
__declspec(dllimport) INT  __stdcall VMProtectSetSerialNumber(const char * SerialNumber);
__declspec(dllimport) INT  __stdcall VMProtectGetSerialNumberState();
__declspec(dllimport) BOOL __stdcall VMProtectGetSerialNumberData(VMProtectSerialNumberData *pData, UINT nSize);
__declspec(dllimport) INT  __stdcall VMProtectGetCurrentHWID(char * HWID, UINT nSize);

#ifdef __cplusplus
}
#endif



#pragma comment(lib,"VMProtectSDK32.lib")


#ifdef __VMP__
//对程序使用VMP
#define BEGIN_VMP(param) VMProtectBeginUltra(param);		//复杂VM
#define BEGIN_VMP_EX(param) VMProtectBegin(param);			//简单VM
#define END_VMP VMProtectEnd();
#define VMPROTECTDECRYPTSTRINGA(szValue)	VMProtectDecryptStringA(szValue)
#define VMPROTECTGETCURRENTHWID(buf,buflen)	VMProtectGetCurrentHWID(buf,buflen);
#define VMPROTECTDECRYPTSTRINGW(szValue)	VMProtectDecryptStringW(szValue)
#else
//取消程序的VMP
#define BEGIN_VMP(param) 
#define BEGIN_VMP_EX(param)
#define END_VMP
#define VMPROTECTDECRYPTSTRINGA(szValue)	szValue
#define VMPROTECTDECRYPTSTRINGW(szValue)	szValue
#define	VMPROTECTGETCURRENTHWID(buf,buflen)	strcpy(buf,"未开启VMP功能");
#endif
