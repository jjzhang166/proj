#pragma once
/*
@author zhp
@date 2014/8/1 5:25
@purpose for game const variable
*/
#include <string>

//身体部位名
const std::wstring kBodyPartName_Head = L"HEAD";
const std::wstring kBodyPartName_Upper = L"UPPER";
const std::wstring kBodyPartName_Lower = L"LOWER";
const std::wstring kBodyPartName_Hand = L"HAND";
const std::wstring kBodyPartName_Foot = L"FOOT";
const std::wstring kBodyPartName_Face = L"FACE";
const std::wstring kBodyPartName_Eye = L"EYE";

//栏目
//装备栏
const std::wstring kTabName_Normal = L"EQUIPMENT_TAB";
const std::wstring kTabName_Consume = L"CONSUME_TAB";
const std::wstring kTabName_Materail = L"MATERIAL_TAB";

//物品品质
const std::wstring kItemQuality_Low = L"LOW";
const std::wstring kItemQuality_Middle = L"MIDDLE";
const std::wstring kItemQuality_High = L"HIGH";
const std::wstring kItemQuality_Rare = L"RARE";
const std::wstring kItemQuality_Legendary = L"LEGENDARY";

//掉落物品名
const std::wstring kDroppedItemType_Gold = L"Gold";

//中英映射
const std::wstring kEn2Ch_MainTitle = L"MainTitle";
const std::wstring kEn2Ch_Name = L"Name";
const std::wstring kEn2Ch_BeginDesc = L"BeginDesc";

//发包消息相关
const std::wstring kSendMsg_CmdNameFastRun = L"FastRun";
const std::wstring kSendMsg_CmdNameStand = L"Stand";
const std::wstring kSendMsg_CmdNameRealize = L"Realize";

//技能
const std::wstring kSkillJobTypeName_Any = L"通用";

//角色最大等级
const int kRoleMaxLevel = 200;

//冷却类别
const std::wstring kItemCooldownCategory_Unseal = L"Unseal";
const std::wstring kItemCooldownCategory_Identify = L"Identify";

//game obj type
const std::wstring kGameObjType_NPC = L"NPC";

//status相关
const std::wstring kStatusName_AttackPower = L"AttackPower";