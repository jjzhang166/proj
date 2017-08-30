#include "stdafx.h"
#include <CmnMix/Cmn_UsefulClass.h>
#include <TlHelp32.h>

//////////////////////////////////////////////////////////////////////////
struct stEnumWndInfo__{
	HWND							m_hWnd;
	const EnumPidWnd::FindInfo*		find_info_;

	stEnumWndInfo__(){
		std::memset(this, 0, sizeof(*this));
	}
};

struct stEnumWndInfoBut__ : stEnumWndInfo__{
	const EnumPidWnd::stButInfo*	but_info_;

	stEnumWndInfoBut__(){
		but_info_ = nullptr;
	}
};
//////////////////////////////////////////////////////////////////////////
HWND EnumPidWnd::FindTheWnd(const FindInfo& find_info)
{
	stEnumWndInfo__ help_info;
	find_info.pid_ = ParsePid(find_info.pid_, find_info);
	help_info.find_info_ = &find_info;
	::EnumWindows(EnumProc_IsTheFeature, (LPARAM)&help_info);
	return help_info.m_hWnd;
}

pt_dword EnumPidWnd::GetPidByName(LPCTSTR pname){
	pt_dword pid = 0;

	TCHAR name[256] = { 0 };
	_tcscpy_s(name, pname);

	::CharLower(name);

	PROCESSENTRY32    lp;
	HANDLE            lh = NULL;
	memset(&lp, 0, sizeof(lp));
	lp.dwSize = sizeof(lp);

	lh = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL ok = ::Process32First(lh, &lp);

	CString str_tmp;
	while (ok)
	{
		str_tmp = lp.szExeFile;
		str_tmp.MakeLower();
		if (str_tmp == name)
		{
			pid = lp.th32ProcessID;
			break;
		}
		ok = ::Process32Next(lh, &lp);
	}
	return pid;
}

BOOL CALLBACK EnumPidWnd::EnumProc_IsTheFeature(HWND hWnd, LPARAM lPraram)
{
	if (NULL == hWnd){ return FALSE; }
	auto the_obj = (stEnumWndInfo__*)lPraram;
	assert(the_obj);
	if (the_obj->m_hWnd)
		return FALSE;
	if (!::IsWindow(hWnd))
		return TRUE;	
	assert(the_obj->find_info_);
	if (the_obj->find_info_->IsTheFeature(hWnd))
	{
		the_obj->m_hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

pt_dword EnumPidWnd::ParsePid(pt_dword the_pid, const FindInfo& find_info)
{
	if (0 == the_pid && _tcslen(find_info.str_szExeFile) != 0)
	{
		the_pid = GetPidByName(find_info.str_szExeFile);
		if (0 == the_pid)
		{
			assert(false);
			return 0;
		}
	}
	return the_pid;
}

BOOL CALLBACK EnumPidWnd::EnumProc_IsTheFeatureBut(HWND hWnd, LPARAM lPraram)
{
	auto the_obj = (stEnumWndInfoBut__*)lPraram;
	assert(the_obj);
	auto bug_info = the_obj->but_info_;
	assert(bug_info);
	if (bug_info->but_wnd_)
	{
		if (hWnd == bug_info->but_wnd_)
			return TRUE;
	}
	if (bug_info->but_thrd_id_)
	{
		auto thrd_id = ::GetWindowThreadProcessId(hWnd, nullptr);
		if (thrd_id == bug_info->but_thrd_id_)
			return TRUE;
	}
	return EnumProc_IsTheFeature(hWnd, lPraram);
}

HWND EnumPidWnd::FindTheWndBut(const FindInfo& find_info, const stButInfo& but_info)
{
	stEnumWndInfoBut__ help_info;
	find_info.pid_ = ParsePid(find_info.pid_, find_info);
	help_info.find_info_ = &find_info;
	help_info.but_info_ = &but_info;
	::EnumWindows(EnumProc_IsTheFeatureBut, (LPARAM)&help_info);
	return help_info.m_hWnd;
}

EnumPidWnd::FindInfo::FindInfo()
{
	str_hParWnd = 0;
	ZeroMemory(str_szExeFile, sizeof(str_szExeFile));
	ZeroMemory(str_szWndName, sizeof(str_szWndName));
	ZeroMemory(str_szClassName, sizeof(str_szClassName));
	str_GWL_STYLE = 0;
	str_GWL_EXSTYLE = 0;
	str_GCL_STYLE = 0;
	str_x = 0;
	str_y = 0;
	pid_ = 0;
	thrd_id_ = 0;
}

bool EnumPidWnd::FindInfo::IsTheFeature(HWND hWnd) const
{
	if (0 != pid_ || 0 != thrd_id_)
	{
		DWORD pid = 0;
		auto thrd_id = ::GetWindowThreadProcessId(hWnd, &pid);
		if (0 != pid_)
		{
			if (pid_ != pid)
				return false;
		}
		if (0 != thrd_id_)
		{
			if (thrd_id != thrd_id_)
				return false;
		}
	}
	if (m_bIsWindowVisible)
	{
		if (::IsWindowVisible(hWnd) != *m_bIsWindowVisible)
			return false;
	}
	
	TCHAR name_buffer[256] = { 0 };
	// 对比窗口名称
	if (_tcslen(str_szWndName) != 0)
	{
		::GetWindowText(hWnd, name_buffer, _countof(name_buffer));
		if (_tcscmp(str_szWndName, name_buffer) != 0)
			return false;
	}

	// 窗口风格
	if (str_GWL_STYLE != 0)
	{
		if (str_GWL_STYLE != ::GetWindowLong(hWnd, GWL_STYLE))
			return false;
	}

	// 扩展窗口风格
	if (str_GWL_EXSTYLE != 0)
	{
		if (str_GWL_EXSTYLE != ::GetWindowLong(hWnd, GWL_EXSTYLE))
			return false;
	}

	// 指定窗口的边框矩形的尺寸
	if (str_x != 0 && str_y != 0)
	{
		RECT rRect;
		if (::GetWindowRect(hWnd, &rRect))
		{
			if (str_x != (rRect.right - rRect.left) || str_y != (rRect.bottom - rRect.top))
				return false;
		}
	}

	// 窗口所属的类的类名
	if (_tcslen(str_szClassName) != 0)
	{
		if (::GetClassName(hWnd, name_buffer, _countof(name_buffer)))
		{
			if (_tcscmp(str_szClassName, name_buffer) != 0)
				return false;
		}
	}

	//窗口类的风格
	if (str_GCL_STYLE != 0)
	{
		if (str_GCL_STYLE != ::GetClassLong(hWnd, GCL_STYLE))
			return false;
	}

	//父窗口句柄
	if (str_hParWnd != NULL)
	{
		if (str_hParWnd != ::GetParent(hWnd))
			return false;
	}

	return true;
}

EnumPidWnd::stButInfo::stButInfo()
{
	but_wnd_ = nullptr;
	but_thrd_id_ = 0;
}
