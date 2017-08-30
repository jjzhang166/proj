#pragma once
/*
@author zhp
@date 2016/4/19 4:55
@purpose stable container
*/
#include <ProtocolX/P_BaseTypeX.h>
#include "BC_Iterator.h"
#include "BC_Base.h"

#pragma pack(push)
#pragma pack(1)

template<typename WrapperValueType>
class BC_StableBufCont{
	template<typename ContT, typename ValueType>
	friend struct BC_Algo_WriteValue_Cont;
public:
	typedef WrapperValueType value_type;
	typedef pt_word size_type;
	static_assert(sizeof(pt_int) > sizeof(size_type), "");
	typedef BC_Iterator<value_type> const_iterator;

public:
	explicit BC_StableBufCont(BC_TypeIndexT value_type_idx){
		value_type_idx_ = value_type_idx;
		value_cnt_ = 0;
	}
	template<typename ContT>
	operator BC_Cont<ContT> () const{
		BC_Cont<ContT> res;
		auto it_end = end();
		for ( auto it = begin(); it < it_end; ++it )
			res.Add(*it);
		return res;
	}
	template<typename T, typename AllocT>
	operator std::vector<T, AllocT> () const{
		std::vector<T, AllocT> res;
		auto it_end = end();
		for ( auto it = begin(); it < it_end; ++it )
			res.push_back(*it);
		return res;
	}

public:
	bool Valid( size_type buffer_size ) const{
		pt_qword valid_size = ValueSize(buffer_size);
		if (valid_size + sizeof(*this) > buffer_size)
		{
			assert(false);
			return false;
		}
		return true;
	}
	pt_dword TotalSize() const{
		return (ValueSize() + sizeof(*this));
	}
	size_type ValueCnt() const{
		return value_cnt_;
	}
	pt_dword ValueSize( size_type buffer_size ) const{
		pt_dword size = sizeof(*this);
		auto it_end = end();
		for ( auto it = begin(); it < it_end; ++it )
		{
			auto cur_size = BC_Size<value_type>::get(*it);
			if ( cur_size <= 0 )
				return -1;
			size += cur_size;
			if ( size > buffer_size )
				return -1;
		}
		size -= sizeof(*this);
		return size;
	}
	pt_dword ValueSize() const{
		return ValueSize(-1);
	}
	inline BC_TypeIndexT GetValueTypeIdx() const{
		return value_type_idx_;
	}

public:
	const_iterator begin() const{
		return const_iterator((const value_type*)(const void*)((const char*)this + sizeof(*this)), 0);
	}
	const_iterator end() const{
		return const_iterator(nullptr, value_cnt_);
	}

private:
	BC_TypeIndexT	value_type_idx_;
	size_type		value_cnt_;
};

namespace Detail{
	struct BC_StableContTag{};
}
typedef BC_StableBufCont<Detail::BC_StableContTag> BC_StableBufCont_PlaceHolder;
static_assert(sizeof(BC_StableBufCont_PlaceHolder) == sizeof(BC_TypeIndexT) + sizeof(BC_StableBufCont_PlaceHolder::size_type), "");

#pragma pack(pop)
