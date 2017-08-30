#pragma once
/*
@author zhp
@date 2016/11/15 0:27
@purpose application component
*/
#include <AppBase/CmnWxThrd.h>
#include <AppBase/WxLuaApp.h>
#include <Common/CmnPriLuaData.h>
#include <Net/CmnLoginInfo.h>
#include <Net/server.h>
#include <Net/Bufcall_Socket.h>
#include <Net/CmnLogin.h>
#include "ConsoleUiOper.h"
#include <CmnMix/Cmn_LuaFile.h>
#include <Script/LuaScript.h>
#include "ConsoleUiOper.h"
#include "AppExecEnv.h"

//////////////////////////////////////////////////////////////////////////
class WxLuaApp_Console;
class Cmn_ThrdIo_Console;
class ConsoleAppUiOperMain;
class ConsoleAppUiOperIo;
class CmnGameConnectionC;
class Cmn_GameDllApp;
class GameConnectionC;
class ConsoleLoginSessionS;
class LoginConnectionC;
class ConsoleSessionS;
class CmnConsoleSession;
class ConsoleLoginMgr;
class CmnConsoleLoginInfoMgrBase;
class LuaCfgParser;
class ConsoleServer;
class ConsoleLoginServer;
class CmnConsoleCS_Connection;
class ConsoleSessionOper;
class ConsoleLoginSessionOper;
class ConsoleAppFactory;
class Cmn_ThrdIo_Console;
class WxLuaApp_Console;
class CmnConsoleScriptEcho;
//////////////////////////////////////////////////////////////////////////
class AcOperSink{
public:
	virtual ~AcOperSink();
	virtual void OnAddAc(const stAcSerInfo& ac_info) = 0;
	virtual void OnDeleteAc(const std::string& item_key) = 0;
	virtual void OnAcConnected(ConsoleSessionS* session, const std::string& item_key) = 0;
	virtual void OnAcDisconnect(const std::string& item_key) = 0;
	virtual void OnUpdateAcItemData(int ac_item_idx, const std::string& item_key, const luabind::object& item_data) = 0;
};
//////////////////////////////////////////////////////////////////////////
//**************opers
class CmnConsoleSessionOper : public BufCall_SocketExcept{
public:
	CmnConsoleSessionOper(CmnConsoleSession& session);
	CmnConsoleSession& GetSession() const;

protected:
	virtual bool OnAttachAcInfo_(long pid, const std::string& ac_item_key);
	virtual bool DoExecConnectDone();
	virtual void OnAcPwdErr_();
	virtual void OnBacAc_();
	virtual void OnFreeze_();

private:
	void SetStateAtDisconnect_(const char* final_state);

protected:
	CmnConsoleSession&		session_;

	BC_REG_CLS_DECLARE(CmnConsoleSessionOper);
};

class ConsoleLoginSessionOper : public CmnConsoleSessionOper{
public:
	ConsoleLoginSessionOper(ConsoleLoginSessionS& session);

protected:
	bool OnAttachAcInfo_(long pid, const std::string& ac_item_key) override;
	virtual void OnS_AttachLoginSession(const std::string& item_key);

private:
	void OnAcPwdErr_() override;
	void OnBacAc_() override;
	void OnFreeze_() override;
	void OnRelogin_();

private:
	BC_REG_CLS_DECLARE(ConsoleLoginSessionOper);
};

