#pragma once
/*
@author zhp
@date 2014/3/29 17:30
@purpose for tuple more
*/
#include <boost/tuple/tuple.hpp>

namespace boost{
namespace tuples{
	//////////////////////////////////////////////////////////////////////////
	//******************************for_each_element**************************
	template <typename Function> 
	void for_each_element( const boost::tuples::null_type&, const Function& ){}

	template <typename Tuple, typename Function>
	void for_each_element( Tuple& t, Function& func ){
		func(t.get_head());
		for_each_element(t.get_tail(), func);
	}
	//////////////////////////////////////////////////////////////////////////
	//************************index_element***********************************
	//func_obj必须有个名为IdxElem的成员函数
	//IdxElem必须第一个模板参数必须是pt_dword
	template <typename Tuple, typename FuncObj> 
	void index_element( Tuple& t, FuncObj func_obj ){
		typedef boost::tuples::length<Tuple> LengthT;

		details::IndexElement_Helper<LengthT::value, Tuple, FuncObj>::Do<0>(t, func_obj);
	}
	//////////////////////////////////////////////////////////////////////////
	//***************************transform_element****************************
	template <typename Tuple, typename Function> 
	void transform_element( const boost::tuples::null_type&, const Tuple& t, const Function& ){}
	template <typename Tuple, typename Function> 
	void transform_element( const Tuple& t, const boost::tuples::null_type&, const Function& ){}
	template <typename Function> 
	void transform_element( const boost::tuples::null_type&, const boost::tuples::null_type&, const Function& ){}
	template <typename Tuple1, typename Tuple2, typename Function> 
	void transform_element( Tuple1& t1, Tuple2& t2, Function& func ){
		func(t1.get_head(), t2.get_head());
		transform_element(t1.get_tail(), t2.get_tail(), func);
	}
	//////////////////////////////////////////////////////////////////////////
	namespace details{
		template<int TupleLen, typename Tuple, typename FuncObj>
		struct IndexElement_Helper{
			template<pt_dword ElemIdx>
			static void Do( Tuple& t, FuncObj& func_obj ){
				func_obj.IdxElem<ElemIdx>(t.get<0>());
				IndexElement_Helper<boost::tuples::length<Tuple::tail_type>::value,
				Tuple::tail_type, FuncObj>::Do<ElemIdx + 1>(t.get_tail(), func_obj);
			}
		};
		template<typename Tuple, typename FuncObj>
		struct IndexElement_Helper<0, Tuple, FuncObj>{
			template<pt_dword ElemIdx>
			static void Do( const Tuple& t, const FuncObj& func_obj ){}
		};
	}	//details
}	//tuples
}	//boost