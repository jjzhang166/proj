#pragma once
/*
@author zhp
@date 2016/12/20 10:16
@purpose ui operator
*/
#include <string>
#include <Net/server.h>
#include <Net/IoTimer.h>
#include <deque>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>


class CmnConsoleSession;
class ConsoleAppUiOperIo;
class Cmn_ThrdIo_Console;
class WxLuaApp_Console;

class ConsoleLoginMgr{
	ConsoleLoginMgr(const ConsoleLoginMgr&) = delete;
	ConsoleLoginMgr& operator = (const ConsoleLoginMgr&) = delete;
	friend class ConsoleAppUiOperIo;

public:
	typedef boost::lock_guard<boost::recursive_mutex> LockGuard;
	struct ItemInfo{
		enum enState{
			kNil,
			kWaitAttachPid,
			kLogining,
		};
		std::string					item_key_;
		boost::weak_ptr<Session>	session_login_;
		IoTimerPtr					timer_login_;
		long						target_process_id_;		//登陆器进程id
		std::string					target_process_info_;
		enState						state_;

		explicit ItemInfo(const std::string& item_key);
		void FreeLoginTimer();
	};
	typedef std::map<std::string, stAcSerInfo> AcInfoContT;

public:
	ConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper);
	virtual ~ConsoleLoginMgr();
	void AddAcItemInfo(const std::string& item_key, const stAcSerInfo& ac_info);
	const stAcSerInfo* FindAcItemInfo(const std::string& item_key) const;
	bool AddLoginItem(const std::string& item_key, bool add_to_first = false);
	bool CancelLogin(const std::string& item_key);
	int GetLoginingCnt() const;
	int GetWaitAttachCnt() const;
	bool HasFreeItem() const;
	void RecycleFreeItemKey(const std::string& item_key);
	bool HasTheProcessInfo(const std::string& process_info) const;
	bool AttachProcessInfo(const std::string& process_info, long pid, const std::string& item_key);
	bool AttachSessionLogin(long pid, const std::string& item_key, CmnConsoleSession& login_session);
	bool AttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& game_session);

protected:
	virtual bool HandleAttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& session);

private:
	void Timeout(const std::string& item_key);
	int GetTheStateCnt(ItemInfo::enState state) const;
	std::string GetFreeItemKey();	//会更改状态

private:
	std::deque<ItemInfo>			login_infos_;
	mutable boost::recursive_mutex	login_mutex_;
	ConsoleAppUiOperIo&				ui_oper_;
	AcInfoContT						ac_info_;
};

class ConsoleAppUiOper{
public:
	virtual ~ConsoleAppUiOper(){}
	virtual bool Login(const std::string& key, const std::string& game_path, bool is_add_to_first) = 0;
	virtual bool CancelLogin(const std::string& key) = 0;
	virtual bool Run(const std::string& key, const std::string& script_name, const std::string& lua_serialize) = 0;
	virtual bool Stop(const std::string& key) = 0;
	virtual bool CloseGame(const std::string& key) = 0;
	virtual bool ShowGameWnd(const std::string& key) = 0;
};

class ConsoleAppUiOperMain : public ConsoleAppUiOper{
	friend class ConsoleAppUiOperIo;
public:
	ConsoleAppUiOperMain(WxLuaApp_Console& app);
	bool Login(const std::string& key, const std::string& game_path, bool is_add_to_first) override;
	bool CancelLogin(const std::string& key) override;
	bool Run(const std::string& key, const std::string& script_name, const std::string& lua_serialize) override;
	bool Stop(const std::string& key) override;
	bool CloseGame(const std::string& key) override;
	bool ShowGameWnd(const std::string& key) override;
	bool IsLogined(const std::string& key) const;
	bool CloseLoginSession(const std::string& key) const;
	Cmn_ThrdIo_Console* GetThrdIo();
	ConsoleAppUiOperIo* GetUiOperIo();

protected:
	virtual bool DoLoginOne(const std::string& key, const stAcSerInfo& ac_info);

protected:
	WxLuaApp_Console&		my_app_;
};

class ConsoleAppUiOperIo : public ConsoleAppUiOper{
	friend class ConsoleAppUiOperMain;
public:
	ConsoleAppUiOperIo(Cmn_ThrdIo_Console& thrd_io);
	~ConsoleAppUiOperIo();
	bool Login(const std::string& key, const std::string& game_path, bool is_add_to_first) override;
	bool CancelLogin(const std::string& key) override;
	bool Run(const std::string& key, const std::string& script_name, const std::string& lua_serialize) override;
	bool Stop(const std::string& key) override;
	bool CloseGame(const std::string& key) override;
	bool ShowGameWnd(const std::string& key) override;
	ConsoleLoginMgr& GetLoginMgr();
	ConsoleAppUiOperMain* GetAppOperMain();
	WxLuaApp_Console* GetApp();
	Cmn_ThrdIo_Console* GetIoThrd();

protected:
	virtual int GetMaxLoginingCnt() const;
	virtual bool DoLoginOne(const std::string& key);
	virtual bool OnLoginedOne(const std::string& process_info, long pid, const std::string& item_key);

protected:
	ConsoleLoginMgr*		login_mgr_;
	Cmn_ThrdIo_Console&		thrd_io_;
};
//////////////////////////////////////////////////////////////////////////
class ConsoleAcCfgMgr{
public:
	struct stAcItemInfo{
		std::string		ac_;
		std::string		pwd_;
		std::string		area_;
		std::string		ser_;

	public:
		friend class ConsoleAcCfgMgr;
		stAcItemInfo();
		stAcItemInfo(const std::string& ac, const std::string& pwd, const std::string& area, const std::string& ser);
		virtual ~stAcItemInfo();
		template<typename T>
		T& CastAs(){
			return static_cast<T&>(*this);
		}
		template<typename T>
		const T& CastAs() const{
			return static_cast<T&>(*this);
		}

	protected:
		typedef std::vector<std::string> StringItemT;
		virtual bool ParseTails(const StringItemT& tails);
		virtual bool WriteTails(std::ostream& ostm) const;

	private:
		bool ReadLine(const std::string& str_line);
		bool WriteLine(std::ostream& ostm) const;
	};
	typedef boost::shared_ptr<stAcItemInfo> AcItemInfoPtrT;
	typedef std::vector<AcItemInfoPtrT> AcItemInfosT;
	typedef IteratorTraits<AcItemInfoPtrT, IterContTag_ByRef> AcItemInfoTraits;
	typedef AcItemInfoTraits::iterator AcItemInfoIterator;
	typedef AcItemInfoTraits::iterator_pointer AcItemInfoIteratorPtr;

public:
	AcItemInfoIteratorPtr Iterator();
	const AcItemInfosT& GetAcItems() const;
	bool Load(const std::string& full_file_name);
	bool Save(const std::string& full_file_name) const;
	void Add(const AcItemInfoPtrT& ac_item);

protected:
	AcItemInfosT		ac_items_;
};
//////////////////////////////////////////////////////////////////////////