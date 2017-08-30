#pragma once
/*
@author zhp
@date 2014-2-18 1:06
@purpose some useful classes
*/
#include "stdafx.h"
#include <cassert>
#include "UsefulFunc.h"
#include <map>
#include <boost/lexical_cast.hpp>
#include <set>
#include "CmnException.h"
#include "../DependSelfLib.h"
#include <CmnMix/Cmn_UsefulClass.h>

//设置MFC当前的模块实例
//AfxGetResourceHandle();AfxGetInstanceHandle();
//ModuleReplace对象释放的时候，会恢复原来的
class ModuleReplace{
public:
	//设置resource handle与instance handle为module
	ModuleReplace( HMODULE module );
	ModuleReplace( HMODULE new_resource_handle, HMODULE new_instance_handle );
	~ModuleReplace();

private:
	void SetNewHandle( HMODULE new_resource_handle, HMODULE new_instance_handle );

private:
	HMODULE			old_resource_handle_;
	HMODULE			old_instance_handle_;
};

//指定开始字符串与结束字符串，定位字符串
template<typename CharT>
class FindBetweenString{
public:
	//str_begin==0则表示从开头到str_end指定的结尾
	FindBetweenString( const CharT* str_begin,  const CharT* str_end ){
		str_begin_ = str_begin;
		str_end_ = str_end;
		assert(str_end);
	}

	//first == begin, second == end
	typedef std::pair<const CharT*, const CharT*> RangePtr;
	RangePtr Parse( const CharT* str_dest ){
		if ( !str_dest )
			goto LABLE_RET;
		const CharT* str_begin;
		if ( !str_begin_ )
			str_begin = str_dest;
		else
			str_begin = StdEx::TStrStr(str_dest, str_begin_);
		if ( !str_begin )
			goto LABLE_RET;
		auto str_end = StdEx::TStrStr(str_begin, str_end_);
		if ( !str_end )
			goto LABLE_RET;
		return RangePtr(str_begin, str_end);
LABLE_RET:
/*
#ifdef _DEBUG
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有有找到指定的字符串，str_begin:" << str_begin_ << ".str_end:" << str_end_ << ".str_dest:" << str_dest;
#endif*/
		return RangePtr(nullptr, nullptr);
	}

private:
	const CharT*				str_begin_;
	const CharT*				str_end_;
};
typedef FindBetweenString<char> FindBetweenStringA;

//运行时类的注册
//runtime class register
class RuntimeClassReg{
public:
	virtual ~RuntimeClassReg();
	//注册一个runtime class
	bool RegisterRuntimeClass( size_t runtime_class_id, const CRuntimeClass* runtime_class );
	//移除某个runtime class
	void RemoveRuntimeClass( size_t runtime_class_id );
	//通过id寻找某个runtime class
	CRuntimeClass* FindRuntimeClass( size_t runtime_class_id ) const;

private:
	std::map<size_t, CRuntimeClass*>			map_runtime_;
};

//////////////////////////////////////////////////////////////////////////
//框架效验
class FrameCheck : public Singleton<FrameCheck, Singleton_Me>{
	FrameCheck();
public:
	inline void Check(){
		auto str_tmp = RandomUniqueStr(_countof(buffer_));
		std::copy(str_tmp.begin(), str_tmp.end(), buffer_);
	}
	void TryCheck();
	inline bool FailedCheck_();

private:
	char					buffer_[256];
	static FrameCheck		only_one_;
};

class SmartCondition{
public:
	typedef FunctionVoid DoFuncT;
	struct AutoResetDo{
		AutoResetDo( SmartCondition& cond );
		bool operator () ();
		bool operator () (pt_dword timeout);
		bool operator () (pt_dword timer_milli_seconds, const DoFuncT& func_do);
		bool operator () (pt_dword timer_milli_seconds, pt_dword timeout, const DoFuncT& func_do);

	private:
		SmartCondition&			cond_;
	};

public:
	SmartCondition();
	void ResetCondition();
	bool CondtionTrue() const;
	void TouchCond();

	bool WaitTouch();
	bool WaitTouch( pt_dword timeout );
	bool TimerDoWaitTouch(pt_dword timer_milli_seconds, const DoFuncT& func_do);
	bool TimerDoWaitTouch(pt_dword timer_milli_seconds, pt_dword timeout, const DoFuncT& func_do);

private:
	bool			cond_true_;
};

