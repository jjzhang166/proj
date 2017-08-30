--dofile==每次都要编译。loadfile==只编译(加载)文件一次
--require不会多次加载同一个文件。

__g_lua_files__ = {
	"Test", "Logic", "task", "BodyPartInfo", "MaterialMap", "TotalStagePosInfo",
	"Scri_Commands"
};

--set lua path
package.path = GetLuaDirectory() .. "?.lua" ..
	";" .. GetScriptDirectory() .. "?.lua";

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
	{ "没", "not" },
	{ "执行", "do" },
	{ "且", "and" },
	{ "或", "or" },
	{ "转换成字符串", "tostring" },
	{ "定义函数", "function" },
	{ "重复", "repeat" },
	{ "直到", "until" },
	{ "中断", "break" }
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

--加载脚本文件
function 加载脚本文件( file_name )
	SetCurRunScriptNameLevel0(file_name)
	RunInfo("加载脚本文件:" .. file_name)
	return LoadScriptFile(file_name);
end

function 加载配置脚本文件( file_name )
	RunInfo("加载配置脚本文件:" .. file_name)
	return LoadScriptFile("配置\\" .. file_name);
end

function 空函数()
end

function 加载任务目录脚本( 脚本名 )
	return LoadScriptFile("任务\\" .. 脚本名);
end

function RetBoolDef( res, def )
	if type(res) ~= "boolean" then
		return def
	end
	return res
end
---------------------------------------
--load all lua files for initial
LoadAndReloadLuaFiles(GLuac_Instance());

g_str_MakeMatchSubStrmb = "ad3738d3-6c45-5077-b5e5-809dd30a4c52"
g_str_MakeMatchSubStrxe = "09fbf906-9ba4-5fd7-82cc-f48609ebb29e"
g_str_MakeMatchSubStrpmb = "d972ebbf-aac4-56fc-8879-dcfec2c3d46b"
