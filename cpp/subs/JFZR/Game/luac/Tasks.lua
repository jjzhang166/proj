--接受指定任务
function AcceptTheTask( quest_ch_name )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("UpdateRole error");
		return;
	end
	if not gps:AcceptTheTask(quest_ch_name) then
		LuaLogE("接受指定任务失败");
		return;
	end
	Delay(100);
	gps:GetReceivedTask():RebuildUpdateAll();
end

--存在指定的已接任务
function IsAcceptedTheTask( quest_ch_name )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("UpdateRole error");
		return false;
	end
	gps:GetReceivedTask():RebuildUpdateAll();
	return gps:IsAcceptedTheTask(quest_ch_name);
end

--指定的已接任务已完成
function TheAcceptedTaskCompletable( quest_ch_name )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("UpdateRole error");
		return false;
	end
	gps:GetReceivedTask():RebuildUpdateAll();
	return gps:TheTaskCompletable(quest_ch_name);
end

--自动提交完成任务
function AutoCommitCompleteTasks()
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("更新角色失败");
		return;
	end
	if not gps:CommitNoCompleteHandlerTasks() then
		LuaLogE("自动提交完成任务失败");
	end
end

--提交所有完成任务
function CommitAllCompleteTasks()
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("更新角色失败");
		return;
	end
	if not gps:CommitCompleteTasks() then
		LuaLogE("自动提交完成任务失败");
	end
end

--重置任务处理映射
function RestTaskHandlerMap( func )
	if type(func) ~= "function" then
		LuaLogE("重置任务处理映射之参数必须是函数类型");
		return false;
	end
	g_task_handle_map_ = {};
	if not GameEn2ChNodeMgr_Me():RebuildUpdateAll() then
		LuaLogE("中英映射机制初始化失败，所以重置任务处理映射失败");
		return false;
	end
	func();
	LuaLog("重置任务处理映射之重置完成，添加了[" .. table.getn(g_task_handle_map_) .. "]项任务处理");
	return true;
end

function TraverseAllTaskHandlers_()
	for i, v in ipairs(g_task_handle_map_) do
		LuaLogW(tostring(i) .. "\tch->" .. v[g_THI_ch_name_] .. "\ten id->" .. WideCharToGbk(v[g_THI_en_id_]) .. "\tkey content->" .. tostring(v[g_THI_key_content_])
		.. "\tfull content->" .. WideCharToGbk(v[g_THI_full_content_]));
	end
end

--根据中文任务名，查找任务处理
function FindTaskHandlerByCh( quest_ch_name )
	if type(quest_ch_name) ~= "string" then
		LuaLogE("查找任务处理之参数必须是字符串类型");
		return;
	end
	for _, v in ipairs(g_task_handle_map_) do
		if v[g_THI_ch_name_] == quest_ch_name then
			return v;
		end
	end
end

g_task_handle_map_ = {};
--根据英文任务ID，查找任务处理
function FindTaskHandlerByEn( quest_id, role_lvl )
	if quest_id == nil or quest_id:empty() then
		LuaLogE("quest_id为空");
		return;
	end
	if role_lvl ~= nil then
		local lvl_limt;
		for _, v in ipairs(g_task_handle_map_) do
			lvl_limt = v[g_THI_lvl_limit_];
			if lvl_limt <= 0 or role_lvl <= lvl_limt then
				if quest_id:compare(v[g_THI_en_id_]) == 0 then
					return v;
				end
			end
		end
		return;
	end
	for _, v in ipairs(g_task_handle_map_) do
		if quest_id:compare(v[g_THI_en_id_]) == 0 then
			return v;
		end
	end
end

function ExistTaskCompleteHandler( quest_id, role_lvl )
	local handler_tbl = FindTaskHandlerByEn(quest_id, role_lvl);
	if handler_tbl == nil then
		return false;
	end
	if handler_tbl[g_THI_complete_func_] ~= nil then
		return true;
	end
	return false;
end

