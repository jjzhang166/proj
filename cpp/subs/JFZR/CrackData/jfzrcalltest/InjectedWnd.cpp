
// InjectedWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "jfzrcalltest.h"
#include "InjectedWnd.h"
#include "BTTraverse.h"

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
	ON_BN_CLICKED(IDCLOSE, &CInjectedWnd::OnClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectedWnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInjectedWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CInjectedWnd::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CInjectedWnd::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CInjectedWnd::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CInjectedWnd::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CInjectedWnd::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CInjectedWnd::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CInjectedWnd::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CInjectedWnd::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CInjectedWnd::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CInjectedWnd::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CInjectedWnd::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CInjectedWnd::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CInjectedWnd::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CInjectedWnd::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CInjectedWnd::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CInjectedWnd::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CInjectedWnd::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CInjectedWnd::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CInjectedWnd::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CInjectedWnd::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &CInjectedWnd::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON24, &CInjectedWnd::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON25, &CInjectedWnd::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON26, &CInjectedWnd::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON27, &CInjectedWnd::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON28, &CInjectedWnd::OnBnClickedButton28)
	ON_BN_CLICKED(IDC_BUTTON29, &CInjectedWnd::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON30, &CInjectedWnd::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON31, &CInjectedWnd::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &CInjectedWnd::OnBnClickedButton32)
	ON_BN_CLICKED(IDC_BUTTON33, &CInjectedWnd::OnBnClickedButton33)
	ON_BN_CLICKED(IDC_BUTTON34, &CInjectedWnd::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON35, &CInjectedWnd::OnBnClickedButton35)
	ON_BN_CLICKED(IDC_BUTTON36, &CInjectedWnd::OnBnClickedButton36)
	ON_BN_CLICKED(IDC_BUTTON37, &CInjectedWnd::OnBnClickedButton37)
	ON_BN_CLICKED(IDC_BUTTON38, &CInjectedWnd::OnBnClickedButton38)
	ON_BN_CLICKED(IDC_BUTTON39, &CInjectedWnd::OnBnClickedButton39)
	ON_BN_CLICKED(IDC_BUTTON40, &CInjectedWnd::OnBnClickedButton40)
	ON_BN_CLICKED(IDC_BUTTON41, &CInjectedWnd::OnBnClickedButton41)
	ON_BN_CLICKED(IDC_BUTTON42, &CInjectedWnd::OnBnClickedButton42)
	ON_BN_CLICKED(IDC_BUTTON43, &CInjectedWnd::OnBnClickedButton43)
	ON_BN_CLICKED(IDC_BUTTON44, &CInjectedWnd::OnBnClickedButton44)
	ON_BN_CLICKED(IDC_BUTTON45, &CInjectedWnd::OnBnClickedButton45)
	ON_BN_CLICKED(IDC_BUTTON46, &CInjectedWnd::OnBnClickedButton46)
	ON_BN_CLICKED(IDC_BUTTON47, &CInjectedWnd::OnBnClickedButton47)
	ON_BN_CLICKED(IDC_BUTTON48, &CInjectedWnd::OnBnClickedButton48)
	ON_BN_CLICKED(IDC_BUTTON49, &CInjectedWnd::OnBnClickedButton49)
	ON_BN_CLICKED(IDC_BUTTON50, &CInjectedWnd::OnBnClickedButton50)
	ON_BN_CLICKED(IDC_BUTTON51, &CInjectedWnd::OnBnClickedButton51)
	ON_BN_CLICKED(IDC_BUTTON52, &CInjectedWnd::OnBnClickedButton52)
	ON_BN_CLICKED(IDC_BUTTON53, &CInjectedWnd::OnBnClickedButton53)
	ON_BN_CLICKED(IDC_BUTTON54, &CInjectedWnd::OnBnClickedButton54)
	ON_BN_CLICKED(IDC_BUTTON55, &CInjectedWnd::OnBnClickedButton55)
	ON_BN_CLICKED(IDC_BUTTON56, &CInjectedWnd::OnBnClickedButton56)
	ON_BN_CLICKED(IDC_BUTTON57, &CInjectedWnd::OnBnClickedButton57)
	ON_BN_CLICKED(IDC_BUTTON58, &CInjectedWnd::OnBnClickedButton58)
	ON_BN_CLICKED(IDC_BUTTON59, &CInjectedWnd::OnBnClickedButton59)
	ON_BN_CLICKED(IDC_BUTTON60, &CInjectedWnd::OnBnClickedButton60)
	ON_BN_CLICKED(IDC_BUTTON61, &CInjectedWnd::OnBnClickedButton61)
	ON_BN_CLICKED(IDC_BUTTON62, &CInjectedWnd::OnBnClickedButton62)
	ON_BN_CLICKED(IDC_BUTTON63, &CInjectedWnd::OnBnClickedButton63)
	ON_BN_CLICKED(IDC_BUTTON64, &CInjectedWnd::OnBnClickedButton64)
	ON_BN_CLICKED(IDC_BUTTON65, &CInjectedWnd::OnBnClickedButton65)
	ON_BN_CLICKED(IDC_BUTTON66, &CInjectedWnd::OnBnClickedButton66)
	ON_BN_CLICKED(IDC_BUTTON67, &CInjectedWnd::OnBnClickedButton67)
	ON_BN_CLICKED(IDC_BUTTON68, &CInjectedWnd::OnBnClickedButton68)
	ON_BN_CLICKED(IDC_BUTTON69, &CInjectedWnd::OnBnClickedButton69)
	ON_BN_CLICKED(IDC_BUTTON70, &CInjectedWnd::OnBnClickedButton70)
	ON_BN_CLICKED(IDC_BUTTON71, &CInjectedWnd::OnBnClickedButton71)
	ON_BN_CLICKED(IDC_BUTTON72, &CInjectedWnd::OnBnClickedButton72)
	ON_BN_CLICKED(IDC_BUTTON73, &CInjectedWnd::OnBnClickedButton73)
	ON_BN_CLICKED(IDC_BUTTON74, &CInjectedWnd::OnBnClickedButton74)
	ON_BN_CLICKED(IDC_BUTTON75, &CInjectedWnd::OnBnClickedButton75)
	ON_BN_CLICKED(IDC_BUTTON76, &CInjectedWnd::OnBnClickedButton76)
	ON_BN_CLICKED(IDC_BUTTON77, &CInjectedWnd::OnBnClickedButton77)
	ON_BN_CLICKED(IDC_BUTTON78, &CInjectedWnd::OnBnClickedButton78)
	ON_BN_CLICKED(IDC_BUTTON79, &CInjectedWnd::OnBnClickedButton79)
	ON_BN_CLICKED(IDC_BUTTON80, &CInjectedWnd::OnBnClickedButton80)
	ON_BN_CLICKED(IDC_BUTTON81, &CInjectedWnd::OnBnClickedButton81)
	ON_BN_CLICKED(IDC_BUTTON82, &CInjectedWnd::OnBnClickedButton82)
	ON_BN_CLICKED(IDC_BUTTON83, &CInjectedWnd::OnBnClickedButton83)
	ON_BN_CLICKED(IDC_BUTTON84, &CInjectedWnd::OnBnClickedButton84)
	ON_BN_CLICKED(IDC_BUTTON85, &CInjectedWnd::OnBnClickedButton85)
	ON_BN_CLICKED(IDC_BUTTON86, &CInjectedWnd::OnBnClickedButton86)
	ON_BN_CLICKED(IDC_BUTTON87, &CInjectedWnd::OnBnClickedButton87)
	ON_BN_CLICKED(IDC_BUTTON88, &CInjectedWnd::OnBnClickedButton88)
	ON_BN_CLICKED(IDC_BUTTON89, &CInjectedWnd::OnBnClickedButton89)
	ON_BN_CLICKED(IDC_BUTTON90, &CInjectedWnd::OnBnClickedButton90)
	ON_BN_CLICKED(IDC_BUTTON91, &CInjectedWnd::OnBnClickedButton91)
	ON_BN_CLICKED(IDC_BUTTON92, &CInjectedWnd::OnBnClickedButton92)
	ON_BN_CLICKED(IDC_BUTTON93, &CInjectedWnd::OnBnClickedButton93)
	ON_BN_CLICKED(IDC_BUTTON94, &CInjectedWnd::OnBnClickedButton94)
	ON_BN_CLICKED(IDC_BUTTON95, &CInjectedWnd::OnBnClickedButton95)
	ON_BN_CLICKED(IDC_BUTTON96, &CInjectedWnd::OnBnClickedButton96)
	ON_BN_CLICKED(IDC_BUTTON97, &CInjectedWnd::OnBnClickedButton97)
	ON_BN_CLICKED(IDC_BUTTON98, &CInjectedWnd::OnBnClickedButton98)
	ON_BN_CLICKED(IDC_BUTTON99, &CInjectedWnd::OnBnClickedButton99)
	ON_BN_CLICKED(IDC_BUTTON100, &CInjectedWnd::OnBnClickedButton100)
	ON_BN_CLICKED(IDC_BUTTON101, &CInjectedWnd::OnBnClickedButton101)
	ON_BN_CLICKED(IDC_BUTTON102, &CInjectedWnd::OnBnClickedButton102)
	ON_BN_CLICKED(IDC_BUTTON103, &CInjectedWnd::OnBnClickedButton103)
	ON_BN_CLICKED(IDC_BUTTON104, &CInjectedWnd::OnBnClickedButton104)
	ON_BN_CLICKED(IDC_BUTTON105, &CInjectedWnd::OnBnClickedButton105)
	ON_BN_CLICKED(IDC_BUTTON106, &CInjectedWnd::OnBnClickedButton106)
	ON_BN_CLICKED(IDC_BUTTON107, &CInjectedWnd::OnBnClickedButton107)
	ON_BN_CLICKED(IDC_BUTTON108, &CInjectedWnd::OnBnClickedButton108)
	ON_BN_CLICKED(IDC_BUTTON109, &CInjectedWnd::OnBnClickedButton109)
	END_MESSAGE_MAP()


