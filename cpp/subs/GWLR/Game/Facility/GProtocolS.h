#pragma once
/*
@author zhp
@date 2014/7/2 19:04
@purpose protocol of game for send/write
*/
#include "GTypes.h"
#include "GProtocolBase.h"

struct CMDS_MsgBase : stBStreamOutput{
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const CMDS_MsgBase& msg );
protected:
	virtual const wchar_t* const GetUriStr() const{
		assert(false);
		return nullptr;
	}
	virtual const std::type_info& GetTypeInfo() const{
		assert(false);
		return typeid(*this);
	};
};

#define CMDS_MsgBase_OUTPUT(derived_t)	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const derived_t& msg ){	\
	return ostm << static_cast<const CMDS_MsgBase&>(msg);	\
}	\
	const std::type_info& GetTypeInfo() const{		\
	return typeid(derived_t);	\
}	\
/**/
#define CMDS_GET_URI_STR( str ) const wchar_t* const GetUriStr() const override{		\
	return str;	\
}	\
/**/

