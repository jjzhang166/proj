#pragma once
/*
@author zhp
@date 2016/4/17 22:29
@purpose runtime type info
*/
#include "BC_Config.h"
#include <CmnMix/Cmn_TypeId.h>
#include <ProtocolX/P_BaseTypeX.h>
#include "BC_Base.h"
//////////////////////////////////////////////////////////////////////////
class BC_TypeInfo{
public:
	virtual TypeId GetTypeId() const = 0;
};

template<typename T>
class BC_TypeInfoImpl;

template <typename T>
struct PC_Type2Index;

template<BC_TypeIndexT>
struct BC_Index2TypeInfo;

template<typename T>
const BC_TypeInfo& BC_GetTypeInfo(){
	return BC_Index2TypeInfo<PC_Type2Index<typename BC_RemoveTypeCVR<T>::type>::Index>::type_info_;
}
//////////////////////////////////////////////////////////////////////////
#define BUFCALL_TYPE_TO_INDEX(x)	\
template<>	\
struct PC_Type2Index<x>{	\
	static const Detail::enBC_TypeIndex Index = Detail::kBCTI_##x;	\
}
/**/

#define BUFCALL_TYPE_INFO(T)	\
template<>	\
class BC_TypeInfoImpl<T> : public BC_TypeInfo{		\
public:	\
	TypeId GetTypeId() const{	\
		static auto id = TypeId(typeid(T));	\
		return id;	\
	}	\
}
/**/
class BC_NullTypeInfo : public BC_TypeInfo{
public:
	TypeId GetTypeId() const;
};
const BC_NullTypeInfo bc_null_type_info;


#define BUFCALL_INDEX_TO_TYPE_INFO(t)	\
template<>	\
struct BC_Index2TypeInfo<Detail::kBCTI_##t>{	\
private:	\
	static const BC_TypeInfoImpl<t>		type_info_impl_;	\
public:	\
	static const BC_TypeInfo&			type_info_;	\
};	\
p_selectany const BC_TypeInfoImpl<t> BC_Index2TypeInfo<Detail::kBCTI_##t>::type_info_impl_;		\
p_selectany const BC_TypeInfo& BC_Index2TypeInfo<Detail::kBCTI_##t>::type_info_ = type_info_impl_;
/**/
//////////////////////////////////////////////////////////////////////////