// CInjectedWnd 消息处理程序
void CInjectedWnd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CInjectedWnd::OnClose()
{
	MessageBox("你忘恢复游戏环境了么？");
}


void KeyDown(DWORD	dwVk)
{
	static HWND hWnd = NULL;

	if( hWnd == NULL )
	{
		FINDWINDOW	theWnd;
		memset(&theWnd,0,sizeof(theWnd));
		strcpy_s(theWnd.str_szExeFile,"JFZR.exe");
		strcpy_s(theWnd.str_szClassName,"JFZR");

		EnumPidWnd theEnumPidWnd;
		hWnd = theEnumPidWnd.FindWindow(&theWnd);
	}

	LONG pUserData = GetWindowLong(hWnd,GWL_USERDATA);
	assert(pUserData);

	if( pUserData != 0 )
	{
		pUserData += JFZR_REVERSAL_WND_PROC_OFFSET1___;


		LPARAM lParam = 1;
		lParam += MapVirtualKey(dwVk, MAPVK_VK_TO_VSC) << 16;

		__asm
		{
			mov eax, lParam
			push eax
			mov eax, dwVk
			push eax
			push WM_KEYDOWN
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}
	}
}


void KeyUp(DWORD dwVk)
{
	static HWND hWnd = NULL;

	if( hWnd == NULL )
	{
		FINDWINDOW	theWnd;
		memset(&theWnd,0,sizeof(theWnd));
		strcpy_s(theWnd.str_szExeFile,"JFZR.exe");
		strcpy_s(theWnd.str_szClassName,"JFZR");

		EnumPidWnd theEnumPidWnd;
		hWnd = theEnumPidWnd.FindWindow(&theWnd);
	}

	LONG pUserData = GetWindowLong(hWnd,GWL_USERDATA);
	assert(pUserData);

	if( pUserData != 0 )
	{
		pUserData += JFZR_REVERSAL_WND_PROC_OFFSET1___;


		LPARAM lParam = 1;
		lParam += MapVirtualKey(dwVk, MAPVK_VK_TO_VSC) << 16;

		lParam += 1 << 30;
		lParam += 1 << 31;

		__asm
		{
			mov eax, lParam
			push eax
			mov eax, dwVk
			push eax
			push WM_KEYUP
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}
	}
}

void KeyPress(DWORD	dwVk)
{
	static HWND hWnd = NULL;

	if( hWnd == NULL )
	{
		FINDWINDOW	theWnd;
		memset(&theWnd,0,sizeof(theWnd));
		strcpy_s(theWnd.str_szExeFile,"JFZR.exe");
		strcpy_s(theWnd.str_szClassName,"JFZR");

		EnumPidWnd theEnumPidWnd;
		hWnd = theEnumPidWnd.FindWindow(&theWnd);
	}

	LONG pUserData = GetWindowLong(hWnd,GWL_USERDATA);
	assert(pUserData);

	if( pUserData != 0 )
	{
		pUserData += JFZR_REVERSAL_WND_PROC_OFFSET1___;


		LPARAM lParam = 1;
		lParam += MapVirtualKey(dwVk, MAPVK_VK_TO_VSC) << 16;

		__asm
		{
			mov eax, lParam
			push eax
			mov eax, dwVk
			push eax
			push WM_KEYDOWN
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}

		lParam += 1 << 30;
		lParam += 1 << 31;

		Sleep(100);

		__asm
		{
			mov eax, lParam
			push eax
			mov eax, dwVk
			push eax
			push WM_KEYUP
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}
	}
}

void mouse(int x, int y)
{
	static HWND hWnd = NULL;

	if( hWnd == NULL )
	{
		FINDWINDOW	theWnd;
		memset(&theWnd,0,sizeof(theWnd));
		strcpy_s(theWnd.str_szExeFile,"JFZR.exe");
		strcpy_s(theWnd.str_szClassName,"JFZR");

		EnumPidWnd theEnumPidWnd;
		hWnd = theEnumPidWnd.FindWindow(&theWnd);
	}

	LONG pUserData = GetWindowLong(hWnd,GWL_USERDATA);
	assert(pUserData);

	if( pUserData != 0 )
	{
		pUserData += JFZR_REVERSAL_WND_PROC_OFFSET1___;

		LONG lxy = MAKELPARAM(x,y);

		__asm
		{
			mov eax, lxy
			push eax
			push 1
			push WM_LBUTTONDOWN
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}

		Sleep(100);

		__asm
		{
			mov eax, lxy
			push eax
			push 0
			PUSH WM_LBUTTONUP
			mov ecx, pUserData
			mov eax, JFZR_REVERSAL_WND_PROC_CALL
			call eax
		}
	}
}

/*++
$ ==>    261B62A4    0137A700  JFZR.0137A700
$+4      261B62A8    1C042FA4  UNICODE "SpinClaw"
$+8      261B62AC    00000000
$+C      261B62B0    00000000
$+10     261B62B4    00000000
$+14     261B62B8    00000008
$+18     261B62BC    0000000F		// 技能名称长度
$+1C     261B62C0    00000000
$+20     261B62C4    00000015		// 技能等级
$+24     261B62C8    00000000
$+28     261B62CC    00000000
$+2C     261B62D0    00000000
$+30     261B62D4    00000000
$+34     261B62D8    00000000
$+38     261B62DC    000AE351
$+3C     261B62E0    00000000
$+40     261B62E4    00000000
$+44     261B62E8    3C21F361		// 技能ID
--*/
void CInjectedWnd::OnBnClickedButton1()
{
	JFZRTHIEFSkill theThiefSkill;

	theThiefSkill.TraverseSkill();

	SetDlgItemText(IDC_EDIT_DATA,theThiefSkill.csTxt);
}

void CInjectedWnd::OnBnClickedButton2()
{
	theMonsterObj.TraverseObj(true);
	SetDlgItemText(IDC_EDIT_DATA,g_csTraverseObjTxt);
}


int SkillHitPacket( const char* pszSayBufffer, char* pOutszSendBuffer)
{
	DWORD dwTemp = 0;
	int nLen = 0x97;
	*(WORD*)pOutszSendBuffer = nLen;
	*(WORD*)(pOutszSendBuffer+2) = 0x55e0;
	*(WORD*)(pOutszSendBuffer+4) = 0x0F816;			// 技能攻击
	*(WORD*)(pOutszSendBuffer+6) = 0x83c3;			// RequestHitInfos
	*(WORD*)(pOutszSendBuffer+8) = 0x393C;			// array
	*(DWORD*)(pOutszSendBuffer+10) = 1;				// size
	*(WORD*)(pOutszSendBuffer+14) = 0x5362;			// item_0
	*(WORD*)(pOutszSendBuffer+16) = 0x5708;			// Apple::HitInfo

	*(DWORD*)(pOutszSendBuffer+18) = 0x0D2F6CC0;				// ProjectileSerial
	*(DWORD*)(pOutszSendBuffer+22) = 0x000A109E;				// DefenderSerial
	*(DWORD*)(pOutszSendBuffer+26) = 0x000A0D2F;				// AttackerSerial

	*(WORD*)(pOutszSendBuffer+30) = 0x0B;
	 memcpy(pOutszSendBuffer+32,L"HIT_TARGET",0x0B*2);

	*(DWORD*)(pOutszSendBuffer+54) = 1;				// HitType
	*(DWORD*)(pOutszSendBuffer+58) = -1;			// DeltaHp
	*(DWORD*)(pOutszSendBuffer+62) = 0;				// RandomSeed
	*(WORD*)(pOutszSendBuffer+66) = 0x0f90e;		// HitProjectileInfo
	*(WORD*)(pOutszSendBuffer+68) = 0x09D72;		// Apple::HitInfo
	*(FLOAT*)(pOutszSendBuffer+70) = 5.039999f;		// AttackPowerRate
	*(DWORD*)(pOutszSendBuffer+74) = 0;				// DamagePlusRateToSuperArmor
	*(DWORD*)(pOutszSendBuffer+78) = 0;				// DamagePlusRateForCounter
	
	*(WORD*)(pOutszSendBuffer+82) = 0x06;
	memcpy(pOutszSendBuffer+84,L"SLASH",0x06*2);	// PhysicalAttribute
	*(WORD*)(pOutszSendBuffer+96) = 0x05;
	memcpy(pOutszSendBuffer+98,L"NONE",0x05*2);		// ElementalAttribute

	*(DWORD*)(pOutszSendBuffer+108) = 0;			// EventOptionSerial
	*(DWORD*)(pOutszSendBuffer+112) = 0;			// EventOptionDamageReduceRate
	*(BYTE*)(pOutszSendBuffer+116) = 0;				// IsCounterAttack
	*(BYTE*)(pOutszSendBuffer+117) = 0;				// IsHitSuperArmor

	*(DWORD*)(pOutszSendBuffer+118) = 0x0FD70B9B4;	// ProjectileHash
	*(BYTE*)(pOutszSendBuffer+122) = 0;				// FirstHit
	*(DWORD*)(pOutszSendBuffer+123) = 0x0FD17221E;	// ActionHash
	*(DWORD*)(pOutszSendBuffer+127) = 0x0FD70B9B4;	// SkillHash
	*(DWORD*)(pOutszSendBuffer+131) = 0x0D;			// SkillLevel

	*(FLOAT*)(pOutszSendBuffer+135) = 1.0f;			// ProjectileScale
	*(FLOAT*)(pOutszSendBuffer+139) = 1.0f;
	*(FLOAT*)(pOutszSendBuffer+143) = 1.0f;

	*(DWORD*)(pOutszSendBuffer+147) = 0;			// TimeStamp

	return (int)*(WORD*)pOutszSendBuffer;
}

