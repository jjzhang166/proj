// InjectedWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "jfzrcalltest.h"
#include "InjectedWnd.h"
#include "public/MMOPacket.h"
#include "public/UsefulFns.h"
#include "MMOClientTraverse.h"
#include "MMOUnSendPacket.h"
#include "MMOCall.h"
#include <ntsecapi.h>
#include <list>
#include "public/UsefulFns.h"
#include  "Tlhelp32.h"
#include "psapi.h"
#include <vector>
#include "public/MHOUi.h"
using namespace std;
#pragma comment(lib, "Psapi.lib ")
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
	ON_BN_CLICKED(IDC_BUTTON1, &CInjectedWnd::OnBtPacket_HanHua)
	ON_BN_CLICKED(IDC_BUTTON2, &CInjectedWnd::OnBt_HookSend)
	ON_BN_CLICKED(IDC_BUTTON3, &CInjectedWnd::OnBtPacket_Walk)
	ON_BN_CLICKED(IDC_BUTTON4, &CInjectedWnd::OnBt_TraObj)
	ON_BN_CLICKED(IDC_BUTTON5, &CInjectedWnd::OnBt_HookModifyHitBody)
	ON_BN_CLICKED(IDC_BUTTON6, &CInjectedWnd::OnBt_UnHook)
	ON_BN_CLICKED(IDC_BUTTON7, &CInjectedWnd::OnBt_SkillCall)
	ON_BN_CLICKED(IDC_BUTTON8, &CInjectedWnd::OnBt_WangLiFb)
	ON_BN_CLICKED(IDC_BUTTON9, &CInjectedWnd::OnBt_HookRecv)
	ON_BN_CLICKED(IDC_BUTTON10, &CInjectedWnd::OnBt_HookDecrypt)
	ON_BN_CLICKED(IDC_BUTTON11, &CInjectedWnd::OnBt_HookDlgInfo)
	ON_BN_CLICKED(IDC_BUTTON12, &CInjectedWnd::OnBt_TraDlgInfo)
	ON_BN_CLICKED(IDC_BUTTON13, &CInjectedWnd::OnBt_TraItem)
	ON_BN_CLICKED(IDC_BUTTON14, &CInjectedWnd::OnBtPacket_AcceptTask)
	ON_BN_CLICKED(IDC_BUTTON15, &CInjectedWnd::OnBt_HookActorHpCall)
	ON_BN_CLICKED(IDC_BUTTON16, &CInjectedWnd::OnBt_TraTaskInfo)
	ON_BN_CLICKED(IDC_BUTTON17, &CInjectedWnd::OnBt_TraReceivedTask)
	ON_BN_CLICKED(IDC_BUTTON18, &CInjectedWnd::OnBt_TraUiInfo)
	ON_BN_CLICKED(IDC_BUTTON19, &CInjectedWnd::OnBt_HookEncrypt)
	ON_BN_CLICKED(IDC_BUTTON20, &CInjectedWnd::OnBt_HookMonsterHpCall)
	ON_BN_CLICKED(IDC_BUTTON21, &CInjectedWnd::OnBt_HookCryMalloc)
	ON_BN_CLICKED(IDC_BUTTON22, &CInjectedWnd::OnBtPacket_OpenNpc)
	ON_BN_CLICKED(IDC_BUTTON23, &CInjectedWnd::OnBtPacket_GuoTu)
	ON_BN_CLICKED(IDC_BUTTON24, &CInjectedWnd::OnBt_TraAcceptableTask)
	ON_BN_CLICKED(IDC_BUTTON25, &CInjectedWnd::OnBt_HookTemplateInfo)
	ON_BN_CLICKED(IDC_BUTTON26, &CInjectedWnd::OnBt_TraUnknownObj)
	ON_BN_CLICKED(IDC_BUTTON27, &CInjectedWnd::OnBt_TraAllShiXianObj)
	ON_BN_CLICKED(IDC_BUTTON29, &CInjectedWnd::OnBt_TraZhuiZongTask)
	ON_BN_CLICKED(IDC_BUTTON28, &CInjectedWnd::OnBtCityGuoTu)
	ON_BN_CLICKED(IDC_BUTTON30, &CInjectedWnd::OnBtPacket_CommitTask)
	ON_BN_CLICKED(IDC_BUTTON31, &CInjectedWnd::OnBtPacket_CollectItem)
	ON_BN_CLICKED(IDC_BUTTON32, &CInjectedWnd::OnBt_TraFbInfo)
	ON_BN_CLICKED(IDC_BUTTON33, &CInjectedWnd::OnBtPacket_EnterFb)
	ON_BN_CLICKED(IDC_BUTTON34, &CInjectedWnd::OnBtPacket_CunQuStore)
	ON_BN_CLICKED(IDC_BUTTON35, &CInjectedWnd::OnBt_ItemTemplateInfo)
	ON_BN_CLICKED(IDC_BUTTON36, &CInjectedWnd::OnBtPacket_GainSupply)
	ON_BN_CLICKED(IDC_BUTTON37, &CInjectedWnd::OnBt_HookOptimizeYouHua)
	ON_BN_CLICKED(IDC_BUTTON38, &CInjectedWnd::OnBt_UseItem)
	ON_BN_CLICKED(IDC_BUTTON39, &CInjectedWnd::OnBtPacket_MakeItem)
	ON_BN_CLICKED(IDC_BUTTON40, &CInjectedWnd::OnBt_HookLoadEndMap)
	ON_BN_CLICKED(IDC_BUTTON41, &CInjectedWnd::OnBtPacket_SellItem)
	ON_BN_CLICKED(IDC_BUTTON42, &CInjectedWnd::OnBt_TraMailInfo)
	ON_BN_CLICKED(IDC_BUTTON43, &CInjectedWnd::OnBtPacket_GetMail)
	ON_BN_CLICKED(IDC_BUTTON44, &CInjectedWnd::OnBtPacket_OpenMail)
	ON_BN_CLICKED(IDC_BUTTON45, &CInjectedWnd::OnBtPacket_DeleteMail)
	ON_BN_CLICKED(IDC_BUTTON46, &CInjectedWnd::OnBt_FarmSeedInfo)
	ON_BN_CLICKED(IDC_BUTTON47, &CInjectedWnd::OnBtPacket_NpcChat)
	ON_BN_CLICKED(IDC_BUTTON48, &CInjectedWnd::OnBt_MouseLeft)
	ON_BN_CLICKED(IDC_BUTTON49, &CInjectedWnd::OnBt_HookFbEnd)
	ON_BN_CLICKED(IDC_BUTTON50, &CInjectedWnd::OnBtPacket_ChooseRole)
	ON_BN_CLICKED(IDC_BUTTON51, &CInjectedWnd::OnBtPacket_RetChooseRole)
	ON_BN_CLICKED(IDC_BUTTON52, &CInjectedWnd::OnBt_TraShopInfo)
	ON_BN_CLICKED(IDC_BUTTON53, &CInjectedWnd::OnBtPacket_BuyItem)
	ON_BN_CLICKED(IDC_BUTTON54, &CInjectedWnd::OnBt_TraCraftInfo)
	ON_BN_CLICKED(IDC_BUTTON55, &CInjectedWnd::OnBtPacket_CraftItem)
	ON_BN_CLICKED(IDC_BUTTON56, &CInjectedWnd::OnBtPacket_LvlUpEquip)
	ON_BN_CLICKED(IDC_BUTTON57, &CInjectedWnd::OnBt_RoleMapInfo)
	ON_BN_CLICKED(IDC_BUTTON58, &CInjectedWnd::OnBtPacket_CreateRole)
	ON_BN_CLICKED(IDC_BUTTON60, &CInjectedWnd::OnBt_PressKey)
	ON_BN_CLICKED(IDC_BUTTON59, &CInjectedWnd::OnBtPacket_LeaveFarm)
	ON_BN_CLICKED(IDC_BUTTON61, &CInjectedWnd::OnBtPacket_EnterFarm)
	ON_BN_CLICKED(IDC_BUTTON62, &CInjectedWnd::OnBtPacket_LvlUpCaiJiDian)
	ON_BN_CLICKED(IDC_BUTTON63, &CInjectedWnd::OnBtPacket_DismSuCai)
	ON_BN_CLICKED(IDC_BUTTON64, &CInjectedWnd::OnBt_HookWuDi)
	ON_BN_CLICKED(IDC_BUTTON65, &CInjectedWnd::OnBt_HookJuQing)
	ON_BN_CLICKED(IDC_BUTTON66, &CInjectedWnd::OnBt_HookRetChooseRole)
	ON_BN_CLICKED(IDC_BUTTON67, &CInjectedWnd::OnBtPacket_AcceptGongHuiTask)
	ON_BN_CLICKED(IDC_BUTTON68, &CInjectedWnd::OnBtPacket_AcceptCatTask)
	ON_BN_CLICKED(IDC_BUTTON69, &CInjectedWnd::OnBtPacket_FindSoldProduct)
	ON_BN_CLICKED(IDC_BUTTON70, &CInjectedWnd::OnBtPacket_NextProductPage)
	ON_BN_CLICKED(IDC_BUTTON71, &CInjectedWnd::OnBtPacket_BuyTheProduct)
	ON_BN_CLICKED(IDC_BUTTON72, &CInjectedWnd::OnBtPacket_RefreshMySellList)
	ON_BN_CLICKED(IDC_BUTTON73, &CInjectedWnd::OnBtPacket_SaleMyProduct)
	ON_BN_CLICKED(IDC_BUTTON74, &CInjectedWnd::OnBtPacket_InviteTeam)
	ON_BN_CLICKED(IDC_BUTTON75, &CInjectedWnd::OnBtPacket_JoinTeam)
	ON_BN_CLICKED(IDC_BUTTON76, &CInjectedWnd::OnBtPacket_LeaveTeam)
	ON_BN_CLICKED(IDC_BUTTON77, &CInjectedWnd::OnBtPacket_RegLeaveTeam)
	ON_BN_CLICKED(IDC_BUTTON78, &CInjectedWnd::OnBtPacket_CreateTeam)
	ON_BN_CLICKED(IDC_BUTTON79, &CInjectedWnd::OnBtPacket_SelectCaptainLine)
	ON_BN_CLICKED(IDC_BUTTON80, &CInjectedWnd::OnBt_FactureInfo)
	ON_BN_CLICKED(IDC_BUTTON81, &CInjectedWnd::OnBtPacket_ActiveGainTasks)
	ON_BN_CLICKED(IDC_BUTTON82, &CInjectedWnd::OnBtPacket_SearchJiYunFb)
	ON_BN_CLICKED(IDC_BUTTON83, &CInjectedWnd::OnBt_HookHitBody)
	ON_BN_CLICKED(IDC_BUTTON84, &CInjectedWnd::OnBt_CalcRoleDirection)
	ON_BN_CLICKED(IDC_BUTTON85, &CInjectedWnd::OnBtPacket_ZuoQiQiu)
	ON_BN_CLICKED(IDC_BUTTON86, &CInjectedWnd::OnBt_FarmCollectPoint)
	ON_BN_CLICKED(IDC_BUTTON87, &CInjectedWnd::OnBt_TraSupply)
	ON_BN_CLICKED(IDC_BUTTON88, &CInjectedWnd::OnBtPacket_GiveUpFb)
	ON_BN_CLICKED(IDC_BUTTON89, &CInjectedWnd::OnBtPacket_PlantSeed)
	ON_BN_CLICKED(IDC_BUTTON90, &CInjectedWnd::OnBt_YouHuaGame)
	ON_BN_CLICKED(IDC_BUTTON91, &CInjectedWnd::OnBtPacket_SendMail)
	ON_BN_CLICKED(IDC_BUTTON92, &CInjectedWnd::OnBt_TraBuffState)
	ON_BN_CLICKED(IDC_BUTTON93, &CInjectedWnd::OnBt_TraBuffInfo)
	ON_BN_CLICKED(IDC_BUTTON94, &CInjectedWnd::OnBt_TraGiftBag)
	ON_BN_CLICKED(IDC_BUTTON95, &CInjectedWnd::OnBtPacket_ReceiveGift)
	ON_BN_CLICKED(IDC_BUTTON96, &CInjectedWnd::OnBt_GetRoleLvlUpInfo)
	ON_BN_CLICKED(IDC_BUTTON97, &CInjectedWnd::OnBtPacket_TidyItem)
	ON_BN_CLICKED(IDC_BUTTON98, &CInjectedWnd::OnBtPacket_GiveUpTask)
	ON_BN_CLICKED(IDC_BUTTON99, &CInjectedWnd::OnBtPacket_LeaveTeachFb)
	ON_BN_CLICKED(IDC_BUTTON100, &CInjectedWnd::OnBtPacket_TeachFb)
	ON_BN_CLICKED(IDC_BUTTON101, &CInjectedWnd::OnBt_HookUseSkillInSky)
	ON_BN_CLICKED(IDC_BUTTON102, &CInjectedWnd::OnBtPacket_GainWeekCredit)
	ON_BN_CLICKED(IDC_BUTTON103, &CInjectedWnd::OnBtPacket_SyncPos)
	ON_BN_CLICKED(IDC_BUTTON104, &CInjectedWnd::OnBt_TidyItem)
	ON_BN_CLICKED(IDC_BUTTON105, &CInjectedWnd::OnBt_HookActorDiMianPos)
	ON_BN_CLICKED(IDC_BUTTON106, &CInjectedWnd::OnBt_TraTaskFbInfo)
	ON_BN_CLICKED(IDC_BUTTON107, &CInjectedWnd::OnBtPacket_DeleteRole)
	ON_BN_CLICKED(IDC_BUTTON108, &CInjectedWnd::OnBtPacket_AddFriend)
	ON_BN_CLICKED(IDC_BUTTON109, &CInjectedWnd::OnBt_ShangJinTaskInfo)
	ON_BN_CLICKED(IDC_BUTTON110, &CInjectedWnd::OnBnClickedButton110)
	ON_BN_CLICKED(IDC_BUTTON111, &CInjectedWnd::OnBnClickedButton111)
	ON_BN_CLICKED(IDC_BUTTON112, &CInjectedWnd::OnBnClickedButton112)
	ON_BN_CLICKED(IDC_BUTTON113, &CInjectedWnd::OnBnClickedButton113)
	ON_BN_CLICKED(IDC_BUTTON114, &CInjectedWnd::OnBnClickedButton114)
	ON_BN_CLICKED(IDC_BUTTON115, &CInjectedWnd::OnBnClickedButton115)
	ON_BN_CLICKED(IDC_BUTTON116, &CInjectedWnd::OnBt_KillCheckThread)
	ON_BN_CLICKED(IDC_BUTTON117, &CInjectedWnd::OnBt_CrcTest)
	ON_BN_CLICKED(IDC_BUTTON118, &CInjectedWnd::OnBt_ResumeCheckThread)
	ON_BN_CLICKED(IDC_BUTTON119, &CInjectedWnd::OnBt_HookGetTickCount)
	ON_BN_CLICKED(IDC_BUTTON120, &CInjectedWnd::OnBt_HookKiUser)
	ON_BN_CLICKED(IDC_BUTTON121, &CInjectedWnd::OnBt_WriteCheck)
	ON_BN_CLICKED(IDC_BUTTON122, &CInjectedWnd::OnBt_HookEnumMod)
	ON_BN_CLICKED(IDC_BUTTON123, &CInjectedWnd::OnBt_TraverseCollect)
	ON_BN_CLICKED(IDC_BUTTON124, &CInjectedWnd::OnBt_CallLpkExternFn)
	ON_BN_CLICKED(IDC_BUTTON125, &CInjectedWnd::OnBt_HookJingCaiJingTou)
	ON_BN_CLICKED(IDC_BTN_UITEST, &CInjectedWnd::OnBt_UiTest)
	ON_BN_CLICKED(IDC_BTN_UITEST2, &CInjectedWnd::OnBt_UiPressTest)
	ON_BN_CLICKED(IDC_BTN_UITEST3, &CInjectedWnd::OnBt_UiInputTest)
	ON_BN_CLICKED(IDC_BTN_UITEST4, &CInjectedWnd::OnBt_InputTest1)
	END_MESSAGE_MAP()


