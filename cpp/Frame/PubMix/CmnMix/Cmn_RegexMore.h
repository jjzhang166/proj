#pragma once
/*
@author zhp
@date 2014-3-24 21:59
@purpose for regex
*/
#include <string>
#include <map>
#include <boost/optional.hpp>
#include <sstream>
#include <vector>
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>

//regex word replace
class RegexWordReplace{
public:
	//增加原字符串到替换字符串的映射
	bool AddMap_Src2Dst_A( const char* str_word_src, const char* str_word_dest );
	bool AddMap_Src2Dst_A( const std::string& str_word_src, const std::string& str_word_dest );
	//删除原字符串的映射
	void EraseMap_Src_A( const char* str_word_src );
	void EraseMap_Src_A( const std::string& str_word_src );
	//进行替换
	std::string Replace_A( const char* str_replaced );
	std::string Replace_A( const std::string& str_replaced );
	//find dest string
	const std::wstring* FindDestStr_W( const wchar_t* str_word_src );
	const std::wstring* FindDestStr_W( const std::wstring& str_word_src );
	//是否为空
	bool empty() const;

private:
	//make one regex word
	std::wstring MakeOneRegexWord_W( const std::wstring& str_word_src ) const;

private:
	std::map<std::wstring, std::wstring>			words_map_;
	boost::optional<std::wstring>					str_regex_;
};

//模式匹配格式
class MatchPatternFormatter{
public:
	virtual ~MatchPatternFormatter();
	//添加子模式格式,相识关系
	virtual void AddSubFormatter( MatchPatternFormatter* sub_formatter );
	//得到模式字符串
	virtual std::wstring PatternString() const = 0;
	//输出到std::ostringstream
	virtual void PatternToOstm( std::wostringstream& ostm ) const = 0;
};

//格式组合
class MatchPatternF_Composite : public MatchPatternFormatter{
public:
	virtual void AddSubFormatter( MatchPatternFormatter* sub_formatter );
	virtual std::wstring PatternString() const;
	virtual void PatternToOstm( std::wostringstream& ostm ) const;

private:
	std::vector<MatchPatternFormatter*>				sub_formatters_;
};

//模式字符串格式
class MatchPattern_String : public MatchPatternFormatter{
public:
	MatchPattern_String( const std::wstring& pattern_str );
	virtual std::wstring PatternString() const;
	virtual void PatternToOstm( std::wostringstream& ostm ) const;

private:
	std::wstring			pattern_string_;
};

class MatchPattern_RefString : public MatchPatternFormatter{
public:
	MatchPattern_RefString( const std::wstring& pattern_str );
	virtual std::wstring PatternString() const;
	virtual void PatternToOstm( std::wostringstream& ostm ) const;

private:
	const std::wstring&			pattern_string_;
};

//带宽度的数字格式
class MatchPattern_Digital : public MatchPatternFormatter{
public:
	MatchPattern_Digital( int digital, pt_dword width = 1, wchar_t fill_char = L' ' );
	virtual std::wstring PatternString() const;
	virtual void PatternToOstm( std::wostringstream& ostm ) const;

private:
	pt_dword				width_;
	wchar_t					fill_char_;
	int						digital_;
};

//任意数字
class MatchPattern_AnyDigital : public MatchPatternFormatter{
public:
	virtual std::wstring PatternString() const;
	virtual void PatternToOstm( std::wostringstream& ostm ) const;
};