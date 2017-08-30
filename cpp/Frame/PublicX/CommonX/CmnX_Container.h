#pragma once
/*
@author zhp
@date 2016/11/25 18:05
@purpose common useful container
*/

template<class _Ty, typename _Container , typename Cmp_>
class Cmnx_OrderSet : public _Container{
public:
	typedef typename _Container::value_type value_type;
	typedef typename _Container::iterator iterator;
	typedef typename _Container::const_iterator const_iterator;

public:
	bool insert(const value_type& value){
		auto it = begin();
		auto it_end = end();
		for (; it != it_end; ++it)
		{
			auto& v = *it;
			if (v == value)
				return false;
			if (cmp_(value, v))
				break;
		}
		if (it == it_end)
			push_back(value);
		else
			__super::insert(it, value);
		return true;
	}
	void erase(const value_type& v){
		auto it = find(v);
		if (it != end())
			__super::erase(it);
	}
	using _Container::erase;
	iterator find(const value_type& v){
		return std::find(begin(), end(), v);
	}
	const_iterator find(const value_type& v) const{
		return std::find(begin(), end(), v);
	}

private:
	Cmp_		cmp_;
};
