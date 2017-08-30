#if !BOOST_PP_IS_ITERATING
# ifndef ADD_ONCE_BC_DATA_CONVER
#  define ADD_ONCE_BC_DATA_CONVER
/*
@author zhp
@date 2016/4/18 19:06
@purpose data converter
*/
#  include <boost/mpl/long.hpp>
#  include <boost/preprocessor/repetition/repeat.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>
#  include <boost/type_traits/has_left_shift.hpp>
#  include <boost/type_traits/has_right_shift.hpp>
#  include <tuple>
#  include <vector>
#  include <array>
#  include <xtr1common>
#  include <type_traits>
#  include <bitset>

#  include "BC_TypeInfo.h"
#  include <CmnMix/Cmn_CharArray.h>
#  include <ProtocolX/P_DefinesX.h>
#  include "BC_Cont.h"
#  include "BC_Base.h"
#  include <CmnMix/Cmn_TemplateUtility.h>
#  include <json/reader.h>
#  include <json/writer.h>
#  include <CmnMix/Cmn_PocoMore.h>
//////////////////////////////////////////////////////////////////////////
struct BC_DataWrapperPolicyNil{};
struct BC_DataWrapperPolicy_NoDoValid{};

template<typename t_info, typename Policy>
struct BC_Algo_DoValidNext;
//////////////////////////////////////////////////////////////////////////
/*
这个是类型转换接口。
对于第二个参数Policy，这里有个编译期BUG，最好每次都给Policy填不同的类型。
*/
template<typename T, typename Policy = BC_DataWrapperPolicyNil>
struct BC_DataWrapper;

template<typename T, typename Policy>
struct BC_TypeConverter;

template<typename T, typename Policy>
struct BC_TypeConverter_0;

template<int type_idx, typename ImplT_, typename T, typename Policy>
struct BC_TypeConverter_1;

template<typename T>
struct BC_DataConverter;

template<typename T>
struct BC_DataHolder;

template<typename ReadT, typename WriteT>
struct BC_Algo;
//////////////////////////////////////////////////////////////////////////
template<typename T, typename t_info, typename SinkT>
struct BC_Algo_DoAsT_Normal;

template<typename T>
struct BC_Algo_DoRead_Normal;

template<typename T>
struct BC_Algo_DoRead_Array;
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct BC_Algo_WriteValue_Normal;
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Policy>
struct BC_DataWrapper{
private:
	typedef BC_TypeConverter<T, Policy> TypeConver_;
	typedef typename TypeConver_::ImplT ConverterImplT;

public:
	//type of wrapper
	typedef typename TypeConver_::type type;
	//TypeIndex info
	typedef typename TypeConver_::t_info t_info;
	//type tag
	typedef typename TypeConver_::type_tag type_tag;

	template<typename Stream>
	static auto AsT( Stream& stm ) ->decltype(ConverterImplT().AsT(stm)){
		return ConverterImplT().AsT(stm);
	}
	template<typename Stream>
	static void Write(Stream& stm, const T& value){
		BC_Algo_DoValidNext<t_info, Policy>::WriteTypeIdx(stm);
		WriteValue(stm, value);
	}
	template<typename Stream>
	static void WriteValue(Stream& stm, const T& value){
		ConverterImplT().WriteValue(stm, value);
	}
};

namespace Detail{
	template<bool BoolT, typename T, typename Policy>
	struct BC_Enum{
		typedef BC_TypeConverter_0<T, Policy> type;
	};
}
template<typename T, typename Policy>
struct BC_TypeConverter{
private:
	typedef typename BC_RemoveTypeCVR<T>::type type_;
	typedef typename ::Detail::BC_Enum<std::is_enum<type_>::value, type_, Policy>::type ImplT_;

public:
	typedef typename BC_TypeConverter_1<PC_Type2Index<typename ImplT_::t_info>::Index, ImplT_, type_, Policy>::ImplT ImplT;
	typedef typename ImplT::type type;
	typedef typename ImplT::t_info t_info;
	typedef typename ImplT::type_tag type_tag;
};

template<typename T, typename Policy>
struct BC_TypeConverter_0 : BC_Algo<BC_Algo_DoAsT_Normal<T, BC_Algo_DoValidNext<T, Policy>, BC_Algo_DoRead_Normal<T> >,
	BC_Algo_WriteValue_Normal<T> >{
	typedef T type;
	typedef T t_info;
	typedef T type_tag;
};

template<int type_idx, typename ImplT_, typename T, typename Policy>
struct BC_TypeConverter_1{
	typedef ImplT_ ImplT;
};

template<typename T>
struct BC_DataConverter{
	typedef const typename BC_RemoveTypeCVR<T>::type& type;
};

template<typename T>
struct BC_DataConverter<BC_DataHolder<T> >{
	typedef const typename BC_RemoveTypeCVR<T>::type& type;
};

template<typename T>
struct BC_DataHolder{
	BC_DataHolder(){
		ptr_ = nullptr;
	}
	BC_DataHolder& operator = ( const T* ptr ){
		ptr_ = ptr;
		return *this;
	}
	operator const T& () const{
		assert(ptr_);
		return *ptr_;
	}
	bool Valid() const{
		return !!ptr_;
	}

