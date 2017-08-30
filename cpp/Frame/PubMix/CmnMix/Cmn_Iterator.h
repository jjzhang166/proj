#pragma once
/*
@author zhp
@date 2016/4/10 22:50
@purpose iterator
*/
#include <memory>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <ProtocolX/P_DefinesX.h>
#include <xtr1common>

//////////////////////////////////////////////////////////////////////////
class IteratorBase{
public:
	virtual ~IteratorBase(){}
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() const = 0;
};
//////////////////////////////////////////////////////////////////////////
//container tag
struct IterContTag_ByCopy{};
struct IterContTag_ByRef{};
struct IterContTag_BackInsert{};
//filter tag
struct IterFilterTag_Custom{};
//这种方式是比较危险的，迭代器析构前必须保证filter的有效性。
struct IterFilterTag_Filter{};
//这种方式必须保证filter被拷贝后，其内容并未被截断，否则可能并未有预见的过滤效果。
struct IterFilterTag_SafeFilter{};
//////////////////////////////////////////////////////////////////////////
struct IterFilter_Getter{
	template<typename T>
	static T& get(T& v){
		return v;
	}
	template<typename T>
	static const T& get(const T& v){
		return v;
	}
};

struct IterFilter_DerefGetter{
	template<typename T>
	static T& get(T* v){
		assert(v);
		return *v;
	}
	template<typename T>
	static const T& get(const T* v){
		assert(v);
		return *v;
	}
};

