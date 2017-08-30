#pragma once
/*
@author zhp
@date 2016/11/3 19:37
@purpose lua common operate
*/
extern "C"
{
#include <lua/lualib.h>
#include <lua/MyLua.h>
}
#include "../DependSelfLib.h"
#include "_Cmn_LuaOperImpl.hpp"
#include <AssetSys/AS_Node.h>
#include <xtr1common>

//////////////////////////////////////////////////////////////////////////
/*
平常用luabind::object的时候一定要小心，最好都用object来操作，少用luabind::iterator来操作（先解引用到luabind::object对象中，再操作）。
*/
bool LuaObjIsNil(const luabind::object& obj);
bool LuaObjIsBool(const luabind::object& obj);
bool LuaObjIsNumber(const luabind::object& obj);
bool LuaObjIsStr(const luabind::object& obj);
bool LuaObjIsTable(const luabind::object& obj);
bool LuaObjIsFn(const luabind::object& obj);
const char* LuaTypeName(const luabind::object& lua_obj);
//////////////////////////////////////////////////////////////////////////
//*************lua debug
void LuaDP_Top();
//range_min、range_max可以为负数。若range_max小于range_min，则range_max=lua_gettop(L)
void LuaDP_Var(int range_min, int range_max, int tbl_lvl);
void LuaDP_Var(int stack_idx, int tbl_lvl);
void LuaDP_Var(int stack_idx);	//tbl_lvl=-1，表等级无上限
void LuaDP_Var();				//遍历所有stack变量。tbl_lvl=-1。
//range_min不可为负。若range_max小于range_min，则表示无上限
void LuaDP_UpVar(int range_min, int range_max, int tbl_lvl, int fn_level);
void LuaDP_UpVar(int upvalue_idx, int tbl_lvl, int fn_level);
void LuaDP_UpVar(int upvalue_idx, int tbl_lvl);		//fn_level=0,表示当前函数
void LuaDP_UpVar(int upvalue_idx);					//tbl_lvl=-1,fn_level=0
void LuaDP_UpVar();									//所有upvalue，其他参数同上。
//输出所有变量(包括upvalue)
void LuaDP_FnInfo(int tbl_lvl, int fn_level);
void LuaDP_FnInfo(int tbl_lvl);			//fn_level=0
void LuaDP_FnInfo();					//tbl_lvl=-1,fn_level=0
void LuaDP_Traceback();
//输出所有变量(包括upvalue)
void LuaDP_TracebackEx();
void LuaDP_TracebackEx(int tbl_lvl);
//////////////////////////////////////////////////////////////////////////
struct LuaObjectPair : std::pair<luabind::object, luabind::object>{
	LuaObjectPair();
	explicit LuaObjectPair(const luabind::object& the_first);
	LuaObjectPair(const luabind::object& the_first, const luabind::object& the_second);
	const luabind::object& First() const;
	luabind::object& First();
	const luabind::object& Second() const;
	luabind::object& Second();
	operator bool() const;
	static void RegForLua(lua_State* l);

private:
	const luabind::object& _First_();
	const luabind::object& _Second_();
};
//////////////////////////////////////////////////////////////////////////

template<typename R>
typename ::Detail::EnsureString<R>::type LuaObjCast(const luabind::object& lua_obj, const R& def_value){
	if (!lua_obj.is_valid())
		return def_value;
	const auto& optional_res = luabind::object_cast_nothrow<typename ::Detail::EnsureString<R>::type>(lua_obj);
	if (optional_res)
		return *optional_res;
	return def_value;
}

luabind::object LuaObjGlobal(lua_State* lstate, const char* glo_name);

/*
tag of lua call
example:
//不指定tagLC_NoThrow时，默认会抛出异常
LuaObjCall<void>(lua_obj, params...)
LuaObjCall<luabind::object>(lua_obj, params...)
//若返回类型不是int，luabind会抛出异常，而LuaObjCallByDlt不会抛出异常，会返回默认值。
LuaObjCall<int>(lua_obj, params...)
//对LuaObjCallByDlt来说，tagLC_NoThrow允许单独出现（不需要tagLCall包装，当然也可以用tagLCall包装一下），因为返回类型已由default_value指定了。
LuaObjCallByDlt<tagLC_NoThrow>(lua_obj, default_value, params...)
LuaObjCall<tagLCall<void, tagLC_NoThrow> >(lua_obj)
//tagLC_NoThrow和返回类型的出现顺序可随机
LuaObjCall<tagLCall<tagLC_NoThrow, double> >(lua_obj)
//对LuaObjCall来说，tagLC_NoThrow也可以单独出现，但无返回值。
LuaObjCall<tagLC_NoThrow>(lua_obj)
*/
template<typename... Args>
struct tagLCall{
	typedef boost::mpl::set<Args...> SetT;
	typedef Detail::tagLC_SetHolder<SetT> SetHolder;
};
//出错不抛出异常
struct tagLC_NoThrow{};

