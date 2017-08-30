--[[
Account = { x = 1 }
function Account:new( o )
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end
--]]
----------------------------------------------------------------------------------
TaskItemHandler = {}
function TaskItemHandler:MakeItem( task_id, task_name, accept_handler, do_handler, done_handler, unknown_param )
	if accept_handler ~= nil and type(accept_handler) ~= "function" then
		LuaLogE("添加任务处理之处理接任务的参数必须是函数类型,任务名->" .. task_name);
		return;
	end
	if do_handler ~= nil and type(do_handler) ~= "function" then
		LuaLogE("添加任务处理之处理做任务的参数必须是函数类型,任务名->" .. task_name);
		return;
	end
	if done_handler ~= nil and type(done_handler) ~= "function" then
		LuaLogE("添加任务处理之处理完成任务的参数必须是函数类型,任务名->" .. task_name);
		return;
	end
	if accept_handler == nil and do_handler == nil and done_handler == nil then
		LuaLogE("添加任务处理之处理接受任务、处理做任务和处理完成任务的函数都为空，添加该任务处理失败,任务名->" .. task_name);
		return;
	end

	local task_handler_item = {}
	task_handler_item.task_id = task_id;
	task_handler_item.task_name = task_name;
	task_handler_item.accept_handler = accept_handler;
	task_handler_item.do_handler = do_handler;
	task_handler_item.done_handler = done_handler;
	task_handler_item.unknown_param = unknown_param;
	return task_handler_item;
end

function TaskItemHandler:EqualId( lhs, rhs )
	return lhs.task_id == rhs.task_id;
end

function TaskItemHandler:TaskName( o )
	return o.task_name;
end

function TaskItemHandler:AcceptExec( o )
	if o == nil then
		return false;
	end
	if o.accept_handler == nil then
		return false
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	if received_task:FindById(o.task_id) ~= nil then
		--已经接过了，就不再同时接另一个任务了
		return true;
	end

	LuaLog("去接该任务:" .. o.task_name);
	o.accept_handler(o.task_id, o.task_name, o.unknown_param)
	received_task:RebuildAll()
	if received_task:FindById(o.task_id) ~= nil then
		LuaLog("成功接到该任务:" .. o.task_name);
		return true
	else
		LuaLogE("无法接取该任务:" .. o.task_name);
		return false
	end

	assert(false)
	return false;
end

function TaskItemHandler:CanAccept( o )
	if o == nil then
		return false;
	end
	if o.accept_handler == nil then
		return false
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	if received_task:FindById(o.task_id) ~= nil then
		return false
	end

	return true;
end

function TaskItemHandler:DoExec( o )
	if o == nil then
		return false;
	end
	if o.do_handler == nil then
		return false;
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	local the_task = received_task:FindById(o.task_id)
	if the_task == nil then
		LuaLogE("没有接取成功，所以无法做该任务:" .. o.task_name);
		return false;
	end
	if the_task:IsDone() then
		--LuaLogE("已经完成了，所以不用做该任务:" .. o.task_name);
		--同一时刻保证只有一个可完成的任务存在
		return true;
	end

	LuaLog("去做该任务:" .. o.task_name);
	o.do_handler(o.task_id, o.task_name, o.unknown_param)
	received_task:RebuildAll();
	the_task = received_task:FindById(o.task_id)
	if the_task == nil or not the_task:IsDone() then
		LuaLogE("没有做成功该任务:" .. o.task_name);
		return false
	else
		LuaLog("成功做了该任务:" .. o.task_name);
		return true
	end

	assert(false)
	return false;
end

function TaskItemHandler:CanDo( o )
	if o == nil then
		return false;
	end
	if o.do_handler == nil then
		return false;
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	local the_task = received_task:FindById(o.task_id)
	if the_task == nil then
		LuaLogE("没有接取成功，所以无法做该任务2:" .. o.task_name);
		return false;
	end
	if the_task:IsDone() then
		return false;
	end

	return true;
end