void CInjectedWnd::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	//theActionSystem.StartStage(L"Ep02Stage02",L"StageStartWarpGate01",1,0);
	//theActionSystem.StartStage(L"RgEvent_Warriortutorial",L"",1,0);	// Stage.RgEvent_Warriortutorial.RgEvent_Warriortutorial.Name >> 勇武之校场
	//theActionSystem.StartStage(L"RgEvent_Magicgunnertutorial",L"",1,0);	// Stage.RgEvent_Magicgunnertutorial.RgEvent_Magicgunnertutorial.Name >> 奥法之校场
	//theActionSystem.StartStage(L"RgEvent_Thieftutorial",L"",1,0);	// Stage.RgEvent_Thieftutorial.RgEvent_Thieftutorial.Name >> 斗志之校场
	theActionSystem.StartStage(L"Ep02Stage01",L"",1,0);
}

void CInjectedWnd::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	extern DWORD g_dwItemStatusValue;
	if( g_dwItemStatusValue	 == 0 )
	{
		assert(false);
		return ;
	}
	try
	{
		GetTimeStampAddr();
	}
	catch (...)
	{
		assert(false);
	}


// 	extern void WhileNpcActionSyncPacket();
// 	WhileNpcActionSyncPacket();Sleep(3000);


	JFZRActionSystem theActionSystem;
	try
	{
		::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("正在清理怪物..."));
		while ( true )
		{
			if( theActionSystem.useskill("HandShot",0,0) /*|| theActionSystem.useskill2("HandShot",0,0)*/  )
			{
				::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("攻击怪物完毕..."));
				Sleep(50);
				break;
			}
			else 
			{
				::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("怪物已经被清理完..."));
				break;
			}
			//break;
		}
	
	}
	catch (...)
	{
		assert(false);
	}

	

	

// 	while ( theActionSystem.useskill("HandShot",0,0) )
// 	{
// 		Sleep(100);
// 	}

// 	for(int i=0; i<100; i++)
// 	{
// 		if( false == theActionSystem.useskill("HandShot",0,0) )
// 			break;
// 		Sleep(10);
// 	}

	//theActionSystem.useskill("HalfMoonKick");
	//theActionSystem.useskill("ShoveStrike");
}

void CInjectedWnd::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRRole theRole;
	theRole.UpData();
	SetDlgItemText(IDC_EDIT_DATA,theRole.csTxt);
// 
// 	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
// 	memset(&theObjStruct,0,sizeof(theObjStruct));
// 	char szTypeCmd[32] = "MISC";
// 	memcpy(theObjStruct.str_szType,szTypeCmd,strlen(szTypeCmd));
// 	JFZRMonsterObj theMonsterObj;
// 	theMonsterObj.TraverseObj();
// 
// 	SetDlgItemText(IDC_EDIT_DATA,theMonsterObj.csTxt);
// 
// 	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,1);
// 
// 	CString csTxt;
// 	for (int i=0; i<theObjStruct.str_nCount; i++)
// 	{
// 		char szPrint[256] = {0};
// 		sprintf_s(szPrint,"job name = %s  obj name = %s  type name = %s  serial = %p x = %f y = %f z = %f\r\n",
// 									theObjStruct.str_theMonsterObj[i].str_szJobName,
// 									theObjStruct.str_theMonsterObj[i].str_szObjName,
// 									theObjStruct.str_theMonsterObj[i].str_szObjType,
// 									theObjStruct.str_theMonsterObj[i].str_dwSerial,
// 									theObjStruct.str_theMonsterObj[i].str_fx,
// 									theObjStruct.str_theMonsterObj[i].str_fy,
// 									theObjStruct.str_theMonsterObj[i].str_fz);
// 
// 		csTxt += szPrint;
// 	}
// 
// 	SetDlgItemText(IDC_EDIT_DATA,csTxt);

	// 测试进入下一个房间
// 	MessageBox("");
// 	JFZRActionSystem theActionSystem;
// 	theActionSystem.GotoNextRoom(L"Warp01");
}

void CInjectedWnd::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RetCity();
}

extern CString g_csWarpGate;
void CInjectedWnd::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRRoom theRoom;
	ROOM theCurRoom;
	theRoom.UpDataRoom(&theCurRoom);

	SetDlgItemText(IDC_EDIT_DATA,g_csWarpGate);
}

void CInjectedWnd::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码

	// 服务器上人物需要站在物品附件
	//KeyPress(VK_W);

	JFZRActionSystem theActionSystem;
	theActionSystem.PickUpItem();
}

bool	g_bStopDoFb = false;
void CInjectedWnd::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRRoom theRoom;
	//theRoom.DoFb(L"Ep01Stage03",1,L"Sector05");
	theRoom.DoFb(L"Ep05Stage05",4,L"Sector07");
}

void CInjectedWnd::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bStopDoFb = true;
}

void CInjectedWnd::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bStopDoFb = false;
	JFZRActionSystem theActionSystem;
	theActionSystem.StartStage(L"Ep02Stage02",L"",1,1);		// 给空字符串就是再次挑战的意思且bIsRestart = 1
}

void CInjectedWnd::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	// 修理装备
// 	JFZRActionSystem theActionSystem;
// 	theActionSystem.RepairItem();

	JFZRItem theItem;
	theItem.TraverseItem2(1);
	theItem.TraverseItem2(2);
	theItem.TraverseItem2(3);
	theItem.TraverseItem2(4);
	theItem.TraverseItem2(5);
	theItem.TraverseItem2(6);
	theItem.TraverseItem2(7);
	SetDlgItemText(IDC_EDIT_DATA,g_csItemPrintStr);
}

void CInjectedWnd::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;

	JFZRRole theRole;
	theRole.UpData();

	FLOAT x = 96.651718f;
	FLOAT y = 96.021019f;
	FLOAT z = theRole.m_fz;

	theActionSystem.ActionSyncPacket(L"FastRun",x,y,z);
	Sleep(10);
	theActionSystem.ActionSyncPacket(L"Stand",x+1,y,z);

// 	FLOAT x = 175.318497f;
// 	FLOAT y = 106.522400f;
// 	FLOAT z = -0.310000f;
// 	theActionSystem.VillageActionSyncPacket(L"FastRun",x,y,z);
// 	theActionSystem.VillageActionSyncPacket(L"Stand",x+1,y,z);
}

void CInjectedWnd::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码

	UpDataMap();
	SetDlgItemText(IDC_EDIT_DATA,g_csPrintMapData);
}

void CInjectedWnd::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.EquipItem(0,0);
}

void CInjectedWnd::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.UnEquipItem(0,0);
}

void CInjectedWnd::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRItem theItem;
	theItem.TraverseItem(6);
	theItem.TraverseItem(7);
	SetDlgItemText(IDC_EDIT_DATA,theItem.csTxt);
}

void CInjectedWnd::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.UseForItem(0,0);
}

void CInjectedWnd::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.ChangeVillage(L"WarpGate01");
}

DWORD g_dwVillageAddr = 0;
_declspec(naked) void GetVillageName()
{
	__asm
	{
		mov g_dwVillageAddr, eax

		MOV ESP,EBP
		POP EBP
		RETN 0x4
	}
}

void CInjectedWnd::OnBnClickedButton20()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD dwVillageNameLen = 0;
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_VILLAGE_NAME_LEN_OFFSET____,&dwVillageNameLen,sizeof(dwVillageNameLen));

	DWORD dwVillageNameAddr = 0;
	if( dwVillageNameLen > 7 )
	{
		_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_VILLAGE_NAME_OFFSET,&dwVillageNameAddr,sizeof(dwVillageNameAddr));
	}
	else
	{
		dwVillageNameAddr = JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_VILLAGE_NAME_OFFSET;
	}
	wchar_t wszReadVillageName[256] = {0};
	_ReadProcessMemory(dwVillageNameAddr,wszReadVillageName,dwVillageNameLen*2+2);

	wchar_t	wszVillageName[256] = {0};
	swprintf_s(wszVillageName,L"village name = %s\r\n",wszReadVillageName);

	CString csTxt;
	csTxt = wszVillageName;

	SetDlgItemText(IDC_EDIT_DATA,csTxt);

}

void CInjectedWnd::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.ChangeVillage(L"WarpGate02");
}

void CInjectedWnd::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.ChangeVillage(L"WarpGate03");
}

void CInjectedWnd::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.ChangeVillage(L"WarpGate04");
}

void CInjectedWnd::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	ROLE_FATIGUE	theFatigue;
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_FATIGUE_OFFSET,&theFatigue,sizeof(theFatigue));

	char	szPrint[256] = {0};
	sprintf_s(szPrint,"[JFZR] fatigue = %d MembershipFatigue = %d\r\n",theFatigue.str_nFatigue,theFatigue.str_nMembershipFatigue);

	SetDlgItemText(IDC_EDIT_DATA,szPrint);
	
}

void CInjectedWnd::OnBnClickedButton25()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;

	// 出售物品
	theActionSystem.SellItem(L"Ep02VillageMerchant_Friar",0,0,1);//Ep02VillageMerchant_Tinguri  Ep02VillageMerchant_Zenith

// 	// 购买物品
	//JFZRActionSystem theActionSystem;
	//theActionSystem.BuyItem(L"Ep02VillageMerchant_Friar",0x000000A6,1);
}

void CInjectedWnd::OnBnClickedButton26()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.LearnSkill(L"DoubleSlash",2);
}

void CInjectedWnd::OnBnClickedButton27()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.CreatePlayer(L"我打你三十个嘴巴",1,0);
}

