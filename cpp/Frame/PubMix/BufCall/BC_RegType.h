#pragma once
/*
@author zhp
@date 2016/4/17 18:44
@purpose 注册类型
*/
#include "BC_Config.h"
#include <string>
#include "BC_TypeToString.h"
#include "BC_TypeInfo.h"
#include <CmnMix/Cmn_CharArray.h>
#include "BC_Cont.h"
#include "BC_Base.h"
#include <CmnMix/Cmn_PocoMore.h>


#define BC_REG_ENUM_(t) kBCTI_##t,
/**/
#define BC_REG_TYPE_(t)				\
	BUFCALL_TYPE_TO_STRING(t);		\
	BUFCALL_TYPE_TO_INDEX(t);		\
	BUFCALL_TYPE_INFO(t);			\
	BUFCALL_INDEX_TO_TYPE_INFO(t);
/**/
#define BC_REG_RUNTIME_(t)	&BC_GetTypeInfo<t>(),
/**/


namespace Detail{
enum enBC_TypeIndex{
	BC_REG_ENUM_(pt_char)
	BC_REG_ENUM_(pt_wchar)
	BC_REG_ENUM_(pt_byte)
	BC_REG_ENUM_(pt_bool)
	BC_REG_ENUM_(pt_short)
	BC_REG_ENUM_(pt_word)
	BC_REG_ENUM_(pt_int)
	BC_REG_ENUM_(pt_dword)
	BC_REG_ENUM_(pt_long)
	BC_REG_ENUM_(pt_ulong)
	BC_REG_ENUM_(pt_float)
	BC_REG_ENUM_(pt_dint)
	BC_REG_ENUM_(pt_qword)
	BC_REG_ENUM_(pt_double)
	BC_REG_ENUM_(pt_ldouble)
	BC_REG_ENUM_(BufCharArray_A)
	BC_REG_ENUM_(BufCharArray_W)
	BC_REG_ENUM_(BC_StableBufCont_PlaceHolder)
	BC_REG_ENUM_(BC_TuplePlaceHolder)
	BC_REG_ENUM_(BC_JsonValuePlaceHolder)
	BC_REG_ENUM_(BC_POD_PlaceHolder)
	BC_REG_ENUM_(BC_Pair_PlaceHolder)
	BC_REG_ENUM_(BC_Class_PlaceHolder)
	BC_REG_ENUM_(DateTimeLocal)
	BC_REG_ENUM_(BC_Enum_PlaceHolder)
	BC_REG_ENUM_(BC_Bitset_PlaceHolder)
	kBCTI_Max
};
static_assert(kBCTI_Max <= BC_TypeIndexT(-1), "这里来调整TypeIndex的类型大小");
}

/*还得支持自定义类型的注册、动态类型的注册*/

BC_REG_TYPE_(pt_char)
BC_REG_TYPE_(pt_wchar)
BC_REG_TYPE_(pt_byte)
BC_REG_TYPE_(pt_bool)
BC_REG_TYPE_(pt_short)
BC_REG_TYPE_(pt_word)
BC_REG_TYPE_(pt_int)
BC_REG_TYPE_(pt_dword)
BC_REG_TYPE_(pt_long)
BC_REG_TYPE_(pt_ulong)
BC_REG_TYPE_(pt_float)
BC_REG_TYPE_(pt_dint)
BC_REG_TYPE_(pt_qword)
BC_REG_TYPE_(pt_double)
BC_REG_TYPE_(pt_ldouble)
BC_REG_TYPE_(BufCharArray_A)
BC_REG_TYPE_(BufCharArray_W)
BC_REG_TYPE_(BC_StableBufCont_PlaceHolder)
BC_REG_TYPE_(BC_TuplePlaceHolder)
BC_REG_TYPE_(BC_JsonValuePlaceHolder)
BC_REG_TYPE_(BC_POD_PlaceHolder)
BC_REG_TYPE_(BC_Pair_PlaceHolder)
BC_REG_TYPE_(BC_Class_PlaceHolder)
BC_REG_TYPE_(DateTimeLocal)
BC_REG_TYPE_(BC_Enum_PlaceHolder)
BC_REG_TYPE_(BC_Bitset_PlaceHolder)
//////////////////////////////////////////////////////////////////////////
BUFCALL_TYPE_TO_STRING(StdString);
BUFCALL_TYPE_TO_STRING(StdWstring);
//////////////////////////////////////////////////////////////////////////


struct BC_TypeInfoCont{
	static const BC_TypeInfo& Find( Detail::enBC_TypeIndex ti ){
		static const BC_TypeInfo* all_type_id[] = {
			BC_REG_RUNTIME_(pt_char)
			BC_REG_RUNTIME_(pt_wchar)
			BC_REG_RUNTIME_(pt_byte)
			BC_REG_RUNTIME_(pt_bool)
			BC_REG_RUNTIME_(pt_short)
			BC_REG_RUNTIME_(pt_word)
			BC_REG_RUNTIME_(pt_int)
			BC_REG_RUNTIME_(pt_dword)
			BC_REG_RUNTIME_(pt_long)
			BC_REG_RUNTIME_(pt_ulong)
			BC_REG_RUNTIME_(pt_float)
			BC_REG_RUNTIME_(pt_dint)
			BC_REG_RUNTIME_(pt_qword)
			BC_REG_RUNTIME_(pt_double)
			BC_REG_RUNTIME_(pt_ldouble)
			BC_REG_RUNTIME_(BufCharArray_A)
			BC_REG_RUNTIME_(BufCharArray_W)
			BC_REG_RUNTIME_(BC_StableBufCont_PlaceHolder)
			BC_REG_RUNTIME_(BC_TuplePlaceHolder)
			BC_REG_RUNTIME_(BC_JsonValuePlaceHolder)
			BC_REG_RUNTIME_(BC_POD_PlaceHolder)
			BC_REG_RUNTIME_(BC_Pair_PlaceHolder)
			BC_REG_RUNTIME_(BC_Class_PlaceHolder)
			BC_REG_RUNTIME_(DateTimeLocal)
			BC_REG_RUNTIME_(BC_Enum_PlaceHolder)
			BC_REG_RUNTIME_(BC_Bitset_PlaceHolder)
		};
		if ( ti >= _countof(all_type_id) )
			return bc_null_type_info;
		return *all_type_id[ti];
	}
};
	
//////////////////////////////////////////////////////////////////////////
//没有注册的类型一律当作BC_Class_PlaceHolder
template <typename T>
struct PC_Type2Index{
	static const auto Index = PC_Type2Index<BC_Class_PlaceHolder>::Index;
};
//////////////////////////////////////////////////////////////////////////