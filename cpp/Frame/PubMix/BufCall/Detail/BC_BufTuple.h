#if !BOOST_PP_IS_ITERATING
# ifndef ADD_ONCE_BC_BUF_TUPLE
#  define ADD_ONCE_BC_BUF_TUPLE
/*
@author zhp
@date 2016/4/20 15:59
此文件已用不到。
*/
#  include <boost/preprocessor/repetition/repeat.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>
//#  include <boost/preprocessor/repetition/enum_binary_params.hpp>
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/punctuation/comma_if.hpp>
#  include <boost/preprocessor/arithmetic/dec.hpp>

#  include "BC_Config.h"
#  include "BC_Base.h"


namespace Detail{
	struct BC_TupleNil{};

	template<pt_word idx, typename TupleT>
	struct BC_TupleElementImpl;

	template<typename TupleT>
	struct BC_TupleSizeImpl;
}

#  pragma pack(push)
#  pragma pack(1)
//////////////////////////////////////////////////////////////////////////
#  define MINUS_N BOOST_PP_DEC(BUF_CALL_MAX_ARITY)
/**/
#  define BUF_CALL_BUF_TUPLE_DEFINE_T(_, n, __)	\
	BOOST_PP_COMMA_IF(n) typename T##n = Detail::BC_TupleNil
/**/
#  define BUF_CALL_BUF_TUPLE_DEFINE_VALUE(_, n, __)	\
	T##n	v##n##_;
/**/
template<BOOST_PP_REPEAT(BUF_CALL_MAX_ARITY, BUF_CALL_BUF_TUPLE_DEFINE_T, _)>
class BC_BufTuple;

template<BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
class BC_BufTuple<Detail::BC_TupleNil BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)>{
};
template<typename T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
class BC_BufTuple : private BC_BufTuple<BOOST_PP_ENUM_PARAMS(MINUS_N, T)>{
public:
	typedef T type;
	

public:
	type	value_;
};

#  undef BUF_CALL_BUF_TUPLE_DEFINE_T
#  undef BUF_CALL_BUF_TUPLE_DEFINE_VALUE
#  undef MINUS_N
//////////////////////////////////////////////////////////////////////////
#  pragma pack(pop)

#  define BOOST_PP_ITERATION_PARAMS_1 \
	(3, (1, BUF_CALL_MAX_ARITY, "BC_BufTuple.h"))
#  include BOOST_PP_ITERATE()

template<pt_word idx, typename TupleT>
struct BC_TupleElement : Detail::BC_TupleElementImpl<idx, typename BC_RemoveTypeCVR<TupleT>::type>{
	typedef Detail::BC_TupleElementImpl<idx, typename BC_RemoveTypeCVR<TupleT>::type> ImplT;
	typedef typename ImplT::type type;
	typedef typename ImplT::type_t type_t;
};

template<typename TupleT>
struct BC_TupleSize{
	static const pt_dword size = Detail::BC_TupleSizeImpl<typename BC_RemoveTypeCVR<TupleT>::type>::size;
};

template<pt_word idx, typename TupleT>
const typename BC_TupleElement<idx, TupleT>::type& BC_Get( const TupleT& tuple ){
	typedef typename BC_TupleElement<idx, TupleT>::type_t type_t;
	typedef typename BC_TupleElement<idx, TupleT>::type type;
	return std::forward<const type&>(((type_t&)tuple).value_);
}
template<pt_word idx, typename TupleT>
typename BC_TupleElement<idx, TupleT>::type& BC_Get( TupleT& tuple ){
	typedef typename BC_TupleElement<idx, TupleT>::type_t type_t;
	typedef typename BC_TupleElement<idx, TupleT>::type type;
	return std::forward<type&>(((type_t&)tuple).value_);
}

# endif	//ADD_ONCE_BC_BUF_TUPLE
#else	//BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()
/**/
# define MINUS_N BOOST_PP_DEC(N)
/**/


namespace Detail{
	template<typename T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
	struct BC_TupleElementImpl<0, BC_BufTuple<T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)> >{
		typedef T type;
		typedef BC_BufTuple<T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)> type_t;
	};
	template<pt_word idx, typename T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
	struct BC_TupleElementImpl<idx, BC_BufTuple<T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)> > : BC_TupleElementImpl<0, BC_BufTuple<BOOST_PP_ENUM_PARAMS(MINUS_N, T)> >{
		typedef typename BC_TupleElementImpl<0, BC_BufTuple<BOOST_PP_ENUM_PARAMS(MINUS_N, T)> > BaseType;
		typedef typename BaseType::type type;
		typedef typename BaseType::type_t type_t;
	};

	template<BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
	struct BC_TupleSizeImpl<BC_BufTuple<Detail::BC_TupleNil BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)> >{
		static const pt_word size = 0;
	};
	template<typename T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)>
	struct BC_TupleSizeImpl<BC_BufTuple<T BOOST_PP_COMMA_IF(MINUS_N) BOOST_PP_ENUM_PARAMS(MINUS_N, T)> >{
		static const pt_dword size = BC_TupleSizeImpl<BC_BufTuple<BOOST_PP_ENUM_PARAMS(MINUS_N, typename T)> >::size + 1;
	};
}


# undef N
# undef MINUS_N

#endif	//BOOST_PP_IS_ITERATING