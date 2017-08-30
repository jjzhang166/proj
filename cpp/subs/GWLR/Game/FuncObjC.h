#pragma once
#include "stdafx.h"
#include <Net/client.h>
#include <Protocol/P_NormalMsg.h>
#include "Net/IoTimer.h"
#include "Facility/GTypes.h"
#include "FuncObjs/FuncObjInter.h"
#include "GLua/GLuaC.h"
#include <Net/Bufcall_Socket.h>
#include "Facility/GClass.h"
#include <Poco/DateTime.h>
#include <Communicate/CG_Defines.h>
#include <Net/BufCallCoorTrans.h>
#include <Common/CmnPriLuaData.h>
#include "Facility/GProtocolR.h"
//////////////////////////////////////////////////////////////////////////
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
	void SetStrDatas( const Json::Value& str_datas );
	//校验控制台
	void CheckConsoleUuid( const std::string& str_uuid, DWORD console_process_id );

	//协调相关
public:

	//获取数据
public:
	void SetMultiHit( int multi_hit_rate );

	//游戏逻辑
public:
	bool CreateRandNameRole();
	bool SelectRole( pt_csz role_name );
	void SetRightBtHit( bool is_right_hit );
	void SetOpenVillagePath( bool open_village_path );
	void SetYouHua(bool is_you_hua);

	//其他
public:
	//设置服务id
	void SetGameSerIdx( int ser_idx );
	//设置游戏号的种类
	void SetGameAcType( int game_ac_type );
	//从控制台发给game的固定的字符串
	void StableString( const std::string& stable_str );
	//让窗口显示在最前
	void ForegroundTheWindow();
	void GetRunInfo(int cnt);

	//拍卖相关
public:
	//得到我的出售列表
	void GetMySellList();
	//得到其他号的出售项
	void GetOtherSellInfo();

	//测试用
public:

private:
	FuncObj_NormalC&				func_obj_normal_;

	BC_REG_CLS_DECLARE(SessionOperC);
};
//////////////////////////////////////////////////////////////////////////
bool MakeTransRoute( P_CoorPacketToT& out_route, enGameAcType game_ac_type, int ser_id );
P_CoorPacketToT MakeTransRoute( enGameAcType game_ac_type, int ser_id );
const P_CoorPacketToT& GetTransRouteAny();
class BufCallCoorTransImpl : public BufCallCoorTrans{
public:
	BufCallCoorTransImpl( FuncObj_NormalC& func_obj_normal );

	//发消息，广播
public:
	//获得对方拍卖信息
	void GainOtherSidePrivateItems();
	//主动发送添加
	void SendAddPrivateItems( const Json::Value& j_value );
	//主动发送给对方阵营，删除
	void SendErasePrivateItems_OtherSide( const Json::Value& j_value );
	//主动发送给己方阵营，删除
	void SendErasePrivateItems_AllSide( const Json::Value& j_value );

	//响应消息
public:
	void OnGainOtherSidePrivateItems();
	//响应添加
	void OnAddPrivateItems( const Json::Value& j_value );
	//响应删除
	void OnErasePrivateItems( const Json::Value& j_value );

	//test
public:

private:
	FuncObj_NormalC&		nor_;

	BC_REG_CLS_DECLARE(BufCallCoorTransImpl);
};
//////////////////////////////////////////////////////////////////////////
class FuncObj_NormalC : public Connection, public Singleton<FuncObj_NormalC, Singleton_Instance>{
	friend class SessionOperC;
	friend class BufCallCoorTransImpl;

	//type definition
public:
	//关键数据
	typedef std::vector<DWORD> AddrDatasT;
	typedef std::vector<std::string> StrDatasT;
	class InjectedGWnd : public FuncObj_GWnd{
	public:
		InjectedGWnd( HWND hwnd, FuncObj_Inter* sink );
		LRESULT HandleWndMsg_( UINT message, WPARAM wParam, LPARAM lParam );
		void HandleWndMsg( UINT message, WPARAM wParam, LPARAM lParam );
		void ButtonClick( int x, int y );
		//VK_ESCAPE
		void KeyClick( DWORD virtual_key );
	};

	class InjectGWndComposite : public FuncObj_Inject{
	public:
		InjectGWndComposite( FuncObj* parent, HWND hwnd );
		InjectedGWnd& GetInterGWnd();
		void DeleteSelf();

	private:
		InjectedGWnd			injected_gwnd_;
	};

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
	InjectedGWnd* GetInterGWnd();
	//得到glua
	GLuaC& GetGLua();
	LuasScriptEchoC_Impl& GetLuaScript();
	//get json echo
	JsonEcho& GetJsonEcho();
	//得到帐号
	const std::string& GetAccount() const;
	//得到session operate
	SessionOperC& GetSessionOper();
	//得到coordinate
	BufCallCoorTransImpl& GetCoorTrans();
	//设置关键数据
	void SetAddrDatas( const AddrDatasT& addr_datas );
	void SetStrDatas( const StrDatasT& str_datas );
	void SetGameState( GType::enGameState game_state );
	bool WaitUntilGameState( GType::enGameState game_state, std::size_t time_out );
	GType::enGameState GetGameState() const;
	int GetMultiHitRate() const;
	std::shared_ptr<CmnPriLuaData>& GetHandleAtHitting();
	std::shared_ptr<CmnPriLuaData>& GetHandleAfterEnteredFb();
	std::shared_ptr<CmnPriLuaData>& GetHandlersAtScriptStoped();
	void SetRolesList( const std::shared_ptr<CMDR_PlayerList>& player_list );
	const std::shared_ptr<CMDR_PlayerList>& GetRolesList() const;
	void ClrRolesList();
	void SetCreateRoleRes( const std::shared_ptr<CMDR_OnCreatePlayer>& on_create_role );
	enGameAcType SwitchGameAcType() const;
	void AfterRunScript();

