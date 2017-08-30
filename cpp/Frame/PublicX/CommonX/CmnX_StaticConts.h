#pragma once
/*
@author zhp
@date 2016/1/31 19:18
@purpose 静态容器
*/
#include <config/targetver.h>
#include <config/AdjustWin32.h>
#include <array>
#include <cassert>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4996)

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
template<typename T_, unsigned int ContSize_>
class CmnStaticContBase{
private:
	typedef char(TypeWrapperT_)[sizeof(T_)];

public:
	static const unsigned int MaxSize = ContSize_;

public:
	typedef T_ ValueTypeT;
	typedef unsigned int size_type;
	typedef size_type IndexT;
	typedef std::array<TypeWrapperT_, MaxSize> InnerContT;
	//////////////////////////////////////////////////////////////////////////
	//std typedefs
	typedef typename InnerContT::value_type value_type;
	typedef typename InnerContT::difference_type difference_type;
	typedef typename InnerContT::reference reference;
	typedef typename InnerContT::const_reference const_reference;
	typedef typename InnerContT::iterator iterator;
	typedef typename InnerContT::const_iterator const_iterator;
	//////////////////////////////////////////////////////////////////////////

public:
	size_type max_size() const{
		return ContSize_;
	}

protected:
	void Construct(IndexT idx, const ValueTypeT& v){
		assert(idx < MaxSize);
		new ((ValueTypeT*)&cont_[idx]) ValueTypeT(v);
	}
	void Destroy(IndexT idx){
		assert(idx < MaxSize);
		((ValueTypeT*)&cont_[idx])->~ValueTypeT();
	}
	void Construct(ValueTypeT* ptr, const ValueTypeT& v){
		assert(ptr >= (ValueTypeT*)(void*)&cont_[0] && ptr < (ValueTypeT*)(void*)&cont_[0] + MaxSize);
		new (ptr) ValueTypeT(v);
	}
	void Destroy(ValueTypeT* ptr){
		assert(ptr >= (ValueTypeT*)(void*)&cont_[0] && ptr < (ValueTypeT*)(void*)&cont_[0] + MaxSize);
		ptr->~ValueTypeT();
	}

public:
	InnerContT	cont_;
};
//////////////////////////////////////////////////////////////////////////
template<typename T_, unsigned int ContSize_>
class CmnStaticVector : public CmnStaticContBase<T_, ContSize_>{
public:
	typedef T_ value_type;
	typedef unsigned int size_type;
	typedef int difference_type;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef value_type& reference;
	typedef const reference const_reference;
	typedef pointer iterator;
	typedef const_pointer const_iterator;

public:
	CmnStaticVector(){
		begin_ = (pointer)(void*)&cont_[0];
		end_ = begin_;
	}
	CmnStaticVector(const CmnStaticVector& rhs) : CmnStaticVector(){
		for (auto& v : rhs)
			push_back(v);
	}
	explicit CmnStaticVector(size_type size) : CmnStaticVector(){
		resize(size);
	}
	CmnStaticVector(std::initializer_list<value_type> init_list) : CmnStaticVector(){
		for (auto& v : init_list)
			push_back(v);
	}
	CmnStaticVector& operator = (const CmnStaticVector& rhs){
		clear();
		for (auto& v : rhs)
			push_back(v);
		return *this;
	}
	~CmnStaticVector(){
		clear();
	}
	iterator begin(){
		return begin_;
	}
	iterator end(){
		return end_;
	}
	iterator begin() const{
		return begin_;
	}
	iterator end() const{
		return end_;
	}
	void push_back(const value_type& v){
		if (end_ - begin_ >= MaxSize)
		{
			assert(false);
			return;
		}
		Construct(end_, v);
		++end_;
	}
	void pop_back(){
		if (begin_ == end_)
		{
			assert(false);
			return;
		}
		--end_;
		Destroy(end_);
	}
	void pop_front(){
		if (begin_ == end_)
		{
			assert(false);
			return;
		}
		erase(begin_);
	}
	void swap(CmnStaticVector& rhs){
		CmnStaticVector vec(*this);
		*this = rhs;
		rhs = vec;
	}
	size_type size() const{
		return end_ - begin_;
	}
	bool empty() const{
		return begin_ == end_;
	}
	const_reference operator[](size_type pos) const{
		assert(size() > pos);
		return *(begin_ + pos);
	}
	reference operator[](size_type pos){
		assert(size() > pos);
		return *(begin_ + pos);
	}
	reference front(){
		return *begin();
	}
	const_reference front() const{
		return (*begin());
	}
	reference back(){
		return (*(end() - 1));
	}
	const_reference back() const{
		return (*(end() - 1));
	}
	void clear(){
		while (!empty())
			pop_back();
	}
	void resize(size_type new_size){
		if (new_size > MaxSize)
		{
			assert(false);
			return;
		}
		auto cur_size = size();
		if (new_size > cur_size)
		{
			new_size -= cur_size;
			while (new_size > 0)
			{
				push_back(value_type());
				--new_size;
			}
		}
		else
		{
			cur_size -= new_size;
			while (cur_size > 0)
			{
				pop_back();
				--cur_size;
			}
		}
	}
	void reserve(size_type new_size){}
	void erase(iterator it){
		if (it >= begin_ && it < end_)
		{
			Destroy(it);
			std::move(it + 1, end_, it);
			--end_;
		}
		else
			assert(false);
	}
	void erase(iterator it, iterator it_end){
		if (it >= it_end)
		{
			assert(it == it_end);
			return;
		}
		if (it < begin_ || it >= end_)
		{
			assert(false);
			return;
		}
		if (it_end < begin_ || it_end > end_)
		{
			assert(false);
			return;
		}
		auto it_old = it;
		while (it != it_end)
		{
			Destroy(it);
			++it;
		}		
		std::move(it_end, end_, it_old);
		end_ -= it_end - it_old;
	}
	bool is_full() const{
		return size() == MaxSize;
	}

public:
	pointer		begin_;
	pointer		end_;
};
//////////////////////////////////////////////////////////////////////////
template<typename T_, unsigned int ContSize_>
class CmnBufferVector{
public:
	typedef CmnStaticVector<T_, ContSize_> StaticVector;
	typedef typename StaticVector::value_type value_type;
	typedef typename StaticVector::size_type size_type;
	typedef typename StaticVector::difference_type difference_type;
	typedef typename StaticVector::reference reference;
	typedef typename StaticVector::const_reference const_reference;
	typedef typename StaticVector::pointer pointer;
	typedef typename StaticVector::const_pointer const_pointer;

public:
	struct MyIterator_ : std::_Iterator012<std::random_access_iterator_tag,
		value_type,
		difference_type,
		pointer,
		reference,
		std::_Iterator_base>{
		MyIterator_(const MyIterator_& rhs){
			the_cont_ = rhs.the_cont_;
			cur_idx_ = rhs.cur_idx_;
		}
		MyIterator_(const CmnBufferVector* cont){
			assert(cont);
			the_cont_ = (CmnBufferVector*)cont;
			cur_idx_ = cont->size();
		}
		MyIterator_(const CmnBufferVector* cont, size_type cur_idx) : the_cont_((CmnBufferVector*)cont), cur_idx_(cur_idx){
			assert(cont);
		}
		MyIterator_& operator = (const MyIterator_& rhs){
			the_cont_ = rhs.the_cont_;
			cur_idx_ = rhs.cur_idx_;
			return *this;
		}
		const_reference operator*() const{
			assert(the_cont_);
			if (cur_idx_ < the_cont_->static_vector_.max_size()) return the_cont_->static_vector_[cur_idx_];
			return the_cont_->vector_[cur_idx_ - the_cont_->static_vector_.max_size()];
		}
		reference operator*(){
			assert(the_cont_);
			if (cur_idx_ < the_cont_->static_vector_.max_size()) return the_cont_->static_vector_[cur_idx_];
			return the_cont_->vector_[cur_idx_ - the_cont_->static_vector_.max_size()];
		}
		const_pointer operator->() const{
			return (std::pointer_traits<const_pointer>::pointer_to(**this));
		}
		pointer operator->(){
			return (std::pointer_traits<pointer>::pointer_to(**this));
		}
		MyIterator_& operator++(){
			assert(the_cont_);
			++cur_idx_;
			return (*this);
		}
		MyIterator_ operator++(int){
			auto _Tmp = *this;
			++*this;
			return (_Tmp);
		}
		MyIterator_& operator--(){
			--cur_idx_;
			return (*this);
		}
		MyIterator_ operator--(int){
			auto _Tmp = *this;
			--*this;
			return (_Tmp);
		}
		MyIterator_& operator+=(difference_type off){
			cur_idx_ += off;
			return (*this);
		}
		MyIterator_ operator+(difference_type off) const{
			auto _Tmp = *this;
			return (_Tmp += off);
		}
		MyIterator_& operator-=(difference_type off){
			return (*this += -off);
		}
		MyIterator_ operator-(difference_type off) const{
			auto _Tmp = *this;
			return (_Tmp -= off);
		}
		difference_type operator - (const MyIterator_& rhs) const{
			return cur_idx_ - rhs.cur_idx_;
		}
		reference operator[](difference_type off) const{
			return (*(*this + off));
		}
		bool operator==(const MyIterator_& rhs) const{
			return cur_idx_ == rhs.cur_idx_;
		}
		bool operator!=(const MyIterator_& rhs) const{
			return (!(*this == rhs));
		}
		bool operator<(const MyIterator_& rhs) const{
			return cur_idx_ < rhs.cur_idx_;
		}
		bool operator>(const MyIterator_& rhs) const{
			return (rhs < *this);
		}
		bool operator<=(const MyIterator_& rhs) const{
			return (!(rhs < *this));
		}
		bool operator>=(const MyIterator_& rhs) const{
			return (!(*this < rhs));
		}

		CmnBufferVector*	the_cont_;
		size_type			cur_idx_;
	};

public:
	typedef MyIterator_ iterator;
	typedef MyIterator_ const_iterator;

public:
	CmnBufferVector(){}
	CmnBufferVector(const CmnBufferVector& rhs){
		for (auto& v : rhs)
			push_back(v);
	}
	explicit CmnBufferVector(size_type size){
		resize(size);
	}
	CmnBufferVector(std::initializer_list<value_type> init_list){
		for (auto& v : init_list)
			push_back(v);
	}
	CmnBufferVector& operator = (const CmnBufferVector& rhs){
		clear();
		for (auto& v : rhs)
			push_back(v);
		return *this;
	}
	iterator begin(){
		return iterator(this, 0);
	}
	iterator end(){
		return iterator(this);
	}
	iterator begin() const{
		return iterator(this, 0);
	}
	iterator end() const{
		return iterator(this);
	}
	void push_back(const value_type& v){
		if (static_vector_.is_full())
			vector_.push_back(v);
		else
			static_vector_.push_back(v);
	}
	void pop_back(){
		if (vector_.empty())
			static_vector_.pop_back();
		else
			vector_.pop_back();
	}
	void pop_front(){
		static_vector_.pop_front();
		if (vector_.empty())
			return;
		static_vector_.push_back(vector_.front());
		vector_.erase(vector_.begin());
	}
	void swap(CmnBufferVector& rhs){
		CmnBufferVector vec(*this);
		*this = rhs;
		rhs = vec;
	}
	size_type size() const{
		return static_vector_.size() + vector_.size();
	}
	bool empty() const{
		return static_vector_.empty();
	}
	const_reference operator[](size_type pos) const{
		return *(begin() + pos);
	}
	reference operator[](size_type pos){
		return *(begin() + pos);
	}
	reference front(){
		return *begin();
	}
	const_reference front() const{
		return (*begin());
	}
	reference back(){
		return (*(end() - 1));
	}
	const_reference back() const{
		return (*(end() - 1));
	}
	void clear(){
		static_vector_.clear();
		vector_.clear();
	}
	void resize(size_type new_size){
		if (static_vector_.max_size() >= new_size)
			return;
		new_size -= static_vector_.max_size();
		vector_.resize(new_size);
	}
	void reserve(size_type new_size){
		if (static_vector_.max_size() >= new_size)
			return;
		new_size -= static_vector_.max_size();
		vector_.reserve(new_size);
	}
	void erase(const iterator& it){
		if (!it.the_cont_)
		{
			assert(false);
			return;
		}
		if (it.cur_idx_ < static_vector_.max_size())
		{
			static_vector_.erase(static_vector_.begin() + it.cur_idx_);
			if (vector_.empty())
				return;
			static_vector_.push_back(vector_.front());
			vector_.erase(vector_.begin());
			return;
		}
		vector_.erase(vector_.begin() + (it.cur_idx_ - static_vector_.max_size()));
	}
	void erase(const iterator& it, const iterator& it_end){
		auto it_idx = std::min(it.cur_idx_, static_vector_.max_size());
		auto it_end_idx = std::min(it_end.cur_idx_, static_vector_.max_size());
		auto it_static = static_vector_.begin();
		static_vector_.erase(it_static + it_idx, it_static + it_end_idx);
		auto it_begin = vector_.begin();
		if (it_end.cur_idx_ > static_vector_.max_size())
			vector_.erase(it_begin, it_begin + (it_end.cur_idx_ - static_vector_.max_size()));
		int cnt = 0;
		for (auto& v : vector_)
		{
			if (static_vector_.is_full())
				break;
			static_vector_.push_back(v);
			++cnt;
		}
		if (cnt > 0)
			vector_.erase(it_begin, it_begin + cnt);
	}
	bool is_full() const{
		return static_vector_.is_full();
	}

private:
	StaticVector				static_vector_;
	std::vector<value_type>		vector_;
};
//////////////////////////////////////////////////////////////////////////
template<typename T_, unsigned int ContSize_>
class CmnStaticQueueBase_ : public CmnStaticContBase<T_, ContSize_>{
public:
	IndexT		first_index_;
	size_type	cur_size_;
};

