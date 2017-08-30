#pragma once
/*
@author zhp
@date 2016/4/17 20:59
@purpose function for call
*/
#include "Detail/BC_CallImpl.hpp"
#include <boost/function_types/components.hpp>
#include <boost/mpl/at.hpp>

template<typename F, typename Signature>
class BC_FunctionImpl : public BC_Function{
public:
	BC_FunctionImpl( F f ) : f_(f){}

private:
	bool DoCall( BC_BufClassRuntimeInfo* pthis, pt_csz function_name, BC_AsStreamR& stm, std::string* err_info ) const override{
		typedef typename boost::mpl::at_c<Signature, 1>::type ThisRefT;
		typedef typename BC_RemoveTypeCVR<ThisRefT>::type ThisT;
		auto obj = dynamic_cast<ThisT*>(pthis);
		if ( !obj )
		{
			if (err_info)
				err_info->assign("无法向下转换类型");
			return false;
		}		
		return BC_InvokeM<Signature>(*obj, function_name, f_, stm, err_info);
	}

private:
	F			f_;
};

namespace Detail{
	template<typename F>
	BC_Function* BC_MakeFunction(F f){
		return new BC_FunctionImpl<F, boost::function_types::components<F>::types>(f);
	}
}
