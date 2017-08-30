#pragma once
/*
@author zhp
@date 2014/9/16
@purpose for manage io connections
*/
#include <set>
#include <list>
#include <Common/GameObjBase.h>
#include "../DependSelfLib.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/lock_guard.hpp>

class IoConnectBase;
class IoConnectSink{
public:
	virtual ~IoConnectSink();
	virtual bool OnConnected( IoConnectBase* connect ) = 0;
	virtual void OnDisconnect( IoConnectBase* connect ) = 0;
};

class IoConnectBase{
public:
	typedef std::list<IoConnectSink*> IoConnectSinksT;

public:
	IoConnectBase();
	virtual ~IoConnectBase();
	virtual bool ConnectDone();
	virtual void Disconnect();
	void AddSink( IoConnectSink* sink );
	void RemoveSink( IoConnectSink* sink );
	bool ExistSink( IoConnectSink* sink ) const;

protected:
	IoConnectSinksT			connect_sinks_;

private:
	bool					dis_connected_;
};

class IoConnectSelfHolder : public IoConnectSink{
public:
	bool OnConnected( IoConnectBase* connect );
	void OnDisconnect( IoConnectBase* connect );
};

class IoConnectsMgr : public IoConnectSink{
public:
	typedef std::set<IoConnectBase*> IoConnectsT;
	typedef IteratorTraits<IoConnectBase*, IterContTag_ByRef> IoConnectTraits;
	typedef IoConnectTraits::iterator_pointer IoConnectIterPtr;
	typedef boost::lock_guard<boost::recursive_mutex> ConnectsLockGuard;

public:
	IoConnectsMgr();
	~IoConnectsMgr();
	bool OnConnected( IoConnectBase* connect );
	void OnDisconnect( IoConnectBase* connect );
	void DisconnectAll();
	IoConnectBase* FindConnectBy(const std::function<bool(IoConnectBase*)>& fn) const;
	IoConnectIterPtr CreateIoConnectIterPtr();

protected:
	IoConnectsT		io_connects_;
	bool			is_disconnect_all_;
	//bool			exit_io_thread_;
	mutable boost::recursive_mutex	connects_mutex_;
};

class IoConnectsMgrSingle : public IoConnectsMgr{
public:
	IoConnectsMgrSingle();
	~IoConnectsMgrSingle();
	static IoConnectsMgrSingle* GetInstance();


private:
	static IoConnectsMgrSingle*		single_this_;
};