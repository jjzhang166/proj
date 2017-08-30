#pragma once
/*
@author zhp
@date 2014/7/6 18:37
@purpose for game char array
*/
#include <Common/CharArray.h>
#include <Common/BinaryStream.h>
#include <vector>
#include "GFuncCall.h"

template<std::size_t CharCnt>
class CharArrayGame : public CharArray<wchar_t, CharCnt>{
public:
	CharArrayGame() : CharArray(){}
	template<std::size_t OtherCharCnt>
	CharArrayGame( const CharArrayGame<OtherCharCnt>& other ) : CharArray(other){}
	CharArrayGame( const CharType* other ) : CharArray(other){}
	CharArrayGame( const StdString& other ) : CharArrayGame(other){}
	template<std::size_t OtherCharCnt>
	CharArrayGame& operator = ( const CharArrayGame<OtherCharCnt>& other ){
		__super::operator = (other);
		return *this;
	}
	CharArrayGame& operator = ( const CharType* other ){
		__super::operator = (other);
		return *this;
	}
	CharArrayGame& operator = ( const StdString& other ){
		__super::operator = (other);
		return *this;
	}
	friend StrOutputBStream&
		operator << ( StrOutputBStream& ostm, const CharArrayGame& c_array ){
			ostm << (WORD)c_array.size() << (const CharType*)c_array.data();
			return ostm;
	}
};

#define MAKE_CHAR_ARRAY_GAME(wc, cag_name) CharArrayGame<_countof(wc) - 1> cag_name(wc)
/**/

//运行时的字符串
template<typename CharT_>
class GameCharRuntimeT{
public:
	typedef std::basic_string<CharT_> StrType;

public:
	GameCharRuntimeT( const StrType& str_src ) : str_src_(const_cast<StrType&>(str_src)){}
	GameCharRuntimeT( StrType& str_src ) : str_src_(str_src){}
	friend StrOutputBStream&
		operator << ( StrOutputBStream& ostm, const GameCharRuntimeT& game_str_warp ){
			ostm << (WORD)(game_str_warp.str_src_.size() + 1);
			if (game_str_warp.str_src_.empty())
				ostm << CharT_(0);
			else
				ostm << game_str_warp.str_src_;
		return ostm;
	}
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, GameCharRuntimeT& recv_value ){
		WORD str_len;
		istm >> str_len;
		if ( str_len < 1 )
		{
			assert(false);
			return istm;
		}
		if ( 1 == str_len )
		{
			CharT_ tmp;
			istm >> tmp;
			assert(0 == tmp);
			recv_value.str_src_.clear();
		}
		else
			istm >> recv_value.str_src_;
		if ( str_len != recv_value.str_src_.size() + 1 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "str_len ->" << str_len << "\t recv_value.size->" << recv_value.str_src_.size() << "\t the str->" << recv_value.str_src_;
			//assert(false);
		}
		return istm;
	}

private:
	StrType&				str_src_;
};
typedef GameCharRuntimeT<wchar_t> GameCharRuntime;
typedef GameCharRuntimeT<char> GameCharRuntimeA;

template<typename CharT_>
class GameCharRuntimeCStrT{
public:
	typedef ATL::CStringT<CharT_, StrTraitMFC<CharT_> > StrType;

public:
	GameCharRuntimeCStrT( const StrType& str_src ) : str_src_(const_cast<StrType&>(str_src)){}
	GameCharRuntimeCStrT( StrType& str_src ) : str_src_(str_src){}
	friend StrOutputBStream&
		operator << ( StrOutputBStream& ostm, const GameCharRuntimeCStrT& game_str_warp ){
			ostm << (WORD)(game_str_warp.str_src_.GetLength() + 1);
			if (game_str_warp.str_src_.IsEmpty())
				ostm << CharT_(0);
			else
				ostm << (const CharT_*)game_str_warp.str_src_;
		return ostm;
	}
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, GameCharRuntimeCStrT& recv_value ){
		WORD str_len;
		istm >> str_len;
		if ( str_len < 1 )
		{
			assert(false);
			return istm;
		}
		if ( 1 == str_len )
		{
			CharT_ tmp;
			istm >> tmp;
			assert(0 == tmp);
			recv_value.str_src_.Empty();
		}
		else
			recv_value >> recv_value.str_src_;
		assert(str_len == recv_value.str_src_.GetLength() + 1);
		return istm;
	}

