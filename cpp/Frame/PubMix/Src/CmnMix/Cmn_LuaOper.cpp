#include "stdafx.h"
#include <CmnMix/Cmn_LuaOper.h>
#include <boost/thread/tss.hpp>
#include <CmnMix/Cmn_LogOut.h>
#include <ProtocolX/P_DefinesX.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <CommonX/CmnX_DllApi.h>
#include <AssetSys/AS_Sys.h>
#include <CmnMix/Cmn_Iterator.h>
#include <pluto/pluto.h>
#include <CmnMix/Cmn_LuaFile.h>
#include <luabind/operator.hpp>

namespace Detail{
	void ToLuaStringImpl(std::string& out, const std::string& v){
		return ToLuaStringImpl(out, v.c_str());
	}
	void ToLuaStringImpl(std::string& out, const char* v){
		out.append(2, '[');
		if (v)
			out.append(v);
		out.append(2, ']');
	}
	void ToLuaStringImpl(std::string& out, const bool& v){
		if (v)
			out.append("true");
		else
			out.append("false");
	}
}

bool LuaObjIsNil(const luabind::object& obj)
{
	if (obj.is_valid() && LUA_TNIL != luabind::type(obj))
		return false;
	return true;
}

bool LuaObjIsBool(const luabind::object& obj)
{
	using namespace luabind;
	if (!obj.is_valid())
		return false;
	if (LUA_TBOOLEAN != type(obj))
		return false;
	return true;
}

bool LuaObjIsNumber(const luabind::object& obj)
{
	using namespace luabind;
	if (!obj.is_valid())
		return false;
	if (LUA_TNUMBER != type(obj))
		return false;
	return true;
}

bool LuaObjIsStr(const luabind::object& obj)
{
	using namespace luabind;
	if (!obj.is_valid())
		return false;
	if (LUA_TSTRING != type(obj))
		return false;
	return true;
}

bool LuaObjIsTable(const luabind::object& obj)
{
	using namespace luabind;
	if (!obj.is_valid())
		return false;
	if (LUA_TTABLE != type(obj))
		return false;
	return true;
}

bool LuaObjIsFn(const luabind::object& obj)
{
	using namespace luabind;
	if (!obj.is_valid())
		return false;
	if (LUA_TFUNCTION != type(obj))
		return false;
	return true;
}

luabind::object LuaObjGlobal(lua_State* lstate, const char* glo_name)
{
	using namespace luabind;
	if (!lstate || !glo_name)
	{
		assert(false);
		return object();
	}
	const auto& tbl_globals = globals(lstate);
	return tbl_globals[glo_name];
}

LStateWrapper::LStateWrapper(lua_State* lstate)
{
	assert(lstate);
	lstate_ = lstate;
}

LStateWrapper::LStateWrapper()
{
	lstate_ = nullptr;
}

luabind::object LStateWrapper::LuaObjGlobal(const char* glo_name) const
{
	return ::LuaObjGlobal(lstate_, glo_name);
}