class ConsoleSessionOper : public CmnConsoleSessionOper{
public:
	ConsoleSessionOper(ConsoleSessionS& session);
	ConsoleSessionS& GetSession() const;
	void SetLuaSerialize(const std::string& lua_str);

protected:
	bool OnAttachAcInfo_(long pid, const std::string& ac_item_key) override;
	bool DoExecConnectDone() override;
	void GameLoginSucceed();
	virtual void OnS_AttachGameSession(const std::string& item_key);
	void OnModifyConsoleCfg_(pt_csz key, pt_csz value);

private:
	int				connection_done_valid_cnt_;
	bool			is_done_connection_done_;
	bool			first_set_lua_serialize_;
	BC_REG_CLS_DECLARE(ConsoleSessionOper);
};
//////////////////////////////////////////////////////////////////////////
//***********sessions
class CmnConsoleSession : public Session{
	friend class CmnConsoleSessionOper;

public:
	CmnConsoleSession(Server& server);
	~CmnConsoleSession();
	CmnConsoleSessionOper* GetSessionOper() const;
	void SetLoginProcessInfo(const std::string& process_info);
	const stAsSerSessionInfo& GetAcInfo() const;
	static WxLuaApp_Console& GetApp();
	static Cmn_ThrdIo_Console& GetIoThrd();
	static ConsoleAppUiOperMain* GetAppUiOper();
	static ConsoleAppUiOperIo* GetIoUiOper();
	static ConsoleLoginMgr& GetConsoleLoginMgr();
	void SetStateAtDisconnect(const char* final_state);

protected:
	bool InitOnce() override;
	bool OnConnected(IoConnectBase* connect) override;
	virtual CmnConsoleSessionOper* CreateSessionOper() = 0;

private:
	bool ConnectDone() override final;
	bool DoConnectDone() override final;

protected:
	CmnConsoleSessionOper*		session_oper_;

protected:
	stAsSerSessionInfo			ac_info_;
	std::string					state_at_disconnect_;
};

class ConsoleLoginSessionS : public CmnConsoleSession{
public:
	ConsoleLoginSessionS(Server& server);

protected:
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;
	virtual void OnS_LoginDisconnect();
	virtual bool SendSomethingAtConnected();

private:
	ConsoleLoginSessionOper* CreateSessionOper() override final;
};

class ConsoleSessionS : public CmnConsoleSession{
	friend class ConsoleSessionOper;
public:
	struct stLuaCfgInfo{
		std::string			lua_cfg_serialize_;
		luabind::object		lua_cfg_;
	};

public:
	ConsoleSessionS(Server& server);
	~ConsoleSessionS();
	ConsoleSessionOper* GetSessionOper() const;
	CmnConsoleScriptEcho* GetScriptEcho() const;
	const stLuaCfgInfo& GetLuaCfgInfo() const;
	LuaCfgParser& GetLuaCfgParser();
	bool ForegroundTheWindow();
	static void ModifyConsoleCfgImpl(const std::string& item_key, pt_csz key, pt_csz value);

protected:
	bool InitOnce() override;
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;
	virtual void OnS_GameDisconnect();

private:
	ConsoleSessionOper* CreateSessionOper() override final;
	void OnModifyConsoleCfg(pt_csz key, pt_csz value);

protected:
	LuaCfgParser*			cfg_parser_;
	stLuaCfgInfo			lua_cfg_info_;
	CmnConsoleScriptEcho*	script_echo_;
	bool					is_reconnect_at_disconnect_;		//掉线重连
	bool					is_reconnect_to_first_;				//掉线重连，是否添加到开头

private:
	IoTimerPtr				timer_login_succeed_;
};
//////////////////////////////////////////////////////////////////////////
//***********server
class ConsoleLoginServer : public Server{
public:
	ConsoleLoginServer(pt_word host_port);

protected:
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;

private:
	ConsoleLoginSessionS* CreateSession() override final;
};

class ConsoleServer : public Server{
public:
	ConsoleServer(pt_word host_port);

protected:
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;

private:
	ConsoleSessionS* CreateSession() override final;
};
//////////////////////////////////////////////////////////////////////////
//CS->Check Server
class CmnConsoleCS_Connection : public CmnDoLogin{
public:
	explicit CmnConsoleCS_Connection(const std::string& check_server_ip);

protected:
	bool OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie,
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type) override;
};
//////////////////////////////////////////////////////////////////////////
class CmnConsoleScriptEcho : public LuasScriptEchoS{
public:
	CmnConsoleScriptEcho(ConsoleSessionS& parent);

protected:
	bool OutputScriptInfo(const std::string& script_info) override;
	bool OperResult(enOperResult result) override;

protected:
	ConsoleSessionS&		session_;

