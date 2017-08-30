#pragma once
/*
@author zhp
@date 2014/4/22 4:02
@purpose for char/wchar_t array
*/
#include <array>
#include <xtr1common>
#include <iostream>
#include <ProtocolX/P_BaseTypeX.h>
#include <cstringt.h>

#pragma pack(push)
#pragma pack(1)

template<typename CharT, pt_dword CharCnt>
class CharArray : public std::array<CharT, CharCnt + 1>{
public:
	typedef CharT CharType;
	typedef std::basic_string<CharType> StdString;

public:
	CharArray(){
		data()[0] = 0;
	}
	template<pt_dword OtherCharCnt>
	explicit CharArray( const CharArray<CharType, OtherCharCnt>& other ){
		StdEx::TStrCpy(data(), other.data(), size());
	}
	explicit CharArray( const CharType* other ){
		if (other)
			StdEx::TStrCpy(data(), other, size());
		else
			data()[0] = 0;
	}
	explicit CharArray( const StdString& other ){
		if (other.empty())
			data()[0] = 0;
		else
			StdEx::TStrCpy(data(), other.c_str(), size());
	}
	template<pt_dword OtherCharCnt>
	CharArray& operator = ( const CharArray<CharType, OtherCharCnt>& other ){
		StdEx::TStrCpy(data(), other.data(), size());
		return *this;
	}
	CharArray& operator = ( const CharType* other ){
		if (other)
			StdEx::TStrCpy(data(), other, size());
		return *this;
	}
	CharArray& operator = ( const StdString& other ){
		if ( !other.empty() )
			StdEx::TStrCpy(data(), other.c_str(), size());
		return *this;
	}
	StdString ToString() const{
		return data();
	}
	bool empty() const{
		return 0 == data()[0];
	}
	operator StdString () const{
		return data();
	}
	friend std::basic_ostream<CharType, std::char_traits<CharType> >&
		operator << ( std::basic_ostream<CharType, std::char_traits<CharType> >& ostm, const CharArray& c_array ){
		ostm << c_array.data();
		return ostm;
	}
	friend std::basic_istream<CharType, std::char_traits<CharType> >& 
		operator >> ( std::basic_istream<CharType, std::char_traits<CharType> >& istm, CharArray& c_array ){
		StdString std_str;
		istm >> std_str;
		c_array = std_str;
		return istm;
	}
};

template<typename CharT>
class CharArray<CharT, 0> : public std::array<CharT, 0>{};

template<typename CharT>
class CStrRef{
public:
	typedef CharT CharType;

public:
	CStrRef(){
		cstr_ = nullptr;
	}
	CStrRef( const CharType* cstr ){
		assert(cstr);
		cstr_ = cstr;
	}

private:
	const CharType*		cstr_;
};

//动态内存中的
template<typename CharT>
class BufCharArray{
public:
	typedef CharT CharType;
	typedef pt_word SizeT;
	static_assert(sizeof(pt_int) > sizeof(SizeT), "");

public:
	BufCharArray(){
		char_cnt_ = 0;
	}
	explicit BufCharArray ( SizeT char_cnt ){
		char_cnt_ = char_cnt;
	}
	explicit BufCharArray( const CharType* cstr ){
		char_cnt_ = StdEx::TStrLen(cstr);
		StdEx::TStrCpy(c_str(), cstr);
	}
	BufCharArray& operator = ( const CharType* cstr ){
		char_cnt_ = StdEx::TStrLen(cstr);
		StdEx::TStrCpy(c_str(), cstr);
		return *this;
	}
	template<typename _Traits, typename _Alloc>
	operator std::basic_string<CharT, _Traits, _Alloc> () const{
		return std::basic_string<CharT, _Traits, _Alloc>(c_str(), char_cnt_);
	}
	template<typename StringTraits>
	operator ATL::CStringT<CharT, StringTraits> () const{
		return ATL::CStringT<CharT, StringTraits>(c_str(), (int)char_cnt_);
	}
	operator const CharType* () const{
		return c_str();
	}

public:
	bool Valid( SizeT buffer_size ) const{
		if (TotalSize() > buffer_size)
		{
			assert(false);
			return false;
		}
		if ( *(c_str() + char_cnt_) != 0 )
		{
			assert(false);
			return false;
		}
		return true;
	}
	pt_dword TotalSize() const{
		return (CharSize() + sizeof(char_cnt_));
	}
	SizeT CharCnt() const{
		return char_cnt_;
	}
	pt_dword CharSize() const{
		return ((pt_dword)char_cnt_ + 1) * sizeof(CharType);
	}
	const CharType* c_str() const{
		return (const CharType*)(const void*)((const char*)this + sizeof(char_cnt_));
	}

private:
	SizeT		char_cnt_;		//必须是第一个成员变量
};
typedef BufCharArray<char> BufCharArray_A;
typedef BufCharArray<wchar_t> BufCharArray_W;
static_assert(sizeof(BufCharArray_A) == sizeof(BufCharArray_A::SizeT), "");

#pragma pack(pop)
