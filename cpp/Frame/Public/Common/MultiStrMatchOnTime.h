#pragma once
/*
@author zhp
@date 2014-2-28 21:07
@purpose 在指定的一段时间内，匹配指定的多个字符串
*/
#include "stdafx.h"
#include <map>
#include "../DependSelfLib.h"

class MultiStrMatchOnTime{
protected:
	MultiStrMatchOnTime();
	virtual ~MultiStrMatchOnTime();

public:
	//表示永不删除template_id对象
	static const size_t			kMultiStrLifecycle_Forever = MAXSIZE_T;

	//interfaces
public:
	static MultiStrMatchOnTime& GetMe();
	//初始化某个template_id对象
	//template_lifecycle==milliseconds
	//is_order_matched == true:按照初始化的顺序匹配
	//是否匹配成功后，该template_id就失效
	bool InitTemplateIdObj( int template_id, size_t template_lifecycle, bool is_order_matched );
	//增加被匹配的字符串
	bool AddMatchedStr( int template_id, const char* matched_str );
	//在指定的template_id对象中，匹配指定的字符串
	//返回true表示已全部成功匹配
	bool AllMatchSpecifyStr( int template_id, const char* specify_str );
	//重置匹配
	void ResetMatch( int template_id );
	//得到永不删除生命期
	size_t GetLifecycleForever() const;
	//是否匹配
	virtual bool IsMatched( const char* str_src, const char* specify_str );

private:
	friend void TimeoutImple( MultiStrMatchOnTime*, size_t, int );
	void Timeout( int template_id );

private:
	std::map<int, void*>								map_impl_;
};

class LuaVM;
//for lua
class MultiStrMatchOnTimeForLua : public MultiStrMatchOnTime{
	MultiStrMatchOnTimeForLua();
public:
	virtual bool IsMatched( const char* str_src, const char* specify_str );

	//register for lua
	static void RegForLua( LuaVM* lua_vm );
	static MultiStrMatchOnTimeForLua& GetMe();

private:
	LuaVM*					lua_vm_;
};