// CInjectedWnd 消息处理程序
void CInjectedWnd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CInjectedWnd::OnBtPacket_HanHua()
{
	// TODO: 在此添加控件通知处理程序代码

	if( MMO_say("123456789",1) )
	{
		AfxMessageBox(_T("MMO_say fail"));
	}
}


void CInjectedWnd::OnBt_HookSend()
{
	// TODO: 在此添加控件通知处理程序代码
	theMmoUnSendPacket.MmoHookSend();
}

void CInjectedWnd::OnBtPacket_Walk()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_walk(406.857758f,359.702362f,89.000000f);
// 	__asm
// 	{
// 		mov edx,dword ptr ds:[MMOCLIENT_REVERSAL_SOCKET_ADDR]         ; MHOClien.0209D0A0
// 		mov ecx,dword ptr ds:[edx+0x4]           ; MHOClien.00C34350
// 		mov eax,dword ptr ds:[ecx]               ; MHOClien.01E2A298
// 		mov edx,dword ptr ds:[eax+0x38]
// 		call edx                                 ; MHOClien.01E2A298
// 		mov edx,dword ptr ds:[eax]
// 		mov ecx,eax                              ; MHOClien.00C34B00
// 		mov eax,dword ptr ds:[edx+0x228]         ; MHOClien.00C34B00
// 		call eax                                 ; MHOClien.00C34B00
// 	}
}

void CInjectedWnd::OnBt_TraObj()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTxt = TraverseObj();
	//csTxt += TraversePlayer();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBt_HookModifyHitBody()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_HookHitParameter();
}

void CInjectedWnd::OnBt_UnHook()
{
	// TODO: 在此添加控件通知处理程序代码

	theMmoUnRecvPacket.MmoUnHookRecv();

	theMmoUnSendPacket.MmoUnHookSend();
	
	MMO_UnHookMonsterRemoteHit();
	{
		// 加密call hook 55 8B EC 8B 45
		byte bCode[] = {0x55 ,0x8B ,0xEC ,0x8B ,0x45};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_ENCRYPTION_CALL+GetCryGameBaseAddr(),bCode,5);
	}
	{
		// 角色减血hook	55 8B EC 83 EC
		byte bCode[] = {0x55 ,0x8B ,0xEC ,0x83 ,0xEC};
		//byte bCode[] = {0x83 ,0xEC ,0x24 ,0x56 ,0x8B};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_ACTOR_FEEB_HP_CALL_HOOK+GetCryGameBaseAddr(),bCode,5);
	}

	{
		// 怪物减血hook	55 8B EC 83 7D
		// 55 8B EC 83 EC
		byte bCode[] = {0x55 ,0x8B ,0xEC ,0x83 ,0xEC};
		//byte bCode[] = {0x55 ,0x8B ,0xEC ,0x83 ,0x7D};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK+GetCryGameBaseAddr(),bCode,5);
	}
	{
		// 怪物减血hook	55 8B EC 83 EC
		byte bCode[] = {0x55 ,0x8B ,0xEC ,0x83 ,0xEC};
		//byte bCode[] = {0x83 ,0x7C ,0x24 ,0x08 ,0x00 ,0x56};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_ACTOR_HIT_HP_CALL_HOOK+GetCryGameBaseAddr(),bCode,5);
	}
	
	{
		byte bCode[] = {0xC3};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_CRYMALLOC_FUNADDR+GetCryGameBaseAddr(),bCode,1);
		_WriteProcessMemory(MMOCLIENT_REVERSAL_INFO_HOOK+GetCryActionBaseAddr(),bCode,1);
	}
	{
		byte bCode[] = {0x8B ,0x10 ,0x8B ,0xC8 ,0x8B};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK+GetCryGameBaseAddr(),bCode,5);
	}
	{
		byte bCode[] = {0x8B ,0xCB ,0x89 ,0xB5 ,0x7C};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK2+GetCryGameBaseAddr(),bCode,5);
	}
	{
		byte bCode[] = {0xC2 ,0x0C ,0x00};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_FB_GAME_OVER_HOOK+GetCryGameBaseAddr(),bCode,3);
	}
	{
		// 5B 8B E5 5D C3
		byte bCode[] = {0x5B ,0x8B ,0xE5 ,0x5D ,0xC3};
		//byte bCode[] = {0x5D ,0x5B ,0x83 ,0xC4 ,0x14};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_HOOK+GetCryGameBaseAddr(),bCode,5);
	}
	{
		//8B 91 C8 00 00
		byte bCode[] = {0x8B ,0x91 ,0xC4 ,0x00 ,0x00};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_HIT_BU_WEI_HOOK,bCode,5);
	}
	{
		// 55 8B EC 53 56
		//byte bCode[] = {0x55 ,0x8B ,0xEC ,0x53 ,0x56};
		//_WriteProcessMemory(MMOCLIENT_REVERSAL_HIT_PARAMETER_CALL_HOOK,bCode,5);
	}
	{
		// 8B 4E 54 8D 46
		byte bCode[] = {0x8B ,0x4E ,0x54 ,0x8D ,0x46};
		_WriteProcessMemory(MMOCLIENT_REVERSAL_DIALOG_HOOK,bCode,5);
	}

	MMO_UnHookHit();
	MMO_UnHookMonsterHit();

	{
		DWORD dwHookDecryAddr = MMOCLIENT_REVERSAL_DECRY_CALL+GetCryGameBaseAddr()+0x7A;

		byte bCode[] = {0xC3};
		_WriteProcessMemory(dwHookDecryAddr,bCode,1);
	}
}



void CInjectedWnd::OnBt_SkillCall()
{
	// TODO: 在此添加控件通知处理程序代码
// 	01663BEE       8B8E 34030000                 MOV ECX,DWORD PTR DS:[ESI+0x334]         ; 2F104F98
// 		01663BF4       50                            PUSH EAX                                 ; 535F0C00
// 		01663BF5       E8 8FE64901                   CALL MHOClien.02B02289
	int nSkillId = GetDlgItemInt(IDC_EDIT_DECRY);
	MMOCall theCall;

	for (int i=0; i<100; i++)
	{
		theCall.useskill(nSkillId);
		Sleep(500);
	}


}

void PrintDecryCallBack(DWORD dwCallBack,int nRetValue)
{
	if( nRetValue <= 20 &&  nRetValue >= 10 )
	{
		char szPrint[256] = {0};
		sprintf_s(szPrint,"[LOG] call = 0x%p ret = %d",dwCallBack,nRetValue);
		OutputDebugStringA(szPrint);
	}
}

_declspec(naked) void HookDecry()
{
	__asm
	{
		pushfd
		pushad

		push eax
		mov eax, [esp+0x24+4]
		push eax
		call PrintDecryCallBack
		add esp, 8

		popad
		popfd
		retn
	}
}


/*++
	函数功能：比较两浮点的大小
	返回值：	1	==》  x大于y
				-1	==》  x小于y
				0	==》  x等于y
--*/
int CompareDouble(double x,double y)
{
	char szDoubleX[256];
	char szDoubleY[256];

	if( int(x) > int(y) )
	{
		return 1;
	}

	if( int(x) < int(y) )
	{
		return -1;
	}

	// 当整数相同时，才比较小数点位
	sprintf_s(szDoubleX,"[D2] %f",x);
	//OutputDebugString(szDoubleX);
	sprintf_s(szDoubleY,"[D2] %f",y);
	//OutputDebugString(szDoubleY);
	return strcmp(szDoubleX,szDoubleY);
}

DWORD	g_dwEncryptionValue = -1;



void DoCountDown(char* pszCountDownStr)
{
	try
	{
		char szPrint[1024] = {0};
		sprintf_s(szPrint,"[LOG] %s",pszCountDownStr);
		OutputDebugStringA(szPrint);
	}
	catch (...)
	{
		
	}
	
	/*++
	[5108] [LOG] 10秒后狩猎结束
	[5108] [LOG] 9秒后狩猎结束
	[5108] [LOG] 8秒后狩猎结束
	[5108] [LOG] 7秒后狩猎结束
	[5108] [LOG] 6秒后狩猎结束
	[5108] [LOG] 5秒后狩猎结束
	[5108] [LOG] 4秒后狩猎结束
	[5108] [LOG] 3秒后狩猎结束
	[5108] [LOG] 2秒后狩猎结束
	[5108] [LOG] 1秒后狩猎结束

	[9772] [LOG] 您前面还有196名玩家正在排队
	[9772] [LOG] 普通玩家需要等待3分钟

	--*/
}

