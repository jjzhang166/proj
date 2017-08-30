#pragma once
/*
@author zhp
@date 2014/9/15 18:33
@purpose cmn events name
*/
#include <string>
/*
PEN->proto event name
CEN->custom event name
PPEN->private proto event name
*/

//////////////////////////////////////////////////////////////////////////
const std::string kPEN_AutoTouchGameEvent("自动触发一个游戏事件");
const std::string kPEN_SelectChannel("选择频道事件");
const std::string kPEN_SelectRole("选择角色事件");
//const std::string kPEN_CreateRole("创建角色事件");
const std::string kPEN_RoleInGame("角色进入到游戏事件");
const std::string kPEN_RetCity("从副本返回到城镇事件");
const std::string kPEN_EnterToFb("进入到副本事件");
const std::string kPEN_AutoRunOneTaskHandler("自动做一个任务处理事件");
const std::string kPEN_AfterDoneOneTask("处理完一个任务事件");
const std::string kPEN_FatigueZero("疲劳值使用完事件");
const std::string kPEN_ChangeJob("转职事件");
const std::string kPEN_AutoDoOneTask("自动接和做任务事件");
const std::string kPEN_HandleMails("处理邮件事件");
const std::string kPEN_HandleDonAllMails("邮件处理完成事件");
const std::string kPEN_HandleTrade("处理交易事件");
const std::string kPEN_DoneTrade("交易完成事件");
//////////////////////////////////////////////////////////////////////////
const std::string kPPEN_AcceptTaskHandlerTasks("_72765A3DFC4C_");
const std::string kPPEN_RunOneAcceptedTaskHandler("_B20354E405E7_");
const std::string kPPEN_RuneOneCompletableTaskHandler("_673E6A8E3E5A_");
const std::string kPPEN_AfterDoneRunOneTaskHandler("_89BC43F3E1A_");
const std::string kPPEN_HandleMails("_1C875673DF5_");
const std::string kPPEN_HandleMailsImpl("_588DD15CA0AF7_");
const std::string kPPEN_HandleMailRelation("_B9069E578B7_");
const std::string kPPEN_HandleTrade("_0E7B074C5131_");
const std::string kPPEN_HandleTradeImpl("_87BBECB29AF7_");
const std::string kPPEN_HandleTradeRelation("_89A887CD3B5D_");
const std::string kPPEN_HandleBuyOtherProducts("_A30B54E4D40B_");
const std::string kPPEN_KeepScriptRunningTimer("_437DE16395BB_");
const std::string kPPEN_WaitRetSelectRole("_E98F6EB90ECC_");
//////////////////////////////////////////////////////////////////////////