#pragma once
/*
@author zhp
@date 2014-2-18 13:49
@purpose function objects of console
*/
#include "Net/server.h"
#include "Protocol/P_BaseStruct.h"
#include "FuncObjs/FuncObjInter.h"
#include "GLuaS.h"
#include "Protocol/P_NormalMsg.h"
#include <Net/Bufcall_Socket.h>
#include <Net/BufCallCoorTrans.h>
#include "../Game/Facility/GTypes.h"
#include "CfgStruct.h"
#include <Communicate/CG_Defines.h>
#include <Net/JsonCall.h>

class FuncObj_NormalS;
class SessionOperS : public BufCall_SocketExcept{
public:
	SessionOperS( FuncObj_NormalS& func_obj_normal );

	//interfaces
public:
	//疲劳
	void SetMatchAddict();
	//设置EP
	void SetEP( double ep );
	//比赛局数
	void SetMatchCnt( int match_cnt );
	//设置运行状态
	void SetRunState( const char* str_run_state );
	//设置教练名
	void SetTrainerName( const char* str_trainer_name );
	//设置教练等级
	void SetTrainerLevel( int level );
	//设置脚本输出信息
	void SetScriptOutputInfo( const std::string& output_info );
	//开始脚本
	void StartRunScript();
	//到了某个游戏场景
	void OnGameState( int game_state );
	//echo message
	void EchoMsg( const char* msg );
	//封号
	void BanAccount();
	//发现好球员
	void DiscoverGoodPlayer();
	//换号
	void ExchangeGameAc();
	//版本到期
	bool VersionEndTime();
	//比赛次数达到上限
	void MatchLimitCnt();
	//设置输出结果
	void SetOutRes(pt_csz out_res);
	//设置球员ID
	void SetSpid(pt_csz spid);

	//test
public:
	//设置球员信息
	void SetPlayersInfo( const Json::Value& players_array );
	//设置物品信息
	void SetItemsInfo( const Json::Value& items_array );
	//设置我的出售列表信息
	void SetMyOfferList( const Json::Value& sold_players );
	//设置其他号的出售项
	void SetOtherOfferItems( const Json::Value& sold_players );

private:
	FuncObj_NormalS&				func_obj_normal_;

	BC_REG_CLS_DECLARE(SessionOperS);
};
//////////////////////////////////////////////////////////////////////////
//协调服务，用于收金等
//game coordinate
class CoorTrans_CS : public CoorTransTrack{
public:
	CoorTrans_CS( FuncObj_NormalS& func_obj_normal, const TrackIterPtrT& track_iter );
	P_CoorTypeT GetCoorType() const;

private:
	FuncObj_NormalS&				nor_;
};
//////////////////////////////////////////////////////////////////////////
class FuncObj_NormalS : public Session{
	friend class SessionOperS;
	friend class CoorTrans_CS;

	//type definitions
public:
	struct stGameInfo{
		double					cur_ep_;
		double					gain_total_ep_;				//获得的总EP
		int						match_cnt_;					//比赛局数
		int						trainer_level_;				//教练等级
		CString					running_state_;				//运行状态
		CString					trainer_name_;				//教练名
		GType::enGameState		game_state_;				//游戏状态

		stGameInfo();
	};
	typedef Iterator<FuncObj_NormalS*> Iterator;
	typedef std::shared_ptr<Iterator> IteratorPtr;

public:
	FuncObj_NormalS( Server& server );

	//interfaces
public:
	//得到FuncObj_IInter
	FuncObj_IInter& GetFuncObjInter();
	//得到glua
	GLuaS& GetGLua();
	//get json echo
	JsonEcho& GetJsonEcho();
	//得到session operate
	SessionOperS& GetSessionOper();
	//得到coordinate
	CoorTrans_CS& GetCoorTrans();
	LuaScriptS& GetLuaScript();
	//游戏信息
	stGameInfo& GetGameInfo();
	//game acount type
	void SetGameAcType( enGameAcType game_ac_type );
	enGameAcType GetGameAcType() const;
	//game service index
	void SetServiceIdx( int service_idx );
	int GetServiceIdx() const;
	//get ac
	const std::string& GetGameAc() const;

public:
	bool SetAccountInfo( const P_Modify_Account& account_info );
	//添加禁止项
	void AddProhibitItem( stSpecificCfg::enProhibitReason reason, LPCTSTR str_run_state );

	//处理消息
protected:
	bool OnCallLuaFuncResult( P_Exec_CallLuaFuncResult& func_res );
	//处理比赛信息
	bool OnMatchInfo( P_Exec_MatchInfo& match_info );
	//远程调用lua
	void CallRemoteLua( const char* lua_func_name, const char* lua_func_param );
	//游戏进程信息
	bool OnGameProccessInfo( const P_Exec_GameProccessInfo& game_proccess_info );

protected:
	virtual bool OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size );
	bool OnConnected( IoConnectBase* connect ) override;
	void DoOnAccepted();
	void OnDisconnect( IoConnectBase* connect ) override;

	//func objs
private:
	FuncObj_IInter			func_obj_inter_;
	GLuaS					glua_;
	JsonEcho				json_echo_;
	SessionOperS			session_oper_;
	CoorTrans_CS			coordinate_;
	LuaScriptS				lua_script_;

private:
	std::string				account_;
	std::string				password_;
	std::string				service_name_;
	stGameInfo				game_info_;
	enGameAcType	game_ac_type_;
	int						service_idx_;
	CString					str_final_run_state_;
	DWORD					game_process_id_;
};
