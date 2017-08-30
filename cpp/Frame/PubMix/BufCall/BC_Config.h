#pragma once
/*
@author zhp
@date 2016/4/18 20:38
@purpose some config,some base type
*/
#include <ProtocolX/P_BaseTypeX.h>
#include <boost/uuid/uuid.hpp>


//type index的类型，默认只占一个字节大小，即可支持256个类型
typedef pt_byte	BC_TypeIndexT;

namespace Detail{
typedef boost::uuids::uuid BC_Uuid;
}	//Detail