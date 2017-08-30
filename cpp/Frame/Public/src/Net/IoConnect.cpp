#include "stdafx.h"
#include <Net/IoConnect.h>
#include <cassert>
#include <Common/CmnException.h>
#include <Common/UsefulFunc.h>

IoConnectSink::~IoConnectSink()
{

}

bool IoConnectBase::ConnectDone()
{
	if (connect_sinks_.empty())
		return true;
	auto sinks_tmp = connect_sinks_;
	for ( auto sink : sinks_tmp )
	{
		assert(sink);
		if ( !ExistSink(sink) )
			continue;
		if ( !sink->OnConnected(this) )
			return false;
	}
	return true;
}

void IoConnectBase::Disconnect()
{
	if (dis_connected_)
		return;
	if (connect_sinks_.empty())
	{
		assert(false);
		return;
	}
	dis_connected_ = true;
	for ( auto sink : connect_sinks_ )
	{
		assert(ExistSink(sink));
		sink->OnDisconnect(this);
	}
	connect_sinks_.clear();
}

void IoConnectBase::AddSink( IoConnectSink* sink )
{
	if ( !sink )
	{
		assert(false);
		return;
	}
	if (ExistSink(sink))
		return;
	connect_sinks_.push_back(sink);
}

void IoConnectBase::RemoveSink( IoConnectSink* sink )
{
	if ( !sink )
	{
		assert(false);
		return;
	}
	auto it = std::find(connect_sinks_.begin(), connect_sinks_.end(), sink);
	if ( it == connect_sinks_.end() )
	{
		assert(false);
		return;
	}
	connect_sinks_.erase(it);
}

bool IoConnectBase::ExistSink( IoConnectSink* sink ) const
{
	if ( !sink )
	{
		assert(false);
		return false;
	}
	for ( auto v : connect_sinks_ )
	{
		if ( v == sink )
			return true;
	}
	return false;
}

IoConnectBase::~IoConnectBase()
{
	//assert(connect_sinks_.empty());
}

IoConnectBase::IoConnectBase()
{
	dis_connected_ = false;
}

//static bool g_connect_mgr_exec_once_ = false;
IoConnectsMgr::~IoConnectsMgr()
{
	for ( auto v : io_connects_ )
	{
		assert(v);
		v->RemoveSink(this);
	}
}

bool IoConnectsMgr::OnConnected( IoConnectBase* connect )
{
	if ( !connect )
	{
		assert(false);
		return false;
	}
	ConnectsLockGuard l(connects_mutex_);
	io_connects_.insert(connect);
	return true;
}

void IoConnectsMgr::OnDisconnect( IoConnectBase* connect )
{
	if ( !connect )
	{
		assert(false);
		return;
	}
	if (is_disconnect_all_)
		return;
	ConnectsLockGuard l(connects_mutex_);
	io_connects_.erase(connect);	
}

void IoConnectsMgr::DisconnectAll()
{
	if (io_connects_.empty())
		return;
	ConnectsLockGuard l(connects_mutex_);
	if (is_disconnect_all_)
	{
		assert(false);
		return;
	}
	is_disconnect_all_ = true;
	auto connects_tmp = io_connects_;
	for ( auto connect : io_connects_ )
	{
		assert(connect);
		connect->Disconnect();
	}
	io_connects_.clear();
	is_disconnect_all_ = false;
}

IoConnectsMgr::IoConnectsMgr()
{
	is_disconnect_all_ = false;
	//exit_io_thread_ = true;
}

IoConnectsMgr::IoConnectIterPtr IoConnectsMgr::CreateIoConnectIterPtr()
{
	return MakeIterator<IoConnectTraits>(io_connects_);
}

IoConnectBase* IoConnectsMgr::FindConnectBy(const std::function<bool(IoConnectBase*)>& fn) const
{
	ConnectsLockGuard l(connects_mutex_);
	for (auto v : io_connects_)
	{
		if (fn(v))
			return v;
	}
	return nullptr;
}

IoConnectsMgrSingle* IoConnectsMgrSingle::single_this_ = nullptr;

IoConnectsMgrSingle::IoConnectsMgrSingle()
{
	assert(!single_this_);
	single_this_ = this;
	//exit_io_thread_ = false;
}

IoConnectsMgrSingle::~IoConnectsMgrSingle()
{
	assert(single_this_);
	single_this_ = nullptr;
}

IoConnectsMgrSingle* IoConnectsMgrSingle::GetInstance()
{
	return single_this_;
}

bool IoConnectSelfHolder::OnConnected( IoConnectBase* connect )
{
	return true;
}

void IoConnectSelfHolder::OnDisconnect( IoConnectBase* connect )
{
	assert(connect);
}
