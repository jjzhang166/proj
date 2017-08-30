#pragma once
/*
@author zhp
@date 2016/4/20 1:52
*/
#include <xutility>
#include <ProtocolX/P_BaseTypeX.h>
#include "BC_Base.h"

template<typename T>
class BC_Iterator : public std::_Iterator012<std::forward_iterator_tag,
	T,
	pt_int,
	const T*,
	const T&,
	std::_Iterator_base>{
public:
	BC_Iterator( const T* cur_ptr, pt_dword cnt ) : cur_ptr_(cur_ptr), cnt_(cnt){}
	BC_Iterator( const BC_Iterator& rhs ){
		cur_ptr_ = rhs.cur_ptr_;
		cnt_ = rhs.cnt_;
	}
	BC_Iterator& operator = ( const BC_Iterator& rhs ){
		cur_ptr_ = rhs.cur_ptr_;
		cnt_ = rhs.cnt_;
	}
	reference operator*() const{
		assert(cur_ptr_);
		return *cur_ptr_;
	}
	pointer operator->() const{
		return (std::pointer_traits<pointer>::pointer_to(**this));
	}
	BC_Iterator& operator++(){
		assert(cur_ptr_);
		auto cur_size = BC_Size<value_type>::get(*cur_ptr_);
		assert(cur_size > 0);
		++cnt_;
		cur_ptr_ = (const T*)((pt_csz)cur_ptr_ + cur_size);
		return (*this);
	}
	BC_Iterator operator++(int){	// postincrement
		auto _Tmp = *this;
		++*this;
		return (_Tmp);
	}
	bool operator<(const BC_Iterator& _Right) const{
		return cnt_ < _Right.cnt_;
	}

private:
	const T*	cur_ptr_;
	pt_dword	cnt_;
};