_declspec(naked) void HookCountDown()
{
	__asm
	{
		pushfd
		pushad

		mov eax, [ebp+0x0C]
		push eax
		mov eax, DoCountDown
		call eax
		add esp, 4

		popad
		popfd

		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN
	}

}

void CInjectedWnd::OnBt_WangLiFb()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT_SHOW,GetKingQuestInfo());





	//Hook(MMOCLIENT_REVERSAL_COUNT_DOWN_HOOK+GetCryGameBaseAddr(),HookCountDown);

// 	DWORD dwObjAddr = GetObjAddr(0x01000002);
// 
// 	char szPrint[256] = {0};
// 	sprintf_s(szPrint,"%p",dwObjAddr);
// 	SetDlgItemText(IDC_EDIT_SHOW,szPrint);
// 
// 	g_dwEncryptionValue = GetDlgItemInt(IDC_EDIT_DECRY);

// 		MMOActor theActor;
// 		theActor.ReadActor();
// 		theActor.WirtePos(0.0f,0.0f,0.0f,0.0f);
// 	{
// 		MMOActor theActor;
// 		theActor.ReadActor();
// 		theActor.WirtePos(1412.212036f,887.768250f,333.177734f,0.0f);
// 	}
// 	Sleep(5000);
// 	{
// 		MMOActor theActor;
// 		theActor.ReadActor();
// 		theActor.WirtePos(1404.096802f,963.444458f,324.926697f,0.0f);
// 	}
// 	extern DWORD g_dwGongJiBuWei;
// 
// 	g_dwGongJiBuWei = GetDlgItemInt(IDC_EDIT_DECRY);
// 	char szPrint[256];
// 	sprintf_s(szPrint,"[LOG] 修改攻击部位为:0x%p",GetWeekCreditCount());
// 	OutputDebugStringA(szPrint);

	//WritePos(846.515198f,986.340027f,19.078125f);

// 	MMO_AddBuffer(0x00002AF7);	// 攻击增加100%
//  MMO_AddBuffer(0x000029AF );	
// 	MMO_AddBuffer(0x04C4B404);	// 攻击力提升30%
// 	MMO_AddBuffer(0x00D59FE5);
// 	MMO_AddBuffer(0x00D5A369);
// 	MMO_AddBuffer(0x00004280);
// 	MMO_AddBuffer(0x00002AF7);
//	MMO_AddBuffer(MMOCLIENT_REVERSAL_SKILL_BUFF_ID_F);	// 奥义觉醒


	//MMO_HookAddBuffer();

// 	MMO_AddBuffer(0x00D5A36F);	// 大工灵药
// 	MMO_AddBuffer(0x00D5A369);	// 攻击力提升
// 	MMO_AddBuffer(0x00D59F8C);	// 锋利度
}




void PrintRecv22( int nLen)
{
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] recv len = %d",nLen);
	OutputDebugStringA(szPrint);
}

_declspec(naked) void HookRecv22()
{
	__asm
	{
		ADD ESP,0x10C

		pushfd
		pushad

		
		push eax
		call PrintRecv22
		add esp, 4

		popad
		popfd

		RETN
	}
}

void CInjectedWnd::OnBt_HookRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	//Hook(0x01D3A8D6,HookRecv22);
	theMmoUnRecvPacket.MmoHookRecv();
}

void CInjectedWnd::OnBt_HookDecrypt()
{
	// TODO: 在此添加控件通知处理程序代码
// 	Hook(0x01CF433B,HookDecry);
// 	//004673C7    C3              RETN

	DWORD dwHookDecryAddr = MMOCLIENT_REVERSAL_DECRY_CALL+GetCryGameBaseAddr()+0x7A;
	Hook(dwHookDecryAddr,HookDecry);	// 0046A5A7    C3              RETN
}



std::list<DWORD> decrylist;



void CInjectedWnd::OnBt_HookDlgInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	HookDialogInfoCall();
}

void CInjectedWnd::OnBt_TraDlgInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetCDialogInfo());
}

void CInjectedWnd::OnBt_TraItem()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTxt = TraverseItem();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBtPacket_AcceptTask()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_accepttask(0x000008C6);
}

DWORD g_dwHookTask = 0x006D562D; //    8DBB 28040000   LEA EDI,DWORD PTR DS:[EBX+0x428]         ; ebx = 对象地址



void PrintTest(DWORD dwObjAddr, int nhp)
{
	char szPrint[256] = {0};
	sprintf_s(szPrint,"[LOG] obj id = 0x%p hp = %d",*(DWORD*)(dwObjAddr+4),nhp);
	OutputDebugStringA(szPrint);
}

_declspec(naked) void HookTest()
{
	__asm
	{
		LEA EDI,DWORD PTR DS:[EBX+0x428]

		pushfd
		pushad

		mov eax, [eax]
		push eax
		push ebx
		call PrintTest
		add esp, 8

		popad
		popfd

		mov eax, g_dwHookTask
		add eax, 6
		jmp eax
	}
}

void CInjectedWnd::OnBt_HookActorHpCall()
{
	// TODO: 在此添加控件通知处理程序代码
	//Hook(g_dwHookTask,HookTest);
	HookActorHpCall();
}

void CInjectedWnd::OnBt_TraTaskInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTxt = GetTaskInfo();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBt_TraReceivedTask()
{
	// TODO: 在此添加控件通知处理程序代码
// 	CString csTxt = GetReceivedTaskInfo();
// 	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
	CString csTxt = TraverseReceivedTask();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBt_TraUiInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTxt = GetUiInfo();		// CMessageInfo
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);

}

void PrintEncryptionCallBack(DWORD dwCallBack, DWORD dwEncryptionAddr, DWORD dwRetValue)
{
	try
	{

		// if( 1000 < dwRetValue && dwRetValue <= 0x10000  )
		if( 10 < dwRetValue && dwRetValue <= 20  )
		{
			//if( 0x00662AB0 != dwCallBack && 0x005DAA27 != dwCallBack )
			{
				char szPrint[256] = {0};
				sprintf_s(szPrint,"[LOG] call = 0x%p addr = 0x%p ret = %d",dwCallBack,dwEncryptionAddr,dwRetValue);
				OutputDebugStringA(szPrint);
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	
}

DWORD dwJmpCall = MMOCLIENT_REVERSAL_ENCRYPTION_CALL+6;
_declspec(naked) void HookEncryption()
{
	
	__asm
	{
		pushfd
		pushad

		mov eax, [esp+0x24+4]
		mov eax, [eax]
		mov edx, [esp+0x24]
		push eax
		push ecx
		push edx
		call PrintEncryptionCallBack
		add esp, 0x0C

		popad
		popfd
		
		PUSH EBP
		MOV EBP,ESP
		MOV EAX,DWORD PTR SS:[EBP+0x8]
		
		jmp DWORD PTR [dwJmpCall]
	}
}


void CInjectedWnd::OnBt_HookEncrypt()
{
	// TODO: 在此添加控件通知处理程序代码
	// MMOCLIENT_REVERSAL_ENCRYPTION_CALL___
	dwJmpCall += GetCryGameBaseAddr();
	Hook(MMOCLIENT_REVERSAL_ENCRYPTION_CALL+GetCryGameBaseAddr(),HookEncryption);
}

void CInjectedWnd::OnBt_HookMonsterHpCall()
{
	// TODO: 在此添加控件通知处理程序代码
	HookMonsterHpCall();
}

//////////////////////////////////////////////////////////////////////////
// CryMalloc 清0


void CryMalloc(DWORD dwCallBackAddr,LPVOID LPMallocAddr,int nLen)
{
	if( nLen >= 0xE0 )
	{
		char szPrint[256];
		sprintf_s(szPrint,"[LOG] call = 0x%p addr = 0x%p len = 0x%p",dwCallBackAddr,(DWORD)LPMallocAddr,nLen);
		OutputDebugStringA(szPrint);
	}
	memset(LPMallocAddr,0,nLen);
}

__declspec(naked) void hook_CryMalloc()
{
	__asm
	{
		pushfd
		pushad

		mov edx, [esp+0x24]
		mov ebx, [esp+0x24+4]
		push ebx
		push eax
		push edx
		call CryMalloc
		add esp, 0x0C

		popad
		popfd

		retn 
	}
}


void CInjectedWnd::OnBt_HookCryMalloc()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(MMOCLIENT_REVERSAL_CRYMALLOC_FUNADDR+GetCryGameBaseAddr(),hook_CryMalloc);
}

void CInjectedWnd::OnBtPacket_OpenNpc()
{
	// TODO: 在此添加控件通知处理程序代码

	// 打开交纳箱 需要角色在交纳箱附近
	MMO_OpenNpc(0x10008C7B,0x0A);

	// 打开npc 
	MMO_OpenNpc(0x10008C7C);
}

void CInjectedWnd::OnBtPacket_GuoTu()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_Vector3 theDoorCoord;
	theDoorCoord.fx = 1568.248f;
	theDoorCoord.fy = 1592.373f;
	theDoorCoord.fz = 142.3750f;
	char szGoTuCmd[] = "SwitchRegionTrigger6to3";			// SwitchRegionTrigger1toC    C表示起点的房间，其他全部是界面上显示的房间序号，此处发包表示 C to 1
	MMO_FbGuoTu(theDoorCoord,szGoTuCmd);
}


void CInjectedWnd::OnBt_TraAcceptableTask()
{
	// TODO: 在此添加控件通知处理程序代码

	CString csTxt = GetReceivedNpcTask();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}


void PrintInfoCallBack(DWORD dwCallBack, DWORD dwInfoAddr, char* pszInfoCmd)
{
	try
	{
		if( strcmp(pszInfoCmd,"CEquipEnforceBreak") != 0 && 
			strcmp(pszInfoCmd,"CMessageParmDescInfo") != 0 &&
			strcmp(pszInfoCmd,"CPetAvatarInfo") != 0 &&
 			strcmp(pszInfoCmd,"CMHLevelInfo") != 0 &&
 			strcmp(pszInfoCmd,"CNewSkillInfo") != 0 &&
 			strcmp(pszInfoCmd,"CSystemUnlockInfo") != 0 &&
 			strcmp(pszInfoCmd,"CConstTable") != 0 &&
 			strcmp(pszInfoCmd,"CItemBaseInfo") != 0 &&
			strcmp(pszInfoCmd,"CMessageInfo") != 0 &&
 			strcmp(pszInfoCmd,"CUnderclothesInfo") != 0 &&
 			strcmp(pszInfoCmd,"CEquipInfo") != 0 &&
 			strcmp(pszInfoCmd,"CFaceInfo") != 0 &&
 			strcmp(pszInfoCmd,"CHairInfo") != 0 &&
 			strcmp(pszInfoCmd,"CAvatarInfo") != 0 && 			
 			strcmp(pszInfoCmd,"CBuffFilterInfo") != 0 &&
 			strcmp(pszInfoCmd,"CBuffInfo") != 0 &&
 			strcmp(pszInfoCmd,"CCalculateInfo") != 0 &&
 			strcmp(pszInfoCmd,"CFarmFacilityInfo") != 0 &&
 			strcmp(pszInfoCmd,"CFarmCPInfo") != 0 && 
 			strcmp(pszInfoCmd,"CTitleInfo") != 0 )

// 		if( strcmp(pszInfoCmd,"CUIOpenProcessInfo") == 0 ||
// 			strcmp(pszInfoCmd,"CMessageParmDescInfo") == 0 )
		{
			char szPrint[256] = {0};
			sprintf_s(szPrint,"[LOG] call = 0x%p addr = 0x%p Info = %s",dwCallBack,dwInfoAddr,pszInfoCmd);
			OutputDebugStringA(szPrint);
		}
	}
	catch (...)
	{
		assert(false);
	}
}

_declspec(naked) void HookInfo()
{
	__asm
	{
		pushfd
		pushad

		mov ebx, [esp+0x24+8]
		mov edx, [esp+0x24]
		push ebx
		push eax
		push edx
		call PrintInfoCallBack
		add esp, 0x0C

		popad
		popfd

		RETN 
	}
}

void CInjectedWnd::OnBt_HookTemplateInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(MMOCLIENT_REVERSAL_INFO_HOOK+GetCryActionBaseAddr(),HookInfo);
}