void CInjectedWnd::OnBnClickedButton28()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.SelectPlayer(L"我打你三十个嘴巴");
}

bool	g_theUserSkillFlag;
void CInjectedWnd::OnBnClickedButton29()
{
	// TODO: 频道界面初始化
	SetDlgItemText(IDC_EDIT_DATA,TraverseChineseEnglish());
// 	SetDlgItemText(IDC_BUTTON29,"开始打怪");
// 	JFZRActionSystem theActionSystem;
// 
// 	g_theUserSkillFlag = true;
// 	while (g_theUserSkillFlag)
// 	{
// 		if( theActionSystem.useskill("ThunderSlash") )
// 		{
// 			Sleep(50);
// 		}
// 		else
// 		{
// 			break;
// 		}
// 	}
// 
// 	g_theUserSkillFlag = false;
// 	SetDlgItemText(IDC_BUTTON29,"停止打怪");
}

void CInjectedWnd::OnBnClickedButton30()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RequestSwitchChannel(L"channel_02_13");
	
}

void CInjectedWnd::OnBnClickedButton31()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theUnPacket.HookUnPacket();
}

void CInjectedWnd::OnBnClickedButton32()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theUnPacket.UnHookUnPacket();
}

void CInjectedWnd::OnBnClickedButton33()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.TeleportToVillage(L"Ep02Stage01_009",3);		// 任务 "临时情报驿官" 之快速移动，直接前往目的地
}

bool ActionSyncPacketWarp(wchar_t *pwszWarp)
{
	JFZRActionSystem theActionSystem;
	JFZRRoom theRoom;
	ROOM	rroom = {0};
	theRoom.UpDataRoom(&rroom);

	JFZRRole theRole;
	theRole.UpData();

	for (int i=0; i<3; i++)
	{
		if( wcscmp(rroom.str_wszRoomWarpEx[i],pwszWarp) == 0 )
		{
			FLOAT x = rroom.str_Coord[i].str_fx;
			FLOAT y = rroom.str_Coord[i].str_fy;


			extern bool GetLocalRoomPosition(FLOAT fx, FLOAT fy, FLOAT * pfz);
			FLOAT z = 0.0f;
			
			bool bRet = GetLocalRoomPosition(x,y,&z);
			assert(bRet);

			if( bRet == false )
			{
				return false;
			}

			z = theRole.m_fz;

			char szPrint[256] = {0};
			sprintf_s(szPrint,"[JFZR] 选择门 %d fx = %f fy = %f fz = %f",i,x,y,z);
			OutputDebugString(szPrint);

// 			JFZRActionSystem theActionSystem;
// 			theActionSystem.DirectionSyncPacket(x,y,z);

// 			theActionSystem.ActionSyncPacket(L"FastRun",x,y,z);
// 			Sleep(10);
			theActionSystem.ActionSyncPacket(L"Stand",x+1,y,z);
			
			return true;
		}
	}

	return false;
}

void CInjectedWnd::OnBnClickedButton34()
{
	// TODO: 在此添加控件通知处理程序代码

 	JFZRActionSystem theActionSystem;
// 	float fx = 141.0000f;
// 	float fy = 122.2673f;
// 	float fz = 0.0f;
// 	theActionSystem.Walk(L"FastRun",fx-1,fy,fz);
// 	theActionSystem.Walk(L"Stand",fx,fy,fz);

	if( ActionSyncPacketWarp(L"Warp01") )
		theActionSystem.GotoNextRoom(L"Warp01");
}

void CInjectedWnd::OnBnClickedButton35()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;

	if( ActionSyncPacketWarp(L"Warp02") )
		theActionSystem.GotoNextRoom(L"Warp02");
}

void CInjectedWnd::OnBnClickedButton36()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;

	if( ActionSyncPacketWarp(L"Warp03") )
		theActionSystem.GotoNextRoom(L"Warp03");
}

void CInjectedWnd::OnBnClickedButton37()
{
	// TODO: 在此添加控件通知处理程序代码

	JFZRActionSystem theActionSystem;
	theActionSystem.MailSend(0);
	theActionSystem.MailSend(1);
	theActionSystem.MailSend(2);
	theActionSystem.MailSend(3);
}

void CInjectedWnd::OnBnClickedButton38()
{
	// TODO: 在此添加控件通知处理程序代码

	JFZRActionSystem theActionSystem;
	theActionSystem.JoinVillage();
}

void CInjectedWnd::OnBnClickedButton39()
{
	// TODO: 在此添加控件通知处理程序代码	
	JFZRActionSystem theActionSystem;
	//theActionSystem.Accept(L"Ep02Stage01_010");
	theActionSystem.Accept(L"Class_Change_Thief_002");
}

void CInjectedWnd::OnBnClickedButton40()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.Complete(L"Class_Change_Thief_002",L"");
}

void CInjectedWnd::OnBnClickedButton41()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theSendPacket.Hook();
}

void CInjectedWnd::OnBnClickedButton42()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theSendPacket.UnHook();

	{
		// 恢复遍历对象
		unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
		_WriteProcessMemory(JFZR_REVERSAL_LOCAL_OBJ_ADDR_HOOK,szCode,5);
		_WriteProcessMemory(JFZR_REVERSAL_LOCAL_CITY_OBJ_HOOK,szCode,5);
		_WriteProcessMemory(JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_HOOK,szCode,5);
	}
	{
		// 00C63710  ^\E9 19FCFFFF     JMP JFZR.00C6332E
		unsigned char szCode[] = {0xE9,0x19,0xFC,0xFF,0xFF};
		_WriteProcessMemory(JFZR_REVERSAL_TIME_STAMP_HOOK,szCode,5);
	}

	{
		// 恢复遍历房间
		unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
		_WriteProcessMemory(JFZR_REVERSAL_LOCAL_ROOM_OBJ_HOOK,szCode,5);
	}

}

DWORD	g_dwCheckAddr = 0;
DWORD	g_dwJmpCheckAddr = 0x00F099F2;

void PrintCheck1(DWORD dwEdx,DWORD dwEcx,DWORD dwEax)
{
	char szPrint[256] = {0};
	DWORD dwTemp1 = 0;
	DWORD dwTemp2 = 0;

	__asm
	{
		mov eax, g_dwCheckAddr
		mov eax, dword ptr [eax]
		mov dwTemp1, eax
		mov eax, g_dwCheckAddr
		mov eax, dword ptr [eax+4]
		mov dwTemp2, eax
	}

	sprintf_s(szPrint,"[JFZR] edx = %p ecx = %p eax = %p ==> %p %p",dwEdx,dwEcx,dwEax,dwTemp1,dwTemp2);
	OutputDebugString(szPrint);
}

void PrintCheck2()
{
 	char szPrint[256] = {0};
 	DWORD dwTemp1 = 0;
 	DWORD dwTemp2 = 0;

	__asm
	{
		mov eax, g_dwCheckAddr
		mov eax, dword ptr [eax]
		mov dwTemp1, eax
		mov eax, g_dwCheckAddr
		mov eax, dword ptr [eax+4]
		mov dwTemp2, eax
	}
	sprintf_s(szPrint,"[JFZR] %p %p error = %p",dwTemp1,dwTemp2,WSAGetLastError());
	OutputDebugString(szPrint);

// 	if( dwTemp1 > 1 )
// 	{
// 		__asm
// 		{
// 			mov eax, g_dwCheckAddr
// 			mov dword ptr [eax], 0
// 		}
// 
// 		__asm
// 		{
// 			mov eax, g_dwCheckAddr
// 			mov eax, dword ptr [eax]
// 			mov dwTemp1, eax
// 			mov eax, g_dwCheckAddr
// 			mov eax, dword ptr [eax+4]
// 			mov dwTemp2, eax
// 		}
// 		sprintf_s(szPrint,"[JFZR] 修改后: %p %p",dwTemp1,dwTemp2);
// 		OutputDebugString(szPrint);
//  	}
}

_declspec(naked) void HookPrintCheck()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		push eax
		mov g_dwCheckAddr, eax
		push ecx
		push edx
		lea eax, PrintCheck1
		call eax
		add esp, 0x0C
	}

	__asm
	{
		popad
		popfd

// 		add dword ptr [esp],1
// 		add dword ptr [esp+4],1

		CALL DWORD PTR DS:[0x131AC2C]

	}

	__asm
	{
		pushfd
		pushad
		

		lea eax, PrintCheck2
		call eax

		popad
		popfd

		jmp dword ptr [g_dwJmpCheckAddr]

	}

}



void CInjectedWnd::OnBnClickedButton43()
{
	// TODO: 在此添加控件通知处理程序代码

	CString csTxt;
	csTxt = GetChinese(L"Skill.Warrior.CryingFury_Roar.Name");

	SetDlgItemText(IDC_EDIT_DATA,csTxt);

}

void CInjectedWnd::OnBnClickedButton44()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseJobTask());
}

void CInjectedWnd::OnBnClickedButton45()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseAllTask());
}

void CInjectedWnd::OnBnClickedButton46()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseReceivedTask());
}

// 测试代码
DWORD	g_dwNpcObjAddr = 0;
DWORD	g_dwNpcObjAddr2 = 0;

void HookNpcObjAddrPrint()
{
	if( g_dwNpcObjAddr != 0 && g_dwNpcObjAddr2 == 0 )
	{
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[JFZR] %p",g_dwNpcObjAddr);
		OutputDebugString(szPrint);

		g_dwNpcObjAddr2 = g_dwNpcObjAddr;
	}
	if( g_dwNpcObjAddr !=0 && g_dwNpcObjAddr2 != 0 )
	{
		if( g_dwNpcObjAddr2 != g_dwNpcObjAddr )
		{
			char szPrint[256] = {0};
			sprintf_s(szPrint,"[JFZR] %p",g_dwNpcObjAddr);
			OutputDebugString(szPrint);

			g_dwNpcObjAddr2 = g_dwNpcObjAddr;
		}
	}

}