	const T*	ptr_;
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
BC_TypeIndexT BC_GetTypeIndex(){
	struct GetTypeIndexTag{};
	return PC_Type2Index<typename BC_DataWrapper<T, GetTypeIndexTag>::t_info>::Index;
}

template<typename T>
struct BC_Algo_WriteValue_Normal{
	template<typename Stream>
	static void WriteValue(Stream& stm, const T& v){
		stm.WriteValue(&v, sizeof(v));
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename ReadT, typename WriteT>
struct BC_Algo : ReadT, WriteT{
	typedef ReadT read_type;
	typedef WriteT write_type;
};
//////////////////////////////////////////////////////////////////////////
template<typename T, typename DoNext, typename SinkT>
struct BC_Algo_DoAsT_Normal{
	template<typename Stream>
	static BC_DataHolder<T> AsT( Stream& stm ){
		BC_DataHolder<T> data;
		if ( !DoNext::DoTypeInfo(stm) )
			return data;
		data = stm.As<T>(SinkT());
		return data;
	}
};

template<typename T>
struct BC_Algo_DoRead_Normal{
	template<typename Stm>
	static bool DoCheckSize( const Stm& stm, const T& value ){
		return stm.LeftSize() >= sizeof(value);
	}
	template<typename Stm>
	static bool DoSkip( Stm& stm, const T& value ){
		return stm.Skip(sizeof(value));
	}
};
template<typename T>
struct BC_Algo_DoRead_Array{
	template<typename Stm>
	static bool DoCheckSize( const Stm& stm, const T& value ){
		return value.Valid(stm.LeftSize());
	}
	template<typename Stm>
	static bool DoSkip( Stm& stm, const T& value ){
		return stm.Skip(value.TotalSize());
	}
};
//////////////////////////////////////////////////////////////////////////
struct BC_TypeTag_Str{};
struct BC_TypeTag_Cont{};
struct BC_TypeTag_Tuple{};
struct BC_TypeTag_JsonValue{};
struct BC_TypeTag_Pair{};
struct BC_TypeTag_Class{};
struct BC_TypeTag_Bitset{};
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Policy>
struct BC_TypeConverter_0<BufCharArray<T>, Policy>{
	typedef BufCharArray<T> type;
	typedef BufCharArray<T> t_info;
	typedef BC_TypeTag_Str type_tag;
};

template<typename T, typename Policy>
struct BC_TypeConverter_0<BC_StableBufCont<T>, Policy>{
	typedef BC_StableBufCont<T> type;
	typedef BC_StableBufCont_PlaceHolder t_info;
	typedef BC_TypeTag_Cont type_tag;
};
//////////////////////////////////////////////////////////////////////////
template<typename t_info, typename Policy>
struct BC_Algo_DoValidNext{
	template<typename Stm>
	static bool DoTypeInfo( Stm& stm ){
		return stm.ValidNextT<t_info>();
	}
	template<typename Stm>
	static void WriteTypeIdx(Stm& stm){
		stm.WriteTypeIdx(PC_Type2Index<t_info>::Index);
	}
};

template<typename t_info>
struct BC_Algo_DoValidNext<t_info, BC_DataWrapperPolicy_NoDoValid>{
	template<typename Stm>
	static bool DoTypeInfo( Stm& stm ){
		return true;
	}
	template<typename Stm>
	static void WriteTypeIdx(Stm&){
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct BC_DataConverter<BC_DataHolder<BufCharArray<T> > >{
	typedef const BufCharArray<T>& type;
};

template<typename CharT>
struct BC_Algo_WriteValue_Char{
	template<typename Stream>
	static void WriteValue(Stream& stm, const CharT* v){
		if (!v)
			v = StdEx::TEmptyStr<CharT>();
		typename BC_DataWrapper<const CharT*, BC_Algo_WriteValue_Char>::type tmp(StdEx::TStrLen(v));
		stm.WriteValue(&tmp, sizeof(tmp));
		stm.WriteValue(v, tmp.CharSize());
	}
};

template<typename StringT>
struct BC_Algo_WriteValue_StdStr{
	template<typename Stream>
	static void WriteValue(Stream& stm, const StringT& v){
		typename BC_DataWrapper<StringT, BC_Algo_WriteValue_StdStr>::type tmp(v.size());
		stm.WriteValue(&tmp, sizeof(tmp));
		stm.WriteValue(v.c_str(), tmp.CharSize());
	}
};

template<typename StringT>
struct BC_Algo_WriteValue_CString{
	template<typename Stream>
	static void WriteValue(Stream& stm, const StringT& v){
		typename BC_DataWrapper<StringT, BC_Algo_WriteValue_CString>::type tmp(v.GetLength());
		stm.WriteValue(&tmp, sizeof(tmp));
		stm.WriteValue(v.GetString(), tmp.CharSize());
	}
};

template<typename CharT, int size>
struct BC_Algo_WriteValue_CharArray{
	template<typename Stream>
	static void WriteValue(Stream& stm, const CharT* v){
		static_assert(size > 0, "");
		if (!v)
		{
			assert(false);
			return;
		}
		auto char_size = size;
		if (*(v + size - 1) == 0)
			--char_size;
		typename BC_DataWrapper<const CharT*, BC_Algo_WriteValue_CharArray>::type tmp(char_size);
		stm.WriteValue(&tmp, sizeof(tmp));
		stm.WriteValue(v, size * sizeof(CharT));
		if (char_size == size)
			stm.Fill(sizeof(CharT), 0);
	}
};

template<typename CharT, typename Policy, typename Algo_WriteValue>
struct BC_TypeConverter_Char : BC_Algo<BC_Algo_DoAsT_Normal<BufCharArray<CharT>, BC_Algo_DoValidNext<BufCharArray<CharT>, Policy>, BC_Algo_DoRead_Array<BufCharArray<CharT> > >,
	Algo_WriteValue >{
	typedef BufCharArray<CharT> type;
	typedef BufCharArray<CharT> t_info;
	typedef BC_TypeTag_Str type_tag;
};

template<typename Policy>
struct BC_TypeConverter<const char*, Policy>{
	typedef BC_TypeConverter_Char<char, Policy, BC_Algo_WriteValue_Char<char> > ImplT;
	typedef typename ImplT::type type;
	typedef typename ImplT::t_info t_info;
	typedef typename ImplT::type_tag type_tag;
};

template<typename Policy>
struct BC_TypeConverter<const wchar_t*, Policy>{
	typedef BC_TypeConverter_Char<wchar_t, Policy, BC_Algo_WriteValue_Char<wchar_t> > ImplT;
	typedef typename ImplT::type type;
	typedef typename ImplT::t_info t_info;
	typedef typename ImplT::type_tag type_tag;
};

//char或wchar_t数组就是一种字符串
template<pt_dword size, typename Policy>
struct BC_TypeConverter_0<char[size], Policy> : BC_TypeConverter_Char<char, Policy, BC_Algo_WriteValue_CharArray<char, size> >{
};

template<pt_dword size, typename Policy>
struct BC_TypeConverter_0<wchar_t[size], Policy> : BC_TypeConverter_Char<wchar_t, Policy, BC_Algo_WriteValue_CharArray<wchar_t, size> >{
};

template<typename CharT, typename _Traits, typename _Alloc, typename Policy>
struct BC_TypeConverter_0<std::basic_string<CharT, _Traits, _Alloc>, Policy> : BC_TypeConverter_Char<CharT, Policy, BC_Algo_WriteValue_StdStr<std::basic_string<CharT, _Traits, _Alloc> > >{
};

template<typename CharT, typename StringTraits, typename Policy>
struct BC_TypeConverter_0<ATL::CStringT<CharT, StringTraits>, Policy> : BC_TypeConverter_Char<CharT, Policy, BC_Algo_WriteValue_CString<ATL::CStringT<CharT, StringTraits> > >{
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct BC_DataConverter<BC_DataHolder<BC_StableBufCont<T> > >{
	typedef const BC_StableBufCont<T>& type;
};

template<typename ValueType>
const BC_TypeInfo& BC_GetValueTypeInfo(){
	struct tag{};
	return BC_GetTypeInfo<typename BC_DataWrapper<ValueType, tag>::t_info>();
}

template<typename AlgoAsT, typename T, typename t_info, typename value_type, typename Policy, typename SinkT>
struct BC_Algo_DoAsT_StableCont{
	template<typename Stream>
	static BC_DataHolder<T> AsT( Stream& stm ){
		BC_DataHolder<T> data;
		if ( !BC_Algo_DoValidNext<t_info, Policy>::DoTypeInfo(stm) )
			return data;
		auto cont_proxy = (T*)(void*)stm.As<T>(SinkT());
		if (!cont_proxy)
			return data;
		auto& the_type_info = BC_TypeInfoCont::Find((Detail::enBC_TypeIndex)cont_proxy->GetValueTypeIdx());
		auto& t_type_info = BC_GetValueTypeInfo<value_type>();
		if (t_type_info.GetTypeId() != the_type_info.GetTypeId())
		{
			assert(false);
			return data;
		}
		data = cont_proxy;
		return data;
	}
};

template<typename ValueType>
struct BC_DataHolder_Cont{
	BC_DataHolder_Cont(){
		valid_ = false;
	}
	template<typename ContT>
	operator BC_Cont<ContT> () const{
		BC_Cont<ContT> res;
		if ( !valid_ )
		{
			assert(false);
			return res;
		}
		auto it_end = cont_.end();
		for ( auto it = cont_.begin(); it != it_end; ++it )
			res.Add(*it);
		return res;
	}
	template<typename T, typename AllocT>
	operator std::vector<T, AllocT> () const{
		std::vector<T, AllocT> res;
		if ( !valid_ )
		{
			assert(false);
			return res;
		}
		auto it_end = cont_.end();
		for ( auto it = cont_.begin(); it != it_end; ++it )
			res.push_back(*it);
		return res;
	}
	operator const std::vector<ValueType>& () const{
		if ( !valid_ )
		{
			assert(false);
			return cont_;
		}
		return cont_;
	}
	bool Valid() const{
		return valid_;
	}

	std::vector<ValueType>	cont_;
	bool					valid_;
};

struct BC_Algo_DoAsT_StableContCpy{};
template<typename WrapperType, typename t_info, typename value_type, typename Policy, typename SinkT>
struct BC_Algo_DoAsT_StableCont<BC_Algo_DoAsT_StableContCpy, WrapperType, t_info, value_type, Policy, SinkT>{
	typedef typename WrapperType::value_type ContValueType;
	typedef typename WrapperType::size_type ContSizeType;

	template<typename Stream>
	static BC_DataHolder_Cont<value_type> AsT( Stream& stm ){
		BC_DataHolder_Cont<value_type> data;
		if ( !BC_Algo_DoValidNext<t_info, Policy>::DoTypeInfo(stm) )
			return data;
		auto cont_proxy = (const WrapperType*)(const void*)stm.As<BC_StableBufCont_PlaceHolder>(BC_Algo_DoRead_Normal<BC_StableBufCont_PlaceHolder>());
		if ( !cont_proxy )
			return data;
		auto& the_type_info = BC_TypeInfoCont::Find((Detail::enBC_TypeIndex)cont_proxy->GetValueTypeIdx());
		auto& t_type_info = BC_GetValueTypeInfo<value_type>();
		if (t_type_info.GetTypeId() != the_type_info.GetTypeId())
		{
			assert(false);
			return data;
		}

		for (auto value_cnt = cont_proxy->ValueCnt(); value_cnt > 0; --value_cnt)
		{
			auto v = BC_DataWrapper<ContValueType, BC_DataWrapperPolicy_NoDoValid>::AsT(stm);
			if ( !v.Valid() )
				return data;
			data.cont_.push_back(static_cast<typename BC_DataConverter<decltype(v)>::type>(v));
		}

		data.valid_ = true;
		return data;
	}
};

template<typename value_type, typename value_type_tag>
struct BC_TypeConver_ContBase_0{
	typedef void AlgoAsT;
	typedef BC_StableBufCont<typename BC_DataWrapper<value_type, BC_TypeConver_ContBase_0>::type> type;
};

template<typename TupleT>
struct BC_TypeConver_ContBase_0<TupleT, BC_TypeTag_Tuple>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<typename BC_DataWrapper<TupleT, BC_TypeConver_ContBase_0>::type> type;
};

template<typename T, pt_word size>
struct BC_TypeConver_ContBase_0<std::array<T, size>, BC_TypeTag_Cont>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<std::array<T, size> > type;
};

template<typename T, typename AllocT>
struct BC_TypeConver_ContBase_0<std::vector<T, AllocT>, BC_TypeTag_Cont>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<std::vector<T, AllocT> > type;
};

template<typename value_type>
struct BC_TypeConver_ContBase_0<value_type, BC_TypeTag_Cont>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<value_type> type;
};

template<>
struct BC_TypeConver_ContBase_0<Json::Value, BC_TypeTag_JsonValue>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<typename BC_DataWrapper<Json::Value, BC_TypeConver_ContBase_0>::type> type;
};

template<typename PairT>
struct BC_TypeConver_ContBase_0<PairT, BC_TypeTag_Pair>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<typename BC_DataWrapper<PairT, BC_TypeConver_ContBase_0>::type> type;
};