private:
	StrType&				str_src_;
};
typedef GameCharRuntimeCStrT<wchar_t> GameCharRuntimeCStr;

template<typename T>
class GameArrayT{
public:
	typedef std::vector<T> ContType;
	typedef typename ContType::value_type value_type;
	
public:
	ContType& GetCont(){
		return cont_;
	}
	const ContType& GetCont() const{
		return cont_;
	}
	void PushBack( const T& v ){
		cont_.push_back(v);
	}
	bool Empty() const{
		return cont_.empty();
	}
	std::size_t Size() const{
		return cont_.size();
	}
	friend StrOutputBStream&
		operator << ( StrOutputBStream& ostm, const GameArrayT& t_array ){
		/*if (t_array.Empty())
		{
			assert(false);
			return ostm;
		}*/
		using namespace GFuncCall;
		static_assert(sizeof(std::size_t) == sizeof(DWORD), "");
		assert(0 != kGameHashArray);
		ostm << kGameHashArray << t_array.Size();
		DWORD idx = 0;
		for ( const auto& item : t_array.cont_ )
		{
			ostm << CalcHash_WORD(L"item_" + std::to_wstring(idx++)) << item;
		}
		return ostm;
	}
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, GameArrayT& recv_value ){
		using namespace GFuncCall;
		WORD hash_array;
		istm >> hash_array;
		if ( hash_array != kGameHashArray )
		{
			assert(false);
			return istm;
		}
		int a_size;
		istm >> a_size;
		if ( a_size < 0 )
		{
			assert(false);
			return istm;
		}
		WORD hash_item;
		value_type v_tmp;
		for ( auto i = 0; i < a_size; ++i )
		{
			istm >> hash_item;
			assert(hash_item == GFuncCall::CalcHash_WORD(L"item_" + std::to_wstring(i)));
			istm >> v_tmp;
			recv_value.cont_.push_back(v_tmp);
		}
		return istm;
	}

private:
	ContType			cont_;
};

template<typename T>
class GameArrayT_{
public:
	typedef std::vector<T> ContType;
	typedef typename ContType::value_type value_type;
	
public:
	ContType& GetCont(){
		return cont_;
	}
	const ContType& GetCont() const{
		return cont_;
	}
	void PushBack( const T& v ){
		cont_.push_back(v);
	}
	bool Empty() const{
		return cont_.empty();
	}
	std::size_t Size() const{
		return cont_.size();
	}
	friend StrOutputBStream&
		operator << ( StrOutputBStream& ostm, const GameArrayT_& t_array ){
		/*if (t_array.Empty())
		{
			assert(false);
			return ostm;
		}*/
		using namespace GFuncCall;
		static_assert(sizeof(std::size_t) == sizeof(DWORD), "");
		assert(0 != kGameHashArray);
		ostm << kGameHashArray << t_array.Size();
		DWORD idx = 0;
		for ( const auto& item : t_array.cont_ )
		{
			ostm << CalcHash_WORD(L"item_" + std::to_wstring(idx++)) << item;
		}
		return ostm;
	}
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, GameArrayT_& recv_value ){
		using namespace GFuncCall;
		WORD hash_array;
		istm >> hash_array;
		if ( hash_array != kGameHashArray )
		{
			assert(false);
			return istm;
		}
		int a_size;
		istm >> a_size;
		if ( a_size < 0 )
		{
			assert(false);
			return istm;
		}
		//WORD hash_item;
		value_type v_tmp;
		for ( auto i = 0; i < a_size; ++i )
		{
			/*istm >> hash_item;
			assert(hash_item == GFuncCall::CalcHash_WORD(L"item_" + std::to_wstring(i)));*/
			istm >> v_tmp;
			recv_value.cont_.push_back(v_tmp);
		}
		return istm;
	}

private:
	ContType			cont_;
};