_declspec(naked) void HookNpcObjAddr()
{
	__asm
	{
		pushad
		pushfd

		mov g_dwNpcObjAddr, ecx

		lea eax, HookNpcObjAddrPrint
		call eax

		popfd
		popad

		LEA EDX,DWORD PTR SS:[EBP+0x10]
		PUSH EDX
		PUSH EDI

		mov eax, JFZR_REVERSAL_ALLOBJ_MAP_FIND_ADDR_HOOK
		add eax, 5
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton47()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseTaskNpc());
}

void CInjectedWnd::OnBnClickedButton48()
{
	// TODO: 在此添加控件通知处理程序代码
	ToFbVillageTraverse();
	SetDlgItemText(IDC_EDIT_DATA,g_csPrintFbVillage);
}

void CInjectedWnd::OnBnClickedButton49()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.Disassembly(0,2,0);
}

void CompleteInGamePrint(char* pszStr)
{
	static bool bGameAcrotInTheVillageFlag = false;		// 玩家在城镇界面的标志

	if( strcmp("UiAs.StageSelectList.IntrusionOccur",pszStr) == 0 )
	{
		OutputDebugString("[JFZR] 入侵发生");
		MessageBox(NULL,"入侵发生","",MB_OK);
	}

	if( strcmp("UiAs.WorldSelectPage.LoginResult.NOT_CONNECTED",pszStr) == 0 )
	{
		OutputDebugString("[JFZR] 连接错误");
	}

	// 翻牌界面
	if( strcmp("UiAs.StageClear_RewardInfo_actions.CountTime.second",pszStr) == 0 )
	{
		//MessageBox(NULL,"秒，可以选择奖励物品","",MB_OK);
		OutputDebugString("[JFZR] 秒，可以选择奖励物品");
		JFZRActionSystem theActionSystem;
		theActionSystem.StageReward(1);
	}

	// BOOS房间选择返回城镇或者继续挑战界面
	if( strcmp("UiAs.StageClear_Buttons.StageClearButtonShow.JoinVillage",pszStr) == 0 || strcmp("UiAs.StageClear_Buttons.RestartStage",pszStr) == 0)
	{
		//MessageBox(NULL,"界面已到副本BOOS房间可点击<返回城镇>界面","",MB_OK);
		OutputDebugString("[JFZR] 界面已到副本BOOS房间可点击<返回城镇>界面");
	}

	static bool bInitFlag = false;
	// 角色选择界面
	if( strcmp("VirtualSetAnimation",pszStr) == 0 )
	{
		//MessageBox(NULL,"角色选择界面","",MB_OK);
		OutputDebugString("[JFZR] 角色选择界面");
		bInitFlag = true;

		bGameAcrotInTheVillageFlag = false;			// 角色不在游戏里
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 在角色界面开启故事动画的验证
	if( bInitFlag == true )
	{
		// 1级新号进入游戏的故事动画
		if( strcmp("/Global/Lobby/G_Story_World01_1",pszStr) == 0 )
		{
			//MessageBox(NULL,"故事动画","",MB_OK);

			OutputDebugString("[JFZR] 故事动画");
			JFZRActionSystem theActionSystem;
			theActionSystem.JoinVillage();
		}
	}

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 角色城镇界面
	if( strcmp("GetEventAlarmInfos",pszStr) == 0 )
	{
		//MessageBox(NULL,"城镇界面","",MB_OK);
		OutputDebugString("[JFZR] 城镇界面");

		bGameAcrotInTheVillageFlag = true;			// 置角色在游戏界面标志

		if( bInitFlag == true )
		{
			// 重新初始化所有完全进入游戏需要初始化的数据，且关闭了故事动画的环境验证
			bInitFlag = false;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 角色在游戏里，验证是否打开菜单
	if( bGameAcrotInTheVillageFlag == true )
	{
		if( strstr(pszStr,"UiAs.ShortcutList_ShortcutList_actions.ShortcutListInitUi.cashExchangeMarket") != 0 )
		{
			OutputDebugString("[JFZR] 打开游戏菜单界面");
			//MessageBox(NULL,"打开游戏菜单界面","",MB_OK);
			// 打开游戏菜单界面
			mouse(464,330);
			mouse(464,330);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 游戏掉线验证
	static bool bLoginStatusFlag = false;
	if( strcmp("UiAs.WorldSelectPage.LoginResult.NONE",pszStr) == 0 )
	{
		bLoginStatusFlag = true;
	}
	else if( strcmp("UiAs.WorldSelectPage.LoginResult.defaultMsg",pszStr) == 0 )
	{
		bLoginStatusFlag = true;
	}
	
	if( bLoginStatusFlag == true )
	{
		if( strcmp("/UI/Lobby/MenuSlide",pszStr) == 0 )
		{
			MessageBox(NULL,"游戏掉线","",MB_OK);
			bLoginStatusFlag = false;
		}
	}

	if( strcmp("GetActorLocation",pszStr) != 0	&&
		strcmp("GetCameraDirection",pszStr) != 0 )
	{
		char szPrint[256] = {0};
		char szBuffer[200] = {0};
		memcpy(szBuffer,pszStr,199);
		sprintf_s(szPrint,"[JFZR] %s",szBuffer);
		OutputDebugString(szPrint);
	}
}

_declspec(naked) void HookCompleteInGame()
{
	__asm
	{
		pushfd
		pushad		// 0x24
					// 0x08
		
		mov eax, dword ptr [esp+0x2C]
		push eax
		lea eax, CompleteInGamePrint
		call eax
		add esp,4
	}

	__asm
	{
		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH -0x1

		mov eax, JFZR_REVERSAL_GAME_CONDITION_HOOK
		add eax, 5
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton50()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool bflag = false;

	try
	{
		if( bflag == false )
		{
			Hook(JFZR_REVERSAL_GAME_CONDITION_HOOK,HookCompleteInGame);
			SetDlgItemText(IDC_EDIT_DATA,"Hook");
			bflag = true;
		}
		else
		{
			unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
			_WriteProcessMemory(JFZR_REVERSAL_GAME_CONDITION_HOOK,szCode,5);
			SetDlgItemText(IDC_EDIT_DATA,"UnHook");
			bflag = false;
		}
	}
	catch (...)
	{
		assert(false);
	}
	
	


}

void CInjectedWnd::OnBnClickedButton51()
{
	// TODO: 角色完全进入游戏再初始化
	SetDlgItemText(IDC_EDIT_DATA,TraverseSdbSkill());
}

void CInjectedWnd::OnBnClickedButton52()
{
	// TODO: 频道界面初始化
	SetDlgItemText(IDC_EDIT_DATA,TraverseSdbItem());
}

void CInjectedWnd::OnBnClickedButton53()
{
	// TODO: 频道界面初始化，或者完全进入游戏界面初始化
	SetDlgItemText(IDC_EDIT_DATA,TraverseSdbStage());
}

void CInjectedWnd::OnBnClickedButton54()
{
	// TODO: 测试代码，不必关心
	static bool bflag = false;

	if( bflag == false )
	{
		SDB_FUN;
		SetDlgItemText(IDC_EDIT_DATA,"Hook");
		bflag = true;
	}
	else
	{
		unsigned char szCode[] = {0x55,0x8B,0xEC,0x51,0x8B,0x51,0x04};
		_WriteProcessMemory(JFZR_REVERSAL_SDB_BASE_ADDR_UP_DATA_HOOK,szCode,7);
		SetDlgItemText(IDC_EDIT_DATA,"UnHook");
		bflag = false;
	}
}

void CInjectedWnd::OnBnClickedButton55()
{
	// TODO: 角色进入游戏再完全初始化，或者角色选择界面初始化
	::SetDlgItemTextW(m_hWnd,IDC_EDIT_DATA,TraverseShopInfo().c_str());
}

// 游戏内部将字符串赋值给申请出来的内存
extern void JFZR_GameMemcpy(DWORD lpAddr, WCHAR* pszStr);

typedef struct  
{
	GAME_STRING_ID		str_theActorName;
	GAME_STRING_ID		str_theUnKnow;
	DWORD				str_dwUnKnow1;
	DWORD				str_dwFlag;
	DWORD				str_dwUnKnow2;
}ACTOR_TEST,*PACTOR_TEST;

void RequestDeselectPlayer()
{
	GAME_STRUCT_ARRAY_POINT	theArgc1;
	GAME_STRUCT_ARRAY_POINT	theArgc2;

	theArgc2.str_dwBeginAddr = JFZR_AllmMemory_Alloc(0x40);
	theArgc2.str_dwEndAddr1 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwEndAddr2 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwCommandAddr	= JFZR_AllmMemory_Alloc(0x30);

	JFZR_GameMemcpy(theArgc2.str_dwBeginAddr,L"我打个一个嘴巴");

	PACTOR_TEST ptheActor = (PACTOR_TEST)theArgc2.str_dwBeginAddr;
	ptheActor->str_dwFlag = 1;

	WCHAR szCmd[] = L"RequestDeselectPlayer";
	memcpy((char*)theArgc2.str_dwCommandAddr,szCmd,sizeof(szCmd));

	__asm
	{
		lea eax, theArgc2
		push eax
		lea eax, theArgc1
		push eax
		mov ecx, 0x0C0AB54C
		mov eax, 0x00BBCE50 
		call eax
	}
}

void RequestPlayerList()
{
	GAME_STRUCT_ARRAY_POINT	theArgc1;
	GAME_STRUCT_ARRAY_POINT	theArgc2 = {0};

	theArgc2.str_dwCommandAddr	= JFZR_AllmMemory_Alloc(0x30);

	WCHAR szCmd[] = L"RequestPlayerList";
	memcpy((char*)theArgc2.str_dwCommandAddr,szCmd,sizeof(szCmd));

	__asm
	{
		lea eax, theArgc2
		push eax
		lea eax, theArgc1
		push eax
		mov ecx, 0x0C0AB54C
		mov eax, 0x00BBD030
		call eax
	}
}



void GetConfigValue()
{
	GAME_STRUCT_ARRAY_POINT	theArgc1;
	GAME_STRUCT_ARRAY_POINT	theArgc2;

	theArgc2.str_dwBeginAddr = JFZR_AllmMemory_Alloc(0x40);
	theArgc2.str_dwEndAddr1 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwEndAddr2 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwCommandAddr	= JFZR_AllmMemory_Alloc(0x30);

	JFZR_GameMemcpy(theArgc2.str_dwBeginAddr,L"autologin");


	WCHAR szCmd[] = L"GetConfigValue";
	memcpy((char*)theArgc2.str_dwCommandAddr,szCmd,sizeof(szCmd));

	__asm
	{
		lea eax, theArgc2
		push eax
		lea eax, theArgc1
		push eax
		mov ecx, 0x0BD07CF4
		mov eax, 0x009FE1D0
		call eax
	}
}


void PlayBgm()
{
	GAME_STRUCT_ARRAY_POINT	theArgc1;
	GAME_STRUCT_ARRAY_POINT	theArgc2;

	theArgc2.str_dwBeginAddr = JFZR_AllmMemory_Alloc(0x40);
	theArgc2.str_dwEndAddr1 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwEndAddr2 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwCommandAddr	= JFZR_AllmMemory_Alloc(0x30);

	JFZR_GameMemcpy(theArgc2.str_dwBeginAddr,L"Lobby_Loop");

	WCHAR szCmd[] = L"PlayBgm";
	memcpy((char*)theArgc2.str_dwCommandAddr,szCmd,sizeof(szCmd));

	__asm
	{
		lea eax, theArgc2
		push eax
		lea eax, theArgc1
		push eax
		mov ecx, 0x0BD08084
		mov eax, 0x0086D960
		call eax
	}
}



void UIOpenDefault()
{
	GAME_STRUCT_ARRAY_POINT	theArgc1;
	GAME_STRUCT_ARRAY_POINT	theArgc2;

	theArgc2.str_dwBeginAddr = JFZR_AllmMemory_Alloc(0x40);
	theArgc2.str_dwEndAddr1 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwEndAddr2 = theArgc2.str_dwBeginAddr+0x40;
	theArgc2.str_dwCommandAddr	= JFZR_AllmMemory_Alloc(0x30);

	JFZR_GameMemcpy(theArgc2.str_dwBeginAddr,L"/UI/Window_OpenDefault");

	PACTOR_TEST ptheActor = (PACTOR_TEST)theArgc2.str_dwBeginAddr;
	ptheActor->str_dwFlag = 1;

	WCHAR szCmd[] = L"PlaySound";
	memcpy((char*)theArgc2.str_dwCommandAddr,szCmd,sizeof(szCmd));

	__asm
	{
		lea eax, theArgc2
		push eax
		lea eax, theArgc1
		push eax
		mov ecx, 0x0BBE16DC
		mov eax, 0x0086D4D0
		call eax
	}
}


void CInjectedWnd::OnBnClickedButton56()
{
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	extern void WhileNpcActionSyncPacket();
	WhileNpcActionSyncPacket();Sleep(3000);


// 	for (int i=0; i<100; i++)
// 	{
// 		if( false == theActionSystem.useskill("IcePick",0,0)  )
// 			break;
// 		Sleep(50);
// 	}

	try
	{
		::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("正在清理怪物..."));
		while ( true )
		{
			if( theActionSystem.useskill("IcePick",0,0) || theActionSystem.useskill2("IcePick",0,0)  )
			{
				Sleep(50);
			}
			else 
			{
				::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("怪物已经被清理完..."));
				break;
			}
			//break;
		}

	}
	catch (...)
	{
		assert(false);
	}
}


void PrintUseSkillHashString(wchar_t* pSkillId)
{
	wchar_t	wszPrint[256];
	swprintf_s(wszPrint,L"[JFZR] skill hash str = %s\r\n",pSkillId);
	OutputDebugStringW(wszPrint);
}

_declspec(naked) void HookPrintUseSkillHashString()
{
	__asm
	{
		pushfd
		pushad

		mov eax, [esp+0x2C]
		mov eax, [eax]
		push eax
		lea eax, PrintUseSkillHashString
		call eax
		add esp, 4

		popad
		popfd

		push ebp
		mov ebp, esp
		push -0x1

		mov eax, JFZR_REVERSAL_USESKILL_HIT_HASH_STRING_HOOK
		add eax, 5
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton57()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool bInitHookFlag = false;

	if( bInitHookFlag == false )
	{
		Hook(JFZR_REVERSAL_USESKILL_HIT_HASH_STRING_HOOK,HookPrintUseSkillHashString);
		SetDlgItemText(IDC_EDIT_DATA,"hash Hook");
		bInitHookFlag = true;
	}
	else
	{
		unsigned char szCode[] = {JFZR_REVERSAL_LOCAL_OBJ_HOOK_OLD_CODE};
		_WriteProcessMemory(JFZR_REVERSAL_USESKILL_HIT_HASH_STRING_HOOK,szCode,5);
		SetDlgItemText(IDC_EDIT_DATA,"hash UnHook");
		bInitHookFlag = false;
	}

}

void CInjectedWnd::OnBnClickedButton58()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.MailRemove(0x00000000001AB252,1);
}



void CInjectedWnd::OnBnClickedButton59()
{
	// TODO: 在此添加控件通知处理程序代码

	GetProjectileSerial();
}

void CInjectedWnd::OnBnClickedButton60()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for(int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("ChargeHack",0,0) )
			break;
		Sleep(50);
	}
}

void CInjectedWnd::OnBnClickedButton61()
{
	// TODO: 在此添加控件通知处理程序代码
	UIOpenDefault();
}


void JFZR_CMD(WCHAR* pszCmd);
void CInjectedWnd::OnBnClickedButton62()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZR_CMD(L"PressMove");
	JFZR_CMD(L"ReleaseMove");
}



void CInjectedWnd::OnBnClickedButton63()
{
	// TODO: 在此添加控件通知处理程序代码
	KeyPress(VK_ESCAPE);
}

void CInjectedWnd::OnBnClickedButton64()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseClassInfo());
}

