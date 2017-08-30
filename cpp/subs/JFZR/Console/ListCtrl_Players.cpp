#include "stdafx.h"
#include "ListCtrl_Players.h"

void CListCtrlStyled::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	//style
	{
		DWORD dwStyle = GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;		//选中某行使整行高亮（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_GRIDLINES;			//网格线（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_CHECKBOXES;			//item前生成checkbox控件
		SetExtendedStyle(dwStyle);				//设置扩展风格
	}
}
void CList_MyOfferList::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 175;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kPlayerId, _T("球员ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kSoldState, _T("拍卖状态"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}

void CList_OtherOffers::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 225;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}

void CList_Skills::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 225;
	InsertColumn(kSkillName, _T("名称"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSkillLvl, _T("等级"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSkillId, _T("id"), LVCFMT_LEFT, kColumnWidth);
}

void CList_Skills::AddSkillInfo( const stSkillInfo& skill_info )
{
	const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, CW2CT(skill_info.skill_name_.c_str()));
	CString str_tmp;
	str_tmp.Format(_T("%u"), skill_info.skill_lvl_);
	SetItemText(kItemMax, kSkillLvl, str_tmp);
	str_tmp.Format(_T("%u"), skill_info.skill_id_);
	SetItemText(kItemMax, kSkillId, str_tmp);
}

void CList_GameObjs::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 150;
	InsertColumn(kObjType, _T("类型"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSerial, _T("序号"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kJobName, _T("职业名"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kObjName, _T("对象名"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kObjTypeNameInGame, _T("游戏类型名"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kCurHp, _T("HP"), LVCFMT_LEFT, kColumnWidth);
}

void CList_GameObjs::AddGameObjInfo( const stGameObjInfo& gameobj_info )
{
	const auto kItemMax = GetItemCount();
	CString str_tmp;
	switch (gameobj_info.obj_type_)
	{
	case kGOT_Invalid:
		str_tmp = _T("无效");
		break;
	case kGOT_MISC:
		str_tmp = _T("MISC");
		break;
	case kGOT_Npc:
		str_tmp = _T("NPC");
		break;
	case kGOT_Player:
		str_tmp = _T("玩家");
		break;
	case kGOT_Unknown:
		str_tmp = _T("掉落物品");
		break;
	case kGOT_Monster:
		str_tmp = _T("怪物");
		break;
	case kGOT_ParseObjType:
		str_tmp = _T("用于解析");
		break;
	default:
		str_tmp = _T("未知");
		break;
	}
	InsertItem(kItemMax, str_tmp);
	str_tmp.Format(_T("%u"), gameobj_info.obj_serial_);
	SetItemText(kItemMax, kSerial, str_tmp);
	SetItemText(kItemMax, kJobName, CW2CT(gameobj_info.obj_job_name_.c_str()));
	SetItemText(kItemMax, kObjName, CW2CT(gameobj_info.obj_name_.c_str()));
	SetItemText(kItemMax, kObjTypeNameInGame, CW2CT(gameobj_info.game_obj_type_name_.c_str()));
	str_tmp.Format(_T("%u"), gameobj_info.cur_hp_);
	SetItemText(kItemMax, kCurHp, str_tmp);
}

CList_GameObjs::stGameObjInfo::stGameObjInfo() : GameObjBase(kGOT_Invalid)
{

}

void CList_Items::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 125;
	InsertColumn(kItemName, _T("名称"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kItemQuality, _T("品质"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kItemDurable, _T("耐久度"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kItemCnt, _T("数量"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kIsSeal, _T("是否封印"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kNeedIdentification, _T("需要鉴定"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kTabCategory, _T("table"), LVCFMT_LEFT, kColumnWidth);
}

void CList_Items::AddItem( const stItem& gitem, GType::enTabCategory tab_category )
{
	/*
	const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, CW2CT(gitem.item_id_.c_str()));
	CString str_tmp;
	str_tmp.Format(_T("%u"), gitem.item_quality_);
	SetItemText(kItemMax, kItemQuality, str_tmp);
	str_tmp.Format(_T("%u"), gitem.item_durable_);
	SetItemText(kItemMax, kItemDurable, str_tmp);
	str_tmp.Format(_T("%u"), gitem.item_cnt_);
	SetItemText(kItemMax, kItemCnt, str_tmp);
	if (gitem.is_sealed_)
		str_tmp = _T("是");
	else
		str_tmp = _T("否");
	SetItemText(kItemMax, kIsSeal, str_tmp);
	if (gitem.unidentified_)
		str_tmp = _T("是");
	else
		str_tmp = _T("否");
	SetItemText(kItemMax, kNeedIdentification, str_tmp);
	str_tmp.Format(_T("%u"), gitem.game_item_addr_);
	SetItemText(kItemMax, kTabCategory, str_tmp);
	*/
}

void CList_Room::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 150;
	InsertColumn(kRoomName, _T("房间名称"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kRoomId, _T("房间ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kDoorName, _T("门名称"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kDoorPos, _T("门坐标"), LVCFMT_LEFT, kColumnWidth);
}

void CList_Room::SetRoomInfo( const stRoomInfo& room_info )
{
	/*const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, CW2CT(room_info.room_name_.c_str()));
	SetItemText(kItemMax, kRoomId, CW2CT(room_info.room_id_.c_str()));
	for ( auto i = 0; i != _countof(room_info.doors_); ++i )
	{
		AddDoorInfo(*(stDoorInfo*)&room_info.doors_[i]);
	}*/
}

void CList_Room::AddDoorInfo( const stDoorInfo& door_info )
{
	/*const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, _T(""));
	SetItemText(kItemMax, kDoorName, CW2CT(door_info.door_name_.c_str()));
	CString str_tmp;
	str_tmp.Format(_T("%f,%f,%f"), door_info.door_pos_.x_, door_info.door_pos_.y_, door_info.door_pos_.z_);
	SetItemText(kItemMax, kDoorPos, str_tmp);*/
}

void CList_Task::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	/*const auto kColumnWidth = 125;
	InsertColumn(kItemName, _T("名称"), LVCFMT_LEFT, kColumnWidth);*/
}