	BC_REG_CLS_DECLARE(CmnConsoleScriptEcho);
};
//////////////////////////////////////////////////////////////////////////
class Cmn_ThrdIo_Console : public Cmn_ThrdIo{
	typedef boost::shared_ptr<ConsoleAppUiOperIo> AppUiOperPtr;
	typedef boost::shared_ptr<ConsoleServer> ConsoleServerPtr;
	typedef boost::shared_ptr<ConsoleLoginServer> ConsoleLoginServerPtr;

public:
	Cmn_ThrdIo_Console();
	~Cmn_ThrdIo_Console();
	bool DoInit() override;
	bool DoRun() override;
	bool IsThrdAlive() const override;
	static WxLuaApp_Console& GetApp();
	ConsoleAppUiOperIo* GetAppUiOper() const;
	ConsoleServerPtr GetConsoleServer() const;
	ConsoleLoginServerPtr GetConsoleLoginServer() const;
	CmnConsoleLoginInfoMgrBase* GetLoginInfoMgr();

protected:
	void BindFrame(lua_State* l) override;
	bool OnInit_() override;
	int OnExit_() override;

private:
	LuaThrdData* CreateLuaVm() override final;

private:
	boost::shared_ptr<CmnPriLuaData>				login_pri_data_;
	boost::shared_ptr<CmnConsoleLoginInfoMgrBase>	login_info_mgr_;

protected:
	AppUiOperPtr							app_ui_oper_;
	ConsoleServerPtr						console_server_;
	ConsoleLoginServerPtr					console_login_server_;
	unsigned int							stack_size_;

private:
	bool									is_just_create_thrd_;
};
//////////////////////////////////////////////////////////////////////////
class ConsoleAppFactory : public WxLuaAppFactory{
public:
	Cmn_LuaCfgResolver* CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag) override;
	CmnConsoleCS_Connection* CreateCheckServerConnection(const std::string& svr_ip) override;
	virtual Cmn_ThrdIo_Console* CreateIoThrd();
	virtual LuaThrdData* CreateIoThrdLuaVm();
	virtual CmnConsoleLoginInfoMgrBase* CreateLoginInfoMgr();
	virtual ConsoleServer* CreateConsoleServer();
	virtual ConsoleLoginServer* CreateConsoleLoginServer();
	virtual ConsoleSessionS* CreateGameSession(ConsoleServer& server);
	virtual ConsoleLoginSessionS* CreateLoginSession(ConsoleLoginServer& server);
	virtual ConsoleSessionOper* CreateGameSessionOper(ConsoleSessionS& session);
	virtual ConsoleLoginSessionOper* CreateLoginSessionOper(ConsoleLoginSessionS& session);
	virtual CmnConsoleScriptEcho* CreateScriptEcho(ConsoleSessionS& session);
	virtual LuaCfgParser* CreateLuaCfgParser(ConsoleSessionS& session);
	virtual ConsoleAppUiOperMain* CreateAppUiOper(WxLuaApp_Console& app);
	virtual ConsoleAppUiOperIo* CreateIoUiOper(Cmn_ThrdIo_Console& io_thrd);
	virtual ConsoleAcCfgMgr::stAcItemInfo* CreateAcCfgItemInfo();
	virtual ConsoleLoginMgr* CreateConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper);
	virtual AppExecEnvBase* CreateExecEnv();

private:
	Cmn_LuaCfgResolver* CreateLuaCfgResolverImpl_(Cmn_LuaCfgResolver::enTag tag);
};

class WxLuaApp_Console : public WxLuaApp{
	friend class ConsoleLoginSessionS;
	friend class ConsoleAppUiOperMain;

protected:
	enum enFindMode{
		kFM_Find,
		kFM_FindChild,
		kFM_FindPosterity,
	};
	enum enD3d9Mode{
		kDM_Profile,
		kDM_Xml,
	};
	enum enFileDirLocation{
		kSFM_AtGamePathRoot,
		kSFM_AtGamePathStarter,
		kSFM_AtSystem,
		kSFM_AtSysAsset,
	};
	struct stPathFindingInfo{
		enFileDirLocation			file_dir_;
		boost::filesystem::path		path_;
		enFindMode					find_mode_;
	};
	struct stSer2IdInfo{
		std::string		area_name_;
		std::string		ser_name_;
		pt_dword		ser_id_;
	};
	typedef std::vector<stSer2IdInfo> Ser2IdContT;
	typedef std::vector<AcOperSink*> AcOperSinkCont;

private:
	struct stCrackCaptchaInfo{
		stCrackCaptchaInfo();
		void SetDmt(const std::string& ac, const std::string& pwd);
		void SetLocal(const std::string& ip, pt_word port);
		bool SameMode(enCrackCaptchaMode mode) const;
		const std::string& GetDmtAc() const;
		const std::string& GetDmtPwd() const;
		const std::string GetLocalIp() const;
		pt_word GetLocalPort() const;

