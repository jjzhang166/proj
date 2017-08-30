// Testdll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Testdll.h"
#include "scan.h"
#include "Log.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTestdllApp

BEGIN_MESSAGE_MAP(CTestdllApp, CWinApp)
END_MESSAGE_MAP()


// CTestdllApp 构造

CTestdllApp::CTestdllApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestdllApp 对象

CTestdllApp theApp;


// CTestdllApp 初始化

BOOL CTestdllApp::InitInstance()
{
	CWinApp::InitInstance();

	char szPrint[256] = {0};

	OutputDebugStringA("[FIFA] **************游戏基址,CALL等地址****************************************************");

	LOG_INIT(LOG_FILE);

	LOG << "#pragma once";
	LOG_END;
	/*++
	游戏基址
	00B4F20B    E8 901E0A00     CALL fifazf.00BF10A0
	00B4F210    8BC8            MOV ECX,EAX
	00B4F212    E8 99FF0900     CALL fifazf.00BEF1B0
	00B4F217    8947 20         MOV DWORD PTR DS:[EDI+0x20],EAX
	00B4F21A    5F              POP EDI
	00B4F21B    5E              POP ESI
	00B4F21C    5D              POP EBP
	00B4F21D    C2 0800         RETN 0x8
	00B4F220    A1 DCF05701     MOV EAX,DWORD PTR DS:[0x157F0DC]
	00B4F225    C3              RETN

	C20800A1????????C3
	E8????????8BC8E8????????8947??5F5E5DC20800A1
	--*/

// 	DWORD	dwGameBase = ScanImme((HANDLE)-1,"E8????????8B??E8????????89????5F5E5DC2",0x156);
// 	wsprintf(szPrint,"[FIFA] 游戏基址:0x%p \r\n",dwGameBase);
// 	OutputDebugString(szPrint);
// 	wsprintf(szPrint,"[FIFA] const MemOper<int> g_pGameBase(0x%p);",dwGameBase);
// 	OutputDebugString(szPrint);
// 	
// 	LOG << "#define UPDATES_FIFA_REVERSAL_GAME_BASE_ADDR  ";
// 	LOG << dwGameBase;
// 	LOG_END;

	

	/*++
	发包CALL
	00F17760    83EC 08         SUB ESP,0x8
	00F17763    8B4424 1C       MOV EAX,DWORD PTR SS:[ESP+0x1C]
	00F17767    53              PUSH EBX
	00F17768    55              PUSH EBP
	00F17769    56              PUSH ESI
	00F1776A    57              PUSH EDI
	00F1776B    8B7C24 20       MOV EDI,DWORD PTR SS:[ESP+0x20]
	00F1776F    8B9487 34010000 MOV EDX,DWORD PTR DS:[EDI+EAX*4+0x134]
	00F17776    8B4424 1C       MOV EAX,DWORD PTR SS:[ESP+0x1C]
	00F1777A    8B18            MOV EBX,DWORD PTR DS:[EAX]

	83EC??8B4424??535556578B7C24??8B9487????????8B4424??8B18 
	--*/

	DWORD	dwHttpSendCall = ScanAddr((HANDLE)-1,"83EC??8B4424??535556578B7C24??8B9487????????8B4424??8B18",0);
	sprintf_s(szPrint,"[FIFA] 发包CALL:0x%p, HOOK1:0x%p, HOOK2:0x%p, HOOK3:0x%p\r\n",dwHttpSendCall,dwHttpSendCall+0x162,dwHttpSendCall+0x17E,dwHttpSendCall+0x1B2);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHttpSendCall_ = 0x%p;\r\n",dwHttpSendCall);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL  ";
	LOG << dwHttpSendCall;
	LOG_END;


	/*++

	// HOOK ssl_write 调用的上一层，获取发包时的socket对象地址
	$ ==>    00486BBA        3D FFFFFF7F          CMP EAX,0x7FFFFFFF
	$+5      00486BBF        76 05                JBE SHORT fifazf.00486BC6
	$+7      00486BC1        B8 FFFFFF7F          MOV EAX,0x7FFFFFFF
	$+C      00486BC6        8D0C76               LEA ECX,DWORD PTR DS:[ESI+ESI*2]
	$+F      00486BC9        8B94CF 60010000      MOV EDX,DWORD PTR DS:[EDI+ECX*8+0x160]
	$+16     00486BD0        50                   PUSH EAX
	$+17     00486BD1        8DB4CF 60010000      LEA ESI,DWORD PTR DS:[EDI+ECX*8+0x160]
	$+1E     00486BD8        53                   PUSH EBX
	$+1F     00486BD9        52                   PUSH EDX
	$+20     00486BDA        E8 31820100          CALL fifazf.0049EE10                     ; ssl_write
	$+25     00486BDF        83C4 0C              ADD ESP,0xC
	$+28     00486BE2        85C0                 TEST EAX,EAX
	$+2A     00486BE4        7F 70                JG SHORT fifazf.00486C56
	$+2C     00486BE6        50                   PUSH EAX
	$+2D     00486BE7        8B06                 MOV EAX,DWORD PTR DS:[ESI]
	$+2F     00486BE9        50                   PUSH EAX
	$+30     00486BEA        E8 519D0100          CALL fifazf.004A0940


	3DFFFFFF7F76????FFFFFF7F8D????8B
	--*/
	DWORD	dwHttpSSL_SocketAddr = ScanAddr((HANDLE)-1,"3DFFFFFF7F76????FFFFFF7F8D????8B",0x16);
	LOG << "#define UPDATES_FIFA_REVERSAL_SSL_READ_AND_WRITE_SOCKET_ADDR_HOOK  ";
	LOG << dwHttpSSL_SocketAddr;
	LOG_END;



	/*++
	SSL_write
	00F45F90    8B4424 04       MOV EAX,DWORD PTR SS:[ESP+0x4]
	00F45F94    8378 20 00      CMP DWORD PTR DS:[EAX+0x20],0x0
	00F45F98    75 22           JNZ SHORT fifazf.00F45FBC
	00F45F9A    68 DE030000     PUSH 0x3DE
	00F45F9F    68 7C433501     PUSH fifazf.0135437C                     ; ASCII ".\\ssl\\ssl_lib.c"
	00F45FA4    68 14010000     PUSH 0x114
	00F45FA9    68 D0000000     PUSH 0xD0
	00F45FAE    6A 14           PUSH 0x14
	00F45FB0    E8 3BA4FCFF     CALL fifazf.00F103F0


	8B4424??8378????75??68DE??000068????????68????????68????????6A??E8
	--*/

	DWORD	dwHttpSSL_write = ScanAddr((HANDLE)-1,"8B4424??8378????75??68DE??000068????????68????????68????????6A??E8",0);
	sprintf_s(szPrint,"[FIFA] SSL_write:0x%p \r\n",dwHttpSSL_write);
	OutputDebugStringA(szPrint); 

	LOG << "#define UPDATES_FIFA_REVERSAL_SSL_WRITE  ";
	LOG << dwHttpSSL_write;
	LOG_END;

	/*++
	SSL_read
	00F45EF0    8B4424 04       MOV EAX,DWORD PTR SS:[ESP+0x4]           ; 收包
	00F45EF4    8378 20 00      CMP DWORD PTR DS:[EAX+0x20],0x0
	00F45EF8    75 22           JNZ SHORT fifazf.00F45F1C
	00F45EFA    68 BF030000     PUSH 0x3BF
	00F45EFF    68 7C433501     PUSH fifazf.0135437C                     ; ASCII ".\\ssl\\ssl_lib.c"
	00F45F04    68 14010000     PUSH 0x114
	00F45F09    68 DF000000     PUSH 0xDF
	00F45F0E    6A 14           PUSH 0x14
	00F45F10    E8 DBA4FCFF     CALL fifazf.00F103F0

	8B4424??8378????75??68BF??000068????????68????????68????????6A??E8
	--*/
	DWORD	dwHttpSSL_read = ScanAddr((HANDLE)-1,"8B4424??8378????75??68BF??000068????????68????????68????????6A??E8",0);
	sprintf_s(szPrint,"[FIFA] SSL_read:0x%p \r\n",dwHttpSSL_read);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_SSL_READ  ";
	LOG << dwHttpSSL_read;
	LOG_END;

	/*++
	SSL_get_error
	00F47AB0      55              PUSH EBP
	00F47AB1      8B6C24 0C       MOV EBP,DWORD PTR SS:[ESP+0xC]
	00F47AB5      85ED            TEST EBP,EBP
	00F47AB7      7E 04           JLE SHORT fifazf.00F47ABD
	00F47AB9      33C0            XOR EAX,EAX
	00F47ABB      5D              POP EBP
	00F47ABC      C3              RETN
	00F47ABD      E8 BE8DFCFF     CALL fifazf.00F10880
	00F47AC2      85C0            TEST EAX,EAX
	00F47AC4      74 1A           JE SHORT fifazf.00F47AE0

	558B6C24??85ED7E??33C05DC3E8????????85C074
	--*/
	DWORD	dwSSL_get_error = ScanAddr((HANDLE)-1,"558B6C24??85ED7E??33C05DC3E8????????85C074",0);
	sprintf_s(szPrint,"[FIFA] SSL_get_error:0x%p \r\n",dwSSL_get_error);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_SSL_GET_ERROR  ";
	LOG << dwSSL_get_error;
	LOG_END;

	/*++
	收包CALL
	00F8AF30    0F291424        MOVAPS DQWORD PTR SS:[ESP],XMM2
	00F8AF34    B9 10000000     MOV ECX,0x10
	00F8AF39    89E6            MOV ESI,ESP
	00F8AF3B    29C1            SUB ECX,EAX
	00F8AF3D    89F6            MOV ESI,ESI
	00F8AF3F    F3:A4           REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[>
	00F8AF41    8B6424 10       MOV ESP,DWORD PTR SS:[ESP+0x10]
	00F8AF45    8B6C24 24       MOV EBP,DWORD PTR SS:[ESP+0x24]
	00F8AF49    0F117D 00       MOVUPS DQWORD PTR SS:[EBP],XMM7
	00F8AF4D    5F              POP EDI
	00F8AF4E    5E              POP ESI
	00F8AF4F    5B              POP EBX
	00F8AF50    5D              POP EBP
	00F8AF51    C3              RETN

	0F291424B9????????89E629C189F6F3A48B6424
	--*/

	DWORD dwHookRecvCallAddr = ScanAddr((HANDLE)-1,"0F291424B9????????89E629C189F6F3A48B6424",0x1d);
	sprintf_s(szPrint,"[FIFA] 收包CALL:0x%p\r\n",dwHookRecvCallAddr+4);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookRecvCallAddr_ = 0x%p;\r\n",dwHookRecvCallAddr);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_ADDR  ";
	LOG << dwHookRecvCallAddr;
	LOG_END;

	/*++
	收包CALL1
	00F99CC8    9D                     POPFD
	00F99CC9    5F                     POP EDI
	00F99CCA    5E                     POP ESI
	00F99CCB    5B                     POP EBX
	00F99CCC    5D                     POP EBP
	00F99CCD    C3                     RETN
	00F99CCE    9C                     PUSHFD
	00F99CCF    90                     NOP
	00F99CD0    8B00                   MOV EAX,DWORD PTR DS:[EAX]
	00F99CD2    8B7C24 24              MOV EDI,DWORD PTR SS:[ESP+0x24]
	00F99CD6    8D7424 B0              LEA ESI,DWORD PTR SS:[ESP-0x50]
	00F99CDA    81E6 C0FFFFFF          AND ESI,0xFFFFFFC0
	00F99CE0    8D9F 71FFFFFF          LEA EBX,DWORD PTR DS:[EDI-0x8F]
	00F99CE6    29F3                   SUB EBX,ESI
	00F99CE8    F7DB                   NEG EBX
	00F99CEA    81E3 C0030000          AND EBX,0x3C0
	00F99CF0    29DE                   SUB ESI,EBX
	00F99CF2    8D9E 00030000          LEA EBX,DWORD PTR DS:[ESI+0x300]
	00F99CF8    29EB                   SUB EBX,EBP
	00F99CFA    81E3 00030000          AND EBX,0x300
	00F99D00    8DAC1D 80080000        LEA EBP,DWORD PTR SS:[EBP+EBX+0x880]
	00F99D07    8D5424 18              LEA EDX,DWORD PTR SS:[ESP+0x18]
	00F99D0B    87E6                   XCHG ESI,ESP
	00F99D0D    81C4 04000000          ADD ESP,0x4
	00F99D13    896C24 18              MOV DWORD PTR SS:[ESP+0x18],EBP
	00F99D17    897424 1C              MOV DWORD PTR SS:[ESP+0x1C],ESI
	00F99D1B    894424 34              MOV DWORD PTR SS:[ESP+0x34],EAX
	00F99D1F    8B02                   MOV EAX,DWORD PTR DS:[EDX]
	00F99D21    8B5A 04                MOV EBX,DWORD PTR DS:[EDX+0x4]
	00F99D24    8B72 10                MOV ESI,DWORD PTR DS:[EDX+0x10]
	00F99D27    8B52 14                MOV EDX,DWORD PTR DS:[EDX+0x14]
	00F99D2A    894424 20              MOV DWORD PTR SS:[ESP+0x20],EAX
	00F99D2E    895C24 24              MOV DWORD PTR SS:[ESP+0x24],EBX
	00F99D32    894C24 28              MOV DWORD PTR SS:[ESP+0x28],ECX
	00F99D36    897C24 2C              MOV DWORD PTR SS:[ESP+0x2C],EDI
	00F99D3A    897424 30              MOV DWORD PTR SS:[ESP+0x30],ESI
	00F99D3E    89F7                   MOV EDI,ESI
	00F99D40    89C6                   MOV ESI,EAX
	00F99D42    81FA 00000000          CMP EDX,0x0
	00F99D48    0F84 62010000          JE fifazf.00F99EB0
	00F99D4E    81F9 10000000          CMP ECX,0x10
	00F99D54    89DA                   MOV EDX,EBX
	00F99D56    0F82 14010000          JB fifazf.00F99E70
	00F99D5C    0FBA6424 34 19         BT DWORD PTR SS:[ESP+0x34],0x19
	00F99D62    0F83 78000000          JNB fifazf.00F99DE0
	00F99D68    0F6F07                 MOVQ MM0,QWORD PTR DS:[EDI]
	00F99D6B    0F6F67 08              MOVQ MM4,QWORD PTR DS:[EDI+0x8]
	00F99D6F    90                     NOP
	00F99D70    0FEF06                 PXOR MM0,QWORD PTR DS:[ESI]
	00F99D73    0FEF66 08              PXOR MM4,QWORD PTR DS:[ESI+0x8]
	00F99D77    8B7C24 2C              MOV EDI,DWORD PTR SS:[ESP+0x2C]
	00F99D7B    E8 20D4FFFF            CALL fifazf.00F971A0

	0F6F070F6F67??900FEF060FEF66??8B7C24??E8
	--*/

	dwHookRecvCallAddr = ScanAddr((HANDLE)-1,"0F6F070F6F67??900FEF060FEF66??8B7C24??E8",-0x9F);
	sprintf_s(szPrint,"[FIFA] 收包CALL1:0x%p\r\n",dwHookRecvCallAddr+4);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookRecvCallAddr1_ = 0x%p;\r\n",dwHookRecvCallAddr);
	OutputDebugStringA(szPrint);
		
	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_1  ";
	LOG << dwHookRecvCallAddr;
	LOG_END;

	/*++
	收包CALL2
	00F99EE0    0F6F0E                 MOVQ MM1,QWORD PTR DS:[ESI]
	00F99EE3    0F6F6E 08              MOVQ MM5,QWORD PTR DS:[ESI+0x8]
	00F99EE7    0FEF07                 PXOR MM0,QWORD PTR DS:[EDI]
	00F99EEA    0FEF67 08              PXOR MM4,QWORD PTR DS:[EDI+0x8]
	00F99EEE    0F7F0F                 MOVQ QWORD PTR DS:[EDI],MM1
	00F99EF1    0F7F6F 08              MOVQ QWORD PTR DS:[EDI+0x8],MM5
	00F99EF5    81E9 10000000          SUB ECX,0x10
	00F99EFB    0F82 2F000000          JB fifazf.00F99F30
	00F99F01    0F7F03                 MOVQ QWORD PTR DS:[EBX],MM0
	00F99F04    0F7F63 08              MOVQ QWORD PTR DS:[EBX+0x8],MM4
	00F99F08    8D5B 10                LEA EBX,DWORD PTR DS:[EBX+0x10]
	00F99F0B    895C24 24              MOV DWORD PTR SS:[ESP+0x24],EBX
	00F99F0F    8D76 10                LEA ESI,DWORD PTR DS:[ESI+0x10]
	00F99F12    897424 20              MOV DWORD PTR SS:[ESP+0x20],ESI
	00F99F16    894C24 28              MOV DWORD PTR SS:[ESP+0x28],ECX
	00F99F1A  ^ 0F85 9CFFFFFF          JNZ fifazf.00F99EBC
	00F99F20    0F77                   EMMS
	00F99F22    8B6424 1C              MOV ESP,DWORD PTR SS:[ESP+0x1C]
	00F99F26    9D                     POPFD
	00F99F27    5F                     POP EDI
	00F99F28    5E                     POP ESI
	00F99F29    5B                     POP EBX
	00F99F2A    5D                     POP EBP
	00F99F2B    C3                     RETN

	0F6F0E0F6F6E??0FEF070FEF67??0F7F0F0F7F6F
	--*/

	dwHookRecvCallAddr = ScanAddr((HANDLE)-1,"0F6F0E0F6F6E??0FEF070FEF67??0F7F0F0F7F6F",0x47);
	sprintf_s(szPrint,"[FIFA] 收包CALL2:0x%p\r\n",dwHookRecvCallAddr+4);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookRecvCallAddr2_ = 0x%p;\r\n",dwHookRecvCallAddr);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_2  ";
	LOG << dwHookRecvCallAddr;
	LOG_END;

	/*++
	收包CALL3
	00F9B281    0F6F06          MOVQ MM0,QWORD PTR DS:[ESI]
	00F9B284    66:0F7F4C24 10  MOVQ QWORD PTR SS:[ESP+0x10],MM1
	00F9B28A    66:0F7F4424 20  MOVQ QWORD PTR SS:[ESP+0x20],MM0
	00F9B290    E8 8BF8FFFF     CALL fifazf.00F9AB20
	00F9B295    8B1C24          MOV EBX,DWORD PTR SS:[ESP]
	00F9B298    8B5424 04       MOV EDX,DWORD PTR SS:[ESP+0x4]
	00F9B29C    66:0FEF4424 10  PXOR MM0,QWORD PTR SS:[ESP+0x10]
	00F9B2A2    66:0F6F4C24 20  MOVQ MM1,QWORD PTR SS:[ESP+0x20]
	00F9B2A8    F3:             PREFIX REP:
	00F9B2A9    0F7F0433        MOVQ QWORD PTR DS:[EBX+ESI],MM0
	00F9B2AD    8D76 10         LEA ESI,DWORD PTR DS:[ESI+0x10]
	00F9B2B0    81EF 10000000   SUB EDI,0x10
	00F9B2B6  ^ 0F83 C4FFFFFF   JNB fifazf.00F9B280
	00F9B2BC    8B5C24 08       MOV EBX,DWORD PTR SS:[ESP+0x8]
	00F9B2C0    8B6424 30       MOV ESP,DWORD PTR SS:[ESP+0x30]
	00F9B2C4    F3:             PREFIX REP:
	00F9B2C5    0F7F0B          MOVQ QWORD PTR DS:[EBX],MM1
	00F9B2C8    5F              POP EDI
	00F9B2C9    5E              POP ESI
	00F9B2CA    5B              POP EBX
	00F9B2CB    5D              POP EBP
	00F9B2CC    C3              RETN

	0F6F06660F7F4C24??660F7F4424??E8
	--*/
	dwHookRecvCallAddr = ScanAddr((HANDLE)-1,"0F6F06660F7F4C24??660F7F4424??E8",0x47);
	sprintf_s(szPrint,"[FIFA] 收包CALL3:0x%p\r\n",dwHookRecvCallAddr+4);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookRecvCallAddr3_ = 0x%p;\r\n",dwHookRecvCallAddr);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_RECV_CALL_3  ";
	LOG << dwHookRecvCallAddr;
	LOG_END;

	/*++
	动画特征
	00B89B1F    8B00            MOV EAX,DWORD PTR DS:[EAX]
	00B89B21    3977 14         CMP DWORD PTR DS:[EDI+0x14],ESI
	00B89B24    72 04           JB SHORT fifazf.00B89B2A
	00B89B26    8B0F            MOV ECX,DWORD PTR DS:[EDI]
	00B89B28    EB 02           JMP SHORT fifazf.00B89B2C
	00B89B2A    8BCF            MOV ECX,EDI
	00B89B2C    50              PUSH EAX
	00B89B2D    51              PUSH ECX
	00B89B2E    68 E0492A01     PUSH fifazf.012A49E0                     ; ASCII "[GfxManager] ASFunc::Invoke : %s, %s\n"
	00B89B33    E8 B864C0FF     CALL fifazf.0078FFF0

	8B003977??72??8B0FEB??8BCF505168????????E8

	$-C      00985A05        8378 14 10      CMP DWORD PTR DS:[EAX+0x14],0x10
	$-8      00985A09        72 02           JB SHORT fifazf.00985A0D
	$-6      00985A0B        8B00            MOV EAX,DWORD PTR DS:[EAX]
	$-4      00985A0D        837F 14 10      CMP DWORD PTR DS:[EDI+0x14],0x10
	$ ==>    00985A11        72 04           JB SHORT fifazf.00985A17
	$+2      00985A13        8B0F            MOV ECX,DWORD PTR DS:[EDI]
	$+4      00985A15        EB 02           JMP SHORT fifazf.00985A19
	$+6      00985A17        8BCF            MOV ECX,EDI
	$+8      00985A19        50              PUSH EAX
	$+9      00985A1A        51              PUSH ECX
	$+A      00985A1B        68 285B5301     PUSH fifazf.01535B28                     ; ASCII "[GfxManager] ASFunc::Invoke : %s, %s\n"
	$+F      00985A20        E8 CBFC2300     CALL fifazf.00BC56F0
	$+14     00985A25        83C4 0C         ADD ESP,0xC
	$+17     00985A28        83BD 04FFFFFF 1>CMP DWORD PTR SS:[EBP-0xFC],0x10
	$+1E     00985A2F        72 0E           JB SHORT fifazf.00985A3F
	$+20     00985A31        FFB5 F0FEFFFF   PUSH DWORD PTR SS:[EBP-0x110]


	72??8B??EB??8B??????68????????E8
	--*/

	DWORD dwHookAnimationFeature = ScanAddr((HANDLE)-1,"72??8B??EB??8B??????68????????E8",0xA,4,2);
	assert(dwHookAnimationFeature);
	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_ANIMATION_FEATURE  ";
	LOG << dwHookAnimationFeature;
	LOG_END;

	/*++
	DX按键

	$-1B     >  8D85 D8FEFFFF   LEA EAX,DWORD PTR SS:[EBP-0x128]
	$-15     >  68 00010000     PUSH 0x100
	$-10     >  50              PUSH EAX
	$-F      >  E8 630746FF     CALL fifazf.005F6580
	$-A      >  8B3F            MOV EDI,DWORD PTR DS:[EDI]
	$-8      >  8B0F            MOV ECX,DWORD PTR DS:[EDI]
	$-6      >  8B41 24         MOV EAX,DWORD PTR DS:[ECX+0x24]
	$-3      >  83C4 08         ADD ESP,0x8
	$ ==>    >  8D95 D8FEFFFF   LEA EDX,DWORD PTR SS:[EBP-0x128]
	$+6      >  52              PUSH EDX
	$+7      >  68 00010000     PUSH 0x100
	$+C      >  57              PUSH EDI
	$+D      >  FFD0            CALL EAX
	$+F      >  85C0            TEST EAX,EAX
	$+11     >  0F85 23030000   JNZ fifazf.01196161

	8D????????????6800010000??????85C00F85

	$-20     00B829AD      68 00010000         PUSH 0x100
	$-1B     00B829B2      50                  PUSH EAX
	$-1A     00B829B3      8D85 FCFEFFFF       LEA EAX,DWORD PTR SS:[EBP-0x104]
	$-14     00B829B9      50                  PUSH EAX
	$-13     00B829BA      E8 5DDAAEFF         CALL fifazf.0067041C                     ; JMP 到 msvcr120.memset
	$-E      00B829BF      8B07                MOV EAX,DWORD PTR DS:[EDI]
	$-C      00B829C1      8D95 FCFEFFFF       LEA EDX,DWORD PTR SS:[EBP-0x104]
	$-6      00B829C7      83C4 0C             ADD ESP,0xC
	$-3      00B829CA      8B08                MOV ECX,DWORD PTR DS:[EAX]
	$-1      00B829CC      52                  PUSH EDX
	$ ==>    00B829CD      68 00010000         PUSH 0x100
	$+5      00B829D2      50                  PUSH EAX
	$+6      00B829D3      FF51 24             CALL DWORD PTR DS:[ECX+0x24]
	$+9      00B829D6      85C0                TEST EAX,EAX
	$+B      00B829D8      0F85 45030000       JNZ fifazf.00B82D23
	$+11     00B829DE      33FF                XOR EDI,EDI
	$+13     00B829E0      397B 08             CMP DWORD PTR DS:[EBX+0x8],EDI
	$+16     00B829E3      0F86 C1020000       JBE fifazf.00B82CAA

	6800010000??FF????85C00F85

	--*/

	DWORD dwHookAfterGetDeviceState = ScanAddr((HANDLE)-1,"6800010000??FF????85C00F85",9);
	sprintf_s(szPrint,"[FIFA] DX按键:0x%p\r\n",dwHookAfterGetDeviceState);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookAfterGetDeviceState_ = 0x%p;\r\n",dwHookAfterGetDeviceState);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_AFTER_GET_DEVICE_STATE  ";
	LOG << dwHookAfterGetDeviceState;
	LOG_END;

	/*++
	DX模式
	01167436    8B76 0C         MOV ESI,DWORD PTR DS:[ESI+0xC]
	01167439    85F6            TEST ESI,ESI
	0116743B    74 11           JE SHORT fifazf.0116744E
	0116743D    8B03            MOV EAX,DWORD PTR DS:[EBX]
	0116743F    8B08            MOV ECX,DWORD PTR DS:[EAX]
	01167441    8B51 34         MOV EDX,DWORD PTR DS:[ECX+0x34]
	01167444    6A 06           PUSH 0x6
	01167446    56              PUSH ESI
	01167447    50              PUSH EAX
	01167448    FFD2            CALL EDX
	0116744A    85C0            TEST EAX,EAX

	8B76??85F6????8B038B088B51??6A065650FFD285C0

	$ ==>    0119D8C6        83CA 02                   OR EDX,0x2
	$+3      0119D8C9        52                        PUSH EDX
	$+4      0119D8CA        8B56 0C                   MOV EDX,DWORD PTR DS:[ESI+0xC]
	$+7      0119D8CD        52                        PUSH EDX
	$+8      0119D8CE        50                        PUSH EAX
	$+9      0119D8CF        8B41 34                   MOV EAX,DWORD PTR DS:[ECX+0x34]
	$+C      0119D8D2        FFD0                      CALL EAX
	$+E      0119D8D4        85C0                      TEST EAX,EAX
	$+10     0119D8D6      ^ 75 AD                     JNZ SHORT fifazf.0119D885
	$+12     0119D8D8        8D8D BCFDFFFF             LEA ECX,DWORD PTR SS:[EBP-0x244]
	$+18     0119D8DE        68 44020000               PUSH 0x244
	$+1D     0119D8E3        51                        PUSH ECX
	$+1E     0119D8E4        E8 27C745FF               CALL fifazf.005FA010
	$+23     0119D8E9        C785 BCFDFFFF 44020000    MOV DWORD PTR SS:[EBP-0x244],0x244
	$+2D     0119D8F3        8B03                      MOV EAX,DWORD PTR DS:[EBX]
	$+2F     0119D8F5        8B10                      MOV EDX,DWORD PTR DS:[EAX]
	$+31     0119D8F7        8B52 3C                   MOV EDX,DWORD PTR DS:[EDX+0x3C]
	$+34     0119D8FA        83C4 08                   ADD ESP,0x8
	$+37     0119D8FD        8D8D BCFDFFFF             LEA ECX,DWORD PTR SS:[EBP-0x244]
	$+3D     0119D903        51                        PUSH ECX
	$+3E     0119D904        50                        PUSH EAX
	$+3F     0119D905        FFD2                      CALL EDX

	83CA02??8B????????8B????FF??85C075??8D8D

	$-2C     00B8113E              837D 18 00           CMP DWORD PTR SS:[EBP+0x18],0x0
	$-28     00B81142              8B85 B4FDFFFF        MOV EAX,DWORD PTR SS:[EBP-0x24C]
	$-22     00B81148              F3:                  PREFIX REP:
	$-21     00B81149              0F6F47 14            MOVQ MM0,QWORD PTR DS:[EDI+0x14]
	$-1D     00B8114D              F3:                  PREFIX REP:
	$-1C     00B8114E              0F7F80 20030000      MOVQ QWORD PTR DS:[EAX+0x320],MM0
	$-15     00B81155              74 22                JE SHORT fifazf.00B81179
	$-13     00B81157              8B0B                 MOV ECX,DWORD PTR DS:[EBX]
	$-11     00B81159              33C0                 XOR EAX,EAX
	$-F      00B8115B              3845 14              CMP BYTE PTR SS:[EBP+0x14],AL
	$-C      00B8115E              0F94C0               SETE AL
	$-9      00B81161              8B11                 MOV EDX,DWORD PTR DS:[ECX]
	$-7      00B81163              8D0485 04000000      LEA EAX,DWORD PTR DS:[EAX*4+0x4]
	$ ==>    00B8116A              83C8 02              OR EAX,0x2
	$+3      00B8116D              50                   PUSH EAX
	$+4      00B8116E              FF76 0C              PUSH DWORD PTR DS:[ESI+0xC]
	$+7      00B81171              51                   PUSH ECX
	$+8      00B81172              FF52 34              CALL DWORD PTR DS:[EDX+0x34]
	$+B      00B81175              85C0                 TEST EAX,EAX
	$+D      00B81177            ^ 75 B0                JNZ SHORT fifazf.00B81129
	$+F      00B81179              68 44020000          PUSH 0x244
	$+14     00B8117E              8D85 B8FDFFFF        LEA EAX,DWORD PTR SS:[EBP-0x248]
	$+1A     00B81184              6A 00                PUSH 0x0
	$+1C     00B81186              50                   PUSH EAX
	$+1D     00B81187              E8 90F2AEFF          CALL fifazf.0067041C                     ; JMP 到 msvcr120.memset

	83??02??FF??????FF????85??75
	--*/

	DWORD dwHookDxInputModel = ScanAddr((HANDLE)-1,"83??02??FF??????FF????85??75",0);
	sprintf_s(szPrint,"[FIFA] DX模式:0x%p\r\n",dwHookDxInputModel);	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookDxInputModel = 0x%p;\r\n",dwHookDxInputModel);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_DX_INPUT_MODEL  ";
	LOG << dwHookDxInputModel;
	LOG_END;

	/*++
	窗口句柄
	007932CF    6A 00           PUSH 0x0
	007932D1    6A 00           PUSH 0x0
	007932D3    57              PUSH EDI
	007932D4    56              PUSH ESI
	007932D5    68 00000080     PUSH 0x80000000
	007932DA    68 00000080     PUSH 0x80000000
	007932DF    53              PUSH EBX
	007932E0    8D8D BCFEFFFF   LEA ECX,DWORD PTR SS:[EBP-0x144]
	007932E6    51              PUSH ECX
	007932E7    52              PUSH EDX
	007932E8    50              PUSH EAX
	007932E9    FF15 18492201   CALL DWORD PTR DS:[0x1224918]                    ; user32.CreateWindowExA
	007932EF    6A 01           PUSH 0x1

	6A006A0057566800000080680000008053

	$ ==>    00BC87EF        6A 00                PUSH 0x0
	$+2      00BC87F1        6A 00                PUSH 0x0
	$+4      00BC87F3        56                   PUSH ESI
	$+5      00BC87F4        57                   PUSH EDI
	$+6      00BC87F5        68 00000080          PUSH 0x80000000
	$+B      00BC87FA        68 00000080          PUSH 0x80000000
	$+10     00BC87FF        53                   PUSH EBX
	$+11     00BC8800        50                   PUSH EAX
	$+12     00BC8801        FF35 44EA7401        PUSH DWORD PTR DS:[0x174EA44]            ; fifazf.01571070
	$+18     00BC8807        FFB5 E4FEFFFF        PUSH DWORD PTR SS:[EBP-0x11C]
	$+1E     00BC880D        FF15 44294D01        CALL DWORD PTR DS:[0x14D2944]
	$+24     00BC8813        6A 01                PUSH 0x1
	$+26     00BC8815        50                   PUSH EAX
	$+27     00BC8816        A3 9CCD8801          MOV DWORD PTR DS:[0x188CD9C],EAX
	$+2C     00BC881B        FF15 B8294D01        CALL DWORD PTR DS:[0x14D29B8]



	6A006A00????68000000806800000080
	--*/

	DWORD dwHookForGainMainWndHandle = ScanAddr((HANDLE)-1,"6A006A00????68000000806800000080",0x24);
	sprintf_s(szPrint,"[FIFA] 窗口句柄:0x%p\r\n",dwHookForGainMainWndHandle);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const unsigned int g_pHookForGainMainWndHandle_ = 0x%p;\r\n",dwHookForGainMainWndHandle);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_FOR_GAIN_MAIN_WND_HANDLE  ";
	LOG << dwHookForGainMainWndHandle;
	LOG_END;

	/*++
	calloc
	00F17750    6A 0C           PUSH 0xC
	00F17752    6A 01           PUSH 0x1
	00F17754    FF15 B0034501   CALL DWORD PTR DS:[0x14503B0]                    ; msvcr100.calloc
	00F1775A    83C4 08         ADD ESP,0x8
	00F1775D    C3              RETN


	CC6A0C6A01FF15????????83C408
	--*/

	DWORD	dwCalloc = ScanImme((HANDLE)-1,"CC6A0C6A01FF15????????83C408",7);
	sprintf_s(szPrint,"[FIFA] calloc:0x%p\r\n",dwCalloc);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGCalloc(0x%p);\r\n",dwCalloc);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_CALLOC  ";
	LOG << dwCalloc;
	LOG_END;


	/*++
	malloc
	00F0EC67    2BC2            SUB EAX,EDX
	00F0EC69    8BD8            MOV EBX,EAX
	00F0EC6B    8D43 02         LEA EAX,DWORD PTR DS:[EBX+0x2]
	00F0EC6E    50              PUSH EAX
	00F0EC6F    FF15 A0034501   CALL DWORD PTR DS:[0x14503A0]            ; msvcr100.malloc
	00F0EC75    8BE8            MOV EBP,EAX
	00F0EC77    83C4 04         ADD ESP,0x4
	00F0EC7A    85ED            TEST EBP,EBP

	2BC28BD88D43??50FF15????????8BE883C40485ED
	--*/

	DWORD	dwMalloc = ScanImme((HANDLE)-1,"2BC28BD88D43??50FF15????????8BE883C40485ED",0x0a);
	sprintf_s(szPrint,"[FIFA] malloc:0x%p\r\n",dwMalloc);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGMalloc(0x%p);\r\n",dwMalloc);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_MALLOC  ";
	LOG << dwMalloc;
	LOG_END;

	/*++
	free
	00F021CA    8D9B 00000000   LEA EBX,DWORD PTR DS:[EBX]
	00F021D0    8B06            MOV EAX,DWORD PTR DS:[ESI]
	00F021D2    8B7E 04         MOV EDI,DWORD PTR DS:[ESI+0x4]
	00F021D5    85C0            TEST EAX,EAX
	00F021D7    74 10           JE SHORT fifazf.00F021E9
	00F021D9    50              PUSH EAX
	00F021DA    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F021E0    83C4 04         ADD ESP,0x4
	00F021E3    C706 00000000   MOV DWORD PTR DS:[ESI],0x0
	00F021E9    56              PUSH ESI
	00F021EA    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free

	8D9B000000008B068B7E??85C0????50FF15
	--*/

	DWORD	dwFree = ScanImme((HANDLE)-1,"8D9B000000008B068B7E??85C0????50FF15",0x12);
	sprintf_s(szPrint,"[FIFA] free:0x%p\r\n",dwFree);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const MemOper<Oper_Cdecl, MemOper_RetrieveAddr> g_pGFree(0x%p);\r\n",dwFree);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_MEMOPER_G_FREE  ";
	LOG << dwFree;
	LOG_END;

	/*++
	按键对象
	00792D27    E8 14883B00     CALL fifazf.00B4B540
	00792D2C    8B10            MOV EDX,DWORD PTR DS:[EAX]
	00792D2E    8B52 40         MOV EDX,DWORD PTR DS:[EDX+0x40]
	00792D31    8BCE            MOV ECX,ESI
	00792D33    C1E9 10         SHR ECX,0x10
	00792D36    0FBFC9          MOVSX ECX,CX
	00792D39    51              PUSH ECX
	00792D3A    0FBFCE          MOVSX ECX,SI
	00792D3D    51              PUSH ECX
	00792D3E    6A 00           PUSH 0x0
	00792D40    6A 01           PUSH 0x1
	00792D42    8BC8            MOV ECX,EAX
	00792D44    FFD2            CALL EDX

	E8????????8B108B52??8BCEC1E9??0FBFC9

	$ ==>    00BC8CD9        E8 12CBDBFF     CALL fifazf.009857F0
	$+5      00BC8CDE        8BCE            MOV ECX,ESI
	$+7      00BC8CE0        C1E9 10         SHR ECX,0x10
	$+A      00BC8CE3        0FBFC9          MOVSX ECX,CX
	$+D      00BC8CE6        8B10            MOV EDX,DWORD PTR DS:[EAX]
	$+F      00BC8CE8        51              PUSH ECX
	$+10     00BC8CE9        0FBFCE          MOVSX ECX,SI
	$+13     00BC8CEC        51              PUSH ECX
	$+14     00BC8CED        6A 00           PUSH 0x0
	$+16     00BC8CEF        6A 01           PUSH 0x1
	$+18     00BC8CF1        8BC8            MOV ECX,EAX
	$+1A     00BC8CF3        FF52 50         CALL DWORD PTR DS:[EDX+0x50]

	E8????????8B??C1????0F????8B????0F
	--*/

	DWORD	dwGetMouseKeyEventHandler = ScanCall((HANDLE)-1,"E8????????8B??C1????0F????8B????0F",1);
	sprintf_s(szPrint,"[FIFA] 按键对象:0x%p\r\n",dwGetMouseKeyEventHandler);
	OutputDebugStringA(szPrint);
	sprintf_s(szPrint,"[FIFA] const MemOper<Oper_Cdecl> g_pGetMouseKeyEventHandler(0x%p);\r\n",dwGetMouseKeyEventHandler);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER  ";
	LOG << dwGetMouseKeyEventHandler;
	LOG_END;

	/*++
	对象基址偏移1
	00BF064F    8BD9            MOV EBX,ECX
	00BF0651    8B73 24         MOV ESI,DWORD PTR DS:[EBX+0x24]
	00BF0654    E8 886EF9FF     CALL fifazf.00B874E1
	00BF0659  ^ 75 8B           JNZ SHORT fifazf.00BF05E6

	8BD98B73??E8
	--*/

	DWORD	dwOffset1 = ScanImme((HANDLE)-1,"8BD98B73??E8",4,1);
	sprintf_s(szPrint,"[FIFA] 对象基址偏移1:0x%p\r\n",dwOffset1);
	OutputDebugStringA(szPrint);


	/*++
	对象基址偏移2
	00B80831    8BEC            MOV EBP,ESP
	00B80833    8B49 34         MOV ECX,DWORD PTR DS:[ECX+0x34]
	00B80836    83EC 24         SUB ESP,0x24
	00B80839    56              PUSH ESI
	00B8083A    57              PUSH EDI
	00B8083B    33FF            XOR EDI,EDI
	00B8083D    3BCF            CMP ECX,EDI
	00B8083F    75 27           JNZ SHORT fifazf.00B80868
	00B80841    8B75 08         MOV ESI,DWORD PTR SS:[EBP+0x8]
	00B80844    57              PUSH EDI
	00B80845    C746 14 0F00000>MOV DWORD PTR DS:[ESI+0x14],0xF

	8BEC8B49??83EC??565733FF3BCF75??8B75??57C746
	--*/

	DWORD	dwOffset2 = ScanImme((HANDLE)-1,"8BEC8B49??83EC??565733FF3BCF75??8B75??57C746",4,1);
	sprintf_s(szPrint,"[FIFA] 对象基址偏移2:0x%p\r\n",dwOffset2);
	OutputDebugStringA(szPrint);


	/*++
	对象1偏移3
	00F100B1    83C4 04         ADD ESP,0x4
	00F100B4    899E D4860000   MOV DWORD PTR DS:[ESI+0x86D4],EBX
	00F100BA    899E C0860000   MOV DWORD PTR DS:[ESI+0x86C0],EBX
	00F100C0    899E C4860000   MOV DWORD PTR DS:[ESI+0x86C4],EBX
	00F100C6    899E D0860000   MOV DWORD PTR DS:[ESI+0x86D0],EBX


	83C404899E????????899E????????899E????????899E
	--*/

// 	DWORD	dwHttpSendCall_NeededObj1 = ScanImme((HANDLE)-1,"83C404899E????????899E????????899E????????899E",0x11);
// 	wsprintf(szPrint,"[FIFA] 对象1偏移3:0x%p\r\n",dwHttpSendCall_NeededObj1);
// 	OutputDebugString(szPrint);
// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj1[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj1);
// 	OutputDebugString(szPrint);
// 
// 	dwOffset1 = 0x0000000C;	// 此特征码是不确定的，存在于VMP中
// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_OFFSET1  ";
// 	LOG << dwOffset1;
// 	LOG_END;
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_OFFSET2  ";
// 	LOG << dwOffset2;
// 	LOG_END;
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_1_OFFSET  ";
// 	LOG << dwHttpSendCall_NeededObj1;
// 	LOG_END;

	/*++
	0x8688
	00F0B612    39BE 88860000   CMP DWORD PTR DS:[ESI+0x8688],EDI
	00F0B618    74 14           JE SHORT fifazf.00F0B62E
	00F0B61A    50              PUSH EAX
	00F0B61B    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F0B621    83C4 04         ADD ESP,0x4
	00F0B624    C786 84860000 0>MOV DWORD PTR DS:[ESI+0x8684],0x0
	00F0B62E    89BE 88860000   MOV DWORD PTR DS:[ESI+0x8688],EDI

	39BE????????74??50FF15????????83C404C786
	--*/

// 	DWORD	dwHttpSendCall_NeededObj2 = ScanImme((HANDLE)-1,"39BE????????74??50FF15????????83C404C786",2);
// 	wsprintf(szPrint,"[FIFA] 对象2偏移3:0x%p\r\n",dwHttpSendCall_NeededObj2);
// 	OutputDebugString(szPrint);
// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj2[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj2);
// 	OutputDebugString(szPrint);
// 	
// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_2_OFFSET  ";
// 	LOG << dwHttpSendCall_NeededObj2;
// 	LOG_END;


	/*++
	0x8684
	00F0B612    39BE 88860000   CMP DWORD PTR DS:[ESI+0x8688],EDI
	00F0B618    74 14           JE SHORT fifazf.00F0B62E
	00F0B61A    50              PUSH EAX
	00F0B61B    FF15 A4034501   CALL DWORD PTR DS:[0x14503A4]            ; msvcr100.free
	00F0B621    83C4 04         ADD ESP,0x4
	00F0B624    C786 84860000 0>MOV DWORD PTR DS:[ESI+0x8684],0x0
	00F0B62E    89BE 88860000   MOV DWORD PTR DS:[ESI+0x8688],EDI

	39BE????????74??50FF15????????83C404C786
	--*/
// 	DWORD	dwHttpSendCall_NeededObj3 = ScanImme((HANDLE)-1,"39BE????????74??50FF15????????83C404C786",0x14);
// 	wsprintf(szPrint,"[FIFA] 对象3偏移3:0x%p\r\n",dwHttpSendCall_NeededObj3);
// 	OutputDebugString(szPrint);
// 	wsprintf(szPrint,"[FIFA] const int g_o_HttpSendCall_NeededObj3[] = { 0x%p, 0x%p, 0x%p };\r\n",dwOffset1,dwOffset2,dwHttpSendCall_NeededObj3);
// 	OutputDebugString(szPrint);
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_HTTP_SEND_CALL_NEEDEDOBJ_3_OFFSET  ";
// 	LOG << dwHttpSendCall_NeededObj3;
// 	LOG_END;
	
	/*++
	SSL_read 偏移4
	00F2DE68    B8 FFFFFF7F            MOV EAX,0x7FFFFFFF
	00F2DE6D    8D0C76                 LEA ECX,DWORD PTR DS:[ESI+ESI*2]
	00F2DE70    8B94CB 60010000        MOV EDX,DWORD PTR DS:[EBX+ECX*8+0x160]
	00F2DE77    50                     PUSH EAX
	00F2DE78    8DBCCB 60010000        LEA EDI,DWORD PTR DS:[EBX+ECX*8+0x160]
	00F2DE7F    55                     PUSH EBP
	00F2DE80    52                     PUSH EDX
	00F2DE81    E8 6A800100            CALL fifazf.00F45EF0                     ; SSL_read
	00F2DE86    8BF0                   MOV ESI,EAX
	00F2DE88    83C4 0C                ADD ESP,0xC

	B8FFFFFF7F8D0C768B94CB????????50
	--*/

	DWORD	dwHttp_SSL_read_Addr4 = ScanImme((HANDLE)-1,"B8FFFFFF7F8D0C768B94CB????????50",11,2);

	/*++
	SSL_read 偏移3
	00F188C5   /75 71                  JNZ SHORT fifazf.00F18938
	00F188C7   |83BB C4010000 00       CMP DWORD PTR DS:[EBX+0x1C4],0x0
	00F188CE   |75 68                  JNZ SHORT fifazf.00F18938
	00F188D0   |85ED                   TEST EBP,EBP
	00F188D2   |75 64                  JNZ SHORT fifazf.00F18938
	00F188D4   |E8 A7F9FFFF            CALL fifazf.00F18280
	00F188D9   |85C0                   TEST EAX,EAX
	00F188DB   |74 40                  JE SHORT fifazf.00F1891D
	00F188DD   |899F 58050000          MOV DWORD PTR DS:[EDI+0x558],EBX
	00F188E3   |39AB D0010000          CMP DWORD PTR DS:[EBX+0x1D0],EBP
	00F188E9   |74 08                  JE SHORT fifazf.00F188F3
	00F188EB   |8B83 94000000          MOV EAX,DWORD PTR DS:[EBX+0x94]
	00F188F1   |EB 06                  JMP SHORT fifazf.00F188F9
	00F188F3   |8B83 84000000          MOV EAX,DWORD PTR DS:[EBX+0x84]
	00F188F9   |50                     PUSH EAX
	00F188FA   |8B43 34                MOV EAX,DWORD PTR DS:[EBX+0x34]

	E8????????85C074??899F????????39AB????????74
	
	--*/
	DWORD	dwHttp_SSL_read_Addr3 = ScanImme((HANDLE)-1,"E8????????85C074??899F????????39AB????????74",0x0b);
	sprintf_s(szPrint,"[FIFA] SSL_read 偏移1:0x%p,偏移2:0x%p,偏移3:0x%p,偏移4:0x%p\r\n",dwOffset1,dwOffset2,dwHttp_SSL_read_Addr3,dwHttp_SSL_read_Addr4);
	OutputDebugStringA(szPrint);

	
	/*++
	用于完全收包后，离开互斥体
	00F1B165   /74 38                  JE SHORT fifazf.00F1B19F
	00F1B167   |8B53 38                MOV EDX,DWORD PTR DS:[EBX+0x38]
	00F1B16A   |8B73 3C                MOV ESI,DWORD PTR DS:[EBX+0x3C]
	00F1B16D   |3BD0                   CMP EDX,EAX
	00F1B16F   |75 04                  JNZ SHORT fifazf.00F1B175
	00F1B171   |3BF1                   CMP ESI,ECX
	00F1B173   |74 2A                  JE SHORT fifazf.00F1B19F
	00F1B175   |83BB D4000000 00       CMP DWORD PTR DS:[EBX+0xD4],0x0
	00F1B17C   |75 21                  JNZ SHORT fifazf.00F1B19F
	...
	00F1B19F    83BB D4020000 00       CMP DWORD PTR DS:[EBX+0x2D4],0x0		// HOOK点
	00F1B1A6    75 2A                  JNZ SHORT fifazf.00F1B1D2
	00F1B1A8    83BB E0000000 00       CMP DWORD PTR DS:[EBX+0xE0],0x0
	00F1B1AF    74 21                  JE SHORT fifazf.00F1B1D2
	00F1B1B1    837F 1C 08             CMP DWORD PTR DS:[EDI+0x1C],0x8
	00F1B1B5    74 1B                  JE SHORT fifazf.00F1B1D2

	B8????????5B83C4??C383BB??????????75??83BB
	--*/

	DWORD g_dwAfterGameReadFullHttpPackage = ScanAddr((HANDLE)-1,"74??8B53??8B73??3BD075??3BF174??83BB",0x3A);
	sprintf_s(szPrint,"[FIFA] const unsigned int	g_pHookAfterGameReadFullHttpPackage = %p;\r\n",g_dwAfterGameReadFullHttpPackage);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_HOOK_AFTER_GAME_READ_FULL_HTTP_PACKAGE  ";
	LOG << g_dwAfterGameReadFullHttpPackage;
	LOG_END;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 优化代码 特征码

	OutputDebugStringA("[FIFA] **************游戏优化代码*****************************************************************");
	/*++
	//1. SLEEP 做事线程,让出时间片
	0047824E   /74 14           JE SHORT fifazf.00478264
	00478250   |E8 EB503100     CALL fifazf.0078D340
	00478255   |E8 F64D3100     CALL fifazf.0078D050
	0047825A   |E8 C1533100     CALL fifazf.0078D620
	0047825F   |E8 4C7DB900     CALL fifazf.0100FFB0

	7414E8????????E8????????E8????????E8
	--*/
// 	DWORD	g_dwHookDoDownCpu = ScanCall((HANDLE)-1,"7414E8????????E8????????E8????????E8",18);
// 	wsprintf(szPrint,"[FIFA] 做事线程,让出时间片:0x%p\r\n",g_dwHookDoDownCpu);
// 	OutputDebugString(szPrint);


	/*++
	// 降低主线程的CPU
	0078C0CD    8B81 E8BB5001   MOV EAX,DWORD PTR DS:[ECX+0x150BBE8]
	0078C0D3    40              INC EAX
	0078C0D4    33D2            XOR EDX,EDX
	0078C0D6    BE E8030000     MOV ESI,0x3E8
	0078C0DB    F7F6            DIV ESI
	0078C0DD    8991 E8BB5001   MOV DWORD PTR DS:[ECX+0x150BBE8],EDX
	0078C0E3    5E              POP ESI
	0078C0E4    8BE5            MOV ESP,EBP
	0078C0E6    5D              POP EBP
	0078C0E7    C3              RETN

	8B81????????4033D2BE????????F7F68991????????5E8BE5
	--*/

// 	DWORD	g_dwHookDoDownCpu1 = ScanAddr((HANDLE)-1,"8B81????????4033D2BE????????F7F68991????????5E8BE5",0x1a);
// 	wsprintf(szPrint,"[FIFA] 降低主线程的CPU:0x%p\r\n",g_dwHookDoDownCpu1);
// 	OutputDebugString(szPrint);

	/*++
	2. 优化渲染
	01115050    8B41 0C         MOV EAX,DWORD PTR DS:[ECX+0xC]
	01115053    8B08            MOV ECX,DWORD PTR DS:[EAX]
	01115055    8B51 44         MOV EDX,DWORD PTR DS:[ECX+0x44]
	01115058    6A 00           PUSH 0x0
	0111505A    6A 00           PUSH 0x0
	0111505C    6A 00           PUSH 0x0
	0111505E    6A 00           PUSH 0x0
	01115060    50              PUSH EAX
	01115061    FFD2            CALL EDX
	01115063    85C0            TEST EAX,EAX
	01115065    0F99C0          SETNS AL
	01115068    C2 0400         RETN 0x4
	0111506B    CC              INT3

	50FFD285C00F99C0C20400CC
	--*/
// 	DWORD g_dwHookPresent = ScanAddr((HANDLE)-1,"50FFD285C00F99C0C20400CC",11);
// 	wsprintf(szPrint,"[FIFA] 优化渲染:0x%p\r\n",g_dwHookPresent);
// 	OutputDebugString(szPrint);

	/*++
	// 降低渲染线程的CPU
	005705A9    FFD0            CALL EAX
	005705AB    8B16            MOV EDX,DWORD PTR DS:[ESI]
	005705AD    8B82 E4000000   MOV EAX,DWORD PTR DS:[EDX+0xE4]
	005705B3    8BCE            MOV ECX,ESI
	005705B5    FFD0            CALL EAX
	005705B7    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705B9    8BC8            MOV ECX,EAX
	005705BB    8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	005705BE    FFD0            CALL EAX
	005705C0    5F              POP EDI
	005705C1    5E              POP ESI
	005705C2    B0 01           MOV AL,0x1
	005705C4    5B              POP EBX
	005705C5    C3              RETN

	FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3
	--*/
// 	DWORD g_dwHookPresent1 = ScanAddr((HANDLE)-1,"FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3",0x17);
// 	wsprintf(szPrint,"[FIFA] 降低渲染线程的CPU:0x%p\r\n",g_dwHookPresent1);
// 	OutputDebugString(szPrint);


	/*++
	黑屏优化
	01120E50    8B4E 1C         MOV ECX,DWORD PTR DS:[ESI+0x1C]
	01120E53    8B0499          MOV EAX,DWORD PTR DS:[ECX+EBX*4]
	01120E56    8B48 14         MOV ECX,DWORD PTR DS:[EAX+0x14]
	01120E59    8B40 24         MOV EAX,DWORD PTR DS:[EAX+0x24]
	01120E5C    8B17            MOV EDX,DWORD PTR DS:[EDI]
	01120E5E    51              PUSH ECX
	01120E5F    8B8A 90010000   MOV ECX,DWORD PTR DS:[EDX+0x190]
	01120E65    6A 00           PUSH 0x0
	01120E67    50              PUSH EAX
	01120E68    53              PUSH EBX
	01120E69    57              PUSH EDI
	01120E6A    FFD1            CALL ECX

	8B4E??8B??998B48??8B40??8B17518B8A

	8B????8B????8B????8B????8B????8B??????????6A

	8B????8B??????????6A00??????FF
	--*/
	DWORD g_dwSetStreamSource = ScanAddr((HANDLE)-1,"8B4E??8B??998B48??8B40??8B17518B8A",/*14*/-0x70);
	sprintf_s(szPrint,"[FIFA] 黑屏优化:0x%p\r\n",g_dwSetStreamSource);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_SET_STREAM_SOURCE  ";
	LOG << g_dwSetStreamSource;
	LOG_END;

	/*++
	给恒平那边的优化地址
	005705A2    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705A4    8BC8            MOV ECX,EAX
	005705A6    8B42 0C         MOV EAX,DWORD PTR DS:[EDX+0xC]
	005705A9    FFD0            CALL EAX
	005705AB    8B16            MOV EDX,DWORD PTR DS:[ESI]
	005705AD    8B82 E4000000   MOV EAX,DWORD PTR DS:[EDX+0xE4]
	005705B3    8BCE            MOV ECX,ESI
	005705B5    FFD0            CALL EAX
	005705B7    8B10            MOV EDX,DWORD PTR DS:[EAX]
	005705B9    8BC8            MOV ECX,EAX
	005705BB    8B42 24         MOV EAX,DWORD PTR DS:[EDX+0x24]
	005705BE    FFD0            CALL EAX                                 ; 004C7E80
	005705C0    5F              POP EDI
	005705C1    5E              POP ESI
	005705C2    B0 01           MOV AL,0x1
	005705C4    5B              POP EBX
	005705C5    C3              RETN

	8B108BC88B42??FFD08B168B82????????8BCEFFD08B108BC88B42??FFD05F5EB0015BC3

	0055B13F                    8B16            MOV EDX,DWORD PTR DS:[ESI]
	0055B141                    8B42 68         MOV EAX,DWORD PTR DS:[EDX+0x68]
	0055B144                    8BCE            MOV ECX,ESI
	0055B146                    FFD0            CALL EAX
	0055B148                    8B10            MOV EDX,DWORD PTR DS:[EAX]
	0055B14A                    8BC8            MOV ECX,EAX
	0055B14C                    8B42 04         MOV EAX,DWORD PTR DS:[EDX+0x4]
	0055B14F                    FFD0            CALL EAX
	0055B151                    B0 01           MOV AL,0x1
	0055B153                    5E              POP ESI
	0055B154                    C3              RETN

	8B??8B????8B??FFD08B??8B??8B????FFD0B001??C3
	--*/
	DWORD g_dwSetStreamSource1 = ScanAddr((HANDLE)-1,"8B??8B????8B??FFD08B??8B??8B????FFD0B001??C3",-0x9F);		// 改到函数头，函数尾部zhp不知道为何HOOK不成功
	sprintf_s(szPrint,"[FIFA] zhp: CPU优化: g_pHookRenderThd_Sleep = 0x%p\r\n",g_dwSetStreamSource1);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_SET_STREAM_SOURCE_ZHP  ";
	LOG << g_dwSetStreamSource1;
	LOG_END;


	/*++
	主线程优化
	0078C0D3    40              INC EAX
	0078C0D4    33D2            XOR EDX,EDX
	0078C0D6    BE E8030000     MOV ESI,0x3E8
	0078C0DB    F7F6            DIV ESI
	0078C0DD    8991 E8BB5001   MOV DWORD PTR DS:[ECX+0x150BBE8],EDX
	0078C0E3    5E              POP ESI
	0078C0E4    8BE5            MOV ESP,EBP
	0078C0E6    5D              POP EBP
	0078C0E7    C3              RETN

	4033D2BE????????F7F68991????????5E8BE55DC3

	--*/
	DWORD g_dwSetStreamSource2 = ScanAddr((HANDLE)-1,"4033D2BE????????F7F68991????????5E8BE55DC3",0x14);
	sprintf_s(szPrint,"[FIFA] 主线程优化:0x%p\r\n",g_dwSetStreamSource2);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_MAIL_THREAD_OPTIMIZE  ";
	LOG << g_dwSetStreamSource2;
	LOG_END;

	/*++

	62A9EA32    8BFF            MOV EDI,EDI
	...
	62A9EA50    8A03            MOV AL,BYTE PTR DS:[EBX]
	62A9EA52    FF4D 08         DEC DWORD PTR SS:[EBP+0x8]
	62A9EA55    8BCF            MOV ECX,EDI
	62A9EA57    E8 7DFFFFFF     CALL fifazfBa.62A9E9D9
	62A9EA5C    43              INC EBX
	62A9EA5D    833E FF         CMP DWORD PTR DS:[ESI],-0x1
	62A9EA60    75 13           JNZ SHORT fifazfBa.62A9EA75


	8A03FF4D??8BCFE8????????43833E
	--*/
	DWORD dwFifazfBaseAddr = (DWORD)LoadLibraryA("fifazfBase.dll");
	assert(dwFifazfBaseAddr);
	DWORD dwFifazfBaseSprintfAddr = ScanAddr((HANDLE)-1,"8A03FF4D??8BCFE8????????43833E",-0x1E,4,1,dwFifazfBaseAddr,dwFifazfBaseAddr+0x000B36000)-dwFifazfBaseAddr;
	sprintf_s(szPrint,"[FIFA] dwFifazfBaseSprintfAddr = 0x%p\r\n",dwFifazfBaseSprintfAddr);
	OutputDebugStringA(szPrint);

	LOG << "#define UPDATES_FIFA_REVERSAL_FIFAZFBASE_SPRINTF_ADDR  ";
	LOG << dwFifazfBaseSprintfAddr;
	LOG_END;


	/*++
	$ ==>    00830E27        8B40 04         MOV EAX,DWORD PTR DS:[EAX+0x4]
	$+3      00830E2A        8B73 08         MOV ESI,DWORD PTR DS:[EBX+0x8]
	$+6      00830E2D        8BCF            MOV ECX,EDI
	$+8      00830E2F        8946 04         MOV DWORD PTR DS:[ESI+0x4],EAX
	$+B      00830E32        E8 B9056500     CALL fifazf.00E813F0
	$+10     00830E37        8B48 04         MOV ECX,DWORD PTR DS:[EAX+0x4]
	$+13     00830E3A        894E 1C         MOV DWORD PTR DS:[ESI+0x1C],ECX
	$+16     00830E3D        D940 08         FLD DWORD PTR DS:[EAX+0x8]
	$+19     00830E40        8BCF            MOV ECX,EDI                              ; hook 取ecx
	$+1B     00830E42        D95E 20         FSTP DWORD PTR DS:[ESI+0x20]
	$+1E     00830E45        E8 C6C10D00     CALL fifazf.0090D010
	$+23     00830E4A        E8 01BDDCFF     CALL fifazf.005FCB50

	8B????8B????8B??89????E8????????8B????89????D9
	--*/
// 	DWORD dwFifaRevampMatchTimeHook = ScanAddr((HANDLE)-1,"8B????8B????8B??89????E8????????8B????89????D9",0x19);
// 	assert(dwFifaRevampMatchTimeHook);
// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_HOOK  ";
// 	LOG << dwFifaRevampMatchTimeHook;
// 	LOG_END;
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET1___  ";
// 	LOG << 0x44;
// 	LOG_END;
// 
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET2___  ";
// 	LOG << 0x50;
// 	LOG_END;
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET3___  ";
// 	LOG << 0x5C;
// 	LOG_END;
// 
// 	LOG << "#define UPDATES_FIFA_REVERSAL_REVAMP_MATCH_TIME_OFFSET4___  ";
// 	LOG << 0x68;
// 	LOG_END;

	/*++
	$-A      >  53                   PUSH EBX
	$-9      >  56                   PUSH ESI
	$-8      >  57                   PUSH EDI
	$-7      >  8BF9                 MOV EDI,ECX
	$-5      >  B9 B0047C01          MOV ECX,fifazf.017C04B0
	$ ==>    >  E8 913D0000          CALL fifazf.00669D60
	$+5      >  68 707B4D01          PUSH fifazf.014D7B70
	$+A      >  8D8F 980E0000        LEA ECX,DWORD PTR DS:[EDI+0xE98]
	$+10     >  E8 F1510200          CALL fifazf.0068B1D0
	$+15     >  8A9F 200F0000        MOV BL,BYTE PTR DS:[EDI+0xF20]
	$+1B     >  8D8F 980E0000        LEA ECX,DWORD PTR DS:[EDI+0xE98]
	$+21     >  E8 B05B0200          CALL fifazf.0068BBA0
	$+26     >  84DB                 TEST BL,BL
	$+28     >  74 10                JE SHORT fifazf.00666004
	$+2A     >  80BF 710D0000 00     CMP BYTE PTR DS:[EDI+0xD71],0x0
	$+31     >  75 07                JNZ SHORT fifazf.00666004
	$+33     >  C687 710D0000 01     MOV BYTE PTR DS:[EDI+0xD71],0x1
	$+3A     >  6A 00                PUSH 0x0
	$+3C     >  E8 75A7F7FF          CALL fifazf.005E0780
	$+41     >  83C4 04              ADD ESP,0x4
	$+44     >  E8 CD435600          CALL fifazf.00BCA3E0
	$+49     >  8BC8                 MOV ECX,EAX
	$+4B     >  E8 76485600          CALL fifazf.00BCA890
	$+50     >  68 707B4D01          PUSH fifazf.014D7B70
	$+55     >  B9 C0047C01          MOV ECX,fifazf.017C04C0
	$+5A     >  E8 A7510200          CALL fifazf.0068B1D0
	$+5F     >  8A1D 48057C01        MOV BL,BYTE PTR DS:[0x17C0548]
	$+65     >  B9 C0047C01          MOV ECX,fifazf.017C04C0
	$+6A     >  E8 675B0200          CALL fifazf.0068BBA0

	E8????????68????????8D??????????E8????????8A??????????8D
	--*/
	DWORD dwFifaStopRenderAndSleepHook = ScanAddr((HANDLE)-1,"E8????????68????????8D??????????E8????????8A??????????8D",-0xA);
	assert(dwFifaStopRenderAndSleepHook);
	LOG << "#define UPDATES_FIFA_REVERSAL_STOP_RENDER_AND_SLEEP_HOOK  ";
	LOG << dwFifaStopRenderAndSleepHook;
	LOG_END;

	LOG << "//**********************基址，偏移更新完毕!**********************//";
	LOG_END;

	return TRUE;
}
