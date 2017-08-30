#pragma once
/*
@author zhp
@date 2017/3/15 6:46
@purpose for string operate
*/
#include <string>
#include <Windows.h>
#include "../DependSelfLib.h"

template<typename T>
inline bool IsUnValidPtr(const T the_ptr, int the_size = 0){
	auto ptr_value = (unsigned int)the_ptr;
	if (ptr_value <= unsigned short(-1))
		return true;
/*
#ifdef _TARGET_WIN32
	if (ptr_value >= 0x80000000)
		return true;
#else
	if (ptr_value >= 0xffff0000)
		return true;
#endif*/
	if (the_size > 0)
	{
		if (IsBadReadPtr((const void*)the_ptr, the_size))
			return true;
	}
	else if (the_size == 0)
	{
		if (IsBadReadPtr((const void*)the_ptr, sizeof(T)))
			return true;
	}
	return false;
}

namespace StdEx{
	//str_max_size是字符个数，包括结束字符
	bool TStrCpy(char* dst_str, const char* src_str, unsigned int str_max_size);
	bool TStrCpy(wchar_t* dst_str, const wchar_t* src_str, unsigned int str_max_size);

	//str len
	unsigned int TStrLen(const char* str);
	unsigned int TStrLen(const wchar_t* str);

	int TStrCmp(const char* lhs, const char* rhs);
	int TStrCmp(const wchar_t* lhs, const wchar_t* rhs);

	template<typename CharT>
	unsigned int TStrMaxLen(const CharT* str, unsigned int max_len){
		unsigned int res = 0;
		if (IsUnValidPtr(str))
			return res;
		for (; max_len > 0 && 0 != *str; ++str, --max_len)
			++res;
		return res;
	}
	template<typename CharT>
	const CharT* TStristr(const CharT* str1, const CharT* str2){
		if (!str1 || !str2)
			return nullptr;
		auto cp = str1;
		const CharT* s1, *s2;

		if (!*str2)
			return str1;

		while (*cp)
		{
			s1 = cp;
			s2 = str2;

			while (*s1 && *s2 && !(std::tolower(*s1) - std::tolower(*s2)))
				s1++, s2++;

			if (!*s2)
				return cp;

			cp++;
		}

		return nullptr;
	}

	template<typename CharT>
	inline const CharT* TStrStr(const CharT* str1, const CharT* str2){
		if (!str1 || !str2)
			return nullptr;
		auto cp = str1;
		const CharT* s1, *s2;

		if (!*str2)
			return str1;

		while (*cp)
		{
			s1 = cp;
			s2 = str2;

			while (*s1 && *s2 && !(*s1 - *s2))
				s1++, s2++;

			if (!*s2)
				return cp;

			cp++;
		}

		return nullptr;
	}

	template<typename CharT>
	const CharT* TEmptyStr();
	template<>
	const char* TEmptyStr<char>();
	template<>
	const wchar_t* TEmptyStr<wchar_t>();
	template<typename CharT>
	const CharT* TEmptyStr(const CharT* str){
		if (IsUnValidPtr(str))
			return TEmptyStr<CharT>();
		return str;
	}
	template<typename CharT>
	const CharT* TEmptyStrAsMax(const CharT* str, unsigned int max_cnt){
		if (IsUnValidPtr(str))
			return TEmptyStr<CharT>();
		auto cnt = TStrMaxLen(str, max_cnt);
		if (cnt >= max_cnt)
			return TEmptyStr<CharT>();
		return str;
	}

	const char* TNilStr(const char* str);
	const char* TNilStrAsMax(const char* str, unsigned int max_cnt);
	const wchar_t* TNilStr(const wchar_t* str);
	const wchar_t* TNilStrAsMax(const wchar_t* str, unsigned int max_cnt);
}

namespace std{
	string to_string(const void* pointer);
	string to_string(const char* str);
	const string& to_string(const string& str);
	string& to_string(string& str);
}