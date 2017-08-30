#pragma once
/*
@author zhp
@date 2017/2/27 17:54
@purpose for game struct
*/
#include <ProtocolX/P_DefinesX.h>
#include <RStd/RVector.h>
#include <RStd/RList.h>
#include <RStd/RBitset.h>
#include <RStd/RString.h>
#include <RStd/RSet.h>
#include <RStd/RMap.h>
#include <RStd/RHash_set.h>
#include <RStd/RHash_map.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include "../DependSelfLib.h"

template<class _Ty,
class _Alloc = _STD allocator<_Ty> >
class GameVector : public RStd::vector<_Ty, _Alloc>{
public:
	inline bool IsValid() const{
		if (_Myfirst == _Mylast)
		{
			if (_Mylast == 0)
				return true;
		}
		if (IsUnValidPtr(_Myfirst))
			return false;
		if (IsUnValidPtr(_Mylast))
			return false;
		if (_Myfirst > _Mylast)
			return false;
		if (((int)_Mylast - (int)_Myfirst) % sizeof(value_type) != 0)
			return false;
		if (_Myend < _Mylast)
			return false;
		return true;
	}
	friend std::ostream& operator << (std::ostream& ostm, const GameVector& v){
		if (!v.IsValid())
		{
			assert(false);
			return ostm;
		}
		for (auto& tmp : v)
			ostm << tmp;
		return ostm;
	}
};
static_assert(4 * 3 == sizeof(GameVector<int>), "");

template<typename Value_>
struct GameArray{
	typedef Value_ value_type;
	typedef value_type* pointer_type;

	inline bool IsValid() const{
		if (cnt_ < 0)
		{
			assert(false);
			return false;
		}
		if (cnt_ == 0)
			return true;
		if (IsUnValidPtr(ptr_))
		{
			assert(false);
			return false;
		}
		return true;
	}
	inline pt_dword size() const{ return (pt_dword)cnt_; }
	inline value_type& operator[](int idx){
		auto res = ptr_ + idx;
		return *res;
	}
	inline const value_type& operator[](int idx) const{
		auto res = ptr_ + idx;
		return *res;
	}
	inline pointer_type begin(){
		return ptr_;
	}
	inline const pointer_type begin() const{
		return ptr_;
	}
	inline pointer_type end(){
		return ptr_ + size();
	}
	inline const pointer_type end() const{
		return ptr_ + size();
	}
	inline bool empty() const{
		return cnt_ == 0;
	}
	friend std::ostream& operator << (std::ostream& ostm, const GameArray& v){
		if (!v.IsValid())
		{
			assert(false);
			return ostm;
		}
		for (auto& tmp : v)
			ostm << tmp;
		return ostm;
	}

private:
	int				cnt_;
	pointer_type	ptr_;
};
static_assert(4 * 2 == sizeof(GameArray<int>), "");

template<typename Value_>
struct GameArrayEx : GameArray<Value_>{
	pointer_type		buffer_end_;
};
static_assert(4 * 3 == sizeof(GameArrayEx<int>), "");

template<class _Ty,
class _Alloc = _STD allocator<_Ty> >
class GameList : public RStd::list<_Ty, _Alloc>{
private:
	GameList() = delete;
	GameList(const GameList&) = delete;
	GameList& operator = (const GameList&) = delete;
	static_assert(8 + sizeof(value_type) == sizeof(_Node), "");
	static_assert(0 == offsetof(_Node, _Next), "");
	static_assert(4 == offsetof(_Node, _Prev), "");
	static_assert(8 == offsetof(_Node, _Myval), "");

public:
	inline bool IsValid() const{
		if (_Mysize < 0)
			return false;
		if (0 == _Mysize)
			return true;
		if (IsUnValidPtr(_Myhead))
			return false;
		return true;
	}
};
static_assert(4 * 2 == sizeof(GameList<int>), "");