template<typename T_, unsigned int Size_, typename PtrT_, typename RefT_>
class CmnStaticQueue_ConstIterator : public std::_Iterator012<std::random_access_iterator_tag,
	T_,
	int,
	PtrT_,
	RefT_,
	std::_Iterator_base>{
public:
	typedef CmnStaticQueue_ConstIterator<T_, Size_, PtrT_, RefT_> _Myiter;
	typedef CmnStaticQueueBase_<T_, Size_> TheContT_;

	typedef std::random_access_iterator_tag iterator_category;
	typedef T_ value_type;
	typedef unsigned int size_type;
	typedef int difference_type;
	typedef PtrT_ pointer;
	typedef RefT_ reference;

public:
	CmnStaticQueue_ConstIterator(const TheContT_* cont, size_type cur_idx) : the_cont_(cont), cur_idx_(cur_idx){}
	CmnStaticQueue_ConstIterator(const _Myiter& rhs) : the_cont_(rhs.the_cont_), cur_idx_(rhs.cur_idx_){}
	_Myiter& operator = (const _Myiter& rhs){
		the_cont_ = rhs.the_cont_;
		cur_idx_ = rhs.cur_idx_;
		return *this;
	}
	reference operator*() const{
		assert(the_cont_);
		return *(value_type*)&the_cont_->cont_[cur_idx_ % TheContT_::MaxSize];
	}
	pointer operator->() const{
		return (std::pointer_traits<pointer>::pointer_to(**this));
	}
	_Myiter& operator++(){
		assert(the_cont_);
		if ( CurIdxFromFristItem() >= the_cont_->cur_size_ - 1 )
		{
			the_cont_ = nullptr;
			return (*this);
		}
		++cur_idx_;
		return (*this);
	}
	_Myiter operator++(int){
		auto _Tmp = *this;
		++*this;
		return (_Tmp);
	}
	_Myiter& operator--(){
		--cur_idx_;
		return (*this);
	}
	_Myiter operator--(int){	
		auto _Tmp = *this;
		--*this;
		return (_Tmp);
	}
	_Myiter& operator+=(difference_type off){
		cur_idx_ += off;
		return (*this);
	}
	_Myiter operator+(difference_type off) const{
		auto _Tmp = *this;
		return (_Tmp += off);
	}
	_Myiter& operator-=(difference_type off){
		return (*this += -off);
	}
	_Myiter operator-(difference_type off) const{
		auto _Tmp = *this;
		return (_Tmp -= off);
	}
	reference operator[](difference_type off) const{
		return (*(*this + off));
	}
	bool operator==(const _Myiter& rhs) const{
		return CurIdxFromFristItem() == rhs.CurIdxFromFristItem();
	}
	bool operator!=(const _Myiter& rhs) const{
		return (!(*this == rhs));
	}
	bool operator<(const _Myiter& rhs) const{
		return CurIdxFromFristItem() < rhs.CurIdxFromFristItem();
	}
	bool operator>(const _Myiter& rhs) const{
		return (rhs < *this);
	}
	bool operator<=(const _Myiter& rhs) const{
		return (!(rhs < *this));
	}
	bool operator>=(const _Myiter& rhs) const{
		return (!(*this < rhs));
	}
	//从第一个元素开始的索引
	size_type CurIdxFromFristItem() const{
		if ( !the_cont_ )
			return TheContT_::MaxSize;
		auto the_idx = cur_idx_ % TheContT_::MaxSize;
		auto the_first_idx = the_cont_->first_index_ % TheContT_::MaxSize;
		int res = (int)the_idx - the_first_idx;
		if ( res >= 0 )
			return (size_type)res;
		return (size_type)(the_cont_->cur_size_ + res);
	}

private:
	const TheContT_*	the_cont_;
	size_type			cur_idx_;
};

