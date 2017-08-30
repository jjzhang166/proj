#include "StdAfx.h"
#include "JFZRHook.h"

JFZRHook::JFZRHook(void)
{
}

JFZRHook::~JFZRHook(void)
{
}

// typedef LONG (NTAPI *LPK_NtCreateFile)(
// 									   OUT PHANDLE             FileHandle,
// 									   IN ACCESS_MASK          DesiredAccess,
// 									   IN POBJECT_ATTRIBUTES   ObjectAttributes,
// 									   OUT PIO_STATUS_BLOCK    IoStatusBlock,
// 									   IN PLARGE_INTEGER       AllocationSize OPTIONAL,
// 									   IN ULONG                FileAttributes,
// 									   IN ULONG                ShareAccess,
// 									   IN ULONG                CreateDisposition,
// 									   IN ULONG                CreateOptions,
// 									   IN PVOID                EaBuffer OPTIONAL,
// 									   IN ULONG                EaLength );
// 
// LPK_NtCreateFile	Old_NtCreateFile;
// LPK_NtCreateFile	Jmp_Old_NtCreateFile;
// 
// LONG NTAPI My_NtCreateFile(
// 						   OUT PHANDLE             FileHandle,
// 						   IN ACCESS_MASK          DesiredAccess,
// 						   IN POBJECT_ATTRIBUTES   ObjectAttributes,
// 						   OUT PIO_STATUS_BLOCK    IoStatusBlock,
// 						   IN PLARGE_INTEGER       AllocationSize OPTIONAL,
// 						   IN ULONG                FileAttributes,
// 						   IN ULONG                ShareAccess,
// 						   IN ULONG                CreateDisposition,
// 						   IN ULONG                CreateOptions,
// 						   IN PVOID                EaBuffer OPTIONAL,
// 						   IN ULONG                EaLength )
// {
// 	wchar_t wszPrint[256] = {0};
// 	//swprintf_s(wszPrint,L"NtCreateFile ==> %s",)
// 	
// 	return Jmp_Old_NtCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,FILE_SHARE_READ | FILE_SHARE_WRITE,CreateDisposition,CreateOptions,EaBuffer,EaLength);
// }
// 
