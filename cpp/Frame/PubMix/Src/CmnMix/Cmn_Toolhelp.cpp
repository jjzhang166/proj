#include "stdafx.h"
#include <CmnMix/Cmn_Toolhelp.h>
#include <CmnMix/Cmn_EnsureCleanup.h>
#include <Psapi.h>
#include <boost/algorithm/string/case_conv.hpp>
#include <CommonX/CmnX_DllApi.h>
#include <CmnMix/Cmn_Hdr.h>     /* See Appendix A. */
#include <filesystem>

#pragma comment (lib,"Psapi.lib")

CToolhelp::CToolhelp(DWORD dwFlags, DWORD dwProcessID)
{

	m_hSnapshot = INVALID_HANDLE_VALUE;
	CreateSnapshot(dwFlags, dwProcessID);
}


///////////////////////////////////////////////////////////////////////////////


CToolhelp::~CToolhelp() 
{

	if (m_hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(m_hSnapshot);
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::CreateSnapshot(DWORD dwFlags, DWORD dwProcessID) 
{

	if (m_hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(m_hSnapshot);

	if (dwFlags == 0) {
		m_hSnapshot = INVALID_HANDLE_VALUE;
	} else {
		m_hSnapshot = CreateToolhelp32Snapshot(dwFlags, dwProcessID);
	}
	return(m_hSnapshot != INVALID_HANDLE_VALUE);
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::EnablePrivilege(PCTSTR szPrivilege, BOOL fEnable) 
{

	// Enabling the debug privilege allows the application to see
	// information about service applications
	BOOL fOk = FALSE;    // Assume function fails
	HANDLE hToken;

	// Try to open this process's access token
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, 
		&hToken)) {

			// Attempt to modify the given privilege
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			LookupPrivilegeValue(NULL, szPrivilege, &tp.Privileges[0].Luid);
			tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
			fOk = (GetLastError() == ERROR_SUCCESS);

			// Don't forget to close the token handle
			CloseHandle(hToken);
	}
	return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::ReadProcessMemory(DWORD dwProcessID, 
								  LPCVOID pvBaseAddress, PVOID pvBuffer, SIZE_T cbRead, 
								  SIZE_T* pNumberOfBytesRead) 
{

	return(Toolhelp32ReadProcessMemory(dwProcessID, pvBaseAddress, pvBuffer, 
		cbRead, pNumberOfBytesRead));
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::ProcessFirst(PPROCESSENTRY32 ppe) const 
{

	BOOL fOk = Process32First(m_hSnapshot, ppe);
	if (fOk && (ppe->th32ProcessID == 0))
		fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
	return(fOk);
}


BOOL CToolhelp::ProcessNext(PPROCESSENTRY32 ppe) const 
{

	BOOL fOk = Process32Next(m_hSnapshot, ppe);
	if (fOk && (ppe->th32ProcessID == 0))
		fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
	return(fOk);
}


BOOL CToolhelp::ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const
{

	BOOL fFound = FALSE;
	for (BOOL fOk = ProcessFirst(ppe); fOk; fOk = ProcessNext(ppe)) {
		fFound = (ppe->th32ProcessID == dwProcessId);
		if (fFound) break;
	}
	return(fFound);
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::ModuleFirst(PMODULEENTRY32 pme) const 
{

	return(Module32First(m_hSnapshot, pme));
}


BOOL CToolhelp::ModuleNext(PMODULEENTRY32 pme) const 
{
	return(Module32Next(m_hSnapshot, pme));
}

BOOL CToolhelp::ModuleFind(HMODULE the_module, PMODULEENTRY32 pme) const
{
	BOOL fFound = FALSE;
	for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
		fFound = (pme->hModule == the_module);
		if (fFound) break;
	}
	return(fFound);
}

BOOL CToolhelp::ModuleFind(LPCTSTR pszModName, PMODULEENTRY32 pme) const
{
	using namespace std::tr2::sys;
	for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
		if (0 == _tcsicmp(pme->szModule, pszModName))
			return TRUE;
		if (0 == _tcsicmp(wpath(pme->szExePath).string().c_str(), wpath(pszModName).string().c_str()))
			return TRUE;
	}
	return FALSE;
}

BOOL CToolhelp::ModuleFind( DWORD module_id, PMODULEENTRY32 pme ) const
{
	for ( BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme) )
	{
		if ( pme->th32ModuleID == module_id )
		{
			return TRUE;
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////


BOOL CToolhelp::ThreadFirst(PTHREADENTRY32 pte) const 
{

	return(Thread32First(m_hSnapshot, pte));
}


BOOL CToolhelp::ThreadNext(PTHREADENTRY32 pte) const 
{

	return(Thread32Next(m_hSnapshot, pte));
}


///////////////////////////////////////////////////////////////////////////////


int CToolhelp::HowManyHeaps() const 
{

	int nHowManyHeaps = 0;
	HEAPLIST32 hl = { sizeof(hl) };
	for (BOOL fOk = HeapListFirst(&hl); fOk; fOk = HeapListNext(&hl))
		nHowManyHeaps++;
	return(nHowManyHeaps);
}


int CToolhelp::HowManyBlocksInHeap(DWORD dwProcessID, 
								   DWORD dwHeapID) const 
{

	int nHowManyBlocksInHeap = 0;
	HEAPENTRY32 he = { sizeof(he) };
	BOOL fOk = HeapFirst(&he, dwProcessID, dwHeapID);
	for (; fOk; fOk = HeapNext(&he))
		nHowManyBlocksInHeap++;
	return(nHowManyBlocksInHeap);
}


BOOL CToolhelp::HeapListFirst(PHEAPLIST32 phl) const 
{

	return(Heap32ListFirst(m_hSnapshot, phl));
}


BOOL CToolhelp::HeapListNext(PHEAPLIST32 phl) const 
{

	return(Heap32ListNext(m_hSnapshot, phl));
}


BOOL CToolhelp::HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, 
						  UINT_PTR dwHeapID) const
{

	return(Heap32First(phe, dwProcessID, dwHeapID));
}


BOOL CToolhelp::HeapNext(PHEAPENTRY32 phe) const {

	return(Heap32Next(phe));
}


BOOL CToolhelp::IsAHeap(HANDLE hProcess, PVOID pvBlock, 
						PDWORD pdwFlags) const 
{
	HEAPLIST32 hl = { sizeof(hl) };
	for (BOOL fOkHL = HeapListFirst(&hl); fOkHL; fOkHL = HeapListNext(&hl)) {
		HEAPENTRY32 he = { sizeof(he) };
		BOOL fOkHE = HeapFirst(&he, hl.th32ProcessID, hl.th32HeapID);
		for (; fOkHE; fOkHE = HeapNext(&he)) {
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQueryEx(hProcess, (PVOID) he.dwAddress, &mbi, sizeof(mbi));
			if (chINRANGE(mbi.AllocationBase, pvBlock, 
				(PBYTE) mbi.AllocationBase + mbi.RegionSize)) {

					*pdwFlags = hl.dwFlags;
					return(TRUE);
			}
		}
	}
	return(FALSE);
}

DWORD CToolhelp::GetParentProcessID( DWORD child_id )
{
	DWORD                     dwParentPID = (DWORD)-1;  
	PROCESS_BASIC_INFORMATION pbi;	
	auto the_func = GetNtQueryInformationProcess();
	if ( !the_func )
	{
		assert(false);
		return MAXDWORD;
	}

	// Get process handle   
	auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, child_id);  
	if (!hProcess)  
	{
		return MAXDWORD;  
	}

	// Retrieve information   
	auto status = the_func(hProcess,  
		ProcessBasicInformation,  
		(PVOID)&pbi,  
		sizeof(PROCESS_BASIC_INFORMATION),  
		NULL);  
	if (!NT_SUCCESS(status))
	{
		assert(false);
		return MAXDWORD;
	}
	dwParentPID = pbi.InheritedFromUniqueProcessId;
	CloseHandle(hProcess);
	return dwParentPID;  
}

int CToolhelp::DoProcessId( const CPath& exe_full_file_name, const std::function<void(DWORD)>& func )
{
	auto file_name_offset = exe_full_file_name.FindFileName();
	if ( file_name_offset < 0 )
	{
		assert(!"路径中找不到应用程序名");
		return 0;
	}
	CString str_exe_file_name = exe_full_file_name + file_name_offset;

	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);
	//tool_hlp.ProcessFirst(&p_entry);

	CString str_tmp;
	TCHAR szbuffer[MAX_PATH];
	int cnt = 0;
	for ( auto bOk = tool_hlp.ProcessFirst(&p_entry); bOk; bOk = tool_hlp.ProcessNext(&p_entry) ) 
	{
		if ( 0 == str_exe_file_name.CompareNoCase(p_entry.szExeFile) )
		{
			CEnsureCloseHandle ensure_close = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_entry.th32ProcessID);
			if ( !ensure_close )
			{
				assert(false);
				continue;
			}
			DWORD copy_size = GetModuleFileNameEx(ensure_close, nullptr, szbuffer, _countof(szbuffer));
			if ( copy_size <= 0 )
			{
				continue;
			}
			if ( 0 == _tcsicmp(szbuffer, exe_full_file_name) )
			{
				++cnt;
				func(p_entry.th32ProcessID);
			}
		}					
	}
	return cnt;
}

bool CToolhelp::GetFileNameFromProcessId( DWORD process_id, DWORD module_id, CString& exe_full_file_name )
{
	CToolhelp tool_hlp(TH32CS_SNAPMODULE, process_id);

	MODULEENTRY32 m_entry;
	m_entry.dwSize = sizeof(m_entry);
	if ( !tool_hlp.ModuleFind(module_id, &m_entry) )
		return false;
	exe_full_file_name = m_entry.szExePath;
	return true;
}

bool CToolhelp::GetFileNameFromProcessId( DWORD process_id, CString& exe_full_file_name )
{
	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);

	if ( !tool_hlp.ProcessFind(process_id, &p_entry) )
		return false;

	TCHAR szbuffer[MAX_PATH];
	//CEnsureCloseHandle ensure_close = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, p_entry.th32ProcessID);
	CEnsureCloseHandle ensure_close = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_entry.th32ProcessID);
	if ( !ensure_close )
	{
		assert(false);
		return false;
	}

	if ( GetModuleFileNameEx(ensure_close, nullptr, szbuffer, _countof(szbuffer)) <= 0 )
	{
		assert(false);
		return false;
	}

	/*DWORD copy_size = _countof(szbuffer);
	if ( !QueryFullProcessImageName(ensure_close, 0, szbuffer, &copy_size) )
	{
		assert(false);
		return false;
	}
	if ( copy_size <= 0 )
	{
		assert(false);
		return false;
	}*/
	exe_full_file_name = szbuffer;
	return true;
}