template<typename T_, unsigned int ContSize_>
class CmnStaticQueue : public CmnStaticQueueBase_<T_, ContSize_>{
public:
	typedef CmnStaticQueueBase_<T_, ContSize_> MyBaseT_;
	typedef CmnStaticQueue_ConstIterator<T_, ContSize_, T_*, T_&> iterator;
	typedef CmnStaticQueue_ConstIterator<T_, ContSize_, const T_*, const T_&> const_iterator;
	friend const_iterator;

public:
	CmnStaticQueue(){
		first_index_ = MaxSize;
		cur_size_ = 0;
	}
	~CmnStaticQueue(){
		while ( !empty() )
			pop();
	}
	CmnStaticQueue(const CmnStaticQueue& rhs){
		first_index_ = MaxSize;
		cur_size_ = 0;
		IndexT idx_tmp = rhs.first_index_;
		size_type size_tmp = rhs.cur_size_;
		while ( size_tmp > 0 )
		{
			push(*(ValueTypeT*)&rhs.cont_[idx_tmp % MaxSize]);
			++idx_tmp;
			--size_tmp;
		}
	}
	CmnStaticQueue& operator = (const CmnStaticQueue& rhs){
		while ( !empty() )
			pop();
		IndexT idx_tmp = rhs.first_index_;
		size_type size_tmp = rhs.cur_size_;
		while ( size_tmp > 0 )
		{
			push(*(ValueTypeT*)&rhs.cont_[idx_tmp % MaxSize]);
			++idx_tmp;
			--size_tmp;
		}
		return *this;
	}
	ValueTypeT& back(){
		assert(!empty());
		return *(ValueTypeT*)&cont_[((int)first_index_ + cur_size_ - 1) % MaxSize];
	}
	const ValueTypeT& back() const{
		assert(!empty());
		return *(const ValueTypeT*)&cont_[((int)first_index_ + cur_size_ - 1) % MaxSize];
	}
	bool empty() const{
		return 0 == cur_size_;
	}
	bool is_full() const{
		return cur_size_ >= MaxSize;
	}
	ValueTypeT& front(){
		assert(!empty());
		return *(ValueTypeT*)&cont_[first_index_ % MaxSize];
	}
	const ValueTypeT& front() const{
		assert(!empty());
		return *(ValueTypeT*)&cont_[first_index_ % MaxSize];
	}
	void pop(){
		assert(!empty());
		Destroy(first_index_ % MaxSize);
		++first_index_;
		--cur_size_;
	}
	void push(const ValueTypeT& v){
		if (is_full())
		{
			assert(false);
			return;
		}
		Construct(((int)first_index_ + cur_size_) % MaxSize, v);
		++cur_size_;
	}
	size_type size() const{
		return cur_size_;
	}
	//////////////////////////////////////////////////////////////////////////
	iterator begin(){
		if (empty())
			return end();
		return iterator((MyBaseT_*)this, first_index_);
	}
	iterator end(){
		return iterator((MyBaseT_*)nullptr, MaxSize);
	}
	const_iterator begin() const{
		if (empty())
			return end();
		return const_iterator((MyBaseT_*)this, first_index_);
	}
	const_iterator end() const{
		return const_iterator((MyBaseT_*)nullptr, MaxSize);
	}
	//////////////////////////////////////////////////////////////////////////
};
//////////////////////////////////////////////////////////////////////////
template<typename T_, unsigned int ContSize_>
class CmnStaticStack : public CmnStaticContBase<T_, ContSize_>{
public:
	CmnStaticStack(){
		top_idx_ = (IndexT)-1;
	}
	~CmnStaticStack(){
		while ( !empty() )
			pop();
	}
	CmnStaticStack( const CmnStaticStack& rhs ){
		top_idx_ = (IndexT)-1;
		IndexT idx_tmp = 0;
		size_type size_tmp = rhs.top_idx_ + 1;
		while ( size_tmp > 0 )
		{
			push(*(ValueTypeT*)&rhs.cont_[idx_tmp]);
			++idx_tmp;
			--size_tmp;
		}
	}
	CmnStaticStack& operator = ( const CmnStaticStack& rhs ){
		while ( !empty() )
			pop();
		IndexT idx_tmp = 0;
		size_type size_tmp = rhs.top_idx_ + 1;
		while ( size_tmp > 0 )
		{
			push(*(ValueTypeT*)&rhs.cont_[idx_tmp]);
			++idx_tmp;
			--size_tmp;
		}
		return *this;
	}
	bool empty() const{
		return top_idx_ >= MaxSize;
	}
	bool is_full() const{
		return top_idx_ == MaxSize - 1;
	}
	size_type size() const{
		return top_idx_ + 1;
	}
	ValueTypeT& top(){
		assert(!empty());
		return *(ValueTypeT*)&cont_[top_idx_];
	}
	const ValueTypeT& top() const{
		assert(!empty());
		return *(ValueTypeT*)&cont_[top_idx_];
	}
	void push( const ValueTypeT& v ){
		if (is_full())
		{
			assert(false);
			return;
		}
		++top_idx_;
		Construct(top_idx_, v);
	}
	void pop(){
		assert(!empty());
		Destroy(top_idx_);
		--top_idx_;
	}

protected:
	IndexT	top_idx_;
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#pragma warning(pop)
