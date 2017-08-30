/******************************************************************************
Module:  Toolhelp.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////

#include <tlhelp32.h>
#include <tchar.h>
#include <functional>
#include <atlpath.h>
#include "../DependSelfLib.h"
///////////////////////////////////////////////////////////////////////////////


class CToolhelp{
private:
   HANDLE m_hSnapshot;

public:
   CToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0);
   ~CToolhelp();

   BOOL CreateSnapshot(DWORD dwFlags, DWORD dwProcessID = 0);
   
   BOOL ProcessFirst(PPROCESSENTRY32 ppe) const;
   BOOL ProcessNext(PPROCESSENTRY32 ppe) const;
   BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const;

   BOOL ModuleFirst(PMODULEENTRY32 pme) const;
   BOOL ModuleNext(PMODULEENTRY32 pme) const;
   BOOL ModuleFind(HMODULE pvBaseAddr, PMODULEENTRY32 pme) const;
   BOOL ModuleFind(LPCTSTR pszModName, PMODULEENTRY32 pme) const;
   BOOL ModuleFind( DWORD module_id, PMODULEENTRY32 pme ) const;
   
   BOOL ThreadFirst(PTHREADENTRY32 pte) const;
   BOOL ThreadNext(PTHREADENTRY32 pte) const;
   
   BOOL HeapListFirst(PHEAPLIST32 phl) const;
   BOOL HeapListNext(PHEAPLIST32 phl) const;
   int  HowManyHeaps() const;

   // Note: The heap block functions do not reference a snapshot and
   // just walk the process's heap from the beginning each time. Infinite 
   // loops can occur if the target process changes its heap while the
   // functions below are enumerating the blocks in the heap.
   BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, 
      UINT_PTR dwHeapID) const;
   BOOL HeapNext(PHEAPENTRY32 phe) const;
   int  HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId) const;
   BOOL IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const;

public:
   static BOOL EnablePrivilege(PCTSTR szPrivilege, BOOL fEnable = TRUE);
   static BOOL ReadProcessMemory(DWORD dwProcessID, LPCVOID pvBaseAddress, 
      PVOID pvBuffer, SIZE_T cbRead, SIZE_T* pNumberOfBytesRead = NULL);

   //进程操作
public:
	//根据进程ID得到程序的全路径
	static bool GetFileNameFromProcessId( DWORD process_id, DWORD module_id, CString& exe_full_file_name );
	static bool GetFileNameFromProcessId( DWORD process_id, CString& exe_full_file_name );
	static bool DoAllProcessIdUntil(const std::function<bool(DWORD)>& func);
	//相同名字的进程
	static int DoProcessId( const CPath& exe_full_file_name, const std::function<void(DWORD)>& func );
	static bool DoProcessIdUntil( const CPath& exe_full_file_name, const std::function<bool(DWORD)>& func );
	static bool DoProcessIdUntilAsName(const CPath& exe_name, const std::function<bool(DWORD)>& func);
	//在某一路径下的进程
	static int DoProcessIdAsPath( const CPath& file_path, const std::function<void(DWORD)>& func );
	//关闭所有指定路径的进程
	static int TeminateAllProcess( const CPath& exe_full_file_name );
	static int TeminateAllProcessAsPath( const CPath& file_path );
	//得到相同名字进程的数量
	static int GetSameProcessCnt( const CPath& exe_full_file_name );

public:
	//得到父进程的id
	static DWORD GetParentProcessID( DWORD child_id );
};
///////////////////////////////////////////////////////////////////////////////