int CToolhelp::TeminateAllProcess( const CPath& exe_full_file_name )
{
	return DoProcessId(exe_full_file_name, []( DWORD process_id ){
		auto process_handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
			if (process_handle)
			{
				CEnsureCloseHandle ensure_close(process_handle);
				::TerminateProcess(process_handle, 0);
			}
	});
}

bool CToolhelp::DoProcessIdUntil( const CPath& exe_full_file_name, const std::function<bool(DWORD)>& func )
{
	auto file_name_offset = exe_full_file_name.FindFileName();
	if ( file_name_offset < 0 )
	{
		assert(!"路径中找不到应用程序名");
		return false;
	}
	CString str_exe_file_name = exe_full_file_name + file_name_offset;

	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);
	//tool_hlp.ProcessFirst(&p_entry);

	CString str_tmp;
	TCHAR szbuffer[MAX_PATH];
	for ( auto bOk = tool_hlp.ProcessFirst(&p_entry); bOk; bOk = tool_hlp.ProcessNext(&p_entry) ) 
	{
		if ( 0 == str_exe_file_name.CompareNoCase(p_entry.szExeFile) )
		{
			CEnsureCloseHandle ensure_close = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_entry.th32ProcessID);
			if ( !ensure_close )
			{
				assert(false);
				continue;
			}
			DWORD copy_size = GetModuleFileNameEx(ensure_close, nullptr, szbuffer, _countof(szbuffer));
			if ( copy_size <= 0 )
			{
				continue;
			}
			if ( 0 == _tcsicmp(szbuffer, exe_full_file_name) )
			{
				if (func(p_entry.th32ProcessID))
					return true;
			}
		}					
	}
	return false;
}

