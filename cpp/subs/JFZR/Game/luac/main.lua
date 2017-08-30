--dofile==每次都要编译。loadfile==只编译(加载)文件一次
--require不会多次加载同一个文件。

__g_lua_files__ = {
	"Test", "Logic", "Tasks", "Logic_GWnd", "HandleAmbient",
	"Scri_Commands", "SendRecvMap",
	"GameStateSetter"
};

--set lua path
package.path = GetLuaDirectory() .. "?.lua" ..
	";" .. GetScriptDirectory() .. "?.lua";

--中文脚本支持
__g_chinese_script_support__ = {
	{ "如果", "if" },
	{ "那么", "then" },
	{ "结束", "end" },
	{ "否则", "else" },
	{ "否则如果", "elseif" },
	{ "循环", "while" },
	{ "局部变量", "local" },
	{ "返回", "return" },
	{ "空", "nil" },
	{ "真", "true" },
	{ "假", "false" },
	{ "不", "not" },
	{ "没有", "not" },
	{ "执行", "do" },
	{ "且", "and" },
	{ "或", "or" },
	{ "转换成字符串", "tostring" },
	{ "定义函数", "function" },
	{ "重复", "repeat" },
	{ "直到", "until" },
};

--return true
function RetTrue()
	return true;
end

g_tbl_make_match_substr_ = {};
g_tbl_make_match_substr_.__add = function( lhs, rhs )
	if type(rhs) ~= "string" then
		LuaLogE("参数必须是字符串类型");
		assert(false, "生成匹配子字符串的参数必须是字符串类型");
		return;
	end
	lhs.str_ = lhs.str_ .. WideCharToGbk(UnsensePerlMetachar(GbkToWideChar(rhs))) .. ".*";
	return lhs;
end

--生成匹配子字符串
function MakeMatchSubStr( str_head )
	if type(str_head) ~= "string" then
		LuaLogE("参数必须是字符串类型");
		assert(false, "生成匹配子字符串的参数必须是字符串类型");
		return;
	end
	local tbl_tmp = {};
	tbl_tmp.str_ = ".*" .. WideCharToGbk(UnsensePerlMetachar(GbkToWideChar(str_head))) .. ".*";
	setmetatable(tbl_tmp, g_tbl_make_match_substr_);
	return tbl_tmp;
end

--load all lua files for initial
LoadAndReloadLuaFiles(GLuac_Instance());


