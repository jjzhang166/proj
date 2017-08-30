// AntiCrack.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AntiCrack.h"
#include "AC_Conne.h"
#include "LuaAc.h"
#include <protect/Protect.h>
#include <boost/thread.hpp>
#include "Protocol/HijackXmlCfg.h"
#include <Common/UsefulFunc.h>
#include "CfgStructs.h"
#include "windows.h"
#include "Process.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void IoRun(){
	VMP_BEGIN_EX;
	ProtectDoIoRun();
	VMP_END;
}

void ClrAtExit()
{
	GetIoService().stop();
	Lua_Ac::ReleaseMe();
	AC_Connect::ClrConne();
}

bool GetUserNameAndUserPwd(std::string& user_name, std::string& user_pwd)
{
	LOG_ENTER_FUNC;
	// 取自身路径
	char	szmepath[260] = {0};
	::GetModuleFileNameA(NULL,szmepath,MAX_PATH);		// D:\FIFA Online 3\TCLS\Tenio\TenioDL\TenioDL.exe

	LOG_O(Log_trace) << szmepath;
	char* pszstr = NULL;
	pszstr = StrStrA(szmepath,"TCLS");	// E:\proj\FIFA\Debug

	if( pszstr == NULL )
	{
		LOG_O(Log_error) << "pszstr = NULL";
		return false;
	}
	*pszstr = 0;

	StrCatA(szmepath,"d3d9.xml");

	LOG_O(Log_trace) << szmepath;

	std::string me_path = szmepath;
	struct Helper
	{
		Helper(const std::string& mepath){
			
			XmlCfg::RestoreHelper<stHijackDllCfg> helper(&cfg, mepath);
		}
		stHijackDllCfg		cfg;
	};
	static Helper helper(me_path);

	//CString game_path =  helper.cfg.inject_dll_full_path_; //    game.dll 的全路径

	// 读账号密码路径

	std::string inject_dll_full_path = CT2CA(helper.cfg.inject_dll_full_path_);

	LOG_O(Log_trace) << inject_dll_full_path;

	char szInjectPath[260] = {0};
	memcpy(szInjectPath,inject_dll_full_path.c_str(),inject_dll_full_path.size());
	char* pinjectpath = StrStrA(szInjectPath,"game.dll");
	if( pinjectpath == NULL )
	{
		LOG_O(Log_error) << "pinjectpath = NULL";
		return false;
	}
	*pinjectpath = 0;

	StrCatA(szInjectPath,"Cfg\\singleton.xml");

	inject_dll_full_path = szInjectPath;

	LOG_O(Log_trace) << inject_dll_full_path;
	// 读账号密码
	Ac_SingletonXml& theSingletonXml = Ac_SingletonXml::GetSelf(inject_dll_full_path);

	user_name = CT2CA(theSingletonXml.authen_account_);
	user_pwd  = CT2CA(theSingletonXml.authen_password_);

	LOG_O(Log_trace) << "user name:" << user_name << "user pwd:" << user_pwd;

	VERIFY(user_name.size());
	VERIFY(user_pwd.size());

	return true;
}

int close()
{
	VMP_BEGIN_EX;
	for (int i=0; i<5; i++)
	{
		CProcess theProcess;
		HANDLE hProcess = (HANDLE)theProcess.GetProcessHandle(_T("TenioDL.exe"));

		LOG_O(Log_trace) << "process handle:" << hProcess;
		if( hProcess != NULL )
		{
			TerminateProcess(hProcess,0);
		}
	}
	VMP_END;
	return 0;
}




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	LogDbgviewInstanceHelper log_dbgview_creator;

	// return close();

	if ( !Lua_Ac::GetMe().InitScript(nullptr, _T("main.lua")) )
	{
		assert(false);
		return 0;
	}
	if ( !AC_Connect::InitConne() )
	{
		assert(!"初始化通信相关失败");
		return 0;
	}
	/*auto& theSingletonXml = Ac_SingletonXml::GetSelf("d:\\tmp\\singleton.xml");
	ClrAtExit();
	return 0;
	std::string user_name, user_pwd;
	assert(GetUserNameAndUserPwd(user_name,user_pwd));
	return 0;*/
	//boost::asio::io_service::work wo(GetIoService());
	//////////////////////////////////////////////////////////////////////////

	VMP_BEGIN;
	GetIoService().post([](){
		//test
		auto& ac_conne = AC_Connect::GetSelfPtr();
		if ( !ac_conne )
		{
			return;
		}

		std::string user_name;
		std::string user_pwd;

		VERIFY(GetUserNameAndUserPwd(user_name,user_pwd));

		ac_conne->GetOper().CheckLogin(user_name,user_pwd);
	});
	PreIoRun(IoRun);
	ClrAtExit();
	VMP_END;

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "正常退出anti进程";
	return 0;
	//////////////////////////////////////////////////////////////////////////	

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ANTICRACK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANTICRACK));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ClrAtExit();
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANTICRACK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ANTICRACK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