template<typename T>
class UseInitDelay{
public:
	UseInitDelay(){
		is_init_ = false;
	}
	void Construct(){
		if (is_init_)
			return;
		new (buffer_) T();
		is_init_ = true;
	}
	T& get(){
		if (is_init_)
			return *(T*)buffer_;
		new (buffer_) T();
		is_init_ = true;
		return *(T*)buffer_;
	}
	operator T& (){
		return get();
	}

private:
	char	buffer_[sizeof(T)];
	bool	is_init_;
};

class IniProfile{
public:
	IniProfile( const std::string& profile_full_file_name );
	IniProfile();
	void SetProfile( const std::string& profile );
	const std::string& GetProfile() const;
	template<typename Setion_, typename Key_, typename T>
	bool Write(const Setion_& setion, const Key_& key, const T& the_value){
		const auto& the_setion = std::to_string(setion);
		const auto& the_key = std::to_string(key);
		const auto& the_str = std::to_string(the_value);
		auto res = !!::WritePrivateProfileStringA(the_setion.c_str(), the_key.c_str(), the_str.c_str(), profile_name_.c_str());
		assert(res);
		return res;
	}
	template<typename Setion_, typename Key_, typename T>
	bool Read(const Setion_& setion, const Key_& key, T& the_value, const T& def_value){
		const auto& the_setion = std::to_string(setion);
		const auto& the_key = std::to_string(key);
		char szbuffer[MAX_PATH];
		if (GetPrivateProfileStringA(the_setion.c_str(), the_key.c_str(), "", szbuffer, sizeof(szbuffer), profile_name_.c_str()) <= 0)
		{
			the_value = def_value;
			return true;
		}
		try{
			the_value = boost::lexical_cast<T>(szbuffer);
			return true;
		} DO_CATCH_ALL_EXCEPTION({
			assert(false);
			return false;
		})
	}

protected:
	std::string		profile_name_;
};

//多用于原子类型的判别，用于LUA函数重载
template<typename T>
class AtomicValue{
public:
	typedef T ValueType;

public:
	AtomicValue(){
		value_ = 0;
	}
	explicit AtomicValue( const T& v ) : value_(v){}
	AtomicValue( const AtomicValue& v ) : value_(v.value_){}
	AtomicValue& operator = ( const AtomicValue& v ){
		value_ = v.value_;
		return *this;
	}

public:
	T			value_;
};

//名单，白名单/黑名单
template<typename ContT_>
class NameListRef{
public:
	typedef typename ContT_::value_type ValueType;
	typedef ContT_ ContType;
	typedef typename ContType::iterator iterator;
	typedef typename ContType::const_iterator const_iterator;

private:
	static bool NameList_TestValueValid_( const std::string& v ){
		if (v.empty())
		{
			assert(false);
			return false;
		}
		return true;
	}
	template<typename T>
	static bool NameList_TestValueValid_( const T& ){
		return true;
	}
	template<typename T>
	static void AddHelper( std::set<T>& values, const T& v ){
		if ( !NameList_TestValueValid_(v) )
			return;
		values.insert(v);
	}
	template<typename T, typename ContT>
	static void AddHelper( ContT& values, const T& v ){
		if ( !NameList_TestValueValid_(v) )
			return;
		if ( std::find(values.begin(), values.end(), v) != values.end() )
			return;
		values.push_back(v);
	}
	template<typename ContT>
	static bool IsInByMatchHelper( const ContT& values, const std::string& v ){
		if (v.empty())
			return false;
		return std::find_if(values.begin(), values.end(), [&v]( const std::string& the_v ){
			return v.find(the_v) != std::string::npos;
		}) != values.end();
	}
	template<typename T, typename ContT>
	static bool IsInByMatchHelper( const ContT& values, const T& v ){
		assert(false);
		return false;
	}

public:
	explicit NameListRef( ContType& values ) : values_(values){}
	NameListRef& operator = (const NameListRef&) = delete;
	NameListRef(const NameListRef&) = delete;
	void Add( const ValueType& v ){
		AddHelper(values_, v);
	}
	template<typename TheContT_>
	void AddSome( const TheContT_& values ){
		for ( auto& v : values )
			Add(v);
	}
	void Delete( const ValueType& v ){
		auto it = std::find(values_.begin(), values_.end(), v);
		if ( it != values_.end() )
			values_.erase(it);
	}
	void clear(){
		values_.clear();
	}
	bool IsIn( const ValueType& v ) const{
		return std::find(values_.begin(), values_.end(), v) != values_.end();
	}
	//模糊匹配
	bool IsInByMatch( const ValueType& v ) const{
		return IsInByMatchHelper(values_, v);
	}
	template<typename Func_>
	bool IsInBy( const Func_& func ){
		return std::find_if(values_.begin(), values_.end(), func) != values_.end();
	}
	template<typename Func_>
	const ValueType* FindBy( const Func_& func ) const{
		auto it = std::find_if(values_.begin(), values_.end(), func);
		if ( it == values_.end() )
			return nullptr;
		return &*it;
	}
	const ContType& GetValues() const{
		return values_;
	}
	ContType& GetValues(){
		return values_;
	}
	int size() const{
		return (int)values_.size();
	}
	bool empty() const{
		return values_.empty();
	}
	const_iterator begin() const{
		return values_.begin();
	}
	iterator begin(){
		return values_.begin();
	}
	const_iterator end() const{
		return values_.end();
	}
	iterator end(){
		return values_.end();
	}

protected:
	ContType&		values_;
};
template<typename ContT_>
NameListRef<ContT_> MakeNameListRef( ContT_& values ){
	return NameListRef<ContT_>(values);
}