template<int TypeSize>
struct GameBitsetTraits{
	static_assert(TypeSize > 0, "");
	typedef typename std::_If<TypeSize <= sizeof(pt_dword), pt_dword, pt_qword>::type _Ty;
	enum{
		kBitsPerWord = 8 * sizeof(_Ty),
		kWords = TypeSize / sizeof(_Ty) - 1,
		kBits = kWords * kBitsPerWord + 1
	};
	static_assert(kWords >= 0, "");
};

template<int TypeSize>
struct GameBitset : RStd::bitset<GameBitsetTraits<TypeSize>::kBits>{
	enum{
		kBits = GameBitsetTraits<TypeSize>::kBits
	};
	static_assert(TypeSize == sizeof(std::bitset<kBits>), "");
};

template<class _Elem, 
class _Traits = _STD char_traits<_Elem>,
class _Alloc = _STD allocator<_Elem> >
class GameStringT : public RStd::basic_string<_Elem, _Traits, _Alloc>{
public:
	typedef RStd::basic_string<_Elem, _Traits, _Alloc> BasicString;

	iterator begin(){
		assert(_Mysize >= 0);
		if (_Myres < _Mysize)
		{
			assert(false);
			return __super::end();
		}
		if (_Mysize >= _BUF_SIZE)
		{
			if (IsUnValidPtr(_Bx._Ptr))
			{
				assert(false);
				return __super::end();
			}
		}
		return __super::begin();
	}
	const_iterator begin() const{
		assert(_Mysize >= 0);
		if (_Myres < _Mysize)
		{
			assert(false);
			return __super::end();
		}
		if (_Mysize >= _BUF_SIZE)
		{
			if (IsUnValidPtr(_Bx._Ptr))
			{
				assert(false);
				return __super::end();
			}
		}
		return __super::begin();
	}
	const value_type* c_str() const{
		assert(_Mysize >= 0);
		if (_Myres < _Mysize)
		{
			assert(false);
			return nullptr;
		}
		if (_Mysize >= _BUF_SIZE)
		{
			if (IsUnValidPtr(_Bx._Ptr))
			{
				assert(false);
				return nullptr;
			}
		}
		return _Myptr();
	}
	bool FindStr(const value_type* str) const{
		return !!StdEx::TStrStr(c_str(), str);
	}
	bool FindIStr(const value_type* str) const{
		return !!StdEx::TStristr(c_str(), str);
	}
	friend std::ostream& operator << (std::ostream& ostm, const GameStringT& v){
		auto str = v.c_str();
		if (!str)
		{
			assert(false);
			return ostm;
		}
		ostm << str;
		return ostm;
	}
};

struct GameWString : GameStringT<wchar_t>{
	std::wstring GetStdStr() const;
	std::string GetStdString() const;
	friend std::ostream& operator << (std::ostream& ostm, const GameWString& v);
};
static_assert(16 + 4 * 2 == sizeof(GameWString), "");

struct GameString : GameStringT<char>{
	std::string GetStdString() const;
};
static_assert(16 + 4 * 2 == sizeof(GameString), "");

namespace RStd{
	template<class _Elem,
	class _Traits,
	class _Alloc> inline
		_STD size_t hash_value(const GameStringT<_Elem, _Traits, _Alloc>& str)
	{
		const basic_string<_Elem, _Traits, _Alloc>& str_base = str;
		return hash_value(str_base);
	}
}

template<class _Kty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_Kty> >
class GameSet : public RStd::set<_Kty, _Pr, _Alloc>{
	static_assert(0xc == offsetof(_Node, _Color), "");
	static_assert(0x10 == offsetof(_Node, _Myval), "");
	static_assert(0x10 + sizeof(value_type) == sizeof(_Node), "");

public:
	inline bool IsValid() const{
		if (_Mysize <= 0)
			return true;
		if (IsUnValidPtr(_Myhead))
			return false;
		return true;
	}
};
static_assert(4 * 2 == sizeof(GameSet<int>), "");

