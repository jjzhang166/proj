#pragma once
/*
@author zhp
@date 2017/4/27 1:44
@purpose queue extention
*/
#include <queue>

namespace Details{
	template<typename ContT, typename value_type, typename difference_type, typename pointer, typename reference>
	struct QueueIterator_ : std::_Iterator012<std::forward_iterator_tag,
		value_type,
		difference_type,
		pointer,
		reference,
		std::_Iterator_base>{
		QueueIterator_(const QueueIterator_& rhs) : the_cont_(rhs.the_cont_){}

		ContT		the_cont_;
	};
}

namespace StdEx{

	template<class _Ty,
	class _Container = vector<_Ty>,
	class _Pr = less<typename _Container::value_type> >
	class priority_queue : public std::priority_queue<_Ty, _Container, _Pr>{
		typedef std::priority_queue<_Ty, _Container, _Pr> MyBase;

	public:
		typedef typename _Container::iterator iterator;
		typedef typename _Container::const_iterator const_iterator;

	public:
		priority_queue() : MyBase(){}
		priority_queue(const priority_queue& rhs) : MyBase(rhs){}
		explicit priority_queue(const _Pr& _Pred) : MyBase(_Pred){}
		priority_queue(const _Pr& _Pred, const _Container& _Cont) : MyBase(_Pred, _Cont){}
		template<class _InIt>
		priority_queue(_InIt _First, _InIt _Last) : MyBase(_First, _Last){}
		template<class _InIt>
		priority_queue(_InIt _First, _InIt _Last, const _Pr& _Pred) : MyBase(_First, _Last, _Pred){}
		template<class _InIt>
		priority_queue(_InIt _First, _InIt _Last, const _Pr& _Pred,
			const _Container& _Cont) : MyBase(_First, _Last, _Pred, _Cont){}
		priority_queue(priority_queue&& _Right) : MyBase(_Right){}
		const _Container& GetContainer() const{
			return c;
		}
		_Container& GetContainer(){
			return c;
		}
		iterator begin(){
			return c.begin();
		}
		iterator end(){
			return c.end();
		}
		const_iterator begin() const{
			return c.begin();
		}
		const_iterator end() const{
			return c.end();
		}
		void Sort(){
			std::make_heap(begin(), end(), comp);
		}
	};
}
