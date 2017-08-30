#pragma once
#include "stdafx.h"
#include <Net/client.h>
#include <Protocol/P_NormalMsg.h>
#include "Net/IoTimer.h"
#include "Facility/GTypes.h"
#include "FuncObjs/FuncObjInter.h"
#include "GLua/GLuaC.h"
#include <Net/JsonCall.h>
#include <dinput.h>
#include "Facility/GClass.h"
#include <Poco/DateTime.h>
#include "../Console/CfgStruct.h"
#include <Net/Bufcall_Socket.h>
#include <Net/BufCallCoorTrans.h>
#include <Communicate/CG_Defines.h>


struct stDxinput_PressKey{
	BYTE				di_key_;			//dinput.h中的DIK_ESCAPE等
	bool				is_down_;			//是否按下
};
// DirectInput keyboard state buffer 
typedef BYTE (*DI_KeyStateBuffer)[256];
typedef std::queue<stDxinput_PressKey> DI_PressKeyQueue;
typedef std::pair<DI_PressKeyQueue, SharedMutex> DI_PressKeyPair;
typedef boost::shared_ptr<DI_PressKeyPair> DI_PressKeyPtr;
//////////////////////////////////////////////////////////////////////////
const P_ObjId kObjIdNormal = DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_(40);
class FuncObj_NormalC;
class SessionOperC : public BufCall_SocketExcept{
public:
	SessionOperC( FuncObj_NormalC& func_obj_normal );

	//用于验证
public:
	//设置是否登陆成功
	void SetLoginSucceed( bool succeed );
	//设置用户结束时间
	void SetEndDateTime( std::time_t end_time );
	//设置关键数据
	void SetAddrDatas( const Json::Value& addr_datas );

	//其他
public:
	//设置服务id
	void SetGameSerIdx( int ser_idx );
	//设置游戏号的种类
	void SetGameAcType( int game_ac_type );
	//从控制台发给game的固定的字符串
	void StableString( const std::string& stable_str );
	//效验公钥
	void CheckRsaPubKey( const std::string& encode_str );

	//测试用
public:
	//得到所有球员数据
	void GetPlayersInfo();
	//得到所有物品数据
	void GetItemsInfo();
	//得到我的出售列表
	void GetMyOfferList();
	//得到其他号的出售项
	void GetOtherOfferItems();

private:
	FuncObj_NormalC&				func_obj_normal_;

	BC_REG_CLS_DECLARE(SessionOperC);
};
//////////////////////////////////////////////////////////////////////////
bool MakeTransRoute( P_CoorPacketToT& out_route, enGameAcType game_ac_type, int ser_id );
P_CoorPacketToT MakeTransRoute( enGameAcType game_ac_type, int ser_id );
const P_CoorPacketToT& GetTransRouteAny();
//协调服务，用于收金等
//game coordinate
class GCoordinateC : public BufCallCoorTrans{
public:
	GCoordinateC( FuncObj_NormalC& func_obj_normal );

	//广播，发消息
public:
	//请求拍卖数据
	void QuestSoldInfo();
	//自己若是仓库号，则发给普通号，若是普通号则发给仓库号
	void SellPlayersSwitch( const Json::Value& sold_players );
	//取消拍卖，通知对方
	void CancelSale_OtherSide( const Json::Value& sold_players );

	//响应消息
public:
	//请求拍卖的信息
	void OnQuestSoldInfo();
	//响应拍卖的信息
	void OnSoldInfo( const Json::Value& sold_players );
	//响应取消拍卖
	void OnCancelSale( const Json::Value& sold_players );

private:
	FuncObj_NormalC&				nor_;

	BC_REG_CLS_DECLARE(GCoordinateC);
};
//////////////////////////////////////////////////////////////////////////
class FuncObj_NormalC : public Connection, public Singleton<FuncObj_NormalC, Singleton_Instance>{
	friend class SessionOperC;
	friend class GCoordinateC;

	//type definition
public:
	//关键数据
	typedef std::vector<DWORD> AddrDatasT;

private:
	FuncObj_NormalC& operator = ( const FuncObj_NormalC& );
	friend void IoRun();

public:
	FuncObj_NormalC();
	~FuncObj_NormalC();

public:
	//初始化游戏
	bool InitGame();
	//初始化FuncObj_InterInjectComposite
	bool InitInter( HWND game_main_wnd );
	//得到inter inject
	FuncObj_Inject* GetInterInject();
	//得到inter GWnd
	FuncObj_GWnd* GetInterGWnd();
	//得到glua
	GLuaC& GetGLua();
	LuasScriptEchoC& GetLuaScript();
	//get json echo
	JsonEcho& GetJsonEcho();
	//得到帐号
	const std::string& GetAccount() const;
	//get dx input press key queue
	DI_PressKeyPtr& GetDI_PressKeyPtr();
	//得到session operate
	SessionOperC& GetSessionOper();
	//得到coordinate
	GCoordinateC& GetCoordinate();
	//设置关键数据
	void SetAddrDatas( const AddrDatasT& addr_datas );
	TimeElapse& GetSetMatchTimer();
	enGameAcType SwitchGameAcType() const;