void CInjectedWnd::OnBnClickedButton65()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseRegionInfo());
}

void CInjectedWnd::OnBnClickedButton66()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RequestProductList(L"050100",L"魔能碎片",1);
}

void CInjectedWnd::OnBnClickedButton67()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.BuyProduct(0x0000000000040908,1);
}

void CInjectedWnd::OnBnClickedButton68()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	// "魔能碎片",ItemTabCategory,ItemSlotIndex,Price,Period,Count
	theActionSystem.RegisterProduct(L"魔能碎片",3,0,10000,3,1);		// 发送之后，如果成功，随后会收到 Recv_UnPacket_RequestMyProductList
}


void CInjectedWnd::OnBnClickedButton69()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RequestMyProductList();
}

void CInjectedWnd::OnBnClickedButton70()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.UnregisterProduct(0x000000000004B54A);
}

void CInjectedWnd::OnBnClickedButton71()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.GetAttachments(0x0000000000026D61,1);
}

void CInjectedWnd::OnBnClickedButton72()
{
	// TODO: 在此添加控件通知处理程序代码
	mouse(464,330);
}

void CInjectedWnd::OnBnClickedButton73()
{
	// TODO: 在此添加控件通知处理程序代码
	//JFZR_REVERSAL_ROLE_OBJ_ADDR
	JFZRRole theRole;
	theRole.UpStaticData();
	SetDlgItemText(IDC_EDIT_DATA,theRole.csTxt);
}

void CInjectedWnd::OnBnClickedButton74()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseActioncommands());
}

void CInjectedWnd::OnBnClickedButton75()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;
	//theActionSystem.useskill("HalfMoonKick",1,1);

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("HalfMoonKick",0,0) )
			break;
		Sleep(1);
	}

	//theActionSystem.Activity(0,50,0,0);
}

void CInjectedWnd::OnBnClickedButton76()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.StageReward(1);
}

void CInjectedWnd::OnBnClickedButton77()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RepairItem();
}

void CInjectedWnd::OnBnClickedButton78()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.RequestProductInfo(3,3);	// 材料栏=3 第3格物品  返回确认包关注结构体 RECV_REQUEST_PRODUCT_INFO
}

void CInjectedWnd::OnBnClickedButton79()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theSendPacket.GameSendPacketHook();
}

void CInjectedWnd::OnBnClickedButton80()
{
	// TODO: 在此添加控件通知处理程序代码
	g_theSendPacket.UnGameSendPacketHook();
}

void CInjectedWnd::OnBnClickedButton81()
{
	// TODO: 在此添加控件通知处理程序代码
	
	JFZRRoom theRoom;
	ROOM theCurRoom;
	theRoom.UpDataCity(&theCurRoom);

	SetDlgItemText(IDC_EDIT_DATA,theRoom.csTxt);
}


void __cdecl printfgamestring(DWORD dwIndex,wchar_t* pstring1,wchar_t* pstring2,wchar_t* pstring3)
{
	try
	{
		wchar_t wszPrint[1024];
		swprintf_s(wszPrint,L"[JFZR] %s ==> %s ==>%s",pstring1,pstring2,pstring3);
		OutputDebugStringW(wszPrint);

		if( wcsstr(pstring3,L"game/resource/applemain/ui2/playtimeeventgauge.gfx") != 0 )
		{
			MessageBox(NULL,"游戏界面","",MB_OK);
		}
	}
	catch (...)
	{
		assert(false);
	}
}

