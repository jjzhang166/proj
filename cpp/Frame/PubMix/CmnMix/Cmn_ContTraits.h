#pragma once
/*
@author zhp
@date 2016/4/21 8:03
*/
#include <queue>
#include <stack>
#include <hash_map>
#include <hash_set>
#include <map>
#include <set>


template<typename ContT>
struct ContTraits;

template<typename ContT>
struct ContAddValue;
//////////////////////////////////////////////////////////////////////////
template<typename ContT>
struct ContAddValue{
	typedef ContT cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.push_back(v);
	}
};

template<typename T_, typename ContT>
struct ContAddValue<std::queue<T_, ContT> >{
	typedef std::queue<T_, ContT> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.push(v);
	}
};

template<typename T_, typename ContT>
struct ContAddValue<std::stack<T_, ContT> >{
	typedef std::stack<T_, ContT> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.push(v);
	}
};

template < 
	class Type,  
	class Container, 
	class Compare> 
struct ContAddValue<std::priority_queue<Type, Container, Compare> >{
	typedef std::priority_queue<Type, Container, Compare> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.push(v);
	}
};

template <
	class Key,
	class Type,
	class Traits,  
	class Allocator>
struct ContAddValue<std::hash_map<Key, Type, Traits, Allocator> >{
	typedef std::hash_map<Key, Type, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Type,
	class Traits,  
	class Allocator>
struct ContAddValue<std::hash_multimap<Key, Type, Traits, Allocator> >{
	typedef std::hash_multimap<Key, Type, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Traits,  
	class Allocator>
struct ContAddValue<std::hash_multiset<Key, Traits, Allocator> >{
	typedef std::hash_multiset<Key, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Traits,  
	class Allocator>
struct ContAddValue<std::hash_set<Key, Traits, Allocator> >{
	typedef std::hash_set<Key, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Type,
	class Traits,  
	class Allocator>
struct ContAddValue<std::map<Key, Type, Traits, Allocator> >{
	typedef std::map<Key, Type, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Type,
	class Traits,  
	class Allocator>
struct ContAddValue<std::multimap<Key, Type, Traits, Allocator> >{
	typedef std::multimap<Key, Type, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Traits,  
	class Allocator>
struct ContAddValue<std::multiset<Key, Traits, Allocator> >{
	typedef std::multiset<Key, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};

template <
	class Key,
	class Traits,  
	class Allocator>
struct ContAddValue<std::set<Key, Traits, Allocator> >{
	typedef std::set<Key, Traits, Allocator> cont_type;

	template<typename T>
	static void Add( cont_type& cont, const T& v ){
		cont.insert(v);
	}
};
//////////////////////////////////////////////////////////////////////////
