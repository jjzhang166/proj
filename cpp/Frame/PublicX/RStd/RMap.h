#pragma once
/*
@author zhp
@date 2017/2/27 7:20
@purpose for map release
*/
#ifndef RC_INVOKED
#include "RXtree.h"
#include <tuple>
#include <map>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

__RSTD_BEGIN_
// TEMPLATE CLASS _Tmap_traits
template<class _Kty,	// key type
class _Ty,	// mapped type
class _Pr,	// comparator predicate type
class _Alloc,	// actual _STD allocator type (should be value _STD allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
class _Tmap_traits
{	// traits required to make _Tree behave like a map
public:
	typedef _Kty key_type;
	typedef _STD pair<const _Kty, _Ty> value_type;
	typedef _Pr key_compare;
	typedef _Alloc allocator_type;

	enum
	{	// make multi parameter visible as an enum constant
		_Multi = _Mfl
	};

	class value_compare
		: public _STD binary_function<value_type, value_type, bool>
	{	// functor for comparing two element values
		friend class _Tmap_traits<_Kty, _Ty, _Pr, _Alloc, _Mfl>;

	public:
		bool operator()(const value_type& _Left,
			const value_type& _Right) const
		{	// test if _Left precedes _Right by comparing just keys
			return (comp(_Left.first, _Right.first));
		}

		value_compare(key_compare _Pred)
			: comp(_Pred)
		{	// construct with specified predicate
		}

	protected:
		key_compare comp;	// the comparator predicate for keys
	};

	template<class _Ty1,
	class _Ty2>
		static const _Kty& _Kfn(const _STD pair<_Ty1, _Ty2>& _Val)
	{	// extract key from element value
		return (_Val.first);
	}
};

