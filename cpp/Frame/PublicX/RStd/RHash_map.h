#pragma once
/*
@author zhp
@date 2017/2/27 8:24
@purpose for hash_map release
*/
#ifndef RC_INVOKED
#include "RXhash.h"
#include <hash_map>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

__RSTDEXT_BEGIN__
using __RSTD__ _Hash;

// TEMPLATE CLASS _Hmap_traits
template<class _Kty,	// key type
class _Ty,	// mapped type
class _Tr,	// comparator predicate type
class _Alloc,	// actual _STD allocator type (should be value _STD allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
class _Hmap_traits
	: public _Tr
{	// traits required to make _Hash behave like a map
public:
	typedef _Kty key_type;
	typedef _STD pair<const _Kty, _Ty> value_type;
	typedef _Tr key_compare;
	typedef _Alloc allocator_type;

	enum
	{	// make multi parameter visible as an enum constant
		_Multi = _Mfl
	};

	_Hmap_traits(const _Tr& _Traits = _Tr())
		: _Tr(_Traits)
	{	// construct with specified comparator
	}

	class value_compare
		: public _STD binary_function<value_type, value_type, bool>
	{	// functor for comparing two element values
	public:
		bool operator()(const value_type& _Left,
			const value_type& _Right) const
		{	// test if _Left precedes _Right by comparing just keys
			return (_Keycompobj(_Left.first, _Right.first));
		}

		value_compare(const key_compare& _Keycomparg)
			: _Keycompobj(_Keycomparg)
		{	// construct with specified predicate
		}

		key_compare _Keycompobj;
	};

	template<class _Ty1,
	class _Ty2>
		static const _Kty& _Kfn(const _STD pair<_Ty1, _Ty2>& _Val)
	{	// extract key from element value
		return (_Val.first);
	}

	template<class _Ty1,
	class _Ty2>
		static const _Ty2& _Nonkfn(const _STD pair<_Ty1, _Ty2>& _Val)
	{	// extract non-key from element value
		return (_Val.second);
	}
};