//luabind::unpack_call，可以支持unpack table，unpack vector等
//若是返回luabind::object，表示只返回一个结果，也可以返回多个结果到lua中，那么就用lua_pushvalue之类的函数
template<typename UnionR, typename... Args>
typename Detail::stLuaTagHlp<UnionR>::ResType
LuaObjCall(const luabind::object& lua_func, const Args&... args){
	typedef typename Detail::stLuaTagHlp<UnionR>::ResType ResType;
	typedef typename Detail::stLuaTagHlp<UnionR>::NoThrow NoThrow;
	if (!LuaObjIsFn(lua_func))
		return ResType();
	return NoThrow::Call<ResType>(lua_func, args...);
}
//若是返回luabind::object，表示只返回一个结果，也可以返回多个结果到lua中，那么就用lua_pushvalue之类的函数
template<typename UnionR, typename... Args>
typename Detail::stLuaTagHlp<UnionR>::ResType
LuaObjCall(lua_State* lstate, const char* glo_name, const Args&... args){
	typedef typename Detail::stLuaTagHlp<UnionR>::ResType ResType;
	typedef typename Detail::stLuaTagHlp<UnionR>::NoThrow NoThrow;
	auto lua_func = LuaObjGlobal(lstate, glo_name);
	if (!LuaObjIsFn(lua_func))
		return ResType();
	return NoThrow::Call<ResType>(lua_func, args...);
}
/**/
//by default
template<typename UnionR = tagLCall<>, typename R, typename... Args>
typename Detail::EnsureString<R>::type LuaObjCallByDlt(const luabind::object& lua_func, const R& def_value, const Args&... args){
	typedef typename Detail::stLuaTagHlp<UnionR>::SetT OrgSetT;
	typedef typename boost::mpl::insert<OrgSetT, luabind::object>::type NewSetT;
	luabind::object lua_res = LuaObjCall<Detail::tagLC_SetHolder<NewSetT> >(lua_func, args...);
	return LuaObjCast(lua_res, def_value);
}
template<typename UnionR = tagLCall<>, typename R, typename... Args>
typename Detail::EnsureString<R>::type 
LuaObjCallByDlt(lua_State* lstate, const char* glo_name, const R& def_value, const Args&... args){
	typedef typename Detail::stLuaTagHlp<UnionR>::SetT OrgSetT;
	typedef typename boost::mpl::insert<OrgSetT, luabind::object>::type NewSetT;
	auto lua_func = LuaObjGlobal(lstate, glo_name);
	luabind::object lua_res = LuaObjCall<Detail::tagLC_SetHolder<NewSetT> >(lua_func, args...);
	return LuaObjCast(lua_res, def_value);
}
/**/
//////////////////////////////////////////////////////////////////////////
//新的函数环境
class LuaNewFEnv{
public:
	explicit LuaNewFEnv(const luabind::object& the_func);
	LuaNewFEnv(const luabind::object& the_func, const luabind::object& new_env);
	~LuaNewFEnv();

private:
	void Init(const luabind::object& the_func, const luabind::object& new_env);

public:
	luabind::object	fn_;
	luabind::object new_fenv_;
	luabind::object	old_fenv_;
};
/**/
//////////////////////////////////////////////////////////////////////////
//lua 序列化
class LuaSerialize{
public:
	static std::string PlutoPersist(const luabind::object& permanents, const luabind::object& root_obj);
	static luabind::object PlutoUnpersist(const luabind::object& permanents, const std::string& buffer);
	static std::string ToString(const luabind::object& lua_obj);
	static std::string ToString(const luabind::object& lua_obj, int tbl_lvl);
	static void ToString(std::ostream& ostm, const luabind::object& lua_obj, int tbl_lvl);
	static bool Key2String(const luabind::object& key, std::string& buffer);
	static std::string Key2String(const luabind::object& key);
	static std::string ToPointerString(const luabind::object& lua_obj);

public:
	template<typename... Args>
	static std::string ToLuaString(const char* separator, const Args&... args){
		std::string res;
		::Detail::ToLuaString(res, separator, args...);
		return res;
	}
	template<typename... Args>
	static std::string ToLuaStringCall(const char* str_fn, const Args&... args){
		std::string res;
		if (str_fn)
			res.append(str_fn);
		res.append(1, '(');
		::Detail::ToLuaString(res, ",", args...);
		res.append(1, ')');
		return res;
	}

private:
	//luabind::object最好不要用std::set容器，因为它用的是<操作符，而vector之类的容器用的是==操作符。
	typedef std::vector<luabind::object> LuaObjContT;
	static bool _ToStringImpl_(LuaObjContT& unique_tbl, std::ostream& buffer, const luabind::object& lua_obj, int tab_num, bool new_line, int tbl_lvl);
};

