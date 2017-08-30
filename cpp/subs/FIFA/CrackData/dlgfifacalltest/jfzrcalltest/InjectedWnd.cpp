// InjectedWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "jfzrcalltest.h"
#include "InjectedWnd.h"
#include <assert.h>
#include "jfzrbase.h"
#include "../command/UsefulFuncs.h"


// CInjectedWnd 对话框

IMPLEMENT_DYNAMIC(CInjectedWnd, CDialog)

CInjectedWnd::CInjectedWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectedWnd::IDD, pParent)
{

}

CInjectedWnd::~CInjectedWnd()
{

}

void CInjectedWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInjectedWnd, CDialog)
	ON_BN_CLICKED(IDOK, &CInjectedWnd::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectedWnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInjectedWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInjectedWnd::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CInjectedWnd::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CInjectedWnd::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CInjectedWnd::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CInjectedWnd::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CInjectedWnd::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CInjectedWnd::OnBnClickedButton9)
END_MESSAGE_MAP()


// CInjectedWnd 消息处理程序
void CInjectedWnd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

typedef struct
{
	DWORD	dwEncryptionAddr1;
	DWORD	dwEncryptionAddr2;
	DWORD	dwEncryptionAddr3;
}FIFA_ENCRY_PTION_,*PFIFA_ENCRY_PTION_;

typedef struct
{
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	PFIFA_ENCRY_PTION_	ptheEncryPtion;
	DWORD	dwUnKnow3;
}FIFA_ENCRY_PTION,*PFIFA_ENCRY_PTION;

#define FIFA_REVERSAL_MATCH_TIME_HOOK___		0x00BC190F
#define FIFA_REVERSAL_ENCRY_PTION_INT_CALL___	0x0061BFC0
#define FIFA_REVERSAL_DECRY_PTION_INT_CALL___	0x00483300

void Encryption_int(PFIFA_ENCRY_PTION ptheFifaEncryPtion, int nEncryptionValue)
{
	try
	{
		__asm
		{
			lea eax, nEncryptionValue
			push eax
			mov ecx, ptheFifaEncryPtion
			mov eax, FIFA_REVERSAL_ENCRY_PTION_INT_CALL___
			call eax
		}
	}
	catch (...)
	{
		assert(false);
	}
}

int Decryption_int(PFIFA_ENCRY_PTION ptheFifaEncryPtion)
{
	int nEncryptionValue = 0;
	try
	{
		__asm
		{
			mov ecx, ptheFifaEncryPtion
			mov eax, FIFA_REVERSAL_DECRY_PTION_INT_CALL___ 
			call eax
			mov nEncryptionValue, eax
		}
	}
	catch (...)
	{
		assert(false);
	}

	return nEncryptionValue;
}

typedef struct
{
	int		nFlag;
	DWORD	dwEncryptionValueAddr;
}FIFA_ENCRY_PITON_ADDR,*PFIFA_ENCRY_PITON_ADDR;

void GetEncryptionValueAddr(PFIFA_ENCRY_PITON_ADDR ptheEncryPtionAddr)
{
	__asm
	{
		push 1
		mov ecx, ptheEncryPtionAddr
		mov eax, 0x006C8620
		call eax
	}
}


void Print(int nValue)
{
	try
	{
		FIFA_ENCRY_PITON_ADDR theEncryPtionAddr;
		GetEncryptionValueAddr(&theEncryPtionAddr);

		if( theEncryPtionAddr.dwEncryptionValueAddr != 0 )
		{
			DWORD dwEncryptionValueAddr = theEncryPtionAddr.dwEncryptionValueAddr+0x10+0x18;
			int nEncryptionValue = Decryption_int((PFIFA_ENCRY_PTION)dwEncryptionValueAddr);

			int nValue = 4000;
			//Encryption_int((PFIFA_ENCRY_PTION)dwEncryptionValueAddr,nValue);


			char szPrint[256];
			sprintf_s(szPrint,"[LOG] addr = 0x%p time = %d",dwEncryptionValueAddr,nEncryptionValue);
			OutputDebugStringA(szPrint);
		}
	}
	catch (...)
	{

	}
}

_declspec(naked) void HookTime()
{
	__asm
	{
		MOV DWORD PTR DS:[ESI+0x353A0],EAX

		pushfd
		pushad

		push eax
		lea eax, Print
		call eax
		add esp, 4

		popad
		popfd

		mov eax, FIFA_REVERSAL_MATCH_TIME_HOOK___
		add eax, 6
		jmp eax
	}
}




