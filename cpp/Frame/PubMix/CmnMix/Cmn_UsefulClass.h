#pragma once
/*
@author zhp
@date 2016/3/24 18:40
@purpose some useful little class
*/
#include "../DependSelfLib.h"
#include <CommonX/CmnX_UsefulClass.h>

class EnumPidWnd{
public:
	struct FindInfo{
		HWND			str_hParWnd;						//父窗口句柄
		TCHAR			str_szExeFile[MAX_ZONE_PATH];		//进程名称,不限大小写,必须填写
		TCHAR			str_szWndName[MAX_ZONE_PATH];		//窗口名称
		TCHAR			str_szClassName[MAX_ZONE_PATH];		//窗口类名
		DWORD			str_GWL_STYLE;						//窗口风格
		DWORD			str_GWL_EXSTYLE;					//扩展窗口风格
		DWORD			str_GCL_STYLE;						//窗口类的风格
		DWORD			str_x;								//窗口宽度
		DWORD			str_y;								//窗口高度
		boost::optional<BOOL>	m_bIsWindowVisible;
		mutable DWORD	pid_;
		DWORD			thrd_id_;

		FindInfo();
		bool IsTheFeature(HWND hWnd) const;
	};
	struct stButInfo{
		HWND		but_wnd_;
		pt_dword	but_thrd_id_;

		stButInfo();
	};

public:
	//EnumWindows枚举的是屏幕的顶层窗口。
	HWND FindTheWnd(const FindInfo& find_info);
	HWND FindTheWndBut(const FindInfo& find_info, const stButInfo& but_info);
	static pt_dword GetPidByName(LPCTSTR pname);

private:
	static BOOL CALLBACK EnumProc_IsTheFeature(HWND hWnd, LPARAM lPraram);
	static BOOL CALLBACK EnumProc_IsTheFeatureBut(HWND hWnd, LPARAM lPraram);
	static pt_dword ParsePid(pt_dword the_pid, const FindInfo& find_info);
};