CString GetAllObjMsg()
{
	CString csRet;

	try
	{
		DWORD	dwBeginListAddr = MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_ADDR+GetCryGameBaseAddr();

		DWORD	dwNextListAddr = *(DWORD*)(dwBeginListAddr);

		char szPrint[256] = {0};
		int i = 0;
		if( dwBeginListAddr != dwNextListAddr )
		{
			do
			{
				DWORD dwObjMsgAddr = *(DWORD*)(dwNextListAddr+MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_OFFSET);

				if( dwObjMsgAddr != 0 )
				{
					char* pszObjMsgName = (char*)*(DWORD*)(dwObjMsgAddr+MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_MSG_NAME___);

					if( pszObjMsgName != NULL )
					{
						sprintf_s(szPrint,"[LOG] Index = %d addr = 0x%p msg = %s\r\n",++i,dwObjMsgAddr,pszObjMsgName);
						OutputDebugStringA(szPrint);
						csRet += szPrint;
					}
				}

				dwNextListAddr = *(DWORD*)dwNextListAddr;

			}while( dwBeginListAddr != dwNextListAddr );
		}

	}
	catch (...)
	{
		assert(false);
	}
	return csRet;
}




void CInjectedWnd::OnBt_TraUnknownObj()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetAllObjMsg());
}



void CInjectedWnd::OnBt_TraAllShiXianObj()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetSuoYouShiXianDuiXiang());
}

void CInjectedWnd::OnBt_TraZhuiZongTask()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetReceivedTaskInfo());
}

void CInjectedWnd::OnBtCityGuoTu()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_CityGuoTu(MMOCLIENT_REVERSAL_MI_DE_LA_CUN_TO_XI_MEI_LUN_SHAN_LU);
}

void CInjectedWnd::OnBtPacket_CommitTask()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_completetask(0x0000000A);
}

void CInjectedWnd::OnBtPacket_CollectItem()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CInjectedWnd::OnBt_TraFbInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetInFbInfo());
}

void CInjectedWnd::OnBtPacket_EnterFb()
{
	// TODO: 在此添加控件通知处理程序代码
	// [LOG] addr = 0x34599CD0 id = 0x000449DE map id = 0x00000579 level = 37 name = [狩猎祭第二日]
	// [LOG] addr = 0x319BACD0 id = 0x00018871 map id = 0x000003EC level = 5 name = 闪光的原因
	// [LOG] addr = 0x2EEA49A0 id = 0x00033459 map id = 0x000003F2 level = 48 name = [火热的猎场]

	/*++
	[10500] [LOG] call = 0x0D0A6251 msg id = 00000019
	06718034  00000019
	06718038  00000000
	0671803C  00000000
	06718040  00000000
	06718044  00000001
	06718048  00000000
	0671804C  00018764
	06718050  00000001
	06718054  00000017

	[10500] [LOG] call = 0x0D0A6336 msg id = 00000021

	06718AFC  00000021
	06718B00  00000000
	06718B04  00000000
	06718B08  00000000
	06718B0C  00018764
	06718B10  00000001

	[10500] [LOG] call = 0x0D0A63ED msg id = 00000023
	06718A08  00000023
	06718A0C  00000000
	06718A10  00000000
	06718A14  00000000
	06718A18  00000001

	大怪鸟挑战  gotofb1
	$ ==>    >00000019
	$+4      >00000000
	$+8      >00000000
	$+C      >00000000
	$+10     >00000001
	$+14     >00000000
	$+18     >00018774
	$+1C     >00000002	// 挑战=2 试炼=1
	$+20     >00000001	// 普通副本


	--*/
	MMO_GotoFb1(0x31CFC,2,4 );
	Sleep(1000);
	MMO_GotoFb2(0x31CFC,2);
	MMO_GotoFbChooseMode(0x31CFC,1);
	Sleep(2000);
	
	MMO_GotoFbChooseMode(0x31CFC,1);
	MMO_GotoFb3(MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_PU_TONG___);
}

void CInjectedWnd::OnBtPacket_CunQuStore()
{
	// TODO: 在此添加控件通知处理程序代码
// 	MMO_ITEM_MOVE_TO theItemMoveTo;
// 
// 	theItemMoveTo.dwKey1 = 0x92704144;
// 	theItemMoveTo.dwKey2 = 0x780554A9;
// 	theItemMoveTo.bScrItemTabIndex = 0;		// 背包 = 0， 仓库 = 2， 装备 = 3， 素材仓库 = 0x0A
// 	theItemMoveTo.wScrItemSlotIndex = 10;
// 	theItemMoveTo.bDstItemTabIndex = 2;
// 	theItemMoveTo.wDstItemSlotIndex = 73;
// 
// 	// 支持仓库存取，装备穿戴
// 	MMO_ItmeMoveTo(theItemMoveTo);


// 	{
// 
// 
// 		MMO_ITEM_MOVE_TO_COUNT theItemMoveTo;
// 
// 		theItemMoveTo.dwMoveItemKey1 = 0x4F000000;
// 		theItemMoveTo.dwMoveItemKey2 = 0xE8837FD0;
// 		theItemMoveTo.bScrItemTabIndex = 0;		// 背包 = 0， 仓库 = 2， 装备 = 3， 素材仓库 = 0x0A
// 		theItemMoveTo.wScrItemSlotIndex = 1;
// 		theItemMoveTo.wMoveItmeCount = 2;		// 移动的物品数量
// 		theItemMoveTo.bDstItemTabIndex = 2;
// 		theItemMoveTo.wDstItemSlotIndex = 1;
// 		theItemMoveTo.dwToItemKey1 = 0x7701340D ;
// 		theItemMoveTo.dwToItemKey2 = 0x56836CD0 ;
// 
// 		MMO_ItmeCountMoveTo(theItemMoveTo);
// 	}

	{
		MMO_ITEM_MOVE_TO_COUNT theItemMoveTo;

		theItemMoveTo.dwMoveItemKey1 = 0x7701340D;
		theItemMoveTo.dwMoveItemKey2 = 0x56836CD0;
		theItemMoveTo.bScrItemTabIndex = 2;		// 背包 = 0， 仓库 = 2， 装备 = 3， 素材仓库 = 0x0A
		theItemMoveTo.wScrItemSlotIndex = 1;
		theItemMoveTo.wMoveItmeCount = 2;		// 移动的物品数量
		theItemMoveTo.bDstItemTabIndex = 0;
		theItemMoveTo.wDstItemSlotIndex = 1;
		theItemMoveTo.dwToItemKey1 = 0 ;		// 空格位置
		theItemMoveTo.dwToItemKey2 = 0 ;

		MMO_ItmeCountMoveToEmpty(theItemMoveTo);
	}
}

void CInjectedWnd::OnBt_ItemTemplateInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetTemplateItemInfo());
}

void CInjectedWnd::OnBtPacket_GainSupply()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_DoBuJiXiang(0x2C,1,1);
}

void CInjectedWnd::OnBt_HookOptimizeYouHua()
{
	// TODO: 在此添加控件通知处理程序代码
	Optimize_Cutscene();
}

void CInjectedWnd::OnBt_UseItem()
{
	// TODO: 在此添加控件通知处理程序代码
	UseItem(5,7);
}

void CInjectedWnd::OnBtPacket_MakeItem()
{
	// TODO: 在此添加控件通知处理程序代码
	//MMO_facture();

	MMO_ManuFactureItem(2,1);
}

void CInjectedWnd::OnBt_HookLoadEndMap()
{
	// TODO: 在此添加控件通知处理程序代码
	HookLoadMapCallEnd();
}

void CInjectedWnd::OnBtPacket_SellItem()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_SellItem(0,0,1);
}

void CInjectedWnd::OnBt_TraMailInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetMailInfo());
}

void CInjectedWnd::OnBtPacket_GetMail()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_GetMailItem(0x567B75DD0000049A,1);		// 获取邮件物品
	MMO_GetMailItem(0x567B75BB00000489,1);		// 获取邮件物品
}

void CInjectedWnd::OnBtPacket_OpenMail()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_OpenMail();		// 在和NPC对话的情况下，此封包才有效，这时候遍历邮件才会有
}

void CInjectedWnd::OnBtPacket_DeleteMail()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_DeleteMail(0x554E90CF0009E38B);
}

void CInjectedWnd::OnBt_FarmSeedInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	//SetDlgItemText(IDC_EDIT_SHOW,GetCollectObjAddr2());   // 放弃此遍历方式
	SetDlgItemText(IDC_EDIT_SHOW,GetFarmSeedInfo());
}

void CInjectedWnd::OnBtPacket_NpcChat()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_TaskNpcChat(0x00000BBD);
}


void CInjectedWnd::OnBt_MouseLeft()
{
	// TODO: 在此添加控件通知处理程序代码
// 	int x = 741;
// 	int y = 434;
	int x = 509;
	int y = 395;
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONDOWN);
	Sleep(100);
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONUP);
}



void FbGameOver()
{
	OutputDebugStringA("[LOG] 猎守副本结束!");

	// 等待60秒

	/*++
	精彩时刻，关卡结算，鼠标按下 和离开关卡
	int x = 674;
	int y = 644;
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONDOWN);
	Sleep(100);
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONUP);

	int x = 679;
	int y = 619;
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONDOWN);
	Sleep(100);
	mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONUP);
	--*/
}

_declspec(naked) void HookFbGameOver()
{
	__asm
	{
		pushfd
		pushad

		call FbGameOver

		popad
		popfd
		
		mov esp, ebp
		pop ebp	
		retn 0x0C
	}
}

void CInjectedWnd::OnBt_HookFbEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(MMOCLIENT_REVERSAL_FB_GAME_OVER_HOOK+GetCryGameBaseAddr(),HookFbGameOver);
}

void CInjectedWnd::OnBtPacket_ChooseRole()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_ChooseRole(0x001119CA );
	MMO_ChooseLine(5);
}

void CInjectedWnd::OnBtPacket_RetChooseRole()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_RetutnChooseRole(0x0020EABD );
}


void CInjectedWnd::OnBt_TraShopInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csTxt;

	char szPrint[256] = {0};

	for (int i=0; i<50; i++)
	{
		sprintf_s(szPrint,"[LOG] ************** %d *******************\r\n",i);
		csTxt += szPrint;
		csTxt += GetShopInfo(i);
	}
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);

	return;

	csTxt = "******************猎守补给品****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_LIE_SHOU_BU_JI_PIN);
	csTxt += "******************艾露猫雇佣****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_AI_LU_MAO_GU_YONG);
	csTxt += "******************艾露农业特供****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_AI_LU_NONG_YE_TE_GONG);
	csTxt += "******************新手猎守装备****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_XIN_SHOU_LIE_SHOU_EQUIP);
	csTxt += "******************庄园必须品****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_ZHUANG_YUAN_BI_XU_PIN);
	csTxt += "******************弹药补给****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_DAN_YAO_BU_JI);
	csTxt += "******************弓箭瓶特卖****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_GONG_JIAN_PING_TE_MAI);
	csTxt += "******************教官-猎守武器入门许可****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_RU_MEN_XU_KE);
	csTxt += "******************教官-武器技能****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_EQUIP_SKILL);
	csTxt += "******************神秘的商人-莉薇****************\r\n";
	csTxt += GetShopInfo(MMOCLIENT_SHOP_INFO_LI_WEI);
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBtPacket_BuyItem()
{
	// TODO: 在此添加控件通知处理程序代码

	// 买不同类型的物品，需要将 MMOCLIENT_SHOP_INFO_EQUIP_SKILL 传入
	MMO_BuyItem(0x00000005 ,1,MMOCLIENT_SHOP_INFO_TE_CAN_SHANG_DIAN);		// 不需要打开NPC，直接就可以购买物品，测试教官是需要打开NPC才可以购买物品的
}

void CInjectedWnd::OnBt_TraCraftInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetCraftInfo());
}

void CInjectedWnd::OnBtPacket_CraftItem()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_CraftItem(0x000003E9);		// 不需要打开NPC，直接就可以锻造物品
}

void CInjectedWnd::OnBtPacket_LvlUpEquip()
{
	// TODO: 在此添加控件通知处理程序代码

	MMO_SEND_UP_EQUIP_PACKET theUpEquip;
	theUpEquip.bSoltTableIndex = 0;
	theUpEquip.wSoltIndex = 4;
	theUpEquip.dwEquipItemId = 0x0CB31;
	theUpEquip.llUpEquipSeiral = 0xD13557C0C2704145;
	MMO_UpEquip(theUpEquip);   // 需要打开NPC，才有效
}