	//可供控制台调用
public:
	//让窗口显示在最前
	void ForegroundTheWindow() const;

	//功能函数
public:
	//关闭所有打开的UI
	void CloseAllUi();
	//就绪状态下(教练选定场景下)，派发任务
	void ReadyState_DistributeTask();
	//设置游戏状态
	void SetGameState( GType::enGameState game_state );
	//得到游戏状态
	GType::enGameState GetGameState();
	static bool IsMatchingState( GType::enGameState game_state );
	bool CanChangeMatchingState( GType::enGameState game_state ) const;
	//点击dx方面的按键
	void PressDxinputKey( BYTE di_key );

	//for lua
public:
	//点击继续游戏
	void LuaContinueOnClick();
	//close notice dialog
	void LuaCloseNoticeDialog();
	//教练选择
	void LuaTrainerSelectScene( std::size_t trainer_idx );
	//教练选定
	void LuaTrainerSelectedScene();
	//关闭每日任务dialog
	void LuaCloseTaskDlg();
	//关闭升级弹框
	void CloseLevelUpDlg( int milliseconds, int times );
	void OnDisconnect( IoConnectBase* connect );
	//封号
	void BanAccount();
	//是否可以进行普通号的交易流程
	bool CanNormalTradeProcedure();
	//是否可以进行仓库号的交易流程
	bool CanStoredTradeProcedure();
	void ElapseTimers();

	//场景相关
public:
	//是否进入到点击继续场景
	bool IsEnterContinueOnClick();
	//点击继续游戏
	void ContinueOnClick( IoTimerPtr& timer );
	//当前顶层窗口是否是公告dlg
	bool IsNoticeDlgOfMosttop();
	//close notice dialog
	void CloseNoticeDialog( IoTimerPtr& timer );
	//是否是教练选择场景
	bool IsTrainerSelectScene();
	//教练选择
	void TrainerSelectScene( IoTimer* timer, std::size_t trainer_idx );
	//是否教练选定场景
	bool IsTrainerSelectedScene();
	//教练选定场景
	void TrainerSelectedScene( IoTimer* timer );
	//每日任务dialog是否弹出
	bool IsTaskDlgShown();
	//关闭每日任务dlg
	void CloseTaskDlg( IoTimer* timer );
	//教练流程
	void TrainerProcedure();

	//创建教练流程
public:
	void CreateTrainerProcedure( IoTimer* timer );
	//输入教练名
	void InputCoachName( char c );

	//修改发包相关,由游戏线程调用
public:
	//是否需要修改发包内容
	bool NeedReplaceSendContent();
	//修改发包内容
	bool ReplaceSendContent( GType::stHttpSendCall_Content& send_content );

	//静态函数
public:
	//唯一实例
	static FuncObj_NormalC* GetInstance();
	//得到inter GWnd
	static FuncObj_GWnd* GetGWnd();

	//处理消息
protected:
	bool OnModifyAccount( P_Modify_Account& modify_account );
	//load library
	bool OnTestLoadLib( P_Exec_TestLoadLib& load_lib );
	//unload library
	bool OnTestUnloadLib( P_Exec_TestUnloadLib& unload_lib );
	//call LuaTest
	bool OnCallLuaFunction( P_Exec_CallLuaFunction& call_lua_test );

	//处理事件
protected:
	
	//私有成员函数
private:
	//清理
	void Clear();

protected:
	bool OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size );
	bool PreConnect();
	bool OnConnected( IoConnectBase* connect );

public:
	std::string					account_;
	std::string					password_;
	std::string					game_ser_name_;
	bool						auto_run_script_;
	HMODULE						test_dll_;

private:
	FuncObj_InterInjectComposite*	inject_;			//代表了注入线程
	boost::shared_ptr<JsonEcho>		json_echo_;
	DI_PressKeyPtr					di_press_key_ptr_;
	SessionOperC					session_oper_;
	GCoordinateC					coordinate_;
	boost::shared_ptr<LuasScriptEchoC>					lua_script_;

	//游戏辅助变量
private:
	GType::enGameState				game_state_;
	int								game_ser_idx_;		//游戏服务器ID

	//验证相关
private:
	//Crypto_RsaInitor	rsa_;
	boost::optional<bool>			login_succeed_;			//验证登陆是否成功
	boost::optional<DateTimeLocal>	end_date_time_;			//验证结束时间
	enGameAcType		game_ac_type_;			//帐号种类
	Poco::DateTime					time_normal_trade_;		//普通号交易流程的计时
	TimeElapse						timer_set_match_end_;

public:
	IoTimerPtr						timer_continue_on_click_;
	IoTimerPtr						timer_close_notice_dlg_;
};