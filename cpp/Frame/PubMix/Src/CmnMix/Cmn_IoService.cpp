#include "stdafx.h"
#include <CmnMix/Cmn_IoService.h>

std::size_t IoService::PollOne()
{
	++do_exec_depth_;
	BOOST_SCOPE_EXIT_ALL(&){
		--do_exec_depth_;
		assert(do_exec_depth_ >= 0);
	};
	return impl_.poll_one();
}

std::size_t IoService::Poll()
{
	++do_exec_depth_;
	BOOST_SCOPE_EXIT_ALL(&){
		--do_exec_depth_;
		assert(do_exec_depth_ >= 0);
	};
	return impl_.poll();
}

std::size_t IoService::Run()
{
	++do_exec_depth_;
	BOOST_SCOPE_EXIT_ALL(&){
		--do_exec_depth_;
		assert(do_exec_depth_ >= 0);
	};
	return impl_.run();
}

std::size_t IoService::RunOne()
{
	++do_exec_depth_;
	BOOST_SCOPE_EXIT_ALL(&){
		--do_exec_depth_;
		assert(do_exec_depth_ >= 0);
	};
	return impl_.run_one();
}

IoService::IoService()
{
	do_exec_depth_ = 0;
}

int IoService::GetExecDepth() const
{
	return do_exec_depth_;
}

IoService::IoSerT& IoService::GetImpl()
{
	return impl_;
}

const IoService::IoSerT& IoService::GetImpl() const
{
	return impl_;
}

IoService::WorkImpl_::WorkImpl_(IoService& ios) : work(ios.GetImpl())
{

}

IoService::WorkImpl_::WorkImpl_(boost::asio::io_service::work& ios) : work(ios)
{

}

IoService::WorkImpl_::WorkImpl_(const WorkImpl_& other) : work(other)
{

}

IoService::WorkImpl_::WorkImpl_(const boost::asio::io_service::work& other) : work(other)
{

}
