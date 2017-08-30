#pragma once
/*
@author zhp
@date 2014-1-04
@purpose 逆向分析出的游戏中的一些类型、结构等
*/
#include "stdafx.h"

namespace GType{
	class G_HttpSendCall_FirstObj;

	//http发包call所需对象1
	class HttpSendCall_NeededObj1;
	//http发包call所需对象2
	class HttpSendCall_NeededObj2;
	//http发包call，发包结构体
	struct stHttpSendCall_Content
	{
		stHttpSendCall_Content();

		const char*		str_http_content;
		DWORD			unknown_equal_zero;
		int				total_size_http_content;			//http package总长度
	};
	//发包所需对象
	struct stHttpSendCall_NeededObj3{
	private:
		stHttpSendCall_NeededObj3();
		char				buffer_unknown[0x60];

		/* static void* operator new( size_t size );
		 static void operator delete( void* pointee );*/
	};

	enum enHttpSendCall_Result
	{
		enHttpSendCall_Result_Fail = -1,	//失败
		enHttpSendCall_Result_Succeed = 0,	//成功
	};

	enum enGameState
	{
		enGameState_Invalid,						//无效状态
		enGameState_PressOnClick,					//点击继续状态
		enGameState_NoticeDlg,						//公告
		enGameState_TrainerSelect,					//教练选择
		enGameState_TrainerSelected,				//教练选定
		enGameState_Managers,						//经理人模式状态
		enGameState_Custom = 6,						//自定义模式状态
		//以下对阵模式会依赖于这些枚举的数值的，所以不要随意更改顺序
		enGameState_Managers_Computer,				//经理人之对阵电脑状态
		enGameState_Managers_Friend,				//经理人之对阵好友状态
		enGameState_Managers_Random,				//经理人之对阵随机对手
		enGameState_Managers_Contract,				//经理人之续约状态
		enGameState_Match_Wait = 11,				//等待房间场景
		enGameState_Match_Choose_Player,			//选择球员
		enGameState_Match_Ready,					//准备比赛场景
		enGameState_Match_BeginFirtAnimate,			//开始踢球前的动画
		enGameState_MatchEnd,						//比赛结束
		enGameState_MatchEnd_Prize = 16,			//比赛结束之比赛奖励
		enGameState_MatchEnd_Back,					//比赛结束之返回比赛房间
		enGameState_ClubManage,						//俱乐部管理场景
		enGameState_AutoSelectPlayer,				//自动选择球员成功
		enGameState_CreateCoach,					//创建教练场景
	};

	//创建教练子状态
	enum enCreateCoach_SubState
	{
		enCC_SubState_Invalid,
		//enCC_SubState_Create,				//创建教练
		enCC_SubState_InputName,			//输入教练名
		enCC_SubState_SelectTeam,			//选择球队
	};

	//比赛模式
	enum enMatchMode
	{
		enMatchMode_Invalid,			//无效
		enMatchMode_Custom,				//自定义模式
		enMatchMode_Managers,			//经理人模式
	};
	
	//对阵模式
	enum enFightMode
	{
		enFightMode_Invalid,			//无效
		enFightMode_Friend,				//对阵好友
		enFightMode_Computer,			//对阵电脑
		//enFightMode_Random,				//对阵随机对手，暂且先不做
	};

	//比赛信息
	struct stMatchInfo{
		DWORD				room_id;			//房间ID
		DWORD				match_id;			//比赛ID

	public:
		stMatchInfo();
		DWORD GetRoomId() const;
		void SetRoomId( DWORD id );
		DWORD GetMatchId() const;
		void SetMatchId( DWORD id );
	};

	//球员职位
	enum enPlayerPosition{
		enPlayerPosition_No = 0,				//无职位
		enPlayerPosition_Third,					//候补球员
		enPlayerPosition_Second,				//替补球员
		enPlayerPosition_First,					//首发球员
	};
}