struct IterFilter_SmartGetter{
	template<typename T>
	static typename T::element_type& get(T& v){
		assert(v);
		return *v;
	}
	template<typename T>
	static const typename T::element_type& get(const T& v){
		assert(v);
		return *v;
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename ValueType_>
class Iterator : public IteratorBase{
public:
	typedef ValueType_ value_type;
	struct DummyIterator{
		DummyIterator(Iterator& it) : iter_(&it){
			iter_->First();
		}
		DummyIterator(){ iter_ = nullptr; }
		DummyIterator(const DummyIterator& rhs) : iter_(rhs.iter_){}
		DummyIterator& operator = (const DummyIterator& rhs){
			iter_ = rhs.iter_;
			return *this;
		}
		ValueType_ operator * () const{
			assert(iter_);
			return iter_->CurItem();
		}
		DummyIterator& operator ++ (){
			assert(iter_);
			iter_->Next();
			return *this;
		}
		DummyIterator operator ++ (int){
			auto tmp = *this;
			++*this;
			return tmp;
		}
		bool operator != (const DummyIterator& rhs) const{
			auto it = iter_ ? iter_ : rhs.iter_;
			if (!it)
			{
				assert(false);
				return false;
			}
			return !it->IsDone();
		}

		Iterator*	iter_;
	};

public:
	virtual value_type CurItem() const = 0;
	DummyIterator begin(){
		return DummyIterator(*this);
	}
	DummyIterator end(){
		return DummyIterator();
	}
};

template<typename ValueType_, 
	typename IterContTag_ = IterContTag_ByCopy,
	typename IterFilterTag_ = IterFilterTag_Custom,
	typename IterFilterGetter = IterFilter_Getter>
struct IteratorTraits{
	typedef ValueType_ value_type;
	typedef Iterator<value_type> iterator;
	typedef boost::shared_ptr<iterator> iterator_pointer;
	typedef IterContTag_ container_tag;
	typedef IterFilterTag_ filter_tag;
	typedef IterFilterGetter filter_getter;
};

template<typename IterTraits_, typename ContT_, typename IterT>
class IteratorCont : public IterTraits_::iterator{
public:
	typedef IterTraits_ IterTraits;
	typedef ContT_ ContT;
	typedef IterT ContIter;
	typedef typename IterTraits::value_type value_type;

public:
	template<typename T>
	IteratorCont(T& cont) : cont_(cont){
		it_ = cont_.begin();
	}
	void First() override{
		it_ = cont_.begin();
	}
	void Next() override{
		if (it_ != cont_.end())
			++it_;
	}
	bool IsDone() const override{
		return it_ == cont_.end();
	}
	value_type CurItem() const override{
		assert(it_ != cont_.end());
		return *it_;
	}

protected:
	ContT		cont_;
	ContIter	it_;
};

template<typename IterTraits_, typename ContT_, typename IterT, typename FilterType_>
class IteratorFilter : public IteratorCont<IterTraits_, ContT_, IterT>{
public:
	typedef FilterType_ FilterType;

public:
	template<typename T>
	IteratorFilter(T& cont, const FilterType& filter) : IteratorCont(cont), filter_(filter){
		it_ = cont_.begin();
	}
	void First() override{
		it_ = cont_.begin();
		NextValid();
	}
	void Next() override{
		if (it_ == cont_.end())
			return;
			++it_;
		NextValid();
	}
	bool IsDone() const override{
		return it_ == cont_.end();
	}
	value_type CurItem() const override{
		assert(it_ != cont_.end());
		return *it_;
	}

protected:
	virtual void NextValid(){
		auto it_end = cont_.end();
		while (it_ != it_end)
		{
			if (filter_(*it_))
				break;
			++it_;
		}
	}

protected:
	FilterType		filter_;
};

template<typename IterTraits_, typename ContT_, typename IterT, typename FilterType_>
class IteratorCounter : public IteratorFilter<IterTraits_, ContT_, IterT, FilterType_>{
public:
	template<typename T>
	IteratorCounter(T& cont, const FilterType& filter, pt_dword max_cnt) : IteratorFilter(cont, filter){
		assert(max_cnt > 0);
		valid_cnt_ = 0;
		max_valid_cnt_ = max_cnt;
	}
	void First() override{
		valid_cnt_ = 0;
		__super::First();
	}
	bool IsDone() const override{
		if (valid_cnt_ > max_valid_cnt_)
			return true;
		return __super::IsDone();
	}

protected:
	void NextValid() override{
		assert(valid_cnt_ <= max_valid_cnt_);
		if (valid_cnt_ >= max_valid_cnt_)
		{
			++valid_cnt_;
			return;
		}
		auto it_end = cont_.end();
		while (it_ != it_end)
		{
			if (filter_(*it_))
			{
				++valid_cnt_;
				break;
			}
			++it_;
		}
	}

private:
	pt_dword	valid_cnt_;
	pt_dword	max_valid_cnt_;
};

template<typename HandlerT_, typename WrapperFuncT_, typename RealItemT_>
class IteratorDecorator : public Iterator<RealItemT_>{
public:
	typedef HandlerT_ HandlerT;
	typedef WrapperFuncT_ WrapperFuncT;
	typedef RealItemT_ value_type;

public:
	IteratorDecorator(const HandlerT& handle, const WrapperFuncT& wrapper_func) : wrapper_func_(wrapper_func){
		handler_ = handle;
		assert(handle);
	}
	void First() override{
		handler_->First();
	}
	void Next() override{
		handler_->Next();
	}
	bool IsDone() const override{
		return handler_->IsDone();
	}
	value_type CurItem() const override{
		return wrapper_func_(handler_->CurItem());
	}

private:
	HandlerT		handler_;
	WrapperFuncT	wrapper_func_;
};

//这个是有问题的，暂时就这么地吧，反正用不到
template<typename IterTraits_, typename ContT_>
class BackInsertIterator : public IterTraits_::iterator{
public:
	typedef std::back_insert_iterator<ContT_> iterator;
	typedef typename IterTraits_::value_type value_type;

public:
	BackInsertIterator(ContT_& cont) : cont_(cont), back_insert_it_(cont_){}

public:
	void First() override{
		std::swap(back_insert_it_, std::move(iterator(cont_)));
	}
	void Next() override{
		if (back_insert_it_ != cont_.end())
			++back_insert_it_;
	}
	bool IsDone() const override{
		return false;
	}
	value_type CurItem() const override{
		return *back_insert_it_;
	}

private:
	iterator	back_insert_it_;
	ContT_&		cont_;
};

template<typename ValueType_>
class IteratorMaker{
public:
	virtual ~IteratorMaker(){}
	virtual boost::shared_ptr<Iterator<ValueType_> > CreateIterator() = 0;
};
//////////////////////////////////////////////////////////////////////////
namespace Detail{
	template<typename FilterT_, typename FilterGetter_, typename FilterTag_>
	struct _IteratorFilterGetter{};

	template<typename FilterT_, typename FilterGetter_>
	struct _IteratorFilterGetter<FilterT_, FilterGetter_, IterFilterTag_Filter>{
		struct type{
			type(const FilterT_& filter) : filter_(filter){}
			template<typename T>
			bool operator()(const T& filted_obj){
				return filter_.DoFilt(FilterGetter_::get(filted_obj));
			}

			const FilterT_&		filter_;
		};
	};

	template<typename FilterT_, typename FilterGetter_>
	struct _IteratorFilterGetter<FilterT_, FilterGetter_, IterFilterTag_SafeFilter>{
		struct type{
			type(const FilterT_& filter) : filter_(filter){}
			template<typename T>
			bool operator()(const T& filted_obj){
				return filter_.DoFilt(FilterGetter_::get(filted_obj));
			}

			FilterT_	filter_;
		};
	};

	template<typename FilterT_, typename FilterGetter_>
	struct _IteratorFilterGetter<FilterT_, FilterGetter_, IterFilterTag_Custom>{
		typedef FilterT_ type;
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename IterTraits_, typename ContT_, typename ContTag_>
	struct _IteratorContGetter{};

	template<typename IterTraits_, typename ContT_>
	struct _IteratorContGetter<IterTraits_, ContT_, IterContTag_ByCopy>{
		typedef typename IterTraits_::iterator_pointer iterator_pointer;
		typedef typename std::remove_cv<ContT_>::type cont_type;
		typedef typename cont_type::iterator cont_iterator;
		typedef IteratorCont<IterTraits_, cont_type, cont_iterator> type;

		static iterator_pointer Make(cont_type& cont){
			return iterator_pointer(new type(cont));
		}

		template<typename FilterTag_, typename FilterGetter_, typename FilterType_>
		static iterator_pointer Make(cont_type& cont, const FilterType_& filter){
			typedef typename _IteratorFilterGetter<FilterType_, FilterGetter_, FilterTag_>::type filter_type;
			typedef IteratorFilter<IterTraits_, cont_type, cont_iterator, filter_type> impl_type;
			return iterator_pointer(new impl_type(cont, filter));
		}

		template<typename FilterTag_, typename FilterGetter_, typename FilterType_>
		static iterator_pointer Make(cont_type& cont, const FilterType_& filter, pt_dword max_cnt){
			typedef typename _IteratorFilterGetter<FilterType_, FilterGetter_, FilterTag_>::type filter_type;
			typedef IteratorCounter<IterTraits_, cont_type, cont_iterator, filter_type> impl_type;
			return iterator_pointer(new impl_type(cont, filter, max_cnt));
		}
	};

	template<typename IterTraits_, typename ContT_>
	struct _IteratorContGetter<IterTraits_, ContT_, IterContTag_ByRef>{
		typedef typename IterTraits_::iterator_pointer iterator_pointer;
		typedef typename std::remove_cv<ContT_>::type cont_type;
		typedef typename cont_type::iterator cont_iterator;
		typedef IteratorCont<IterTraits_, cont_type&, cont_iterator> type;

		static iterator_pointer Make(cont_type& cont){
			return iterator_pointer(new type(cont));
		}

		template<typename FilterTag_, typename FilterGetter_, typename FilterType_>
		static iterator_pointer Make(cont_type& cont, const FilterType_& filter){
			typedef typename _IteratorFilterGetter<FilterType_, FilterGetter_, FilterTag_>::type filter_type;
			typedef IteratorFilter<IterTraits_, cont_type&, cont_iterator, filter_type> impl_type;
			return iterator_pointer(new impl_type(cont, filter));
		}

		template<typename FilterTag_, typename FilterGetter_, typename FilterType_>
		static iterator_pointer Make(cont_type& cont, const FilterType_& filter, pt_dword max_cnt){
			typedef typename _IteratorFilterGetter<FilterType_, FilterGetter_, FilterTag_>::type filter_type;
			typedef IteratorCounter<IterTraits_, cont_type&, cont_iterator, filter_type> impl_type;
			return iterator_pointer(new impl_type(cont, filter, max_cnt));
		}
	};

	template<typename IterTraits_, typename ContT_>
	struct _IteratorContGetter<IterTraits_, ContT_, IterContTag_BackInsert>{
		typedef typename IterTraits_::iterator_pointer iterator_pointer;
		typedef typename std::remove_cv<ContT_>::type cont_type;
		typedef BackInsertIterator<IterTraits_, cont_type> type;

		static iterator_pointer Make(cont_type& cont){
			return iterator_pointer(new type(cont));
		}
	};
}
//////////////////////////////////////////////////////////////////////////
template<typename IterTraits_, typename ContT_>
typename IterTraits_::iterator_pointer MakeIterator(ContT_& cont){
	typedef typename IterTraits_::container_tag container_tag;
	typedef Detail::_IteratorContGetter<IterTraits_, ContT_, container_tag> type;
	return type::Make(cont);
}

template<typename IterTraits_, typename ContT_, typename FilterType_>
typename IterTraits_::iterator_pointer MakeIterator(ContT_& cont, const FilterType_& filter){
	typedef typename IterTraits_::container_tag container_tag;
	typedef typename IterTraits_::filter_tag filter_tag;
	typedef typename IterTraits_::filter_getter filter_getter;
	typedef Detail::_IteratorContGetter<IterTraits_, ContT_, container_tag> type;
	return type::Make<filter_tag, filter_getter>(cont, filter);
}

template<typename IterTraits_, typename ContT_, typename FilterType_>
typename IterTraits_::iterator_pointer MakeIterator(ContT_& cont, const FilterType_& filter, pt_dword max_cnt){
	typedef typename IterTraits_::container_tag container_tag;
	typedef typename IterTraits_::filter_tag filter_tag;
	typedef typename IterTraits_::filter_getter filter_getter;
	typedef Detail::_IteratorContGetter<IterTraits_, ContT_, container_tag> type;
	return type::Make<filter_tag, filter_getter>(cont, filter, max_cnt);
}

template<typename IterTraits_, typename HandlerT_, typename WrapperFuncT_>
typename IterTraits_::iterator_pointer MakeWrapperIterator(const HandlerT_& handler, const WrapperFuncT_& wrapper_fn){
	typedef typename IterTraits_::iterator_pointer IterBasePtr;
	typedef typename IterTraits_::value_type value_type;
	return IterBasePtr(new IteratorDecorator<HandlerT_, WrapperFuncT_, value_type>(handler, wrapper_fn));
}
//////////////////////////////////////////////////////////////////////////