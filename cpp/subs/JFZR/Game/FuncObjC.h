#pragma once
#include "stdafx.h"
#include <socket/client.h>
#include <Protocol/P_NormalMsg.h>
#include "Socket/IoTimer.h"
#include "Facility/GTypes.h"
#include "FuncObjs/FuncObjInter.h"
#include "GLua/GLuaC.h"
#include "Script/JsonCall.h"
#include <dinput.h>
#include "Facility/GClass.h"
#include <Poco/DateTime.h>
#include <Communicate/CG_Defines.h>
#include <Socket/JsonEchoCoorTrans.h>

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
class FuncObj_NormalC;
class SessionOperC : public JsonEchoBase{
public:
	SessionOperC( FuncObj_NormalC& func_obj_normal );

	//用于验证
public:
	//设置是否登陆成功
	void SetLoginSucceed( bool succeed );
	//设置用户结束时间
	void SetEndDateTime( Poco::UInt32 timestamp_lower, Poco::UInt32 timestamp_upper );
	//设置关键数据
	void SetAddrDatas( const Json::Value& addr_datas );

	//协调相关
public:

	//获取数据
public:
	//获得技能信息
	Json::Value GetSkillsInfo();
	//获得游戏对象信息
	Json::Value GetGameObjsInfo();
	//获得物品信息
	Json::Value GetAllItems();
	//获得房间信息
	Json::Value GetRoomInfo();
	//得到我的出售列表
	void GetMyOfferList();
	//得到其他号的出售项
	void GetOtherOfferItems();

	//游戏逻辑
public:
	bool CreateRandNameRole();
	bool SelectRole( const std::string& role_name );

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
	//让窗口显示在最前
	void ForegroundTheWindow();

	//测试用
public:

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	FuncObj_NormalC&				func_obj_normal_;
};
//////////////////////////////////////////////////////////////////////////
bool MakeTransRoute( P_CoorPacketHeadT& out_route, enGameAcType game_ac_type );
P_CoorPacketHeadT MakeTransRoute( enGameAcType game_ac_type );
class JECoorTransImpl : public JECoorTrans{
public:
	JECoorTransImpl( FuncObj_NormalC& func_obj_normal );

	//响应广播消息
public:
	bool GainPriProductsInfo();
	bool GainProductsInfo( int ac_type );

	//响应单独消息
public:
	void OnProductsInfo( const Json::Value& json_uuid, const Json::Value& json_v );
	//得到所有球员数据
	void GetPlayersInfo();
	//得到所有物品数据
	void GetItemsInfo();
	//得到我的出售列表
	void GetMyOfferList();
	//得到其他号的出售项
	void GetOtherOfferItems();

	//another
public:

	//test
public:

protected:
	bool OnThisMemCall( const stThisCallHelper& helper );

private:
	FuncObj_NormalC&		nor_;
};
//////////////////////////////////////////////////////////////////////////
class FuncObj_NormalC : public Connection_Aes, public Singleton<FuncObj_NormalC, Singleton_Instance>{
	friend class SessionOperC;
	friend class GCoordinateC;

	//type definition
public:
	//关键数据
	typedef std::vector<DWORD> AddrDatasT;
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
	//检查登陆失败窗口
	void BeginCheckLoginFailed();
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
	//get dx input press key queue
	DI_PressKeyPtr GetDI_PressKeyPtr();
	//得到session operate
	SessionOperC& GetSessionOper();
	//得到coordinate
	JECoorTransImpl& GetCoorTrans();
	//设置关键数据
	void SetAddrDatas( const AddrDatasT& addr_datas );
	void SetGameState( GType::enGameState game_state );
	bool WaitUntilGameState( GType::enGameState game_state, std::size_t time_out );
	GType::enGameState GetGameState() const;
	
	//可供控制台调用
public:
	

	//功能函数
public:
	//关闭所有打开的UI
	void CloseAllUi();
	//点击dx方面的按键
	void PressDxinputKey( BYTE di_key );
	void OnCreatePlayerResult( CMDR_OnCreatePlayer::enResult result );

	//for lua
public:
	void OnDisconnect( IoConnectBase* connect );
	//封号
	void BanAccount();

	//场景相关
public:
	//选择频道
	bool ChooseChannel();
	void SetChannelId( const std::wstring& channel_id );
	//创建角色
	boost::optional<bool> CreateRole( const std::wstring& role_name, GType::enJobCategory job );
	bool CreateRandNameRole( GType::enJobCategory job );
	//选择角色
	bool SelectRole( const std::wstring& role_name );
	bool SelectRoleA( const std::string& role_name );
	//是否选择过角色了
	bool IsSelectedRole() const;
	void SetSelectedRole( bool selected_role );
	//自动创建或选择角色
	bool AutoCreateOrSelectRole();
	//返回到角色选择界面
	bool RetChooseRoleScene();
	//按ESP直到弹出游戏菜单
	bool UntilShowGameMenuByPressEsp();
	//button click
	bool ButtonClick( int x, int y );
	bool KeyClick( DWORD virtual_key );

	//another
public:
	
	//修改发包相关,由游戏线程调用
public:

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

protected:
	virtual bool OnModify( P_MsgNT msg_num, void* param, size_t param_size );
	virtual bool OnExec( P_MsgNT msg_num, void* param, size_t param_size );
	virtual bool PreConnect();
	bool OnConnected( IoConnectBase* connect );

private:
	std::string					account_;
	std::string					password_;
	bool						auto_run_script_;
	HMODULE						test_dll_;

private:
	InjectGWndComposite*			inject_;			//代表了注入线程
	GLuaC							glua_;
	JsonEcho						json_echo_;
	DI_PressKeyPtr					di_press_key_ptr_;
	SessionOperC					session_oper_;
	JECoorTransImpl					coor_trans_;
	LuasScriptEchoC_Impl			lua_script_;

	//游戏辅助变量
private:
	int								game_ser_idx_;		//游戏服务器ID
	GType::enGameState				game_state_;
	std::wstring					channel_id_;
	boost::optional<CMDR_OnCreatePlayer::enResult>		on_create_player_result_;
	bool							selected_role_;		//是否选择过角色了

	//验证相关
private:
	CryptoStrCompose::SubCryptoT	public_key_;			//验证公钥
	CryptoStrCompose::SubCryptoT	private_key_;
	boost::optional<bool>			login_succeed_;			//验证登陆是否成功
	boost::optional<Poco::DateTime>	end_date_time_;			//验证结束时间
	enGameAcType					game_ac_type_;			//帐号种类
	Poco::DateTime					time_normal_trade_;		//普通号交易流程的计时
	IoTimerPtr						timer_check_login_valid_;
	IoTimerPtr						timer_check_respond_;	//检验是否无响应
};