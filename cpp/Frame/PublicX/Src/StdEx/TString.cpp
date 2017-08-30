#include "stdafx.h"
#include <StdEx/TString.h>
#include <assert.h>

const char* StdEx::TNilStr(const char* str)
{
	if (IsUnValidPtr(str))
		return "nil";
	return str;
}

const wchar_t* StdEx::TNilStr(const wchar_t* str)
{
	if (IsUnValidPtr(str))
		return L"nil";
	return str;
}

const char* StdEx::TNilStrAsMax(const char* str, unsigned int max_cnt)
{
	if (IsUnValidPtr(str))
		return "nil";
	auto cnt = TStrMaxLen(str, max_cnt);
	if (cnt >= max_cnt)
		return "nil";
	return str;
}

const wchar_t* StdEx::TNilStrAsMax(const wchar_t* str, unsigned int max_cnt)
{
	if (IsUnValidPtr(str))
		return L"nil";
	auto cnt = TStrMaxLen(str, max_cnt);
	if (cnt >= max_cnt)
		return L"nil";
	return str;
}

template<>
const char* StdEx::TEmptyStr<char>()
{
	return "";
}

template<>
const wchar_t* StdEx::TEmptyStr<wchar_t>()
{
	return L"";
}

unsigned int StdEx::TStrLen(const char* str)
{
	unsigned int res = 0;
	if (!str)
		return res;
	for (; 0 != *str; ++str)
		++res;
	return res;
}

unsigned int StdEx::TStrLen(const wchar_t* str)
{
	unsigned int res = 0;
	if (!str)
		return res;
	for (; 0 != *str; ++str)
		++res;
	return res;
}

bool StdEx::TStrCpy(char* dst_str, const char* src_str, unsigned int str_max_size)
{
	if (str_max_size <= 0)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dst_str))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(src_str))
	{
		assert(false);
		return false;
	}
	--str_max_size;
	for (unsigned int i = 0; i != str_max_size && *src_str != 0; ++i, ++dst_str, ++src_str)
	{
		*dst_str = *src_str;
	}
	*dst_str = 0;
	return true;
}

bool StdEx::TStrCpy(wchar_t* dst_str, const wchar_t* src_str, unsigned int str_max_size)
{
	if (str_max_size <= 0)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dst_str))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(src_str))
	{
		assert(false);
		return false;
	}
	--str_max_size;
	for (unsigned int i = 0; i != str_max_size && *src_str != 0; ++i, ++dst_str, ++src_str)
	{
		*dst_str = *src_str;
	}
	*dst_str = 0;
	return true;
}

int StdEx::TStrCmp(const char* lhs, const char* rhs)
{
	return strcmp(lhs, rhs);
}

int StdEx::TStrCmp(const wchar_t* lhs, const wchar_t* rhs)
{
	return wcscmp(lhs, rhs);
}

std::string std::to_string(const void* pointer)
{
	char buffer[65];
	buffer[0] = 0;
	return _i64toa((__int64)pointer, buffer, 16);
}

const std::string& std::to_string(const string& str)
{
	return str;
}

std::string& std::to_string(string& str)
{
	return str;
}

std::string std::to_string(const char* str)
{
	if (!str)
	{
		assert(false);
		return std::string();
	}
	return str;
}