template<class _Kty, class _Ty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_Kty> >
class GameMap : public RStd::map<_Kty, _Ty, _Pr, _Alloc>{
	static_assert(0xc == offsetof(_Node, _Color), "");
	static_assert(0x10 == offsetof(_Node, _Myval), "");
	static_assert(0x10 + sizeof(value_type) == sizeof(_Node), "");
	static_assert(sizeof(value_type) >= sizeof(key_type) + sizeof(mapped_type), "");

public:
	inline bool IsValid() const{
		if (_Mysize <= 0)
			return true;
		if (IsUnValidPtr(_Myhead))
			return false;
		return true;
	}
};
static_assert(4 * 2 == sizeof(GameMap<bool, int>), "");

template<class _Kty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_Kty> >
class GameMultiset : public RStd::multiset<_Kty, _Pr, _Alloc>{
	static_assert(0xc == offsetof(_Node, _Color), "");
	static_assert(0x10 == offsetof(_Node, _Myval), "");
	static_assert(0x10 + sizeof(value_type) == sizeof(_Node), "");

public:
	inline bool IsValid() const{
		if (_Mysize <= 0)
			return true;
		if (IsUnValidPtr(_Myhead))
			return false;
		return true;
	}
};
static_assert(4 * 2 == sizeof(GameMultiset<int>), "");

template<class _Kty, class _Ty,
class _Pr = _STD less<_Kty>,
class _Alloc = _STD allocator<_Kty> >
class GameMultimap : public RStd::multimap<_Kty, _Ty, _Pr, _Alloc>{
	static_assert(0xc == offsetof(_Node, _Color), "");
	static_assert(0x10 == offsetof(_Node, _Myval), "");
	static_assert(0x10 + sizeof(value_type) == sizeof(_Node), "");
	static_assert(sizeof(value_type) >= sizeof(key_type) + sizeof(mapped_type), "");

public:
	inline bool IsValid() const{
		if (_Mysize <= 0)
			return true;
		if (IsUnValidPtr(_Myhead))
			return false;
		return true;
	}
};
static_assert(4 * 2 == sizeof(GameMultimap<bool, int>), "");

template<class _Kty, RStd::enXHashMemberTag _XHashMemberTag = RStd::enXHashMemberTag::kStd,
class _Tr = RStd::hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_Kty> >
class GameHash_set : public RStd::hash_set<_Kty, _XHashMemberTag, _Tr, _Alloc>{
public:
	typedef typename _Myvec::value_type VecValueType;
	typedef typename _Mylist::_Node _Node;
	static_assert(4 == sizeof(VecValueType), "");
	static_assert((sizeof(void*) * 2 + sizeof(value_type)) / sizeof(_Node) <= 1, "");
	static_assert(std::is_same<_Unchecked_iterator, VecValueType>::value, "");
	static_assert(std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_iterator>::value
		|| std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_const_iterator>::value, "");
	static_assert(std::is_same<const_iterator, typename _Mylist::const_iterator>::value, "");

public:
	using _Myt::_List;
	using _Myt::_Vec;
	using _Myt::_Mask;
	using _Myt::_Maxidx;
	using _Myt::_Max_bucket_size;		//0x3F800000==0.1，标准库这个值默认为0.1
};
static_assert(0 == offsetof(GameHash_set<int>, _List), "");
static_assert(8 == offsetof(GameHash_set<int>, _Vec), "");
static_assert(20 == offsetof(GameHash_set<int>, _Mask), "");
static_assert(24 == offsetof(GameHash_set<int>, _Maxidx), "");
static_assert(28 == offsetof(GameHash_set<int>, _Max_bucket_size), "");
static_assert(32 == sizeof(GameHash_set<int>), "");

