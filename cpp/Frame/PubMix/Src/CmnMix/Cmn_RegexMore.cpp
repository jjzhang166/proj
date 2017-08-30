#include "stdafx.h"
#include <CmnMix/Cmn_RegexMore.h>
#include <boost/regex.hpp>
#include <CmnMix/Cmn_UsefulFunc.h>

struct RegHelper_{
	RegHelper_( RegexWordReplace& impl ) : impl_(impl){}
	template <typename T>
	std::wstring operator () ( const T& match_res ){
		const auto& str_word_src = match_res.str();
		const auto dest_str = impl_.FindDestStr_W(str_word_src);
		if (dest_str)
			return *dest_str;
		else
		{
			assert(!"找不到目标字符串");
			return str_word_src;
		}
	}

private:
	RegexWordReplace&			impl_;
};

std::string RegexWordReplace::Replace_A( const char* str_replaced )
{
	if ( !str_replaced )
	{
		assert(false);
		return "str_replaced is empty";
	}
	if (words_map_.empty())
	{
		assert(false);
		return "words_map_ is empty";
	}

	if ( !str_regex_ )
	{
		std::wstring str_tmp;
		for ( const auto& str_word : words_map_ )
		{
			str_tmp += std::move(MakeOneRegexWord_W(str_word.first));
			str_tmp += L'|';
		}
		if (str_tmp.empty())
		{
			assert(false);
			return "RegexWordReplace::Replace occur error";
		}
		str_tmp.pop_back();
		str_regex_ = std::move(str_tmp);
	}

	boost::wregex reg(str_regex_.get(), /*boost::wregex::icase|*/boost::wregex::perl);
	return std::move(
			WideCharToGbk(std::move(
				boost::regex_replace(
					std::move(GbkToWideChar(str_replaced)), reg, RegHelper_(*this)))));
}

std::string RegexWordReplace::Replace_A( const std::string& str_replaced )
{
	return Replace_A(str_replaced.c_str());
}

std::wstring RegexWordReplace::MakeOneRegexWord_W( const std::wstring& str_word_src ) const
{
	assert(!str_word_src.empty());
	std::wstring str_res(L"\\<");
	return std::move(str_res + str_word_src + L"\\>");
}

const std::wstring* RegexWordReplace::FindDestStr_W( const std::wstring& str_word_src )
{
	auto it = words_map_.find(str_word_src);
	if ( it != words_map_.end() )
	{
		return &it->second;
	}
	return nullptr;
}

const std::wstring* RegexWordReplace::FindDestStr_W( const wchar_t* str_word_src )
{
	auto it = words_map_.find(str_word_src);
	if ( it != words_map_.end() )
	{
		return &it->second;
	}
	return nullptr;
}

bool RegexWordReplace::AddMap_Src2Dst_A( const char* str_word_src, const char* str_word_dest )
{
	if ( !str_word_src || !str_word_dest )
	{
		assert(false);
		return false;
	}

	str_regex_.reset();
	words_map_[std::move(GbkToWideChar(str_word_src))] = std::move(GbkToWideChar(str_word_dest));
	return true;
}

bool RegexWordReplace::AddMap_Src2Dst_A( const std::string& str_word_src, const std::string& str_word_dest )
{
	return AddMap_Src2Dst_A(str_word_src.c_str(), str_word_dest.c_str());
}

void RegexWordReplace::EraseMap_Src_A( const char* str_word_src )
{
	if ( !str_word_src )
	{
		assert(false);
		return;
	}
	str_regex_.reset();
	words_map_.erase(std::move(GbkToWideChar(str_word_src)));
}

void RegexWordReplace::EraseMap_Src_A( const std::string& str_word_src )
{
	EraseMap_Src_A(str_word_src.c_str());
}

bool RegexWordReplace::empty() const
{
	return words_map_.empty();
}

void MatchPatternF_Composite::AddSubFormatter( MatchPatternFormatter* sub_formatter )
{
	if ( !sub_formatter )
	{
		assert(false);
		return;
	}
	sub_formatters_.push_back(sub_formatter);
}

std::wstring MatchPatternF_Composite::PatternString() const
{
	std::wstring str_res;
	for ( auto formatter : sub_formatters_ )
	{
		assert(formatter);
		str_res += formatter->PatternString();
	}
	return str_res;
}

void MatchPatternF_Composite::PatternToOstm( std::wostringstream& ostm ) const
{
	for ( auto formatter : sub_formatters_ )
	{
		assert(formatter);
		formatter->PatternToOstm(ostm);
	}
}

void MatchPatternFormatter::AddSubFormatter( MatchPatternFormatter* sub_formatter )
{

}

MatchPatternFormatter::~MatchPatternFormatter()
{

}

std::wstring MatchPattern_String::PatternString() const
{
	return pattern_string_;
}

void MatchPattern_String::PatternToOstm( std::wostringstream& ostm ) const
{
	ostm << pattern_string_;
}

MatchPattern_String::MatchPattern_String( const std::wstring& pattern_str ) : pattern_string_(pattern_str)
{

}

MatchPattern_Digital::MatchPattern_Digital( int digital, pt_dword width /*= 1*/, wchar_t fill_char /*= L' ' */ )
{
	assert(width >= 1);
	width_ = width;
	fill_char_ = fill_char;
	digital_ = digital;
}

std::wstring MatchPattern_Digital::PatternString() const
{
	std::wostringstream ostm;
	ostm.width(width_);
	ostm.fill(fill_char_);
	ostm << digital_;
	return ostm.str();
}

void MatchPattern_Digital::PatternToOstm( std::wostringstream& ostm ) const
{
	ostm.width(width_);
	ostm.fill(fill_char_);
	ostm << digital_;
}

MatchPattern_RefString::MatchPattern_RefString( const std::wstring& pattern_str ) : pattern_string_(pattern_str)
{

}

std::wstring MatchPattern_RefString::PatternString() const
{
	return pattern_string_;
}

void MatchPattern_RefString::PatternToOstm( std::wostringstream& ostm ) const
{
	ostm << pattern_string_;
}

std::wstring MatchPattern_AnyDigital::PatternString() const
{
	return L"\\d+?";
}

void MatchPattern_AnyDigital::PatternToOstm( std::wostringstream& ostm ) const
{
	ostm << L"\\d+?";
}