template<typename T>
struct BC_TypeConver_ContBase_0<T, BC_TypeTag_Class>{
	typedef BC_Algo_DoAsT_StableContCpy AlgoAsT;
	typedef BC_StableBufCont<typename BC_DataWrapper<T, BC_TypeConver_ContBase_0>::type> type;
};

template<typename ContT>
struct BC_TypeConver_ContBase{
private:
	typedef typename BC_RemoveTypeCVR<ContT>::type ThinCont;

public:
	typedef typename BC_RemoveTypeCVR<typename ThinCont::value_type>::type value_type;

private:
	typedef BC_DataWrapper<value_type, BC_TypeConver_ContBase> ValueTypeOfWrapper;
	typedef BC_TypeConver_ContBase_0<value_type, typename ValueTypeOfWrapper::type_tag> ImplT;

public:
	typedef typename ImplT::AlgoAsT AlgoAsT;
	typedef typename ImplT::type type;
	typedef BC_StableBufCont_PlaceHolder t_info;
	typedef BC_TypeTag_Cont type_tag;
};

template<typename ContT, typename ValueType>
struct BC_Algo_WriteValue_Cont{
	template<typename Stream>
	static void WriteValue(Stream& stm, const ContT& cont){
		BC_StableBufCont_PlaceHolder tmp(BC_GetTypeIndex<ValueType>());
		auto old_size = stm.GetStrBuffer().size();
		stm.WriteValue(&tmp, sizeof(tmp));
		BC_StableBufCont_PlaceHolder::size_type cnt = 0;
		for (auto& v : cont)
		{
			BC_DataWrapper<ValueType, BC_Algo_WriteValue_Cont>::WriteValue(stm, v);
			++cnt;
		}
		auto ptr = (BC_StableBufCont_PlaceHolder*)(void*)((char*)stm.GetStrBuffer().c_str() + old_size);
		ptr->value_cnt_ = cnt;
	}
};