void CInjectedWnd::OnBt_RoleMapInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	MMOActor theActor;
	theActor.ReadActor();
	SetDlgItemText(IDC_EDIT_SHOW,theActor.GetRoleInMap());
}

void CInjectedWnd::OnBtPacket_CreateRole()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_CreateRole("d地方的来看");
}

void PressKey(DWORD	dwVk)
{
	LPARAM lParam = 1;
	lParam += MapVirtualKey(dwVk, MAPVK_VK_TO_VSC) << 16;

	HWND hWnd = ::FindWindowA("CryENGINE","怪物猎人Online");

	if( hWnd == NULL )
	{
		return ;
	}
	__asm
	{
		mov edi, lParam
		push edi			// 00020001
		mov ebx, dwVk
		push ebx			// 00000031
		mov esi, WM_KEYDOWN		// 00000100
		push esi
		mov eax, hWnd
		push eax			// 000503AA
		mov eax, 0x01516C00
		CALL EAX
	}

// 	__asm
// 	{
// 		mov edi, lParam
// 		push edi			// 00020001
// 		mov ebx, dwVk
// 		push ebx			// 00000031
// 		mov esi, WM_CHAR		// 00000102
// 		push esi
// 		mov eax, hWnd
// 		push eax			// 000503AA
// 		mov eax, 0x01516C00
// 		CALL EAX
// 	}

	lParam += 1 << 30;
	lParam += 1 << 31;

	Sleep(500);

	__asm
	{
		mov edi, lParam
		push edi			// 00020001
		mov ebx, dwVk
		push ebx			// 00000031
		mov esi, WM_KEYUP		// 00000101
		push esi
		mov eax, hWnd
		push eax			// 000503AA

		mov eax, 0x01516C00
		CALL EAX
	}
}



void CInjectedWnd::OnBt_PressKey()
{
	// TODO: 在此添加控件通知处理程序代码
	//PressKey(VK_W);
	//Hook(MMOCLIENT_REVERSAL_GETDEVICEDATE_HOOK___,HookGetDeviceData);

	for (int i=0; i<10; i++)
	{
		DxPressKey(DIK_ESCAPE,1);
		Sleep(100);
		DxPressKey(DIK_ESCAPE,0);

		Sleep(500);

		int x = 504;
		int y = 397;
		mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONDOWN);
		Sleep(100);
		mouse_down(x,y,MMOCLIENT_REVERSAL_LBUTTONUP);

// 		Sleep(1000);
	}

	


// 	Sleep(2000);
// 
// 	DxPressKey(DIK_GRAVE,1);
// 	Sleep(1000);
// 	DxPressKey(DIK_GRAVE,0);

// 	DxPressKey(DIK_A,1);
// 	Sleep(1000);
// 	DxPressKey(DIK_A,0);
// 
// 	DxPressKey(DIK_S,1);
// 	Sleep(1000);
// 	DxPressKey(DIK_S,0);
// 
// 	DxPressKey(DIK_D,1);
// 	Sleep(1000);
// 	DxPressKey(DIK_D,0);
}

void CInjectedWnd::OnBtPacket_LeaveFarm()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_LeaveZhuangYuan();
}

void CInjectedWnd::OnBtPacket_EnterFarm()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_GotoZhuangYuan(GetZhuangYuanId());
}

void CInjectedWnd::OnBtPacket_LvlUpCaiJiDian()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_UpCollect(3);   // 从0开始按顺序来数，育菇木床
}

void CInjectedWnd::OnBtPacket_DismSuCai()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_DISMANTLING theDismantling;

	theDismantling.nDismantlingCount = 1;

	theDismantling.theDismantling->dwItemKey1 = 0xA7A6D144;
	theDismantling.theDismantling->dwItemKey2 = 0x39955516;
	theDismantling.theDismantling->bItemTabIndex = 0;
	theDismantling.theDismantling->wItemSlotIndex = 17;
	theDismantling.theDismantling->wItemCount = 1;

	MMO_Dismantling(theDismantling);
}



void CInjectedWnd::OnBt_HookWuDi()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_HookMonsterHit();
	MMO_HookMonsterRemoteHit();
}



void Dosprintf_s(char* pszPrint)
{
	try
	{
		if( pszPrint !=  NULL )
		{
			char szPrint[1024];
			sprintf_s(szPrint,"[LOG] %s",pszPrint);
			OutputDebugStringA(szPrint);

			if( strcmp("武器斩味下降",pszPrint) == 0 )
			{
				// 武器斩味下降，使用物品
				MMOActor theActor;
				theActor.ReadActor();
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
}

_declspec(naked) void Hooksprintf_s()
{
	__asm
	{
		pushfd
		pushad

		MOV EAX,DWORD PTR DS:[EAX+MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_OFFSET___]
		push eax
		call Dosprintf_s
		add esp, 4

		popad
		popfd

// 		POP EBP
// 		POP EBX
// 		ADD ESP,0x14
// 		RETN

		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN
	}
}



void CInjectedWnd::OnBt_HookJuQing()
{
	// TODO: 在此添加控件通知处理程序代码

	Hook(MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_HOOK+GetCryGameBaseAddr(),Hooksprintf_s);
}

DWORD GetRetRoleChooseJieMianHook()
{
	return MMOCLIENT_REVERSAL_RET_ROLE_CHOOSE_JIE_MIAN_HOOK+GetCryGameBaseAddr();
}

void PrintChooseRole()
{
#if _DEBUG
	OutputDebugStringA("[LOG] 人物选择界面");
#endif
}

_declspec(naked) void HookRetChooseRole()
{
	__asm
	{
// 		MOV EAX,DWORD PTR DS:[EBX+0x24]
// 		MOV BYTE PTR SS:[EBP+0xB],0x0

		pushad
			call PrintChooseRole

		popad
		
		push eax
		mov eax, GetRetRoleChooseJieMianHook
		call eax
		mov edx,eax
		pop eax



		sub esp,0x34
		mov eax,esp
		add edx,5
		jmp edx

// 		push eax
// 		mov eax, GetRetRoleChooseJieMianHook
// 		mov ecx, eax
// 		pop eax
// 		add ecx, 5
// 		jmp ecx
	}
}

void CInjectedWnd::OnBt_HookRetChooseRole()
{
	// TODO: 在此添加控件通知处理程序代码
	Hook(GetRetRoleChooseJieMianHook(),HookRetChooseRole);
}

void CInjectedWnd::OnBtPacket_AcceptGongHuiTask()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_AcceptGongHuiTask(0x64,0x00031129);			// 此ID为副本的id，遍历副本信息的按钮  [LOG] addr = 0x313AB9A0 id = 0x00031129 map id = 0x000003E9 level = 19 name = 蓝色急速讨伐战
	MMO_GotoFb2(0x00031129,MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_GONG_HUI___);
	Sleep(2000);
	MMO_GotoFb3(MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_GONG_HUI___);
}

void CInjectedWnd::OnBtPacket_AcceptCatTask()
{
	// TODO: 在此添加控件通知处理程序代码
	/*++
	千狩
	[LOG] addr = 0x313D89A0 id = 0x000226C8 map id = 0x00000579 level = 30 name = 千狩试炼·一段
	[LOG] addr = 0x313D8670 id = 0x000226C9 map id = 0x00000579 level = 35 name = 千狩试炼·二段
	[LOG] addr = 0x313D8340 id = 0x000226CA map id = 0x00000579 level = 40 name = 千狩试炼·三段
	[LOG] addr = 0x313D8010 id = 0x000226CB map id = 0x00000579 level = 45 name = 千狩试炼·四段
	[LOG] addr = 0x313D9CD0 id = 0x000226CC map id = 0x00000579 level = 50 name = 千狩试炼·五段
	[LOG] addr = 0x313D99A0 id = 0x000226CD map id = 0x00000579 level = 55 name = 千狩试炼·六段
	[LOG] addr = 0x313D9670 id = 0x000226CE map id = 0x00000579 level = 55 name = 千狩试炼·七段

	喵喵
	[LOG] addr = 0x33261DB0 id = 0x00018866 map id = 0x000003EC level = 22 difflevel = 1 fbtype = 3 副本完成 = 0 name = 飞虫歼灭战
	[LOG] addr = 0x3328F220 id = 0x00018B2A map id = 0x000003F3 level = 37 difflevel = 1 fbtype = 3 副本完成 = 0 name = 猫的支援术
	[LOG] addr = 0x33274DB0 id = 0x0001898F map id = 0x000003EF level = 32 difflevel = 1 fbtype = 3 副本完成 = 0 name = 破碎的探测器
	[LOG] addr = 0x3327BB60 id = 0x000189F7 map id = 0x000003F0 level = 32 difflevel = 1 fbtype = 3 副本完成 = 0 name = 雪地修行术
	[LOG] addr = 0x33262B60 id = 0x0001886D map id = 0x000003EC level = 27 difflevel = 1 fbtype = 3 副本完成 = 0 name = 泡泡残留
	[LOG] addr = 0x332746C0 id = 0x00018992 map id = 0x000003EF level = 32 difflevel = 1 fbtype = 3 副本完成 = 0 name = 中和毒素的方法
	[LOG] addr = 0x33267470 id = 0x000188CB map id = 0x000003ED level = 27 difflevel = 1 fbtype = 3 副本完成 = 0 name = 小电龙与大电球
	[LOG] addr = 0x3328EB60 id = 0x00018B20 map id = 0x000003F3 level = 37 difflevel = 1 fbtype = 3 副本完成 = 0 name = 雌火龙也有春天
	[LOG] addr = 0x33261910 id = 0x00018868 map id = 0x000003EC level = 27 difflevel = 1 fbtype = 3 副本完成 = 0 name = 毒性道具测试
	[LOG] addr = 0x33260220 id = 0x00018865 map id = 0x000003EC level = 22 difflevel = 1 fbtype = 3 副本完成 = 0 name = 蟹壳素收集


	[4872] [LOG] 梅杰波尔坦 fb id = 0x0002BFE9 模板地图 id = 0x0000070A addr = 0x1EE144F8 name = Vge002
	[4872] [LOG] 过图封包 : coord = 1329.985107 1068.712646 16.540251 cmd = Teleport_To_MainArea		从小猫的房间返回到0号房间
	[4872] [LOG] 过图封包 : coord = 1344.364014 1184.265137 17.584417 cmd = Teleport_To_Cat_Area		从0号房间到1号小猫的房间

	[LOG] 门对象 addr = 0x8F751488 addr2 = 0x00000000 local id = 0x0000026F id = 0xFFFFFFFF room = 0 class = 0x00000690 packet id = 0x00000000 name = 未知 name = Teleport_To_Cat_Area x = 1349.549438 y = 1186.096069 z = 20.308100

	[LOG] 门对象 addr = 0x33D93B08 addr2 = 0x00000000 local id = 0x00000267 id = 0xFFFFFFFF room = 2 class = 0x00000690 packet id = 0x00000000 name = 未知 name = Teleport_To_MainArea x = 1334.700562 y = 1073.217407 z = 20.308100
	--*/
	// MMO_AcceptQianShouTask(0x00018935);		// 适用于千狩和喵喵副本

	MMO_GotoFb2(0x00018935,MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_QIAN_SHOU___);
	Sleep(2000);
	MMO_GotoFb3(MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_QIAN_SHOU___);
}

void CInjectedWnd::OnBtPacket_FindSoldProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	//MMO_ReqItemBytype(1,1,1);
	MMO_ReqItemBytype("药草",0x000011A8);
}

void CInjectedWnd::OnBtPacket_NextProductPage()
{
	// TODO: 在此添加控件通知处理程序代码
	static int nPageCount = 8;
	nPageCount += 8;
	MMO_GetSearchItemListByPage(nPageCount);		// 翻页功能，界面不会更新，查看recv的信息即可
}

void CInjectedWnd::OnBtPacket_BuyTheProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_REQ_BUY_ITEM theReqBuyItem;
	theReqBuyItem.dwReqItemId = 0x00013D1D;
	theReqBuyItem.dwReqItemKey = 0x555E14A0;
	theReqBuyItem.nPrice = 400;
	theReqBuyItem.dwItemId = 8;
	theReqBuyItem.nItemCount = 5;
	MMO_ReqBuyItem(theReqBuyItem);
}