int CToolhelp::GetSameProcessCnt( const CPath& exe_full_file_name )
{
	int cnt = 0;
	DoProcessId(exe_full_file_name, [&]( DWORD ){
		++cnt;
	});
	return cnt;
}

int CToolhelp::DoProcessIdAsPath( const CPath& file_path, const std::function<void(DWORD)>& func )
{
	if ( !file_path.IsDirectory() )
	{
		assert(!"不是有效的路径");
		return 0;
	}
	
	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);
	//tool_hlp.ProcessFirst(&p_entry);
	std::string str_file_path = CT2CA(file_path);
	str_file_path = boost::to_lower_copy(str_file_path);

	TCHAR szbuffer[MAX_PATH] = { 0 };
	int cnt = 0;
	for ( auto bOk = tool_hlp.ProcessFirst(&p_entry); bOk; bOk = tool_hlp.ProcessNext(&p_entry) ) 
	{
		CEnsureCloseHandle ensure_close = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_entry.th32ProcessID);
		if ( !ensure_close )
		{
			//assert(false);
			continue;
		}
		DWORD copy_size = GetModuleFileNameEx(ensure_close, nullptr, szbuffer, _countof(szbuffer));
		if ( copy_size <= 0 )
			continue;

		std::string str_file_full_name = CT2CA(szbuffer);
		str_file_full_name = boost::to_lower_copy(str_file_full_name);
		auto the_pos = str_file_full_name.find(str_file_path);
		if ( the_pos == 0 )
		{
			++cnt;
			func(p_entry.th32ProcessID);
		}					
	}
	return cnt;
}