--task handler index
g_THI_ch_name_ = 1;
g_THI_en_id_ = 2;
g_THI_do_func = 3;
g_THI_lvl_limit_ = 4;
g_THI_complete_func_ = 5;
g_THI_key_content_ = 6;
g_THI_full_content_ = 7;
g_THI_match_quest_id_ = 8;

--添加任务处理，role_lvl_limit表示角色达到该等级则不做该任务，为0或没有该参数表示不忽略
function AddTaskHandler( quest_ch_name, func, role_lvl_limit, complete_func, key_content, match_quest_id )
	if type(quest_ch_name) ~= "string" then
		LuaLogE("添加任务处理之任务名必须是字符串类型,任务名->" .. quest_ch_name);
		return;
	end
	if type(func) ~= "function" then
		LuaLogE("添加任务处理之处理任务的参数必须是函数类型,任务名->" .. quest_ch_name);
		return;
	end
	if complete_func ~= nil and type(complete_func) ~= "function" then
		LuaLogE("添加任务处理之完成任务的参数必须是函数类型,任务名->" .. quest_ch_name);
		return;
	end
	if role_lvl_limit == nil then
		role_lvl_limit = 0;
	end
	if type(role_lvl_limit) ~= "number" then
		LuaLogE("添加任务处理之角色等级限制必须是数字类型,任务名->" .. quest_ch_name);
		return;
	end
	if key_content == nil then
		key_content = GetEmptyString();
	end
	if type(key_content) ~= "string" then
		LuaLogE("添加任务处理之关键任务内容必须是字符串类型,任务名->" .. quest_ch_name);
		return;
	end
	if match_quest_id == nil then
		match_quest_id = GetEmptyString();
	end
	if type(match_quest_id) ~= "string" then
		LuaLogE("添加任务处理之匹配任务ID的参数必须是字符串类型,任务名->" .. quest_ch_name);
		return;
	end
	local en2ch_mgr = GameEn2ChNodeMgr_Me();
	local quest_index = en2ch_mgr:GetQuestIndex();
	if quest_index == nil then
		LuaLogE("string quest_index不存在，所以添加任务处理失败,任务名->" .. quest_ch_name);
		return;
	end
	local wstr_quest_id = quest_index:ThreeCh2EnA(quest_ch_name, key_content, match_quest_id);
	if wstr_quest_id:empty() then
		LuaLogE("添加任务处理之找不到这样的任务->" .. quest_ch_name);
		return;
	end
	--[[
	local task_template = TaskCurJobTemplateMgr_Me();
	if not task_template:RebuildUpdateAll() then
		task_template:RebuildUpdateAll()
	end
	if task_template:FindByTaskId(wstr_quest_id) == nil then
		LuaLogE("添加任务处理之实际上找不到这样的任务id->" .. WideCharToGbk(wstr_quest_id) .. ",任务名为->" .. quest_ch_name);
		return;
	end
	--]]
	if FindTaskHandlerByEn(wstr_quest_id) ~= nil then
		LuaLogE(quest_ch_name .. "\t该任务名已经存在任务处理映射了，所以此次添加不成功,任务名->" .. quest_ch_name);
		return;
	end
	local content_idx = en2ch_mgr:GetQuestBeginDescIndex();
	if content_idx == nil then
		LuaLogE("添加任务处理之conten index不存在,任务名->" .. quest_ch_name);
		return;
	end
	--LuaLog("添加任务处理之成功添加->" .. quest_ch_name);
	local full_content = content_idx:En2Ch(wstr_quest_id);
	table.insert(g_task_handle_map_, { quest_ch_name, wstr_quest_id, func, role_lvl_limit, complete_func, key_content, full_content, match_quest_id });
end

