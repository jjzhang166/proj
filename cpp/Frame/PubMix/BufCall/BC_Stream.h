#pragma once
/*
@author zhp
@date 2016/4/17 20:50
*/
#include <CmnMix/Cmn_BinaryStream.h>
#include "BC_Config.h"
#include "BC_RegType.h"
#include "BC_Cont.h"
#include "BC_DataConver.h"
#include "BC_Base.h"
#include "Detail/BC_DataCheck.hpp"
//#include <CmnMix/Cmn_LogOut.h>

//////////////////////////////////////////////////////////////////////////
template<typename ContT>
BC_Cont<const ContT&> BC_MakeCont( const ContT& cont ){
	return BC_Cont<const ContT&>(cont);
}

namespace Detail{
	template<typename T>
	struct BC_MakeArrayImpl{};
	template<typename T, int size>
	struct BC_MakeArrayImpl<T[size]>{
		template<typename U>
		static const std::array<T, size>& get(const U& v){
			static_assert(sizeof(v) == sizeof(std::array<T, size>), "");
			return PointerToRef((const std::array<T, size>*)(const void*)v);
		}
	};
}
//////////////////////////////////////////////////////////////////////////
class BC_AsStreamR{
public:
	BC_AsStreamR( const char* buffer, pt_dword buffer_size );

public:
	struct StreamWrapper{
		explicit StreamWrapper(BC_AsStreamR& stm) : stm_(stm){
			valid_ = true;
		}
		template<typename T>
		StreamWrapper& operator& (T& v){
			if (!valid_)
				return *this;
			auto pn = BC_DataWrapper<T, StreamWrapper>::AsT(stm_);
			if (!pn.Valid())
			{
				assert(false);
				throw Detail::BC_Except_Serial("operator& serialzation failed");
				valid_ = false;
				return *this;
			}
			SetValueAux<T>(v, static_cast<typename BC_DataConverter<decltype(pn)>::type>(pn));
			return *this;
		}
		bool Valid() const{
			return valid_;
		}
		BC_AsStreamR& GetStream() const{
			return stm_;
		}
		void OccurError(){
			valid_ = false;
		}

	private:
		BC_AsStreamR&	stm_;
		bool			valid_;
	};

public:
	const Detail::BC_Uuid* GetFuncId() const;
	pt_dword ReadSize() const;
	pt_dword LeftSize() const;
	bool Skip( pt_dword skip_size );
	const char* CurPtr() const;

public:
	/*template<typename T>
	BC_AsStreamR& operator >> (T& v){
		auto pn = BC_DataWrapper<T, BC_AsStreamR>::AsT(stm_);
		if (!pn.Valid())
		{
			assert(false);
			throw Detail::BC_Except_Serial("operator& serialzation failed123");
			return *this;
		}
		SetValueAux<T>(v, static_cast<typename BC_DataConverter<decltype(pn)>::type>(pn));
		return *this;
	}*/
	template<typename T>
	bool ValidNextT(){
		BC_TypeIndexT type_idx;
		if ( !impl_.Read(&type_idx, sizeof(type_idx)) )
			return false;
		auto& the_type_info = BC_TypeInfoCont::Find((Detail::enBC_TypeIndex)type_idx);
		auto& t_type_info = BC_GetValueTypeInfo<T>();
		if ( t_type_info.GetTypeId() != the_type_info.GetTypeId() )
		{
			assert(false);
			return false;
		}
		return true;
	}
	template<typename T, typename TypeConverter>
	const T* As( TypeConverter converter ){
		auto ptr = static_cast<const T*>((const void*)impl_.CurPtr());
		assert(ptr);
		if ( !converter.DoCheckSize(*this, *ptr) )
			return nullptr;
		if ( !converter.DoSkip(*this, *ptr) )
			return nullptr;
		return ptr;
	}

private:
	StableInputBStream		impl_;
};