int CToolhelp::TeminateAllProcessAsPath( const CPath& file_path )
{
	return DoProcessIdAsPath(file_path, []( DWORD process_id ){
		auto process_handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
		if (process_handle)
		{
			CEnsureCloseHandle ensure_close(process_handle);
			::TerminateProcess(process_handle, 0);
		}
	});
}

bool CToolhelp::DoProcessIdUntilAsName(const CPath& exe_name, const std::function<bool(DWORD)>& func)
{
	auto file_name_offset = exe_name.FindFileName();
	if (file_name_offset < 0)
	{
		assert(!"路径中找不到应用程序名");
		return false;
	}
	CString str_exe_file_name = exe_name + file_name_offset;

	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);

	for (auto bOk = tool_hlp.ProcessFirst(&p_entry); bOk; bOk = tool_hlp.ProcessNext(&p_entry))
	{
		if (0 == str_exe_file_name.CompareNoCase(p_entry.szExeFile))
		{
			if (func(p_entry.th32ProcessID))
				return true;
		}
	}
	return false;
}

bool CToolhelp::DoAllProcessIdUntil(const std::function<bool(DWORD)>& func)
{
	CToolhelp tool_hlp(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 p_entry;
	p_entry.dwSize = sizeof(p_entry);

	for (auto bOk = tool_hlp.ProcessFirst(&p_entry); bOk; bOk = tool_hlp.ProcessNext(&p_entry))
	{
		if (func(p_entry.th32ProcessID))
			return true;
	}
	return false;
}


//////////////////////////////// End of File //////////////////////////////////