template<typename ContT, typename Policy>
struct BC_TypeConverter_0<BC_Cont<ContT>, Policy> : BC_Algo<BC_Algo_DoAsT_StableCont<typename BC_TypeConver_ContBase<ContT>::AlgoAsT, typename BC_TypeConver_ContBase<ContT>::type,
				typename BC_TypeConver_ContBase<ContT>::t_info, typename BC_TypeConver_ContBase<ContT>::value_type, Policy, BC_Algo_DoRead_Array<typename BC_TypeConver_ContBase<ContT>::type> >
				, BC_Algo_WriteValue_Cont<ContT, typename BC_TypeConver_ContBase<ContT>::value_type> >,
			BC_TypeConver_ContBase<ContT>{
};

template<typename ContT, typename Policy>
struct BC_TypeConver_Cont_Vec_Base : BC_Algo<BC_Algo_DoAsT_StableCont<typename BC_TypeConver_ContBase<ContT>::AlgoAsT, typename BC_TypeConver_ContBase<ContT>::type,
	typename BC_TypeConver_ContBase<ContT>::t_info, typename BC_TypeConver_ContBase<ContT>::value_type, Policy, BC_Algo_DoRead_Array<typename BC_TypeConver_ContBase<ContT>::type> >
	, BC_Algo_WriteValue_Cont<ContT, typename BC_TypeConver_ContBase<ContT>::value_type> >,
	BC_TypeConver_ContBase<ContT>{
};

