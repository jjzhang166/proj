#pragma once

typedef struct  
{
	HWND	str_hParWnd;						// 父窗口句柄
	char	str_szExeFile[MAX_ZONE_PATH];		// 进程名称,不限大小写,必须填写
	char	str_szWndName[MAX_ZONE_PATH];		// 窗口名称
	char	str_szClassName[MAX_ZONE_PATH];		// 窗口类名
	DWORD	str_GWL_STYLE;						// 窗口风格
	DWORD	str_GWL_EXSTYLE;					// 扩展窗口风格
	DWORD	str_GCL_STYLE;						// 窗口类的风格
	DWORD	str_x;								// x
	DWORD	str_y;								// y
}FINDWINDOW,*PFINDWINDOW;

class EnumPidWnd
{
public:
	HWND	m_hWnd;
	DWORD	m_dwPid;
	bool	m_bIsHaveWnd;
	BOOL	m_bIsWindowVisible;
	PFINDWINDOW	m_pWnd;

	DWORD	m_dwWndCount;
public:
	EnumPidWnd(void);
	~EnumPidWnd(void);
	HWND FindWindow(PFINDWINDOW pfindwnd, bool bIsWindowVisible = true);
	unsigned long GetPidByName(LPCSTR pname);
	DWORD GetShowWndCount(char* pszExeFile);
};