void CInjectedWnd::OnBtPacket_RefreshMySellList()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_GetMySellDetail();
}

void CInjectedWnd::OnBtPacket_SaleMyProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_SHANG_JIA_AUCTION_ITEM theAuctionItem;
	theAuctionItem.bTabIndex = 0;
	theAuctionItem.wSlotIndex = 0;
	theAuctionItem.dwItemId = 0x000052EA;
	theAuctionItem.wItemCount = 1;
	theAuctionItem.nPrice1 = 100;
	theAuctionItem.nPrice2 = 10;
	theAuctionItem.nTimeFalg = 0;
	
	MMO_ShangJiaAuctionItem(theAuctionItem);
}

void CInjectedWnd::OnBtPacket_InviteTeam()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_InviteTeam(0x00B09319);
}

void CInjectedWnd::OnBtPacket_JoinTeam()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_JoinTeam(0x00B09319,"110");
}

void CInjectedWnd::OnBtPacket_LeaveTeam()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_ReqQuitTeam();
}

void CInjectedWnd::OnBtPacket_RegLeaveTeam()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_ReqKickMember(0x00B09319);
}

void CInjectedWnd::OnBtPacket_CreateTeam()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_CreateTeamReq("110");
}

void CInjectedWnd::OnBtPacket_SelectCaptainLine()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_ServerLineLaunch(6);
}

#define MMOCLIENT_REVERSAL_HIT_CALL_ADDR___			0x0149D9E0

void SendHitCallAddr(DWORD dwHitInfoAddr)
{
	try
	{
		if( dwHitInfoAddr == 0 )
			return ;

		DWORD dwHitBuWeiIndex = *(DWORD*)(dwHitInfoAddr+0x48);

		char szPrint[256];
		sprintf_s(szPrint,"[LOG] 攻击部位 = 0x%p",dwHitBuWeiIndex);
		OutputDebugStringA(szPrint);
	
	}
	catch (...)
	{

	}
}

DWORD dwJmpAddr = MMOCLIENT_REVERSAL_HIT_CALL_ADDR___+5;
_declspec(naked) void HookPoXieGongJiBuWei()
{
	
	__asm
	{
		pushfd
		pushad

		mov esi, dword ptr[esp+0x24+4]
		push esi
		call SendHitCallAddr
		add esp, 4

		popad
		popfd

		MOV EAX,0x48648
		jmp dword ptr[dwJmpAddr]

	}
}

void CInjectedWnd::OnBt_FactureInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	//Hook(MMOCLIENT_REVERSAL_HIT_CALL_ADDR___,HookPoXieGongJiBuWei);
	SetDlgItemText(IDC_EDIT_SHOW,GetManuFactureInfo());
}

void CInjectedWnd::OnBtPacket_ActiveGainTasks()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_RequestEntrustList(MMOCLIENT_REVERSAL_PACKET_MSG_ID_MIAO_MIAO_KEY___);
}

void CInjectedWnd::OnBtPacket_SearchJiYunFb()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_GetInstanceEnterData();
}



void CInjectedWnd::OnBt_HookHitBody()
{
	// TODO: 在此添加控件通知处理程序代码

	MMO_HookHitBuWei();
}



void CInjectedWnd::OnBt_CalcRoleDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		DWORD dwGameAddr2 = *(DWORD*)(MMOCLIENT_REVERSAL_GAME_ADDR+GetCryGameBaseAddr());
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET1);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET2);
		dwGameAddr2 = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET3___);

		DWORD dwActorId = *(DWORD*)(dwGameAddr2+MMOCLIENT_REVERSAL_ACTOR_OFFSET_ID___);
		DWORD dwActorObjAddr = GetActorObjAddr(dwActorId);
		DWORD dwActorAddr = *(DWORD*)(dwActorObjAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___);
		MMO_Direction2* ptheCurActorDirection = (MMO_Direction2*)(dwActorAddr+MMOCLIENT_REVERSAL_TRAVERSE_OBJ_DIR_OFFSET___);


		dwActorAddr += MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___;		// 取到玩家的dir1和dir2的偏移

		// 拷贝过来作为参数传入
		MMO_ActorVector theActorDirAddr = {0};		// 这里的面向需要自己计算，并赋值合理的面向参与计算
		memcpy_s(&theActorDirAddr,sizeof(theActorDirAddr),(LPCVOID)dwActorAddr,sizeof(theActorDirAddr));

		// 返回计算后的面向数据
		MMO_Direction2 theDirection = GetHitSendDir(theActorDirAddr);

		// 返回结果判断x和y是否全部为0，如果全部为0则失败
		TCHAR szPrint[256] = {0};
		_stprintf_s(szPrint, _T("[LOG] addr = 0x%p 返回结果 x = %f y = %f 游戏内角色dir x = %f y = %f"), dwActorObjAddr, theDirection.dir_x, theDirection.dir_y, ptheCurActorDirection->dir_x, ptheCurActorDirection->dir_y);
		OutputDebugString(szPrint);

		SetDlgItemText(IDC_EDIT_SHOW,szPrint);
	}
	catch (...)
	{

	}

}

void CInjectedWnd::OnBtPacket_ZuoQiQiu()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_ZuoQiQiuQianWangMeiJieBoErTan();
}

void CInjectedWnd::OnBt_FarmCollectPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	/*++
	遍历采集物品时，对应的英文名
	MineEx 				未知
	FishEx 				未知
	BugEX 				未知
	Honey_Collector_All 蜂房
	FishLV1 			鱼塘-
	FishLV2 			鱼塘2
	LandLV1 			草药田1
	LandLV2 			草药田2
	LandLV3 			草药田3
	MineLVMax1 			矿场
	BugLVMax 			虫栖地
	MushRoomLVMax 		育菇木床
	--*/

	CString csTxt = GetFarmFacilityState();
	csTxt += GetFarmFacilityInfo();
	SetDlgItemText(IDC_EDIT_SHOW,csTxt);
}

void CInjectedWnd::OnBt_TraSupply()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetRequireCurSlotSupplysAll());
}

void CInjectedWnd::OnBtPacket_GiveUpFb()
{
	// TODO: 在此添加控件通知处理程序代码

	MMO_OnReturnTown_OK();
}

void CInjectedWnd::OnBtPacket_PlantSeed()
{
	// TODO: 在此添加控件通知处理程序代码
	/*++
	[LOG] i = 256 Item addr = 0x5330C3F8 id = 0x0000520B name = 解毒草 count = 10 key = 0x57B69145 0x26F553F7 等级 = 1 拥有上限 = 0 物品颜色 = 1 携带上限 = 10 物品类型 = 1 部位类型1 = 8 物品类型2 = 2 角色等级  = 0
	[LOG] i = 257 Item addr = 0x5330C438 id = 0x0000520A name = 火药草 count = 17 key = 0x4F667149 0xD75554E2 等级 = 1 拥有上限 = 0 物品颜色 = 1 携带上限 = 10 物品类型 = 1 部位类型1 = 8 物品类型2 = 2 角色等级  = 0

	物品描述		  		物品类型		部位类型1		部位类型2
	种子						1				8				2

	参数分3个： 药草田1 = 0 药草田2 = 1 药草田3 = 2
	--*/
	MMO_FramSeed(0x0000520B,1);
}



void CInjectedWnd::OnBt_YouHuaGame()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwSleepTime = GetDlgItemInt(IDC_EDIT_DECRY);
	char szPrint[256];
	sprintf_s(szPrint,"[LOG] Sleep 0x%p",dwSleepTime);
	OutputDebugStringA(szPrint);
	
	MMO_Render(dwSleepTime);
}

void CInjectedWnd::OnBtPacket_SendMail()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_SEND_MAIL theSendMail = {0};

	memcpy_s(theSendMail.szMailActorName,sizeof(theSendMail.szMailActorName),"叶日天",strlen("叶日天")+1);
	memcpy_s(theSendMail.szMailTitle,sizeof(theSendMail.szMailTitle),"标题",strlen("标题")+1);
	memcpy_s(theSendMail.szMailData,sizeof(theSendMail.szMailData),"内容",strlen("内容")+1);
	theSendMail.nGold = 0;
// 	theSendMail.nAttachmemtCount = 3;
// 	theSendMail.theAttachmemt[0].bTableIndex = 0;
// 	theSendMail.theAttachmemt[0].wSlotIndex = 23;
// 	//theSendMail.theAttachmemt[0].llItemSerial = 0x90C372F02502760D;  // [LOG] i = 1 Item addr = 0x5218AD58 id = 0x0000526A name = 素材玉 count = 2 key = 0x2502760D 0x90C372F0
// 	theSendMail.theAttachmemt[0].llItemSerial = 0x74EF873BE0017502; 
// 	theSendMail.theAttachmemt[0].nItemCount = 1;
// 
// 	theSendMail.theAttachmemt[0].bTableIndex = 0;
// 	theSendMail.theAttachmemt[0].wSlotIndex = 22;
// 	theSendMail.theAttachmemt[0].llItemSerial = 0x942F873BE0017502; 
// 	theSendMail.theAttachmemt[0].nItemCount = 1;
// 
// 	theSendMail.theAttachmemt[0].bTableIndex = 0;
// 	theSendMail.theAttachmemt[0].wSlotIndex = 21;
// 	theSendMail.theAttachmemt[0].llItemSerial = 0x93AF873BE0017502; 
// 	theSendMail.theAttachmemt[0].nItemCount = 1;

	MMO_SendMail(theSendMail);
} 

void CInjectedWnd::OnBt_TraBuffState()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetRoleBuffState());
}

void CInjectedWnd::OnBt_TraBuffInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetBuffInfo());
}

void CInjectedWnd::OnBt_TraGiftBag()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetGiftBagInfo());
}

void CInjectedWnd::OnBtPacket_ReceiveGift()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_GetGfitBag(0x00000087 );
}

void CInjectedWnd::OnBt_GetRoleLvlUpInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_SHOW,GetCPlayerLevelUpInfo());
}

void CInjectedWnd::OnBtPacket_TidyItem()
{
	// TODO: 在此添加控件通知处理程序代码
	// MMO_TidyInventoryItem
	MMO_TIDY_ITEM theTidyItem = {0};

	theTidyItem.wTabSoltIndex = 2;		// 普通仓库=2 素材仓库=0x0A 背包=0
	theTidyItem.wSoltCount = 199;		// 物品有效格子，从0开始计数
	theTidyItem.wItemCount = 6;			// 有效物品的数量

	theTidyItem.theTidyItemInfo[0].llkey = 0xC405560B49666144;		// 物品的key
	theTidyItem.theTidyItemInfo[0].nSloltIndex = 0;					// 物品所在格子

	theTidyItem.theTidyItemInfo[1].llkey = 0xC405560B49666144;
	theTidyItem.theTidyItemInfo[1].nSloltIndex = 1;

	theTidyItem.theTidyItemInfo[2].llkey = 0xDA6559021AC65142;
	theTidyItem.theTidyItemInfo[2].nSloltIndex = 2;

	theTidyItem.theTidyItemInfo[3].llkey = 0xDA6559021AC65142;		// 物品的key
	theTidyItem.theTidyItemInfo[3].nSloltIndex = 3;					// 物品所在格子

	theTidyItem.theTidyItemInfo[4].llkey = 0x7C8558F05BC6A147;
	theTidyItem.theTidyItemInfo[4].nSloltIndex = 4;

	theTidyItem.theTidyItemInfo[5].llkey = 0x7C8558F05BC6A147;
	theTidyItem.theTidyItemInfo[5].nSloltIndex = 6;

	MMO_TidyInventoryItem(&theTidyItem);

}

void CInjectedWnd::OnBtPacket_GiveUpTask()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_giveuptask(0x088b);
}

void CInjectedWnd::OnBtPacket_LeaveTeachFb()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_XinShouXunLian_LiKaiJiaoXue();
}

void CInjectedWnd::OnBtPacket_TeachFb()
{
	// TODO: 在此添加控件通知处理程序代码
	// [LOG] addr = 0x2DEA5050 id = 0x00022664 map id = 0x0000057A level = 0 name = 新手武器训练
	MMO_JinRuJiaoXueFb(0x00022664);
}


void CInjectedWnd::OnBt_HookUseSkillInSky()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_KongZhongUseSkillJmp();
	
}

