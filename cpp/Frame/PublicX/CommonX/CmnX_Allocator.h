#pragma once
/*
@author zhp
@date 2016/2/9 21:20
@purpose for allocate memory
*/
#include <config/targetver.h>
#include <config/AdjustWin32.h>
#include <xstddef>

template<typename _Ty, typename AllocImpl>
class CmnAllcator{
public:
	typedef _Ty value_type;
	typedef CmnAllcator<value_type, AllocImpl> other;
	template<typename _Other, typename OtherAllocImpl>
	struct rebind{
		typedef CmnAllcator<_Other, OtherAllocImpl> other;
	};
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef void* void_pointer;
	typedef const void* const_void_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef unsigned int size_type;

	pointer address(reference _Val) const{
		return (std::addressof(_Val));
	}
	const_pointer address(const_reference _Val) const{
		return (std::addressof(_Val));
	}
	CmnAllcator() = default;
	CmnAllcator(const other&) = default;
	template<typename _Other, typename OtherAllocImpl>
	CmnAllcator(const CmnAllcator<_Other, OtherAllocImpl>&){}

	template<class _Other, typename OtherAllocImpl>
	other& operator=(const CmnAllcator<_Other, OtherAllocImpl>&){
		return (*this);
	}
	void deallocate(pointer _Ptr, size_type){
		AllocImpl::Free(_Ptr);
	}
	pointer allocate(size_type _Count){
		return (_Allocate(_Count, (pointer)0));
	}

	pointer allocate(size_type _Count, const void*){
		return (allocate(_Count));
	}

	void construct(value_type* _Ptr){
		::new ((void*)_Ptr) value_type();
	}

	void construct(value_type* _Ptr, const value_type& _Val){
		::new ((void *)_Ptr) value_type(_Val);
	}

	template<typename T1, typename T2, typename... Args>
	void construct(value_type *_Ptr, const T1& v1, const T2& v2, const Args&... args){
		::new ((void *)_Ptr) value_type(v1, v2, args...);
	}

	template<class _Uty>
	void destroy(_Uty* _Ptr){
		_Ptr->~_Uty();
	}
	size_type max_size() const{
		return ((size_type)(-1) / sizeof(value_type));
	}

private:
	template<class T>
	inline T* _Allocate(size_type _Count, T*){
		void* _Ptr = 0;

		if (_Count == 0)
			;
		else if ((max_size() < _Count)
			|| (_Ptr = AllocImpl::Malloc(_Count * sizeof(T))) == nullptr)
			throw std::bad_alloc();
		return ((T*)_Ptr);
	}
};
