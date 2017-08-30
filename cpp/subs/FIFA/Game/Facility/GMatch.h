#pragma once
/*
@author zhp
@date 2014-3-1 16:49
@purpose for match
*/
#include "MatchInterface.h"
#include "Common/UsefulClass.h"

class MatchModeImpl;
//一般性的实现
class FightModeImpl : public IFightMode{
protected:
	FightModeImpl();

public:
	virtual void PressSpecifyFightMode( IoTimer* timer );
	virtual IMatchMode* GetParent() const;
	virtual void SetParent( IMatchMode* parent );
	virtual void BeginMatch();
	virtual void EnterWaiting();
	virtual void EnterChoosePlayer();
	virtual void EnterMatchReady();
	virtual void EnterMatchAllReady();
	virtual void MatchEnd();
	virtual void MatchEnd_Prize();
	virtual void MatchEnd_Back();

public:
	//是否可以点击选择球员之开始
	virtual bool CanPressChoosePlayer_Begin() const;
	//是否可以点击就绪之开始
	virtual bool CanPressReady_Begin() const;

protected:
	//是否可以点击选择对阵模式了
	virtual bool IsCanPressFightMode() const;
	//lua中处理点击具体对阵模式的函数名
	virtual const char* GetLuaHandlePressFightModeFuncName() const;
	//是否可以开始比赛了
	virtual bool IsCanBegin() const;
	//开始比赛
	void BeginMatch( IoTimer* timer );
	//是否可以点击比赛结束之比赛结果
	bool CanPressMatchEnd_Result() const;
	//点击比赛结束之比赛结果
	void PressMatchEnd_Result( IoTimer* timer );
	//是否可以点击比赛结束之比赛奖励
	bool CanPressMatchEnd_Prize() const;
	//点击比赛结束之比赛奖励
	void PressMatchEnd_Prize( IoTimer* timer );

protected:
	MatchModeImpl*				parent_;
	int							match_cnt_;					//比赛次数
	time_t						time_match_begin_;			//开始比赛时间
};

//fight mode invalid
class FightMode_Invalid : public FightModeImpl{
public:
	virtual GType::enFightMode GetFightMode() const;
	virtual void PressSpecifyFightMode( IoTimer* timer );
	virtual void BeginMatch();
	virtual void EnterWaiting();
	virtual void EnterChoosePlayer();
	virtual void EnterMatchReady();
	virtual bool CanPressChoosePlayer_Begin() const;
	virtual bool CanPressReady_Begin() const;
	virtual void MatchEnd();
	virtual void MatchEnd_Prize();
	virtual void MatchEnd_Back();

	DECLARE_DYNCREATE(FightMode_Invalid)
};

class GMatchMgr;
//一般性的实现
class MatchModeImpl : public IMatchMode, public RuntimeClassReg{
protected:
	MatchModeImpl();
	~MatchModeImpl();

public:
	virtual bool IsSupportFightMode( GType::enFightMode fight_mode ) const;
	virtual bool ChooseFightMode( GType::enFightMode fight_mode );
	virtual IFightMode* FightMode() const;
	virtual void SafeDelte();
	virtual void PressSpecifyMatchMode( IoTimer* timer );
	virtual void EnteredRoom();
	virtual IGMatchMgr* GetParent() const;
	virtual void SetParent( IGMatchMgr* parent );
	virtual const char* GetLuaHandlePressBeginGameFuncName() const;
	virtual GType::stMatchInfo& GetMatchInfo();
	virtual void MatchRoom_CloseContractDlg();

public:
	void EnterWaitingImpl( IoTimer* timer = nullptr );
	void EnterChoosePlayerImpl( IoTimer* timer = nullptr );
	void EnterMatchReadyImpl( IoTimer* timer = nullptr );

protected:	
	//是否可以点击选择比赛模式了
	bool IsCanPressMatchMode() const;
	//lua中处理点击具体比赛模式的函数名
	virtual const char* GetLuaHandlePressMatchModeFuncName();
	//比赛房间之续约对话框是否关闭了
	bool IsMatchRoomContractDlgClosed() const;
	//执行比赛房间之关闭续约对话框
	void ImplMatchRoomCloseContractDlg( IoTimer* timer );
	
protected:
	FightModeImpl*				fight_mode_;
	GMatchMgr*					parent_;

private:
	FightMode_Invalid			fight_mode_invalid_;
};

