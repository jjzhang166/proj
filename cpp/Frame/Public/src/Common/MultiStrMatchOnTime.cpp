#include "stdafx.h"
#include <Common/MultiStrMatchOnTime.h>
#include <cassert>
#include <Common/LogOut.h>
#include "Net/IoTimer.h"
#include <string>
#include <vector>
#include <cassert>
#include "Script/lua_base.h"

void TimeoutImple( MultiStrMatchOnTime* multi_match, size_t times, int template_id ){
	multi_match->Timeout(template_id);
}

class MultiStrMatchOnTimeImpl{
public:
	MultiStrMatchOnTimeImpl( MultiStrMatchOnTime* multi_match, size_t template_lifecycle, int template_id ){
		lifecycle_ = template_lifecycle;
		multi_str_match_ = multi_match;
		assert(multi_str_match_);
		if ( MultiStrMatchOnTime::kMultiStrLifecycle_Forever != template_lifecycle )
		{
			SetIoTimer(template_lifecycle, 1, [=]( size_t times ){
				TimeoutImple(multi_match, times, template_id);
			}); 
		}
	}
	virtual ~MultiStrMatchOnTimeImpl(){}

	virtual bool AddMatchedStr( const char* matched_str ){
		matched_strings_.push_back(MatchHelper(matched_str));
		return true;
	}
	virtual bool AllMatchSpecifyStr( const char* specify_str ) = 0;
	//重置匹配
	void ResetMatch(){
		for ( auto& match_helper : matched_strings_ )
		{
			match_helper.is_matched_ = false;
		}
	}
	//得到生命期
	size_t GetLifecycle() const{
		return lifecycle_;
	}

protected:
	struct MatchHelper{
		MatchHelper(){
			is_matched_ = false;
		}
		MatchHelper( const char* matched_str ) : matched_str_(matched_str){
			is_matched_ = false;
		}
		bool			is_matched_;
		std::string		matched_str_;
	};

protected:
	MatchHelper* GetFirstUnmatched(){
		for ( auto& order_helper : matched_strings_ )
		{
			if ( !order_helper.is_matched_ )
			{
				return &order_helper;
			}
		}
		return nullptr;
	}

protected:
	bool								is_matched_then_invalid_;
	std::vector<MatchHelper>			matched_strings_;
	size_t								lifecycle_;
	MultiStrMatchOnTime*				multi_str_match_;
};

class MultiStrMatchOnTimeImpl_Order : public MultiStrMatchOnTimeImpl{
public:
	MultiStrMatchOnTimeImpl_Order( MultiStrMatchOnTime* multi_match, size_t template_lifecycle, int template_id ) :
		MultiStrMatchOnTimeImpl(multi_match, template_lifecycle, template_id){}
	virtual bool AllMatchSpecifyStr( const char* specify_str ){
		MatchHelper* unmatched = GetFirstUnmatched();
		if ( !unmatched )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "已全都匹配成功，再次匹配已没什么意义了";
			return false;
		}

		unmatched->is_matched_  = multi_str_match_->IsMatched(unmatched->matched_str_.c_str(), specify_str);
		if ( !unmatched->is_matched_ )
			return false;
		return !GetFirstUnmatched();
	}
};

class MultiStrMatchOnTimeImpl_Unorder : MultiStrMatchOnTimeImpl{
public:
	MultiStrMatchOnTimeImpl_Unorder( MultiStrMatchOnTime* multi_match, size_t template_lifecycle, int template_id ) :
		MultiStrMatchOnTimeImpl(multi_match, template_lifecycle, template_id){}

	virtual bool AllMatchSpecifyStr( const char* specify_str ){
		size_t unmatched_cnt = 0;
		for ( auto& unmatched : matched_strings_ )
		{
			if ( !unmatched.is_matched_ )
			{
				unmatched.is_matched_ = multi_str_match_->IsMatched(unmatched.matched_str_.c_str(), specify_str);
				unmatched_cnt += !unmatched.is_matched_;
				if (unmatched.is_matched_)
					break;
			}
		}
		return unmatched_cnt ? false : !GetFirstUnmatched();
	}
};
#define MULTI_MATCH_CAST(p) ((MultiStrMatchOnTimeImpl*)(p))
#define THREAD_MATCH_	(void*)0
/**/

MultiStrMatchOnTime& MultiStrMatchOnTime::GetMe()
{
	static MultiStrMatchOnTime multi_match;
	//千万不能忘了在注入线程里设置下thread_id
	return multi_match;
}

MultiStrMatchOnTime::MultiStrMatchOnTime()
{
}

