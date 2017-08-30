#pragma once
/*
@author zhp
@date 2017/2/27 8:24
@purpose for hash_set release
*/
#ifndef RC_INVOKED
#include "RXhash.h"
#include <hash_set>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

__RSTDEXT_BEGIN__
using _STD allocator;
using _STD enable_if;
using _STD equal_to;
using _STD hash;
using _STD is_convertible;
using _STD pair;
using __RSTD__ _Hash;

#if __RSTD_ITERATOR_DEBUG_LEVEL__ == 2
using _STD _Debug_range;
#endif /* __RSTD_ITERATOR_DEBUG_LEVEL__ == 2 */

// TEMPLATE CLASS _Hset_traits
template<class _Kty,	// key type (same as value type)
class _Tr,	// comparator predicate type
class _Alloc,	// actual _STD allocator type (should be value _STD allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
class _Hset_traits
	: public _Tr
{	// traits required to make _Hash behave like a set
public:
	typedef _Kty key_type;
	typedef _Kty value_type;
	typedef _Tr key_compare;
	typedef _Alloc allocator_type;

	enum
	{	// make multi parameter visible as an enum constant
		_Multi = _Mfl
	};

	_Hset_traits(const _Tr& _Traits = _Tr())
		: _Tr(_Traits)
	{	// construct with specified comparator
	}

	typedef key_compare value_compare;

	static const _Kty& _Kfn(const value_type& _Val)
	{	// return entire value as key
		return (_Val);
	}

	static int _Nonkfn(const value_type&)
	{	// extract "non-key" from element value (for container equality)
		return (0);
	}
};

