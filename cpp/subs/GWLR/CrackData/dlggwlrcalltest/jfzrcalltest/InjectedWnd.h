#pragma once


// CInjectedWnd 对话框

class CInjectedWnd : public CDialog
{
	DECLARE_DYNAMIC(CInjectedWnd)

public:
	CInjectedWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInjectedWnd();

// 对话框数据
	enum { IDD = IDD_DLG_INJECTED_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBtPacket_HanHua();
	afx_msg void OnBt_HookSend();
	afx_msg void OnBtPacket_Walk();
	afx_msg void OnBt_TraObj();
	afx_msg void OnBt_HookModifyHitBody();
	afx_msg void OnBt_UnHook();
	afx_msg void OnBt_SkillCall();
	afx_msg void OnBt_WangLiFb();
	afx_msg void OnBt_HookRecv();
	afx_msg void OnBt_HookDecrypt();
	afx_msg void OnBt_HookDlgInfo();
	afx_msg void OnBt_TraDlgInfo();
	afx_msg void OnBt_TraItem();
	afx_msg void OnBtPacket_AcceptTask();
	afx_msg void OnBt_HookActorHpCall();
	afx_msg void OnBt_TraTaskInfo();
	afx_msg void OnBt_TraReceivedTask();
	afx_msg void OnBt_TraUiInfo();
	afx_msg void OnBt_HookEncrypt();
	afx_msg void OnBt_HookMonsterHpCall();
	afx_msg void OnBt_HookCryMalloc();
	afx_msg void OnBtPacket_OpenNpc();
	afx_msg void OnBtPacket_GuoTu();
	afx_msg void OnBt_TraAcceptableTask();
	afx_msg void OnBt_HookTemplateInfo();
	afx_msg void OnBt_TraUnknownObj();
	afx_msg void OnBt_TraAllShiXianObj();
	afx_msg void OnBt_TraZhuiZongTask();
	afx_msg void OnBtCityGuoTu();
	afx_msg void OnBtPacket_CommitTask();
	afx_msg void OnBtPacket_CollectItem();
	afx_msg void OnBt_TraFbInfo();
	afx_msg void OnBtPacket_EnterFb();
	afx_msg void OnBtPacket_CunQuStore();
	afx_msg void OnBt_ItemTemplateInfo();
	afx_msg void OnBtPacket_GainSupply();
	afx_msg void OnBt_HookOptimizeYouHua();
	afx_msg void OnBt_UseItem();
	afx_msg void OnBtPacket_MakeItem();
	afx_msg void OnBt_HookLoadEndMap();
	afx_msg void OnBtPacket_SellItem();
	afx_msg void OnBt_TraMailInfo();
	afx_msg void OnBtPacket_GetMail();
	afx_msg void OnBtPacket_OpenMail();
	afx_msg void OnBtPacket_DeleteMail();
	afx_msg void OnBt_FarmSeedInfo();
	afx_msg void OnBtPacket_NpcChat();
	afx_msg void OnBt_MouseLeft();
	afx_msg void OnBt_HookFbEnd();
	afx_msg void OnBtPacket_ChooseRole();
	afx_msg void OnBtPacket_RetChooseRole();
	afx_msg void OnBt_TraShopInfo();
	afx_msg void OnBtPacket_BuyItem();
	afx_msg void OnBt_TraCraftInfo();
	afx_msg void OnBtPacket_CraftItem();
	afx_msg void OnBtPacket_LvlUpEquip();
	afx_msg void OnBt_RoleMapInfo();
	afx_msg void OnBtPacket_CreateRole();
	afx_msg void OnBt_PressKey();
	afx_msg void OnBtPacket_LeaveFarm();
	afx_msg void OnBtPacket_EnterFarm();
	afx_msg void OnBtPacket_LvlUpCaiJiDian();
	afx_msg void OnBtPacket_DismSuCai();
	afx_msg void OnBt_HookWuDi();
	afx_msg void OnBt_HookJuQing();
	afx_msg void OnBt_HookRetChooseRole();
	afx_msg void OnBtPacket_AcceptGongHuiTask();
	afx_msg void OnBtPacket_AcceptCatTask();
	afx_msg void OnBtPacket_FindSoldProduct();
	afx_msg void OnBtPacket_NextProductPage();
	afx_msg void OnBtPacket_BuyTheProduct();
	afx_msg void OnBtPacket_RefreshMySellList();
	afx_msg void OnBtPacket_SaleMyProduct();
	afx_msg void OnBtPacket_InviteTeam();
	afx_msg void OnBtPacket_JoinTeam();
	afx_msg void OnBtPacket_LeaveTeam();
	afx_msg void OnBtPacket_RegLeaveTeam();
	afx_msg void OnBtPacket_CreateTeam();
	afx_msg void OnBtPacket_SelectCaptainLine();
	afx_msg void OnBt_FactureInfo();
	afx_msg void OnBtPacket_ActiveGainTasks();
	afx_msg void OnBtPacket_SearchJiYunFb();
	afx_msg void OnBt_HookHitBody();
	afx_msg void OnBt_CalcRoleDirection();
	afx_msg void OnBtPacket_ZuoQiQiu();
	afx_msg void OnBt_FarmCollectPoint();
	afx_msg void OnBt_TraSupply();
	afx_msg void OnBtPacket_GiveUpFb();
	afx_msg void OnBtPacket_PlantSeed();
	afx_msg void OnBt_YouHuaGame();
	afx_msg void OnBtPacket_SendMail();
	afx_msg void OnBt_TraBuffState();
	afx_msg void OnBt_TraBuffInfo();
	afx_msg void OnBt_TraGiftBag();
	afx_msg void OnBtPacket_ReceiveGift();
	afx_msg void OnBt_GetRoleLvlUpInfo();
	afx_msg void OnBtPacket_TidyItem();
	afx_msg void OnBtPacket_GiveUpTask();
	afx_msg void OnBtPacket_LeaveTeachFb();
	afx_msg void OnBtPacket_TeachFb();
	afx_msg void OnBt_HookUseSkillInSky();
	afx_msg void OnBtPacket_GainWeekCredit();
	afx_msg void OnBtPacket_SyncPos();
	afx_msg void OnBt_TidyItem();
	afx_msg void OnBt_HookActorDiMianPos();
	afx_msg void OnBt_TraTaskFbInfo();
	afx_msg void OnBtPacket_DeleteRole();
	afx_msg void OnBtPacket_AddFriend();
	afx_msg void OnBt_ShangJinTaskInfo();
	afx_msg void OnBnClickedButton110();
	afx_msg void OnBnClickedButton111();
	afx_msg void OnBnClickedButton112();
	afx_msg void OnBnClickedButton113();
	afx_msg void OnBnClickedButton114();
	afx_msg void OnBnClickedButton115();
	afx_msg void OnBt_KillCheckThread();
	afx_msg void OnBt_CrcTest();
	afx_msg void OnBt_ResumeCheckThread();
	afx_msg void OnBt_HookGetTickCount();
	afx_msg void OnBt_HookKiUser();
	afx_msg void OnBt_WriteCheck();
	afx_msg void OnBt_HookEnumMod();
	afx_msg void OnBt_TraverseCollect();
	afx_msg void OnBt_CallLpkExternFn();
	afx_msg void OnBt_HookJingCaiJingTou();
	afx_msg void OnBt_UiTest();
	afx_msg void OnBt_UiPressTest();
	afx_msg void OnBt_UiInputTest();
	afx_msg void OnBt_InputTest1();
};