void CInjectedWnd::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_MATCH_TIME_HOOK___,HookTime);

	
}

void PrintEncryption_int(DWORD dwCallAddr,DWORD dwValue)
{
	if( dwValue == 2608 )
	{
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] call = 0x%p value = %d",dwCallAddr,dwValue);
		OutputDebugStringA(szPrint);
	}
}

_declspec(naked) void HookEncryption_int()
{
	__asm
	{
		mov eax, dword ptr [esp+4]
		mov eax, dword ptr [eax]
		mov dword ptr [ecx],eax
		retn 4

		pushfd
		pushad

		mov eax, [esp+0x24+4]
		mov ebx, [esp+0x24+8]
		push eax
		push ebx
		lea eax, PrintEncryption_int
		call eax
		add esp, 8

		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		MOV EAX,DWORD PTR DS:[0x1503708]

		mov edx, FIFA_REVERSAL_ENCRY_PTION_INT_CALL___
		add edx, 8
		jmp edx
	}
}


_declspec(naked) void HookDecryption_int()
{
	__asm
	{
		mov eax, dword ptr[ecx]
		retn 
	}
}

void CInjectedWnd::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_ENCRY_PTION_INT_CALL___,HookEncryption_int);
	Hook(FIFA_REVERSAL_DECRY_PTION_INT_CALL___,HookDecryption_int);
}

void CInjectedWnd::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{

		for (int i=0; i<10; i++)
		{
			DWORD dwEncryptionAddr =  *(DWORD*)0x14F8FD4;
			if( dwEncryptionAddr == 0 )
				break;
			dwEncryptionAddr = *(DWORD*)(dwEncryptionAddr+0x8);
			if( dwEncryptionAddr == 0 )
				break;
			dwEncryptionAddr = *(DWORD*)(dwEncryptionAddr+0xF0+i*4);

			if( dwEncryptionAddr != 0 )
			{
				DWORD dwEncryptionValueAddr = dwEncryptionAddr+0x10+0x18;
				int nEncryptionValue = Decryption_int((PFIFA_ENCRY_PTION)dwEncryptionValueAddr);

				Encryption_int((PFIFA_ENCRY_PTION)dwEncryptionValueAddr,nEncryptionValue+100);

				int nEncryptionValue2 = Decryption_int((PFIFA_ENCRY_PTION)dwEncryptionValueAddr);

				char szPrint[256];
				sprintf_s(szPrint,"[LOG] addr = 0x%p time = %d time2 = %d",dwEncryptionValueAddr,nEncryptionValue,nEncryptionValue2);
				OutputDebugStringA(szPrint);
			}

		}


	}
	catch (...)
	{
		assert(false);
	}
}

#define FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___		0x00830AA0

_declspec(naked) void HookRevampTime()
{
	__asm
	{
		MOV ECX,EDI
		FSTP DWORD PTR DS:[ESI+0x20]

		cmp DWORD PTR [ECX+0x44], 20
		ja UN_REVAMP
		mov eax, 0x10000
		mov dword ptr [ECX+0x50], eax

UN_REVAMP:
		mov eax, FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___
		add eax, 5
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK___,HookRevampTime);
}


void FIFA_PressKey(DWORD dwVk)
{
	PressKey(dwVk,WM_KEYDOWN);
	PressKey(dwVk,WM_KEYUP);
}


void CInjectedWnd::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

// 	WM_LBUTTONDOWN
// 	WM_LBUTTONUP
// 	WM_KEYDOWN
// 	WM_KEYUP

//	FIFA_MouseLeftClick(837,616);
//	FIFA_PressKey(VK_ESCAPE);

}


void PrintSSLReadAndWriteSocketAddr(DWORD dwSocketAddr)
{
	char szPrint[256];
	sprintf_s(szPrint,"[LOG] ssl read and wirte socket addr = 0x%p",dwSocketAddr);
	OutputDebugStringA(szPrint);
}

_declspec(naked) void HookGetSSLReadAndWriteSocketAddr()
{
	__asm
	{
		pushfd
		pushad

		push edx
		mov eax, PrintSSLReadAndWriteSocketAddr
		call eax
		add esp,4

		popad
		popfd

		PUSH EAX
		LEA ESI,DWORD PTR DS:[EDI+ECX*8+0x160]

		mov eax, FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK
		add eax, 8
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	Hook(FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK,HookGetSSLReadAndWriteSocketAddr);
}

void PrintAnimationFeature(char* pszAcimationStr)
{
	try
	{
		char szPrint[1024];
		sprintf_s(szPrint,sizeof(szPrint),"[LOG] %s",pszAcimationStr);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		
	}

}

_declspec(naked) void HookAnimationFeature()
{
	__asm
	{
		pushfd
		pushad
		
		push ecx
		lea eax, PrintAnimationFeature
		call eax
		add esp, 4

		popad
		popfd

		PUSH 0x1535B28
		mov eax, FIFA_REVERSAL_HOOK_ANIMATION_FEATURE
		add eax, 5
		jmp eax
	}
}


void CInjectedWnd::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_HOOK_ANIMATION_FEATURE,HookAnimationFeature);
}