template<class _Kty,
class _Ty, RStd::enXHashMemberTag _XHashMemberTag = RStd::enXHashMemberTag::kStd,
class _Tr = RStd::hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class GameHash_map : public RStd::hash_map<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>{
public:
	typedef typename _Myvec::value_type VecValueType;
	typedef typename _Mylist::_Node _Node;
	static_assert(4 == sizeof(VecValueType), "");
	static_assert((sizeof(void*) * 2 + sizeof(value_type)) / sizeof(_Node) <= 1, "");
	static_assert(std::is_same<_Unchecked_iterator, VecValueType>::value, "");
	static_assert(std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_iterator>::value
		|| std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_const_iterator>::value, "");
	static_assert(sizeof(value_type) >= sizeof(key_type) + sizeof(mapped_type), "");
	static_assert(std::is_same<const_iterator, typename _Mylist::const_iterator>::value, "");

public:
	using _Myt::_List;
	using _Myt::_Vec;
	using _Myt::_Mask;
	using _Myt::_Maxidx;
	using _Myt::_Max_bucket_size;		//0x3F800000==0.1，标准库这个值默认为0.1
};
static_assert(32 == sizeof(GameHash_map<int, int>), "");

template<class _Kty, RStd::enXHashMemberTag _XHashMemberTag = RStd::enXHashMemberTag::kStd,
class _Tr = RStd::hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_Kty> >
class GameHash_multiset : public RStd::hash_multiset<_Kty, _XHashMemberTag, _Tr, _Alloc>{
public:
	typedef typename _Myvec::value_type VecValueType;
	typedef typename _Mylist::_Node _Node;
	static_assert(4 == sizeof(VecValueType), "");
	static_assert((sizeof(void*) * 2 + sizeof(value_type)) / sizeof(_Node) <= 1, "");
	static_assert(std::is_same<_Unchecked_iterator, VecValueType>::value, "");
	static_assert(std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_iterator>::value
		|| std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_const_iterator>::value, "");
	static_assert(std::is_same<const_iterator, typename _Mylist::const_iterator>::value, "");

public:
	using _Myt::_List;
	using _Myt::_Vec;
	using _Myt::_Mask;
	using _Myt::_Maxidx;
	using _Myt::_Max_bucket_size;		//0x3F800000==0.1，标准库这个值默认为0.1
};
static_assert(0 == offsetof(GameHash_multiset<int>, _List), "");
static_assert(8 == offsetof(GameHash_multiset<int>, _Vec), "");
static_assert(20 == offsetof(GameHash_multiset<int>, _Mask), "");
static_assert(24 == offsetof(GameHash_multiset<int>, _Maxidx), "");
static_assert(28 == offsetof(GameHash_multiset<int>, _Max_bucket_size), "");
static_assert(32 == sizeof(GameHash_multiset<int>), "");

template<class _Kty,
class _Ty, RStd::enXHashMemberTag _XHashMemberTag = RStd::enXHashMemberTag::kStd,
class _Tr = RStd::hash_compare<_Kty, _STD less<_Kty> >,
class _Alloc = _STD allocator<_STD pair<const _Kty, _Ty> > >
class GameHash_multimap : public RStd::hash_multimap<_Kty, _Ty, _XHashMemberTag, _Tr, _Alloc>{
public:
	typedef typename _Myvec::value_type VecValueType;
	typedef typename _Mylist::_Node _Node;
	static_assert(4 == sizeof(VecValueType), "");
	static_assert((sizeof(void*) * 2 + sizeof(value_type)) / sizeof(_Node) <= 1, "");
	static_assert(std::is_same<_Unchecked_iterator, VecValueType>::value, "");
	static_assert(std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_iterator>::value
		|| std::is_same<_Unchecked_iterator, typename _Mylist::_Unchecked_const_iterator>::value, "");
	static_assert(sizeof(value_type) >= sizeof(key_type) + sizeof(mapped_type), "");
	static_assert(std::is_same<const_iterator, typename _Mylist::const_iterator>::value, "");

public:
	using _Myt::_List;
	using _Myt::_Vec;
	using _Myt::_Mask;
	using _Myt::_Maxidx;
	using _Myt::_Max_bucket_size;		//0x3F800000==0.1，标准库这个值默认为0.1
};
static_assert(32 == sizeof(GameHash_multimap<int, int>), "");