template<typename T, typename AllocT, typename Policy>
struct BC_TypeConverter_0<std::vector<T, AllocT>, Policy> : BC_TypeConver_Cont_Vec_Base<std::vector<T, AllocT>, Policy>{
};

struct BC_Algo_DoAsT_StableContArray{};
template<typename T, typename t_info, typename value_type, typename Policy, typename SinkT>
struct BC_Algo_DoAsT_StableCont<BC_Algo_DoAsT_StableContArray, T, t_info, value_type, Policy, SinkT>{
	template<typename Stream>
	static BC_DataHolder<T> AsT( Stream& stm ){
		BC_DataHolder<T> data;
		if ( !BC_Algo_DoValidNext<t_info, Policy>::DoTypeInfo(stm) )
			return data;
		auto cont_proxy = (const BC_StableBufCont_PlaceHolder*)(const void*)stm.As<BC_StableBufCont_PlaceHolder>(BC_Algo_DoRead_Normal<BC_StableBufCont_PlaceHolder>());
		if (!cont_proxy)
			return data;
		auto& the_type_info = BC_TypeInfoCont::Find((Detail::enBC_TypeIndex)cont_proxy->GetValueTypeIdx());
		auto& t_type_info = BC_GetValueTypeInfo<value_type>();
		if (t_type_info.GetTypeId() != the_type_info.GetTypeId())
		{
			assert(false);
			return data;
		}
		auto ptr_array = stm.As<T>(BC_Algo_DoRead_Normal<T>());
		if (ptr_array->max_size() != cont_proxy->ValueCnt())
		{
			assert(false);
			return data;
		}
		data = ptr_array;
		return data;
	}
};

template<typename T, int size>
struct BC_TypeConver_StdArrayBase{
	typedef typename BC_RemoveTypeCVR<T>::type value_type;
	typedef std::array<value_type, size> type;
	typedef BC_StableBufCont_PlaceHolder t_info;
	typedef BC_TypeTag_Cont type_tag;
};

template<typename T, int size, typename Policy>
struct BC_TypeConverter_0<std::array<T, size>, Policy> : BC_Algo<BC_Algo_DoAsT_StableCont<BC_Algo_DoAsT_StableContArray, typename BC_TypeConver_StdArrayBase<T, size>::type,
	typename BC_TypeConver_StdArrayBase<T, size>::t_info, typename BC_TypeConver_StdArrayBase<T, size>::value_type, Policy, void>
	, BC_Algo_WriteValue_Cont<std::array<T, size>, T> >,
	BC_TypeConver_StdArrayBase<T, size>{
};

template<typename T, int size>
struct BC_TypeConver_ArrayBase{
	typedef typename BC_RemoveTypeCVR<T>::type value_type;
	typedef T (type)[size];
	typedef BC_StableBufCont_PlaceHolder t_info;
	typedef BC_TypeTag_Cont type_tag;
};
template<typename T, int size, typename Policy>
struct BC_TypeConverter_0<T[size], Policy> : BC_Algo<BC_Algo_DoAsT_StableCont<BC_Algo_DoAsT_StableContArray, typename BC_TypeConver_ArrayBase<T, size>::type,
	typename BC_TypeConver_ArrayBase<T, size>::t_info, typename BC_TypeConver_ArrayBase<T, size>::value_type, Policy, void>
	, BC_Algo_WriteValue_Cont<typename BC_TypeConver_ArrayBase<T, size>::type, T> >,
	BC_TypeConver_ArrayBase<T, size>{};
//////////////////////////////////////////////////////////////////////////
template<>
struct BC_DataHolder<Json::Value>{
	BC_DataHolder(){
		valid_ = false;
	}
	operator const Json::Value& () const{
		if ( !valid_ )
		{
			assert(false);
			return holder_;
		}
		return holder_;
	}
	bool Valid() const{
		return valid_;
	}

	Json::Value	holder_;
	bool		valid_;
};