///////////////////////////////////////////////////////////////////////////////////////////
// HOOK GetDeviceState
DWORD	g_pFunGetDeviceStateAddr;

BOOL	g_bKeyBoardFlag = false;

void PrintKeyBoard(int i)
{
	char szPrint[256] = {0};
	switch(i)
	{
	case DIK_ESCAPE:
		{
			sprintf_s(szPrint,"[INPUT] 按键：ESC");
		}
		break;
	case DIK_A:
		{
			sprintf_s(szPrint,"[INPUT] 按键：A");
		}
		break;
	case DIK_S:
		{
			sprintf_s(szPrint,"[INPUT] 按键：S");
		}
		break;
	case DIK_D:
		{
			sprintf_s(szPrint,"[INPUT] 按键：D");
		}
		break;
	case DIK_W:
		{
			sprintf_s(szPrint,"[INPUT] 按键：W");
		}
		break;
	case DIK_Q:
		{
			sprintf_s(szPrint,"[INPUT] 按键：Q");
		}
		break;
	default:
		{

		}
	}

	OutputDebugStringA(szPrint);
}

void DoGetDeviceState(byte* pKeyBuffer,int nlen)
{
	if( nlen == 0x100 )
	{
		for(int i = 0; i < 256; i++ ) 
		{
			if( pKeyBuffer[i] & 0x80 ) 
			{
				// 这里输出所有的DX按键
				PrintKeyBoard(i);
			}
			else
			{
				// 这里不停的给ESC，方便测试ESC	
				if( DIK_ESCAPE == i )
				{
					if( g_bKeyBoardFlag == FALSE )
					{
						pKeyBuffer[DIK_ESCAPE] = 0x80;
						PrintKeyBoard(i);
						g_bKeyBoardFlag = TRUE;
					}
					else if( g_bKeyBoardFlag == TRUE )
					{
						g_bKeyBoardFlag = FALSE;
					}
				}
			}

		}
	}
}

#define	GAME_GETDEVICESTATE_JNZ		0x00B82D23		// 需要测试的时候，手动更新
// $+2      01167F02          85C0            test    eax, eax
// $+4      01167F04          0F85 17030000   jnz     fifazf.01168221			01168221 - 0x01167F02 = 0x31f
// $+A      01167F0A          3943 08         cmp     dword ptr ds:[ebx+8], eax	01167F0A - 0x01167F02 = 8

__declspec(naked) void DoHookGameGetDeviceState()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		push 0x100
		lea eax, [EBP-FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE_BUFF_OFFSET___]
		push eax
		lea eax, DoGetDeviceState
		call eax
		add esp, 8
	}

	__asm
	{
		popad
		popfd
	}

	__asm
	{
		test    eax, eax
		jnz     GAME_GETDEVICESTATE_JNZ
		mov		ecx, FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE
		add		ecx, 0x8
		jmp		ecx
	}
}


void CInjectedWnd::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE,DoHookGameGetDeviceState);
}

int SleepTime(int nTime)
{
	static int nCount = 0;

	if( nCount++ > 20 )
	{
		Sleep(nTime);
		nCount = 0;
		return 1;
	}

	return 0;
}

#define FIFA_REVERSAL_SLEEP_MAIN_THREAD_HOOK___		0x00665FC0

_declspec(naked) void SleepMainThread()
{
	__asm
	{
		pushfd
		pushad

		push 50
		lea eax, SleepTime
		call eax
		add esp, 4

		test eax,eax
		jz RENDER_RETURN
		popad
		popfd
	
		PUSH EBX
		PUSH ESI
		PUSH EDI
		MOV EDI,ECX
		mov ecx, FIFA_REVERSAL_SLEEP_MAIN_THREAD_HOOK___
		add ecx, 5
		jmp ecx

RENDER_RETURN:
		popad
		popfd

		retn
	}
}

void CInjectedWnd::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(FIFA_REVERSAL_SLEEP_MAIN_THREAD_HOOK___,SleepMainThread);
}