// TEMPLATE CLASS hash_map
template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag = enXHashMemberTag::kStd,
class _Tr = hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class hash_map
	: public _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, false>, _XHashMemberTag>
{	// hash table of {key, mapped} values, unique keys
public:
	typedef hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, false>, _XHashMemberTag> _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;
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

	hash_map()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty map from defaults
	}

	explicit hash_map(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty map from defaults, _STD allocator
	}

	hash_map(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._List._Getal().select_on_container_copy_construction())


	{	// construct map by copying _Right
	}

	hash_map(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct map by copying _Right, _STD allocator
	}

	explicit hash_map(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct empty map from comparator
	}

	hash_map(const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct empty map from comparator and _STD allocator
	}

	template<class _Iter>
	hash_map(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct map from sequence, defaults
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	hash_map(_Iter _First, _Iter _Last,
		const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct map from sequence, comparator
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	hash_map(_Iter _First, _Iter _Last,
		const key_compare& _Traits,
		const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct map from sequence, comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	hash_map(_Myt&& _Right)
		: _Mybase(_STD move(_Right), _Right._List._Getal())
	{	// construct map by moving _Right
	}

	hash_map(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
	{	// construct map by moving _Right, _STD allocator
	}

	_Myt& operator=(_Myt&& _Right)
	{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
	}

	mapped_type& operator[](key_type&& _Keyval)
	{	// find element matching _Keyval or insert with default mapped
		iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end())
			_Where = this->insert(
			_STD pair<key_type, mapped_type>(
			_STD move(_Keyval),
			mapped_type())).first;
		return (_Where->second);
	}

	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	hash_map(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_map(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_map(_XSTD initializer_list<value_type> _Ilist,
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

	//	void insert(_XSTD initializer_list<value_type> _Ilist)
	//		{	// insert initializer_list
	//		this->insert(_Ilist.begin(), _Ilist.end());
	//		}

	mapped_type& operator[](const key_type& _Keyval)
	{	// find element matching _Keyval or insert with default mapped
		iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end())
			_Where = this->insert(
			_STD pair<key_type, mapped_type>(
			_Keyval,
			mapped_type())).first;
		return (_Where->second);
	}

	mapped_type& at(const key_type& _Keyval)
	{	// find element matching _Keyval
		iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end())
			_STD _Xout_of_range("invalid hash_map<K, T> key");
		return (_Where->second);
	}

	const mapped_type& at(const key_type& _Keyval) const
	{	// find element matching _Keyval
		const_iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end())
			_STD _Xout_of_range("invalid hash_map<K, T> key");
		return (_Where->second);
	}
};

template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator==(
	const hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_map equality
	return (_STD _Hash_equal(_Left, _Right));
}

template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator!=(
	const hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_map inequality
	return (!(_Left == _Right));
}

// TEMPLATE CLASS hash_multimap
template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag = enXHashMemberTag::kStd,
class _Tr = hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class hash_multimap
	: public _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, true>, _XHashMemberTag>
{	// hash table of {key, mapped} values, non-unique keys
public:
	typedef hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc> _Myt;
	typedef _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, true>, _XHashMemberTag> _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;	// old name, magically gone
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

	hash_multimap()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty map from defaults
	}

	explicit hash_multimap(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty map from defaults, _STD allocator
	}

	hash_multimap(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._List._Getal().select_on_container_copy_construction())


	{	// construct map by copying _Right
	}

	hash_multimap(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct map by copying _Right, _STD allocator
	}

	explicit hash_multimap(const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct empty map from comparator
	}

	hash_multimap(const key_compare& _Traits, const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct empty map from comparator and _STD allocator
	}

	template<class _Iter>
	hash_multimap(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct map from sequence, defaults
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	hash_multimap(_Iter _First, _Iter _Last,
		const key_compare& _Traits)
		: _Mybase(_Traits, allocator_type())
	{	// construct map from sequence, comparator
		_Mybase::insert(_First, _Last);
	}
	template<class _Iter>
	hash_multimap(_Iter _First, _Iter _Last,
		const key_compare& _Traits,
		const allocator_type& _Al)
		: _Mybase(_Traits, _Al)
	{	// construct map from sequence, comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	hash_multimap(_Myt&& _Right)
		: _Mybase(_STD move(_Right), _Right._List._Getal())
	{	// construct map by moving _Right
	}

	hash_multimap(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
	{	// construct map by moving _Right, _STD allocator
	}

	_Myt& operator=(_Myt&& _Right)
	{	// assign by moving _Right
		_Mybase::operator=(_STD move(_Right));
		return (*this);
	}

	template<class _Valty>
	typename _STD enable_if<_STD is_convertible<_Valty, value_type>::value,
		iterator>::type
		insert(_Valty&& _Val)
	{	// insert a {key, mapped} value
		return (_Mybase::insert(_STD forward<_Valty>(_Val)).first);
	}

	template<class _Valty>
	typename _STD enable_if<_STD is_convertible<_Valty, value_type>::value,
		iterator>::type
		insert(const_iterator _Where, _Valty&& _Val)
	{	// insert a {key, mapped} value, with hint
		return (_Mybase::insert(_Where, _STD forward<_Valty>(_Val)));
	}

	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	hash_multimap(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_multimap(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	hash_multimap(_XSTD initializer_list<value_type> _Ilist,
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
	{	// insert a {key, mapped} value
		return (_Mybase::insert(_Val).first);
	}

	iterator insert(const_iterator _Where, const value_type& _Val)
	{	// insert a {key, mapped} value, with hint
		return (_Mybase::insert(_Where, _Val));
	}

	template<class _Iter>
	void insert(_Iter _First, _Iter _Last)
	{	// insert [_First, _Last), arbitrary iterators
		_Mybase::insert(_First, _Last);
	}
};

template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator==(
	const hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_multimap equality
	return (_STD _Hash_equal(_Left, _Right));
}

template<class _Kty,
class _Ty, enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	bool operator!=(
	const hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	const hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// test for hash_multimap inequality
	return (!(_Left == _Right));
}
_STDEXT_END
__RSTD_BEGIN_
using __RSTDEXT__ hash_map;
using __RSTDEXT__ hash_multimap;

template<class _Kty, class _Ty, enXHashMemberTag _XHashMemberTag, class _Tr, class _Alloc>
_STD hash_map<_Kty, _Ty, _Tr, _Alloc> to_std(
	const __RSTD__ hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& v){
	_STD hash_map<_Kty, _Ty, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, class _Tr, class _Alloc>
__RSTD__ hash_map<_Kty, _Ty, enXHashMemberTag::kStd, _Tr, _Alloc> from_std(
	const _STD hash_map<_Kty, _Ty, _Tr, _Alloc>& v){
	__RSTD__ hash_map<_Kty, _Ty, enXHashMemberTag::kStd, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, enXHashMemberTag _XHashMemberTag, class _Tr, class _Alloc>
_STD hash_multimap<_Kty, _Ty, _Tr, _Alloc> to_std(
	const __RSTD__ hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& v){
	_STD hash_multimap<_Kty, _Ty, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, class _Tr, class _Alloc>
__RSTD__ hash_multimap<_Kty, _Ty, enXHashMemberTag::kStd, _Tr, _Alloc> from_std(
	const _STD hash_multimap<_Kty, _Ty, _Tr, _Alloc>& v){
	__RSTD__ hash_multimap<_Kty, _Ty, enXHashMemberTag::kStd, _Tr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

_STD_END

_STD_BEGIN

template<class _Kty,
class _Ty, __RSTD__ enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	void swap(__RSTD__ hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	__RSTD__ hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// swap _Left and _Right hash_maps
	_Left.swap(_Right);
}

template<class _Kty,
class _Ty, __RSTD__ enXHashMemberTag _XHashMemberTag,
class _Tr,
class _Alloc> inline
	void swap(__RSTD__ hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Left,
	__RSTD__ hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>& _Right)
{	// swap _Left and _Right hash_multimaps
	_Left.swap(_Right);
}
_STDEXT_END

#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
