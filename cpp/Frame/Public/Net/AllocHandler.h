#pragma once
/*
@author zhp
@date 2013-12-22
@purpose allocate handler
*/
#include <boost/noncopyable.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/asio.hpp>
#include <Protocol/P_Defines.h>
#include "../DependSelfLib.h"

class handler_allocator
	: private boost::noncopyable
{
public:
	handler_allocator();
	void* allocate(pt_dword size);
	void deallocate(void* pointer);

private:
	boost::aligned_storage<P_BufferMaxLen> storage_;
	bool in_use_;
};

template <typename Handler>
class custom_alloc_handler
{
public:
	custom_alloc_handler(handler_allocator& a, Handler h)
		: allocator_(a),
		handler_(h)
	{
	}

	template <typename Arg1>
	void operator()(Arg1 arg1)
	{
		handler_(arg1);
	}

	template <typename Arg1, typename Arg2>
	void operator()(Arg1 arg1, Arg2 arg2)
	{
		handler_(arg1, arg2);
	}

	friend void* asio_handler_allocate(pt_dword size,
		custom_alloc_handler<Handler>* this_handler)
	{
		return this_handler->allocator_.allocate(size);
	}

	friend void asio_handler_deallocate(void* pointer, pt_dword /*size*/,
		custom_alloc_handler<Handler>* this_handler)
	{
		this_handler->allocator_.deallocate(pointer);
	}

private:
	handler_allocator& allocator_;
	Handler handler_;
};

// Helper function to wrap a handler object to add custom allocation.
template <typename Handler>
inline custom_alloc_handler<Handler> make_custom_alloc_handler(
	handler_allocator& a, Handler h)
{
	return custom_alloc_handler<Handler>(a, h);
}