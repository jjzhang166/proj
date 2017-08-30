#pragma once
/*
@author zhp
@date 2016/4/20 15:09
*/
#include "BC_Base.h"
#include "BC_DataConver.h"
#include <CmnMix/Cmn_CharArray.h>
#include <tuple>
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct BC_Size;

namespace Detail{

	template<typename T, typename type_tag>
	struct BC_SizeImpl{
		static pt_dword get( const T& v ){
			return sizeof(v);
		}
	};

	template<typename T>
	struct BC_SizeImpl<T, BC_TypeTag_Str>{
		static pt_dword get( const T& v ){
			return v.TotalSize();
		}
	};

	template<typename T, int size>
	struct BC_SizeImpl<std::array<T, size>, BC_TypeTag_Cont>{
		template<typename U>
		static pt_dword get( const U& v ){
			static_assert(sizeof(v) == sizeof(T) * size, "");
			return sizeof(v) + sizeof(BC_StableBufCont_PlaceHolder);
		}
	};
	template<typename T>
	struct BC_SizeImpl<T, BC_TypeTag_Cont>{
		static pt_dword get( const T& v ){
			return v.TotalSize();
		}
	};

	template<typename T>
	struct BC_SizeImpl<T, BC_TypeTag_JsonValue>{
		static pt_dword get( const T& v ){
			auto ptr = (const void*)&v;
			return BC_Size<BufCharArray_A>::get(PointerToRef((const BufCharArray_A*)ptr));
		}
	};

	template<typename type_tag>
	struct BC_GetTypeOffset{
		static pt_dword get(){
			return sizeof(BC_TypeIndexT);
		}
	};

	template<typename PairT>
	struct BC_SizeImpl<PairT, BC_TypeTag_Pair>{
		static pt_dword get( const PairT& v ){
			typedef typename PairT::first_type first_type;
			typedef typename PairT::second_type second_type;
			typedef typename BC_DataWrapper<first_type, BC_SizeImpl>::type first_wrapper_type;
			typedef typename BC_DataWrapper<first_type, BC_SizeImpl>::type_tag first_type_tag;
			typedef typename BC_DataWrapper<second_type, BC_SizeImpl>::type second_wrapper_type;
			typedef typename BC_DataWrapper<second_type, BC_SizeImpl>::type_tag second_type_tag;

			auto ptr = (const void*)&v;
			auto item_ptr = (const void*)((pt_csz)ptr + BC_GetTypeOffset<first_type_tag>::get());
			auto cur_size = BC_GetTypeOffset<first_type_tag>::get() + BC_Size<first_wrapper_type>::get(PointerToRef((const first_wrapper_type*)item_ptr));

			item_ptr = (pt_csz)ptr + cur_size + BC_GetTypeOffset<second_type_tag>::get();
			return cur_size + BC_GetTypeOffset<second_type_tag>::get() + BC_Size<second_wrapper_type>::get(PointerToRef((const second_wrapper_type*)item_ptr));
		}
	};

	template<typename TupleT, pt_dword idx>
	struct BC_SizeOfTuple{
		template<typename T>
		static pt_dword get( const T* tuple_item ){
			typedef typename std::tuple_element<std::tuple_size<TupleT>::value - idx, TupleT>::type value_type;
			typedef typename BC_DataWrapper<value_type, BC_SizeOfTuple>::type wrapper_type;
			typedef typename BC_DataWrapper<value_type, BC_SizeOfTuple>::type_tag type_tag;

			auto item_ptr = (const void*)((pt_csz)(const void*)tuple_item + BC_GetTypeOffset<type_tag>::get());
			auto cur_size = BC_GetTypeOffset<type_tag>::get() + BC_Size<wrapper_type>::get(PointerToRef((const wrapper_type*)item_ptr));
			return cur_size + BC_SizeOfTuple<TupleT, idx - 1>::get((pt_csz)tuple_item + cur_size);
		}
	};
	template<typename TupleT>
	struct BC_SizeOfTuple<TupleT, 0>{
		template<typename T>
		static pt_dword get( const T* ){
			return 0;
		}
	};
	template<typename TupleT>
	struct BC_SizeImpl<TupleT, BC_TypeTag_Tuple>{
		static pt_dword get( const TupleT& v ){
			typedef typename BC_RemoveTypeCVR<TupleT>::type TupleType;
			return BC_SizeOfTuple<TupleType, std::tuple_size<TupleType>::value>::get(&v);
		}
	};

}

template<typename T>
struct BC_Size{
	typedef BC_DataWrapper<T, BC_Size> DataWrapper;
	typedef typename DataWrapper::type type;
	typedef typename DataWrapper::type_tag type_tag;
	static pt_dword get( const T& v ){
		auto ptr = (const void*)&v;
		return Detail::BC_SizeImpl<type, type_tag>::get(PointerToRef((const type*)ptr));
	}
};
