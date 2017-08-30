
g_is_start_run_info = true
g_is_out_to_console = true
function RunInfo( str )
	if g_is_start_run_info then
		LuaLog("当前执行信息：" .. str);
		if g_is_out_to_console then
			OutputScriptInfo(str)
		end
	end
end


function LuaHandleRoleEnteredGame( func, ... )
	local the_param = { ... }
	return scmd():HandleRoleEnteredGame(function ()
		return func(unpack(the_param))
	end)
end

function LuaHandleGameScene( func, ... )
	local the_param = { ... }
	return scmd():HandleGameScene(function ()
		return func(unpack(the_param))
	end)
end

function 给表添加一个唯一的值( tbl, value )
	if type(tbl) ~= "table" then
		LuaLogE("给表添加一个唯一的值之表参数必须是表类型")
		return false
	end
	if value == nil then
		LuaLogE("给表添加一个唯一的值之值参数必须不能为空")
		return false
	end
	for _, v in ipairs(tbl) do
		if v == value then
			return true
		end
	end
	RunInfo("给表添加一个唯一的值，值：" .. tostring(value))
	table.insert(tbl, value)
	return true
end