template<typename ValueT_, typename ContT_ = std::set<ValueT_> >
class NameList : public NameListRef<ContT_>{
public:
	NameList() : NameListRef<ContT_>(values_holder_){}
	NameList(const NameList& rhs) : NameListRef(values_holder_), values_holder_(rhs.values_holder_){}
	NameList& operator = (const NameList& rhs){
		values_holder_ = rhs.values_holder_;
		return *this;
	}

protected:
	ContType		values_holder_;
};

//根据指定的字符串随机组合
class RandomCombineStr{
public:
	typedef NameList<std::string, std::vector<std::string> > StrT;
	typedef boost::optional<const std::string&> OptionalString;

public:
	RandomCombineStr();
	pt_dword GetMaxLen() const{ return max_len_; }
	pt_dword GetMinLen() const{ return min_len_; }
	static void RegForLua(lua_State* l);
	static RandomCombineStr& GetDefMe();

	//setting
public:
	//设置组合后的总字符最大最小长度
	void SetTotalLen_MinMax( int min_len, int max_len );
	//设置前缀字符出现几率
	void SetOccurRate_Pre( double rate_pre );
	//设置间隔字符出现几率
	void SetOccurRate_Middle( double rate_middle );
	//设置后缀字符出现几率
	void SetOccurRate_Suffix( double rate_suffix );

	//添加
public:
	//添加前缀字符
	void AddStr_Pre( const std::string& str_pre );
	void AddSomeStr_Pre( const std::string& str_some_pre );
	//添加间隔字符
	void AddStr_Middle( const std::string& str_middle );
	void AddSomeStr_Middle( const std::string& str_some_middle );
	//添加后缀字符
	void AddStr_Suffix( const std::string& str_suffix );
	void AddSomeStr_Suffix( const std::string& str_some_suffix );
	//添加随机字符
	void AddRandomStr( const std::string& str_random );
	void AddSomeRandomStr( const std::string& str_some_random );
	//清空所有数据，不包括所有字符和配置
	void ClearAllData();

	//生成
public:
	//在一个字符串的基础上随机生成一个，如果想要生成全新的，str_base为空即可
	std::string GenRandomStr( const std::string& str_base ) const;

private:
	static std::vector<std::string> SeparateTheStr( const std::string& the_str );
	static const std::string& GetRandomOneStrBy( const StrT& strs );
	static std::string GetRandomSomeStrBy(const StrT& strs, OptionalString str_middle, pt_dword max_char_size);
	static std::string FilterValidStr( const std::string& str );

private:
	StrT			strs_pre_;
	StrT			strs_middle_;
	StrT			strs_suffix_;
	StrT			strs_random_;

private:
	pt_dword		max_len_;
	pt_dword		min_len_;
	double			occur_rate_pre_;
	double			occur_rate_middle_;
	double			occur_rate_suffix_;
};

class CookieHelper : public Singleton<CookieHelper, Singleton_MakeMe>{
public:
	//不用这个了
	CookieHelper() = delete;
	const std::string& GetCookie( const std::string& whose );
	bool UpdateCookie( const std::string& whose, const std::string& new_cookie );

private:
	std::map<std::string, std::string>		cookies_;
};

class LocalInfo{
public:
	//机器码
	static std::string GetTheMachineCode();
	//得到cookie值
	static std::string GetTheCookie(const std::string& whose);
	//设置cookie
	static void SetNewCookie(const std::string& whose, const std::string& str_cookie);

private:
	static const std::string& GetMachineCodeSysFileInfo();

private:
	static const wchar_t*const kCookieFileName;
};
