#if !BOOST_PP_IS_ITERATING
# ifndef ADD_ONCE_FE_ENV
#  define ADD_ONCE_FE_ENV
/*
@author zhp
@date 2016/5/12 22:39
@purpose FE->function execute environment,函数执行环境
*/
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/enum_params.hpp>
#  include <boost/function_types/is_member_function_pointer.hpp>
#  include <boost/function_types/function_pointer.hpp>
#  include <boost/function_types/components.hpp>
#  include <boost/mpl/long.hpp>
#  include <boost/mpl/at.hpp>
#  include <boost/mpl/pop_front.hpp>
#  include <xtr1common>


template<typename Env, typename F>
struct FE_Env;
/*
最好只使用这个宏。
Env：类型，用来判断环境，是个函数对象。若是想在判断环境失败后抛出异常的话，则可在函数对象内部去实现。
f：全局函数或成员函数，最多有CMN_ARITY_MAX个参数。
*/
#define FE_MakeEnv(Env, f) (&FE_Env<Env, decltype(Detail::FE_MakeFunc(&f))>::Call<&f>)
//make original env
#define FE_MakeOriEnv(Env, f) ((decltype(Detail::FE_MakeFunc(&f)))FE_MakeEnv(Env, f))
/*
只对成员函数有效。
转移成FE_Env对象指针。
*/
template<typename Env, typename T, typename F>
const FE_Env<Env, F>* FE_ToEnv(const T* obj_ptr, F);
//调用成员函数的时候使用
#define FE_ToCall(Env, obj_ptr, f) FE_ToEnv<Env>(obj_ptr, &f)->Call<&f>

namespace Detail{
	template<typename T, bool is_mem_fn, typename Env, typename F>
	struct FE_EnvBase;
	template<typename Types, bool IsMemFn>
	struct FE_ParamTypes{
		typedef typename boost::mpl::pop_front<Types>::type type;
	};
	template<typename Types>
	struct FE_ParamTypes<Types, true>{
		typedef typename boost::mpl::pop_front<typename FE_ParamTypes<Types, false>::type>::type type;
	};
	template<typename F>
	struct FE_EnvTraits{
		typedef typename boost::function_types::components<F>::types types;
		
		typedef typename boost::mpl::at_c<types, 0>::type result_type;
		static const auto is_mem_fn = boost::function_types::is_member_function_pointer<F>::value;
		typedef typename FE_ParamTypes<types, is_mem_fn>::type parameter_types;
		static const auto arity = is_mem_fn ? boost::mpl::size<types>::value - 2 : boost::mpl::size<types>::value - 1;
		typedef typename std::remove_cv<typename std::remove_reference<typename boost::mpl::at_c<types, 1>::type>::type>::type class_type;
	};

	template<typename F>
	F FE_MakeFunc(F);

# define FUNC_ENV_IMPL_DEFINE(_, n, __)	BOOST_PP_COMMA_IF(n) typename boost::mpl::at_c<parameter_types, n>::type p##n

#  define BOOST_PP_ITERATION_PARAMS_1 \
	(3, (0, CMN_ARITY_MAX, "FE_Env.h"))
#  include BOOST_PP_ITERATE()

# undef FUNC_ENV_IMPL_DEFINE

}	//Detail

template<typename Env, typename F>
struct FE_Env : Detail::FE_EnvBase<boost::mpl::long_<Detail::FE_EnvTraits<F>::arity>,
	Detail::FE_EnvTraits<F>::is_mem_fn, Env, F>{
	FE_Env() = delete;
	typedef F function_type;
};

template<typename Env, typename T, typename F>
const FE_Env<Env, F>* FE_ToEnv(const T* obj_ptr, F){
	const typename Detail::FE_EnvTraits<F>::class_type* cls_obj_ptr = obj_ptr;
	return (const FE_Env<Env, F>*)(void*)cls_obj_ptr;
}

# endif	//ADD_ONCE_FE_ENV
#else	//BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()

template<bool is_mem_fn, typename Env, typename F>
struct FE_EnvBase<boost::mpl::long_<N>, is_mem_fn, Env, F>{
	typedef typename FE_EnvTraits<F>::result_type result_type;
	typedef typename FE_EnvTraits<F>::parameter_types parameter_types;
	template<F Fn>
	static result_type Call(BOOST_PP_REPEAT(N, FUNC_ENV_IMPL_DEFINE, _)){
		Env env;
		if (env())
		{
			return Fn(BOOST_PP_ENUM_PARAMS(N, p));
		}
		return result_type();
	}
};

template<typename Env, typename F>
struct FE_EnvBase<boost::mpl::long_<N>, true, Env, F>{
	typedef typename FE_EnvTraits<F>::result_type result_type;
	typedef typename FE_EnvTraits<F>::parameter_types parameter_types;
	typedef typename FE_EnvTraits<F>::class_type class_type;
	template<F Fn>
	result_type Call(BOOST_PP_REPEAT(N, FUNC_ENV_IMPL_DEFINE, _)) const{
		Env env;
		if (env())
		{
			auto this_ptr = (class_type*)(void*)this;
			return (this_ptr->*Fn)(BOOST_PP_ENUM_PARAMS(N, p));
		}
		return result_type();
	}
};

# undef N

#endif	//BOOST_PP_IS_ITERATING