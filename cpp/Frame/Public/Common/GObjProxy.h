#pragma once
/*
@author zhp
@date 2014/1/21
@purpose game object proxy
*/
#include <boost/preprocessor/repetition/repeat.hpp>

//game object proxy
template<typename ClassType>
class GObjP{
public:
	template<typename T>
	GObjP( T obj_addr ){
		real_obj_addr_ = (ClassType*)(void*)obj_addr;
		assert(real_obj_addr_);
	}
	ClassType* operator -> () const{
		return real_obj_addr_;
		static_assert(sizeof(ClassType) <= sizeof(int), "这个类里面，一定不能有成员变量，否则编码的时候就得极其小心");
	}

private:
	ClassType*				real_obj_addr_;
};



#define _GAME_OBJ_PROXY_REPEAT_M(z, n, name)	\
	virtual void name##n() = 0;	\
/**/
//辅助产生指定数目和指定名称的虚函数
//generate virtual functions
#define GAME_OBJ_PROXY_GEN_V_FUNC(func_name, n)		BOOST_PP_REPEAT(n, _GAME_OBJ_PROXY_REPEAT_M, func_name)	\
/**/