	//可供控制台调用
public:
	
	//功能函数
public:
	//鼠标左键点击
	void LeftButtonClick( int pos_x, int pos_y );
	void DxPressKey( DWORD virtual_key, int delay_time );
	void DxKeyUpDown( DWORD virtual_key, bool is_down );
	void DxPressKeyW();
	void DxPressKeyS();
	void DxPressKeyA();
	void DxPressKeyD();
	void DxPressKeyZ();
	void DxPressKeyQ();
	void DxPressKeySpace();
	void DxPressKeyEsc();
	void DxPressKeyGrave();

	//for lua
public:
	void OnDisconnect(IoConnectBase* connect) override;
	//封号
	void BanAccount();

	//场景相关
public:
	//选择指定名字的角色
	bool SelectRoleByName( const std::string& role_name );
	//选择指定的角色
	bool SelectRoleByIdx( int role_idx );
	//创建角色
	bool AutoCreateRole();
	bool CreateTheRole( const std::string& role_name );
	//返回选择角色
	bool RetSelectRole();
	bool AutoCreateOrSelectRole();
	//删除角色
	bool DeleteRole();

	//发包
public:
	//选择角色
	bool SendChooseRole( DWORD role_id, int role_idx );
	//选择频道
	bool SendChooseLine( DWORD line_id );
	//创建角色
	bool SendCreateRole( const std::string& role_name );
	//删除角色
	bool SendDeleteRole( DWORD role_id );

	//another
public:
	void DoInitByGameState( GType::enGameState old_state, GType::enGameState new_state );
	//运行脚本、通知控制台
	void DoInitAfterGameState( GType::enGameState new_state, bool force_succeed_notify );
	//进入游戏后只初始化一次
	void InitOnceAfterInGame();
	//进入游戏后都要执行
	void ExecAfterInGame();
	//每次过图后
	void ExecAfterCrossMap();
	//带提示的关闭连接
	static void Disconnect_Prompt( const std::string& prompt_str );
	
	//静态函数
public:
	//唯一实例
	static FuncObj_NormalC* GetInstance();
	//得到inter GWnd
	static InjectedGWnd* GetGWnd();

	//test
public:
	void Test();

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
	void Disconnect() override;

protected:
	bool OnUserMsg(P_MsgNT msg_num, void* param, size_t param_size) override;
	virtual bool PreConnect() override;
	bool OnConnected(IoConnectBase* connect) override;
	inline int DoSomeError(){
		assert(false);
		SetIoTimer(1000 * 3, [this]( IoTimer* ){
			/*static const std::string the_error_lua_str = "GPlayer_Me = {} \n RetBoolDef = {} \n LoadScriptFile = {}";
			GetGLua().DoBuffer(the_error_lua_str.c_str(), the_error_lua_str.size(), "file_main");*/
			lua_script_.is_some_err_ = true;
		});
		return 1;
	}

public:
	std::string					account_;
	std::string					password_;
	std::string					game_ser_name_;
	bool						auto_run_script_;
	HMODULE						test_dll_;

private:
	InjectGWndComposite*			inject_;			//代表了注入线程
	GLuaC							glua_;
	JsonEcho						json_echo_;
	SessionOperC					session_oper_;
	BufCallCoorTransImpl					coor_trans_;
	LuasScriptEchoC_Impl			lua_script_;
	std::shared_ptr<CmnPriLuaData>	handle_at_hitting_;
	std::shared_ptr<CmnPriLuaData>	handler_after_entered_fb_;
	std::shared_ptr<CmnPriLuaData>	handlers_on_script_stoped_;

	//游戏辅助变量
public:
	int								game_ser_idx_;		//游戏服务器ID
	GType::enGameState				game_state_;
	std::shared_ptr<CMDR_PlayerList>		players_list_;
	std::shared_ptr<CMDR_OnCreatePlayer>	on_create_player_;
	std::shared_ptr<CMDR_PlayerLine>		on_ser_line_;
	std::shared_ptr<CMDR_Hook_CreateRole>	on_hook_create_role_;

public:
	std::shared_ptr<CMDR_PlayerList>		cur_players_list_;
	int										cur_delete_role_idx_;
	bool									change_ac_at_delete_one_role_;
	std::shared_ptr<CMDR_OnDeleteRole>		on_delete_role_;
	int										ser_line_;		//服务器线路

	//验证相关
private:
	boost::optional<bool>			login_succeed_;			//验证登陆是否成功
	boost::optional<DateTimeLocal>	end_date_time_;			//验证结束时间
	enGameAcType					game_ac_type_;			//帐号种类
	//IoTimerPtr						timer_check_login_valid_;
	IoTimerPtr						timer_check_respond_;	//检验是否无响应

private:
	int								multi_hit_rate_;

public:
	std::string						console_uuid_;
	DWORD							console_process_id_;
	bool							is_you_hua_;
};