// TEMPLATE CLASS hash_set
template<class _Kty, enXHashMemberTag _XHashMemberTag = enXHashMemberTag::kStd,
class _Tr = hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_Kty> >
class hash_set
	: public _Hash<_Hset_traits<_Kty, _Tr, _Alloc, false>, _XHashMemberTag>
{	// hash table of key values, unique keys
public:
	typedef hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hset_traits<_Kty, _Tr, _Alloc, false>, _XHashMemberTag > _Mybase;
	typedef _Kty key_type;
	typedef _Tr key_compare;
	typedef typename _Mybase::_Pairib _Pairib;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::reverse_iterator reverse_iterator;
	typedef typename _Mybase::const_reverse_iterator
		const_reverse_iterator;
	typedef typename _Mybase::value_type value_type;

	hash_set()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty set from defaults
	}

	explicit hash_set(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty set from defaults, _STD allocator
	}

	hash_set(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._List._Getal().select_on_container_copy_construction())


	{	// construct set by copying _Right
	}

	hash_set(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct set by copying _Right, _STD allocator
	}

	explicit hash_set(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct empty set from comparator
	}

	hash_set(const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct empty set from comparator and _STD allocator
	}

	template<class _Iter>
	hash_set(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct set from sequence, defaults
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	hash_set(_Iter _First, _Iter _Last,
		const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct set from sequence, comparator
		_Mybase::insert(_First, _Last);
	}
	template<class _Iter>
	hash_set(_Iter _First, _Iter _Last,
		const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct set from sequence, comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	hash_set(_Myt&& _Right)
		: _Mybase(_STD move(_Right), _Right._List._Getal())
	{	// construct set by moving _Right
	}

	hash_set(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
	{	// construct set by moving _Right, _STD allocator
	}

	_Myt& operator=(_Myt&& _Right)
	{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
	}

	_Pairib insert(value_type&& _Val)
	{	// insert a {key, mapped} value
		return (_Mybase::insert(_STD forward<value_type>(_Val)));
	}

	iterator insert(const_iterator _Where, value_type&& _Val)
	{	// insert a {key, mapped} value, with hint
		return (_Mybase::insert(_Where, _STD forward<value_type>(_Val)));
	}

	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	hash_set(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_set(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_set(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct from initializer_list, comparator, and _STD allocator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
	{	// assign initializer_list
		this->clear();
		this->insert(_Ilist.begin(), _Ilist.end());
		return (*this);
	}

	void insert(_XSTD initializer_list<value_type> _Ilist)
	{	// insert initializer_list
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	_Pairib insert(const value_type& _Val)
	{	// insert a key value
		return (_Mybase::insert(_Val));
	}

	iterator insert(const_iterator _Where, const value_type& _Val)
	{	// insert a key value, with hint
		return (_Mybase::insert(_Where, _Val));
	}

	template<class _Iter>
	void insert(_Iter _First, _Iter _Last)
	{	// insert [_First, _Last), arbitrary iterators
		_Mybase::insert(_First, _Last);
	}
};

template<class _Kty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator==(
	const hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_set equality
	return (_Hash_equal(_Left, _Right));
}

template<class _Kty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator!=(
	const hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_set inequality
	return (!(_Left == _Right));
}

// TEMPLATE CLASS hash_multiset
template<class _Kty, enXHashMemberTag _XHashMemberTag = enXHashMemberTag::kStd,
class _Tr = hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_Kty> >
class hash_multiset
	: public _Hash<_Hset_traits<_Kty, _Tr, _Alloc, true>, _XHashMemberTag>
{	// hash table of key values, non-unique keys
public:
	typedef hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hset_traits<_Kty, _Tr, _Alloc, true>, _XHashMemberTag> _Mybase;
	typedef _Kty key_type;
	typedef _Tr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::reverse_iterator reverse_iterator;
	typedef typename _Mybase::const_reverse_iterator
		const_reverse_iterator;
	typedef typename _Mybase::value_type value_type;

	hash_multiset()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty set from defaults
	}

	explicit hash_multiset(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty set from defaults, _STD allocator
	}

	hash_multiset(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._List._Getal().select_on_container_copy_construction())


	{	// construct set by copying _Right
	}

	hash_multiset(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct set by copying _Right, _STD allocator
	}

	explicit hash_multiset(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct empty set from comparator
	}

	hash_multiset(const key_compare& _Traits,
		const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct empty set from comparator and _STD allocator
	}

	template<class _Iter>
	hash_multiset(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from sequence, defaults
		_Mybase::insert(_First, _Last);
	}
	template<class _Iter>
	hash_multiset(_Iter _First, _Iter _Last,
		const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct from sequence, comparator
		_Mybase::insert(_First, _Last);
	}
	template<class _Iter>
	hash_multiset(_Iter _First, _Iter _Last,
		const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct from sequence, comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	hash_multiset(_Myt&& _Right)
		: _Mybase(_STD move(_Right), _Right._List._Getal())
	{	// construct set by moving _Right
	}

	hash_multiset(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
	{	// construct set by moving _Right, _STD allocator
	}

	_Myt& operator=(_Myt&& _Right)
	{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
	}

	iterator insert(value_type&& _Val)
	{	// insert a {key, mapped} value
		return (_Mybase::insert(_STD forward<value_type>(_Val)).first);
	}

	iterator insert(const_iterator _Where, value_type&& _Val)
	{	// insert a {key, mapped} value, with hint
		return (_Mybase::insert(_Where, _STD forward<value_type>(_Val)));
	}

	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_multiset(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct from initializer_list, comparator, and _STD allocator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	_Myt& operator=(_XSTD initializer_list<value_type> _Ilist)
	{	// assign initializer_list
		this->clear();
		this->insert(_Ilist.begin(), _Ilist.end());
		return (*this);
	}

	void insert(_XSTD initializer_list<value_type> _Ilist)
	{	// insert initializer_list
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	iterator insert(const value_type& _Val)
	{	// insert a key value
		return (_Mybase::insert(_Val).first);
	}

	iterator insert(const_iterator _Where, const value_type& _Val)
	{	// insert a key value, with hint
		return (_Mybase::insert(_Where, _Val));
	}

	template<class _Iter>
	void insert(_Iter _First, _Iter _Last)
	{	// insert [_First, _Last), arbitrary iterators
		_Mybase::insert(_First, _Last);
	}
};

template<class _Kty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator==(
	const hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_multiset equality
	return (_Hash_equal(_Left, _Right));
}

template<class _Kty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator!=(
	const hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_multiset inequality
	return (!(_Left == _Right));
}
_STDEXT_END

__RSTD_BEGIN_
using __RSTDEXT__ hash_set;
using __RSTDEXT__ hash_multiset;

template<class _Kty, enXHashMemberTag _XHashMemberTag, class _Tr, class _Alloc>
_STD hash_set<_Kty, _Tr, _Alloc> to_std(
	const __RSTD__ hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& v){
	_STD hash_set<_Kty, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Tr, class _Alloc>
__RSTD__ hash_set<_Kty, enXHashMemberTag::kStd, _Tr, _Alloc> from_std(const _STD hash_set<_Kty, _Tr, _Alloc>& v){
	__RSTD__ hash_set<_Kty, enXHashMemberTag::kStd, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, enXHashMemberTag _XHashMemberTag, class _Tr, class _Alloc>
_STD hash_multiset<_Kty, _Tr, _Alloc> to_std(const __RSTD__ hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& v){
	_STD hash_multiset<_Kty, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Tr, class _Alloc>
__RSTD__ hash_multiset<_Kty, enXHashMemberTag::kStd, _Tr, _Alloc> from_std(const _STD hash_multiset<_Kty, _Tr, _Alloc>& v){
	__RSTD__ hash_multiset<_Kty, enXHashMemberTag::kStd, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}
_STD_END

_STD_BEGIN

template<class _Kty, __RSTD__ enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	void swap(__RSTD__ hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	__RSTD__ hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// swap _Left and _Right hash_multisets
	_Left.swap(_Right);
}

template<class _Kty, __RSTD__ enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	void swap(__RSTD__ hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	__RSTD__ hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// swap _Left and _Right hash_sets
	_Left.swap(_Right);
}

_STD_END

#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