	private:
		enCrackCaptchaMode		mode_;
		std::string				str_buffer_;
		std::string				str_buffer1_;
		pt_word					port_;
	};
	typedef std::vector<stPathFindingInfo> FilePathCont;
	typedef std::pair<stPathFindingInfo, std::string> RenamePair;
	typedef std::vector<RenamePair> FileRenameCont;
	typedef std::pair<stPathFindingInfo, stPathFindingInfo> OverwritePair;
	typedef std::vector<OverwritePair> FileOverwriteCont;
	struct stD3d9CfgInfo{
		enD3d9Mode				d3d9_mode_;
		stPathFindingInfo		game_file_path_;
		stPathFindingInfo		inject_file_path_;
	};
	typedef std::vector<stD3d9CfgInfo> D3d9CfgCont;

public:
	WxLuaApp_Console();
	~WxLuaApp_Console();
	ConsoleAppUiOperMain* GetAppUiOper() const;
	AS_NodePtr GetGameDll() const;
	std::string GenAcItemKey(const std::string& ac, const std::string& area, const std::string& ser);
	Cmn_ThrdIo_Console* GetIoThrd_();
	ConsoleAppFactory& GetAppFactory() const;
	AppExecEnvBase& GetExecEnv() const;
	const std::string& GetGameClientStarterPath() const;
	const std::string& GetStarterProcessCommand() const;
	void SetStarterProcessCommand(const std::string& command);
	virtual bool CanLoginGameAc() const;
	virtual void OnLoginFailed(const std::string& item_key, const stAsSerSessionInfo& ac_info);
	static bool RunScript(const std::string& item_key);
	static void LoginByItemKey(const std::string& item_key, bool add_to_first);
	AcOperSinkCont& GetAcOperSinks();
	void AddAcOperSink(AcOperSink* sink);
	void ModifyAcItemData(const std::string& item_key, const std::string& key, const std::string& value);
	//根据辅助的帐号类型决定每个连接的最大数量
	virtual pt_dword GetMaxConnectedCntByAcType(pt_dword ac_type) const;
	//重新登陆
	void ReloginAc(const std::string& item_key);
	static WxLuaApp_Console& GetApp();

	//服务器相关
public:
	void AddAreaSer2Id(const std::string& area_name, const std::string& ser_name, pt_dword ser_id);
	void ParseAreaSer2Id(const luabind::object& tbl);
	boost::optional<pt_dword> FindSerId(const std::string& area_name, const std::string& ser_name) const;

protected:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	const char* GetGameDllName() const;
	ConsoleAppFactory* CreateAppFactory() override;
	void OnFrameCreated();
	virtual void ParseMainCfg(const luabind::object& main_cfg);
	//每隔几分钟执行一次
	virtual void TimerDoMinutes();
	virtual void OnUpdateAcItem(int ac_item_idx, const std::string& item_key, const luabind::object& item_data);

	//游戏文件操作相关
protected:
	virtual void OnSetGamePath(const std::string& game_path, bool is_reset);
	virtual bool ModifySomeGameFiles();
	void AddGameFileDelete(const stPathFindingInfo& game_path);
	void AddGameFileRename(const stPathFindingInfo& game_path, const std::string& new_name);
	void AddGameFileRenameToDir(const stPathFindingInfo& game_path, const std::string& new_name);
	void AddGameFileOverwrite(const stPathFindingInfo& game_path, const stPathFindingInfo& src_file_path);
	void AddD3d9Cfg(enD3d9Mode d3d9_mode, const stPathFindingInfo& game_file_path, const stPathFindingInfo& inject_file_path);
	virtual bool PreLoginGame();
	bool CloseTheGameProcess(const boost::filesystem::path& file_path) const;
	virtual bool PreResolveGameFiles();
	virtual boost::filesystem::path GetStarterRalativePath() const;
	void OnLuaAddAc(const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser);
	void OnLuaDeleteAc(const std::string& item_key);
	//创建伪exe
	void CreateExeAsDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& exe_file_name);
	//删除文件或文件夹
	void DeleteFileOrDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& file_name);
	//重命名文件或文件夹
	void RenameFileOrDirectory(const boost::filesystem::path& path_dir, const boost::filesystem::path& file_name);