class LuaSerializable{
public:
	virtual ~LuaSerializable();
	virtual bool SerializeR(const luabind::object& lua_obj) = 0;
	virtual bool SerializeW(luabind::object& lua_obj) const = 0;
};
//////////////////////////////////////////////////////////////////////////
/**/
//LuaParamHolder尽量少用，能用lua_pcall之类的函数的，就尽量用它们。
class LuaParamHolder{
public:
	//保存
	explicit LuaParamHolder(lua_State* lstate);
	//恢复
	~LuaParamHolder();

private:
	luabind::object	params;
	lua_State*		lstate_;
};
/**/
//打包堆栈
class LuaStackPack{
public:
	explicit LuaStackPack(lua_State* lstate);
	luabind::object PackToTable(int stack_idx_begin, int stack_idx_end, bool is_remove) const;
	bool UnpackToStack(const luabind::object& tbl) const;

private:
	lua_State*		lstate_;
};
/**/
//////////////////////////////////////////////////////////////////////////
class LStateWrapper{
protected:
	LStateWrapper();

public:
	LStateWrapper(lua_State* lstate);
	virtual ~LStateWrapper(){}
	lua_State* lua() const{ return lstate_; }
	
public:
	//若是返回luabind::object，表示只返回一个结果，也可以返回多个结果到lua中，那么就用lua_pushvalue之类的函数
	luabind::object LuaObjGlobal(const char* glo_name) const;
	template<typename UnionR, typename... Args>
	typename Detail::stLuaTagHlp<UnionR>::ResType
		LuaObjCall(const char* glo_name, const Args&... args) const{
		return ::LuaObjCall<UnionR>(lstate_, glo_name, args...);
	}
	template<typename UnionR = tagLCall<>, typename R, typename... Args>
	typename Detail::EnsureString<R>::type
		LuaObjCallByDlt(const char* glo_name, const R& def_value, const Args&... args) const{
		return ::LuaObjCallByDlt<UnionR>(lstate_, glo_name, def_value, args...);
	}

	//only load, not execute
public:
	//相当于lua中的loadfile
	virtual luabind::object LoadFile(const char* file_name) const;
	luabind::object LoadString(const char* str) const;
	luabind::object LoadBuffer(const char* buffer, size_t size, const char* buff_name = nullptr) const;

	//load and execute
public:
	//若是返回luabind::object，表示只返回一个结果，也可以返回多个结果到lua中，那么就用lua_pushvalue之类的函数
	luabind::object DoFile(const char* file_name) const;
	luabind::object DoString(const char* str) const;
	luabind::object DoBuffer(const char* buffer, size_t size, const char* buff_name = nullptr) const;
	luabind::object Require(const char* modname) const;

	//debug
	//DP->debug print
public:
	void DP_Top() const;
	//range_min、range_max可以为负数。若range_max小于range_min，则range_max=lua_gettop(L)
	void DP_Var(int range_min, int range_max, int tbl_lvl) const;
	//range_min不可为负。若range_max小于range_min，则表示无上限
	void DP_UpVar(int range_min, int range_max, int tbl_lvl, int fn_level) const;
	//输出所有变量(包括upvalue)
	void DP_FnInfo(int tbl_lvl, int fn_level) const;
	void DP_Traceback() const;
	//输出所有变量
	void DP_TracebackEx(int tbl_lvl) const;
	bool D_GetFnInfo(int fn_level, std::ostream& out, int tbl_lvl) const;
	//得到调用者函数名
	static int LuaD_GetLevelFnName(lua_State* L);