void CInjectedWnd::OnBtPacket_GainWeekCredit()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_WEEK_CREDIT theWeekCredit;
	if (MMO_GetWeekCreditCount(1, theWeekCredit))
		MMO_sendT(theWeekCredit,MMOCLIENT_REVERSAL_GET_WEEK_CREDIT_COUNT___);
}


void CInjectedWnd::OnBtPacket_SyncPos()
{
	// TODO: 在此添加控件通知处理程序代码

	// 遍历对象按钮遍历到的数据
	// [LOG] 0x56CF7F28 addr = 0x58620B18 addr2 = 0x4F72BB50 id = 0000FFCE packet id = 0x00210308 room = 3 status = 0 0 name = eqhYGGNmTa8w title =  type = 00000597 npc type = -1 
	// dir = 0.945519 -0.325568 dir1 = -0.788011 0.615661 dir2 = -0.615661 -0.788011  x = 1409.660767 y = 953.669373 z = 321.334808
	SYN_COOR_DINATES theSynCoorDinates = {0};
	theSynCoorDinates.ptheAcrot.fx = 853.684143f;	// 门的坐标
	theSynCoorDinates.ptheAcrot.fy = 777.603210f;
	theSynCoorDinates.ptheAcrot.fz = 17.921875f+2.0f;

	theSynCoorDinates.theDir1.dir_x = 0.2389493f;	// dir   dir和dir1可以取当前角色的
	theSynCoorDinates.theDir1.dir_y = 0.9710320f;

	theSynCoorDinates.theDir2.dir_x = -1.874781f;	// dir1
	theSynCoorDinates.theDir2.dir_y = 3.578657f;
	
	// 取同步坐标随机数
	theSynCoorDinates.llSysRandom = GetSynCoorDinatesRandom();
	// 同步坐标到服务器
	MMO_SynCoorDinates(theSynCoorDinates);


// 	MMO_Vector3 theDoorCoord;
// 	theDoorCoord.fx = 1568.248f;	// 过图
// 	theDoorCoord.fy = 1592.373f;
// 	theDoorCoord.fz = 142.3750f;
// 	char szGoTuCmd[] = "SwitchRegionTrigger6to3";			// SwitchRegionTrigger1toC    C表示起点的房间，其他全部是界面上显示的房间序号，此处发包表示 C to 1
// 	MMO_FbGuoTu(theDoorCoord,szGoTuCmd);
}




void CInjectedWnd::OnBt_TidyItem()
{
	// TODO: 在此添加控件通知处理程序代码
	TidyInventoryItem(0xA);
	TidyInventoryItem(0x2);
	TidyInventoryItem(0x0);
}

void CInjectedWnd::OnBt_HookActorDiMianPos()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_HookMapCoorDinates();
}

void CInjectedWnd::OnBt_TraTaskFbInfo()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT_SHOW,GetTaskFbInfo());
	
}

void CInjectedWnd::OnBtPacket_DeleteRole()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_DeleteRole(0x0021113B);
}

void CInjectedWnd::OnBtPacket_AddFriend()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_AddFrient("一生何求");
}



void CInjectedWnd::OnBt_ShangJinTaskInfo()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT_SHOW,GetShangJinTaskInfo());
}

void CInjectedWnd::OnBnClickedButton110()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_Request_Deal(0x00309E6B);
}

void CInjectedWnd::OnBnClickedButton111()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_DEAL_MOVE_ITEM theDealMoveItem;
	theDealMoveItem.wScrItemTabIndex = 0;
	theDealMoveItem.wScrItemSlotIndex = 0x0C;
	theDealMoveItem.wDealSlotaIndex = 1;
	theDealMoveItem.wMoveItemCount = 3;
	MMO_Deal_Move_Item(theDealMoveItem);
}

void CInjectedWnd::OnBnClickedButton112()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_Lock_Deal();
}

void CInjectedWnd::OnBnClickedButton113()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_Enter_Deal();
}

void CInjectedWnd::OnBnClickedButton114()
{
	// 如果要交易 987 金币 例子如下  每次输入一个数值进去，游戏都是及时发包的
	MMO_Deal_Gold(9);		
	MMO_Deal_Gold(98);
	MMO_Deal_Gold(987);
}

void CInjectedWnd::OnBnClickedButton115()
{
	// TODO: 在此添加控件通知处理程序代码
	MMO_UnLock_KingFb(0x000189D8);
}


typedef   enum   _THREADINFOCLASS   { 
	ThreadBasicInformation, 
	ThreadTimes, 
	ThreadPriority, 
	ThreadBasePriority, 
	ThreadAffinityMask, 
	ThreadImpersonationToken, 
	ThreadDescriptorTableEntry, 
	ThreadEnableAlignmentFaultFixup, 
	ThreadEventPair_Reusable, 
	ThreadQuerySetWin32StartAddress, 
	ThreadZeroTlsCell, 
	ThreadPerformanceCount, 
	ThreadAmILastThread, 
	ThreadIdealProcessor, 
	ThreadPriorityBoost, 
	ThreadSetTlsArrayAddress, 
	ThreadIsIoPending, 
	ThreadHideFromDebugger, 
	ThreadBreakOnTermination, 
	MaxThreadInfoClass 
}   THREADINFOCLASS;


typedef HANDLE (__stdcall *OPENTHREAD) (DWORD dwFlag, BOOL bUnknow, DWORD dwThreadId);
typedef LONG (__stdcall *_pfnZwQueryInformationThread) (
	IN   HANDLE   ThreadHandle, 
	IN   THREADINFOCLASS   ThreadInformationClass, 
	OUT   PVOID   ThreadInformation, 
	IN   ULONG   ThreadInformationLength, 
	OUT   PULONG   ReturnLength   OPTIONAL 
	);
OPENTHREAD lpOpenThread;

_pfnZwQueryInformationThread ZwQueryInformationThread;

typedef struct
{
	DWORD	dwThreadId;
	bool		nFlag;
}KILL_THREAD,*PKILL_THREAD;


vector<DWORD> g_vdwKillCheckThread;
PVOID ShowThreadInfo(DWORD dwThreadPid) 
{
	PVOID lpStartAddr = NULL; 
	HANDLE	hThread = NULL;     
	hThread = lpOpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadPid);
	if (hThread == NULL) 
		return FALSE;

	ZwQueryInformationThread(hThread,
		ThreadQuerySetWin32StartAddress,   
		&lpStartAddr,   
		sizeof(lpStartAddr),   
		NULL);

	DWORD dwThreadAddr = (DWORD)lpStartAddr;
	//dwThreadAddr &= 0xFFFF;

	DWORD dwTerSafebeginAddr = (DWORD)GetModuleHandleA("tersafe.dll");
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("tersafe.dll"), &modinfo, sizeof(MODULEINFO));
	DWORD  dwTerSafeendAddr = dwTerSafebeginAddr + modinfo.SizeOfImage;


	DWORD dwTenRpcsDllBAddr=(DWORD)GetModuleHandleA("TenRpcs.dll");
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("TenRpcs.dll"), &modinfo, sizeof(MODULEINFO));
	DWORD dwTenRpcsDllEAddr=dwTenRpcsDllBAddr+modinfo.SizeOfImage;


	DWORD dwTenSpDllBAddr=(DWORD)GetModuleHandleA("TenSP.dll");
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("TenSP.dll"), &modinfo, sizeof(MODULEINFO));
	DWORD dwTenSpDllEAddr=dwTenSpDllBAddr+modinfo.SizeOfImage;





// 	if ((dwThreadAddr>=dwTerSafebeginAddr && dwThreadAddr<=dwTerSafeendAddr) /*||*/
// // 		(dwThreadAddr >=dwTenRpcsDllBAddr &&   dwThreadAddr <= dwTenRpcsDllEAddr ) ||
//  		/*(dwThreadAddr >=dwTenSpDllBAddr  && dwThreadAddr<=dwTenSpDllEAddr))*/)
// 	{
// 		
// 		CString csOutString;
// 		csOutString.Format(_T("[LOG] 已暂停一个线程IDHex=%p Id=%d   线程入口=%p"),hThread,hThread,dwThreadAddr);
// 		OutputDebugString(csOutString);
// 		SuspendThread(hThread);
// 		g_vdwKillCheckThread.push_back(dwThreadPid);
// 	}
 	dwThreadAddr &= 0xFFFF;
	if (dwThreadAddr == 0x233B  || dwThreadAddr == 0x236B)
	{
		g_vdwKillCheckThread.push_back((DWORD)hThread);
#if _DEBUG
		OutputDebugString(_T("[LOG] 已经干掉了0x233B线程"));
#endif
		g_vdwKillCheckThread.push_back(dwThreadPid);
		SuspendThread(hThread);

	}



	CloseHandle(hThread);
	return lpStartAddr;
}
BOOL KillCheckThread()
{
	ZwQueryInformationThread=(_pfnZwQueryInformationThread)GetProcAddress(LoadLibraryA("ntdll.dll"),"ZwQueryInformationThread");
	lpOpenThread=(OPENTHREAD)GetProcAddress(LoadLibraryA("kernel32.dll"),"OpenThread");

	PVOID lpThreadAddr = NULL;
	DWORD dwThreadPid = 0;
	HANDLE hThreadShot=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
	THREADENTRY32 *tagThreadInfo = new THREADENTRY32;
	tagThreadInfo->dwSize = sizeof(THREADENTRY32);

	while(Thread32Next(hThreadShot,tagThreadInfo)!=FALSE)
	{
		if(GetCurrentProcessId()==tagThreadInfo->th32OwnerProcessID)
		{
			lpThreadAddr = ShowThreadInfo(tagThreadInfo->th32ThreadID);
			// 			dwThreadPid = tagThreadInfo->th32ThreadID;
			// 			int dwThreadAddr = (int)lpThreadAddr;
		}     
	}
	CloseHandle(hThreadShot);
	return TRUE;
}

void CInjectedWnd::OnBt_KillCheckThread()
{
	KillCheckThread();
	// TODO: Add your control notification handler code here
}
typedef void* (__cdecl* LPK_memcpy)(	void* dst,
									const void* src,
									size_t count
									);
LPK_memcpy Old_memcpy;
LPK_memcpy Jmp_Old_memcpy;

DWORD g_dwCryGameBaseAddr = 0;

void* __cdecl My_memcpy(
	void* dst,
	const void* src,
	size_t count
	)
{
		if( g_dwCryGameBaseAddr == 0 )
		{
			g_dwCryGameBaseAddr = (DWORD)GetModuleHandleA("CryGame.dll");

			if( g_dwCryGameBaseAddr != 0 )
			{
				g_dwCryGameBaseAddr += 0x1000;
			}
		}

		if( g_dwCryGameBaseAddr != 0 )
		{
			if( g_dwCryGameBaseAddr <= (DWORD)src && (DWORD)src <= (g_dwCryGameBaseAddr+0x01733000) )
			{
				OutputDebugStringA("[LOG] MemcpyCRC!!!!跟老子玩阴的，干死你!!");
				while(true)
				{
					Sleep(1000);
				}
			}
		}

	return Jmp_Old_memcpy(dst,src,count);
}
DWORD g_dwReal_Jmp_Old_memcpy;
_declspec(naked) void Real_Jmp_Old_memcpy()
{
	__asm
	{
		PUSH EBP
			MOV EBP,ESP
			PUSH EDI
			PUSH ESI
			jmp dword ptr[g_dwReal_Jmp_Old_memcpy]
	}
}
void CInjectedWnd::OnBt_CrcTest()
{
	Old_memcpy = (LPK_memcpy)GetProcAddress(LoadLibraryA("ntdll.dll"),"memcpy");
	if( Old_memcpy != 0 )
	{
		Hook((DWORD)Old_memcpy,My_memcpy);
		Jmp_Old_memcpy = (LPK_memcpy)Real_Jmp_Old_memcpy;
		g_dwReal_Jmp_Old_memcpy = (DWORD)Old_memcpy+5;
	}
	// TODO: Add your control notification handler code here
}




BOOL CmpIsCheckThread(DWORD dwTheardId)
{
	for (DWORD i=0;i<g_vdwKillCheckThread.size();i++)
	{
		if (dwTheardId==g_vdwKillCheckThread[i])
		{
#if _DEBUG
			char szPirnt[30];
			sprintf_s(szPirnt,"[LOG] 拦截到一个 ID=%d",GetCurrentThreadId());
			OutputDebugStringA(szPirnt);
#endif
			while (true)
			{
				Sleep(1000*10);
			}
			return TRUE;
		}
	}
	return FALSE;
}


