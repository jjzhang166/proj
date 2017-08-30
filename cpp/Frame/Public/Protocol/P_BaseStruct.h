#pragma once
/*
@author zhp
@date 2013-12-25
@purpose basic classes
*/
#include "stdafx.h"
#include "P_Defines.h"
#include <vector>
#include <string>

struct stPlayerInfo{
	std::string					name_;
	pt_word						level_;
};

struct stAccountInfo{
	std::string					account_;
	std::string					password_;
	std::vector<stPlayerInfo>	players_;
};

//自定义判断标志
template<typename T>
struct stSpecifyTag_T{
	stSpecifyTag_T( const char* str_tag ){
		if (str_tag)
		{
			memcpy(tag_value_, str_tag, sizeof(tag_value_));
		}
	}

	bool operator == ( const stSpecifyTag_T& rhs ){
		return 0 == memcmp(tag_value_, rhs.tag_value_, sizeof(tag_value_));
	}

private:
	BYTE			tag_value_[sizeof(T)];
};

template<size_t SizeN>
struct stSpecifyTag_N{
	stSpecifyTag_N( const char* str_tag ){
		if (str_tag)
		{
			memcpy(tag_value_, str_tag, SizeN);
		}
	}
	bool operator == ( const stSpecifyTag_N& rhs ){
		return 0 == memcmp(tag_value_, rhs.tag_value_, SizeN);
	}

private:
	BYTE			tag_value_[SizeN];
};