function TaskItemHandler:DoneExec( o )
	if o == nil then
		return false;
	end
	if o.done_handler == nil then
		return false;
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	local the_task = received_task:FindById(o.task_id)
	if the_task == nil then
		LuaLogE("没有接取成功，所以无法完成该任务:" .. o.task_name);
		return false;
	end
	if not the_task:IsDone() then
		--LuaLogE("还未做成功，所以无法完成该任务:" .. o.task_name);
		return false;
	end

	LuaLog("去完成该任务:" .. o.task_name);
	o.done_handler(o.task_id, o.task_name, o.unknown_param)
	received_task:RebuildAll();
	the_task = received_task:FindById(o.task_id)
	if the_task == nil then
		LuaLog("成功完成了该任务:" .. o.task_name);
		return true
	else
		LuaLogE("没有成功完成该任务:" .. o.task_name);
		return false
	end

	assert(false)
	return true;
end

function TaskItemHandler:CanDone( o )
	if o == nil then
		return false;
	end
	if o.done_handler == nil then
		return false;
	end

	local received_task = GPlayer_Me():GetReceivedTaskMgr();
	local the_task = received_task:FindById(o.task_id)
	if the_task == nil then
		LuaLogE("没有接取成功，所以无法完成该任务2:" .. o.task_name);
		return false;
	end
	if not the_task:IsDone() then
		return false;
	end
	return true
end

function TaskItemHandler:Name2IdImpl( tra_task, task_id, task_name )
	if task_id ~= nil then
		for v in Closure_Iterator(tra_task) do
			if v:GetTaskId() == task_id then
				return true, task_id;
			end
		end
	else
		local task_id_first;
		local cnt = 0;
		for v in Closure_Iterator(tra_task) do
			if 0 == cnt then
				task_id_first = v:GetTaskId();
				cnt = cnt + 1;
			elseif 1 == cnt then
				LuaLogW("添加任务处理之有同名的任务,默认匹配第一个任务，任务名->" .. task_name);
				break
			end
		end
		if task_id_first ~= nil then
			return true, task_id_first
		end
	end
	return false
end

function TaskItemHandler:Name2Id( task_id, task_name )
	if type(task_name) ~= "string" then
		LuaLogE("添加任务处理之任务名必须是字符串类型,任务名->" .. task_name);
		return false;
	end
	if task_id ~= nil and type(task_id) ~= "number" then
		LuaLogE("添加任务处理之任务ID必须是数字类型,任务名->" .. task_name);
		return false;
	end

	local task_mgr = GTaskTemplateMgr_Me();
	local tra_task = task_mgr:TraverseMainTaskByName(task_name);
	if tra_task == nil then
		assert(false);
		return false;
	end
	local res, real_task_id = TaskItemHandler:Name2IdImpl(tra_task, task_id, task_name)
	if res == true then
		return res, real_task_id
	end

	tra_task = task_mgr:TraverseAllTaskByName(task_name);
	if tra_task == nil then
		assert(false);
		return false;
	end
	res, real_task_id = TaskItemHandler:Name2IdImpl(tra_task, task_id, task_name)
	if res == true then
		return res, real_task_id
	end

	LuaLogW("添加任务处理之不存在这样的任务，任务名->" .. task_name);
	return false;
end

function TaskItemHandler:TraverseInfo( o )
	LuaLog("任务id: " .. tostring(o.task_id) .. " 任务名:" .. o.task_name .. "  接函数:" .. tostring(o.accept_handler)
		.. "  做函数：" .. tostring(o.do_handler) .. "  完成函数：" .. tostring(o.done_handler) ..
		"  自定义参数：" .. tostring(o.unknown_param));