DWORD g_dwResumeThread=0;
_declspec(naked) void HookResumeThread()
{
	__asm
	{
		mov eax,[esp+0x2C]
		pushad
		pushfd
		
		push eax
		mov eax,CmpIsCheckThread
		call eax
		add esp,0x4
		cmp eax,1
		je RetCode

		popfd
		popad
		mov eax,[ebp+0xc]
		cmp eax,0x3
		mov edx,g_dwResumeThread
		add edx,5
		jmp edx 
		
RetCode:

		
		
		popfd
		popad
		pop ebp
		mov eax,1
		ret 4

	}
}
void CInjectedWnd::OnBt_ResumeCheckThread()
{

	 g_dwResumeThread = (DWORD)GetProcAddress(LoadLibraryA("KernelBase.dll"),"ResumeThread")+3;
	if (g_dwResumeThread != 0)
	{
		KillCheckThread();
		Hook(g_dwResumeThread,HookResumeThread);
	}


	// TODO: Add your control notification handler code here
}
DWORD g_dwJmpGetTickCount = 0;

DWORD g_dwStaticTickCount = 0;
/*
typedef struct _GetTickCount
{

};*/

void PrintGetTickCount(DWORD dwCurTickCount)
{
	if( g_dwStaticTickCount == 0 )
	{
		g_dwStaticTickCount = dwCurTickCount;
	}
}



_declspec(naked) void HookGetTickCount()
{
	__asm
	{
		pushfd
			push eax
			mov eax, PrintGetTickCount
			call eax
			add esp, 4
			popfd

			mov eax, g_dwStaticTickCount;
		retn 
			//jmp dword ptr[g_dwJmpGetTickCount]
	}
}


void CInjectedWnd::OnBt_HookGetTickCount()
{
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	DWORD dwGetTickCount = (DWORD)GetProcAddress(LoadLibraryA("KernelBase.dll"),"GetTickCount");
	Hook(dwGetTickCount+0x33,HookGetTickCount);
	// TODO: Add your control notification handler code here
}



DWORD g_KiUserExAddr=0;
_declspec(naked) void HookCrc()
{
	__asm
	{
		cld
			mov ecx,dword ptr ss:[esp+0x4]
		mov edx,g_KiUserExAddr
			add edx,5
			jmp edx
	}

}
void CInjectedWnd::OnBt_HookKiUser()
{
	g_KiUserExAddr= (DWORD)GetProcAddress(LoadLibraryA("ntdll.dll"),"KiUserExceptionDispatcher");
	Hook(g_KiUserExAddr,HookCrc); //测试用
	// TODO: Add your control notification handler code here
}
_declspec(naked) void MyRet()
{
	__asm
	{
		ret 
	}
}

_declspec(naked) void MyRet4()
{
	__asm
	{
		ret 4
	}
}

_declspec(naked) void MyRet8()
{
	__asm
	{
		ret 8
	}
}
_declspec(naked) void MyRetC()
{
	__asm
	{
		ret 0xc
	}
}

_declspec(naked) void MyRet10()
{
	__asm
	{
		ret 0x10
	}
}
void SleeThread()
{
 		Sleep(5);
}
void WhileSleepThread()
{

	while (true)
	{
		Sleep(1000);
	}
}

_declspec(naked) void HookSleepThreadRet8()
{
	__asm
	{
		call SleeThread

		ret 8
	}

}

_declspec(naked) void HookSleepThreadRet4()
{
	__asm
	{
		call SleeThread
		ret 4
	}

}


void IdaGameCheck()
{
	__try
	{
		DWORD dwCheckBase=(DWORD)GetModuleHandleA("tersafe.dll")+UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMETersafeDllCheckVit_ADDR;
		dwCheckBase= *(DWORD *)dwCheckBase;
		DWORD dwCheckBaseOffset1=*(DWORD*)(dwCheckBase+0x58);
		DWORD dwCheckBaseOffset2=*(DWORD*)(dwCheckBaseOffset1);  

		DWORD dwTenRec=*(DWORD *)(dwCheckBaseOffset2+0x10);
		DWORD dwTenRecIda=*(DWORD *)dwTenRec;
		*(DWORD *)dwTenRecIda=(DWORD)MyRet;
		*(DWORD *)(dwTenRecIda+4)=(DWORD)MyRetC;
#if _DEBUG
		OutputDebugStringA("[LOG]  写入TenRecIda成功");
#endif


		//////////////////////////////MHOClientBase.dll模块的处理 ////////////
		DWORD dwMHoClientBase=*(DWORD *)(dwCheckBaseOffset2+4);
		dwMHoClientBase=*(DWORD *)(dwMHoClientBase+0x10);
		DWORD dwMHOClientBaseIda=*(DWORD *)dwMHoClientBase;

		DWORD dwError=0;
		VirtualProtectEx((HANDLE)-1, (LPVOID)dwMHOClientBaseIda, 0x100, PAGE_EXECUTE_READWRITE, &dwError); 
		*(DWORD *)dwMHOClientBaseIda=(DWORD)MyRet;
 		*(DWORD *)(dwMHOClientBaseIda+4)=(DWORD)MyRetC;

#if _DEBUG
		OutputDebugStringA("[LOG]  写入MHOClientBaseida成功");
#endif

		/////////////////////////////////////////////////////////////////////////



		
		////////////////////////////SATERFASE.DLL模块处理////////////////////////
		DWORD dwtersafeBase=*(DWORD *)(dwCheckBaseOffset2+4);
		dwtersafeBase=*(DWORD *)(dwtersafeBase+4);
		dwtersafeBase=*(DWORD *)(dwtersafeBase+0x10);
		DWORD dwtersafeBaseIda=*(DWORD *)(dwtersafeBase);

		VirtualProtectEx((HANDLE)-1, (LPVOID)dwtersafeBaseIda, 0x100, PAGE_EXECUTE_READWRITE, &dwError); 

	    *(DWORD *)dwtersafeBaseIda=(DWORD)MyRet;
	    *(DWORD *)(dwtersafeBaseIda+4)=(DWORD)MyRetC;
		*(DWORD *)(dwtersafeBaseIda+8)=(DWORD)MyRet;
		*(DWORD *)(dwtersafeBaseIda+0x14)=(DWORD)MyRet8;
		*(DWORD *)(dwtersafeBaseIda+0x1C)=(DWORD)MyRet10;
		*(DWORD *)(dwtersafeBaseIda+0x20)=(DWORD)MyRet;

		//*(DWORD *)(dwtersafeBaseIda+0xA8)=(DWORD)MyRet8;
		//*(DWORD *)(dwtersafeBaseIda+0xF0)=(DWORD)MyRet8;//这个HOOK了不能移动视角
		//*(DWORD *)(dwtersafeBaseIda+0xF4)=(DWORD)MyRet4;
#if _DEBUG
		OutputDebugStringA("[LOG]  写入SATERFASEIda成功");
#endif
// 
// 
// 		///////////////////////////////////////////////////////////////////////////




// 		//364  睡死

		
			unsigned char szCodeChar[]={0x90,0x90};
			WriteProcessMemory((HANDLE)-1,(LPVOID)UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckNop1_ADDR,&szCodeChar,2,NULL);
			WriteProcessMemory((HANDLE)-1,(LPVOID)UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckNop2_ADDR,&szCodeChar,2,NULL);
			Hook(UPDATES_MMOCLIENT_REVERSAL_CHECK_GAMECheckSleep_ADDR,WhileSleepThread);

			DWORD dwMHoClientBaseIdaAddr=0x13A7000;
			VirtualProtectEx((HANDLE)-1, (LPVOID)dwMHoClientBaseIdaAddr, 0x500, PAGE_EXECUTE_READWRITE, &dwError); 

			*(DWORD *)(dwMHoClientBaseIdaAddr+0x240)=(DWORD)HookSleepThreadRet4;
			*(DWORD *)(dwMHoClientBaseIdaAddr+0x1F4)=(DWORD)HookSleepThreadRet8;
			*(DWORD *)(dwMHoClientBaseIdaAddr+0x31c)=(DWORD)HookSleepThreadRet8;
// 			0x31c   8
//         

#if _DEBUG
			OutputDebugStringA("[LOG]  写入MHoClientBaseIdaAddr成功");
#endif


	}__except(1)
	{

	}



}

void CInjectedWnd::OnBt_WriteCheck()
{
	IdaGameCheck();
	// TODO: Add your control notification handler code here
}
_declspec(naked) void HookK32EnumProcessModules()
{
	__asm
	{
		call WhileSleepThread
	}

}

void CInjectedWnd::OnBt_HookEnumMod()
{
	DWORD dwEnumProcessModules = (DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"),"K32EnumProcessModules");
	Hook(dwEnumProcessModules,HookK32EnumProcessModules);
	dwEnumProcessModules=(DWORD)GetProcAddress(LoadLibraryA("Kernel32.dll"),"K32EnumProcessModulesEx");
	Hook(dwEnumProcessModules,HookK32EnumProcessModules);
	// TODO: Add your control notification handler code here
}


void CInjectedWnd::OnBt_TraverseCollect()
{
	SetDlgItemText(IDC_EDIT_SHOW,GetSuoYouShiXianDuiXiang(MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_COLLECT___));
	// TODO: Add your control notification handler code here
}


void CInjectedWnd::OnBt_CallLpkExternFn()
{
	typedef void (WINAPI*MyUseGDIWidthCache)(void);
	 MyUseGDIWidthCache dwMyUseGDIWidthCache=(MyUseGDIWidthCache)GetProcAddress(LoadLibraryA("Lpk.dll"),"MyUseGDIWidthCache");
	 dwMyUseGDIWidthCache();
	// TODO: Add your control notification handler code here
}


void CInjectedWnd::OnBt_HookJingCaiJingTou()
{

	HookJingcai();
	// TODO: Add your control notification handler code here
}


void CInjectedWnd::OnBt_UiTest()
{
	MHOUi::TraUi("123123123123123");
	// TODO: Add your control notification handler code here
}


void CInjectedWnd::OnBt_UiPressTest()
{
	// TODO: Add your control notification handler code here
	MHOUi::ClickButton("root1.rolelist.item6.mc_norole");
}


void CInjectedWnd::OnBt_UiInputTest()
{

// 	MHOUi::ClickButton(MMOCLINET_UI_CR_BTN_OLDPLAY);
// 	Sleep(1000);
// 	MHOUi::ClickButton(MMOCLINET_UI_CR_BTN_CREATEROLE);
// 	Sleep(1000);
// 	MHOUi::ClickButton(MMOCLINET_UI_CR_BTN_MC_RAND);
// 	Sleep(1000);
// 	MHOUi::ClickButton(MMOCLINET_UI_CR_BTN_MC_NEXTSTEP);
// 	Sleep(1000);

	MHOUi::PutString(MMOCLINET_UI_CN_EDIT_CREATENAME,L"大哥说我∏ss");
	Sleep(100);
	MHOUi::ClickButton(MMOCLINET_UI_CR_BTN_CREATEOK);
	Sleep(100);
	// TODO: Add your control notification handler code here
}






void KeyPro(HWND hWnd,UINT uKey)
{
	DWORD dwfszWdMou=(DWORD)GetModuleHandleA("fszwd.dat");
	if (dwfszWdMou)
	{
		dwfszWdMou+=0x101B9D;
	}
	  WNDPROC Wnrd=(WNDPROC)dwfszWdMou;
	
	 CallWindowProcW(Wnrd,hWnd,WM_KEYDOWN,VK_SHIFT,0x20001);

	CallWindowProcW(Wnrd,hWnd,WM_KEYDOWN,uKey,0x20001);
	CallWindowProcW(Wnrd,hWnd,WM_CHAR,uKey,0x20001);
	CallWindowProcW(Wnrd,hWnd,WM_KEYUP,uKey,0xC0020001);

	CallWindowProcW(Wnrd,hWnd,WM_KEYUP,VK_SHIFT,0x20001);
}

void CInjectedWnd::OnBt_InputTest1()
{
	KeyPro((HWND)0x5A10E4,0x31);

	// TODO: Add your control notification handler code here
}
