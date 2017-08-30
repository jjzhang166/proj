#pragma once
/*
@author zhp
@date 2014/5/11 13:49
@purpose for test, players info
*/
#include "stdafx.h"
#include <Common/UsefulClass.h>
#include "../Game/Facility/GSkill.h"
#include "../Game/Facility/GameObj.h"
//#include "../Game/Facility/GItem.h"
//#include "../Game/Facility/GRoom.h"

class CListCtrlStyled : public CListCtrl{
protected:
	virtual void PreSubclassWindow();
};

//我的出售列表
class CList_MyOfferList : public CListCtrlStyled, public Singleton<CList_MyOfferList, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kPlayerId,
		kId,
		kSpid,
#endif
		kSoldState,					
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};

protected:
	virtual void PreSubclassWindow();
};

//其他号的出售项
class CList_OtherOffers : public CListCtrlStyled, public Singleton<CList_OtherOffers, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kId,
		kSpid,
#endif
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};

protected:
	virtual void PreSubclassWindow();
};

class CList_Skills : public CListCtrlStyled, public Singleton<CList_Skills, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kSkillName = 0,
		kSkillLvl,
		kSkillId,
	};
	struct stSkillInfo : public GSkillObj{
		friend class CList_Skills;
	};

	//interface
public:
	void AddSkillInfo( const stSkillInfo& skill_info );

protected:
	virtual void PreSubclassWindow();
};

class CList_GameObjs : public CListCtrlStyled, public Singleton<CList_GameObjs, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kObjType = 0,
		kSerial,
		kJobName,
		kObjName,
		kObjTypeNameInGame,		//游戏中对象的类型名
		kCurHp,
	};
	struct stGameObjInfo : public GameObjBase{
		friend class CList_GameObjs;
		stGameObjInfo();
	};

	//interface
public:
	void AddGameObjInfo( const stGameObjInfo& gameobj_info );

protected:
	virtual void PreSubclassWindow();
};

class CList_Items : public CListCtrlStyled, public Singleton<CList_Items, Singleton_Instance>{
public:
	enum enColumns{
		kItemName = 0,
		kItemQuality,
		kItemDurable,
		kItemCnt,
		kIsSeal,
		kNeedIdentification,
		kTabCategory,
	};
	struct stItem/* : public GItem*/{
		friend class CList_Items;
	};

	//interface
public:
	void AddItem( const stItem& item_info, GType::enTabCategory tab_category );

protected:
	virtual void PreSubclassWindow();
};

class CList_Room : public CListCtrlStyled, public Singleton<CList_Room, Singleton_Instance>{
public:
	enum enColumns{
		kRoomName = 0,
		kRoomId,
		kDoorName,
		kDoorPos,
	};
	struct stRoomInfo /*: public GRoomBase*/{
		friend class CList_Room;
	};
	struct stDoorInfo /*: public GRoomDoor*/{
		friend class CList_Room;
	};

public:
	void SetRoomInfo( const stRoomInfo& room_info );

private:
	void AddDoorInfo( const stDoorInfo& door_info );

protected:
	virtual void PreSubclassWindow();
};

class CList_Task : public CListCtrlStyled, public Singleton<CList_Task, Singleton_Instance>{
public:

protected:
	virtual void PreSubclassWindow();
};