end
----------------------------------------------------------------------------------
TaskItemHandlerMgr = {}
function TaskItemHandlerMgr:AddItem( task_name, accept_handler, do_handler, done_handler, unknown_param, task_id )
	local res, real_task_id = TaskItemHandler:Name2Id(task_id, task_name);
	if res == false then
		return false
	end
	if self:FindById(real_task_id) ~= nil then
		LuaLogW("添加任务处理之已存在这样的任务处理了，之前的任务处理映射将被覆盖，任务名->" .. task_name)
	end
	local task_item = TaskItemHandler:MakeItem(real_task_id, task_name, accept_handler, do_handler, done_handler, unknown_param)
	if task_item == nil then
		return false;
	end
	self:GetStoreTbl()[real_task_id] = task_item;
	return true;
end

function TaskItemHandlerMgr:GetStoreTbl()
	assert(false)
end

function TaskItemHandlerMgr:FindById( task_id )
	return self:GetStoreTbl()[task_id];
end

function TaskItemHandlerMgr:AcceptById( task_id )
	return TaskItemHandler:AcceptExec(self:FindById(task_id));
end

function TaskItemHandlerMgr:DoById( task_id )
	return TaskItemHandler:DoExec(self:FindById(task_id));
end

function TaskItemHandlerMgr:DoneById( task_id )
	return TaskItemHandler:DoneExec(self:FindById(task_id));
end

function TaskItemHandlerMgr:AutoAcceptOne()
	local gpm = GPlayer_Me();
	local accept = gpm:GetAcceptableTasks();
	accept:Update();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll()
	local tbl = {}
	accept:CpyIdToLua(tbl);
	for _, v in ipairs(tbl) do
		if self:AcceptById(v) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:HasAcceptableOne()
	local gpm = GPlayer_Me();
	local accept = gpm:GetAcceptableTasks();
	accept:Update();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll()
	local tbl = {}
	accept:CpyIdToLua(tbl);
	for _, v in ipairs(tbl) do
		if TaskItemHandler:CanAccept(self:FindById(v)) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:AutoDoOne()
	local gpm = GPlayer_Me();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll();
	local tbl = {}
	received:CpyIdToLua(tbl, false);
	for _, v in ipairs(tbl) do
		if self:DoById(v) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:HasCanDoOne()
	local gpm = GPlayer_Me();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll();
	local tbl = {}
	received:CpyIdToLua(tbl, false);
	for _, v in ipairs(tbl) do
		if TaskItemHandler:CanDo(self:FindById(v)) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:AutoDoneOne()
	local gpm = GPlayer_Me();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll();
	local tbl = {}
	received:CpyIdToLua(tbl, true);
	for _, v in ipairs(tbl) do
		if self:DoneById(v) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:CanDoneOne()
	local gpm = GPlayer_Me();
	local received = gpm:GetReceivedTaskMgr();
	received:RebuildAll();
	local tbl = {}
	received:CpyIdToLua(tbl, true);
	for _, v in ipairs(tbl) do
		if TaskItemHandler:CanDone(self:FindById(v)) then
			return true
		end
	end
	return false
end

function TaskItemHandlerMgr:new( o )
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function TaskItemHandlerMgr:TraverseInfo()
	local tbl = self:GetStoreTbl()
	for _, v in pairs(tbl) do
		TaskItemHandler:TraverseInfo(v);
	end
end
----------------------------------------------------------------------------------
--task handler-->TH
TH_Mgr = { the_tbl = {} }
TH_Mgr = TaskItemHandlerMgr:new(TH_Mgr);
function TH_Mgr:GetStoreTbl()
	return self.the_tbl;
end
function TH_Mgr:ClearAll()
	self.the_tbl = {};
end

function SafeSkipRun_AutoAcceptOne()
	return TH_Mgr:AutoAcceptOne();
end

function SafeSkipRun_AutoDoOne()
	return TH_Mgr:AutoDoOne();
end

function SafeSkipRun_AutoDoneOne()
	return TH_Mgr:AutoDoneOne();
end

function HasUnHandledAtTaskHandles()
	return TH_Mgr:HasAcceptableOne() or TH_Mgr:HasCanDoOne() or TH_Mgr:CanDoneOne()
end
----------------------------------------------------------------------------------
