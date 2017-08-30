#include "stdafx.h"
#include <RStd/RContTest.h>
#include <iostream>
#include <RStd/RSet.h>
#include <RStd/RMap.h>
#include <RStd/RVector.h>
#include <RStd/RList.h>
#include <RStd/RDeque.h>
#include <RStd/RQueue.h>
#include <RStd/RStack.h>
#include <RStd/RString.h>
#include <RStd/RBitset.h>
#include <RStd/RHash_set.h>
#include <RStd/RHash_map.h>
#include <unordered_map>
#include <deque>
#include <forward_list>


void TestRStdCont()
{
	std::deque<int> std_deque;
	std::forward_list<int> fl;
	auto fl_size = sizeof(fl);
	static_assert(20 == sizeof(std_deque), "");
	RStd::set<int> set_;
	RStd::vector<int> vector_;
	vector_.push_back(1);
	for (auto& v : vector_)
	{
		int i = 0;
	}
	vector_.pop_back();
	RStd::list<int> list_;
	list_.push_back(2);
	for (auto& v : list_)
	{
		int i = 0;
	}
	list_.pop_back();
	RStd::deque<int> deque_;
	deque_.push_back(2);
	for (auto& v : deque_)
	{
		int i = 0;
	}
	for (auto& v : set_)
	{
		int i = 0;
	}
	deque_.front();
	deque_.back();
	deque_.pop_back();

	set_.insert(1);
	RStd::map<int, int> map_;

	map_.insert(std::make_pair(2, 3));
	for (auto& v : map_)
	{
		int i = 0;
	}
	RStd::queue<int> queue_;
	queue_.push(1);
	queue_.back();
	queue_.pop();
	RStd::stack<int> stack_;
	stack_.push(1);
	stack_.top();
	stack_.pop();
	RStd::string string_;
	string_.c_str();
	string_ += "134";
	for (auto v : string_)
	{
		int i = 0;
	}
	RStd::bitset<20> bitset_;
	bitset_.test(10);
	bitset_.reset(1);
	bitset_.set(2);
	bitset_.all();
	RStd::hash_map<const char*, int> hash_map_;
	hash_map_.insert(std::make_pair("123", 2));
	hash_map_.insert(std::make_pair("456", 3));
	for (auto& v : hash_map_)
	{
		int i = 0;
	}
	hash_map_.find("123");
	auto hash_map_cnt = hash_map_.size();
	RStd::hash_set<int> hash_set_;

	hash_set_.insert(1);
	for (auto& v : hash_set_)
	{
		int i = 0;
	}
	RStd::hash_multiset<int> hash_multiset_;
	hash_multiset_.insert(1);
	for (auto& v : hash_multiset_)
	{
		int i = 0;
	}
	RStd::hash_multimap<int, int> hash_multimap_;
	hash_multimap_.insert(std::make_pair(1, 2));
	for (auto& v : hash_multimap_)
	{
		int i = 0;
	}
	RStd::multiset<RStd::string> mset;
	mset.insert("123");
	for (auto& v : mset)
	{
		int i = 0;
	}
	RStd::multimap<double, RStd::wstring> mmap;
	mmap.insert(std::make_pair(1.2, L"fdasf"));
	for (auto& v : mmap)
	{
		int i = 0;
	}
	using namespace RStd;
	auto bitset2 = to_std(bitset_);
	from_std(bitset2);
	auto deque2 = to_std(deque_);
	from_std(deque2);
	auto hash_map2 = to_std(hash_map_);
	from_std(hash_map2);
	auto hash_set2 = to_std(hash_set_);
	from_std(hash_set2);
	auto list2 = to_std(list_);
	from_std(list2);
	auto map2 = to_std(map_);
	from_std(map2);
	auto set2 = to_std(set_);
	from_std(set2);
	auto string2 = to_std(string_);
	from_std(string2);
	auto vector2 = to_std(vector_);
	from_std(vector2);
	auto hash_multimap2 = to_std(hash_multimap_);
	from_std(hash_multimap2);
	auto hash_multiset2 = to_std(hash_multiset_);
	from_std(hash_multiset2);
	std::cout << "vector:" << sizeof(vector_) << std::endl;
	std::cout << "list:" << sizeof(list_) << std::endl;
	std::cout << "deque:" << sizeof(deque_) << std::endl;
	std::cout << "string:" << sizeof(string_) << std::endl;
	std::cout << "set:" << sizeof(set_) << std::endl;
	std::cout << "multiset:" << sizeof(mset) << std::endl;
	std::cout << "map:" << sizeof(map_) << std::endl;
	std::cout << "multimap:" << sizeof(mmap) << std::endl;
	std::cout << "hash_set:" << sizeof(hash_set_) << std::endl;
	std::cout << "hash_map:" << sizeof(hash_map_) << std::endl;
	std::cout << "hash_multiset:" << sizeof(hash_multiset_) << std::endl;
	std::cout << "hash_multimap:" << sizeof(hash_multimap_) << std::endl;
	std::cout << "queue:" << sizeof(queue_) << std::endl;
	std::cout << "stack:" << sizeof(stack_) << std::endl;
}
