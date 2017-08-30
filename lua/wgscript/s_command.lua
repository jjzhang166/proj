require("s_cmncfg")
s_command = {
    RunInfo = nil,
    RunInfoP = nil,     --P->param
    RunInfoF = nil,     --F->Format
    RunInfoS = nil,     --S->Specific
    CopyAssetFile = nil,
    ----------------------
    --test
    TestReloadCmnLua = nil,
    TestReloadGameLua = nil,
    ----------------------
    开启执行信息 = nil,
    关闭执行信息 = nil,
    开启输出到控制台 = nil,
    关闭输出到控制台 = nil,
    延迟 = nil,       --单位秒
    等待 = nil,       --单位秒
    空函数 = nil,
    加载脚本 = nil,
    加载配置脚本 = nil,
    加载任务脚本 = nil,
    自动选择运行脚本 = nil,
    改变运行的脚本 = nil,
    执行并等待事件 = nil,
    触发事件 = nil,
    ----------------------
    设置最大角色数量 = nil,
    自动创建选择角色 = nil,
}
GetMyApp():AddScriptExecEnv("s_command", s_command)
local g_is_start_run_info = true
local g_is_out_to_console = true
local print = print
local OutputScriptInfo = OutputScriptInfo

function s_command.RunInfo(...)
    if g_is_start_run_info then
        local str = tostring(...)
		print("执行：" .. str)
		if g_is_out_to_console then
			OutputScriptInfo(str)
		end
	end
end

function s_command.RunInfoP(...)
	if g_is_start_run_info then
        local fnName = LuaD_GetLevelFnName(2)
        local p_n = select("#", ...)
        local str
        if p_n > 0 then
            str = string.rep("%s,\t", p_n - 1)
            str = str .. "%s"
            str = string.format(str, ...)
        else
            str = ""
        end
        str = fnName .. "(" .. str .. ")"
		print("执行：" .. str)
		if g_is_out_to_console then
			OutputScriptInfo(fnName)
		end
	end
end

function s_command.RunInfoF(...)
	if g_is_start_run_info then
        local param = {...}
        local str = ""
        local idx = 0
		for i = 1, table.maxn(param) / 2 do
			str = str .. "\t" .. param[i] .. ":%s"
            idx = idx + 1
		end
        for i = 1, idx do
            table.remove(param, 1)
        end
        str = string.format(str, unpack(param))
        local fnName = LuaD_GetLevelFnName(2)
        str = fnName .. "\t" .. str
		print("执行：" .. str)
		if g_is_out_to_console then
			OutputScriptInfo(fnName)
		end
	end
end

function s_command.RunInfoS(...)
	if g_is_start_run_info then
        local fnName = LuaD_GetLevelFnName(2)
        local str = fnName .. "\t\t" .. tostring(...)
		print("执行：" .. str)
		if g_is_out_to_console then
			OutputScriptInfo(fnName)
		end
	end
end
local RunInfoP = s_command.RunInfoP
local RunInfoF = s_command.RunInfoF
local RunInfoS = s_command.RunInfoS

function s_command.CopyAssetFile(fileName, newFilePath)
    return scmd.CopyAssetFile(fileName, newFilePath)
end

function s_command.TestReloadCmnLua(dirName, scriptName)
    local app = GetMyApp()
    app:EraseScriptExecEnv(scriptName)
    dofile([[F:\proj\lua\]] .. dirName "\\".. scriptName .. ".lua")
end

function s_command.TestReloadGameLua(projName, scriptName, isAndroid)
    local path = [[F:\proj\cpp\subs\]]
    if isAndroid then
        path = path .. [[android\]]
    end
    dofile(path .. projName .. [[\Game\lua\]] .. scriptName .. ".lua")
end

function s_command.开启执行信息()
	g_is_start_run_info = true
end

function s_command.关闭执行信息()
	g_is_start_run_info = false
end

function s_command.开启输出到控制台()
	g_is_out_to_console = true
end

function s_command.关闭输出到控制台()
	g_is_out_to_console = false
end

function s_command.延迟(seconds)
    if seconds == nil then seconds = 0.01 end
    RunInfoP(seconds .. "秒")
    return Sleep(seconds * 1000)
end

function s_command.等待(seconds)
    if seconds == nil then seconds = 0.01 end
    RunInfoP(seconds .. "秒")
    return Sleep(seconds * 1000)
end

function s_command.空函数()
end

function s_command.加载脚本(file_name)
	RunInfoP(file_name)
	return LoadScriptFile(file_name)
end

function s_command.加载配置脚本(file_name)
	RunInfoP(file_name)
	return LoadScriptFile("配置\\" .. file_name);
end

function s_command.加载任务脚本(file_name)
    RunInfoP(file_name)
	return LoadScriptFile("任务\\" .. file_name);
end

function s_command.自动选择运行脚本(sceneHandler, fnGetScriptName)
    RunInfoP()
	return AutoChooseRunScript(sceneHandler, fnGetScriptName);
end

function s_command.改变运行的脚本(topN)
    if topN == nil then topN = 0 end
    RunInfoP(topN)
	return ChangeRunningScript(topN);
end

function s_command.执行并等待事件(fnRun, fnEventHandler, event)
    RunInfoP()
	return RunWaitEvent(fnRun, fnEventHandler, event);
end

function s_command.触发事件(eventName)
    RunInfoP(eventName)
	return TriggerEvent(eventName);
end

function s_command.设置最大角色数量(maxRoleCnt)
    RunInfoP(maxRoleCnt)
	return SetMaxGameRoles(maxRoleCnt);
end

function s_command.自动创建选择角色()
    RunInfoP()
	return AutoCreateSelectRole();
end