template<typename T, typename t_info, typename Policy>
struct BC_Algo_DoAsT_JsonValue{
	template<typename Stream>
	static BC_DataHolder<T> AsT( Stream& stm ){
		BC_DataHolder<T> data;
		if ( !BC_Algo_DoValidNext<t_info, Policy>::DoTypeInfo(stm) )
			return data;
		auto str = stm.As<BufCharArray_A>(BC_Algo_DoRead_Array<BufCharArray_A>());
		if ( !str )
			return data;
		Json::Reader j_read;
		auto begin = str->c_str();
		if ( !j_read.parse(begin, begin + str->CharCnt(), data.holder_, false) )
			return data;
		data.valid_ = true;
		return data;
	}
};

struct BC_TypeConver_JsonBase{
	typedef Json::Value type;
	typedef BC_JsonValuePlaceHolder t_info;
	typedef BC_TypeTag_JsonValue type_tag;
};

struct BC_Algo_WriteValue_JsonValue{
	template<typename Stream>
	static void WriteValue(Stream& stm, const Json::Value& v){
		Json::FastWriter writer;
		const auto& str = writer.write(v);
		BC_DataWrapper<decltype(str), BC_Algo_WriteValue_JsonValue>::WriteValue(stm, str);
	}
};

template<typename Policy>
struct BC_TypeConverter_0<Json::Value, Policy> : BC_Algo<BC_Algo_DoAsT_JsonValue<BC_TypeConver_JsonBase::type, BC_TypeConver_JsonBase::t_info, Policy>
	, BC_Algo_WriteValue_JsonValue>,
	BC_TypeConver_JsonBase{
};
//////////////////////////////////////////////////////////////////////////
template<typename T, typename Policy>
struct BC_TypeConverter_POD_Base : BC_Algo<BC_Algo_DoAsT_Normal<T, BC_Algo_DoValidNext<BC_POD_PlaceHolder, Policy>, BC_Algo_DoRead_Normal<T> >,
	BC_Algo_WriteValue_Normal<T> >{
	//static_assert(std::is_pod<T>::value == true, "T must be a POD type");

	typedef T type;
	typedef BC_POD_PlaceHolder t_info;
	typedef T type_tag;
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct BC_DataHolder_Class{
	BC_DataHolder_Class(){
		valid_ = false;
	}
	operator const T& () const{
		if (!valid_)
		{
			assert(false);
			return holder_;
		}
		return holder_;
	}
	bool Valid() const{
		return valid_;
	}

	T		holder_;
	bool	valid_;
};
namespace Detail{
	template<typename StmT, typename T>
	struct BC_ClassSerializeTraits{
		static const auto has_left_shift = boost::has_left_shift<StmT, T>::value;
		static const auto has_right_shift = boost::has_right_shift<StmT, T>::value;
	};

	template<bool HasLeftShift>
	struct BC_ClassSerializeW{
		template<typename StmT, typename T>
		static void Do(StmT& stm, T& v){
			v.serialize(stm);
		}
	};
	template<>
	struct BC_ClassSerializeW<true>{
		template<typename StmT, typename T>
		static void Do(StmT& stm, const T& v){
			stm << v;
		}
	};
	template<bool HasRightShift>
	struct BC_ClassSerializeR{
		template<typename StmT, typename T>
		static void Do(StmT& stm, T& v){
			v.serialize(stm);
		}
	};
	template<>
	struct BC_ClassSerializeR<true>{
		template<typename StmT, typename T>
		static void Do(StmT& stm, T& v){
			stm >> v;
		}
	};
	struct BC_ClassSerialize{
		template<typename Stm, typename T>
		static void DoWrite(Stm& stm, T& v){
			BC_ClassSerializeW<BC_ClassSerializeTraits<Stm, T>::has_left_shift>::Do(stm, v);
		}
		template<typename Stm, typename T>
		static void DoRead(Stm& stm, T& v){
			BC_ClassSerializeR<BC_ClassSerializeTraits<Stm, T>::has_right_shift>::Do(stm, v);
		}
	};
}
template<typename T, typename DoNext>
struct BC_Algo_DoAsT_Class{
	template<typename Stream>
	static BC_DataHolder_Class<T> AsT(Stream& stm){
		BC_DataHolder_Class<T> data;
		if (!DoNext::DoTypeInfo(stm))
			return data;
		typedef typename Stream::StreamWrapper StreamWrapperType;
		StreamWrapperType stm_wrapper(stm);
		::Detail::BC_ClassSerialize::DoRead(stm_wrapper, data.holder_);
		if (stm_wrapper.Valid())
			data.valid_ = true;
		return data;
	}
};

template<typename T>
struct BC_Algo_WriteValue_Class{
	template<typename Stream>
	static void WriteValue(Stream& stm, const T& v){
		typedef typename Stream::StreamWrapper StreamWrapperType;
		StreamWrapperType stm_wrapper(stm);
		::Detail::BC_ClassSerialize::DoWrite(stm_wrapper, const_cast<T&>(v));
	}
};

template<typename T, typename Policy>
struct BC_TypeConverter_Class_Base : BC_Algo<BC_Algo_DoAsT_Class<T, BC_Algo_DoValidNext<BC_Class_PlaceHolder, Policy> >,
	BC_Algo_WriteValue_Class<T> >{
	typedef T type;
	typedef BC_Class_PlaceHolder t_info;
	typedef BC_TypeTag_Class type_tag;
};

template<typename ImplT_, typename T, typename Policy>
struct BC_TypeConverter_1<PC_Type2Index<BC_Class_PlaceHolder>::Index, ImplT_, T, Policy>{
	typedef BC_TypeConverter_Class_Base<T, Policy> ImplT;
};
//////////////////////////////////////////////////////////////////////////
struct BC_TypeWrapper_Datetime{
	operator const std::time_t&() const{
		return time_;
	}
	operator DateTimeLocal() const{
		return DateTimeLocal(Poco::Timestamp::fromEpochTime(time_));
	}
	operator Poco::DateTime() const{
		return Poco::DateTime(Poco::Timestamp::fromEpochTime(time_));
	}
	std::time_t		time_;
};
static_assert(sizeof(BC_TypeWrapper_Datetime) == sizeof(std::time_t), "");
struct BC_Algo_WriteValue_Datetime{
	template<typename Stream>
	static void WriteValue(Stream& stm, const DateTimeLocal& v){
		const auto& time = v.GetEpochTime();
		stm.WriteValue(&time, sizeof(time));
	}
	template<typename Stream>
	static void WriteValue(Stream& stm, const Poco::DateTime& v){
		const auto& time = v.GetEpochTime();
		stm.WriteValue(&time, sizeof(time));
	}
};
template<typename Policy>
struct BC_TypeConverter_Datetime_Base : BC_Algo<BC_Algo_DoAsT_Normal<BC_TypeWrapper_Datetime, BC_Algo_DoValidNext<DateTimeLocal, Policy>, BC_Algo_DoRead_Normal<BC_TypeWrapper_Datetime> >,
	BC_Algo_WriteValue_Datetime>{
	typedef BC_TypeWrapper_Datetime type;
	typedef DateTimeLocal t_info;
	typedef DateTimeLocal type_tag;
};
template<typename Policy>
struct BC_TypeConverter_0<DateTimeLocal, Policy> : BC_TypeConverter_Datetime_Base<Policy>{
};
template<typename Policy>
struct BC_TypeConverter_0<Poco::DateTime, Policy> : BC_TypeConverter_Datetime_Base<Policy>{
};
//////////////////////////////////////////////////////////////////////////
template<std::size_t _Bits, typename Policy>
struct BC_TypeConverter_0<std::bitset<_Bits>, Policy>
	: BC_Algo<BC_Algo_DoAsT_Normal<std::bitset<_Bits>, BC_Algo_DoValidNext<BC_Bitset_PlaceHolder, Policy>,
	BC_Algo_DoRead_Normal<std::bitset<_Bits> > >,
	BC_Algo_WriteValue_Normal<std::bitset<_Bits> > >{
	typedef std::bitset<_Bits> type;
	typedef BC_Bitset_PlaceHolder t_info;
	typedef BC_TypeTag_Bitset type_tag;
};
//////////////////////////////////////////////////////////////////////////
namespace Detail{
	template<typename T, typename Policy>
	struct BC_TypeConverter_Enum : BC_Algo<BC_Algo_DoAsT_Normal<T, BC_Algo_DoValidNext<BC_Enum_PlaceHolder, Policy>, BC_Algo_DoRead_Normal<T> >,
		BC_Algo_WriteValue_Normal<T> >{
		typedef T type;
		typedef BC_Enum_PlaceHolder t_info;
		typedef T type_tag;
	};

	template<typename T, typename Policy>
	struct BC_Enum<true, T, Policy>{
		static_assert(sizeof(pt_int) == sizeof(T), "");
		typedef BC_TypeConverter_Enum<T, Policy> type;
	};
}
//////////////////////////////////////////////////////////////////////////
template<typename PairT>
struct BC_DataHolder_Pair{
	typedef PairT type;

	BC_DataHolder_Pair(){
		is_succeed_ = false;
	}
	template<typename Stm>
	void DoRead( Stm& stm ){
		auto v_first = BC_DataWrapper<typename type::first_type, BC_DataHolder_Pair>::AsT(stm);
		if ( !v_first.Valid() )
			return;
		auto v_second = BC_DataWrapper<typename type::second_type, BC_DataHolder_Pair>::AsT(stm);
		if ( !v_second.Valid() )
			return;
		SetValueAux<typename type::first_type>(value_.first, static_cast<typename BC_DataConverter<decltype(v_first)>::type>(v_first));
		SetValueAux<typename type::second_type>(value_.second, static_cast<typename BC_DataConverter<decltype(v_second)>::type>(v_second));
		is_succeed_ = true;
	}
	operator const PairT& () const{
		assert(is_succeed_);
		return value_;
	}
	bool Valid() const{
		return is_succeed_;
	}

	PairT	value_;
	bool	is_succeed_;
};

template<typename PairT, typename DoValid>
struct BC_Algo_DoAsT_Pair{
	template<typename Stream>
	static BC_DataHolder_Pair<PairT> AsT( Stream& stm ){
		BC_DataHolder_Pair<PairT> data;
		if ( !DoValid::DoTypeInfo(stm) )
			return data;
		data.DoRead(stm);
		return data;
	}
};

template<typename PairT>
struct BC_Algo_WriteValue_Pair{
	template<typename Stream>
	static void WriteValue(Stream& stm, const PairT& v){
		BC_DataWrapper<decltype(v.first), BC_Algo_WriteValue_Pair>::Write(stm, v.first);
		BC_DataWrapper<decltype(v.second), BC_Algo_WriteValue_Pair>::Write(stm, v.second);
	}
};

template<typename PairT, typename Policy>
struct BC_TypeConver_PairBase : BC_Algo<BC_Algo_DoAsT_Pair<PairT, BC_Algo_DoValidNext<BC_Pair_PlaceHolder, Policy> >,
	BC_Algo_WriteValue_Pair<PairT> >{
	typedef PairT type;
	typedef BC_Pair_PlaceHolder t_info;
	typedef BC_TypeTag_Pair type_tag;
};

template<typename T1, typename T2, typename Policy>
struct BC_TypeConverter_0<std::pair<T1, T2>, Policy> : BC_TypeConver_PairBase<std::pair<T1, T2>, Policy>{
};
//////////////////////////////////////////////////////////////////////////
template<typename TupleT>
struct BC_DataHolder_Tuple{
	typedef TupleT type;

	BC_DataHolder_Tuple(){
		is_succeed_ = false;
	}
#  define BUF_CALL_DATA_CONVER_TUPLE_DO_READ
	/**/
#  define BUF_CALL_DATA_CONVER_DEFINE(_, n, __)	\
	auto p##n = BC_DataWrapper<typename std::tuple_element<n, type>::type, BC_DataHolder_Tuple>::AsT(stm);	\
	if ( !p##n.Valid() )	\
		return;
	/**/
#  define BUF_CALL_DATA_CONVER_ASSIGN(_, n, param)		\
	SetValueAux<typename std::tuple_element<n, type>::type>(std::get<n>(tuple_), static_cast<typename BC_DataConverter<decltype(param##n)>::type>(param##n));
	/**/
#  define BOOST_PP_ITERATION_PARAMS_1 \
	(3, (1, CMN_ARITY_MAX, "BC_DataConver.h"))
#  include BOOST_PP_ITERATE()

#  undef BUF_CALL_DATA_CONVER_TUPLE_DO_READ
#  undef BUF_CALL_DATA_CONVER_DEFINE
#  undef BUF_CALL_DATA_CONVER_ASSIGN

	operator const TupleT& () const{
		assert(is_succeed_);
		return tuple_;
	}
	bool Valid() const{
		return is_succeed_;
	}

	TupleT	tuple_;
	bool	is_succeed_;
};

template<typename TupleT, typename DoValid>
struct BC_Algo_DoAsT_Tuple_{
	template<typename Stream>
	static BC_DataHolder_Tuple<TupleT> AsT( Stream& stm ){
		BC_DataHolder_Tuple<TupleT> data;
		if ( !DoValid::DoTypeInfo(stm) )
			return data;
		data.DoRead(stm, boost::mpl::long_<std::tuple_size<TupleT>::value>());
		return data;
	}
};

template<typename TupleT, pt_word size>
struct BC_Algo_WriteValue_Tuple{
	template<typename Stream>
	static void WriteValue(Stream& stm, const TupleT& tuple){
		BC_Algo_WriteValue_Tuple<TupleT, size - 1>::WriteValue(stm, tuple);
		const auto& v = std::get<size - 1>(tuple);
		BC_DataWrapper<typename std::tuple_element<size - 1, TupleT>::type, BC_Algo_WriteValue_Tuple>::Write(stm, v);
	}
};
template<typename TupleT>
struct BC_Algo_WriteValue_Tuple<TupleT, 0>{
	template<typename Stream>
	static void WriteValue(Stream& stm, const TupleT& tuple){}
};

template<typename TupleT, typename Policy>
struct BC_TypeConver_TupleBase_ : BC_Algo<BC_Algo_DoAsT_Tuple_<TupleT, BC_Algo_DoValidNext<BC_TuplePlaceHolder, Policy> >,
	BC_Algo_WriteValue_Tuple<TupleT, std::tuple_size<TupleT>::value> >{
	typedef TupleT type;
	typedef BC_TuplePlaceHolder t_info;
	typedef BC_TypeTag_Tuple type_tag;
};

template<typename... Args, typename Policy>
struct BC_TypeConverter_0<std::tuple<Args...>, Policy> : BC_TypeConver_TupleBase_<std::tuple<Args...>, Policy>{
};
//////////////////////////////////////////////////////////////////////////
# endif	//ADD_ONCE_BC_DATA_CONVER
#else	//BOOST_PP_IS_ITERATING
//////////////////////////////////////////////////////////////////////////
# ifdef BUF_CALL_DATA_CONVER_TUPLE_DO_READ

#  define N BOOST_PP_ITERATION()

template<typename Stm>
void DoRead( Stm& stm, boost::mpl::long_<N> ){
	BOOST_PP_REPEAT(N, BUF_CALL_DATA_CONVER_DEFINE, _)

	BOOST_PP_REPEAT(N, BUF_CALL_DATA_CONVER_ASSIGN, p)

	is_succeed_ = true;
	return;
}

#  undef N

# endif //BUF_CALL_DATA_CONVER_TUPLE_DO_READ
//////////////////////////////////////////////////////////////////////////
#endif	//BOOST_PP_IS_ITERATING
