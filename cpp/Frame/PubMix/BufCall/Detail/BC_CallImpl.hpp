#if !BOOST_PP_IS_ITERATING
# ifndef ADD_ONCE_BC_CALL_IMPL
#  define ADD_ONCE_BC_CALL_IMPL
/*
@author zhp
@date 2016/4/18 4:43
*/
#  include <boost/mpl/long.hpp>
#  include <boost/mpl/at.hpp>
#  include <boost/mpl/pop_front.hpp>
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/repetition/repeat.hpp>

#  include "../BC_Stream.h"
#  include "../BC_DataConver.h"
#  include "../BC_TypeToString.h"


class BC_BufClassRuntimeInfo{
public:
	virtual ~BC_BufClassRuntimeInfo(){}
};

class BC_Function{
public:
	virtual ~BC_Function(){}
	virtual bool DoCall( BC_BufClassRuntimeInfo* pthis, pt_csz function_name, BC_AsStreamR& stm, std::string* err_info ) const = 0;
};

namespace Detail{
	template <typename Signature>
	bool BC_ErrStr_NoMatch( pt_csz function_name, std::string* err_info, pt_csz pre_str ){
		if (err_info)
		{
			err_info->assign(pre_str);
			err_info->append(BC_FormatSignature(function_name, Signature()));
		}
		return false;
	}
}

template <typename Signature, typename Cls, typename F>
bool BC_InvokeM( Cls& self, pt_csz function_name, F f, BC_AsStreamR& stm, std::string* err_info )
{
	//typedef typename boost::mpl::at_c<Signature, 0>::type ResultT;

	typedef typename boost::mpl::pop_front<Signature>::type NoResultT;
	typedef typename boost::mpl::pop_front<NoResultT>::type ParamSigT;
	try{
		if (!BC_InvokeMember<ParamSigT>(self, f, stm, boost::mpl::long_<boost::mpl::size<ParamSigT>::value>()))
			return Detail::BC_ErrStr_NoMatch<Signature>(function_name, err_info, "²ÎÊý²»Æ¥Åä£º");
	}DO_CATCH_THE_EXCEPTION(const Detail::BC_Exception& e, {
		return Detail::BC_ErrStr_NoMatch<Signature>(function_name, err_info, e.what());
	})
	return true;
}

/**/
# define BUF_CALL_CALL_IMPL_DEFINE(_, n, __)	\
	auto p##n = BC_DataWrapper<typename boost::mpl::at_c<ParamSigT, n>::type, Cls>::AsT(stm);	\
	if ( !p##n.Valid() )	\
		return false;
/**/
# define BUF_CALL_CALL_IMPL_DO_CALL(_, n, param)		\
	BOOST_PP_COMMA_IF(n) static_cast<typename BC_DataConverter<decltype(param##n)>::type>(param##n)
/**/
#  define BOOST_PP_ITERATION_PARAMS_1 \
	(3, (0, CMN_ARITY_MAX, "BC_CallImpl.hpp"))
#  include BOOST_PP_ITERATE()

# undef BUF_CALL_CALL_IMPL_DEFINE
# undef BUF_CALL_CALL_IMPL_DO_CALL

# endif	//ADD_ONCE_BC_CALL_IMPL
#else	//BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()

template<typename ParamSigT, typename Cls, typename F>
bool BC_InvokeMember( Cls& self, F f, BC_AsStreamR& stm, boost::mpl::long_<N> ){	
	BOOST_PP_REPEAT(N, BUF_CALL_CALL_IMPL_DEFINE, _)

	if ( stm.LeftSize() != 0 )
		return false;

	(self.*f)(BOOST_PP_REPEAT(N, BUF_CALL_CALL_IMPL_DO_CALL, p));
	return true;
}

# undef N

#endif	//BOOST_PP_IS_ITERATING