private:
	void SetAcItemInfo(const std::string& item_key, const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser);
	void SetGameSessionCfg(const std::string& item_key, const std::string& lua_serialize);
	int GetDirFileNames(const luabind::object& out_tbl, const std::string& dir_path, const std::string& file_extension, bool res_has_extension);

	//游戏文件操作相关
private:
	bool DiscoverGameClientStarterPath();
	void SetCrackCaptchaDmt(const std::string& ac, const std::string& pwd);
	void SetCrackCaptchaLocal(const std::string& ip, pt_word port);
	bool ResolveGameFiles(const boost::filesystem::path& game_dir_path);
	AS_NodePtr SmartFindTheFile(const stPathFindingInfo& path_info) const;
	AS_NodePtr GetTheFileDirByMode(enFileDirLocation mode) const;
	static AS_NodePtr FindTheFileByFindMode(const AS_NodePtr& file_dir, const boost::filesystem::path& file_path, enFindMode find_mode);

protected:
	ConsoleAppUiOperMain*		app_ui_oper_;
	bool						ac_item_key_use_uuid_;
	std::string					console_singleton_str_;
	AppExecEnvBase*				exec_env_;
	pt_csz						game_dll_name_;
	
public:
	std::string					game_path_;
	std::string					game_client_starter_path_;
	AS_NodePtr					game_dir_asnode_;
	AS_NodePtr					game_dir_starter_asnode_;
	pt_word						port_game_;
	pt_word						port_login_;
	std::string					version_game_dll_;
	std::string					version_login_dll_;
	std::string					end_date_time_;
	pt_dword					time_login2game_succeed_;		//游戏登陆器的计时(直到game连接成功)
	pt_dword					time_game_login_succeed_;		//游戏登陆成功的计时
	bool						glo_auto_run_script_;
	std::string					starter_process_command_;
	pt_dword					ac_type_;						//辅助帐号的类型

private:
	stCrackCaptchaInfo			crack_captcha_info_;
	FilePathCont				game_files_delete_;
	FileRenameCont				game_files_rename_;
	FileRenameCont				game_files_rename_to_dir_;
	FileOverwriteCont			game_files_overwrite_;
	D3d9CfgCont					d3d9_configs_;
	boost::filesystem::path		game_exe_name_;
	boost::filesystem::path		starter_relative_path_;

private:
	Ser2IdContT					ser2id_cont_;
	AcOperSinkCont				ac_oper_sinks_;
};
//////////////////////////////////////////////////////////////////////////
template<int MaxCnt, unsigned int OverflowCnt, int... ItemCntInfo>
struct ConsoleConnectCntInfo{
	enum{
		//最大连接数量
		kGameConnectionMaxCnt = MaxCnt,
		//不在kAcceptableGameConnectionCnt此列的，也就是超出的，全部设为此连接数
		kDefConnectCntAtOverflow = OverflowCnt,
	};
	typedef const int(type)[sizeof...(ItemCntInfo)];
	static type& ConnectItemCnt(){
		//每个帐号可接受的连接数量
		static const int kAcceptableGameConnectionCnt[] = { ItemCntInfo... };
		return kAcceptableGameConnectionCnt;
	}
};

class CmnConsoleLoginInfoMgrBase : public CmnLoginInfoMgrBase{
public:
	pt_dword GetMaxConnectedCntByAcType(pt_dword ac_type) const override final;

protected:
	void DoCloseTheSession(const BindAcInfoT& the_clr_ac) override;
};

template<int MaxCnt, unsigned int OverflowCnt, int... ItemCntInfo>
class CmnConsoleLoginInfoMgr : public CmnLoginInfoMgr<
	ConsoleConnectCntInfo<MaxCnt, OverflowCnt, ItemCntInfo...>
	, CmnConsoleLoginInfoMgrBase>{};
//////////////////////////////////////////////////////////////////////////