	//mylua
public:
	static bool My_TableSort(const luabind::object& tbl);
	static std::string My_ToString(const luabind::object& obj);
	static int My_DbgTracebackToStr(lua_State *L);

	//another
public:
	template<typename T>
	luabind::object CreateMultiIdxFn(const T& cont){
		if (cont.empty())
		{
			assert(false);
			return luabind::object();
		}
		auto new_tbl = luabind::newtable(lstate_);
		int idx = 1;
		for (auto& v : cont)
		{
			new_tbl[idx] = v;
			++idx;
		}
		return LuaObjCall<luabind::object>("CreateMultiIdxFn", new_tbl);
	}
	template<typename T, typename F>
	luabind::object CreateMultiIdxFn(const T& cont, const F& f){
		if (cont.empty())
		{
			assert(false);
			return luabind::object();
		}
		auto new_tbl = luabind::newtable(lstate_);
		int idx = 1;
		for (auto& v : cont)
		{
			new_tbl[idx] = f(v);
			++idx;
		}
		return LuaObjCall<luabind::object>("CreateMultiIdxFn", new_tbl);
	}
	static bool ObjectEqual(const luabind::object& lhs, const luabind::object& rhs);
	//覆盖表
	static bool DeepCoverTbl(const luabind::object& src, const luabind::object& dst);
	//复制表
	static bool CopyTbl(const luabind::object& src, const luabind::object& dst, bool copy_metatable);
	//如果目标已存在，则不复制
	static bool OptionalCopy(const luabind::object& src, const luabind::object& dst);

public:
	lua_State*		lstate_;
};

class LStateWrapperAS : public LStateWrapper{
protected:
	LStateWrapperAS();

public:
	LStateWrapperAS(lua_State* lstate, const char* as_path = nullptr);
	void SetLuaPath(const char* as_path);
	luabind::object LoadFile(const char* file_name) const override;
	AS_NodePtr GetRootPath() const;
	static AS_NodePtr FindAS_LuaFile(const AS_NodePtr& lua_dir, const char* file_name);

protected:
	AS_NodePtr		lua_path_root_;
};

class Cmn_LuaCfgResolver;
class Cmn_LuaCfgFileBase_;
class LuaThrdData : public LStateWrapperAS{
	LuaThrdData(const LuaThrdData&) = delete;
	LuaThrdData& operator = (const LuaThrdData&) = delete;
	typedef boost::shared_ptr<Cmn_LuaCfgResolver> LuaCfgResolverPtrT;

public:
	LuaThrdData(lua_State* lstate = nullptr, const char* as_path = "lua");
	~LuaThrdData();
	//SetMe和GetMe必须由lua线程调用
	static void SetMe(LuaThrdData* thrd_data);
	//不同的线程不同的实例，要先设置SetMe
	static LuaThrdData& GetMe();
	void SetMainLuaName(const char* main);
	const char* GetMainLuaName() const;
	void SetScriptDirName(const char* script_dir);
	void SetMustHasLuaRoot(bool must_has_lua_root);
	static void RegLuabind(lua_State* lstate);
	template<typename T>
	static T* AsPtr(){
		return static_cast<T*>(&LuaThrdData::GetMe());
	}
	template<typename T>
	static T& AsRef(){
		typedef typename std::remove_cv<T>::type UnCvT;
		typedef std::remove_reference<UnCvT>::type OrgT;
		return *static_cast<OrgT*>(&LuaThrdData::GetMe());
	}
	static const char* GetRequireLuaFilesName();

protected:
	//全局函数
	virtual void BindGlobalFn();
	//结构体
	virtual void BindStruct();
	//类
	virtual void BindClass();
	//常量、枚举类型等
	virtual void BindConst();
	//其他
	virtual void BindOther();

protected:
	//Init由SetMe调用
	virtual bool InitThrdData();

private:
	void AddRequireFiles(const char* lua_file, const AS_NodePtr& node);
	static int Require_PreLoad_(lua_State *L);

private:
	AS_NodePtr					require_luafiles_node_;
	LuaCfgResolverPtrT			require_cfg_resolver_;
	Cmn_LuaCfgFileBase_*		require_cfg_file_;
	bool						self_lstate_;
	const char*					main_lua_name_;
	const char*					script_dir_name_;
	bool						must_has_lua_root_;
	static const char* const	kRequireLuaFilesName_;
};
//////////////////////////////////////////////////////////////////////////