bool MultiStrMatchOnTime::InitTemplateIdObj( int template_id, size_t template_lifecycle, bool is_order_matched )
{
	THREAD_MATCH_;
	auto it = map_impl_.find(template_id);
	if ( it != map_impl_.end() )
		return false;
	if (is_order_matched)
		map_impl_[template_id] = new MultiStrMatchOnTimeImpl_Order(this, template_lifecycle, template_id);
	else
		map_impl_[template_id] = new MultiStrMatchOnTimeImpl_Unorder(this, template_lifecycle, template_id);
	return true;
}

bool MultiStrMatchOnTime::AddMatchedStr( int template_id, const char* matched_str )
{
	THREAD_MATCH_;
	if ( !matched_str )
		return false;
	auto it = map_impl_.find(template_id);
	if ( it != map_impl_.end() )
		return MULTI_MATCH_CAST(it->second)->AddMatchedStr(matched_str);
	return false;
}

bool MultiStrMatchOnTime::AllMatchSpecifyStr( int template_id, const char* specify_str )
{
	THREAD_MATCH_;
	if ( !specify_str )
		return false;
	auto it = map_impl_.find(template_id);
	if ( it != map_impl_.end() )
	{
		if (MULTI_MATCH_CAST(it->second)->AllMatchSpecifyStr(specify_str))
		{
			if ( kMultiStrLifecycle_Forever != MULTI_MATCH_CAST(it->second)->GetLifecycle() )
			{
				delete MULTI_MATCH_CAST(it->second);
				it->second = nullptr;
				LOG_ENTER_FUNC;
				LOG_O(Log_trace) << "template_id:" << template_id << "因为匹配成功而被删除";
			}
			return true;
		}
	}
	return false;
}

void MultiStrMatchOnTime::Timeout( int template_id )
{
	THREAD_MATCH_;
	auto it = map_impl_.find(template_id);
	if ( it != map_impl_.end() )
	{
		if (it->second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "template_id:" << template_id << "因为超时被删除";
		}
		delete MULTI_MATCH_CAST(it->second);
		map_impl_.erase(it);
	}
}

MultiStrMatchOnTime::~MultiStrMatchOnTime()
{
	for ( auto v : map_impl_ )
	{
		delete MULTI_MATCH_CAST(v.second);
	}
}

size_t MultiStrMatchOnTime::GetLifecycleForever() const
{
	return kMultiStrLifecycle_Forever;
}

void MultiStrMatchOnTime::ResetMatch( int template_id )
{
	THREAD_MATCH_;
	auto it = map_impl_.find(template_id);
	if ( it != map_impl_.end() )
	{
		if (it->second)
		{
			MULTI_MATCH_CAST(it->second)->ResetMatch();
		}
	}
}

bool MultiStrMatchOnTime::IsMatched( const char* str_src, const char* specify_str )
{
	return 0 == strcmp(str_src, specify_str);
}

#undef MULTI_MATCH_CAST
#undef THREAD_MATCH_

void MultiStrMatchOnTimeForLua::RegForLua( LuaVM* lua_vm )
{
	using namespace luabind;
	assert(lua_vm);
	module(lua_vm->lstate_)
	[
		class_<MultiStrMatchOnTimeForLua>("MultiStrMatchOnTimeForLua")
		.def("InitTemplateIdObj", &MultiStrMatchOnTimeForLua::InitTemplateIdObj)
		.def("AddMatchedStr", &MultiStrMatchOnTimeForLua::AddMatchedStr)
		.def("AllMatchSpecifyStr", &MultiStrMatchOnTimeForLua::AllMatchSpecifyStr)
		.def("GetLifecycleForever", &MultiStrMatchOnTimeForLua::GetLifecycleForever)
		.def("ResetMatch", &MultiStrMatchOnTimeForLua::ResetMatch),
		def("MultiStrMatchOnTime_Me", &MultiStrMatchOnTimeForLua::GetMe)
	];

	lua_vm->DoString("	\
		function __MultiStrMatched_Helper__( str_src, specify_str )	\
			return string.find(str_src, specify_str) ~= nil;	\
		end	\
		");
	GetMe().lua_vm_ = lua_vm;
}

MultiStrMatchOnTimeForLua& MultiStrMatchOnTimeForLua::GetMe()
{
	static MultiStrMatchOnTimeForLua multi_str;
	return multi_str;
}

MultiStrMatchOnTimeForLua::MultiStrMatchOnTimeForLua()
{
	lua_vm_ = nullptr;
}

bool MultiStrMatchOnTimeForLua::IsMatched( const char* str_src, const char* specify_str )
{
	assert(lua_vm_);
	return lua_vm_->LuaObjCallByDlt("__MultiStrMatched_Helper__", false, str_src, specify_str);
}
