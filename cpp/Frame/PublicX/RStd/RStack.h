#pragma once
/*
@author zhp
@date 2017/2/27 6:37
@purpose for stack release
*/
#ifndef RC_INVOKED
#include "RDeque.h"

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new
__RSTD_BEGIN_
// TEMPLATE CLASS stack
template<class _Ty,
class _Container = __RSTD__ deque<_Ty> >
class stack
{	// LIFO queue implemented with a container
public:
	typedef stack<_Ty, _Container> _Myt;
	typedef _Container container_type;
	typedef typename _Container::value_type value_type;
	typedef typename _Container::size_type size_type;
	typedef typename _Container::reference reference;
	typedef typename _Container::const_reference const_reference;

	stack()
		: c()
	{	// construct with empty container
	}

	stack(const _Myt& _Right)
		: c(_Right.c)
	{	// construct by copying _Right
	}

	explicit stack(const _Container& _Cont)
		: c(_Cont)
	{	// construct by copying specified container
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		c = _Right.c;
		return (*this);
	}


	template<class _Alloc,
	class = typename _STD enable_if<_STD uses_allocator<_Container, _Alloc>::value,
		void>::type>
		explicit stack(const _Alloc& _Al)
		: c(_Al)
	{	// construct with allocator
	}

	template<class _Alloc,
	class = typename _STD enable_if<_STD uses_allocator<_Container, _Alloc>::value,
		void>::type>
		stack(const _Myt& _Right, const _Alloc& _Al)
		: c(_Right.c, _Al)
	{	// construct by copying specified container
	}

	template<class _Alloc,
	class = typename _STD enable_if<_STD uses_allocator<_Container, _Alloc>::value,
		void>::type>
		stack(const _Container& _Cont, const _Alloc& _Al)
		: c(_Cont, _Al)
	{	// construct by copying specified container
	}

	stack(_Myt&& _Right)
		_NOEXCEPT_OP(_STD is_nothrow_move_constructible<_Container>::value)
		: c(_STD move(_Right.c))
	{	// construct by moving _Right
	}

	explicit stack(_Container&& _Cont)
		: c(_STD move(_Cont))
	{	// construct by moving specified container
	}

	template<class _Alloc,
	class = typename _STD enable_if<_STD uses_allocator<_Container, _Alloc>::value,
		void>::type>
		stack(_Myt&& _Right, const _Alloc& _Al)
		: c(_STD move(_Right.c), _Al)
	{	// construct by moving specified container
	}

	template<class _Alloc,
	class = typename _STD enable_if<_STD uses_allocator<_Container, _Alloc>::value,
		void>::type>
		stack(_Container&& _Cont, const _Alloc& _Al)
		: c(_STD move(_Cont), _Al)
	{	// construct by moving specified container
	}

	_Myt& operator=(_Myt&& _Right)
		_NOEXCEPT_OP(_STD is_nothrow_move_assignable<_Container>::value)
	{	// assign by moving _Right
		c = _STD move(_Right.c);
		return (*this);
	}

	void push(value_type&& _Val)
	{	// insert element at beginning
		c.push_back(_STD move(_Val));
	}

	template<class... _Valty>
	void emplace(_Valty&&... _Val)
	{	// insert element at beginning
		c.emplace_back(_STD forward<_Valty>(_Val)...);
	}


	bool empty() const
	{	// test if stack is empty
		return (c.empty());
	}

	size_type size() const
	{	// test length of stack
		return (c.size());
	}

	reference top()
	{	// return last element of mutable stack
		return (c.back());
	}

	const_reference top() const
	{	// return last element of nonmutable stack
		return (c.back());
	}

	void push(const value_type& _Val)
	{	// insert element at end
		c.push_back(_Val);
	}

	void pop()
	{	// erase last element
		c.pop_back();
	}

	const _Container& _Get_container() const
	{	// get reference to container
		return (c);
	}

	void swap(_Myt& _Right)
		_NOEXCEPT_OP(_NOEXCEPT_OP(_Swap_adl(c, _Right.c)))
	{	// exchange contents with _Right
		_Swap_adl(c, _Right.c);
	}

protected:
	_Container c;	// the underlying container
};

template<class _Ty,
class _Container> inline
	bool operator==(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test for stack equality
	return (_Left._Get_container() == _Right._Get_container());
}

template<class _Ty,
class _Container> inline
	bool operator!=(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test for stack inequality
	return (!(_Left == _Right));
}

template<class _Ty,
class _Container> inline
	bool operator<(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test if _Left < _Right for stacks
	return (_Left._Get_container() < _Right._Get_container());
}

template<class _Ty,
class _Container> inline
	bool operator>(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test if _Left > _Right for stacks
	return (_Right < _Left);
}

template<class _Ty,
class _Container> inline
	bool operator<=(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test if _Left <= _Right for stacks
	return (!(_Right < _Left));
}

template<class _Ty,
class _Container> inline
	bool operator>=(const stack<_Ty, _Container>& _Left,
	const stack<_Ty, _Container>& _Right)
{	// test if _Left >= _Right for stacks
	return (!(_Left < _Right));
}
_STD_END

_STD_BEGIN
template<class _Ty,
class _Container,
class _Alloc>
struct _STD uses_allocator<__RSTD__ stack<_Ty, _Container>, _Alloc>
	: _STD uses_allocator<_Container, _Alloc>
{	// true_type if container allocator enabled
};
// stack TEMPLATE FUNCTIONS
template<class _Ty,
class _Container> inline
	void swap(__RSTD__ stack<_Ty, _Container>& _Left,
	__RSTD__ stack<_Ty, _Container>& _Right)
	_NOEXCEPT_OP(_NOEXCEPT_OP(_Left.swap(_Right)))
{	// swap _Left and _Right stacks
	_Left.swap(_Right);
}

_STD_END

#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