__declspec(naked) void hook_printfgamestring()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		mov eax, [esp+0x24+0x10]
		mov ebx, [esp+0x24+0x0C]
		mov ecx, [esp+0x24+0x08]
		mov edx, [esp+0x24+0x04]
		push eax
		push ebx
		push ecx
		push edx
		lea eax, printfgamestring
		call eax
		add esp, 0x10

	}

	__asm
	{
		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH -0x1


		mov eax, 0x007A1E00
		add eax, 5
		jmp eax
	}
}

void CInjectedWnd::OnBnClickedButton82()
{
	Hook(0x007A1E00,hook_printfgamestring);
}


void CInjectedWnd::OnBnClickedButton83()
{
	GetTimeStampAddr();
	char szPrint[256];
	sprintf_s(szPrint,"TimeStamp = 0x%p",GetTimeStamp());
	SetDlgItemText(IDC_EDIT_DATA,szPrint);
}

void CInjectedWnd::OnBnClickedButton84()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("SmallMeteor",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton85()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("Horizon",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton86()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("TekkenPunch",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton87()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("CrystalShower",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton88()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("UpperClaw",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton89()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("正在清理怪物..."));
	while( theActionSystem.useskill("Stinger",0,0) ||  theActionSystem.useskill2("Stinger",0,0) )
	{
		Sleep(50);
	}	
	
	{
		::SendMessage(m_hWnd,WM_SETTEXT,NULL,(LPARAM)TEXT("怪物已经被清理完..."));
	}

// 	for (int i=0; i<100; i++)
// 	{
// 		if( false == theActionSystem.useskill("Stinger",0,0) )
// 			break;
// 		Sleep(1);
// 	}
}

void CInjectedWnd::OnBnClickedButton90()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("LightningSlash",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton91()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("5Slash",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton92()
{
	// TODO: 在此添加控件通知处理程序代码
	GetTimeStampAddr();
	JFZRActionSystem theActionSystem;

	for (int i=0; i<100; i++)
	{
		if( false == theActionSystem.useskill("ShadowAttack",0,0) )
			break;
		Sleep(1);
	}
}

void CInjectedWnd::OnBnClickedButton93()
{
	// TODO: 在此添加控件通知处理程序代码
	// 打开箱子
	JFZRActionSystem theActionSystem;
	theActionSystem.OpenItemPackage(1,0,-1,0);	// 不需要钥匙开箱子的
	//theActionSystem.OpenItemPackage(1,0,1,1);		// 需要钥匙开箱子的

	//theActionSystem.UseItem(1,0);
}


void CInjectedWnd::OnBnClickedButton94()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseSkillHitHash());

}

void CInjectedWnd::OnBnClickedButton95()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.DirectionSyncPacket(96.651718f,96.021019f,-0.020203f);
}

void CInjectedWnd::OnBnClickedButton96()
{
	JFZRRole theRole;
	theRole.UpData();

	PREV_ACTION_DATA thePrevActionData;
	thePrevActionData.dwSerial = theRole.m_dwSerial;
	
	if( true == TraversePrevActionPlayTime_(&thePrevActionData) )
	{
		char szPrint[256];
		sprintf_s(szPrint,"fActionPlaytimeData = %f dwPrevAction = %p\r\n",thePrevActionData.fPrevActionPlayTime,thePrevActionData.dwPrevActionSerial);

		extern CString g_csPrevActionPlayTimeStr;
		g_csPrevActionPlayTimeStr += szPrint;
		SetDlgItemText(IDC_EDIT_DATA,g_csPrevActionPlayTimeStr);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_DATA,"没有获取到任何数据");
	}
}


void CInjectedWnd::OnBnClickedButton97()
{
	// TODO: 在此添加控件通知处理程序代码
	extern FLOAT GetSpeed(DWORD dwSpeedAddr);
	char szPrint[256];
	sprintf_s(szPrint,"fAttackSpeed = %f fMoveSpeed = %f fEvadeRating = %f",GetSpeed(JFZR_REVERSAL_GET_ATTACK_SPEED_ADDR),GetSpeed(JFZR_REVERSAL_GET_MOVE_SPEED_ADDR),GetSpeed(JFZR_REVERSAL_GET_EVADE_RATING_ADDR));
	SetDlgItemText(IDC_EDIT_DATA,szPrint);
}

void CInjectedWnd::OnBnClickedButton98()
{
	// TODO: 在此添加控件通知处理程序代码
}

DWORD GetNpcActionHash(DWORD dwSerial,wchar_t* pActionId)
{
//	OutputDebugString("[JFZR] GetNpcActionRealizeHash!!!");
	try
	{
		JFZR_MONSTER_OBJ_STRUCT theObjStruct;
		memset(&theObjStruct,0,sizeof(theObjStruct));
		memcpy(theObjStruct.str_szType,L"NPC",wcslen(L"NPC")*2);


		// 遍历对象
		theMonsterObj.TraverseObj(true);

		if( theMonsterObj.GetObjCount(L"NPC") == 0 )
		{
			return 0;
		}

		theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,100);

		for (int i=0; i<theObjStruct.str_nCount; i++)
		{
			if( theObjStruct.str_theMonsterObj[i].str_dwSerial == dwSerial )
			{
				DWORD dwActionHash = GetHash(theObjStruct.str_theMonsterObj[i].str_szJobName,pActionId);
// 				wchar_t wszPrint[256] = {0};
// 				swprintf_s(wszPrint,L"[JFZR] %s ==>  ClassId = %s ActionHash = %p",pActionId,theObjStruct.str_theMonsterObj[i].str_szJobName,dwActionHash);
// 				OutputDebugStringW(wszPrint);
				return dwActionHash;
			}
		}

		//assert(false);
		
	}
	catch (...)
	{	
		assert(false);
	}

	return 0;
}

DWORD GetNpcActionStandby_Hold_RealizeHash(DWORD dwSerial)
{
	OutputDebugString("[JFZR] GetNpcActionStandby_Hold_RealizeHash!!!");
	try
	{
		JFZR_MONSTER_OBJ_STRUCT theObjStruct;
		memset(&theObjStruct,0,sizeof(theObjStruct));
		memcpy(theObjStruct.str_szType,L"NPC",wcslen(L"NPC")*2);


		// 遍历对象
		theMonsterObj.TraverseObj(true);

		if( theMonsterObj.GetObjCount(L"NPC") == 0 )
		{
			return 0;
		}

		theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,100);

		for (int i=0; i<theObjStruct.str_nCount; i++)
		{
			if( theObjStruct.str_theMonsterObj[i].str_dwSerial == dwSerial )
			{
				DWORD dwActionHash = GetHash(theObjStruct.str_theMonsterObj[i].str_szJobName,L"Standby_Hold_Realize");
				wchar_t wszPrint[256] = {0};
				swprintf_s(wszPrint,L"[JFZR] Standby_Hold_Realize ==>  ClassId = %s ActionHash = %p",theObjStruct.str_theMonsterObj[i].str_szJobName,dwActionHash);
				OutputDebugStringW(wszPrint);
				return dwActionHash;
			}
		}

		//assert(false);

	}
	catch (...)
	{	
		assert(false);
	}

	return 0;
}

void WhileNpcActionSyncPacket()
{
	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
	memset(&theObjStruct,0,sizeof(theObjStruct));
	memcpy(theObjStruct.str_szType,L"NPC",wcslen(L"NPC")*2+1);

	// 遍历对象
	theMonsterObj.TraverseObj(false);

	if( theMonsterObj.GetObjCount(L"NPC") == 0 )
	{
		return ;
	}

	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,100);

	JFZRRole theRole;
	theRole.UpData();


	JFZRActionSystem theActionSystem;
	try
	{
		JFZRActionSystem theActionSystem;

		/*
		for (int j=0; j<5; j++)
		{
			for (int i=0; i<theObjStruct.str_nCount; i++)
			{

				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"Standby_Hold") == 0 )	// 保持站立
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"Standby_Hold_Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
					if( j == 3 )
						theActionSystem.NpcActionSyncPacket(L"RandomRun",&theObjStruct.str_theMonsterObj[i]);
				}

				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"Standby_Patrol") == 0 )	// 巡逻
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
					theActionSystem.NpcActionSyncPacket(L"RandomRun",&theObjStruct.str_theMonsterObj[i]);

				}

				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"Stanby_Sleeping") == 0 )
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"Stanby_Sleeping_Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
						theActionSystem.NpcActionSyncPacket(L"BullDog_Alert",&theObjStruct.str_theMonsterObj[i]);
					if( j == 3 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
				}

				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"StandBy_Up") == 0 )
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"StandBy_Up_Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
					if( j == 3 )
						theActionSystem.NpcActionSyncPacket(L"BullDog_StrongAttack",&theObjStruct.str_theMonsterObj[i]);
					if( j == 4 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
				}

				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"Standby_IdleAction") == 0 )
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
					if( j == 3 )
						theActionSystem.NpcActionSyncPacket(L"BullDog_StrongAttack",&theObjStruct.str_theMonsterObj[i]);

				}


				if( wcscmp(theObjStruct.str_theMonsterObj[i].str_szDefaultActionId,L"Stand") == 0 )
				{
					//assert(false);
					if( j == 0 )
						theActionSystem.NpcActionSyncPacket(L"Realize",&theObjStruct.str_theMonsterObj[i]);
					if( j == 1 )
						theActionSystem.NpcActionSyncPacket(L"Stand",&theObjStruct.str_theMonsterObj[i]);
					if( j == 2 )
						theActionSystem.NpcActionSyncPacket(L"RandomRun",&theObjStruct.str_theMonsterObj[i]);
					if( j == 3 )
						theActionSystem.NpcActionSyncPacket(L"BullDog_StrongAttack",&theObjStruct.str_theMonsterObj[i]);

				}
			}
			Sleep(500);
		}
		*/



		int i=0;
		for (i=0; i<theObjStruct.str_nCount; i++)
		{
			// 本地修改坐标，实现一次攻击一个NPC
			theActionSystem.LocalActionSyncPacket(theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz+50.0f);
			Sleep(100);
		}
		
// 		theActionSystem.LocalActionSyncPacket(theObjStruct.str_theMonsterObj[i].str_fx,theObjStruct.str_theMonsterObj[i].str_fy,theObjStruct.str_theMonsterObj[i].str_fz+50.0f);
// 		Sleep(100);

		theActionSystem.LocalActionSyncPacket(theRole.m_fx,theRole.m_fy,theRole.m_fz+50.0f);

	}
	catch (...)
	{
		assert(false);
	}
}