--做一个已接任务处理，如果没有去做则返回假,否则为真
function RunOneAcceptedTaskHandler()
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("角色更新Failed");
		return false;
	end
	local role_lvl = gps:GetLevel();
	if role_lvl < 1 then
		LuaLogE("角色等级不正确->" .. tostring(role_lvl));
		return false;
	end
	local accepted_task_mgr = gps:GetReceivedTask();
	local handler_tbl;
	for accepted_task in Closure_Iterator(accepted_task_mgr:TraverseUncompleteTasks()) do
		handler_tbl = FindTaskHandlerByEn(accepted_task:GetStrTaskId(), role_lvl);
		if handler_tbl ~= nil then
			break;
		end
	end
	if handler_tbl == nil then
		return false;
	end
	local do_func = handler_tbl[g_THI_do_func];
	if do_func == nil then
		assert(false, "任务处理之处理函数为空");
		return false;
	end
	LuaLog("开始处理任务->" .. handler_tbl[g_THI_ch_name_]);
	local res = do_func();
	LuaLog("处理任务完成->" .. handler_tbl[g_THI_ch_name_]);
	if res ~= nil then
		if type(res) ~= "boolean"  then
			LuaLogE(handler_tbl[g_THI_ch_name_] .. "\t的任务处理函数的返回类型必须是布尔类型，用来表示该函数执行成功与否");
			return false;
		end
		if res == false then
			return false;
		end
	end
	--AutoCommitCompleteTasks();
	return true;
end

function RunTheAcceptedTaskHandler( quest_id )
	local task_handler = FindTaskHandlerByEn(quest_id);
	if task_handler == nil then
		return false;
	end
	local do_handler = task_handler[g_THI_do_func]
	if do_handler == nil then
		return false;
	end
	do_handler();
	return true;
end

--完成一个已接任务处理，如果没有去做则返回假,否则为真
function RunOneCompletableTaskHandler()
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("角色更新Failed");
		return false;
	end
	local role_lvl = gps:GetLevel();
	if role_lvl < 1 then
		LuaLogE("角色等级不正确->" .. tostring(role_lvl));
		return false;
	end
	local accepted_task_mgr = gps:GetReceivedTask();
	local complete_handler, handler_tbl;
	for accepted_task in Closure_Iterator(accepted_task_mgr:TraverseCompleteTasks()) do
		handler_tbl = FindTaskHandlerByEn(accepted_task:GetStrTaskId(), role_lvl);
		if handler_tbl ~= nil then
			complete_handler = handler_tbl[g_THI_complete_func_];
			if complete_handler ~= nil then
				break;
			end
		end
	end
	if complete_handler == nil then
		return false;
	end
	LuaLog("开始处理完成任务->" .. handler_tbl[g_THI_ch_name_]);
	local res = complete_handler();
	LuaLog("处理完成任务完成->" .. handler_tbl[g_THI_ch_name_]);
	if res ~= nil then
		if type(res) ~= "boolean"  then
			LuaLogE(handler_tbl[g_THI_ch_name_] .. "\t的任务完成处理函数的返回类型必须是布尔类型，用来表示该函数执行成功与否");
			return false;
		end
		if res == false then
			return false;
		end
	end
	if not accepted_task_mgr:RebuildUpdateAll() then
		LuaLogE("完成一个已接任务处理之，后面更新已接任务信息失败");
		return false;
	end
	return gps:CommitTask(handler_tbl[g_THI_en_id_]);
end

function RunTheCompletableTaskHandler( quest_id )
	local task_handler = FindTaskHandlerByEn(quest_id);
	if task_handler == nil then
		return false;
	end
	local complete_handler = task_handler[g_THI_complete_func_]
	if complete_handler == nil then
		return false;
	end
	complete_handler();
	return true;
end