// TEMPLATE CLASS map
template<class _Kty,
class _Ty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class map
	: public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> >
{	// ordered red-black tree of {key, mapped} values, unique keys
public:
	typedef map<_Kty, _Ty, _Pr, _Alloc> _Myt;
	typedef _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> > _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;	// retained
	typedef _Pr key_compare;
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

	map()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty map from defaults
	}

	explicit map(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty map from defaults, _STD allocator
	}

	map(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._Getal().select_on_container_copy_construction())


	{	// construct map by copying _Right
	}

	map(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct map by copying _Right, _STD allocator
	}

	explicit map(const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct empty map from comparator
	}

	map(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct empty map from comparator and _STD allocator
	}

	template<class _Iter>
	map(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct map from [_First, _Last), defaults
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	map(_Iter _First, _Iter _Last,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct map from [_First, _Last), comparator
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	map(_Iter _First, _Iter _Last,
		const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct map from [_First, _Last), comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	map(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
	{	// construct map by moving _Right
	}

	map(_Myt&& _Right, const allocator_type& _Al)
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
		if (_Where == this->end()
			|| this->_Getcomp()(_Keyval, this->_Key(_Where._Mynode())))

			_Where = this->emplace_hint(_Where,
			piecewise_construct,
			_STD forward_as_tuple(_STD move(_Keyval)),
			tuple<>());


		return (_Where->second);
	}

	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	map(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	map(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	map(_XSTD initializer_list<value_type> _Ilist,
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
		if (_Where == this->end()
			|| this->_Getcomp()(_Keyval, this->_Key(_Where._Mynode())))

			_Where = this->emplace_hint(_Where,
			piecewise_construct,
			_STD forward_as_tuple(_Keyval),
			tuple<>());


		return (_Where->second);
	}

	mapped_type& at(const key_type& _Keyval)
	{	// find element matching _Keyval
		iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end()
			|| this->_Getcomp()(_Keyval, this->_Key(_Where._Mynode())))
			_Xout_of_range("invalid map<K, T> key");
		return (_Where->second);
	}

	const mapped_type& at(const key_type& _Keyval) const
	{	// find element matching _Keyval
		const_iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end()
			|| this->_Getcomp()(_Keyval, this->_Key(_Where._Mynode())))
			_Xout_of_range("invalid map<K, T> key");
		return (_Where->second);
	}
};

// TEMPLATE CLASS multimap
template<class _Kty,
class _Ty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class multimap
	: public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, true> >
{	// ordered red-black tree of {key, mapped} values, non-unique keys
public:
	typedef multimap<_Kty, _Ty, _Pr, _Alloc> _Myt;
	typedef _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, true> > _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;	// retained
	typedef _Pr key_compare;
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

	multimap()
		: _Mybase(key_compare(), allocator_type())
	{	// construct empty map from defaults
	}

	explicit multimap(const allocator_type& _Al)
		: _Mybase(key_compare(), _Al)
	{	// construct empty map from defaults, _STD allocator
	}

	multimap(const _Myt& _Right)

		: _Mybase(_Right,
		_Right._Getal().select_on_container_copy_construction())


	{	// construct map by copying _Right
	}

	multimap(const _Myt& _Right, const allocator_type& _Al)
		: _Mybase(_Right, _Al)
	{	// construct map by copying _Right, _STD allocator
	}

	explicit multimap(const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct empty map from comparator
	}

	multimap(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct empty map from comparator and _STD allocator
	}

	template<class _Iter>
	multimap(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
	{	// construct map from [_First, _Last), defaults
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	multimap(_Iter _First, _Iter _Last,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct map from [_First, _Last), comparator
		_Mybase::insert(_First, _Last);
	}

	template<class _Iter>
	multimap(_Iter _First, _Iter _Last,
		const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
	{	// construct map from [_First, _Last), comparator, and _STD allocator
		_Mybase::insert(_First, _Last);
	}

	_Myt& operator=(const _Myt& _Right)
	{	// assign by copying _Right
		_Mybase::operator=(_Right);
		return (*this);
	}

	multimap(_Myt&& _Right)
		: _Mybase(_STD move(_Right))
	{	// construct map by moving _Right
	}

	multimap(_Myt&& _Right, const allocator_type& _Al)
		: _Mybase(_STD move(_Right), _Al)
	{	// construct map by moving _Right
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

	template<class... _Valty>
	iterator emplace(_Valty&&... _Val)
	{	// try to insert value_type(_Val...), favoring right side
		return (_Mybase::emplace(_STD forward<_Valty>(_Val)...).first);
	}


	void swap(_Myt& _Right)
	{	// exchange contents with non-movable _Right
		_Mybase::swap(_Right);
	}

	multimap(_XSTD initializer_list<value_type> _Ilist)
		: _Mybase(key_compare(), allocator_type())
	{	// construct from initializer_list, defaults
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	multimap(_XSTD initializer_list<value_type> _Ilist,
		const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
	{	// construct from initializer_list, comparator
		this->insert(_Ilist.begin(), _Ilist.end());
	}

	multimap(_XSTD initializer_list<value_type> _Ilist,
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

template<class _Kty, class _Ty, class _Pr, class _Alloc>
_STD map<_Kty, _Ty, _Pr, _Alloc> to_std(const __RSTD__ map<_Kty, _Ty, _Pr, _Alloc>& v){
	_STD map<_Kty, _Ty, _Pr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, class _Pr, class _Alloc>
__RSTD__ map<_Kty, _Ty, _Pr, _Alloc> from_std(const _STD map<_Kty, _Ty, _Pr, _Alloc>& v){
	__RSTD__ map<_Kty, _Ty, _Pr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, class _Pr, class _Alloc>
_STD multimap<_Kty, _Ty, _Pr, _Alloc> to_std(const __RSTD__ multimap<_Kty, _Ty, _Pr, _Alloc>& v){
	_STD multimap<_Kty, _Ty, _Pr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}

template<class _Kty, class _Ty, class _Pr, class _Alloc>
__RSTD__ multimap<_Kty, _Ty, _Pr, _Alloc> from_std(const _STD multimap<_Kty, _Ty, _Pr, _Alloc>& v){
	__RSTD__ multimap<_Kty, _Ty, _Pr, _Alloc> res;
	for (auto& va : v)
	{
		res.insert(va);
	}
	return res;
}
_STD_END

_STD_BEGIN
template<class _Kty,
class _Ty,
class _Pr,
class _Alloc> inline
	void swap(__RSTD__ multimap<_Kty, _Ty, _Pr, _Alloc>& _Left,
	__RSTD__ multimap<_Kty, _Ty, _Pr, _Alloc>& _Right)
{	// swap _Left and _Right multimaps
	_Left.swap(_Right);
}

template<class _Kty,
class _Ty,
class _Pr,
class _Alloc> inline
	void swap(__RSTD__ map<_Kty, _Ty, _Pr, _Alloc>& _Left,
	__RSTD__ map<_Kty, _Ty, _Pr, _Alloc>& _Right)
{	// swap _Left and _Right maps
	_Left.swap(_Right);
}
_STD_END
#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