//match model invalid
class MatchModel_Invalid : public MatchModeImpl{
public:
	virtual GType::enMatchMode GetMatchMode() const;

	DECLARE_DYNCREATE(MatchModel_Invalid)
};

//game match manage
class GMatchMgr : public IGMatchMgr, public RuntimeClassReg{
	GMatchMgr();
	friend IGMatchMgr& IGMatchMgr::GetMe();

public:
	~GMatchMgr();

public:
	//是否支持指定的比赛模式
	virtual bool IsSupportMatchMode( GType::enMatchMode match_mode );
	//选择比赛模式与对阵模式
	virtual bool ChooseMatch( GType::enMatchMode match_mode );
	//得到当前比赛模式
	virtual GType::enMatchMode GetCurMatchMode() const;
	//安全释放
	virtual void SafeDelete();
	//得到比赛模式对象
	virtual IMatchMode* MatchMode() const;
	virtual void PressFriendshipMatch();
	//比赛信息
	GType::stMatchInfo& GetMatchInfo();

	//implement
private:
	//选定场景下，是否就绪
	bool IsReadyAtSelectedScene();
	//点击友谊赛
	void PressFriendshipMatch( IoTimer* timer );

private:
	IMatchMode*					match_mode_;
	MatchModel_Invalid			match_mode_invalid_;
	GType::stMatchInfo			match_info_;
};

//经理人模式
class MatchMode_Managers : public MatchModeImpl{
public:
	MatchMode_Managers();

public:
	virtual GType::enMatchMode GetMatchMode() const;
	virtual void EnteredRoom();
	virtual const char* GetLuaHandlePressBeginGameFuncName() const;

protected:
	virtual const char* GetLuaHandlePressMatchModeFuncName();

	DECLARE_DYNCREATE(MatchMode_Managers)
};

//自定义模式
class MatchMode_Custom : public MatchModeImpl{
public:
	MatchMode_Custom();

public:
	virtual GType::enMatchMode GetMatchMode() const;
	virtual void EnteredRoom();

	DECLARE_DYNCREATE(MatchMode_Custom)
};

//对阵电脑
class FightMode_Computer : virtual public FightModeImpl{
public:
	virtual GType::enFightMode GetFightMode() const;

	DECLARE_DYNCREATE(FightMode_Computer)
};

//对阵好友
class FightMode_Friend : virtual public FightModeImpl{
public:
	virtual GType::enFightMode GetFightMode() const;

	DECLARE_DYNCREATE(FightMode_Friend)
};

//经理人模式之对阵电脑
class FightMode_Managers_Computer : public FightMode_Computer{
public:

protected:
	virtual bool IsCanPressFightMode() const;
	virtual const char* GetLuaHandlePressFightModeFuncName() const;
	virtual bool IsCanBegin() const;
	virtual void BeginMatch();

	DECLARE_DYNCREATE(FightMode_Managers_Computer)
};

//经理人模式之对阵好友
class FightMode_Managers_Friend : public FightMode_Friend{
public:

protected:
	virtual bool IsCanPressFightMode() const;
	virtual const char* GetLuaHandlePressFightModeFuncName() const;
	virtual bool IsCanBegin() const;
	virtual void BeginMatch();

	DECLARE_DYNCREATE(FightMode_Managers_Friend)
};
//对阵随机对手
//暂且先不支持
/*
class FightMode_Random : public FightModeImpl{
public:
	virtual GType::enFightMode GetFightMode() const;

	DECLARE_DYNCREATE(FightMode_Random)
};*/