--接受一个任务处理任务，如果没有接受成功则返回假，否则为真
function AcceptOneTaskHandlerTask()
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("角色更新Failed");
		return false;
	end
	local role_lvl = gps:GetLevel();
	if role_lvl < 1 then
		LuaLogE("角色等级不正确->" .. tostring(role_lvl));
		return false;
	end
	local npc_mgr = TaskNpcMgr_Me();
	if not npc_mgr:RebuildUpdateAll() then
		LuaLogE("task npc mgr update failed");
		return false;
	end
	local quest_id;
	for task_npc in Closure_Iterator(npc_mgr:TraverseAcceptableNpc()) do
		for task_node in Closure_Iterator(task_npc:GetAcceptableMgr():TraverseAllTask()) do
			if FindTaskHandlerByEn(task_node:GetTaskId(), role_lvl) ~= nil then
				quest_id = task_node:GetTaskId();
				break;
			end
		end
		if quest_id ~= nil then
			break;
		end
	end
	if quest_id == nil then
		return false;
	end
	if not gps:AcceptTheTaskW(quest_id) then
		return false;
	end
	Delay(300);
	return true;
end

--自动做任务处理任务
function AutoRunTaskHandlerTasks()
	repeat
		AutoCommitCompleteTasks();
		repeat
		until not AcceptOneTaskHandlerTask();
	until not RunOneAcceptedTaskHandler() and not RunOneCompletableTaskHandler();
	return true;
end

function AcceptAllTaskHandlerTasks()
	AutoCommitCompleteTasks();
	repeat
	until not AcceptOneTaskHandlerTask();
end

--接受任意一个任务
function AcceptLocalMapAnyTask()
	UpdateMapRoom();
	UpdateAllGameObjs();
	local gps = UpdateRole();
	if gps == nil then
		return false;
	end
	return gps:AcceptLocalMapAnyTask();
end

--接受本地图任意所有任务
function AcceptLocalMapAnyAllTasks()
	local gps = UpdateRole();
	if gps == nil then
		return 0;
	end
	return gps:AcceptLocalMapAnyAllTasks();
end

--索引任务内容的关键部分
function IdxTaskKeyContents( sub_contents )
	if type(sub_contents) ~= "string" then
		LuaLogE("索引任务内容的关键部分的参数必须是字符串类型");
		assert(false, "索引任务内容的关键部分的参数必须是字符串类型");
		return;
	end
	return MakeMatchSubStr(sub_contents);
end

--索引任务ID的关键部分
function IdxTaskIdKey( sub_contents )
	if type(sub_contents) ~= "string" then
		LuaLogE("索引任务ID的关键部分的参数必须是字符串类型");
		assert(false, "索引任务ID的关键部分的参数必须是字符串类型");
		return;
	end
	return MakeMatchSubStr(sub_contents);
end

--添加详细的任务处理
function AddSpecifyTaskHandler( quest_ch_name, func, role_lvl_limt, complete_func, key_sub, match_quest_id )
	local key_content;
	if key_sub ~= nil then
		key_content = key_sub.str_;
	end
	local match_quest;
	if match_quest_id ~= nil then
		match_quest = match_quest_id.str_;
	end
	return AddTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_content, match_quest);
end

--添加详细的职业任务处理
function AddSpecifyJobTaskHandler( quest_ch_name, func, role_lvl_limt, complete_func, key_sub )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("添加详细的职业任务处理之角色信息更新失败，建议角色进入游戏后再重置任务处理映射");
		return;
	end
	local job_name = gps:GetJobName();
	if job_name:empty() then
		LuaLogE("添加详细的职业任务处理之角色的职业名为空");
		return;
	end
	return AddSpecifyTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_sub, IdxTaskIdKey(WideCharToGbk(job_name)));
end

function AddSpecifySubJobTaskHandler( quest_ch_name, func, role_lvl_limt, complete_func, key_sub )
	local gps = UpdateRole();
	if gps == nil then
		LuaLogE("添加详细的分支职业任务处理之角色信息更新失败，建议角色进入游戏后再重置任务处理映射");
		return;
	end
	local job_name = gps:GetSpecifyJobName();
	if job_name:empty() then
		LuaLogE("添加详细的分支职业任务处理之角色的职业名为空");
		return;
	end
	return AddSpecifyTaskHandler(quest_ch_name, func, role_lvl_limt, complete_func, key_sub, IdxTaskIdKey(WideCharToGbk(job_name)));
end
