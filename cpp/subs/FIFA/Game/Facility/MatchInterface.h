#pragma once
/*
@author zhp
@date 2014-3-1 20:33
@purpose for match facility interface
*/
#include "stdafx.h"
#include "GTypes.h"
#include "Net/IoTimer.h"

class IMatchMode;
//fight mode interface
class IFightMode : public CObject{
public:
	virtual ~IFightMode();
	//得到对阵模式
	virtual GType::enFightMode GetFightMode() const = 0;
	//点击具体的对阵模式
	virtual void PressSpecifyFightMode( IoTimer* timer ) = 0;
	//get parent
	virtual IMatchMode* GetParent() const = 0;
	//set parent
	virtual void SetParent( IMatchMode* parent ) = 0;
	//开始比赛
	virtual void BeginMatch() = 0;
	//进入了等待状态
	virtual void EnterWaiting() = 0;
	//进入了选择球员
	virtual void EnterChoosePlayer() = 0;
	//进入了比赛就绪状态
	virtual void EnterMatchReady() = 0;
	//所有人都已就绪，即开始比赛的动画状态
	virtual void EnterMatchAllReady() = 0;
	//比赛结束
	virtual void MatchEnd() = 0;
	//比赛结束之比赛奖励
	virtual void MatchEnd_Prize() = 0;
	//比赛结束之返回比赛房间
	virtual void MatchEnd_Back() = 0;
};

class IGMatchMgr;
//match mode interface
class IMatchMode : public CObject{
public:
	virtual ~IMatchMode();
	//测试是否支持指定的对阵模式
	virtual bool IsSupportFightMode( GType::enFightMode fight_mode ) const = 0;
	//选择指定的对阵模式
	virtual bool ChooseFightMode( GType::enFightMode fight_mode ) = 0;
	//得到比赛模式
	virtual GType::enMatchMode GetMatchMode() const = 0;
	//得到当前对战模式
	virtual IFightMode* FightMode() const = 0;
	//安全释放
	virtual void SafeDelte() = 0;
	//点击具体的比赛模式
	virtual void PressSpecifyMatchMode( IoTimer* timer ) = 0;
	//已经进入房间了
	virtual void EnteredRoom() = 0;
	//get parent
	virtual IGMatchMgr* GetParent() const = 0;
	//set parent
	virtual void SetParent( IGMatchMgr* parent ) = 0;
	//lua中处理点击开始对阵模式的函数名
	virtual const char* GetLuaHandlePressBeginGameFuncName() const = 0;
	//得到match info
	virtual GType::stMatchInfo& GetMatchInfo() = 0;
	//关闭比赛房间之续约对话框
	virtual void MatchRoom_CloseContractDlg() = 0;
};

//interface game manager
class IGMatchMgr{
public:
	static IGMatchMgr& GetMe();
	virtual ~IGMatchMgr();
	//是否支持指定的比赛模式
	virtual bool IsSupportMatchMode( GType::enMatchMode match_mode ) = 0;
	//选择比赛模式与对阵模式
	virtual bool ChooseMatch( GType::enMatchMode match_mode ) = 0;
	//得到当前比赛模式
	virtual GType::enMatchMode GetCurMatchMode() const = 0;
	//安全释放
	virtual void SafeDelete() = 0;
	//得到比赛模式对象
	virtual IMatchMode* MatchMode() const = 0;
	//点击友谊赛
	virtual void PressFriendshipMatch() = 0;
	//得到比赛信息
	virtual GType::stMatchInfo& GetMatchInfo() = 0;
};