class BC_AsStreamW{
public:
	explicit BC_AsStreamW( const Detail::BC_Uuid& func_id );
	template<typename T>
	BC_AsStreamW& operator<< ( const T& value ){
		//try{
			BC_DataWrapper<T, BC_AsStreamW>::Write(*this, value);
		/*}DO_CATCH_THE_EXCEPTION(const Detail::BC_Exception& e, {
			LOG_O(Log_debug) << e.what();
			assert(false);
		})*/
		return *this;
	}

public:
	struct StreamWrapper{
		explicit StreamWrapper(BC_AsStreamW& stm) : stm_(stm){
		}
		template<typename T>
		StreamWrapper& operator& (const T& v){
			stm_ << v;
			return *this;
		}
		BC_AsStreamW& GetStream() const{
			return stm_;
		}

	private:
		BC_AsStreamW&	stm_;
	};

public:
	const std::string& GetStrBuffer() const;
	void WriteTypeIdx( BC_TypeIndexT type_idx );
	void WriteValue(const void* src, pt_dword src_size);
	void Fill(pt_dword size, unsigned char value);

private:
	StrOutputBStream		impl_;
};
//////////////////////////////////////////////////////////////////////////
class BC_NoTypeIdxStreamR{
public:
	BC_NoTypeIdxStreamR(pt_pcbyte buffer, pt_dword buffer_size);

public:
	struct StreamWrapper{
		explicit StreamWrapper(BC_NoTypeIdxStreamR& stm) : stm_(stm){
			valid_ = true;
		}
		template<typename T>
		StreamWrapper& operator& (T& v){
			if (!valid_)
				return *this;
			auto pn = BC_DataWrapper<T, BC_DataWrapperPolicy_NoDoValid>::AsT(stm_);
			if (!pn.Valid())
			{
				assert(false);
				//throw Detail::BC_Except_Serial("operator& serialzation failed1");
				valid_ = false;
				return *this;
			}
			SetValueAux<T>(v, static_cast<typename BC_DataConverter<decltype(pn)>::type>(pn));
			return *this;
		}
		bool Valid() const{
			return valid_;
		}
		BC_NoTypeIdxStreamR& GetStream() const{
			return stm_;
		}
		void OccurError(){
			valid_ = false;
		}

	private:
		BC_NoTypeIdxStreamR&	stm_;
		bool					valid_;
	};

public:
	pt_dword LeftSize() const;
	bool Skip(pt_dword skip_size);
	const char* CurPtr() const;

public:
	/*template<typename T>
	BC_NoTypeIdxStreamR& operator >> (T& v){
		auto pn = BC_DataWrapper<T, BC_DataWrapperPolicy_NoDoValid>::AsT(stm_);
		if (!pn.Valid())
		{
			assert(false);
			//throw Detail::BC_Except_Serial("operator& serialzation failed");
			return *this;
		}
		SetValueAux<T>(v, static_cast<typename BC_DataConverter<decltype(pn)>::type>(pn));
		return *this;
	}*/
	template<typename T>
	bool ValidNextT(){
		return true;
	}
	template<typename T, typename TypeConverter>
	const T* As(TypeConverter converter){
		auto ptr = static_cast<const T*>((const void*)impl_.CurPtr());
		assert(ptr);
		if (!converter.DoCheckSize(*this, *ptr))
			return nullptr;
		if (!converter.DoSkip(*this, *ptr))
			return nullptr;
		return ptr;
	}

private:
	StableInputBStream		impl_;
};

class BC_NoTypeIdxStreamW{
public:
	BC_NoTypeIdxStreamW(void* buffer_begin, unsigned int buffer_size);
	template<typename T>
	BC_NoTypeIdxStreamW& operator<< (const T& value){
		BC_DataWrapper<T, BC_DataWrapperPolicy_NoDoValid>::Write(*this, value);
		return *this;
	}

public:
	struct StreamWrapper{
		explicit StreamWrapper(BC_NoTypeIdxStreamW& stm) : stm_(stm){
		}
		template<typename T>
		StreamWrapper& operator& (const T& v){
			stm_ << v;
			return *this;
		}
		BC_NoTypeIdxStreamW& GetStream() const{
			return stm_;
		}

	private:
		BC_NoTypeIdxStreamW&	stm_;
	};

public:
	void WriteTypeIdx(BC_TypeIndexT type_idx);
	void WriteValue(const void* src, pt_dword src_size);
	void Fill(pt_dword size, unsigned char value);
	pt_dword LeftSize() const;
	pt_dword GetWriteSize() const;

private:
	StableOutputBStream		impl_;
};
//////////////////////////////////////////////////////////////////////////