void CInjectedWnd::OnBnClickedButton99()
{
	// TODO: 在此添加控件通知处理程序代码
// 	JFZRActionSystem theActionSystem;
// 	theActionSystem.CreateNpcByClient(0x00183328,0x1);

	WhileNpcActionSyncPacket();
}

DWORD	g_dwWarpSerial = 0;
void CInjectedWnd::OnBnClickedButton100()
{
	// TODO: 在此添加控件通知处理程序代码

	JFZRActionSystem theActionSystem;
	theActionSystem.DestroyActors(g_dwWarpSerial);
	g_dwWarpSerial = 0;
}

bool g_bChangeSectorFlag = false;
void CInjectedWnd::OnBnClickedButton101()
{
	// TODO: 在此添加控件通知处理程序代码

	if( g_bChangeSectorFlag == false )
	{
		g_bChangeSectorFlag = true;
		SetDlgItemText(IDC_EDIT_DATA,"开启移动封包拦截");
	}
	else
	{
		g_bChangeSectorFlag = false;
		SetDlgItemText(IDC_EDIT_DATA,"关闭移动封包拦截");
	}
}

typedef struct
{
	DWORD				dwVFunAddr;
	GAME_STRING_ID		theClassId;
	GAME_STRING_ID		theActionId;
}SDB_ACTION_KEY,*PSDB_ACTION_KEY;

void PrintHash(PSDB_ACTION_KEY ptheActionKey,DWORD dwActionKey)
{
	try
	{
		wchar_t wszPrint[256] = {0};
		swprintf_s(wszPrint,L"[JFZR] class = %s action = %s key = %p",GetString(&ptheActionKey->theClassId),GetString(&ptheActionKey->theActionId),dwActionKey);
		OutputDebugStringW(wszPrint);

		if( wcscmp(GetString(&ptheActionKey->theActionId),L"Realize") == 0 )
		{
			assert(false);
		}
	}
	catch (...)
	{
		OutputDebugString("[JFZR] PrintHash error!!!");
	}
}

__declspec(naked) void hook_GetHash()
{
	__asm
	{
		pushfd
		pushad
			
		mov edi, dword ptr [esp+0x24+0x4]
		push eax
		push edi
		lea eax, PrintHash
		call eax
		add esp, 8

		popad
		popfd

		retn
	}
}

void CInjectedWnd::OnBnClickedButton102()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(0x00471C44,hook_GetHash);
}


void CInjectedWnd::OnBnClickedButton103()
{
	// TODO: 在此添加控件通知处理程序代码
	extern DWORD g_dwValidationData_X;		// 过图验证数据x
	extern DWORD g_dwValidationData_Y;		// 过图验证数据y
	extern BYTE  g_bIsDummyExist;

	FLOAT	fPositionX = 87.405983f;
	FLOAT	fPositionY = 65.154320f;
	__asm
	{
		fld dword ptr[fPositionX]
		mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
		call eax
		mov dword ptr[g_dwValidationData_X], eax
		fld dword ptr[fPositionY]
		mov eax, JFZR_REVERSAL_VALIDATION_DATA_CALL
		call eax
		mov dword ptr[g_dwValidationData_Y], eax
	}

	g_bIsDummyExist = 1;
}

DWORD g_dwLocalRoomPositionAddr = 0;
_declspec(naked) void HookGetLocalRoomPosition()
{
	__asm
	{
		mov g_dwLocalRoomPositionAddr, ecx
	}
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		PUSH EBX
		PUSH ESI

		mov eax, JFZR_REVERSAL_GET_LOCAL_ROOM_POSITION_HOOK
		add eax, 5
		jmp eax
	}
}

typedef struct
{
	GAME_STRING_ID_		theId;
	DWORD				dwRoomPositionAddr;
}WARP_GATE_POINT,*PWARP_GATE_POINT;

typedef struct  
{
	DWORD			dwFunAddr;
	GAME_STRUCT_ARRAY_POINT		theArray1;
	GAME_STRUCT_ARRAY_POINT		theArray2;
	DWORD						dwUnKnow;
	XYZ							theRoomPosition;
}WARP_GATE_EFFECT_INFO,*PWARP_GATE_EFFECT_INFO;

CString g_csLocalRoomPositon;

bool GetLocalRoomPosition(FLOAT fx, FLOAT fy, FLOAT * pfz)
{
	g_csLocalRoomPositon = "";
	g_dwLocalRoomPositionAddr = 0;

	Hook(JFZR_REVERSAL_GET_LOCAL_ROOM_POSITION_HOOK,HookGetLocalRoomPosition);

	while ( !g_dwLocalRoomPositionAddr )
	{
		Sleep(10);
	}

	GAME_STRUCT_ARRAY_POINT	theRoomPosition;
	_ReadProcessMemory(g_dwLocalRoomPositionAddr+JFZR_REVERSAL_GET_LOCAL_ROOM_POSITION_OFFSET___,&theRoomPosition,sizeof(theRoomPosition));

	assert(theRoomPosition.str_dwBeginAddr);
	assert(theRoomPosition.str_dwEndAddr1);


	if( theRoomPosition.str_dwBeginAddr == 0 || theRoomPosition.str_dwEndAddr1 == 0 )
	{
		return false;
	}

	for (DWORD i=theRoomPosition.str_dwBeginAddr; i<theRoomPosition.str_dwEndAddr1; i+=sizeof(WARP_GATE_POINT) )
	{
		WARP_GATE_POINT	theWarpGatePoint;
		_ReadProcessMemory(i,&theWarpGatePoint,sizeof(theWarpGatePoint));

		WARP_GATE_EFFECT_INFO	theWarpGateEffectInfo;
		_ReadProcessMemory(theWarpGatePoint.dwRoomPositionAddr,&theWarpGateEffectInfo,sizeof(theWarpGateEffectInfo));

		wchar_t wszPrint[256];
		swprintf_s(wszPrint,L"[JFZR] id = %s x = %f y = %f z = %f\r\n",
			GetString_(&theWarpGatePoint.theId),
			theWarpGateEffectInfo.theRoomPosition.fx,
			theWarpGateEffectInfo.theRoomPosition.fy,
			theWarpGateEffectInfo.theRoomPosition.fz);

		g_csLocalRoomPositon += wszPrint;

		if( ((int)fx == (int)theWarpGateEffectInfo.theRoomPosition.fx) && ((int)fy == (int)theWarpGateEffectInfo.theRoomPosition.fy) )
		{
			*pfz = theWarpGateEffectInfo.theRoomPosition.fz;

			return true;
		}
	}
	
	return false;
}

void CInjectedWnd::OnBnClickedButton104()
{
	// TODO: 在此添加控件通知处理程序代码
	FLOAT fx = 0.0f;
	FLOAT fy = 0.0f;
	FLOAT fz = 0.0f;
	assert(GetLocalRoomPosition(fx,fy,&fz));
	SetDlgItemText(IDC_EDIT_DATA,g_csLocalRoomPositon);

}

DWORD g_dwItemStatusValue = 0;
void CInjectedWnd::OnBnClickedButton105()
{
	// TODO: 在此添加控件通知处理程序代码
// 	char szPrint[256] ={0};
// 	sprintf_s(szPrint,"Direction = %p",GetDirection(0.8065239f));	// 3a74
// 	SetDlgItemText(IDC_EDIT_DATA,szPrint);

	JFZRItem theItem;
	g_dwItemStatusValue = theItem.TraverseItem2(6);
	g_dwItemStatusValue = (DWORD)(g_dwItemStatusValue*0.8f);
	char szPrint[256] ={0};
	sprintf_s(szPrint,"ItemStatusValue = %d",g_dwItemStatusValue);	// 3a74
	SetDlgItemText(IDC_EDIT_DATA,szPrint);

	
}


void CInjectedWnd::OnBnClickedButton106()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char szCode[] = {0x90,0x90,0x90};
	_WriteProcessMemory(JFZR_REVERSAL_WUDI_CODE_ADDR___,szCode,3);
}

bool g_bNpcActionSyncPacketFlag = false;
void CInjectedWnd::OnBnClickedButton107()
{
	// TODO: 在此添加控件通知处理程序代码
	if( g_bNpcActionSyncPacketFlag == false )
	{
		g_bNpcActionSyncPacketFlag = true;
		SetDlgItemText(IDC_EDIT_DATA,"开启移动封包");
	}
	else
	{
		g_bNpcActionSyncPacketFlag = false;
		SetDlgItemText(IDC_EDIT_DATA,"关闭移动封包");
	}
}

void CInjectedWnd::OnBnClickedButton108()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_DATA,TraverseChapter());
}

void CInjectedWnd::OnBnClickedButton109()
{
	// TODO: 在此添加控件通知处理程序代码
	JFZRActionSystem theActionSystem;
	theActionSystem.GiveUp(L"Ep03Stage02_007");
}
