#pragma once
/*
@author zhp
@date 2014/7/8 22:21
@purpose use as stream, but is binary
*/
#include <string>
#include <cassert>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <afxstr.h>
#include "../DependSelfLib.h"
#include <CommonX/CmnX_UsefulClass.h>

class StrOutputBStream{
public:
	StrOutputBStream();
	const std::string& GetStrBuffer() const;
	void Write( const void* src, pt_dword src_size );
	void Fill( pt_dword size, char value );
	template<typename CharT_>
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const std::basic_string<CharT_>& str_src ){
		if (str_src.empty())
			return ostm;
		ostm.Write(str_src.c_str(), (str_src.size() + 1) * sizeof(CharT_));
		return ostm;
	}
	template<typename CharT_>
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const ATL::CStringT<CharT_, StrTraitMFC<CharT_> >& str_src ){
		if (str_src.IsEmpty())
			return ostm;
		ostm.Write((const CharT_*)str_src, (str_src.GetLength() + 1) * sizeof(CharT_));
		return ostm;
	}
	template<typename CharT_>
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, std::basic_string<CharT_>& str_src ){
		if (str_src.empty())
			return ostm;
		ostm.Write(str_src.c_str(), (str_src.size() + 1) * sizeof(CharT_));
		return ostm;
	}
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const char* str_src ){
		ostm.Write(str_src, strlen(str_src) + sizeof(char));
		return ostm;
	}
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, char* str_src ){
		ostm.Write(str_src, strlen(str_src) + sizeof(char));
		return ostm;
	}
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const wchar_t* str_src ){
		ostm.Write(str_src, (wcslen(str_src) + 1) * sizeof(wchar_t));
		return ostm;
	}
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, wchar_t* str_src ){
		ostm.Write(str_src, (wcslen(str_src) + 1) * sizeof(wchar_t));
		return ostm;
	}
	template<typename T, pt_dword Cnt>
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const T (&ar)[Cnt] ){
		ostm.Write(ar, sizeof(ar));
		return ostm;
	}
	template<typename T>
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const T& ar ){
		ostm.Write(&ar, sizeof(ar));
		return ostm;
	}

protected:
	std::string				str_buffer_;
};

struct stBStreamOutput{
public:
	virtual ~stBStreamOutput();
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const stBStreamOutput& msg );

protected:
	virtual void Write( StrOutputBStream& ostm ) const;
};
#define B_STREAM_OUTPUT(derived_t)	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const derived_t& msg ){	\
	return ostm << static_cast<const stBStreamOutput&>(msg);	\
	}	\
/**/
class StrInputBStreamBase{
public:
	virtual ~StrInputBStreamBase();
	virtual void Append( const char* buffer, pt_dword buffer_size ) = 0;
	virtual bool Read( void* dst, pt_dword dst_size ) = 0;
	//ÖØÐÂ¶ÁÈ¡
	virtual void ReRead() = 0;
	virtual const char* CurPtr() const = 0;
	virtual pt_dword LeftSize() const = 0;
	virtual pt_dword ReadSize() const = 0;
	virtual bool Skip( pt_dword skip_size ) = 0;
	template<typename CharT_>
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istr, std::basic_string<CharT_>& str_dst ){
		auto left_size = istr.LeftSize();
		if ( left_size < 1 )
		{
			assert(false);
			return istr;
		}
		auto str_cnt = StdEx::TStrLen((const CharT_*)(const void*)istr.CurPtr());
		if ( str_cnt < 1 )
		{
			assert(false);
			return istr;
		}
		const auto read_size = (str_cnt + 1) * sizeof(CharT_);
		if ( left_size < read_size )
		{
			assert(false);
			return istr;
		}
		str_dst.resize(str_cnt);
		if ( !istr.Read((char*)str_dst.c_str(), read_size) )
		{
			assert(false);
			return istr;
		}
		return istr;
	}
	template<typename CharT_>
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istr, ATL::CStringT<CharT_, StrTraitMFC<CharT_> >& str_dst ){
		std::basic_string<CharT_> str_tmp;
		istr >> str_tmp;
		str_dst = str_tmp.c_str();
		return istr;
	}
	template<typename T, pt_dword Cnt>
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istr, T (&ar)[Cnt] ){
		if ( !istr.Read((void*)ar, sizeof(ar)) )
		{
			assert(false);
			return istr;
		}
		return istr;
	}
	template<typename T>
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istr, T& ar ){
		if ( !istr.Read((void*)&ar, sizeof(ar)) )
		{
			assert(false);
			return istr;
		}
		return istr;
	}
};

struct stBStreamInput{
public:
	virtual ~stBStreamInput();
	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, stBStreamInput& recv_value );

protected:
	virtual bool Read( StrInputBStreamBase& istm );
};
#define B_STREAM_INPUT(derived_t)	friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, derived_t& recv_value ){	\
	return istm >> static_cast<stBStreamInput&>(recv_value);	\
}	\
/**/
template<typename T>
class StrInputSkip;
template<typename CharT_>
class StrInputSkipStr_{};
template<typename CharT_>
class StrInputSkip<StrInputSkipStr_<CharT_> > : public stBStreamInput{
	bool Read( StrInputBStreamBase& istm ){
		WORD str_len;
		istm >> str_len;
		if ( str_len < 1 )
		{
			assert(false);
			return false;
		}
		return istm.Skip(str_len * sizeof(CharT_));
	}
	B_STREAM_INPUT(StrInputSkip)
};
template<typename T>
class StrInputSkip : public stBStreamInput{
	bool Read( StrInputBStreamBase& istm ){
		return istm.Skip(sizeof(T));
	}
	B_STREAM_INPUT(StrInputSkip)
};
template<typename CharT_>
StrInputSkip<StrInputSkipStr_<CharT_> > MakeStrInputSkipStr(){
	return StrInputSkip<StrInputSkipStr_<CharT_> >();
}
template<typename T>
StrInputSkip<T> MakeStrInputSkip(){
	return StrInputSkip<T>();
}

class StrInputBStream : public StrInputBStreamBase{
public:
	StrInputBStream( const char* buffer, pt_dword buffer_size );
	StrInputBStream( const std::string& str_buffer );
	void Append( const char* buffer, pt_dword buffer_size );
	bool Read( void* dst, pt_dword dst_size );
	void ReRead();
	const char* CurPtr() const;
	pt_dword LeftSize() const;
	pt_dword ReadSize() const;
	bool Skip( pt_dword skip_size );

protected:
	std::string				str_buffer_;
	pt_dword				cur_idx_;
};

class StableInputBStream : public StrInputBStreamBase{
public:
	StableInputBStream( const char* buffer, pt_dword buffer_size );
	StableInputBStream( const char* buffer_begin, const char* buffer_end );
	StableInputBStream( const std::string& str_input );
	void Append( const char* buffer, pt_dword buffer_size );
	bool Read( void* dst, pt_dword dst_size );
	void ReRead();
	const char* CurPtr() const;
	pt_dword LeftSize() const;
	pt_dword ReadSize() const;
	bool Skip( pt_dword skip_size );
	const char* GetBuffer() const;

protected:
	const char*			buffer_begin_;
	const char*			cur_ptr_;
	const char*			buffer_end_;
};

class StableOutputBStream{
public:
	StableOutputBStream(void* buffer_begin, unsigned int buffer_size);
	void Write(const void* src, pt_dword src_size);
	void Fill(pt_dword size, unsigned char value);
	pt_dword LeftSize() const;
	pt_dword GetWriteSize() const;

private:
	CmnX_CodeInfo		buffer_;
	pt_dword			write_size_;
};