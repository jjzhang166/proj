#pragma once
/*
@author zhp
@date 2016/4/20 1:46
*/
#include <xtr1common>
#include <CmnMix/Cmn_ContTraits.h>
#include <json/value.h>
#include <CmnMix/Cmn_TypeId.h>
#include <stdexcept>
//#include <CmnMix/Cmn_TemplateUtility.h>

template<typename T>
struct BC_Size;

#pragma pack(push)
#pragma pack(1)

template<typename ContT>
struct BC_Cont{
	typedef ContT type;
	//typedef typename DeduceContValueType<ContT>::value_type value_type;

	BC_Cont(){}
	template<typename T>
	explicit BC_Cont( const T& cont ) : cont_(cont){}
	explicit BC_Cont( const BC_Cont& cont ) : cont_(cont.cont_){}
	BC_Cont( BC_Cont&& cont ) : cont_(cont.cont_){}
	operator const ContT& () const{
		return cont_;
	}
	BC_Cont& operator = ( const BC_Cont& cont ){
		cont_ = cont.cont_;
		return *this;
	}
	BC_Cont& operator = ( BC_Cont&& cont ){
		cont_ = cont.cont_;
		return *this;
	}
	template<typename T>
	BC_Cont& Add( const T& v ){
		ContAddValue<type>::Add(cont_, v);
		return *this;
	}

	friend bool operator < ( const BC_Cont& lhs, const BC_Cont& rhs ){
		return lhs.cont_ < rhs.cont_;
	}
	friend bool operator == ( const BC_Cont& lhs, const BC_Cont& rhs ){
		return lhs.cont_ == rhs.cont_;
	}
	friend bool operator != ( const BC_Cont& lhs, const BC_Cont& rhs ){
		return lhs.cont_ != rhs.cont_;
	}

	ContT		cont_;
};

#pragma pack(pop)

template<typename T>
struct BC_RemoveTypeCVR{
	typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
};
//////////////////////////////////////////////////////////////////////////
struct BC_TuplePlaceHolder{};
struct BC_POD_PlaceHolder{};
struct BC_Pair_PlaceHolder{};
struct BC_Class_PlaceHolder{};
struct BC_Bitset_PlaceHolder{};
typedef Json::Value BC_JsonValuePlaceHolder;
enum class BC_Enum_PlaceHolder{};
static_assert(sizeof(pt_int) == sizeof(BC_Enum_PlaceHolder), "");
//////////////////////////////////////////////////////////////////////////
namespace bufcall{
	typedef TypeId ClassId;
	typedef void(*RegClassFnT)();

	class ClassProxyMgr;

	namespace Detail{
		template<typename T>
		ClassId BC_GetTypeId(){
			return ClassId(typeid(typename BC_RemoveTypeCVR<T>::type));
		}
	}
}

namespace Detail{
	class BC_Exception : public std::runtime_error{
	protected:
		BC_Exception(const std::string& err) : std::runtime_error(err){}
	};
	class BC_Except_Serial : public BC_Exception{
	public:
		explicit BC_Except_Serial(const std::string& err) : BC_Exception(err){}
	};
	class BC_Except_CheckErr : public BC_Exception{
	public:
		explicit BC_Except_CheckErr(const std::string& err) : BC_Exception(err){}
	};
}