luabind::object LStateWrapper::LoadFile(const char* file_name) const
{
	using namespace luabind;
#ifdef _DEBUG
	auto top_index_old = lua_gettop(lstate_);
#endif
	if (0 == luaL_loadfile(lstate_, file_name))
	{
		object chunk(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		assert(lua_gettop(lstate_) == top_index_old);
		return chunk;
	}
	object err_msg(from_stack(lstate_, -1));
	throw CmnLuaException(LuaObjCast(err_msg, "没有错误信息的脚本错误, tag:LoadFile"));
	assert(false);
	return object();
}

luabind::object LStateWrapper::LoadString(const char* str) const
{
	using namespace luabind;
#ifdef _DEBUG
	auto top_index_old = lua_gettop(lstate_);
#endif
	if (0 == luaL_loadstring(lstate_, str))
	{
		object chunk(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		assert(lua_gettop(lstate_) == top_index_old);
		return chunk;
	}
	object err_msg(from_stack(lstate_, -1));
	throw CmnLuaException(LuaObjCast(err_msg, "没有错误信息的脚本错误, tag:LoadString"));
	assert(false);
	return object();
}

luabind::object LStateWrapper::LoadBuffer(const char* buffer, size_t size, const char* buff_name) const
{
	using namespace luabind;
	assert(buffer);
	assert(size >= 0);
	if (!buff_name)
		buff_name = "ShyBuf";
#ifdef _DEBUG
	auto top_index_old = lua_gettop(lstate_);
#endif
	if (0 == luaL_loadbuffer(lstate_, buffer, size, buff_name))
	{
		object chunk(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		assert(lua_gettop(lstate_) == top_index_old);
		return chunk;
	}
	object err_msg(from_stack(lstate_, -1));
	throw CmnLuaException(LuaObjCast(err_msg, "没有错误信息的脚本错误, tag:LoadBuffer"));
	assert(false);
	return object();
}

luabind::object LStateWrapper::DoFile(const char* file_name) const
{
	return ::LuaObjCall<luabind::object>(LoadFile(file_name));
}

luabind::object LStateWrapper::DoString(const char* str) const
{
	return ::LuaObjCall<luabind::object>(LoadString(str));
}

luabind::object LStateWrapper::DoBuffer(const char* buffer, size_t size, const char* buff_name) const
{
	return ::LuaObjCall<luabind::object>(LoadBuffer(buffer, size, buff_name));
}

boost::thread_specific_ptr<LuaThrdData>	g_lua_thrd_data_;
void LuaThrdData::SetMe(LuaThrdData* thrd_data)
{
	if (g_lua_thrd_data_.get())
	{
		//必须由lua线程调用
		assert(false);
		return;
	}
	if (!thrd_data)
	{
		assert(false);
		return;
	}
	g_lua_thrd_data_.reset(thrd_data);
	if (!thrd_data->InitThrdData())
	{
		assert(false);
		return;
	}
}

LuaThrdData& LuaThrdData::GetMe()
{
	auto ptr = g_lua_thrd_data_.get();
	if (!ptr)
	{
		//必须由lua线程调用
		assert(false);
		throw CmnGeneralRunException("LuaThrdData为空");
	}
	return *ptr;
}

template<typename T>
struct ExceptionHandlerImpl : luabind::detail::exception_handler_base{
	ExceptionHandlerImpl(const T& handler) : handler_(handler){}
	void handle(lua_State* luastate) const{
		assert(luastate);
		handler_();
	}
	const T	handler_;
};
template<typename T>
ExceptionHandlerImpl<T>* MakeExceptionHandler(const T& handler){
	return new ExceptionHandlerImpl<T>(handler);
}

LONG WINAPI HandleCrash_(const bool& is_ignore, PEXCEPTION_POINTERS pExceptPtrs){
	if (is_ignore)
		return EXCEPTION_CONTINUE_SEARCH;
	return HandleCrashImpl(EXCEPTION_EXECUTE_HANDLER, pExceptPtrs);
}
struct PreTryCatchImpl : luabind::detail::PreTryCatch{
	int StdCatch(const luabind::detail::SaveCall& save_call, bool& is_ignore){
		try{
			return save_call.DoCall();
		} 
		//捕获所有能捕获的C++异常，若捕获不了，则表示可能是其他SEH异常，将由上层函数的异常处理来处理（生成dump，并抛出自定义异常）。
		DO_CATCH_THE_EXCEPTION(const std::exception&, {
			is_ignore = true;
			throw;
		})
	}
	int Handle(const luabind::detail::SaveCall& save_call){
		bool is_ignore = false;
		__try{
			return StdCatch(save_call, is_ignore);
		}
		__except (HandleCrash_(is_ignore, GetExceptionInformation())){
			throw CmnExitIoThreadException("出现了C++捕获不了的未知异常");
		}
		assert(false);
		return 0;
	}
};

bool LuaThrdData::InitThrdData()
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "init lua";*/
	if (!lstate_)
	{
		assert(false);
		return false;
	}
	if (lua_path_root_)
		require_luafiles_node_ = lua_path_root_->FindPosterity(kRequireLuaFilesName_);
#ifdef _DEBUG
	auto top_index_old = lua_gettop(lstate_);
#endif	
	const auto& the_bind_err = CmnExceptionBase::DefCatchCall([=](){
		BindGlobalFn();
		BindStruct();
		BindClass();
		BindConst();
		BindOther();
		assert(top_index_old == lua_gettop(lstate_));
		return kEmptyStr;
	});
	if (!the_bind_err.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "bind some lua error:" << the_bind_err;
		return false;
	}
	const auto& the_file_err = CmnExceptionBase::DefCatchCall([=]() -> std::string{
		Require(GetMainLuaName());
		assert(top_index_old == lua_gettop(lstate_));
		return kEmptyStr;
	});
	if (!the_file_err.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "加载私有脚本：" << GetMainLuaName() << " 失败：" << the_file_err;
		return false;
	}
	require_cfg_resolver_.reset();
	require_cfg_file_ = nullptr;
	require_luafiles_node_.reset();
	auto old_lua_root = lua_path_root_;
	if (script_dir_name_)
	{
		SetLuaPath(script_dir_name_);
		if (lua_path_root_)
			require_luafiles_node_ = lua_path_root_->FindPosterity(kRequireLuaFilesName_);
	}
	else
	{
		lua_path_root_.reset();
	}
	if (!lua_path_root_ && must_has_lua_root_)
	{
		lua_path_root_ = old_lua_root;
		assert(lua_path_root_);
	}
	return true;
}

static void My_luaL_openlibs_(lua_State *L) {
	const luaL_Reg g_my_lualibs_[] = {
		{ "", luaopen_base },
		{ LUA_LOADLIBNAME, luaopen_package },
		{ LUA_TABLIBNAME, luaopen_table },
		{ LUA_IOLIBNAME, luaopen_io },
		{ LUA_OSLIBNAME, luaopen_os },
		{ LUA_STRLIBNAME, luaopen_string },
		{ LUA_MATHLIBNAME, luaopen_math },
		//{ LUA_DBLIBNAME, luaopen_debug },
		{ NULL, NULL }
	};
	//////////////////////////////////////////////////////////////////////////
	const luaL_Reg *lib = g_my_lualibs_;
	for (; lib->func; lib++) {
		lua_pushcfunction(L, lib->func);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}
}

LuaThrdData::LuaThrdData(lua_State* lstate, const char* as_path) : LStateWrapperAS(lstate, as_path)
{
	if (lstate)
	{
		lstate_ = lstate;
		self_lstate_ = false;
	}
	else
	{
		self_lstate_ = true;
		lstate_ = lua_open();
		assert(lstate_);
		My_luaL_openlibs_(lstate_);
	}
	main_lua_name_ = "main.lua";
	script_dir_name_ = nullptr;
	require_cfg_file_ = nullptr;
	must_has_lua_root_ = false;
}

void LuaDoNothing()
{

}

void MsgBox(const char* msg)
{
	::MessageBoxTimeout(nullptr, msg, nullptr, MB_OK, 0, 1000 * 30);
}

void LuaLogT(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << log;
}

void LuaLogD(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << log;
}

void LuaLog(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << log;
}

void LuaLogW(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_warning) << log;
}

void LuaLogE(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << log;
}

void LuaLogF(const char* log){
	LOG_ENTER_FUNC;
	LOG_O(Log_fatal) << log;
}

int LuaNowTime(){
	return ((int)time(NULL));
}

luabind::object CallByDlt(const luabind::object& func, const luabind::object& dlt_value)
{
	return LuaObjCallByDlt(func, dlt_value);
}

luabind::object _Lua_loadfile(const char* file_name)
{
	if (!file_name)
	{
		assert(false);
		return luabind::object();
	}
	return LuaThrdData::GetMe().LoadFile(file_name);
}

luabind::object _Lua_dofile(const char* file_name)
{
	if (!file_name)
	{
		assert(false);
		return luabind::object();
	}
	return LuaThrdData::GetMe().DoFile(file_name);
}

void L_DummySleep__(unsigned long milli_seconds)
{
	return DummySleep(milli_seconds);
}

void LuaThrdData::BindGlobalFn()
{
	LuaObjectPair::RegForLua(lstate_);
	using namespace luabind;
	module(lstate_)
		[
			def("loadfile", &_Lua_loadfile),
			def("dofile", &_Lua_dofile),
			def("LuaLogT", &LuaLogT),
			def("LuaLogD", &LuaLogD),
			def("LuaLog", &LuaLog),
			def("LuaLogW", &LuaLogW),
			def("LuaLogE", &LuaLogE),
			def("LuaLogF", &LuaLogF),
			def("LuaNowTime", &LuaNowTime),
			def("LuaDoNothing", &LuaDoNothing),
			def("MsgBox", &MsgBox),
			def("CallByDlt", &CallByDlt),
			def("SafeRun_SkipExcept", &CmnExceptionBase::SafeRun_SkipExcept),
			def("Sleep", &L_DummySleep__),
			def("Delay", &L_DummySleep__)
		];
	auto lua_glo = globals(lstate_);
	//////////////////////////////////////////////////////////////////////////
	lua_glo["LuaD_GetLevelFnName"] = cpp2lua(lstate_, LuaD_GetLevelFnName);
	//////////////////////////////////////////////////////////////////////////
	object lua_package = lua_glo["package"];
	object preload = lua_package["preload"];
	assert(LUA_TNIL == type(getmetatable(preload)));
	object mt_tbl = newtable(lstate_);
	mt_tbl.push(lstate_);
	lua_pushlightuserdata(lstate_, this);
	lua_pushcclosure(lstate_, Require_PreLoad_, 1);
	lua_setfield(lstate_, -2, "__index");
	lua_pop(lstate_, 1);
	setmetatable(preload, mt_tbl);
	//////////////////////////////////////////////////////////////////////////
	//pluto的时候有用到，不要转移到其他地方。
	DoString(R"(
function CreateMultiIdxFn(tbl)
	return function(t, key)
		for _, v in ipairs(tbl) do
			local res = v[key]
			if res ~= nil then return res end
		end
	end
end
	)");

	DoString(R"(
		local tostr = tostring
		function tostring(...)
			local res = ""
			local param = {...}
			for i = 1, table.maxn(param) do
				res = res .. tostr(param[i])
			end
			return res
		end
		function LogT(...)
			LuaLogT(tostring(...))
			return LogT
		end
		function LogD(...)
			LuaLogD(tostring(...))
			return LogD
		end
		function Log(...)
			LuaLog(tostring(...))
			return Log
		end
		function LogW(...)
			LuaLogW(tostring(...))
			return LogW
		end
		function LogE(...)
			LuaLogE(tostring(...))
			return LogE
		end
		function LogF(...)
			LuaLogF(tostring(...))
			return LogF
		end
		print = Log
		
		local emptyFn = function() end
		function Closure_Iterator( Iterator )
			if Iterator == nil then
				return emptyFn
			end
			Iterator:First();
			return function()
				if not Iterator:IsDone() then
					local item = Iterator:CurItem();
					Iterator:Next();
					return item;
				end
			end
		end

		Iterate = Closure_Iterator
		
		function LuaSafeRun_SkipExcept( func, def_res, ... )
			local the_param = { ... }
			local the_func = function ()
				return func(unpack(the_param))
			end
			return SafeRun_SkipExcept(the_func, def_res)
		end

		安全执行之跳过异常 = LuaSafeRun_SkipExcept

		function CopyTheTbl( tbl )
			if tbl == nil then
				return
			end
			local tbl_tmp = {}
			for k, v in pairs(_G) do
				tbl_tmp[k] = v
			end
			return tbl_tmp
		end

		复制这个表 = CopyTheTbl
		)");
}

void LuaThrdData::BindStruct()
{

}

const std::string& GetEmptyString(){
	return kEmptyStr;
}
const std::wstring& GetEmptyWString(){
	return kEmptyWStr;
}

LuaNewFEnv::LuaNewFEnv(const luabind::object& the_func)
{
	Init(the_func, luabind::object());
}

LuaNewFEnv::LuaNewFEnv(const luabind::object& the_func, const luabind::object& new_env)
{
	Init(the_func, new_env);
}

LuaNewFEnv::~LuaNewFEnv()
{
	if (fn_.is_valid())
	{
		auto lstate = fn_.interpreter();
		fn_.push(lstate);
		old_fenv_.push(lstate);
		lua_setfenv(lstate, -2);
		lua_pop(lstate, 1);
	}
}

void LuaNewFEnv::Init(const luabind::object& the_func, const luabind::object& new_env)
{
	if (!the_func.is_valid())
	{
		assert(false);
		return;
	}
	fn_ = the_func;
	using namespace luabind;
	auto lstate = the_func.interpreter();
#ifdef _DEBUG 
	auto old_top = lua_gettop(lstate);
#endif
	the_func.push(lstate);

	lua_getfenv(lstate, -1);
	assert(LUA_TNIL != lua_type(lstate, -1));
	{
		object old_env(from_stack(lstate, -1));
		old_fenv_.swap(old_env);
	}
	lua_pop(lstate, 1);

	if (new_env.is_valid())
	{
		assert(LUA_TTABLE == type(new_env));
		new_fenv_ = new_env;
	}
	else
	{
		newtable(lstate).swap(new_fenv_);
		auto mt_tbl = newtable(lstate);
		rawset(mt_tbl, "__index", old_fenv_);
		setmetatable(new_fenv_, mt_tbl);
	}
	new_fenv_.push(lstate);
	{
		luabind::detail::stack_pop pop(lstate, 1);
		if (lua_setfenv(lstate, -2) == 0)
			assert(false);
	}

	assert(old_top == lua_gettop(lstate));
}

static std::string __WideCharToGbk__(const std::wstring& str_wide){
	return WideCharToGbk(str_wide);
}
void LuaThrdData::BindClass()
{
	using namespace luabind;
	using namespace std;
	module(lstate_)
		[
			class_<wstring>("wstring")
			.def(constructor<>())
			.def(constructor<const wstring&>())
			.def(constructor<const wchar_t*>())
			.def("empty", &wstring::empty)
			.def("compare", (int (wstring::*)(const wstring&) const)&wstring::compare),
			def("GbkToWideChar", (wstring(*)(const string&))&GbkToWideChar),
			def("WideCharToGbk", &__WideCharToGbk__),
			def("UnsensePerlMetachar", &UnsensePerlMetachar),
			def("UnsensePerlMetacharA", &UnsensePerlMetacharA),
			def("GetEmptyString", &GetEmptyString),
			def("GetEmptyWString", &GetEmptyWString),

			class_<LStateWrapper>("LStateWrapper")
			.def("LoadFile", &LStateWrapper::LoadFile)
			.def("LoadString", &LStateWrapper::LoadString)
			.def("LoadBuffer", &LStateWrapper::LoadBuffer)
			.def("DoFile", &LStateWrapper::DoFile)
			.def("DoString", &LStateWrapper::DoString)
			.def("DoBuffer", &LStateWrapper::DoBuffer),
			def("OptionalCopy", &LStateWrapper::OptionalCopy),

			class_<LStateWrapperAS, LStateWrapper>("LStateWrapperAS"),

			class_<LuaThrdData, LStateWrapperAS>("LuaThrdData"),

			class_<__int64>("int64")
			.def(constructor<>())
			.def(constructor<int>())
			.def(self + __int64())
			.def(self - __int64())
			.def(self * __int64())
			.def(self / __int64())
			.def(self ^ __int64())
			.def(self < __int64())
			.def(self <= __int64())
			.def(self == __int64())
		];
}

void LuaThrdData::BindConst()
{

}

void LuaThrdData::BindOther()
{

}

LuaThrdData::~LuaThrdData()
{
	if (self_lstate_ && lstate_)
	{
		lua_close(lstate_);
		lstate_ = nullptr;
	}
}

const char* LuaThrdData::GetMainLuaName() const
{
	assert(main_lua_name_);
	return main_lua_name_;
}

void LuaThrdData::SetMainLuaName(const char* main)
{
	assert(main);
	main_lua_name_ = main;
}

void LuaThrdData::RegLuabind(lua_State* lstate)
{
	assert(lstate);
	luabind::open(lstate);
	luabind::detail::register_exception_handler(MakeExceptionHandler(&CmnExceptionBase::CatchDelayException));
	luabind::detail::SetPreTryCatch(new PreTryCatchImpl);
	luabind::set_pcall_callback(My_DbgTracebackToStr);
	luabind::disable_super_deprecation();
}

void LuaThrdData::SetScriptDirName(const char* script_dir)
{
	script_dir_name_ = script_dir;
}

int LuaThrdData::Require_PreLoad_(lua_State *L)
{
	assert(2 == lua_gettop(L));
	auto thrd_data = (LuaThrdData*)lua_touserdata(L, lua_upvalueindex(1));
	assert(thrd_data);
	auto key = lua_tostring(L, 2);
	assert(key);
	auto node = FindAS_LuaFile(thrd_data->lua_path_root_, key);
	if (!node)
		return 0;
	std::ostringstream ostm;
	if (!node->Serialize(ostm))
	{
		assert(false);
		return 0;
	}
	boost::filesystem::path path(key);
	if (!path.has_extension())
		path += ".lua";
	const auto& str_path = path.string();
	const auto& buffer = ostm.str();
	auto res = thrd_data->LoadBuffer(buffer.c_str(), buffer.size(), str_path.c_str());
	if (!res.is_valid())
	{
		assert(false);
		return 0;
	}
	thrd_data->AddRequireFiles(str_path.c_str(), node);
	res.push(L);
	return 1;
}

void LuaThrdData::AddRequireFiles(const char* lua_file, const AS_NodePtr& node)
{
	if (!require_luafiles_node_)
		return;
	assert(lua_file);
	assert(node);
	if (!lua_path_root_)
	{
		assert(false);
		return;
	}
	bool resolved = false;
	if (!require_cfg_resolver_)
	{
		require_cfg_resolver_.reset(new Cmn_LuaCfgResolver(lua_path_root_, lstate_, Cmn_LuaCfgResolver::kNormal));
		auto file = require_cfg_resolver_->AddFile(kRequireLuaFilesName_, 0);
		if (!file)
		{
			assert(false);
			return;
		}
		require_cfg_file_ = static_cast<Cmn_LuaCfgFileBase_*>(file.get());
		require_cfg_resolver_->Resolve(kEmptyStr);
		resolved = true;
	}
	if (!require_cfg_file_)
	{
		assert(false);
		return;
	}
	auto& as = AS_Sys::GetMe();
	if (as.HasTheFile(require_cfg_file_->GetTable(), lua_file))
		return;
	if (!resolved)
		require_cfg_resolver_->Resolve(kEmptyStr);
	if (!as.AddFileToTbl(require_cfg_file_->GetTable(), lua_path_root_, node))
	{
		assert(false);
		return;
	}
	require_cfg_file_->Save();
}

const char* LuaThrdData::GetRequireLuaFilesName()
{
	return kRequireLuaFilesName_;
}

void LuaThrdData::SetMustHasLuaRoot(bool must_has_lua_root)
{
	must_has_lua_root_ = must_has_lua_root;
}

const char* const LuaThrdData::kRequireLuaFilesName_ = "luafiles.luafiles";

LStateWrapperAS::LStateWrapperAS()
{
	lstate_ = nullptr;
	SetLuaPath(nullptr);
}

LStateWrapperAS::LStateWrapperAS(lua_State* lstate, const char* as_path)
{
	lstate_ = lstate;
	SetLuaPath(as_path);
}

AS_NodePtr LStateWrapperAS::GetRootPath() const
{
	return lua_path_root_;
}

void LStateWrapperAS::SetLuaPath(const char* as_path)
{
	auto& root = AS_Sys::GetMe().GetRoot();
	if (!root)
	{
		assert(false);
		return;
	}
	if (as_path)
		lua_path_root_ = root->Find(as_path);
	else
		lua_path_root_ = root;
}

luabind::object LStateWrapperAS::LoadFile(const char* file_name) const
{
	if (!lua_path_root_)
		return __super::LoadFile(file_name);
	if (!file_name)
	{
		assert(false);
		return luabind::object();
	}
	auto node = FindAS_LuaFile(lua_path_root_, file_name);
	if (!node)
		return __super::LoadFile(file_name);
	std::ostringstream ostm;
	if (!node->Serialize(ostm))
	{
		assert(false);
		return luabind::object();
	}
	const auto& buffer = ostm.str();
	if (buffer.empty())
	{
		//assert(false);
		return luabind::object();
	}
	boost::filesystem::path path(file_name);
	if (!path.has_extension())
		path += ".lua";
	const auto& str_path = path.string();
	return LoadBuffer(buffer.c_str(), buffer.size(), str_path.c_str());
}

AS_NodePtr LStateWrapperAS::FindAS_LuaFile(const AS_NodePtr& lua_dir, const char* file_name)
{
	if (!file_name || !lua_dir)
	{
		assert(false);
		return nullptr;
	}
	boost::filesystem::path path(file_name);
	if (!path.has_extension())
		path += ".lua";
	auto node = lua_dir->Find(path);
	if (!node)
		node = lua_dir->FindPosterity(path);
	return node;
}

static int __Pluto_persist_bufwriter__(lua_State *L, const void* p, size_t sz, void* ud) {
	auto wi = (std::string*)ud;
	wi->append((const char*)p, sz);
	return 0;
}
std::string LuaSerialize::PlutoPersist(const luabind::object& permanents, const luabind::object& root_obj)
{
	if (!permanents.is_valid() || !root_obj.is_valid())
	{
		assert(false);
		return std::string();
	}
	using namespace luabind;
	if (type(permanents) != LUA_TTABLE)
	{
		assert(false);
		return std::string();
	}
	auto L = permanents.interpreter();
	if (L != root_obj.interpreter())
	{
		assert(false);
		return std::string();
	}
	std::string buffer;

	LuaParamHolder param_holder(L);
	lua_settop(L, 0);
	permanents.push(L);
	root_obj.push(L);
	pluto_persist(L, __Pluto_persist_bufwriter__, (void*)&buffer);
	return buffer;
}

struct stPlutoUnpersistBufReader{
	const char* buf;
	size_t		size;
};
static const char* __Pluto_unpersist_bufreader__(lua_State *L, void *ud, size_t *sz) {
	auto* li = (stPlutoUnpersistBufReader*)ud;
	if (li->size == 0) {
		return nullptr;
	}
	*sz = li->size;
	li->size = 0;
	return li->buf;
}

bool LStateWrapper::ObjectEqual(const luabind::object& lhs, const luabind::object& rhs)
{
	using namespace luabind;
	auto valid_value = (int)lhs.is_valid() + (int)rhs.is_valid();
	if (0 == valid_value)
		return true;
	if (valid_value != 2)
		return false;
	auto L = lhs.interpreter();
	assert(L == rhs.interpreter());
	auto lhs_type = type(lhs);
	if (LUA_TTABLE != lhs_type)
	{
		lhs.push(L);
		rhs.push(L);
		luabind::detail::stack_pop pop(L, 2);
		return 1 == lua_equal(L, -1, -2);
	}
	if (LUA_TTABLE != type(rhs))
		return false;
	for (iterator it(lhs), end; it != end; ++it)
	{
		object rhs_value = rawget(rhs, it.key());
		if (!ObjectEqual(*it, rhs_value))
			return false;
	}
	return true;
}

luabind::object LuaSerialize::PlutoUnpersist(const luabind::object& permanents, const std::string& buffer)
{
	if (!permanents.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	using namespace luabind;
	if (type(permanents) != LUA_TTABLE)
	{
		assert(false);
		return luabind::object();
	}
	if (buffer.empty())
		return luabind::object();
	stPlutoUnpersistBufReader li;
	li.buf = buffer.c_str();
	li.size = buffer.size();

	auto L = permanents.interpreter();
	LuaParamHolder param_holder(L);
	lua_settop(L, 0);
	permanents.push(L);
	pluto_unpersist(L, __Pluto_unpersist_bufreader__, (void*)&li);
	object res(from_stack(L, -1));
	return res;
}

std::string LuaSerialize::ToString(const luabind::object& lua_obj)
{
	std::stringstream buffer;
	LuaObjContT unique_tbl;
	if (!_ToStringImpl_(unique_tbl, buffer, lua_obj, 0, false, -1))
	{
		assert(false);
		return std::string();
	}
	return buffer.str();
}

std::string LuaSerialize::ToString(const luabind::object& lua_obj, int tbl_lvl)
{
	std::stringstream buffer;
	LuaObjContT unique_tbl;
	if (!_ToStringImpl_(unique_tbl, buffer, lua_obj, 0, false, tbl_lvl))
	{
		assert(false);
		return kEmptyStr;
	}
	return buffer.str();
}

void LuaSerialize::ToString(std::ostream& ostm, const luabind::object& lua_obj, int tbl_lvl)
{
	LuaObjContT unique_tbl;
	if (!_ToStringImpl_(unique_tbl, ostm, lua_obj, 0, false, tbl_lvl))
	{
		assert(false);
	}
}

bool LuaSerialize::_ToStringImpl_(LuaObjContT& unique_tbl, std::ostream& buffer, const luabind::object& lua_obj, int tab_num, bool new_line, int tbl_lvl)
{
	using namespace luabind;
	if (new_line)
	{
		buffer << '\n';
		if (tab_num > 0)
		{
			buffer << std::string(tab_num, '\t');
		}
	}
	auto obj_type = type(lua_obj);
	switch (obj_type)
	{
	case LUA_TNIL:
	{
		buffer << "nil";
		break;
	}
	case LUA_TBOOLEAN:
	{
		if (LuaObjCast(lua_obj, true))
			buffer << "true";
		else
			buffer << "false";
		break;
	}
	case LUA_TNUMBER:
	{
		auto L = lua_obj.interpreter();
		lua_obj.push(L);
		size_t size = 0;
		auto str = lua_tolstring(L, -1, &size);
		buffer.write(str, size);
		lua_pop(L, 1);
		break;
	}
	case LUA_TSTRING:
	{
		buffer << "[[";
		buffer << LuaObjCast(lua_obj, kEmptyStr);
		buffer << "]]";
		break;
	}
	case LUA_TTABLE:
	{
		if (0 == tbl_lvl)
		{
			buffer << "{ --[[忽略此表的内容]] }";
			break;
		}
		for (auto& v : unique_tbl)
		{
			if (v == lua_obj)
			{
				buffer << "nil; --[[是个循环引用的表]]" << std::flush;
				return true;
			}
		}
		unique_tbl.push_back(lua_obj);
		buffer << std::endl;
		if (tab_num > 0)
			buffer << std::string(tab_num, '\t');
		auto old_tab_num = tab_num;
		++tab_num;
		buffer << '{';
		iterator it(lua_obj), end;
		int cnt = 0;
		for (int def_idx = 1; it != end; ++it, ++cnt)
		{
			object v(*it);
			auto key = it.key();
			auto key_type = type(key);
			if (LUA_TNUMBER == key_type)
			{
				//lua默认的索引序号为1，后面元素的序号以此递增。若是不指定索引序号，那么该元素无论在表中的何处位置，它的索引序号都为默认值（第一个没指定的是1，第二个是2...）
				auto real_idx = LuaObjCast(key, 0);
				if (real_idx != def_idx)
				{
					buffer << '\n';
					if (tab_num > 0)
						buffer << std::string(tab_num, '\t');
					buffer << '[';
					buffer << std::to_string(real_idx);
					buffer << "] = ";
					if (!_ToStringImpl_(unique_tbl, buffer, v, tab_num, false, tbl_lvl - 1))
						return false;
				}
				else
				{
					++def_idx;
					if (!_ToStringImpl_(unique_tbl, buffer, v, tab_num, true, tbl_lvl - 1))
						return false;
				}
			}
			else if (LUA_TSTRING == key_type)
			{
				const auto& str_key = LuaObjCast(key, kEmptyStr);
				if (str_key.empty())
				{
					assert(false);
					return false;
				}
				buffer << '\n';
				if (tab_num > 0)
					buffer << std::string(tab_num, '\t');
				buffer << "[\"" << str_key << "\"]";
				buffer << " = ";
				if (!_ToStringImpl_(unique_tbl, buffer, v, tab_num, false, tbl_lvl - 1))
					return false;
			}
			else
			{
				buffer << '\n';
				if (tab_num > 0)
					buffer << std::string(tab_num, '\t');
				const auto& str_key = ToPointerString(key);
				buffer << str_key;
				buffer << " = ";
				if (!_ToStringImpl_(unique_tbl, buffer, v, tab_num, false, tbl_lvl - 1))
					return false;
			}
		}
		if (cnt > 0)
		{
			buffer << '\n';
			if (old_tab_num > 0)
				buffer << std::string(old_tab_num, '\t');
		}
		buffer << '}';
		break;
	}
	default:
	{
		const auto& str = ToPointerString(lua_obj);
		buffer << str;
	}
	}
	buffer << ';' << std::flush;
	return true;
}

bool LuaSerialize::Key2String(const luabind::object& key, std::string& buffer)
{
	using namespace luabind;
	if (LUA_TNUMBER == type(key))
	{
		auto key_idx = LuaObjCast(key, 0);
		buffer.append(1, '[');
		buffer.append(std::to_string(key_idx));
		buffer.append("] = ");
	}
	else
	{
		const auto& str_key = LuaObjCast(key, std::string());
		if (str_key.empty())
		{
			assert(false);
			return false;
		}
		//buffer.append("[\"");
		buffer.append(str_key);
		//buffer.append("\"]");
		buffer.append(" = ");
	}
	return true;
}

std::string LuaSerialize::Key2String(const luabind::object& key)
{
	std::string buffer;
	if (!Key2String(key, buffer))
		return std::string();
	return buffer;
}

std::string LuaSerialize::ToPointerString(const luabind::object& lua_obj)
{
	using namespace luabind;
	auto lstate = lua_obj.interpreter();
	assert(lstate);
	std::string buffer;
	buffer.append(2, '[');
	buffer.append(lua_typename(lstate, type(lua_obj)));
	buffer.append(" : ");
	lua_obj.push(lstate);
	detail::stack_pop pop(lstate, 1);
	buffer.append(std::to_string(lua_topointer(lstate, -1)));
	buffer.append(2, ']');
	return buffer;
}

LuaParamHolder::LuaParamHolder(lua_State* lstate)
{
	lstate_ = nullptr;
	if (!lstate)
	{
		assert(false);
		return;
	}
	using namespace luabind;
	auto size = lua_gettop(lstate);
	if (size <= 0)
	{
		lstate_ = lstate;
		return;
	}
	else if (size == 1)
	{
		lstate_ = lstate;
		params = object(from_stack(lstate, 1));
		return;
	}
	params.swap(newtable(lstate));
	for (auto i = 1; i <= size; ++i)
	{
		params[i] = object(from_stack(lstate, i));
	}
}

LuaParamHolder::~LuaParamHolder()
{
	auto lstate = lstate_;
	if (!lstate)
		lstate = params.interpreter();
	if (!lstate)
		return;
	lua_settop(lstate, 0);
	if (lstate_)
	{
		if (!params.is_valid())
			return;
		params.push(lstate);
		return;
	}
	luabind::iterator it(params), end;
	for (; it != end; ++it)
	{
		luabind::object tmp(*it);
		tmp.push(lstate);
	}
}

LuaStackPack::LuaStackPack(lua_State* lstate)
{
	assert(lstate);
	lstate_ = lstate;
}

luabind::object LuaStackPack::PackToTable(int stack_idx_begin, int stack_idx_end, bool is_remove) const
{
	using namespace luabind;
	if (!lstate_)
	{
		assert(false);
		return object();
	}
	if (stack_idx_begin >= 0)
	{
		if (stack_idx_end <= 0)
		{
			stack_idx_end = lua_gettop(lstate_);
		}
		if (stack_idx_begin == stack_idx_end)
			return object();
		if (stack_idx_begin == 0)
			stack_idx_begin = 1;
	}
	if (stack_idx_begin > stack_idx_end)
		return object();
	auto top = lua_gettop(lstate_);
	if (stack_idx_begin < 0)
	{
		stack_idx_begin += top + 1;
		stack_idx_end += top + 1;
	}
	if (stack_idx_begin <= 0 || stack_idx_end <= 0)
	{
		assert(false);
		return object();
	}
	if (stack_idx_begin > top || stack_idx_end > top)
	{
		assert(false);
		return object();
	}
	assert(stack_idx_end - stack_idx_begin >= 0);
	auto tbl = newtable(lstate_);
	for (auto it = stack_idx_begin, idx = 1; it <= stack_idx_end; ++it, ++idx)
	{
		tbl[idx] = object(from_stack(lstate_, it));
	}
	if (is_remove)
	{
		auto n = stack_idx_end - stack_idx_begin;
		while (n >= 0)
		{
			lua_remove(lstate_, stack_idx_begin);
			--n;
		}
	}
	return tbl;
}

bool LuaStackPack::UnpackToStack(const luabind::object& tbl) const
{
	using namespace luabind;
	if (!tbl.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return false;
	}
	if (!lstate_)
	{
		assert(false);
		return false;
	}
	iterator it(tbl), end;
	for (; it != end; ++it)
	{
		object tmp(*it);
		tmp.push(lstate_);
	}
	return true;
}

bool LStateWrapper::My_TableSort(const luabind::object& tbl)
{
	using namespace luabind;
	auto L = tbl.interpreter();
	if (!L)
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != luabind::type(tbl))
	{
		assert(false);
		return false;
	}
	lua_pushcfunction(L, luaMy_tblsort);
	object f(from_stack(L, -1));
	lua_pop(L, 1);
	::LuaObjCall<void>(f, tbl);
	return true;
}

static void __DeepCoverTbl__(const luabind::object& src, const luabind::object& dst){
	using namespace luabind;
	for (iterator it(src), it_end; it != it_end; ++it) 
	{
		object v(*it);
		object key = it.key();
		if (LUA_TTABLE == type(v))
		{
			luabind::object new_tbl = rawget(dst, key);
			if (LUA_TTABLE != type(new_tbl))
			{
				newtable(dst.interpreter()).swap(new_tbl);
				rawset(dst, key, new_tbl);
			}
			__DeepCoverTbl__(v, new_tbl);
		}
		else
		{			
			rawset(dst, key, v);
		}		
	}
	auto mt = getmetatable(src);
	if (LUA_TTABLE == type(mt))
	{
		setmetatable(dst, mt);
	}
}

void LuaDP_Top()
{
	return LuaThrdData::GetMe().DP_Top();
}

void LuaDP_Var(int range_min, int range_max, int tbl_lvl)
{
	return LuaThrdData::GetMe().DP_Var(range_min, range_max, tbl_lvl);
}

void LuaDP_Var(int stack_idx, int tbl_lvl)
{
	return LuaDP_Var(stack_idx, stack_idx, tbl_lvl);
}

void LuaDP_Var(int stack_idx)
{
	return LuaDP_Var(stack_idx, stack_idx, -1);
}

void LuaDP_Var()
{
	return LuaDP_Var(1, 0, -1);
}

void LuaDP_UpVar(int range_min, int range_max, int tbl_lvl, int fn_level)
{
	return LuaThrdData::GetMe().DP_UpVar(range_min, range_max, tbl_lvl, fn_level);
}

void LuaDP_UpVar(int upvalue_idx, int tbl_lvl, int fn_level)
{
	return LuaDP_UpVar(upvalue_idx, upvalue_idx, tbl_lvl, fn_level);
}

void LuaDP_UpVar(int upvalue_idx, int tbl_lvl)
{
	return LuaDP_UpVar(upvalue_idx, upvalue_idx, tbl_lvl, 0);
}

void LuaDP_UpVar(int upvalue_idx)
{
	return LuaDP_UpVar(upvalue_idx, upvalue_idx, -1, 0);
}

void LuaDP_UpVar()
{
	return LuaDP_UpVar(1, 0, -1, 0);
}

void LuaDP_FnInfo(int tbl_lvl, int fn_level)
{
	return LuaThrdData::GetMe().DP_FnInfo(tbl_lvl, fn_level);
}

void LuaDP_FnInfo(int tbl_lvl)
{
	return LuaDP_FnInfo(tbl_lvl, 0);
}

void LuaDP_FnInfo()
{
	return LuaDP_FnInfo(-1, 0);
}

void LuaDP_Traceback()
{
	return LuaThrdData::GetMe().DP_Traceback();
}

void LuaDP_TracebackEx(int tbl_lvl)
{
	return LuaThrdData::GetMe().DP_TracebackEx(tbl_lvl);
}

void LuaDP_TracebackEx()
{
	return LuaDP_TracebackEx(-1);
}

const char* LuaTypeName(const luabind::object& lua_obj)
{
	if (!lua_obj.is_valid())
	{
		assert(false);
		return "";
	}
	return lua_typename(lua_obj.interpreter(), luabind::type(lua_obj));
}

bool LStateWrapper::CopyTbl(const luabind::object& src, const luabind::object& dst, bool copy_metatable)
{
	using namespace luabind;
	if (!src.is_valid() || !dst.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(src) || LUA_TTABLE != type(dst))
	{
		assert(false);
		return false;
	}
	for (iterator it(src), it_end; it != it_end; ++it)
	{
		object v(*it);
		rawset(dst, it.key(), v);
	}
	if (copy_metatable)
	{
		auto mt = getmetatable(src);
		if (LUA_TTABLE == type(mt))
		{
			setmetatable(dst, mt);
		}
	}
	return true;
}

bool LStateWrapper::DeepCoverTbl(const luabind::object& src, const luabind::object& dst)
{
	using namespace luabind;
	if (!src.is_valid() || !dst.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(src) || LUA_TTABLE != type(dst))
	{
		assert(false);
		return false;
	}
	__DeepCoverTbl__(src, dst);
	return true;
}

std::string LStateWrapper::My_ToString(const luabind::object& obj)
{
	using namespace luabind;
	auto L = obj.interpreter();
	if (!L)
	{
		assert(false);
		return std::string();
	}
	lua_pushcfunction(L, luaMy_tostring);
	object f(from_stack(L, -1));
	lua_pop(L, 1);
	return ::LuaObjCall<std::string>(f, obj);
}

bool LStateWrapper::OptionalCopy(const luabind::object& src, const luabind::object& dst)
{
	using namespace luabind;
	if (!src.is_valid() || !dst.is_valid())
	{
		assert(false);
		return false;
	}
	if (LUA_TTABLE != type(src) || LUA_TTABLE != type(dst))
	{
		assert(false);
		return false;
	}
	for (iterator it(src), it_end; it != it_end; ++it)
	{
		object v(*it);
		auto key = it.key();
		object exist = dst[key];
		if (LUA_TNIL == type(exist))
			rawset(dst, key, v);
	}
	return true;
}

int LStateWrapper::My_DbgTracebackToStr(lua_State *L)
{
	if (!lua_isstring(L, 1))  /* 'message' not a string? */
		return 1;  /* keep it intact */
	lua_pushcfunction(L, luaMy_DbgTraceback);
	lua_pushvalue(L, 1);  /* pass error message */
	lua_pushinteger(L, 2);  /* skip this function and traceback */
	lua_call(L, 2, 1);		//尽量不要用lua_call，尽量用LuaObjCall
	return 1;
}

luabind::object LStateWrapper::Require(const char* modname) const
{
	//不能直接调用ll_require，会出错的。
	using namespace luabind;
	assert(modname);
	object glo = globals(lstate_);
	object f = glo["require"];
	return ::LuaObjCall<luabind::object>(f, modname);
}

void LStateWrapper::DP_Top() const
{
	LOG_O(Log_debug) << lua_gettop(lstate_);
}

void LStateWrapper::DP_Var(int range_min, int range_max, int tbl_lvl) const
{
	auto top_n = lua_gettop(lstate_);
	if (range_max < range_min)
		range_max = top_n;
	if (range_min < 0)
		range_min += top_n + 1;
	if (range_max < 0)
		range_max += top_n + 1;
	if (range_max > top_n)
		range_max = top_n;
	if (range_min > top_n)
		range_min = top_n;
	if (range_min <= 0)
		return;
	assert(range_max >= range_min);
	using namespace luabind;
	for (; range_min <= range_max; ++range_min)
	{
		object o(from_stack(lstate_, range_min));
		const auto& str = LuaSerialize::ToString(o, tbl_lvl);
		LOG_O(Log_debug) << "stack idx(" << range_min << "),type(" << LuaTypeName(o) << "),value->" << str;
	}
}

void LStateWrapper::DP_UpVar(int range_min, int range_max, int tbl_lvl, int fn_level) const
{
	using namespace luabind;
	if (range_min < 1)
	{
		assert(false);
		return;
	}
	lua_Debug ar;
	if (!lua_getstack(lstate_, fn_level, &ar))
		return;
	if (0 == lua_getinfo(lstate_, "uf", &ar))
	{
		assert(false);
		return;
	}
	detail::stack_pop pop(lstate_, 1);
	if (range_max < range_min)
		range_max = ar.nups;
	std::stringstream out;
	for (; range_min <= range_max; ++range_min)
	{
		auto key = lua_getupvalue(lstate_, -1, range_min);
		if (!key)
			break;
		out << ("upvalue idx(") << range_min << "),key:" << key << ",value_type:" << std::flush;
		object o(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		out << LuaTypeName(o) << ",value:" << std::flush;
		const auto& str = LuaSerialize::ToString(o, tbl_lvl);
		out << str << std::endl;
	}
	LOG_O(Log_debug) << out.str();
}

void LStateWrapper::DP_FnInfo(int tbl_lvl, int fn_level) const
{
	std::ostringstream ostm;
	ostm << "DP_FnInfo";
	D_GetFnInfo(fn_level, ostm, tbl_lvl);
	LOG_O(Log_debug) << ostm.str();
}

void LStateWrapper::DP_Traceback() const
{
	using namespace luabind;
	lua_pushcfunction(lstate_, My_DbgTracebackToStr);
	object o(from_stack(lstate_, -1));
	lua_pop(lstate_, 1);
	const auto& str = ::LuaObjCall<std::string>(o, "DP_Traceback");
	LOG_O(Log_debug) << str;
}

void LStateWrapper::DP_TracebackEx(int tbl_lvl) const
{
	std::ostringstream ostm;
	ostm << "DP_TracebackEx";
	for (int level = 0;; ++level)
	{
		if (!D_GetFnInfo(level, ostm, tbl_lvl))
			break;
	}
	LOG_O(Log_debug) << ostm.str();
}

bool LStateWrapper::D_GetFnInfo(int fn_level, std::ostream& out, int tbl_lvl) const
{
	using namespace luabind;
	lua_Debug ar;
	if (!lua_getstack(lstate_, fn_level, &ar))
		return false;
	if (0 == lua_getinfo(lstate_, "Snluf", &ar))
	{
		assert(false);
		return false;
	}
	object the_fn(from_stack(lstate_, -1));
	lua_pop(lstate_, 1);
	out << "\n\t" << ar.short_src << ':' << std::flush;
	if (ar.currentline > 0)
	{
		out << ar.currentline << ':' << std::flush;
	}
	if (*ar.namewhat != '\0')  /* is there a name? */
	{
		out << " in function " << ar.name << std::flush;
	}
	else 
	{
		if (*ar.what == 'm')  /* main? */
			out << " in main chunk" << std::flush;
		else if (*ar.what == 'C' || *ar.what == 't')
			out << " ?" << std::flush;  /* C function or tail call */
		else
		{
			out << ("in function <") << ar.short_src << ':' << ar.linedefined << '>' << std::flush;
		}
	}
	for (int local_idx = 1;; ++local_idx)
	{
		auto key = lua_getlocal(lstate_, &ar, local_idx);
		if (!key)
			break;
		out << ("\nlocal variable idx(") << local_idx << "),key:" << key << ",value_type:" << std::flush;
		object o(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		out << LuaTypeName(o) << ",value:" << std::flush;
		const auto& str = LuaSerialize::ToString(o, tbl_lvl);
		out << str << std::flush;
	}
	//////////////////////////////////////////////////////////////////////////
	the_fn.push(lstate_);
	detail::stack_pop pop(lstate_, 1);
	for (int local_idx = 1; local_idx <= ar.nups; ++local_idx)
	{
		auto key = lua_getupvalue(lstate_, -1, local_idx);
		if (!key)
			break;
		out << ("\nupvalue idx(") << local_idx << "),key:" << key << ",value_type:" << std::flush;
		object o(from_stack(lstate_, -1));
		lua_pop(lstate_, 1);
		out << LuaTypeName(o) << ",value:" << std::flush;
		const auto& str = LuaSerialize::ToString(o, tbl_lvl);
		out << str;
	}
	return true;
}

int LStateWrapper::LuaD_GetLevelFnName(lua_State* L)
{
	int level = 1;
	auto top_n = lua_gettop(L);
	if (top_n > 0)
	{
		if (LUA_TNUMBER == lua_type(L, 1))
		{
			level = (int)lua_tonumber(L, 1);
		}
	}
	using namespace luabind;
	lua_Debug ar;
	if (!lua_getstack(L, level, &ar))
	{
		assert(false);
		return 0;
	}
	if (0 == lua_getinfo(L, "n", &ar))
	{
		assert(false);
		return 0;
	}
	if (!ar.name)
	{
		lua_pushstring(L, "未知函数");
		//assert(false);
		return 1;
	}
	lua_pushstring(L, ar.name);
	return 1;
}

LuaObjectPair::LuaObjectPair(const luabind::object& the_first, const luabind::object& the_second)
	: _Myt(the_first, the_second)
{
}

LuaObjectPair::LuaObjectPair() : _Myt()
{
}

LuaObjectPair::LuaObjectPair(const luabind::object& the_first) : _Myt(the_first, luabind::object())
{

}

const luabind::object& LuaObjectPair::First() const
{
	return first;
}

luabind::object& LuaObjectPair::First()
{
	return first;
}

const luabind::object& LuaObjectPair::Second() const
{
	return second;
}

luabind::object& LuaObjectPair::Second()
{
	return second;
}

const luabind::object& LuaObjectPair::_First_()
{
	return first;
}

const luabind::object& LuaObjectPair::_Second_()
{
	return second;
}

void LuaObjectPair::RegForLua(lua_State* l)
{
	assert(l);
	using namespace luabind;
	module(l)[
		class_<LuaObjectPair>("LuaObjectPair")
		.def(constructor<>())
		.def(constructor<const luabind::object&>())
		.def(constructor<const luabind::object&, const luabind::object&>())
		.def_readwrite("first", &LuaObjectPair::_First_)
		.def_readwrite("second", &LuaObjectPair::_Second_)
	];
}

LuaObjectPair::operator bool() const
{
	return first.is_valid() || second.is_valid();
}

LuaSerializable::~LuaSerializable()
{

}
